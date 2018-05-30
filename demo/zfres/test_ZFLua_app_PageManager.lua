local pm = ZFUIPageManagerBasic();

pm:observerAdd(ZFUIPageManager.EventManagerOnCreate(), function(listenerData, userData)
        zfLog("manager onCreate");
        local pm = listenerData:sender();

        local managerContainer = pm:managerContainer();

        local topView = ZFUILinearLayout();
        pm:tagSet("topView", topView);
        managerContainer:childAdd(topView, ZFUISizeParamFillWrap());
        topView:layoutOrientationSet(ZFUIOrientation.e_Left());

        local leftButton = ZFUIButtonBasic();
        pm:tagSet("leftButton", leftButton);
        topView:childAdd(leftButton, ZFUISizeParamFillWrap(), ZFUIAlign.e_Center());
        leftButton:layoutParam():layoutWeightSet(1);
        leftButton:buttonLabelTextSet("Back");
        leftButton:viewBackgroundColorSet(ZFUIColorRandom());
        leftButton:layoutParam():layoutReserveSpaceWhenNotVisibleSet(zftrue);
        leftButton:viewVisibleSet(zffalse);
        leftButton:observerAdd(ZFUIButton.EventButtonOnClick(), function(listenerData, userData)
                local pm = userData:objectHolded();
                pm:pageAtIndex(pm:pageCount() - 1):pageDestroy();
            end, pm:objectHolder());

        local centerButton = ZFUIButtonBasic();
        pm:tagSet("centerButton", centerButton);
        topView:childAdd(centerButton, ZFUISizeParamFillWrap(), ZFUIAlign.e_Center());
        centerButton:layoutParam():layoutWeightSet(2);
        centerButton:viewBackgroundColorSet(ZFUIColorRandom());

        local rightButton = ZFUIButtonBasic();
        pm:tagSet("rightButton", rightButton);
        topView:childAdd(rightButton, ZFUISizeParamFillWrap(), ZFUIAlign.e_Center());
        rightButton:layoutParam():layoutWeightSet(1);
        rightButton:buttonLabelTextSet("ExitTest");
        rightButton:viewBackgroundColorSet(ZFUIColorRandom());
        rightButton:observerAdd(ZFUIButton.EventButtonOnClick(), function(listenerData, userData)
                local pm = userData:objectHolded();
                pm:embededPause();
                pm:embededDestroy();
            end, pm:objectHolder());

        local pageContainer = ZFUIView();
        pm:tagSet("pageContainer", pageContainer);
        managerContainer:childAdd(pageContainer, ZFUISizeParamFillFill());
        pm:pageContainerSet(pageContainer);
    end);

return pm;

