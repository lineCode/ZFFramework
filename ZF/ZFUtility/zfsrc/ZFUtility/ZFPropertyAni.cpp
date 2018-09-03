/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFPropertyAni.h"

#include "ZFCore/ZFSTLWrapper/zfstl_vector.h"
#include "ZFCore/ZFSTLWrapper/zfstl_map.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFPropertyAniSetting)
ZFOBJECT_SINGLETON_DEFINE_DETAIL(ZFPropertyAniSetting, ZFPropertyAniSetting, ZFPropertyAniSetting, instance, ZFLevelZFFrameworkHigh)

ZFOBSERVER_EVENT_REGISTER(ZFPropertyAniSetting, PropertyAniOnStop)

// ============================================================
zfclassFwd _ZFP_I_ZFPropertyAniTaskData;
zfclassPOD _ZFP_ZFPropertyAniData
{
public:
    _ZFP_I_ZFPropertyAniTaskData *taskData;
    void *from;
    void *to;
};
typedef zfstlmap<const ZFProperty *, _ZFP_ZFPropertyAniData> _ZFP_ZFPropertyAniPropertyMapType;
typedef zfstlmap<ZFObject *, _ZFP_ZFPropertyAniPropertyMapType> _ZFP_ZFPropertyAniObjectMapType;

zfclass _ZFP_I_ZFPropertyAniTaskData : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_I_ZFPropertyAniTaskData, ZFObject)

public:
    zfbool running;
    zftimet startTime;
    ZFPropertyAniSetting *setting;
    ZFTimer *updateTimer;
    _ZFP_ZFPropertyAniObjectMapType targets;
protected:
    virtual void objectOnInit(void)
    {
        zfsuper::objectOnInit();
        this->running = zffalse;
        this->setting = zfAlloc(ZFPropertyAniSetting);
        this->updateTimer = zfAlloc(ZFTimer);
    }
    virtual void objectOnDealloc(void)
    {
        this->updateTimer->timerStop();
        zfRelease(this->setting);
        zfRelease(this->updateTimer);
        zfsuper::objectOnDealloc();
    }
};

static zfindex _ZFP_ZFPropertyAniOverrideFlag = 0;
static zfbool _ZFP_ZFPropertyAniProcessFlag = zffalse;
#define _ZFP_ZFPropertyAniStateAssert() \
    zfCoreAssertWithMessageTrim(!_ZFP_ZFPropertyAniProcessFlag, \
        zfTextA("[ZFPropertyAni] re-enter during clean is not allowed"))

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFPropertyAniDataHolder, ZFLevelZFFrameworkNormal)
{
    this->timerTaskListener = ZFCallbackForFunc(zfself::timerTask);
    this->objectPropertyOnUpdateListener = ZFCallbackForFunc(zfself::objectPropertyOnUpdate);
    this->objectOnDeallocListener = ZFCallbackForFunc(zfself::objectOnDealloc);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFPropertyAniDataHolder)
{
    this->removeAll();
}
public:
    _ZFP_ZFPropertyAniObjectMapType targets;
    ZFCoreArrayPOD<_ZFP_I_ZFPropertyAniTaskData *> tasks;
    ZFCoreArrayPOD<_ZFP_I_ZFPropertyAniTaskData *> tasksRunning;
    ZFListener timerTaskListener;
    ZFListener objectPropertyOnUpdateListener;
    ZFListener objectOnDeallocListener;
public:
    void removeAll(void)
    {
        _ZFP_ZFPropertyAniStateAssert();
        _ZFP_ZFPropertyAniProcessFlag = zftrue;
        ++_ZFP_ZFPropertyAniOverrideFlag;

        for(zfindex i = 0; i < this->tasks.count(); ++i)
        {
            _ZFP_I_ZFPropertyAniTaskData *taskData = this->tasks[i];
            taskData->setting->observerNotify(ZFPropertyAniSetting::EventPropertyAniOnStop());
            zfRelease(taskData);
        }
        this->tasks.removeAll();

        for(zfindex i = 0; i < this->tasksRunning.count(); ++i)
        {
            _ZFP_I_ZFPropertyAniTaskData *taskData = this->tasksRunning[i];
            taskData->updateTimer->timerStop();
            taskData->setting->observerNotify(ZFPropertyAniSetting::EventPropertyAniOnStop());
            zfRelease(taskData);
        }
        this->tasksRunning.removeAll();

        for(_ZFP_ZFPropertyAniObjectMapType::iterator itObj = this->targets.begin(); itObj != this->targets.end(); ++itObj)
        {
            itObj->first->observerRemove(
                ZFObject::EventObjectBeforeDealloc(),
                this->objectOnDeallocListener);
            for(_ZFP_ZFPropertyAniPropertyMapType::iterator itProp = itObj->second.begin(); itProp != itObj->second.end(); ++itProp)
            {
                const ZFProperty *property = itProp->first;
                property->callbackValueSet(property, itObj->first, itProp->second.to);
                property->callbackValueRelease(property, itObj->first, itProp->second.from);
                property->callbackValueRelease(property, itObj->first, itProp->second.to);
            }
        }
        this->targets.clear();

        --_ZFP_ZFPropertyAniOverrideFlag;
        _ZFP_ZFPropertyAniProcessFlag = zffalse;
    }
public:
    static ZFLISTENER_PROTOTYPE_EXPAND(timerTask)
    {
        _ZFP_ZFPropertyAniStateAssert();
        _ZFP_ZFPropertyAniProcessFlag = zftrue;
        _ZFP_I_ZFPropertyAniTaskData *taskData = userData->objectHolded();
        zftimet curTime = ZFTime::timestamp();
        zffloat progress = (zffloat)(curTime - taskData->startTime) / taskData->setting->aniDurationFixed();
        zfbool finished = zffalse;
        if(progress < 0 || progress > 1)
        {
            finished = zftrue;
        }
        else
        {
            if(taskData->setting->aniCurve() != zfnull)
            {
                progress = taskData->setting->aniCurve()->progressUpdate(progress);
            }
        }

        if(finished)
        {
            taskData->updateTimer->timerStop();
            _timerTaskUpdateFinished(taskData);
            ZF_GLOBAL_INITIALIZER_CLASS(ZFPropertyAniDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFPropertyAniDataHolder);
            d->tasksRunning.removeElement(taskData);
            _ZFP_ZFPropertyAniProcessFlag = zffalse;
            taskData->setting->observerNotify(ZFPropertyAniSetting::EventPropertyAniOnStop());
            zfRelease(taskData);
        }
        else
        {
            _timerTaskUpdate(taskData, progress);
            _ZFP_ZFPropertyAniProcessFlag = zffalse;
        }
    }
    static void _timerTaskUpdateFinished(ZF_IN _ZFP_I_ZFPropertyAniTaskData *taskData)
    {
        ++_ZFP_ZFPropertyAniOverrideFlag;
        ZF_GLOBAL_INITIALIZER_CLASS(ZFPropertyAniDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFPropertyAniDataHolder);
        for(_ZFP_ZFPropertyAniObjectMapType::iterator itObj = taskData->targets.begin(); itObj != taskData->targets.end(); ++itObj)
        {
            _ZFP_ZFPropertyAniObjectMapType::iterator itObjGlobal = d->targets.find(itObj->first);
            for(_ZFP_ZFPropertyAniPropertyMapType::iterator itProp = itObj->second.begin(); itProp != itObj->second.end(); ++itProp)
            {
                const ZFProperty *property = itProp->first;
                property->callbackValueSet(property, itObj->first, itProp->second.to);
                property->callbackValueRelease(property, itObj->first, itProp->second.from);
                property->callbackValueRelease(property, itObj->first, itProp->second.to);
                itObjGlobal->second.erase(property);
            }
            if(itObjGlobal->second.empty())
            {
                itObjGlobal->first->observerRemove(
                    ZFObject::EventObjectBeforeDealloc(),
                    d->objectOnDeallocListener);
                d->targets.erase(itObjGlobal);
            }
        }
        taskData->targets.clear();
        --_ZFP_ZFPropertyAniOverrideFlag;
    }
    static void _timerTaskUpdate(ZF_IN _ZFP_I_ZFPropertyAniTaskData *taskData, ZF_IN zffloat progress)
    {
        ++_ZFP_ZFPropertyAniOverrideFlag;
        ZF_GLOBAL_INITIALIZER_CLASS(ZFPropertyAniDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFPropertyAniDataHolder);
        for(_ZFP_ZFPropertyAniObjectMapType::iterator itObj = taskData->targets.begin(); itObj != taskData->targets.end(); )
        {
            _ZFP_ZFPropertyAniObjectMapType::iterator itObjGlobal = d->targets.find(itObj->first);
            for(_ZFP_ZFPropertyAniPropertyMapType::iterator itProp = itObj->second.begin(); itProp != itObj->second.end(); )
            {
                const ZFProperty *property = itProp->first;
                if(!property->callbackProgressUpdate(
                        property, itObj->first, itProp->second.from, itProp->second.to, progress
                    ))
                {
                    property->callbackValueSet(property, itObj->first, itProp->second.to);
                    property->callbackValueRelease(property, itObj->first, itProp->second.from);
                    property->callbackValueRelease(property, itObj->first, itProp->second.to);
                    itObj->second.erase(itProp++);
                    itObjGlobal->second.erase(property);
                }
                else
                {
                    ++itProp;
                }
            }
            if(itObjGlobal->second.empty())
            {
                zfCoreAssert(itObj->second.empty());
                itObjGlobal->first->observerRemove(
                    ZFObject::EventObjectBeforeDealloc(),
                    d->objectOnDeallocListener);
                taskData->targets.erase(itObj++);
            }
            else
            {
                ++itObj;
            }
        }

        if(taskData->targets.empty())
        {
            d->tasksRunning.removeElement(taskData);
            taskData->updateTimer->timerStop();
            taskData->setting->observerNotify(ZFPropertyAniSetting::EventPropertyAniOnStop());
            zfRelease(taskData);
        }
        --_ZFP_ZFPropertyAniOverrideFlag;
    }
    static ZFLISTENER_PROTOTYPE_EXPAND(objectPropertyOnUpdate)
    {
        if(_ZFP_ZFPropertyAniOverrideFlag > 0)
        {
            return ;
        }
        const ZFProperty *property = listenerData.param0->to<v_ZFProperty *>()->zfv;
        if(property->propertyOwnerClass() == ZFPropertyAniSetting::ClassData())
        {
            return ;
        }
        const void *oldValue = listenerData.param1->to<v_VoidPointerConst *>()->zfv;
        if(oldValue == zfnull)
        {
            return ;
        }
        ZFObject *ownerObj = listenerData.sender;
        ZFPropertyCallbackValueGetHolder _valueGetHolder(property, ownerObj);
        const void *curValue = _valueGetHolder.value();
        if(property->callbackCompare(property, ownerObj, curValue, oldValue) == ZFCompareTheSame
            || !property->callbackProgressUpdate(property, ownerObj, zfnull, zfnull, 1))
        {
            return ;
        }

        ZF_GLOBAL_INITIALIZER_CLASS(ZFPropertyAniDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFPropertyAniDataHolder);

        _ZFP_ZFPropertyAniData data;
        data.taskData = d->tasks.getLast();
        data.from = property->callbackValueStore(property, ownerObj, oldValue);
        data.to = property->callbackValueStore(property, ownerObj, curValue);

        _ZFP_ZFPropertyAniObjectMapType::iterator itObjGlobal = d->targets.find(ownerObj);
        if(itObjGlobal != d->targets.end())
        {
            _ZFP_ZFPropertyAniPropertyMapType::iterator itPropGlobal = itObjGlobal->second.find(property);
            if(itPropGlobal != itObjGlobal->second.end())
            {
                data.taskData = itPropGlobal->second.taskData;

                itPropGlobal->first->callbackValueRelease(itPropGlobal->first, ownerObj, itPropGlobal->second.from);
                itPropGlobal->first->callbackValueRelease(itPropGlobal->first, ownerObj, itPropGlobal->second.to);
            }
        }
        else
        {
            ownerObj->observerAdd(
                ZFObject::EventObjectBeforeDealloc(),
                d->objectOnDeallocListener);
        }

        d->targets[ownerObj][property] = data;
        data.taskData->targets[ownerObj][property] = data;

        // restore old value
        ++_ZFP_ZFPropertyAniOverrideFlag;
        property->callbackValueSet(property, ownerObj, data.from);
        --_ZFP_ZFPropertyAniOverrideFlag;
    }
    static ZFLISTENER_PROTOTYPE_EXPAND(objectOnDealloc)
    {
        ZF_GLOBAL_INITIALIZER_CLASS(ZFPropertyAniDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFPropertyAniDataHolder);
        ZFObject *ownerObj = listenerData.sender;
        _ZFP_ZFPropertyAniObjectMapType::iterator itObjGlobal = d->targets.find(ownerObj);
        if(itObjGlobal == d->targets.end())
        {
            return ;
        }
        _ZFP_ZFPropertyAniStateAssert();
        _ZFP_ZFPropertyAniProcessFlag = zftrue;

        for(_ZFP_ZFPropertyAniPropertyMapType::iterator itPropGlobal = itObjGlobal->second.begin();
            itPropGlobal != itObjGlobal->second.end(); )
        {
            itPropGlobal->first->callbackValueRelease(itPropGlobal->first, ownerObj, itPropGlobal->second.from);
            itPropGlobal->first->callbackValueRelease(itPropGlobal->first, ownerObj, itPropGlobal->second.to);

            _ZFP_I_ZFPropertyAniTaskData *taskData = itPropGlobal->second.taskData;
            _ZFP_ZFPropertyAniObjectMapType::iterator itObj = taskData->targets.find(ownerObj);
            itObj->second.erase(itPropGlobal->first);
            if(itObj->second.empty())
            {
                taskData->targets.erase(itObj);
                if(taskData->targets.empty() && taskData->running)
                {
                    taskData->updateTimer->timerStop();
                    d->tasksRunning.removeElement(taskData);
                    taskData->setting->observerNotify(ZFPropertyAniSetting::EventPropertyAniOnStop());
                    zfRelease(taskData);
                }
            }

            itObjGlobal->second.erase(itPropGlobal++);
        }
        d->targets.erase(itObjGlobal);

        _ZFP_ZFPropertyAniProcessFlag = zffalse;
    }
ZF_GLOBAL_INITIALIZER_END(ZFPropertyAniDataHolder)

// ============================================================
ZFMETHOD_FUNC_DEFINE_0(ZFPropertyAniSetting *, ZFPropertyAniBegin)
{
    _ZFP_ZFPropertyAniStateAssert();
    zfCoreMutexLocker();
    ++_ZFP_ZFPropertyAniOverrideFlag;
    ZF_GLOBAL_INITIALIZER_CLASS(ZFPropertyAniDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFPropertyAniDataHolder);
    _ZFP_I_ZFPropertyAniTaskData *taskData = zfAlloc(_ZFP_I_ZFPropertyAniTaskData);
    d->tasks.add(taskData);

    if(d->tasks.count() == 1)
    {
        ZFObjectGlobalEventObserver().observerAdd(
            ZFObject::EventObjectPropertyValueOnUpdate(),
            d->objectPropertyOnUpdateListener);
    }

    taskData->setting->aniDurationSet(ZFPropertyAniSetting::instance()->aniDuration());
    taskData->setting->aniIntervalSet(ZFPropertyAniSetting::instance()->aniInterval());
    taskData->setting->aniCurveSet(ZFPropertyAniSetting::instance()->aniCurve());

    --_ZFP_ZFPropertyAniOverrideFlag;
    return taskData->setting;
}

ZFMETHOD_FUNC_DEFINE_0(zfbool, ZFPropertyAniEnd)
{
    _ZFP_ZFPropertyAniStateAssert();
    zfCoreMutexLocker();
    ++_ZFP_ZFPropertyAniOverrideFlag;
    ZF_GLOBAL_INITIALIZER_CLASS(ZFPropertyAniDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFPropertyAniDataHolder);
    zfCoreAssert(!d->tasks.isEmpty());

    zfbool ret = zffalse;
    _ZFP_I_ZFPropertyAniTaskData *taskData = d->tasks.removeLastAndGet();
    if(!taskData->targets.empty())
    {
        taskData->running = zftrue;
        d->tasksRunning.add(taskData);
        taskData->startTime = ZFTime::timestamp();
        taskData->updateTimer->timerIntervalSet(taskData->setting->aniInterval());
        taskData->updateTimer->observerAdd(
            ZFTimer::EventTimerOnActivate(),
            d->timerTaskListener,
            taskData->objectHolder());
        taskData->updateTimer->timerStart();
        ret = zftrue;
    }

    if(d->tasks.count() == 0)
    {
        ZFObjectGlobalEventObserver().observerRemove(
            ZFObject::EventObjectPropertyValueOnUpdate(),
            d->objectPropertyOnUpdateListener);
    }
    --_ZFP_ZFPropertyAniOverrideFlag;

    if(!ret)
    {
        taskData->setting->observerNotify(ZFPropertyAniSetting::EventPropertyAniOnStop());
        zfRelease(taskData);
    }
    return ret;
}

ZFMETHOD_FUNC_DEFINE_0(ZFPropertyAniSetting *, ZFPropertyAniState)
{
    ZF_GLOBAL_INITIALIZER_CLASS(ZFPropertyAniDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFPropertyAniDataHolder);
    if(d->tasks.isEmpty())
    {
        return zfnull;
    }
    else
    {
        return d->tasks.getLast()->setting;
    }
}

// ============================================================
static const ZFProperty *_ZFP_ZFPropertyAniPrepare(ZF_IN ZFObject *obj,
                                                   ZF_IN const zfchar *propertyName)
{
    if(obj == zfnull)
    {
        return zfnull;
    }
    const ZFProperty *property = obj->classData()->propertyForName(propertyName);
    if(property == zfnull
        || property->setterMethod()->methodPrivilegeType() != ZFMethodPrivilegeTypePublic
        || !property->callbackProgressUpdate(property, obj, zfnull, zfnull, 0))
    {
        return zfnull;
    }
    return property;
}
ZFMETHOD_FUNC_DEFINE_8(void, ZFPropertyAni,
                       ZFMP_IN(ZFObject *, obj),
                       ZFMP_IN(const zfchar *, propertyName),
                       ZFMP_IN(const zfchar *, fromValueString),
                       ZFMP_IN(const zfchar *, toValueString),
                       ZFMP_IN_OPT(zftimet, aniDuration, 0),
                       ZFMP_IN_OPT(ZFTimeLineCurve *, aniCurve, zfnull),
                       ZFMP_IN_OPT(const ZFListener &, finishListener, ZFCallbackNull()),
                       ZFMP_IN_OPT(ZFObject *, userData, zfnull))
{
    const ZFProperty *property = _ZFP_ZFPropertyAniPrepare(obj, propertyName);
    if(property == zfnull)
    {
        return ;
    }
    zfautoObject fromValue;
    zfautoObject toValue;
    if(property->propertyIsRetainProperty())
    {
        if(!ZFSerializeFromString(fromValue, property->propertyClassOfRetainProperty(), fromValueString)
            || !ZFSerializeFromString(toValue, property->propertyClassOfRetainProperty(), toValueString))
        {
            return ;
        }
        property->callbackValueSet(property, obj, &fromValue);
    }
    else
    {
        const ZFTypeIdBase *typeId = ZFTypeIdGet(property->propertyTypeId());
        if(typeId == zfnull)
        {
            return ;
        }
        if(!typeId->typeIdWrapper(fromValue)
            || !typeId->typeIdWrapper(toValue))
        {
            return ;
        }
        ZFTypeIdWrapper *fromValueWrapper = fromValue;
        ZFTypeIdWrapper *toValueWrapper = toValue;
        if(fromValueWrapper == zfnull || toValueWrapper == zfnull)
        {
            return ;
        }
        if(!fromValueWrapper->wrappedValueFromString(fromValueString)
            || !toValueWrapper->wrappedValueFromString(toValueString))
        {
            return ;
        }
        property->callbackValueSet(property, obj, fromValueWrapper->wrappedValue());
    }

    ZFPropertyAniBlock(setting);
    setting->aniDurationSet(aniDuration);
    setting->aniCurveSet(aniCurve);
    if(finishListener.callbackIsValid())
    {
        setting->observerAdd(ZFObserverAddParam()
                .eventIdSet(ZFPropertyAniSetting::EventPropertyAniOnStop())
                .observerSet(finishListener)
                .userDataSet(userData)
                .autoRemoveAfterActivateSet(zftrue)
            );
    }

    if(property->propertyIsRetainProperty())
    {
        property->callbackValueSet(property, obj, &toValue);
    }
    else
    {
        ZFTypeIdWrapper *toValueWrapper = toValue;
        property->callbackValueSet(property, obj, toValueWrapper->wrappedValue());
    }
}
ZFMETHOD_FUNC_DEFINE_8(void, ZFPropertyAni,
                       ZFMP_IN(ZFObject *, obj),
                       ZFMP_IN(const zfchar *, propertyName),
                       ZFMP_IN(ZFObject *, fromValue),
                       ZFMP_IN(ZFObject *, toValue),
                       ZFMP_IN_OPT(zftimet, aniDuration, 0),
                       ZFMP_IN_OPT(ZFTimeLineCurve *, aniCurve, zfnull),
                       ZFMP_IN_OPT(const ZFListener &, finishListener, ZFCallbackNull()),
                       ZFMP_IN_OPT(ZFObject *, userData, zfnull))
{
    const ZFProperty *property = _ZFP_ZFPropertyAniPrepare(obj, propertyName);
    if(property == zfnull)
    {
        return ;
    }
    if(property->propertyIsRetainProperty())
    {
        if(fromValue != zfnull && !fromValue->classData()->classIsTypeOf(property->propertyClassOfRetainProperty()))
        {
            return ;
        }
        if(toValue != zfnull && !toValue->classData()->classIsTypeOf(property->propertyClassOfRetainProperty()))
        {
            return ;
        }
        zfautoObject fromValueHolder = fromValue;
        property->callbackValueSet(property, obj, &fromValueHolder);
    }
    else
    {
        const ZFTypeIdBase *typeId = ZFTypeIdGet(property->propertyTypeId());
        if(typeId == zfnull)
        {
            return ;
        }
        ZFTypeIdWrapper *fromValueWrapper = ZFCastZFObject(ZFTypeIdWrapper *, fromValue);
        ZFTypeIdWrapper *toValueWrapper = ZFCastZFObject(ZFTypeIdWrapper *, toValue);
        if(fromValueWrapper == zfnull || toValueWrapper == zfnull
            || !zfscmpTheSame(fromValueWrapper->wrappedValueTypeId(), typeId->typeId())
            || !zfscmpTheSame(toValueWrapper->wrappedValueTypeId(), typeId->typeId())
            )
        {
            return ;
        }
        property->callbackValueSet(property, obj, fromValueWrapper->wrappedValue());
    }

    ZFPropertyAniBlock(setting);
    setting->aniDurationSet(aniDuration);
    setting->aniCurveSet(aniCurve);
    if(finishListener.callbackIsValid())
    {
        setting->observerAdd(ZFObserverAddParam()
                .eventIdSet(ZFPropertyAniSetting::EventPropertyAniOnStop())
                .observerSet(finishListener)
                .userDataSet(userData)
                .autoRemoveAfterActivateSet(zftrue)
            );
    }

    if(property->propertyIsRetainProperty())
    {
        zfautoObject toValueHolder = toValue;
        property->callbackValueSet(property, obj, &toValueHolder);
    }
    else
    {
        ZFTypeIdWrapper *toValueWrapper = ZFCastZFObject(ZFTypeIdWrapper *, toValue);
        property->callbackValueSet(property, obj, toValueWrapper->wrappedValue());
    }
}

ZF_NAMESPACE_GLOBAL_END

