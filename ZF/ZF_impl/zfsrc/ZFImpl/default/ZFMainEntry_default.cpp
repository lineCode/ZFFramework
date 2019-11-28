#include "ZFImpl_default_ZF_impl.h"
#include "ZFCore.h"

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
    zfCoreArgSplit(tmpList, zfstringToUTF8(lpCmdLine, ZFStringEncoding::e_UTF16).cString());
    for(zfindex i = 0; i < tmpList.count(); ++i)
    {
        v_zfstring *s = zfAlloc(v_zfstring, tmpList.get(i).cString());
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

