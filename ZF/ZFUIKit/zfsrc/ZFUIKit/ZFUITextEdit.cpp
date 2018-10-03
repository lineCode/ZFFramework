/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFUITextEdit.h"
#include "protocol/ZFProtocolZFUITextEdit.h"

#include "ZFUIViewFocus.h"
#include "ZFUIOnScreenKeyboardState.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFENUM_DEFINE(ZFUITextEditKeyboardType)

ZFENUM_DEFINE(ZFUITextEditKeyboardReturnType)
ZFENUM_DEFINE(ZFUITextEditKeyboardReturnAction)

ZFSTYLE_DEFAULT_DEFINE(ZFUITextEdit)

// ============================================================
// _ZFP_ZFUITextEditPrivate
zfclassNotPOD _ZFP_ZFUITextEditPrivate
{
public:
    ZFUITextEdit *pimplOwner;
    zfbool textEditing;
    zfbool textChangedByImplFlag;
    zfbool textSelectRangeChangedByImplFlag;

public:
    void updateSizeRelatedProperty(void)
    {
        ZFPROTOCOL_ACCESS(ZFUITextEdit)->textShadowOffsetSet(this->pimplOwner, ZFUISizeApplyScale(this->pimplOwner->textShadowOffset(), this->pimplOwner->scaleFixed()));
        ZFPROTOCOL_ACCESS(ZFUITextEdit)->textSizeSet(this->pimplOwner, ZFUISizeApplyScale(this->pimplOwner->textSize(), this->pimplOwner->scaleFixed()));
    }

public:
    _ZFP_ZFUITextEditPrivate(void)
    : pimplOwner(zfnull)
    , textEditing(zffalse)
    , textChangedByImplFlag(zffalse)
    , textSelectRangeChangedByImplFlag(zffalse)
    {
    }
};

// ============================================================
// ZFUITextEdit
ZFOBJECT_REGISTER(ZFUITextEdit)

ZFOBSERVER_EVENT_REGISTER(ZFUITextEdit, TextOnEditBegin)
ZFOBSERVER_EVENT_REGISTER(ZFUITextEdit, TextOnEditEnd)
ZFOBSERVER_EVENT_REGISTER(ZFUITextEdit, TextOnChangeCheck)
ZFOBSERVER_EVENT_REGISTER(ZFUITextEdit, TextOnChange)
ZFOBSERVER_EVENT_REGISTER(ZFUITextEdit, TextOnReturnClick)
ZFOBSERVER_EVENT_REGISTER(ZFUITextEdit, TextOnEditConfirm)

ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFUITextEdit, zfbool, textEditEnable)
{
    ZFPROTOCOL_ACCESS(ZFUITextEdit)->textEditEnableSet(this, this->textEditEnable());
}
ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFUITextEdit, zfbool, textEditSecured)
{
    ZFPROTOCOL_ACCESS(ZFUITextEdit)->textEditSecuredSet(this, this->textEditSecured());
}
ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFUITextEdit, ZFUITextEditKeyboardTypeEnum, textEditKeyboardType)
{
    ZFPROTOCOL_ACCESS(ZFUITextEdit)->textEditKeyboardTypeSet(this, this->textEditKeyboardType());
}
ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFUITextEdit, ZFUITextEditKeyboardReturnTypeEnum, textEditKeyboardReturnType)
{
    ZFPROTOCOL_ACCESS(ZFUITextEdit)->textEditKeyboardReturnTypeSet(this, this->textEditKeyboardReturnType());
}
ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFUITextEdit, ZFRegExp *, textEditFilter)
{
    if(!this->text().isEmpty() && !this->textShouldChange(this->text()))
    {
        this->textSet("");
    }
}
ZFPROPERTY_OVERRIDE_ON_VERIFY_DEFINE(ZFUITextEdit, ZFIndexRange, textSelectRange)
{
    zfindex textLength = this->text().length();
    if(propertyValue.start >= textLength)
    {
        propertyValue = ZFIndexRangeZero();
    }
    else if(propertyValue.count > textLength - propertyValue.start)
    {
        propertyValue.count = textLength - propertyValue.start;
    }
}
ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFUITextEdit, ZFIndexRange, textSelectRange)
{
    if(d->textSelectRangeChangedByImplFlag)
    {
        d->textSelectRangeChangedByImplFlag = zffalse;
    }
    else
    {
        ZFPROTOCOL_ACCESS(ZFUITextEdit)->textSelectRangeSet(this, this->textSelectRange());
    }
}

ZFPROPERTY_OVERRIDE_ON_VERIFY_DEFINE(ZFUITextEdit, zfstring, text)
{
    if(!propertyValue.isEmpty() && !this->textShouldChange(propertyValue))
    {
        propertyValue.removeAll();
    }
}
ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFUITextEdit, zfstring, text)
{
    if(d->textChangedByImplFlag)
    {
        d->textChangedByImplFlag = zffalse;
    }
    else
    {
        ZFPROTOCOL_ACCESS(ZFUITextEdit)->textSet(this, this->text());
    }
    if(propertyValueOld.compare(this->text()) != 0)
    {
        ZFCastZFObjectUnchecked(ZFUIView *, this->textPlaceHolder())->viewVisibleSet(this->text().isEmpty());
        this->layoutRequest();
    }
}

ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFUITextEdit, ZFUITextAppearanceEnum, textAppearance)
{
    ZFPROTOCOL_ACCESS(ZFUITextEdit)->textAppearanceSet(this, this->textAppearance());
    if(this->textAppearance() != propertyValueOld)
    {
        this->layoutRequest();
    }
}
ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFUITextEdit, ZFUIAlignFlags, textAlign)
{
    ZFPROTOCOL_ACCESS(ZFUITextEdit)->textAlignSet(this, this->textAlign());
    this->textPlaceHolder()->textAlignSet(this->textAlign());
}
ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFUITextEdit, ZFUIColor, textColor)
{
    ZFPROTOCOL_ACCESS(ZFUITextEdit)->textColorSet(this, this->textColor());
}
ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFUITextEdit, ZFUIColor, textShadowColor)
{
    ZFPROTOCOL_ACCESS(ZFUITextEdit)->textShadowColorSet(this, this->textShadowColor());
}
ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFUITextEdit, ZFUISize, textShadowOffset)
{
    ZFPROTOCOL_ACCESS(ZFUITextEdit)->textShadowOffsetSet(this, ZFUISizeApplyScale(this->textShadowOffset(), this->scaleFixed()));
}
ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFUITextEdit, zfint, textSize)
{
    ZFPROTOCOL_ACCESS(ZFUITextEdit)->textSizeSet(this, ZFUISizeApplyScale(this->textSize(), this->scaleFixed()));
    if(this->textSize() != propertyValueOld)
    {
        this->layoutRequest();
    }
}

void ZFUITextEdit::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFUITextEditPrivate);
    d->pimplOwner = this;

    zfCoreAssert(this->nativeImplView() == zfnull);
    zfclassNotPOD _ZFP_ZFUITextEdit_nativeImplViewDestroy
    {
    public:
        static void action(ZF_IN ZFUIView *view,
                           ZF_IN void *nativeImplView)
        {
            ZFPROTOCOL_ACCESS(ZFUITextEdit)->nativeTextEditDestroy(view->to<ZFUITextEdit *>(), nativeImplView);
        }
    };
    this->nativeImplViewSet(ZFPROTOCOL_ACCESS(ZFUITextEdit)->nativeTextEditCreate(this),
        _ZFP_ZFUITextEdit_nativeImplViewDestroy::action);

    ZFUIView *textPlaceHolderTmp = ZFCastZFObject(ZFUIView *, this->textPlaceHolder());
    if(textPlaceHolderTmp == zfnull)
    {
        if(this->textPlaceHolder() != zfnull)
        {
            zfCoreCriticalClassNotTypeOf(this->textPlaceHolder()->classData(), ZFUIView::ClassData());
        }
        else
        {
            zfCoreCriticalMessage("textPlaceHolder must not be null");
        }
        return ;
    }
    this->internalImplViewAdd(textPlaceHolderTmp);
    textPlaceHolderTmp->layoutParam()->sizeParamSet(ZFUISizeParamFillFill());
    textPlaceHolderTmp->serializableRefLayoutParam()->sizeParamSet(ZFUISizeParamFillFill());
}
void ZFUITextEdit::objectOnDealloc(void)
{
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}
void ZFUITextEdit::objectOnInitFinish(void)
{
    zfsuper::objectOnInitFinish();
    d->updateSizeRelatedProperty();
}

void ZFUITextEdit::objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
{
    zfsuper::objectInfoOnAppend(ret);

    if(!this->textPlaceHolder()->text().isEmpty())
    {
        zfstringAppend(ret, " (%s)", this->textPlaceHolder()->text().cString());
    }

    if(this->textEditSecured())
    {
        ret += " EditSecured";
    }
    else if(!this->text().isEmpty())
    {
        zfstringAppend(ret, " \"%s\"", this->text().cString());
    }

    if(!this->textEditEnable())
    {
        ret += " EditDisabled";
    }
}

ZFMETHOD_DEFINE_2(ZFUITextEdit, void, measureTextEdit,
                  ZFMP_OUT(ZFUISize &, ret),
                  ZFMP_IN_OPT(const ZFUISize &, sizeHint, ZFUISizeZero()))
{
    const ZFUIMargin &nativeImplViewMargin = this->nativeImplViewMargin();
    ZFUISizeApplyScaleReversely(ret, ZFPROTOCOL_ACCESS(ZFUITextEdit)->measureNativeTextEdit(this,
        ZFUISizeApplyScale(ZFUIViewLayoutParam::sizeHintOffset(sizeHint, ZFUISizeMake(
                    0 - ZFUIMarginGetWidth(nativeImplViewMargin),
                    0 - ZFUIMarginGetHeight(nativeImplViewMargin)
                )),
            this->scaleFixed()),
        ZFUISizeApplyScale(this->textSize(), this->scaleFixed())),
        this->scaleFixed());
    if(ZFPropertyIsValueAccessed(ZFPropertyAccess(ZFUITextEdit, textPlaceHolder), this)
        && !this->textPlaceHolder()->text().isEmpty())
    {
        ZFUISize hintSize = ZFUISizeZero();
        this->textPlaceHolder()->to<ZFUITextView *>()->measureTextView(hintSize);
        if(ret.width < hintSize.width)
        {
            ret.width = hintSize.width;
        }
        if(ret.height < hintSize.height)
        {
            ret.height = hintSize.height;
        }
    }
    ZFUISizeApplyMarginReversely(ret, ret, nativeImplViewMargin);
}

void ZFUITextEdit::_ZFP_ZFUITextEdit_textNotifyBeginEdit(void)
{
    d->textEditing = zftrue;
    this->textOnEditBegin();
}
void ZFUITextEdit::_ZFP_ZFUITextEdit_textNotifyEndEdit(void)
{
    d->textEditing = zffalse;
    this->textOnEditEnd();
}
void ZFUITextEdit::_ZFP_ZFUITextEdit_textNotifyChange(ZF_IN const zfchar *newText)
{
    zfstring oldText = this->text();

    d->textChangedByImplFlag = zftrue;
    this->textSet(newText);

    this->textOnChange(oldText);
}
void ZFUITextEdit::_ZFP_ZFUITextEdit_textSelectRangeNotifyChange(void)
{
    if(d->textSelectRangeChangedByImplFlag)
    {
        return ;
    }

    ZFIndexRange tmp = ZFIndexRangeZero();
    ZFPROTOCOL_ACCESS(ZFUITextEdit)->textSelectRange(this, tmp);

    d->textSelectRangeChangedByImplFlag = zftrue;
    this->textSelectRangeSet(tmp);
}
void ZFUITextEdit::_ZFP_ZFUITextEdit_textNotifyReturnClicked(void)
{
    this->textOnReturnClick();

    switch(this->textEditKeyboardReturnAction())
    {
        case ZFUITextEditKeyboardReturnAction::e_None:
            break;
        case ZFUITextEditKeyboardReturnAction::e_Confirm:
            this->textEditNotifyConfirm();
            break;
        case ZFUITextEditKeyboardReturnAction::e_FocusNext:
            if(!this->viewFocused() || !this->textEditConfirmWhenLostFocus())
            {
                this->textEditNotifyConfirm();
            }
            if(this->viewFocused())
            {
                ZFUIView *next = ZFUIViewFocusNextFind(this);
                if(next == zfnull)
                {
                    this->textEditEnd();
                }
                else
                {
                    ZFUITextEdit *nextTmp = ZFCastZFObject(ZFUITextEdit *, next);
                    if(nextTmp != zfnull)
                    {
                        nextTmp->textEditBegin();
                    }
                    else
                    {
                        next->viewFocusRequest(zftrue);
                    }
                }
            }
            break;
        case ZFUITextEditKeyboardReturnAction::e_HideKeyboard:
        {
            this->textEditNotifyConfirm();
            ZFUIOnScreenKeyboardState *keyboardState = ZFUIOnScreenKeyboardState::instanceForView(this);
            if(keyboardState != zfnull && keyboardState->keyboardShowing() && this->textEditing())
            {
                this->textEditEnd();
            }
        }
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            return ;
    }
}
ZFMETHOD_DEFINE_1(ZFUITextEdit, zfbool, textShouldChange,
                  ZFMP_IN(const zfchar *, newText))
{
    zfbool shouldChange = zftrue;
    this->textOnChangeCheck(newText, shouldChange);
    if(!shouldChange && newText != zfnull && *newText != '\0')
    {
        return zffalse;
    }
    else
    {
        return zftrue;
    }
}
ZFMETHOD_DEFINE_0(ZFUITextEdit, void, textEditBegin)
{
    ZFPROTOCOL_ACCESS(ZFUITextEdit)->textEditBegin(this);
}
ZFMETHOD_DEFINE_0(ZFUITextEdit, void, textEditEnd)
{
    ZFPROTOCOL_ACCESS(ZFUITextEdit)->textEditEnd(this);
}
ZFMETHOD_DEFINE_0(ZFUITextEdit, zfbool, textEditing)
{
    return d->textEditing;
}

void ZFUITextEdit::textOnEditBegin(void)
{
    this->observerNotify(ZFUITextEdit::EventTextOnEditBegin());
}
void ZFUITextEdit::textOnEditEnd(void)
{
    this->observerNotify(ZFUITextEdit::EventTextOnEditEnd());
}
void ZFUITextEdit::textOnChangeCheck(ZF_IN const zfchar *newText, ZF_IN_OUT zfbool &shouldChange)
{
    shouldChange = zftrue;
    if(newText != zfnull && *newText != '\0' && this->textEditFilter() != zfnull)
    {
        ZFRegExpResult regexpResult;
        this->textEditFilter()->regExpMatchExact(regexpResult, newText);
        if(!regexpResult.matched)
        {
            shouldChange = zffalse;
            return ;
        }
    }

    if(this->observerHasAdd(ZFUITextEdit::EventTextOnChangeCheck()))
    {
        zfautoObject t = ZFValueEditable::boolValueCreate(shouldChange);
        this->observerNotify(ZFUITextEdit::EventTextOnChangeCheck(), zflineAlloc(ZFString, newText), t.toObject());
        shouldChange = t.to<ZFValue *>()->boolValue();
    }
}
void ZFUITextEdit::textOnChange(ZF_IN const zfchar *oldText)
{
    if(this->observerHasAdd(ZFUITextEdit::EventTextOnChange()))
    {
        zfblockedAlloc(ZFString, oldTextTmp, oldText);
        this->observerNotify(ZFUITextEdit::EventTextOnChange(), oldTextTmp);
    }
}
void ZFUITextEdit::textOnReturnClick(void)
{
    this->observerNotify(ZFUITextEdit::EventTextOnReturnClick());
}
void ZFUITextEdit::textOnEditConfirm(void)
{
    this->observerNotify(ZFUITextEdit::EventTextOnEditConfirm());
}

void ZFUITextEdit::scaleOnChange(void)
{
    zfsuper::scaleOnChange();
    d->updateSizeRelatedProperty();
}
void ZFUITextEdit::layoutOnMeasure(ZF_OUT ZFUISize &ret,
                                   ZF_IN const ZFUISize &sizeHint,
                                   ZF_IN const ZFUISizeParam &sizeParam)
{
    ret = this->viewSizePrefered();
    ZFUISize tmp = ZFUISizeZero();
    this->measureTextEdit(tmp, sizeHint);
    if(ret.width < tmp.width)
    {
        ret.width = tmp.width;
    }
    if(ret.height < 0)
    {
        ret.height = ZFUIGlobalStyle::DefaultStyle()->itemSizeControl();
    }
}
void ZFUITextEdit::viewEventOnKeyEvent(ZF_IN ZFUIKeyEvent *keyEvent)
{
    switch(keyEvent->keyCode)
    {
        case ZFUIKeyCode::e_kShift:
        case ZFUIKeyCode::e_kTab:
            zfsuper::viewEventOnKeyEvent(keyEvent);
            break;
        default:
            keyEvent->eventResolvedSet(zftrue);
            break;
    }
}
void ZFUITextEdit::viewFocusOnChange(void)
{
    zfsuper::viewFocusOnChange();
    if(!this->viewFocused() && this->textEditConfirmWhenLostFocus())
    {
        this->textEditNotifyConfirm();
    }
}

zfbool ZFUITextEdit::internalViewShouldLayout(ZF_IN ZFUIView *internalView)
{
    if(internalView == this->textPlaceHolder()->toObject())
    {
        return zffalse;
    }
    return zfsuper::internalViewShouldLayout(internalView);
}
void ZFUITextEdit::internalBgViewOnLayout(ZF_IN const ZFUIRect &bounds)
{
    zfsuper::internalBgViewOnLayout(bounds);

    ZFUITextView *textPlaceHolder = this->textPlaceHolder()->to<ZFUITextView *>();
    textPlaceHolder->layout(ZFUIRectApplyMargin(
            bounds,
            ZFUIMarginInc(this->nativeImplViewMargin(), textPlaceHolder->layoutParam()->layoutMargin())
        ));
}

ZF_NAMESPACE_GLOBAL_END

