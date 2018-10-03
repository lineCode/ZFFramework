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
 * @file ZFTypeId_ZFCoreArray.h
 * @brief reflectable type define
 */

#ifndef _ZFI_ZFTypeId_ZFCoreArray_h_
#define _ZFI_ZFTypeId_ZFCoreArray_h_

#include "ZFTypeId_spec.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/** @cond ZFPrivateDoc */

template<typename T_ZFCoreArray>
zfclassNotPOD _ZFP_ZFCoreArrayConvert
{
public:
    template<typename T_Type>
    static zfbool from(ZF_OUT ZFCoreArray<zfautoObject> &holder, ZF_IN const ZFCoreArray<T_Type> &v);
    template<typename T_Type>
    static zfbool to(ZF_IN_OUT ZFCoreArray<zfautoObject> &holder, ZF_OUT ZFCoreArray<T_Type> &v);
    template<typename T_Type>
    static zfbool accessAvailable(ZF_IN_OUT zfautoObject &obj);
    template<typename T_Type>
    static T_ZFCoreArray *access(ZF_IN_OUT zfautoObject &obj, ZF_IN _ZFP_PropAliasDetachCallback detachCallback);
    template<typename T_Type>
    static void accessFinish(ZF_IN_OUT zfautoObject &obj);
};

template<>
zfclassNotPOD _ZFP_ZFCoreArrayConvert<ZFCoreArray<zfautoObject> >
{
public:
    template<typename T_Type>
    static zfbool from(ZF_OUT ZFCoreArray<zfautoObject> &holder, ZF_IN const ZFCoreArray<zfautoObject> &v);
    template<typename T_Type>
    static zfbool to(ZF_IN_OUT ZFCoreArray<zfautoObject> &holder, ZF_OUT ZFCoreArray<zfautoObject> &v);
    template<typename T_Type>
    static zfbool accessAvailable(ZF_IN_OUT zfautoObject &obj);
    template<typename T_Type>
    static ZFCoreArray<zfautoObject> *access(ZF_IN_OUT zfautoObject &obj, ZF_IN _ZFP_PropAliasDetachCallback detachCallback);
    template<typename T_Type>
    static void accessFinish(ZF_IN_OUT zfautoObject &obj);
};

// ============================================================
// ZFCoreArray
/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFCoreArray>
 *       <SomeType />
 *       ... // each element
 *   </ZFCoreArray>
 * @endcode
 *
 * string data:
 * @code
 *   [e0_encoded_string_data, e1_encoded_string_data, ...]
 * @endcode
 */
#define ZFTypeId_ZFCoreArray() "ZFCoreArray"

/** @brief type wrapper for #ZFTypeId::Value */
zfclass ZF_ENV_EXPORT v_ZFCoreArray : zfextends ZFTypeIdWrapper
{
    ZFOBJECT_DECLARE(v_ZFCoreArray, ZFTypeIdWrapper)
public:
    /** @brief the value, see #ZFTypeId::Value */
    ZFCoreArray<zfautoObject> zfv;
protected:
    /** @brief init with value */
    void objectOnInit(ZF_IN const ZFCoreArray<zfautoObject> &v)
    {
        this->zfv.copyFrom(v);
    }

    /** @brief init with value */
    template<typename T_Type>
    void objectOnInit(ZF_IN const ZFCoreArray<T_Type> &v)
    {
        this->objectOnInit();
        _ZFP_ZFCoreArrayConvert<ZFCoreArray<T_Type> >::template from<T_Type>(this->zfv, v);
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
    virtual ZFCompareResult objectCompare(ZF_IN ZFObject *anotherObj)
    {
        zfself *t = ZFCastZFObject(zfself *, anotherObj);
        if(t == zfnull)
        {
            return ZFCompareUncomparable;
        }
        else
        {
            return this->zfv.objectCompare(t->zfv);
        }
    }
public:
    zfoverride
    virtual void assignAction(ZF_IN ZFTypeIdWrapper *ref)
    {
        zfself *refTmp = ZFCastZFObject(zfself *, ref);
        if(refTmp != zfnull)
        {
            this->zfv = refTmp->zfv;
        }
    }
    zfoverride
    virtual const zfchar *wrappedValueTypeId(void)
    {
        return ZFTypeId_ZFCoreArray();
    }
    zfoverride
    virtual void *wrappedValue(void) {return &(this->zfv);}
    zfoverride
    virtual void wrappedValueSet(ZF_IN const void *v)
    {
        if(v != zfnull)
        {
            this->zfv.copyFrom(*(ZFCoreArray<zfautoObject> *)v);
        }
        else
        {
            this->zfv.removeAll();
        }
    }
    virtual void wrappedValueSet(ZF_IN const ZFCoreArrayBase &v)
    {
        this->wrappedValueSet((const void *)&v);
    }
    zfoverride
    virtual void wrappedValueGet(ZF_IN void *v)
    {
        *(ZFCoreArrayBase *)v = this->zfv;
    }
public:
    zfoverride
    virtual void wrappedValueReset(void)
    {
        this->zfv.removeAll();
    }
    zfoverride
    virtual zfbool wrappedValueIsInit(void)
    {
        return this->zfv.isEmpty();
    }
    zfoverride
    virtual ZFCompareResult wrappedValueCompare(ZF_IN const void *v0,
                                                ZF_IN const void *v1)
    {
        if(v0 == zfnull)
        {
            if(v1 == zfnull || ((const ZFCoreArray<zfautoObject> *)v1)->isEmpty())
            {
                return ZFCompareTheSame;
            }
            else
            {
                return ZFCompareUncomparable;
            }
        }
        else
        {
            if(v1 == zfnull)
            {
                if(((const ZFCoreArray<zfautoObject> *)v0)->isEmpty())
                {
                    return ZFCompareTheSame;
                }
                else
                {
                    return ZFCompareUncomparable;
                }
            }
            else
            {
                return ((const ZFCoreArray<zfautoObject> *)v0)->objectCompare(
                    *(const ZFCoreArray<zfautoObject> *)v1);
            }
        }
    }
    zfoverride
    virtual void wrappedValueGetInfo(ZF_IN_OUT zfstring &ret,
                                     ZF_IN const void *v)
    {
        ((const ZFCoreArray<zfautoObject> *)v)->objectInfoOfContentT(ret, ZFCoreElementInfoGetter<zfautoObject>::elementInfoGetter);
    }
    zfoverride
    virtual zfbool wrappedValueProgressUpdate(ZF_IN const void *from,
                                              ZF_IN const void *to,
                                              ZF_IN zffloat progress)
    {
        return zffalse;
    }
public:
    zfoverride
    virtual zfbool wrappedValueFromData(ZF_IN const ZFSerializableData &serializableData,
                                        ZF_OUT_OPT zfstring *outErrorHint = zfnull,
                                        ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull)
    {
        ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
            "not available for plain array type");
        return zffalse;
    }
    zfoverride
    virtual zfbool wrappedValueToData(ZF_OUT ZFSerializableData &serializableData,
                                      ZF_OUT_OPT zfstring *outErrorHint = zfnull)
    {
        ZFSerializableUtil::errorOccurred(outErrorHint,
            "not available for plain array type");
        return zffalse;
    }
    zfoverride
    virtual zfbool wrappedValueFromString(ZF_IN const zfchar *src,
                                          ZF_IN_OPT zfindex srcLen = zfindexMax())
    {
        return zffalse;
    }
    zfoverride
    virtual zfbool wrappedValueToString(ZF_IN_OUT zfstring &s)
    {
        return zffalse;
    }
};

template<typename T_Type>
zfclassNotPOD ZFTypeId<ZFCoreArray<T_Type> > : zfextendsNotPOD ZFTypeIdBase
{
public:
    enum {
        TypeIdRegistered = ZFTypeId<T_Type>::TypeIdRegistered,
        TypeIdSerializable = ZFTypeId<T_Type>::TypeIdSerializable,
    };
    static inline const zfchar *TypeId(void)
    {
        return ZFTypeId_ZFCoreArray();
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
        ZFObject *t = zfAlloc(v_ZFCoreArray);
        v = t;
        zfRelease(t);
        return zftrue;
    }
    static zfbool ValueStore(ZF_OUT zfautoObject &obj, ZF_IN ZFCoreArray<T_Type> const &v)
    {
        v_ZFCoreArray *holder = zfAlloc(v_ZFCoreArray);
        obj = holder;
        zfRelease(holder);
        return _ZFP_ZFCoreArrayConvert<ZFCoreArray<T_Type> >::template from<T_Type>(holder->zfv, v);
    }
    template<typename T_Access = ZFCoreArray<T_Type>
        , int T_IsPointer = ((zftTraits<typename zftTraits<T_Access>::TrNoRef>::TrIsPtr
            && zftTypeIsSame<
                    typename zftTraits<T_Access>::TrNoRef,
                    ZFCoreArray<T_Type>
                >::TypeIsSame != 1)
            ? 1 : 0)
        , typename T_Fix = void
        >
    zfclassNotPOD Value
    {
    public:
        static zfbool accessAvailable(ZF_IN_OUT zfautoObject &obj)
        {
            return _ZFP_ZFCoreArrayConvert<ZFCoreArray<T_Type> >::template accessAvailable<T_Type>(obj);
        }
        static T_Access access(ZF_IN_OUT zfautoObject &obj)
        {
            return *_ZFP_ZFCoreArrayConvert<ZFCoreArray<T_Type> >::template access<T_Type>(obj, _ZFP_PropAliasOnDetach);
        }
        static void accessFinish(ZF_IN_OUT zfautoObject &obj)
        {
            _ZFP_ZFCoreArrayConvert<ZFCoreArray<T_Type> >::template accessFinish<T_Type>(obj);
        }
    };
    template<typename T_Access>
    zfclassNotPOD Value<T_Access, 1>
    {
    public:
        static zfbool accessAvailable(ZF_IN_OUT zfautoObject &obj)
        {
            return _ZFP_ZFCoreArrayConvert<ZFCoreArray<T_Type> >::template accessAvailable<T_Type>(obj);
        }
        static typename zftTraits<T_Access>::TrNoRef access(ZF_IN_OUT zfautoObject &obj)
        {
            return _ZFP_ZFCoreArrayConvert<ZFCoreArray<T_Type> >::template access<T_Type>(obj, _ZFP_PropAliasOnDetach);
        }
        static void accessFinish(ZF_IN_OUT zfautoObject &obj)
        {
            _ZFP_ZFCoreArrayConvert<ZFCoreArray<T_Type> >::template accessFinish<T_Type>(obj);
        }
    };
private:
    static void _ZFP_PropAliasOnDetach(ZF_IN ZFObject *obj,
                                       ZF_IN void *v)
    {
        ZFCoreArray<zfautoObject> holderTmp;
        ZFCoreArray<T_Type> &vTmp = *(ZFCoreArray<T_Type> *)v;
        if(_ZFP_ZFCoreArrayConvert<ZFCoreArray<T_Type> >::template from<T_Type>(holderTmp, vTmp))
        {
            v_ZFCoreArray *holder = ZFCastZFObjectUnchecked(v_ZFCoreArray *, obj);
            holder->zfv = holderTmp;
        }
    }
};

template<typename T_Type>
zfclassNotPOD ZFTypeId<ZFCoreArrayPOD<T_Type> > : zfextendsNotPOD ZFTypeIdBase
{
public:
    enum {
        TypeIdRegistered = ZFTypeId<T_Type>::TypeIdRegistered,
        TypeIdSerializable = ZFTypeId<T_Type>::TypeIdSerializable,
    };
    static inline const zfchar *TypeId(void)
    {
        return ZFTypeId_ZFCoreArray();
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
        ZFObject *t = zfAlloc(v_ZFCoreArray);
        v = t;
        zfRelease(t);
        return zftrue;
    }
    static zfbool ValueStore(ZF_OUT zfautoObject &obj, ZF_IN ZFCoreArray<T_Type> const &v)
    {
        return ZFTypeId<ZFCoreArray<T_Type> >::ValueStore(obj, v);
    }
    template<typename T_Access = ZFCoreArrayPOD<T_Type>
        , int T_IsPointer = ((zftTraits<typename zftTraits<T_Access>::TrNoRef>::TrIsPtr
            && zftTypeIsSame<
                    typename zftTraits<T_Access>::TrNoRef,
                    ZFCoreArray<T_Type>
                >::TypeIsSame != 1)
            ? 1 : 0)
        , typename T_Fix = void
        >
    zfclassNotPOD Value
    {
    public:
        static zfbool accessAvailable(ZF_IN_OUT zfautoObject &obj)
        {
            return _ZFP_ZFCoreArrayConvert<ZFCoreArrayPOD<T_Type> >::template accessAvailable<T_Type>(obj);
        }
        static T_Access access(ZF_IN_OUT zfautoObject &obj)
        {
            return *_ZFP_ZFCoreArrayConvert<ZFCoreArrayPOD<T_Type> >::template access<T_Type>(obj, _ZFP_PropAliasOnDetach);
        }
        static void accessFinish(ZF_IN_OUT zfautoObject &obj)
        {
            _ZFP_ZFCoreArrayConvert<ZFCoreArrayPOD<T_Type> >::template accessFinish<T_Type>(obj);
        }
    };
    template<typename T_Access>
    zfclassNotPOD Value<T_Access, 1>
    {
    public:
        static zfbool accessAvailable(ZF_IN_OUT zfautoObject &obj)
        {
            return _ZFP_ZFCoreArrayConvert<ZFCoreArrayPOD<T_Type> >::template accessAvailable<T_Type>(obj);
        }
        static typename zftTraits<T_Access>::TrNoRef access(ZF_IN_OUT zfautoObject &obj)
        {
            return _ZFP_ZFCoreArrayConvert<ZFCoreArrayPOD<T_Type> >::template access<T_Type>(obj, _ZFP_PropAliasOnDetach);
        }
        static void accessFinish(ZF_IN_OUT zfautoObject &obj)
        {
            _ZFP_ZFCoreArrayConvert<ZFCoreArrayPOD<T_Type> >::template accessFinish<T_Type>(obj);
        }
    };
private:
    static void _ZFP_PropAliasOnDetach(ZF_IN ZFObject *obj,
                                       ZF_IN void *v)
    {
        ZFCoreArray<zfautoObject> holderTmp;
        ZFCoreArrayPOD<T_Type> &vTmp = *(ZFCoreArrayPOD<T_Type> *)v;
        if(_ZFP_ZFCoreArrayConvert<ZFCoreArrayPOD<T_Type> >::template from<T_Type>(holderTmp, vTmp))
        {
            v_ZFCoreArray *holder = ZFCastZFObjectUnchecked(v_ZFCoreArray *, obj);
            holder->zfv = holderTmp;
        }
    }
};

// ============================================================
template<typename T_ZFCoreArray>
template<typename T_Type>
zfbool _ZFP_ZFCoreArrayConvert<T_ZFCoreArray>::from(ZF_OUT ZFCoreArray<zfautoObject> &holder, ZF_IN const ZFCoreArray<T_Type> &v)
{
    for(zfindex i = 0; i < v.count(); ++i)
    {
        zfautoObject tmp;
        if(!ZFTypeId<T_Type>::ValueStore(tmp, v[i]))
        {
            return zffalse;
        }
        holder.add(tmp);
    }
    return zftrue;
}

template<typename T_ZFCoreArray>
template<typename T_Type>
zfbool _ZFP_ZFCoreArrayConvert<T_ZFCoreArray>::to(ZF_IN_OUT ZFCoreArray<zfautoObject> &holder, ZF_OUT ZFCoreArray<T_Type> &v)
{
    typedef T_Type const & T_Type_;
    for(zfindex i = 0; i < holder.count(); ++i)
    {
        if(!ZFTypeId<T_Type>::template Value<T_Type_>::accessAvailable(holder[i]))
        {
            return zffalse;
        }
        v.add(ZFTypeId<T_Type>::template Value<T_Type_>::access(holder[i]));
        ZFTypeId<T_Type>::template Value<T_Type_>::accessFinish(holder[i]);
    }
    return zftrue;
}

template<typename T_ZFCoreArray>
template<typename T_Type>
zfbool _ZFP_ZFCoreArrayConvert<T_ZFCoreArray>::accessAvailable(ZF_IN_OUT zfautoObject &obj)
{
    v_ZFCoreArray *holder = ZFCastZFObject(v_ZFCoreArray *, obj);
    if(holder == zfnull)
    {
        return zffalse;
    }
    typedef T_Type const & T_Type_;
    for(zfindex i = 0; i < holder->zfv.count(); ++i)
    {
        if(!ZFTypeId<T_Type>::template Value<T_Type_>::accessAvailable(holder->zfv[i]))
        {
            return zffalse;
        }
    }
    return zftrue;
}

template<typename T_ZFCoreArray>
template<typename T_Type>
T_ZFCoreArray *_ZFP_ZFCoreArrayConvert<T_ZFCoreArray>::access(ZF_IN_OUT zfautoObject &obj, ZF_IN _ZFP_PropAliasDetachCallback detachCallback)
{
    v_ZFCoreArray *holder = ZFCastZFObjectUnchecked(v_ZFCoreArray *, obj);
    T_ZFCoreArray *tmp = zfnew(T_ZFCoreArray);
    _ZFP_ZFCoreArrayConvert<T_ZFCoreArray>::template to<T_Type>(holder->zfv, *tmp);
    _ZFP_PropAliasAttach(obj, tmp, "ZFCoreArray", detachCallback);
    return tmp;
}

template<typename T_ZFCoreArray>
template<typename T_Type>
void _ZFP_ZFCoreArrayConvert<T_ZFCoreArray>::accessFinish(ZF_IN_OUT zfautoObject &obj)
{
    _ZFP_PropAliasDetach(obj, "ZFCoreArray");
}

// ============================================================
template<typename T_Type>
zfbool _ZFP_ZFCoreArrayConvert<ZFCoreArray<zfautoObject> >::from(ZF_OUT ZFCoreArray<zfautoObject> &holder, ZF_IN const ZFCoreArray<zfautoObject> &v)
{
    holder = v;
    return zftrue;
}

template<typename T_Type>
zfbool _ZFP_ZFCoreArrayConvert<ZFCoreArray<zfautoObject> >::to(ZF_IN_OUT ZFCoreArray<zfautoObject> &holder, ZF_OUT ZFCoreArray<zfautoObject> &v)
{
    v = holder;
    return zftrue;
}

template<typename T_Type>
zfbool _ZFP_ZFCoreArrayConvert<ZFCoreArray<zfautoObject> >::accessAvailable(ZF_IN_OUT zfautoObject &obj)
{
    return (ZFCastZFObject(v_ZFCoreArray *, obj) != zfnull);
}

template<typename T_Type>
ZFCoreArray<zfautoObject> *_ZFP_ZFCoreArrayConvert<ZFCoreArray<zfautoObject> >::access(ZF_IN_OUT zfautoObject &obj, ZF_IN _ZFP_PropAliasDetachCallback detachCallback)
{
    return &(ZFCastZFObjectUnchecked(v_ZFCoreArray *, obj)->zfv);
}

template<typename T_Type>
void _ZFP_ZFCoreArrayConvert<ZFCoreArray<zfautoObject> >::accessFinish(ZF_IN_OUT zfautoObject &obj)
{
}

/** @endcond */

// ============================================================
/** @brief convert array from string */
template<typename T_Type>
zfbool ZFCoreArrayFromString(ZF_OUT ZFCoreArray<T_Type> &v,
                             ZF_IN zfbool (*elementFromString)(
                                 ZF_OUT T_Type &,
                                 ZF_IN const zfchar *,
                                 ZF_IN_OPT zfindex),
                             ZF_IN const zfchar *src,
                             ZF_IN_OPT zfindex srcLen = zfindexMax())
{
    ZFCoreArrayPOD<ZFIndexRange> pos;
    if(!zfCoreDataPairSplitString(pos, zfindexMax(), src, srcLen, ",", "[", "]", zftrue))
    {
        return zffalse;
    }
    for(zfindex i = 0; i < pos.count(); ++i)
    {
        zfstring elementString;
        zfCoreDataDecode(elementString, zfstring(src + pos[i].start, pos[i].count));
        zftValue<T_Type> t;
        if(!elementFromString(t.zfv, elementString.cString(), elementString.length()))
        {
            return zffalse;
        }
        v.add(t.zfv);
    }
    return zftrue;
}
/** @brief convert array to string */
template<typename T_Type>
zfbool ZFCoreArrayToString(ZF_OUT zfstring &s,
                           ZF_IN zfbool (*elementToString)(
                               ZF_OUT zfstring &,
                               ZF_IN T_Type const &),
                           ZF_IN ZFCoreArray<T_Type> const &v)
{
    s += "[";
    for(zfindex i = 0; i < v.count(); ++i)
    {
        if(i != 0)
        {
            s += ", ";
        }

        zfstring elementString;
        if(!elementToString(elementString, v[i]))
        {
            return zffalse;
        }
        zfCoreDataEncode(s, elementString.cString(), elementString.length());
    }
    s += "]";
    return zftrue;
}
/** @brief convert array to string */
template<typename T_Type>
zfstring ZFCoreArrayToString(ZF_IN ZFCoreArray<T_Type> const &v,
                             ZF_IN zfbool (*elementToString)(
                                 ZF_OUT zfstring &ret,
                                 ZF_IN T_Type const &))
{
    zfstring ret;
    if(ZFCoreArrayToString(ret, v, elementToString))
    {
        return ret;
    }
    else
    {
        return zfstring();
    }
}

// ============================================================
/** @brief convert array from serializable data */
template<typename T_Type>
zfbool ZFCoreArrayFromData(ZF_OUT ZFCoreArray<T_Type> &v,
                           ZF_IN zfbool (*elementFromData)(
                               ZF_OUT T_Type &,
                               ZF_IN const ZFSerializableData &,
                               ZF_OUT_OPT zfstring *outErrorHint,
                               ZF_OUT_OPT ZFSerializableData *),
                           ZF_IN const ZFSerializableData &serializableData,
                           ZF_OUT_OPT zfstring *outErrorHint = zfnull,
                           ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull)
{
    v.removeAll();

    if(!ZFSerializableUtil::requireItemClass(serializableData, ZFTypeId_ZFCoreArray(), outErrorHint, outErrorPos))
    {
        return zffalse;
    }

    for(zfindex i = 0; i < serializableData.elementCount(); ++i)
    {
        const ZFSerializableData &element = serializableData.elementAtIndex(i);
        if(element.resolved())
        {
            continue;
        }

        zftValue<T_Type> t;
        if(!elementFromData(t.zfv, element, outErrorHint, outErrorPos))
        {
            return zffalse;
        }
        v.add(t.zfv);
    }

    return zftrue;
}
/** @brief convert array to serializable data */
template<typename T_Type>
zfbool ZFCoreArrayToData(ZF_OUT ZFSerializableData &serializableData,
                         ZF_IN zfbool (*elementToData)(
                             ZF_OUT ZFSerializableData &,
                             ZF_IN T_Type const &,
                             ZF_OUT_OPT zfstring *outErrorHint),
                         ZF_IN ZFCoreArray<T_Type> const &v,
                         ZF_OUT_OPT zfstring *outErrorHint = zfnull)
{
    serializableData.itemClassSet(ZFTypeId_ZFCoreArray());
    for(zfindex i = 0; i < v.count(); ++i)
    {
        ZFSerializableData element;
        if(!elementToData(element, v[i], outErrorHint))
        {
            return zffalse;
        }
        serializableData.elementAdd(element);
    }
    return zftrue;
}
/** @brief convert array to serializable data */
template<typename T_Type>
ZFSerializableData ZFCoreArrayToData(ZF_IN ZFCoreArray<T_Type> const &v,
                                     ZF_IN zfbool (*elementToData)(
                                         ZF_OUT ZFSerializableData &,
                                         ZF_IN T_Type const &,
                                         ZF_OUT_OPT zfstring *outErrorHint),
                                     ZF_OUT_OPT zfstring *outErrorHint = zfnull)
{
    ZFSerializableData ret;
    if(ZFCoreArrayToData(ret, elementToData, v, outErrorHint))
    {
        return ret;
    }
    else
    {
        return ZFSerializableData();
    }
}

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFTypeId_ZFCoreArray_h_

