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

ZFOBSERVER_EVENT_REGISTER(ZFUIListCellUpdater, CellOnUpdate)
ZFOBSERVER_EVENT_REGISTER(ZFUIListCellUpdater, CellOnRecycle)

#define _ZFP_ZFUIListCellUpdater_cacheKey(cacheKey, key) \
    zfchar *cacheKey = zfsConnect(zfText("_ZFP_ZFUIListCellUpdater_cacheKey"), key); \
    zfblockedFree(cacheKey)
ZFMETHOD_DEFINE_1(ZFUIListCellUpdater, zfautoObject, itemCacheAccess,
                  ZFMP_IN(const zfchar *, key))
{
    _ZFP_ZFUIListCellUpdater_cacheKey(cacheKey, key);
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
ZFMETHOD_DEFINE_2(ZFUIListCellUpdater, void, itemCacheRecycle,
                  ZFMP_IN(const zfchar *, key),
                  ZFMP_IN(ZFObject *, cache))
{
    _ZFP_ZFUIListCellUpdater_cacheKey(cacheKey, key);
    ZFArrayEditable *cacheList = this->toObject()->tagGet<ZFArrayEditable *>(cacheKey);
    if(cacheList == zfnull)
    {
        cacheList = zfAlloc(ZFArrayEditable);
        this->toObject()->tagSet(cacheKey, cacheList);
        zfRelease(cacheList);
    }
    cacheList->add(cache);
}

// ============================================================
ZFTYPEID_ACCESS_ONLY_DEFINE(ZFUIListCellUpdaterParam, ZFUIListCellUpdaterParam)

ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFUIListCellUpdaterParam, ZFUIListCell *, cell)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFUIListCellUpdaterParam, zfindex, cellIndex)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFUIListCellUpdaterParam, zfindex, cellCount)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFUIListCellUpdaterParam, ZFUIOrientationEnum, listOrientation)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFUIListCellUpdaterParam, ZFUISize, listContainerSize)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFUIListCellUpdaterParam, zfint, cellSizeHint)

ZF_NAMESPACE_GLOBAL_END

