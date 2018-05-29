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
 * @file ZFTypeIdFwd.h
 * @brief reflectable type define
 */

#ifndef _ZFI_ZFTypeIdFwd_h_
#define _ZFI_ZFTypeIdFwd_h_

#include "zfautoObjectFwd.h"
#include "ZFAny.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief shows that the property has no type and can not be used in advanced property copy operation,
 *   ensured empty string
 */
#define ZFTypeId_none() zfText("")
/**
 * @brief type id for retain property, see #ZFProperty::propertyTypeId
 */
#define ZFTypeId_ZFObject() zfText("ZFObject")

// ============================================================
zfclassFwd ZFSerializableData;
/**
 * @brief base protocol for #ZFTypeId
 */
zfclassNotPOD ZF_ENV_EXPORT ZFTypeIdBase
{
public:
    virtual ~ZFTypeIdBase(void) {}

public:
    /**
     * @brief see #ZFTypeId::TypeIdSerializable
     */
    virtual zfbool typeIdSerializable(void) const zfpurevirtual;
    /**
     * @brief the type id
     */
    virtual const zfchar *typeId(void) const zfpurevirtual;
    /**
     * @brief return the proper wrapper type if available
     *
     * if available:
     * -  if the type is ZFObject type, set v to #zfautoObjectNull and return true,
     *   the value would be serialized by #ZFObjectFromString series later
     * -  if the type is not ZFObject type,
     *   impl should set v to proper holder type (#ZFTypeIdWrapper)
     *   and return true
     */
    virtual zfbool typeIdWrapper(ZF_OUT zfautoObject &v) const zfpurevirtual;
};

// ============================================================
extern ZF_ENV_EXPORT void _ZFP_ZFTypeIdRegister(ZF_IN const zfchar *typeId,
                                                ZF_IN ZFTypeIdBase *typeIdData);
extern ZF_ENV_EXPORT void _ZFP_ZFTypeIdUnregister(ZF_IN const zfchar *typeId);
/**
 * @brief access type id data
 */
extern ZF_ENV_EXPORT const ZFTypeIdBase *ZFTypeIdGet(ZF_IN const zfchar *typeId);
/**
 * @brief access type id data
 */
extern ZF_ENV_EXPORT void ZFTypeIdGetAllT(ZF_OUT ZFCoreArray<const ZFTypeIdBase *> &ret);
/**
 * @brief access type id data
 */
inline ZFCoreArrayPOD<const ZFTypeIdBase *> ZFTypeIdGetAll(void)
{
    ZFCoreArrayPOD<const ZFTypeIdBase *> ret;
    ZFTypeIdGetAllT(ret);
    return ret;
}

// ============================================================
template<typename T_Dummy>
zfclassNotPOD _ZFP_ZFTypeIdRegChecker
{
};
/**
 * @brief type data traits for #ZFTYPEID_DECLARE
 *
 * for advanced reflection use only\n
 * type must be registered by #ZFTYPEID_DECLARE before use this template,
 * and T_Type must be plain type without reference,
 * use #zftTraits::TrNoRef if necessary
 */
template<typename T_Type, typename T_ZFObjectFix = void, typename T_PointerFix = void>
zfclassNotPOD ZFTypeId : zfextendsNotPOD ZFTypeIdBase
{
public:
    /** @cond ZFPrivateDoc */
    typedef typename _ZFP_ZFTypeIdRegChecker<T_Type>::AllTypeMustBeRegisteredBy_ZFTYPEID_XXX _TypeChecker;
    /** @endcond */
public:
    enum {
        /**
         * @brief whether this type already registered
         *
         * type is registered if declared by any of these:
         * -  #ZFTYPEID_DECLARE
         * -  #ZFTYPEID_ACCESS_ONLY_DECLARE
         * -  #ZFTYPEID_ALIAS_DECLARE
         * -  #ZFTYPEID_DISABLE
         */
        TypeIdRegistered = 0,
        /**
         * @brief whether this type can be serialized
         *
         * type is serializable if declared by any of these:
         * -  #ZFTYPEID_DECLARE
         * -  #ZFTYPEID_ALIAS_DECLARE
         */
        TypeIdSerializable = 0,
    };

    /**
     * @brief get type id, or return #ZFTypeId_none if not registered
     */
    static const zfchar *TypeId(void);

    /**
     * @brief store the value to wrapper object
     *
     * the stored value can be further accessed by #ZFTypeId::Value
     */
    static zfbool ValueStore(ZF_OUT zfautoObject &obj, ZF_IN T_Type const &v);

    /**
     * @brief try access as raw value
     *
     * this method used to convert types from/to ZFObject without knowing its actual type,
     * currently used by #ZFMethod::methodGenericInvoker\n
     * \n
     * types must be registered by #ZFTYPEID_DECLARE or #ZFTYPEID_ALIAS_DECLARE,
     * which would automatically declare a wrapper object type for you\n
     * assume your type's name is "YourType",
     * then there would be a wrapper class named "v_YourType",
     * with only one plain member variable named "zfv":
     * @code
     *   zfclass v_YourType : zfextends ZFTypeIdWrapper
     *   {
     *       ZFOBJECT_DECLARE(v_YourType, ZFTypeIdWrapper)
     *   public:
     *       YourType zfv;
     *   };
     * @endcode
     * \n
     * this method may or may not access the original value\n
     * must first check whether it's available to access
     * by #ZFTypeId::Value::accessAvailable\n
     */
    template<typename T_Access = T_Type>
    zfclassNotPOD Value
    {
    public:
        /** @brief try access as raw value, see #ZFTypeId::Value */
        static zfbool accessAvailable(ZF_IN ZFObject *obj);
        /** @brief try access as raw value, see #ZFTypeId::Value */
        static T_Access access(ZF_IN ZFObject *obj);
    };
    /*
     * if available, the templates above should handle these types,
     * and access as proper type:
     * -  Type
     * -  Type &
     * -  Type const &
     * -  Type *
     * -  Type * &
     * -  const Type *
     * -  const Type * &
     * -  Type * const &
     * -  const Type * const &
     */
};

// ============================================================
/**
 * @brief register a custom type
 *
 * by default, all of your type must be registered by #ZFTYPEID_DECLARE series,
 * for some special case,
 * you may need to register your type manually,
 * to achieve this, you must:
 * -  specialize template #ZFTypeId
 * -  use this macro to register your type
 */
#define ZFTYPEID_ID_DATA_REGISTER(TypeName, Type) \
    ZF_STATIC_REGISTER_INIT(PropTIReg_##TypeName) \
    { \
        typedef Type _ZFP_PropTypeW2_##TypeName; \
        _ZFP_ZFTypeIdRegister(ZFTypeId_##TypeName(), \
            zfnew(ZFTypeId<_ZFP_PropTypeW2_##TypeName>)); \
        ZFMethodFuncUserRegister_0(dummy, { \
                return ZFTypeId_##TypeName(); \
            }, const zfchar *, ZFTypeId_##TypeName); \
    } \
    ZF_STATIC_REGISTER_DESTROY(PropTIReg_##TypeName) \
    { \
        ZFMethodFuncUserUnregister(ZFMethodFuncGet(zfnull, ZFM_TOSTRING(ZFTypeId_##TypeName))); \
        _ZFP_ZFTypeIdUnregister(ZFTypeId_##TypeName()); \
    } \
    ZF_STATIC_REGISTER_END(PropTIReg_##TypeName)

// ============================================================
#define _ZFP_ZFTYPEID_ID_DATA_DECLARE(TypeName, Type) \
    typedef Type _ZFP_PropTypeW_##TypeName; \
    /** @brief type wrapper for #ZFTypeId::Value */ \
    zfclass ZF_ENV_EXPORT v_##TypeName : zfextends ZFTypeIdWrapper \
    { \
        ZFOBJECT_DECLARE_WITH_CUSTOM_CTOR(v_##TypeName, ZFTypeIdWrapper) \
    public: \
        /** @brief the value, see #ZFTypeId::Value */ \
        _ZFP_PropTypeW_##TypeName zfv; \
    protected: \
        v_##TypeName(void) : zfv() {} \
    protected: \
        /** @brief init with value */ \
        virtual void objectOnInit(ZF_IN _ZFP_PropTypeW_##TypeName const &value) \
        { \
            this->objectOnInit(); \
            this->zfv = value; \
        } \
        zfoverride \
        virtual void objectOnInit(void) {zfsuper::objectOnInit();} \
        zfoverride \
        virtual inline void objectInfoOnAppendTokenLeft(ZF_IN_OUT zfstring &ret) {} \
        zfoverride \
        virtual inline void objectInfoOnAppendTokenRight(ZF_IN_OUT zfstring &ret) {} \
        zfoverride \
        virtual void objectInfoOnAppend(ZF_IN_OUT zfstring &ret); \
    public: \
        zfoverride \
        virtual ZFCompareResult objectCompare(ZF_IN ZFObject *anotherObj); \
    public: \
        zfoverride \
        virtual void assignAction(ZF_IN ZFTypeIdWrapper *ref); \
        zfoverride \
        virtual const zfchar *wrappedValueTypeId(void); \
        zfoverride \
        virtual void *wrappedValue(void) {return &(this->zfv);} \
        zfoverride \
        virtual void wrappedValueSet(ZF_IN const void *v) {this->zfv = *(const _ZFP_PropTypeW_##TypeName *)v;} \
        zfoverride \
        virtual void wrappedValueGet(ZF_IN void *v) {*(_ZFP_PropTypeW_##TypeName *)v = this->zfv;} \
    public: \
        zfoverride \
        virtual void wrappedValueReset(void) \
        {this->zfv = zftValue<_ZFP_PropTypeW_##TypeName>().zfv;} \
        zfoverride \
        virtual zfbool wrappedValueIsInit(void); \
        zfoverride \
        virtual ZFCompareResult wrappedValueCompare(ZF_IN const void *v0, \
                                                    ZF_IN const void *v1); \
        zfoverride \
        virtual void wrappedValueGetInfo(ZF_IN_OUT zfstring &ret, \
                                         ZF_IN const void *v); \
        zfoverride \
        virtual zfbool wrappedValueProgressUpdate(ZF_IN const void *from, \
                                                  ZF_IN const void *to, \
                                                  ZF_IN zffloat progress); \
    public: \
        zfoverride \
        virtual zfbool wrappedValueFromData(ZF_IN const ZFSerializableData &serializableData, \
                                            ZF_OUT_OPT zfstring *outErrorHint = zfnull, \
                                            ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull) \
        {return TypeName##FromData(this->zfv, serializableData, outErrorHint, outErrorPos);} \
        zfoverride \
        virtual zfbool wrappedValueToData(ZF_OUT ZFSerializableData &serializableData, \
                                          ZF_OUT_OPT zfstring *outErrorHint = zfnull) \
        {return TypeName##ToData(serializableData, this->zfv, outErrorHint);} \
        zfoverride \
        virtual zfbool wrappedValueFromString(ZF_IN const zfchar *src, \
                                              ZF_IN_OPT zfindex srcLen = zfindexMax()) \
        {return TypeName##FromString(this->zfv, src, srcLen);} \
        zfoverride \
        virtual zfbool wrappedValueToString(ZF_IN_OUT zfstring &s) \
        {return TypeName##ToString(s, this->zfv);} \
    }; \
    /** @cond ZFPrivateDoc */ \
    template<> \
    zfclassNotPOD ZFTypeId<_ZFP_PropTypeW_##TypeName> : zfextendsNotPOD ZFTypeIdBase \
    { \
    public: \
        enum { \
            TypeIdRegistered = 1, \
            TypeIdSerializable = 1, \
        }; \
        static inline const zfchar *TypeId(void) \
        { \
            return ZFTypeId_##TypeName(); \
        } \
        zfoverride \
        virtual zfbool typeIdSerializable(void) const \
        { \
            return TypeIdSerializable; \
        } \
        zfoverride \
        virtual const zfchar *typeId(void) const \
        { \
            return TypeId(); \
        } \
        zfoverride \
        virtual zfbool typeIdWrapper(ZF_OUT zfautoObject &v) const \
        { \
            v_##TypeName *t = zfAlloc(v_##TypeName); \
            v = t; \
            zfRelease(t); \
            return zftrue; \
        } \
        static zfbool ValueStore(ZF_OUT zfautoObject &obj, ZF_IN _ZFP_PropTypeW_##TypeName const &v) \
        { \
            v_##TypeName *t = zfAlloc(v_##TypeName, v); \
            obj = t; \
            zfRelease(t); \
            return zftrue; \
        } \
        template<typename T_Access = _ZFP_PropTypeW_##TypeName \
            , int T_IsPointer = ((zftTraits<typename zftTraits<T_Access>::TrNoRef>::TrIsPtr \
                && zftTypeIsSame< \
                        typename zftTraits<T_Access>::TrNoRef, \
                        _ZFP_PropTypeW_##TypeName \
                    >::TypeIsSame != 1) \
                ? 1 : 0) \
            , typename T_Fix = void \
            > \
        zfclassNotPOD Value \
        { \
        public: \
            static zfbool accessAvailable(ZF_IN ZFObject *obj) \
            { \
                return (ZFCastZFObject(v_##TypeName *, obj) != zfnull); \
            } \
            static T_Access access(ZF_IN ZFObject *obj) \
            { \
                return ZFCastZFObject(v_##TypeName *, obj)->zfv; \
            } \
        }; \
        template<typename T_Access> \
        zfclassNotPOD Value<T_Access, 1> \
        { \
        public: \
            static zfbool accessAvailable(ZF_IN ZFObject *obj) \
            { \
                return (ZFCastZFObject(v_##TypeName *, obj) != zfnull); \
            } \
            static typename zftTraits<T_Access>::TrNoRef access(ZF_IN ZFObject *obj) \
            { \
                return &(ZFCastZFObject(v_##TypeName *, obj)->zfv); \
            } \
        }; \
    }; \
    /** @endcond */

#define _ZFP_ZFTYPEID_ID_DATA_DEFINE(TypeName, Type, convertFromStringAction, convertToStringAction) \
    ZFOBJECT_REGISTER(v_##TypeName) \
    void v_##TypeName::objectInfoOnAppend(ZF_IN_OUT zfstring &ret) \
    { \
        ZFCoreElementInfoGetter<Type>::elementInfoGetter(ret, this->zfv); \
    } \
    ZFCompareResult v_##TypeName::objectCompare(ZF_IN ZFObject *anotherObj) \
    { \
        ZFTypeIdWrapper *t = ZFCastZFObject(ZFTypeIdWrapper *, anotherObj); \
        if(t == zfnull || !zfscmpTheSame(this->wrappedValueTypeId(), t->wrappedValueTypeId())) \
        { \
            return ZFCompareUncomparable; \
        } \
        else \
        { \
            return ZFComparerDefault(this->zfv, *(_ZFP_PropTypeW_##TypeName *)t->wrappedValue()); \
        } \
    } \
    void v_##TypeName::assignAction(ZF_IN ZFTypeIdWrapper *ref) \
    { \
        zfself *refTmp = ZFCastZFObject(zfself *, ref); \
        if(refTmp != zfnull) \
        { \
            this->zfv = refTmp->zfv; \
        } \
    } \
    const zfchar *v_##TypeName::wrappedValueTypeId(void) \
    { \
        return ZFTypeId<_ZFP_PropTypeW_##TypeName>::TypeId(); \
    } \
    zfbool v_##TypeName::wrappedValueIsInit(void) \
    { \
        return (ZFComparerDefault(this->zfv, zftValue<_ZFP_PropTypeW_##TypeName>().zfv) == ZFCompareTheSame); \
    } \
    ZFCompareResult v_##TypeName::wrappedValueCompare(ZF_IN const void *v0, \
                                                      ZF_IN const void *v1) \
    { \
        return ZFComparerDefault(*(const _ZFP_PropTypeW_##TypeName *)v0, *(const _ZFP_PropTypeW_##TypeName *)v1); \
    } \
    void v_##TypeName::wrappedValueGetInfo(ZF_IN_OUT zfstring &ret, \
                                           ZF_IN const void *v) \
    { \
        ZFCoreElementInfoGetter<_ZFP_PropTypeW_##TypeName>::elementInfoGetter(ret, *(const _ZFP_PropTypeW_##TypeName *)v); \
    } \
    zfbool v_##TypeName::wrappedValueProgressUpdate(ZF_IN const void *from, \
                                                    ZF_IN const void *to, \
                                                    ZF_IN zffloat progress) \
    { \
        return _ZFP_PropTypeProgressUpdate<_ZFP_PropTypeW_##TypeName>(this->zfv, from, to, progress); \
    }

// ============================================================
#define _ZFP_ZFTYPEID_ID_DATA_ACCESS_ONLY_DECLARE(TypeName, Type) \
    typedef Type _ZFP_PropTypeW_##TypeName; \
    /** @brief type wrapper for #ZFTypeId::Value */ \
    zfclass ZF_ENV_EXPORT v_##TypeName : zfextends ZFTypeIdWrapper \
    { \
        ZFOBJECT_DECLARE_WITH_CUSTOM_CTOR(v_##TypeName, ZFTypeIdWrapper) \
    public: \
        /** @brief the value, see #ZFTypeId::Value */ \
        _ZFP_PropTypeW_##TypeName zfv; \
    protected: \
        v_##TypeName(void) : zfv() {} \
    protected: \
        /** @brief init with value */ \
        virtual void objectOnInit(ZF_IN _ZFP_PropTypeW_##TypeName const &value) \
        { \
            this->objectOnInit(); \
            this->zfv = value; \
        } \
        zfoverride \
        virtual void objectOnInit(void) {zfsuper::objectOnInit();} \
        zfoverride \
        virtual inline void objectInfoOnAppendTokenLeft(ZF_IN_OUT zfstring &ret) {} \
        zfoverride \
        virtual inline void objectInfoOnAppendTokenRight(ZF_IN_OUT zfstring &ret) {} \
        zfoverride \
        virtual void objectInfoOnAppend(ZF_IN_OUT zfstring &ret); \
    public: \
        zfoverride \
        virtual ZFCompareResult objectCompare(ZF_IN ZFObject *anotherObj); \
    public: \
        zfoverride \
        virtual void assignAction(ZF_IN ZFTypeIdWrapper *ref); \
        zfoverride \
        virtual const zfchar *wrappedValueTypeId(void); \
        zfoverride \
        virtual void *wrappedValue(void) {return &(this->zfv);} \
        zfoverride \
        virtual void wrappedValueSet(ZF_IN const void *v) {this->zfv = *(const _ZFP_PropTypeW_##TypeName *)v;} \
        zfoverride \
        virtual void wrappedValueGet(ZF_IN void *v) {*(_ZFP_PropTypeW_##TypeName *)v = this->zfv;} \
    public: \
        zfoverride \
        virtual void wrappedValueReset(void) \
        {this->zfv = zftValue<_ZFP_PropTypeW_##TypeName>().zfv;} \
        zfoverride \
        virtual zfbool wrappedValueIsInit(void); \
        zfoverride \
        virtual ZFCompareResult wrappedValueCompare(ZF_IN const void *v0, \
                                                    ZF_IN const void *v1); \
        zfoverride \
        virtual void wrappedValueGetInfo(ZF_IN_OUT zfstring &ret, \
                                         ZF_IN const void *v); \
        zfoverride \
        virtual zfbool wrappedValueProgressUpdate(ZF_IN const void *from, \
                                                  ZF_IN const void *to, \
                                                  ZF_IN zffloat progress); \
    public: \
        zfoverride \
        virtual zfbool wrappedValueFromData(ZF_IN const ZFSerializableData &serializableData, \
                                            ZF_OUT_OPT zfstring *outErrorHint = zfnull, \
                                            ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull) \
        { \
            ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData, \
                zfText("registered type %s is not serializable"), ZFM_TOSTRING_DIRECT(TypeName)); \
            return zffalse; \
        } \
        zfoverride \
        virtual zfbool wrappedValueToData(ZF_OUT ZFSerializableData &serializableData, \
                                          ZF_OUT_OPT zfstring *outErrorHint = zfnull) \
        { \
            ZFSerializableUtil::errorOccurred(outErrorHint, \
                zfText("registered type %s is not serializable"), ZFM_TOSTRING_DIRECT(TypeName)); \
            return zffalse; \
        } \
        zfoverride \
        virtual zfbool wrappedValueFromString(ZF_IN const zfchar *src, \
                                              ZF_IN_OPT zfindex srcLen = zfindexMax()) \
        {return zffalse;} \
        zfoverride \
        virtual zfbool wrappedValueToString(ZF_IN_OUT zfstring &s) \
        {return zffalse;} \
    }; \
    /** @cond ZFPrivateDoc */ \
    template<> \
    zfclassNotPOD ZFTypeId<_ZFP_PropTypeW_##TypeName> : zfextendsNotPOD ZFTypeIdBase \
    { \
    public: \
        enum { \
            TypeIdRegistered = 1, \
            TypeIdSerializable = 0, \
        }; \
        static inline const zfchar *TypeId(void) \
        { \
            return ZFTypeId_##TypeName(); \
        } \
        zfoverride \
        virtual zfbool typeIdSerializable(void) const \
        { \
            return TypeIdSerializable; \
        } \
        zfoverride \
        virtual const zfchar *typeId(void) const \
        { \
            return TypeId(); \
        } \
        zfoverride \
        virtual zfbool typeIdWrapper(ZF_OUT zfautoObject &v) const \
        { \
            v_##TypeName *t = zfAlloc(v_##TypeName); \
            v = t; \
            zfRelease(t); \
            return zftrue; \
        } \
        static zfbool ValueStore(ZF_OUT zfautoObject &obj, ZF_IN _ZFP_PropTypeW_##TypeName const &v) \
        { \
            v_##TypeName *t = zfAlloc(v_##TypeName, v); \
            obj = t; \
            zfRelease(t); \
            return zftrue; \
        } \
        template<typename T_Access = _ZFP_PropTypeW_##TypeName \
            , int T_IsPointer = ((zftTraits<typename zftTraits<T_Access>::TrNoRef>::TrIsPtr \
                && zftTypeIsSame< \
                        typename zftTraits<T_Access>::TrNoRef, \
                        _ZFP_PropTypeW_##TypeName \
                    >::TypeIsSame != 1) \
                ? 1 : 0) \
            , typename T_Fix = void \
            > \
        zfclassNotPOD Value \
        { \
        public: \
            static zfbool accessAvailable(ZF_IN ZFObject *obj) \
            { \
                return (ZFCastZFObject(v_##TypeName *, obj) != zfnull); \
            } \
            static T_Access access(ZF_IN ZFObject *obj) \
            { \
                return ZFCastZFObject(v_##TypeName *, obj)->zfv; \
            } \
        }; \
        template<typename T_Access> \
        zfclassNotPOD Value<T_Access, 1> \
        { \
        public: \
            static zfbool accessAvailable(ZF_IN ZFObject *obj) \
            { \
                return (ZFCastZFObject(v_##TypeName *, obj) != zfnull); \
            } \
            static typename zftTraits<T_Access>::TrNoRef access(ZF_IN ZFObject *obj) \
            { \
                return &(ZFCastZFObject(v_##TypeName *, obj)->zfv); \
            } \
        }; \
    }; \
    /** @endcond */

#define _ZFP_ZFTYPEID_ID_DATA_ACCESS_ONLY_DEFINE(TypeName, Type) \
    ZFOBJECT_REGISTER(v_##TypeName) \
    void v_##TypeName::objectInfoOnAppend(ZF_IN_OUT zfstring &ret) \
    { \
        ZFCoreElementInfoGetter<Type>::elementInfoGetter(ret, this->zfv); \
    } \
    ZFCompareResult v_##TypeName::objectCompare(ZF_IN ZFObject *anotherObj) \
    { \
        ZFTypeIdWrapper *t = ZFCastZFObject(ZFTypeIdWrapper *, anotherObj); \
        if(t == zfnull || !zfscmpTheSame(this->wrappedValueTypeId(), t->wrappedValueTypeId())) \
        { \
            return ZFCompareUncomparable; \
        } \
        else \
        { \
            return ZFComparerDefault(this->zfv, *(_ZFP_PropTypeW_##TypeName *)t->wrappedValue()); \
        } \
    } \
    void v_##TypeName::assignAction(ZF_IN ZFTypeIdWrapper *ref) \
    { \
        zfself *refTmp = ZFCastZFObject(zfself *, ref); \
        if(refTmp != zfnull) \
        { \
            this->zfv = refTmp->zfv; \
        } \
    } \
    const zfchar *v_##TypeName::wrappedValueTypeId(void) \
    { \
        return ZFTypeId<_ZFP_PropTypeW_##TypeName>::TypeId(); \
    } \
    zfbool v_##TypeName::wrappedValueIsInit(void) \
    { \
        return (ZFComparerDefault(this->zfv, zftValue<_ZFP_PropTypeW_##TypeName>().zfv) == ZFCompareTheSame); \
    } \
    ZFCompareResult v_##TypeName::wrappedValueCompare(ZF_IN const void *v0, \
                                                      ZF_IN const void *v1) \
    { \
        return ZFComparerDefault(*(const _ZFP_PropTypeW_##TypeName *)v0, *(const _ZFP_PropTypeW_##TypeName *)v1); \
    } \
    void v_##TypeName::wrappedValueGetInfo(ZF_IN_OUT zfstring &ret, \
                                           ZF_IN const void *v) \
    { \
        ZFCoreElementInfoGetter<_ZFP_PropTypeW_##TypeName>::elementInfoGetter(ret, *(const _ZFP_PropTypeW_##TypeName *)v); \
    } \
    zfbool v_##TypeName::wrappedValueProgressUpdate(ZF_IN const void *from, \
                                                    ZF_IN const void *to, \
                                                    ZF_IN zffloat progress) \
    { \
        return _ZFP_PropTypeProgressUpdate<_ZFP_PropTypeW_##TypeName>(this->zfv, from, to, progress); \
    }

#define _ZFP_ZFTYPEID_ID_DATA_ACCESS_ONLY_DEFINE_UNCOMPARABLE(TypeName, Type) \
    ZFOBJECT_REGISTER(v_##TypeName) \
    void v_##TypeName::objectInfoOnAppend(ZF_IN_OUT zfstring &ret) \
    { \
        ZFCoreElementInfoGetter<Type>::elementInfoGetter(ret, this->zfv); \
    } \
    ZFCompareResult v_##TypeName::objectCompare(ZF_IN ZFObject *anotherObj) \
    { \
        return ZFCompareUncomparable; \
    } \
    void v_##TypeName::assignAction(ZF_IN ZFTypeIdWrapper *ref) \
    { \
        zfself *refTmp = ZFCastZFObject(zfself *, ref); \
        if(refTmp != zfnull) \
        { \
            this->zfv = refTmp->zfv; \
        } \
    } \
    const zfchar *v_##TypeName::wrappedValueTypeId(void) \
    { \
        return ZFTypeId<_ZFP_PropTypeW_##TypeName>::TypeId(); \
    } \
    zfbool v_##TypeName::wrappedValueIsInit(void) \
    { \
        return zffalse; \
    } \
    ZFCompareResult v_##TypeName::wrappedValueCompare(ZF_IN const void *v0, \
                                                      ZF_IN const void *v1) \
    { \
        return ZFCompareUncomparable; \
    } \
    void v_##TypeName::wrappedValueGetInfo(ZF_IN_OUT zfstring &ret, \
                                           ZF_IN const void *v) \
    { \
        ZFCoreElementInfoGetter<_ZFP_PropTypeW_##TypeName>::elementInfoGetter(ret, *(const _ZFP_PropTypeW_##TypeName *)v); \
    } \
    zfbool v_##TypeName::wrappedValueProgressUpdate(ZF_IN const void *from, \
                                                    ZF_IN const void *to, \
                                                    ZF_IN zffloat progress) \
    { \
        return _ZFP_PropTypeProgressUpdate<_ZFP_PropTypeW_##TypeName>(this->zfv, from, to, progress); \
    }

// ============================================================
#define _ZFP_ZFTYPEID_ID_DATA_DISABLE(Type) \
    typedef Type _ZFP_PropTypeW_##TypeName; \
    /** @cond ZFPrivateDoc */ \
    template<> \
    zfclassNotPOD ZFTypeId<Type> : zfextendsNotPOD ZFTypeIdBase \
    { \
    public: \
        enum { \
            TypeIdRegistered = 1, \
            TypeIdSerializable = 0, \
        }; \
        static inline const zfchar *TypeId(void) \
        { \
            return ZFTypeId_none(); \
        } \
        static zfbool ValueStore(ZF_OUT zfautoObject &obj, ZF_IN _ZFP_PropTypeW_##TypeName const &v) \
        { \
            return zffalse; \
        } \
        template<typename T_Access = Type> \
        zfclassNotPOD Value \
        { \
        public: \
            static zfbool accessAvailable(ZF_IN ZFObject *obj) \
            { \
                return zffalse; \
            } \
            static typename zftTraits<T_Access>::TrNoRef access(ZF_IN ZFObject *obj) \
            { \
                return typename zftTraits<T_Access>::TrNoRef(); \
            } \
        }; \
    }; \
    /** @endcond */

// ============================================================
#define _ZFP_ZFTYPEID_ID_DATA_ALIAS_DECLARE(AliasToTypeName, AliasToType, TypeName, Type) \
    /** @cond ZFPrivateDoc */ \
    typedef Type _ZFP_PropTypeW_##TypeName; \
    template<> \
    zfclassNotPOD ZFTypeId<Type> : zfextendsNotPOD ZFTypeIdBase \
    { \
    public: \
        enum { \
            TypeIdRegistered = ZFTypeId<AliasToType>::TypeIdRegistered, \
            TypeIdSerializable = ZFTypeId<AliasToType>::TypeIdSerializable, \
        }; \
        static inline const zfchar *TypeId(void) \
        { \
            return ZFTypeId<AliasToType>::TypeId(); \
        } \
        zfoverride \
        virtual zfbool typeIdSerializable(void) const \
        { \
            return TypeIdSerializable; \
        } \
        zfoverride \
        virtual const zfchar *typeId(void) const \
        { \
            return TypeId(); \
        } \
        zfoverride \
        virtual zfbool typeIdWrapper(ZF_OUT zfautoObject &v) const \
        { \
            ZFTypeId<AliasToType> t; \
            t.typeIdWrapper(v); \
            return zftrue; \
        } \
        static zfbool ValueStore(ZF_OUT zfautoObject &obj, ZF_IN _ZFP_PropTypeW_##TypeName const &v) \
        { \
            return ZFTypeId<AliasToType>::ValueStore(obj, (AliasToType)v); \
        } \
        template<typename T_Access = _ZFP_PropTypeW_##TypeName \
            , int T_IsPointer = ((zftTraits<typename zftTraits<T_Access>::TrNoRef>::TrIsPtr \
                && zftTypeIsSame< \
                        typename zftTraits<T_Access>::TrNoRef, \
                        _ZFP_PropTypeW_##TypeName \
                    >::TypeIsSame != 1) \
                ? 1 : 0) \
            , typename T_Fix = void \
            > \
        zfclassNotPOD Value \
        { \
        public: \
            static zfbool accessAvailable(ZF_IN ZFObject *obj) \
            { \
                return (obj != zfnull && ZFTypeId<AliasToType>::Value<AliasToType const &>::accessAvailable(obj)); \
            } \
            static T_Access access(ZF_IN ZFObject *obj) \
            { \
                AliasToType const &aliasValue = ZFTypeId<AliasToType>::Value<AliasToType const &>::access(obj); \
                _ZFP_PropTypeW_##TypeName *v = zfnew(_ZFP_PropTypeW_##TypeName); \
                *v = (_ZFP_PropTypeW_##TypeName)aliasValue; \
                _ZFP_PropAliasAttach(obj, v, ZFM_TOSTRING(Type), _ZFP_PropAliasOnDetach); \
                return *v; \
            } \
        private: \
            static void _ZFP_PropAliasOnDetach(ZF_IN ZFObject *obj, \
                                               ZF_IN void *v) \
            { \
                _ZFP_PropTypeW_##TypeName *vTmp = (_ZFP_PropTypeW_##TypeName *)v; \
                if(ZFTypeId<AliasToType>::Value<AliasToType &>::accessAvailable(obj)) \
                { \
                    AliasToType &aliasValue = ZFTypeId<AliasToType>::Value<AliasToType &>::access(obj); \
                    aliasValue = (AliasToType)*vTmp; \
                } \
                zfdelete(vTmp); \
            } \
        }; \
        template<typename T_Access> \
        zfclassNotPOD Value<T_Access, 1> \
        { \
        private: \
             typedef typename zftTraits<T_Access>::TrNoRef _TrNoRef; \
        public: \
            static zfbool accessAvailable(ZF_IN ZFObject *obj) \
            { \
                return (obj != zfnull && ZFTypeId<AliasToType>::Value<AliasToType const &>::accessAvailable(obj)); \
            } \
            static T_Access access(ZF_IN ZFObject *obj) \
            { \
                AliasToType const &aliasValue = ZFTypeId<AliasToType>::Value<AliasToType const &>::access(obj); \
                _ZFP_PropTypeW_##TypeName *v = zfnew(_ZFP_PropTypeW_##TypeName); \
                *v = (_ZFP_PropTypeW_##TypeName)aliasValue; \
                _TrNoRef *p = zfnew(_TrNoRef); \
                *p = v; \
                _ZFP_PropAliasAttach(obj, p, ZFM_TOSTRING(Type), _ZFP_PropAliasOnDetach); \
                return *p; \
            } \
        private: \
            static void _ZFP_PropAliasOnDetach(ZF_IN ZFObject *obj, \
                                               ZF_IN void *v) \
            { \
                _TrNoRef *p = (_TrNoRef *)v; \
                _ZFP_PropTypeW_##TypeName *vTmp = (_ZFP_PropTypeW_##TypeName *)*p; \
                if(ZFTypeId<AliasToType>::Value<AliasToType &>::accessAvailable(obj)) \
                { \
                    AliasToType &aliasValue = ZFTypeId<AliasToType>::Value<AliasToType &>::access(obj); \
                    aliasValue = (AliasToType)*vTmp; \
                } \
                zfdelete(vTmp); \
                zfdelete(p); \
            } \
        }; \
    }; \
    /** @endcond */ \
    /** @brief type wrapper for #ZFTypeId::Value */ \
    zfclass ZF_ENV_EXPORT v_##TypeName : zfextends v_##AliasToTypeName \
    { \
        ZFOBJECT_DECLARE(v_##TypeName, v_##AliasToTypeName) \
    };
#define _ZFP_ZFTYPEID_ID_DATA_ALIAS_DEFINE(AliasToTypeName, AliasToType, TypeName, Type) \
    ZFOBJECT_REGISTER(v_##TypeName)

// ============================================================
// special alias implicit convert
typedef void (*_ZFP_PropAliasDetachCallback)(ZF_IN ZFObject *obj,
                                             ZF_IN void *v);
extern ZF_ENV_EXPORT void _ZFP_PropAliasAttach(ZF_IN ZFObject *obj,
                                               ZF_IN void *v,
                                               ZF_IN const zfchar *typeName,
                                               ZF_IN _ZFP_PropAliasDetachCallback detachCallback);

extern ZF_ENV_EXPORT void _ZFP_ZFTypeIdWrapperMarkConst(ZF_IN_OUT_OPT ZFObject *zfv);
template<typename T_Type, int isConst = (zffalse
        || zftTraits<T_Type>::TrModifier == zftTraitsModifier_R
        || zftTraits<T_Type>::TrModifier == zftTraitsModifier_P
        || zftTraits<T_Type>::TrModifier == zftTraitsModifier_PR
        || zftTraits<T_Type>::TrModifier == zftTraitsModifier_PCR
        || zftTraits<T_Type>::TrModifier == zftTraitsModifier_CPR
        || zftTraits<T_Type>::TrModifier == zftTraitsModifier_CPR
    ) ? 1 : 0>
zfclassNotPOD _ZFP_ZFTypeIdWrapperMarkConstCheck
{
public:
    static inline void a(ZF_IN_OUT_OPT ZFObject *zfv)
    {
    }
};
template<typename T_Type>
zfclassNotPOD _ZFP_ZFTypeIdWrapperMarkConstCheck<T_Type, 1>
{
public:
    static inline void a(ZF_IN_OUT_OPT ZFObject *zfv)
    {
        _ZFP_ZFTypeIdWrapperMarkConst(zfv);
    }
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFTypeIdFwd_h_

