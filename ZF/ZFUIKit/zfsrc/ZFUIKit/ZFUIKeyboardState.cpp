/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFUIKeyboardState.h"
#include "protocol/ZFProtocolZFUIKeyboardState.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFUIKeyboardState)

ZFOBSERVER_EVENT_REGISTER(ZFUIKeyboardState, KeyPressedOnChange)

ZFOBJECT_SINGLETON_DEFINE_DETAIL(ZFUIKeyboardState, ZFUIKeyboardState, ZFUIKeyboardState, instance, ZFLevelZFFrameworkHigh)

ZFMETHOD_DEFINE_1(ZFUIKeyboardState, zfbool, keyPressed,
                  ZFMP_IN(ZFUIKeyCodeEnum, keyCode))
{
    ZFPROTOCOL_INTERFACE_CLASS(ZFUIKeyboardState) *impl = ZFPROTOCOL_TRY_ACCESS(ZFUIKeyboardState);
    if(impl != zfnull)
    {
        return impl->keyPressed(keyCode);
    }
    return zffalse;
}
ZFMETHOD_DEFINE_1(ZFUIKeyboardState, zfbool, keyPressedRaw,
                  ZFMP_IN(zfflags, keyCodeRaw))
{
    ZFPROTOCOL_INTERFACE_CLASS(ZFUIKeyboardState) *impl = ZFPROTOCOL_TRY_ACCESS(ZFUIKeyboardState);
    if(impl != zfnull)
    {
        return impl->keyPressedRaw(keyCodeRaw);
    }
    return zffalse;
}

void ZFUIKeyboardState::objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
{
    zfsuper::objectInfoOnAppend(ret);
    zfstring tmp;
    for(zfindex i = 0; i < ZFUIKeyCode::EnumCount(); ++i)
    {
        if(this->keyPressed((ZFUIKeyCodeEnum)ZFUIKeyCode::EnumValueAtIndex(i)))
        {
            if(!tmp.isEmpty())
            {
                tmp += ", ";
            }
            tmp += ZFUIKeyCode::EnumNameAtIndex(i);
        }
    }
    if(!tmp.isEmpty())
    {
        ret += ", pressed: ";
        ret += tmp;
    }
}

ZF_NAMESPACE_GLOBAL_END

