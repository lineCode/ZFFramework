/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFImpl_default_ZFAlgorithm_impl.h"
#include "ZFAlgorithm/protocol/ZFProtocolZFJsonEscapeChar.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFJsonEscapeCharImpl_default, ZFJsonEscapeChar, ZFProtocolLevel::e_Default)
public:
    virtual void jsonEscapeCharEncode(ZF_OUT const ZFOutput &dst,
                                      ZF_IN const zfchar *src,
                                      ZF_IN_OPT zfindex count = zfindexMax())
    {
        for(const zfchar *srcEnd = src + ((count == zfindexMax()) ? zfslen(src) : count); src != srcEnd; ++src)
        {
            if(*src == '\"')
            {
                dst << "\\\"";
            }
            else if(*src == '\\')
            {
                dst << "\\\\";
            }
            else if(*src == '\n')
            {
                dst << "\\n";
            }
            else if(*src == '\r')
            {
                dst << "\\r";
            }
            else if(*src == '\t')
            {
                dst << "\\t";
            }
            else if(*src == '\f')
            {
                dst << "\\f";
            }
            else if(*src == '\b')
            {
                dst << "\\b";
            }
            else
            {
                dst << *src;
            }
        }
    }
    virtual void jsonEscapeCharDecode(ZF_OUT const ZFOutput &dst,
                                      ZF_IN const zfchar *src,
                                      ZF_IN_OPT zfindex count = zfindexMax())
    {
        for(const zfchar *srcEnd = src + ((count == zfindexMax()) ? zfslen(src) : count); src != srcEnd; ++src)
        {
            if(*src == '\\' && src + 1 != srcEnd)
            {
                ++src;
                if(*src == '\"')
                {
                    dst << '\"';
                }
                else if(*src == '\\')
                {
                    dst << '\\';
                }
                else if(*src == 'n')
                {
                    dst << '\n';
                }
                else if(*src == 'r')
                {
                    dst << '\r';
                }
                else if(*src == 't')
                {
                    dst << '\t';
                }
                else if(*src == 'f')
                {
                    dst << '\f';
                }
                else if(*src == 'b')
                {
                    dst << '\b';
                }
                else
                {
                    dst << '\\';
                    dst << *src;
                }
            }
            else
            {
                dst << *src;
            }
        }
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFJsonEscapeCharImpl_default)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFJsonEscapeCharImpl_default)

ZF_NAMESPACE_GLOBAL_END

