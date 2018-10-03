/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFUIImageIO.h"
#include "protocol/ZFProtocolZFUIImage.h"
#include "protocol/ZFProtocolZFUIImageIO.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUIImageScale
ZFMETHOD_FUNC_DEFINE_2(zfautoObject, ZFUIImageScale,
                       ZFMP_IN(ZFUIImage *, image),
                       ZFMP_IN(const ZFUISize &, newSize))
{
    if(image == zfnull || image->nativeImage() == zfnull)
    {
        return zfnull;
    }
    zffloat scale = image->imageScaleFixed();
    void *nativeImage = ZFPROTOCOL_ACCESS(ZFUIImageIO)->imageApplyScale(
        scale,
        image->nativeImage(),
        ZFUISizeApplyScale(newSize, scale),
        ZFUIMarginApplyScale(image->imageNinePatch(), scale));
    if(nativeImage == zfnull)
    {
        return zfnull;
    }
    zfautoObject ret = ZFUIImage::ClassData()->newInstance();
    ret.to<ZFUIImage *>()->nativeImageSet(nativeImage);
    ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageRelease(nativeImage);
    return ret;
}

// ============================================================
// ZFUIImageLoadFromNativeImage
ZFMETHOD_FUNC_DEFINE_1(zfautoObject, ZFUIImageLoadFromNativeImage,
                       ZFMP_IN(void *, nativeImage))
{
    if(nativeImage == zfnull)
    {
        return zfnull;
    }
    zfautoObject ret = ZFUIImage::ClassData()->newInstance();
    ret.to<ZFUIImage *>()->nativeImageSet(nativeImage);
    return ret;
}

// ============================================================
// input
ZFUIIMAGE_SERIALIZE_TYPE_DEFINE(input, ZFUIImageSerializeType_input)
{
    ZFCallback input;
    if(!ZFCallbackFromData(input, serializableData, outErrorHint, outErrorPos))
    {
        return zffalse;
    }
    if(!input.callbackIsValid())
    {
        ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
            "invalid callback");
        return zffalse;
    }
    if(!ZFUIImageEncodeFromFile(ret, input))
    {
        ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
            "load image failed");
        return zffalse;
    }
    return zftrue;
}

ZFMETHOD_FUNC_DEFINE_1(zfautoObject, ZFUIImageLoadFromInput,
                       ZFMP_IN(const ZFInput &, input))
{
    zfautoObject ret = ZFUIImage::ClassData()->newInstance();
    ZFUIImage *image = ret;
    if(!ZFUIImageEncodeFromFile(image, input))
    {
        return zfnull;
    }

    if(!input.callbackSerializeCustomDisabled())
    {
        ZFSerializableData inputData;
        if(ZFCallbackToData(inputData, input))
        {
            image->imageSerializableTypeSet(ZFUIImageSerializeType_input);
            image->imageSerializableDataSet(&inputData);
        }
    }

    return ret;
}

// ============================================================
// color
ZFUIIMAGE_SERIALIZE_TYPE_DEFINE(color, ZFUIImageSerializeType_color)
{
    ZFUIColor color = ZFUIColorZero();
    { // color
        const ZFSerializableData *categoryData = ZFSerializableUtil::checkElementByCategory(serializableData, ZFSerializableKeyword_ZFUIImageIO_color);
        if(categoryData != zfnull)
        {
            if(!ZFUIColorFromData(color, *categoryData, outErrorHint, outErrorPos))
            {
                return zffalse;
            }
        }
    }

    ZFUISize size = ZFUISizeMake(1);
    { // size
        const ZFSerializableData *categoryData = ZFSerializableUtil::checkElementByCategory(serializableData, ZFSerializableKeyword_ZFUIImageIO_color_size);
        if(categoryData != zfnull)
        {
            if(!ZFUISizeFromData(size, *categoryData, outErrorHint, outErrorPos))
            {
                return zffalse;
            }
            if(size.width <= 0 || size.height <= 0)
            {
                ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, *categoryData,
                    "invalid size: %s", ZFUISizeToString(size).cString());
                return zffalse;
            }
        }
    }

    ZFSerializableData imageData;
    {
        if(!ZFUIColorIsEqual(color, ZFUIColorZero()))
        {
            ZFSerializableData categoryData;
            if(!ZFUIColorToData(categoryData, color, outErrorHint))
            {
                return zffalse;
            }
            categoryData.categorySet(ZFSerializableKeyword_ZFUIImageIO_color);
            imageData.elementAdd(categoryData);
        }
        if(!ZFUISizeIsEqual(size, ZFUISizeMake(1, 1)))
        {
            ZFSerializableData categoryData;
            if(!ZFUISizeToData(categoryData, size, outErrorHint))
            {
                return zffalse;
            }
            categoryData.categorySet(ZFSerializableKeyword_ZFUIImageIO_color_size);
            imageData.elementAdd(categoryData);
        }
    }

    void *nativeImage = ZFPROTOCOL_ACCESS(ZFUIImageIO)->imageLoadFromColor(
        ZFUIGlobalStyle::DefaultStyle()->imageScale(),
        color,
        ZFUISizeApplyScale(size, ZFUIGlobalStyle::DefaultStyle()->imageScale()));
    ret->nativeImageSet(nativeImage);
    ret->imageSerializableDataSet(&imageData);
    ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageRelease(nativeImage);

    return zftrue;
}
ZFMETHOD_FUNC_DEFINE_2(zfautoObject, ZFUIImageLoadFromColor,
                       ZFMP_IN(const ZFUIColor &, color),
                       ZFMP_IN_OPT(const ZFUISize &, size, ZFUISizeZero()))
{
    ZFUISize sizeTmp = ZFUISizeMake(zfmMax(1, size.width), zfmMax(1, size.height));
    void *nativeImage = ZFPROTOCOL_ACCESS(ZFUIImageIO)->imageLoadFromColor(
        ZFUIGlobalStyle::DefaultStyle()->imageScale(),
        color,
        ZFUISizeApplyScale(sizeTmp, ZFUIGlobalStyle::DefaultStyle()->imageScale()));
    if(nativeImage == zfnull)
    {
        return zfnull;
    }

    zfautoObject ret = ZFUIImage::ClassData()->newInstance();
    ZFUIImage *image = ret;
    image->nativeImageSet(nativeImage);
    ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageRelease(nativeImage);

    ZFSerializableData imageData;

    do
    {
        // color
        if(color != ZFUIColorZero())
        {
            ZFSerializableData categoryData;
            if(!ZFUIColorToData(categoryData, color))
            {
                break;
            }
            categoryData.categorySet(ZFSerializableKeyword_ZFUIImageIO_color);
            imageData.elementAdd(categoryData);
        }

        // size
        if(!ZFUISizeIsEqual(sizeTmp, ZFUISizeMake(1, 1)))
        {
            ZFSerializableData categoryData;
            if(!ZFUISizeToData(categoryData, sizeTmp))
            {
                break;
            }
            categoryData.categorySet(ZFSerializableKeyword_ZFUIImageIO_color_size);
            imageData.elementAdd(categoryData);
        }

        imageData.itemClassSet(ZFSerializableKeyword_node);
    } while(zffalse);

    if(imageData.itemClass() != zfnull)
    {
        image->imageSerializableTypeSet(ZFUIImageSerializeType_color);
        image->imageSerializableDataSet(&imageData);
    }

    return ret;
}

ZF_NAMESPACE_GLOBAL_END

