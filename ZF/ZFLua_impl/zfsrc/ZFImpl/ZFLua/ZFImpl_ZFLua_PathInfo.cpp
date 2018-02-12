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
            "    local pathInfo = ZFPathInfo();"
            "    pathInfo:pathTypeSet('"
        );
    _ZFP_ZFImpl_ZFLua_implSetupPathInfo_escape(ret, pathInfo->pathType);
    ret += zfText("');"
            "    pathInfo:pathDataSet('"
        );
    _ZFP_ZFImpl_ZFLua_implSetupPathInfo_escape(ret, pathInfo->pathData);
    ret += zfText("');"
            "    return pathInfo;"
            "end;"
            "local function ZFLuaExecuteLocalFileT(localFilePath, ...)"
            "    return ZFLuaExecuteT(ZFInputCallbackForLocalFile(zfl_pathInfo(), localFilePath), ...);"
            "end;"
            "local function ZFLuaExecuteLocalFile(localFilePath, ...)"
            "    return ZFLuaExecute(ZFInputCallbackForLocalFile(zfl_pathInfo(), localFilePath), ...);"
            "end;"
        );
}

// ============================================================
ZFImpl_ZFLua_implSetupCallback_DEFINE(PathInfo, {
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
                "function ZFLuaExecuteLocalFileT(localFilePath, ...)\n"
                "    error('ZFLuaExecuteLocalFile can only be called within file context', 2);\n"
                "    return zffalse;\n"
                "end\n"
                "function ZFLuaExecuteLocalFile(localFilePath, ...)\n"
                "    error('ZFLuaExecuteLocalFile can only be called within file context', 2);\n"
                "    return zffalse;\n"
                "end\n"
            ));
    }, {
    })

ZF_NAMESPACE_GLOBAL_END

