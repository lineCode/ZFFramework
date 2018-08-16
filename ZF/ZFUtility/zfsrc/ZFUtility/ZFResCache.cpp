/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFResCache.h"

#include "ZFCore/ZFSTLWrapper/zfstl_map.h"
#include "ZFCore/ZFSTLWrapper/zfstl_string.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFResCache)
ZFOBJECT_SINGLETON_DEFINE_DETAIL(ZFResCache, ZFResCache, ZFResCache, instance, ZFLevelZFFrameworkEssential)

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFResCacheAutoCleanup, ZFLevelZFFrameworkPostNormal)
{
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFResCacheAutoCleanup)
{
    ZFResCache::instance()->cacheMaxSizeSet(0);
    ZFResCache::instance()->cacheRemoveAll();
}
ZF_GLOBAL_INITIALIZER_END(ZFResCacheAutoCleanup)

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFResCacheHolder, ZFLevelZFFrameworkStatic)
{
    this->resOnDeallocListener = ZFCallbackForFunc(zfself::resOnDealloc);
}
zfstlmap<zfstlstringZ, ZFObject *> keyMap;
zfstlmap<ZFObject *, const zfchar *> valueMap;
ZFListener resOnDeallocListener;
static ZFLISTENER_PROTOTYPE_EXPAND(resOnDealloc)
{
    zfCoreMutexLocker();
    ZF_GLOBAL_INITIALIZER_CLASS(ZFResCacheHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFResCacheHolder);
    zfstlmap<ZFObject *, const zfchar *>::iterator itValue = d->valueMap.find(listenerData.sender);
    if(itValue != d->valueMap.end())
    {
        d->keyMap.erase(itValue->second);
        d->valueMap.erase(itValue);
    }
}
ZF_GLOBAL_INITIALIZER_END(ZFResCacheHolder)

// ============================================================
ZFMETHOD_FUNC_DEFINE_2(zfautoObject, zfRes,
                       ZFMP_IN(const zfchar *, resFilePath),
                       ZFMP_IN_OPT(zfbool, enableCache, zftrue))
{
    ZFInput input = ZFInputForResFile(resFilePath);
    if(!input.callbackIsValid())
    {
        return zfnull;
    }
    zfCoreMutexLocker();
    ZF_GLOBAL_INITIALIZER_CLASS(ZFResCacheHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFResCacheHolder);
    if(input.callbackId() != zfnull)
    {
        zfstlmap<zfstlstringZ, ZFObject *>::iterator itKey = d->keyMap.find(input.callbackId());
        if(itKey != d->keyMap.end())
        {
            return itKey->second;
        }
    }
    zfautoObject ret;
    if(!ZFObjectIOLoadT(ret, input))
    {
        return zfnull;
    }
    if(enableCache && input.callbackId() != zfnull && ret != zfnull)
    {
        ZFResCache::instance()->cacheAdd(ret);
        d->keyMap[input.callbackId()] = ret;
        d->valueMap[ret] = d->keyMap.find(input.callbackId())->first.c_str();
        ret->observerAdd(ZFObject::EventObjectBeforeDealloc(), d->resOnDeallocListener);
    }
    return ret;
}

ZFMETHOD_FUNC_DEFINE_INLINE_1(zfautoObject, zfResNoCache,
                              ZFMP_IN(const zfchar *, resFilePath))

ZF_NAMESPACE_GLOBAL_END

