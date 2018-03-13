/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "zfautoObject.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/** @cond ZFPrivateDoc */
zfautoObject::zfautoObject(ZF_IN zfautoObject const &ref)
{
    zfCoreMutexLock();
    d = ref.d;
    if(d)
    {
        ++(d->refCount);
    }
    zfCoreMutexUnlock();
}
zfautoObject::~zfautoObject(void)
{
    zfCoreMutexLock();
    if(d)
    {
        --(d->refCount);
        if(d->refCount == 0)
        {
            zflockfree_zfRelease(d->obj);
            zfdelete(d);
        }
    }
    zfCoreMutexUnlock();
}

zfautoObject &zfautoObject::operator = (ZF_IN zfautoObject const &ref)
{
    zfCoreMutexLock();
    _ZFP_zfautoObjectPrivate *dTmp = d;
    d = ref.d;
    if(d)
    {
        ++(d->refCount);
    }
    if(dTmp != zfnull)
    {
        --(dTmp->refCount);
        if(dTmp->refCount == 0)
        {
            zflockfree_zfRelease(dTmp->obj);
            zfdelete(dTmp);
        }
    }
    zfCoreMutexUnlock();
    return *this;
}
zfautoObject &zfautoObject::operator = (ZF_IN zft_zfnull obj)
{
    zfCoreMutexLock();
    (void)obj;
    _ZFP_zfautoObjectPrivate *dTmp = d;
    d = zfnull;
    if(dTmp)
    {
        --(dTmp->refCount);
        if(dTmp->refCount == 0)
        {
            zflockfree_zfRelease(dTmp->obj);
            zfdelete(dTmp);
        }
    }
    zfCoreMutexUnlock();
    return *this;
}
/** @endcond */

const zfautoObject _ZFP_zfautoObjectNull;

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_USER_REGISTER_0({return zfautoObjectNull();}, zfautoObject, zfautoObjectNull)

ZF_NAMESPACE_GLOBAL_END
#endif

