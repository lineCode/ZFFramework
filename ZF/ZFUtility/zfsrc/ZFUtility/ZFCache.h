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
    ZFOBJECT_DECLARE(ZFCache, ZFObject)

public:
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when #cacheAdd, to check whether cache is valid,
     * param0 is the cached object,
     * param1 is a #ZFValueEditable holds bool value to show whether the cache is valid
     */
    ZFOBSERVER_EVENT(CacheOnCheck)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when #cacheAdd,
     * param0 is the cached object
     */
    ZFOBSERVER_EVENT(CacheOnAdd)

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
    /**
     * @brief called to check whether the cache is valid
     */
    virtual zfbool cacheOnCheck(ZF_IN ZFObject *cacheValue)
    {
        if(this->observerHasAdd(ZFCache::EventCacheOnCheck()))
        {
            zfautoObject cacheValid = ZFValueEditable::boolValueCreate(zftrue);
            this->observerNotify(ZFCache::EventCacheOnCheck(), cacheValue, cacheValid);
            return cacheValid.to<ZFValue *>()->boolValue();
        }
        else
        {
            return zftrue;
        }
    }
    /**
     * @brief called when cache would be added
     */
    virtual inline void cacheOnAdd(ZF_IN ZFObject *cacheValue)
    {
        this->observerNotify(ZFCache::EventCacheOnAdd(), cacheValue);
    }

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
 *   ZFCACHEHOLDER_DEFINE(YourClass, {
 *       // cache holder setup
 *       cacheHolder->cacheMaxSizeSet(100);
 *
 *       // setup cache reset action if necessary
 *       ZFLISTENER_LOCAL(cacheOnAdd, {
 *           listenerData.param0->to<YourCacheType *>()->resetYourCache();
 *       })
 *       cacheHolder->observerAdd(ZFCache::EventCacheOnAdd(), cacheOnAdd);
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

