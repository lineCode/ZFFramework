/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFImpl_ZFLua_PathInfo.h"

#include "ZFCore/ZFSTLWrapper/zfstl_map.h"
#include "ZFCore/ZFSTLWrapper/zfstl_string.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
static zfstlmap<zfstlstringZ, zfbool> &_ZFP_ZFLuaImportOnceData(void)
{
    static zfstlmap<zfstlstringZ, zfbool> d;
    return d;
}
static int _ZFP_ZFLuaLocalInput(ZF_IN lua_State *L, ZF_IN zfbool requireValid, ZF_IN zfbool importOnce);

// ============================================================
static int _ZFP_ZFLuaImport(ZF_IN lua_State *L)
{
    return _ZFP_ZFLuaLocalInput(L, zftrue, zffalse);
}
static int _ZFP_ZFLuaImportOnce(ZF_IN lua_State *L)
{
    return _ZFP_ZFLuaLocalInput(L, zftrue, zftrue);
}
static int _ZFP_ZFLuaImportOnceReset(ZF_IN lua_State *L)
{
    _ZFP_ZFLuaImportOnceData().clear();
    return 0;
}
static int _ZFP_ZFLuaImportAll(ZF_IN lua_State *L);
static int _ZFP_ZFLuaRes(ZF_IN lua_State *L)
{
    return _ZFP_ZFLuaLocalInput(L, zffalse, zffalse);
}

// ============================================================
static void _ZFP_ZFLuaImportOnceCheck(ZF_IN_OUT v_ZFCallback *ret, ZF_IN zfbool importOnce)
{
    if(importOnce)
    {
        if(ret != zfnull && ret->zfv.callbackId() != zfnull)
        {
            zfstlmap<zfstlstringZ, zfbool> &m = _ZFP_ZFLuaImportOnceData();
            if(m.find(ret->zfv.callbackId()) != m.end())
            {
                ret->zfv = ZFCallbackNull();
            }
            else
            {
                m[ret->zfv.callbackId()] = zftrue;
            }
        }
    }
}
static int _ZFP_ZFLuaLocalInput(ZF_IN lua_State *L, ZF_IN zfbool requireValid, ZF_IN zfbool importOnce)
{
    zfautoObject paramHolder;
    if(ZFImpl_ZFLua_toObject(paramHolder, L, 2)
        && ZFCastZFObject(v_ZFCallback *, paramHolder) != zfnull)
    {
        _ZFP_ZFLuaImportOnceCheck(paramHolder, importOnce);
        ZFImpl_ZFLua_luaPush(L, paramHolder);
        return 1;
    }

    zfblockedAlloc(v_ZFCallback, ret);

    ZFImpl_ZFLua_toObject(paramHolder, L, 1);
    v_ZFPathInfo *pathInfo = paramHolder;
    if(pathInfo == zfnull)
    {
        if(requireValid)
        {
            ZFLuaErrorOccurredTrim(
                "unable to access pathInfo, got: %s",
                ZFImpl_ZFLua_luaObjectInfo(L, 1, zftrue).cString());
            return ZFImpl_ZFLua_luaError(L);
        }
        else
        {
            ZFImpl_ZFLua_luaPush(L, ret);
            return 1;
        }
    }

    zfstring localFilePath;
    if(!ZFImpl_ZFLua_toString(localFilePath, L, 2))
    {
        if(requireValid)
        {
            ZFLuaErrorOccurredTrim(
                "unable to access localFilePath, got: %s",
                ZFImpl_ZFLua_luaObjectInfo(L, 2, zftrue).cString());
            return ZFImpl_ZFLua_luaError(L);
        }
        else
        {
            ZFImpl_ZFLua_luaPush(L, ret);
            return 1;
        }
    }

    ret->zfv.callbackSerializeCustomDisable(zftrue);
    ZFInputForLocalFileT(ret->zfv, pathInfo->zfv, localFilePath);
    if(!ret->zfv.callbackIsValid())
    {
        if(requireValid)
        {
            ZFLuaErrorOccurredTrim(
                "unable to load local file \"%s\" relative to \"%s\"",
                localFilePath.cString(),
                ZFPathInfoToString(pathInfo->zfv).cString());
            return ZFImpl_ZFLua_luaError(L);
        }
        else
        {
            ZFImpl_ZFLua_luaPush(L, ret);
            return 1;
        }
    }

    _ZFP_ZFLuaImportOnceCheck(ret, importOnce);
    ZFImpl_ZFLua_luaPush(L, ret);
    return 1;
}

// ============================================================
static void _ZFP_ZFLuaImportAllLoop(ZF_IN lua_State *L,
                                    ZF_IN const ZFFilePathInfoData &impl,
                                    ZF_IN const ZFPathInfo &pathInfo,
                                    ZF_IN const ZFListener &importCallback,
                                    ZF_IN ZFObject *importCallbackUserData,
                                    ZF_IN zfbool recursive);
static int _ZFP_ZFLuaImportAllExecute(ZF_IN lua_State *L,
                                      ZF_IN v_ZFPathInfo *pathInfo,
                                      ZF_IN const ZFListener &importCallback,
                                      ZF_IN ZFObject *importCallbackUserData);
static int _ZFP_ZFLuaImportAllWrap(ZF_IN lua_State *L,
                                   ZF_IN const ZFPathInfo &pathInfo,
                                   ZF_IN const ZFListener &importCallback,
                                   ZF_IN ZFObject *importCallbackUserData,
                                   ZF_IN zfbool recursive);
static int _ZFP_ZFLuaImportAll(ZF_IN lua_State *L)
{
    int count = (int)lua_gettop(L);

    ZFListener importCallback;
    if(count >= 3)
    {
        zfautoObject importCallbackHolder;
        if(ZFImpl_ZFLua_toCallback(importCallbackHolder, L, 3))
        {
            importCallback = ZFCastZFObject(v_ZFCallback *, importCallbackHolder)->zfv;
        }
    }

    zfautoObject importCallbackUserData;
    if(count >= 4)
    {
        ZFImpl_ZFLua_toObject(importCallbackUserData, L, 4);
    }

    zfbool recursive = zftrue;
    if(count >= 5)
    {
        zfautoObject recursiveHolder;
        if(ZFImpl_ZFLua_toNumberT(recursiveHolder, L, 5))
        {
            ZFValue *t = recursiveHolder;
            if(t != zfnull && t->valueConvertableTo(ZFValueType::e_bool))
            {
                recursive = t->boolValue();
            }
        }
    }

    zfautoObject paramHolder;
    if(ZFImpl_ZFLua_toObject(paramHolder, L, 2)
        && ZFCastZFObject(v_ZFPathInfo *, paramHolder) != zfnull)
    {
        return _ZFP_ZFLuaImportAllWrap(
            L,
            ZFCastZFObject(v_ZFPathInfo *, paramHolder)->zfv,
            importCallback,
            importCallbackUserData,
            recursive);
    }
    else
    {
        ZFImpl_ZFLua_toObject(paramHolder, L, 1);
        v_ZFPathInfo *pathInfo = paramHolder;
        if(pathInfo == zfnull)
        {
            ZFLuaErrorOccurredTrim(
                "unable to access pathInfo, got: %s",
                ZFImpl_ZFLua_luaObjectInfo(L, 1, zftrue).cString());
            return ZFImpl_ZFLua_luaError(L);
        }

        zfstring localFilePath;
        if(!ZFImpl_ZFLua_toString(localFilePath, L, 2))
        {
            ZFLuaErrorOccurredTrim(
                "unable to access localFilePath, got: %s",
                ZFImpl_ZFLua_luaObjectInfo(L, 2, zftrue).cString());
            return ZFImpl_ZFLua_luaError(L);
        }

        ZFPathInfo localPathInfo;
        ZFPathInfoForLocalFileT(localPathInfo, pathInfo->zfv, localFilePath);
        return _ZFP_ZFLuaImportAllWrap(L, localPathInfo, importCallback, importCallbackUserData, recursive);
    }
}
static int _ZFP_ZFLuaImportAllWrap(ZF_IN lua_State *L,
                                   ZF_IN const ZFPathInfo &pathInfo,
                                   ZF_IN const ZFListener &importCallback,
                                   ZF_IN ZFObject *importCallbackUserData,
                                   ZF_IN zfbool recursive)
{
    const ZFFilePathInfoData *impl = ZFFilePathInfoDataGet(pathInfo.pathType);
    if(impl == zfnull)
    {
        ZFLuaErrorOccurredTrim(
            "no such path type: %s",
            ZFPathInfoToString(pathInfo).cString());
        return ZFImpl_ZFLua_luaError(L);
    }
    if(impl->callbackIsDir(pathInfo.pathData))
    {
        _ZFP_ZFLuaImportAllLoop(L, *impl, pathInfo, importCallback, importCallbackUserData, recursive);
        return 0;
    }
    else
    {
        zfblockedAlloc(v_ZFPathInfo, pathInfoHolder, pathInfo);
        return _ZFP_ZFLuaImportAllExecute(L, pathInfoHolder, importCallback, importCallbackUserData);
    }
}
static int _ZFP_ZFLuaImportAllExecute(ZF_IN lua_State *L,
                                      ZF_IN v_ZFPathInfo *pathInfo,
                                      ZF_IN const ZFListener &importCallback,
                                      ZF_IN ZFObject *importCallbackUserData)
{
    ZFInput input;
    input.callbackSerializeCustomDisable(zftrue);
    ZFInputForPathInfoT(input, pathInfo->zfv.pathType, pathInfo->zfv.pathData);
    if(!input.callbackIsValid())
    {
        ZFLuaErrorOccurredTrim(
            "unable to load: %s",
            ZFPathInfoToString(pathInfo->zfv).cString());
        return ZFImpl_ZFLua_luaError(L);
    }

    zfstlmap<zfstlstringZ, zfbool> &m = _ZFP_ZFLuaImportOnceData();
    if(m.find(input.callbackId()) != m.end())
    {
        return 0;
    }
    m[input.callbackId()] = zftrue;

    if(importCallback.callbackIsValid())
    {
        importCallback.execute(ZFListenerData().param0Set(pathInfo), importCallbackUserData);
    }
    ZFLuaExecute(input, zfnull, L);
    return 0;
}
static void _ZFP_ZFLuaImportAllLoop(ZF_IN lua_State *L,
                                    ZF_IN const ZFFilePathInfoData &impl,
                                    ZF_IN const ZFPathInfo &pathInfo,
                                    ZF_IN const ZFListener &importCallback,
                                    ZF_IN ZFObject *importCallbackUserData,
                                    ZF_IN zfbool recursive)
{
    ZFFileFindData fd;
    if(impl.callbackFindFirst(fd, pathInfo.pathData))
    {
        zfblockedAlloc(v_ZFPathInfo, childPathInfo);
        childPathInfo->zfv.pathType = pathInfo.pathType;
        do
        {
            childPathInfo->zfv.pathData.removeAll();
            if(!impl.callbackToChild(pathInfo.pathData, childPathInfo->zfv.pathData, fd.fileName()))
            {
                continue;
            }

            if(fd.fileIsDir())
            {
                if(!recursive)
                {
                    continue;
                }
                _ZFP_ZFLuaImportAllLoop(L, impl, childPathInfo->zfv, importCallback, importCallbackUserData, recursive);
            }
            else
            {
                _ZFP_ZFLuaImportAllExecute(L, childPathInfo, importCallback, importCallbackUserData);
            }
        } while(impl.callbackFindNext(fd));
        impl.callbackFindClose(fd);
    }
}

// ============================================================
ZFImpl_ZFLua_implSetupCallback_DEFINE(PathInfo, {
        ZFImpl_ZFLua_luaCFunctionRegister(L, "_ZFP_ZFLuaImport", _ZFP_ZFLuaImport);
        ZFImpl_ZFLua_luaCFunctionRegister(L, "_ZFP_ZFLuaImportOnce", _ZFP_ZFLuaImportOnce);
        ZFImpl_ZFLua_luaCFunctionRegister(L, "_ZFP_ZFLuaImportAll", _ZFP_ZFLuaImportAll);
        ZFImpl_ZFLua_luaCFunctionRegister(L, "ZFLuaImportOnceReset", _ZFP_ZFLuaImportOnceReset);
        ZFImpl_ZFLua_luaCFunctionRegister(L, "_ZFP_ZFLuaRes", _ZFP_ZFLuaRes);

        /*
         * the default version,
         * would be hide by local one
         */
        ZFImpl_ZFLua_execute(L,
                "function ZFLuaPathInfo()\n"
                "    return zfnull;\n"
                "end\n"
            );
    }, {
    })

ZFImpl_ZFLua_implPathInfo_DEFINE(ZFLuaPathInfo,
        "function ()"
        "    return zfl_l;"
        "end"
    )
ZFImpl_ZFLua_implPathInfo_DEFINE(ZFLuaImport,
        "function (l, ...)"
        "    return ZFLuaExecute(_ZFP_ZFLuaImport(zfl_l, l), ZFCoreArrayCreate(...), zfl_L());"
        "end"
    )
ZFImpl_ZFLua_implPathInfo_DEFINE(ZFLuaImportOnce,
        "function (l, ...)"
        "    return ZFLuaExecute(_ZFP_ZFLuaImportOnce(zfl_l, l), ZFCoreArrayCreate(...), zfl_L());"
        "end"
    )
ZFImpl_ZFLua_implPathInfo_DEFINE(ZFLuaImportAll,
        "function (l, ...)"
        "    _ZFP_ZFLuaImportAll(zfl_l, l, ...);"
        "end"
    )
ZFImpl_ZFLua_implPathInfo_DEFINE(ZFLuaRes,
        "function (l)"
        "    return ZFObjectIOLoad(_ZFP_ZFLuaRes(zfl_l, l));"
        "end"
    )
ZFImpl_ZFLua_implPathInfo_DEFINE(ZFLuaExecute,
        "function (...)"
        "    local arg={...};"
        "    ZF.ZFLuaExecute(arg[1] or ZFCallback(), arg[2] or ZFCoreArray(), arg[3] or zfl_L());"
        "end"
    )
ZFImpl_ZFLua_implPathInfo_DEFINE(ZFLuaGC,
        "function (...)"
        "    local arg={...};"
        "    ZF.ZFLuaGC(arg[1] or zfl_L());"
        "end"
    )

ZF_NAMESPACE_GLOBAL_END

