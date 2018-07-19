/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFCoreStatistic.h"
#include "ZFCoreStringUtil.h"
#include "ZFCoreMap.h"
#include "ZFNamespaceImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN
ZF_NAMESPACE_BEGIN(ZFCoreStatistic)

static ZFCoreMap &_ZFP_ZFCoreStatisticDataMap(void)
{
    static ZFCoreMap d;
    return d;
}

void invokeCountLog(ZF_IN const zfchar *key)
{
    ZFCoreMap &m = _ZFP_ZFCoreStatisticDataMap();
    zfindex *count = m.get<zfindex *>(key);
    if(count != zfnull)
    {
        ++(*count);
    }
    else
    {
        m.set(key, ZFCorePointerForObject<zfindex *>(zfnew(zfindex, 1)));
    }
}
void invokeCountRemove(ZF_IN const zfchar *key)
{
    _ZFP_ZFCoreStatisticDataMap().remove(key);
}
void invokeCountRemoveAll(void)
{
    _ZFP_ZFCoreStatisticDataMap().removeAll();
}
zfindex invokeCountGet(ZF_IN const zfchar *key)
{
    zfindex *count = _ZFP_ZFCoreStatisticDataMap().get<zfindex *>(key);
    return ((count != zfnull) ? *count : 0);
}

ZF_NAMESPACE_END_WITH_REGISTER(ZFCoreStatistic, ZF_NAMESPACE_GLOBAL)
ZF_NAMESPACE_GLOBAL_END

