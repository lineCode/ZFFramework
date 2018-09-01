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
 * @file ZFUIListCellUpdaterBasic.h
 * @brief common updater of #ZFUIListCellUpdater
 */

#ifndef _ZFI_ZFUIListCellUpdaterBasic_h_
#define _ZFI_ZFUIListCellUpdaterBasic_h_

#include "ZFUIListCellUpdater.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief basic updater that insert separator to list cell
 */
zfclass ZF_ENV_EXPORT ZFUIListCellUpdaterBasic : zfextends ZFStyleableObject, zfimplements ZFUIListCellUpdater
{
    ZFOBJECT_DECLARE(ZFUIListCellUpdaterBasic, ZFStyleableObject)
    ZFIMPLEMENTS_DECLARE(ZFUIListCellUpdater)

public:
    /**
     * @brief separator size, 1 by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfint, separatorSize,
                                1)

    /**
     * @brief separator color, #ZFUIGlobalStyle::colorSeparator by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUIColor, separatorColor,
                                ZFUIGlobalStyle::DefaultStyle()->colorSeparator())

    /**
     * @brief separator margin, #ZFUIMarginZero by default
     *
     * note, for vertical list view, only left and right margin take effect,
     * and for horizontal list view, only top and bottom margin take effect
     */
    ZFPROPERTY_ASSIGN(ZFUIMargin, separatorMargin)

    /**
     * @brief whether add separator before first list cell, false by default
     */
    ZFPROPERTY_ASSIGN(zfbool, separatorIncludingHead)
    /**
     * @brief whether add separator after last list cell, false by default
     */
    ZFPROPERTY_ASSIGN(zfbool, separatorIncludingTail)

public:
    zfoverride
    virtual void cellOnUpdate(ZF_IN const ZFUIListCellUpdaterParam &updateParam);
    zfoverride
    virtual void cellOnRecycle(ZF_IN ZFUIListCell *cell);
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIListCellUpdaterBasic_h_

