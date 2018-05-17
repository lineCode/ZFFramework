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
 * @file ZFEncrypt.h
 * @brief encrypt util
 */

#ifndef _ZFI_ZFEncrypt_h_
#define _ZFI_ZFEncrypt_h_

#include "ZFAlgorithmDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief encrypt util, result value is ensured to have printable chars only
 *
 * which encrypt algorithm would be used, is depends on ZFEncrypt protocol
 */
ZFMETHOD_FUNC_DECLARE_3(zfbool, ZFEncrypt,
                        ZFMP_IN_OUT(const ZFOutput &, output),
                        ZFMP_IN(const ZFInput &, input),
                        ZFMP_IN(const zfchar *, encryptKey))
/** @brief see #ZFEncrypt */
ZFMETHOD_FUNC_DECLARE_3(zfbool, ZFDecrypt,
                        ZFMP_IN_OUT(const ZFOutput &, output),
                        ZFMP_IN(const ZFInput &, input),
                        ZFMP_IN(const zfchar *, encryptKey))

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFEncrypt_h_

