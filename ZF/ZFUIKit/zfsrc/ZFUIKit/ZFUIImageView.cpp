/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFUIImageView.h"
#include "protocol/ZFProtocolZFUIView.h"
#include "protocol/ZFProtocolZFUIImageView.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFSTYLE_DEFAULT_DEFINE(ZFUIImageView)

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIImageViewListenerHolder, ZFLevelZFFrameworkEssential)
{
    this->imageNinePatchChangedListener = ZFCallbackForFunc(zfself::imageNinePatchChanged);
}
public:
    ZFListener imageNinePatchChangedListener;
    static ZFLISTENER_PROTOTYPE_EXPAND(imageNinePatchChanged)
    {
        const ZFProperty *property = listenerData.param0->to<v_ZFProperty *>()->zfv;
        if(property == ZFPropertyAccess(ZFUIImage, imageNinePatch))
        {
            ZFUIImageView *imageView = userData->objectHolded();
            ZFUIImage *image = imageView->image();
            ZFPROTOCOL_ACCESS(ZFUIImageView)->imageNinePatchChanged(
                imageView,
                image->imageScaleFixed(),
                ZFUIMarginApplyScale(image->imageNinePatch(), image->imageScaleFixed()));
        }
    }
ZF_GLOBAL_INITIALIZER_END(ZFUIImageViewListenerHolder)

// ============================================================
// ZFUIImageView
ZFOBJECT_REGISTER(ZFUIImageView)

ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFUIImageView, ZFUIImage *, image)
{
    ZF_GLOBAL_INITIALIZER_CLASS(ZFUIImageViewListenerHolder) *listenerHolder = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIImageViewListenerHolder);
    if(this->image() != zfnull)
    {
        this->image()->observerAdd(ZFObject::EventObjectPropertyValueOnUpdate(), listenerHolder->imageNinePatchChangedListener, this->objectHolder());
    }

    ZFPROTOCOL_ACCESS(ZFUIImageView)->imageSet(this,
        this->image() && this->image()->nativeImage()
            ? this->image()
            : zfnull);
    ZFUIImage *image = this->image();
    if(image != zfnull)
    {
        ZFPROTOCOL_ACCESS(ZFUIImageView)->imageNinePatchChanged(
            this,
            image->imageScaleFixed(),
            ZFUIMarginApplyScale(image->imageNinePatch(), image->imageScaleFixed()));
    }
    this->layoutRequest();
}
ZFPROPERTY_OVERRIDE_ON_DETACH_DEFINE(ZFUIImageView, ZFUIImage *, image)
{
    ZF_GLOBAL_INITIALIZER_CLASS(ZFUIImageViewListenerHolder) *listenerHolder = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIImageViewListenerHolder);
    if(this->image() != zfnull)
    {
        this->image()->observerRemove(ZFObject::EventObjectPropertyValueOnUpdate(), listenerHolder->imageNinePatchChangedListener);
    }
}

ZFMETHOD_DEFINE_1(ZFUIImageView, void, measureImageView,
                  ZFMP_OUT(ZFUISize &, ret))
{
    ret = ((this->image() != zfnull) ? this->image()->imageSize() : ZFUISizeZero());
    ZFUISizeApplyMarginReversely(ret, ret, this->imageMargin());
}

void ZFUIImageView::objectOnInit(void)
{
    zfsuper::objectOnInit();

    zfCoreAssert(this->nativeImplView() == zfnull);
    zfclassNotPOD _ZFP_ZFUIImageView_nativeImplViewDestroy
    {
    public:
        static void action(ZF_IN ZFUIView *view,
                           ZF_IN void *nativeImplView)
        {
            ZFPROTOCOL_ACCESS(ZFUIImageView)->nativeImageViewDestroy(view->to<ZFUIImageView *>(), nativeImplView);
        }
    };
    this->nativeImplViewSet(
        ZFPROTOCOL_ACCESS(ZFUIImageView)->nativeImageViewCreate(this),
        _ZFP_ZFUIImageView_nativeImplViewDestroy::action);
}
void ZFUIImageView::objectOnDealloc(void)
{
    this->imageSet(zfnull);
    zfsuper::objectOnDealloc();
}

void ZFUIImageView::objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
{
    zfsuper::objectInfoOnAppend(ret);
    if(this->image() != zfnull)
    {
        ret += zfText(" ");
        this->image()->objectInfoT(ret);
    }
}

void ZFUIImageView::nativeImplViewMarginImplUpdate(ZF_IN_OUT ZFUIMargin &nativeImplViewMargin)
{
    zfsuper::nativeImplViewMarginImplUpdate(nativeImplViewMargin);
    ZFUIMarginInc(nativeImplViewMargin, nativeImplViewMargin, this->imageMargin());
}
void ZFUIImageView::nativeImplViewOnLayout(ZF_OUT ZFUIRect &ret,
                                           ZF_IN const ZFUIRect &bounds,
                                           ZF_IN const ZFUIMargin &nativeImplViewMargin)
{
    ZFUIContentScaleTypeApply(
        ret,
        this->imageScaleType(),
        ZFUIRectApplyMargin(bounds, nativeImplViewMargin),
        this->image() ? this->image()->imageSize() : ZFUISizeZero());
}
void ZFUIImageView::layoutOnMeasure(ZF_OUT ZFUISize &ret,
                                    ZF_IN const ZFUISize &sizeHint,
                                    ZF_IN const ZFUISizeParam &sizeParam)
{
    this->measureImageView(ret);
}

ZF_NAMESPACE_GLOBAL_END

