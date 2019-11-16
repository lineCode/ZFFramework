#include "ZF2048UIBlock.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFSTYLE_DEFAULT_DEFINE(ZF2048UIBlock)

ZFOBJECT_REGISTER(ZF2048UIBlock)

ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZF2048UIBlock, ZF2048Value, blockValue)
{
    if(this->blockValue() == 0)
    {
        this->imageSet(zfnull);
        this->blockTitle()->viewVisibleSet(zffalse);
        return ;
    }

    zfstring skinKey = zfstringWithFormat("ZF2048/block/block_%d", (zfint)this->blockValue());
    this->imageSet(ZFStyleGet(skinKey));
    if(this->image() == zfnull)
    {
        this->imageSet(ZFStyleGet("ZF2048/block/block_na"));
        this->blockTitle()->viewVisibleSet(zftrue);
    }
    else
    {
        this->blockTitle()->viewVisibleSet(zffalse);
    }
}

void ZF2048UIBlock::objectOnInit(void)
{
    zfsuper::objectOnInit();
    this->internalFgViewAdd(this->blockTitle());
    this->blockTitle()->layoutParam()->layoutAlignSet(ZFUIAlign::e_Center);
    this->blockTitle()->viewVisibleSet(zffalse);
    this->blockTitle()->textAlignSet(ZFUIAlign::e_Center);
    this->blockTitle()->textColorSet(ZFUIColorWhite());
    this->blockTitle()->textShadowColorSet(ZFUIColorGrayDark());
    this->blockTitle()->textAppearanceSet(ZFUITextAppearance::e_Bold);
    this->blockTitle()->textSizeSet(24);
}

ZF_NAMESPACE_GLOBAL_END

