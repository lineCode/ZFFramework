/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFLog.h"
#include "ZFLog_StackTrace.h"
#include "ZFString.h"

#include "protocol/ZFProtocolZFOutput.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
static void _ZFP_zfCoreLogZFLogWrapper(ZF_IN const zfcharA *s)
{
    if(ZFPROTOCOL_TRY_ACCESS(ZFOutput) != zfnull)
    {
        ZFPROTOCOL_ACCESS(ZFOutput)->outputCoreLog(s);
    }
    else
    {
        // try to print to std output
        fprintf(stderr, zfTextA("%s"), s);
    }
}

static void _ZFP_ZFLog_ZFCoreLogExtFunc(ZF_IN const ZFCallerInfo &callerInfo)
{
    if(zfLogStackTraceAvailable())
    {
        zfCoreLogTrim(ZFStringZ2A(zfLogStackTrace(zfnull, 2).cString()));
    }
}

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFLog_ZFCoreLogExt, ZFLevelZFFrameworkEssential)
{
    zfCoreLogOutputCallbackSet(_ZFP_zfCoreLogZFLogWrapper);
    zfCoreCriticalErrorCallbackAdd(_ZFP_ZFLog_ZFCoreLogExtFunc);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFLog_ZFCoreLogExt)
{
    zfCoreCriticalErrorCallbackRemove(_ZFP_ZFLog_ZFCoreLogExtFunc);
}
ZF_GLOBAL_INITIALIZER_END(ZFLog_ZFCoreLogExt)

ZF_NAMESPACE_GLOBAL_END

