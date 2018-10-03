/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZF2048UIFrame.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_GLOBAL_INITIALIZER_INIT(ZF2048UISkinInit)
{
    ZFStyleLoad(ZFSerializableDataFromXml(ZFXmlParseFirstElement(ZFInputForResFile("ZF2048/lang/lang_default.xml"))));
    ZFStyleLoad(ZFPathInfo(ZFPathType_res, "ZF2048/skin/default"));
}
ZF_GLOBAL_INITIALIZER_END(ZF2048UISkinInit)

ZF_NAMESPACE_GLOBAL_END

