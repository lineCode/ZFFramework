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
 * @file ZFObjectIO_zfsd.h
 * @brief IO type for #ZFObjectIOLoad
 */

#ifndef _ZFI_ZFObjectIO_zfsd_h_
#define _ZFI_ZFObjectIO_zfsd_h_

#include "ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief see #ZFObjectIOLoad
 *
 * for "*.zfsd" file, which should contain data converted by #ZFObjectToOutput
 */
#define ZFObjectIO_zfsd zfText("zfsd")

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFObjectIO_zfsd_h_

