/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFUIListCellUpdater.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFUIListCellUpdater)

#define _ZFP_ZFUIListCellUpdater_cacheKey(cacheKey, key) \
    zfchar *cacheKey = zfsConnect(zfText("_ZFP_ZFUIListCellUpdater_cacheKey"), key); \
    zfblockedFree(cacheKey)
zfautoObject ZFUIListCellUpdater::itemCacheAccess(ZF_IN const zfchar *key)
{
    _ZFP_ZFUIListCellUpdater_cacheKey(cacheKey, key);
    ZFArrayEditable *cacheList = this->tagGet<ZFArrayEditable *>(cacheKey);
    if(cacheList != zfnull && !cacheList->isEmpty())
    {
        zfautoObject ret = cacheList->getLast();
        cacheList->removeLast();
        return ret;
    }
    else
    {
        return zfnull;
    }
}
void ZFUIListCellUpdater::itemCacheRecycle(ZF_IN const zfchar *key, ZF_IN ZFObject *cache)
{
    _ZFP_ZFUIListCellUpdater_cacheKey(cacheKey, key);
    ZFArrayEditable *cacheList = this->tagGet<ZFArrayEditable *>(cacheKey);
    if(cacheList == zfnull)
    {
        cacheList = zfAlloc(ZFArrayEditable);
        this->tagSet(cacheKey, cacheList);
        zfRelease(cacheList);
    }
    cacheList->add(cache);
}

ZF_NAMESPACE_GLOBAL_END

