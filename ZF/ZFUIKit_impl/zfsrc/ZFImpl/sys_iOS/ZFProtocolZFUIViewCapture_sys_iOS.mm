/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFImpl_sys_iOS_ZFUIKit.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIViewCapture.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIView.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIImage.h"

#if ZF_ENV_sys_iOS

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIViewCaptureImpl_sys_iOS, ZFUIViewCapture, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT(zfText("iOS:UIView"))
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIView, zfText("iOS:UIView"))
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIImage, zfText("iOS:UIImage"))
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()
public:
    virtual zfbool viewCapture(ZF_IN ZFUIView *view,
                               ZF_IN_OUT ZFUIImage *image)
    {
        UIView *nativeView = (__bridge UIView *)view->nativeView();

        UIGraphicsBeginImageContext(nativeView.frame.size);
        CGContextRef context = UIGraphicsGetCurrentContext();
        [nativeView.layer renderInContext:context];
        UIImage *nativeImage = UIGraphicsGetImageFromCurrentImageContext();
        UIGraphicsEndImageContext();

        image->nativeImageSet((__bridge void *)nativeImage);

        return (nativeImage != nil);
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIViewCaptureImpl_sys_iOS)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFUIViewCaptureImpl_sys_iOS)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_iOS

