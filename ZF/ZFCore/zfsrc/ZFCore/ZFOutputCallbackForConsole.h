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
 * @file ZFOutputCallbackForConsole.h
 * @brief common callback defines
 */

#ifndef _ZFI_ZFOutputCallbackForConsole_h_
#define _ZFI_ZFOutputCallbackForConsole_h_

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
#define ZFCallbackSerializeCustomType_ZFOutputCallbackForConsole zfText("ZFOutputCallbackForConsole")

/**
 * @brief default output callback to output to console, source must be zfchar *string
 */
ZFEXPORT_VAR_READONLY_DECLARE(ZFOutputCallback, ZFOutputCallbackForConsole)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFOutputCallbackForConsole_h_

