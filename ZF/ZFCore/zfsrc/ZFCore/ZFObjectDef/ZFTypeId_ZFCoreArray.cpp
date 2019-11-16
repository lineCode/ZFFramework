#include "ZFTypeId_ZFCoreArray.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

void v_ZFCoreArray::objectInfoT(ZF_IN_OUT zfstring &ret)
{
    this->zfv.objectInfoOfContentT(ret, ZFCoreElementInfoGetter<zfautoObject>::elementInfoGetter);
}

ZF_NAMESPACE_GLOBAL_END

