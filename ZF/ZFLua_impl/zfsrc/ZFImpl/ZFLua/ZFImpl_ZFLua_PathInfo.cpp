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

static void _ZFP_ZFImpl_ZFLua_implSetupPathInfo_escape(ZF_OUT zfstring &ret,
                                                       ZF_IN const zfchar *p)
{
    const zfchar *pL = p;
    while(*p)
    {
        if(*p == '\'')
        {
            if(p != pL)
            {
                ret.append(pL, p - pL);
            }
            ret += zfText("\\'");
        }
        ++p;
    }
    if(p != pL)
    {
        ret.append(pL, p - pL);
    }
}
static int _ZFP_ZFLuaImportOpen(ZF_IN lua_State *L)
{
    zfautoObject pathInfoHolder;
    ZFImpl_ZFLua_toObject(pathInfoHolder, L, 1);
    v_ZFPathInfo *pathInfo = pathInfoHolder;
    if(pathInfo == zfnull)
    {
        ZFLuaErrorOccurredTrim(
            zfText("[ZFLuaImport] unable to access pathInfo, got: %s"),
            ZFImpl_ZFLua_luaObjectInfo(L, 1, zftrue).cString());
        return ZFImpl_ZFLua_luaError(L);
    }

    zfstring localFilePath;
    if(!ZFImpl_ZFLua_toString(localFilePath, L, 2))
    {
        ZFLuaErrorOccurredTrim(
            zfText("[ZFLuaImport] unable to access localFilePath, got: %s"),
            ZFImpl_ZFLua_luaObjectInfo(L, 2, zftrue).cString());
        return ZFImpl_ZFLua_luaError(L);
    }

    zfblockedAlloc(v_ZFCallback, ret);
    ret->zfv.callbackSerializeCustomDisable();
    ZFInputCallbackForLocalFileT(ret->zfv, pathInfo->zfv, localFilePath);
    if(!ret->zfv.callbackIsValid())
    {
        ZFLuaErrorOccurredTrim(
            zfText("[ZFLuaImport] unable to load local file \"%s\" relative to \"%s\""),
            localFilePath.cString(),
            ZFPathInfoToString(pathInfo->zfv).cString());
        return ZFImpl_ZFLua_luaError(L);
    }

    ZFImpl_ZFLua_luaPush(L, ret);
    return 1;
}
void ZFImpl_ZFLua_implSetupPathInfo(ZF_OUT zfstring &ret,
                                    ZF_IN const ZFPathInfo *pathInfo)
{
    if(pathInfo == zfnull ||  pathInfo->pathType.isEmpty() || pathInfo->pathData.isEmpty())
    {
        return ;
    }
    // no endl, to prevent native lua error from having wrong line number
    ret += zfText(
            "local function zfl_pathInfo()"
            "    return ZFPathInfo('"
        );
    _ZFP_ZFImpl_ZFLua_implSetupPathInfo_escape(ret, pathInfo->pathType);
    ret += ZFSerializableKeyword_ZFPathInfo_separator;
    _ZFP_ZFImpl_ZFLua_implSetupPathInfo_escape(ret, pathInfo->pathData);
    ret += zfText(
            "');"
            "end;"
            "local function ZFLuaImport(localFilePath, ...)"
            "    return ZFLuaExecute(_ZFP_ZFLuaImportOpen(zfl_pathInfo(), localFilePath), ...);"
            "end;"
        );
}

// ============================================================
ZFImpl_ZFLua_implSetupCallback_DEFINE(PathInfo, {
        ZFImpl_ZFLua_luaCFunctionRegister(L, zfText("_ZFP_ZFLuaImportOpen"), _ZFP_ZFLuaImportOpen);

        /*
         * the default version,
         * would be hide by local one
         */
        ZFImpl_ZFLua_execute(L, zfText(
                "function zfl_pathInfo()\n"
                "    return zfnull;\n"
                "end\n"
            ));

        ZFImpl_ZFLua_execute(L, zfText(
                "function ZFLuaImport(localFilePath, ...)\n"
                "    error('ZFLuaImport can only be called within file context', 2);\n"
                "    return zffalse;\n"
                "end\n"
            ));
    }, {
    })

ZF_NAMESPACE_GLOBAL_END

