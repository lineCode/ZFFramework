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
 * @file ZFDynamicInvoker.h
 * @brief dynamic invoker util for advanced dynamic impl
 */

#ifndef _ZFI_ZFDynamicInvoker_h_
#define _ZFI_ZFDynamicInvoker_h_

#include "ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief wrapper for unknown types for #ZFDI_invoke
 */
zfclass ZF_ENV_EXPORT ZFDI_Wrapper : zfextends ZFObject
{
    ZFOBJECT_DECLARE(ZFDI_Wrapper, ZFObject)

public:
    /**
     * @brief the data
     *
     * the data must store strings that can be converted by:
     * -  #ZFTypeIdWrapper::wrappedValueFromString
     * -  #ZFSerializeFromString
     */
    zfstring zfv;

public:
    zfoverride
    virtual zfidentity objectHash(void)
    {
        return zfidentityCalcString(this->zfv);
    }
    zfoverride
    virtual ZFCompareResult objectCompare(ZF_IN ZFObject *anotherObj)
    {
        zfself *ref = ZFCastZFObject(zfself *, anotherObj);
        if(ref != zfnull)
        {
            return ZFComparerDefault(this->zfv, ref->zfv);
        }
        else
        {
            return ZFCompareUncomparable;
        }
    }
protected:
    zfoverride
    virtual void objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
    {
        zfsuper::objectInfoOnAppend(ret);
        ret += zfText(", zfv: ");
        ret += this->zfv;
    }
};

// ============================================================
/**
 * @brief util method to convert to string
 *
 * support these types:
 * -  #v_zfstring
 * -  #ZFString
 * -  #ZFDI_Wrapper
 */
extern ZF_ENV_EXPORT const zfchar *ZFDI_toString(ZF_IN ZFObject *obj);

/**
 * @brief util method to find class as well as type id wrapper class
 *
 * we will try to find class by #ZFClass::classForName,
 * if unable to find,
 * we will append #ZFTypeIdWrapperPrefixName to find again, e.g.
 * "ClassName" to "v_ClassName" or "NS.ClassName" to "NS.v_ClassName"
 */
extern ZF_ENV_EXPORT const ZFClass *ZFDI_classForName(ZF_IN const zfchar *className,
                                                      ZF_IN const zfchar *NS);

/**
 * @brief perform advanced dynamic invoke
 *
 * note: when mentioned `string`, we means any type
 * that can be converted by #ZFDI_toString
 *
 * NS can be any string describe namespace,
 * and these values are considered in global scope:
 * -  null or empty string
 * -  #ZF_NAMESPACE_GLOBAL_NAME
 * -  #ZF_NAMESPACE_GLOBAL_ABBR_NAME
 *
 * type can be:
 * -  #v_ZFClass : same as #ZFDI_alloc
 * -  #v_ZFMethod : invoke using the method
 * -  string :
 *   -  holds method name ("methodName")
 *     or method name with namespace ("NS.methodName")
 *   -  holds #ZFClass::className or #ZFClass::classNameFull,
 *     see #ZFDI_alloc for how to find the class\n
 *     this rule only applied when obj is null
 *
 * params can be :
 * -  #ZFObject type for retain type
 * -  #ZFTypeIdWrapper for assign type
 * -  #ZFMethodGenericInvokerDefaultParam for default param
 * -  #ZFDI_Wrapper, we will try to convert to desired type if possible
 */
extern ZF_ENV_EXPORT zfbool ZFDI_invoke(ZF_OUT zfautoObject &ret
                                        , ZF_OUT_OPT zfstring *errorHint
                                        , ZF_IN_OPT ZFObject *obj
                                        , ZF_IN_OPT const zfchar *NS
                                        , ZF_IN ZFObject *type
                                        , ZF_IN zfindex paramCount
                                        , ZF_IN_OPT ZFObject *param0 = ZFMethodGenericInvokerDefaultParam()
                                        , ZF_IN_OPT ZFObject *param1 = ZFMethodGenericInvokerDefaultParam()
                                        , ZF_IN_OPT ZFObject *param2 = ZFMethodGenericInvokerDefaultParam()
                                        , ZF_IN_OPT ZFObject *param3 = ZFMethodGenericInvokerDefaultParam()
                                        , ZF_IN_OPT ZFObject *param4 = ZFMethodGenericInvokerDefaultParam()
                                        , ZF_IN_OPT ZFObject *param5 = ZFMethodGenericInvokerDefaultParam()
                                        , ZF_IN_OPT ZFObject *param6 = ZFMethodGenericInvokerDefaultParam()
                                        , ZF_IN_OPT ZFObject *param7 = ZFMethodGenericInvokerDefaultParam()
                                        /* ZFMETHOD_MAX_PARAM */);

/**
 * @brief perform advanced dynamic invoke
 *
 * for string type, NS, and more info, see also #ZFDI_invoke
 *
 * type can be:
 * -  #v_ZFClass : alloc using the class
 * -  string : holds #ZFClass::className or #ZFClass::classNameFull
 *
 * when finding class using string, we would try to find by:
 * -  #ZFClass::classForName
 * -  if not able to find, we would try to add the "v_" prefix
 *   to the class name then find again, e.g.
 *   "ClassName" to "v_ClassName" or "NS.ClassName" to "NS.v_ClassName"
 *
 * for the params, see #ZFDI_invoke for more info
 */
extern ZF_ENV_EXPORT zfbool ZFDI_alloc(ZF_OUT zfautoObject &ret
                                       , ZF_OUT_OPT zfstring *errorHint
                                       , ZF_IN_OPT const zfchar *NS
                                       , ZF_IN ZFObject *type
                                       , ZF_IN zfindex paramCount
                                       , ZF_IN_OPT ZFObject *param0 = ZFMethodGenericInvokerDefaultParam()
                                       , ZF_IN_OPT ZFObject *param1 = ZFMethodGenericInvokerDefaultParam()
                                       , ZF_IN_OPT ZFObject *param2 = ZFMethodGenericInvokerDefaultParam()
                                       , ZF_IN_OPT ZFObject *param3 = ZFMethodGenericInvokerDefaultParam()
                                       , ZF_IN_OPT ZFObject *param4 = ZFMethodGenericInvokerDefaultParam()
                                       , ZF_IN_OPT ZFObject *param5 = ZFMethodGenericInvokerDefaultParam()
                                       , ZF_IN_OPT ZFObject *param6 = ZFMethodGenericInvokerDefaultParam()
                                       , ZF_IN_OPT ZFObject *param7 = ZFMethodGenericInvokerDefaultParam()
                                       /* ZFMETHOD_MAX_PARAM */);
/** @brief see #ZFDI_alloc */
extern ZF_ENV_EXPORT zfbool ZFDI_alloc(ZF_OUT zfautoObject &ret
                                       , ZF_OUT_OPT zfstring *errorHint
                                       , ZF_IN const ZFClass *cls
                                       , ZF_IN zfindex paramCount
                                       , ZF_IN_OPT ZFObject *param0 = ZFMethodGenericInvokerDefaultParam()
                                       , ZF_IN_OPT ZFObject *param1 = ZFMethodGenericInvokerDefaultParam()
                                       , ZF_IN_OPT ZFObject *param2 = ZFMethodGenericInvokerDefaultParam()
                                       , ZF_IN_OPT ZFObject *param3 = ZFMethodGenericInvokerDefaultParam()
                                       , ZF_IN_OPT ZFObject *param4 = ZFMethodGenericInvokerDefaultParam()
                                       , ZF_IN_OPT ZFObject *param5 = ZFMethodGenericInvokerDefaultParam()
                                       , ZF_IN_OPT ZFObject *param6 = ZFMethodGenericInvokerDefaultParam()
                                       , ZF_IN_OPT ZFObject *param7 = ZFMethodGenericInvokerDefaultParam()
                                       );

// ============================================================
/**
 * @brief util method to convert param type from
 */
extern ZF_ENV_EXPORT zfbool ZFDI_paramConvert(ZF_OUT zfautoObject &ret,
                                              ZF_IN const zfchar *typeId,
                                              ZF_IN ZFDI_Wrapper *wrapper,
                                              ZF_OUT_OPT zfstring *errorHint = zfnull);

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFDynamicInvoker_h_

