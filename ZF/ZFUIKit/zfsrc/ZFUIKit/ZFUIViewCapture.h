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
 * @file ZFUIViewCapture.h
 * @brief capture ZFUIView to ZFUIImage
 */

#ifndef _ZFI_ZFUIViewCapture_h_
#define _ZFI_ZFUIViewCapture_h_

#include "ZFUIView.h"
#include "ZFUIImage.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief capture ZFUIView to ZFUIImage
 */
ZFMETHOD_FUNC_DECLARE_1(zfautoObject, ZFUIViewCapture,
                        ZFMP_IN(ZFUIView *, view))

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIViewCapture_h_

