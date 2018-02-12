/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFSkinUtil_ZFSerializableHelper.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_CREATOR_DEFINE(skin, ZFObjectCreatorType_skin)
{
    return zfSkin(data);
}

ZF_NAMESPACE_GLOBAL_END

