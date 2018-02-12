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
 * @file ZFProtocolZFFilePath.h
 * @brief protocol for ZFFile
 */

#ifndef _ZFI_ZFProtocolZFFilePath_h_
#define _ZFI_ZFProtocolZFFilePath_h_

#include "ZFCore/ZFProtocol.h"
#include "ZFCore/ZFFile.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for ZFFile
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFFilePath)
public:
    /**
     * @brief see #ZFFilePathForModule
     */
    virtual const zfchar *pathForModule(void) zfpurevirtual;
    /**
     * @brief see #ZFFilePathForModuleFile
     */
    virtual const zfchar *pathForModuleFile(void) zfpurevirtual;

    /**
     * @brief see #ZFFilePathForSetting
     */
    virtual const zfchar *pathForSetting(void) zfpurevirtual;
    /**
     * @brief see #ZFFilePathForSettingSet
     */
    virtual void pathForSettingSet(ZF_IN const zfchar *path = zfnull) zfpurevirtual;

    /**
     * @brief see #ZFFilePathForStorage
     */
    virtual const zfchar *pathForStorage(void) zfpurevirtual;
    /**
     * @brief see #ZFFilePathForStorageSet
     */
    virtual void pathForStorageSet(ZF_IN const zfchar *path = zfnull) zfpurevirtual;

    /**
     * @brief see #ZFFilePathForStorageShared
     */
    virtual const zfchar *pathForStorageShared(void) zfpurevirtual;
    /**
     * @brief see #ZFFilePathForStorageSharedSet
     */
    virtual void pathForStorageSharedSet(ZF_IN const zfchar *path = zfnull) zfpurevirtual;

    /**
     * @brief see #ZFFilePathForCache
     */
    virtual const zfchar *pathForCache(void) zfpurevirtual;
    /**
     * @brief see #ZFFilePathForCacheSet
     */
    virtual void pathForCacheSet(ZF_IN const zfchar *path = zfnull) zfpurevirtual;
    /**
     * @brief see #ZFFilePathForCacheClear
     */
    virtual void pathForCacheClear(void) zfpurevirtual;
ZFPROTOCOL_INTERFACE_END(ZFFilePath)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFFilePath_h_

