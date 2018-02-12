/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFUIKit_test.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIView.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFUIKit_nativeViewEmbed_test : zfextends ZFFramework_test_TestCase, zfimplements ZFUISysWindowEmbedImpl
{
    ZFOBJECT_DECLARE(ZFUIKit_nativeViewEmbed_test, ZFFramework_test_TestCase)
    ZFIMPLEMENTS_DECLARE(ZFUISysWindowEmbedImpl)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUIView)
        ZFUIWindow *window = zfnull;
        ZFUIView *container = zfnull;
        ZFUIKit_test_prepareTestWindow(window, container, this);

        // prepare native view
        // here we use a dummy normal view for short, plain native view is also supported
        zfblockedAlloc(ZFUIView, nativeViewHolder);
        this->_nativeViewWrapperHolder = nativeViewHolder;
        this->_nativeViewWrapper = nativeViewHolder;
        this->_nativeView = nativeViewHolder->nativeView();

        // attach native view to ZF (via ZFUINativeViewWrapper)
        zfblockedAlloc(ZFUINativeViewWrapper, wrapper);
        this->_wrapper = wrapper;
        wrapper->wrappedNativeViewSet(this->_nativeView);
        container->childAdd(wrapper);
        wrapper->viewBackgroundColorSet(ZFUIColorRandom());
        wrapper->layoutParam()->sizeParamSet(ZFUISizeParamFillFill());
        wrapper->layoutParam()->layoutMarginSet(ZFUIMarginMake(20));

        // attach ZF to native view (via #ZFUISysWindow)
        this->_sysWindowHolder = ZFUISysWindow::nativeWindowEmbed(this);
        this->_sysWindow = this->_sysWindowHolder;
        this->notifyOnCreate(this->_sysWindow, zfnull);
        this->notifyOnResume(this->_sysWindow);

        // ZFUIWindow can attach to custom ZFUISysWindow
        zfblockedAlloc(ZFUIWindow, embedWindow);
        embedWindow->windowOwnerSysWindowSet(this->_sysWindow);
        embedWindow->windowShow();
        embedWindow->windowLayoutParam()->layoutMarginSet(ZFUIMarginMake(20));
        embedWindow->viewBackgroundColorSet(ZFUIColorRandom());

        this->_wrapper->observerAdd(
            ZFUIView::EventViewLayoutOnLayoutPrepare(),
            ZFCallbackForMemberMethod(this, ZFMethodAccess(zfself, nativeViewOnLayout)));
    }

private:
    zfautoObject _nativeViewWrapperHolder;
    ZFUIView *_nativeViewWrapper;
    void *_nativeView;
    ZFUINativeViewWrapper *_wrapper;
    zfautoObject _sysWindowHolder;
    ZFUISysWindow *_sysWindow;

private:
    ZFLISTENER_DECLARE(nativeViewOnLayout)
    {
        ZFUIRect frame = this->notifyMeasureWindow(
            this->_sysWindow,
            ZFUIRectApplyScale(ZFUIRectGetBounds(this->_wrapper->layoutedFrame()), this->_sysWindow->rootView()->scaleFixed()),
            ZFUIMarginZero()
        );
        ZFPROTOCOL_ACCESS(ZFUIView)->viewFrameSet(this->_sysWindow->rootView(), frame);
    }

    // ============================================================
    // ZFUISysWindowEmbedImpl
public:
    zfoverride
    virtual void nativeWindowOnCleanup(ZF_IN ZFUISysWindow *sysWindow)
    {
        zfsuperI(ZFUISysWindowEmbedImpl)::nativeWindowOnCleanup(sysWindow);
    }

    zfoverride
    virtual void nativeWindowRootViewOnAdd(ZF_IN ZFUISysWindow *sysWindow,
                                           ZF_OUT void *&nativeParentView)
    {
        zfsuperI(ZFUISysWindowEmbedImpl)::nativeWindowRootViewOnAdd(sysWindow, nativeParentView);

        // here we use default impl for short
        ZFPROTOCOL_ACCESS(ZFUIView)->childAdd(
            this->_nativeViewWrapper,
            sysWindow->rootView(),
            0, ZFUIViewChildLayer::e_Normal, 0);

        nativeParentView = this->_nativeViewWrapper->nativeView();
    }
    zfoverride
    virtual void nativeWindowRootViewOnRemove(ZF_IN ZFUISysWindow *sysWindow)
    {
        zfsuperI(ZFUISysWindowEmbedImpl)::nativeWindowRootViewOnRemove(sysWindow);

        ZFPROTOCOL_ACCESS(ZFUIView)->childRemove(
            this->_nativeViewWrapper,
            sysWindow->rootView(),
            0, ZFUIViewChildLayer::e_Normal, 0);
    }

    zfoverride
    virtual ZFUISysWindow *modalWindowShow(ZF_IN ZFUISysWindow *sysWindowOwner)
    {
        zfCoreCriticalNotSupported();
        return zfnull;
    }
    zfoverride
    virtual void modalWindowFinish(ZF_IN ZFUISysWindow *sysWindowOwner,
                                   ZF_IN ZFUISysWindow *sysWindowToFinish)
    {
        zfCoreCriticalNotSupported();
    }

    zfoverride
    virtual void sysWindowLayoutParamOnInit(ZF_IN ZFUISysWindow *sysWindow)
    {
        zfsuperI(ZFUISysWindowEmbedImpl)::sysWindowLayoutParamOnInit(sysWindow);
    }
    zfoverride
    virtual void sysWindowLayoutParamOnChange(ZF_IN ZFUISysWindow *sysWindow)
    {
        zfsuperI(ZFUISysWindowEmbedImpl)::sysWindowLayoutParamOnChange(sysWindow);
    }

    zfoverride
    virtual ZFUIOrientationEnum sysWindowOrientation(ZF_IN ZFUISysWindow *sysWindow)
    {
        return zfsuperI(ZFUISysWindowEmbedImpl)::sysWindowOrientation(sysWindow);
    }
    zfoverride
    virtual void sysWindowOrientationFlagsSet(ZF_IN ZFUISysWindow *sysWindow,
                                              ZF_IN const ZFUIOrientationFlags &flags)
    {
        zfsuperI(ZFUISysWindowEmbedImpl)::sysWindowOrientationFlagsSet(sysWindow, flags);
    }
};
ZFOBJECT_REGISTER(ZFUIKit_nativeViewEmbed_test)

ZF_NAMESPACE_GLOBAL_END

