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
 * @file ZFObjectIO_lua.h
 * @brief IO type for #ZFObjectIOLoad
 */

#ifndef _ZFI_ZFObjectIO_lua_h_
#define _ZFI_ZFObjectIO_lua_h_

#include "ZFLuaExecute.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief see #ZFObjectIOLoad
 *
 * for "*.lua" file, which should be executed by #ZFLuaExecute
 * and return an object
 */
#define ZFObjectIO_lua zfText("lua")

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFObjectIO_lua_h_

