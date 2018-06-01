/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFImpl_sys_iOS_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIViewPositionOnScreen.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIView.h"

#if ZF_ENV_sys_iOS

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIViewPositionOnScreenImpl_sys_iOS, ZFUIViewPositionOnScreen, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT(zfText("iOS:UIView"))
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIView, zfText("iOS:UIView"))
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()
public:
    virtual void viewPositionOnScreen(ZF_IN ZFUIView *view,
                                      ZF_OUT ZFUIRect &rect)
    {
        UIView *nativeView = (__bridge UIView *)view->nativeView();

        CGRect nativeRect = CGRectZero;
        if(nativeView.window == nil)
        {
            nativeRect = nativeView.frame;
        }
        else
        {
            nativeRect = [nativeView convertRect:nativeView.bounds toView:nil];
        }
        ZFImpl_sys_iOS_ZFUIKit_impl_ZFUIRectFromCGRectT(rect, nativeRect);
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIViewPositionOnScreenImpl_sys_iOS)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFUIViewPositionOnScreenImpl_sys_iOS)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_iOS

