/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFCoreLog_CommonLog.h"
#include "ZFCoreSPrintf.h"

ZF_NAMESPACE_GLOBAL_BEGIN

void _ZFP_zfCoreLogCriticalMessage(ZF_IN const ZFCallerInfo &callerInfo,
                                   ZF_IN const zfchar *fmt,
                                   ...)
{
    va_list vaList;
    va_start(vaList, fmt);
    _ZFP_zfCoreLogCriticalMessageV(callerInfo, fmt, vaList);
    va_end(vaList);
}
void _ZFP_zfCoreLogCriticalMessageV(ZF_IN const ZFCallerInfo &callerInfo,
                                    ZF_IN const zfchar *fmt,
                                    ZF_IN va_list vaList)
{
    zfCoreLogTrim("============================================================");
    zfCoreLogTrimNoEndl("| ");
    zfCoreLogDetailV(callerInfo, fmt, vaList);
    zfCoreLogTrim("============================================================");
}
void _ZFP_zfCoreCritical(ZF_IN const ZFCallerInfo &callerInfo,
                         ZF_IN const zfchar *fmt,
                         ...)
{
    va_list vaList;
    va_start(vaList, fmt);
    _ZFP_zfCoreCriticalV(callerInfo, fmt, vaList);
    va_end(vaList);
}
void _ZFP_zfCoreCriticalV(ZF_IN const ZFCallerInfo &callerInfo,
                          ZF_IN const zfchar *fmt,
                          ZF_IN va_list vaList)
{
    zfCoreCriticalErrorPrepareDetail(callerInfo);
    zfCoreLogCriticalMessageDetailV(callerInfo, fmt, vaList);
    zfCoreCriticalErrorDetail(callerInfo);
}

ZF_NAMESPACE_GLOBAL_END

