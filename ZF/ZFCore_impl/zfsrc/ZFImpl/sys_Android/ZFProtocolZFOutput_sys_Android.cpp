/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFImpl_sys_Android_ZFCore.h"
#include "ZFCore/protocol/ZFProtocolZFOutput.h"
#include "ZFCore/ZFString.h"

#if ZF_ENV_sys_Android
ZF_NAMESPACE_GLOBAL_BEGIN

#define _ZFP_ZFImpl_sys_Android_ZFOutput_output(fmt, ...) \
    AndroidLogDetail(AndroidLogLevelA, AndroidLogTagDefault, fmt, ##__VA_ARGS__)

zfclass _ZFP_I_ZFOutputImpl_sys_Android_SyncObject : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_I_ZFOutputImpl_sys_Android_SyncObject, ZFObject)
};

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFOutputImpl_sys_Android, ZFOutput, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT(zfText("Android:Logcat"))
public:
    zfoverride
    virtual void protocolOnInitFinish(void)
    {
        zfsuper::protocolOnInitFinish();
        this->syncObj = zfAlloc(_ZFP_I_ZFOutputImpl_sys_Android_SyncObject);
    }
    zfoverride
    virtual void protocolOnDeallocPrepare(void)
    {
        if(this->savedString.length() > 0)
        {
            _ZFP_ZFImpl_sys_Android_ZFOutput_output(zfTextA("%s"), this->savedString.cString());
        }
        zfRelease(this->syncObj);
        zfsuper::protocolOnDeallocPrepare();
    }

public:
    virtual void outputCoreLog(ZF_IN const zfcharA *s)
    {
        zfstringA tmp = s;
        this->checkOutput(tmp);
    }
    virtual void outputLog(ZF_IN const zfchar *s, ZF_IN_OPT zfindex count = zfindexMax())
    {
        zfsynchronizedObject(this->syncObj);
        if(count == zfindexMax())
        {
            this->savedString += ZFStringZ2A(s);
        }
        else
        {
            this->savedString += ZFStringZ2A(zfstring(s, count));
        }
        this->checkOutput(this->savedString);
    }
private:
    ZFObject *syncObj;
    zfstringA savedString;
    void checkOutput(ZF_IN_OUT zfstringA &s)
    {
        if(s.length() > 0)
        {
            if(s[s.length() - 1] == '\n')
            {
                if(s.length() == 1)
                {
                    _ZFP_ZFImpl_sys_Android_ZFOutput_output(zfTextA(" "));
                }
                else
                {
                    s.remove(s.length() - 1);
                    _ZFP_ZFImpl_sys_Android_ZFOutput_output(zfTextA("%s"), s.cString());
                }
                s.removeAll();
            }
            else
            {
                zfindex index = zfstringFindReversely(s, '\n');
                if(index != zfindexMax())
                {
                    if(index == 0)
                    {
                        _ZFP_ZFImpl_sys_Android_ZFOutput_output(zfTextA(" "));
                    }
                    else
                    {
                        s[index] = '\0';
                        _ZFP_ZFImpl_sys_Android_ZFOutput_output(zfTextA("%s"), s.cString());
                    }
                    s.remove(0, index + 1);
                }
            }
        }
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFOutputImpl_sys_Android)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFOutputImpl_sys_Android)

ZF_NAMESPACE_GLOBAL_END

#endif

