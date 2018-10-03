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
 * @file ZFTypeId_spec.h
 * @brief reflectable type define
 */

#ifndef _ZFI_ZFTypeId_spec_h_
#define _ZFI_ZFTypeId_spec_h_

#include "ZFTypeIdCore.h"
#include "ZFIOCallback.h"
#include "zfautoObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// void
/** @brief see #ZFTYPEID_DECLARE */
#define ZFTypeId_void() "void"

/** @cond ZFPrivateDoc */
template<>
zfclassNotPOD ZFTypeId<void> : zfextendsNotPOD ZFTypeIdBase
{
public:
    enum {
        TypeIdRegistered = 1,
        TypeIdSerializable = 0,
    };
    static inline const zfchar *TypeId(void)
    {
        return ZFTypeId_void();
    }
    zfoverride
    virtual zfbool typeIdSerializable(void) const
    {
        return TypeIdSerializable;
    }
    zfoverride
    virtual const zfchar *typeId(void) const
    {
        return TypeId();
    }
    zfoverride
    virtual zfbool typeIdWrapper(ZF_OUT zfautoObject &v) const
    {
        return zffalse;
    }
    template<typename T_Access = zfint>
    zfclassNotPOD Value
    {
    public:
        static zfbool accessAvailable(ZF_IN_OUT zfautoObject &obj)
        {
            return zffalse;
        }
        static typename zftTraits<T_Access>::TrType access(ZF_IN_OUT zfautoObject &obj)
        {
            return typename zftTraits<T_Access>::TrType();
        }
        static void accessFinish(ZF_IN_OUT zfautoObject &obj)
        {
        }
    };
};
/** @endcond */

// ============================================================
ZFTYPEID_DECLARE(ZFCallerInfo, ZFCallerInfoHolder)
ZFTYPEID_ALIAS_DECLARE(ZFCallerInfo, ZFCallerInfoHolder, ZFCallerInfoWrap, ZFCallerInfo)
ZFOUTPUT_TYPE(ZFCallerInfoHolder, {output << v.callerInfo();})
ZFOUTPUT_TYPE(ZFCallerInfo, {output << v.callerInfo();})

// ============================================================
// zfautoObject
/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <MyObject />
 * @endcode
 */
ZFTYPEID_DECLARE_WITH_CUSTOM_WRAPPER(zfautoObject, zfautoObject)
ZFOUTPUT_TYPE_DECLARE(zfautoObject)

/** @cond ZFPrivateDoc */
template<>
zfclassNotPOD ZFTypeId<zfautoObject> : zfextendsNotPOD ZFTypeIdBase
{
public:
    enum {
        TypeIdRegistered = 1,
        TypeIdSerializable = 1,
    };
    static inline const zfchar *TypeId(void)
    {
        return ZFTypeId_zfautoObject();
    }
    zfoverride
    virtual zfbool typeIdSerializable(void) const
    {
        return TypeIdSerializable;
    }
    zfoverride
    virtual const zfchar *typeId(void) const
    {
        return TypeId();
    }
    zfoverride
    virtual zfbool typeIdWrapper(ZF_OUT zfautoObject &v) const
    {
        v = zfnull;
        return zftrue;
    }
    static zfbool ValueStore(ZF_OUT zfautoObject &obj, ZF_IN zfautoObject const &v)
    {
        obj = v;
        return zftrue;
    }
    template<typename T_Access = zfautoObject
        , int T_IsPointer = ((zftTraits<typename zftTraits<T_Access>::TrNoRef>::TrIsPtr
            && zftTypeIsSame<
                    typename zftTraits<T_Access>::TrNoRef,
                    zfautoObject
                >::TypeIsSame != 1)
            ? 1 : 0)
        , typename T_Fix = void
        >
    zfclassNotPOD Value
    {
    public:
        static zfbool accessAvailable(ZF_IN_OUT zfautoObject &obj)
        {
            return zftrue;
        }
        static T_Access access(ZF_IN_OUT zfautoObject &obj)
        {
            return obj;
        }
        static void accessFinish(ZF_IN_OUT zfautoObject &obj)
        {
        }
    };
    template<typename T_Access>
    zfclassNotPOD Value<T_Access, 1>
    {
    public:
        static zfbool accessAvailable(ZF_IN_OUT zfautoObject &obj)
        {
            return zftrue;
        }
        static typename zftTraits<T_Access>::TrNoRef access(ZF_IN_OUT zfautoObject &obj)
        {
            return &obj;
        }
        static void accessFinish(ZF_IN_OUT zfautoObject &obj)
        {
        }
    };
};
/** @endcond */

// ============================================================
// zfautoObjectT
/** @cond ZFPrivateDoc */
template<typename T_ZFObject>
zfclassNotPOD ZFTypeId<zfautoObjectT<T_ZFObject> > : zfextendsNotPOD ZFTypeIdBase
{
public:
    enum {
        TypeIdRegistered = 1,
        TypeIdSerializable = 1,
    };
    static inline const zfchar *TypeId(void)
    {
        return ZFTypeId_zfautoObject();
    }
    zfoverride
    virtual zfbool typeIdSerializable(void) const
    {
        return TypeIdSerializable;
    }
    zfoverride
    virtual const zfchar *typeId(void) const
    {
        return TypeId();
    }
    zfoverride
    virtual zfbool typeIdWrapper(ZF_OUT zfautoObject &v) const
    {
        v = zfnull;
        return zftrue;
    }
    static zfbool ValueStore(ZF_OUT zfautoObject &obj, ZF_IN zfautoObjectT<T_ZFObject> const &v)
    {
        obj = v;
        return zftrue;
    }
    template<typename T_Access = zfautoObjectT<T_ZFObject>
        , int T_IsPointer = ((zftTraits<typename zftTraits<T_Access>::TrNoRef>::TrIsPtr
            && zftTypeIsSame<
                    typename zftTraits<T_Access>::TrNoRef,
                    zfautoObjectT<T_ZFObject>
                >::TypeIsSame != 1)
            ? 1 : 0)
        , typename T_Fix = void
        >
    zfclassNotPOD Value
    {
    public:
        static zfbool accessAvailable(ZF_IN_OUT zfautoObject &obj)
        {
            return zftrue;
        }
        static T_Access access(ZF_IN_OUT zfautoObject &obj)
        {
            return (T_Access)obj;
        }
        static void accessFinish(ZF_IN_OUT zfautoObject &obj)
        {
        }
    };
    template<typename T_Access>
    zfclassNotPOD Value<T_Access, 1>
    {
    public:
        static zfbool accessAvailable(ZF_IN_OUT zfautoObject &obj)
        {
            return zftrue;
        }
        static typename zftTraits<T_Access>::TrNoRef access(ZF_IN_OUT zfautoObject &obj)
        {
            // zfautoObjectT ensured safe for reinterpret cast
            return (typename zftTraits<T_Access>::TrNoRef)&obj;
        }
        static void accessFinish(ZF_IN_OUT zfautoObject &obj)
        {
        }
    };
};
/** @endcond */

// ============================================================
// ZFObject
/** @cond ZFPrivateDoc */
template<typename T_Type>
zfclassNotPOD ZFTypeId<T_Type,
        typename zftEnableIf<zftIsZFObject(typename zftTraits<T_Type>::TrType)>::EnableIf
    > : zfextendsNotPOD ZFTypeIdBase
{
public:
    enum {
        TypeIdRegistered = 1,
        TypeIdSerializable = 1,
    };
    static inline const zfchar *TypeId(void)
    {
        return zftTraits<T_Type>::TrType::ClassData()->classNameFull();
    }
    zfoverride
    virtual zfbool typeIdSerializable(void) const
    {
        return TypeIdSerializable;
    }
    zfoverride
    virtual const zfchar *typeId(void) const
    {
        return TypeId();
    }
    zfoverride
    virtual zfbool typeIdWrapper(ZF_OUT zfautoObject &v) const
    {
        v = zfnull;
        return zftrue;
    }
    static zfbool ValueStore(ZF_OUT zfautoObject &obj, ZF_IN T_Type const &v)
    {
        obj = ZFCastZFObjectUnchecked(ZFObject *, v);
        return zftrue;
    }
    template<typename T_Access = T_Type
        , int T_IsPointer = ((zftTraits<typename zftTraits<T_Access>::TrNoRef>::TrIsPtr
            && zftTypeIsSame<
                    typename zftTraits<T_Access>::TrNoRef,
                    T_Type
                >::TypeIsSame != 1)
            ? 1 : 0)
        , typename T_Fix = void
        >
    zfclassNotPOD Value
    {
    public:
        static zfbool accessAvailable(ZF_IN_OUT zfautoObject &obj)
        {
            return (obj == zfnull || ZFCastZFObject(T_Type, obj) != zfnull);
        }
        static T_Access access(ZF_IN_OUT zfautoObject &obj)
        {
            if(obj == zfnull)
            {
                static T_Type _d = zfnull;
                return _d;
            }
            T_Type *holder = zfnew(T_Type, ZFCastZFObject(T_Type, obj));
            _ZFP_PropAliasAttach(obj, holder,
                zfsConnectLineFree(
                    zftTraits<T_Type>::TrType::ClassData()->classNameFull(),
                    "_",
                    zftTraits<T_Access>::ModifierName()),
                _ZFP_PropAliasOnDetach);
            return *holder;
        }
        static void accessFinish(ZF_IN_OUT zfautoObject &obj)
        {
            if(obj == zfnull)
            {
                return ;
            }
            _ZFP_PropAliasDetach(obj,
                zfsConnectLineFree(
                    zftTraits<T_Type>::TrType::ClassData()->classNameFull(),
                    "_",
                    zftTraits<T_Access>::ModifierName())
                );
        }
    private:
        static void _ZFP_PropAliasOnDetach(ZF_IN ZFObject *obj,
                                           ZF_IN void *v)
        {
            T_Type *vTmp = (T_Type *)v;
            zfdelete(vTmp);
        }
    };
    template<typename T_Access>
    zfclassNotPOD Value<T_Access, 1>
    {
    private:
        typedef typename zftTraits<T_Access>::TrNoRef _TrNoRef;
    public:
        static zfbool accessAvailable(ZF_IN_OUT zfautoObject &obj)
        {
            return (obj == zfnull || ZFCastZFObject(T_Type, obj) != zfnull);
        }
        static T_Access access(ZF_IN_OUT zfautoObject &obj)
        {
            if(obj == zfnull)
            {
                static T_Type *_d = zfnull;
                return _d;
            }
            _TrNoRef *holder = zfnew(_TrNoRef);
            *holder = ZFCastZFObject(T_Type, obj);
            _ZFP_PropAliasAttach(obj, holder,
                zfsConnectLineFree(
                    typename zftTraits<_TrNoRef>::TrType::ClassData()->classNameFull(),
                    "_",
                    zftTraits<T_Access>::ModifierName()),
                _ZFP_PropAliasOnDetach);
            return *holder;
        }
        static void accessFinish(ZF_IN_OUT zfautoObject &obj)
        {
            if(obj == zfnull)
            {
                return ;
            }
            _ZFP_PropAliasAttach(obj,
                zfsConnectLineFree(
                    typename zftTraits<_TrNoRef>::TrType::ClassData()->classNameFull(),
                    "_",
                    zftTraits<T_Access>::ModifierName())
                );
        }
    private:
        static void _ZFP_PropAliasOnDetach(ZF_IN ZFObject *obj,
                                           ZF_IN void *v)
        {
            _TrNoRef *vTmp = (_TrNoRef *)v;
            zfdelete(vTmp);
        }
    };
};
/** @endcond */

// ============================================================
// ZFAny
/** @cond ZFPrivateDoc */
template<>
zfclassNotPOD ZFTypeId<ZFAny> : zfextendsNotPOD ZFTypeIdBase
{
public:
    enum {
        TypeIdRegistered = 1,
        TypeIdSerializable = 1,
    };
    static inline const zfchar *TypeId(void)
    {
        return ZFObject::ClassData()->classNameFull();
    }
    zfoverride
    virtual zfbool typeIdSerializable(void) const
    {
        return TypeIdSerializable;
    }
    zfoverride
    virtual const zfchar *typeId(void) const
    {
        return TypeId();
    }
    zfoverride
    virtual zfbool typeIdWrapper(ZF_OUT zfautoObject &v) const
    {
        v = zfnull;
        return zftrue;
    }
    static zfbool ValueStore(ZF_OUT zfautoObject &obj, ZF_IN ZFAny const &v)
    {
        obj = v.toObject();
        return zftrue;
    }
    template<typename T_Access = ZFAny
        , int T_IsPointer = ((zftTraits<typename zftTraits<T_Access>::TrNoRef>::TrIsPtr
            && zftTypeIsSame<
                    typename zftTraits<T_Access>::TrNoRef,
                    ZFAny
                >::TypeIsSame != 1)
            ? 1 : 0)
        , typename T_Fix = void
        >
    zfclassNotPOD Value
    {
    public:
        static zfbool accessAvailable(ZF_IN_OUT zfautoObject &obj)
        {
            return zftrue;
        }
        static ZFAny access(ZF_IN_OUT zfautoObject &obj)
        {
            return obj;
        }
        static void accessFinish(ZF_IN_OUT zfautoObject &obj)
        {
        }
    };
    template<typename T_Access>
    zfclassNotPOD Value<T_Access, 1>
    {
    private:
        typedef typename zftTraits<T_Access>::TrNoRef _TrNoRef;
    public:
        static zfbool accessAvailable(ZF_IN_OUT zfautoObject &obj)
        {
            return zffalse;
        }
        static const ZFAny *access(ZF_IN_OUT zfautoObject &obj)
        {
            return zfnull;
        }
        static void accessFinish(ZF_IN_OUT zfautoObject &obj)
        {
        }
    };
};
/** @endcond */

// ============================================================
// pointer type
/** @cond ZFPrivateDoc */
template<typename T_Type>
zfclassNotPOD ZFTypeId<T_Type,
        typename zftEnableIf<!zftIsZFObject(typename zftTraits<T_Type>::TrType)>::EnableIf,
        typename zftEnableIf<zftTraits<T_Type>::TrIsPtr>::EnableIf
    > : zfextendsNotPOD ZFTypeIdBase
{
public:
    typedef typename zftTraits<T_Type>::TrType T_Type_;
public:
    enum {
        TypeIdRegistered = ZFTypeId<T_Type_>::TypeIdRegistered,
        TypeIdSerializable = ZFTypeId<T_Type_>::TypeIdSerializable,
    };
    static inline const zfchar *TypeId(void)
    {
        return ZFTypeId<T_Type_>::TypeId();
    }
    zfoverride
    virtual zfbool typeIdSerializable(void) const
    {
        return TypeIdSerializable;
    }
    zfoverride
    virtual const zfchar *typeId(void) const
    {
        return TypeId();
    }
    zfoverride
    virtual zfbool typeIdWrapper(ZF_OUT zfautoObject &v) const
    {
        ZFTypeId<T_Type_> t;
        t.typeIdWrapper(v);
        return zftrue;
    }
    static zfbool ValueStore(ZF_OUT zfautoObject &obj, T_Type const &v)
    {
        if(v == zfnull)
        {
            obj = zfnull;
            return zftrue;
        }
        else
        {
            return ZFTypeId<T_Type_>::ValueStore(obj, *v);
        }
    }
    static zfbool ValueStore(ZF_OUT zfautoObject &obj, T_Type_ const &v)
    {
        return ZFTypeId<T_Type_>::ValueStore(obj, *v);
    }
    template<typename T_Access = T_Type
        , int T_IsPointer = ((zftTraits<typename zftTraits<T_Access>::TrNoRef>::TrIsPtr
            && zftTypeIsSame<
                    typename zftTraits<T_Access>::TrNoRef,
                    T_Type_
                >::TypeIsSame != 1)
            ? 1 : 0)
        , typename T_Fix = void
        >
    zfclassNotPOD Value
    {
    public:
        static zfbool accessAvailable(ZF_IN_OUT zfautoObject &obj)
        {
            return ZFTypeId<T_Type_>::template Value<T_Access>::accessAvailable(obj);
        }
        static T_Access access(ZF_IN_OUT zfautoObject &obj)
        {
            return ZFTypeId<T_Type_>::template Value<T_Access>::access(obj);
        }
        static void accessFinish(ZF_IN_OUT zfautoObject &obj)
        {
            ZFTypeId<T_Type_>::template Value<T_Access>::accessFinish(obj);
        }
    };
    template<typename T_Access>
    zfclassNotPOD Value<T_Access, 1>
    {
    public:
        static zfbool accessAvailable(ZF_IN_OUT zfautoObject &obj)
        {
            return ((obj == zfnull) || ZFTypeId<T_Type_>::template Value<T_Access>::accessAvailable(obj));
        }
        static typename zftTraits<T_Access>::TrNoRef access(ZF_IN_OUT zfautoObject &obj)
        {
            return ((obj == zfnull) ? zfnull : ZFTypeId<T_Type_>::template Value<T_Access>::access(obj));
        }
        static void accessFinish(ZF_IN_OUT zfautoObject &obj)
        {
            if(obj != zfnull)
            {
                ZFTypeId<T_Type_>::template Value<T_Access>::accessFinish(obj);
            }
        }
    };
};
/** @endcond */

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFTypeId_spec_h_

