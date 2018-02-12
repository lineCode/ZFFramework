/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFSynchronize.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFObject *_ZFP_zfsynchronizeObject = zfnull;
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(zfsynchronizeDataHolder, ZFLevelZFFrameworkEssential)
{
    _ZFP_zfsynchronizeObject = zfAlloc(ZFObject);
}
ZF_GLOBAL_INITIALIZER_DESTROY(zfsynchronizeDataHolder)
{
    zfRelease(_ZFP_zfsynchronizeObject);
    _ZFP_zfsynchronizeObject = zfnull;
}
ZF_GLOBAL_INITIALIZER_END(zfsynchronizeDataHolder)

// ============================================================
#if _ZFP_ZFSYNCHRONIZE_LOG_ENABLE
static void _ZFP_ZFSynchronizeLogAction(ZF_IN const ZFCallerInfo &callerInfo,
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

_ZFP_ZFSynchronizedObjectContainerWithLog::_ZFP_ZFSynchronizedObjectContainerWithLog(ZF_IN ZFObject *obj,
                                                                                     ZF_IN const ZFCallerInfo &callerInfo)
: m_obj(obj)
, callerInfo(callerInfo)
{
    if(obj == zfnull)
    {
        zfCoreCriticalErrorPrepare();
        _ZFP_ZFSynchronizeLogAction(callerInfo,
            zfTextA("try synchronize a null object"));
        zfCoreCriticalError();
        return ;
    }
    zfRetain(m_obj);
    _ZFP_ZFSynchronizeLogAction(callerInfo,
        zfTextA("lock object %s"),
        zfsCoreZ2A(m_obj->objectInfoOfInstance().cString()));
    m_obj->_ZFP_ZFObjectLock();
    _ZFP_ZFSynchronizeLogAction(callerInfo,
        zfTextA("lock object %s success"),
        zfsCoreZ2A(m_obj->objectInfoOfInstance().cString()));
}
_ZFP_ZFSynchronizedObjectContainerWithLog::~_ZFP_ZFSynchronizedObjectContainerWithLog(void)
{
    _ZFP_ZFSynchronizeLogAction(callerInfo,
        zfTextA("unlock object %s"),
        zfsCoreZ2A(m_obj->objectInfoOfInstance().cString()));
    m_obj->_ZFP_ZFObjectUnlock();
    _ZFP_ZFSynchronizeLogAction(callerInfo,
        zfTextA("unlock object %s success"),
        zfsCoreZ2A(m_obj->objectInfoOfInstance().cString()));
    zfRelease(m_obj);
}

// ============================================================
void _ZFP_ZFSynchronizedObjectLockWithLog(ZF_IN ZFObject *obj,
                                          ZF_IN const ZFCallerInfo &callerInfo)
{
    if(obj == zfnull)
    {
        zfCoreCriticalErrorPrepare();
        _ZFP_ZFSynchronizeLogAction(callerInfo,
            zfTextA("try lock a null object"));
        zfCoreCriticalError();
        return ;
    }

    zfRetain(obj);
    _ZFP_ZFSynchronizeLogAction(callerInfo,
        zfTextA("lock object %s"),
        zfsCoreZ2A(obj->objectInfoOfInstance().cString()));
    obj->_ZFP_ZFObjectLock();
    _ZFP_ZFSynchronizeLogAction(callerInfo,
        zfTextA("lock object %s success"),
        callerInfo.callerInfoA().cString(),
        zfsCoreZ2A(obj->objectInfoOfInstance().cString()));
}
void _ZFP_ZFSynchronizedObjectUnlockWithLog(ZF_IN ZFObject *obj,
                                            ZF_IN const ZFCallerInfo &callerInfo)
{
    if(obj == zfnull)
    {
        zfCoreCriticalErrorPrepare();
        _ZFP_ZFSynchronizeLogAction(callerInfo,
            zfTextA("try unlock a null object"));
        zfCoreCriticalError();
        return ;
    }

    _ZFP_ZFSynchronizeLogAction(callerInfo,
        zfTextA("unlock object %s"),
        zfsCoreZ2A(obj->objectInfoOfInstance().cString()));
    obj->_ZFP_ZFObjectUnlock();
    _ZFP_ZFSynchronizeLogAction(callerInfo,
        zfTextA("unlock object %s success"),
        zfsCoreZ2A(obj->objectInfoOfInstance().cString()));
    zfRelease(obj);
}

// ============================================================
_ZFP_ZFSynchronizeContainerWithLog::_ZFP_ZFSynchronizeContainerWithLog(ZF_IN const ZFCallerInfo &callerInfo)
: locked(zffalse)
, callerInfo()
{
    if(_ZFP_zfsynchronizeObject)
    {
        zfRetain(_ZFP_zfsynchronizeObject);
        _ZFP_ZFSynchronizeLogAction(callerInfo,
            zfTextA("lock global sync object"));
        _ZFP_zfsynchronizeObject->_ZFP_ZFObjectLock();
        _ZFP_ZFSynchronizeLogAction(callerInfo,
            zfTextA("lock global sync object success"));
        this->locked = zftrue;
    }
}
_ZFP_ZFSynchronizeContainerWithLog::~_ZFP_ZFSynchronizeContainerWithLog(void)
{
    if(this->locked && _ZFP_zfsynchronizeObject)
    {
        _ZFP_ZFSynchronizeLogAction(callerInfo,
            zfTextA("unlock global sync object"));
        _ZFP_zfsynchronizeObject->_ZFP_ZFObjectUnlock();
        _ZFP_ZFSynchronizeLogAction(callerInfo,
            zfTextA("unlock global sync object success"));
        zfRelease(_ZFP_zfsynchronizeObject);
    }
}

// ============================================================
void _ZFP_ZFSynchronizeLockWithLog(ZF_IN const ZFCallerInfo &callerInfo)
{
    if(_ZFP_zfsynchronizeObject)
    {
        zfRetain(_ZFP_zfsynchronizeObject);
        _ZFP_ZFSynchronizeLogAction(callerInfo,
            zfTextA("lock global sync object"));
        _ZFP_zfsynchronizeObject->_ZFP_ZFObjectLock();
        _ZFP_ZFSynchronizeLogAction(callerInfo,
            zfTextA("lock global sync object success"));
    }
}
void _ZFP_ZFSynchronizeUnlockWithLog(ZF_IN const ZFCallerInfo &callerInfo)
{
    if(_ZFP_zfsynchronizeObject)
    {
        _ZFP_ZFSynchronizeLogAction(callerInfo,
            zfTextA("unlock global sync object"));
        _ZFP_zfsynchronizeObject->_ZFP_ZFObjectUnlock();
        _ZFP_ZFSynchronizeLogAction(callerInfo,
            zfTextA("unlock global sync object success"));
        zfRelease(_ZFP_zfsynchronizeObject);
    }
}
#endif // #if _ZFP_ZFSYNCHRONIZE_LOG_ENABLE

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_0(void, zfsynchronizeLock)
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_0(void, zfsynchronizeUnlock)
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(void, zfsynchronizedObjectLock, ZFMP_IN(ZFObject *, obj))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(void, zfsynchronizedObjectUnlock, ZFMP_IN(ZFObject *, obj))

ZF_NAMESPACE_GLOBAL_END
#endif

