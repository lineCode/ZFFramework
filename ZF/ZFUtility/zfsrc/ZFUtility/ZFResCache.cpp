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

ZFMETHOD_FUNC_DEFINE_2(zfautoObject, zfRes,
                       ZFMP_IN(const zfchar *, resFilePath),
                       ZFMP_IN_OPT(zfbool, enableCache, zftrue))
{
    zfautoObject ret;
    ZFInput input = ZFInputForResFile(resFilePath);
    zfbool cacheAvailable = !zfsIsEmpty(input.callbackId());

    if(cacheAvailable)
    {
        ret = ZFResCache::instance()->cacheGet(input.callbackId());
        if(ret != zfnull)
        {
            return ret;
        }
    }

    if(!ZFObjectIOLoad(ret, input))
    {
        return zfnull;
    }
    if(enableCache && cacheAvailable)
    {
        ZFResCache::instance()->cacheAdd(input.callbackId(), ret);
    }
    return ret;
}

ZFMETHOD_FUNC_DEFINE_INLINE_1(zfautoObject, zfResNoCache,
                              ZFMP_IN(const zfchar *, resFilePath))

ZF_NAMESPACE_GLOBAL_END

