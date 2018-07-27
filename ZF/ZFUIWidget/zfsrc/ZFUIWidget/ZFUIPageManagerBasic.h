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
 * @file ZFUIPageManagerBasic.h
 * @brief basic page manager
 */

#ifndef _ZFI_ZFUIPageManagerBasic_h_
#define _ZFI_ZFUIPageManagerBasic_h_

#include "ZFUIPageManager.h"
#include "ZFUIPageBasic.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief basic page manager
 *
 * minimum steps to use the basic page manager:
 * -# during #ZFUIPageManager::managerOnCreate: (either by override or by event observer)
 *   -  create your manager's view tree
 *   -  setup #ZFUIPageManagerBasic::pageContainer
 *   -  prepare your home page, and create it by #ZFUIPageManager::requestPageCreate
 * -# call #embededCreateWindow to create window and attach
 * -# finally, show the window
 */
zfclass ZFUIPageManagerBasic : zfextends ZFObject, zfimplements ZFUIPageManager
{
    ZFOBJECT_DECLARE(ZFUIPageManagerBasic, ZFObject)
    ZFIMPLEMENTS_DECLARE(ZFUIPageManager)

private:
    static zfautoObject _ZFP_managerContainerInit(void)
    {
        zfblockedAlloc(ZFUILinearLayout, ret);
        ret->layoutOrientationSet(ZFUIOrientation::e_Top);
        return ret;
    }
public:
    /**
     * @brief the manager container as view
     *
     * by default, a manager use #managerContainer as view to hold the page manage logic,
     * you should manually add the container to view tree,
     * and manually manage the manager's life cycle by #ZFUIPageManager::embededCreate series,
     * this was designed for most compatibility and flexibility\n
     * \n
     * for most case, it's common to use #ZFUIWindow to hold the manager,
     * so we also have an util method #embededCreateWindow, which would:
     * -  create a #ZFUIWindow
     * -  attach #managerContainer to the window
     * -  call #embededCreate
     * -  when the window shows, call #embededResume
     * -  when the window hides, call #embededPause
     * -  when the window destroy, call #embededDestroy
     * -  when manager destroy, detach from the window, then hide the window
     */
    ZFPROPERTY_RETAIN_READONLY(ZFUILinearLayout *, managerContainer, _ZFP_managerContainerInit())
    /**
     * @brief the page container,
     *   must be set during manager's #ZFUIPageManager::managerOnCreate
     */
    ZFPROPERTY_RETAIN(ZFUIView *, pageContainer)
    /**
     * @brief create a #ZFUIWindow to hold the manager for short,
     *   see #managerContainer for more info
     */
    ZFMETHOD_DECLARE_0(zfautoObject, embededCreateWindow)

protected:
    zfoverride
    virtual void managerUIBlockedOnChange(void)
    {
        zfsuperI(ZFUIPageManager)::managerUIBlockedOnChange();
        this->managerContainer()->viewUIEnableTreeSet(!this->managerUIBlocked());
    }
    zfoverride
    virtual void pageOnAttach(ZF_IN ZFUIPage *page,
                              ZF_IN ZFUIPageResumeReasonEnum reason)
    {
        zfsuperI(ZFUIPageManager)::pageOnAttach(page, reason);

        zfCoreAssertWithMessageTrim(this->pageContainer() != zfnull,
            zfText("pageContainer not set"));
        ZFUIPageBasic *pageTmp = page->toAny();
        this->pageContainer()->childAdd(pageTmp->pageContainer(), ZFUISizeParamFillFill());
    }
    zfoverride
    virtual void pageOnDetach(ZF_IN ZFUIPage *page,
                              ZF_IN ZFUIPagePauseReasonEnum reason)
    {
        zfsuperI(ZFUIPageManager)::pageOnDetach(page, reason);

        ZFUIPageBasic *pageTmp = page->toAny();
        this->pageContainer()->childRemove(pageTmp->pageContainer());
    }
    zfoverride
    virtual void pageAniOnUpdatePriority(ZF_IN ZFUIPage *resumePageOrNull,
                                         ZF_IN ZFUIPage *pausePageOrNull,
                                         ZF_IN zfbool pausePageHasHigherPriority)
    {
        zfsuperI(ZFUIPageManager)::pageAniOnUpdatePriority(resumePageOrNull, pausePageOrNull, pausePageHasHigherPriority);

        ZFUIPageBasic *pageTmp = zfnull;
        if(pausePageHasHigherPriority)
        {
            if(pausePageOrNull != zfnull)
            {
                pageTmp = pausePageOrNull->toAny();
            }
        }
        else
        {
            if(resumePageOrNull != zfnull)
            {
                pageTmp = resumePageOrNull->toAny();
            }
        }

        if(pageTmp != zfnull)
        {
            zfCoreAssertWithMessageTrim(this->pageContainer() != zfnull,
                zfText("pageContainer not set"));
            this->pageContainer()->childMove(pageTmp->pageContainer(), zfindexMax());
        }
    }
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIPageManagerBasic_h_

