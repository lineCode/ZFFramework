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
 * @file ZFTimeLineProperty_ZFCore4.h
 * @brief see #ZFTimeLineProperty
 */
#ifndef _ZFI_ZFTimeLineProperty_ZFCore4_h_
#define _ZFI_ZFTimeLineProperty_ZFCore4_h_

#include "ZFTimeLineProperty.h"
// ZFTAG_DOXYGEN_BUG
// this file was separated from ZFTimeLineProperty_ZFCore.h
// because of doxygen can not parse large macro generated file
// the original error message was:
//   "input buffer overflow, can't enlarge buffer because scanner uses REJECT"
ZF_NAMESPACE_GLOBAL_BEGIN

/** @cond ZFPrivateDoc */
#if defined(_MSC_VER)
    // implicit convert from float to int64
    #pragma warning(push)
    #pragma warning(disable:4244)
#endif
/** @endcond */
ZFTIMELINEPROPERTY_DECLARE_BY_VALUE(zftimet)
/** @cond ZFPrivateDoc */
#if defined(_MSC_VER)
    #pragma warning(pop)
#endif
/** @endcond */

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFTimeLineProperty_ZFCore4_h_

