/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFImpl_default_ZFCore.h"
#include "ZFCore/protocol/ZFProtocolZFOutput.h"
#include "ZFCore/ZFString.h"
#include <stdio.h>

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFOutputImpl_default, ZFOutput, ZFProtocolLevel::e_Default)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT(zfText("C:printf"))
public:
    virtual void outputCoreLog(ZF_IN const zfcharA *s)
    {
        printf(zfTextA("%s"), s);
    }
    virtual void outputLog(ZF_IN const zfchar *s, ZF_IN_OPT zfindex count = zfindexMax())
    {
        if(count == zfindexMax())
        {
            printf(zfTextA("%s"), ZFStringZ2A(s));
        }
        else
        {
            printf(zfTextA("%s"), ZFStringZ2A(zfstring(s, count).cString()));
        }
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFOutputImpl_default)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFOutputImpl_default)

ZF_NAMESPACE_GLOBAL_END

