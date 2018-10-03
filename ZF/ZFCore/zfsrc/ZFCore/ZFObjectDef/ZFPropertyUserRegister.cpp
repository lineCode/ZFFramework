/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFPropertyUserRegister.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfautoObject _ZFP_I_PropURDIVH::create(ZF_IN void *v,
                                       ZF_IN DeleteCallback deleteCallback,
                                       ZF_IN_OPT ZFObject *retainValue /* = zfnull */)
{
    zfblockedAlloc(_ZFP_I_PropURDIVH, holder);
    holder->v = v;
    holder->deleteCallback = deleteCallback;
    holder->retainValueHolder = retainValue;
    return holder;
}

void ZFPropertyUserUnregister(ZF_IN const ZFProperty *zfproperty)
{
    if(zfproperty == zfnull)
    {
        return ;
    }
    zfCoreMutexLocker();
    zfCoreAssertWithMessageTrim(zfproperty->propertyIsUserRegister(),
            "[ZFPropertyUserUnregister] property %s is not user registered",
            zfproperty->objectInfo().cString()
        );

    _ZFP_ZFPropertyUnregister(zfproperty);
}

ZF_NAMESPACE_GLOBAL_END

