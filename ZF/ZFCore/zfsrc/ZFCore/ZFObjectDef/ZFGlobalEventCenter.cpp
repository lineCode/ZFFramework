/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFGlobalEventCenter.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFGlobalEventCenter)
ZFOBJECT_SINGLETON_DEFINE_DETAIL(ZFGlobalEventCenter, ZFGlobalEventCenter, ZFGlobalEventCenter, instance, ZFLevelZFFrameworkEssential)

ZF_NAMESPACE_GLOBAL_END

