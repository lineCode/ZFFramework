/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFUIOnScreenKeyboardState.h"
#include "protocol/ZFProtocolZFUIOnScreenKeyboardState.h"
#include "ZFUIWindow.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFUIOnScreenKeyboardState)

ZFOBSERVER_EVENT_REGISTER(ZFUIOnScreenKeyboardState, KeyboardStateOnChange)

ZFMETHOD_DEFINE_1(ZFUIOnScreenKeyboardState, ZFUIOnScreenKeyboardState *, instanceForSysWindow,
                  ZFMP_IN_OPT(ZFUISysWindow *, sysWindow, zfnull))
{
    if(sysWindow == zfnull)
    {
        sysWindow = ZFUISysWindow::mainWindow();
    }
    ZFUIOnScreenKeyboardState *ret = sysWindow->tagGet<ZFUIOnScreenKeyboardState *>("_ZFP_ZFUIOnScreenKeyboardState");
    if(ret == zfnull)
    {
        zfblockedAlloc(ZFUIOnScreenKeyboardState, tmp);
        sysWindow->tagSet("_ZFP_ZFUIOnScreenKeyboardState", tmp);
        tmp->_ZFP_ZFUIOnScreenKeyboardState_keyboardOwnerSysWindow = sysWindow;
        ret = tmp;
    }
    return ret;
}
ZFMETHOD_DEFINE_1(ZFUIOnScreenKeyboardState, ZFUIOnScreenKeyboardState *, instanceForView,
                  ZFMP_IN_OPT(ZFUIView *, view, zfnull))
{
    return ZFUIOnScreenKeyboardState::instanceForSysWindow(ZFUIWindow::sysWindowForView(view));
}

ZFMETHOD_DEFINE_0(ZFUIOnScreenKeyboardState, ZFUISysWindow *, keyboardOwnerSysWindow)
{
    return _ZFP_ZFUIOnScreenKeyboardState_keyboardOwnerSysWindow;
}

ZFMETHOD_DEFINE_0(ZFUIOnScreenKeyboardState, zfbool, keyboardShowing)
{
    return this->_ZFP_ZFUIOnScreenKeyboardState_keyboardShowing;
}
ZFMETHOD_DEFINE_0(ZFUIOnScreenKeyboardState, const ZFUIRect &, keyboardFrame)
{
    return this->_ZFP_ZFUIOnScreenKeyboardState_keyboardFrame;
}
ZFMETHOD_DEFINE_0(ZFUIOnScreenKeyboardState, const ZFUIRect &, keyboardFramePrev)
{
    return this->_ZFP_ZFUIOnScreenKeyboardState_keyboardFramePrev;
}

ZFMETHOD_DEFINE_1(ZFUIOnScreenKeyboardState, void, keyboardFixClientFrameT,
                  ZFMP_OUT(ZFUIRect &, clientFrame))
{
    zffloat scale = this->keyboardOwnerSysWindow()->rootView()->scaleFixed();
    ZFPROTOCOL_ACCESS(ZFUIOnScreenKeyboardState)->keyboardFixClientFrame(
        this,
        ZFUIRectApplyScale(this->keyboardFrame(), scale),
        clientFrame);
    ZFUIRectApplyScaleReversely(clientFrame, clientFrame, scale);
}

void ZFUIOnScreenKeyboardState::objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
{
    zfsFromPointerT(ret, this->keyboardOwnerSysWindow());
    if(this->keyboardShowing())
    {
        ret += " keyboardFrame: ";
        ZFUIRectToString(ret, this->keyboardFrame());
    }
    else
    {
        ret += " keyboardNotShow";
    }
}

ZF_NAMESPACE_GLOBAL_END

