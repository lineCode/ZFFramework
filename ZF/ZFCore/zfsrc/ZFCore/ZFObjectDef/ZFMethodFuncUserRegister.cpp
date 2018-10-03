/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFMethodFuncUserRegister.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

void ZFMethodFuncUserUnregister(ZF_IN const ZFMethod *method)
{
    if(method == zfnull)
    {
        return ;
    }
    zfCoreMutexLocker();
    zfCoreAssertWithMessageTrim(method->methodIsUserRegister(),
            "[ZFMethodFuncUserUnregister] method %s is not user registered",
            method->objectInfo().cString()
        );
    zfCoreAssertWithMessageTrim(method->methodIsFunctionType(),
            "[ZFMethodFuncUserUnregister] method %s is not function type",
            method->objectInfo().cString()
        );

    _ZFP_ZFMethodUnregister(method);
}

ZF_NAMESPACE_GLOBAL_END

