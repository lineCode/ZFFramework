/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFLuaGC.h"
#include "ZFLuaState.h"
#include "protocol/ZFProtocolZFLua.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFMETHOD_FUNC_DEFINE_1(void, ZFLuaGC,
                       ZFMP_IN_OPT(void *, L, zfnull))
{
    ZFPROTOCOL_ACCESS(ZFLua)->luaGC(L ? L : ZFLuaState());
}

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFLuaGCAutoApply, ZFLevelZFFrameworkNormal)
{
    this->classChangeListener = ZFCallbackForFunc(zfself::classChange);
    ZFClassDataChangeObserver.observerAdd(
        ZFGlobalEvent::EventClassDataChange(),
        this->classChangeListener);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFLuaGCAutoApply)
{
    ZFClassDataChangeObserver.observerRemove(
        ZFGlobalEvent::EventClassDataChange(),
        this->classChangeListener);
}
ZFListener classChangeListener;
static ZFLISTENER_PROTOTYPE_EXPAND(classChange)
{
    const ZFClassDataChangeData *data = listenerData.param0->to<ZFPointerHolder *>()->holdedDataPointer<const ZFClassDataChangeData *>();
    if(data->changedClass != zfnull && data->changeType != ZFClassDataChangeTypeUpdate)
    {
        ZFCoreArrayPOD<void *> L;
        ZFLuaStateAttached(L);
        for(zfindex i = 0; i < L.count(); ++i)
        {
            ZFLuaGC(L[i]);
        }
    }
}
ZF_GLOBAL_INITIALIZER_END(ZFLuaGCAutoApply)

ZF_NAMESPACE_GLOBAL_END

