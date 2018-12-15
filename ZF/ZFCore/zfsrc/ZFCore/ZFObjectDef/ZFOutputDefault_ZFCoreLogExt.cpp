/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFOutputDefault.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
static void _ZFP_ZFOutputDefault_ZFCoreLogExt(ZF_IN const zfchar *s)
{
    ZFOutputDefault().execute(s);
}

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFOutputDefault_ZFCoreLogExt, ZFLevelZFFrameworkEssential)
{
    zfCoreLogOutputCallbackSet(_ZFP_ZFOutputDefault_ZFCoreLogExt);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFOutputDefault_ZFCoreLogExt)
{
    zfCoreLogOutputCallbackSet();
}
ZF_GLOBAL_INITIALIZER_END(ZFOutputDefault_ZFCoreLogExt)

ZF_NAMESPACE_GLOBAL_END

