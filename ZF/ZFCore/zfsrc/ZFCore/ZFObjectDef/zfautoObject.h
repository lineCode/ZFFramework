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
 * @file zfautoObject.h
 * @brief smart pointer for ZFObject
 */

#ifndef _ZFI_zfautoObject_h_
#define _ZFI_zfautoObject_h_

#include "ZFObjectCore.h"
#include "ZFObjectRetain.h"
#include "ZFAny.h"
#include "zfautoObjectFwd.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFM_CLASS_HAS_MEMBER_DECLARE(zfautoObject, toObject, ZFObject *(T::*F)(void))
template<typename T, int T_isZFObject>
zfclassNotPOD _ZFP_zfautoObjectWrap
{
public:
    static void assign(ZF_IN_OUT zfautoObject &v, ZF_IN T const &p)
    {
        v = zfautoObject(ZFCastZFObjectUnchecked(ZFObject *, p));
    }
    static zfbool equal(ZF_IN const zfautoObject &v, ZF_IN T const &p)
    {
        return (v.toObject() == ZFCastZFObjectUnchecked(ZFObject *, p));
    }
};
template<typename T>
zfclassNotPOD _ZFP_zfautoObjectWrap<T, 0>
{
public:
    static void assign(ZF_IN_OUT zfautoObject &v, ZF_IN T const &p)
    {
        zfCoreAssert(p == 0);
        v = zfautoObjectNull();
    }
    static zfbool equal(ZF_IN const zfautoObject &v, ZF_IN T const &p)
    {
        return (v.toObject() == zfnull && p == 0);
    }
};
/** @cond ZFPrivateDoc */
template<typename T_ZFObject>
zfautoObject::zfautoObject(ZF_IN T_ZFObject const &p)
{
    zfCoreMutexLock();
    ZFObject *obj = ZFCastZFObjectUnchecked(ZFObject *, p);
    if(obj)
    {
        d = zfnew(_ZFP_zfautoObjectPrivate);
        d->obj = zflockfree_zfRetain(obj);
    }
    else
    {
        d = zfnull;
    }
    zfCoreMutexUnlock();
}
template<typename T_ZFObject>
zfautoObject &zfautoObject::operator = (ZF_IN T_ZFObject const &p)
{
    _ZFP_zfautoObjectWrap<
            T_ZFObject,
            ZFM_CLASS_HAS_MEMBER(zfautoObject, toObject, typename zftTraits<T_ZFObject>::TrType) ? 1 : 0
        >::assign(*this, p);
    return *this;
}
template<typename T_ZFObject>
zfbool zfautoObject::operator == (ZF_IN T_ZFObject const &p) const
{
    return _ZFP_zfautoObjectWrap<
            T_ZFObject,
            ZFM_CLASS_HAS_MEMBER(zfautoObject, toObject, typename zftTraits<T_ZFObject>::TrType) ? 1 : 0
        >::equal(*this, p);
}
/** @endcond */

// ============================================================
// ZFCastZFObject fix for ZFAny
inline ZFObject *_ZFP_ObjCastFromUnknown(zfautoObject const &obj)
{
    return obj.toObject();
}
inline void _ZFP_ObjCastToUnknown(zfautoObject &ret,
                                  ZFObject * const &obj)
{
    ret = obj;
}

inline ZFObject *_ZFP_ObjCastFromUnknownUnchecked(zfautoObject const &obj)
{
    return obj.toObject();
}
inline void _ZFP_ObjCastToUnknownUnchecked(zfautoObject &ret,
                                           ZFObject * const &obj)
{
    ret = obj;
}

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_zfautoObject_h_

