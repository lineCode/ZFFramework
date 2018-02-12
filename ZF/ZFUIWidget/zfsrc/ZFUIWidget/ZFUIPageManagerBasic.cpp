/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFUIPageManagerBasic.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFUIPageManagerBasic)

#define _ZFP_ZFUIPageManagerBasic_window zfText("_ZFP_ZFUIPageManagerBasic_window")
static ZFLISTENER_PROTOTYPE_EXPAND(_ZFP_ZFUIPageManagerBasic_managerResume)
{
    ZFUIPageManager *pm = userData->to<ZFObjectHolder *>()->holdedObj;
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
    ZFUIPageManager *pm = userData->to<ZFObjectHolder *>()->holdedObj;
    if(pm->managerResumed())
    {
        pm->embededPause();
    }
}
static ZFLISTENER_PROTOTYPE_EXPAND(_ZFP_ZFUIPageManagerBasic_managerDestroy)
{
    ZFUIPageManager *pm = userData->to<ZFObjectHolder *>()->holdedObj;
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
        windowHolder->holdedObj.to<ZFUIWindow *>()->windowHide();
    }
}
ZFMETHOD_DEFINE_0(ZFUIPageManagerBasic, zfautoObject, embededCreateWindow)
{
    zfblockedAlloc(ZFUIWindow, window);
    window->viewBackgroundColorSet(ZFUIColorWhite());

    window->childAdd(this->managerContainer(), ZFUISizeParamFillFill());
    this->tagSet(_ZFP_ZFUIPageManagerBasic_window, window->objectHolder());

    window->observerAdd(ZFUIWindow::EventWindowOnShow(),
        ZFCallbackForRawFunction(_ZFP_ZFUIPageManagerBasic_managerResume),
        this->objectHolder(),
        this);
    window->observerAdd(ZFUIWindow::EventWindowOwnerSysWindowOnResume(),
        ZFCallbackForRawFunction(_ZFP_ZFUIPageManagerBasic_managerResume),
        this->objectHolder(),
        this);

    window->observerAdd(ZFUIWindow::EventWindowOnHide(),
        ZFCallbackForRawFunction(_ZFP_ZFUIPageManagerBasic_managerPause),
        this->objectHolder(),
        this);
    window->observerAdd(ZFUIWindow::EventWindowOwnerSysWindowOnPause(),
        ZFCallbackForRawFunction(_ZFP_ZFUIPageManagerBasic_managerPause),
        this->objectHolder(),
        this);

    window->observerAdd(ZFObject::EventObjectBeforeDealloc(),
        ZFCallbackForRawFunction(_ZFP_ZFUIPageManagerBasic_managerDestroy),
        this->objectHolder(),
        this);

    this->observerAdd(ZFUIPageManager::EventManagerOnDestroy(),
        ZFCallbackForRawFunction(_ZFP_ZFUIPageManagerBasic_managerOnDestroy),
        zfnull,
        zfnull,
        zfHint("autoRemoveAfterActivate")zftrue);

    return window;
}

ZF_NAMESPACE_GLOBAL_END

