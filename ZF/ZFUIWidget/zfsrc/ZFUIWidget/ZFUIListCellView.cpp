/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFUIListCellView.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFOBJECT_REGISTER(ZFUIListCellView)

void ZFUIListCellView::objectOnInitFinish(void)
{
    zfsuper::objectOnInitFinish();

    zfint itemMargin = ZFUIGlobalStyle::DefaultStyle()->itemMargin();

    this->internalBgViewAdd(this->cellIconContainer());
    this->cellIconContainer()->layoutParam()->layoutAlignSet(ZFUIAlign::e_Center);
    this->cellIconContainer()->layoutParam()->layoutMarginSet(ZFUIMarginMake(itemMargin, itemMargin, 0, itemMargin));

    this->internalBgViewAdd(this->cellCenterContainer());
    this->cellCenterContainer()->layoutParam()->layoutAlignSet(ZFUIAlign::e_Center);
    this->cellCenterContainer()->layoutParam()->layoutMarginSet(ZFUIMarginMake(itemMargin));

    this->internalBgViewAdd(this->cellAccessoryContainer());
    this->cellAccessoryContainer()->layoutParam()->layoutAlignSet(ZFUIAlign::e_Center);
    this->cellAccessoryContainer()->layoutParam()->layoutMarginSet(ZFUIMarginMake(0, itemMargin, itemMargin, itemMargin));
}

static void _ZFP_ZFUIListCellView_measureContent(ZF_IN ZFUIListCellView *view,
                                                 ZF_IN const ZFUISize &sizeHint,
                                                 ZF_OUT ZFUISize &cellIconSize,
                                                 ZF_OUT ZFUISize &cellCenterSize,
                                                 ZF_OUT ZFUISize &cellAccessorySize)
{
    view->cellIconContainer()->layoutMeasure(sizeHint, ZFUISizeParamWrapWrap());
    cellIconSize = view->cellIconContainer()->layoutMeasuredSize();
    if(cellIconSize.width > 0)
    {
        cellIconSize = ZFUISizeApplyMarginReversely(cellIconSize, view->cellIconContainer()->layoutParam()->layoutMargin());
    }

    view->cellAccessoryContainer()->layoutMeasure(sizeHint, ZFUISizeParamWrapWrap());
    cellAccessorySize = view->cellAccessoryContainer()->layoutMeasuredSize();
    if(cellAccessorySize.width > 0)
    {
        cellAccessorySize = ZFUISizeApplyMarginReversely(cellAccessorySize, view->cellAccessoryContainer()->layoutParam()->layoutMargin());
    }

    ZFUISize cellCenterSizeHint = sizeHint;
    if(cellCenterSizeHint.width >= 0)
    {
        cellCenterSizeHint.width -= cellIconSize.width + cellAccessorySize.width;
        cellCenterSizeHint.width -= ZFUIMarginGetWidth(view->cellCenterContainer()->layoutParam()->layoutMargin());
        if(cellCenterSizeHint.width < 0)
        {
            cellCenterSizeHint.width = 0;
        }
    }
    view->cellCenterContainer()->layoutMeasure(cellCenterSizeHint, ZFUISizeParamFillWrap());
    cellCenterSize = view->cellCenterContainer()->layoutMeasuredSize();
    cellCenterSize.width += ZFUIMarginGetWidth(view->cellCenterContainer()->layoutParam()->layoutMargin());
}
void ZFUIListCellView::layoutOnMeasure(ZF_OUT ZFUISize &ret,
                                       ZF_IN const ZFUISize &sizeHint,
                                       ZF_IN const ZFUISizeParam &sizeParam)
{
    ZFUISize cellIconSize = ZFUISizeZero();
    ZFUISize cellCenterSize = ZFUISizeZero();
    ZFUISize cellAccessorySize = ZFUISizeZero();
    _ZFP_ZFUIListCellView_measureContent(this, sizeHint, cellIconSize, cellCenterSize, cellAccessorySize);
    ret.width = cellIconSize.width + cellCenterSize.width + cellAccessorySize.width;
    ret.height = zfmMax(zfmMax(cellIconSize.height, cellCenterSize.height), cellAccessorySize.height);
}
void ZFUIListCellView::internalBgViewOnLayout(ZF_IN const ZFUIRect &bounds)
{
    zfsuper::internalBgViewOnLayout(bounds);

    ZFUISize cellIconSize = ZFUISizeZero();
    ZFUISize cellCenterSize = ZFUISizeZero();
    ZFUISize cellAccessorySize = ZFUISizeZero();
    _ZFP_ZFUIListCellView_measureContent(this, bounds.size, cellIconSize, cellCenterSize, cellAccessorySize);

    this->cellIconContainer()->layout(ZFUIAlignApply(
        ZFUIAlign::e_LeftInner | ZFUIAlignGetY(this->cellIconContainer()->layoutParam()->layoutAlign()),
        bounds,
        this->cellIconContainer()->layoutMeasuredSize(),
        this->cellIconContainer()->layoutParam()->layoutMargin()));

    ZFUIMargin cellCenterMargin = this->cellCenterContainer()->layoutParam()->layoutMargin();
    cellCenterMargin.left += cellIconSize.width;
    cellCenterMargin.right += cellAccessorySize.width;
    this->cellCenterContainer()->layout(ZFUIAlignApply(
        ZFUIAlign::e_LeftInner | ZFUIAlignGetY(this->cellCenterContainer()->layoutParam()->layoutAlign()),
        bounds,
        this->cellCenterContainer()->layoutMeasuredSize(),
        cellCenterMargin));

    this->cellAccessoryContainer()->layout(ZFUIAlignApply(
        ZFUIAlign::e_RightInner | ZFUIAlignGetY(this->cellAccessoryContainer()->layoutParam()->layoutAlign()),
        bounds,
        this->cellAccessoryContainer()->layoutMeasuredSize(),
        this->cellAccessoryContainer()->layoutParam()->layoutMargin()));
}
zfbool ZFUIListCellView::internalViewShouldLayout(ZF_IN ZFUIView *internalView)
{
    if(internalView == this->cellIconContainer()
        || internalView == this->cellCenterContainer()
        || internalView == this->cellAccessoryContainer())
    {
        return zffalse;
    }
    else
    {
        return zfsuper::internalViewShouldLayout(internalView);
    }
}

ZF_NAMESPACE_GLOBAL_END

