/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFImpl_default_ZF.h"
#include "ZFCore/ZFMainEntry.h"
#include "ZFCore/ZFString.h"
#include "ZFCore/ZFArray.h"

#if !ZF_ENV_sys_Android && !ZF_ENV_sys_iOS && !ZF_ENV_sys_Qt

#if !ZF_ENV_sys_WindowsCE
int main(int argc, char **argv)
#else // #if ZF_ENV_sys_WindowsCE
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
#endif // #if ZF_ENV_sys_WindowsCE #else
{

#if !ZF_ENV_sys_WindowsCE
    return ZFMainCommonEntry(argc, argv);
#else
    ZFFrameworkInit();
    ZFArrayEditable *params = zfAlloc(ZFArrayEditable);

    ZFCoreArray<zfstring> tmpList;
    zfCoreArgSplit(tmpList, ZFStringW2Z(lpCmdLine));
    for(zfindex i = 0; i < tmpList.count(); ++i)
    {
        ZFString *s = zfAlloc(ZFString, tmpList.get(i).cString());
        params->add(s);
        zfRelease(s);
    }

    zfint result = ZFMainExecute(params);
    zfRelease(params);
    ZFFrameworkCleanup();
    return result;
#endif // #if ZF_ENV_sys_WindowsCE #else
}

#endif // #if !ZF_ENV_sys_Android && !ZF_ENV_sys_iOS

