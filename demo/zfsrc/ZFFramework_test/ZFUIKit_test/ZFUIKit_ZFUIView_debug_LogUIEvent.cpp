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

ZF_NAMESPACE_GLOBAL_BEGIN

#if 0
ZF_GLOBAL_INITIALIZER_INIT(ZFUIKit_ZFUIView_debug_LogUIEvent)
{
    this->onUIEventListener = ZFCallbackForFunc(zfself::onUIEvent);
    ZFObjectGlobalEventObserver().observerAdd(
            ZFUIView::EventViewOnEvent(),
            this->onUIEventListener
        );
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIKit_ZFUIView_debug_LogUIEvent)
{
    ZFObjectGlobalEventObserver().observerRemove(
            ZFUIView::EventViewOnEvent(),
            this->onUIEventListener
        );
}
private:
    ZFListener onUIEventListener;
    static ZFLISTENER_PROTOTYPE_EXPAND(onUIEvent)
    {
        zfLogTrimT() << listenerData.sender->objectInfoOfInstance() << listenerData.param0;
    }
ZF_GLOBAL_INITIALIZER_END(ZFUIKit_ZFUIView_debug_LogUIEvent)
#endif

ZF_NAMESPACE_GLOBAL_END

