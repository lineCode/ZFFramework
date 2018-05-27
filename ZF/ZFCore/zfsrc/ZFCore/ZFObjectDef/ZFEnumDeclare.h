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
 * @file ZFEnumDeclare.h
 * @brief ZFEnum declare impl
 */

#ifndef _ZFI_ZFEnumDeclare_h_
#define _ZFI_ZFEnumDeclare_h_

#include "ZFTypeId.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassFwd _ZFP_ZFEnumDataPrivate;
zfclassNotPOD ZF_ENV_EXPORT _ZFP_ZFEnumData
{
public:
    _ZFP_ZFEnumData(void);
    ~_ZFP_ZFEnumData(void);
public:
    zfbool needInitFlag;
    const ZFClass *ownerClass;
    zfuint enumDefault;
    zfbool enumIsFlags;
    void add(ZF_IN zfbool isEnableDuplicateValue,
             ZF_IN zfuint value,
             ZF_IN const zfchar *name);
    zfindex enumCount(void) const;
    zfindex enumIndexForValue(ZF_IN zfuint value) const;
    zfuint enumValueAtIndex(ZF_IN zfindex index) const;
    const zfchar *enumNameAtIndex(ZF_IN zfindex index) const;
    zfbool enumContainValue(ZF_IN zfuint value) const;
    zfuint enumValueForName(ZF_IN const zfchar *name) const;
    const zfchar *enumNameForValue(ZF_IN zfuint value) const;
private:
    _ZFP_ZFEnumDataPrivate *d;
};
extern ZF_ENV_EXPORT _ZFP_ZFEnumData *_ZFP_ZFEnumDataAccess(ZF_IN const ZFClass *ownerClass);
extern ZF_ENV_EXPORT void _ZFP_ZFEnumDataCleanup(ZF_IN const ZFClass *ownerClass);
extern ZF_ENV_EXPORT void _ZFP_ZFEnumDataCleanup(ZF_IN const _ZFP_ZFEnumData *d);
zfclassNotPOD _ZFP_ZFEnumDataHolder
{
public:
    _ZFP_ZFEnumDataHolder(ZF_IN const _ZFP_ZFEnumData *d)
    : d(d)
    {
    }
    _ZFP_ZFEnumDataHolder(void)
    {
        _ZFP_ZFEnumDataCleanup(d);
    }
public:
    const _ZFP_ZFEnumData *d;
};

// ============================================================
#define _ZFP_ZFENUM_BEGIN(EnumName) \
    zfclass ZF_ENV_EXPORT EnumName : zfextends ZFEnum \
    { \
        ZFOBJECT_DECLARE(EnumName, ZFEnum) \
    public: \
        /** @brief see @ref EnumName */ \
        typedef enum \
        {

#define _ZFP_ZFENUM_VALUE(Value) /** \n */ e_##Value,
#define _ZFP_ZFENUM_VALUE_WITH_INIT(Value, initValue) /** @brief \n Value(initValue)\n */ e_##Value = initValue,

#define _ZFP_ZFENUM_SEPARATOR(EnumName, isEnableDuplicateValue_) \
            /* ensure sizeof(enum) == sizeof(zfuint) */ \
            /* required for enum value reinterpret cast (EnumReinterpretCast) */ \
            _ZFP_ZFEnumMax = ((zfuint)-1), \
        } ZFEnumType; \
    public: \
        /** @brief see #ZFObject::objectHash */ \
        zfoverride \
        virtual zfidentity objectHash(void) \
        { \
            return zfidentityHash( \
                zfidentityCalcString(zfself::ClassData()->className()), \
                zfidentityCalcPOD(this->enumValue())); \
        } \
    public: \
        /** @brief get the count of enum value */ \
        static zfindex EnumCount(void) \
        { \
            return zfself::_ZFP_ZFEnumDataRef()->enumCount(); \
        } \
        /** @brief get the index of enum value, or zfindexMax() if no such enum value */ \
        static zfindex EnumIndexForValue(ZF_IN zfuint value) \
        { \
            return zfself::_ZFP_ZFEnumDataRef()->enumIndexForValue(value); \
        } \
        /** @brief get the value at index, or ZFEnumInvalid if not exist */ \
        static zfuint EnumValueAtIndex(ZF_IN zfindex index) \
        { \
            return zfself::_ZFP_ZFEnumDataRef()->enumValueAtIndex(index); \
        } \
        /** @brief get the name at index, or ZFEnumNameInvalid if not exist */ \
        static const zfchar *EnumNameAtIndex(ZF_IN zfindex index) \
        { \
            return zfself::_ZFP_ZFEnumDataRef()->enumNameAtIndex(index); \
        } \
        /** @brief return true if contain the specified value */ \
        static zfbool EnumContainValue(ZF_IN zfuint value) \
        { \
            return zfself::_ZFP_ZFEnumDataRef()->enumContainValue(value); \
        } \
        /** @brief get value with name, or ZFEnumInvalid if not exist */ \
        static zfuint EnumValueForName(ZF_IN const zfchar *name) \
        { \
            return zfself::_ZFP_ZFEnumDataRef()->enumValueForName(name); \
        } \
        /** @brief get name with value, or ZFEnumNameInvalid if not exist */ \
        static const zfchar *EnumNameForValue(ZF_IN zfuint value) \
        { \
            return zfself::_ZFP_ZFEnumDataRef()->enumNameForValue(value); \
        } \
    public: \
        zfoverride \
        virtual zfindex enumCount(void) \
        { \
            return zfself::EnumCount(); \
        } \
        zfoverride \
        virtual zfuint enumDefault(void) \
        { \
            return zfself::EnumDefault(); \
        } \
        zfoverride \
        virtual zfbool enumIsFlags(void) \
        { \
            return zfself::EnumIsFlags(); \
        } \
        zfoverride \
        virtual zfindex enumIndexForValue(ZF_IN zfuint value) \
        { \
            return zfself::EnumIndexForValue(value); \
        } \
        zfoverride \
        virtual zfuint enumValueAtIndex(ZF_IN zfindex index) \
        { \
            return zfself::EnumValueAtIndex(index); \
        } \
        zfoverride \
        virtual const zfchar *enumNameAtIndex(ZF_IN zfindex index) \
        { \
            return zfself::EnumNameAtIndex(index); \
        } \
        zfoverride \
        virtual zfbool enumContainValue(ZF_IN zfuint value) \
        { \
            return zfself::EnumContainValue(value); \
        } \
        zfoverride \
        virtual zfuint enumValueForName(ZF_IN const zfchar *name) \
        { \
            return zfself::EnumValueForName(name); \
        } \
        zfoverride \
        virtual const zfchar *enumNameForValue(ZF_IN zfuint value) \
        { \
            return zfself::EnumNameForValue(value); \
        } \
    public: \
        static const _ZFP_ZFEnumData *_ZFP_ZFEnumDataRef(void) \
        { \
            static _ZFP_ZFEnumDataHolder d(_ZFP_ZFEnumDataInit()); \
            return d.d; \
        } \
        static const _ZFP_ZFEnumData *_ZFP_ZFEnumDataInit(void) \
        { \
            zfCoreMutexLocker(); \
            _ZFP_ZFEnumData *d = _ZFP_ZFEnumDataAccess(EnumName::ClassData()); \
            if(d->needInitFlag) \
            { \
                d->needInitFlag = zffalse; \
                zfbool isEnableDuplicateValue = isEnableDuplicateValue_;

#define _ZFP_ZFENUM_VALUE_REGISTER_WITH_NAME(Value, Name) \
                d->add(isEnableDuplicateValue, zfself::e_##Value, Name);

#define _ZFP_ZFENUM_END(EnumName) \
    _ZFP_ZFENUM_END_DETAIL(EnumName, zffalse, ZFEnumInvalid())
#define _ZFP_ZFENUM_END_WITH_DEFAULT(EnumName, defaultEnum) \
    _ZFP_ZFENUM_END_DETAIL(EnumName, zffalse, defaultEnum)

#define _ZFP_ZFENUM_END_FLAGS(EnumName, EnumFlagsName) \
    _ZFP_ZFENUM_END_DETAIL(EnumName, zftrue, ZFEnumInvalid()) \
    _ZFP_ZFENUM_FLAGS_DECLARE(EnumName, EnumFlagsName, EnumName::EnumDefault())
#define _ZFP_ZFENUM_END_FLAGS_WITH_DEFAULT(EnumName, EnumFlagsName, defaultEnum, defaultEnumFlags) \
    _ZFP_ZFENUM_END_DETAIL(EnumName, zftrue, defaultEnum) \
    _ZFP_ZFENUM_FLAGS_DECLARE(EnumName, EnumFlagsName, defaultEnumFlags)

#define _ZFP_ZFENUM_END_DETAIL(EnumName, IsFlags, EnumDefaultAction) \
                d->enumDefault = (zfuint)(EnumDefaultAction); \
                if(d->enumDefault == ZFEnumInvalid()) \
                { \
                    d->enumDefault = d->enumValueAtIndex(0); \
                } \
                d->enumIsFlags = (IsFlags); \
            } \
            return d; \
        } \
    public: \
        /** @brief see #ZFEnum::enumIsFlags */ \
        static inline zfbool EnumIsFlags(void) \
        { \
            return (EnumName::ZFEnumType)zfself::_ZFP_ZFEnumDataRef()->enumIsFlags; \
        } \
        /** @brief default value for @ref EnumName */ \
        static inline EnumName::ZFEnumType EnumDefault(void) \
        { \
            return (EnumName::ZFEnumType)zfself::_ZFP_ZFEnumDataRef()->enumDefault; \
        } \
    public: \
        zfoverride \
        virtual const zfchar *wrappedValueTypeId(void); \
    }; \
    /** @brief editable version of @ref EnumName */ \
    zfclass ZF_ENV_EXPORT EnumName##Editable : zfextends EnumName \
    { \
        ZFOBJECT_DECLARE(EnumName##Editable, EnumName) \
    public: \
        zfoverride \
        virtual void enumValueSet(ZF_IN zfuint value) \
        { \
            zfsuper::enumValueSet(value); \
        } \
    }; \
    /** @brief same as @ref EnumName##Enum, see @ref EnumName */ \
    typedef EnumName::ZFEnumType EnumName##Enum; \
    _ZFP_ZFENUM_CONVERTER_DECLARE(EnumName) \
    _ZFP_ZFENUM_PROP_TYPE_DECLARE(EnumName)

// ============================================================
extern ZF_ENV_EXPORT const _ZFP_ZFEnumData *_ZFP_ZFEnumDataFind(ZF_IN const ZFClass *enumClass);
extern ZF_ENV_EXPORT void _ZFP_ZFEnumMethodReg(ZF_IN_OUT ZFCoreArrayPOD<const ZFMethod *> &ret,
                                               ZF_IN const _ZFP_ZFEnumData *d);
#define _ZFP_ZFENUM_DEFINE(EnumName) \
    _ZFP_ZFENUM_CONVERTER_DEFINE(EnumName) \
    ZFOBJECT_REGISTER(EnumName) \
    ZFOBJECT_REGISTER(EnumName##Editable) \
    _ZFP_ZFENUM_PROP_TYPE_DEFINE(EnumName) \
    ZF_STATIC_REGISTER_INIT(EnumReg_##EnumName) \
    { \
        for(zfindex i = 0; i < EnumName::EnumCount(); ++i) \
        { \
            ZFMethodUserRegisterDetail_0(resultMethod, &ivk_e, EnumName::ClassData(), \
                public, ZFMethodTypeStatic, \
                EnumName##Enum, zfstringWithFormat(zfText("e_%s"), EnumName::EnumNameAtIndex(i))); \
            _m.add(resultMethod); \
        } \
        _ZFP_ZFEnumMethodReg(_m, EnumName::_ZFP_ZFEnumDataRef()); \
    } \
    ZF_STATIC_REGISTER_DESTROY(EnumReg_##EnumName) \
    { \
        for(zfindex i = 0; i < _m.count(); ++i) \
        { \
            ZFMethodUserUnregister(_m[i]); \
        } \
    } \
    ZFCoreArrayPOD<const ZFMethod *> _m; \
    static EnumName##Enum ivk_e(ZF_IN const ZFMethod *invokerMethod, ZF_IN ZFObject *invokerObject) \
    {return (EnumName##Enum)EnumName::EnumValueForName(invokerMethod->methodName() + 2);} \
    ZF_STATIC_REGISTER_END(EnumReg_##EnumName)

#define _ZFP_ZFENUM_DEFINE_FLAGS(EnumName, EnumFlagsName) \
    _ZFP_ZFENUM_DEFINE(EnumName) \
    _ZFP_ZFENUM_FLAGS_DEFINE(EnumName, EnumFlagsName)

// ============================================================
#define _ZFP_ZFENUM_CONVERTER_DECLARE(EnumName) \
    /** @brief see @ref EnumName, return empty string if error */ \
    extern ZF_ENV_EXPORT zfbool EnumName##ToString(ZF_IN_OUT zfstring &ret, ZF_IN EnumName *const &value); \
    /** @brief see @ref EnumName, return empty string if error */ \
    inline zfstring EnumName##ToString(ZF_IN EnumName *const &value) \
    { \
        zfstring ret; \
        EnumName##ToString(ret, value); \
        return ret; \
    } \
    /** @brief see @ref EnumName, return enum object if success */ \
    extern ZF_ENV_EXPORT zfbool EnumName##FromString(ZF_OUT zfautoObject &ret, \
                                                     ZF_IN const zfchar *src, \
                                                     ZF_IN_OPT zfindex srcLen = zfindexMax()); \
    ZFOUTPUT_TYPE_DECLARE(EnumName##Enum)
#define _ZFP_ZFENUM_CONVERTER_DEFINE(EnumName) \
    zfbool EnumName##ToString(ZF_IN_OUT zfstring &ret, ZF_IN EnumName *const &value) \
    { \
        ret += ((value == zfnull) ? zfText("") : value->enumName()); \
        return zftrue; \
    } \
    zfbool EnumName##FromString(ZF_OUT zfautoObject &ret, \
                                ZF_IN const zfchar *src, \
                                ZF_IN_OPT zfindex srcLen /* = zfindexMax() */) \
    { \
        if(zfsncmp(src, ZFEnumNameInvalid(), srcLen) == 0) \
        { \
            ret = zflineAlloc(EnumName, ZFEnumInvalid()); \
            return zftrue; \
        } \
        zfuint tmpValue = EnumName::EnumValueForName( \
            (srcLen == zfindexMax()) ? src : zfstring(src, srcLen).cString()); \
        if(tmpValue == ZFEnumInvalid()) \
        { \
            return zffalse; \
        } \
        else \
        { \
            ret = zflineAlloc(EnumName, tmpValue); \
            return zftrue; \
        } \
    } \
    ZFOUTPUT_TYPE_DEFINE(EnumName##Enum, {output << EnumName::EnumNameForValue(v);})

// ============================================================
#define _ZFP_ZFENUM_FLAGS_DECLARE(EnumName, EnumFlagsName, defaultValue) \
    /** @brief EnumName as #zfflags, see @ref EnumName, @ref EnumFlagsName##ToString, @ref EnumFlagsName##FromString */ \
    zffinal zfclassPOD ZF_ENV_EXPORT EnumFlagsName \
    { /* this class should be POD-like to support enum value reinterpret cast (EnumReinterpretCast) */ \
    public: \
        /** @brief default value for EnumFlagsName */ \
        static zfuint EnumDefault(void) \
        { \
            return defaultValue; \
        } \
    public: \
        /** @cond ZFPrivateDoc */ \
        EnumFlagsName(void) : flags(0) {} \
        EnumFlagsName(ZF_IN const zfuint &flags) : flags(flags) {} \
        EnumFlagsName(ZF_IN const EnumName##Enum &flags) : flags((zfuint)flags) {} \
        EnumFlagsName(ZF_IN const EnumFlagsName &ref) : flags(ref.flags) {} \
    public: \
        const zfuint &enumValue(void) const {return this->flags;} \
        void enumValueSet(ZF_IN const zfuint &flags) {this->flags = flags;} \
        void enumValueSet(ZF_IN const EnumName##Enum &flags) {this->flags = (zfuint)flags;} \
    public: \
        operator const zfuint & (void) const {return this->flags;} \
        EnumFlagsName &operator = (ZF_IN const zfuint &flags) {this->flags = flags; return *this;} \
        EnumFlagsName &operator = (ZF_IN const EnumName##Enum &flags) {this->flags = (zfuint)flags; return *this;} \
        EnumFlagsName &operator = (ZF_IN const EnumFlagsName &ref) {this->flags = ref.flags; return *this;} \
        zfbool operator == (ZF_IN const zfuint &flags) const {return (this->flags == flags);} \
        zfbool operator == (ZF_IN const EnumName##Enum &flags) const {return (this->flags == (zfuint)flags);} \
        zfbool operator == (ZF_IN const EnumFlagsName &ref) const {return (this->flags == ref.flags);} \
        zfbool operator != (ZF_IN const zfuint &flags) const {return (this->flags != flags);} \
        zfbool operator != (ZF_IN const EnumName##Enum &flags) const {return (this->flags != (zfuint)flags);} \
        zfbool operator != (ZF_IN const EnumFlagsName &ref) const {return (this->flags != ref.flags);} \
        EnumFlagsName &operator |= (ZF_IN const zfuint &flags) {this->flags |= flags; return *this;} \
        EnumFlagsName &operator |= (ZF_IN const EnumName##Enum &flags) {this->flags |= (zfuint)flags; return *this;} \
        EnumFlagsName &operator |= (ZF_IN const EnumFlagsName &ref) {this->flags |= ref.flags; return *this;} \
        EnumFlagsName &operator &= (ZF_IN const zfuint &flags) {this->flags &= flags; return *this;} \
        EnumFlagsName &operator &= (ZF_IN const EnumName##Enum &flags) {this->flags &= (zfuint)flags; return *this;} \
        EnumFlagsName &operator &= (ZF_IN const EnumFlagsName &ref) {this->flags &= ref.flags; return *this;} \
    public: \
        void objectInfoT(ZF_IN_OUT zfstring &ret) const; \
        inline zfstring objectInfo(void) const \
        { \
            zfstring ret; \
            this->objectInfoT(ret); \
            return ret; \
        } \
        /** @endcond */ \
    private: \
        zfuint flags; \
    }; \
    ZFOUTPUT_TYPE_DECLARE(EnumFlagsName) \
    _ZFP_ZFENUM_FLAGS_PROP_TYPE_DECLARE(EnumName, EnumFlagsName)

#define _ZFP_ZFENUM_FLAGS_DEFINE(EnumName, EnumFlagsName) \
    ZFOUTPUT_TYPE_DEFINE(EnumFlagsName, {output << v.objectInfo();}) \
    void EnumFlagsName::objectInfoT(ZF_IN_OUT zfstring &ret) const \
    { \
        zfflagsToString(ret, EnumName::ClassData(), (zfflags)this->enumValue()); \
    } \
    _ZFP_ZFENUM_FLAGS_PROP_TYPE_DEFINE(EnumName, EnumFlagsName)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFEnumDeclare_h_

#include "ZFEnumDeclarePropType.h"

