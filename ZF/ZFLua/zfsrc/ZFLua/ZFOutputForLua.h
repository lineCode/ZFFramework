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
 * @file ZFOutputForLua.h
 * @brief util to create #ZFOutput for lua
 */

#ifndef _ZFI_ZFOutputForLua_h_
#define _ZFI_ZFOutputForLua_h_

#include "ZFLuaDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief util to create #ZFOutput for lua
 *
 * luaCallback's param0 would be a #v_zfstring,
 * and param1 holds a #v_zfindex that indicates the #ZFOutput's return value
 * @code
 *   ZFOutputForLua(function(listenerData, userData)
 *       local text = listenerData:param0()
 *       local result = yourOutput(text)
 *       listenerData:param1():zfvSet(result)
 *   end)
 * @endcode
 */
ZFMETHOD_FUNC_DECLARE_2(ZFOutput, ZFOutputForLua,
                        ZFMP_IN(const ZFListener &, luaCallback),
                        ZFMP_IN_OPT(ZFObject *, userData, zfnull))

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFOutputForLua_h_

