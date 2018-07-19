/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFApp.h"

ZF_NAMESPACE_GLOBAL_BEGIN
ZF_NAMESPACE_BEGIN(ZFApp)
/** @cond ZFPrivateDoc */ // ZFTAG_DOXYGEN_BUG: all uppercase macro not skipped within namespace

ZFMETHOD_FUNC_DEFINE_0(void, appExit)
{
    ZFFrameworkCleanup();
    exit(0);
}

/** @endcond */
ZF_NAMESPACE_END_WITH_REGISTER(ZFApp, ZF_NAMESPACE_GLOBAL)
ZF_NAMESPACE_GLOBAL_END

