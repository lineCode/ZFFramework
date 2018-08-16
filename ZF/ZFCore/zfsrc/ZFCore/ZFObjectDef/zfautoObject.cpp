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

// ============================================================
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
            zfpoolDelete(d);
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
            zfpoolDelete(dTmp);
        }
    }
    zfCoreMutexUnlock();
    return *this;
}
void _ZFP_zfautoObjectAssign(ZF_IN_OUT _ZFP_zfautoObjectPrivate *&d,
                             ZF_IN ZFObject *obj)
{
    zfCoreMutexLock();
    zflockfree_zfRetain(obj);
    if(d)
    {
        ZFObject *objTmp = d->obj;
        if(d->refCount == 1)
        {
            d->obj = obj;
            zflockfree_zfRelease(objTmp);
        }
        else
        {
            --(d->refCount);
            if(obj)
            {
                d = zfpoolNew(_ZFP_zfautoObjectPrivate, obj);
            }
            else
            {
                d = zfnull;
            }
        }
    }
    else if(obj)
    {
        d = zfpoolNew(_ZFP_zfautoObjectPrivate, obj);
    }
    zfCoreMutexUnlock();
}

const zfautoObject _ZFP_zfautoObjectNull;

// ============================================================
void _ZFP_zfautoObjectTError(void)
{
    zfCoreCriticalMessageTrim(zfTextA("[zfautoObjectT] cast from incompatible type"));
}

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_USER_REGISTER_0({return zfnull;}, zfautoObject, zfautoObjectNull)

ZF_NAMESPACE_GLOBAL_END
#endif

