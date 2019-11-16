#include "ZF2048UIFrame.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_GLOBAL_INITIALIZER_INIT(ZF2048UISkinInit)
{
    ZFStyleLoad(ZFPathInfo(ZFPathType_res(), "ZF2048/lang/lang_default.xml"));
    ZFStyleLoad(ZFPathInfo(ZFPathType_res(), "ZF2048/skin/default"));
}
ZF_GLOBAL_INITIALIZER_END(ZF2048UISkinInit)

ZF_NAMESPACE_GLOBAL_END

