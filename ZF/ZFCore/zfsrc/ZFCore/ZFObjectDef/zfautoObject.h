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
zfautoObject::zfautoObject(ZF_IN T_ZFObject const &p)
{
    zfCoreMutexLock();
    ZFObject *obj = p.toObject();
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
zfautoObject &zfautoObject::operator = (ZF_IN T_ZFObject *p)
{
    zfCoreMutexLock();
    ZFObject *obj = (p ? p->toObject() : zfnull);
    zflockfree_zfRetain(obj);
    if(d)
    {
        --(d->refCount);
        if(d->refCount == 0)
        {
            zflockfree_zfRelease(d->obj);
            zfdelete(d);
        }
    }
    if(obj)
    {
        d = zfnew(_ZFP_zfautoObjectPrivate);
        d->obj = obj;
    }
    else
    {
        d = zfnull;
    }
    zfCoreMutexUnlock();
    return *this;
}
template<typename T_ZFObject>
zfautoObject &zfautoObject::operator = (ZF_IN T_ZFObject const &p)
{
    zfCoreMutexLock();
    ZFObject *obj = p.toObject();
    zflockfree_zfRetain(obj);
    if(d)
    {
        --(d->refCount);
        if(d->refCount == 0)
        {
            zflockfree_zfRelease(d->obj);
            zfdelete(d);
        }
    }
    if(obj)
    {
        d = zfnew(_ZFP_zfautoObjectPrivate);
        d->obj = obj;
    }
    else
    {
        d = zfnull;
    }
    zfCoreMutexUnlock();
    return *this;
}
/** @endcond */

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_zfautoObject_h_

