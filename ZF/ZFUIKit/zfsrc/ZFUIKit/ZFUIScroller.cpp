/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFUIScroller.h"
#include "ZFUIScrollerDefault.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFUIScroller)

static const ZFClass *_ZFP_ZFUIScroller_cls = zfnull;
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIScrollerRegister, ZFLevelZFFrameworkHigh)
{
    _ZFP_ZFUIScroller_cls = ZFUIScrollerDefault::ClassData();
}
ZF_GLOBAL_INITIALIZER_END(ZFUIScrollerRegister)

ZFMETHOD_FUNC_DEFINE_1(void, ZFUIScrollerClassSet,
                       ZFMP_IN(const ZFClass *, cls))
{
    if(cls != zfnull && !cls->classIsTypeOf(ZFUIScroller::ClassData()))
    {
        zfCoreLog("class %s not type of %s",
            cls->classNameFull(),
            ZFUIScroller::ClassData()->classNameFull());
        return ;
    }

    if(cls == zfnull)
    {
        cls = ZFUIScrollerDefault::ClassData();
    }
    _ZFP_ZFUIScroller_cls = cls;
}
ZFMETHOD_FUNC_DEFINE_0(const ZFClass *, ZFUIScrollerClass)
{
    return _ZFP_ZFUIScroller_cls;
}

ZF_NAMESPACE_GLOBAL_END

