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
 * @file ZFObjectIO_image.h
 * @brief IO type for #ZFObjectIOLoad
 */

#ifndef _ZFI_ZFObjectIO_image_h_
#define _ZFI_ZFObjectIO_image_h_

#include "ZFUIImageIO.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief see #ZFObjectIOLoad
 *
 * for "*.png *.jpg *.jpeg" file, raw image files\n
 * additional image extension can be added by #ZFObjectIO_image_imageExtAdd
 */
#define ZFObjectIO_image zfText("image")

// ============================================================
/**
 * @brief see #ZFObjectIO_image
 *
 * add file ext that should be treated as raw image IO type\n
 * default value:
 * -  "png"
 * -  "jpg"
 * -  "jpeg"
 */
extern ZF_ENV_EXPORT void ZFObjectIO_image_imageExtAdd(ZF_IN const zfchar *imageExt);
/** @brief see #ZFObjectIO_image_imageExtAdd */
extern ZF_ENV_EXPORT void ZFObjectIO_image_imageExtRemove(ZF_IN const zfchar *imageExt);
/** @brief see #ZFObjectIO_image_imageExtAdd */
extern ZF_ENV_EXPORT void ZFObjectIO_image_imageExtGetAllT(ZF_OUT ZFCoreArrayPOD<const zfchar *> &ret);
/** @brief see #ZFObjectIO_image_imageExtAdd */
inline ZFCoreArrayPOD<const zfchar *> ZFObjectIO_image_imageExtGetAll(void)
{
    ZFCoreArrayPOD<const zfchar *> ret;
    ZFObjectIO_image_imageExtGetAllT(ret);
    return ret;
}

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFObjectIO_image_h_

