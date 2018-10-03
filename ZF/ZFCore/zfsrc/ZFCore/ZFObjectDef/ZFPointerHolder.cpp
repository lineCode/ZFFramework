/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFPointerHolder.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFPointerHolder
ZFOBJECT_REGISTER(ZFPointerHolder)
#define _ZFP_ZFPointerHolderCacheSize 20
static ZFPointerHolder *_ZFP_ZFPointerHolderCache[_ZFP_ZFPointerHolderCacheSize];
static ZFPointerHolder **_ZFP_ZFPointerHolderCacheAvailable = _ZFP_ZFPointerHolderCache;
static ZFPointerHolder **_ZFP_ZFPointerHolderCacheAvailableEnd = _ZFP_ZFPointerHolderCache + _ZFP_ZFPointerHolderCacheSize;
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFPointerHolderCacheHolder, ZFLevelZFFrameworkEssential)
{
    zfmemset(_ZFP_ZFPointerHolderCache, 0, sizeof(_ZFP_ZFPointerHolderCache));
    _ZFP_ZFPointerHolderCacheAvailable = _ZFP_ZFPointerHolderCache;
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFPointerHolderCacheHolder)
{
    while(_ZFP_ZFPointerHolderCacheAvailable > _ZFP_ZFPointerHolderCache)
    {
        zfRelease(*_ZFP_ZFPointerHolderCacheAvailable);
        --_ZFP_ZFPointerHolderCacheAvailable;
    }
}
ZF_GLOBAL_INITIALIZER_END(ZFPointerHolderCacheHolder)
ZFPointerHolder *ZFPointerHolder::cacheGet(void)
{
    ZFPointerHolder *ret = zfnull;
    zfCoreMutexLock();
    if(_ZFP_ZFPointerHolderCacheAvailable > _ZFP_ZFPointerHolderCache)
    {
        ret = *(_ZFP_ZFPointerHolderCacheAvailable - 1);
        --_ZFP_ZFPointerHolderCacheAvailable;
    }
    else
    {
        ret = zfAlloc(ZFPointerHolder);
    }
    zfCoreMutexUnlock();
    return ret;
}
void ZFPointerHolder::cacheAdd(ZF_IN ZFPointerHolder *obj)
{
    zfCoreMutexLock();
    obj->holdedData = zfnull;
    obj->tagRemoveAll();
    if(_ZFP_ZFPointerHolderCacheAvailable == _ZFP_ZFPointerHolderCacheAvailableEnd)
    {
        zfRelease(obj);
    }
    else
    {
        *_ZFP_ZFPointerHolderCacheAvailable = obj;
        ++_ZFP_ZFPointerHolderCacheAvailable;
    }
    zfCoreMutexUnlock();
}

void ZFPointerHolder::objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
{
    zfsuper::objectInfoOnAppend(ret);
    ret += " ";
    zfsFromPointerT(ret, this->holdedData);
}
zfidentity ZFPointerHolder::objectHash(void)
{
    return zfidentityCalcPointer(this->holdedData);
}
ZFCompareResult ZFPointerHolder::objectCompare(ZF_IN ZFObject *anotherObj)
{
    if(this == anotherObj) {return ZFCompareTheSame;}
    zfself *another = ZFCastZFObject(zfself *, anotherObj);
    if(another == zfnull) {return ZFCompareUncomparable;}

    if(this->holdedData == another->holdedData)
    {
        return ZFCompareTheSame;
    }
    else
    {
        return ZFCompareUncomparable;
    }
}

ZF_NAMESPACE_GLOBAL_END

