/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFUIListAdapter.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFOBJECT_REGISTER(ZFUIListAdapter)

ZFOBSERVER_EVENT_REGISTER(ZFUIListAdapter, ListCellOnUpdate)
ZFOBSERVER_EVENT_REGISTER(ZFUIListAdapter, ListCellCacheOnRecycle)

#define _ZFP_ZFUIListAdapter_cacheKey(cacheKey, key) \
    zfchar *cacheKey = zfsConnect(zfText("_ZFP_ZFUIListAdapter_cacheKey"), key); \
    zfblockedFree(cacheKey)
zfautoObject ZFUIListAdapter::cellCacheDefaultAccess(ZF_IN const zfchar *key)
{
    _ZFP_ZFUIListAdapter_cacheKey(cacheKey, key);
    ZFArrayEditable *cacheList = this->toObject()->tagGet<ZFArrayEditable *>(cacheKey);
    if(cacheList != zfnull && !cacheList->isEmpty())
    {
        zfautoObject ret = cacheList->getLast();
        cacheList->removeLast();
        return ret;
    }
    else
    {
        return zfautoObjectNull();
    }
}
void ZFUIListAdapter::cellCacheDefaultRecycle(ZF_IN const zfchar *key, ZF_IN ZFUIListCell *cell)
{
    _ZFP_ZFUIListAdapter_cacheKey(cacheKey, key);
    ZFArrayEditable *cacheList = this->toObject()->tagGet<ZFArrayEditable *>(cacheKey);
    if(cacheList == zfnull)
    {
        cacheList = zfAlloc(ZFArrayEditable);
        this->toObject()->tagSet(cacheKey, cacheList);
        zfRelease(cacheList);
    }
    cacheList->add(cell);
}

ZF_NAMESPACE_GLOBAL_END

