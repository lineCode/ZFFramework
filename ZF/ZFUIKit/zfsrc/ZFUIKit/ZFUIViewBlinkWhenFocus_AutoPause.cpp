/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFUIViewBlinkWhenFocus.h"

#include "ZFUIWindow.h"
#include "ZFUISysWindow.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIViewBlinkWhenFocus_AutoPause_DataHolder, ZFLevelZFFrameworkLow)
{
    this->doActionListener = ZFCallbackForRawFunction(zfself::doAction);
    ZFObjectGlobalEventObserver().observerAdd(ZFUIWindow::EventWindowOnShow(), this->doActionListener);
    ZFObjectGlobalEventObserver().observerAdd(ZFUIWindow::EventWindowOnHide(), this->doActionListener);
    ZFObjectGlobalEventObserver().observerAdd(ZFUISysWindow::EventSysWindowOnResume(), this->doActionListener);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIViewBlinkWhenFocus_AutoPause_DataHolder)
{
    ZFObjectGlobalEventObserver().observerRemove(ZFUIWindow::EventWindowOnShow(), this->doActionListener);
    ZFObjectGlobalEventObserver().observerRemove(ZFUIWindow::EventWindowOnHide(), this->doActionListener);
    ZFObjectGlobalEventObserver().observerRemove(ZFUISysWindow::EventSysWindowOnResume(), this->doActionListener);
}
public:
    ZFListener doActionListener;
    static ZFLISTENER_PROTOTYPE_EXPAND(doAction)
    {
        ZFUIViewBlinkWhenFocusAutoApplyPauseForTime();
    }
ZF_GLOBAL_INITIALIZER_END(ZFUIViewBlinkWhenFocus_AutoPause_DataHolder)

ZF_NAMESPACE_GLOBAL_END

