/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
/**
 * @file ZFMethodDynamicRegister.h
 * @brief user registered ZFMethod
 */

#ifndef _ZFI_ZFMethodDynamicRegister_h_
#define _ZFI_ZFMethodDynamicRegister_h_

#include "ZFMethod.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassFwd ZFMethodDynamicRegisterParam;
/**
 * @brief dynamic register a method,
 *   return null if fail
 *
 * comparing to #ZFMethodUserRegister_0, a dynamic method would:
 * -  completely dynamic, can be registered from script language
 * -  use #ZFMethodGenericInvoker to implement,
 *   have lower performance (trade for flexibility)
 *
 * @note dynamic registered contents would be removed automatically
 *   during #ZFFrameworkCleanup as level #ZFLevelZFFrameworkHigh
 * @warning (ZFTAG_LIMITATION) for dynamic registered method,
 *   if your method has return value,
 *   you must take care the life cycle of the return value\n
 *   when the method called by plain #ZFMethod::execute
 *   instead of #ZFMethod::methodGenericInvoker,
 *   the #ZFTypeIdWrapper created by generic invoker
 *   would be released immediately after method call,
 *   which would cause the raw return value
 *   points to invalid address\n
 *   to solve this, you have these workaround when implementing
 *   #ZFMethodDynamicRegisterParam::methodGenericInvoker:
 *   -  try not to have "non value type" return value for dynamic method
 *     (i.e. reference or pointer type)
 *   -  store return value to owner object as #ZFObject::tagSet
 *     (at this case, you should take care of circular reference,
 *     and value's life cycle)
 *   -  #zfRetain and #zfautoRelease to extends the return value's life time
 *     (at this case, you should ensure the method's caller won't store
 *     the return value for a long time)
 */
extern ZF_ENV_EXPORT const ZFMethod *ZFMethodDynamicRegister(ZF_IN const ZFMethodDynamicRegisterParam &param,
                                                             ZF_OUT_OPT zfstring *errorHint = zfnull);
/**
 * @brief see #ZFMethodDynamicRegister
 */
extern ZF_ENV_EXPORT void ZFMethodDynamicUnregister(ZF_IN const ZFMethod *method);

// ============================================================
/* ZFMETHOD_MAX_PARAM */
zfclassFwd _ZFP_ZFMethodDynamicRegisterParamPrivate;
/**
 * @brief param for #ZFMethodDynamicRegister
 *
 * required:
 * -  methodOwnerClass or methodNamespace
 * -  methodGenericInvoker
 * -  methodName
 *
 * optional:
 * -  methodDynamicRegisterUserData, null by default
 * -  methodType, #ZFMethodTypeStatic by default
 * -  methodPrivilegeType, #ZFMethodPrivilegeTypePublic by default
 * -  methodReturnTypeId, #ZFTypeId_void by default
 * -  methodReturnTypeName, empty by default
 */
zffinal zfclassLikePOD ZF_ENV_EXPORT ZFMethodDynamicRegisterParam
{
public:
    /** @brief see #ZFMethodDynamicRegister */
    ZFMethodDynamicRegisterParam &methodDynamicRegisterUserDataSet(ZF_IN ZFObject *methodDynamicRegisterUserData);
    /** @brief see #ZFMethodDynamicRegister */
    ZFObject *methodDynamicRegisterUserData(void) const;

    /** @brief see #ZFMethodDynamicRegister */
    ZFMethodDynamicRegisterParam &methodOwnerClassSet(ZF_IN const ZFClass *methodOwnerClass);
    /** @brief see #ZFMethodDynamicRegister */
    const ZFClass *methodOwnerClass(void) const;

    /** @brief see #ZFMethodDynamicRegister */
    ZFMethodDynamicRegisterParam &methodNamespaceSet(ZF_IN const zfchar *methodNamespace);
    /** @brief see #ZFMethodDynamicRegister */
    const zfchar *methodNamespace(void) const;

    /** @brief see #ZFMethodDynamicRegister */
    ZFMethodDynamicRegisterParam &methodGenericInvokerSet(ZF_IN ZFMethodGenericInvoker methodGenericInvoker);
    /** @brief see #ZFMethodDynamicRegister */
    ZFMethodGenericInvoker methodGenericInvoker(void) const;

    /** @brief see #ZFMethodDynamicRegister */
    ZFMethodDynamicRegisterParam &methodTypeSet(ZF_IN ZFMethodType methodType);
    /** @brief see #ZFMethodDynamicRegister */
    ZFMethodType methodType(void) const;

    /** @brief see #ZFMethodDynamicRegister */
    ZFMethodDynamicRegisterParam &methodPrivilegeTypeSet(ZF_IN ZFMethodPrivilegeType methodPrivilegeType);
    /** @brief see #ZFMethodDynamicRegister */
    ZFMethodPrivilegeType methodPrivilegeType(void) const;

    /** @brief see #ZFMethodDynamicRegister */
    ZFMethodDynamicRegisterParam &methodNameSet(ZF_IN const zfchar *methodName);
    /** @brief see #ZFMethodDynamicRegister */
    const zfchar *methodName(void) const;

    /** @brief see #ZFMethodDynamicRegister */
    ZFMethodDynamicRegisterParam &methodReturnTypeIdSet(ZF_IN const zfchar *methodReturnTypeId);
    /** @brief see #ZFMethodDynamicRegister */
    const zfchar *methodReturnTypeId(void) const;

    /** @brief see #ZFMethodDynamicRegister */
    ZFMethodDynamicRegisterParam &methodReturnTypeNameSet(ZF_IN const zfchar *methodReturnTypeName);
    /** @brief see #ZFMethodDynamicRegister */
    const zfchar *methodReturnTypeName(void) const;

    /** @brief see #ZFMethodDynamicRegister */
    ZFMethodDynamicRegisterParam &methodParamAdd(ZF_IN const zfchar *methodParamTypeId,
                                                 ZF_IN_OPT const zfchar *methodParamTypeName = zfnull,
                                                 ZF_IN_OPT ZFMethodParamDefaultValueCallback methodParamDefaultValueCallback = zfnull);
    /** @brief see #ZFMethodDynamicRegister */
    zfindex methodParamCount(void) const;
    /** @brief see #ZFMethodDynamicRegister */
    const zfchar *methodParamTypeIdAtIndex(ZF_IN zfindex index) const;
    /** @brief see #ZFMethodDynamicRegister */
    const zfchar *methodParamTypeNameAtIndex(ZF_IN zfindex index) const;
    /** @brief see #ZFMethodDynamicRegister */
    ZFMethodParamDefaultValueCallback methodParamDefaultValueCallbackAtIndex(ZF_IN zfindex index) const;

public:
    /** @cond ZFPrivateDoc */
    ZFMethodDynamicRegisterParam(void);
    ZFMethodDynamicRegisterParam(ZF_IN ZFMethodDynamicRegisterParam const &ref);
    ~ZFMethodDynamicRegisterParam(void);
    ZFMethodDynamicRegisterParam &operator = (ZF_IN ZFMethodDynamicRegisterParam const &ref);
    zfbool operator == (ZF_IN ZFMethodDynamicRegisterParam const &ref) const;
    zfbool operator != (ZF_IN ZFMethodDynamicRegisterParam const &ref) const {return !this->operator==(ref);}
    /** @endcond */

private:
    _ZFP_ZFMethodDynamicRegisterParamPrivate *d;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFMethodDynamicRegister_h_

