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

// ============================================================
ZFMETHOD_DEFINE_2(ZFUIListAdapter, zfint, cellSizeAtIndex,
                  ZFMP_IN(zfindex, index),
                  ZFMP_IN(ZFUIListCell *, cell))
{
    if(this->cellSizeFill())
    {
        switch(this->listOrientation())
        {
            case ZFUIOrientation::e_Left:
            case ZFUIOrientation::e_Right:
                return this->listContainerSize().width;
            case ZFUIOrientation::e_Top:
            case ZFUIOrientation::e_Bottom:
                return this->listContainerSize().height;
            default:
                zfCoreCriticalShouldNotGoHere();
                return -1;
        }
    }
    else
    {
        return this->cellSizeHint();
    }
}

// ============================================================
#define _ZFP_ZFUIListAdapter_cacheKey(cacheKey, key) \
    zfchar *cacheKey = zfsConnect(zfText("_ZFP_ZFUIListAdapter_cacheKey"), key); \
    zfblockedFree(cacheKey)
ZFMETHOD_DEFINE_1(ZFUIListAdapter, zfautoObject, cellCacheDefaultAccess,
                  ZFMP_IN(const zfchar *, key))
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
        return zfnull;
    }
}
ZFMETHOD_DEFINE_2(ZFUIListAdapter, void, cellCacheDefaultRecycle,
                  ZFMP_IN(const zfchar *, key),
                  ZFMP_IN(ZFUIListCell *, cell))
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

// ============================================================
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFUIListAdapter, zfindex, cellCount)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFUIListAdapter, zfautoObject, cellAtIndex, ZFMP_IN(zfindex, index))

ZF_NAMESPACE_GLOBAL_END

