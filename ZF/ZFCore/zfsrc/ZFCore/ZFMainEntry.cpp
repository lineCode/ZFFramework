/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFMainEntry.h"
#include "ZFString.h"

ZF_NAMESPACE_GLOBAL_BEGIN

static _ZFP_ZFMainFuncType &_ZFP_ZFMainFunc_(void)
{
    static _ZFP_ZFMainFuncType _func = zfnull;
    return _func;
}
#define _ZFP_ZFMainFunc (_ZFP_ZFMainFunc_())
void _ZFP_ZFMainRegister(ZF_IN _ZFP_ZFMainFuncType func)
{
    _ZFP_ZFMainFunc = func;
}
zfint ZFMainExecute(ZF_IN_OPT ZFCoreArray<zfstring> const &params)
{
    _ZFP_ZFMainFuncType &func = _ZFP_ZFMainFunc;
    if(func != zfnull)
    {
        return func(params);
    }
    else
    {
        zfCoreLog(zfTextA("ZFMAIN_ENTRY not set"));
        return -1;
    }
}

int ZFMainCommonEntry(ZF_IN int argc /* = 0 */, char **argv /* = zfnull */)
{
    ZFFrameworkInit();
    ZFCoreArray<zfstring> params;
    for(int i = 0; i < argc; ++i)
    {
        params.add(ZFStringA2Z(argv[i]));
    }
    zfint ret = ZFMainExecute(params);
    ZFFrameworkCleanup();
    return ret;
}

ZF_NAMESPACE_GLOBAL_END

