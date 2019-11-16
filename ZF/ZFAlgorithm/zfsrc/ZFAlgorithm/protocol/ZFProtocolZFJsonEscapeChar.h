/**
 * @file ZFProtocolZFJsonEscapeChar.h
 * @brief protocol for ZFJson
 */

#ifndef _ZFI_ZFProtocolZFJsonEscapeChar_h_
#define _ZFI_ZFProtocolZFJsonEscapeChar_h_

#include "ZFCore/ZFProtocol.h"
#include "ZFAlgorithm/ZFJson.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for ZFJson
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFJsonEscapeChar)
    public:
    /**
     * @brief see #ZFJsonEscapeCharEncode
     */
    virtual void jsonEscapeCharEncode(ZF_OUT const ZFOutput &dst,
                                      ZF_IN const zfchar *src,
                                      ZF_IN_OPT zfindex count = zfindexMax()) zfpurevirtual;
    /**
     * @brief see #ZFJsonEscapeCharEncode
     */
    virtual void jsonEscapeCharDecode(ZF_OUT const ZFOutput &dst,
                                      ZF_IN const zfchar *src,
                                      ZF_IN_OPT zfindex count = zfindexMax()) zfpurevirtual;
ZFPROTOCOL_INTERFACE_END(ZFJsonEscapeChar)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFJsonEscapeChar_h_

