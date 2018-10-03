/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFUIViewBlink.h"
#include "ZFUIImageView.h"
#include "ZFAnimationNativeView.h"

#include "ZFUIKit/protocol/ZFProtocolZFAnimationNativeView.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
ZFOBSERVER_EVENT_GLOBAL_REGISTER(ViewBlinkOn)
ZFOBSERVER_EVENT_GLOBAL_REGISTER(ViewBlinkOff)
ZF_NAMESPACE_END(ZFGlobalEvent)

// ============================================================
#define _ZFP_ZFUIViewBlink_DEBUG_noAni 0
#define _ZFP_ZFUIViewBlink_DEBUG_duration 0
#define _ZFP_ZFUIViewBlink_DEBUG_color 0

#define _ZFP_ZFUIViewBlink_tag_ani "_ZFP_ZFUIViewBlink_tag_ani"
#define _ZFP_ZFUIViewBlink_tag_blinkView "_ZFP_ZFUIViewBlink_tag_blinkView"
#define _ZFP_ZFUIViewBlink_tag_delayTaskId "_ZFP_ZFUIViewBlink_tag_delayTaskId"
#define _ZFP_ZFUIViewBlink_tag_delayId "_ZFP_ZFUIViewBlink_tag_delayId"
#define _ZFP_ZFUIViewBlink_tag_blinkCountLeft "_ZFP_ZFUIViewBlink_tag_blinkCountLeft"

zfclass _ZFP_ZFUIViewBlinkView : zfextends ZFUIImageView
{
    ZFOBJECT_DECLARE(_ZFP_ZFUIViewBlinkView, ZFUIImageView)
};

static void _ZFP_ZFUIViewBlinkDoOn(ZF_IN ZFUIView *view, ZF_IN const ZFUIViewBlinkParam &blinkParam);
static void _ZFP_ZFUIViewBlinkDoOff(ZF_IN ZFUIView *view);
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIViewBlinkDataHolder, ZFLevelZFFrameworkEssential)
{
    this->viewOnDeallocListener = ZFCallbackForFunc(zfself::viewOnDealloc);
}
public:
    ZFListener viewOnDeallocListener;
    ZFCoreArrayPOD<ZFUIView *> blinkingViews;
    ZFIdentityGenerator delayTaskIdGenerator;
    static ZFLISTENER_PROTOTYPE_EXPAND(viewOnDealloc)
    {
        _ZFP_ZFUIViewBlinkDoOff(listenerData.sender->to<ZFUIView *>());
    }
ZF_GLOBAL_INITIALIZER_END(ZFUIViewBlinkDataHolder)

// ============================================================
ZFTYPEID_ACCESS_ONLY_DEFINE(ZFUIViewBlinkParam, ZFUIViewBlinkParam)

ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFUIViewBlinkParam, ZFUIImage * const &, blinkImage)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFUIViewBlinkParam, void, blinkImageSet, ZFMP_IN(ZFUIImage * const &, blinkImage))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFUIViewBlinkParam, zftimet const &, blinkDuration)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFUIViewBlinkParam, void, blinkDurationSet, ZFMP_IN(zftimet const &, blinkDuration))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFUIViewBlinkParam, zfindex const &, blinkCount)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFUIViewBlinkParam, void, blinkCountSet, ZFMP_IN(zfindex const &, blinkCount))

// ============================================================
ZFMETHOD_FUNC_DEFINE_2(void, ZFUIViewBlink,
                       ZFMP_IN(ZFUIView *, view),
                       ZFMP_IN_OPT(const ZFUIViewBlinkParam &, blinkParam, ZFUIViewBlinkParam()))
{
    if(blinkParam.blinkCount() <= 0)
    {
        return ;
    }
    if(view)
    {
        _ZFP_ZFUIViewBlinkDoOn(view, blinkParam);
    }
}
ZFMETHOD_FUNC_DEFINE_INLINE_4(void, ZFUIViewBlink,
                              ZFMP_IN(ZFUIView *, view),
                              ZFMP_IN(ZFUIImage *, blinkImage),
                              ZFMP_IN_OPT(zftimet, blinkDuration, zftimetZero()),
                              ZFMP_IN_OPT(zfindex, blinkCount, 1))

// ============================================================
// blink action
static void _ZFP_ZFUIViewBlink_noAni_doOff(ZF_IN ZFUIView *view);
static void _ZFP_ZFUIViewBlinkDoOn(ZF_IN ZFUIView *view, ZF_IN const ZFUIViewBlinkParam &blinkParam)
{
    _ZFP_ZFUIViewBlinkView *blinkView = view->tagGet<_ZFP_ZFUIViewBlinkView *>(_ZFP_ZFUIViewBlink_tag_blinkView);
    if(blinkView != zfnull)
    {
        return ;
    }

    {
        zfblockedAlloc(_ZFP_ZFUIViewBlinkView, t);
        view->internalFgViewAdd(t);
        view->tagSet(_ZFP_ZFUIViewBlink_tag_blinkView, t);
        t->layoutParam()->sizeParamSet(ZFUISizeParamFillFill());
        blinkView = t;
    }
    blinkView->imageSet((blinkParam.blinkImage() != zfnull)
        ? blinkParam.blinkImage()
        : ZFUIViewBlinkImageDefault().to<ZFUIImage *>());

    if(ZFPROTOCOL_IS_AVAILABLE(ZFAnimationNativeView) && !_ZFP_ZFUIViewBlink_DEBUG_noAni)
    {
        if(blinkParam.blinkCount() > 1)
        {
            view->tagSet(_ZFP_ZFUIViewBlink_tag_blinkCountLeft, ZFValueEditable::indexValueCreate(blinkParam.blinkCount() - 1).toObject());
        }

        zfblockedAlloc(ZFAnimationNativeView, ani);
        view->tagSet(_ZFP_ZFUIViewBlink_tag_ani, ani);
        ani->aniAlphaToSet(0);
        #if _ZFP_ZFUIViewBlink_DEBUG_duration
            ani->aniDurationSet(5000);
        #else
            ani->aniDurationSet(blinkParam.blinkDuration());
        #endif

        ZFLISTENER_LOCAL(aniOnStopListener, {
            ZFAnimation *ani = listenerData.sender->to<ZFAnimation *>();
            ZFUIView *blinkView = ani->aniTarget()->to<ZFUIView *>();
            ZFUIView *view = userData->objectHolded();

            ZFValueEditable *blinkCountLeft = view->tagGet<ZFValueEditable *>(_ZFP_ZFUIViewBlink_tag_blinkCountLeft);
            if(blinkCountLeft != zfnull)
            {
                if(blinkCountLeft->indexValue() <= 1)
                {
                    view->tagRemove(_ZFP_ZFUIViewBlink_tag_blinkCountLeft);
                }
                else
                {
                    blinkCountLeft->indexValueSet(blinkCountLeft->indexValue() - 1);
                }

                ani->aniStart();
                return ;
            }

            view->observerRemove(ZFObject::EventObjectBeforeDealloc(), ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewBlinkDataHolder)->viewOnDeallocListener);

            view->tagRemove(_ZFP_ZFUIViewBlink_tag_ani);
            view->tagRemove(_ZFP_ZFUIViewBlink_tag_blinkView);
            blinkView->viewRemoveFromParent();
            ani->aniTargetSet(zfnull);

            ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewBlinkDataHolder)->blinkingViews.removeElement(view);
            ZFGlobalEventCenter::instance()->observerNotifyWithCustomSender(view, ZFGlobalEvent::EventViewBlinkOff());
        })
        view->observerAdd(ZFObject::EventObjectBeforeDealloc(), ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewBlinkDataHolder)->viewOnDeallocListener);
        ani->observerAdd(ZFAnimation::EventAniOnStopOrOnInvalid(), aniOnStopListener, view->objectHolder());
        ani->aniTargetSet(blinkView);

        ZFGlobalEventCenter::instance()->observerNotifyWithCustomSender(view, ZFGlobalEvent::EventViewBlinkOn());
        ani->aniStart();
    }
    else
    {
        ZFLISTENER_LOCAL(blinkDelayOnFinish, {
            ZFUIView *view = userData->objectHolded();
            ZFValue *delayTaskId = listenerData.param0->to<ZFValue *>();
            ZFValue *delayTaskIdCur = view->tagGet<ZFValue *>(_ZFP_ZFUIViewBlink_tag_delayTaskId);
            if(delayTaskId != delayTaskIdCur)
            {
                return ;
            }

            ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewBlinkDataHolder)->delayTaskIdGenerator.idRelease(delayTaskId->identityValue());
            _ZFP_ZFUIViewBlink_noAni_doOff(view);
        })
        view->observerAdd(ZFObject::EventObjectBeforeDealloc(), ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewBlinkDataHolder)->viewOnDeallocListener);

        ZFGlobalEventCenter::instance()->observerNotifyWithCustomSender(view, ZFGlobalEvent::EventViewBlinkOn());
        zfidentity delayTaskId = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewBlinkDataHolder)->delayTaskIdGenerator.idAcquire();
        zfautoObject delayTaskIdTag = ZFValue::identityValueCreate(delayTaskId);
        view->tagSet(_ZFP_ZFUIViewBlink_tag_delayTaskId, delayTaskIdTag.toObject());
        zfidentity delayId = ZFThreadExecuteInMainThreadAfterDelay(
            #if _ZFP_ZFUIViewBlink_DEBUG_duration
                (zftimet)5000
            #else
                (zftimet)(blinkParam.blinkDuration() * blinkParam.blinkCount())
            #endif
            ,
            blinkDelayOnFinish,
            view->objectHolder(),
            ZFListenerData().param0Set(delayTaskIdTag.toObject())
            );
        view->tagSet(_ZFP_ZFUIViewBlink_tag_delayId, ZFValue::identityValueCreate(delayId).toObject());
    }
}
static void _ZFP_ZFUIViewBlink_noAni_doOff(ZF_IN ZFUIView *view)
{
    ZFUIView *blinkView = view->tagGet<ZFUIView *>(_ZFP_ZFUIViewBlink_tag_blinkView);

    view->tagRemove(_ZFP_ZFUIViewBlink_tag_blinkView);
    blinkView->viewRemoveFromParent();

    ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewBlinkDataHolder)->blinkingViews.removeElement(view);
    view->tagRemove(_ZFP_ZFUIViewBlink_tag_delayTaskId);
    view->tagRemove(_ZFP_ZFUIViewBlink_tag_delayId);
    ZFGlobalEventCenter::instance()->observerNotifyWithCustomSender(view, ZFGlobalEvent::EventViewBlinkOff());
}
static void _ZFP_ZFUIViewBlinkDoOff(ZF_IN ZFUIView *view)
{
    _ZFP_ZFUIViewBlinkView *blinkView = view->tagGet<_ZFP_ZFUIViewBlinkView *>(_ZFP_ZFUIViewBlink_tag_blinkView);
    if(blinkView == zfnull)
    {
        return ;
    }

    ZFAnimation *ani = view->tagGet<ZFAnimation *>(_ZFP_ZFUIViewBlink_tag_ani);
    if(ani != zfnull)
    {
        view->tagRemove(_ZFP_ZFUIViewBlink_tag_blinkCountLeft);
        ani->aniStop();
    }
    else
    {
        _ZFP_ZFUIViewBlink_noAni_doOff(view);
    }
}

// ============================================================
// other
ZFEXPORT_VAR_DEFINE(zfautoObject, ZFUIViewBlinkImageDefault, zfnull)
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIViewBlinkInitSetting, ZFLevelZFFrameworkNormal)
{
    (void)ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewBlinkDataHolder);
    #if _ZFP_ZFUIViewBlink_DEBUG_color
        ZFUIViewBlinkImageDefaultSet(ZFUIImageLoadFromColor(ZFUIColorMake(1, 0, 0)));
    #else
        ZFUIViewBlinkImageDefaultSet(zfRes("ZFUIKit/ZFUIViewBlinkImage.xml"));
    #endif
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIViewBlinkInitSetting)
{
    ZFUIViewBlinkImageDefaultSet(zfnull);
}
ZF_GLOBAL_INITIALIZER_END(ZFUIViewBlinkInitSetting)

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIViewBlinkCleanup, ZFLevelZFFrameworkLow)
{
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIViewBlinkCleanup)
{
    ZF_GLOBAL_INITIALIZER_CLASS(ZFUIViewBlinkDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewBlinkDataHolder);
    while(!d->blinkingViews.isEmpty())
    {
        _ZFP_ZFUIViewBlinkDoOff(d->blinkingViews[d->blinkingViews.count() - 1]);
    }
}
ZF_GLOBAL_INITIALIZER_END(ZFUIViewBlinkCleanup)

ZF_NAMESPACE_GLOBAL_END

