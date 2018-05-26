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
 * @file ZFResCache.h
 * @brief utility to load serializable from resource with cache logic
 */

#ifndef _ZFI_ZFResCache_h_
#define _ZFI_ZFResCache_h_

#include "ZFObjectCache.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief cache for serializable from resources
 *
 * note: res cache would be clean up and disabled during #ZFFrameworkCleanup
 * of level #ZFLevelZFFrameworkPostNormal to prevent dependency broken,
 * by setting #ZFObjectCache::cacheMaxSize to zero
 */
zfclass ZF_ENV_EXPORT ZFResCache : zfextends ZFObjectCache
{
    ZFOBJECT_DECLARE(ZFResCache, ZFObjectCache)
    ZFOBJECT_SINGLETON_DECLARE(ZFResCache, instance)

    ZFPROPERTY_OVERRIDE_ON_INIT_INLINE(zfindex, cacheMaxSize)
    {
        propertyValue = 100;
    }
};

/**
 * @brief load resource by #ZFObjectIOLoad and cache if enabled
 *
 * cache would be stored in #ZFResCache,
 * you should not modified the returned object if cache enabled
 */
ZFMETHOD_FUNC_DECLARE_2(zfautoObject, zfRes,
                        ZFMP_IN(const zfchar *, resFilePath),
                        ZFMP_IN_OPT(zfbool, enableCache, zftrue))
/**
 * @brief see #zfRes
 */
ZFMETHOD_FUNC_DECLARE_INLINE_1(zfautoObject, zfResNoCache,
                               ZFMP_IN(const zfchar *, resFilePath))
{
    return zfRes(resFilePath, zffalse);
}

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFResCache_h_

