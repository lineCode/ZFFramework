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
#include "ZFImpl_ZFLua_metatable.h"

#include "ZFCore/ZFSTLWrapper/zfstl_string.h"
#include "ZFCore/ZFSTLWrapper/zfstl_vector.h"
#include "ZFCore/ZFSTLWrapper/zfstl_map.h"

#define _ZFP_ZFCallbackForLua_DEBUG ZF_ENV_DEBUG

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFImpl_ZFLua_luaStateHolder, ZFLevelAppEssential)
{
    L = (lua_State *)ZFLuaStateOpen();
    this->builtinLuaEnv = zftrue;
    ZFLuaStateAttach(L);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFImpl_ZFLua_luaStateHolder)
{
    ZFLuaStateDetach(L);
    if(this->builtinLuaEnv)
    {
        ZFLuaStateClose(L);
    }
}
lua_State *L;
zfbool builtinLuaEnv;
ZF_GLOBAL_INITIALIZER_END(ZFImpl_ZFLua_luaStateHolder)
lua_State *_ZFP_ZFImpl_ZFLua_luaState(void)
{
    return ZF_GLOBAL_INITIALIZER_INSTANCE(ZFImpl_ZFLua_luaStateHolder)->L;
}
void ZFImpl_ZFLua_luaStateChange(ZF_IN lua_State *L)
{
    zfCoreMutexLocker();
    ZF_GLOBAL_INITIALIZER_CLASS(ZFImpl_ZFLua_luaStateHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFImpl_ZFLua_luaStateHolder);

    zfCoreAssert(L != zfnull);
    if(L == d->L)
    {
        return ;
    }

    ZFLuaStateDetach(d->L);
    if(d->builtinLuaEnv)
    {
        d->builtinLuaEnv = zffalse;
        ZFLuaStateClose(d->L);
    }
    d->L = L;
    ZFLuaStateAttach(d->L);
}

void *ZFImpl_ZFLua_luaStateOpen(void)
{
    return ZFImpl_ZFLua_luaOpen();
}
void ZFImpl_ZFLua_luaStateClose(ZF_IN lua_State *L)
{
    ZFImpl_ZFLua_luaClose(L);
}

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFImpl_ZFLua_luaStateGlobalHolder, ZFLevelZFFrameworkLow)
{
}
zfstlmap<lua_State *, zfbool> attachedState;
ZFCoreArrayPOD<lua_State *> attachedStateList;
ZFCoreArrayPOD<_ZFP_ZFImpl_ZFLua_ImplSetupCallback> setupAttach;
ZFCoreArrayPOD<_ZFP_ZFImpl_ZFLua_ImplSetupCallback> setupDetach;
ZF_GLOBAL_INITIALIZER_END(ZFImpl_ZFLua_luaStateGlobalHolder)

void ZFImpl_ZFLua_luaStateAttach(ZF_IN lua_State *L)
{
    zfCoreAssert(L != zfnull);
    zfCoreMutexLocker();

    ZF_GLOBAL_INITIALIZER_CLASS(ZFImpl_ZFLua_luaStateGlobalHolder) *d
        = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFImpl_ZFLua_luaStateGlobalHolder);

    zfCoreAssertWithMessageTrim(d->attachedState.find(L) == d->attachedState.end(),
        zfTextA("[ZFImpl_ZFLua_luaStateAttach] lua state %p already attached"),
        L);
    d->attachedState[L] = zftrue;
    d->attachedStateList.add(L);

    ZFImpl_ZFLua_luaClassRegister<zfautoObject>(L, zfText("zfautoObject"));

    // zfnull
    zfclassNotPOD _ZFP_ZFImpl_ZFLua_zfnullHolder
    {
    public:
        static zfautoObject get_zfnull(void)
        {
            return zfnull;
        }
    };
    ZFImpl_ZFLua_luaFunctionRegister(L, zfText("_ZFP_ZFImpl_ZFLua_zfnull"), _ZFP_ZFImpl_ZFLua_zfnullHolder::get_zfnull);
    ZFImpl_ZFLua_execute(L, zfText(
            "zfnull = _ZFP_ZFImpl_ZFLua_zfnull()\n"
        ));

    // zftrue, zffalse
    zfclassNotPOD _ZFP_ZFImpl_ZFLua_zfboolHolder
    {
    public:
        static zfautoObject get_zftrue(void)
        {
            return zflineAlloc(v_zfbool, zftrue);
        }
        static zfautoObject get_zffalse(void)
        {
            return zflineAlloc(v_zfbool, zffalse);
        }
    };
    ZFImpl_ZFLua_luaFunctionRegister(L, zfText("_ZFP_ZFImpl_ZFLua_zftrue"), _ZFP_ZFImpl_ZFLua_zfboolHolder::get_zftrue);
    ZFImpl_ZFLua_luaFunctionRegister(L, zfText("_ZFP_ZFImpl_ZFLua_zffalse"), _ZFP_ZFImpl_ZFLua_zfboolHolder::get_zffalse);
    ZFImpl_ZFLua_execute(L, zfText(
            "zftrue = _ZFP_ZFImpl_ZFLua_zftrue()\n"
            "zffalse = _ZFP_ZFImpl_ZFLua_zffalse()\n"
        ));

    // zfl_L
    zfclassNotPOD _ZFP_ZFImpl_ZFLua_LHolder
    {
    public:
        static int f(ZF_IN lua_State *L)
        {
            zfblockedAlloc(v_VoidPointer, ret, (void *)L);
            zfautoObject tmp = ret;
            ZFImpl_ZFLua_luaPush(L, tmp);
            return 1;
        }
    };
    ZFImpl_ZFLua_luaCFunctionRegister(L, zfText("zfl_L"), _ZFP_ZFImpl_ZFLua_LHolder::f);

    // zfl_call metatable
    ZFImpl_ZFLua_execute(L, zfText(
            "_ZFP_zfl_index = function(tbl, k)\n"
            "    local t = tbl .. '.' .. k\n" // ZFNamespaceSeparator
            "    local d = debug.getmetatable(t)\n"
            "    d.__index = _ZFP_zfl_index\n"
            "    d.__call = _ZFP_zfl_call\n"
            "    debug.setmetatable(t, d)\n"
            "    return t\n"
            "end\n"
            "_ZFP_zfl_call = function(tbl, ...)\n"
            "    return zfl_callStatic(tbl, ...)\n"
            "end\n"
        ));

    // each impl setup callback
    for(zfindex i = 0; i < d->setupAttach.count(); ++i)
    {
        d->setupAttach[i](L);
    }
}
void ZFImpl_ZFLua_luaStateDetach(ZF_IN lua_State *L)
{
    zfCoreMutexLocker();

    ZF_GLOBAL_INITIALIZER_CLASS(ZFImpl_ZFLua_luaStateGlobalHolder) *d
        = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFImpl_ZFLua_luaStateGlobalHolder);

    zfCoreAssertWithMessageTrim(d->attachedState.find(L) != d->attachedState.end(),
        zfTextA("[ZFImpl_ZFLua_luaStateDetach] lua state %p not attached"),
        L);

    // each impl setup callback
    for(zfindex i = 0; i < d->setupDetach.count(); ++i)
    {
        d->setupDetach[i](L);
    }

    d->attachedState.erase(L);
    d->attachedStateList.removeElement(L);
}
void ZFImpl_ZFLua_luaStateListT(ZF_IN_OUT ZFCoreArray<lua_State *> &ret)
{
    ret.addFrom(ZF_GLOBAL_INITIALIZER_INSTANCE(ZFImpl_ZFLua_luaStateGlobalHolder)->attachedStateList);
}
const ZFCoreArrayPOD<lua_State *> &ZFImpl_ZFLua_luaStateList(void)
{
    return ZF_GLOBAL_INITIALIZER_INSTANCE(ZFImpl_ZFLua_luaStateGlobalHolder)->attachedStateList;
}

// ============================================================
void _ZFP_ZFImpl_ZFLua_implSetupCallbackRegister(ZF_IN _ZFP_ZFImpl_ZFLua_ImplSetupCallback setupAttachCallback,
                                                 ZF_IN _ZFP_ZFImpl_ZFLua_ImplSetupCallback setupDetachCallback)
{
    zfCoreMutexLocker();

    ZF_GLOBAL_INITIALIZER_CLASS(ZFImpl_ZFLua_luaStateGlobalHolder) *d
        = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFImpl_ZFLua_luaStateGlobalHolder);
    d->setupAttach.add(setupAttachCallback);
    d->setupDetach.add(setupDetachCallback);
}
void _ZFP_ZFImpl_ZFLua_implSetupCallbackUnregister(ZF_IN _ZFP_ZFImpl_ZFLua_ImplSetupCallback setupAttachCallback,
                                                   ZF_IN _ZFP_ZFImpl_ZFLua_ImplSetupCallback setupDetachCallback)
{
    zfCoreMutexLocker();

    ZF_GLOBAL_INITIALIZER_CLASS(ZFImpl_ZFLua_luaStateGlobalHolder) *d
        = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFImpl_ZFLua_luaStateGlobalHolder);
    d->setupAttach.removeElement(setupAttachCallback);
    d->setupDetach.removeElement(setupDetachCallback);
}

// ============================================================
void ZFImpl_ZFLua_implSetupScope(ZF_IN_OUT lua_State *L, ZF_IN const zfchar *scopeName)
{
    const zfchar *scopeNameList[2] = {scopeName, zfnull};
    ZFImpl_ZFLua_implSetupScope(L, scopeNameList);
}
static void _ZFP_ZFImpl_ZFLua_implSetupScope(ZF_IN_OUT zfstring &code,
                                             ZF_IN const zfchar *scopeName)
{
    zfstringAppend(code, zfText(
            "%s = '%s'\n"
            "local tbl = debug.getmetatable(%s)\n"
            "tbl.__index = _ZFP_zfl_index\n"
            "tbl.__call = _ZFP_zfl_call\n"
            "debug.setmetatable(%s, tbl)\n"
        ), scopeName, scopeName, scopeName, scopeName);
}
void ZFImpl_ZFLua_implSetupScope(ZF_IN_OUT lua_State *L, ZF_IN const zfchar **scopeNameList)
{
    zfstring code;
    for(zfindex i = 0; *scopeNameList != zfnull; ++scopeNameList, ++i)
    {
        _ZFP_ZFImpl_ZFLua_implSetupScope(code, *scopeNameList);
        if(zfsncmp(*scopeNameList, ZFImpl_ZFLua_PropTypePrefix, ZFImpl_ZFLua_PropTypePrefixLen) == 0
            && !zfsIsEmpty(*scopeNameList + ZFImpl_ZFLua_PropTypePrefixLen))
        {
            _ZFP_ZFImpl_ZFLua_implSetupScope(code, *scopeNameList + ZFImpl_ZFLua_PropTypePrefixLen);
            ++i;
        }
        if(i >= 100)
        {
            ZFImpl_ZFLua_execute(L, code);
            code.removeAll();
            i = 0;
        }
    }
    if(!code.isEmpty())
    {
        ZFImpl_ZFLua_execute(L, code);
    }
}
void ZFImpl_ZFLua_implSetupObject(ZF_IN_OUT lua_State *L, ZF_IN_OPT int objIndex /* = -1 */)
{
    ZFImpl_ZFLua_implSetupObject_metatable(L, objIndex);
}

// ============================================================
static zfautoObject _ZFP_ZFImpl_ZFLua_implDispatchReturnValueNotSetInstance;
const zfautoObject &_ZFP_ZFImpl_ZFLua_implDispatchReturnValueNotSet(void)
{
    return _ZFP_ZFImpl_ZFLua_implDispatchReturnValueNotSetInstance;
}
zfclass _ZFP_I_ZFImpl_ZFLua_ImplDispatchReturnValueNotSet : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_I_ZFImpl_ZFLua_ImplDispatchReturnValueNotSet, ZFObject)
};
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(_ZFP_ZFImpl_ZFLua_implDispatchReturnValueNotSetInstanceInit, ZFLevelZFFrameworkLow)
{
    _ZFP_ZFImpl_ZFLua_implDispatchReturnValueNotSetInstance
        = zflineAlloc(_ZFP_I_ZFImpl_ZFLua_ImplDispatchReturnValueNotSet);
}
ZF_GLOBAL_INITIALIZER_DESTROY(_ZFP_ZFImpl_ZFLua_implDispatchReturnValueNotSetInstanceInit)
{
    _ZFP_ZFImpl_ZFLua_implDispatchReturnValueNotSetInstance = zfnull;
}
ZF_GLOBAL_INITIALIZER_END(_ZFP_ZFImpl_ZFLua_implDispatchReturnValueNotSetInstanceInit)

static zfstlmap<zfstlstringZ, zfstlmap<zfstlstringZ, zfstlvector<_ZFP_ZFImpl_ZFLua_ImplDispatchCallback> > > _ZFP_ZFImpl_ZFLua_implDispatchData;
void _ZFP_ZFImpl_ZFLua_implDispatchRegister(ZF_IN _ZFP_ZFImpl_ZFLua_ImplDispatchCallback invokerCallback,
                                            ZF_IN_OPT const zfchar *classOrNamespace /* = zfnull */,
                                            ZF_IN_OPT const zfchar *methodName /* = zfnull */)
{
    zfCoreAssert(invokerCallback != zfnull);
    zfCoreMutexLocker();

    if(classOrNamespace == zfnull
        || zfscmpTheSame(classOrNamespace, ZF_NAMESPACE_GLOBAL_NAME)
        || zfscmpTheSame(classOrNamespace, ZF_NAMESPACE_GLOBAL_ABBR_NAME))
    {
        classOrNamespace = zfText("");
    }
    if(methodName == zfnull)
    {
        methodName = zfText("");
    }
    _ZFP_ZFImpl_ZFLua_implDispatchData[classOrNamespace][methodName].push_back(invokerCallback);
}
void _ZFP_ZFImpl_ZFLua_implDispatchUnregister(ZF_IN _ZFP_ZFImpl_ZFLua_ImplDispatchCallback invokerCallback,
                                              ZF_IN_OPT const zfchar *classOrNamespace /* = zfnull */,
                                              ZF_IN_OPT const zfchar *methodName /* = zfnull */)
{
    zfCoreMutexLocker();

    if(classOrNamespace == zfnull
        || zfscmpTheSame(classOrNamespace, ZF_NAMESPACE_GLOBAL_NAME)
        || zfscmpTheSame(classOrNamespace, ZF_NAMESPACE_GLOBAL_ABBR_NAME))
    {
        classOrNamespace = zfText("");
    }
    if(methodName == zfnull)
    {
        methodName = zfText("");
    }

    zfstlmap<zfstlstringZ, zfstlmap<zfstlstringZ, zfstlvector<_ZFP_ZFImpl_ZFLua_ImplDispatchCallback> > >::iterator methodIt = _ZFP_ZFImpl_ZFLua_implDispatchData.find(classOrNamespace);
    if(methodIt != _ZFP_ZFImpl_ZFLua_implDispatchData.end())
    {
        zfstlmap<zfstlstringZ, zfstlvector<_ZFP_ZFImpl_ZFLua_ImplDispatchCallback> >::iterator methodNameIt = methodIt->second.find(methodName);
        if(methodNameIt != methodIt->second.end())
        {
            for(zfstlsize i = 0; i < methodNameIt->second.size(); ++i)
            {
                if(methodNameIt->second[i] == invokerCallback)
                {
                    methodNameIt->second.erase(methodNameIt->second.begin() + i);
                    break;
                }
            }
            if(methodNameIt->second.empty())
            {
                methodIt->second.erase(methodNameIt);
            }
        }
        if(methodIt->second.empty())
        {
            _ZFP_ZFImpl_ZFLua_implDispatchData.erase(methodIt);
        }
    }
}
void ZFImpl_ZFLua_implDispatch(ZF_IN_OUT ZFImpl_ZFLua_ImplDispatchInfo &dispatchInfo)
{
    if(dispatchInfo.classOrNamespace == zfnull
        || zfscmpTheSame(dispatchInfo.classOrNamespace, ZF_NAMESPACE_GLOBAL_NAME)
        || zfscmpTheSame(dispatchInfo.classOrNamespace, ZF_NAMESPACE_GLOBAL_ABBR_NAME))
    {
        dispatchInfo.classOrNamespace = zfText("");
    }
    if(dispatchInfo.methodName == zfnull)
    {
        dispatchInfo.methodName = zfText("");
    }

    zfstlmap<zfstlstringZ, zfstlmap<zfstlstringZ, zfstlvector<_ZFP_ZFImpl_ZFLua_ImplDispatchCallback> > >::iterator methodIt = _ZFP_ZFImpl_ZFLua_implDispatchData.find(dispatchInfo.classOrNamespace);
    if(methodIt != _ZFP_ZFImpl_ZFLua_implDispatchData.end())
    {
        // Scope::methodName
        zfstlmap<zfstlstringZ, zfstlvector<_ZFP_ZFImpl_ZFLua_ImplDispatchCallback> >::iterator methodNameIt = methodIt->second.find(dispatchInfo.methodName);
        if(methodNameIt != methodIt->second.end())
        {
            for(zfstlsize i = 0; i < methodNameIt->second.size(); ++i)
            {
                methodNameIt->second[i](dispatchInfo);
                if(dispatchInfo.dispatchResult != ZFImpl_ZFLua_ImplDispatchResultForward)
                {
                    return ;
                }
            }
        }

        // Scope::*
        methodNameIt = methodIt->second.find(zfText(""));
        if(methodNameIt != methodIt->second.end())
        {
            for(zfstlsize i = 0; i < methodNameIt->second.size(); ++i)
            {
                methodNameIt->second[i](dispatchInfo);
                if(dispatchInfo.dispatchResult != ZFImpl_ZFLua_ImplDispatchResultForward)
                {
                    return ;
                }
            }
        }
    }

    methodIt = _ZFP_ZFImpl_ZFLua_implDispatchData.find(ZFImpl_ZFLua_implDispatchAll);
    if(methodIt != _ZFP_ZFImpl_ZFLua_implDispatchData.end())
    {
        // *::methodName
        zfstlmap<zfstlstringZ, zfstlvector<_ZFP_ZFImpl_ZFLua_ImplDispatchCallback> >::iterator methodNameIt = methodIt->second.find(dispatchInfo.methodName);
        if(methodNameIt != methodIt->second.end())
        {
            for(zfstlsize i = 0; i < methodNameIt->second.size(); ++i)
            {
                methodNameIt->second[i](dispatchInfo);
                if(dispatchInfo.dispatchResult != ZFImpl_ZFLua_ImplDispatchResultForward)
                {
                    return ;
                }
            }
        }

        // *::*
        methodNameIt = methodIt->second.find(zfText(""));
        if(methodNameIt != methodIt->second.end())
        {
            for(zfstlsize i = 0; i < methodNameIt->second.size(); ++i)
            {
                methodNameIt->second[i](dispatchInfo);
                if(dispatchInfo.dispatchResult != ZFImpl_ZFLua_ImplDispatchResultForward)
                {
                    return ;
                }
            }
        }
    }
}

// ============================================================
zfbool ZFImpl_ZFLua_execute(ZF_IN lua_State *L,
                            ZF_IN const zfchar *buf,
                            ZF_IN_OPT zfindex bufLen /* = zfindexMax() */,
                            ZF_OUT_OPT zfautoObject *luaResult /* = zfnull */,
                            ZF_IN_OPT const ZFCoreArray<zfautoObject> *luaParams /* = zfnull */,
                            ZF_OUT_OPT zfstring *errHint /* = zfnull */,
                            ZF_IN_OPT const zfchar *chunkInfo /* = zfnull */)
{
    ZF_GLOBAL_INITIALIZER_CLASS(ZFImpl_ZFLua_luaStateGlobalHolder) *d
        = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFImpl_ZFLua_luaStateGlobalHolder);
    if(d->attachedState.find(L) == d->attachedState.end())
    {
        if(errHint != zfnull)
        {
            zfstringAppend(*errHint, zfText("lua_State %p not attached"), L);
        }
        return zffalse;
    }

#if ZF_ENV_ZFCHAR_USE_CHAR_A
    int error = luaL_loadbuffer(L, buf, (bufLen == zfindexMax()) ? zfslen(buf) : bufLen, zfsCoreZ2A(chunkInfo));
#endif
#if ZF_ENV_ZFCHAR_USE_CHAR_W
    zfstringA _buf;
    ZFString::toUTF8(
        _buf,
        (bufLen == zfindexMax()) ? buf : zfstring(buf, bufLen).cString(),
        ZFStringEncodingForZFChar);
    int error = luaL_loadbuffer(L, _buf, _buf.length(), zfsCoreZ2A(chunkInfo));
#endif

    if(error == 0)
    {
        if(luaParams != zfnull && !luaParams->isEmpty())
        {
            for(zfindex i = 0; i < luaParams->count(); ++i)
            {
                ZFImpl_ZFLua_luaPush(L, luaParams->get(i));
            }
        }
        error = lua_pcall(
            L,
            (luaParams != zfnull ? (int)luaParams->count() : 0),
            (luaResult != zfnull ? 1 : 0),
            0);
    }

    if(error != 0)
    {
        zfstring errHintTmp;
        const char *nativeError = lua_tostring(L, -1);
        zfbool isBuiltinError = (zfstringFind(nativeError, ZFImpl_ZFLua_dummyError) != zfindexMax());
        if(!isBuiltinError)
        {
            errHintTmp += ZFStringA2Z(nativeError);
            if(errHint != zfnull)
            {
                *errHint += errHintTmp;
            }
        }

#if !ZF_ENV_ZFLUA_USE_EXCEPTION
        zfstringA _errorMsg;
        if(!errHintTmp.isEmpty())
        {
            zfstringAppend(_errorMsg, zfTextA("%s\n"), ZFStringZ2A(errHintTmp.cString()));
        }
        _errorMsg += zfTextA(
                "| [ZFLua]\n"
                "|     native lua error occurred with no exception support\n"
                "|     (which would cause unrecoverable C++ memory leak or logic error)\n"
                "|     to enable exception support\n"
                "|     add ZF_ENV_ZFLUA_USE_EXCEPTION to your compiler"
            );
        zfCoreCriticalMessageTrim(zfTextA("%s"), _errorMsg.cString());
#endif

        lua_pop(L, 1);
        return zffalse;
    }

    if(luaResult != zfnull)
    {
        if(lua_isuserdata(L, -1))
        {
            *luaResult = ZFImpl_ZFLua_luaGet(L, -1);
            lua_pop(L, 1);
        }
        else if(ZFImpl_ZFLua_toNumberT(*luaResult, L, -1, zftrue))
        {
            lua_pop(L, 1);
        }
        else
        {
            zfstring t;
            if(ZFImpl_ZFLua_toString(t, L, -1, zftrue))
            {
                zfblockedAlloc(v_zfstring, v);
                v->zfv = t;
                *luaResult = v;
                lua_pop(L, 1);
            }
        }
    }

    return zftrue;
}

// ============================================================
// utils
void ZFImpl_ZFLua_luaObjectInfoT(ZF_OUT zfstring &ret,
                                 ZF_IN lua_State *L,
                                 ZF_IN int luaStackOffset,
                                 ZF_IN_OPT zfbool printLuaType /* = zffalse */)
{
    if(printLuaType)
    {
        ret += zfText("(");
        ret += zfsCoreA2Z(luaL_typename(L, luaStackOffset));
        ret += zfText(")");
    }

    switch(lua_type(L, luaStackOffset))
    {
        case LUA_TNIL:
            ret += zfText("<nil>");
            break;
        case LUA_TBOOLEAN:
            zfboolToString(ret, (lua_toboolean(L, luaStackOffset) != 0));
            break;
        case LUA_TLIGHTUSERDATA:
        case LUA_TUSERDATA:
            {
                zfautoObject obj;
                if(ZFImpl_ZFLua_toObject(obj, L, luaStackOffset))
                {
                    ZFObjectInfoT(ret, obj);
                }
                else
                {
                    zfsFromPointerT(ret, lua_topointer(L, luaStackOffset));
                }
            }
            break;
        case LUA_TNUMBER:
            zfdoubleToString(ret, (zfdouble)lua_tonumber(L, luaStackOffset));
            break;
        case LUA_TSTRING:
            ret += ZFStringA2Z(lua_tostring(L, luaStackOffset));
            break;
        case LUA_TTABLE:
            lua_getglobal(L, "zfl_tableInfo");
            if(lua_isnil(L, -1))
            {
                lua_pop(L, 1);
                zfsFromPointerT(ret, lua_topointer(L, luaStackOffset));
            }
            else
            {
                if(luaStackOffset < 0)
                {
                    lua_pushvalue(L, luaStackOffset - 1);
                }
                else
                {
                    lua_pushvalue(L, luaStackOffset);
                }
                lua_call(L, 1, 1);
                ret += ZFStringA2Z(lua_tostring(L, -1));
                lua_pop(L, 1);
            }
            break;
        case LUA_TFUNCTION:
            zfsFromPointerT(ret, lua_topointer(L, luaStackOffset));
            break;
        case LUA_TTHREAD:
            zfsFromPointerT(ret, lua_topointer(L, luaStackOffset));
            break;
        default:
            ret += zfText("<Unknown>");
            break;
    }
}
zfbool ZFImpl_ZFLua_toObject(ZF_OUT zfautoObject &param,
                             ZF_IN lua_State *L,
                             ZF_IN int luaStackOffset)
{
    if(lua_isuserdata(L, luaStackOffset))
    {
        param = ZFImpl_ZFLua_luaGet(L, luaStackOffset);
        return zftrue;
    }
    else
    {
        return zffalse;
    }
}

zfbool ZFImpl_ZFLua_toGeneric(ZF_OUT zfautoObject &param,
                              ZF_IN lua_State *L,
                              ZF_IN int luaStackOffset)
{
    if(ZFImpl_ZFLua_toObject(param, L, luaStackOffset))
    {
        return zftrue;
    }
    else if(ZFImpl_ZFLua_toCallback(param, L, luaStackOffset))
    {
        return zftrue;
    }
    zfblockedAlloc(ZFDI_Wrapper, t);
    if(ZFImpl_ZFLua_toString(t->zfv, L, luaStackOffset, zftrue))
    {
        param = t;
        return zftrue;
    }
    else
    {
        return zffalse;
    }
}

zfclass _ZFP_I_ZFImpl_ZFLua_ZFCallbackForLuaHolder : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_I_ZFImpl_ZFLua_ZFCallbackForLuaHolder, ZFObject)
public:
    lua_State *L;
    int luaFunc;
#if _ZFP_ZFCallbackForLua_DEBUG
    zfstring luaFuncInfo;
#endif

    ZFLISTENER_INLINE(callback)
    {
        lua_rawgeti(L, LUA_REGISTRYINDEX, luaFunc);
        if(lua_isfunction(L, -1))
        {
            zfblockedAlloc(v_ZFListenerData, listenerDataTmp, listenerData);
            ZFImpl_ZFLua_luaPush(L, listenerDataTmp);

            ZFImpl_ZFLua_luaPush(L, userData);

            lua_pcall(L, 2, 0, 0);
        }
    }
protected:
    zfoverride
    virtual void objectOnDeallocPrepare(void)
    {
        luaL_unref(L, LUA_REGISTRYINDEX, luaFunc);
        zfsuper::objectOnDeallocPrepare();
    }
};
zfbool ZFImpl_ZFLua_toCallback(ZF_OUT zfautoObject &param,
                               ZF_IN lua_State *L,
                               ZF_IN int luaStackOffset)
{
    if(ZFImpl_ZFLua_toObject(param, L, luaStackOffset))
    {
        v_ZFCallback *callbackTmp = param;
        if(callbackTmp != zfnull)
        {
            return zftrue;
        }
        else
        {
            return zffalse;
        }
    }

    if(lua_isfunction(L, luaStackOffset))
    {
        zfblockedAlloc(v_ZFCallback, ret);
        zfblockedAlloc(_ZFP_I_ZFImpl_ZFLua_ZFCallbackForLuaHolder, holder);
        holder->L = L;
        lua_pushvalue(L, luaStackOffset);
        holder->luaFunc = luaL_ref(L, LUA_REGISTRYINDEX);
        ret->zfv = ZFCallbackForMemberMethod(holder, ZFMethodAccess(_ZFP_I_ZFImpl_ZFLua_ZFCallbackForLuaHolder, callback));
        ret->zfv.callbackOwnerObjectRetain();
        param = ret;

        #if _ZFP_ZFCallbackForLua_DEBUG
        {
            const zfcharA *buf = zfTextA(
                    "local arg={...}\n"
                    "local info=debug.getinfo(arg[1])\n"
                    "return info['source'] .. ':' .. info['linedefined']\n"
                );
            int error = luaL_loadbuffer(L, buf, zfslen(buf), "[ZFLuaDebug]");
            if(error == 0)
            {
                lua_pushvalue(L, luaStackOffset);
                error = lua_pcall(L, 1, 1, 0);

                if(error == 0 && ZFImpl_ZFLua_toString(holder->luaFuncInfo, L, -1, zftrue))
                {
                    lua_pop(L, 1);
                }
            }

            if(error != 0)
            {
                zfstring errHintTmp;
                const char *nativeError = lua_tostring(L, -1);
                zfCoreCriticalMessageTrim(zfTextA("[ZFLuaDebug] %s"), nativeError);
                lua_pop(L, 1);
            }
        }
        #endif
        return zftrue;
    }
    else
    {
        return zffalse;
    }
}

zfbool ZFImpl_ZFLua_toString(ZF_IN_OUT zfstring &s,
                             ZF_IN lua_State *L,
                             ZF_IN int luaStackOffset,
                             ZF_IN_OPT zfbool allowEmpty /* = zffalse */,
                             ZF_OUT_OPT const ZFClass **holderCls /* = zfnull */)
{
    if(holderCls != zfnull) {*holderCls = zfnull;}
    if(lua_isstring(L, luaStackOffset))
    {
        s += ZFStringA2Z(lua_tostring(L, luaStackOffset));
        return zftrue;
    }
    if(lua_isboolean(L, luaStackOffset))
    {
        zfboolToString(s, (zfbool)lua_toboolean(L, luaStackOffset));
        return zftrue;
    }
    if(!lua_isuserdata(L, luaStackOffset))
    {
        return zffalse;
    }

    zfautoObject const &param = ZFImpl_ZFLua_luaGet(L, luaStackOffset);
    return ZFImpl_ZFLua_toString(s, param.toObject(), allowEmpty, holderCls);
}
zfbool ZFImpl_ZFLua_toString(ZF_IN_OUT zfstring &s,
                             ZF_IN ZFObject *obj,
                             ZF_IN_OPT zfbool allowEmpty /* = zffalse */,
                             ZF_OUT_OPT const ZFClass **holderCls /* = zfnull */)
{
    if(holderCls != zfnull) {*holderCls = zfnull;}
    if(obj == zfnull)
    {
        return allowEmpty;
    }

    const ZFClass *cls = obj->classData();
    if(cls->classIsTypeOf(ZFString::ClassData()))
    {
        if(holderCls != zfnull) {*holderCls = ZFString::ClassData();}
        ZFString *t = obj->to<ZFString *>();
        if(t != zfnull)
        {
            s += t->stringValue();
        }
        return zftrue;
    }
    else if(cls->classIsTypeOf(v_zfstring::ClassData()))
    {
        if(holderCls != zfnull) {*holderCls = v_zfstring::ClassData();}
        s += obj->to<v_zfstring *>()->zfv;
        return zftrue;
    }
    else if(cls->classIsTypeOf(ZFDI_Wrapper::ClassData()))
    {
        if(holderCls != zfnull) {*holderCls = ZFDI_Wrapper::ClassData();}
        s += obj->to<ZFDI_Wrapper *>()->zfv;
        return zftrue;
    }
    else
    {
        return zffalse;
    }
}

zfbool ZFImpl_ZFLua_toNumberT(ZF_OUT zfautoObject &ret,
                              ZF_IN lua_State *L,
                              ZF_IN int luaStackOffset,
                              ZF_IN_OPT zfbool allowEmpty /* = zffalse */,
                              ZF_OUT_OPT const ZFClass **holderCls /* = zfnull */)
{
    if(holderCls != zfnull) {*holderCls = zfnull;}
    int success = 0;
    lua_Number num = lua_tonumberx(L, luaStackOffset, &success);
    if(success)
    {
        ret = ZFValue::doubleValueCreate((zfdouble)num);
        return zftrue;
    }
    if(!lua_isuserdata(L, luaStackOffset))
    {
        return zffalse;
    }

    zfautoObject const &param = ZFImpl_ZFLua_luaGet(L, luaStackOffset);
    return ZFImpl_ZFLua_toNumberT(ret, param.toObject(), allowEmpty, holderCls);
}

zfbool ZFImpl_ZFLua_toNumberT(ZF_OUT zfautoObject &ret,
                              ZF_IN ZFObject *obj,
                              ZF_IN_OPT zfbool allowEmpty /* = zffalse */,
                              ZF_OUT_OPT const ZFClass **holderCls /* = zfnull */)
{
    if(holderCls != zfnull) {*holderCls = zfnull;}
    if(obj == zfnull)
    {
        if(allowEmpty)
        {
            ret = ZFValue::intValueCreate(0);
            return zftrue;
        }
        else
        {
            return zffalse;
        }
    }

    const ZFClass *cls = obj->classData();
    if(holderCls != zfnull) {*holderCls = cls;}
    if(cls->classIsTypeOf(ZFValue::ClassData()))
    {
        ret = obj;
        return zftrue;
    }
    else if(cls->classIsTypeOf(v_zfbool::ClassData()))
    {
        ret = ZFValue::boolValueCreate(obj->to<v_zfbool *>()->zfv);
        return zftrue;
    }
    else if(cls->classIsTypeOf(v_zfindex::ClassData()))
    {
        ret = ZFValue::indexValueCreate(obj->to<v_zfindex *>()->zfv);
        return zftrue;
    }
    else if(cls->classIsTypeOf(v_zfint::ClassData()))
    {
        ret = ZFValue::intValueCreate(obj->to<v_zfint *>()->zfv);
        return zftrue;
    }
    else if(cls->classIsTypeOf(v_zfuint::ClassData()))
    {
        ret = ZFValue::unsignedIntValueCreate(obj->to<v_zfuint *>()->zfv);
        return zftrue;
    }
    else if(cls->classIsTypeOf(v_zffloat::ClassData()))
    {
        ret = ZFValue::floatValueCreate(obj->to<v_zffloat *>()->zfv);
        return zftrue;
    }
    else if(cls->classIsTypeOf(v_zfdouble::ClassData()))
    {
        ret = ZFValue::doubleValueCreate(obj->to<v_zfdouble *>()->zfv);
        return zftrue;
    }
    else if(cls->classIsTypeOf(v_zflongdouble::ClassData()))
    {
        ret = ZFValue::longDoubleValueCreate(obj->to<v_zflongdouble *>()->zfv);
        return zftrue;
    }
    else if(cls->classIsTypeOf(v_zfbyte::ClassData()))
    {
        ret = ZFValue::unsignedIntValueCreate((zfuint)obj->to<v_zfbyte *>()->zfv);
        return zftrue;
    }
    else if(cls->classIsTypeOf(v_zftimet::ClassData()))
    {
        ret = ZFValue::timeValueCreate(obj->to<v_zftimet *>()->zfv);
        return zftrue;
    }
    else if(cls->classIsTypeOf(v_zfflags::ClassData()))
    {
        ret = ZFValue::flagsValueCreate(obj->to<v_zfflags *>()->zfv);
        return zftrue;
    }
    else if(cls->classIsTypeOf(v_zfidentity::ClassData()))
    {
        ret = ZFValue::identityValueCreate(obj->to<v_zfidentity *>()->zfv);
        return zftrue;
    }
    else if(cls->classIsTypeOf(ZFEnum::ClassData()))
    {
        ret = ZFValue::flagsValueCreate((zfflags)obj->to<ZFEnum *>()->enumValue());
        return zftrue;
    }
    else
    {
        return zffalse;
    }
}

zfbool ZFImpl_ZFLua_toLuaValue(ZF_IN lua_State *L,
                               ZF_IN ZFObject *obj,
                               ZF_IN_OPT zfbool allowEmpty /* = zffalse */)
{
    if(obj == zfnull)
    {
        if(allowEmpty)
        {
            lua_pushnumber(L, 0);
            return zftrue;
        }
        else
        {
            return zffalse;
        }
    }

    zfautoObject v;
    if(ZFImpl_ZFLua_toNumberT(v, obj, allowEmpty))
    {
        ZFValue *t = v.to<ZFValue *>();
        switch(t->valueType())
        {
            case ZFValueType::e_bool:
                lua_pushboolean(L, t->boolValue());
                return 1;
            case ZFValueType::e_char:
            {
                zfstlstring s;
                s += (char)t->charValue();
                lua_pushstring(L, s.c_str());
            }
                return 1;
            case ZFValueType::e_int:
                lua_pushinteger(L, (lua_Integer)t->intValue());
                return 1;
            case ZFValueType::e_unsignedInt:
                lua_pushinteger(L, (lua_Integer)t->unsignedIntValue());
                return 1;
            case ZFValueType::e_index:
                lua_pushinteger(L, (lua_Integer)t->indexValue());
                return 1;
            case ZFValueType::e_float:
                lua_pushnumber(L, (lua_Number)t->floatValue());
                return 1;
            case ZFValueType::e_double:
                lua_pushnumber(L, (lua_Number)t->doubleValue());
                return 1;
            case ZFValueType::e_longDouble:
                lua_pushnumber(L, (lua_Number)t->longDoubleValue());
                return 1;
            case ZFValueType::e_time:
                lua_pushinteger(L, (lua_Integer)t->timeValue());
                return 1;
            case ZFValueType::e_flags:
                lua_pushinteger(L, (lua_Integer)t->flagsValue());
                return 1;
            case ZFValueType::e_identity:
                lua_pushinteger(L, (lua_Integer)t->identityValue());
                return 1;
            case ZFValueType::e_serializableData:
            default:
                ZFLuaErrorOccurredTrim(zfText("[zfl_luaValue] unknown param type, got %s"),
                    t->objectInfo().cString());
                ZFImpl_ZFLua_luaError(L);
                return zffalse;
        }
    }

    zfstring s;
    if(ZFImpl_ZFLua_toString(s, obj, allowEmpty))
    {
        lua_pushstring(L, ZFStringZ2A(s.cString()));
        return 1;
    }

    ZFLuaErrorOccurredTrim(zfText("[zfl_luaValue] unknown param type, got %s"),
        obj->objectInfo().cString());
    ZFImpl_ZFLua_luaError(L);
    return zffalse;
}

zfbool ZFImpl_ZFLua_zfstringAppend(ZF_IN lua_State *L,
                                   ZF_IN_OUT zfstring &s,
                                   ZF_IN_OPT int luaParamOffset /* = 1 */)
{
    int count = (int)lua_gettop(L);
    if(count <= luaParamOffset - 1)
    {
        return zftrue;
    }

    zfstring fmt;
    if(!ZFImpl_ZFLua_toString(fmt, L, luaParamOffset))
    {
        return zffalse;
    }

    zfstring params[ZFMETHOD_MAX_PARAM];
    for(int i = luaParamOffset; i < count; ++i)
    {
        zfautoObject t;
        if(ZFImpl_ZFLua_toObject(t, L, i + 1))
        {
            ZFObjectInfoT(params[i - luaParamOffset], t.toObject());
        }
        else if(ZFImpl_ZFLua_toString(params[i - luaParamOffset], L, i + 1))
        {
            // nothing to do
        }
        else
        {
            ZFImpl_ZFLua_luaObjectInfoT(params[i - luaParamOffset], L, i + 1);
        }
    }

    /* ZFMETHOD_MAX_PARAM */
    zfstringAppend(s, fmt.cString()
            , params[0].cString()
            , params[1].cString()
            , params[2].cString()
            , params[3].cString()
            , params[4].cString()
            , params[5].cString()
            , params[6].cString()
            , params[7].cString()
        );

    return zftrue;
}

int ZFImpl_ZFLua_luaError(ZF_IN lua_State *L)
{
#if ZF_ENV_DEBUG
    zfCoreCriticalMessage(zfText("native lua error"));
#endif
    return luaL_error(L, ZFImpl_ZFLua_dummyError);
}


ZF_NAMESPACE_GLOBAL_END

