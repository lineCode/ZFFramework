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

#include "ZFPropertyType.h"
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
        /** @brief calculate hash for value */ \
        static zfidentity hashForValue(ZF_IN zfuint value) \
        { \
            return zfidentityHash( \
                zfidentityCalcString(zfself::ClassData()->className()), \
                zfidentityCalcString(zfself::EnumNameForValue(value))); \
        } \
        /** @brief see #ZFObject::objectHash */ \
        virtual zfidentity objectHash(void) \
        { \
            return zfself::hashForValue(this->enumValue()); \
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
    private: \
        static const _ZFP_ZFEnumData *_ZFP_ZFEnumDataRef(void) \
        { \
            static const _ZFP_ZFEnumData *d = _ZFP_ZFEnumDataInit(); \
            return d; \
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
    _ZFP_ZFENUM_END_DETAIL(EnumName, zffalse, (EnumName::ZFEnumType)EnumName::EnumValueAtIndex(0))
#define _ZFP_ZFENUM_END_WITH_DEFAULT(EnumName, defaultEnum) \
    _ZFP_ZFENUM_END_DETAIL(EnumName, zffalse, defaultEnum)

#define _ZFP_ZFENUM_END_FLAGS(EnumName, EnumFlagsName) \
    _ZFP_ZFENUM_END_DETAIL(EnumName, zftrue, (EnumName::ZFEnumType)EnumName::EnumValueAtIndex(0)) \
    _ZFP_ZFENUM_FLAGS_DECLARE(EnumName, EnumFlagsName, EnumName::EnumDefault())
#define _ZFP_ZFENUM_END_FLAGS_WITH_DEFAULT(EnumName, EnumFlagsName, defaultEnum, defaultEnumFlags) \
    _ZFP_ZFENUM_END_DETAIL(EnumName, zftrue, defaultEnum) \
    _ZFP_ZFENUM_FLAGS_DECLARE(EnumName, EnumFlagsName, defaultEnumFlags)

#define _ZFP_ZFENUM_END_DETAIL(EnumName, IsFlags, EnumDefaultAction) \
            } \
            return d; \
        } \
    public: \
        /** @brief see #ZFEnum::enumIsFlags */ \
        static inline zfbool EnumIsFlags(void) {return IsFlags;} \
        /** @brief default value for @ref EnumName */ \
        static inline EnumName::ZFEnumType EnumDefault(void) \
        {return EnumDefaultAction;} \
    public: \
        _ZFP_ZFENUM_PROP_TYPE_WRAPPER_DECLARE(EnumName) \
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
                public, ZFMethodIsStatic, \
                EnumName##Enum, zfstringWithFormat(zfText("e_%s"), EnumName::EnumNameAtIndex(i))); \
            _m.add(resultMethod); \
        } \
        \
        { \
            ZFMethodUserRegisterDetail_0(resultMethod, &ivk_EnumIsFlags, EnumName::ClassData(), \
                public, ZFMethodIsStatic, \
                zfbool, zfText("EnumIsFlags")); \
            _m.add(resultMethod); \
        } \
        { \
            ZFMethodUserRegisterDetail_0(resultMethod, &ivk_EnumDefault, EnumName::ClassData(), \
                public, ZFMethodIsStatic, \
                zfuint, zfText("EnumDefault")); \
            _m.add(resultMethod); \
        } \
        { \
            ZFMethodUserRegisterDetail_1(resultMethod, &ivk_hashForValue, EnumName::ClassData(), \
                public, ZFMethodIsStatic, \
                zfidentity, zfText("hashForValue"), \
                ZFMP_IN(zfuint, value)); \
            _m.add(resultMethod); \
        } \
        { \
            ZFMethodUserRegisterDetail_0(resultMethod, &ivk_EnumCount, EnumName::ClassData(), \
                public, ZFMethodIsStatic, \
                zfindex, zfText("EnumCount")); \
            _m.add(resultMethod); \
        } \
        { \
            ZFMethodUserRegisterDetail_1(resultMethod, &ivk_EnumIndexForValue, EnumName::ClassData(), \
                public, ZFMethodIsStatic, \
                zfindex, zfText("EnumIndexForValue"), \
                ZFMP_IN(zfuint, value)); \
            _m.add(resultMethod); \
        } \
        { \
            ZFMethodUserRegisterDetail_1(resultMethod, &ivk_EnumValueAtIndex, EnumName::ClassData(), \
                public, ZFMethodIsStatic, \
                zfuint, zfText("EnumValueAtIndex"), \
                ZFMP_IN(zfindex, index)); \
            _m.add(resultMethod); \
        } \
        { \
            ZFMethodUserRegisterDetail_1(resultMethod, &ivk_EnumNameAtIndex, EnumName::ClassData(), \
                public, ZFMethodIsStatic, \
                const zfchar *, zfText("EnumNameAtIndex"), \
                ZFMP_IN(zfindex, index)); \
            _m.add(resultMethod); \
        } \
        { \
            ZFMethodUserRegisterDetail_1(resultMethod, &ivk_EnumContainValue, EnumName::ClassData(), \
                public, ZFMethodIsStatic, \
                zfbool, zfText("EnumContainValue"), \
                ZFMP_IN(zfuint, value)); \
            _m.add(resultMethod); \
        } \
        { \
            ZFMethodUserRegisterDetail_1(resultMethod, &ivk_EnumValueForName, EnumName::ClassData(), \
                public, ZFMethodIsStatic, \
                zfuint, zfText("EnumValueForName"), \
                ZFMP_IN(const zfchar *, name)); \
            _m.add(resultMethod); \
        } \
        { \
            ZFMethodUserRegisterDetail_1(resultMethod, &ivk_EnumNameForValue, EnumName::ClassData(), \
                public, ZFMethodIsStatic, \
                const zfchar *, zfText("EnumNameForValue"), \
                ZFMP_IN(zfuint, value)); \
            _m.add(resultMethod); \
        } \
        { \
            ZFMethodUserRegisterDetail_1(resultMethod, &ivk_enumValueSet, EnumName::ClassData(), \
                public, ZFMethodIsVirtual, \
                void, zfText("enumValueSet"), \
                ZFMP_IN(zfuint, value)); \
            _m.add(resultMethod); \
        } \
    } \
    ZF_STATIC_REGISTER_DESTROY(EnumReg_##EnumName) \
    { \
        for(zfindex i = 0; i < _m.count(); ++i) \
        { \
            ZFMethodUserUnregister(_m[i]); \
        } \
    } \
    ZFCoreArrayPOD<const ZFMethod *> _m; \
    \
    static EnumName##Enum ivk_e(ZF_IN const ZFMethod *invokerMethod, ZF_IN ZFObject *invokerObject) \
    {return (EnumName##Enum)EnumName::EnumValueForName(invokerMethod->methodName() + 2);} \
    \
    static zfbool ivk_EnumIsFlags(ZF_IN const ZFMethod *invokerMethod, ZF_IN ZFObject *invokerObject) \
    {return EnumName::EnumIsFlags();} \
    \
    static zfuint ivk_EnumDefault(ZF_IN const ZFMethod *invokerMethod, ZF_IN ZFObject *invokerObject) \
    {return EnumName::EnumDefault();} \
    \
    static zfidentity ivk_hashForValue(ZF_IN const ZFMethod *invokerMethod, ZF_IN ZFObject *invokerObject, ZF_IN zfuint value) \
    {return EnumName::hashForValue(value);} \
    \
    static zfindex ivk_EnumCount(ZF_IN const ZFMethod *invokerMethod, ZF_IN ZFObject *invokerObject) \
    {return EnumName::EnumCount();} \
    \
    static zfindex ivk_EnumIndexForValue(ZF_IN const ZFMethod *invokerMethod, ZF_IN ZFObject *invokerObject, ZF_IN zfuint value) \
    {return EnumName::EnumIndexForValue(value);} \
    \
    static zfuint ivk_EnumValueAtIndex(ZF_IN const ZFMethod *invokerMethod, ZF_IN ZFObject *invokerObject, ZF_IN zfindex index) \
    {return EnumName::EnumValueAtIndex(index);} \
    \
    static const zfchar *ivk_EnumNameAtIndex(ZF_IN const ZFMethod *invokerMethod, ZF_IN ZFObject *invokerObject, ZF_IN zfindex index) \
    {return EnumName::EnumNameAtIndex(index);} \
    \
    static zfbool ivk_EnumContainValue(ZF_IN const ZFMethod *invokerMethod, ZF_IN ZFObject *invokerObject, ZF_IN zfuint value) \
    {return EnumName::EnumContainValue(value);} \
    \
    static zfuint ivk_EnumValueForName(ZF_IN const ZFMethod *invokerMethod, ZF_IN ZFObject *invokerObject, ZF_IN const zfchar *name) \
    {return EnumName::EnumValueForName(name);} \
    \
    static const zfchar *ivk_EnumNameForValue(ZF_IN const ZFMethod *invokerMethod, ZF_IN ZFObject *invokerObject, ZF_IN zfuint value) \
    {return EnumName::EnumNameForValue(value);} \
    \
    static void ivk_enumValueSet(ZF_IN const ZFMethod *invokerMethod, ZF_IN ZFObject *invokerObject, ZF_IN zfuint value) \
    {return invokerObject->to<EnumName##Editable *>()->enumValueSet(value);} \
    \
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
        operator const zfuint &(void) const {return this->flags;} \
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

