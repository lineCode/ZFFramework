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
 * @file ZFOutputForConsole.h
 * @brief common callback defines
 */

#ifndef _ZFI_ZFOutputForConsole_h_
#define _ZFI_ZFOutputForConsole_h_

#include "ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief see #ZFCALLBACK_SERIALIZE_CUSTOM_TYPE_DEFINE
 *
 * serializable data:
 * @code
 *   <node>
 *   </node>
 * @endcode
 */
#define ZFCallbackSerializeCustomType_ZFOutputForConsole zfText("ZFOutputForConsole")

/**
 * @brief default output callback to output to console, source must be zfchar *string
 */
ZFEXPORT_VAR_READONLY_DECLARE(ZFOutput, ZFOutputForConsole)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFOutputForConsole_h_

