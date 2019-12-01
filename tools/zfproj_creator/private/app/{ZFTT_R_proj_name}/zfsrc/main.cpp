#include "ZFCore.h"
#include "ZFUIKit.h"
#include "ZFUIWidget.h"

static void myTestCode(void);

ZFMAIN_ENTRY(params)
{
    // TODO: add your code here
    myTestCode();
    return 0;
}

static void myTestCode(void)
{
    zfblockedAlloc(ZFUIWindow, window);
    window->windowShow();

    zfblockedAlloc(ZFUITextView, textView);
    window->childAdd(textView);
    textView->layoutParam()->layoutAlign(ZFUIAlign::e_Center);
    textView->text("hello world\n\nhello ZFFramework");
    textView->textSingleLine(zffalse);

    zfLogTrimT() << "welcome to ZFFramework";
}

