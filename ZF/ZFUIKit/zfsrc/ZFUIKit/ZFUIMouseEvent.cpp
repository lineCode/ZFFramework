/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFUIMouseEvent.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFENUM_DEFINE(ZFUIMouseAction)
ZFENUM_DEFINE(ZFUIMouseButton)

ZFOBJECT_REGISTER(ZFUIMouseEvent)
ZFCACHEHOLDER_DEFINE(ZFUIMouseEvent)

void ZFUIMouseEvent::objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
{
    ret += ZFUIMouseAction::EnumNameForValue(this->mouseAction);
    ret += zfText(" ");
    ZFUIPointToString(ret, this->mousePoint);

    if(this->mouseButton != ZFUIMouseButton::e_MouseButtonLeft)
    {
        ret += zfText(" ");
        ret += ZFUIMouseButton::EnumNameForValue(this->mouseButton);
    }

    if(this->eventResolved())
    {
        ret += zfText(" (resolved)");
    }
}

void ZFUIMouseEvent::eventOnApplyScale(ZF_IN zffloat scale)
{
    zfsuper::eventOnApplyScale(scale);
    this->mousePoint = ZFUIPointApplyScale(this->mousePoint, scale);
}
void ZFUIMouseEvent::eventOnApplyScaleReversely(ZF_IN zffloat scale)
{
    zfsuper::eventOnApplyScale(scale);
    this->mousePoint = ZFUIPointApplyScaleReversely(this->mousePoint, scale);
}

// ============================================================
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFUIMouseEvent, zfidentity, mouseId)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFUIMouseEvent, ZFUIMouseActionEnum, mouseAction)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFUIMouseEvent, ZFUIPoint, mousePoint)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFUIMouseEvent, ZFUIMouseButtonEnum, mouseButton)

ZF_NAMESPACE_GLOBAL_END

