/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFOutputForConsole.h"
#include "protocol/ZFProtocolZFOutput.h"
#include "ZFString.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFCALLBACK_SERIALIZE_CUSTOM_TYPE_DEFINE(ZFOutputForConsole, ZFCallbackSerializeCustomType_ZFOutputForConsole)
{
    ret = ZFOutputForConsole();
    serializableData.resolveMark();
    return ret.callbackIsValid();
}

// ============================================================
// ZFOutputForConsole
static zfindex _ZFP_ZFOutputForConsoleFunction(ZF_IN const void *s, ZF_IN zfindex count)
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
static ZFOutput _ZFP_ZFOutputForConsole_create(void)
{
    ZFOutput ret = ZFCallbackForFunc(_ZFP_ZFOutputForConsoleFunction);
    ret.callbackSerializeCustomTypeSet(ZFCallbackSerializeCustomType_ZFOutputForConsole);
    ret.callbackSerializeCustomDataSet(ZFSerializableData());
    return ret;
}

ZFEXPORT_VAR_READONLY_DEFINE(ZFOutput, ZFOutputForConsole, _ZFP_ZFOutputForConsole_create())
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFOutputForConsoleInit, ZFLevelZFFrameworkEssential)
{
    ZFExportVarEnsureInit_ZFOutputForConsole();
    ZFOutputDefaultSet(ZFOutputForConsole());
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFOutputForConsoleInit)
{
    ZFOutputDefaultSet(ZFCallbackNull());
}
ZF_GLOBAL_INITIALIZER_END(ZFOutputForConsoleInit)

ZF_NAMESPACE_GLOBAL_END

