/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFUISysWindow.h"
#include "protocol/ZFProtocolZFUISysWindowEmbedNativeView.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclass ZF_ENV_EXPORT _ZFP_ZFUISysWindowEmbedNativeViewImpl : zfextends ZFObject, zfimplements ZFUISysWindowEmbedImpl
{
    ZFOBJECT_DECLARE(_ZFP_ZFUISysWindowEmbedNativeViewImpl, ZFObject)
    ZFIMPLEMENTS_DECLARE(ZFUISysWindowEmbedImpl)

public:
    void *nativeParent;

    // ============================================================
public:
    virtual void nativeWindowOnCleanup(ZF_IN ZFUISysWindow *sysWindow)
    {
    }

    virtual void nativeWindowRootViewOnAdd(ZF_IN ZFUISysWindow *sysWindow,
                                           ZF_OUT void *&nativeParentView)
    {
        nativeParentView = this->nativeParent;
        ZFPROTOCOL_ACCESS(ZFUISysWindowEmbedNativeView)->nativeViewAdd(
            this->nativeParent, sysWindow->rootView()->nativeView());
    }
    virtual void nativeWindowRootViewOnRemove(ZF_IN ZFUISysWindow *sysWindow)
    {
        ZFPROTOCOL_ACCESS(ZFUISysWindowEmbedNativeView)->nativeViewRemote(
            this->nativeParent, sysWindow->rootView()->nativeView());
    }

    virtual zfautoObject modalWindowShow(ZF_IN ZFUISysWindow *sysWindowOwner)
    {
        return ZFUISysWindow::nativeWindowEmbedNativeView(this->nativeParent);
    }
    virtual void modalWindowFinish(ZF_IN ZFUISysWindow *sysWindowOwner,
                                   ZF_IN ZFUISysWindow *sysWindowToFinish)
    {
        sysWindowToFinish->nativeWindowEmbedImplDestroy();
    }
};

// ============================================================
ZFMETHOD_DEFINE_1(ZFUISysWindow, zfautoObject, nativeWindowEmbedNativeView,
                  ZFMP_IN(void *, nativeParent))
{
    zfblockedAlloc(_ZFP_ZFUISysWindowEmbedNativeViewImpl, embedImpl);
    embedImpl->nativeParent = nativeParent;
    zfautoObject ret = ZFUISysWindow::nativeWindowEmbed(embedImpl);
    ZFUISysWindow *window = ret;
    embedImpl->notifyOnCreate(window, zfnull);
    embedImpl->notifyOnResume(window);
    return ret;
}

ZF_NAMESPACE_GLOBAL_END

