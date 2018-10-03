/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFFilterForZFMethod.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

static void _ZFP_ZFFilterForZFMethodGetSettingInfo(ZF_OUT zfstring &ret,
                                                   ZF_IN const ZFFilterForZFMethod *filter)
{
    ret += '(';
    zfindex flagsCount = 0;
    if(filter->ignorePublic())
    {
        ++flagsCount;
        ret += "ignorePublic, ";
    }
    if(filter->ignoreProtected())
    {
        ++flagsCount;
        ret += "ignoreProtected, ";
    }
    if(filter->ignorePrivate())
    {
        ++flagsCount;
        ret += "ignorePrivate, ";
    }

    if(flagsCount == 0)
    {
        ret.remove(ret.length() - 1);
    }
    else
    {
        ret.remove(ret.length() - 2);
        ret += ')';
    }
}
void ZFFilterForZFMethod::objectInfoT(ZF_IN_OUT zfstring &ret) const
{
    _ZFP_ZFFilterForZFMethodGetSettingInfo(ret, this);
    zfsuper::objectInfoT(ret);
}

zfbool ZFFilterForZFMethod::filterOnCheckActive(ZF_IN const ZFMethod * const &e) const
{
    if(!zfsuper::filterOnCheckActive(e)
       || !this->classFilter.filterCheckActive(e->methodOwnerClass()))
    {
        return zffalse;
    }

    if((e->methodPrivilegeType() == ZFMethodPrivilegeTypePrivate && this->ignorePrivate())
       || (e->methodPrivilegeType() == ZFMethodPrivilegeTypeProtected && this->ignoreProtected())
       || (e->methodPrivilegeType() == ZFMethodPrivilegeTypePublic && this->ignorePublic()))
    {
        return zffalse;
    }

    return zftrue;
}

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForZFMethod, void, copyFrom, ZFMP_IN(ZFFilterForZFMethod const &, ref))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFFilterForZFMethod, void, filterAdd, ZFMP_IN(const ZFMethod * const &, e), ZFMP_IN_OPT(ZFFilterType, filterType, ZFFilterTypeExclude))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFFilterForZFMethod, void, filterRemove, ZFMP_IN(const ZFMethod * const &, e), ZFMP_IN_OPT(ZFFilterType, filterType, ZFFilterTypeExclude))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForZFMethod, void, filterRemoveAtIndex, ZFMP_IN(zfindex, index))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFFilterForZFMethod, void, filterRemoveAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFFilterForZFMethod, zfindex, filterCount)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForZFMethod, const ZFMethod *, filterGet, ZFMP_IN(zfindex, index))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForZFMethod, ZFFilterType, filterGetFilterType, ZFMP_IN(zfindex, index))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForZFMethod, zfbool, filterCheckActive, ZFMP_IN(const ZFMethod * const &, e))

ZF_NAMESPACE_GLOBAL_END
#endif

