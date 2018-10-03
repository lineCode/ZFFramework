/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFUIViewTreePrint.h"

#include "ZFUIKit/protocol/ZFProtocolZFUIView.h"

ZF_NAMESPACE_GLOBAL_BEGIN

static void _ZFP_ZFUIViewTreePrintDelayedAction(ZF_IN_OUT ZFListenerData &listenerData, ZF_IN ZFObject *userData)
{
    ZFListenerHolder *data = ZFCastZFObjectUnchecked(ZFListenerHolder *, userData);
    ZFUIViewTreePrint(ZFCastZFObjectUnchecked(ZFUIView *, data->listenerData.param0), data->runnable);
}
ZFMETHOD_FUNC_DEFINE_2(void, ZFUIViewTreePrintDelayed,
                       ZFMP_IN(ZFUIView *, view),
                       ZFMP_IN_OPT(const ZFOutput &, outputCallback, ZFOutputDefault()))
{
    ZFThreadTaskRequest(
        ZFCallbackForFunc(_ZFP_ZFUIViewTreePrintDelayedAction),
        zflineAlloc(ZFListenerHolder, outputCallback, ZFListenerData().param0Set(view)));
}

ZFMETHOD_FUNC_DEFINE_3(void, ZFUIViewTreePrintDelayed,
                       ZFMP_IN(zftimet, delay),
                       ZFMP_IN(ZFUIView *, view),
                       ZFMP_IN_OPT(const ZFOutput &, outputCallback, ZFOutputDefault()))
{
    ZFThreadExecuteInMainThreadAfterDelay(
        delay,
        ZFCallbackForFunc(_ZFP_ZFUIViewTreePrintDelayedAction),
        zflineAlloc(ZFListenerHolder, outputCallback, ZFListenerData().param0Set(view)));
}

// ============================================================
zfclassPOD _ZFP_ZFUIViewTreePrintData
{
public:
    const ZFClass *viewClass;
    ZFUIViewTreePrintInfoGetter viewInfoGetter;
};

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIViewTreePrintDataHolder, ZFLevelZFFrameworkEssential)
{
}
public:
    // ordered by:
    // ZFUIView, classBaseA, classChildA, classBaseB, classChildB
    ZFCoreArrayPOD<_ZFP_ZFUIViewTreePrintData> datas;
ZF_GLOBAL_INITIALIZER_END(ZFUIViewTreePrintDataHolder)

zfclassPOD _ZFP_ZFUIViewTreePrintPrintData
{
public:
    ZFUIView *view;
    zfindex depth;
    zfindex siblingIndex;
    ZFUIViewChildLayerEnum layer;
};
static ZFObject *_ZFP_ZFUIViewTreePrintSyncObject = zfnull;
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIViewTreePrintSyncObjectHolder, ZFLevelZFFrameworkHigh)
{
    _ZFP_ZFUIViewTreePrintSyncObject = zfAlloc(ZFObject);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIViewTreePrintSyncObjectHolder)
{
    zfRelease(_ZFP_ZFUIViewTreePrintSyncObject);
    _ZFP_ZFUIViewTreePrintSyncObject = zfnull;
}
ZF_GLOBAL_INITIALIZER_END(ZFUIViewTreePrintSyncObjectHolder);

ZFMETHOD_FUNC_DEFINE_2(void, ZFUIViewTreePrint,
                       ZFMP_IN(ZFUIView *, view),
                       ZFMP_IN_OPT(const ZFOutput &, outputCallback, ZFOutputDefault()))
{
    if(view == zfnull || !outputCallback.callbackIsValid())
    {
        return ;
    }

    zfsynchronize(_ZFP_ZFUIViewTreePrintSyncObject);

    ZFCoreArrayPOD<_ZFP_ZFUIViewTreePrintPrintData> printDatas;
    _ZFP_ZFUIViewTreePrintPrintData rootPrintData;
    rootPrintData.view = view;
    rootPrintData.depth = 0;
    rootPrintData.siblingIndex = 0;
    rootPrintData.layer = ZFUIViewChildLayer::e_Normal;
    printDatas.add(rootPrintData);

    outputCallback.execute("====================== view tree begin =====================\n");
    do
    {
        _ZFP_ZFUIViewTreePrintPrintData printData = printDatas.getLast();
        printDatas.removeLast();

        // ignore delegate view
        if(printData.view->viewDelegateForParent())
        {
            continue;
        }

        // all children
        ZFCoreArrayPOD<ZFUIView *> implViews = printData.view->internalImplViewArray();
        ZFCoreArrayPOD<ZFUIView *> bgViews = printData.view->internalBgViewArray();
        ZFCoreArrayPOD<ZFUIView *> normalViews = printData.view->childArray();
        ZFCoreArrayPOD<ZFUIView *> fgViews = printData.view->internalFgViewArray();
        for(zfindex i = fgViews.count() - 1; i != zfindexMax(); --i)
        {
            _ZFP_ZFUIViewTreePrintPrintData printDataTmp;
            printDataTmp.view = fgViews.get(i)->to<ZFUIView *>();
            printDataTmp.depth = printData.depth + 1;
            printDataTmp.siblingIndex = i;
            printDataTmp.layer = ZFUIViewChildLayer::e_InternalFg;
            printDatas.add(printDataTmp);
        }
        for(zfindex i = normalViews.count() - 1; i != zfindexMax(); --i)
        {
            _ZFP_ZFUIViewTreePrintPrintData printDataTmp;
            printDataTmp.view = normalViews.get(i)->to<ZFUIView *>();
            printDataTmp.depth = printData.depth + 1;
            printDataTmp.siblingIndex = i;
            printDataTmp.layer = ZFUIViewChildLayer::e_Normal;
            printDatas.add(printDataTmp);
        }
        for(zfindex i = bgViews.count() - 1; i != zfindexMax(); --i)
        {
            _ZFP_ZFUIViewTreePrintPrintData printDataTmp;
            printDataTmp.view = bgViews.get(i)->to<ZFUIView *>();
            printDataTmp.depth = printData.depth + 1;
            printDataTmp.siblingIndex = i;
            printDataTmp.layer = ZFUIViewChildLayer::e_InternalBg;
            printDatas.add(printDataTmp);
        }
        for(zfindex i = implViews.count() - 1; i != zfindexMax(); --i)
        {
            _ZFP_ZFUIViewTreePrintPrintData printDataTmp;
            printDataTmp.view = implViews.get(i)->to<ZFUIView *>();
            printDataTmp.depth = printData.depth + 1;
            printDataTmp.siblingIndex = i;
            printDataTmp.layer = ZFUIViewChildLayer::e_InternalImpl;
            printDatas.add(printDataTmp);
        }

        outputCallback.execute("|");
        outputCallback.execute(zfstringWithFormat("%2zi", printData.siblingIndex).cString());
        for(zfindex i = printData.depth - 1; i != zfindexMax(); --i)
        {
            outputCallback.execute(" |");
        }

        switch(printData.layer)
        {
            case ZFUIViewChildLayer::e_Normal:
                outputCallback.execute(" ");
                break;
            case ZFUIViewChildLayer::e_InternalImpl:
                outputCallback.execute(" impl ");
                break;
            case ZFUIViewChildLayer::e_InternalBg:
                outputCallback.execute(" bg ");
                break;
            case ZFUIViewChildLayer::e_InternalFg:
                outputCallback.execute(" fg ");
                break;
            default:
                zfCoreCriticalShouldNotGoHere();
                return ;
        }

        ZFCoreArrayPOD<_ZFP_ZFUIViewTreePrintData> &datas = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewTreePrintDataHolder)->datas;
        zfbool exist = zffalse;
        for(zfindex i = datas.count() - 1; i != zfindexMax(); --i)
        {
            if(printData.view->classData()->classIsTypeOf(datas[i].viewClass))
            {
                datas[i].viewInfoGetter(printData.view, outputCallback);
                exist = zftrue;
                break;
            }
        }
        if(!exist)
        {
            zfstring tmp;
            printData.view->objectInfoT(tmp);
            zfindex startFix = 0;
            zfindex countFix = 0;
            zfindex tokenLeftLen = zfslen(ZFTOKEN_ZFObjectInfoLeft);
            zfindex tokenRightLen = zfslen(ZFTOKEN_ZFObjectInfoRight);
            if(tmp.length() >= tokenLeftLen
                && zfsncmp(tmp.cString(), ZFTOKEN_ZFObjectInfoLeft, tokenLeftLen) == 0)
            {
                startFix += tokenLeftLen;
                countFix += tokenLeftLen;
            }
            if(tmp.length() >= tokenRightLen
                && zfsncmp(tmp.cString() - tokenRightLen, ZFTOKEN_ZFObjectInfoRight, tokenRightLen) == 0)
            {
                countFix += tokenRightLen;
            }
            outputCallback.execute(tmp.cString() + startFix, tmp.length() - countFix);
        }
        outputCallback.execute("\n");
    } while(!printDatas.isEmpty());
    outputCallback.execute("====================== view tree  end  =====================\n");
}

void ZFUIViewTreePrintInfoGetterSet(ZF_IN const ZFClass *viewClass,
                                    ZF_IN ZFUIViewTreePrintInfoGetter viewInfoGetter)
{
    zfCoreAssert(viewClass != zfnull && viewClass->classIsTypeOf(ZFUIView::ClassData()));
    ZFCoreArrayPOD<_ZFP_ZFUIViewTreePrintData> &datas = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewTreePrintDataHolder)->datas;
    if(viewInfoGetter == zfnull)
    {
        for(zfindex i = 0; i < datas.count(); ++i)
        {
            const ZFClass *cls = datas[i].viewClass;
            if(viewClass == cls)
            {
                datas.remove(i);
                break;
            }
        }
        return ;
    }

    zfindex indexAddTo = zfindexMax();
    for(zfindex i = 0; i < datas.count(); ++i)
    {
        const ZFClass *cls = datas[i].viewClass;
        if(viewClass == cls)
        {
            datas[i].viewInfoGetter = viewInfoGetter;
            break;
        }
        else if(cls->classIsTypeOf(viewClass))
        {
            indexAddTo = i;
            break;
        }
        else if(viewClass->classIsTypeOf(cls))
        {
            indexAddTo = i + 1;
        }
    }

    _ZFP_ZFUIViewTreePrintData data;
    data.viewClass = viewClass;
    data.viewInfoGetter = viewInfoGetter;
    if(indexAddTo == zfindexMax())
    {
        datas.add(data);
    }
    else
    {
        datas.add(indexAddTo, data);
    }
}
ZFUIViewTreePrintInfoGetter ZFUIViewTreePrintInfoGetterGet(ZF_IN const ZFClass *viewClass)
{
    ZFCoreArrayPOD<_ZFP_ZFUIViewTreePrintData> &datas = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewTreePrintDataHolder)->datas;
    for(zfindex i = datas.count() - 1; i != zfindexMax(); --i)
    {
        if(datas[i].viewClass == viewClass)
        {
            return datas[i].viewInfoGetter;
        }
    }
    return zfnull;
}

ZF_NAMESPACE_GLOBAL_END

