/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFUITextView.h"
#include "protocol/ZFProtocolZFUITextView.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFSTYLE_DEFAULT_DEFINE(ZFUITextView)

// ============================================================
static void _ZFP_ZFUITextView_updateSizeRelatedProperty(ZF_IN ZFUITextView *owner)
{
    ZFPROTOCOL_INTERFACE_CLASS(ZFUITextView) *impl = ZFPROTOCOL_ACCESS(ZFUITextView);
    impl->textShadowOffsetSet(owner, ZFUISizeApplyScale(owner->textShadowOffset(), owner->scaleFixed()));
    impl->textSizeSet(owner, ZFUISizeApplyScale(owner->textSize(), owner->scaleFixed()));
    impl->textSizeAutoChangeMinSizeSet(owner, ZFUISizeApplyScale(owner->textSizeAutoChangeMinSize(), owner->scaleFixed()));
    impl->textSizeAutoChangeMaxSizeSet(owner, ZFUISizeApplyScale(owner->textSizeAutoChangeMaxSize(), owner->scaleFixed()));
}

// ============================================================
// ZFUITextView
ZFOBJECT_REGISTER(ZFUITextView)

ZFPROPERTY_CUSTOM_ON_UPDATE_DEFINE(ZFUITextView, zfstring, text)
{
    ZFPROTOCOL_ACCESS(ZFUITextView)->textSet(this, this->text());
    if(propertyValueOld.compare(this->text()) != 0)
    {
        this->layoutRequest();
    }
}

ZFPROPERTY_CUSTOM_ON_UPDATE_DEFINE(ZFUITextView, ZFUITextAppearanceEnum, textAppearance)
{
    ZFPROTOCOL_ACCESS(ZFUITextView)->textAppearanceSet(this, this->textAppearance());
    if(this->textAppearance() != propertyValueOld)
    {
        this->layoutRequest();
    }
}
ZFPROPERTY_CUSTOM_ON_UPDATE_DEFINE(ZFUITextView, ZFUIAlignFlags, textAlign)
{
    ZFPROTOCOL_ACCESS(ZFUITextView)->textAlignSet(this, this->textAlign());
}
ZFPROPERTY_CUSTOM_ON_UPDATE_DEFINE(ZFUITextView, ZFUIColor, textColor)
{
    ZFPROTOCOL_ACCESS(ZFUITextView)->textColorSet(this, this->textColor());
}
ZFPROPERTY_CUSTOM_ON_UPDATE_DEFINE(ZFUITextView, ZFUIColor, textShadowColor)
{
    ZFPROTOCOL_ACCESS(ZFUITextView)->textShadowColorSet(this, this->textShadowColor());
}
ZFPROPERTY_CUSTOM_ON_UPDATE_DEFINE(ZFUITextView, ZFUISize, textShadowOffset)
{
    ZFPROTOCOL_ACCESS(ZFUITextView)->textShadowOffsetSet(this, ZFUISizeApplyScale(this->textShadowOffset(), this->scaleFixed()));
}
ZFPROPERTY_CUSTOM_ON_UPDATE_DEFINE(ZFUITextView, zfint, textSize)
{
    ZFPROTOCOL_ACCESS(ZFUITextView)->textSizeSet(this, ZFUISizeApplyScale(this->textSize(), this->scaleFixed()));
    if(this->textSize() != propertyValueOld)
    {
        this->layoutRequest();
    }
}
ZFPROPERTY_CUSTOM_ON_UPDATE_DEFINE(ZFUITextView, zfint, textSizeAutoChangeMinSize)
{
    ZFPROTOCOL_ACCESS(ZFUITextView)->textSizeAutoChangeMinSizeSet(this, ZFUISizeApplyScale(this->textSizeAutoChangeMinSize(), this->scaleFixed()));
    if(this->textSizeAutoChangeMinSize() != propertyValueOld)
    {
        this->layoutRequest();
    }
}
ZFPROPERTY_CUSTOM_ON_UPDATE_DEFINE(ZFUITextView, zfint, textSizeAutoChangeMaxSize)
{
    ZFPROTOCOL_ACCESS(ZFUITextView)->textSizeAutoChangeMaxSizeSet(this, ZFUISizeApplyScale(this->textSizeAutoChangeMaxSize(), this->scaleFixed()));
    if(this->textSizeAutoChangeMaxSize() != propertyValueOld)
    {
        this->layoutRequest();
    }
}
ZFPROPERTY_CUSTOM_ON_UPDATE_DEFINE(ZFUITextView, zfbool, textSingleLine)
{
    ZFPROTOCOL_ACCESS(ZFUITextView)->textSingleLineSet(this, this->textSingleLine());
    if(this->textSingleLine() != propertyValueOld)
    {
        this->layoutRequest();
    }
}
ZFPROPERTY_CUSTOM_ON_UPDATE_DEFINE(ZFUITextView, ZFUITextTruncateModeEnum, textTruncateMode)
{
    ZFPROTOCOL_ACCESS(ZFUITextView)->textTruncateModeSet(this, this->textTruncateMode());
    if(this->textTruncateMode() != propertyValueOld)
    {
        this->layoutRequest();
    }
}

void ZFUITextView::objectOnInit(void)
{
    zfsuper::objectOnInit();

    zfCoreAssert(this->nativeImplView() == zfnull);
    zfclassNotPOD _ZFP_ZFUITextView_nativeImplViewDestroy
    {
    public:
        static void action(ZF_IN ZFUIView *view,
                           ZF_IN void *nativeImplView)
        {
            ZFPROTOCOL_ACCESS(ZFUITextView)->nativeTextViewDestroy(view->to<ZFUITextView *>(), nativeImplView);
        }
    };
    this->nativeImplViewSet(ZFPROTOCOL_ACCESS(ZFUITextView)->nativeTextViewCreate(this),
                            _ZFP_ZFUITextView_nativeImplViewDestroy::action);
}
void ZFUITextView::objectOnDealloc(void)
{
    zfsuper::objectOnDealloc();
}
void ZFUITextView::objectOnInitFinish(void)
{
    zfsuper::objectOnInitFinish();
    _ZFP_ZFUITextView_updateSizeRelatedProperty(this);
}

void ZFUITextView::objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
{
    zfsuper::objectInfoOnAppend(ret);

    if(!this->text().isEmpty())
    {
        ret += zfText(" \"");
        ret += this->text();
        ret += zfText("\"");
    }
}

ZFMETHOD_DEFINE_2(ZFUITextView, void, measureTextView,
                  ZFMP_OUT(ZFUISize &, ret),
                  ZFMP_IN_OPT(const ZFUISize &, sizeHint, ZFUISizeZero()))
{
    const ZFUIMargin &nativeImplViewMargin = this->nativeImplViewMargin();
    ZFUISizeApplyScaleReversely(ret, ZFPROTOCOL_ACCESS(ZFUITextView)->measureNativeTextView(this,
        ZFUISizeApplyScale(ZFUIViewLayoutParam::sizeHintOffset(sizeHint, ZFUISizeMake(
                    0 - ZFUIMarginGetWidth(nativeImplViewMargin),
                    0 - ZFUIMarginGetHeight(nativeImplViewMargin)
                )),
            this->scaleFixed()),
        ZFUISizeApplyScale(this->textSize(), this->scaleFixed())),
        this->scaleFixed());
    ZFUISizeApplyMarginReversely(ret, ret, nativeImplViewMargin);
}

ZFMETHOD_DEFINE_0(ZFUITextView, zfint, textSizeCurrent)
{
    return ZFUISizeApplyScaleReversely(ZFPROTOCOL_ACCESS(ZFUITextView)->textSizeCurrent(this), this->scaleFixed());
}

void ZFUITextView::scaleOnChange(void)
{
    zfsuper::scaleOnChange();
    _ZFP_ZFUITextView_updateSizeRelatedProperty(this);
}
void ZFUITextView::layoutOnMeasure(ZF_OUT ZFUISize &ret,
                                   ZF_IN const ZFUISize &sizeHint,
                                   ZF_IN const ZFUISizeParam &sizeParam)
{
    this->measureTextView(ret, sizeHint);
}
void ZFUITextView::internalImplViewOnLayout(ZF_IN const ZFUIRect &bounds)
{
    zfsuper::internalFgViewOnLayout(bounds);
    ZFPROTOCOL_ACCESS(ZFUITextView)->layoutNativeTextView(this,
        ZFUISizeApplyScale(ZFUISizeApplyMargin(bounds.size, this->nativeImplViewMargin()), this->scaleFixed()));
}

// ============================================================
ZFLANGAPPLY_DEFINE(ZFUITextView, ZFUITextViewText, {obj->textSet(langValue);})

ZF_NAMESPACE_GLOBAL_END

