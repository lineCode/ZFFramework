/**
 * @file ZFProtocolZFXmlEscapeChar.h
 * @brief protocol for ZFXml
 */

#ifndef _ZFI_ZFProtocolZFXmlEscapeChar_h_
#define _ZFI_ZFProtocolZFXmlEscapeChar_h_

#include "ZFCore/ZFProtocol.h"
#include "ZFAlgorithm/ZFXml.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for ZFXml
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFXmlEscapeChar)
public:
    /**
     * @brief see #ZFXmlEscapeCharEncode
     */
    virtual void xmlEscapeCharEncode(ZF_OUT const ZFOutput &dst,
                                     ZF_IN const zfchar *src,
                                     ZF_IN_OPT zfindex count = zfindexMax()) zfpurevirtual;
    /**
     * @brief see #ZFXmlEscapeCharEncode
     */
    virtual void xmlEscapeCharDecode(ZF_OUT const ZFOutput &dst,
                                     ZF_IN const zfchar *src,
                                     ZF_IN_OPT zfindex count = zfindexMax()) zfpurevirtual;
ZFPROTOCOL_INTERFACE_END(ZFXmlEscapeChar)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFXmlEscapeChar_h_

