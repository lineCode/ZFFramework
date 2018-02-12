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
 * @file ZFProtocolZFEncrypt.h
 * @brief protocol for ZFEncrypt
 */

#ifndef _ZFI_ZFProtocolZFEncrypt_h_
#define _ZFI_ZFProtocolZFEncrypt_h_

#include "ZFCore/ZFProtocol.h"
#include "ZFAlgorithm/ZFEncrypt.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for ZFEncrypt
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFEncrypt)
public:
    /**
     * @brief see #ZFEncrypt
     *
     * result must contain printable chars only, encoded with base64 is recommended
     */
    virtual zfbool encrypt(ZF_IN_OUT const ZFOutputCallback &output,
                           ZF_IN const ZFInputCallback &input,
                           ZF_IN const zfchar *key) zfpurevirtual;
    /**
     * @brief see #ZFDecrypt
     */
    virtual zfbool decrypt(ZF_IN_OUT const ZFOutputCallback &output,
                           ZF_IN const ZFInputCallback &input,
                           ZF_IN const zfchar *key) zfpurevirtual;
ZFPROTOCOL_INTERFACE_END(ZFEncrypt)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFEncrypt_h_

