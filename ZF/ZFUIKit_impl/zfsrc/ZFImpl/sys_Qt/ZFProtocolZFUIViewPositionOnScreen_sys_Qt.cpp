/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFImpl_sys_Qt_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIViewPositionOnScreen.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIView.h"

#if ZF_ENV_sys_Qt

#include <QWidget>

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIViewPositionOnScreenImpl_sys_Qt, ZFUIViewPositionOnScreen, ZFProtocolLevel::e_SystemHigh)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT(zfText("Qt:QWidget"))
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIView, zfText("Qt:QWidget"))
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()
public:
    virtual void viewPositionOnScreen(ZF_IN ZFUIView *view,
                                      ZF_OUT ZFUIRect &rect)
    {
        QWidget *nativeView = ZFCastStatic(QWidget *, view->nativeView());

        QPoint nativePos = nativeView->mapToGlobal(QPoint(0, 0));
        rect.point.x = nativePos.x();
        rect.point.y = nativePos.y();
        rect.size = view->layoutedFrame().size;
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIViewPositionOnScreenImpl_sys_Qt)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFUIViewPositionOnScreenImpl_sys_Qt)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_Qt

