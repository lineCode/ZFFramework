/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFUIDialogForInput.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFSTYLE_DEFAULT_DEFINE(ZFUIDialogForInput)

void ZFUIDialogForInput::objectOnInit(void)
{
    zfsuper::objectOnInit();

    this->dialogContentContainer()->childAdd(this->inputView());

    ZFLISTENER_LOCAL(inputOnConfirm, {
        ZFUIDialogForInput *dialog = userData->objectHolded();
        ZFUIButton *button = dialog->dialogButtonYes(zffalse);
        if(button != zfnull)
        {
            button->buttonSimulateClick();
        }
    })
    this->inputView()->observerAdd(ZFUITextEdit::EventTextOnEditConfirm(), inputOnConfirm, this->objectHolder());
}
void ZFUIDialogForInput::objectOnDealloc(void)
{
    zfsuper::objectOnDealloc();
}

ZF_NAMESPACE_GLOBAL_END

