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
 * @file zfstl_string.h
 * @brief stl wrapper, note it's internal use only
 */

#ifndef _ZFI_zfstl_string_h_
#define _ZFI_zfstl_string_h_

#include "zfstl_define.h"
#include <string>

// ============================================================
/** @brief stl wrapper */
#ifndef zfstlstring
    #define zfstlstring std::string
#endif

/** @brief stl wrapper */
#ifndef zfstlbasicstring
    #define zfstlbasicstring std::basic_string
#endif

// ============================================================
/** @brief zfchar version of stl string */
#ifndef zfstlstringZ
    #define zfstlstringZ zfstlbasicstring<zfchar>
#endif

#endif // #ifndef _ZFI_zfstl_string_h_

