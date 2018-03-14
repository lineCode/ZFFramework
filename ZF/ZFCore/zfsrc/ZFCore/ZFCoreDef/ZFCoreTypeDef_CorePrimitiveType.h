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
 * @file ZFCoreTypeDef_CorePrimitiveType.h
 * @brief types for ZFFramework
 */

#ifndef _ZFI_ZFCoreTypeDef_CorePrimitiveType_h_
#define _ZFI_ZFCoreTypeDef_CorePrimitiveType_h_

#include "ZFCoreEnvDef.h"
#include <stddef.h>

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// must ensure these types differs from each other
/** @cond ZFPrivateDoc */
ZF_ENV_SENSITIVE("must ensure these types differs from each other")
#define _ZFT_zftrue true
#define _ZFT_zffalse false
typedef bool                    _ZFT_zfbool;
typedef char                    _ZFT_zfcharA;
typedef wchar_t                 _ZFT_zfcharW;
typedef unsigned char           _ZFT_zfbyte;
typedef int                     _ZFT_zfint;
typedef unsigned int            _ZFT_zfuint;
typedef unsigned long int       _ZFT_zfindex;
typedef float                   _ZFT_zffloat;
/** @endcond */
// must ensure these types differs from each other
// ============================================================

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFCoreTypeDef_CorePrimitiveType_h_

