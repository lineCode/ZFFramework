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

#include "ZFCore/ZFSTLWrapper/zfstl_map.h"
#include "ZFCore/ZFSTLWrapper/zfstl_string.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
static void _ZFP_ZFImpl_ZFLua_ZFMethod_setupGlobalMethod(ZF_IN const ZFCoreArrayPOD<lua_State *> &luaStateList,
                                                         ZF_IN const ZFMethod *method)
{
    if(!method->methodIsFunctionType()
        || method->methodNamespace() != zfnull)
    {
        return ;
    }

    zfstring code;
    zfstringAppend(code,
            "function %s(...)\n"
            "    return zfl_callStatic2('', '%s', ...)\n"
            "end\n"
        , method->methodName(), method->methodName());
    for(zfindex i = 0; i < luaStateList.count(); ++i)
    {
        ZFImpl_ZFLua_execute(luaStateList[i], code);
    }
}
static ZFLISTENER_PROTOTYPE_EXPAND(_ZFP_ZFImpl_ZFLua_ZFMethod_methodOnChange)
{
    const ZFClassDataChangeData *data = listenerData.param0->to<ZFPointerHolder *>()->holdedDataPointer<const ZFClassDataChangeData *>();
    if(data->changedMethod != zfnull && data->changeType == ZFClassDataChangeTypeAttach)
    {
        if(data->changedMethod->methodIsFunctionType())
        {
            const ZFCoreArrayPOD<lua_State *> &luaStateList = ZFImpl_ZFLua_luaStateList();
            for(zfindex i = 0; i < luaStateList.count(); ++i)
            {
                ZFImpl_ZFLua_implSetupScope(luaStateList[i], data->changedMethod->methodNamespace());
            }

            _ZFP_ZFImpl_ZFLua_ZFMethod_setupGlobalMethod(luaStateList, data->changedMethod);
        }
    }
}
ZFImpl_ZFLua_implSetupCallback_DEFINE(ZFMethod, ZFM_EXPAND({
        ZFCoreArrayPOD<const ZFMethod *> allMethod = ZFMethodFuncGetAll();
        if(!allMethod.isEmpty())
        {
            zfstlmap<zfstlstringZ, zfbool> methodNamespaceList;
            ZFCoreArrayPOD<lua_State *> luaStateList;
            luaStateList.add(L);
            for(zfindex i = 0; i < allMethod.count(); ++i)
            {
                const ZFMethod *method = allMethod[i];
                if(method->methodNamespace() != zfnull)
                {
                    zfindex dotPos = zfstringFind(method->methodNamespace(), zfindexMax(), ZFNamespaceSeparator());
                    if(dotPos == zfindexMax())
                    {
                        methodNamespaceList[method->methodNamespace()] = zftrue;
                    }
                    else
                    {
                        methodNamespaceList[zfstring(method->methodNamespace(), dotPos).cString()] = zftrue;
                    }
                }
                _ZFP_ZFImpl_ZFLua_ZFMethod_setupGlobalMethod(luaStateList, method);
            }

            if(!methodNamespaceList.empty())
            {
                const zfchar **tmp = (const zfchar **)zfmalloc(sizeof(const zfchar *) * (allMethod.count() + 1));
                zfblockedFree(tmp);
                zfindex i = 0;
                for(zfstlmap<zfstlstringZ, zfbool>::iterator it = methodNamespaceList.begin(); it != methodNamespaceList.end(); ++it)
                {
                    tmp[i] = it->first.c_str();
                    ++i;
                }
                tmp[i] = zfnull;
                ZFImpl_ZFLua_implSetupScope(L, tmp);
            }
        }

        ZFClassDataChangeObserver.observerAdd(
            ZFGlobalEvent::EventClassDataChange(),
            ZFCallbackForFunc(_ZFP_ZFImpl_ZFLua_ZFMethod_methodOnChange));

        ZFImpl_ZFLua_implSetupScope(L, ZF_NAMESPACE_GLOBAL_NAME);
        ZFImpl_ZFLua_implSetupScope(L, ZF_NAMESPACE_GLOBAL_ABBR_NAME);
    }), {
        ZFClassDataChangeObserver.observerRemove(
            ZFGlobalEvent::EventClassDataChange(),
            ZFCallbackForFunc(_ZFP_ZFImpl_ZFLua_ZFMethod_methodOnChange));
    })

ZF_NAMESPACE_GLOBAL_END

