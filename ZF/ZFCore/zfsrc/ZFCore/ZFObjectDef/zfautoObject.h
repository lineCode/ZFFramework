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

#include "ZFObjectRetain.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/** @cond ZFPrivateDoc */
template<typename T_ZFObject>
zfautoObject::zfautoObject(ZF_IN T_ZFObject *p)
{
    zfCoreMutexLock();
    ZFObject *obj = (p ? p->toObject() : zfnull);
    if(obj)
    {
        d = zfpoolNew(_ZFP_zfautoObjectPrivate,
            zflockfree_zfRetain(obj));
    }
    else
    {
        d = zfnull;
    }
    zfCoreMutexUnlock();
}
template<typename T_ZFObject>
zfautoObject::zfautoObject(ZF_IN T_ZFObject const &p)
{
    zfCoreMutexLock();
    ZFObject *obj = _ZFP_ZFAnyCast(T_ZFObject, p);
    if(obj)
    {
        d = zfpoolNew(_ZFP_zfautoObjectPrivate,
            zflockfree_zfRetain(obj));
    }
    else
    {
        d = zfnull;
    }
    zfCoreMutexUnlock();
}

extern ZF_ENV_EXPORT void _ZFP_zfautoObjectAssign(ZF_IN_OUT _ZFP_zfautoObjectPrivate *&d,
                                                  ZF_IN ZFObject *obj);
template<typename T_ZFObject>
zfautoObject &zfautoObject::operator = (ZF_IN T_ZFObject *p)
{
    _ZFP_zfautoObjectAssign(d, p ? p->toObject() : zfnull);
    return *this;
}
template<typename T_ZFObject>
zfautoObject &zfautoObject::operator = (ZF_IN T_ZFObject const &p)
{
    _ZFP_zfautoObjectAssign(d, _ZFP_ZFAnyCast(T_ZFObject, p));
    return *this;
}
/** @endcond */

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_zfautoObject_h_

