/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
/**
 * @file ZFUIPageBasic.h
 * @brief basic page
 */

#ifndef _ZFI_ZFUIPageBasic_h_
#define _ZFI_ZFUIPageBasic_h_

#include "ZFUIPage.h"
#include "ZFUILinearLayout.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief basic page, see #ZFUIPageManagerBasic for how to use
 */
zfclass ZFUIPageBasic : zfextends ZFObject, zfimplements ZFUIPage
{
    ZFOBJECT_DECLARE(ZFUIPageBasic, ZFObject)
    ZFIMPLEMENTS_DECLARE(ZFUIPage)

private:
    static zfautoObject _ZFP_pageContainerInit(void)
    {
        zfblockedAlloc(ZFUILinearLayout, ret);
        ret->layoutOrientationSet(ZFUIOrientation::e_Top);
        return ret;
    }
public:
    /**
     * @brief the page view, also used as page ani target
     */
    ZFPROPERTY_RETAIN_READONLY(ZFUILinearLayout *, pageContainer, _ZFP_pageContainerInit())

protected:
    zfoverride
    virtual void pageAniOnInit(void)
    {
        ZFAnimationNativeView *pageAni = zfnull;

        this->pageAniResumeByRequestSet(pageAni = zflineAlloc(ZFAnimationNativeView));
        pageAni->aniTranslateXFromSet(1);

        this->pageAniResumeFromBackgroundSet(pageAni = zflineAlloc(ZFAnimationNativeView));
        pageAni->aniTranslateXFromSet(-1);

        this->pageAniPauseToBackgroundSet(pageAni = zflineAlloc(ZFAnimationNativeView));
        pageAni->aniTranslateXToSet(-1);

        this->pageAniPauseBeforeDestroySet(pageAni = zflineAlloc(ZFAnimationNativeView));
        pageAni->aniTranslateXToSet(1);
    }
    zfoverride
    virtual void pageAniOnUpdateAniTarget(ZF_IN ZFAnimation *pageAni)
    {
        zfsuperI(ZFUIPage)::pageAniOnUpdateAniTarget(pageAni);
        pageAni->aniTargetSet(this->pageContainer());
    }
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIPageBasic_h_

