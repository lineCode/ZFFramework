/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "zfsynchronize.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
#if _ZFP_ZFSYNCHRONIZE_LOG_ENABLE
static void _ZFP_zfsynchronizeLog(ZF_IN const ZFCallerInfo &callerInfo,
                                  ZF_IN const zfcharA *fmt,
                                  ...)
{
    va_list vaList;
    va_start(vaList, fmt);
    zfstringA text;
    zfstringAppendV(text, fmt, vaList);
    va_end(vaList);
    printf("%s %s\n", callerInfo.callerInfoA().cString(), text.cString());
}

_ZFP_zfsynchronizeContainerWithLog::_ZFP_zfsynchronizeContainerWithLog(ZF_IN ZFObject *obj,
                                                                       ZF_IN const ZFCallerInfo &callerInfo)
: m_obj(obj)
, callerInfo(callerInfo)
{
    if(obj == zfnull)
    {
        zfCoreCriticalErrorPrepare();
        _ZFP_zfsynchronizeLog(callerInfo,
            zfTextA("try synchronize a null object"));
        zfCoreCriticalError();
        return ;
    }
    _ZFP_zfsynchronizeLog(callerInfo,
        zfTextA("lock object %s"),
        zfsCoreZ2A(m_obj->objectInfoOfInstance().cString()));
    m_obj->_ZFP_ZFObjectLock();
    _ZFP_zfsynchronizeLog(callerInfo,
        zfTextA("lock object %s success"),
        zfsCoreZ2A(m_obj->objectInfoOfInstance().cString()));
}
_ZFP_zfsynchronizeContainerWithLog::~_ZFP_zfsynchronizeContainerWithLog(void)
{
    _ZFP_zfsynchronizeLog(callerInfo,
        zfTextA("unlock object %s"),
        zfsCoreZ2A(m_obj->objectInfoOfInstance().cString()));
    m_obj->_ZFP_ZFObjectUnlock();
    _ZFP_zfsynchronizeLog(callerInfo,
        zfTextA("unlock object %s success"),
        zfsCoreZ2A(m_obj->objectInfoOfInstance().cString()));
}

// ============================================================
void _ZFP_zfsynchronizeLockWithLog(ZF_IN ZFObject *obj,
                                   ZF_IN const ZFCallerInfo &callerInfo)
{
    if(obj == zfnull)
    {
        zfCoreCriticalErrorPrepare();
        _ZFP_zfsynchronizeLog(callerInfo,
            zfTextA("try lock a null object"));
        zfCoreCriticalError();
        return ;
    }

    _ZFP_zfsynchronizeLog(callerInfo,
        zfTextA("lock object %s"),
        zfsCoreZ2A(obj->objectInfoOfInstance().cString()));
    obj->_ZFP_ZFObjectLock();
    _ZFP_zfsynchronizeLog(callerInfo,
        zfTextA("lock object %s success"),
        callerInfo.callerInfoA().cString(),
        zfsCoreZ2A(obj->objectInfoOfInstance().cString()));
}
void _ZFP_zfsynchronizeUnlockWithLog(ZF_IN ZFObject *obj,
                                     ZF_IN const ZFCallerInfo &callerInfo)
{
    if(obj == zfnull)
    {
        zfCoreCriticalErrorPrepare();
        _ZFP_zfsynchronizeLog(callerInfo,
            zfTextA("try unlock a null object"));
        zfCoreCriticalError();
        return ;
    }

    _ZFP_zfsynchronizeLog(callerInfo,
        zfTextA("unlock object %s"),
        zfsCoreZ2A(obj->objectInfoOfInstance().cString()));
    obj->_ZFP_ZFObjectUnlock();
    _ZFP_zfsynchronizeLog(callerInfo,
        zfTextA("unlock object %s success"),
        zfsCoreZ2A(obj->objectInfoOfInstance().cString()));
}
#endif // #if _ZFP_ZFSYNCHRONIZE_LOG_ENABLE

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(void, zfsynchronizeLock, ZFMP_IN(ZFObject *, obj))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(void, zfsynchronizeUnlock, ZFMP_IN(ZFObject *, obj))

ZF_NAMESPACE_GLOBAL_END
#endif

