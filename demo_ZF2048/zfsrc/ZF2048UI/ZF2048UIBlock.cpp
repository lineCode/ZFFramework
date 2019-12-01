#include "ZF2048UIBlock.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFSTYLE_DEFAULT_DEFINE(ZF2048UIBlock)

ZFOBJECT_REGISTER(ZF2048UIBlock)

ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZF2048UIBlock, ZF2048Value, blockValue)
{
    if(this->blockValue() == 0)
    {
        this->image(zfnull);
        this->blockTitle()->viewVisible(zffalse);
        return ;
    }

    zfstring skinKey = zfstringWithFormat("ZF2048/block/block_%d", (zfint)this->blockValue());
    this->image(ZFStyleGet(skinKey));
    if(this->image() == zfnull)
    {
        this->image(ZFStyleGet("ZF2048/block/block_na"));
        this->blockTitle()->viewVisible(zftrue);
    }
    else
    {
        this->blockTitle()->viewVisible(zffalse);
    }
}

void ZF2048UIBlock::objectOnInit(void)
{
    zfsuper::objectOnInit();
    this->internalFgViewAdd(this->blockTitle());
    this->blockTitle()->layoutParam()->layoutAlign(ZFUIAlign::e_Center);
    this->blockTitle()->viewVisible(zffalse);
    this->blockTitle()->textAlign(ZFUIAlign::e_Center);
    this->blockTitle()->textColor(ZFUIColorWhite());
    this->blockTitle()->textShadowColor(ZFUIColorGrayDark());
    this->blockTitle()->textAppearance(ZFUITextAppearance::e_Bold);
    this->blockTitle()->textSize(24);
}

ZF_NAMESPACE_GLOBAL_END

