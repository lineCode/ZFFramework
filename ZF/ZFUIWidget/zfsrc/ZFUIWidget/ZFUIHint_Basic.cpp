#include "ZFUIHint_Basic.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// hint with simple text and icon
ZFSTYLE_DEFAULT_DEFINE(ZFUIHintContentBasic)

ZFMETHOD_FUNC_DEFINE_2(zfautoObject, ZFUIHintMake,
                       ZFMP_IN(const zfchar *, text),
                       ZFMP_IN_OPT(ZFUIImage *, icon, zfnull))
{
    zfblockedAlloc(ZFUIHint, hint);

    zfblockedAlloc(ZFAnimationNativeView, hintAniShow);
    hint->hintAniShowSet(hintAniShow);
    hintAniShow->aniAlphaFromSet(0);
    zfblockedAlloc(ZFAnimationNativeView, hintAniHide);
    hint->hintAniHideSet(hintAniHide);
    hintAniHide->aniAlphaToSet(0);

    zfblockedAlloc(ZFUIHintContentBasic, hintContent);
    hint->hintContentSet(hintContent);
    hintContent->buttonLabelTextSet(text);
    hintContent->buttonIconImageSet(icon);

    return hint;
}
ZFMETHOD_FUNC_DEFINE_INLINE_2(zfautoObject, ZFUIHintShow,
                              ZFMP_IN(const zfchar *, text),
                              ZFMP_IN_OPT(ZFUIImage *, icon, zfnull))

ZF_NAMESPACE_GLOBAL_END

