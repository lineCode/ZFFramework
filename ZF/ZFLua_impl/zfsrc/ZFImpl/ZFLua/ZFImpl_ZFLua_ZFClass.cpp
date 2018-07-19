/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFImpl_ZFLua.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
static ZFLISTENER_PROTOTYPE_EXPAND(_ZFP_ZFImpl_ZFLua_ZFClass_classOnChange)
{
    const ZFClassDataChangeData *data = listenerData.param0->to<ZFPointerHolder *>()->holdedDataPointer<const ZFClassDataChangeData *>();
    if(data->changedClass != zfnull && data->changeType == ZFClassDataChangeTypeAttach
        && data->changedClass->classNamespace() == zfnull)
    {
        const ZFCoreArrayPOD<lua_State *> &luaStateList = ZFImpl_ZFLua_luaStateList();
        for(zfindex i = 0; i < luaStateList.count(); ++i)
        {
            ZFImpl_ZFLua_implSetupScope(luaStateList[i], data->changedClass->className());
        }
    }
}
ZFImpl_ZFLua_implSetupCallback_DEFINE(ZFClass, {
        ZFCoreArrayPOD<const ZFClass *> allClass = ZFClassGetAll();
        if(!allClass.isEmpty())
        {
            const zfchar **classNameList = (const zfchar **)zfmalloc(sizeof(const zfchar *) * (allClass.count() + 1));
            zfblockedFree(classNameList);
            zfindex attached = 0;
            for(zfindex i = 0; i < allClass.count(); ++i)
            {
                if(allClass[i]->classNamespace() == zfnull)
                {
                    classNameList[attached] = allClass[i]->className();
                    ++attached;
                }
            }
            classNameList[attached] = zfnull;

            ZFImpl_ZFLua_implSetupScope(L, classNameList);
        }

        ZFClassDataChangeObserver.observerAdd(
            ZFGlobalEvent::EventClassDataChange(),
            ZFCallbackForFunc(_ZFP_ZFImpl_ZFLua_ZFClass_classOnChange));
    }, {
        ZFClassDataChangeObserver.observerRemove(
            ZFGlobalEvent::EventClassDataChange(),
            ZFCallbackForFunc(_ZFP_ZFImpl_ZFLua_ZFClass_classOnChange));
    })

ZF_NAMESPACE_GLOBAL_END

