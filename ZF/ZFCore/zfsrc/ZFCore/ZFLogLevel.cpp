/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFLogLevel.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFENUM_DEFINE(ZFLogLevel)

ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
ZFOBSERVER_EVENT_GLOBAL_REGISTER(LogLevelOnChange)
ZF_NAMESPACE_END(ZFGlobalEvent)

static ZFLogLevelEnum _ZFP_ZFLogLevelGlobal = ZFLogLevel::EnumDefault();
void ZFLogLevelSet(ZF_IN ZFLogLevelEnum level)
{
    _ZFP_ZFLogLevelGlobal = level;
    if(ZFFrameworkStateCheck(ZFLevelZFFrameworkEssential) == ZFFrameworkStateAvailable)
    {
        ZFGlobalEventCenter::instance()->observerNotify(ZFGlobalEvent::EventLogLevelOnChange());
    }
}
ZFLogLevelEnum ZFLogLevelGet(void)
{
    return _ZFP_ZFLogLevelGlobal;
}
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFLogLevelReset, ZFLevelZFFrameworkEssential)
{
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFLogLevelReset)
{
    _ZFP_ZFLogLevelGlobal = ZFLogLevel::EnumDefault();
}
ZF_GLOBAL_INITIALIZER_END(ZFLogLevelReset)

ZF_NAMESPACE_GLOBAL_END

