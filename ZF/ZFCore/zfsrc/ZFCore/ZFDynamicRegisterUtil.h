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
 * @file ZFDynamicRegisterUtil.h
 * @brief user registered ZFProperty
 */

#ifndef _ZFI_ZFDynamicRegisterUtil_h_
#define _ZFI_ZFDynamicRegisterUtil_h_

#include "ZFObject.h"

ZF_NAMESPACE_GLOBAL_BEGIN
/* ZFMETHOD_MAX_PARAM */

/** @brief data to hold params for #ZFMethodGenericInvoker */
zfclass ZF_ENV_EXPORT ZFDynamicMethodData : zfextends ZFObject
{
    ZFOBJECT_DECLARE_WITH_CUSTOM_CTOR(ZFDynamicMethodData, ZFObject)

public:
    zfbool invokeSuccess; /**< @brief data to hold params for #ZFMethodGenericInvoker */
    const ZFMethod *invokerMethod; /**< @brief data to hold params for #ZFMethodGenericInvoker */
    ZFObject *invokerObject; /**< @brief data to hold params for #ZFMethodGenericInvoker */
    zfstring errorHint; /**< @brief data to hold params for #ZFMethodGenericInvoker */
    zfautoObject ret; /**< @brief data to hold params for #ZFMethodGenericInvoker */
    ZFObject *param0; /**< @brief data to hold params for #ZFMethodGenericInvoker */
    ZFObject *param1; /**< @brief data to hold params for #ZFMethodGenericInvoker */
    ZFObject *param2; /**< @brief data to hold params for #ZFMethodGenericInvoker */
    ZFObject *param3; /**< @brief data to hold params for #ZFMethodGenericInvoker */
    ZFObject *param4; /**< @brief data to hold params for #ZFMethodGenericInvoker */
    ZFObject *param5; /**< @brief data to hold params for #ZFMethodGenericInvoker */
    ZFObject *param6; /**< @brief data to hold params for #ZFMethodGenericInvoker */
    ZFObject *param7; /**< @brief data to hold params for #ZFMethodGenericInvoker */
protected:
    /** @cond ZFPrivateDoc */
    ZFDynamicMethodData(void)
    : ZFObject()
    , invokeSuccess(zftrue)
    , invokerMethod(zfnull)
    , invokerObject(zfnull)
    , errorHint()
    , ret()
    , param0(zfnull)
    , param1(zfnull)
    , param2(zfnull)
    , param3(zfnull)
    , param4(zfnull)
    , param5(zfnull)
    , param6(zfnull)
    , param7(zfnull)
    {
    }
    /** @endcond */
public:
    zfoverride
    virtual zfbool objectIsPrivate(void)
    {
        return zftrue;
    }
    zfoverride
    virtual zfbool objectIsInternal(void)
    {
        return zftrue;
    }
};


/** @brief data to hold params for #ZFPropertyDynamicRegisterInitValueCallback */
zfclass ZF_ENV_EXPORT ZFDynamicPropertyData : zfextends ZFObject
{
    ZFOBJECT_DECLARE_WITH_CUSTOM_CTOR(ZFDynamicPropertyData, ZFObject)

public:
    zfautoObject ret; /**< @brief data to hold params for #ZFPropertyDynamicRegisterInitValueCallback */
    const ZFProperty *property; /**< @brief data to hold params for #ZFPropertyDynamicRegisterInitValueCallback */
protected:
    /** @cond ZFPrivateDoc */
    ZFDynamicPropertyData(void)
    : ZFObject()
    , ret()
    , property(zfnull)
    {
    }
    /** @endcond */
public:
    zfoverride
    virtual zfbool objectIsPrivate(void)
    {
        return zftrue;
    }
    zfoverride
    virtual zfbool objectIsInternal(void)
    {
        return zftrue;
    }
};

// ============================================================
zfclassFwd _ZFP_ZFDynamicPrivate;
/**
 * @brief util class to dynamic register class/method/property
 *
 * usage:
 * @code
 *   ZFDynamic()
 *       .classBegin(xxx)
 *           .method(xxx)
 *           .property(xxx)
 *       .classEnd()
 *       .NSBegin(xxx)
 *           .method(xxx)
 *       .NSEnd();
 * @endcode
 *
 * when any steps failed, #errorCallbackNotify would be called,
 * and all further call would be ignored\n
 * \n
 * you may store the returned ZFDynamic object,
 * and use #removeAll to remove all registered items at once
 */
zfclassLikePOD ZF_ENV_EXPORT ZFDynamic
{
public:
    /** @cond ZFPrivateDoc */
    ZFDynamic(void);
    ZFDynamic(ZF_IN const ZFDynamic &ref);
    ZFDynamic &operator = (ZF_IN const ZFDynamic &ref);
    zfbool operator == (ZF_IN const ZFDynamic &ref) const;
    zfbool operator != (ZF_IN const ZFDynamic &ref) const {return !this->operator == (ref);}
    /** @endcond */

public:
    /** @brief see #ZFDynamic */
    void removeAll(void);
    /** @brief see #ZFDynamic */
    const ZFCoreArrayPOD<const ZFClass *> &allClass(void) const;
    /** @brief see #ZFDynamic */
    const ZFCoreArrayPOD<const ZFMethod *> &allMethod(void) const;
    /** @brief see #ZFDynamic */
    const ZFCoreArrayPOD<const ZFProperty *> &allProperty(void) const;

public:
    /** @brief see #ZFDynamic */
    ZFDynamic &classBegin(ZF_IN const zfchar *className,
                          ZF_IN_OPT const ZFClass *parentClass = ZFObject::ClassData());
    /** @brief see #ZFDynamic */
    ZFDynamic &classBegin(ZF_IN const zfchar *className,
                          ZF_IN const zfchar *parentClassName);
    /** @brief see #ZFDynamic */
    ZFDynamic &classBegin(ZF_IN const ZFClass *cls);
    /** @brief see #ZFDynamic */
    ZFDynamic &classEnd(void);

public:
    /** @brief see #ZFDynamic */
    ZFDynamic &NSBegin(ZF_IN_OPT const zfchar *methodNamespace = ZFMethodFuncNamespaceGlobal);
    /** @brief see #ZFDynamic */
    ZFDynamic &NSEnd(void);

public:
    /**
     * @brief see #ZFDynamic
     *
     * util method to register method,
     * methodCallback's param0 is #ZFDynamicMethodData,
     * userData is methodCallbackUserData
     */
    ZFDynamic &method(ZF_IN const ZFListener &methodCallback
                      , ZF_IN ZFObject *methodCallbackUserData
                      , ZF_IN const zfchar *methodReturnTypeId
                      , ZF_IN const zfchar *methodName
                      , ZF_IN_OPT const zfchar *methodParamTypeId0 = zfnull
                      , ZF_IN_OPT const zfchar *methodParamTypeId1 = zfnull
                      , ZF_IN_OPT const zfchar *methodParamTypeId2 = zfnull
                      , ZF_IN_OPT const zfchar *methodParamTypeId3 = zfnull
                      , ZF_IN_OPT const zfchar *methodParamTypeId4 = zfnull
                      , ZF_IN_OPT const zfchar *methodParamTypeId5 = zfnull
                      , ZF_IN_OPT const zfchar *methodParamTypeId6 = zfnull
                      , ZF_IN_OPT const zfchar *methodParamTypeId7 = zfnull
                      );
    /** @brief see #ZFDynamic */
    ZFDynamic &method(ZF_IN ZFMethodGenericInvoker methodGenericInvoker
                      , ZF_IN ZFObject *methodDynamicRegisterUserData
                      , ZF_IN const zfchar *methodReturnTypeId
                      , ZF_IN const zfchar *methodName
                      , ZF_IN_OPT const zfchar *methodParamTypeId0 = zfnull
                      , ZF_IN_OPT const zfchar *methodParamTypeId1 = zfnull
                      , ZF_IN_OPT const zfchar *methodParamTypeId2 = zfnull
                      , ZF_IN_OPT const zfchar *methodParamTypeId3 = zfnull
                      , ZF_IN_OPT const zfchar *methodParamTypeId4 = zfnull
                      , ZF_IN_OPT const zfchar *methodParamTypeId5 = zfnull
                      , ZF_IN_OPT const zfchar *methodParamTypeId6 = zfnull
                      , ZF_IN_OPT const zfchar *methodParamTypeId7 = zfnull
                      );
    /** @brief see #ZFDynamic */
    ZFDynamic &method(ZF_IN const ZFMethodDynamicRegisterParam &param);

public:
    /**
     * @brief see #ZFDynamic
     *
     * initValueCallback can be specified to setup property's init value,
     * param0 is #ZFDynamicPropertyData,
     * userData is the initValueCallbackUserData
     */
    ZFDynamic &property(ZF_IN const zfchar *propertyTypeId,
                        ZF_IN const zfchar *propertyName,
                        ZF_IN_OPT const ZFListener &initValueCallback = ZFCallbackNull(),
                        ZF_IN_OPT ZFObject *initValueCallbackUserData = zfnull);
    /** @brief see #ZFDynamic */
    ZFDynamic &property(ZF_IN const ZFClass *propertyClassOfRetainProperty,
                        ZF_IN const zfchar *propertyName,
                        ZF_IN_OPT const ZFListener &initValueCallback = ZFCallbackNull(),
                        ZF_IN_OPT ZFObject *initValueCallbackUserData = zfnull);
    /** @brief see #ZFDynamic */
    ZFDynamic &property(ZF_IN const ZFPropertyDynamicRegisterParam &param);

public:
    /** @brief error callback which would be called if error occurred */
    ZFDynamic &errorCallbackAdd(ZF_IN const ZFOutputCallback &errorCallback = ZFOutputCallbackDefault());
    /** @brief see #errorCallbackAdd */
    ZFDynamic &errorCallbackRemove(ZF_IN const ZFOutputCallback &errorCallback);
    /** @brief see #errorCallbackAdd */
    zfindex errorCallbackCount(void) const;
    /** @brief see #errorCallbackAdd */
    const ZFOutputCallback &errorCallbackAtIndex(ZF_IN zfindex index) const;
    /** @brief see #errorCallbackAdd */
    void errorCallbackNotify(ZF_IN const zfchar *errorHint) const;

private:
    _ZFP_ZFDynamicPrivate *d;
};
ZFPROPERTY_TYPE_ACCESS_ONLY_DECLARE(ZFDynamic, ZFDynamic)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFDynamicRegisterUtil_h_

