/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFSerializableDataRefType.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFSerializableDataRefTypeDataHolder, ZFLevelZFFrameworkStatic)
{
}
public:
    ZFCoreMap refTypeMap; // _ZFP_ZFSerializableDataRefInfoLoadCallback *
ZF_GLOBAL_INITIALIZER_END(ZFSerializableDataRefTypeDataHolder)
#define _ZFP_ZFSerializableData_refTypeMap (ZF_GLOBAL_INITIALIZER_INSTANCE(ZFSerializableDataRefTypeDataHolder)->refTypeMap)

void ZFSerializableDataRefTypeGetAllT(ZF_OUT ZFCoreArray<const zfchar *> &ret)
{
    zfCoreMutexLocker();
    _ZFP_ZFSerializableData_refTypeMap.allKeyT(ret);
}

void _ZFP_ZFSerializableDataRefTypeRegister(ZF_IN const zfchar *refType,
                                            ZF_IN _ZFP_ZFSerializableDataRefInfoLoadCallback callback)
{
    zfCoreMutexLocker();
    _ZFP_ZFSerializableDataRefInfoLoadCallback *tmp = (_ZFP_ZFSerializableDataRefInfoLoadCallback *)zfmalloc(sizeof(_ZFP_ZFSerializableDataRefInfoLoadCallback));
    *tmp = callback;
    _ZFP_ZFSerializableData_refTypeMap.set(
        refType,
        ZFCorePointerForPOD<_ZFP_ZFSerializableDataRefInfoLoadCallback *>(tmp));
}
void _ZFP_ZFSerializableDataRefTypeUnregister(ZF_IN const zfchar *refType)
{
    zfCoreMutexLocker();
    _ZFP_ZFSerializableData_refTypeMap.remove(refType);
}
_ZFP_ZFSerializableDataRefInfoLoadCallback _ZFP_ZFSerializableDataRefTypeGet(ZF_IN const zfchar *refType)
{
    _ZFP_ZFSerializableDataRefInfoLoadCallback *callback = _ZFP_ZFSerializableData_refTypeMap
        .get<_ZFP_ZFSerializableDataRefInfoLoadCallback *>(refType);
    return (callback ? *callback : zfnull);
}

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(void, ZFSerializableDataRefTypeGetAllT, ZFMP_OUT(ZFCoreArray<const zfchar *> &, ret))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_0(ZFCoreArrayPOD<const zfchar *>, ZFSerializableDataRefTypeGetAll)

ZF_NAMESPACE_GLOBAL_END
#endif

