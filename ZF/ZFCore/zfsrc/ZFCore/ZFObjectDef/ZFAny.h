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
 * @file ZFAny.h
 * @brief util class to cast ZFObject types freely
 */

#ifndef _ZFI_ZFAny_h_
#define _ZFI_ZFAny_h_

#include "ZFObjectCast.h"
ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd ZFObject;
zfclassFwd ZFInterface;

ZFM_CLASS_HAS_MEMBER_DECLARE(ZFAny, toObject, ZFObject *(T::*F)(void))
template<typename T_Type, int isZFObject>
zfclassNotPOD _ZFP_ZFAnyCastT
{
public:
    static ZFObject *c(ZF_IN T_Type obj)
    {
        return (ZFObject *)obj;
    }
};
template<typename T_Type>
zfclassNotPOD _ZFP_ZFAnyCastT<T_Type, 1>
{
public:
    static ZFObject *c(ZF_IN T_Type obj)
    {
        return obj.toObject();
    }
};
#define _ZFP_ZFAnyCast(T_Type, obj) \
    (_ZFP_ZFAnyCastT<T_Type, ZFM_CLASS_HAS_MEMBER(ZFAny, toObject, T_Type)>::c(obj))

// ============================================================
// ZFAny
/**
 * @brief util method to cast ZFObject types freely
 *
 * usage:
 * @code
 *   YourObjectType *obj = ...;
 *   ZFAny any = obj;
 *
 *   // cast to any other object type, return null if not able to cast
 *   AnotherObjectType *another = any;
 *
 *   // or use toObject
 *   ZFObject *orig = any.toObject();
 * @endcode
 */
zfclassLikePOD ZF_ENV_EXPORT ZFAny
{
    /** @cond ZFPrivateDoc */
public:
    ZFAny(void)
    : _obj(zfnull)
    {
    }
    template<typename T_ZFObject>
    ZFAny(ZF_IN T_ZFObject *obj)
    : _obj(obj ? obj->toObject() : zfnull)
    {
    }
    template<typename T_ZFObject>
    ZFAny(ZF_IN T_ZFObject const &obj)
    : _obj(_ZFP_ZFAnyCast(T_ZFObject, obj))
    {
    }

public:
    template<typename T_ZFObject>
    ZFAny &operator = (ZF_IN T_ZFObject *obj)
    {
        this->_obj = (obj ? obj->toObject() : zfnull);
        return *this;
    }
    template<typename T_ZFObject>
    ZFAny &operator = (ZF_IN T_ZFObject const &obj)
    {
        this->_obj = _ZFP_ZFAnyCast(T_ZFObject, obj);
        return *this;
    }

public:
    template<typename T_ZFObject>
    zfbool operator == (ZF_IN T_ZFObject *obj) const
    {
        return (this->_obj == obj);
    }
    template<typename T_ZFObject>
    zfbool operator != (ZF_IN T_ZFObject *obj) const
    {
        return (this->_obj != obj);
    }
    template<typename T_ZFObject>
    zfbool operator == (ZF_IN T_ZFObject const &obj) const
    {
        return (this->_obj == _ZFP_ZFAnyCast(T_ZFObject, obj));
    }
    template<typename T_ZFObject>
    zfbool operator != (ZF_IN T_ZFObject const &obj) const
    {
        return (this->_obj != _ZFP_ZFAnyCast(T_ZFObject, obj));
    }

public:
    ZFObject *operator -> (void) const
    {
        return this->toObject();
    }
    operator bool (void) const
    {
        return (this->_obj != zfnull);
    }
    template<typename T_ZFObject>
    inline operator T_ZFObject * (void) const
    {
        return ZFCastZFObject(T_ZFObject *, this->_obj);
    }
    /** @endcond */

public:
    /**
     * @brief get the holded object
     */
    inline ZFObject *toObject(void) const
    {
        return this->_obj;
    }
    /**
     * @brief cast by #ZFCastZFObjectUnchecked
     */
    template<typename T_ZFObject>
    inline T_ZFObject to(void) const
    {
        return ZFCastZFObjectUnchecked(T_ZFObject, this->_obj);
    }

private:
    ZFObject *_obj;
};

// ============================================================
// ZFAnyT
/**
 * @brief util method to cast ZFObject types freely, see #ZFAny
 *
 * usage:
 * @code
 *   YourObjectType *obj = ...;
 *   ZFAnyT<BaseObjectType *> any = obj;
 *
 *   // cast to any other object type
 *   // AnotherObjectType must be child of BaseObjectType
 *   // else, compile error
 *   AnotherObjectType *another = any;
 *
 *   // or use toObject
 *   ZFObject *orig = any.toObject();
 * @endcode
 */
template<typename T_ZFObjectBase>
zfclassLikePOD ZF_ENV_EXPORT ZFAnyT
{
    /** @cond ZFPrivateDoc */
public:
    ZFAnyT(void)
    : _obj(zfnull)
    {
    }
    template<typename T_ZFObject>
    ZFAnyT(ZF_IN const ZFAnyT<T_ZFObject *> &ref)
    : _obj(ref.toObjectT())
    {
    }
    template<typename T_ZFObject>
    ZFAnyT(ZF_IN T_ZFObject *obj)
    : _obj(obj)
    {
    }
    template<typename T_ZFObject>
    ZFAnyT(ZF_IN T_ZFObject const &obj)
    : _obj(ZFCastZFObject(T_ZFObjectBase, _ZFP_ZFAnyCast(T_ZFObject, obj)))
    {
    }

public:
    template<typename T_ZFObject>
    ZFAnyT<T_ZFObjectBase> &operator = (ZF_IN const ZFAnyT<T_ZFObject *> &ref)
    {
        this->_obj = ref.toObjectT();
        return *this;
    }
    template<typename T_ZFObject>
    ZFAnyT<T_ZFObjectBase> &operator = (ZF_IN T_ZFObject *obj)
    {
        this->_obj = obj;
        return *this;
    }
    template<typename T_ZFObject>
    ZFAnyT<T_ZFObjectBase> &operator = (ZF_IN T_ZFObject const &obj)
    {
        this->_obj = ZFCastZFObject(T_ZFObjectBase, _ZFP_ZFAnyCast(T_ZFObject, obj));
        return *this;
    }

public:
    template<typename T_ZFObject>
    zfbool operator == (ZF_IN const ZFAnyT<T_ZFObject *> &ref) const
    {
        return (this->_obj == ref.toObjectT());
    }
    template<typename T_ZFObject>
    zfbool operator != (ZF_IN const ZFAnyT<T_ZFObject *> &ref) const
    {
        return (this->_obj != ref.toObjectT());
    }
    template<typename T_ZFObject>
    zfbool operator == (ZF_IN T_ZFObject *obj) const
    {
        return (this->_obj == obj);
    }
    template<typename T_ZFObject>
    zfbool operator != (ZF_IN T_ZFObject *obj) const
    {
        return (this->_obj != obj);
    }
    template<typename T_ZFObject>
    zfbool operator == (ZF_IN T_ZFObject const &obj) const
    {
        return (this->_obj == ZFCastZFObject(T_ZFObjectBase, _ZFP_ZFAnyCast(T_ZFObject, obj)));
    }
    template<typename T_ZFObject>
    zfbool operator != (ZF_IN T_ZFObject const &obj) const
    {
        return (this->_obj != ZFCastZFObject(T_ZFObjectBase, _ZFP_ZFAnyCast(T_ZFObject, obj)));
    }

public:
    T_ZFObjectBase operator -> (void) const
    {
        return this->toObjectT();
    }
    operator bool (void) const
    {
        return (this->toObject() != zfnull);
    }
    template<typename T_ZFObject>
    inline operator T_ZFObject * (void) const
    {
        return ZFCastZFObjectUnchecked(ZFM_EXPAND(
            typename _ZFP_ZFAnyTypeChecker<T_ZFObject *,
                    zftTypeIsTypeOf<
                            typename zftTraits<T_ZFObject>::TrType,
                            typename zftTraits<T_ZFObjectBase>::TrType
                        >::TypeIsTypeOf
                >::TypeMatched),
            this->_obj);
    }
    inline ZFObject *toObject(void) const
    {
        return (this->_obj ? this->_obj->toObject() : zfnull);
    }
    template<typename T_ZFObject>
    inline T_ZFObject to(void) const
    {
        return ZFCastZFObjectUnchecked(ZFM_EXPAND(
            typename _ZFP_ZFAnyTypeChecker<T_ZFObject,
                    zftTypeIsTypeOf<
                            typename zftTraits<T_ZFObject>::TrType,
                            typename zftTraits<T_ZFObjectBase>::TrType
                        >::TypeIsTypeOf
                >::TypeMatched),
            this->_obj);
    }
    inline T_ZFObjectBase toObjectT(void) const
    {
        return this->_obj;
    }
    /** @endcond */

private:
    template<typename T_ZFObject, int canCast>
    zfclassNotPOD _ZFP_ZFAnyTypeChecker
    {
    };
    template<typename T_ZFObject>
    zfclassNotPOD _ZFP_ZFAnyTypeChecker<T_ZFObject, 1>
    {
    public:
        typedef T_ZFObject TypeMatched;
    };
private:
    T_ZFObjectBase _obj;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFAny_h_

