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
 * @file ZFPathType_common.h
 * @brief common #ZFPathInfo
 */

#ifndef _ZFI_ZFPathType_common_h_
#define _ZFI_ZFPathType_common_h_

#include "ZFPathType_file.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief see #ZFPathInfo
 *
 * pathData is file's relative path within #ZFFilePathForModule
 * @note path must be well formed, use #ZFFilePathFormat if necessary
 */
#define ZFPathType_modulePath "modulePath"

/**
 * @brief see #ZFPathInfo
 *
 * pathData is file's relative path within #ZFFilePathForStorage
 * @note path must be well formed, use #ZFFilePathFormat if necessary
 */
#define ZFPathType_storagePath "storagePath"

/**
 * @brief see #ZFPathInfo
 *
 * pathData is file's relative path within #ZFFilePathForStorageShared
 * @note path must be well formed, use #ZFFilePathFormat if necessary
 */
#define ZFPathType_storageSharedPath "storageSharedPath"

/**
 * @brief see #ZFPathInfo
 *
 * pathData is file's relative path within #ZFFilePathForCache
 *
 * note, the cache path's content would be removed accorrding to #ZFFilePathForCacheClear
 * @note path must be well formed, use #ZFFilePathFormat if necessary
 */
#define ZFPathType_cachePath "cachePath"

/**
 * @brief see #ZFPathInfo
 *
 * pathData is plain text
 */
#define ZFPathType_text "text"

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFPathType_common_h_

