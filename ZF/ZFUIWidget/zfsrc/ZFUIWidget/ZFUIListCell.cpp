/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFUIListCell.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFSTYLE_DEFAULT_DEFINE(ZFUIListCell)

// ============================================================
ZFOBJECT_REGISTER(ZFUIListCell)
ZFPROPERTY_CUSTOM_ON_ATTACH_DEFINE(ZFUIListCell, ZFUIView *, cellView)
{
    if(this->cellView() != zfnull)
    {
        this->internalFgViewAdd(this->cellView(), this->cellViewLayoutParam());
    }
}
ZFPROPERTY_CUSTOM_ON_DETACH_DEFINE(ZFUIListCell, ZFUIView *, cellView)
{
    if(this->cellView() != zfnull)
    {
        this->internalFgViewRemove(this->cellView());
    }
}
void ZFUIListCell::layoutOnMeasure(ZF_OUT ZFUISize &ret,
                                   ZF_IN const ZFUISize &sizeHint,
                                   ZF_IN const ZFUISizeParam &sizeParam)
{
    if(this->cellView() == zfnull)
    {
        zfsuper::layoutOnMeasure(ret, sizeHint, sizeParam);
        return ;
    }

    this->cellView()->layoutMeasure(sizeHint, ZFUISizeParamWrapWrap());
    ret = this->cellView()->layoutMeasuredSize();
}

ZF_NAMESPACE_GLOBAL_END

