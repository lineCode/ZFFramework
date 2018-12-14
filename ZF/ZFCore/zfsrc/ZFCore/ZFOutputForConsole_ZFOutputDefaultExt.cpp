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

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFOutputForConsole_ZFOutputDefaultExt, ZFLevelZFFrameworkEssential)
{
    ZFExportVarEnsureInit_ZFOutputForConsole();
    ZFOutputDefaultAdd(ZFOutputForConsole());
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFOutputForConsole_ZFOutputDefaultExt)
{
    ZFOutputDefaultRemove(ZFOutputForConsole());
}
ZF_GLOBAL_INITIALIZER_END(ZFOutputForConsole_ZFOutputDefaultExt)

ZF_NAMESPACE_GLOBAL_END

