/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFObjectSmartPointer.h"
#include "ZFObjectImpl.h"
#include "zfsynchronize.h"

#include "ZFCore/ZFSTLWrapper/zfstl_string.h"
#include "ZFCore/ZFSTLWrapper/zfstl_map.h"
#include "ZFCore/ZFSTLWrapper/zfstl_vector.h"

ZF_NAMESPACE_GLOBAL_BEGIN

static ZFCoreQueuePOD<ZFObjectHolder *> _ZFP_ZFObjectCache_objectHolder;
static ZFCoreQueuePOD<v_ZFProperty *> _ZFP_ZFObjectCache_prop0;
static ZFCoreQueuePOD<v_VoidPointerConst *> _ZFP_ZFObjectCache_prop1;
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFObjectCacheAutoClean, ZFLevelZFFrameworkStatic)
{
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFObjectCacheAutoClean)
{
    while(!_ZFP_ZFObjectCache_objectHolder.isEmpty())
    {
        zfRelease(_ZFP_ZFObjectCache_objectHolder.queueTake());
    }
    while(!_ZFP_ZFObjectCache_prop0.isEmpty())
    {
        zfRelease(_ZFP_ZFObjectCache_prop0.queueTake());
    }
    while(!_ZFP_ZFObjectCache_prop1.isEmpty())
    {
        zfRelease(_ZFP_ZFObjectCache_prop1.queueTake());
    }
}
ZF_GLOBAL_INITIALIZER_END(ZFObjectCacheAutoClean)

// ============================================================
// _ZFP_ZFObjectPrivate
typedef zfstlmap<zfstlstringZ, zfautoObject> _ZFP_ZFObjectTagMapType;
zfclassNotPOD _ZFP_ZFObjectPrivate
{
public:
    zfuint objectRetainCount;
    ZFObjectInstanceState objectInstanceState;
    ZFObjectHolder *objectHolder;
    void *mutexImpl;
    _ZFP_ZFObjectTagMapType tagMap;
    zfstlvector<const ZFProperty *> propertyAccessed;
    enum {
        stateFlag_objectIsPrivate = 1 << 0,
        stateFlag_objectIsInternal = 1 << 1,
        stateFlag_observerHasAddFlag_objectAfterAlloc = 1 << 2,
        stateFlag_observerHasAddFlag_objectBeforeDealloc = 1 << 3,
        stateFlag_observerHasAddFlag_objectPropertyValueOnUpdate = 1 << 4,
    };
    zfuint stateFlags;

public:
    _ZFP_ZFObjectPrivate(ZF_IN const ZFClass *cls)
    : objectRetainCount(1)
    , objectInstanceState(ZFObjectInstanceStateOnInit)
    , objectHolder(zfnull)
    , mutexImpl(zfnull)
    , tagMap()
    , propertyAccessed()
    , stateFlags(0)
    {
        if(cls->classIsPrivate())
        {
            ZFBitSet(this->stateFlags, _ZFP_ZFObjectPrivate::stateFlag_objectIsPrivate);
        }
        if(cls->classIsInternal())
        {
            ZFBitSet(this->stateFlags, _ZFP_ZFObjectPrivate::stateFlag_objectIsInternal);
        }
    }
};

// ============================================================
// ZFObject
ZFOBJECT_REGISTER(ZFObject)

ZFOBSERVER_EVENT_REGISTER(ZFObject, ObjectAfterAlloc)
ZFOBSERVER_EVENT_REGISTER(ZFObject, ObjectBeforeDealloc)
ZFOBSERVER_EVENT_REGISTER(ZFObject, ObjectPropertyValueOnUpdate)

static zfuint _ZFP_ZFObject_stateFlags = 0;
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFObject_stateFlags, ZFLevelZFFrameworkStatic)
{
    ZFObjectGlobalEventObserver().observerHasAddStateAttach(ZFObject::EventObjectAfterAlloc(), &_ZFP_ZFObject_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectAfterAlloc);
    ZFObjectGlobalEventObserver().observerHasAddStateAttach(ZFObject::EventObjectBeforeDealloc(), &_ZFP_ZFObject_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectBeforeDealloc);
    ZFObjectGlobalEventObserver().observerHasAddStateAttach(ZFObject::EventObjectPropertyValueOnUpdate(), &_ZFP_ZFObject_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectPropertyValueOnUpdate);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFObject_stateFlags)
{
    ZFObjectGlobalEventObserver().observerHasAddStateDetach(ZFObject::EventObjectAfterAlloc(), &_ZFP_ZFObject_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectAfterAlloc);
    ZFObjectGlobalEventObserver().observerHasAddStateDetach(ZFObject::EventObjectBeforeDealloc(), &_ZFP_ZFObject_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectBeforeDealloc);
    ZFObjectGlobalEventObserver().observerHasAddStateDetach(ZFObject::EventObjectPropertyValueOnUpdate(), &_ZFP_ZFObject_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectPropertyValueOnUpdate);
}
ZF_GLOBAL_INITIALIZER_END(ZFObject_stateFlags)

zfindex ZFObject::objectRetainCount(void)
{
    return (zfindex)d->objectRetainCount;
}

ZFObjectHolder *ZFObject::objectHolder(void)
{
    zfCoreMutexLocker();
    if(d->objectHolder == zfnull)
    {
        if(_ZFP_ZFObjectCache_objectHolder.isEmpty())
        {
            d->objectHolder = zfAlloc(ZFObjectHolder);
        }
        else
        {
            d->objectHolder = _ZFP_ZFObjectCache_objectHolder.queueTake();
        }
        d->objectHolder->objectHoldedSet(this);
    }
    return d->objectHolder;
}
ZFAny ZFObject::objectHolded(void)
{
    return this->to<ZFObjectHolder *>()->_ZFP_objectHolded;
}

void ZFObject::objectInfoOfInstanceT(ZF_IN_OUT zfstring &ret)
{
    ret += this->classData()->classNameFull();
    ret += "(";
    zfsFromPointerT(ret, this);
    ret += ")";
}
void ZFObject::objectInfoT(ZF_IN_OUT zfstring &ret)
{
    this->objectInfoOnAppendTokenLeft(ret);
    this->objectInfoOnAppend(ret);
    this->objectInfoOnAppendTokenRight(ret);
}
void ZFObject::objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
{
    this->objectInfoOfInstanceT(ret);
}
zfidentity ZFObject::objectHash(void)
{
    return zfidentityCalcPointer(this);
}
ZFCompareResult ZFObject::objectCompare(ZF_IN ZFObject *anotherObj)
{
    return ((this == anotherObj) ? ZFCompareTheSame : ZFCompareUncomparable);
}

/* ZFMETHOD_MAX_PARAM */
zfautoObject ZFObject::invoke(ZF_IN const zfchar *methodName
                              , ZF_IN_OPT ZFObject *param0 /* = ZFMethodGenericInvokerDefaultParam() */
                              , ZF_IN_OPT ZFObject *param1 /* = ZFMethodGenericInvokerDefaultParam() */
                              , ZF_IN_OPT ZFObject *param2 /* = ZFMethodGenericInvokerDefaultParam() */
                              , ZF_IN_OPT ZFObject *param3 /* = ZFMethodGenericInvokerDefaultParam() */
                              , ZF_IN_OPT ZFObject *param4 /* = ZFMethodGenericInvokerDefaultParam() */
                              , ZF_IN_OPT ZFObject *param5 /* = ZFMethodGenericInvokerDefaultParam() */
                              , ZF_IN_OPT ZFObject *param6 /* = ZFMethodGenericInvokerDefaultParam() */
                              , ZF_IN_OPT ZFObject *param7 /* = ZFMethodGenericInvokerDefaultParam() */
                              , ZF_OUT_OPT zfbool *success /* = zfnull */
                              , ZF_OUT_OPT zfstring *errorHint /* = zfnull */
                              )
{
    ZFCoreArrayPOD<const ZFMethod *> methodList = this->classData()->methodForNameGetAll(methodName);
    zfstring errorHintTmp;
    zfautoObject ret;
    const ZFMethod *m = zfnull;
    for(zfindex i = 0; i < methodList.count(); ++i)
    {
        m = methodList[i];
        zfautoObject paramList[ZFMETHOD_MAX_PARAM] = {
            param0,
            param1,
            param2,
            param3,
            param4,
            param5,
            param6,
            param7,
        };
        if(m->methodGenericInvoker()(m, this, &errorHintTmp, ret, paramList))
        {
            if(success != zfnull)
            {
                *success = zftrue;
            }
            return ret;
        }
    }
    if(success != zfnull)
    {
        *success = zffalse;
    }
    if(methodList.isEmpty())
    {
        zfstringAppend(errorHint,
            "no such method: %s",
            methodName);
    }
    else
    {
        zfstringAppend(errorHint,
            "no matching method to call, last error reason: %s, for method: %s",
            methodName,
            errorHintTmp.cString(),
            m ? m->objectInfo().cString() : ZFTOKEN_zfnull);
    }
    return zfautoObjectNull();
}

zfbool ZFObject::tagHasSet(void)
{
    return !(d->tagMap.empty());
}
void ZFObject::tagSet(ZF_IN const zfchar *key,
                      ZF_IN ZFObject *tag)
{
    zfCoreMutexLocker();
    if(ZFBitTest(d->objectInstanceState, ZFObjectInstanceStateOnDealloc) && tag != zfnull)
    {
        zfCoreCriticalMessageTrim("[ZFObject] you must not set tag while object is deallocating, class: %s, tag: %s",
            this->classData()->classNameFull(),
            key);
        return ;
    }
    if(key == zfnull)
    {
        return ;
    }

    _ZFP_ZFObjectTagMapType &m = d->tagMap;
    _ZFP_ZFObjectTagMapType::iterator it = m.find(key);
    if(it == m.end())
    {
        if(tag != zfnull)
        {
            m[key] = tag;
        }
    }
    else
    {
        zfautoObject holder = it->second;
        if(tag == zfnull)
        {
            m.erase(it);
        }
        else
        {
            it->second = tag;
        }
    }
}
ZFObject *ZFObject::tagGet(ZF_IN const zfchar *key)
{
    if(key == zfnull)
    {
        return zfnull;
    }
    zfCoreMutexLocker();
    _ZFP_ZFObjectTagMapType::iterator it = d->tagMap.find(key);
    if(it != d->tagMap.end())
    {
        return it->second.toObject();
    }
    else
    {
        return zfnull;
    }
}
void ZFObject::tagGetAllKeyValue(ZF_IN_OUT ZFCoreArray<const zfchar *> &allKey,
                                 ZF_IN_OUT ZFCoreArray<ZFObject *> &allValue)
{
    zfCoreMutexLocker();
    _ZFP_ZFObjectTagMapType &m = d->tagMap;
    allKey.capacitySet(allKey.count() + m.size());
    allValue.capacitySet(allValue.count() + m.size());
    for(_ZFP_ZFObjectTagMapType::iterator it = m.begin(); it != m.end(); ++it)
    {
        allKey.add(it->first.c_str());
        allValue.add(it->second.toObject());
    }
}
zfautoObject ZFObject::tagRemoveAndGet(ZF_IN const zfchar *key)
{
    if(key != zfnull)
    {
        zfCoreMutexLocker();
        _ZFP_ZFObjectTagMapType::iterator it = d->tagMap.find(key);
        if(it != d->tagMap.end())
        {
            zfautoObject ret = it->second;
            d->tagMap.erase(it);
            return ret;
        }
    }
    return zfnull;
}
void ZFObject::tagRemoveAll(void)
{
    zfCoreMutexLocker();
    if(!d->tagMap.empty())
    {
        _ZFP_ZFObjectTagMapType tmp;
        tmp.swap(d->tagMap);
    }
}


void ZFObject::observerOnAdd(ZF_IN zfidentity eventId)
{
    if(zffalse)
    {
    }
    else if(eventId == ZFObject::EventObjectAfterAlloc())
    {
        ZFBitSet(d->stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectAfterAlloc);
    }
    else if(eventId == ZFObject::EventObjectBeforeDealloc())
    {
        ZFBitSet(d->stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectBeforeDealloc);
    }
    else if(eventId == ZFObject::EventObjectPropertyValueOnUpdate())
    {
        ZFBitSet(d->stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectPropertyValueOnUpdate);
    }
}
void ZFObject::observerOnRemove(ZF_IN zfidentity eventId)
{
    if(zffalse)
    {
    }
    else if(eventId == ZFObject::EventObjectAfterAlloc())
    {
        ZFBitUnset(d->stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectAfterAlloc);
    }
    else if(eventId == ZFObject::EventObjectBeforeDealloc())
    {
        ZFBitUnset(d->stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectBeforeDealloc);
    }
    else if(eventId == ZFObject::EventObjectPropertyValueOnUpdate())
    {
        ZFBitUnset(d->stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectPropertyValueOnUpdate);
    }
}

void ZFObject::_ZFP_ZFObjectLock(void)
{
    if(d->mutexImpl)
    {
        _ZFP_ZFObjectMutexImplLock(d->mutexImpl);
    }
    else
    {
        zfCoreMutexLock();
        if(_ZFP_ZFObjectMutexImplInit)
        {
            if(d->mutexImpl == zfnull)
            {
                d->mutexImpl = _ZFP_ZFObjectMutexImplInit();
            }
        }
        zfCoreMutexUnlock();
        if(d->mutexImpl)
        {
            _ZFP_ZFObjectMutexImplLock(d->mutexImpl);
        }
    }
}
void ZFObject::_ZFP_ZFObjectUnlock(void)
{
    if(d->mutexImpl)
    {
        _ZFP_ZFObjectMutexImplUnlock(d->mutexImpl);
    }
}
zfbool ZFObject::_ZFP_ZFObjectTryLock(void)
{
    if(d->mutexImpl)
    {
        return _ZFP_ZFObjectMutexImplTryLock(d->mutexImpl);
    }
    else
    {
        zfCoreMutexLock();
        if(_ZFP_ZFObjectMutexImplInit)
        {
            if(d->mutexImpl == zfnull)
            {
                d->mutexImpl = _ZFP_ZFObjectMutexImplInit();
            }
        }
        zfCoreMutexUnlock();
        if(d->mutexImpl)
        {
            return _ZFP_ZFObjectMutexImplTryLock(d->mutexImpl);
        }
        else
        {
            return zffalse;
        }
    }
}

ZFObject *ZFObject::_ZFP_ZFObjectCheckOnInit(void)
{
    if(d == zfnull)
    {
        zfCoreCriticalMessageTrim("[ZFObject] ZFObject::objectOnInit() not called");
        return zfnull;
    }

    d->objectInstanceState = ZFObjectInstanceStateOnInitFinish;
    this->classData()->_ZFP_ZFClass_propertyAutoInitAction(this);
    this->_ZFP_ObjI_onInitIvk();
    this->objectOnInitFinish();
    d->objectInstanceState = ZFObjectInstanceStateIdle;

    if(!this->objectIsInternal())
    {
        this->classData()->_ZFP_ZFClass_instanceObserverNotify(this);
        if(ZFBitTest(d->stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectAfterAlloc)
            || ZFBitTest(_ZFP_ZFObject_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectAfterAlloc))
        {
            this->observerNotify(ZFObject::EventObjectAfterAlloc());
        }
    }

    return this;
}
void ZFObject::_ZFP_ZFObjectCheckRelease(void)
{
    if(ZFBitTest(d->stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectBeforeDealloc)
        || ZFBitTest(_ZFP_ZFObject_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectBeforeDealloc))
    {
        if(d->objectRetainCount == 1)
        {
            this->observerNotify(ZFObject::EventObjectBeforeDealloc());
            this->tagRemoveAll();
            if(d->objectRetainCount > 1)
            {
                this->objectOnRelease();
                this->observerRemoveAll(ZFObject::EventObjectBeforeDealloc());
                return ;
            }
        }
    }

    this->objectOnRelease();
    if(d->objectRetainCount > 0) {return ;}

    d->objectInstanceState = ZFObjectInstanceStateOnDeallocPrepare;
    this->objectOnDeallocPrepare();
    this->_ZFP_ObjI_onDeallocIvk();
    for(zfstlsize i = d->propertyAccessed.size() - 1; i != (zfstlsize)-1; --i)
    {
        const ZFProperty *property = d->propertyAccessed[i];
        property->_ZFP_ZFProperty_callbackDealloc(property, this);
    }
    d->objectInstanceState = ZFObjectInstanceStateOnDealloc;
    this->objectOnDealloc();
    if(d != zfnull)
    {
        zfCoreCriticalMessageTrim("[ZFObject] ZFObject::objectOnDealloc() not called");
        return ;
    }
    this->classData()->_ZFP_ZFClass_objectDesctuct(this);
}

void ZFObject::objectOnInit(void)
{
    if(d != zfnull)
    {
        zfCoreCriticalMessageTrim("[ZFObject] ZFObject::objectOnInit() called twice");
        return ;
    }

    // note that (d != zfnull) is also used to check whether ZFObject::objectOnInit() is called
    d = zfpoolNew(_ZFP_ZFObjectPrivate, this->classData());
    this->observerHolder()._ZFP_ZFObserverHolder_observerOwnerSet(this);
}
void ZFObject::objectOnDealloc(void)
{
    if(d == zfnull)
    {
        zfCoreCriticalMessageTrim("[ZFObject] ZFObject::objectOnDealloc() called twice");
        return ;
    }

    if(d->mutexImpl)
    {
        _ZFP_ZFObjectMutexImplDealloc(d->mutexImpl);
        d->mutexImpl = zfnull;
    }

    if(d->objectHolder)
    {
        d->objectHolder->objectHoldedSet(zfnull);
        if(d->objectHolder->objectRetainCount() == 1
            && _ZFP_ZFObjectCache_objectHolder.count() < 32)
        {
            _ZFP_ZFObjectCache_objectHolder.queuePut(d->objectHolder);
        }
        else
        {
            zfRelease(d->objectHolder);
        }
    }

    zfpoolDelete(d);
    // note that (d == zfnull) is also used to check whether ZFObject::objectOnDealloc() is called
    d = zfnull;
}
void ZFObject::objectOnInitFinish(void)
{
}
void ZFObject::objectOnDeallocPrepare(void)
{
    this->tagRemoveAll();
    this->observerRemoveAll();
}
void ZFObject::objectOnRetain(void)
{
    zfCoreAssertWithMessageTrim(d->objectRetainCount > 0,
        "[ZFObject] retain an object while deallocating: %s", this->objectInfoOfInstance().cString());
    ++(d->objectRetainCount);
}
void ZFObject::objectOnRelease(void)
{
    --(d->objectRetainCount);
}

ZFObjectInstanceState ZFObject::objectInstanceState(void)
{
    return d->objectInstanceState;
}

zfbool ZFObject::objectIsPrivate(void)
{
    return ZFBitTest(d->stateFlags, _ZFP_ZFObjectPrivate::stateFlag_objectIsPrivate)
        || ZFBitTest(d->stateFlags, _ZFP_ZFObjectPrivate::stateFlag_objectIsInternal);
}
zfbool ZFObject::objectIsInternal(void)
{
    return ZFBitTest(d->stateFlags, _ZFP_ZFObjectPrivate::stateFlag_objectIsInternal);
}

void ZFObject::_ZFP_ZFObject_objectPropertyValueAttach(ZF_IN const ZFProperty *property)
{
    d->propertyAccessed.push_back(property);
}
void ZFObject::_ZFP_ZFObject_objectPropertyValueDetach(ZF_IN const ZFProperty *property)
{
    for(zfstlsize i = d->propertyAccessed.size() - 1; i != (zfstlsize)-1; --i)
    {
        if(d->propertyAccessed[i] == property)
        {
            d->propertyAccessed.erase(d->propertyAccessed.begin() + i);
        }
    }
}

void ZFObject::objectPropertyValueOnUpdate(ZF_IN const ZFProperty *property, ZF_IN const void *oldValue)
{
    if(!this->objectIsInternal()
        && (ZFBitTest(d->stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectPropertyValueOnUpdate)
            || ZFBitTest(_ZFP_ZFObject_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectPropertyValueOnUpdate)))
    {
        if(_ZFP_ZFObjectCache_prop0.isEmpty())
        {
            v_ZFProperty *param0 = zfAlloc(v_ZFProperty, property);
            v_VoidPointerConst *param1 = zfAlloc(v_VoidPointerConst, oldValue);
            this->observerNotify(ZFObject::EventObjectPropertyValueOnUpdate(), param0, param1);
            _ZFP_ZFObjectCache_prop0.queuePut(param0);
            _ZFP_ZFObjectCache_prop1.queuePut(param1);
        }
        else
        {
            v_ZFProperty *param0 = _ZFP_ZFObjectCache_prop0.queueTake();
            param0->zfv = property;
            v_VoidPointerConst *param1 = _ZFP_ZFObjectCache_prop1.queueTake();
            param1->zfv = oldValue;
            this->observerNotify(ZFObject::EventObjectPropertyValueOnUpdate(), param0, param1);
            if(_ZFP_ZFObjectCache_prop0.count() < 8)
            {
                _ZFP_ZFObjectCache_prop0.queuePut(param0);
                _ZFP_ZFObjectCache_prop1.queuePut(param1);
            }
            else
            {
                zfRelease(param0);
                zfRelease(param1);
            }
        }
    }
}

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFEXPORT_ENUM_DEFINE(ZFObjectInstanceState, ZFObjectInstanceStateOnInit, ZFObjectInstanceStateOnInitFinish, ZFObjectInstanceStateIdle, ZFObjectInstanceStateOnDeallocPrepare, ZFObjectInstanceStateOnDealloc)

ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFObject, const ZFClass *, classData)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFObject, ZFObjectHolder *, objectHolder)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFObject, ZFAny, objectHolded)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFObject, void, objectInfoOfInstanceT, ZFMP_IN_OUT(zfstring &, ret))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFObject, zfstring, objectInfoOfInstance)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFObject, void, objectInfoT, ZFMP_IN_OUT(zfstring &, ret))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFObject, zfstring, objectInfo)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFObject, zfidentity, objectHash)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFObject, ZFCompareResult, objectCompare, ZFMP_IN(ZFObject *, anotherObj))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_8(ZFObject, zfautoObject, invoke
        , ZFMP_IN(const zfchar *, methodName)
        , ZFMP_IN_OPT(ZFObject *, param0, ZFMethodGenericInvokerDefaultParam())
        , ZFMP_IN_OPT(ZFObject *, param1, ZFMethodGenericInvokerDefaultParam())
        , ZFMP_IN_OPT(ZFObject *, param2, ZFMethodGenericInvokerDefaultParam())
        , ZFMP_IN_OPT(ZFObject *, param3, ZFMethodGenericInvokerDefaultParam())
        , ZFMP_IN_OPT(ZFObject *, param4, ZFMethodGenericInvokerDefaultParam())
        , ZFMP_IN_OPT(ZFObject *, param5, ZFMethodGenericInvokerDefaultParam())
        , ZFMP_IN_OPT(ZFObject *, param6, ZFMethodGenericInvokerDefaultParam())
        /* ZFMETHOD_MAX_PARAM , ZFMP_IN_OPT(ZFObject *, param7, ZFMethodGenericInvokerDefaultParam()) */
        /* ZFMETHOD_MAX_PARAM , ZFMP_OUT_OPT(zfbool *, success, zfnull) */
        /* ZFMETHOD_MAX_PARAM , ZFMP_OUT_OPT(zfstring *, errorHint, zfnull) */
    )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFObject, zfbool, tagHasSet)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_2(ZFObject, void, tagSet, ZFMP_IN(const zfchar *, key), ZFMP_IN(ZFObject *, tag))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFObject, ZFObject *, tagGet, ZFMP_IN(const zfchar *, key))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_2(ZFObject, void, tagGetAllKeyValue, ZFMP_IN_OUT(ZFCoreArray<const zfchar *> &, allKey), ZFMP_IN_OUT(ZFCoreArray<ZFObject *> &, allValue))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFObject, void, tagRemove, ZFMP_IN(const zfchar *, key))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFObject, zfautoObject, tagRemoveAndGet, ZFMP_IN(const zfchar *, key))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFObject, void, tagRemoveAll)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFObject, const ZFObserverHolder &, observerHolder)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_6(ZFObject, zfidentity, observerAdd, ZFMP_IN(zfidentity, eventId), ZFMP_IN(const ZFListener &, observer), ZFMP_IN_OPT(ZFObject *, userData, zfnull), ZFMP_IN_OPT(ZFObject *, owner, zfnull), ZFMP_IN_OPT(zfbool, autoRemoveAfterActivate, zffalse), ZFMP_IN_OPT(ZFLevel, observerLevel, ZFLevelAppNormal))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFObject, zfidentity, observerAdd, ZFMP_IN(const ZFObserverAddParam &, param))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFObject, void, observerMoveToFirst, ZFMP_IN(zfidentity, taskId))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_2(ZFObject, void, observerRemove, ZFMP_IN(zfidentity, eventId), ZFMP_IN(const ZFListener &, callback))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_4(ZFObject, void, observerRemove, ZFMP_IN(zfidentity, eventId), ZFMP_IN(const ZFListener &, callback), ZFMP_IN(ZFObject *, userData), ZFMP_IN_OPT(ZFComparer<ZFObject *>::Comparer, userDataComparer, ZFComparerCheckEqual))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFObject, void, observerRemoveByTaskId, ZFMP_IN(zfidentity, taskId))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFObject, void, observerRemoveByOwner, ZFMP_IN(ZFObject *, owner))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFObject, void, observerRemoveAll, ZFMP_IN(zfidentity, eventId))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFObject, void, observerRemoveAll)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFObject, zfbool, observerHasAdd)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFObject, zfbool, observerHasAdd, ZFMP_IN(zfidentity, eventId))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_3(ZFObject, void, observerNotify, ZFMP_IN(zfidentity, eventId), ZFMP_IN_OPT(ZFObject *, param0, zfnull), ZFMP_IN_OPT(ZFObject *, param1, zfnull))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_4(ZFObject, void, observerNotifyWithCustomSender, ZFMP_IN(ZFObject *, customSender), ZFMP_IN(zfidentity, eventId), ZFMP_IN_OPT(ZFObject *, param0, zfnull), ZFMP_IN_OPT(ZFObject *, param1, zfnull))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFObject, ZFObjectInstanceState, objectInstanceState)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFObject, zfbool, objectIsPrivate)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFObject, zfbool, objectIsInternal)

ZF_NAMESPACE_GLOBAL_END
#endif

