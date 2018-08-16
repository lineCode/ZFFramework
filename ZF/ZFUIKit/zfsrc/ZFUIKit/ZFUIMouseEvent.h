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
 * @file ZFUIMouseEvent.h
 * @brief mouse event for ui elements
 */

#ifndef _ZFI_ZFUIMouseEvent_h_
#define _ZFI_ZFUIMouseEvent_h_

#include "ZFUIEvent.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief mouse action for ZFUIMouseEvent
 */
ZFENUM_BEGIN(ZFUIMouseAction)
    ZFENUM_VALUE(MouseDown) /**< @brief mouse down */
    ZFENUM_VALUE(MouseMove) /**< @brief mouse move */
    ZFENUM_VALUE(MouseUp) /**< @brief mouse up */
    ZFENUM_VALUE(MouseCancel) /**< @brief mouse cancel */
    ZFENUM_VALUE(MouseHoverEnter) /**< @brief mouse hover enter */
    ZFENUM_VALUE(MouseHover) /**< @brief mouse hover */
    ZFENUM_VALUE(MouseHoverExit) /**< @brief mouse hover exit */
ZFENUM_SEPARATOR(ZFUIMouseAction)
    ZFENUM_VALUE_REGISTER(MouseDown)
    ZFENUM_VALUE_REGISTER(MouseMove)
    ZFENUM_VALUE_REGISTER(MouseUp)
    ZFENUM_VALUE_REGISTER(MouseCancel)
    ZFENUM_VALUE_REGISTER(MouseHoverEnter)
    ZFENUM_VALUE_REGISTER(MouseHover)
    ZFENUM_VALUE_REGISTER(MouseHoverExit)
ZFENUM_END(ZFUIMouseAction)

/**
 * @brief mouse action for ZFUIMouseEvent
 */
ZFENUM_BEGIN(ZFUIMouseButton)
    ZFENUM_VALUE(MouseButtonLeft) /**< @brief left button */
    ZFENUM_VALUE(MouseButtonRight) /**< @brief right button */
    ZFENUM_VALUE(MouseButtonCenter) /**< @brief center button */
ZFENUM_SEPARATOR(ZFUIMouseButton)
    ZFENUM_VALUE_REGISTER(MouseButtonLeft)
    ZFENUM_VALUE_REGISTER(MouseButtonRight)
    ZFENUM_VALUE_REGISTER(MouseButtonCenter)
ZFENUM_END(ZFUIMouseButton)

// ============================================================
/**
 * @brief mouse event for ui elements
 */
zffinal zfclass ZF_ENV_EXPORT ZFUIMouseEvent : zfextends ZFUIEvent
{
    ZFOBJECT_DECLARE_WITH_CUSTOM_CTOR(ZFUIMouseEvent, ZFUIEvent)

public:
    /**
     * @brief a unique value to differ mouse from each other
     *
     * mouseId keeps the same from mouse down/move/up/cancel,
     * another mouse down would renew the mouseId
     */
    zfidentity mouseId;
    /**
     * @brief mouse action
     */
    ZFUIMouseActionEnum mouseAction;
    /**
     * @brief mouse point
     */
    ZFUIPoint mousePoint;
    /**
     * @brief mouse button
     *
     * note, this value is invalid for mouse hover event,
     * and would always be #ZFUIMouseButton::e_MouseButtonLeft
     */
    ZFUIMouseButtonEnum mouseButton;

public:
    ZFCACHEHOLDER_DECLARE()

protected:
    zfoverride
    virtual void objectInfoOnAppend(ZF_IN_OUT zfstring &ret);

public:
    zfoverride
    virtual void eventOnApplyScale(ZF_IN zffloat scale);
    zfoverride
    virtual void eventOnApplyScaleReversely(ZF_IN zffloat scale);

protected:
    /** @cond ZFPrivateDoc */
    ZFUIMouseEvent(void)
    : mouseId(zfidentityInvalid())
    , mouseAction(ZFUIMouseAction::e_MouseDown)
    , mousePoint()
    , mouseButton(ZFUIMouseButton::e_MouseButtonLeft)
    {
    }
    /** @endcond */
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIMouseEvent_h_

