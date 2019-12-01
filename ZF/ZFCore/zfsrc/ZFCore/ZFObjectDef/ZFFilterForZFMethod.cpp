#include "ZFFilterForZFMethod.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

void ZFFilterForZFMethod::objectInfoT(ZF_IN_OUT zfstring &ret) const
{
    ret += '(';
    zfindex flagsCount = 0;
    if(this->ignorePublic())
    {
        ++flagsCount;
        ret += "ignorePublic, ";
    }
    if(this->ignoreProtected())
    {
        ++flagsCount;
        ret += "ignoreProtected, ";
    }
    if(this->ignorePrivate())
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
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForZFMethod, const ZFMethod *, filterElementAtIndex, ZFMP_IN(zfindex, index))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForZFMethod, ZFFilterType, filterTypeAtIndex, ZFMP_IN(zfindex, index))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForZFMethod, zfbool, filterCheckActive, ZFMP_IN(const ZFMethod * const &, e))

ZF_NAMESPACE_GLOBAL_END
#endif

