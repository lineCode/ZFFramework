/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFImpl_default_ZFCore_impl.h"
#include "ZFCore/protocol/ZFProtocolZFThreadTaskRequest.h"
#include "ZFCore/ZFThread.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFThreadTaskRequestImpl_default, ZFThreadTaskRequest, ZFProtocolLevel::e_Default)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("ZFFramework:ZFThread")

public:
    virtual void taskRequest(ZF_IN const ZFListener &task,
                             ZF_IN ZFObject *param0,
                             ZF_IN ZFObject *param1)
    {
        ZFLISTENER_LOCAL(threadCallback, {
            ZFLISTENER_LOCAL(mainThreadCallback, {
                ZFListenerHolder *listenerHolder = ZFCastZFObjectUnchecked(ZFListenerHolder *, userData);
                listenerHolder->runnableExecute();
            })
            ZFThreadExecuteInMainThread(mainThreadCallback, userData);
        })
        ZFThreadExecuteInNewThread(threadCallback,
            zflineAlloc(ZFListenerHolder, task, ZFListenerData().param0Set(param0).param1Set(param1)));
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFThreadTaskRequestImpl_default)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFThreadTaskRequestImpl_default)

ZF_NAMESPACE_GLOBAL_END

