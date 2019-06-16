/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFLog.h"
#include "ZFTime.h"
#include "ZFOutputForFormat.h"

ZF_NAMESPACE_GLOBAL_BEGIN

static ZFMutex *_ZFP_ZFLogMutex = zfnull;
zfclassFwd _ZFP_I_ZFLogFormat;
static _ZFP_I_ZFLogFormat *_ZFP_ZFLogFormatHolder = zfnull;

// ============================================================
zfclass _ZFP_I_ZFLogFormat : zfextends ZFObject, zfimplements ZFOutputFormat
{
    ZFOBJECT_DECLARE(_ZFP_I_ZFLogFormat, ZFObject)
    ZFIMPLEMENTS_DECLARE(ZFOutputFormat)

public:
    zfbool autoSpace;
    zfbool autoEndl;

protected:
    zfoverride
    virtual void objectOnInit(void)
    {
        zfsuper::objectOnInit();
        this->autoSpace = zftrue;
        this->autoEndl = zftrue;
    }

protected:
    zfoverride
    virtual void format(ZF_IN_OUT zfstring &ret,
                        ZF_IN ZFOutputFormatStepEnum outputStep,
                        ZF_IN const zfchar *src,
                        ZF_IN zfindex srcLen,
                        ZF_IN zfindex writtenLen,
                        ZF_IN zfindex outputCount,
                        ZF_IN_OUT_OPT void *&state)
    {
        switch(outputStep)
        {
            case ZFOutputFormatStep::e_OnInit:
                if(_ZFP_ZFLogMutex != zfnull)
                {
                    _ZFP_ZFLogMutex->mutexLock();
                }
                break;
            case ZFOutputFormatStep::e_OnDealloc:
                this->autoSpace = zftrue;
                this->autoEndl = zftrue;
                if(_ZFP_ZFLogMutex != zfnull)
                {
                    _ZFP_ZFLogMutex->mutexUnlock();
                }
                break;
            case ZFOutputFormatStep::e_OnOutput:
                if(this->autoSpace && outputCount > 0)
                {
                    ret += " ";
                }
                ret.append(src, srcLen);
                break;
            case ZFOutputFormatStep::e_OnOutputEnd:
                if(this->autoEndl)
                {
                    ret += "\n";
                }
                break;
            default:
                zfCoreCriticalShouldNotGoHere();
                break;
        }
    }
};

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFLogDataHolder, ZFLevelZFFrameworkEssential)
{
    _ZFP_ZFLogMutex = zfAlloc(ZFMutex);
    _ZFP_ZFLogFormatHolder = zfAlloc(_ZFP_I_ZFLogFormat);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFLogDataHolder)
{
    zfRelease(_ZFP_ZFLogFormatHolder);
    _ZFP_ZFLogFormatHolder = zfnull;
    zfRelease(_ZFP_ZFLogMutex);
    _ZFP_ZFLogMutex = zfnull;
}
ZF_GLOBAL_INITIALIZER_END(ZFLogDataHolder)

ZFMETHOD_FUNC_DEFINE_0(ZFMutex *, ZFLogMutex)
{
    return _ZFP_ZFLogMutex;
}

zfstring _ZFP_ZFLogHeaderString(ZF_IN const ZFCallerInfo &callerInfo)
{
    zfstring ret;
    ZFTimeInfo ti = ZFTime::currentTimeInfo();
    zfstringAppend(ret,
        "%02d:%02d:%02d.%03d ",
        ti.hour,
        ti.minute,
        ti.second,
        ti.miliSecond);
    callerInfo.callerInfoT(ret);
    return ret;
}

// ============================================================
static zfindex _ZFP_zfLogOnOutput(ZF_IN const void *src, ZF_IN zfindex size)
{
    if(size == zfindexMax())
    {
        size = zfslen((const zfchar *)src) * sizeof(zfchar);
    }
    ZFOutputDefault().execute(src, size);
    return size;
}
ZFOutput zfLogTrimT(void)
{
    ZFOutput ret;
    ret.callbackSerializeCustomDisable(zftrue);
    ZFOutputForFormatT(ret, ZFCallbackForFunc(_ZFP_zfLogOnOutput), _ZFP_ZFLogFormatHolder);
    return ret;
}

// ============================================================
const ZFOutput &operator << (const ZFOutput &output, _ZFP_ZFLogAutoSpaceOn const &v)
{
    _ZFP_I_ZFLogFormat *format = ZFCastZFObject(_ZFP_I_ZFLogFormat *,
        ZFOutputForFormatGetFormat(output));
    if(format != zfnull)
    {
        format->autoSpace = zftrue;
    }
    return output;
}
const ZFOutput &operator << (const ZFOutput &output, _ZFP_ZFLogAutoSpaceOff const &v)
{
    _ZFP_I_ZFLogFormat *format = ZFCastZFObject(_ZFP_I_ZFLogFormat *,
        ZFOutputForFormatGetFormat(output));
    if(format != zfnull)
    {
        format->autoSpace = zffalse;
    }
    return output;
}
const ZFOutput &operator << (const ZFOutput &output, _ZFP_ZFLogAutoEndlOn const &v)
{
    _ZFP_I_ZFLogFormat *format = ZFCastZFObject(_ZFP_I_ZFLogFormat *,
        ZFOutputForFormatGetFormat(output));
    if(format != zfnull)
    {
        format->autoEndl = zftrue;
    }
    return output;
}
const ZFOutput &operator << (const ZFOutput &output, _ZFP_ZFLogAutoEndlOff const &v)
{
    _ZFP_I_ZFLogFormat *format = ZFCastZFObject(_ZFP_I_ZFLogFormat *,
        ZFOutputForFormatGetFormat(output));
    if(format != zfnull)
    {
        format->autoEndl = zffalse;
    }
    return output;
}

const _ZFP_ZFLogAutoSpaceOn ZFLogAutoSpaceOn;
const _ZFP_ZFLogAutoSpaceOff ZFLogAutoSpaceOff;
const _ZFP_ZFLogAutoEndlOn ZFLogAutoEndlOn;
const _ZFP_ZFLogAutoEndlOff ZFLogAutoEndlOff;

// ============================================================
// other convenient method
zfstring _ZFP_zfLogCurTimeString(void)
{
    zfstring s;
    ZFTimeInfo ti = ZFTime::currentTimeInfo();
    zfstringAppend(s,
        "%02d:%02d:%02d.%03d",
        ti.hour,
        ti.minute,
        ti.second,
        ti.miliSecond);
    return s;
}
ZFMETHOD_FUNC_DEFINE_0(const zfchar *, zfLogCurTimeString)
{
    return zfLogCurTimeString();
}

// ============================================================
ZFMETHOD_USER_REGISTER_8({
        v_ZFCallback *owner = ZFCastZFObjectUnchecked(v_ZFCallback *, invokerObject);
        zfstring buf;
        zfstringAppend(buf, fmt
                , param0
                , param1
                , param2
                , param3
                , param4
                , param5
                , param6
            );
        ZFOutput output = owner->zfv;
        output.execute(buf.cString(), buf.length());
        return owner;
    }, v_ZFCallback, v_ZFCallback *, log
    , ZFMP_IN(const zfchar *, fmt)
    , ZFMP_IN_OPT(const zfchar *, param0, zfnull)
    , ZFMP_IN_OPT(const zfchar *, param1, zfnull)
    , ZFMP_IN_OPT(const zfchar *, param2, zfnull)
    , ZFMP_IN_OPT(const zfchar *, param3, zfnull)
    , ZFMP_IN_OPT(const zfchar *, param4, zfnull)
    , ZFMP_IN_OPT(const zfchar *, param5, zfnull)
    , ZFMP_IN_OPT(const zfchar *, param6, zfnull)
    ) /** ZFMETHOD_MAX_PARAM */

ZF_NAMESPACE_GLOBAL_END

