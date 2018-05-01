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
 * @file ZFPointerHolder.h
 * @brief utilities for ZFObject
 */

#ifndef _ZFI_ZFPointerHolder_h_
#define _ZFI_ZFPointerHolder_h_

#include "ZFObjectCore.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFPointerHolder
/**
 * @brief used to hold a non ZFObject type for performance
 */
zffinal zfclass ZF_ENV_EXPORT ZFPointerHolder : zfextends ZFObject
{
    ZFOBJECT_DECLARE_WITH_CUSTOM_CTOR(ZFPointerHolder, ZFObject)

protected:
    ZFPointerHolder(void) : holdedData(zfnull) {}

protected:
    /**
     * @brief init with pointer
     */
    virtual void objectOnInit(ZF_IN const void *holdedData)
    {
        this->objectOnInit();
        this->holdedData = holdedData;
    }
    zfoverride
    virtual void objectOnInit(void)
    {
        zfsuper::objectOnInit();
    }

public:
    /**
     * @brief used to hold the raw pointer,
     *   you must ensure it's valid during the holder object's life cycle
     */
    const void *holdedData;

public:
    /** @brief util method to cast #holdedData */
    template<typename T_RawType>
    inline T_RawType holdedDataPointer(void)
    {
        return ZFCastStatic(T_RawType, this->holdedData);
    }
    /** @brief util method to cast #holdedData */
    template<typename T_RawType>
    inline T_RawType holdedDataRef(void)
    {
        typedef typename zftTraits<T_RawType>::TrConstPtr T_RawTypeConstPointer;
        return *ZFCastStatic(T_RawTypeConstPointer, this->holdedData);
    }

public:
    /**
     * @brief access cached object for performance
     *
     * should be paired with #cacheAdd
     */
    static ZFPointerHolder *cacheGet(void);
    /**
     * @brief release the cached object
     *
     * after calling this method, you must not access the obj any more by any chances
     */
    static void cacheAdd(ZF_IN ZFPointerHolder *obj);

protected:
    zfoverride
    virtual void objectInfoOnAppend(ZF_IN_OUT zfstring &ret);
public:
    zfoverride
    virtual zfidentity objectHash(void);
    zfoverride
    virtual ZFCompareResult objectCompare(ZF_IN ZFObject *anotherObj);
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFPointerHolder_h_

