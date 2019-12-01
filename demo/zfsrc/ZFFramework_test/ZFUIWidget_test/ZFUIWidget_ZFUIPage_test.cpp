#include "ZFUIWidget_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFUIWidget_ZFUIPage_test_Page : zfextends ZFUIPageBasic
{
    ZFOBJECT_DECLARE(ZFUIWidget_ZFUIPage_test_Page, ZFUIPageBasic)

    ZFPROPERTY_RETAIN_WITH_INIT(ZFUILinearLayout *, _titleView, zflineAlloc(ZFUILinearLayout))
    ZFPROPERTY_RETAIN_WITH_INIT(ZFUIButtonBasic *, _titleLeftView, zflineAlloc(ZFUIButtonBasic))
    ZFPROPERTY_RETAIN_WITH_INIT(ZFUITextView *, _titleCenterView, zflineAlloc(ZFUITextView))
    ZFPROPERTY_RETAIN_WITH_INIT(ZFUIButtonBasic *, _contentView, zflineAlloc(ZFUIButtonBasic))

protected:
    zfoverride
    virtual void pageOnCreate(void)
    {
        zfsuper::pageOnCreate();

        this->pageContainer()->childAdd(this->_titleView());
        this->_titleView()->layoutParam()->sizeParam(ZFUISizeParamFillWrap());
        this->_titleView()->viewBackgroundColor(ZFUIColorRandom());
        this->_titleView()->layoutOrientation(ZFUIOrientation::e_Left);
        this->_titleView()->viewSizeMin(ZFUISizeMake(ZFUIGlobalStyle::DefaultStyle()->itemSizeButton()));

        this->_titleView()->childAdd(this->_titleLeftView());
        this->_titleLeftView()->layoutParam<ZFUILinearLayoutParam *>()->layoutWeight(1);
        this->_titleLeftView()->buttonLabelText("back");
        this->_titleLeftView()->viewBackgroundColor(ZFUIColorRandom());
        {
            ZFLISTENER_LOCAL(_titleLeftViewOnClick, {
                userData->objectHolded<ZFUIPage *>()->pageDestroy();
            })
            this->_titleLeftView()->observerAdd(ZFUIButton::EventButtonOnClick(), _titleLeftViewOnClick, this->objectHolder());
        }

        this->_titleView()->childAdd(this->_titleCenterView());
        this->_titleCenterView()->layoutParam<ZFUILinearLayoutParam *>()->layoutWeight(3);
        this->_titleCenterView()->text(zfstringWithFormat("page %p", this));
        this->_titleCenterView()->textAlign(ZFUIAlign::e_Center);

        this->pageContainer()->childAdd(this->_contentView());
        this->_contentView()->layoutParam()->sizeParam(ZFUISizeParamFillFill());
        this->_contentView()->viewBackgroundColor(ZFUIColorRandom());
        this->_contentView()->buttonLabelText(zfstringWithFormat("belong to %s", this->pageGroupId().cString()));
        {
            ZFLISTENER_LOCAL(_contentViewOnClick, {
                ZFUIPage *page = userData->objectHolded();
                page->pageManager()->requestPageCreate(ZFUIPageRequestPageCreateParam()
                    .page(zflineAlloc(zfself))
                    .pageGroupId(page->pageGroupId()));
            })
            this->_contentView()->observerAdd(ZFUIButton::EventButtonOnClick(), _contentViewOnClick, this->objectHolder());
        }
    }
    static ZFCompareResult pageFindFirstByPageGroupId(ZF_IN ZFUIPage * const &e0,
                                                      ZF_IN const zfchar * const &e1)
    {
        if(zfscmpTheSame(e0->pageGroupId().cString(), e1))
        {
            return ZFCompareTheSame;
        }
        return ZFCompareUncomparable;
    }
    zfoverride
    virtual void pageOnResume(ZF_IN ZFUIPageResumeReasonEnum reason)
    {
        zfsuper::pageOnResume(reason);
        zfindex index = this->pageManager()->pageList().find(this->pageGroupId().cString(), zfself::pageFindFirstByPageGroupId);
        if(index != zfindexMax() && this->pageManager()->pageAtIndex(index) != this)
        {
            this->_titleLeftView()->viewVisible(zftrue);
        }
        else
        {
            this->_titleLeftView()->viewVisible(zffalse);
        }
    }
};
zfclass ZFUIWidget_ZFUIPage_test_PageManager : zfextends ZFUIPageManagerBasic
{
    ZFOBJECT_DECLARE(ZFUIWidget_ZFUIPage_test_PageManager, ZFUIPageManagerBasic)

    ZFPROPERTY_RETAIN_WITH_INIT(ZFUIView *, _pageContainer, zflineAlloc(ZFUIView))
    ZFPROPERTY_RETAIN_WITH_INIT(ZFUILinearLayout *, _buttonLayout, zflineAlloc(ZFUILinearLayout))

protected:
    zfoverride
    virtual void managerOnCreate(void)
    {
        zfsuper::managerOnCreate();

        this->pageContainer(this->_pageContainer());

        this->managerContainer()->childAdd(this->_pageContainer());
        this->_pageContainer()->layoutParam()->sizeParam(ZFUISizeParamFillFill());
        this->_pageContainer()->layoutParam<ZFUILinearLayoutParam *>()->layoutWeight(1);

        this->managerContainer()->childAdd(this->_buttonLayout());
        this->_buttonLayout()->layoutParam()->sizeParam(ZFUISizeParamFillWrap());
        this->_buttonLayout()->layoutOrientation(ZFUIOrientation::e_Left);

        for(zfindex i = 0; i < 4; ++i)
        {
            zfblockedAlloc(v_zfstring, pageGroupId, zfstringWithFormat("pageGroup %zi", i));

            this->requestPageCreate(ZFUIPageRequestPageCreateParam()
                .page(zflineAlloc(ZFUIWidget_ZFUIPage_test_Page))
                .pageGroupId(pageGroupId->zfv)
                .pageAutoResume(zffalse));

            zfblockedAlloc(ZFUIButtonBasic, button);
            this->_buttonLayout()->childAdd(button);
            button->layoutParam<ZFUILinearLayoutParam *>()->layoutWeight(1);
            button->buttonLabelText(zfstringWithFormat("tab %zi", i));
            button->buttonCheckable(zftrue);
            ZFLISTENER_LOCAL(buttonOnClick, {
                ZFUIWidget_ZFUIPage_test_PageManager *pageManager = userData->objectTag("pageManager")->objectHolded();
                v_zfstring *pageGroupId = userData->objectTag<v_zfstring *>("pageGroupId");

                if(!listenerData.sender<ZFUIButton *>()->buttonChecked())
                {
                    listenerData.sender<ZFUIButton *>()->buttonChecked(zftrue);

                    zfbool first = zftrue;
                    for(zfindex i = 0; i < pageManager->pageCount(); ++i)
                    {
                        if(pageManager->pageAtIndex(i)->pageGroupId().compare(pageGroupId->zfv) == 0)
                        {
                            if(first)
                            {
                                first = zffalse;
                            }
                            else
                            {
                                pageManager->pageAtIndex(i)->pageDestroy();
                            }
                        }
                    }
                }
                else
                {
                    for(zfindex i = 0; i < pageManager->_buttonLayout()->childCount(); ++i)
                    {
                        ZFUIButton *button = pageManager->_buttonLayout()->childAtIndex(i)->toAny();
                        button->buttonChecked(zffalse);
                    }
                    listenerData.sender<ZFUIButton *>()->buttonChecked(zftrue);

                    zfblockedAlloc(ZFAnimationNativeView, resumeAni);
                    resumeAni->aniScaleXFrom(0.8f);
                    resumeAni->aniScaleYFrom(0.8f);
                    resumeAni->aniAlphaFrom(0.5f);
                    zfblockedAlloc(ZFAnimationNativeView, pauseAni);
                    pauseAni->aniAlphaTo(0);
                    pageManager->pageAniOverrideForOnce(resumeAni, pauseAni);

                    pageManager->requestPageGroupResume(pageGroupId->zfv);
                }
            })
            zfblockedAlloc(ZFObject, userData);
            userData->objectTag("pageManager", this->objectHolder());
            userData->objectTag("pageGroupId", pageGroupId);
            button->observerAdd(ZFUIButton::EventButtonOnClick(), buttonOnClick, userData);
            zffloat r = zfmRand(255) / 255.0f;
            zffloat g = zfmRand(255) / 255.0f;
            zffloat b = zfmRand(255) / 255.0f;
            button->buttonBackgroundStyleNormal()->image(ZFUIImageLoadFromColor(ZFUIColorMake(r, g, b, 0.25f)));
            button->buttonBackgroundStyleHighlighted()->image(ZFUIImageLoadFromColor(ZFUIColorMake(r, g, b)));
        }
        this->pageAniOverrideForOnce(zfnull, zfnull);
        this->_buttonLayout()->childAtIndex(0)->to<ZFUIButton *>()->buttonChecked(zftrue);
    }
};

zfclass ZFUIWidget_ZFUIPage_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFUIWidget_ZFUIPage_test, ZFFramework_test_TestCase)

private:
    ZFUIPageManagerBasic *_pageManager;

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUIView);
        ZFFramework_test_protocolCheck(ZFAnimationNativeView);
        ZFUIWindow *window = zfnull;
        ZFUIView *container = zfnull;
        ZFUIKit_test_prepareTestWindow(window, container, this);

        this->_pageManager = zfAlloc(ZFUIWidget_ZFUIPage_test_PageManager);
        this->_pageManager->embededCreate();
        container->childAdd(this->_pageManager->managerContainer(), ZFUISizeParamFillFill());
        this->_pageManager->managerContainer()->layoutParam()->layoutMargin(ZFUIMarginMake(20));
        this->_pageManager->embededResume();

        this->prepareSettingButton(window, this->_pageManager);
    }
    zfoverride
    virtual void testCaseOnStop(ZF_IN ZFResultTypeEnum testCaseResult)
    {
        this->_pageManager->embededPause();
        this->_pageManager->embededDestroy();
        zfRelease(this->_pageManager);
        this->_pageManager = zfnull;
        zfsuper::testCaseOnStop(testCaseResult);
    }

private:
    void prepareSettingButton(ZF_IN ZFUIWindow *window,
                              ZF_IN ZFUIPageManagerBasic *pageManager)
    {
        zfblockedAlloc(ZFArrayEditable, settings);

        ZFUIKit_test_prepareSettingButtonWithTestWindow(window, settings);
    }
};
ZFOBJECT_REGISTER(ZFUIWidget_ZFUIPage_test)

ZF_NAMESPACE_GLOBAL_END

