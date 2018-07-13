/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFImpl_sys_Qt_ZFCore_impl.h"
#include "ZFCore/protocol/ZFProtocolZFImplOutput.h"
#include "ZFCore/ZFString.h"

#if ZF_ENV_sys_Qt
#include <QDebug>
#include <QTextCodec>
#include <QMutex>

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFImplOutputImpl_sys_Qt, ZFImplOutput, ZFProtocolLevel::e_SystemHigh)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT(zfText("Qt:qDebug"))
public:
    zfoverride
    virtual void protocolOnDealloc(void)
    {
        QMutexLocker _logMutexLocker(&_logMutex);
        if(this->savedString.length() > 0)
        {
            qDebug() << this->savedString.cString();
        }
        zfsuper::protocolOnDealloc();
    }

public:
    virtual void outputCoreLog(ZF_IN const zfcharA *s)
    {
        zfstringA tmp = s;
        this->checkOutput(tmp);
    }
    virtual void outputLog(ZF_IN const zfchar *s, ZF_IN_OPT zfindex count = zfindexMax())
    {
        QMutexLocker _logMutexLocker(&_logMutex);
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
    zfstringA savedString;
    void checkOutput(ZF_IN_OUT zfstringA &s)
    {
        if(s.length() > 0)
        {
            if(s[s.length() - 1] == '\n')
            {
                s.remove(s.length() - 1);
                qDebug() << s.cString();
                s.removeAll();
            }
            else
            {
                zfindex index = zfstringFindReversely(s, '\n');
                if(index != zfindexMax())
                {
                    s[index] = '\0';
                    qDebug() << s.cString();
                    s.remove(0, index + 1);
                }
            }
        }
    }
private:
    QMutex _logMutex;
ZFPROTOCOL_IMPLEMENTATION_END(ZFImplOutputImpl_sys_Qt)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFImplOutputImpl_sys_Qt)

ZF_NAMESPACE_GLOBAL_END

#endif

