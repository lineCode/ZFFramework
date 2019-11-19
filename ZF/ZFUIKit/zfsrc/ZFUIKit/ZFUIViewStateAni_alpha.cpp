#include "ZFUIViewStateAni_alpha.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFUIViewStateAniSettingForAlpha)
ZFOBJECT_SINGLETON_DEFINE_DETAIL(ZFUIViewStateAniSettingForAlpha, ZFUIViewStateAniSettingForAlpha, ZFUIViewStateAniSettingForAlpha, instance, ZFLevelZFFrameworkHigh)

zfclass _ZFP_ZFUIViewStateAniImpl_alpha : zfextends ZFObject, zfimplements ZFUIViewStateAniImplProtocol
{
    ZFOBJECT_DECLARE(_ZFP_ZFUIViewStateAniImpl_alpha, ZFObject)
    ZFIMPLEMENTS_DECLARE(ZFUIViewStateAniImplProtocol)

public:
    zfoverride
    virtual void stateAniViewAniPrepare(ZF_IN ZFUIView *view, ZF_IN_OUT ZFArrayEditable *aniList)
    {
        zfsuperI(ZFUIViewStateAniImplProtocol)::stateAniViewAniPrepare(view, aniList);
        if(!aniList->isEmpty()) {return ;}

        ZFUIViewStateAniSettingForAlpha *setting = ZFUIViewStateAniSettingForAlpha::instance();
        if(!setting->aniEnable() || !setting->aniFilter.filterCheckActive(view)) {return ;}

        const ZFUIRect &viewFrame = view->viewFrame();
        const ZFUIRect &viewFramePrev = view->viewFramePrev();
        if((viewFramePrev.size.width != 0 && viewFramePrev.size.height != 0)
            || viewFrame.size.width <= setting->aniToleranceX()
            || viewFrame.size.height <= setting->aniToleranceY())
        {
            return ;
        }

        zfblockedAlloc(ZFAnimationNativeView, ani);
        aniList->add(ani);
        ani->aniTargetSet(view);

        ani->aniDurationSet(setting->aniDuration());
        ani->aniCurveSet(setting->aniCurve());
        ani->aniAlphaFromSet(0);
    }
};

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIViewStateAniImpl_alpha_register, ZFLevelZFFrameworkHigh)
{
    this->impl = zfAlloc(_ZFP_ZFUIViewStateAniImpl_alpha);
    ZFUIViewStateAniImplRegister(this->impl);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIViewStateAniImpl_alpha_register)
{
    ZFUIViewStateAniImplUnregister(this->impl);
    zfRelease(this->impl);
}
public:
    ZFUIViewStateAniImplProtocol *impl;
ZF_GLOBAL_INITIALIZER_END(ZFUIViewStateAniImpl_alpha_register)

ZF_NAMESPACE_GLOBAL_END

