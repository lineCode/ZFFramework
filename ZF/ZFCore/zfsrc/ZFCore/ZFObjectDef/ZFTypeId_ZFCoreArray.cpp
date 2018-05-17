/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFTypeId_ZFCoreArray.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/** @cond ZFPrivateDoc */
void v_ZFCoreArray::objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
{
    this->zfv.objectInfoOfContentT(ret, ZFCoreElementInfoGetter<zfautoObject>::elementInfoGetter);
}
/** @endcond */

ZF_NAMESPACE_GLOBAL_END

