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
 * @file ZFProtocolZFOutput.h
 * @brief protocol for output logs,
 *   used by zfCoreLog, ZFLog, etc,
 *   as default output callback
 */

#ifndef _ZFI_ZFProtocolZFOutput_h_
#define _ZFI_ZFProtocolZFOutput_h_

#include "ZFCore/ZFProtocol.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for output logs
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFOutput)
ZFPROTOCOL_INTERFACE_OPTIONAL
public:
    /**
     * @brief output callback for zfCoreLog,
     *   usually as critical error messages
     *
     * within this method,
     * you must not access any other ZFFramework's method
     */
    virtual void outputCoreLog(ZF_IN const zfcharA *s) zfpurevirtual;
    /**
     * @brief output callback for ZFLog
     */
    virtual void outputLog(ZF_IN const zfchar *s, ZF_IN_OPT zfindex count = zfindexMax()) zfpurevirtual;
ZFPROTOCOL_INTERFACE_END(ZFOutput)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFOutput_h_

