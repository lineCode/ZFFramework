/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFObjectIO.h"
#include "ZFObjectImpl.h"

#include "../ZFSTLWrapper/zfstl_string.h"
#include "../ZFSTLWrapper/zfstl_map.h"
#include "../ZFSTLWrapper/zfstl_deque.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassLikePOD _ZFP_ZFObjectIOData
{
public:
    zfstring registerSig;
    _ZFP_ZFObjectIOCallback_checker checker;
    _ZFP_ZFObjectIOCallback_fromInput fromInput;
    _ZFP_ZFObjectIOCallback_toOutput toOutput;
};
static zfstlmap<zfstlstringZ, zfstldeque<_ZFP_ZFObjectIOData> > &_ZFP_ZFObjectIODataMap(void)
{
    static zfstlmap<zfstlstringZ, zfstldeque<_ZFP_ZFObjectIOData> > m;
    return m;
}

void _ZFP_ZFObjectIORegister(ZF_IN const zfchar *registerSig,
                             ZF_IN const zfchar *fileExt,
                             ZF_IN _ZFP_ZFObjectIOCallback_checker checker,
                             ZF_IN _ZFP_ZFObjectIOCallback_fromInput fromInput,
                             ZF_IN _ZFP_ZFObjectIOCallback_toOutput toOutput)
{
    zfCoreMutexLocker();
    zfstlmap<zfstlstringZ, zfstldeque<_ZFP_ZFObjectIOData> > &m = _ZFP_ZFObjectIODataMap();
    zfstlmap<zfstlstringZ, zfstldeque<_ZFP_ZFObjectIOData> >::iterator it = m.find(fileExt);
    if(it != m.end())
    {
        for(zfstlsize i = 0; i < it->second.size(); ++i)
        {
            if(it->second[i].registerSig.compare(registerSig) == 0)
            {
                zfCoreCriticalMessageTrim(zfTextA("[ZFObjectIO] \"%s\" already registered"),
                    zfsCoreZ2A(registerSig));
                return ;
            }
        }
    }

    _ZFP_ZFObjectIOData data;
    data.registerSig = registerSig;
    data.checker = checker;
    data.fromInput = fromInput;
    data.toOutput = toOutput;
    m[fileExt].push_back(data);
}
void _ZFP_ZFObjectIOUnregister(ZF_IN const zfchar *registerSig)
{
    zfCoreMutexLocker();
    zfstlmap<zfstlstringZ, zfstldeque<_ZFP_ZFObjectIOData> > &m = _ZFP_ZFObjectIODataMap();
    for(zfstlmap<zfstlstringZ, zfstldeque<_ZFP_ZFObjectIOData> >::iterator it = m.begin(); it != m.end(); ++it)
    {
        for(zfstlsize i = 0; i < it->second.size(); ++i)
        {
            if(it->second[i].registerSig.compare(registerSig) == 0)
            {
                it->second.erase(it->second.begin() + i);
                if(it->second.empty())
                {
                    m.erase(it);
                }
                return ;
            }
        }
    }
}

// ============================================================
static zfbool _ZFP_ZFObjectIOCheckAction(ZF_OUT _ZFP_ZFObjectIOData *&ret,
                                         ZF_IN const ZFCallback &callback,
                                         ZF_OUT_OPT zfstring *outErrorHint,
                                         ZF_IN_OUT zfstlmap<zfstlstringZ, zfstldeque<_ZFP_ZFObjectIOData> >::iterator &it,
                                         ZF_IN const zfchar *fileExt)
{
    for(zfstlsize i = it->second.size() - 1; i != (zfstlsize)-1; --i)
    {
        _ZFP_ZFObjectIOData &d = it->second[i];
        if(d.checker(*callback.pathInfo(), fileExt))
        {
            ret = &d;
            // move recently matched to tail for better performance
            if(i != it->second.size() - 1)
            {
                _ZFP_ZFObjectIOData t = d;
                it->second.erase(it->second.begin() + i);
                it->second.push_back(t);
            }
            return zftrue;
        }
    }
    return zffalse;
}
static zfbool _ZFP_ZFObjectIOCheck(ZF_OUT _ZFP_ZFObjectIOData *&ret,
                                   ZF_IN const ZFCallback &callback,
                                   ZF_OUT_OPT zfstring *outErrorHint = zfnull)
{
    if(callback.pathInfo() == zfnull)
    {
        zfstringAppend(outErrorHint, zfText("callback does not contain pathInfo"));
        return zffalse;
    }
    const zfchar *tmp = callback.pathInfo()->pathData.cString();
    const zfchar *fileExt = tmp + callback.pathInfo()->pathData.length() - 1;
    while(*fileExt != '.' && fileExt > tmp) {--fileExt;}
    if(*fileExt != '.')
    {
        fileExt = zfnull;
    }
    else
    {
        ++fileExt;
    }
    if(fileExt == zfnull)
    {
        fileExt = zfText("");
    }

    zfCoreMutexLocker();
    zfstlmap<zfstlstringZ, zfstldeque<_ZFP_ZFObjectIOData> > &m = _ZFP_ZFObjectIODataMap();
    zfstlmap<zfstlstringZ, zfstldeque<_ZFP_ZFObjectIOData> >::iterator it = m.find(fileExt);
    if(it != m.end())
    {
        if(_ZFP_ZFObjectIOCheckAction(ret, callback, outErrorHint, it, fileExt))
        {
            return zftrue;
        }
    }
    if(!zfsIsEmpty(fileExt))
    {
        it = m.find(zfText(""));
        if(it != m.end())
        {
            if(_ZFP_ZFObjectIOCheckAction(ret, callback, outErrorHint, it, fileExt))
            {
                return zftrue;
            }
        }
    }
    zfstringAppend(outErrorHint,
        zfText("no impl can resolve: %s"),
        ZFPathInfoToString(*callback.pathInfo()).cString());
    return zffalse;
}
zfbool ZFObjectIOLoad(ZF_OUT zfautoObject &ret,
                      ZF_IN const ZFInputCallback &input,
                      ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */)
{
    _ZFP_ZFObjectIOData *data = zfnull;
    if(!_ZFP_ZFObjectIOCheck(data, input, outErrorHint))
    {
        return zffalse;
    }
    else
    {
        return data->fromInput(ret, input, outErrorHint);
    }
}
zfautoObject ZFObjectIOLoad(ZF_IN const ZFInputCallback &input,
                            ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */)
{
    zfautoObject ret;
    ZFObjectIOLoad(ret, input, outErrorHint);
    return ret;
}
zfbool ZFObjectIOSave(ZF_IN_OUT const ZFOutputCallback &output,
                      ZF_IN ZFObject *obj,
                      ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */)
{
    _ZFP_ZFObjectIOData *data = zfnull;
    if(!_ZFP_ZFObjectIOCheck(data, output, outErrorHint))
    {
        return zffalse;
    }
    else
    {
        return data->toOutput(output, obj, outErrorHint);
    }
}

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(zfbool, ZFObjectIOLoad, ZFMP_OUT(zfautoObject &, ret), ZFMP_IN(const ZFInputCallback &, input), ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zfautoObject, ZFObjectIOLoad, ZFMP_IN(const ZFInputCallback &, input), ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(zfbool, ZFObjectIOSave, ZFMP_IN_OUT(const ZFOutputCallback &, output), ZFMP_IN(ZFObject *, obj), ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull))

ZF_NAMESPACE_GLOBAL_END
#endif

