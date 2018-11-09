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
 * @file ZFPropertyCallbackDefaultImpl.h
 * @brief default callback impl for #ZFProperty
 */

#ifndef _ZFI_ZFPropertyCallbackDefaultImpl_h_
#define _ZFI_ZFPropertyCallbackDefaultImpl_h_

#include "ZFProperty.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
template<typename T_PropHT, typename T_PropVT>
inline void _ZFP_propCbDValueSet(ZF_IN const ZFProperty *property,
                                 ZF_IN ZFObject *dstObj,
                                 ZF_IN const void *value)
{
    property->setterMethod()->_ZFP_execute<void, T_PropVT const &>(dstObj, *(const T_PropHT *)value);
}
extern ZF_ENV_EXPORT void _ZFP_propCbDValueSet_generic(ZF_IN const ZFProperty *property,
                                                       ZF_IN ZFObject *dstObj,
                                                       ZF_IN const void *value);

// ============================================================
template<typename T_PropHT, typename T_PropVT>
inline const void *_ZFP_propCbDValueGet_assign(ZF_IN const ZFProperty *property,
                                               ZF_IN ZFObject *ownerObj,
                                               ZF_IN_OUT void *&valueToken)
{
    return &(property->getterMethod()->_ZFP_execute<T_PropVT const &>(ownerObj));
}
inline void _ZFP_propCbDValueGetRelease_dummy(ZF_IN const ZFProperty *property,
                                              ZF_IN ZFObject *ownerObj,
                                              ZF_IN void *valueToken,
                                              ZF_IN const void *value)
{
}
extern ZF_ENV_EXPORT const void *_ZFP_propCbDValueGet_generic(ZF_IN const ZFProperty *property,
                                                              ZF_IN ZFObject *ownerObj,
                                                              ZF_IN_OUT void *&valueToken);
extern ZF_ENV_EXPORT void _ZFP_propCbDValueGetRelease_generic(ZF_IN const ZFProperty *property,
                                                              ZF_IN ZFObject *ownerObj,
                                                              ZF_IN void *valueToken,
                                                              ZF_IN const void *value);

// ============================================================
template<typename T_PropHT>
inline ZFCompareResult _ZFP_propCbDCompare(ZF_IN const ZFProperty *property,
                                           ZF_IN ZFObject *ownerObj,
                                           ZF_IN const void *v0,
                                           ZF_IN const void *v1)
{
    return ZFComparerDefault(*(const T_PropHT *)v0, *(const T_PropHT *)v1);
}
extern ZF_ENV_EXPORT ZFCompareResult _ZFP_propCbDCompare_generic(ZF_IN const ZFProperty *property,
                                                                 ZF_IN ZFObject *ownerObj,
                                                                 ZF_IN const void *v0,
                                                                 ZF_IN const void *v1);

// ============================================================
template<typename T_PropHT>
inline void _ZFP_propCbDGetInfo(ZF_IN const ZFProperty *property,
                                ZF_IN ZFObject *ownerObj,
                                ZF_IN const void *value,
                                ZF_IN_OUT zfstring &ret)
{
    ZFCoreElementInfoGetter<T_PropHT>::elementInfoGetter(ret, *(const T_PropHT *)value);
}
extern ZF_ENV_EXPORT void _ZFP_propCbDGetInfo_generic(ZF_IN const ZFProperty *property,
                                                      ZF_IN ZFObject *ownerObj,
                                                      ZF_IN const void *value,
                                                      ZF_IN_OUT zfstring &ret);

// ============================================================
extern ZF_ENV_EXPORT void _ZFP_ZFPropertyValueStoreImpl(ZF_IN const ZFProperty *property,
                                                        ZF_IN ZFObject *ownerObj,
                                                        ZF_IN void *valueStored,
                                                        ZF_IN ZFCorePointerBase *valueHolder);
extern ZF_ENV_EXPORT void _ZFP_ZFPropertyValueReleaseImpl(ZF_IN const ZFProperty *property,
                                                          ZF_IN ZFObject *ownerObj,
                                                          ZF_IN void *valueStored);
template<typename T_PropHT>
inline void *_ZFP_propCbDValueStore(ZF_IN const ZFProperty *property,
                                    ZF_IN ZFObject *ownerObj,
                                    ZF_IN_OPT const void *value /* = zfnull */)
{
    T_PropHT *valueStored = zfnull;
    if(value == zfnull)
    {
        valueStored = zfnew(T_PropHT);
    }
    else
    {
        valueStored = zfnew(T_PropHT, *(const T_PropHT *)value);
    }
    _ZFP_ZFPropertyValueStoreImpl(property, ownerObj, valueStored, zfnew(ZFCorePointerForObject<T_PropHT *>, valueStored));
    return valueStored;
}
extern ZF_ENV_EXPORT void *_ZFP_propCbDValueStore_generic(ZF_IN const ZFProperty *property,
                                                          ZF_IN ZFObject *ownerObj,
                                                          ZF_IN_OPT const void *value /* = zfnull */);
template<typename T_PropHT>
inline void _ZFP_propCbDValueRelease(ZF_IN const ZFProperty *property,
                                     ZF_IN ZFObject *ownerObj,
                                     ZF_IN void *value)
{
    _ZFP_ZFPropertyValueReleaseImpl(property, ownerObj, value);
}
extern ZF_ENV_EXPORT void _ZFP_propCbDValueRelease_generic(ZF_IN const ZFProperty *property,
                                                           ZF_IN ZFObject *ownerObj,
                                                           ZF_IN void *valueStored);

// ============================================================
template<typename T_PropVT>
zfclassNotPOD _ZFP_ZFTypeId_propCbSerialize
{
public:
    static ZFPropertyCallbackSerializeFrom f(void)
    {
        return zfnull;
    }
    static ZFPropertyCallbackSerializeTo t(void)
    {
        return zfnull;
    }
};

extern ZF_ENV_EXPORT zfbool _ZFP_propCbDSerializeFrom_generic(ZF_IN const ZFProperty *propertyInfo,
                                                              ZF_IN ZFObject *ownerObject,
                                                              ZF_IN const ZFSerializableData &serializableData,
                                                              ZF_OUT_OPT zfstring *outErrorHint = zfnull,
                                                              ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull);
extern ZF_ENV_EXPORT zfbool _ZFP_propCbDSerializeFrom_impl(ZF_OUT zfautoObject &zfv,
                                                           ZF_IN const ZFProperty *propertyInfo,
                                                           ZF_IN const ZFSerializableData &serializableData,
                                                           ZF_OUT_OPT zfstring *outErrorHint = zfnull,
                                                           ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull);
extern ZF_ENV_EXPORT void _ZFP_propCbDSerializeFrom_errorOccurred(ZF_IN const ZFSerializableData &serializableData,
                                                                  ZF_OUT_OPT zfstring *outErrorHint = zfnull,
                                                                  ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull);
template<typename T_PropVT>
zfbool _ZFP_propCbDSerializeFrom(ZF_IN const ZFProperty *propertyInfo,
                                 ZF_IN ZFObject *ownerObject,
                                 ZF_IN const ZFSerializableData &serializableData,
                                 ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */,
                                 ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */)
{
    zfautoObject zfv;
    if(_ZFP_propCbDSerializeFrom_impl(zfv, propertyInfo, serializableData, outErrorHint, outErrorPos))
    {
        if(ZFTypeId<T_PropVT>::template Value<T_PropVT>::zfvAccessAvailable(zfv))
        {
            propertyInfo->setterMethod()->execute<void, T_PropVT const &>(ownerObject,
                    ZFTypeId<T_PropVT>::template Value<T_PropVT const &>::zfvAccess(zfv)
                );
            ZFTypeId<T_PropVT>::template Value<T_PropVT const &>::zfvAccessFinish(zfv);
            return zftrue;
        }
        else
        {
            _ZFP_propCbDSerializeFrom_errorOccurred(serializableData, outErrorHint, outErrorPos);
            return zffalse;
        }
    }
    else
    {
        return zffalse;
    }
}
template<typename T_PropVT>
ZFPropertyCallbackSerializeFrom _ZFP_propCbDSerializeFrom_get(void)
{
    if(_ZFP_ZFTypeId_propCbSerialize<T_PropVT>::f())
    {
        return _ZFP_ZFTypeId_propCbSerialize<T_PropVT>::f();
    }
    else
    {
        return _ZFP_propCbDSerializeFrom<T_PropVT>;
    }
}

extern ZF_ENV_EXPORT zfbool _ZFP_propCbDSerializeTo_generic(ZF_IN const ZFProperty *propertyInfo,
                                                            ZF_IN ZFObject *ownerObject,
                                                            ZF_OUT ZFSerializableData &serializableData,
                                                            ZF_OUT_OPT zfstring *outErrorHint = zfnull);
extern ZF_ENV_EXPORT zfbool _ZFP_propCbDSerializeTo_impl(ZF_IN const ZFProperty *propertyInfo,
                                                         ZF_IN ZFObject *zfv,
                                                         ZF_OUT ZFSerializableData &serializableData,
                                                         ZF_OUT_OPT zfstring *outErrorHint = zfnull);
extern ZF_ENV_EXPORT void _ZFP_propCbDSerializeTo_errorOccurred(ZF_IN const ZFSerializableData &serializableData,
                                                                ZF_OUT_OPT zfstring *outErrorHint = zfnull);
template<typename T_PropVT>
zfbool _ZFP_propCbDSerializeTo(ZF_IN const ZFProperty *propertyInfo,
                               ZF_IN ZFObject *ownerObject,
                               ZF_OUT ZFSerializableData &serializableData,
                               ZF_OUT_OPT zfstring *outErrorHint = zfnull)
{
    zfautoObject zfv;
    if(ZFTypeId<T_PropVT>::ValueStore(zfv, propertyInfo->getterMethod()->execute<T_PropVT const &>(ownerObject)))
    {
        return _ZFP_propCbDSerializeTo_impl(propertyInfo, zfv, serializableData, outErrorHint);
    }
    else
    {
        _ZFP_propCbDSerializeTo_errorOccurred(serializableData, outErrorHint);
        return zffalse;
    }
}
template<typename T_PropVT>
ZFPropertyCallbackSerializeTo _ZFP_propCbDSerializeTo_get(void)
{
    if(_ZFP_ZFTypeId_propCbSerialize<T_PropVT>::t())
    {
        return _ZFP_ZFTypeId_propCbSerialize<T_PropVT>::t();
    }
    else
    {
        return _ZFP_propCbDSerializeTo<T_PropVT>;
    }
}

// ============================================================
template<typename T_Type, typename T_TypeFix = void, typename T_ReservedFix = void>
zfclassNotPOD _ZFP_ZFPropertyProgressHolder
{
public:
    static zfbool update(ZF_IN_OUT_OPT void *ret = zfnull,
                         ZF_IN_OPT const void *from = zfnull,
                         ZF_IN_OPT const void *to = zfnull,
                         ZF_IN_OPT zffloat progress = 1)
    {
        return zffalse;
    }
};
template<typename T_PropHT, typename T_PropVT>
inline zfbool _ZFP_propCbDProgressUpdate(ZF_IN const ZFProperty *property,
                                         ZF_IN ZFObject *ownerObj,
                                         ZF_IN_OPT const void *from = zfnull,
                                         ZF_IN_OPT const void *to = zfnull,
                                         ZF_IN_OPT zffloat progress = 1)
{
    if(from == zfnull)
    {
        return _ZFP_ZFPropertyProgressHolder<T_PropVT>::update();
    }
    else
    {
        T_PropHT v = T_PropHT();
        if(_ZFP_ZFPropertyProgressHolder<T_PropVT>::update(&v, from, to, progress))
        {
            property->setterMethod()->_ZFP_execute<void, T_PropVT const &>(ownerObj, v);
            return zftrue;
        }
        else
        {
            return zffalse;
        }
    }
}
template<typename T_PropVT>
inline zfbool _ZFP_PropTypeProgressUpdate(ZF_IN_OUT T_PropVT &zfv,
                                          ZF_IN_OPT const void *from = zfnull,
                                          ZF_IN_OPT const void *to = zfnull,
                                          ZF_IN_OPT zffloat progress = 1)
{
    if(from == zfnull)
    {
        return _ZFP_ZFPropertyProgressHolder<T_PropVT>::update();
    }
    else
    {
        return _ZFP_ZFPropertyProgressHolder<T_PropVT>::update(&zfv, from, to, progress);
    }
}
extern ZF_ENV_EXPORT zfbool _ZFP_propCbDProgressUpdate_generic(ZF_IN const ZFProperty *property,
                                                               ZF_IN ZFObject *ownerObj,
                                                               ZF_IN_OPT const void *from /* = zfnull */,
                                                               ZF_IN_OPT const void *to /* = zfnull */,
                                                               ZF_IN_OPT zffloat progress /* = 1 */);

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFPropertyCallbackDefaultImpl_h_

