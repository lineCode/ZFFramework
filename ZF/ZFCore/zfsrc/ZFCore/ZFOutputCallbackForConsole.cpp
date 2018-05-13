/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFOutputCallbackForConsole.h"
#include "protocol/ZFProtocolZFOutput.h"
#include "ZFString.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFCALLBACK_SERIALIZE_CUSTOM_TYPE_DEFINE(ZFOutputCallbackForConsole, ZFCallbackSerializeCustomType_ZFOutputCallbackForConsole)
{
    ret = ZFOutputCallbackForConsole();
    serializableData.resolveMark();
    return ret.callbackIsValid();
}

// ============================================================
// ZFOutputCallbackForConsole
static zfindex _ZFP_ZFOutputCallbackForConsoleFunction(ZF_IN const void *s, ZF_IN zfindex count)
{
    ZFPROTOCOL_INTERFACE_CLASS(ZFOutput) *impl = ZFPROTOCOL_TRY_ACCESS(ZFOutput);
    zfindex sLen = ((count == zfindexMax()) ? zfslen((const zfchar *)s) : (count / sizeof(zfchar)));
    if(impl == zfnull)
    {
        if(sLen == zfindexMax())
        {
            printf(zfTextA("%s"), ZFStringZ2A(s));
        }
        else
        {
            zfstring tmp((const zfchar *)s, sLen);
            printf(zfTextA("%s"), ZFStringZ2A(tmp.cString()));
        }
    }
    else
    {
        impl->outputLog((const zfchar *)s, sLen);
    }
    return sLen * sizeof(zfchar);
}
static ZFOutputCallback _ZFP_ZFOutputCallbackForConsole_create(void)
{
    ZFOutputCallback ret = ZFCallbackForFunc(_ZFP_ZFOutputCallbackForConsoleFunction);
    ret.callbackSerializeCustomTypeSet(ZFCallbackSerializeCustomType_ZFOutputCallbackForConsole);
    ret.callbackSerializeCustomDataSet(ZFSerializableData());
    return ret;
}

ZFEXPORT_VAR_READONLY_DEFINE(ZFOutputCallback, ZFOutputCallbackForConsole, _ZFP_ZFOutputCallbackForConsole_create())
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFOutputCallbackForConsoleInit, ZFLevelZFFrameworkEssential)
{
    ZFExportVarEnsureInit_ZFOutputCallbackForConsole();
    ZFOutputCallbackDefaultSet(ZFOutputCallbackForConsole());
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFOutputCallbackForConsoleInit)
{
    ZFOutputCallbackDefaultSet(ZFCallbackNull());
}
ZF_GLOBAL_INITIALIZER_END(ZFOutputCallbackForConsoleInit)

ZF_NAMESPACE_GLOBAL_END

