/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZF2048UISkin.h"
#include "ZF2048UIFrame.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_GLOBAL_INITIALIZER_INIT(ZF2048UIBlockImageInit)
{
    ZF2048UISkinLoad();
}
ZF_GLOBAL_INITIALIZER_END(ZF2048UIBlockImageInit)

void ZF2048UISkinLoad(ZF_IN const zfchar *resPath /* = zfText("ZF2048/skin/default") */)
{
    { // block_bg.png
        zfstring path = zfstringWithFormat(zfText("%s/block/block_bg.png"), resPath);
        zfautoObject image = zfRes(path);
        if(image == zfnull)
        {
            zfSkinSet(zfText("ZF2048_block_bg"), zfnull);
        }
        else
        {
            zfblockedAlloc(ZFUIImageView, skin);
            skin->imageSet(image.to<ZFUIImage *>());
            zfSkinSet(zfText("ZF2048_block_bg"), skin);
        }
    }
    { // block_na.png
        zfstring path = zfstringWithFormat(zfText("%s/block/block_na.png"), resPath);
        zfautoObject image = zfRes(path);
        if(image == zfnull)
        {
            zfSkinSet(zfText("ZF2048_block_na"), zfnull);
        }
        else
        {
            zfblockedAlloc(ZFUIImageView, skin);
            skin->imageSet(image.to<ZFUIImage *>());
            zfSkinSet(zfText("ZF2048_block_na"), skin);
        }
    }
    { // frame
        zfstring path = zfstringWithFormat(zfText("%s/frame/frame_bg.xml"), resPath);
        zfautoObject image = zfRes(path);
        if(image == zfnull)
        {
            zfSkinSet(zfText("ZF2048_frame"), zfnull);
        }
        else
        {
            zfblockedAlloc(ZF2048UIFrame, skin);
            skin->frameBackgroundImageSet(image.to<ZFUIImage *>());
            zfSkinSet(zfText("ZF2048_frame"), skin);
        }
    }

    // each block "block_(2,4,8,...).png"
    ZFFileFindData fd;
    zfstring blockPath = zfstringWithFormat(zfText("%s/block"), resPath);
    if(ZFFileResFindFirst(fd, blockPath))
    {
        do
        {
            if(fd.fileIsDir())
            {
                continue;
            }
            zfstring fileName = fd.fileName();
            const zfchar *filePrefix = zfText("block_");
            const zfchar *fileExt = zfText(".png");
            if(!zfscmpTheSame(fileName.cString() + (fileName.length() - zfslen(fileExt)), fileExt)
                || zfsncmp(fileName.cString(), filePrefix, zfslen(filePrefix)) != 0)
            {
                continue;
            }
            zfstring blockValueString(fileName.cString() + zfslen(filePrefix), fileName.length() - zfslen(filePrefix) - zfslen(fileExt));
            ZF2048Value blockValue = (ZF2048Value)0;
            if(zfintFromString(blockValue, blockValueString))
            {
                zfstring skinKey = zfstringWithFormat(zfText("ZF2048_block_%d"), (zfint)blockValue);
                zfautoObject image = zfRes(fd.filePath());
                if(image == zfnull)
                {
                    zfSkinSet(skinKey, zfnull);
                }
                else
                {
                    zfblockedAlloc(ZFUIImageView, skin);
                    skin->imageSet(image.to<ZFUIImage *>());
                    zfSkinSet(skinKey, skin);
                }
            }
        } while(ZFFileResFindNext(fd));
        ZFFileResFindClose(fd);
    }

    zfSkinNotifyChanged();
}

ZF_NAMESPACE_GLOBAL_END

