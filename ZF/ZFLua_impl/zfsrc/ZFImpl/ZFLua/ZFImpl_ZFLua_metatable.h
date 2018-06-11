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
 * @file ZFImpl_ZFLua_metatable.h
 * @brief zfautoObject metatable impl
 */

#ifndef _ZFI_ZFImpl_ZFLua_metatable_h_
#define _ZFI_ZFImpl_ZFLua_metatable_h_

#include "ZFImpl_ZFLua.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief setup metatable for zfautoObject in lua env
 */
extern ZF_ENV_EXPORT void ZFImpl_ZFLua_implSetupObject_metatable(ZF_IN_OUT lua_State *L, ZF_IN_OPT int objIndex = -1);

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFImpl_ZFLua_metatable_h_

