#ifndef _ZFI_ZF2048UIBlock_h_
#define _ZFI_ZF2048UIBlock_h_

#include "ZFUIWidget.h"
#include "../ZF2048Core/ZF2048Types.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZF2048UIBlock
/**
 * @brief linear container view that layout children in linear by specified orientation
 */
zfclass ZF2048UIBlock : zfextends ZFUIImageView
{
    ZFOBJECT_DECLARE(ZF2048UIBlock, ZFUIImageView)
    ZFSTYLE_DEFAULT_DECLARE(ZF2048UIBlock)

    ZFALLOC_CACHE_RELEASE({
        cache->blockValue(0);
        cache->blockTitle()->text("");
    })

public:
    ZFPROPERTY_ASSIGN_WITH_INIT(ZF2048Value, blockValue, 0)
    ZFPROPERTY_OVERRIDE_ON_ATTACH_DECLARE(ZF2048Value, blockValue)

    ZFPROPERTY_RETAIN_READONLY(ZFUITextView *, blockTitle, zflineAlloc(ZFUITextView))

protected:
    zfoverride
    virtual void objectOnInit(void);
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZF2048UIBlock_h_

