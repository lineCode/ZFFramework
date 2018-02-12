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

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassPOD _ZFP_ZFObjectIOData
{
public:
    zfstring registerSig;
    _ZFP_ZFObjectIOCallback_checker checker;
    _ZFP_ZFObjectIOCallback_fromInput fromInput;
    _ZFP_ZFObjectIOCallback_toOutput toOutput;
};
static ZFCoreArrayPOD<_ZFP_ZFObjectIOData> &_ZFP_ZFObjectIODataMap(void)
{
    static ZFCoreArrayPOD<_ZFP_ZFObjectIOData> m;
    return m;
}

void _ZFP_ZFObjectIORegister(ZF_IN const zfchar *registerSig,
                             ZF_IN _ZFP_ZFObjectIOCallback_checker checker,
                             ZF_IN _ZFP_ZFObjectIOCallback_fromInput fromInput,
                             ZF_IN _ZFP_ZFObjectIOCallback_toOutput toOutput)
{
    ZFCoreArrayPOD<_ZFP_ZFObjectIOData> &m = _ZFP_ZFObjectIODataMap();
    for(zfindex i = 0; i < m.count(); ++i)
    {
        if(zfscmpTheSame(m[i].registerSig.cString(), registerSig))
        {
            zfCoreCriticalMessageTrim(zfTextA("[ZFObjectIO] \"%s\" already registered"),
                zfsCoreZ2A(registerSig));
            return ;
        }
    }

    _ZFP_ZFObjectIOData data;
    data.checker = checker;
    data.fromInput = fromInput;
    data.toOutput = toOutput;
    m.add(data);
}
void _ZFP_ZFObjectIOUnregister(ZF_IN const zfchar *registerSig)
{
    ZFCoreArrayPOD<_ZFP_ZFObjectIOData> &m = _ZFP_ZFObjectIODataMap();
    for(zfindex i = 0; i < m.count(); ++i)
    {
        if(zfscmpTheSame(m[i].registerSig.cString(), registerSig))
        {
            m.remove(i);
            break;
        }
    }
}

// ============================================================
static zfbool _ZFP_ZFObjectIOCheck(ZF_OUT _ZFP_ZFObjectIOData &ret,
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

    zfCoreMutexLocker();
    ZFCoreArrayPOD<_ZFP_ZFObjectIOData> &m = _ZFP_ZFObjectIODataMap();
    for(zfindex i = m.count() - 1; i != zfindexMax(); --i)
    {
        if(m[i].checker(*callback.pathInfo(), fileExt))
        {
            ret = m[i];
            // move recently matched to tail for better performance
            m.move(i, zfindexMax());
            return zftrue;
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
    _ZFP_ZFObjectIOData data;
    if(!_ZFP_ZFObjectIOCheck(data, input, outErrorHint))
    {
        return zffalse;
    }
    else
    {
        return data.fromInput(ret, input, outErrorHint);
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
    _ZFP_ZFObjectIOData data;
    if(!_ZFP_ZFObjectIOCheck(data, output, outErrorHint))
    {
        return zffalse;
    }
    else
    {
        return data.toOutput(output, obj, outErrorHint);
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

