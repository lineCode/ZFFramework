#include "ZFUIViewStateAni_move.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFUIViewStateAniSettingForMove)
ZFOBJECT_SINGLETON_DEFINE_DETAIL(ZFUIViewStateAniSettingForMove, ZFUIViewStateAniSettingForMove, ZFUIViewStateAniSettingForMove, instance, ZFLevelZFFrameworkHigh)

zfclass _ZFP_ZFUIViewStateAniImpl_move : zfextends ZFObject, zfimplements ZFUIViewStateAniImplProtocol
{
    ZFOBJECT_DECLARE(_ZFP_ZFUIViewStateAniImpl_move, ZFObject)
    ZFIMPLEMENTS_DECLARE(ZFUIViewStateAniImplProtocol)

public:
    zfoverride
    virtual void stateAniViewAniPrepare(ZF_IN ZFUIView *view, ZF_IN_OUT ZFArrayEditable *aniList)
    {
        zfsuperI(ZFUIViewStateAniImplProtocol)::stateAniViewAniPrepare(view, aniList);
        if(!aniList->isEmpty()) {return ;}

        ZFUIViewStateAniSettingForMove *setting = ZFUIViewStateAniSettingForMove::instance();
        if(!setting->aniEnable() || !setting->aniFilter.filterCheckActive(view)) {return ;}

        const ZFUIRect &viewFramePrev = view->viewFramePrev();
        if(viewFramePrev.size.width == 0 || viewFramePrev.size.height == 0)
        {
            return ;
        }
        const ZFUIRect &viewFrame = view->viewFrame();
        zfint aniTolerance = setting->aniTolerance();
        if(zfmAbs(viewFrame.point.x - viewFramePrev.point.x) <= aniTolerance
                && zfmAbs(viewFrame.point.y - viewFramePrev.point.y) <= aniTolerance
                && zfmAbs(viewFrame.size.width - viewFramePrev.size.width) <= aniTolerance * 2
                && zfmAbs(viewFrame.size.height - viewFramePrev.size.height) <= aniTolerance * 2
            )
        {
            return ;
        }

        zfblockedAlloc(ZFAnimationNativeView, ani);
        aniList->add(ani);
        ani->aniTarget(view);

        ani->aniDuration(setting->aniDuration());
        ani->aniCurve(setting->aniCurve());

        if(zfmAbs(viewFrame.size.width - viewFramePrev.size.width) > aniTolerance * 2 && viewFrame.size.width > 0)
        {
            ani->aniScaleXFrom((zffloat)viewFramePrev.size.width / viewFrame.size.width);
        }
        if(zfmAbs(viewFrame.size.height - viewFramePrev.size.height) > aniTolerance * 2 && viewFrame.size.height > 0)
        {
            ani->aniScaleYFrom((zffloat)viewFramePrev.size.height / viewFrame.size.height);
        }

        ZFUIPoint from = ZFUIRectGetCenter(viewFramePrev);
        ZFUIPoint to = ZFUIRectGetCenter(viewFrame);
        if(zfmAbs(from.x - to.x) > aniTolerance)
        {
            ani->aniTranslatePixelXFrom((zfint)((from.x - to.x) / ani->aniScaleXFrom()));
        }
        if(zfmAbs(from.y - to.y) > aniTolerance)
        {
            ani->aniTranslatePixelYFrom((zfint)((from.y - to.y) / ani->aniScaleYFrom()));
        }
    }
};

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIViewStateAniImpl_move_register, ZFLevelZFFrameworkHigh)
{
    this->impl = zfAlloc(_ZFP_ZFUIViewStateAniImpl_move);
    ZFUIViewStateAniImplRegister(this->impl);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIViewStateAniImpl_move_register)
{
    ZFUIViewStateAniImplUnregister(this->impl);
    zfRelease(this->impl);
}
public:
    ZFUIViewStateAniImplProtocol *impl;
ZF_GLOBAL_INITIALIZER_END(ZFUIViewStateAniImpl_move_register)

ZF_NAMESPACE_GLOBAL_END

