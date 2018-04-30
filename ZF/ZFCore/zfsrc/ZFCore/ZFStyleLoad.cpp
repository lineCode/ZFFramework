/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFStyleLoad.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
static void _ZFP_ZFStyleLoadImpl(ZF_IN const ZFFilePathInfoData &fileImpl,
                                 ZF_IN const zfchar *pathType,
                                 ZF_IN const zfchar *pathData,
                                 ZF_IN const ZFFilterForString *fileNameFilter,
                                 ZF_IN const ZFFilterForString *dirNameFilter,
                                 ZF_IN const zfchar *relativePath)
{
    ZFFileFindData fd;
    if(fileImpl.callbackFindFirst(fd, pathData))
    {
        do
        {
            if(*fd.fileName() == '.' || *fd.fileName() == '_'
                || zfstringFind(fd.fileName(), zfindexMax(), zfText("._.")) != zfindexMax())
            {
                continue;
            }

            zfstring relativePathTmp = relativePath;
            if(!relativePathTmp.isEmpty())
            {
                relativePathTmp += ZFFileSeparator();
            }
            relativePathTmp += fd.fileName();

            zfstring pathDataChild;
            fileImpl.callbackToChild(pathData, pathDataChild, fd.fileName());
            if(fd.fileIsDir())
            {
                if(dirNameFilter != zfnull && !dirNameFilter->filterCheckActive(fd.fileName()))
                {
                    continue;
                }
                _ZFP_ZFStyleLoadImpl(fileImpl, pathType, pathDataChild, fileNameFilter, dirNameFilter, relativePathTmp);
            }
            else
            {
                if(fileNameFilter != zfnull && !fileNameFilter->filterCheckActive(fd.fileName()))
                {
                    continue;
                }
                zfautoObject styleValue;
                if(ZFObjectIOLoad(styleValue, ZFInputCallbackForPathInfo(ZFPathInfo(pathType, pathDataChild))))
                {
                    ZFFilePathOfWithoutExt(relativePathTmp, relativePathTmp);
                    ZFStyleSet(relativePathTmp, styleValue);
                }
            }
        } while(fileImpl.callbackFindNext(fd));
        fileImpl.callbackFindClose(fd);
    }
}

ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFStyleLoad,
                       ZFMP_IN(const ZFPathInfo &, pathInfo),
                       ZFMP_IN_OPT(const ZFFilterForString *, fileNameFilter, zfnull),
                       ZFMP_IN_OPT(const ZFFilterForString *, dirNameFilter, zfnull))
{
    const ZFFilePathInfoData *fileImpl = ZFFilePathInfoDataGet(pathInfo.pathType);
    if(fileImpl == zfnull || !fileImpl->callbackIsDir(pathInfo.pathData))
    {
        return zffalse;
    }

    ZFStyleChangeBegin();
    _ZFP_ZFStyleLoadImpl(*fileImpl, pathInfo.pathType, pathInfo.pathData, fileNameFilter, dirNameFilter, zfnull);
    ZFStyleChangeEnd();
    return zftrue;
}

// ============================================================
ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFStyleLoad,
                       ZFMP_IN(const ZFSerializableData &, serializableData))
{
    ZFStyleChangeBegin();
    for(zfindex i = 0; i < serializableData.elementCount(); ++i)
    {
        const ZFSerializableData &child = serializableData.elementAtIndex(i);
        const zfchar *styleKey = ZFSerializableUtil::checkPropertyName(child);
        if(styleKey != zfnull)
        {
            zfautoObject styleValueHolder = ZFObjectFromData(child);
            ZFStyleable *styleValue = styleValueHolder;
            if(styleValue != zfnull)
            {
                ZFStyleSet(styleKey, styleValue);
            }
        }
    }
    ZFStyleChangeEnd();
    return zftrue;
}

// ============================================================
ZFMETHOD_FUNC_DEFINE_1(zfautoObject, ZFStyleCreate,
                       ZFMP_IN(const zfchar *, styleKey))
{
    ZFStyleable *style = ZFStyleGet(styleKey);
    if(style != zfnull)
    {
        zfautoObject ret = style->classData()->newInstance();
        ZFStyleable *t = ret;
        if(t->styleKeySet(styleKey))
        {
            return ret;
        }
    }
    return zfnull;
}

ZF_NAMESPACE_GLOBAL_END

