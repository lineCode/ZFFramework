#include "ZFCore.h"
#include "ZFUIKit.h"
#include "ZFUIWidget.h"

#include "ZF2048Core/ZF2048Core.h"
#include "ZF2048UI/ZF2048UIFrame.h"
#include "ZF2048App.h"

ZFMAIN_ENTRY(params)
{
    if(zftrue)
    {
        zfblockedAlloc(ZFUIWindow, window);
        window->windowShow();

        zfblockedAlloc(ZF2048App, game);
        window->childAdd(game);
        game->layoutParam()->sizeParam(ZFUISizeParamFillFill());
        game->layoutParam()->layoutMargin(ZFUIMarginMake(0, 20, 0, 0));
    }
    else
    {
        zfblockedAlloc(ZF2048Core, game);
        game->reset();
        game->debugStatus();

        zfblockedAlloc(ZFUIWindow, window);
        window->objectTag("game", game);
        window->windowShow();
        zfblockedAlloc(ZFUILinearLayout, ll);
        window->childAdd(ll);
        ll->layoutParam()->layoutAlign(ZFUIAlign::e_Center);
        ll->layoutParam()->sizeParam(ZFUISizeParamFillWrap());

        zfblockedAlloc(ZFUIButtonBasic, left);
        ll->childAdd(left);
        left->buttonLabelText("left");
        ZFLISTENER_LOCAL(leftOnClick, {
            ZF2048Core *game = userData->toAny();
            game->moveLeft();
            game->debugStatus();
        })
        left->observerAdd(ZFUIButton::EventButtonOnClick(), leftOnClick, game);

        zfblockedAlloc(ZFUIButtonBasic, top);
        ll->childAdd(top);
        top->buttonLabelText("top");
        ZFLISTENER_LOCAL(topOnClick, {
            ZF2048Core *game = userData->toAny();
            game->moveTop();
            game->debugStatus();
        })
        top->observerAdd(ZFUIButton::EventButtonOnClick(), topOnClick, game);

        zfblockedAlloc(ZFUIButtonBasic, right);
        ll->childAdd(right);
        right->buttonLabelText("right");
        ZFLISTENER_LOCAL(rightOnClick, {
            ZF2048Core *game = userData->toAny();
            game->moveRight();
            game->debugStatus();
        })
        right->observerAdd(ZFUIButton::EventButtonOnClick(), rightOnClick, game);

        zfblockedAlloc(ZFUIButtonBasic, bottom);
        ll->childAdd(bottom);
        bottom->buttonLabelText("bottom");
        ZFLISTENER_LOCAL(bottomOnClick, {
            ZF2048Core *game = userData->toAny();
            game->moveBottom();
            game->debugStatus();
        })
        bottom->observerAdd(ZFUIButton::EventButtonOnClick(), bottomOnClick, game);

        zfblockedAlloc(ZFUIButtonBasic, reset);
        ll->childAdd(reset);
        reset->buttonLabelText("reset");
        ZFLISTENER_LOCAL(resetOnClick, {
            ZF2048Core *game = userData->toAny();
            game->reset();
            game->debugStatus();
        })
        reset->observerAdd(ZFUIButton::EventButtonOnClick(), resetOnClick, game);

        zfblockedAlloc(ZFUIButtonBasic, undo);
        ll->childAdd(undo);
        undo->buttonLabelText("undo");
        ZFLISTENER_LOCAL(undoOnClick, {
            ZF2048Core *game = userData->toAny();
            game->undo();
            game->debugStatus();
        })
        undo->observerAdd(ZFUIButton::EventButtonOnClick(), undoOnClick, game);

        for(zfindex i = 0; i < ll->childCount(); ++i)
        {
            ZFUIButtonBasic *button = ll->childAtIndex(i)->toAny();
            button->viewBackgroundColor(ZFUIColorRandom());
            button->layoutParam<ZFUILinearLayoutParam *>()->layoutWeight(1);
        }
    }

    return 0;
}

#if 0
ZF_GLOBAL_INITIALIZER_INIT(AutoPrintUIViewTree)
{
    ZFLISTENER_LOCAL(action, {
        ZFUIViewTreePrint(ZFUISysWindow::mainWindow()->rootView());
    })
    ZFUISysWindow::mainWindow()->observerAdd(ZFUISysWindow::EventSysWindowOnPause(), action);
}
ZF_GLOBAL_INITIALIZER_END(AutoPrintUIViewTree)
#endif

#if 1 // just for fun, click one view 3 times at same point, would change language
ZF_GLOBAL_INITIALIZER_INIT(testLangLoad)
{
    ZFObjectGlobalEventObserver().observerAdd(ZFUIView::EventViewOnEvent(), ZFCallbackForFunc(zfself::action));
}
ZF_GLOBAL_INITIALIZER_DESTROY(testLangLoad)
{
    ZFObjectGlobalEventObserver().observerRemove(ZFUIView::EventViewOnEvent(), ZFCallbackForFunc(zfself::action));
}
private:
    static ZFLISTENER_PROTOTYPE_EXPAND(action)
    {
        ZFUIMouseEvent *event = listenerData.param0<ZFUIMouseEvent *>();
        if(event == zfnull || event->mouseAction != ZFUIMouseAction::e_MouseUp)
        {
            return ;
        }
        static ZFUIPoint lastPoint = ZFUIPointZero();
        static zfindex lastCount = 0;
        static zfbool langFlag = zftrue;
        if(zfmAbs(event->mousePoint.x - lastPoint.x) <= 5
           && zfmAbs(event->mousePoint.y - lastPoint.y) <= 5)
        {
            ++lastCount;
            if(lastCount == 3)
            {
                lastCount = 0;
                langFlag = !langFlag;
                const zfchar *langFile = langFlag ? "ZF2048/lang/lang_default.xml" : "ZF2048/lang/lang_zh-CN.xml";
                ZFStyleLoad(ZFPathInfo(ZFPathType_res(), langFile));
            }
        }
        else
        {
            lastCount = 0;
        }
        lastPoint = event->mousePoint;
    }
ZF_GLOBAL_INITIALIZER_END(testLangLoad)
#endif

