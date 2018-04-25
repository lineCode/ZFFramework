local args = {...};
local argsCount = #args;

local pm = args[1];

pm:observerAdd(ZFUIPage.EventPageOnResume(), ZFCallbackForLua(function(listenerData, userData)
        local page = listenerData:sender();
        local pm = page:pageManager();
        pm:tagGet("leftButton"):viewVisibleSet(pm:pageCount() > 1);
        pm:tagGet("centerButton"):buttonLabelTextSet(page:objectInfoOfInstance());
    end), pm:objectHolder());

_ZFP_ZFLua_app_test_setupPage = function(page)
        page:observerAdd(ZFUIPage.EventPageOnCreate(), ZFCallbackForLua(function(listenerData, userData)
                zfLog("page onCreate");
                local page = listenerData:sender();

                local pageView = ZFUIButtonBasic();
                page:pageContainer():childAdd(pageView, ZFUISizeParamFillFill());

                pageView:buttonLabelTextSet(page:objectInfoOfInstance());
                pageView:viewBackgroundColorSet(ZFUIColorRandom());
                pageView:observerAdd(ZFUIButton.EventButtonOnClick(), ZFCallbackForLua(function(listenerData, userData)
                        local page = userData:objectHolded();
                        page:pageManager():requestPageCreate(_ZFP_ZFLua_app_test_setupPage(ZFUIPageBasic()));
                    end), page:objectHolder());
            end));
        return page;
    end

pm:requestPageCreate(_ZFP_ZFLua_app_test_setupPage(ZFUIPageBasic()));
pm:pageAniOverrideForOnce(zfnull, zfnull);

