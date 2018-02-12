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
 * @file ZFEnumDeclarePropType.h
 * @brief ZFEnum declare impl
 */

#ifndef _ZFI_ZFEnumDeclarePropType_h_
#define _ZFI_ZFEnumDeclarePropType_h_

#include "ZFEnumDeclare.h"
ZF_NAMESPACE_GLOBAL_BEGIN

template<typename T> zfclassNotPOD _ZFP_ZFEnumCanModify {public: enum {CanModify = 0};};
template<typename T> zfclassNotPOD _ZFP_ZFEnumCanModify<const T *> {public: enum {CanModify = 0};};
template<typename T> zfclassNotPOD _ZFP_ZFEnumCanModify<T *> {public: enum {CanModify = 1};};
template<typename T> zfclassNotPOD _ZFP_ZFEnumCanModify<const T &> {public: enum {CanModify = 0};};
template<typename T> zfclassNotPOD _ZFP_ZFEnumCanModify<T &> {public: enum {CanModify = 1};};

// ============================================================
// ZFPropertyTypeWrapper
#define _ZFP_ZFENUM_PROP_TYPE_WRAPPER_DECLARE(EnumName) \
    public: \
        zfoverride \
        virtual const zfchar *wrappedValueTypeId(void);

// ============================================================
// normal enum
#define _ZFP_ZFENUM_PROP_TYPE_DECLARE(EnumName) \
    ZFPROPERTY_TYPE_DECLARE_WITH_CUSTOM_WRAPPER(EnumName##Enum, EnumName##Enum) \
    /** @cond ZFPrivateDoc */ \
    template<> \
    zfclassNotPOD ZFPropertyTypeIdData<EnumName##Enum> : zfextendsNotPOD ZFPropertyTypeIdDataBase \
    { \
        _ZFP_ZFPROPERTY_TYPE_ID_DATA_BASE_EXPAND(EnumName##Enum) \
    public: \
        enum { \
            PropertyRegistered = 1, \
            PropertySerializable = 1, \
        }; \
        static inline const zfchar *PropertyTypeId(void) \
        { \
            return ZFPropertyTypeId_##EnumName##Enum(); \
        } \
        zfoverride \
        virtual zfbool propertyWrapper(ZF_OUT zfautoObject &v) const \
        { \
            EnumName *t = zfAlloc(EnumName); \
            v = t; \
            zfRelease(t); \
            return zftrue; \
        } \
        static zfbool ValueStore(ZF_OUT zfautoObject &obj, ZF_IN zfuint const &v) \
        { \
            EnumName *t = zfAlloc(EnumName, v); \
            obj = t; \
            zfRelease(t); \
            return zftrue; \
        } \
        static zfbool ValueStore(ZF_OUT zfautoObject &obj, ZF_IN EnumName##Enum const &v) \
        { \
            EnumName *t = zfAlloc(EnumName, v); \
            obj = t; \
            zfRelease(t); \
            return zftrue; \
        } \
        template<typename T_Access = EnumName##Enum \
            , int T_IsPointer = ((zftTraits<T_Access>::TrIsPtr \
                && zftTypeIsSame< \
                        typename zftTraits<T_Access>::TrNoRef, \
                        EnumName##Enum \
                    >::TypeIsSame != 1) \
                ? 1 : 0) \
            , typename T_Fix = void \
            > \
        zfclassNotPOD Value \
        { \
        public: \
            typedef typename zftTraits<T_Access>::TrNoRef T_NoRef; \
        public: \
            static zfbool accessAvailable(ZF_IN ZFObject *obj) \
            { \
                return (ZFCastZFObject(EnumName##Editable *, obj) != zfnull \
                    || (ZFCastZFObject(EnumName *, obj) != zfnull && !_ZFP_ZFEnumCanModify<T_Access>::CanModify)); \
            } \
            static T_Access access(ZF_IN ZFObject *obj) \
            { \
                /* EnumReinterpretCast */ \
                return *(T_NoRef *)(&(ZFCastZFObject(EnumName *, obj)->_ZFP_ZFEnum_value)); \
            } \
        }; \
        template<typename T_Access> \
        zfclassNotPOD Value<T_Access, 1> \
        { \
        public: \
            typedef typename zftTraits<T_Access>::TrNoRef T_NoRef; \
        public: \
            static zfbool accessAvailable(ZF_IN ZFObject *obj) \
            { \
                return (ZFCastZFObject(EnumName##Editable *, obj) != zfnull \
                    || (ZFCastZFObject(EnumName *, obj) != zfnull && !_ZFP_ZFEnumCanModify<T_NoRef>::CanModify)); \
            } \
            static T_NoRef access(ZF_IN ZFObject *obj) \
            { \
                /* EnumReinterpretCast */ \
                return (T_NoRef)(&(ZFCastZFObject(EnumName *, obj)->_ZFP_ZFEnum_value)); \
            } \
        }; \
    }; \
    /** @endcond */

#define _ZFP_ZFENUM_PROP_TYPE_DEFINE(EnumName) \
    ZFPROPERTY_TYPE_DEFINE_BY_STRING_CONVERTER_WITH_CUSTOM_WRAPPER(EnumName##Enum, EnumName##Enum, { \
            if(zfsncmp(src, ZFEnumNameInvalid(), srcLen) == 0) \
            { \
                v = (EnumName##Enum)ZFEnumInvalid(); \
                return zftrue; \
            } \
            v = (EnumName##Enum)EnumName::EnumValueForName( \
                    (srcLen == zfindexMax()) ? src : zfstring(src, srcLen).cString() \
                ); \
            return ((zfuint)v != ZFEnumInvalid()); \
        }, { \
            s += EnumName::EnumNameForValue(v); \
            return zftrue; \
        }) \
    const zfchar *EnumName::wrappedValueTypeId(void) \
    { \
        return ZFPropertyTypeId_##EnumName##Enum(); \
    }


// ============================================================
// enum flags
#define _ZFP_ZFENUM_FLAGS_PROP_TYPE_DECLARE(EnumName, EnumFlagsName) \
    ZFPROPERTY_TYPE_DECLARE_WITH_CUSTOM_WRAPPER(EnumFlagsName, EnumFlagsName) \
    /** @cond ZFPrivateDoc */ \
    template<> \
    zfclassNotPOD ZFPropertyTypeIdData<EnumFlagsName> : zfextendsNotPOD ZFPropertyTypeIdDataBase \
    { \
        _ZFP_ZFPROPERTY_TYPE_ID_DATA_BASE_EXPAND(EnumFlagsName) \
    public: \
        enum { \
            PropertyRegistered = 1, \
            PropertySerializable = 1, \
        }; \
        static inline const zfchar *PropertyTypeId(void) \
        { \
            return ZFPropertyTypeId_##EnumFlagsName(); \
        } \
        zfoverride \
        virtual zfbool propertyWrapper(ZF_OUT zfautoObject &v) const \
        { \
            EnumName *t = zfAlloc(EnumName); \
            v = t; \
            zfRelease(t); \
            return zftrue; \
        } \
        static zfbool ValueStore(ZF_OUT zfautoObject &obj, ZF_IN zfuint const &v) \
        { \
            EnumName *t = zfAlloc(EnumName, v); \
            obj = t; \
            zfRelease(t); \
            return zftrue; \
        } \
        static zfbool ValueStore(ZF_OUT zfautoObject &obj, ZF_IN EnumName##Enum const &v) \
        { \
            EnumName *t = zfAlloc(EnumName, (zfuint)v); \
            obj = t; \
            zfRelease(t); \
            return zftrue; \
        } \
        static zfbool ValueStore(ZF_OUT zfautoObject &obj, ZF_IN EnumFlagsName const &v) \
        { \
            EnumName *t = zfAlloc(EnumName, (zfuint)v); \
            obj = t; \
            zfRelease(t); \
            return zftrue; \
        } \
        template<typename T_Access = EnumFlagsName \
            , int T_IsPointer = ((zftTraits<T_Access>::TrIsPtr \
                && zftTypeIsSame< \
                        typename zftTraits<T_Access>::TrNoRef, \
                        EnumFlagsName \
                    >::TypeIsSame != 1) \
                ? 1 : 0) \
            , typename T_Fix = void \
            > \
        zfclassNotPOD Value \
        { \
        public: \
            typedef typename zftTraits<T_Access>::TrNoRef T_NoRef; \
        public: \
            static zfbool accessAvailable(ZF_IN ZFObject *obj) \
            { \
                return (ZFCastZFObject(EnumName##Editable *, obj) != zfnull \
                    || (ZFCastZFObject(EnumName *, obj) != zfnull && !_ZFP_ZFEnumCanModify<T_Access>::CanModify)); \
            } \
            static T_Access access(ZF_IN ZFObject *obj) \
            { \
                /* EnumReinterpretCast */ \
                return *(T_NoRef *)(&(ZFCastZFObject(EnumName *, obj)->_ZFP_ZFEnum_value)); \
            } \
        }; \
        template<typename T_Access> \
        zfclassNotPOD Value<T_Access, 1> \
        { \
        public: \
            typedef typename zftTraits<T_Access>::TrNoRef T_NoRef; \
        public: \
            static zfbool accessAvailable(ZF_IN ZFObject *obj) \
            { \
                return (ZFCastZFObject(EnumName##Editable *, obj) != zfnull \
                    || (ZFCastZFObject(EnumName *, obj) != zfnull && !_ZFP_ZFEnumCanModify<T_NoRef>::CanModify)); \
            } \
            static T_NoRef access(ZF_IN ZFObject *obj) \
            { \
                /* EnumReinterpretCast */ \
                return (T_NoRef)(&(ZFCastZFObject(EnumName *, obj)->_ZFP_ZFEnum_value)); \
            } \
        }; \
    }; \
    /** @endcond */

#define _ZFP_ZFENUM_FLAGS_PROP_TYPE_DEFINE(EnumName, EnumFlagsName) \
    ZFPROPERTY_TYPE_DEFINE_BY_STRING_CONVERTER_WITH_CUSTOM_WRAPPER(EnumFlagsName, EnumFlagsName, { \
            zfflags flags = zfflagsZero(); \
            if(!zfflagsFromString(flags, \
                EnumName::ClassData(), \
                src, srcLen)) \
            { \
                return zffalse; \
            } \
            v.enumValueSet((zfuint)flags); \
            return zftrue; \
        }, { \
            return zfflagsToString(s, EnumName::ClassData(), (zfflags)v.enumValue()); \
        })

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFEnumDeclarePropType_h_

