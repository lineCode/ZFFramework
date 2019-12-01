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
        ret->layoutOrientation(ZFUIOrientation::e_Top);
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

        this->pageAniResumeByRequest(pageAni = zflineAlloc(ZFAnimationNativeView));
        pageAni->aniTranslateXFrom(1);

        this->pageAniResumeFromBackground(pageAni = zflineAlloc(ZFAnimationNativeView));
        pageAni->aniTranslateXFrom(-1);

        this->pageAniPauseToBackground(pageAni = zflineAlloc(ZFAnimationNativeView));
        pageAni->aniTranslateXTo(-1);

        this->pageAniPauseBeforeDestroy(pageAni = zflineAlloc(ZFAnimationNativeView));
        pageAni->aniTranslateXTo(1);
    }
    zfoverride
    virtual void pageAniOnUpdateAniTarget(ZF_IN ZFAnimation *pageAni)
    {
        zfsuperI(ZFUIPage)::pageAniOnUpdateAniTarget(pageAni);
        pageAni->aniTarget(this->pageContainer());
    }
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIPageBasic_h_

