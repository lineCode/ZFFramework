/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFCoreTypeDef_ClassType.h"

ZF_NAMESPACE_GLOBAL_BEGIN

void *zfmallocZero(zfindex size)
{
    void *ret = zfmalloc(size);
    if(ret)
    {
        zfmemset(ret, 0, size);
    }
    return ret;
}
void *zfreallocZero(void *oldPtr, zfindex newSize, zfindex oldSize)
{
    void *ret = zfrealloc(oldPtr, newSize);
    if(ret && newSize > oldSize)
    {
        zfmemset(((zfbyte *)ret) + oldSize, 0, newSize - oldSize);
    }
    return ret;
}

ZF_NAMESPACE_GLOBAL_END

