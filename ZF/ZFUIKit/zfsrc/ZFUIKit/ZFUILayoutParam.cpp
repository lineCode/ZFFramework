/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFUILayoutParam.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFENUM_DEFINE(ZFUISizeType)

// ============================================================
// ZFUISizeParam
ZFTYPEID_DEFINE_BY_STRING_CONVERTER(ZFUISizeParam, ZFUISizeParam, {
        ZFCoreArrayPOD<zfindexRange> pos;
        if(!zfCoreDataPairSplitString(pos, 2, src, srcLen))
        {
            return zffalse;
        }

        if(!ZFUISizeTypeEnumFromString(v.width, src + pos[0].start, pos[0].count))
        {
            return zffalse;
        }

        if(!ZFUISizeTypeEnumFromString(v.height, src + pos[1].start, pos[1].count))
        {
            return zffalse;
        }

        return zftrue;
    }, {
        s += zfText("(");
        ZFUISizeTypeEnumToString(s, v.width);
        s += zfText(", ");
        ZFUISizeTypeEnumToString(s, v.height);
        s += zfText(")");
        return zftrue;
    })

ZFMETHOD_FUNC_DEFINE_INLINE_2(ZFUISizeParam, ZFUISizeParamMake,
                              ZFMP_IN(ZFUISizeTypeEnum const &, width),
                              ZFMP_IN(ZFUISizeTypeEnum const &, height))
ZFMETHOD_FUNC_DEFINE_INLINE_1(ZFUISizeParam, ZFUISizeParamMake,
                              ZFMP_IN(ZFUISizeTypeEnum const &, v))

ZFEXPORT_VAR_READONLY_DEFINE(ZFUISizeParam, ZFUISizeParamWrapWrap, ZFUISizeParamMake(ZFUISizeType::e_Wrap, ZFUISizeType::e_Wrap))
ZFEXPORT_VAR_READONLY_DEFINE(ZFUISizeParam, ZFUISizeParamWrapFill, ZFUISizeParamMake(ZFUISizeType::e_Wrap, ZFUISizeType::e_Fill))
ZFEXPORT_VAR_READONLY_DEFINE(ZFUISizeParam, ZFUISizeParamFillWrap, ZFUISizeParamMake(ZFUISizeType::e_Fill, ZFUISizeType::e_Wrap))
ZFEXPORT_VAR_READONLY_DEFINE(ZFUISizeParam, ZFUISizeParamFillFill, ZFUISizeParamMake(ZFUISizeType::e_Fill, ZFUISizeType::e_Fill))

// ============================================================
ZFOBJECT_REGISTER(ZFUILayoutParam)

ZFOBSERVER_EVENT_REGISTER(ZFUILayoutParam, LayoutParamOnChange)

ZF_NAMESPACE_GLOBAL_END

