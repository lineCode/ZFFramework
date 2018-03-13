/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFObjectCreator_common.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_CREATOR_DEFINE(ZFMethod, ZFObjectCreatorType_ZFMethod)
{
    if(data == zfnull)
    {
        return zfnull;
    }

    ZFCoreArrayPOD<zfindexRange> pos;
    if(!zfCoreDataPairSplitString(
            pos, zfHint("desiredCountOrIndexMax")3,
            data, zfHint("srcLen")zfindexMax(),
            zfHint("separatorTokens")zfText(":"),
            zfHint("leftToken")zfnull, zfHint("rightToken")zfnull,
            zfHint("allowEmptyItem")zftrue)
       || pos[0].count == 0 || pos[1].count != 0 || pos[2].count == 0)
    {
        return zfnull;
    }

    const ZFClass *cls = ZFClass::classForName(zfstring(data + pos[0].start, pos[0].count));
    if(cls == zfnull) {return zfnull;}

    const ZFMethod *method = cls->methodForName(zfstring(data + pos[2].start, pos[2].count));
    if(method == zfnull || !method->methodIsStatic()) {return zfnull;}

    return method->executeStatic<zfautoObject>();
}

ZF_NAMESPACE_GLOBAL_END

