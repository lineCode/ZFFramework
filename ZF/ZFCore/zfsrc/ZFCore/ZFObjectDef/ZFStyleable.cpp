/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFStyleable.h"
#include "ZFObjectImpl.h"
#include "ZFClassUtil.h"
#include "ZFPropertyUtil.h"
#include "ZFListenerDeclare.h"

#include "../ZFSTLWrapper/zfstl_string.h"
#include "../ZFSTLWrapper/zfstl_map.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFStyleable *ZFStyleable::defaultStyle(void)
{
    const ZFMethod *method = this->classData()->methodForName(zfText("DefaultStyleReflect"));
    if(method != zfnull)
    {
        return method->execute<zfautoObject>(zfnull).to<ZFStyleable *>();
    }
    else
    {
        return zfnull;
    }
}

// ============================================================
zfclass _ZFP_I_ZFStyleable_PropertyTypeHolder : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_I_ZFStyleable_PropertyTypeHolder, ZFObject)

public:
    ZFCoreArrayPOD<const ZFProperty *> normalProperty;
    ZFCoreArrayPOD<const ZFProperty *> styleableProperty;
    ZFCoreArrayPOD<const ZFProperty *> copyableProperty;
};

// ============================================================
void ZFStyleable::styleableCopyFrom(ZF_IN ZFStyleable *anotherStyleable)
{
    if(anotherStyleable == zfnull)
    {
        return ;
    }
    if(this->styleKey() == zfnull)
    {
        this->styleKeySet(anotherStyleable->styleKey());
    }

    _ZFP_I_ZFStyleable_PropertyTypeHolder *holderTmp = this->_ZFP_ZFStyleable_getPropertyTypeHolder();
    const ZFClass *thisCls = this->classData();
    const ZFClass *anotherCls = anotherStyleable->classData();
    ZFObject *anotherStyleableObject = anotherStyleable->toObject();;
    const ZFProperty *property = zfnull;

    for(zfindex i = holderTmp->normalProperty.count() - 1; i != zfindexMax(); --i)
    {
        property = holderTmp->normalProperty[i];
        if(!anotherCls->classIsTypeOf(property->propertyOwnerClass()))
        {
            continue;
        }
        if(!thisCls->_ZFP_ZFClass_propertyInitStepIsTheSame(property, anotherCls)
            || property->callbackIsValueAccessed(property, anotherStyleableObject))
        {
            this->styleableOnCopyPropertyFrom(anotherStyleable, property, ZFStyleable::PropertyTypeNormal);
        }
    }
    for(zfindex i = holderTmp->styleableProperty.count() - 1; i != zfindexMax(); --i)
    {
        property = holderTmp->styleableProperty[i];
        if(!anotherCls->classIsTypeOf(property->propertyOwnerClass()))
        {
            continue;
        }
        if(!thisCls->_ZFP_ZFClass_propertyInitStepIsTheSame(property, anotherCls)
            || property->callbackIsValueAccessed(property, anotherStyleableObject))
        {
            this->styleableOnCopyPropertyFrom(anotherStyleable, property, ZFStyleable::PropertyTypeStyleable);
        }
    }
    for(zfindex i = holderTmp->copyableProperty.count() - 1; i != zfindexMax(); --i)
    {
        property = holderTmp->copyableProperty[i];
        if(!anotherCls->classIsTypeOf(property->propertyOwnerClass()))
        {
            continue;
        }
        if(!thisCls->_ZFP_ZFClass_propertyInitStepIsTheSame(property, anotherCls)
            || property->callbackIsValueAccessed(property, anotherStyleableObject))
        {
            this->styleableOnCopyPropertyFrom(anotherStyleable, property, ZFStyleable::PropertyTypeCopyable);
        }
    }

    this->styleableOnCopyFrom(anotherStyleable);
}
ZFStyleable::PropertyType ZFStyleable::styleableOnCheckPropertyType(ZF_IN const ZFProperty *property)
{
    if(property->propertyIsRetainProperty()
        && property->setterMethod()->methodPrivilegeType() == ZFMethodPrivilegeTypePrivate
        && property->getterMethod()->methodPrivilegeType() != ZFMethodPrivilegeTypePrivate)
    {
        if(property->propertyClassOfRetainProperty()->classIsTypeOf(ZFStyleable::ClassData()))
        {
            return ZFStyleable::PropertyTypeStyleable;
        }
        else if(property->propertyClassOfRetainProperty()->classIsTypeOf(ZFCopyable::ClassData()))
        {
            return ZFStyleable::PropertyTypeCopyable;
        }
    }
    if(property->setterMethod()->methodPrivilegeType() == ZFMethodPrivilegeTypePrivate
        || property->getterMethod()->methodPrivilegeType() == ZFMethodPrivilegeTypePrivate)
    {
        return ZFStyleable::PropertyTypeNotStyleable;
    }
    return ZFStyleable::PropertyTypeNormal;
}

void ZFStyleable::styleableOnCopyPropertyFrom(ZF_IN ZFStyleable *anotherStyleable,
                                              ZF_IN const ZFProperty *property,
                                              ZF_IN ZFStyleable::PropertyType propertyType)
{
    if(this->styleKey(property) == zfnull)
    {
        this->styleKeySet(property, anotherStyleable->styleKey(property));
    }
    switch(propertyType)
    {
        case ZFStyleable::PropertyTypeNormal:
        {
            ZFPropertyCopy(property, this->toObject(), anotherStyleable->toObject());
        }
            break;
        case ZFStyleable::PropertyTypeStyleable:
        {
            ZFPropertyCallbackValueGetHolder _valueGetHolder(property, this->toObject());
            ZFPropertyCallbackValueGetHolder _valueGetHolderRef(property, anotherStyleable->toObject());
            ZFStyleable *selfPropertyValue = *(const zfautoObject *)_valueGetHolder.value();
            ZFStyleable *anotherPropertyValue = *(const zfautoObject *)_valueGetHolderRef.value();
            if(selfPropertyValue != zfnull && anotherPropertyValue != zfnull)
            {
                selfPropertyValue->styleableCopyFrom(anotherPropertyValue);
            }
        }
            break;
        case ZFStyleable::PropertyTypeCopyable:
        {
            ZFPropertyCallbackValueGetHolder _valueGetHolder(property, this->toObject());
            ZFPropertyCallbackValueGetHolder _valueGetHolderRef(property, anotherStyleable->toObject());
            ZFCopyable *selfPropertyValue = *(const zfautoObject *)_valueGetHolder.value();
            ZFObject *anotherPropertyValue = *(const zfautoObject *)_valueGetHolderRef.value();
            if(selfPropertyValue != zfnull && anotherPropertyValue != zfnull)
            {
                selfPropertyValue->copyFrom(anotherPropertyValue);
            }
        }
            break;
        case ZFStyleable::PropertyTypeNotStyleable:
        default:
            zfCoreCriticalShouldNotGoHere();
            return ;
    }
}

_ZFP_I_ZFStyleable_PropertyTypeHolder *ZFStyleable::_ZFP_ZFStyleable_getPropertyTypeHolder(void)
{
    zfCoreMutexLocker();
    _ZFP_I_ZFStyleable_PropertyTypeHolder *holder = this->classData()->classTagGet<_ZFP_I_ZFStyleable_PropertyTypeHolder *>(
        _ZFP_I_ZFStyleable_PropertyTypeHolder::ClassData()->classNameFull());
    if(holder == zfnull)
    {
        zflockfree_zfblockedAlloc(_ZFP_I_ZFStyleable_PropertyTypeHolder, holderTmp);

        const ZFCoreArrayPOD<const ZFProperty *> allProperty = ZFClassUtil::allProperty(this->classData());
        const ZFProperty *propertyTmp = zfnull;
        for(zfindex i = 0; i < allProperty.count(); ++i)
        {
            propertyTmp = allProperty[i];
            switch(this->styleableOnCheckPropertyType(propertyTmp))
            {
                case ZFStyleable::PropertyTypeNotStyleable:
                    break;
                case ZFStyleable::PropertyTypeNormal:
                    holderTmp->normalProperty.add(propertyTmp);
                    break;
                case ZFStyleable::PropertyTypeStyleable:
                    holderTmp->styleableProperty.add(propertyTmp);
                    break;
                case ZFStyleable::PropertyTypeCopyable:
                    holderTmp->copyableProperty.add(propertyTmp);
                    break;
            }
        }

        holder = holderTmp;
        this->classData()->classTagSet(
            _ZFP_I_ZFStyleable_PropertyTypeHolder::ClassData()->classNameFull(),
            holderTmp);
        this->ClassData()->classDataChangeAutoRemoveTagAdd(_ZFP_I_ZFStyleable_PropertyTypeHolder::ClassData()->classNameFull());
    }
    return holder;
}

// ============================================================
ZFOBJECT_REGISTER(ZFStyleableObject)

// ============================================================
ZF_STATIC_INITIALIZER_INIT(ZFStyleableDefaultStyleDataHolder)
{
}
ZFCoreMap instanceDataMap; // _ZFP_ZFStyleableDefaultPointerHolder *
ZF_STATIC_INITIALIZER_END(ZFStyleableDefaultStyleDataHolder)

_ZFP_ZFStyleableDefaultPointerHolder *_ZFP_ZFStyleableDefaultRefAccess(ZF_IN const zfchar *name)
{
    zfCoreMutexLocker();
    ZFCoreMap &m = ZF_STATIC_INITIALIZER_INSTANCE(ZFStyleableDefaultStyleDataHolder)->instanceDataMap;
    ZFCorePointerBase *v = m.get(name);
    if(v == zfnull)
    {
        m.set(name, ZFCorePointerForObject<_ZFP_ZFStyleableDefaultPointerHolder *>(zfnew(_ZFP_ZFStyleableDefaultPointerHolder)));
        v = m.get(name);
    }
    return v->pointerValueT<_ZFP_ZFStyleableDefaultPointerHolder *>();
}

// ============================================================
zfclass _ZFP_ZFStyleDefaultApplyAutoCopyTaskData : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_ZFStyleDefaultApplyAutoCopyTaskData, ZFObject)

public:
    ZFCoreArrayPOD<ZFObjectHolder *> styles;
};
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFStyleDefaultApplyAutoCopyDataHolder, ZFLevelZFFrameworkEssential)
{
    this->styleOnDeallocListener = ZFCallbackForFunc(zfself::styleOnDealloc);
    this->defaultStyleOnChangeListener = ZFCallbackForFunc(zfself::defaultStyleOnChange);
}
public:
    ZFListener styleOnDeallocListener;
    ZFListener defaultStyleOnChangeListener;
public:
    static ZFLISTENER_PROTOTYPE_EXPAND(styleOnDealloc)
    {
        zfCoreMutexLocker();

        ZFStyleable *defaultStyle = listenerData.sender->to<ZFStyleable *>()->defaultStyle();
        zfCoreAssert(defaultStyle != zfnull);
        _ZFP_ZFStyleDefaultApplyAutoCopyTaskData *taskData = defaultStyle->toObject()
            ->tagGet<_ZFP_ZFStyleDefaultApplyAutoCopyTaskData *>(_ZFP_ZFStyleDefaultApplyAutoCopyTaskData::ClassData()->classNameFull());
        zfCoreAssert(taskData != zfnull);

        taskData->styles.removeElement(listenerData.sender->objectHolder());

        if(taskData->styles.isEmpty())
        {
            defaultStyle->toObject()->tagRemove(_ZFP_ZFStyleDefaultApplyAutoCopyTaskData::ClassData()->classNameFull());
            defaultStyle->toObject()->observerRemove(ZFObject::EventObjectPropertyValueOnUpdate(),
                ZF_GLOBAL_INITIALIZER_INSTANCE(ZFStyleDefaultApplyAutoCopyDataHolder)->defaultStyleOnChangeListener);
        }
    }
    static ZFLISTENER_PROTOTYPE_EXPAND(defaultStyleOnChange)
    {
        zfCoreMutexLocker();

        const ZFProperty *property = listenerData.param0->to<ZFPointerHolder *>()->holdedDataPointer<const ZFProperty *>();
        ZFStyleable *defaultStyle = listenerData.sender->to<ZFStyleable *>();
        _ZFP_ZFStyleDefaultApplyAutoCopyTaskData *taskData = defaultStyle->toObject()
            ->tagGet<_ZFP_ZFStyleDefaultApplyAutoCopyTaskData *>(_ZFP_ZFStyleDefaultApplyAutoCopyTaskData::ClassData()->classNameFull());

        ZFCoreArrayPOD<ZFObjectHolder *> styles;
        styles.copyFrom(taskData->styles);
        for(zfindex i = 0; i < styles.count(); ++i)
        {
            ZFPropertyCopy(property, styles[i]->objectHolded(), listenerData.sender);
        }
    }
ZF_GLOBAL_INITIALIZER_END(ZFStyleDefaultApplyAutoCopyDataHolder)

void ZFStyleDefaultApplyAutoCopy(ZF_IN ZFStyleable *style)
{
    zfCoreMutexLocker();
    if(style != zfnull && !style->styleableIsDefaultStyle())
    {
        ZFStyleable *defaultStyle = style->defaultStyle();
        if(defaultStyle != zfnull)
        {
            _ZFP_ZFStyleDefaultApplyAutoCopyTaskData *taskData = defaultStyle->toObject()
                ->tagGet<_ZFP_ZFStyleDefaultApplyAutoCopyTaskData *>(_ZFP_ZFStyleDefaultApplyAutoCopyTaskData::ClassData()->classNameFull());
            if(taskData == zfnull)
            {
                taskData = zfAlloc(_ZFP_ZFStyleDefaultApplyAutoCopyTaskData);
                defaultStyle->toObject()->tagSet(_ZFP_ZFStyleDefaultApplyAutoCopyTaskData::ClassData()->classNameFull(), taskData);
                zfRelease(taskData);

                defaultStyle->toObject()->observerAdd(ZFObject::EventObjectPropertyValueOnUpdate(),
                    ZF_GLOBAL_INITIALIZER_INSTANCE(ZFStyleDefaultApplyAutoCopyDataHolder)->defaultStyleOnChangeListener);
            }
            taskData->styles.add(style->toObject()->objectHolder());
            style->toObject()->observerAdd(ZFObject::EventObjectBeforeDealloc(),
                ZF_GLOBAL_INITIALIZER_INSTANCE(ZFStyleDefaultApplyAutoCopyDataHolder)->styleOnDeallocListener);
        }
    }
}

// ============================================================
// style holder
static zfstlmap<zfstlstringZ, zfautoObject> &_ZFP_ZFStyleHolder(void)
{
    static zfstlmap<zfstlstringZ, zfautoObject> d;
    return d;
}
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFStyleCleanup, ZFLevelZFFrameworkNormal)
{
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFStyleCleanup)
{
    _ZFP_ZFStyleHolder().clear();
}
ZF_GLOBAL_INITIALIZER_END(ZFStyleCleanup)

static zfstlmap<zfstlstringZ, _ZFP_ZFStyleDecoder> &_ZFP_ZFStyleDecoderMap(void)
{
    static zfstlmap<zfstlstringZ, _ZFP_ZFStyleDecoder> d;
    return d;
}
void _ZFP_ZFStyleDecoderRegister(ZF_IN const zfchar *registerSig,
                                 ZF_IN _ZFP_ZFStyleDecoder decoder)
{
    zfCoreMutexLocker();
    zfstlmap<zfstlstringZ, _ZFP_ZFStyleDecoder> &m = _ZFP_ZFStyleDecoderMap();
    zfCoreAssert(registerSig != zfnull && decoder != zfnull);
    zfCoreAssertWithMessageTrim(m.find(registerSig) == m.end(),
        zfTextA("[ZFSTYLE_DECODER_DEFINE] %s already registered"),
        registerSig);
    m[registerSig] = decoder;
}
void _ZFP_ZFStyleDecoderUnregister(ZF_IN const zfchar *registerSig)
{
    zfCoreMutexLocker();
    zfstlmap<zfstlstringZ, _ZFP_ZFStyleDecoder> &m = _ZFP_ZFStyleDecoderMap();
    m.erase(registerSig);
}

void ZFStyleSet(ZF_IN const zfchar *styleKey, ZF_IN ZFStyleable *styleValue)
{
    if(styleValue)
    {
        zfCoreMutexLock();
        _ZFP_ZFStyleHolder()[styleKey] = styleValue;
        zfCoreMutexUnlock();
    }
}
zfautoObject ZFStyleGet(ZF_IN const zfchar *styleKey)
{
    if(zfsIsEmpty(styleKey))
    {
        return zfnull;
    }

    zfCoreMutexLocker();
    zfautoObject ret;
    zfstlmap<zfstlstringZ, _ZFP_ZFStyleDecoder> &m = _ZFP_ZFStyleDecoderMap();
    for(zfstlmap<zfstlstringZ, _ZFP_ZFStyleDecoder>::iterator it = m.begin(); it != m.end(); ++it)
    {
        if(it->second(ret, styleKey))
        {
            return ret;
        }
    }

    zfstlmap<zfstlstringZ, zfautoObject> &d = _ZFP_ZFStyleHolder();
    zfstlmap<zfstlstringZ, zfautoObject>::iterator it = d.find(styleKey);
    if(it != d.end())
    {
        return it->second;
    }
    else
    {
        return zfnull;
    }
}
void ZFStyleGetAll(ZF_IN_OUT ZFCoreArrayPOD<const zfchar *> &styleKey,
                   ZF_IN_OUT ZFCoreArrayPOD<ZFStyleable *> &styleValue)
{
    zfCoreMutexLocker();
    zfstlmap<zfstlstringZ, zfautoObject> &d = _ZFP_ZFStyleHolder();
    for(zfstlmap<zfstlstringZ, zfautoObject>::iterator it = d.begin(); it != d.end(); ++it)
    {
        styleKey.add(it->first.c_str());
        styleValue.add(it->second);
    }
}
void ZFStyleRemoveAll(void)
{
    zfCoreMutexLock();
    zfstlmap<zfstlstringZ, zfautoObject> d;
    d.swap(_ZFP_ZFStyleHolder());
    zfCoreMutexUnlock();
}

static zfint _ZFP_ZFStyleChangeBeginFlag = 0;
void ZFStyleChangeBegin()
{
    zfCoreMutexLock();
    ++_ZFP_ZFStyleChangeBeginFlag;
}
void ZFStyleChangeEnd()
{
    zfCoreAssertWithMessageTrim(_ZFP_ZFStyleChangeBeginFlag != 0,
        zfTextA("ZFStyleChangeBegin/ZFStyleChangeEnd not paired"));
    --_ZFP_ZFStyleChangeBeginFlag;
    zfbool needNotify = (_ZFP_ZFStyleChangeBeginFlag == 0);
    zfCoreMutexUnlock();
    if(needNotify)
    {
        ZFObjectGlobalEventObserver().observerNotify(ZFGlobalEvent::EventZFStyleOnChange());
    }
}

ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
ZFOBSERVER_EVENT_GLOBAL_REGISTER(ZFStyleOnChange)
ZFOBSERVER_EVENT_GLOBAL_REGISTER(ZFStyleOnInvalid)
ZF_NAMESPACE_END(ZFGlobalEvent)

static zfbool _ZFP_ZFStyleInvalidCheckDisableFlag = zffalse;
void ZFStyleInvalidCheckDisable(ZF_IN zfbool enable)
{
    _ZFP_ZFStyleInvalidCheckDisableFlag = zftrue;
}
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFStyleInvalidAssert, ZFLevelZFFrameworkHigh)
{
    ZFLISTENER_LOCAL(action, {
            if(_ZFP_ZFStyleInvalidCheckDisableFlag) {return ;}
            const ZFProperty *property = listenerData.param0->to<ZFPointerHolder *>()->holdedDataPointer<const ZFProperty *>();
            const zfchar *styleKey = listenerData.param1->to<ZFPointerHolder *>()->holdedDataPointer<const zfchar *>();
            if(property == zfnull)
            {
                zfCoreCriticalMessageTrim(
                    zfTextA("[ZFStyle] %s unable to apply style \"%s\""),
                    zfsCoreZ2A(listenerData.sender->objectInfoOfInstance().cString()),
                    zfsCoreZ2A(styleKey));
            }
            else
            {
                zfCoreCriticalMessageTrim(
                    zfTextA("[ZFStyle] %s unable to apply style \"%s\" for property %s"),
                    zfsCoreZ2A(listenerData.sender->objectInfoOfInstance().cString()),
                    zfsCoreZ2A(styleKey),
                    zfsCoreZ2A(property->objectInfo().cString()));
            }
        })
    this->taskId = ZFObjectGlobalEventObserver().observerAdd(
        ZFGlobalEvent::EventZFStyleOnInvalid(),
        action);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFStyleInvalidAssert)
{
    _ZFP_ZFStyleInvalidCheckDisableFlag = zffalse;
    ZFObjectGlobalEventObserver().observerRemoveByTaskId(this->taskId);
}
zfidentity taskId;
ZF_GLOBAL_INITIALIZER_END(ZFStyleInvalidAssert)

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFStyleable, ZFStyleable *, defaultStyle)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFStyleable, void, styleableCopyFrom, ZFMP_IN(ZFStyleable *, anotherStyleable))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFStyleable, zfbool, styleableIsDefaultStyle)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFStyleable, zfbool, styleKeySet, ZFMP_IN(const zfchar *, styleKey))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFStyleable, const zfchar *, styleKey)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_2(ZFStyleable, zfbool, styleKeySet, ZFMP_IN(const ZFProperty *, property), ZFMP_IN(const zfchar *, styleKey))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFStyleable, const zfchar *, styleKey, ZFMP_IN(const ZFProperty *, property))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_2(ZFStyleable, zfbool, styleKeySet, ZFMP_IN(const zfchar *, propertyName), ZFMP_IN(const zfchar *, styleKey))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFStyleable, const zfchar *, styleKey, ZFMP_IN(const zfchar *, propertyName))

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(void, ZFStyleDefaultApplyAutoCopy, ZFMP_IN(ZFStyleable *, styleValue))

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(void, ZFStyleSet, ZFMP_IN(const zfchar *, styleKey), ZFMP_IN(ZFStyleable *, styleValue))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(zfautoObject, ZFStyleGet, ZFMP_IN(const zfchar *, styleKey))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(void, ZFStyleGetAll, ZFMP_IN_OUT(ZFCoreArrayPOD<const zfchar *> &, styleKey), ZFMP_IN_OUT(ZFCoreArrayPOD<ZFStyleable *>, styleValue))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_0(void, ZFStyleRemoveAll)
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_0(void, ZFStyleChangeBegin)
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_0(void, ZFStyleChangeEnd)

ZF_NAMESPACE_GLOBAL_END
#endif

