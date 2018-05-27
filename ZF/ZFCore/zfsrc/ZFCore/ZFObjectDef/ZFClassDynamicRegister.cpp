/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFClassDynamicRegister.h"
#include "ZFObjectImpl.h"

#include "../ZFSTLWrapper/zfstl_map.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFClassDynamicRegisterAutoRemove, ZFLevelZFFrameworkHigh)
{
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFClassDynamicRegisterAutoRemove)
{
    if(!m.empty())
    {
        zfstlmap<const ZFClass *, zfbool> t;
        t.swap(m);
        for(zfstlmap<const ZFClass *, zfbool>::iterator it = t.begin(); it != t.end(); ++it)
        {
            ZFClass::_ZFP_ZFClassUnregister(zfnull, it->first);
        }
    }
}
zfstlmap<const ZFClass *, zfbool> m;
ZF_GLOBAL_INITIALIZER_END(ZFClassDynamicRegisterAutoRemove)

// ============================================================
const ZFClass *ZFClassDynamicRegister(ZF_IN const zfchar *className,
                                      ZF_IN const ZFClass *parent,
                                      ZF_IN_OPT ZFObject *classDynamicRegisterUserData /* = zfnull */,
                                      ZF_OUT_OPT zfstring *errorHint /* = zfnull */)
{
    if(parent == zfnull || parent->classIsAbstract() || zfsIsEmpty(className))
    {
        zfstringAppend(errorHint,
            zfText("invalid parent or className, parent: %s, className: %s"),
            parent ? parent->objectInfo().cString() : ZFTOKEN_zfnull,
            className);
        return zfnull;
    }
    const ZFClass *cls = ZFClass::classForName(className);
    if(cls != zfnull)
    {
        zfstringAppend(errorHint,
            zfText("class %s already exist while registering to parent: %s"),
            cls->objectInfo().cString(),
            parent->objectInfo().cString());
        return zfnull;
    }
    cls = ZFClass::_ZFP_ZFClassRegister(
        zfnull,
        className,
        parent,
        parent->_ZFP_objectConstructor(),
        parent->_ZFP_objectDestructor(),
        zfnull,
        zffalse,
        zftrue,
        classDynamicRegisterUserData);
    ZF_GLOBAL_INITIALIZER_INSTANCE(ZFClassDynamicRegisterAutoRemove)->m[cls] = zftrue;
    return cls;
}
void ZFClassDynamicUnregister(ZF_IN const ZFClass *cls)
{
    if(cls == zfnull)
    {
        return ;
    }
    if(!cls->classIsDynamicRegister())
    {
        zfCoreCriticalMessageTrim(
            zfTextA("[ZFClassDynamicRegister] unregistering class %s that is not dyanmiac registered"),
            zfsCoreZ2A(cls->objectInfo().cString()));
    }
    ZF_GLOBAL_INITIALIZER_INSTANCE(ZFClassDynamicRegisterAutoRemove)->m.erase(cls);
    ZFClass::_ZFP_ZFClassUnregister(zfnull, cls);
}

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_4(const ZFClass *, ZFClassDynamicRegister, ZFMP_IN(const zfchar *, className), ZFMP_IN(const ZFClass *, parent), ZFMP_IN_OPT(ZFObject *, classDynamicRegisterUserData, zfnull), ZFMP_OUT_OPT(zfstring *, errorHint, zfnull))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(void, ZFClassDynamicUnregister, ZFMP_IN(const ZFClass *, cls))

ZF_NAMESPACE_GLOBAL_END
#endif

