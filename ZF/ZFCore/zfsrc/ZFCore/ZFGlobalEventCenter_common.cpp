/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFGlobalEventCenter_common.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
ZFOBSERVER_EVENT_GLOBAL_REGISTER(AppOnCreate)
ZFOBSERVER_EVENT_GLOBAL_REGISTER(AppOnDestroy)
ZFOBSERVER_EVENT_GLOBAL_REGISTER(AppOnMemoryLow)
ZF_NAMESPACE_END(ZFGlobalEvent)

static void _ZFP_ZFGlobalEventCenter_common_AppOnCreate(void)
{
    ZFGlobalEventCenter::instance()->observerNotify(ZFGlobalEvent::EventAppOnCreate());
}
static void _ZFP_ZFGlobalEventCenter_common_AppOnDestroy(void)
{
    ZFGlobalEventCenter::instance()->observerNotify(ZFGlobalEvent::EventAppOnDestroy());
}

ZF_STATIC_REGISTER_INIT(_ZFP_ZFGlobalEventCenter_common_register)
{
    ZFFrameworkInitFinishCallbacks.add(_ZFP_ZFGlobalEventCenter_common_AppOnCreate);
    ZFFrameworkCleanupPrepareCallbacks.add(_ZFP_ZFGlobalEventCenter_common_AppOnDestroy);
}
ZF_STATIC_REGISTER_DESTROY(_ZFP_ZFGlobalEventCenter_common_register)
{
    ZFFrameworkInitFinishCallbacks.removeElement(_ZFP_ZFGlobalEventCenter_common_AppOnCreate);
    ZFFrameworkCleanupPrepareCallbacks.removeElement(_ZFP_ZFGlobalEventCenter_common_AppOnDestroy);
}
ZF_STATIC_REGISTER_END(_ZFP_ZFGlobalEventCenter_common_register)

ZF_NAMESPACE_GLOBAL_END

