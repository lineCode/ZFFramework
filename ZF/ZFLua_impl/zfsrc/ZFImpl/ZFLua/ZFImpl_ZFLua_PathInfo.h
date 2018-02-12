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
 * @file ZFImpl_ZFLua_PathInfo.h
 * @brief path info impl
 */

#ifndef _ZFI_ZFImpl_ZFLua_PathInfo_h_
#define _ZFI_ZFImpl_ZFLua_PathInfo_h_

#include "ZFImpl_ZFLua.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief setup path info for lua execute
 */
extern ZF_ENV_EXPORT void ZFImpl_ZFLua_implSetupPathInfo(ZF_OUT zfstring &ret,
                                                         ZF_IN const ZFPathInfo *pathInfo);

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFImpl_ZFLua_PathInfo_h_

