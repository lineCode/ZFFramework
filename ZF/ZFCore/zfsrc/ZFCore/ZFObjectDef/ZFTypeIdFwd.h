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
#define ZFTypeId_none() ""

// ============================================================
/** @brief class prefix for subclass of #ZFTypeIdWrapper */
#define ZFTypeIdWrapperPrefix v_
/** @brief see #ZFTypeIdWrapperPrefix */
#define ZFTypeIdWrapperPrefixName "v_"
/** @brief see #ZFTypeIdWrapperPrefix */
#define ZFTypeIdWrapperPrefixNameLen 2 // zfslen("v_")

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
     *   the value would be serialized as #ZFSerializable
     *   according to object class
     * -  if the type is not ZFObject type,
     *   impl should set v to proper holder type (#ZFTypeIdWrapper)
     *   and return true
     */
    virtual zfbool typeIdWrapper(ZF_OUT zfautoObject &v) const zfpurevirtual;
};

// ============================================================
extern ZF_ENV_EXPORT void _ZFP_ZFTypeIdRegister(ZF_IN const zfchar *typeId,
                                                ZF_IN ZFTypeIdBase *typeIdData);
extern ZF_ENV_EXPORT ZFTypeIdBase *_ZFP_ZFTypeIdUnregister(ZF_IN const zfchar *typeId);
/**
 * @brief access type id data
 */
extern ZF_ENV_EXPORT const ZFTypeIdBase *ZFTypeIdGet(ZF_IN const zfchar *typeId);
/** @brief see #ZFTypeIdGetAll */
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
     * after access, we may (not ensured) call #accessFinish
     * to allow impl to do some cleanup steps
     */
    template<typename T_Access = T_Type>
    zfclassNotPOD Value
    {
    public:
        /** @brief try access as raw value, see #ZFTypeId::Value */
        static zfbool accessAvailable(ZF_IN_OUT zfautoObject &obj);
        /** @brief try access as raw value, see #ZFTypeId::Value */
        static T_Access access(ZF_IN_OUT zfautoObject &obj);
        /** @brief finish access as raw value, see #ZFTypeId::Value */
        static void accessFinish(ZF_IN_OUT zfautoObject &obj);
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
 *
 * note, type id can only be registered within global namespace (#ZF_NAMESPACE_GLOBAL)
 */
#define ZFTYPEID_ID_DATA_REGISTER(TypeName, Type) \
    ZF_STATIC_REGISTER_INIT(PropTIReg_##TypeName) \
    { \
        typedef Type _ZFP_PropTypeW2_##TypeName; \
        _ZFP_ZFTypeIdRegister(ZFTypeId_##TypeName(), \
            zfnew(ZFTypeId<_ZFP_PropTypeW2_##TypeName>)); \
        ZFMethodFuncUserRegister_0(dummy, { \
                return ZFTypeId_##TypeName(); \
            }, ZF_NAMESPACE_GLOBAL_NAME, const zfchar *, ZFM_TOSTRING(ZFTypeId_##TypeName)); \
    } \
    ZF_STATIC_REGISTER_DESTROY(PropTIReg_##TypeName) \
    { \
        ZFMethodFuncUserUnregister(ZFMethodFuncGet(zfnull, ZFM_TOSTRING(ZFTypeId_##TypeName))); \
        zfdelete(_ZFP_ZFTypeIdUnregister(ZFTypeId_##TypeName())); \
    } \
    ZF_STATIC_REGISTER_END(PropTIReg_##TypeName)

// ============================================================
#define _ZFP_ZFTYPEID_WRAPPER_DECLARE(TypeName, Type) \
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
                                            ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull); \
        zfoverride \
        virtual zfbool wrappedValueToData(ZF_OUT ZFSerializableData &serializableData, \
                                          ZF_OUT_OPT zfstring *outErrorHint = zfnull); \
        zfoverride \
        virtual zfbool wrappedValueFromString(ZF_IN const zfchar *src, \
                                              ZF_IN_OPT zfindex srcLen = zfindexMax()); \
        zfoverride \
        virtual zfbool wrappedValueToString(ZF_IN_OUT zfstring &s); \
    };

#define _ZFP_ZFTYPEID_WRAPPER_DEFINE_COMMON(TypeName, Type) \
    ZFOBJECT_REGISTER(v_##TypeName) \
    void v_##TypeName::objectInfoOnAppend(ZF_IN_OUT zfstring &ret) \
    { \
        ZFCoreElementInfoGetter<Type>::elementInfoGetter(ret, this->zfv); \
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
    } \
    ZFMETHOD_USER_REGISTER_1({ \
            invokerObject->to<v_##TypeName *>()->zfv = value; \
        }, v_##TypeName, void, zfvSet, \
        ZFMP_IN(_ZFP_PropTypeW_##TypeName const &, value))

#define _ZFP_ZFTYPEID_WRAPPER_DEFINE_SERIALIZABLE(TypeName, Type) \
    zfbool v_##TypeName::wrappedValueFromData(ZF_IN const ZFSerializableData &serializableData, \
                                              ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */, \
                                              ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */) \
    {return TypeName##FromData(this->zfv, serializableData, outErrorHint, outErrorPos);} \
    zfbool v_##TypeName::wrappedValueToData(ZF_OUT ZFSerializableData &serializableData, \
                                            ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */) \
    {return TypeName##ToData(serializableData, this->zfv, outErrorHint);} \
    zfbool v_##TypeName::wrappedValueFromString(ZF_IN const zfchar *src, \
                                                ZF_IN_OPT zfindex srcLen /* = zfindexMax() */) \
    {return TypeName##FromString(this->zfv, src, srcLen);} \
    zfbool v_##TypeName::wrappedValueToString(ZF_IN_OUT zfstring &s) \
    {return TypeName##ToString(s, this->zfv);}

#define _ZFP_ZFTYPEID_WRAPPER_DEFINE_NOT_SERIALIZABLE(TypeName, Type) \
    zfbool v_##TypeName::wrappedValueFromData(ZF_IN const ZFSerializableData &serializableData, \
                                              ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */, \
                                              ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */) \
    { \
        ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData, \
            "registered type %s is not serializable", ZFM_TOSTRING_DIRECT(TypeName)); \
        return zffalse; \
    } \
    zfbool v_##TypeName::wrappedValueToData(ZF_OUT ZFSerializableData &serializableData, \
                                            ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */) \
    { \
        ZFSerializableUtil::errorOccurred(outErrorHint, \
            "registered type %s is not serializable", ZFM_TOSTRING_DIRECT(TypeName)); \
        return zffalse; \
    } \
    zfbool v_##TypeName::wrappedValueFromString(ZF_IN const zfchar *src, \
                                                ZF_IN_OPT zfindex srcLen /* = zfindexMax() */) \
    {return zffalse;} \
    zfbool v_##TypeName::wrappedValueToString(ZF_IN_OUT zfstring &s) \
    {return zffalse;}

#define _ZFP_ZFTYPEID_WRAPPER_DEFINE_COMPARABLE(TypeName, Type) \
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
    zfbool v_##TypeName::wrappedValueIsInit(void) \
    { \
        return (ZFComparerDefault(this->zfv, zftValue<_ZFP_PropTypeW_##TypeName>().zfv) == ZFCompareTheSame); \
    } \
    ZFCompareResult v_##TypeName::wrappedValueCompare(ZF_IN const void *v0, \
                                                      ZF_IN const void *v1) \
    { \
        return ZFComparerDefault(*(const _ZFP_PropTypeW_##TypeName *)v0, *(const _ZFP_PropTypeW_##TypeName *)v1); \
    }

#define _ZFP_ZFTYPEID_WRAPPER_DEFINE_UNCOMPARABLE(TypeName, Type) \
    ZFCompareResult v_##TypeName::objectCompare(ZF_IN ZFObject *anotherObj) \
    { \
        return ZFCompareUncomparable; \
    } \
    zfbool v_##TypeName::wrappedValueIsInit(void) \
    { \
        return zffalse; \
    } \
    ZFCompareResult v_##TypeName::wrappedValueCompare(ZF_IN const void *v0, \
                                                      ZF_IN const void *v1) \
    { \
        return ZFCompareUncomparable; \
    }

// ============================================================
#define _ZFP_ZFTYPEID_DECLARE(TypeName, Type) \
    _ZFP_ZFTYPEID_WRAPPER_DECLARE(TypeName, Type) \
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
            static zfbool accessAvailable(ZF_IN_OUT zfautoObject &obj) \
            { \
                return (ZFCastZFObject(v_##TypeName *, obj) != zfnull); \
            } \
            static T_Access access(ZF_IN_OUT zfautoObject &obj) \
            { \
                return ZFCastZFObject(v_##TypeName *, obj)->zfv; \
            } \
            static void accessFinish(ZF_IN_OUT zfautoObject &obj) \
            { \
            } \
        }; \
        template<typename T_Access> \
        zfclassNotPOD Value<T_Access, 1> \
        { \
        public: \
            static zfbool accessAvailable(ZF_IN_OUT zfautoObject &obj) \
            { \
                return (ZFCastZFObject(v_##TypeName *, obj) != zfnull); \
            } \
            static typename zftTraits<T_Access>::TrNoRef access(ZF_IN_OUT zfautoObject &obj) \
            { \
                return &(ZFCastZFObject(v_##TypeName *, obj)->zfv); \
            } \
            static void accessFinish(ZF_IN_OUT zfautoObject &obj) \
            { \
            } \
        }; \
    }; \
    /** @endcond */

#define _ZFP_ZFTYPEID_DEFINE(TypeName, Type) \
    _ZFP_ZFTYPEID_WRAPPER_DEFINE_COMMON(TypeName, Type) \
    _ZFP_ZFTYPEID_WRAPPER_DEFINE_SERIALIZABLE(TypeName, Type) \
    _ZFP_ZFTYPEID_WRAPPER_DEFINE_COMPARABLE(TypeName, Type)

// ============================================================
#define _ZFP_ZFTYPEID_ACCESS_ONLY_DECLARE(TypeName, Type) \
    _ZFP_ZFTYPEID_WRAPPER_DECLARE(TypeName, Type) \
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
            static zfbool accessAvailable(ZF_IN_OUT zfautoObject &obj) \
            { \
                return (ZFCastZFObject(v_##TypeName *, obj) != zfnull); \
            } \
            static T_Access access(ZF_IN_OUT zfautoObject &obj) \
            { \
                return ZFCastZFObject(v_##TypeName *, obj)->zfv; \
            } \
            static void accessFinish(ZF_IN_OUT zfautoObject &obj) \
            { \
            } \
        }; \
        template<typename T_Access> \
        zfclassNotPOD Value<T_Access, 1> \
        { \
        public: \
            static zfbool accessAvailable(ZF_IN_OUT zfautoObject &obj) \
            { \
                return (ZFCastZFObject(v_##TypeName *, obj) != zfnull); \
            } \
            static T_Access access(ZF_IN_OUT zfautoObject &obj) \
            { \
                return &(ZFCastZFObject(v_##TypeName *, obj)->zfv); \
            } \
            static void accessFinish(ZF_IN_OUT zfautoObject &obj) \
            { \
            } \
        }; \
    }; \
    /** @endcond */

#define _ZFP_ZFTYPEID_ACCESS_ONLY_DEFINE(TypeName, Type) \
    _ZFP_ZFTYPEID_WRAPPER_DEFINE_COMMON(TypeName, Type) \
    _ZFP_ZFTYPEID_WRAPPER_DEFINE_NOT_SERIALIZABLE(TypeName, Type) \
    _ZFP_ZFTYPEID_WRAPPER_DEFINE_COMPARABLE(TypeName, Type)

#define _ZFP_ZFTYPEID_ACCESS_ONLY_DEFINE_UNCOMPARABLE(TypeName, Type) \
    _ZFP_ZFTYPEID_WRAPPER_DEFINE_COMMON(TypeName, Type) \
    _ZFP_ZFTYPEID_WRAPPER_DEFINE_NOT_SERIALIZABLE(TypeName, Type) \
    _ZFP_ZFTYPEID_WRAPPER_DEFINE_UNCOMPARABLE(TypeName, Type)

// ============================================================
#define _ZFP_ZFTYPEID_DISABLE(Type) \
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
            static zfbool accessAvailable(ZF_IN_OUT zfautoObject &obj) \
            { \
                return zffalse; \
            } \
            static typename zftTraits<T_Access>::TrNoRef access(ZF_IN_OUT zfautoObject &obj) \
            { \
                return typename zftTraits<T_Access>::TrNoRef(); \
            } \
            static void accessFinish(ZF_IN_OUT zfautoObject &obj) \
            { \
            } \
        }; \
    }; \
    /** @endcond */

// ============================================================
#define _ZFP_ZFTYPEID_ALIAS_DECLARE(AliasToTypeName, AliasToType, TypeName, Type) \
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
            static zfbool accessAvailable(ZF_IN_OUT zfautoObject &obj) \
            { \
                return (obj != zfnull && ZFTypeId<AliasToType>::Value<AliasToType const &>::accessAvailable(obj)); \
            } \
            static T_Access access(ZF_IN_OUT zfautoObject &obj) \
            { \
                AliasToType const &aliasValue = ZFTypeId<AliasToType>::Value<AliasToType const &>::access(obj); \
                _ZFP_PropTypeW_##TypeName *v = zfnew(_ZFP_PropTypeW_##TypeName); \
                *v = (_ZFP_PropTypeW_##TypeName)aliasValue; \
                _ZFP_PropAliasAttach(obj, v, ZFM_TOSTRING(Type), _ZFP_PropAliasOnDetach); \
                return *v; \
            } \
            static void accessFinish(ZF_IN_OUT zfautoObject &obj) \
            { \
                _ZFP_PropAliasDetach(obj, ZFM_TOSTRING(Type)); \
            } \
        private: \
            static void _ZFP_PropAliasOnDetach(ZF_IN ZFObject *obj, \
                                               ZF_IN void *v) \
            { \
                zfautoObject objTmp = obj; \
                _ZFP_PropTypeW_##TypeName *vTmp = (_ZFP_PropTypeW_##TypeName *)v; \
                if(ZFTypeId<AliasToType>::Value<AliasToType &>::accessAvailable(objTmp)) \
                { \
                    AliasToType &aliasValue = ZFTypeId<AliasToType>::Value<AliasToType &>::access(objTmp); \
                    aliasValue = (AliasToType)*vTmp; \
                    ZFTypeId<AliasToType>::Value<AliasToType &>::accessFinish(objTmp); \
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
            static zfbool accessAvailable(ZF_IN_OUT zfautoObject &obj) \
            { \
                return (obj != zfnull && ZFTypeId<AliasToType>::Value<AliasToType const &>::accessAvailable(obj)); \
            } \
            static T_Access access(ZF_IN_OUT zfautoObject &obj) \
            { \
                AliasToType const &aliasValue = ZFTypeId<AliasToType>::Value<AliasToType const &>::access(obj); \
                _ZFP_PropTypeW_##TypeName *v = zfnew(_ZFP_PropTypeW_##TypeName); \
                *v = (_ZFP_PropTypeW_##TypeName)aliasValue; \
                _TrNoRef *p = zfnew(_TrNoRef); \
                *p = v; \
                _ZFP_PropAliasAttach(obj, p, ZFM_TOSTRING(Type), _ZFP_PropAliasOnDetach); \
                return *p; \
            } \
            static void accessFinish(ZF_IN_OUT zfautoObject &obj) \
            { \
                _ZFP_PropAliasDetach(obj, ZFM_TOSTRING(Type)); \
            } \
        private: \
            static void _ZFP_PropAliasOnDetach(ZF_IN ZFObject *obj, \
                                               ZF_IN void *v) \
            { \
                zfautoObject objTmp = obj; \
                _TrNoRef *p = (_TrNoRef *)v; \
                _ZFP_PropTypeW_##TypeName *vTmp = (_ZFP_PropTypeW_##TypeName *)*p; \
                if(ZFTypeId<AliasToType>::Value<AliasToType &>::accessAvailable(objTmp)) \
                { \
                    AliasToType &aliasValue = ZFTypeId<AliasToType>::Value<AliasToType &>::access(objTmp); \
                    aliasValue = (AliasToType)*vTmp; \
                    ZFTypeId<AliasToType>::Value<AliasToType &>::accessFinish(objTmp); \
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
#define _ZFP_ZFTYPEID_ALIAS_DEFINE(AliasToTypeName, AliasToType, TypeName, Type) \
    ZFOBJECT_REGISTER(v_##TypeName)

// ============================================================
// special alias implicit convert
typedef void (*_ZFP_PropAliasDetachCallback)(ZF_IN ZFObject *obj,
                                             ZF_IN void *v);
extern ZF_ENV_EXPORT void _ZFP_PropAliasAttach(ZF_IN ZFObject *obj,
                                               ZF_IN void *v,
                                               ZF_IN const zfchar *typeName,
                                               ZF_IN _ZFP_PropAliasDetachCallback detachCallback);
extern ZF_ENV_EXPORT void _ZFP_PropAliasDetach(ZF_IN ZFObject *obj,
                                               ZF_IN const zfchar *typeName);

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

