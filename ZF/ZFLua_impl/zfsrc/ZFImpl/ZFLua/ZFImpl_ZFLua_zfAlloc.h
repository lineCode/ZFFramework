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
 * @file ZFImpl_ZFLua_zfAlloc.h
 * @brief lua impl
 */

#ifndef _ZFI_ZFImpl_ZFLua_zfAlloc_h_
#define _ZFI_ZFImpl_ZFLua_zfAlloc_h_

#include "ZFImpl_ZFLua.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief for impl to alloc object
 */
extern ZF_ENV_EXPORT zfbool ZFImpl_ZFLua_zfAlloc(ZF_OUT zfautoObject &ret,
                                                 ZF_IN lua_State *L,
                                                 ZF_IN const ZFClass *cls,
                                                 ZF_IN int paramCount,
                                                 ZF_IN int luaParamOffset);

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFImpl_ZFLua_zfAlloc_h_

