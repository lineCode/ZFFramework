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
 * @file ZFCache.h
 * @brief util object to hold #ZFObject as cache
 */

#ifndef _ZFI_ZFCache_h_
#define _ZFI_ZFCache_h_

#include "ZFUtilityDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassFwd _ZFP_ZFCachePrivate;
/**
 * @brief util object to hold #ZFObject as cache
 */
zfclass ZF_ENV_EXPORT ZFCache : zfextends ZFObject
{
    ZFOBJECT_DECLARE_WITH_CUSTOM_CTOR(ZFCache, ZFObject)

public:
    /** @brief callback for impl */
    typedef zfbool (*CacheOnAddImpl)(ZF_IN ZFObject *cache);
    /** @brief callback for impl */
    typedef zfbool (*CacheOnCheckImpl)(ZF_IN ZFObject *cache);

public:
    /** @brief callback for impl */
    ZFCache::CacheOnAddImpl cacheOnAddImpl;
    /** @brief callback for impl */
    ZFCache::CacheOnCheckImpl cacheOnCheckImpl;

protected:
    /** @cond ZFPrivateDoc */
    ZFCache(void) : cacheOnAddImpl(zfnull), cacheOnCheckImpl(zfnull) {}
    /** @endcond */

public:
    /**
     * @brief max cache size, 10 by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfindex, cacheMaxSize, 10)
    ZFPROPERTY_OVERRIDE_ON_ATTACH_DECLARE(zfindex, cacheMaxSize)

    /**
     * @brief whether invoke #cacheTrim when receive #ZFGlobalEvent::EventAppOnMemoryLow, true by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfbool, cacheTrimWhenReceiveMemoryWarning, zftrue)
    ZFPROPERTY_OVERRIDE_ON_ATTACH_DECLARE(zfbool, cacheTrimWhenReceiveMemoryWarning)

    /**
     * @brief leave how many cache alive while #cacheTrim, 3 by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfindex, cacheTrimThreshold, 3)

public:
    /**
     * @brief add cache
     *
     * if cache exceeds #cacheMaxSize,
     * the cacheValue would be discarded
     */
    ZFMETHOD_DECLARE_1(void, cacheAdd,
                       ZFMP_IN(ZFObject *, cacheValue))
    /**
     * @brief access cache, or return #zfautoObjectNull if not exist
     *
     * if no cache available and cacheClass is not null,
     * we will create a new instance and return using the cacheClass
     */
    ZFMETHOD_DECLARE_1(zfautoObject, cacheGet,
                       ZFMP_IN_OPT(const ZFClass *, cacheClass, zfnull))

    /**
     * @brief remove all cache
     */
    ZFMETHOD_DECLARE_0(void, cacheRemoveAll)
    /**
     * @brief trim the cache to reduce memory
     *
     * by default, this method would call #cacheTrimBySize
     */
    ZFMETHOD_DECLARE_0(void, cacheTrim)
    /**
     * @brief util method to trim the cache
     *   so that the cached data won't exceeds specified size
     */
    ZFMETHOD_DECLARE_1(void, cacheTrimBySize,
                       ZFMP_IN(zfindex , size))

public:
    /**
     * @brief access all cache data, used for debug only
     */
    ZFMETHOD_DECLARE_0(ZFCoreArray<zfautoObject>, cacheGetAll)
    /**
     * @brief see #cacheGetAll
     */
    ZFMETHOD_DECLARE_1(void, cacheGetAllT,
                       ZFMP_IN_OUT(ZFCoreArray<zfautoObject> &, ret))

protected:
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);
    zfoverride
    virtual void objectOnDeallocPrepare(void);

private:
    _ZFP_ZFCachePrivate *d;
};

// ============================================================
/**
 * @brief util to declare a #ZFCache
 *
 * usage:
 * @code
 *   // in header file
 *   zfclass YourClass : zfextends ZFObject
 *   {
 *       ZFCACHEHOLDER_DECLARE()
 *   };
 *
 *   // in cpp file
 *   static zfbool myCacheOnAdd(ZF_IN ZFObject *cacheValue) {...}
 *   static zfbool myCacheOnCheck(ZF_IN ZFObject *cacheValue) {...}
 *   ZFCACHEHOLDER_DEFINE(YourClass, {
 *       // cache holder setup
 *       cacheHolder->cacheMaxSizeSet(100);
 *
 *       // setup cache setup action if necessary
 *       cacheHolder->cacheOnAddImpl = myCacheOnAdd;
 *       cacheHolder->cacheOnCheckImpl = myCacheOnCheck;
 *   })
 *
 *   // the macro would declare this reflectable #ZFMethod for you
 *   static ZFCache *cacheHolder(void);
 * @endcode
 */
#define ZFCACHEHOLDER_DECLARE() \
    /** @brief cache holder, see #ZFCACHEHOLDER_DECLARE */ \
    ZFMETHOD_DECLARE_DETAIL_0(public, ZFMethodTypeStatic, \
                              ZFCache *, cacheHolder)

/** @brief see #ZFCACHEHOLDER_DECLARE */
#define ZFCACHEHOLDER_DEFINE(OwnerClass, ...) \
    ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFCacheHolder_##OwnerClass, ZFLevelZFFrameworkEssential) \
    { \
        cacheHolder = zfAlloc(ZFCache); \
        { \
            __VA_ARGS__ \
        } \
    } \
    ZF_GLOBAL_INITIALIZER_DESTROY(ZFCacheHolder_##OwnerClass) \
    { \
        zfRelease(cacheHolder); \
    } \
    ZFCache *cacheHolder; \
    ZF_GLOBAL_INITIALIZER_END(ZFCacheHolder_##OwnerClass) \
    ZFMETHOD_DEFINE_0(OwnerClass, ZFCache *, cacheHolder) \
    { \
        return ZF_GLOBAL_INITIALIZER_INSTANCE(ZFCacheHolder_##OwnerClass)->cacheHolder; \
    }

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFCache_h_

