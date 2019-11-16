#include "ZFUIKit_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_GLOBAL_INITIALIZER_INIT(ZFUIOnScreenKeyboardState_test)
{
    this->observerOwner = zflineAlloc(ZFArrayEditable);

    ZFLISTENER_LOCAL(sysWindowOnCreate, {
        ZFLISTENER_LOCAL(action, {
            zfLogTrimT() << "[ZFUIOnScreenKeyboardState] state changed:" << listenerData.sender;
        })
        ZFUIOnScreenKeyboardState *state = ZFUIOnScreenKeyboardState::instanceForSysWindow(listenerData.sender->toAny());
        state->observerAdd(ZFObserverAddParam()
                .eventIdSet(ZFUIOnScreenKeyboardState::EventKeyboardStateOnChange())
                .observerSet(action)
                .ownerSet(userData)
            );
        userData->to<ZFArrayEditable *>()->add(state->objectHolder());
    })
    ZFObjectGlobalEventObserver().observerAdd(ZFObserverAddParam()
            .eventIdSet(ZFUISysWindow::EventSysWindowOnCreate())
            .observerSet(sysWindowOnCreate)
            .ownerSet(this->observerOwner)
            .userDataSet(this->observerOwner)
        );

    ZFLISTENER_LOCAL(sysWindowOnDestroy, {
        ZFUIOnScreenKeyboardState *state = ZFUIOnScreenKeyboardState::instanceForSysWindow(listenerData.sender->toAny());
        state->observerRemoveByOwner(userData);
        userData->to<ZFArrayEditable *>()->removeElement(state->objectHolder());
    })
    ZFObjectGlobalEventObserver().observerAdd(ZFObserverAddParam()
            .eventIdSet(ZFUISysWindow::EventSysWindowOnDestroy())
            .observerSet(sysWindowOnDestroy)
            .ownerSet(this->observerOwner)
            .userDataSet(this->observerOwner)
        );
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIOnScreenKeyboardState_test)
{
    ZFObjectGlobalEventObserver().observerRemoveByOwner(this->observerOwner);
    ZFArray *attached = this->observerOwner;
    for(zfindex i = 0; i < attached->count(); ++i)
    {
        attached->get(i)->objectHolded()->observerRemoveByOwner(this->observerOwner);
    }
}
zfautoObject observerOwner;
ZF_GLOBAL_INITIALIZER_END(ZFUIOnScreenKeyboardState_test)

ZF_NAMESPACE_GLOBAL_END

