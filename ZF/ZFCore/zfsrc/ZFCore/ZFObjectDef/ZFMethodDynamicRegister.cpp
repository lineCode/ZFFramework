/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFMethodDynamicRegister.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassNotPOD _ZFP_ZFMethodDynamicRegisterParamPrivate
{
public:
    zfuint refCount;
    zfautoObject methodDynamicRegisterUserData;
    const ZFClass *methodOwnerClass;
    zfstring methodNamespace;
    ZFMethodGenericInvoker methodGenericInvoker;
    ZFMethodType methodType;
    ZFMethodPrivilegeType methodPrivilegeType;
    zfstring methodName;
    zfstring methodReturnTypeId;
    zfstring methodReturnTypeName;
    zfindex methodParamCount;
    zfstring methodParamTypeId[ZFMETHOD_MAX_PARAM];
    zfstring methodParamTypeName[ZFMETHOD_MAX_PARAM];
    ZFMethodParamDefaultValueCallback methodParamDefaultValueCallback[ZFMETHOD_MAX_PARAM];

public:
    _ZFP_ZFMethodDynamicRegisterParamPrivate(void)
    : refCount(1)
    , methodDynamicRegisterUserData()
    , methodOwnerClass(zfnull)
    , methodNamespace()
    , methodGenericInvoker(zfnull)
    , methodType(ZFMethodTypeStatic)
    , methodPrivilegeType(ZFMethodPrivilegeTypePublic)
    , methodName()
    , methodReturnTypeId(ZFTypeId_void())
    , methodReturnTypeName()
    , methodParamCount(0)
    , methodParamTypeId()
    , methodParamTypeName()
    , methodParamDefaultValueCallback()
    {
    }
};

// ============================================================
ZFMethodDynamicRegisterParam &ZFMethodDynamicRegisterParam::methodDynamicRegisterUserDataSet(ZF_IN ZFObject *methodDynamicRegisterUserData)
{
    d->methodDynamicRegisterUserData = methodDynamicRegisterUserData;
    return *this;
}
ZFObject *ZFMethodDynamicRegisterParam::methodDynamicRegisterUserData(void) const
{
    return d->methodDynamicRegisterUserData;
}

ZFMethodDynamicRegisterParam &ZFMethodDynamicRegisterParam::methodOwnerClassSet(ZF_IN const ZFClass *methodOwnerClass)
{
    d->methodOwnerClass = methodOwnerClass;
    return *this;
}
const ZFClass *ZFMethodDynamicRegisterParam::methodOwnerClass(void) const
{
    return d->methodOwnerClass;
}

ZFMethodDynamicRegisterParam &ZFMethodDynamicRegisterParam::methodNamespaceSet(ZF_IN const zfchar *methodNamespace)
{
    d->methodNamespace = methodNamespace;
    return *this;
}
const zfchar *ZFMethodDynamicRegisterParam::methodNamespace(void) const
{
    return (d->methodNamespace.isEmpty() ? zfnull : d->methodNamespace.cString());
}

ZFMethodDynamicRegisterParam &ZFMethodDynamicRegisterParam::methodGenericInvokerSet(ZF_IN ZFMethodGenericInvoker methodGenericInvoker)
{
    d->methodGenericInvoker = methodGenericInvoker;
    return *this;
}
ZFMethodGenericInvoker ZFMethodDynamicRegisterParam::methodGenericInvoker(void) const
{
    return d->methodGenericInvoker;
}

ZFMethodDynamicRegisterParam &ZFMethodDynamicRegisterParam::methodTypeSet(ZF_IN ZFMethodType methodType)
{
    d->methodType = methodType;
    return *this;
}
ZFMethodType ZFMethodDynamicRegisterParam::methodType(void) const
{
    return d->methodType;
}

ZFMethodDynamicRegisterParam &ZFMethodDynamicRegisterParam::methodPrivilegeTypeSet(ZF_IN ZFMethodPrivilegeType methodPrivilegeType)
{
    d->methodPrivilegeType = methodPrivilegeType;
    return *this;
}
ZFMethodPrivilegeType ZFMethodDynamicRegisterParam::methodPrivilegeType(void) const
{
    return d->methodPrivilegeType;
}

ZFMethodDynamicRegisterParam &ZFMethodDynamicRegisterParam::methodNameSet(ZF_IN const zfchar *methodName)
{
    d->methodName = methodName;
    return *this;
}
const zfchar *ZFMethodDynamicRegisterParam::methodName(void) const
{
    return (d->methodName.isEmpty() ? zfnull : d->methodName.cString());
}

ZFMethodDynamicRegisterParam &ZFMethodDynamicRegisterParam::methodReturnTypeIdSet(ZF_IN const zfchar *methodReturnTypeId)
{
    d->methodReturnTypeId = methodReturnTypeId;
    return *this;
}
const zfchar *ZFMethodDynamicRegisterParam::methodReturnTypeId(void) const
{
    return (d->methodReturnTypeId.isEmpty() ? zfnull : d->methodReturnTypeId.cString());
}

ZFMethodDynamicRegisterParam &ZFMethodDynamicRegisterParam::methodReturnTypeNameSet(ZF_IN const zfchar *methodReturnTypeName)
{
    d->methodReturnTypeName = methodReturnTypeName;
    return *this;
}
const zfchar *ZFMethodDynamicRegisterParam::methodReturnTypeName(void) const
{
    return (d->methodReturnTypeName.isEmpty() ? d->methodReturnTypeId.cString() : d->methodReturnTypeName.cString());
}

ZFMethodDynamicRegisterParam &ZFMethodDynamicRegisterParam::methodParamAdd(ZF_IN const zfchar *methodParamTypeId,
                                                                           ZF_IN_OPT const zfchar *methodParamTypeName /* = zfnull */,
                                                                           ZF_IN_OPT ZFMethodParamDefaultValueCallback methodParamDefaultValueCallback /* = zfnull */)
{
    zfCoreAssert(d->methodParamCount < ZFMETHOD_MAX_PARAM);
    if(methodParamTypeId != zfnull)
    {
        d->methodParamTypeId[d->methodParamCount] = methodParamTypeId;
        d->methodParamTypeName[d->methodParamCount] = methodParamTypeName;
        d->methodParamDefaultValueCallback[d->methodParamCount] = methodParamDefaultValueCallback;
        ++(d->methodParamCount);
    }
    return *this;
}
zfindex ZFMethodDynamicRegisterParam::methodParamCount(void) const
{
    return d->methodParamCount;
}
const zfchar *ZFMethodDynamicRegisterParam::methodParamTypeIdAtIndex(ZF_IN zfindex index) const
{
    return (index < d->methodParamCount ? d->methodParamTypeId[index].cString() : zfnull);
}
const zfchar *ZFMethodDynamicRegisterParam::methodParamTypeNameAtIndex(ZF_IN zfindex index) const
{
    if(index < d->methodParamCount)
    {
        if(d->methodParamTypeName[index].isEmpty())
        {
            return d->methodParamTypeId[index].cString();
        }
        else
        {
            return d->methodParamTypeName[index].cString();
        }
    }
    else
    {
        return zfnull;
    }
}
ZFMethodParamDefaultValueCallback ZFMethodDynamicRegisterParam::methodParamDefaultValueCallbackAtIndex(ZF_IN zfindex index) const
{
    return (index < d->methodParamCount ? d->methodParamDefaultValueCallback[index] : zfnull);
}

/** @cond ZFPrivateDoc */
ZFMethodDynamicRegisterParam::ZFMethodDynamicRegisterParam(void)
: d(zfnew(_ZFP_ZFMethodDynamicRegisterParamPrivate))
{
}
ZFMethodDynamicRegisterParam::ZFMethodDynamicRegisterParam(ZF_IN ZFMethodDynamicRegisterParam const &ref)
: d(ref.d)
{
    ++(d->refCount);
}
ZFMethodDynamicRegisterParam::~ZFMethodDynamicRegisterParam(void)
{
    --(d->refCount);
    if(d->refCount == 0)
    {
        zfdelete(d);
    }
}
ZFMethodDynamicRegisterParam &ZFMethodDynamicRegisterParam::operator = (ZF_IN ZFMethodDynamicRegisterParam const &ref)
{
    ++(ref.d->refCount);
    --(d->refCount);
    if(d->refCount == 0)
    {
        zfdelete(d);
    }
    d = ref.d;
    return *this;
}
zfbool ZFMethodDynamicRegisterParam::operator == (ZF_IN ZFMethodDynamicRegisterParam const &ref) const
{
    return (d == ref.d);
}
/** @endcond */

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFMethodDynamicRegisterParam, ZFMethodDynamicRegisterParam &, methodDynamicRegisterUserDataSet, ZFMP_IN(ZFObject *, methodDynamicRegisterUserData))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFMethodDynamicRegisterParam, ZFObject *, methodDynamicRegisterUserData)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFMethodDynamicRegisterParam, ZFMethodDynamicRegisterParam &, methodOwnerClassSet, ZFMP_IN(const ZFClass *, methodOwnerClass))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFMethodDynamicRegisterParam, const ZFClass *, methodOwnerClass)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFMethodDynamicRegisterParam, ZFMethodDynamicRegisterParam &, methodNamespaceSet, ZFMP_IN(const zfchar *, methodNamespace))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFMethodDynamicRegisterParam, const zfchar *, methodNamespace)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFMethodDynamicRegisterParam, ZFMethodDynamicRegisterParam &, methodGenericInvokerSet, ZFMP_IN(ZFMethodGenericInvoker, methodGenericInvoker))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFMethodDynamicRegisterParam, ZFMethodGenericInvoker, methodGenericInvoker)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFMethodDynamicRegisterParam, ZFMethodDynamicRegisterParam &, methodTypeSet, ZFMP_IN(ZFMethodType, methodType))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFMethodDynamicRegisterParam, ZFMethodType, methodType)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFMethodDynamicRegisterParam, ZFMethodDynamicRegisterParam &, methodPrivilegeTypeSet, ZFMP_IN(ZFMethodPrivilegeType, methodPrivilegeType))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFMethodDynamicRegisterParam, ZFMethodPrivilegeType, methodPrivilegeType)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFMethodDynamicRegisterParam, ZFMethodDynamicRegisterParam &, methodNameSet, ZFMP_IN(const zfchar *, methodName))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFMethodDynamicRegisterParam, const zfchar *, methodName)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFMethodDynamicRegisterParam, ZFMethodDynamicRegisterParam &, methodReturnTypeIdSet, ZFMP_IN(const zfchar *, methodReturnTypeId))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFMethodDynamicRegisterParam, const zfchar *, methodReturnTypeId)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFMethodDynamicRegisterParam, ZFMethodDynamicRegisterParam &, methodReturnTypeNameSet, ZFMP_IN(const zfchar *, methodReturnTypeName))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFMethodDynamicRegisterParam, const zfchar *, methodReturnTypeName)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_3(v_ZFMethodDynamicRegisterParam, ZFMethodDynamicRegisterParam &, methodParamAdd, ZFMP_IN(const zfchar *, methodParamTypeId), ZFMP_IN_OPT(const zfchar *, methodParamTypeName, zfnull), ZFMP_IN_OPT(ZFMethodParamDefaultValueCallback, methodParamDefaultValueCallback, zfnull))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFMethodDynamicRegisterParam, zfindex, methodParamCount)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFMethodDynamicRegisterParam, const zfchar *, methodParamTypeIdAtIndex, ZFMP_IN(zfindex, index))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFMethodDynamicRegisterParam, const zfchar *, methodParamTypeNameAtIndex, ZFMP_IN(zfindex, index))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFMethodDynamicRegisterParam, ZFMethodParamDefaultValueCallback, methodParamDefaultValueCallbackAtIndex, ZFMP_IN(zfindex, index))

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(const ZFMethod *, ZFMethodDynamicRegister, ZFMP_IN(const ZFMethodDynamicRegisterParam &, param), ZFMP_OUT_OPT(zfstring *, errorHint, zfnull))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(void, ZFMethodDynamicUnregister, ZFMP_IN(const ZFMethod *, method))

ZF_NAMESPACE_GLOBAL_END
#endif

