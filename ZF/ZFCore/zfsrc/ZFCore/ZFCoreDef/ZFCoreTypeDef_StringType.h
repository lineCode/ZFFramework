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
 * @file ZFCoreTypeDef_StringType.h
 * @brief types for ZFFramework
 */

#ifndef _ZFI_ZFCoreTypeDef_StringType_h_
#define _ZFI_ZFCoreTypeDef_StringType_h_

#include "ZFCoreTypeDef_CoreType.h"
#include "ZFCoreTypeDef_CharType.h"
#include "ZFCoreString.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/** @brief see #_zfstr */
#ifndef _ZFT_zfstringA
    typedef ZFCoreStringA zfstringA;
#else
    typedef _ZFT_zfstringA zfstringA;
#endif

/** @brief see #_zfstr */
#ifndef _ZFT_zfstringW
    typedef ZFCoreStringW zfstringW;
#else
    typedef _ZFT_zfstringW zfstringW;
#endif

/** @brief see #_zfstr */
#ifndef _ZFT_zfstring
    typedef ZFCoreString zfstring;
#else
    typedef _ZFT_zfstring zfstring;
#endif

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFCoreTypeDef_StringType_h_

