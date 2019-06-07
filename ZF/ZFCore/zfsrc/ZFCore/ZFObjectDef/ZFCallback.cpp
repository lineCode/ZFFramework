/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFCallback.h"
#include "ZFObjectImpl.h"

#include "ZFCore/ZFSTLWrapper/zfstl_string.h"
#include "ZFCore/ZFSTLWrapper/zfstl_map.h"
#include "ZFCore/ZFSTLWrapper/zfstl_vector.h"

ZF_NAMESPACE_GLOBAL_BEGIN

void _ZFP_ZFCallback_executeNullCallback(void)
{
    zfCoreCriticalMessageTrim("try execute a null callback");
}

// ============================================================
// _ZFP_ZFCallbackPrivate
typedef zfstlmap<zfstlstringZ, zfautoObject> _ZFP_ZFCallbackTagMap;
zfclassNotPOD ZF_ENV_EXPORT _ZFP_ZFCallbackPrivate
{
public:
    zfuint refCount;
    zfchar *callbackId;
    _ZFP_ZFCallbackTagMap callbackTagMap;
    ZFCallbackType callbackType;
    ZFObject *callbackOwnerObj; // assign only
    zfuint callbackOwnerObjectRetainFlag;
    const ZFMethod *callbackMethod;
    ZFFuncAddrType callbackRawFunc;
    zfchar *serializableCustomType;
    ZFSerializableData *serializableCustomData;
    ZFPathInfo *pathInfo;

public:
    _ZFP_ZFCallbackPrivate(void)
    : refCount(1)
    , callbackId(zfnull)
    , callbackTagMap()
    , callbackType(ZFCallbackTypeDummy)
    , callbackOwnerObj(zfnull)
    , callbackOwnerObjectRetainFlag(0)
    , callbackMethod(zfnull)
    , callbackRawFunc(zfnull)
    , serializableCustomType(zfnull)
    , serializableCustomData(zfnull)
    , pathInfo(zfnull)
    {
    }
    ~_ZFP_ZFCallbackPrivate(void)
    {
        zffree(this->callbackId);
        if(this->callbackOwnerObjectRetainFlag != 0)
        {
            zfRelease(this->callbackOwnerObj);
        }
        zffree(this->serializableCustomType);
        zfdelete(this->serializableCustomData);
        zfdelete(this->pathInfo);
    }
};

// ============================================================
// global
static void _ZFP_ZFCallbackPrivateDataChange(_ZFP_ZFCallbackPrivate *&oldData, _ZFP_ZFCallbackPrivate *newData)
{
    _ZFP_ZFCallbackPrivate *dTmp = oldData;
    oldData = newData;
    if(newData != zfnull)
    {
        ++newData->refCount;
    }
    if(dTmp != zfnull)
    {
        --dTmp->refCount;
        if(dTmp->refCount == 0)
        {
            zfdelete(dTmp);
        }
    }
}

// ============================================================
// ZFCallback
ZFCallback::ZFCallback(void)
: d(zfnull)
{
}
ZFCallback::ZFCallback(ZF_IN const ZFCallback &ref)
: d(zfnull)
{
    this->operator = (ref);
}
ZFCallback &ZFCallback::operator = (ZF_IN const ZFCallback &ref)
{
    _ZFP_ZFCallbackPrivateDataChange(d, ref.d);
    return *this;
}
ZFCallback::~ZFCallback(void)
{
    if(d != zfnull)
    {
        --d->refCount;
        if(d->refCount == 0)
        {
            zfdelete(d);
        }
    }
}
ZFCallback ZFCallback::_ZFP_ZFCallbackCreate(ZF_IN ZFCallbackType callbackType,
                                             ZF_IN ZFObject *callbackOwnerObj,
                                             ZF_IN const ZFMethod *callbackMethod,
                                             ZF_IN ZFFuncAddrType callbackRawFunc)
{
    ZFCallback callback;
    callback.d = zfnew(_ZFP_ZFCallbackPrivate);
    switch(callbackType)
    {
        case ZFCallbackTypeDummy:
            break;
        case ZFCallbackTypeMethod:
            zfCoreAssertWithMessageTrim(callbackMethod != zfnull, "[ZFCallback] method is null");
            zfCoreAssertWithMessageTrim(callbackMethod->methodType() == ZFMethodTypeStatic,
                "[ZFCallback] method \"%s\" is not class static member type",
                callbackMethod->objectInfo().cString());
            callback.d->callbackType = ZFCallbackTypeMethod;
            callback.d->callbackMethod = callbackMethod;
            break;
        case ZFCallbackTypeMemberMethod:
            zfCoreAssertWithMessageTrim(callbackOwnerObj != zfnull && callbackMethod != zfnull,
                "[ZFCallback] invalid callback, ownerObj: %s, method: %s",
                zfsFromPointer(callbackOwnerObj).cString(),
                zfsFromPointer(callbackMethod).cString());
            zfCoreAssertWithMessageTrim(callbackMethod->methodType() != ZFMethodTypeStatic,
                "[ZFCallback] method \"%s\" is not class member type",
                callbackMethod->objectInfo().cString());
            zfCoreAssertWithMessageTrim(callbackOwnerObj->classData()->classIsTypeOf(callbackMethod->methodOwnerClass()),
                "[ZFCallback] object %s has no such method \"%s\"",
                callbackOwnerObj->objectInfoOfInstance().cString(),
                callbackMethod->objectInfo().cString());
            callback.d->callbackType = ZFCallbackTypeMemberMethod;
            callback.d->callbackOwnerObj = callbackOwnerObj;
            callback.d->callbackMethod = callbackMethod;
            break;
        case ZFCallbackTypeRawFunction:
            zfCoreAssertWithMessageTrim(callbackRawFunc != zfnull, "[ZFCallback] invalid function address");
            callback.d->callbackType = ZFCallbackTypeRawFunction;
            callback.d->callbackRawFunc = callbackRawFunc;
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            break;
    }
    return callback;
}

zfindex ZFCallback::objectRetainCount(void) const
{
    return (d ? d->refCount : 1);
}

void ZFCallback::objectInfoT(ZF_IN_OUT zfstring &ret) const
{
    switch(this->callbackType())
    {
        case ZFCallbackTypeDummy:
            ret += "ZFCallbackNull";
            break;
        case ZFCallbackTypeMethod:
        case ZFCallbackTypeMemberMethod:
            this->callbackMethod()->objectInfoT(ret);
            break;
        case ZFCallbackTypeRawFunction:
            ret += ZFTOKEN_ZFObjectInfoLeft;
            ret += "ZFCallback func: ";
            zfsFromPointerT(ret, ZFCastReinterpret(const void *, this->callbackFunctionAddr()));
            ret += ZFTOKEN_ZFObjectInfoRight;
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            break;
    }
    if(this->callbackOwnerObject() != zfnull)
    {
        ret += ", owner: ";
        this->callbackOwnerObject()->objectInfoT(ret);
    }
    if(d != zfnull && !d->callbackTagMap.empty())
    {
        ret += ", tags: ";
        _ZFP_ZFCallbackTagMap &m = d->callbackTagMap;
        for(_ZFP_ZFCallbackTagMap::iterator it = m.begin(); it != m.end(); ++it)
        {
            if(it != m.begin())
            {
                ret += ", ";
            }
            ret += "<";
            ret += it->first.c_str();
            ret += ", ";
            ZFObjectInfoT(ret, it->second);
            ret += ">";
        }
    }
}

ZFCompareResult ZFCallback::objectCompare(ZF_IN const ZFCallback &ref) const
{
    return ((d == ref.d || (
            this->callbackType() == ref.callbackType()
            && this->callbackOwnerObject() == ref.callbackOwnerObject()
            && this->callbackMethod() == ref.callbackMethod()
            && this->callbackFunctionAddr() == ref.callbackFunctionAddr()
            ))
        ? ZFCompareTheSame : ZFCompareUncomparable);
}

void ZFCallback::callbackIdSet(ZF_IN const zfchar *callbackId)
{
    if(d == zfnull)
    {
        d = zfnew(_ZFP_ZFCallbackPrivate);
    }
    if(callbackId != zfnull && *callbackId == '\0')
    {
        callbackId = zfnull;
    }
    zfsChange(d->callbackId, callbackId);
}
const zfchar *ZFCallback::callbackId(void) const
{
    return (d ? d->callbackId : zfnull);
}

void ZFCallback::callbackTagSet(ZF_IN const zfchar *key,
                                ZF_IN ZFObject *tag)
{
    if(key == zfnull)
    {
        return ;
    }
    if(d == zfnull)
    {
        d = zfnew(_ZFP_ZFCallbackPrivate);
    }

    _ZFP_ZFCallbackTagMap &m = d->callbackTagMap;
    _ZFP_ZFCallbackTagMap::iterator it = m.find(key);
    if(it == m.end())
    {
        if(tag != zfnull)
        {
            m[key] = tag;
        }
    }
    else
    {
        if(tag == zfnull)
        {
            zfautoObject holder = it->second;
            ZFUNUSED(holder);
            m.erase(it);
        }
        else
        {
            zfautoObject holder = it->second;
            ZFUNUSED(holder);
            it->second = tag;
        }
    }
}
ZFObject *ZFCallback::callbackTagGet(ZF_IN const zfchar *key) const
{
    if(d != zfnull && key != zfnull)
    {
        _ZFP_ZFCallbackTagMap &m = d->callbackTagMap;
        _ZFP_ZFCallbackTagMap::iterator it = m.find(key);
        if(it != m.end())
        {
            return it->second.toObject();
        }
    }
    return zfnull;
}
void ZFCallback::callbackTagGetAllKeyValue(ZF_IN_OUT ZFCoreArray<const zfchar *> &allKey,
                                           ZF_IN_OUT ZFCoreArray<ZFObject *> &allValue) const
{
    if(d != zfnull)
    {
        _ZFP_ZFCallbackTagMap &m = d->callbackTagMap;
        allKey.capacitySet(allKey.count() + m.size());
        allValue.capacitySet(allValue.count() + m.size());
        for(_ZFP_ZFCallbackTagMap::iterator it = m.begin(); it != m.end(); ++it)
        {
            allKey.add(it->first.c_str());
            allValue.add(it->second.toObject());
        }
    }
}
zfautoObject ZFCallback::callbackTagRemoveAndGet(ZF_IN const zfchar *key)
{
    if(d != zfnull && key != zfnull)
    {
        _ZFP_ZFCallbackTagMap &m = d->callbackTagMap;
        _ZFP_ZFCallbackTagMap::iterator it = m.find(key);
        if(it != m.end())
        {
            zfautoObject ret = it->second;
            m.erase(it);
            return ret;
        }
    }
    return zfnull;
}
void ZFCallback::callbackTagRemoveAll(void)
{
    if(d != zfnull && !d->callbackTagMap.empty())
    {
        _ZFP_ZFCallbackTagMap tmp;
        tmp.swap(d->callbackTagMap);
    }
}

ZFCallbackType ZFCallback::callbackType(void) const
{
    return (d ? d->callbackType : ZFCallbackTypeDummy);
}

ZFObject *ZFCallback::callbackOwnerObject(void) const
{
    return (d ? d->callbackOwnerObj : zfnull);
}

const ZFMethod *ZFCallback::callbackMethod(void) const
{
    return (d ? d->callbackMethod : zfnull);
}

ZFFuncAddrType ZFCallback::callbackFunctionAddr(void) const
{
    return (d ? d->callbackRawFunc : zfnull);
}

void ZFCallback::callbackClear(void)
{
    _ZFP_ZFCallbackPrivateDataChange(d, zfnull);
}

void ZFCallback::callbackOwnerObjectRetain(void) const
{
    if(d != zfnull && d->callbackOwnerObj != zfnull && !d->callbackOwnerObjectRetainFlag)
    {
        zfRetain(d->callbackOwnerObj);
        ++(d->callbackOwnerObjectRetainFlag);
    }
}
void ZFCallback::callbackOwnerObjectRelease(void) const
{
    if(d != zfnull && d->callbackOwnerObjectRetainFlag > 0)
    {
        --(d->callbackOwnerObjectRetainFlag);
        if(d->callbackOwnerObjectRetainFlag == 0)
        {
            zfRetainChange(d->callbackOwnerObj, zfnull);
        }
    }
}

void ZFCallback::callbackSerializeCustomTypeSet(ZF_IN const zfchar *customType)
{
    if(d == zfnull)
    {
        d = zfnew(_ZFP_ZFCallbackPrivate);
    }
    zfsChange(d->serializableCustomType, customType);
}
const zfchar *ZFCallback::callbackSerializeCustomType(void) const
{
    return (d ? d->serializableCustomType : zfnull);
}
void ZFCallback::callbackSerializeCustomDataSet(ZF_IN const ZFSerializableData *customData)
{
    if(d == zfnull)
    {
        d = zfnew(_ZFP_ZFCallbackPrivate);
    }
    if(customData == zfnull)
    {
        if(d->serializableCustomData != zfnull)
        {
            zfdelete(d->serializableCustomData);
            d->serializableCustomData = zfnull;
        }
    }
    else
    {
        if(d->serializableCustomData != zfnull)
        {
            *(d->serializableCustomData) = *customData;
        }
        else
        {
            d->serializableCustomData = zfnew(ZFSerializableData, *customData);
        }
    }
}
const ZFSerializableData *ZFCallback::callbackSerializeCustomData(void) const
{
    return (d ? d->serializableCustomData : zfnull);
}

const ZFPathInfo *ZFCallback::pathInfo(void) const
{
    return (d ? d->pathInfo : zfnull);
}
void ZFCallback::pathInfoSet(ZF_IN const ZFPathInfo *pathInfo)
{
    if(d == zfnull)
    {
        d = zfnew(_ZFP_ZFCallbackPrivate);
    }
    if(pathInfo != zfnull && !(pathInfo->pathType.isEmpty() && pathInfo->pathData.isEmpty()))
    {
        if(d->pathInfo == zfnull)
        {
            d->pathInfo = zfnew(ZFPathInfo, *pathInfo);
        }
        else
        {
            *(d->pathInfo) = *pathInfo;
        }
    }
    else
    {
        if(d->pathInfo != zfnull)
        {
            zfdelete(d->pathInfo);
            d->pathInfo = zfnull;
        }
    }
}
void ZFCallback::pathInfoSet(ZF_IN const zfchar *pathType, ZF_IN const zfchar *pathData)
{
    if(d == zfnull)
    {
        d = zfnew(_ZFP_ZFCallbackPrivate);
    }
    if(!zfsIsEmpty(pathType) || !zfsIsEmpty(pathData))
    {
        if(d->pathInfo == zfnull)
        {
            d->pathInfo = zfnew(ZFPathInfo, pathType, pathData);
        }
        else
        {
            d->pathInfo->pathType = pathType;
            d->pathInfo->pathData = pathData;
        }
    }
    else
    {
        if(d->pathInfo != zfnull)
        {
            zfdelete(d->pathInfo);
            d->pathInfo = zfnull;
        }
    }
}

ZFFuncAddrType ZFCallback::_ZFP_ZFCallbackCached_callbackInvoker_rawFunction(void) const
{
    return (d ? d->callbackRawFunc : zfnull);
}
ZFObject *ZFCallback::_ZFP_ZFCallbackCached_callbackOwnerObj(void) const
{
    return (d ? d->callbackOwnerObj : zfnull);
}

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFCallback, void, callbackIdSet, ZFMP_IN(const zfchar *, callbackId))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallback, const zfchar *, callbackId)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFCallback, void, callbackTagSet, ZFMP_IN(const zfchar *, key), ZFMP_IN(ZFObject *, tag))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFCallback, ZFObject *, callbackTagGet, ZFMP_IN(const zfchar *, key))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFCallback, void, callbackTagGetAllKeyValue, ZFMP_IN_OUT(ZFCoreArray<const zfchar *> &, allKey), ZFMP_IN_OUT(ZFCoreArray<ZFObject *> &, allValue))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFCallback, void, callbackTagRemove, ZFMP_IN(const zfchar *, key))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFCallback, zfautoObject, callbackTagRemoveAndGet, ZFMP_IN(const zfchar *, key))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallback, void, callbackTagRemoveAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallback, zfbool, callbackIsValid)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallback, ZFCallbackType, callbackType)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallback, ZFObject *, callbackOwnerObject)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallback, const ZFMethod *, callbackMethod)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallback, ZFFuncAddrType, callbackFunctionAddr)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallback, void, callbackClear)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallback, void, callbackOwnerObjectRetain)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallback, void, callbackOwnerObjectRelease)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFCallback, void, callbackSerializeCustomTypeSet, ZFMP_IN(const zfchar *, customType))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallback, const zfchar *, callbackSerializeCustomType)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFCallback, void, callbackSerializeCustomDataSet, ZFMP_IN(const ZFSerializableData &, customData))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallback, const ZFSerializableData *, callbackSerializeCustomData)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFCallback, void, callbackSerializeCustomDisable, ZFMP_IN(zfbool, disable))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallback, zfbool, callbackSerializeCustomDisabled)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallback, const ZFPathInfo *, pathInfo)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFCallback, void, pathInfoSet, ZFMP_IN(const ZFPathInfo *, pathInfo))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFCallback, void, pathInfoSet, ZFMP_IN(const zfchar *, pathType), ZFMP_IN(const zfchar *, pathData))

// ============================================================
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_0(ZFCallback, ZFCallbackNull)
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(ZFCallback, ZFCallbackForMethod, ZFMP_IN(const ZFMethod *, zfmethod))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(ZFCallback, ZFCallbackForMemberMethod, ZFMP_IN(ZFObject *, obj), ZFMP_IN(const ZFMethod *, zfmethod))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(ZFCallback, ZFCallbackForFunc, ZFMP_IN(ZFFuncAddrType, callbackRawFunc))

ZF_NAMESPACE_GLOBAL_END
#endif

