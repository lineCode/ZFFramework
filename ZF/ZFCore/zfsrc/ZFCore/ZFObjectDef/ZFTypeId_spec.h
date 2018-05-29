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
#define ZFTypeId_void() zfText("void")

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
        static zfbool accessAvailable(ZF_IN ZFObject *obj)
        {
            return zffalse;
        }
        static typename zftTraits<T_Access>::TrType access(ZF_IN ZFObject *obj)
        {
            return typename zftTraits<T_Access>::TrType();
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

/** @brief type wrapper for #ZFTypeId::Value */
zfclass ZF_ENV_EXPORT v_zfautoObject : zfextends ZFTypeIdWrapper
{
    ZFOBJECT_DECLARE_WITH_CUSTOM_CTOR(v_zfautoObject, ZFTypeIdWrapper)
public:
    /** @brief the value, see #ZFTypeId::Value */
    zfautoObject zfv;
protected:
    v_zfautoObject(void) : zfv() {}
protected:
    /** @brief init with value */
    virtual void objectOnInit(ZF_IN zfautoObject const &value)
    {
        this->objectOnInit();
        this->zfv = value;
    }
    zfoverride
    virtual void objectOnInit(void) {zfsuper::objectOnInit();}
    zfoverride
    virtual inline void objectInfoOnAppendTokenLeft(ZF_IN_OUT zfstring &ret) {}
    zfoverride
    virtual inline void objectInfoOnAppendTokenRight(ZF_IN_OUT zfstring &ret) {}
    zfoverride
    virtual void objectInfoOnAppend(ZF_IN_OUT zfstring &ret);
public:
    zfoverride
    virtual ZFCompareResult objectCompare(ZF_IN ZFObject *anotherObj);
public:
    zfoverride
    virtual void assignAction(ZF_IN ZFTypeIdWrapper *ref);
    zfoverride
    virtual const zfchar *wrappedValueTypeId(void);
    zfoverride
    virtual void *wrappedValue(void) {return &(this->zfv);}
    zfoverride
    virtual void wrappedValueSet(ZF_IN const void *v) {this->zfv = *(const zfautoObject *)v;}
    zfoverride
    virtual void wrappedValueGet(ZF_IN void *v) {*(zfautoObject *)v = this->zfv;}
public:
    zfoverride
    virtual void wrappedValueReset(void)
    {this->zfv = zftValue<zfautoObject>().zfv;}
    zfoverride
    virtual zfbool wrappedValueIsInit(void);
    zfoverride
    virtual ZFCompareResult wrappedValueCompare(ZF_IN const void *v0,
                                                ZF_IN const void *v1);
    zfoverride
    virtual void wrappedValueGetInfo(ZF_IN_OUT zfstring &ret,
                                     ZF_IN const void *v);
    zfoverride
    virtual zfbool wrappedValueProgressUpdate(ZF_IN const void *from,
                                              ZF_IN const void *to,
                                              ZF_IN zffloat progress);
public:
    zfoverride
    virtual zfbool wrappedValueFromData(ZF_IN const ZFSerializableData &serializableData,
                                        ZF_OUT_OPT zfstring *outErrorHint = zfnull,
                                        ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull)
    {return zfautoObjectFromData(this->zfv, serializableData, outErrorHint, outErrorPos);}
    zfoverride
    virtual zfbool wrappedValueToData(ZF_OUT ZFSerializableData &serializableData,
                                      ZF_OUT_OPT zfstring *outErrorHint = zfnull)
    {return zfautoObjectToData(serializableData, this->zfv, outErrorHint);}
    zfoverride
    virtual zfbool wrappedValueFromString(ZF_IN const zfchar *src,
                                          ZF_IN_OPT zfindex srcLen = zfindexMax())
    {return zfautoObjectFromString(this->zfv, src, srcLen);}
    zfoverride
    virtual zfbool wrappedValueToString(ZF_IN_OUT zfstring &s)
    {return zfautoObjectToString(s, this->zfv);}
};
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
        v_zfautoObject *t = zfAlloc(v_zfautoObject);
        v = t;
        zfRelease(t);
        return zftrue;
    }
    static zfbool ValueStore(ZF_OUT zfautoObject &obj, ZF_IN zfautoObject const &v)
    {
        v_zfautoObject *t = zfAlloc(v_zfautoObject);
        t->zfv = v;
        obj = t;
        zfRelease(t);
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
        static zfbool accessAvailable(ZF_IN ZFObject *obj)
        {
            return (ZFCastZFObject(v_zfautoObject *, obj) != zfnull);
        }
        static T_Access access(ZF_IN ZFObject *obj)
        {
            v_zfautoObject *t = ZFCastZFObject(v_zfautoObject *, obj);
            return t->zfv;
        }
    };
    template<typename T_Access>
    zfclassNotPOD Value<T_Access, 1>
    {
    public:
        static zfbool accessAvailable(ZF_IN ZFObject *obj)
        {
            return (ZFCastZFObject(v_zfautoObject *, obj) != zfnull);
        }
        static typename zftTraits<T_Access>::TrNoRef access(ZF_IN ZFObject *obj)
        {
            v_zfautoObject *t = ZFCastZFObject(v_zfautoObject *, obj);
            return &(t->zfv);
        }
    };
    template<typename T_Fix>
    zfclassNotPOD Value<zfautoObject, 0, T_Fix>
    {
    public:
        static zfbool accessAvailable(ZF_IN ZFObject *obj)
        {
            return zftrue;
        }
        static zfautoObject access(ZF_IN ZFObject *obj)
        {
            v_zfautoObject *t = ZFCastZFObject(v_zfautoObject *, obj);
            if(t != zfnull)
            {
                return t->zfv;
            }
            else
            {
                return zfautoObject(obj);
            }
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
        v_zfautoObject *t = zfAlloc(v_zfautoObject);
        v = t;
        zfRelease(t);
        return zftrue;
    }
    static zfbool ValueStore(ZF_OUT zfautoObject &obj, ZF_IN zfautoObjectT<T_ZFObject> const &v)
    {
        v_zfautoObject *t = zfAlloc(v_zfautoObject);
        t->zfv = v;
        obj = t;
        zfRelease(t);
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
        static zfbool accessAvailable(ZF_IN ZFObject *obj)
        {
            return (ZFCastZFObject(v_zfautoObject *, obj) != zfnull);
        }
        static T_Access access(ZF_IN ZFObject *obj)
        {
            v_zfautoObject *t = ZFCastZFObject(v_zfautoObject *, obj);
            return (T_Access)(t->zfv);
        }
    };
    template<typename T_Access>
    zfclassNotPOD Value<T_Access, 1>
    {
    public:
        static zfbool accessAvailable(ZF_IN ZFObject *obj)
        {
            return (ZFCastZFObject(v_zfautoObject *, obj) != zfnull);
        }
        static typename zftTraits<T_Access>::TrNoRef access(ZF_IN ZFObject *obj)
        {
            v_zfautoObject *t = ZFCastZFObject(v_zfautoObject *, obj);
            return (typename zftTraits<T_Access>::TrNoRef)&(t->zfv);
        }
    };
    template<typename T_Fix>
    zfclassNotPOD Value<zfautoObjectT<T_ZFObject>, 0, T_Fix>
    {
    public:
        static zfbool accessAvailable(ZF_IN ZFObject *obj)
        {
            return zftrue;
        }
        static zfautoObjectT<T_ZFObject> access(ZF_IN ZFObject *obj)
        {
            v_zfautoObject *t = ZFCastZFObject(v_zfautoObject *, obj);
            if(t != zfnull)
            {
                return t->zfv;
            }
            else
            {
                return zfautoObjectT<T_ZFObject>(obj);
            }
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
        return ZFTypeId_ZFObject();
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
        static zfbool accessAvailable(ZF_IN ZFObject *obj)
        {
            if(obj == zfnull)
            {
                return zftrue;
            }
            else
            {
                v_zfautoObject *t = ZFCastZFObject(v_zfautoObject *, obj);
                if(t == zfnull)
                {
                    return (ZFCastZFObject(T_Type, obj) != zfnull);
                }
                else
                {
                    return (t->zfv.toObject() == zfnull || ZFCastZFObject(T_Type, t->zfv.toObject()) != zfnull);
                }
            }
        }
        static T_Access access(ZF_IN ZFObject *obj)
        {
            static T_Type _d = zfnull;
            if(obj == zfnull)
            {
                return _d;
            }
            v_zfautoObject *t = ZFCastZFObject(v_zfautoObject *, obj);
            if(t != zfnull)
            {
                if(t->zfv.toObject() == zfnull)
                {
                    return _d;
                }
                else
                {
                    obj = t->zfv.toObject();
                }
            }
            T_Type *holder = zfnew(T_Type, ZFCastZFObject(T_Type, obj));
            _ZFP_PropAliasAttach(obj, holder,
                zfsConnectLineFree(zfText("ZFObject_"), zftTraits<T_Access>::ModifierName()),
                _ZFP_PropAliasOnDetach);
            return *holder;
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
        static zfbool accessAvailable(ZF_IN ZFObject *obj)
        {
            if(obj == zfnull)
            {
                return zftrue;
            }
            else
            {
                v_zfautoObject *t = ZFCastZFObject(v_zfautoObject *, obj);
                if(t == zfnull)
                {
                    return (ZFCastZFObject(T_Type, obj) != zfnull);
                }
                else
                {
                    return (t->zfv.toObject() == zfnull || ZFCastZFObject(T_Type, t->zfv.toObject()) != zfnull);
                }
            }
        }
        static T_Access access(ZF_IN ZFObject *obj)
        {
            static T_Type *_d = zfnull;
            if(obj == zfnull)
            {
                return _d;
            }
            v_zfautoObject *t = ZFCastZFObject(v_zfautoObject *, obj);
            if(t != zfnull)
            {
                if(t->zfv.toObject() == zfnull)
                {
                    return _d;
                }
                else
                {
                    obj = t->zfv.toObject();
                }
            }
            _TrNoRef *holder = zfnew(_TrNoRef);
            *holder = ZFCastZFObject(T_Type, obj);
            _ZFP_PropAliasAttach(obj, holder,
                zfsConnectLineFree(zfText("ZFObject_"), zftTraits<T_Access>::ModifierName()),
                _ZFP_PropAliasOnDetach);
            return *holder;
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
        return ZFTypeId_ZFObject();
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
        return zftrue;
    }
    static zfbool ValueStore(ZF_OUT zfautoObject &obj, ZF_IN ZFAny const &v)
    {
        obj = v.toObject();
        return zftrue;
    }
    static zfbool ValueStore(ZF_OUT zfautoObject &obj, ZF_IN ZFObject *v)
    {
        obj = v;
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
        static zfbool accessAvailable(ZF_IN ZFObject *obj)
        {
            return (obj != zfnull);
        }
        static T_Access access(ZF_IN ZFObject *obj)
        {
            v_zfautoObject *t = ZFCastZFObject(v_zfautoObject *, obj);
            if(t != zfnull)
            {
                obj = t->zfv;
            }
            ZFAny *holder = zfnew(ZFAny, obj);
            _ZFP_PropAliasAttach(obj, holder,
                zfsConnectLineFree(zfText("ZFAny_"), zftTraits<T_Access>::ModifierName()),
                _ZFP_PropAliasOnDetach);
            return *holder;
        }
    private:
        static void _ZFP_PropAliasOnDetach(ZF_IN ZFObject *obj,
                                           ZF_IN void *v)
        {
            ZFAny *vTmp = (ZFAny *)v;
            zfdelete(vTmp);
        }
    };
    template<typename T_Access>
    zfclassNotPOD Value<T_Access, 1>
    {
    public:
        static zfbool accessAvailable(ZF_IN ZFObject *obj)
        {
            return zffalse;
        }
        static ZFAny *access(ZF_IN ZFObject *obj)
        {
            return zfnull;
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
        static zfbool accessAvailable(ZF_IN ZFObject *obj)
        {
            return ZFTypeId<T_Type_>::template Value<T_Access>::accessAvailable(obj);
        }
        static T_Access access(ZF_IN ZFObject *obj)
        {
            return ZFTypeId<T_Type_>::template Value<T_Access>::access(obj);
        }
    };
    template<typename T_Access>
    zfclassNotPOD Value<T_Access, 1>
    {
    public:
        static zfbool accessAvailable(ZF_IN ZFObject *obj)
        {
            return ((obj == zfnull) || ZFTypeId<T_Type_>::template Value<T_Access>::accessAvailable(obj));
        }
        static typename zftTraits<T_Access>::TrNoRef access(ZF_IN ZFObject *obj)
        {
            return ((obj == zfnull) ? zfnull : ZFTypeId<T_Type_>::template Value<T_Access>::access(obj));
        }
    };
};
/** @endcond */

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFTypeId_spec_h_

