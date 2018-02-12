/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#ifndef _ZFI_ZF2048AppAutoMoveRunner_h_
#define _ZFI_ZF2048AppAutoMoveRunner_h_

#include "../ZF2048Game.h"
#include "ZF2048AppAutoMoveAction.h"
ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd _ZFP_ZF2048AppAutoMoveRunnerPrivate;
zfclass ZF2048AppAutoMoveRunner : zfextends ZFObject
{
    ZFOBJECT_DECLARE(ZF2048AppAutoMoveRunner, ZFObject)

    ZFOBSERVER_EVENT(RunnerOnStop)

public:
    ZF2048Game *game;
    ZFCoreArrayPOD<ZF2048AppAutoMoveActionEnum> actionList;

public:
    virtual void runnerStart(void);
    virtual void runnerStop(void);
    virtual zfbool runningStarted(void);
    virtual zfbool runnerUnableToMove(void);

protected:
    virtual void runnerOnStop(void)
    {
        this->observerNotify(zfself::EventRunnerOnStop());
    }

protected:
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);

private:
    _ZFP_ZF2048AppAutoMoveRunnerPrivate *d;
    friend zfclassFwd _ZFP_ZF2048AppAutoMoveRunnerPrivate;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZF2048AppAutoMoveRunner_h_

