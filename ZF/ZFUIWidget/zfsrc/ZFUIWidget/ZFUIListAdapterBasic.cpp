/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFUIListAdapterBasic.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFUIListAdapterBasicParam)

ZFOBJECT_REGISTER(ZFUIListAdapterBasic)

ZFOBSERVER_EVENT_REGISTER(ZFUIListAdapterBasic, ListCellCount)
ZFOBSERVER_EVENT_REGISTER(ZFUIListAdapterBasic, ListCellAtIndex)
ZFOBSERVER_EVENT_REGISTER(ZFUIListAdapterBasic, ListCellSizeAtIndex)
ZFOBSERVER_EVENT_REGISTER(ZFUIListAdapterBasic, ListCellCacheOnAccess)

ZF_NAMESPACE_GLOBAL_END

