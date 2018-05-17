/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFEncrypt.h"
#include "protocol/ZFProtocolZFEncrypt.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFEncrypt,
                       ZFMP_IN_OUT(const ZFOutput &, output),
                       ZFMP_IN(const ZFInput &, input),
                       ZFMP_IN(const zfchar *, encryptKey))
{
    if(!input.callbackIsValid() || !output.callbackIsValid())
    {
        return zffalse;
    }
    return ZFPROTOCOL_ACCESS(ZFEncrypt)->encrypt(output, input, encryptKey);
}
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFDecrypt,
                       ZFMP_IN_OUT(const ZFOutput &, output),
                       ZFMP_IN(const ZFInput &, input),
                       ZFMP_IN(const zfchar *, encryptKey))
{
    if(!input.callbackIsValid() || !output.callbackIsValid())
    {
        return zffalse;
    }
    return ZFPROTOCOL_ACCESS(ZFEncrypt)->decrypt(output, input, encryptKey);
}

ZF_NAMESPACE_GLOBAL_END

