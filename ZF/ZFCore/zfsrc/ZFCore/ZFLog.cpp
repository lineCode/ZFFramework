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
#include "ZFOutputForConsole.h"

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
                        ZF_IN zfself::OutputStep outputStep,
                        ZF_IN const zfchar *src,
                        ZF_IN zfindex srcLen,
                        ZF_IN zfindex writtenLen)
    {
        switch(outputStep)
        {
            case OutputStepBegin:
                if(_ZFP_ZFLogMutex != zfnull)
                {
                    _ZFP_ZFLogMutex->mutexLock();
                }
                break;
            case OutputStepEnd:
                if(this->autoEndl)
                {
                    ret += zfText("\n");
                }
                this->autoSpace = zftrue;
                this->autoEndl = zftrue;
                if(_ZFP_ZFLogMutex != zfnull)
                {
                    _ZFP_ZFLogMutex->mutexUnlock();
                }
                break;
            case OutputStepAction:
                if(this->autoSpace && writtenLen > 0)
                {
                    ret += zfText(" ");
                }
                ret.append(src, srcLen);
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

    ZFExportVarEnsureInit_ZFOutputForConsole();
    ZFLogOutputList.add(ZFOutputForConsole());
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFLogDataHolder)
{
    ZFLogOutputList.removeAll();
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
        zfText("%02d:%02d:%02d.%03d "),
        ti.hour,
        ti.minute,
        ti.second,
        ti.miliSecond);
    callerInfo.callerInfoT(ret);
    return ret;
}

ZFCoreArray<ZFOutput> ZFLogOutputList;

// ============================================================
static zfindex _ZFP_zfLogOnOutput(ZF_IN const void *src, ZF_IN zfindex size)
{
    if(size == zfindexMax())
    {
        size = zfslen((const zfchar *)src) * sizeof(zfchar);
    }
    for(zfindex i = 0; i < ZFLogOutputList.count(); ++i)
    {
        ZFLogOutputList[i].execute(src, size);
    }
    return size;
}
ZFOutput zfLogTrimT(void)
{
    ZFOutput ret;
    ret.callbackSerializeCustomDisable();
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
        zfText("%02d:%02d:%02d.%03d"),
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

ZF_NAMESPACE_GLOBAL_END

