/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFImpl_sys_Qt_ZFUIKit.h"

#include "ZFUIKit.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIViewFocus.h"

#if ZF_ENV_sys_Qt

#include <QWidget>
#include <QEvent>
#include <QCoreApplication>

class _ZFP_ZFUIViewImpl_sys_Qt_FocusProxyToken : public QObject
{
    Q_OBJECT

public:
    ZFUIView *ownerZFUIView;
    QWidget *nativeOwner;
    QWidget *nativeImplView;

public:
    void attach(ZF_IN ZFUIView *ownerZFUIView,
                ZF_IN QWidget *nativeOwner,
                ZF_IN QWidget *nativeImplView)
    {
        this->nativeOwner->installEventFilter(this);
        if(this->nativeImplView != zfnull)
        {
            this->nativeImplView->installEventFilter(this);
        }
    }
    void detach(void)
    {
        this->nativeOwner->removeEventFilter(this);
        if(this->nativeImplView != zfnull)
        {
            this->nativeImplView->removeEventFilter(this);
        }
    }

public:
    _ZFP_ZFUIViewImpl_sys_Qt_FocusProxyToken(ZF_IN ZFUIView *ownerZFUIView,
                                             ZF_IN QWidget *nativeOwner,
                                             ZF_IN QWidget *nativeImplView)
    : QObject()
    , ownerZFUIView(ownerZFUIView)
    , nativeOwner(nativeOwner)
    , nativeImplView(nativeImplView)
    {
        this->attach(ownerZFUIView, nativeOwner, nativeImplView);
    }
    ~_ZFP_ZFUIViewImpl_sys_Qt_FocusProxyToken(void)
    {
        this->detach();
    }

protected:
    bool eventFilter(QObject *obj, QEvent *event)
    {
        static ZFUIView *_lastFocused = zfnull;
        switch(event->type())
        {
            case QEvent::FocusIn:
                if(this->ownerZFUIView != _lastFocused)
                {
                    _lastFocused = this->ownerZFUIView;
                    ZFPROTOCOL_ACCESS(ZFUIViewFocus)->notifyViewFocusChanged(this->ownerZFUIView);
                }
                break;
            case QEvent::FocusOut:
                if(this->ownerZFUIView == _lastFocused)
                {
                    _lastFocused = zfnull;
                    ZFPROTOCOL_ACCESS(ZFUIViewFocus)->notifyViewFocusChanged(this->ownerZFUIView);
                }
                break;
            default:
                break;
        }
        return false;
    }
};

void *_ZFP_ZFUIViewImpl_sys_Qt_FocusProxy_attach(ZF_IN ZFUIView *ownerZFUIView,
                                                 ZF_IN QWidget *nativeOwner,
                                                 ZF_IN QWidget *nativeImplViewOrNull,
                                                 ZF_IN_OPT void *tokenOld /* = zfnull */)
{
    if(tokenOld == zfnull)
    {
        return new _ZFP_ZFUIViewImpl_sys_Qt_FocusProxyToken(ownerZFUIView, nativeOwner, nativeImplViewOrNull);
    }
    else
    {
        _ZFP_ZFUIViewImpl_sys_Qt_FocusProxyToken *token = (_ZFP_ZFUIViewImpl_sys_Qt_FocusProxyToken *)tokenOld;
        token->detach();
        token->attach(ownerZFUIView, nativeOwner, nativeImplViewOrNull);
        return token;
    }
}
void _ZFP_ZFUIViewImpl_sys_Qt_FocusProxy_detach(ZF_IN void *token)
{
    _ZFP_ZFUIViewImpl_sys_Qt_FocusProxyToken *tokenTmp = (_ZFP_ZFUIViewImpl_sys_Qt_FocusProxyToken *)token;
    tokenTmp->detach();
}
void _ZFP_ZFUIViewImpl_sys_Qt_FocusProxy_cleanup(ZF_IN void *token)
{
    delete ZFCastStatic(_ZFP_ZFUIViewImpl_sys_Qt_FocusProxyToken *, token);
}
void _ZFP_ZFUIViewImpl_sys_Qt_FocusProxy_viewFocusableSet(ZF_IN void *token, ZF_IN zfbool v)
{
    _ZFP_ZFUIViewImpl_sys_Qt_FocusProxyToken *t = ZFCastStatic(_ZFP_ZFUIViewImpl_sys_Qt_FocusProxyToken *, token);
    if(v)
    {
        t->nativeOwner->setFocusPolicy(Qt::ClickFocus);
        if(t->nativeImplView != zfnull &&  t->nativeImplView->focusPolicy() != Qt::NoFocus)
        {
            t->nativeImplView->setFocusPolicy(Qt::ClickFocus);
            t->nativeOwner->setFocusProxy(t->nativeImplView);
        }
    }
    else
    {
        t->nativeOwner->setFocusPolicy(Qt::NoFocus);
        t->nativeOwner->setFocusProxy(NULL);
    }
}
void _ZFP_ZFUIViewImpl_sys_Qt_FocusProxy_viewFocusRequest(ZF_IN void *token, ZF_IN zfbool v)
{
    _ZFP_ZFUIViewImpl_sys_Qt_FocusProxyToken *t = ZFCastStatic(_ZFP_ZFUIViewImpl_sys_Qt_FocusProxyToken *, token);
    if(v)
    {
        t->nativeOwner->setFocus();
    }
    else
    {
        t->nativeOwner->clearFocus();
    }
}
zfbool _ZFP_ZFUIViewImpl_sys_Qt_FocusProxy_viewFocused(ZF_IN void *token)
{
    _ZFP_ZFUIViewImpl_sys_Qt_FocusProxyToken *t = ZFCastStatic(_ZFP_ZFUIViewImpl_sys_Qt_FocusProxyToken *, token);
    return t->nativeOwner->hasFocus();
}

// ============================================================
class _ZFP_ZFUIViewImpl_sys_Qt_FocusProxyGlobalToken : public QObject
{
    Q_OBJECT

public:
    QWidget *focused;

public:
    _ZFP_ZFUIViewImpl_sys_Qt_FocusProxyGlobalToken(void)
    : QObject()
    , focused(NULL)
    {
        QCoreApplication::instance()->installEventFilter(this);
    }
    ~_ZFP_ZFUIViewImpl_sys_Qt_FocusProxyGlobalToken(void)
    {
        QCoreApplication::instance()->removeEventFilter(this);
    }

protected:
    bool eventFilter(QObject *obj, QEvent *event)
    {
        switch(event->type())
        {
            case QEvent::FocusIn:
                this->focused = qobject_cast<QWidget *>(obj);
                break;
            case QEvent::FocusOut:
                this->focused = NULL;
                break;
            default:
                break;
        }
        return false;
    }
};
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIViewImpl_sys_Qt_FocusProxyGlobalDataHolder, ZFLevelZFFrameworkEssential)
{
    this->token = new _ZFP_ZFUIViewImpl_sys_Qt_FocusProxyGlobalToken();
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIViewImpl_sys_Qt_FocusProxyGlobalDataHolder)
{
    delete this->token;
}
_ZFP_ZFUIViewImpl_sys_Qt_FocusProxyGlobalToken *token;
ZF_GLOBAL_INITIALIZER_END(ZFUIViewImpl_sys_Qt_FocusProxyGlobalDataHolder)

zfbool _ZFP_ZFUIViewImpl_sys_Qt_FocusProxy_viewFocusedRecursive(ZF_IN void *token)
{
    _ZFP_ZFUIViewImpl_sys_Qt_FocusProxyToken *t = ZFCastStatic(_ZFP_ZFUIViewImpl_sys_Qt_FocusProxyToken *, token);
    QWidget *focused = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewImpl_sys_Qt_FocusProxyGlobalDataHolder)->token->focused;
    while(focused != NULL)
    {
        if(focused == t->nativeOwner)
        {
            return zftrue;
        }
        focused = focused->parentWidget();
    }
    return zffalse;
}

#include "ZFProtocolZFUIView_sys_Qt_FocusProxy.moc"

#endif // #if ZF_ENV_sys_Qt

