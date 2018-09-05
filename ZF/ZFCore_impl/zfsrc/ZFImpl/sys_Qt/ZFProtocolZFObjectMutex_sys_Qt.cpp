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
#include "ZFCore/protocol/ZFProtocolZFObjectMutex.h"

#if ZF_ENV_sys_Qt
#include <QMutex>

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassNotPOD _ZFP_ZFObjectMutexImpl_sys_Qt
{
public:
    static void *implInit(void)
    {
        return zfnew(QMutex, QMutex::Recursive);
    }
    static void implDealloc(ZF_IN void *implObject)
    {
        QMutex *mutex = (QMutex *)implObject;
        zfdelete(mutex);
    }
    static void implLock(ZF_IN void *implObject)
    {
        QMutex *mutex = (QMutex *)implObject;
        mutex->lock();
    }
    static void implUnlock(ZF_IN void *implObject)
    {
        QMutex *mutex = (QMutex *)implObject;
        mutex->unlock();
    }
    static zfbool implTryLock(ZF_IN void *implObject)
    {
        QMutex *mutex = (QMutex *)implObject;
        return mutex->tryLock();
    }
};
ZFOBJECT_MUTEX_IMPL_DEFINE(ZFObjectMutexImpl_sys_Qt, ZFProtocolLevel::e_SystemHigh, {
        ZFObjectMutexImplSet(
                _ZFP_ZFObjectMutexImpl_sys_Qt::implInit,
                _ZFP_ZFObjectMutexImpl_sys_Qt::implDealloc,
                _ZFP_ZFObjectMutexImpl_sys_Qt::implLock,
                _ZFP_ZFObjectMutexImpl_sys_Qt::implUnlock,
                _ZFP_ZFObjectMutexImpl_sys_Qt::implTryLock
            );
    })

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_Qt

