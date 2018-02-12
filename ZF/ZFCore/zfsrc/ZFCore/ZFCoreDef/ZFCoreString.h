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
 * @file ZFCoreString.h
 * @brief tricks to wrap std::string
 */

#ifndef _ZFI_ZFCoreString_h_
#define _ZFI_ZFCoreString_h_

#include "ZFCoreString_impl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/** @brief see #_zfstr */
#ifndef _ZFT_ZFCoreStringA
    typedef ZFCoreStringA_impl ZFCoreStringA;
#else
    typedef _ZFT_ZFCoreStringA ZFCoreStringA;
#endif

/** @brief see #_zfstr */
#ifndef _ZFT_ZFCoreStringW
    typedef ZFCoreStringW_impl ZFCoreStringW;
#else
    typedef _ZFT_ZFCoreStringW ZFCoreStringW;
#endif

/**
 * @brief string types used in ZFFramework
 */
#ifndef _ZFT_ZFCoreString
    #if ZF_ENV_ZFCHAR_USE_CHAR_A
        typedef ZFCoreStringA ZFCoreString;
    #endif
    #if ZF_ENV_ZFCHAR_USE_CHAR_W
        typedef ZFCoreStringW ZFCoreString;
    #endif
#else
    typedef _ZFT_ZFCoreString ZFCoreString;
#endif

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFCoreString_h_

