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
 * @file ZFTypeIdCore.h
 * @brief reflectable type define
 */

#ifndef _ZFI_ZFTypeIdCore_h_
#define _ZFI_ZFTypeIdCore_h_

#include "ZFStyleable.h"
#include "ZFMethodUserRegister.h"
#include "ZFMethodFuncUserRegister.h"
#include "ZFMethodFuncDeclare.h"
#include "ZFObjectUtil.h"
#include "ZFSerializableUtil.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief register a type
 *
 * usually for implementation use only,
 * ZFFramework would supply most of the types,
 * however you may use this to register your own types\n
 * \n
 * typical code to register a type:
 * @code
 *   // ============================================================
 *   // in h file
 *   // declare your type, name must be unique
 *   / **
 *    * add your Doxygen docs here
 *    * /
 *   ZFTYPEID_DECLARE(YourType, YourType)
 *
 *   // ============================================================
 *   // in cpp file
 *   // register your type
 *   ZFTYPEID_DEFINE(YourType, YourType, {
 *           // serializeFrom callback, proto type:
 *           //   zfbool YourTypeFromData(
 *           //       ZF_OUT YourType &v,
 *           //       ZF_IN const ZFSerializableData &serializableData,
 *           //       ZF_OUT_OPT zfstring *outErrorHint = zfnull,
 *           //       ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull);
 *           // you should:
 *           //   * check whether the type match the serializableData's type
 *           //   * serialize from serializableData
 *       }, {
 *           // serializeTo callback, proto type:
 *           //   zfbool YourTypeToData(
 *           //       ZF_OUT ZFSerializableData &serializableData,
 *           //       ZF_IN YourType const &v,
 *           //       ZF_OUT_OPT zfstring *outErrorHint = zfnull);
 *           // you should:
 *           //   * save type name to the serializableData
 *           //   * save type value to the serializableData
 *       }, {
 *           // convertFromString callback, proto type:
 *           //   zfbool YourTypeFromString(
 *           //       ZF_OUT YourType &v,
 *           //       ZF_IN const zfchar *src,
 *           //       ZF_IN_OPT zfindex srcLen = zfindexMax());
 *       }, {
 *           // convertToString callbackk, proto type:
 *           //   zfbool YourTypeToString(
 *           //       ZF_IN_OUT zfstring &s,
 *           //       ZF_IN YourType const &v);
 *       })
 *
 *   // or, you may use #ZFTYPEID_DEFINE_BY_STRING_CONVERTER for short,
 *   // if your type support converter from/to string
 *   ZFTYPEID_DEFINE_BY_STRING_CONVERTER(YourType, YourType, YourTypeFromString, YourTypeToString)
 *
 *   // or, you may use #ZFTYPEID_DEFINE_BY_SERIALIZABLE_CONVERTER for short,
 *   // if your type support converter from/to ZFSerializableData
 *   ZFTYPEID_DEFINE_BY_SERIALIZABLE_CONVERTER(YourType, YourType, YourTypeFromData, YourTypeToData)
 * @endcode
 *
 * once registered, your type can be used as #ZFPROPERTY_ASSIGN
 * which benefits from the powerful automatic serialization logic,
 * or, can be used as reflectable param or return type for #ZFMETHOD_INLINE_0\n
 * \n
 * \n
 * ADVANCED:\n
 * -  a type can be used as #ZFPROPERTY_ASSIGN only if declared by #ZFTYPEID_DECLARE
 * -  no inheritance support for #ZFTYPEID_DECLARE
 * -  for aliased type, you may use #ZFTYPEID_ALIAS_DECLARE
 */
#define ZFTYPEID_DECLARE(TypeName, Type) \
    ZFTYPEID_DECLARE_WITH_CUSTOM_WRAPPER(TypeName, Type) \
    _ZFP_ZFTYPEID_ID_DATA_DECLARE(TypeName, Type)

/** @brief see #ZFTYPEID_DECLARE */
#define ZFTYPEID_DECLARE_WITH_CUSTOM_WRAPPER(TypeName, Type) \
    /** \n */ \
    inline const zfchar *ZFTypeId_##TypeName(void) \
    { \
        return ZFM_TOSTRING_DIRECT(TypeName); \
    } \
    /** @brief see #ZFTYPEID_DECLARE */ \
    extern ZF_ENV_EXPORT zfbool TypeName##FromData(ZF_OUT Type &v, \
                                                   ZF_IN const ZFSerializableData &serializableData, \
                                                   ZF_OUT_OPT zfstring *outErrorHint = zfnull, \
                                                   ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull); \
    /** @brief see #ZFTYPEID_DECLARE */ \
    inline Type TypeName##FromData(ZF_IN const ZFSerializableData &serializableData, \
                                   ZF_OUT_OPT zfstring *outErrorHint = zfnull, \
                                   ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull) \
    { \
        Type ret; \
        if(TypeName##FromData(ret, serializableData, outErrorHint, outErrorPos)) \
        { \
            return ret; \
        } \
        else \
        { \
            typedef Type _Type; \
            return _Type();\
        } \
        return ret; \
    } \
    /** @brief see #ZFTYPEID_DECLARE */ \
    extern ZF_ENV_EXPORT zfbool TypeName##ToData(ZF_OUT ZFSerializableData &serializableData, \
                                                 ZF_IN Type const &v, \
                                                 ZF_OUT_OPT zfstring *outErrorHint = zfnull); \
    /** @brief see #ZFTYPEID_DECLARE */ \
    inline ZFSerializableData TypeName##ToData(ZF_IN Type const &v, \
                                               ZF_OUT_OPT zfstring *outErrorHint = zfnull) \
    { \
        ZFSerializableData ret; \
        if(TypeName##ToData(ret, v, outErrorHint)) \
        { \
            return ret; \
        } \
        else \
        { \
            return ZFSerializableData(); \
        } \
    } \
    ZFCORETYPE_STRING_CONVERTER_DECLARE(TypeName, Type)

/** @brief see #ZFTYPEID_DECLARE */
#define ZFTYPEID_DEFINE(TypeName, Type, serializeFromAction, serializeToAction, convertFromStringAction, convertToStringAction) \
    ZFTYPEID_DEFINE_WITH_CUSTOM_WRAPPER(TypeName, Type, ZFM_EXPAND(serializeFromAction), ZFM_EXPAND(serializeToAction), ZFM_EXPAND(convertFromStringAction), ZFM_EXPAND(convertToStringAction)) \
    _ZFP_ZFTYPEID_ID_DATA_DEFINE(TypeName, Type, ZFM_EXPAND(convertFromStringAction), ZFM_EXPAND(convertToStringAction))

/** @brief see #ZFTYPEID_DECLARE */
#define ZFTYPEID_DEFINE_WITH_CUSTOM_WRAPPER(TypeName, Type, serializeFromAction, serializeToAction, convertFromStringAction, convertToStringAction) \
    template<> \
    zfclassNotPOD _ZFP_ZFTypeId_propCbSerialize<Type> \
    { \
    public: \
        static ZFPropertyCallbackSerializeFrom f(void) \
        { \
            return serializeFrom; \
        } \
        static ZFPropertyCallbackSerializeTo t(void) \
        { \
            return serializeTo; \
        } \
    public: \
        static zfbool serializeFrom(ZF_IN const ZFProperty *propertyInfo, \
                                    ZF_IN ZFObject *ownerObject, \
                                    ZF_IN const ZFSerializableData &serializableData, \
                                    ZF_OUT_OPT zfstring *outErrorHint = zfnull, \
                                    ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull) \
        { \
            if(ZFSerializableUtil::requireSerializableClass(propertyInfo->propertyTypeId(), serializableData, outErrorHint, outErrorPos) == zfnull) \
            { \
                return zffalse; \
            } \
            Type v; \
            if(!TypeName##FromData(v, serializableData, outErrorHint, outErrorPos)) \
            { \
                return zffalse; \
            } \
            propertyInfo->setterMethod()->execute<void, Type const &>(ownerObject, v); \
            return zftrue; \
        } \
        static zfbool serializeTo(ZF_IN const ZFProperty *propertyInfo, \
                                  ZF_IN ZFObject *ownerObject, \
                                  ZF_OUT ZFSerializableData &serializableData, \
                                  ZF_OUT_OPT zfstring *outErrorHint = zfnull) \
        { \
            serializableData.itemClassSet(propertyInfo->propertyTypeId()); \
            serializableData.propertyNameSet(propertyInfo->propertyName()); \
            Type const &v = propertyInfo->getterMethod()->execute<Type const &>(ownerObject); \
            return TypeName##ToData(serializableData, v, outErrorHint); \
        } \
    }; \
    zfbool TypeName##FromData(ZF_OUT Type &v, \
                              ZF_IN const ZFSerializableData &serializableData, \
                              ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */, \
                              ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */) \
    { \
        ZFM_EXPAND(serializeFromAction) \
    } \
    zfbool TypeName##ToData(ZF_OUT ZFSerializableData &serializableData, \
                            ZF_IN Type const &v, \
                            ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */) \
    { \
        ZFM_EXPAND(serializeToAction) \
    } \
    ZFCORETYPE_STRING_CONVERTER_DEFINE(TypeName, Type, ZFM_EXPAND(convertFromStringAction), ZFM_EXPAND(convertToStringAction)) \
    _ZFP_ZFTYPEID_DEFINE_METHOD_REGISTER(TypeName, Type) \
    ZFTYPEID_ID_DATA_REGISTER(TypeName, Type)

#define _ZFP_ZFTYPEID_DEFINE_METHOD_REGISTER(TypeName, Type) \
    ZF_STATIC_REGISTER_INIT(PropMtdReg_##TypeName) \
    { \
        ZFMethodFuncUserRegister_4(method_FromSerializable, { \
                return TypeName##FromData(v, serializableData, outErrorHint, outErrorPos); \
            }, zfbool, TypeName##FromData \
            , ZFMP_OUT(Type &, v) \
            , ZFMP_IN(const ZFSerializableData &, serializableData) \
            , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull) \
            , ZFMP_OUT_OPT(ZFSerializableData *, outErrorPos, zfnull) \
            ); \
        _method_FromSerializable = method_FromSerializable; \
        ZFMethodFuncUserRegister_3(method_ToSerializable, { \
                return TypeName##ToData(serializableData, v, outErrorHint); \
            }, zfbool, TypeName##ToData \
            , ZFMP_OUT(ZFSerializableData &, serializableData) \
            , ZFMP_IN(Type const &, v) \
            , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull) \
            ); \
        _method_ToSerializable = method_ToSerializable; \
        ZFMethodFuncUserRegister_2(method_ToSerializable2, { \
                return TypeName##ToData(v, outErrorHint); \
            }, ZFSerializableData, TypeName##ToData \
            , ZFMP_IN(Type const &, v) \
            , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull) \
            ); \
        _method_ToSerializable2 = method_ToSerializable2; \
        ZFMethodFuncUserRegister_3(method_FromString, { \
                return TypeName##FromString(v, src, srcLen); \
            }, zfbool, TypeName##FromString \
            , ZFMP_OUT(Type &, v) \
            , ZFMP_IN(const zfchar *, src) \
            , ZFMP_OUT_OPT(zfindex, srcLen, zfindexMax()) \
            ); \
        _method_FromString = method_FromString; \
        ZFMethodFuncUserRegister_2(method_ToString, { \
                return TypeName##ToString(s, v); \
            }, zfbool, TypeName##ToString \
            , ZFMP_OUT(zfstring &, s) \
            , ZFMP_IN(Type const &, v) \
            ); \
        _method_ToString = method_ToString; \
        ZFMethodFuncUserRegister_1(method_ToString2, { \
                return TypeName##ToString(v); \
            }, zfstring, TypeName##ToString \
            , ZFMP_IN(Type const &, v) \
            ); \
        _method_ToString2 = method_ToString2; \
    } \
    ZF_STATIC_REGISTER_DESTROY(PropMtdReg_##TypeName) \
    { \
        ZFMethodFuncUserUnregister(_method_FromSerializable); \
        ZFMethodFuncUserUnregister(_method_ToSerializable); \
        ZFMethodFuncUserUnregister(_method_ToSerializable2); \
        ZFMethodFuncUserUnregister(_method_FromString); \
        ZFMethodFuncUserUnregister(_method_ToString); \
        ZFMethodFuncUserUnregister(_method_ToString2); \
    } \
    public: \
        const ZFMethod *_method_FromSerializable; \
        const ZFMethod *_method_ToSerializable; \
        const ZFMethod *_method_ToSerializable2; \
        const ZFMethod *_method_FromString; \
        const ZFMethod *_method_ToString; \
        const ZFMethod *_method_ToString2; \
    ZF_STATIC_REGISTER_END(PropMtdReg_##TypeName)

/** @brief see #ZFTYPEID_DECLARE */
#define ZFTYPEID_DEFINE_BY_STRING_CONVERTER(TypeName, Type, convertFromStringAction, convertToStringAction) \
    ZFTYPEID_DEFINE_BY_STRING_CONVERTER_WITH_CUSTOM_WRAPPER(TypeName, Type, ZFM_EXPAND(convertFromStringAction), ZFM_EXPAND(convertToStringAction)) \
    _ZFP_ZFTYPEID_ID_DATA_DEFINE(TypeName, Type, ZFM_EXPAND(convertFromStringAction), ZFM_EXPAND(convertToStringAction))

/** @brief see #ZFTYPEID_DECLARE */
#define ZFTYPEID_DEFINE_BY_STRING_CONVERTER_WITH_CUSTOM_WRAPPER(TypeName, Type, convertFromStringAction, convertToStringAction) \
    ZFTYPEID_DEFINE_WITH_CUSTOM_WRAPPER(TypeName, Type, { \
        if(ZFSerializableUtil::requireSerializableClass(ZFTypeId_##TypeName(), serializableData, outErrorHint, outErrorPos) == zfnull) \
        { \
            return zffalse; \
        } \
        const zfchar *valueString = ZFSerializableUtil::checkPropertyValue(serializableData); \
        if(valueString == zfnull) \
        { \
            typedef Type _Type; \
            v = _Type(); \
            return zftrue; \
        } \
        if(!TypeName##FromString(v, valueString)) \
        { \
            ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData, \
                zfText("invalid value: \"%s\""), valueString); \
            return zffalse; \
        } \
        serializableData.resolveMark(); \
        return zftrue; \
    }, { \
        serializableData.itemClassSet(ZFTypeId_##TypeName()); \
        zfstring s; \
        if(!TypeName##ToString(s, v)) \
        { \
            ZFSerializableUtil::errorOccurred(outErrorHint, \
                zfText("unable to convert value to string")); \
            return zffalse; \
        } \
        serializableData.propertyValueSet(s.isEmpty() ? zfnull : s.cString()); \
        return zftrue; \
    }, ZFM_EXPAND(convertFromStringAction), ZFM_EXPAND(convertToStringAction))

/** @brief see #ZFTYPEID_DECLARE */
#define ZFTYPEID_DEFINE_BY_SERIALIZABLE_CONVERTER(TypeName, Type, serializeFromAction, serializeToAction) \
    ZFTYPEID_DEFINE_BY_SERIALIZABLE_CONVERTER_WITH_CUSTOM_WRAPPER(TypeName, Type, ZFM_EXPAND(serializeFromAction), ZFM_EXPAND(serializeToAction)) \
    _ZFP_ZFTYPEID_ID_DATA_DEFINE(TypeName, Type, ZFM_EXPAND(convertFromStringAction), ZFM_EXPAND(convertToStringAction))

/** @brief see #ZFTYPEID_DECLARE */
#define ZFTYPEID_DEFINE_BY_SERIALIZABLE_CONVERTER_WITH_CUSTOM_WRAPPER(TypeName, Type, serializeFromAction, serializeToAction) \
    ZFTYPEID_DEFINE_WITH_CUSTOM_WRAPPER(TypeName, Type, ZFM_EXPAND(serializeFromAction), ZFM_EXPAND(serializeToAction), { \
        ZFSerializableData serializableData; \
        return (ZFSerializableDataFromString(serializableData, src, srcLen) \
            && TypeName##FromData(v, serializableData)); \
    }, { \
        ZFSerializableData serializableData; \
        if(TypeName##ToData(serializableData, v)) \
        { \
            return ZFSerializableDataToString(s, serializableData); \
        } \
        else \
        { \
            return zffalse; \
        } \
    })

// ============================================================
/**
 * @brief declare a type that reflectable and accessable, but not serializable
 *
 * by default, all return types and method types in ZFMethod
 * must be registered by #ZFTYPEID_DECLARE
 * for advanced reflection logic\n
 * if you simply want basic method declaration,
 * you may use this macro to disable your type explicitly\n
 * see #ZFTYPEID_DECLARE for more info
 */
#define ZFTYPEID_ACCESS_ONLY_DECLARE(TypeName, Type) \
    /** \n */ \
    inline const zfchar *ZFTypeId_##TypeName(void) \
    { \
        return ZFM_TOSTRING_DIRECT(TypeName); \
    } \
    _ZFP_ZFTYPEID_ID_DATA_ACCESS_ONLY_DECLARE(TypeName, Type)
/** @brief see #ZFTYPEID_ACCESS_ONLY_DECLARE */
#define ZFTYPEID_ACCESS_ONLY_DEFINE(TypeName, Type) \
    _ZFP_ZFTYPEID_ID_DATA_ACCESS_ONLY_DEFINE(TypeName, Type) \
    ZFTYPEID_ID_DATA_REGISTER(TypeName, Type)
/** @brief see #ZFTYPEID_ACCESS_ONLY_DECLARE */
#define ZFTYPEID_ACCESS_ONLY_DEFINE_UNCOMPARABLE(TypeName, Type) \
    _ZFP_ZFTYPEID_ID_DATA_ACCESS_ONLY_DEFINE_UNCOMPARABLE(TypeName, Type) \
    ZFTYPEID_ID_DATA_REGISTER(TypeName, Type)

// ============================================================
/**
 * @brief alias an existing type to another type, see #ZFTYPEID_DECLARE
 *
 * the original type must be declared by #ZFTYPEID_DECLARE or #ZFTYPEID_ACCESS_ONLY_DECLARE\n
 * the newly aliased type would share same type id,
 * serializable converter and string converter,
 * with the original exist one\n
 * see #ZFTYPEID_DECLARE for more info
 */
#define ZFTYPEID_ALIAS_DECLARE(AliasToTypeName, AliasToType, TypeName, Type) \
    /** @brief see @ref ZFTypeId_##AliasToTypeName */ \
    inline const zfchar *ZFTypeId_##TypeName(void) \
    { \
        return ZFTypeId_##AliasToTypeName(); \
    } \
    _ZFP_ZFTYPEID_ID_DATA_ALIAS_DECLARE(AliasToTypeName, AliasToType, TypeName, Type)
/** @brief see #ZFTYPEID_ALIAS_DECLARE */
#define ZFTYPEID_ALIAS_DEFINE(AliasToTypeName, AliasToType, TypeName, Type) \
    _ZFP_ZFTYPEID_ID_DATA_ALIAS_DEFINE(AliasToTypeName, AliasToType, TypeName, Type)

// ============================================================
/**
 * @brief explicitly mark the type can not be reflected when used in #ZFMETHOD_INLINE_0 or #ZFPROPERTY_ASSIGN
 *
 * by default, all return types and method types in ZFMethod
 * must be registered by #ZFTYPEID_DECLARE
 * for advanced reflection logic\n
 * if you simply want basic method declaration,
 * you may use this macro to disable your type explicitly\n
 * see #ZFTYPEID_DECLARE for more info
 */
#define ZFTYPEID_DISABLE(Type) \
    _ZFP_ZFTYPEID_ID_DATA_DISABLE(Type)

// ============================================================
/**
 * @brief dummy base for all wrapper types,
 *   see #ZFTypeId::Value
 *
 * serializable data:
 * @code
 *   <WrapperTypeClass
 *       value="string content" // optional, empty by default
 *   />
 * @endcode
 */
zfabstract ZF_ENV_EXPORT ZFTypeIdWrapper : zfextends ZFStyleableObject
{
    ZFOBJECT_DECLARE_ABSTRACT_WITH_CUSTOM_CTOR(ZFTypeIdWrapper, ZFStyleableObject)

public:
    /**
     * @brief whether the holded value should not be changed
     */
    zfbool wrappedValueIsConst;
    /**
     * @brief util to mark #wrappedValueIsConst
     */
    static void markConst(ZF_IN_OUT_OPT ZFObject *obj)
    {
        zfself *t = ZFCastZFObject(zfself *, obj);
        if(t != zfnull)
        {
            t->wrappedValueIsConst = zftrue;
        }
    }

/** @cond ZFPrivateDoc */
protected:
    ZFTypeIdWrapper(void) : wrappedValueIsConst(zffalse) {}
/** @endcond */

public:
    /**
     * @brief copy internal value, assert fail if #wrappedValueIsConst
     */
    zffinal ZFTypeIdWrapper *assign(ZF_IN ZFTypeIdWrapper *ref)
    {
        zfCoreAssert(!this->wrappedValueIsConst);
        this->assignAction(ref);
        return this;
    }
protected:
    /**
     * @brief see #assign
     */
    virtual void assignAction(ZF_IN ZFTypeIdWrapper *ref) zfpurevirtual;

public:
    /**
     * @brief type id for this wrapper type
     */
    virtual const zfchar *wrappedValueTypeId(void) zfpurevirtual;
    /**
     * @brief access the value's address
     */
    virtual void *wrappedValue(void) zfpurevirtual;
    /**
     * @brief set the value, no type safe check
     */
    virtual void wrappedValueSet(ZF_IN const void *v) zfpurevirtual;
    /**
     * @brief get the value, no type safe check
     */
    virtual void wrappedValueGet(ZF_IN void *v) zfpurevirtual;

public:
    /**
     * @brief reset the value to it's init value
     */
    virtual void wrappedValueReset(void) zfpurevirtual;
    /**
     * @brief true if the value is in init value state
     */
    virtual zfbool wrappedValueIsInit(void) zfpurevirtual;
    /**
     * @brief compare the value
     */
    virtual ZFCompareResult wrappedValueCompare(ZF_IN const void *v0,
                                                ZF_IN const void *v1) zfpurevirtual;
    /**
     * @brief get value info
     */
    virtual void wrappedValueGetInfo(ZF_IN_OUT zfstring &ret,
                                     ZF_IN const void *v) zfpurevirtual;
    /**
     * @brief update property value by progress, see #ZFTYPEID_PROGRESS_DECLARE
     */
    virtual zfbool wrappedValueProgressUpdate(ZF_IN const void *from,
                                              ZF_IN const void *to,
                                              ZF_IN zffloat progress) zfpurevirtual;

public:
    /**
     * @brief convert from serializable data
     */
    virtual zfbool wrappedValueFromData(ZF_IN const ZFSerializableData &serializableData,
                                        ZF_OUT_OPT zfstring *outErrorHint = zfnull,
                                        ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull) zfpurevirtual;
    /**
     * @brief convert to serializable data
     */
    virtual zfbool wrappedValueToData(ZF_OUT ZFSerializableData &serializableData,
                                      ZF_OUT_OPT zfstring *outErrorHint = zfnull) zfpurevirtual;
    /**
     * @brief convert from string
     */
    virtual zfbool wrappedValueFromString(ZF_IN const zfchar *src,
                                          ZF_IN_OPT zfindex srcLen = zfindexMax()) zfpurevirtual;
    /**
     * @brief convert to string
     */
    virtual zfbool wrappedValueToString(ZF_IN_OUT zfstring &s) zfpurevirtual;

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

protected:
    zfoverride
    virtual void styleableOnCopyFrom(ZF_IN ZFStyleable *anotherStyleable)
    {
        zfsuper::styleableOnCopyFrom(anotherStyleable);
        this->wrappedValueSet(anotherStyleable->to<ZFTypeIdWrapper *>()->wrappedValue());
    }

protected:
    zfoverride
    virtual zfbool serializableOnSerializeFromData(ZF_IN const ZFSerializableData &serializableData,
                                                   ZF_OUT_OPT zfstring *outErrorHint = zfnull,
                                                   ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull)
    {
        if(!zfsuperI(ZFSerializable)::serializableOnSerializeFromData(serializableData, outErrorHint, outErrorPos)) {return zffalse;}

        const zfchar *valueString = ZFSerializableUtil::checkPropertyValue(serializableData);
        if(valueString == zfnull)
        {
            this->wrappedValueReset();
        }
        else if(!this->wrappedValueFromString(valueString))
        {
            ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
                zfText("failed to convert from \"%s\""), valueString);
            return zffalse;
        }
        return zftrue;
    }
    zfoverride
    virtual zfbool serializableOnSerializeToData(ZF_IN_OUT ZFSerializableData &serializableData,
                                                 ZF_IN ZFSerializable *referencedOwnerOrNull,
                                                 ZF_OUT_OPT zfstring *outErrorHint = zfnull)
    {
        if(!zfsuperI(ZFSerializable)::serializableOnSerializeToData(serializableData, referencedOwnerOrNull, outErrorHint)) {return zffalse;}
        zfself *ref = ZFCastZFObject(zfself *, referencedOwnerOrNull);

        if((ref == zfnull && !this->wrappedValueIsInit())
            || (ref != zfnull && this->objectCompare(ref) != ZFCompareTheSame))
        {
            zfstring valueString;
            if(!this->wrappedValueToString(valueString))
            {
                return zffalse;
            }
            serializableData.propertyValueSet(valueString);
        }

        return zftrue;
    }
};

// ============================================================
/**
 * @brief register Type's progress update logic
 *
 * a progress update means,
 * the type can be changed by timeline by supply a "from/to" value range\n
 * for example, an float type with range "[0.5, 3.0]" and progress "0.3",
 * would result "0.5 + (3.0 - 0.5) * 0.3", which is 1.25\n
 * this is useful to make a property change with animation\n
 * \n
 * to use this, register your type by this macro, with this proto type:
 * @code
 *   void update(ZF_IN_OUT void *ret,
 *               ZF_IN const void *from,
 *               ZF_IN const void *to,
 *               ZF_IN zffloat progress);
 * @endcode
 * then use them by #ZFProperty::callbackProgressUpdate
 */
#define ZFTYPEID_PROGRESS_DECLARE(Type, progressUpdateAction) \
    template<typename T_Type> \
    zfclassNotPOD _ZFP_ZFPropertyProgressHolder< \
            T_Type, \
            typename zftEnableIf<zftTypeIsTypeOf<T_Type, Type>::TypeIsTypeOf>::EnableIf \
        > \
    { \
    public: \
        static zfbool update(ZF_IN_OUT_OPT void *_ret = zfnull, \
                             ZF_IN_OPT const void *_from = zfnull, \
                             ZF_IN_OPT const void *_to = zfnull, \
                             ZF_IN_OPT zffloat progress = 1) \
        { \
            if(_ret == zfnull) {return zftrue;} \
            typedef Type _ZFP_PP; \
            _ZFP_PP &ret = *(_ZFP_PP *)_ret; \
            _ZFP_PP const &from = *(const _ZFP_PP *)_from; \
            _ZFP_PP const &to = *(const _ZFP_PP *)_to; \
            { \
                progressUpdateAction \
            } \
            return zftrue; \
        } \
    };
/**
 * @brief util macro to declare #ZFTYPEID_PROGRESS_DECLARE by raw value calculating
 */
#define ZFTYPEID_PROGRESS_DECLARE_BY_VALUE(Type) \
    ZFTYPEID_PROGRESS_DECLARE(Type, { \
            ret = (Type)(from + (Type)((to - from) * progress)); \
        })

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFTypeIdCore_h_

