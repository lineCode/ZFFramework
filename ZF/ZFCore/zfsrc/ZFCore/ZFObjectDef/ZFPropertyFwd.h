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
 * @file ZFPropertyFwd.h
 * @brief property for ZFObject type
 */

#ifndef _ZFI_ZFPropertyFwd_h_
#define _ZFI_ZFPropertyFwd_h_

#include "ZFObjectClassTypeFwd.h"
#include "ZFTypeIdFwd.h"
ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd ZFProperty;
zfclassFwd ZFObject;
zfclassFwd ZFSerializableData;

// ============================================================
/**
 * @brief used to check whether the property has been accessed
 *
 * usage:
 * @code
 *   const ZFProperty *propertyInfo = ...;
 *   propertyInfo->callbackIsValueAccessed(propertyInfo, ownerObj);
 * @endcode
 */
typedef zfbool (*ZFPropertyCallbackIsValueAccessed)(ZF_IN const ZFProperty *property,
                                                    ZF_IN ZFObject *ownerObj);
/** @brief change default impl for #ZFPropertyCallbackIsValueAccessed, use with caution */
extern ZF_ENV_EXPORT void ZFPropertyCallbackIsValueAccessedChange(ZF_IN const ZFProperty *property,
                                                                  ZF_IN ZFPropertyCallbackIsValueAccessed callback);

// ============================================================
/**
 * @brief used to check whether the property is in init value state
 *
 * usage:
 * @code
 *   const ZFProperty *propertyInfo = ...;
 *   propertyInfo->callbackIsInitValue(propertyInfo, ownerObj, zfnull);
 * @endcode
 * @note the outInitValue can be set to access the init value (only when the checker returned false)
 * @note for the type of the value, see #ZFPropertyCallbackValueGet
 */
typedef zfbool (*ZFPropertyCallbackIsInitValue)(ZF_IN const ZFProperty *property,
                                                ZF_IN ZFObject *ownerObj,
                                                ZF_OUT_OPT void *outInitValue /* = zfnull */);
/** @brief change default impl for #ZFPropertyCallbackIsInitValue, use with caution */
extern ZF_ENV_EXPORT void ZFPropertyCallbackIsInitValueChange(ZF_IN const ZFProperty *property,
                                                              ZF_IN ZFPropertyCallbackIsInitValue callback);

// ============================================================
/**
 * @brief used to set property value without knowing the type
 *
 * @note for the type of the value, see #ZFPropertyCallbackValueGet
 */
typedef void (*ZFPropertyCallbackValueSet)(ZF_IN const ZFProperty *property,
                                           ZF_IN ZFObject *dstObj,
                                           ZF_IN const void *value);
/** @brief change default impl for #ZFPropertyCallbackValueSet, use with caution */
extern ZF_ENV_EXPORT void ZFPropertyCallbackValueSetChange(ZF_IN const ZFProperty *property,
                                                           ZF_IN ZFPropertyCallbackValueSet callback);

// ============================================================
/**
 * @brief used to get property value without knowing the type
 *
 * @note for retain property, the returned pointer points to a #zfautoObject\n
 *   for assign property, the returned pointer pointer
 */
typedef const void *(*ZFPropertyCallbackValueGet)(ZF_IN const ZFProperty *property,
                                                  ZF_IN ZFObject *ownerObj);
/** @brief change default impl for #ZFPropertyCallbackValueGet, use with caution */
extern ZF_ENV_EXPORT void ZFPropertyCallbackValueGetChange(ZF_IN const ZFProperty *property,
                                                           ZF_IN ZFPropertyCallbackValueGet callback);

// ============================================================
/**
 * @brief used to reset the property to its init state
 */
typedef void (*ZFPropertyCallbackValueReset)(ZF_IN const ZFProperty *property,
                                             ZF_IN ZFObject *ownerObj);
/** @brief change default impl for #ZFPropertyCallbackValueReset, use with caution */
extern ZF_ENV_EXPORT void ZFPropertyCallbackValueResetChange(ZF_IN const ZFProperty *property,
                                                             ZF_IN ZFPropertyCallbackValueReset callback);

// ============================================================
/**
 * @brief used to compare property's value
 *
 * usage:
 * @code
 *   const ZFProperty *propertyInfo = ...;
 *   ZFCompareResult result = propertyInfo->callbackCompare(propertyInfo, ownerObj, v0, v1);
 * @endcode
 * @note for the type of the value, see #ZFPropertyCallbackValueGet
 */
typedef ZFCompareResult (*ZFPropertyCallbackCompare)(ZF_IN const ZFProperty *property,
                                                     ZF_IN ZFObject *ownerObj,
                                                     ZF_IN const void *v0,
                                                     ZF_IN const void *v1);
/** @brief change default impl for #ZFPropertyCallbackCompare, use with caution */
extern ZF_ENV_EXPORT void ZFPropertyCallbackCompareChange(ZF_IN const ZFProperty *property,
                                                          ZF_IN ZFPropertyCallbackCompare callback);

// ============================================================
/**
 * @brief used to get info of the property without knowing the type,
 *   usually for debug use
 *
 * @note for the type of the value, see #ZFPropertyCallbackValueGet
 */
typedef void (*ZFPropertyCallbackGetInfo)(ZF_IN const ZFProperty *property,
                                          ZF_IN ZFObject *ownerObj,
                                          ZF_IN const void *value,
                                          ZF_IN_OUT zfstring &ret);
/** @brief change default impl for #ZFPropertyCallbackGetInfo, use with caution */
extern ZF_ENV_EXPORT void ZFPropertyCallbackGetInfoChange(ZF_IN const ZFProperty *property,
                                                          ZF_IN ZFPropertyCallbackGetInfo callback);

// ============================================================
/**
 * @brief used to store the property value
 *
 * @note for the type of the value, see #ZFPropertyCallbackValueGet
 */
typedef void *(*ZFPropertyCallbackValueStore)(ZF_IN const ZFProperty *property,
                                              ZF_IN ZFObject *ownerObj,
                                              ZF_IN_OPT const void *value /* = zfnull */);
/** @brief change default impl for #ZFPropertyCallbackValueStore, use with caution */
extern ZF_ENV_EXPORT void ZFPropertyCallbackValueStoreChange(ZF_IN const ZFProperty *property,
                                                             ZF_IN ZFPropertyCallbackValueStore callback);

// ============================================================
/**
 * @brief used to release the property value that was stored by #ZFPropertyCallbackValueStore
 *
 * @note for the type of the value, see #ZFPropertyCallbackValueGet
 */
typedef void (*ZFPropertyCallbackValueRelease)(ZF_IN const ZFProperty *property,
                                               ZF_IN ZFObject *ownerObj,
                                               ZF_IN void *valueStored);
/** @brief change default impl for #ZFPropertyCallbackValueRelease, use with caution */
extern ZF_ENV_EXPORT void ZFPropertyCallbackValueReleaseChange(ZF_IN const ZFProperty *property,
                                                               ZF_IN ZFPropertyCallbackValueRelease callback);

// ============================================================
/**
 * @brief used to serialize assign property, see also #ZFTypeIdWrapper::wrappedValueFromData
 */
typedef zfbool (*ZFPropertyCallbackSerializeFrom)(ZF_IN const ZFProperty *propertyInfo,
                                                  ZF_IN ZFObject *ownerObject,
                                                  ZF_IN const ZFSerializableData &serializableData,
                                                  ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */,
                                                  ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */);
/** @brief change default impl for #ZFPropertyCallbackSerializeFrom, use with caution */
extern ZF_ENV_EXPORT void ZFPropertyCallbackSerializeFromChange(ZF_IN const ZFProperty *property,
                                                                ZF_IN ZFPropertyCallbackSerializeFrom callback);

// ============================================================
/**
 * @brief used to serialize assign property, see also #ZFTypeIdWrapper::wrappedValueToData
 */
typedef zfbool (*ZFPropertyCallbackSerializeTo)(ZF_IN const ZFProperty *propertyInfo,
                                                ZF_IN ZFObject *ownerObject,
                                                ZF_OUT ZFSerializableData &serializableData,
                                                ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */);
/** @brief change default impl for #ZFPropertyCallbackSerializeTo, use with caution */
extern ZF_ENV_EXPORT void ZFPropertyCallbackSerializeToChange(ZF_IN const ZFProperty *property,
                                                              ZF_IN ZFPropertyCallbackSerializeTo callback);

// ============================================================
/**
 * @brief used to update property value accorrding to progress,
 *   see #ZFTYPEID_PROGRESS_DECLARE
 *
 * @note for the type of the value, see #ZFPropertyCallbackValueGet
 */
typedef zfbool (*ZFPropertyCallbackProgressUpdate)(ZF_IN const ZFProperty *property,
                                                   ZF_IN ZFObject *ownerObj,
                                                   ZF_IN_OPT const void *from /* = zfnull */,
                                                   ZF_IN_OPT const void *to /* = zfnull */,
                                                   ZF_IN_OPT zffloat progress /* = 1 */);
/** @brief change default impl for #ZFPropertyCallbackProgressUpdate, use with caution */
extern ZF_ENV_EXPORT void ZFPropertyCallbackProgressUpdateChange(ZF_IN const ZFProperty *property,
                                                                 ZF_IN ZFPropertyCallbackProgressUpdate callback);

// ============================================================
/**
 * @brief for user registered property only, used to setup a property's init value
 */
typedef void (*ZFPropertyCallbackUserRegisterInitValueSetup)(ZF_IN const ZFProperty *property,
                                                             ZF_IN_OUT void *v);

// ============================================================
typedef void (*_ZFP_PropLifeCycleWrapper)(ZF_IN ZFObject *propertyOwnerObject,
                                          ZF_IN void *propertyValue,
                                          ZF_IN const void *propertyValueOld);
zfclassPOD ZF_ENV_EXPORT _ZFP_PropLifeCycleData
{
public:
    const ZFClass *propertyOwnerClass;
    _ZFP_PropLifeCycleWrapper propertyLifeCycleWrapper;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFPropertyFwd_h_

