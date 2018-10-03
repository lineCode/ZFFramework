/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFImpl_default_ZFCore_impl.h"
#include "ZFCore/protocol/ZFProtocolZFTimer.h"
#include "ZFCore/ZFThread.h"
#include "ZFCore/ZFValue.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass _ZFP_ZFTimerImpl_default_Timer : zfextends ZFObject
{
    ZFOBJECT_DECLARE_WITH_CUSTOM_CTOR(_ZFP_ZFTimerImpl_default_Timer, ZFObject)

public:
    ZFPROTOCOL_INTERFACE_CLASS(ZFTimer) *impl;
    ZFTimer *timer;
private:
    zfbool timerThreadStarted;
    zfidentity threadCallbackTaskId;
    zfidentity threadCallbackId;
    zfidentity mainThreadCallbackTaskId;
    zfidentity mainThreadCallbackId;
protected:
    _ZFP_ZFTimerImpl_default_Timer(void)
    : impl(zfnull)
    , timer(zfnull)
    , timerThreadStarted(zffalse)
    , threadCallbackTaskId(zfidentityInvalid())
    , threadCallbackId(zfidentityInvalid())
    , mainThreadCallbackTaskId(zfidentityInvalid())
    , mainThreadCallbackId(zfidentityInvalid())
    {
    }

public:
    zffinal void timerStart(void)
    {
        ++(this->threadCallbackTaskId);
        this->threadCallbackId = ZFThreadExecuteInNewThread(
            ZFCallbackForMemberMethod(this, ZFMethodAccess(zfself, threadCallback)),
            this /* pass this as dummy param to keep retain count */,
            ZFListenerData().param0Set(ZFValue::identityValueCreate(this->threadCallbackTaskId))
            );
    }
    zffinal void timerStop(void)
    {
        ++(this->threadCallbackTaskId);
        ++(this->mainThreadCallbackTaskId);
        ZFThreadExecuteCancel(this->threadCallbackId);
        ZFThreadExecuteCancel(this->mainThreadCallbackId);
        if(this->timerThreadStarted)
        {
            this->timerThreadStarted = zffalse;
            this->impl->notifyTimerStop(this->timer);
        }
    }

public:
    ZFLISTENER_INLINE(threadCallback)
    {
        zfidentity curId = ZFCastZFObjectUnchecked(ZFValue *, listenerData.param0)->identityValue();

        // delay
        if(curId != this->threadCallbackTaskId) {return ;}
        if(this->timer->timerDelay() > 0)
        {
            ZFThread::sleep(this->timer->timerDelay());
        }

        // start
        if(curId != this->threadCallbackTaskId) {return ;}
        this->timerThreadStarted = zftrue;
        this->impl->notifyTimerStart(this->timer);
        if(curId != this->threadCallbackTaskId) {return ;}

        // timer
        while(curId == this->threadCallbackTaskId)
        {
            if(this->timer->timerActivateInMainThread())
            {
                ++(this->mainThreadCallbackTaskId);
                this->mainThreadCallbackId = ZFThreadExecuteInNewThread(
                    ZFCallbackForMemberMethod(this, ZFMethodAccess(zfself, mainThreadCallback)),
                    zfnull,
                    ZFListenerData().param0Set(ZFValue::identityValueCreate(this->mainThreadCallbackTaskId).toObject())
                    );
            }
            else
            {
                this->impl->notifyTimerActivate(this->timer);
            }
            ZFThread::sleep(this->timer->timerInterval());
        }
    }
    ZFLISTENER_INLINE(mainThreadCallback)
    {
        zfidentity curId = ZFCastZFObjectUnchecked(ZFValue *, listenerData.param0)->identityValue();
        if(curId != this->mainThreadCallbackTaskId) {return ;}
        this->impl->notifyTimerActivate(this->timer);
    }
};

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFTimerImpl_default, ZFTimer, ZFProtocolLevel::e_Default)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("ZFFramework:ZFThread")
public:
    virtual void *nativeTimerCreate(ZF_IN ZFTimer *timer)
    {
        _ZFP_ZFTimerImpl_default_Timer *token = zfAlloc(_ZFP_ZFTimerImpl_default_Timer);
        token->impl = this;
        token->timer = timer;
        return token;
    }
    virtual void nativeTimerDestroy(ZF_IN ZFTimer *timer,
                                    ZF_IN void *nativeTimer)
    {
        _ZFP_ZFTimerImpl_default_Timer *token = ZFCastStatic(_ZFP_ZFTimerImpl_default_Timer *, nativeTimer);
        zfRelease(token);
    }
    virtual void timerStart(ZF_IN ZFTimer *timer)
    {
        _ZFP_ZFTimerImpl_default_Timer *token = ZFCastStatic(_ZFP_ZFTimerImpl_default_Timer *, timer->nativeTimer());
        token->impl = this;
        token->timer = timer;
        token->timerStart();
    }
    virtual void timerStop(ZF_IN ZFTimer *timer)
    {
        _ZFP_ZFTimerImpl_default_Timer *token = ZFCastStatic(_ZFP_ZFTimerImpl_default_Timer *, timer->nativeTimer());
        token->timerStop();
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFTimerImpl_default)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFTimerImpl_default)

ZF_NAMESPACE_GLOBAL_END

