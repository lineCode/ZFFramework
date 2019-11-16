#include "ZFUIPageManagerBasic.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFUIPageManagerBasic)

#define _ZFP_ZFUIPageManagerBasic_window "_ZFP_ZFUIPageManagerBasic_window"
static ZFLISTENER_PROTOTYPE_EXPAND(_ZFP_ZFUIPageManagerBasic_managerResume)
{
    ZFUIPageManager *pm = userData->objectHolded();
    if(!pm->managerResumed())
    {
        if(!pm->managerCreated())
        {
            pm->embededCreate();
        }
        pm->embededResume();
    }
}
static ZFLISTENER_PROTOTYPE_EXPAND(_ZFP_ZFUIPageManagerBasic_managerPause)
{
    ZFUIPageManager *pm = userData->objectHolded();
    if(pm->managerResumed())
    {
        pm->embededPause();
    }
}
static ZFLISTENER_PROTOTYPE_EXPAND(_ZFP_ZFUIPageManagerBasic_managerDestroy)
{
    ZFUIPageManager *pm = userData->objectHolded();
    pm->toObject()->tagRemove(_ZFP_ZFUIPageManagerBasic_window);
    if(pm->managerCreated())
    {
        if(pm->managerResumed())
        {
            pm->embededPause();
        }
        pm->embededDestroy();
    }
}
static ZFLISTENER_PROTOTYPE_EXPAND(_ZFP_ZFUIPageManagerBasic_managerOnDestroy)
{
    ZFUIPageManagerBasic *pm = listenerData.sender->toAny();
    ZFObjectHolder *windowHolder = pm->tagGet<ZFObjectHolder *>(_ZFP_ZFUIPageManagerBasic_window);
    pm->managerContainer()->viewRemoveFromParent();
    if(windowHolder != zfnull)
    {
        windowHolder->objectHolded<ZFUIWindow *>()->windowHide();
    }
}
ZFMETHOD_DEFINE_0(ZFUIPageManagerBasic, zfautoObject, embededCreateWindow)
{
    zfblockedAlloc(ZFUIWindow, window);
    window->viewBackgroundColorSet(ZFUIColorWhite());

    window->childAdd(this->managerContainer(), ZFUISizeParamFillFill());
    this->tagSet(_ZFP_ZFUIPageManagerBasic_window, window->objectHolder());

    window->observerAdd(ZFUIWindow::EventWindowOnShow(),
        ZFCallbackForFunc(_ZFP_ZFUIPageManagerBasic_managerResume),
        this->objectHolder(),
        this);
    window->observerAdd(ZFUIWindow::EventWindowOwnerSysWindowOnResume(),
        ZFCallbackForFunc(_ZFP_ZFUIPageManagerBasic_managerResume),
        this->objectHolder(),
        this);

    window->observerAdd(ZFUIWindow::EventWindowOnHide(),
        ZFCallbackForFunc(_ZFP_ZFUIPageManagerBasic_managerPause),
        this->objectHolder(),
        this);
    window->observerAdd(ZFUIWindow::EventWindowOwnerSysWindowOnPause(),
        ZFCallbackForFunc(_ZFP_ZFUIPageManagerBasic_managerPause),
        this->objectHolder(),
        this);

    window->observerAdd(ZFObject::EventObjectBeforeDealloc(),
        ZFCallbackForFunc(_ZFP_ZFUIPageManagerBasic_managerDestroy),
        this->objectHolder(),
        this);

    this->observerAdd(ZFUIPageManager::EventManagerOnDestroy(),
        ZFCallbackForFunc(_ZFP_ZFUIPageManagerBasic_managerOnDestroy),
        zfnull,
        zfnull,
        zfHint("autoRemoveAfterActivate")zftrue);

    return window;
}

ZF_NAMESPACE_GLOBAL_END

