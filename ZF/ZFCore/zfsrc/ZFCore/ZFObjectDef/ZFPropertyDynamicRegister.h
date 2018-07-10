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
 * @file ZFPropertyDynamicRegister.h
 * @brief user registered ZFProperty
 */

#ifndef _ZFI_ZFPropertyDynamicRegister_h_
#define _ZFI_ZFPropertyDynamicRegister_h_

#include "ZFMethodDynamicRegister.h"
#include "ZFProperty.h"
#include "ZFObjectSmartPointer.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassFwd ZFPropertyDynamicRegisterParam;
/**
 * @brief dynamic register a property,
 *   return null if fail
 *
 * comparing to #ZFPropertyUserRegisterRetain, a dynamic property would:
 * -  completely dynamic, can be registered from script language
 * -  use #ZFMethodGenericInvoker to implement,
 *   have lower performance (trade for flexibility)
 * -  no weak property support (assign property with ZFObject type)
 * -  you may register a property by supplying existing setter and getter method,
 *   or simply leave it empty to use builtin impl
 *
 * @note dynamic registered contents would be removed automatically
 *   during #ZFFrameworkCleanup as level #ZFLevelZFFrameworkHigh
 */
extern ZF_ENV_EXPORT const ZFProperty *ZFPropertyDynamicRegister(ZF_IN const ZFPropertyDynamicRegisterParam &param,
                                                                 ZF_OUT_OPT zfstring *errorHint = zfnull);
/**
 * @brief see #ZFPropertyDynamicRegister
 */
extern ZF_ENV_EXPORT void ZFPropertyDynamicUnregister(ZF_IN const ZFProperty *property);

// ============================================================
/**
 * @brief callback to setup init value for #ZFPropertyDynamicRegister
 *
 * returned object must be valid to #ZFTypeIdBase::typeIdWrapper
 */
typedef zfautoObject (*ZFPropertyDynamicRegisterInitValueCallback)(ZF_IN const ZFProperty *property);

// ============================================================
zfclassFwd _ZFP_ZFPropertyDynamicRegisterParamPrivate;
/** @brief param for #ZFPropertyDynamicRegister */
zffinal zfclassLikePOD ZF_ENV_EXPORT ZFPropertyDynamicRegisterParam
{
public:
    /** @brief see #ZFPropertyDynamicRegister */
    ZFPropertyDynamicRegisterParam &propertyDynamicRegisterUserDataSet(ZF_IN ZFObject *propertyDynamicRegisterUserData);
    /** @brief see #ZFPropertyDynamicRegister */
    ZFObject *propertyDynamicRegisterUserData(void) const;

    /** @brief see #ZFPropertyDynamicRegister */
    ZFPropertyDynamicRegisterParam &propertyOwnerClassSet(ZF_IN const ZFClass *propertyOwnerClass);
    /** @brief see #ZFPropertyDynamicRegister */
    const ZFClass *propertyOwnerClass(void) const;

    /** @brief see #ZFPropertyDynamicRegister */
    ZFPropertyDynamicRegisterParam &propertyTypeIdSet(ZF_IN const zfchar *propertyTypeId);
    /** @brief see #ZFPropertyDynamicRegister */
    const zfchar *propertyTypeId(void) const;

    /** @brief see #ZFPropertyDynamicRegister */
    ZFPropertyDynamicRegisterParam &propertyTypeNameSet(ZF_IN const zfchar *propertyTypeName);
    /** @brief see #ZFPropertyDynamicRegister */
    const zfchar *propertyTypeName(void) const;

    /** @brief see #ZFPropertyDynamicRegister */
    ZFPropertyDynamicRegisterParam &propertyNameSet(ZF_IN const zfchar *propertyName);
    /** @brief see #ZFPropertyDynamicRegister */
    const zfchar *propertyName(void) const;

    /** @brief see #ZFPropertyDynamicRegister */
    ZFPropertyDynamicRegisterParam &propertyClassOfRetainPropertySet(ZF_IN const ZFClass *propertyClassOfRetainProperty);
    /** @brief see #ZFPropertyDynamicRegister */
    const ZFClass *propertyClassOfRetainProperty(void) const;

    /** @brief see #ZFPropertyDynamicRegister */
    ZFPropertyDynamicRegisterParam &propertyInitValueCallbackSet(ZF_IN ZFPropertyDynamicRegisterInitValueCallback propertyInitValueCallback);
    /** @brief see #ZFPropertyDynamicRegister */
    ZFPropertyDynamicRegisterInitValueCallback propertyInitValueCallback(void) const;

    /** @brief see #ZFPropertyDynamicRegister */
    ZFPropertyDynamicRegisterParam &propertySetterTypeSet(ZF_IN ZFMethodPrivilegeType propertySetterType);
    /** @brief see #ZFPropertyDynamicRegister */
    ZFMethodPrivilegeType propertySetterType(void) const;

    /** @brief see #ZFPropertyDynamicRegister */
    ZFPropertyDynamicRegisterParam &propertyGetterTypeSet(ZF_IN ZFMethodPrivilegeType propertyGetterType);
    /** @brief see #ZFPropertyDynamicRegister */
    ZFMethodPrivilegeType propertyGetterType(void) const;

    // ============================================================
    // for custom impl
    /** @brief see #ZFPropertyDynamicRegister */
    ZFPropertyDynamicRegisterParam &propertyCustomImplSet(ZF_IN const ZFMethod *propertySetterMethod
                                                          , ZF_IN const ZFMethod *propertyGetterMethod
                                                          , ZF_IN ZFPropertyCallbackIsValueAccessed callbackIsValueAccessed
                                                          , ZF_IN ZFPropertyCallbackIsInitValue callbackIsInitValue
                                                          , ZF_IN ZFPropertyCallbackValueReset callbackValueReset
                                                          , ZF_IN_OPT ZFPropertyCallbackValueSet callbackValueSet = zfnull
                                                          , ZF_IN_OPT ZFPropertyCallbackValueGet callbackValueGet = zfnull
                                                          , ZF_IN_OPT ZFPropertyCallbackValueGetRelease callbackValueGetRelease = zfnull
                                                          , ZF_IN_OPT ZFPropertyCallbackCompare callbackCompare = zfnull
                                                          , ZF_IN_OPT ZFPropertyCallbackGetInfo callbackGetInfo = zfnull
                                                          , ZF_IN_OPT ZFPropertyCallbackValueStore callbackValueStore = zfnull
                                                          , ZF_IN_OPT ZFPropertyCallbackValueRelease callbackValueRelease = zfnull
                                                          , ZF_IN_OPT ZFPropertyCallbackProgressUpdate callbackProgressUpdate = zfnull
                                                          );

    /** @brief see #ZFPropertyDynamicRegister */
    const ZFMethod *propertyCustomImplSetterMethod(void) const;
    /** @brief see #ZFPropertyDynamicRegister */
    const ZFMethod *propertyCustomImplGetterMethod(void) const;
    /** @brief see #ZFPropertyDynamicRegister */
    ZFPropertyCallbackIsValueAccessed propertyCustomImplCallbackIsValueAccessed(void) const;
    /** @brief see #ZFPropertyDynamicRegister */
    ZFPropertyCallbackIsInitValue propertyCustomImplCallbackIsInitValue(void) const;
    /** @brief see #ZFPropertyDynamicRegister */
    ZFPropertyCallbackValueReset propertyCustomImplCallbackValueReset(void) const;
    /** @brief see #ZFPropertyDynamicRegister */
    ZFPropertyCallbackValueSet propertyCustomImplCallbackValueSet(void) const;
    /** @brief see #ZFPropertyDynamicRegister */
    ZFPropertyCallbackValueGet propertyCustomImplCallbackValueGet(void) const;
    /** @brief see #ZFPropertyDynamicRegister */
    ZFPropertyCallbackValueGetRelease propertyCustomImplCallbackValueGetRelease(void) const;
    /** @brief see #ZFPropertyDynamicRegister */
    ZFPropertyCallbackCompare propertyCustomImplCallbackCompare(void) const;
    /** @brief see #ZFPropertyDynamicRegister */
    ZFPropertyCallbackGetInfo propertyCustomImplCallbackGetInfo(void) const;
    /** @brief see #ZFPropertyDynamicRegister */
    ZFPropertyCallbackValueStore propertyCustomImplCallbackValueStore(void) const;
    /** @brief see #ZFPropertyDynamicRegister */
    ZFPropertyCallbackValueRelease propertyCustomImplCallbackValueRelease(void) const;
    /** @brief see #ZFPropertyDynamicRegister */
    ZFPropertyCallbackProgressUpdate propertyCustomImplCallbackProgressUpdate(void) const;

    // ============================================================
public:
    /** @cond ZFPrivateDoc */
    ZFPropertyDynamicRegisterParam(void);
    ZFPropertyDynamicRegisterParam(ZF_IN ZFPropertyDynamicRegisterParam const &ref);
    ~ZFPropertyDynamicRegisterParam(void);
    ZFPropertyDynamicRegisterParam &operator = (ZF_IN ZFPropertyDynamicRegisterParam const &ref);
    zfbool operator == (ZF_IN ZFPropertyDynamicRegisterParam const &ref) const;
    zfbool operator != (ZF_IN ZFPropertyDynamicRegisterParam const &ref) const {return !this->operator==(ref);}
    /** @endcond */

private:
    _ZFP_ZFPropertyDynamicRegisterParamPrivate *d;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFPropertyDynamicRegister_h_

