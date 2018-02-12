/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFImpl_sys_iOS_ZFCore.h"
#include "ZFCore/protocol/ZFProtocolZFFileCwd.h"

#if ZF_ENV_sys_iOS
#import <Foundation/Foundation.h>

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFFileCwdImpl_sys_iOS, ZFFileCwd, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT(zfText("iOS:SandboxPath"))
public:
    virtual const zfchar *pathForCwd(void)
    {
        static zfstring _pathForCwd;
        ZFImpl_sys_iOS_zfstringFromNSString(_pathForCwd, [NSFileManager defaultManager].currentDirectoryPath);
        return _pathForCwd;
    }
    virtual zfbool pathForCwdChange(ZF_IN const zfchar *pathForCwd)
    {
        return ([[NSFileManager defaultManager] changeCurrentDirectoryPath:ZFImpl_sys_iOS_zfstringToNSString(pathForCwd)] == YES);
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFFileCwdImpl_sys_iOS)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFFileCwdImpl_sys_iOS)

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_iOS

