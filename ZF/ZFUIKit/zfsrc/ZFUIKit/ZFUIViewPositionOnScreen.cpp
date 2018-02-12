/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFUIViewPositionOnScreen.h"
#include "protocol/ZFProtocolZFUIViewPositionOnScreen.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_DEFINE_2(void, ZFUIViewPositionOnScreen,
                       ZFMP_OUT(ZFUIRect &, rect),
                       ZFMP_IN(ZFUIView *, view))
{
    ZFPROTOCOL_ACCESS(ZFUIViewPositionOnScreen)->viewPositionOnScreen(view, rect);
    rect = ZFUIRectApplyScaleReversely(rect, view->scaleFixed());
}
ZFMETHOD_FUNC_DEFINE_INLINE_1(ZFUIRect, ZFUIViewPositionOnScreen,
                              ZFMP_IN(ZFUIView *, view))

ZF_NAMESPACE_GLOBAL_END

