/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFLuaExecute.h"
#include "ZFLuaState.h"
#include "ZFLuaGC.h"
#include "protocol/ZFProtocolZFLua.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
static zfbool _ZFP_ZFLuaExecute(ZF_IN const ZFPathInfo *pathInfoOrNull,
                                ZF_IN const ZFInput &input,
                                ZF_IN zfautoObject *luaResult,
                                ZF_IN const ZFCoreArray<zfautoObject> *luaParams,
                                ZF_IN void *L)
{
    if(!input.callbackIsValid())
    {
        return zffalse;
    }
    zfstring errHint;
    if(!ZFPROTOCOL_ACCESS(ZFLua)->luaExecute(L ? L : ZFLuaState(), pathInfoOrNull, input, luaResult, luaParams, &errHint))
    {
        if(!errHint.isEmpty())
        {
            ZFLuaErrorOccurredTrim("[ZFLua] %s", errHint.cString());
        }
        return zffalse;
    }
    else
    {
        return zftrue;
    }
}

ZFMETHOD_FUNC_DEFINE_3(zfautoObject, ZFLuaExecute,
                       ZFMP_IN(const ZFInput &, input),
                       ZFMP_IN_OPT(const ZFCoreArray<zfautoObject> *, luaParams, zfnull),
                       ZFMP_IN_OPT(void *, L, zfnull))
{
    zfautoObject ret;
    if(_ZFP_ZFLuaExecute(input.pathInfo(), input, &ret, luaParams, L))
    {
        ZFLuaGC(L);
        return ret;
    }
    else
    {
        return zfnull;
    }
}

ZFMETHOD_FUNC_DEFINE_3(zfautoObject, ZFLuaExecute,
                       ZFMP_IN(const zfchar *, buf),
                       ZFMP_IN_OPT(const ZFCoreArray<zfautoObject> *, luaParams, zfnull),
                       ZFMP_IN_OPT(void *, L, zfnull))
{
    zfautoObject ret;
    if(_ZFP_ZFLuaExecute(zfnull, ZFInputForBuffer(buf), &ret, luaParams, L))
    {
        ZFLuaGC(L);
        return ret;
    }
    else
    {
        return zfnull;
    }
}

ZF_NAMESPACE_GLOBAL_END

