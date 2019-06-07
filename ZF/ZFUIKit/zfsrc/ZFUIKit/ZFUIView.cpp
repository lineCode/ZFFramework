/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFUIView.h"
#include "protocol/ZFProtocolZFUIView.h"
#include "protocol/ZFProtocolZFUIViewFocus.h"
#include "ZFUIViewFocus.h"

#include "ZFCore/ZFSTLWrapper/zfstl_string.h"
#include "ZFCore/ZFSTLWrapper/zfstl_map.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFSTYLE_DEFAULT_DEFINE(ZFUIView)

// ============================================================
// attached listeners
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIViewListenerHolder, ZFLevelZFFrameworkEssential)
{
    this->layoutParamChangedListener = ZFCallbackForFunc(zfself::layoutParamChanged);
    this->viewPropertyOnUpdateListener = ZFCallbackForFunc(zfself::viewPropertyOnUpdate);
}
public:
    ZFListener layoutParamChangedListener;
    ZFListener viewPropertyOnUpdateListener;
private:
    static ZFLISTENER_PROTOTYPE_EXPAND(layoutParamChanged)
    {
        userData->objectHolded<ZFUIView *>()->layoutRequest();
    }
    static ZFLISTENER_PROTOTYPE_EXPAND(viewPropertyOnUpdate)
    {
        userData->objectHolded<ZFUIView *>()->_ZFP_ZFUIView_viewPropertyNotifyUpdate();
    }
ZF_GLOBAL_INITIALIZER_END(ZFUIViewListenerHolder)

// ============================================================
// _ZFP_ZFUIViewPrivate
static zfuint _ZFP_ZFUIView_layoutRequestOverrideFlag = 0;
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIViewLayoutRequestOverrideInit, ZFLevelZFFrameworkEssential)
{
    _ZFP_ZFUIView_layoutRequestOverrideFlag = 0;
}
ZF_GLOBAL_INITIALIZER_END(ZFUIViewLayoutRequestOverrideInit)

zfclassLikePOD _ZFP_ZFUIViewLayerData
{
public:
    ZFCoreArrayPOD<ZFUIView *> views;
};
typedef zfstlmap<zfstlstringZ, zfbool> _ZFP_ZFUIViewInternalViewAutoSerializeTagMapType;
zfclassNotPOD _ZFP_ZFUIViewPrivate
{
public:
    void *nativeView;
    void *nativeImplView;
    ZFUIViewNativeImplViewDeleteCallback nativeImplViewDeleteCallback;
    ZFUIMargin nativeImplViewMargin;

    ZFUIView *viewDelegateParent;
    ZFUIView *viewDelegate;

    ZFUIView *viewParent;
    ZFUIViewChildLayerEnum viewLayer;
    ZFUIViewLayoutParam *layoutParam; // retain
    ZFUIViewLayoutParam *serializableRefLayoutParam; // retain
    zfautoObject serializableRefLayoutParamCache;

    _ZFP_ZFUIViewInternalViewAutoSerializeTagMapType internalViewAutoSerializeTags;

    zffloat scaleForImpl;
    zffloat scaleFixed;
    /*
     * here's a memo for the scale logic:
     *
     * we have these type of scale:
     * * scaleForApp : set by ZFUIRootView, can be set by user, 1 by default
     * * scaleForImpl : impl's scale, ZFUIViewImpl::nativeViewScaleForImpl by default
     *   this value depends on impl's screen's scale, may have different value on different screen,
     *   would be changed by impl while adding to native view
     * * scaleFixed : final scale value, always equal to (scaleForApp * scaleForImpl)
     *   (sizeInZFFramework * scaleFixed) would be the final pixel size that passed to implementation
     *
     * while adding to parent,
     * scaleForApp, scaleForImpl and scaleFixed would be automatically changed to parent's setting
     * (during viewOnAddToParent)
     *
     * while adding to native view,
     * only scaleForImpl would be automatically changed to impl's scale (during nativeViewScaleForImpl),
     * but app's scale would be kept
     *
     * ZFUISysWindow would have its ZFUIRootView's scale setting set from ZFUISysWindow's implementation
     *
     * to change scale, app would use #ZFUIRootView::scaleForAppSet from ZFUISysWindow
     *
     * scaleOnChange would be called each time scaleFixed changed
     */

    ZFUIViewMeasureResult *measureResult;
    zfuint layoutRequestOverrideFlag;
    ZFUIRect layoutedFramePrev;
    ZFUIRect layoutedFrame;
    ZFUIRect nativeImplViewFrame;
    _ZFP_ZFUIViewLayerData layerInternalImpl;
    _ZFP_ZFUIViewLayerData layerInternalBg;
    _ZFP_ZFUIViewLayerData layerNormal;
    _ZFP_ZFUIViewLayerData layerInternalFg;
    enum {
        stateFlag_viewDelegateForParent = 1 << 0,
        stateFlag_layoutRequested = 1 << 1,
        stateFlag_layouting = 1 << 2,
        stateFlag_layoutedFramePrevResetFlag = 1 << 3,
        stateFlag_observerHasAddFlag_viewChildOnChange = 1 << 4,
        stateFlag_observerHasAddFlag_viewChildOnAdd = 1 << 5,
        stateFlag_observerHasAddFlag_viewChildOnRemove = 1 << 6,
        stateFlag_observerHasAddFlag_viewOnAddToParent = 1 << 7,
        stateFlag_observerHasAddFlag_viewOnRemoveFromParent = 1 << 8,
        stateFlag_observerHasAddFlag_layoutOnLayoutRequest = 1 << 9,
        stateFlag_observerHasAddFlag_viewPropertyOnUpdate = 1 << 10,
    };
    zfuint stateFlag;

public:
    _ZFP_ZFUIViewPrivate(void)
    : nativeView(zfnull)
    , nativeImplView(zfnull)
    , nativeImplViewDeleteCallback(zfnull)
    , nativeImplViewMargin(ZFUIMarginZero())
    , viewDelegateParent(zfnull)
    , viewDelegate(zfnull)
    , viewParent(zfnull)
    , viewLayer(ZFUIViewChildLayer::e_Normal)
    , layoutParam(zfnull)
    , serializableRefLayoutParam(zfnull)
    , serializableRefLayoutParamCache()
    , internalViewAutoSerializeTags()
    , scaleForImpl(1)
    , scaleFixed(1)
    , measureResult(zfnull)
    , layoutRequestOverrideFlag(0)
    , layoutedFramePrev(ZFUIRectZero())
    , layoutedFrame(ZFUIRectZero())
    , nativeImplViewFrame(ZFUIRectZero())
    , layerInternalImpl()
    , layerInternalBg()
    , layerNormal()
    , layerInternalFg()
    , stateFlag(0)
    {
        ZFBitSet(this->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_layoutRequested);
    }

public:
    ZFUIView *virtualParent(ZF_IN ZFUIView *parent)
    {
        while(parent->viewDelegateForParent())
        {
            parent = parent->viewParent();
        }
        return parent;
    }
    void layoutParamSet(ZF_IN ZFUIView *owner, ZF_IN ZFUIViewLayoutParam *newLayoutParam)
    {
        ZF_GLOBAL_INITIALIZER_CLASS(ZFUIViewListenerHolder) *listenerHolder = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewListenerHolder);
        zfRetain(newLayoutParam);
        if(newLayoutParam)
        {
            newLayoutParam->observerAdd(
                ZFUILayoutParam::EventLayoutParamOnChange(),
                listenerHolder->layoutParamChangedListener,
                owner->objectHolder());
        }
        if(this->layoutParam)
        {
            this->layoutParam->observerRemove(ZFUILayoutParam::EventLayoutParamOnChange(), listenerHolder->layoutParamChangedListener);
        }
        zfRelease(this->layoutParam);
        this->layoutParam = newLayoutParam;
    }
    zfindex viewLayerPrevCount(ZF_IN _ZFP_ZFUIViewLayerData &layer)
    {
        zfindex nativeImplFix = (this->nativeImplView ? 1 : 0);
        if(&layer == &(this->layerNormal))
        {
            return this->layerInternalImpl.views.count() + nativeImplFix
                + this->layerInternalBg.views.count();
        }
        else if(&layer == &(this->layerInternalImpl))
        {
            return 0;
        }
        else if(&layer == &(this->layerInternalBg))
        {
            return this->layerInternalImpl.views.count() + nativeImplFix;
        }
        else if(&layer == &(this->layerInternalFg))
        {
            return this->layerInternalImpl.views.count() + nativeImplFix
                + this->layerInternalBg.views.count()
                + this->layerNormal.views.count();
        }
        else
        {
            zfCoreCriticalShouldNotGoHere();
            return 0;
        }
    }
    void checkUpdateChildScale(ZF_IN ZFUIView *owner, ZF_IN ZFUIView *child)
    {
        child->_ZFP_ZFUIView_scaleSetRecursively(owner->scaleFixed(), owner->scaleForImpl());
    }
    void childAdd(ZF_IN ZFUIView *owner,
                  ZF_IN ZFUIViewChildLayerEnum childLayer,
                  ZF_IN _ZFP_ZFUIViewLayerData &layer,
                  ZF_IN ZFUIView *view,
                  ZF_IN ZFUIViewLayoutParam *layoutParam,
                  ZF_IN zfindex atIndex)
    {
        zfCoreAssertWithMessageTrim(!ZFBitTest(owner->objectInstanceState(), ZFObjectInstanceStateOnDealloc),
            "[ZFUIView] add child during parent's dealloc is not allowed");
        zfCoreAssertWithMessageTrim(view != zfnull, "[ZFUIView] add null child");
        zfCoreAssertWithMessageTrim(view->viewParent() == zfnull, "[ZFUIView] add child which already has parent, you should remove it first");
        zfRetain(view);

        if(this->serializableRefLayoutParamCache == zfnull)
        {
            this->serializableRefLayoutParamCache = owner->layoutParamCreate();
        }
        view->serializableRefLayoutParamSet(this->serializableRefLayoutParamCache);

        zfbool layoutParamNeedRelease = zffalse;
        if(layoutParam == zfnull)
        {
            layoutParam = view->layoutParam();
        }
        if(layoutParam == zfnull)
        {
            layoutParam = zfRetain(owner->layoutParamCreate().to<ZFUIViewLayoutParam *>());
            layoutParamNeedRelease = zftrue;
        }
        else
        {
            if(!layoutParam->classData()->classIsTypeOf(owner->layoutParamClass()))
            {
                layoutParamNeedRelease = zftrue;
                ZFUIViewLayoutParam *tmp = zfRetain(owner->layoutParamCreate().to<ZFUIViewLayoutParam *>());
                tmp->styleableCopyFrom(layoutParam);
                layoutParam = tmp;
            }
        }
        view->_ZFP_ZFUIView_parentChanged(owner, layoutParam, childLayer);
        if(atIndex == zfindexMax())
        {
            atIndex = layer.views.count();
        }
        layer.views.add(atIndex, view);
        owner->implChildOnAdd(view, this->viewLayerPrevCount(layer) + atIndex, childLayer, atIndex);

        owner->layoutRequest();
        view->layoutRequest();

        if(layoutParamNeedRelease)
        {
            zfRelease(layoutParam);
        }

        this->checkUpdateChildScale(owner, view);
        ZFUIView *virtualParent = this->virtualParent(owner);
        virtualParent->viewChildOnAdd(view, childLayer);
        view->viewOnAddToParent(virtualParent);
        virtualParent->viewChildOnChange();
    }
    void childRemove(ZF_IN ZFUIView *owner,
                     ZF_IN ZFUIViewChildLayerEnum childLayer,
                     ZF_IN _ZFP_ZFUIViewLayerData &layer,
                     ZF_IN ZFUIView *view)
    {
        zfindex index = this->childFind(layer, view);
        if(index != zfindexMax())
        {
            this->childRemoveAtIndex(owner, childLayer, layer, index);
        }
    }
    void childRemoveAtIndex(ZF_IN ZFUIView *owner,
                            ZF_IN ZFUIViewChildLayerEnum childLayer,
                            ZF_IN _ZFP_ZFUIViewLayerData &layer,
                            ZF_IN zfindex index)
    {
        ZFUIView *child = layer.views.get(index);

        child->_ZFP_ZFUIView_parentChanged(zfnull, zfnull, ZFUIViewChildLayer::e_Normal);
        layer.views.remove(index);
        owner->implChildOnRemove(child, this->viewLayerPrevCount(layer) + index, childLayer, index);

        owner->layoutRequest();

        ZFUIView *virtualParent = this->virtualParent(owner);
        virtualParent->viewChildOnRemove(child, childLayer);
        child->viewOnRemoveFromParent(virtualParent);
        virtualParent->viewChildOnChange();
        zfRelease(child);
    }
    void childRemoveAll(ZF_IN ZFUIView *owner,
                        ZF_IN ZFUIViewChildLayerEnum childLayer,
                        ZF_IN _ZFP_ZFUIViewLayerData &layer)
    {
        if(layer.views.isEmpty())
        {
            return ;
        }

        owner->layoutRequest();

        ZFCoreArrayPOD<ZFUIView *> tmp = layer.views;
        layer.views = ZFCoreArrayPOD<ZFUIView *>();

        zfindex prevLayerCount = this->viewLayerPrevCount(layer);
        for(zfindex i = tmp.count() - 1; i != zfindexMax(); --i)
        {
            tmp[i]->_ZFP_ZFUIView_parentChanged(zfnull, zfnull, ZFUIViewChildLayer::e_Normal);
            owner->implChildOnRemove(tmp[i], prevLayerCount + i, childLayer, i);
        }

        ZFUIView *virtualParent = this->virtualParent(owner);
        for(zfindex i = tmp.count() - 1; i != zfindexMax(); --i)
        {
            ZFUIView *child = tmp[i];
            virtualParent->viewChildOnRemove(child, childLayer);
            child->viewOnRemoveFromParent(virtualParent);
            zfRelease(child);
        }

        virtualParent->viewChildOnChange();
    }
    void childMove(ZF_IN ZFUIView *owner,
                   ZF_IN ZFUIViewChildLayerEnum childLayer,
                   ZF_IN _ZFP_ZFUIViewLayerData &layer,
                   ZF_IN zfindex fromIndex,
                   ZF_IN zfindex toIndexOrIndexMax)
    {
        if(toIndexOrIndexMax == zfindexMax())
        {
            toIndexOrIndexMax = this->childCount(layer) - 1;
        }
        if(fromIndex >= this->childCount(layer))
        {
            zfCoreCriticalIndexOutOfRange(fromIndex, this->childCount(layer));
            return ;
        }
        if(toIndexOrIndexMax >= this->childCount(layer))
        {
            zfCoreCriticalIndexOutOfRange(fromIndex, this->childCount(layer));
            return ;
        }
        if(fromIndex == toIndexOrIndexMax)
        {
            return ;
        }

        ZFUIView *child = layer.views.get(fromIndex);
        layer.views.move(fromIndex, toIndexOrIndexMax);

        zfindex prevLayerCount = this->viewLayerPrevCount(layer);
        owner->implChildOnRemove(child, prevLayerCount + fromIndex, childLayer, fromIndex);
        owner->implChildOnAdd(child, prevLayerCount + toIndexOrIndexMax, childLayer, toIndexOrIndexMax);

        ZFUIView *virtualParent = this->virtualParent(owner);
        virtualParent->viewChildOnChange();
    }
    void childMove(ZF_IN ZFUIView *owner,
                   ZF_IN ZFUIViewChildLayerEnum childLayer,
                   ZF_IN _ZFP_ZFUIViewLayerData &layer,
                   ZF_IN ZFUIView *child,
                   ZF_IN zfindex toIndexOrIndexMax)
    {
        zfindex fromIndex = this->childFind(layer, child);
        if(fromIndex != zfindexMax())
        {
            this->childMove(owner, childLayer, layer, fromIndex, toIndexOrIndexMax);
        }
    }
    void childReplace(ZF_IN ZFUIView *owner,
                      ZF_IN ZFUIViewChildLayerEnum childLayer,
                      ZF_IN _ZFP_ZFUIViewLayerData &layer,
                      ZF_IN zfindex atIndex,
                      ZF_IN ZFUIView *toReplace)
    {
        zfCoreAssert(atIndex < layer.views.count());
        zfCoreAssert(toReplace != zfnull);
        zfCoreAssert(toReplace->viewParent() == zfnull);

        ZFUIView *old = layer.views[atIndex];
        layer.views[atIndex] = toReplace;
        zfindex fixedIndex = this->viewLayerPrevCount(layer) + atIndex;
        owner->implChildOnRemove(old, fixedIndex, childLayer, atIndex);
        owner->implChildOnAdd(toReplace, fixedIndex, childLayer, atIndex);

        this->checkUpdateChildScale(owner, toReplace);
        toReplace->_ZFP_ZFUIView_parentChanged(owner, old->layoutParam(), childLayer);
        old->_ZFP_ZFUIView_parentChanged(zfnull, zfnull, ZFUIViewChildLayer::e_Normal);

        ZFUIView *virtualParent = this->virtualParent(owner);
        virtualParent->viewChildOnRemove(old, childLayer);
        old->viewOnRemoveFromParent(virtualParent);
        virtualParent->viewChildOnAdd(toReplace, childLayer);
        toReplace->viewOnAddToParent(virtualParent);
        virtualParent->viewChildOnChange();

        zfRelease(old);
    }

    zfindex childCount(ZF_IN _ZFP_ZFUIViewLayerData &layer)
    {
        return layer.views.count();
    }
    ZFUIView *childAtIndex(ZF_IN _ZFP_ZFUIViewLayerData &layer,
                           ZF_IN zfindex index)
    {
        return layer.views.get(index);
    }
    zfindex childFind(ZF_IN _ZFP_ZFUIViewLayerData &layer,
                      ZF_IN ZFUIView *view)
    {
        return layer.views.find(view);
    }
    zfbool childArrayIsTheSame(ZF_IN ZFUIView *view0,
                               ZF_IN ZFUIView *view1,
                               ZF_IN ZFUIViewChildLayerEnum layer)
    {
        const ZFCoreArrayPOD<ZFUIView *> *children0 = zfnull;
        const ZFCoreArrayPOD<ZFUIView *> *children1 = zfnull;
        switch(layer)
        {
            case ZFUIViewChildLayer::e_Normal:
                children0 = &(view0->d->layerNormal.views);
                children1 = &(view1->d->layerNormal.views);
                break;
            case ZFUIViewChildLayer::e_InternalImpl:
                children0 = &(view0->d->layerInternalImpl.views);
                children1 = &(view1->d->layerInternalImpl.views);
                break;
            case ZFUIViewChildLayer::e_InternalBg:
                children0 = &(view0->d->layerInternalBg.views);
                children1 = &(view1->d->layerInternalBg.views);
                break;
            case ZFUIViewChildLayer::e_InternalFg:
                children0 = &(view0->d->layerInternalFg.views);
                children1 = &(view1->d->layerInternalFg.views);
                break;
            default:
                zfCoreCriticalShouldNotGoHere();
                return zffalse;
        }

        if(children0->count() != children1->count())
        {
            return zffalse;
        }
        for(zfindex i = children0->count() - 1; i != zfindexMax(); --i)
        {
            if(ZFObjectCompare(children0->get(i), children1->get(i)) != ZFCompareTheSame)
            {
                return zffalse;
            }
        }
        return zftrue;
    }
    ZFUIViewLayoutParam *childLayoutParamAtIndex(ZF_IN _ZFP_ZFUIViewLayerData &layer,
                                                 ZF_IN zfindex index)
    {
        return layer.views.get(index)->layoutParam();
    }

    zfbool serializeInternalViewFromCategoryData(ZF_IN ZFUIView *owner,
                                                 ZF_IN ZFUIViewChildLayerEnum childLayer,
                                                 ZF_IN const ZFSerializableData &categoryData,
                                                 ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */,
                                                 ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */)
    {
        zfautoObject internalView;
        if(!ZFObjectFromData(internalView, categoryData, outErrorHint, outErrorPos))
        {
            return zffalse;
        }
        if(internalView == zfnull)
        {
            ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, categoryData,
                "null view");
            return zffalse;
        }
        if(!internalView.toObject()->classData()->classIsTypeOf(ZFUIView::ClassData()))
        {
            ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, categoryData,
                "%s not type of %s",
                internalView.toObject()->objectInfoOfInstance().cString(), ZFUIView::ClassData()->classNameFull());
            return zffalse;
        }
        ZFUIView *internalViewTmp = internalView.to<ZFUIView *>();
        if(internalViewTmp->viewId().isEmpty())
        {
            ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, categoryData,
                "auto serialized internal view %s has no viewId",
                internalViewTmp->objectInfoOfInstance().cString());
            return zffalse;
        }
        ZFUIView *exist = zfnull;
        const ZFCoreArrayPOD<ZFUIView *> *views = zfnull;
        switch(childLayer)
        {
            case ZFUIViewChildLayer::e_Normal:
                zfCoreCriticalShouldNotGoHere();
                break;
            case ZFUIViewChildLayer::e_InternalImpl:
                views = &(this->layerInternalImpl.views);
                break;
            case ZFUIViewChildLayer::e_InternalBg:
                views = &(this->layerInternalBg.views);
                break;
            case ZFUIViewChildLayer::e_InternalFg:
                views = &(this->layerInternalFg.views);
                break;
            default:
                zfCoreCriticalShouldNotGoHere();
                return zffalse;
        }
        for(zfindex i = views->count() - 1; i != zfindexMax(); --i)
        {
            ZFUIView *tmp = views->get(i)->to<ZFUIView *>();
            if(tmp->viewId().compare(internalViewTmp->viewId()) == 0)
            {
                exist = tmp;
                break;
            }
        }
        if(exist == zfnull)
        {
            owner->internalViewAutoSerializeTagAdd(internalViewTmp->viewId().cString());
            switch(childLayer)
            {
                case ZFUIViewChildLayer::e_Normal:
                    zfCoreCriticalShouldNotGoHere();
                    break;
                case ZFUIViewChildLayer::e_InternalImpl:
                    owner->internalImplViewAdd(internalViewTmp);
                    break;
                case ZFUIViewChildLayer::e_InternalBg:
                    owner->internalBgViewAdd(internalViewTmp);
                    break;
                case ZFUIViewChildLayer::e_InternalFg:
                    owner->internalFgViewAdd(internalViewTmp);
                    break;
                default:
                    zfCoreCriticalShouldNotGoHere();
                    return zffalse;
            }
        }
        else
        {
            exist->styleableCopyFrom(internalViewTmp);
        }
        return zftrue;
    }
    zfbool serializeInternalViewToCategoryData(ZF_IN ZFUIViewChildLayerEnum childLayer,
                                               ZF_IN_OUT ZFSerializableData &serializableData,
                                               ZF_IN ZFUIView *ref,
                                               ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */)
    {
        const ZFCoreArrayPOD<ZFUIView *> *views = zfnull;
        const ZFCoreArrayPOD<ZFUIView *> *viewsRef = zfnull;
        const zfchar *categoryTag = zfnull;
        switch(childLayer)
        {
            case ZFUIViewChildLayer::e_Normal:
                zfCoreCriticalShouldNotGoHere();
                break;
            case ZFUIViewChildLayer::e_InternalImpl:
                views = &(this->layerInternalImpl.views);
                if(ref != zfnull)
                {
                    viewsRef = &(ref->d->layerInternalImpl.views);
                }
                break;
                categoryTag = ZFSerializableKeyword_ZFUIView_internalImplView;
            case ZFUIViewChildLayer::e_InternalBg:
                views = &(this->layerInternalBg.views);
                if(ref != zfnull)
                {
                    viewsRef = &(ref->d->layerInternalBg.views);
                }
                categoryTag = ZFSerializableKeyword_ZFUIView_internalBgView;
                break;
            case ZFUIViewChildLayer::e_InternalFg:
                views = &(this->layerInternalFg.views);
                if(ref != zfnull)
                {
                    viewsRef = &(ref->d->layerInternalFg.views);
                }
                categoryTag = ZFSerializableKeyword_ZFUIView_internalFgView;
                break;
            default:
                zfCoreCriticalShouldNotGoHere();
                return zffalse;
        }
        if(ref == zfnull || viewsRef->count() == 0)
        {
            for(zfindex i = 0; i < views->count(); ++i)
            {
                ZFUIView *tmp = views->get(i)->to<ZFUIView *>();
                if(tmp->viewId().isEmpty()
                    || this->internalViewAutoSerializeTags.find(tmp->viewId().cString()) == this->internalViewAutoSerializeTags.end())
                {
                    continue;
                }
                ZFSerializableData childData;
                if(!ZFObjectToData(childData, tmp, outErrorHint))
                {
                    return zffalse;
                }
                childData.categorySet(categoryTag);
                serializableData.elementAdd(childData);
            }
        }
        else
        {
            for(zfindex i = 0; i < views->count(); ++i)
            {
                ZFUIView *tmp = views->get(i)->to<ZFUIView *>();
                if(tmp->viewId().isEmpty()
                    || this->internalViewAutoSerializeTags.find(tmp->viewId().cString()) == this->internalViewAutoSerializeTags.end())
                {
                    continue;
                }
                zfbool exist = zffalse;
                for(zfindex iRef = viewsRef->count() - 1; iRef != zfindexMax(); --iRef)
                {
                    if(viewsRef->get(iRef)->to<ZFUIView *>()->viewId().compare(tmp->viewId().cString()) == 0)
                    {
                        exist = zftrue;
                        break;
                    }
                }
                if(exist)
                {
                    continue;
                }
                ZFSerializableData childData;
                if(!ZFObjectToData(childData, tmp, outErrorHint))
                {
                    return zffalse;
                }
                childData.categorySet(categoryTag);
                serializableData.elementAdd(childData);
            }
        }
        return zftrue;
    }
};

static zfuint _ZFP_ZFUIView_stateFlags = 0;
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIView_stateFlags, ZFLevelZFFrameworkStatic)
{
    ZFObjectGlobalEventObserver().observerHasAddStateAttach(ZFUIView::EventViewChildOnChange(), &_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_viewChildOnChange);
    ZFObjectGlobalEventObserver().observerHasAddStateAttach(ZFUIView::EventViewChildOnAdd(), &_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_viewChildOnAdd);
    ZFObjectGlobalEventObserver().observerHasAddStateAttach(ZFUIView::EventViewChildOnRemove(), &_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_viewChildOnRemove);
    ZFObjectGlobalEventObserver().observerHasAddStateAttach(ZFUIView::EventViewOnAddToParent(), &_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_viewOnAddToParent);
    ZFObjectGlobalEventObserver().observerHasAddStateAttach(ZFUIView::EventViewOnRemoveFromParent(), &_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_viewOnRemoveFromParent);
    ZFObjectGlobalEventObserver().observerHasAddStateAttach(ZFUIView::EventViewLayoutOnLayoutRequest(), &_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_layoutOnLayoutRequest);
    ZFObjectGlobalEventObserver().observerHasAddStateAttach(ZFUIView::EventViewPropertyOnUpdate(), &_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_viewPropertyOnUpdate);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIView_stateFlags)
{
    ZFObjectGlobalEventObserver().observerHasAddStateDetach(ZFUIView::EventViewChildOnChange(), &_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_viewChildOnChange);
    ZFObjectGlobalEventObserver().observerHasAddStateDetach(ZFUIView::EventViewChildOnAdd(), &_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_viewChildOnAdd);
    ZFObjectGlobalEventObserver().observerHasAddStateDetach(ZFUIView::EventViewChildOnRemove(), &_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_viewChildOnRemove);
    ZFObjectGlobalEventObserver().observerHasAddStateDetach(ZFUIView::EventViewOnAddToParent(), &_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_viewOnAddToParent);
    ZFObjectGlobalEventObserver().observerHasAddStateDetach(ZFUIView::EventViewOnRemoveFromParent(), &_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_viewOnRemoveFromParent);
    ZFObjectGlobalEventObserver().observerHasAddStateDetach(ZFUIView::EventViewLayoutOnLayoutRequest(), &_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_layoutOnLayoutRequest);
    ZFObjectGlobalEventObserver().observerHasAddStateDetach(ZFUIView::EventViewPropertyOnUpdate(), &_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_viewPropertyOnUpdate);
}
ZF_GLOBAL_INITIALIZER_END(ZFUIView_stateFlags)

// ============================================================
// ZFUIView
ZFOBJECT_REGISTER(ZFUIView)

ZFOBSERVER_EVENT_REGISTER(ZFUIView, ViewChildOnChange)
ZFOBSERVER_EVENT_REGISTER(ZFUIView, ViewChildOnAdd)
ZFOBSERVER_EVENT_REGISTER(ZFUIView, ViewChildOnRemove)
ZFOBSERVER_EVENT_REGISTER(ZFUIView, ViewOnAddToParent)
ZFOBSERVER_EVENT_REGISTER(ZFUIView, ViewOnRemoveFromParent)
ZFOBSERVER_EVENT_REGISTER(ZFUIView, ViewScaleOnChange)
ZFOBSERVER_EVENT_REGISTER(ZFUIView, ViewFocusOnChange)
ZFOBSERVER_EVENT_REGISTER(ZFUIView, ViewOnEvent)
ZFOBSERVER_EVENT_REGISTER(ZFUIView, ViewLayoutOnLayoutRequest)
ZFOBSERVER_EVENT_REGISTER(ZFUIView, ViewLayoutOnMeasureFinish)
ZFOBSERVER_EVENT_REGISTER(ZFUIView, ViewLayoutOnLayoutPrepare)
ZFOBSERVER_EVENT_REGISTER(ZFUIView, ViewLayoutOnLayout)
ZFOBSERVER_EVENT_REGISTER(ZFUIView, ViewLayoutOnLayoutFinish)
ZFOBSERVER_EVENT_REGISTER(ZFUIView, NativeImplViewMarginOnUpdate)
ZFOBSERVER_EVENT_REGISTER(ZFUIView, ViewPropertyOnUpdate)

// ============================================================
// serialize
void ZFUIView::serializableRefLayoutParamSet(ZF_IN ZFUIViewLayoutParam *serializableRefLayoutParam)
{
    zfRetain(serializableRefLayoutParam);
    zfRelease(d->serializableRefLayoutParam);
    d->serializableRefLayoutParam = serializableRefLayoutParam;
}
ZFUIViewLayoutParam *ZFUIView::serializableRefLayoutParam(void)
{
    return d->serializableRefLayoutParam;
}

zfbool ZFUIView::serializableOnSerializeFromData(ZF_IN const ZFSerializableData &serializableData,
                                                 ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */,
                                                 ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */)
{
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeFromData(serializableData, outErrorHint, outErrorPos)) {return zffalse;}

    if(this->serializableOnCheckNeedSerializeChildren())
    {
        this->childRemoveAll();
    }
    d->layoutParamSet(this, zfnull);

    for(zfindex i = 0; i < serializableData.elementCount(); ++i)
    {
        const ZFSerializableData &categoryData = serializableData.elementAtIndex(i);
        if(categoryData.resolved()) {continue;}
        const zfchar *category = ZFSerializableUtil::checkCategory(categoryData);
        if(category == zfnull) {continue;}

        if(zfscmpTheSame(category, ZFSerializableKeyword_ZFUIView_child))
        {
            zfautoObject element;
            if(!ZFObjectFromData(element, categoryData, outErrorHint, outErrorPos))
            {
                return zffalse;
            }
            if(element == zfnull)
            {
                ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, categoryData,
                    "null view");
                return zffalse;
            }
            if(!element.toObject()->classData()->classIsTypeOf(ZFUIView::ClassData()))
            {
                ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, categoryData,
                    "%s not type of %s",
                    element.toObject()->objectInfoOfInstance().cString(), ZFUIView::ClassData()->classNameFull());
                return zffalse;
            }
            ZFUIView *child = element.to<ZFUIView *>();
            this->childAdd(child, child->layoutParam());

            categoryData.resolveMark();
        }
        else if(zfscmpTheSame(category, ZFSerializableKeyword_ZFUIView_layoutParam))
        {
            zfautoObject layoutParam;
            if(!ZFObjectFromData(layoutParam, categoryData, outErrorHint, outErrorPos))
            {
                return zffalse;
            }
            if(layoutParam == zfnull)
            {
                ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, categoryData,
                    "null layoutParam");
                return zffalse;
            }
            if(!layoutParam.toObject()->classData()->classIsTypeOf(ZFUIViewLayoutParam::ClassData()))
            {
                ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, categoryData,
                    "%s not type of %s",
                    layoutParam.toObject()->objectInfoOfInstance().cString(), ZFUIViewLayoutParam::ClassData()->classNameFull());
                return zffalse;
            }

            d->layoutParamSet(this, layoutParam);
        }
        else if(zfscmpTheSame(category, ZFSerializableKeyword_ZFUIView_internalImplView))
        {
            if(!d->serializeInternalViewFromCategoryData(this,
                ZFUIViewChildLayer::e_InternalImpl,
                categoryData,
                outErrorHint,
                outErrorPos))
            {
                return zffalse;
            }
        }
        else if(zfscmpTheSame(category, ZFSerializableKeyword_ZFUIView_internalBgView))
        {
            if(!d->serializeInternalViewFromCategoryData(this,
                ZFUIViewChildLayer::e_InternalBg,
                categoryData,
                outErrorHint,
                outErrorPos))
            {
                return zffalse;
            }
        }
        else if(zfscmpTheSame(category, ZFSerializableKeyword_ZFUIView_internalFgView))
        {
            if(!d->serializeInternalViewFromCategoryData(this,
                ZFUIViewChildLayer::e_InternalFg,
                categoryData,
                outErrorHint,
                outErrorPos))
            {
                return zffalse;
            }
        }
    }
    return zftrue;
}
zfbool ZFUIView::serializableOnSerializeToData(ZF_IN_OUT ZFSerializableData &serializableData,
                                               ZF_IN ZFSerializable *referencedOwnerOrNull,
                                               ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */)
{
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeToData(serializableData, referencedOwnerOrNull, outErrorHint)) {return zffalse;}
    zfself *ref = ZFCastZFObject(zfself *, referencedOwnerOrNull);

    // layoutParam
    if(d->layoutParam != zfnull && this->viewParent() != zfnull)
    {
        ZFUIViewLayoutParam *refLayoutParam = ((ref == zfnull) ? zfnull : ref->d->layoutParam);
        if(refLayoutParam == zfnull)
        {
            refLayoutParam = d->serializableRefLayoutParam;
        }
        if(refLayoutParam != zfnull)
        {
            ZFSerializableData categoryData;
            if(!ZFObjectToData(categoryData, d->layoutParam, outErrorHint, refLayoutParam))
            {
                return zffalse;
            }
            if(categoryData.attributeCount() > 0 || categoryData.elementCount() > 0)
            {
                categoryData.categorySet(ZFSerializableKeyword_ZFUIView_layoutParam);
                serializableData.elementAdd(categoryData);
            }
        }
    }

    { // internal views
        if(!d->internalViewAutoSerializeTags.empty())
        {
            if(!d->serializeInternalViewToCategoryData(ZFUIViewChildLayer::e_InternalImpl, serializableData, ref, outErrorHint))
            {
                return zffalse;
            }
            if(!d->serializeInternalViewToCategoryData(ZFUIViewChildLayer::e_InternalBg, serializableData, ref, outErrorHint))
            {
                return zffalse;
            }
            if(!d->serializeInternalViewToCategoryData(ZFUIViewChildLayer::e_InternalFg, serializableData, ref, outErrorHint))
            {
                return zffalse;
            }
        }
    }

    // all children
    if(this->serializableOnCheckNeedSerializeChildren())
    {
        if(ref == zfnull || ref->childCount() == 0)
        {
            for(zfindex i = 0, count = this->childCount(); i < count; ++i)
            {
                ZFSerializableData childData;
                if(!ZFObjectToData(childData, this->childAtIndex(i), outErrorHint))
                {
                    return zffalse;
                }
                childData.categorySet(ZFSerializableKeyword_ZFUIView_child);
                serializableData.elementAdd(childData);
            }
        }
        else
        {
            if(!d->childArrayIsTheSame(this, ref, ZFUIViewChildLayer::e_Normal))
            {
                ZFSerializableUtil::errorOccurred(outErrorHint,
                    "child mismatch, this: %s, ref: %s",
                    this->objectInfoOfInstance().cString(), ref->objectInfoOfInstance().cString());
                return zffalse;
            }
        }
    }
    return zftrue;
}

// ============================================================
// properties
ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFUIView, zfstring, viewDelegateClass)
{
    const ZFClass *cls = ZFClass::classForName(this->viewDelegateClass());
    zfautoObject viewDelegateTmp;
    if(cls != zfnull)
    {
        zfCoreAssertWithMessageTrim(this->viewDelegateSupported(),
            "[ZFUIView] viewDelegate \"%s\" not supported for view: %s",
            this->viewDelegateClass().cString(),
            this->objectInfo().cString());

        viewDelegateTmp = cls->newInstance();
    }
    this->viewDelegateSet(viewDelegateTmp);
}
ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFUIView, zfbool, viewVisible)
{
    if(this->viewVisible() != propertyValueOld)
    {
        ZFPROTOCOL_ACCESS(ZFUIView)->viewVisibleSet(this, this->viewVisible());
        this->layoutRequest();
    }
}
ZFPROPERTY_OVERRIDE_ON_VERIFY_DEFINE(ZFUIView, zffloat, viewAlpha)
{
    propertyValue = zfmApplyRange<zffloat>(propertyValue, 0, 1);
}
ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFUIView, zffloat, viewAlpha)
{
    if(propertyValue != propertyValueOld)
    {
        ZFPROTOCOL_ACCESS(ZFUIView)->viewAlphaSet(this, this->viewAlpha());
    }
}
ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFUIView, zfbool, viewFocusable)
{
    ZFPROTOCOL_INTERFACE_CLASS(ZFUIViewFocus) *impl = ZFPROTOCOL_TRY_ACCESS(ZFUIViewFocus);
    if(impl != zfnull)
    {
        impl->viewFocusableSet(this, this->viewFocusable());
    }
}
ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFUIView, zfbool, viewUIEnable)
{
    ZFPROTOCOL_ACCESS(ZFUIView)->viewUIEnableSet(this, this->viewUIEnable());
}
ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFUIView, zfbool, viewUIEnableTree)
{
    ZFPROTOCOL_ACCESS(ZFUIView)->viewUIEnableTreeSet(this, this->viewUIEnableTree());
}
ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFUIView, zfbool, viewMouseHoverEventEnable)
{
    ZFPROTOCOL_ACCESS(ZFUIView)->viewMouseHoverEventEnableSet(this, this->viewMouseHoverEventEnable());
}

ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFUIView, ZFUISize, viewSizePrefered)
{
    if(this->viewSizePrefered() != propertyValueOld)
    {
        this->layoutRequest();
    }
}
ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFUIView, ZFUISize, viewSizeMin)
{
    if(this->viewSizeMin() != propertyValueOld)
    {
        this->layoutRequest();
    }
}
ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFUIView, ZFUISize, viewSizeMax)
{
    if(this->viewSizeMax() != propertyValueOld)
    {
        this->layoutRequest();
    }
}
ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFUIView, ZFUIColor, viewBackgroundColor)
{
    ZFPROTOCOL_ACCESS(ZFUIView)->viewBackgroundColorSet(this, this->viewBackgroundColor());
}

// ============================================================
// init and dealloc
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIViewNativeViewCache, ZFLevelZFFrameworkNormal)
{
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIViewNativeViewCache)
{
    while(!this->nativeViewCache.isEmpty())
    {
        ZFPROTOCOL_ACCESS(ZFUIView)->nativeViewDestroy(this->nativeViewCache.removeLastAndGet());
    }
}
ZFCoreArrayPOD<void *> nativeViewCache;
ZF_GLOBAL_INITIALIZER_END(ZFUIViewNativeViewCache)

void ZFUIView::objectOnInit(void)
{
    zfsuper::objectOnInit();

    d = zfpoolNew(_ZFP_ZFUIViewPrivate);

    d->measureResult = zflockfree_zfAllocWithCache(ZFUIViewMeasureResult);
    if(ZFFrameworkStateCheck(ZFLevelZFFrameworkNormal) != ZFFrameworkStateAvailable)
    {
        d->nativeView = ZFPROTOCOL_ACCESS(ZFUIView)->nativeViewCreate(this);
    }
    else
    {
        ZFCoreArrayPOD<void *> &nativeViewCache = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewNativeViewCache)->nativeViewCache;
        if(nativeViewCache.isEmpty())
        {
            d->nativeView = ZFPROTOCOL_ACCESS(ZFUIView)->nativeViewCreate(this);
        }
        else
        {
            d->nativeView = nativeViewCache.removeLastAndGet();
            ZFPROTOCOL_ACCESS(ZFUIView)->nativeViewCacheOnRestore(this, d->nativeView);
        }
    }
    d->scaleForImpl = ZFPROTOCOL_ACCESS(ZFUIView)->nativeViewScaleForImpl(d->nativeView);
    d->scaleFixed = d->scaleForImpl;
}
void ZFUIView::objectOnDealloc(void)
{
    if(d->viewDelegate != zfnull)
    {
        ZFUIView *viewDelegate = d->viewDelegate;
        d->viewDelegate = zfnull;
        ZFBitUnset(viewDelegate->d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_viewDelegateForParent);
        zfRelease(viewDelegate);
    }

    this->nativeImplViewSet(zfnull, zfnull);
    if(ZFFrameworkStateCheck(ZFLevelZFFrameworkNormal) == ZFFrameworkStateAvailable
        && ZFPROTOCOL_ACCESS(ZFUIView)->nativeViewCacheOnSave(d->nativeView)
        && ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewNativeViewCache)->nativeViewCache.count() < 100)
    {
        ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewNativeViewCache)->nativeViewCache.add(d->nativeView);
    }
    else
    {
        ZFPROTOCOL_ACCESS(ZFUIView)->nativeViewDestroy(d->nativeView);
    }
    d->nativeView = zfnull;

    for(zfindex i = d->layerNormal.views.count() - 1; i != zfindexMax(); --i)
    {
        zfRelease(d->layerNormal.views.get(i));
    }
    for(zfindex i = d->layerInternalImpl.views.count() - 1; i != zfindexMax(); --i)
    {
        zfRelease(d->layerInternalImpl.views.get(i));
    }
    for(zfindex i = d->layerInternalBg.views.count() - 1; i != zfindexMax(); --i)
    {
        zfRelease(d->layerInternalBg.views.get(i));
    }
    for(zfindex i = d->layerInternalFg.views.count() - 1; i != zfindexMax(); --i)
    {
        zfRelease(d->layerInternalFg.views.get(i));
    }
    d->layoutParamSet(this, zfnull);
    zfRelease(d->measureResult);
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}
void ZFUIView::objectOnInitFinish(void)
{
    zfsuper::objectOnInitFinish();
    this->viewPropertyOnUpdate();
}
void ZFUIView::objectOnDeallocPrepare(void)
{
    ZFThreadTaskCancelWithOwner(this);

    if(this->viewDelegate() == zfnull)
    { // directly remove all children, better performance
        this->implChildOnRemoveAllForDealloc();

        ZFCoreArrayPOD<ZFUIView *> &layerNormal = d->layerNormal.views;
        ZFCoreArrayPOD<ZFUIView *> &layerInternalFg = d->layerInternalFg.views;
        ZFCoreArrayPOD<ZFUIView *> &layerInternalBg = d->layerInternalBg.views;
        ZFCoreArrayPOD<ZFUIView *> &layerInternalImpl = d->layerInternalImpl.views;

        for(zfindex i = layerNormal.count() - 1; i != zfindexMax(); --i)
        {
            ZFUIView *child = layerNormal[i];
            child->_ZFP_ZFUIView_parentChanged(zfnull, zfnull, ZFUIViewChildLayer::e_Normal);
            this->viewChildOnRemove(child, ZFUIViewChildLayer::e_Normal);
            child->viewOnRemoveFromParent(this);
            zfRelease(child);
        }
        for(zfindex i = layerInternalFg.count() - 1; i != zfindexMax(); --i)
        {
            ZFUIView *child = layerInternalFg[i];
            child->_ZFP_ZFUIView_parentChanged(zfnull, zfnull, ZFUIViewChildLayer::e_Normal);
            this->viewChildOnRemove(child, ZFUIViewChildLayer::e_InternalFg);
            child->viewOnRemoveFromParent(this);
            zfRelease(child);
        }
        for(zfindex i = layerInternalBg.count() - 1; i != zfindexMax(); --i)
        {
            ZFUIView *child = layerInternalBg[i];
            child->_ZFP_ZFUIView_parentChanged(zfnull, zfnull, ZFUIViewChildLayer::e_Normal);
            this->viewChildOnRemove(child, ZFUIViewChildLayer::e_InternalBg);
            child->viewOnRemoveFromParent(this);
            zfRelease(child);
        }
        for(zfindex i = layerInternalImpl.count() - 1; i != zfindexMax(); --i)
        {
            ZFUIView *child = layerInternalImpl[i];
            child->_ZFP_ZFUIView_parentChanged(zfnull, zfnull, ZFUIViewChildLayer::e_Normal);
            this->viewChildOnRemove(child, ZFUIViewChildLayer::e_InternalImpl);
            child->viewOnRemoveFromParent(this);
            zfRelease(child);
        }

        layerNormal.removeAll();
        layerInternalFg.removeAll();
        layerInternalBg.removeAll();
        layerInternalImpl.removeAll();
        this->viewChildOnChange();
    }
    else
    { // normal remove, one by one, worse performance
        // normal child should be removed first due to viewDelegate
        this->childRemoveAll();
        d->childRemoveAll(this, ZFUIViewChildLayer::e_InternalFg, d->layerInternalFg);
        d->childRemoveAll(this, ZFUIViewChildLayer::e_InternalBg, d->layerInternalBg);
        d->childRemoveAll(this, ZFUIViewChildLayer::e_InternalImpl, d->layerInternalImpl);
    }

    zfsuper::objectOnDeallocPrepare();
}

zfidentity ZFUIView::objectHash(void)
{
    zfidentity hash = zfidentityHash(zfidentityCalcString(this->classData()->classNameFull()) , this->childCount());
    for(zfindex i = 0; i < this->childCount(); ++i)
    {
        hash = zfidentityHash(hash, this->childAtIndex(i)->objectHash());
    }
    return hash;
}
ZFCompareResult ZFUIView::objectCompare(ZF_IN ZFObject *anotherObj)
{
    if(this == anotherObj) {return ZFCompareTheSame;}
    zfself *another = ZFCastZFObject(zfself *, anotherObj);
    if(another == zfnull) {return ZFCompareUncomparable;}

    if(this->childCount() != another->childCount())
    {
        return ZFCompareUncomparable;
    }
    if(ZFObjectCompare(d->layoutParam, another->d->layoutParam) != ZFCompareTheSame)
    {
        if((d->layoutParam == zfnull || ZFObjectCompare(d->layoutParam, d->serializableRefLayoutParam) == ZFCompareTheSame)
            != (another->d->layoutParam == zfnull || ZFObjectCompare(another->d->layoutParam, another->d->serializableRefLayoutParam) == ZFCompareTheSame))
        {
            return ZFCompareUncomparable;
        }
    }
    if(!ZFClassUtil::allPropertyIsEqual(this, another)
        || !d->childArrayIsTheSame(this, another, ZFUIViewChildLayer::e_InternalImpl)
        || !d->childArrayIsTheSame(this, another, ZFUIViewChildLayer::e_InternalBg)
        || !d->childArrayIsTheSame(this, another, ZFUIViewChildLayer::e_InternalFg)
        || !d->childArrayIsTheSame(this, another, ZFUIViewChildLayer::e_Normal))
    {
        return ZFCompareUncomparable;
    }
    return ZFCompareTheSame;
}

static void _ZFP_ZFUIView_objectInfo_viewDelegate(ZF_IN_OUT zfstring &ret, ZF_IN ZFUIView *view)
{
    if(view->viewDelegate() == zfnull)
    {
        return ;
    }
    zfindex count = 1;
    {
        ZFUIView *parent = view;
        while(parent->viewDelegateForParent())
        {
            ++count;
            parent = parent->viewParent();
        }
    }
    ret += "\n";
    for(zfindex i = 0; i < count; ++i)
    {
        ret += "    ";
    }
    ret += "(delegate) ";
    view->viewDelegate()->objectInfoT(ret);
}
void ZFUIView::objectInfoT(ZF_IN_OUT zfstring &ret)
{
    zfsuper::objectInfoT(ret);
    _ZFP_ZFUIView_objectInfo_viewDelegate(ret, this);
}
void ZFUIView::objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
{
    zfsuper::objectInfoOnAppend(ret);

    if(!this->viewId().isEmpty())
    {
        zfstringAppend(ret, " [%s]", this->viewId().cString());
    }

    ret += " ";
    ZFUIRectToString(ret, this->layoutedFrame());

    if(!this->viewVisible())
    {
        ret += " Hidden";
    }

    if(!this->viewUIEnableTree())
    {
        ret += " UITreeDisabled";
    }
    else if(!this->viewUIEnable())
    {
        ret += " UIDisabled";
    }
}

ZFMETHOD_DEFINE_0(ZFUIView, void *, nativeImplView)
{
    return d->nativeImplView;
}
void ZFUIView::nativeImplViewSet(ZF_IN void *nativeImplView,
                                 ZF_IN ZFUIViewNativeImplViewDeleteCallback nativeImplViewDeleteCallback)
{
    void *nativeImplViewOld = d->nativeImplView;
    ZFUIViewNativeImplViewDeleteCallback nativeImplViewDeleteCallbackOld = d->nativeImplViewDeleteCallback;
    d->nativeImplView = nativeImplView;
    d->nativeImplViewDeleteCallback = nativeImplViewDeleteCallback;
    ZFPROTOCOL_ACCESS(ZFUIView)->nativeImplViewSet(this, nativeImplViewOld, d->nativeImplView, d->layerInternalImpl.views.count());
    if(nativeImplViewOld && nativeImplViewDeleteCallbackOld)
    {
        nativeImplViewDeleteCallbackOld(this, nativeImplViewOld);
    }
}
ZFMETHOD_DEFINE_0(ZFUIView, const ZFUIMargin &, nativeImplViewMargin)
{
    return d->nativeImplViewMargin;
}
ZFMETHOD_DEFINE_0(ZFUIView, void, nativeImplViewMarginUpdate)
{
    ZFUIMargin newValue = this->nativeImplViewMarginCustom();
    this->nativeImplViewMarginImplUpdate(newValue);
    if(d->nativeImplViewMargin != newValue)
    {
        d->nativeImplViewMargin = newValue;
        this->layoutRequest();
        this->nativeImplViewMarginOnUpdate();
    }
}
ZFMETHOD_DEFINE_0(ZFUIView, const ZFUIRect &, nativeImplViewFrame)
{
    return d->nativeImplViewFrame;
}

ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFUIView, ZFUIMargin, nativeImplViewMarginCustom)
{
    if(propertyValue != propertyValueOld)
    {
        this->nativeImplViewMarginUpdate();
    }
}

// ============================================================
void ZFUIView::implChildOnAdd(ZF_IN ZFUIView *child,
                              ZF_IN zfindex virtualIndex,
                              ZF_IN ZFUIViewChildLayerEnum childLayer,
                              ZF_IN zfindex childLayerIndex)
{
    ZFPROTOCOL_ACCESS(ZFUIView)->childAdd(this, child, virtualIndex, childLayer, childLayerIndex);
}
void ZFUIView::implChildOnRemove(ZF_IN ZFUIView *child,
                                 ZF_IN zfindex virtualIndex,
                                 ZF_IN ZFUIViewChildLayerEnum childLayer,
                                 ZF_IN zfindex childLayerIndex)
{
    ZFPROTOCOL_ACCESS(ZFUIView)->childRemove(this, child, virtualIndex, childLayer, childLayerIndex);
}
void ZFUIView::implChildOnRemoveAllForDealloc(void)
{
    ZFPROTOCOL_ACCESS(ZFUIView)->childRemoveAllForDealloc(this);
}

// ============================================================
// parent
void ZFUIView::_ZFP_ZFUIView_parentChanged(ZF_IN ZFUIView *viewParent,
                                           ZF_IN ZFUIViewLayoutParam *layoutParam,
                                           ZF_IN ZFUIViewChildLayerEnum viewLayer)
{
    if(viewParent == zfnull)
    {
        d->viewParent = zfnull;
        d->viewDelegateParent = zfnull;
        this->layoutedFramePrevReset();
    }
    else
    {
        d->viewParent = d->virtualParent(viewParent);
        if(ZFBitTest(viewParent->d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_viewDelegateForParent))
        {
            d->viewDelegateParent = viewParent;
        }
        else
        {
            d->viewDelegateParent = zfnull;
        }
    }

    if(layoutParam != zfnull)
    {
        // only change layoutParam if not null
        // keep old one for performance
        d->layoutParamSet(this, layoutParam);
    }
    else
    {
        this->serializableRefLayoutParamSet(zfnull);
    }

    d->viewLayer = viewLayer;
}
ZFMETHOD_DEFINE_0(ZFUIView, void *, nativeView)
{
    return d->nativeView;
}
void ZFUIView::_ZFP_ZFUIView_nativeViewNotifyAdd(ZF_IN ZFUIView *view, ZF_IN void *nativeParentView)
{
    zfCoreAssert(view != zfnull && nativeParentView != zfnull);

    zfRetain(view);
    zffloat scaleForImpl = ZFPROTOCOL_ACCESS(ZFUIView)->nativeViewScaleForImpl(nativeParentView);
    view->_ZFP_ZFUIView_scaleSetRecursively(
        view->scaleFixed() * scaleForImpl / view->scaleForImpl(),
        scaleForImpl);
}
void ZFUIView::_ZFP_ZFUIView_nativeViewNotifyRemove(ZF_IN ZFUIView *view)
{
    zfCoreAssert(view != zfnull);
    zfRelease(view);
}

// ============================================================
// view delegate logic
void ZFUIView::viewDelegateSet(ZF_IN ZFUIView *viewDelegate)
{
    if(viewDelegate == d->viewDelegate)
    {
        return ;
    }
    zfCoreAssertWithMessage(viewDelegate != this, "you must not set viewDelegate to self");
    zfCoreAssertWithMessage(viewDelegate == zfnull || viewDelegate->viewParent() == zfnull,
        "setting a viewDelegate which already has parent");

    ZFUIView *viewDelegateOld = d->viewDelegate;
    d->viewDelegate = zfnull;
    zfblockedAlloc(ZFArrayEditable, children);
    zfblockedAlloc(ZFArrayEditable, childLayoutParams);
    if(viewDelegateOld != zfnull)
    {
        for(zfindex i = 0; i < viewDelegateOld->childCount(); ++i)
        {
            children->add(viewDelegateOld->childAtIndex(i));
            childLayoutParams->add(viewDelegateOld->childAtIndex(i)->layoutParam());
        }
        viewDelegateOld->childRemoveAll();
        ZFBitUnset(viewDelegateOld->d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_viewDelegateForParent);
        viewDelegateOld->viewRemoveFromParent();
    }

    if(viewDelegate != zfnull)
    {
        for(zfindex i = 0; i < this->childCount(); ++i)
        {
            children->add(this->childAtIndex(i));
            childLayoutParams->add(this->childAtIndex(i)->layoutParam());
        }
        this->childRemoveAll();

        d->viewDelegate = zfRetain(viewDelegate);
    }

    ZFUIView *viewToAdd = this;
    if(d->viewDelegate != zfnull)
    {
        this->internalFgViewAdd(d->viewDelegate,
            zfHint("layoutParam")zfnull,
            zfHint("addAsTopMost")zffalse);
        viewToAdd = d->viewDelegate;
        ZFBitSet(d->viewDelegate->d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_viewDelegateForParent);
    }
    for(zfindex i = 0; i < children->count(); ++i)
    {
        viewToAdd->childAdd(
            children->get<ZFUIView *>(i),
            childLayoutParams->get<ZFUIViewLayoutParam *>(i));
    }

    if(viewDelegateOld != zfnull)
    {
        this->viewDelegateOnDealloc(viewDelegateOld);
    }
    if(d->viewDelegate != zfnull)
    {
        this->viewDelegateOnInit(d->viewDelegate);
    }
    zfRelease(viewDelegateOld);
}

ZFMETHOD_DEFINE_0(ZFUIView, zfbool, viewDelegateForParent)
{
    return ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_viewDelegateForParent);
}
ZFMETHOD_DEFINE_0(ZFUIView, ZFUIView *, viewDelegateParent)
{
    return d->viewDelegateParent;
}
ZFMETHOD_DEFINE_0(ZFUIView, ZFUIView *, viewDelegate)
{
    return d->viewDelegate;
}
void ZFUIView::viewDelegateLayoutOnMeasure(ZF_OUT ZFUISize &ret,
                                           ZF_IN const ZFUISize &sizeHint,
                                           ZF_IN const ZFUISizeParam &sizeParam)
{
    const ZFUIMargin &viewDelegateMargin = d->viewDelegate->layoutParam()->layoutMargin();
    ret = d->viewDelegate->layoutMeasure(
        ZFUIViewLayoutParam::sizeHintOffset(sizeHint, ZFUISizeMake(
            0 - ZFUIMarginGetWidth(viewDelegateMargin),
            0 - ZFUIMarginGetHeight(viewDelegateMargin))),
        sizeParam);
    ZFUISizeApplyMarginReversely(ret, ret, viewDelegateMargin);
}
void ZFUIView::viewDelegateLayoutOnMeasureFinish(ZF_IN_OUT ZFUISize &measuredSize,
                                                 ZF_IN const ZFUISize &sizeHint,
                                                 ZF_IN const ZFUISizeParam &sizeParam)
{
    ZFUISize tmp = ZFUISizeApplyMargin(measuredSize, d->viewDelegate->layoutParam()->layoutMargin());
    d->viewDelegate->layoutOnMeasureFinish(tmp, sizeHint, sizeParam);
    measuredSize = ZFUISizeApplyMarginReversely(tmp, d->viewDelegate->layoutParam()->layoutMargin());
}
void ZFUIView::viewDelegateLayoutOnLayoutPrepare(ZF_IN const ZFUIRect &bounds)
{
    // nothing to do
}
void ZFUIView::viewDelegateLayoutOnLayout(ZF_IN const ZFUIRect &bounds)
{
    d->viewDelegate->layout(
        ZFUIRectApplyMargin(bounds, d->viewDelegate->layoutParam()->layoutMargin()));
}
void ZFUIView::viewDelegateLayoutOnLayoutFinish(ZF_IN const ZFUIRect &bounds)
{
    // nothing to do
}

// ============================================================
// view focus
ZFMETHOD_DEFINE_0(ZFUIView, zfbool, viewFocused)
{
    ZFPROTOCOL_INTERFACE_CLASS(ZFUIViewFocus) *impl = ZFPROTOCOL_TRY_ACCESS(ZFUIViewFocus);
    if(impl != zfnull)
    {
        return impl->viewFocused(this);
    }
    else
    {
        return zffalse;
    }
}
ZFMETHOD_DEFINE_1(ZFUIView, void, viewFocusRequest,
                  ZFMP_IN(zfbool, viewFocus))
{
    ZFPROTOCOL_INTERFACE_CLASS(ZFUIViewFocus) *impl = ZFPROTOCOL_TRY_ACCESS(ZFUIViewFocus);
    if(impl != zfnull)
    {
        if(!viewFocus || this->viewFocusable())
        {
            impl->viewFocusRequest(this, viewFocus);
        }
    }
}
ZFMETHOD_DEFINE_0(ZFUIView, ZFUIView *, viewFocusFind)
{
    ZFPROTOCOL_INTERFACE_CLASS(ZFUIViewFocus) *impl = ZFPROTOCOL_TRY_ACCESS(ZFUIViewFocus);
    if(impl == zfnull)
    {
        return zfnull;
    }
    else
    {
        return impl->viewFocusFind(this);
    }
}

// ============================================================
// parent
ZFMETHOD_DEFINE_0(ZFUIView, ZFUIView *, viewParent)
{
    return d->viewParent;
}
ZFMETHOD_DEFINE_0(ZFUIView, ZFUIView *, viewParentVirtual)
{
    if(d->viewDelegateParent != zfnull)
    {
        return d->viewDelegateParent;
    }
    else
    {
        return d->viewParent;
    }
}

ZFMETHOD_DEFINE_0(ZFUIView, void, viewRemoveFromParent)
{
    if(this->viewParent() != zfnull)
    {
        switch(this->viewLayer())
        {
            case ZFUIViewChildLayer::e_Normal:
                this->viewParent()->childRemove(this);
                break;
            case ZFUIViewChildLayer::e_InternalImpl:
                this->viewParent()->internalImplViewRemove(this);
                break;
            case ZFUIViewChildLayer::e_InternalBg:
                this->viewParent()->internalBgViewRemove(this);
                break;
            case ZFUIViewChildLayer::e_InternalFg:
                this->viewParent()->internalFgViewRemove(this);
                break;
            default:
                zfCoreCriticalShouldNotGoHere();
                return ;
        }
    }
}

// ============================================================
// scale settings
ZFMETHOD_DEFINE_0(ZFUIView, zffloat, scaleForApp)
{
    return (d->scaleFixed / d->scaleForImpl);
}
ZFMETHOD_DEFINE_0(ZFUIView, zffloat, scaleForImpl)
{
    return d->scaleForImpl;
}
ZFMETHOD_DEFINE_0(ZFUIView, zffloat, scaleFixed)
{
    return d->scaleFixed;
}
ZFMETHOD_DEFINE_0(ZFUIView, zffloat, scaleForImplPhysicalPixel)
{
    return ZFPROTOCOL_ACCESS(ZFUIView)->nativeViewScaleForPhysicalPixel(d->nativeView);
}
void ZFUIView::scaleOnChange(void)
{
    this->observerNotify(ZFUIView::EventViewScaleOnChange());
}
void ZFUIView::_ZFP_ZFUIView_scaleSetRecursively(ZF_IN zffloat scaleFixed,
                                                 ZF_IN zffloat scaleForImpl)
{
    if(zffloatNotEqual(d->scaleFixed, scaleFixed)
        || zffloatNotEqual(d->scaleForImpl, scaleForImpl))
    {
        d->scaleFixed = scaleFixed;
        d->scaleForImpl = scaleForImpl;
        for(zfindex i = d->layerInternalImpl.views.count() - 1; i != zfindexMax(); --i)
        {
            d->layerInternalImpl.views.get(i)->to<ZFUIView *>()->_ZFP_ZFUIView_scaleSetRecursively(scaleFixed, scaleForImpl);
        }
        for(zfindex i = d->layerInternalBg.views.count() - 1; i != zfindexMax(); --i)
        {
            d->layerInternalBg.views.get(i)->to<ZFUIView *>()->_ZFP_ZFUIView_scaleSetRecursively(scaleFixed, scaleForImpl);
        }
        for(zfindex i = d->layerInternalFg.views.count() - 1; i != zfindexMax(); --i)
        {
            d->layerInternalFg.views.get(i)->to<ZFUIView *>()->_ZFP_ZFUIView_scaleSetRecursively(scaleFixed, scaleForImpl);
        }
        for(zfindex i = this->childCount() - 1; i != zfindexMax(); --i)
        {
            this->childAtIndex(i)->_ZFP_ZFUIView_scaleSetRecursively(scaleFixed, scaleForImpl);
        }
        this->layoutRequest();
        this->scaleOnChange();
    }
}

// ============================================================
// layout logic
ZFMETHOD_DEFINE_0(ZFUIView, zfautoObject, layoutParamCreate)
{
    zfautoObject layoutParam = this->layoutParamClass()->newInstance();
    if(layoutParam == zfnull || !layoutParam.toObject()->classData()->classIsTypeOf(ZFUIViewLayoutParam::ClassData()))
    {
        return zfnull;
    }
    this->layoutParamOnUpdate(layoutParam.to<ZFUIViewLayoutParam *>());
    return layoutParam;
}
const ZFClass *ZFUIView::layoutParamClass(void)
{
    return ZFUIViewLayoutParam::ClassData();
}

ZFMETHOD_DEFINE_1(ZFUIView, void, layoutParamSet,
                  ZFMP_IN(ZFUIViewLayoutParam *, layoutParam))
{
    if(this->viewParent() != zfnull && layoutParam != zfnull && !layoutParam->classData()->classIsTypeOf(this->layoutParamClass()))
    {
        zfautoObject layoutParamHolder = this->layoutParamCreate();
        ZFUIViewLayoutParam *layoutParamTmp = layoutParamHolder.to<ZFUIViewLayoutParam *>();
        layoutParamTmp->styleableCopyFrom(layoutParam);
        d->layoutParamSet(this, layoutParamTmp);
    }
    else
    {
        d->layoutParamSet(this, layoutParam);
    }
}
ZFMETHOD_DEFINE_0(ZFUIView, ZFUIViewLayoutParam *, layoutParam)
{
    return d->layoutParam;
}

ZFMETHOD_DEFINE_1(ZFUIView, void, layoutRequestOverrideSet,
                  ZFMP_IN(zfbool, layoutRequestOverride))
{
    if(layoutRequestOverride)
    {
        ++(d->layoutRequestOverrideFlag);
    }
    else
    {
        zfCoreAssert(d->layoutRequestOverrideFlag > 0);
        --(d->layoutRequestOverrideFlag);
    }
}
ZFMETHOD_DEFINE_0(ZFUIView, zfindex, layoutRequestOverride)
{
    return d->layoutRequestOverrideFlag;
}

void ZFUIView::_ZFP_ZFUIView_notifyLayoutRootView(ZF_IN const ZFUIRect &bounds)
{
    ++_ZFP_ZFUIView_layoutRequestOverrideFlag;
    this->layoutMeasure(bounds.size, ZFUISizeParamFillFill());
    this->layout(bounds);
    --_ZFP_ZFUIView_layoutRequestOverrideFlag;
}

ZFMETHOD_DEFINE_0(ZFUIView, void, layoutRequest)
{
    if(!ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_layoutRequested) && _ZFP_ZFUIView_layoutRequestOverrideFlag == 0 && d->layoutRequestOverrideFlag == 0)
    {
        ZFBitSet(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_layoutRequested);
        if(d->viewDelegate != zfnull)
        {
            ZFBitTest(d->viewDelegate->d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_layoutRequested);
        }
        this->layoutOnLayoutRequest(this);

        ZFUIView *view = this->viewParentVirtual();
        while(view != zfnull && !ZFBitTest(view->d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_layoutRequested))
        {
            ZFBitSet(view->d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_layoutRequested);
            view->layoutOnLayoutRequest(this);
            view = view->viewParentVirtual();
        }

        ZFPROTOCOL_ACCESS(ZFUIView)->layoutRequest(this);
    }
}
ZFMETHOD_DEFINE_0(ZFUIView, zfbool, layoutRequested)
{
    return ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_layoutRequested);
}
ZFMETHOD_DEFINE_0(ZFUIView, zfbool, layouting)
{
    return ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_layouting);
}
ZFMETHOD_DEFINE_2(ZFUIView, const ZFUISize &, layoutMeasure,
                  ZFMP_IN(const ZFUISize &, sizeHint),
                  ZFMP_IN(const ZFUISizeParam &, sizeParam))
{
    if(ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_layoutRequested)
        || !ZFUISizeIsEqual(d->measureResult->sizeHint, sizeHint)
        || !ZFUISizeParamIsEqual(d->measureResult->sizeParam, sizeParam))
    {
        if(!ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_layoutRequested)
            && ZFUISizeParamIsEqual(d->measureResult->sizeParam, sizeParam))
        {
            // if all these cond matchs, result is ensured not changed
            // * prev measure with wrap, and result a size less than size hint
            // * cur measure with a larger size hint than prev
            if(zftrue
                && (d->measureResult->sizeHint.width == sizeHint.width
                    || (sizeParam.width == ZFUISizeType::e_Wrap && (
                        (d->measureResult->sizeHint.width < 0 && (sizeHint.width < 0 || d->measureResult->measuredSize.width <= sizeHint.width))
                        || (d->measureResult->measuredSize.width < d->measureResult->sizeHint.width && (sizeHint.width == -1 || sizeHint.width >= d->measureResult->sizeHint.width))
                    )))
                && (d->measureResult->sizeHint.height == sizeHint.height
                    || (sizeParam.height == ZFUISizeType::e_Wrap && (
                        (d->measureResult->sizeHint.height < 0 && (sizeHint.height < 0 || d->measureResult->measuredSize.height <= sizeHint.height))
                        || (d->measureResult->measuredSize.height < d->measureResult->sizeHint.height && (sizeHint.height == -1 || sizeHint.height >= d->measureResult->sizeHint.height))
                    )))
                )
            {
                return d->measureResult->measuredSize;
            }
        }

        d->measureResult->measuredSize = ZFUISizeInvalid();
        d->measureResult->sizeHint = sizeHint;
        d->measureResult->sizeParam = sizeParam;
        if(sizeParam.width == ZFUISizeType::e_Fill && sizeParam.height == ZFUISizeType::e_Fill)
        {
            d->measureResult->sizeHint = sizeHint;
            d->measureResult->measuredSize.width = zfmMax(0, d->measureResult->measuredSize.width);
            d->measureResult->measuredSize.height = zfmMax(0, d->measureResult->measuredSize.height);
        }
        else if(d->viewDelegate != zfnull)
        {
            this->viewDelegateLayoutOnMeasure(d->measureResult->measuredSize, sizeHint, sizeParam);
        }
        else
        {
            this->layoutOnMeasure(d->measureResult->measuredSize, sizeHint, sizeParam);
        }

        if(d->viewDelegate != zfnull)
        {
            this->viewDelegateLayoutOnMeasureFinish(d->measureResult->measuredSize, sizeHint, sizeParam);
        }
        this->layoutOnMeasureFinish(d->measureResult->measuredSize, sizeHint, sizeParam);
        {
            this->observerNotify(ZFUIView::EventViewLayoutOnMeasureFinish(), d->measureResult);
        }

        if(d->measureResult->measuredSize.width < 0)
        {
            if(this->viewSizePrefered().width >= 0)
            {
                d->measureResult->measuredSize.width = this->viewSizePrefered().width;
            }
            else
            {
                d->measureResult->measuredSize.width = 0;
            }
        }
        if(d->measureResult->measuredSize.height < 0)
        {
            if(this->viewSizePrefered().height >= 0)
            {
                d->measureResult->measuredSize.height = this->viewSizePrefered().height;
            }
            else
            {
                d->measureResult->measuredSize.height = 0;
            }
        }

        ZFUIViewLayoutParam::sizeHintApply(d->measureResult->measuredSize, d->measureResult->measuredSize, sizeHint, sizeParam);
        ZFUISizeApplyRange(d->measureResult->measuredSize, d->measureResult->measuredSize, this->viewSizeMin(), this->viewSizeMax());
    }
    return d->measureResult->measuredSize;
}
ZFMETHOD_DEFINE_0(ZFUIView, const ZFUISize &, layoutMeasuredSize)
{
    return d->measureResult->measuredSize;
}
ZFMETHOD_DEFINE_1(ZFUIView, void, layout,
                  ZFMP_IN(const ZFUIRect &, rect))
{
    if(ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_layoutRequested)
        || !ZFUISizeIsEqual(d->layoutedFrame.size, rect.size))
    {
        if(!ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_layouting) && !ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_layoutedFramePrevResetFlag))
        {
            d->layoutedFramePrev = d->layoutedFrame;
        }
        ZFBitUnset(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_layoutedFramePrevResetFlag);
        if(d->layoutedFrame != rect)
        {
            d->layoutedFrame = rect;
            ZFPROTOCOL_ACCESS(ZFUIView)->viewFrameSet(this, ZFUIRectApplyScale(rect, this->scaleFixed()));
        }

        ZFUIRect bounds = ZFUIRectGetBounds(rect);

        if(d->nativeImplView != zfnull)
        {
            ZFUIRect nativeImplViewFrame = ZFUIRectZero();
            this->nativeImplViewOnLayout(nativeImplViewFrame, bounds, this->nativeImplViewMargin());
            if(d->nativeImplViewFrame != nativeImplViewFrame)
            {
                d->nativeImplViewFrame = nativeImplViewFrame;
                ZFPROTOCOL_ACCESS(ZFUIView)->nativeImplViewFrameSet(this, ZFUIRectApplyScale(nativeImplViewFrame, this->scaleFixed()));
            }
        }

        ZFBitSet(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_layouting);

        // layout prepare
        if(d->viewDelegate != zfnull)
        {
            this->viewDelegateLayoutOnLayoutPrepare(bounds);
        }
        this->layoutOnLayoutPrepare(bounds);
        this->observerNotify(ZFUIView::EventViewLayoutOnLayoutPrepare());

        // layout
        if(d->viewDelegate != zfnull)
        {
            this->viewDelegateLayoutOnLayout(bounds);
        }
        else
        {
            this->layoutOnLayout(bounds);
        }
        this->observerNotify(ZFUIView::EventViewLayoutOnLayout());

        // internal views
        this->internalImplViewOnLayout(bounds);
        this->internalBgViewOnLayout(bounds);
        this->internalFgViewOnLayout(bounds);

        // layout finish
        if(d->viewDelegate != zfnull)
        {
            this->viewDelegateLayoutOnLayoutFinish(bounds);
        }
        this->layoutOnLayoutFinish(bounds);
        this->observerNotify(ZFUIView::EventViewLayoutOnLayoutFinish());

        ZFBitUnset(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_layouting);
    }
    else if(!ZFUIRectIsEqual(d->layoutedFrame, rect)) {
        // size not changed but point changed, notify impl to move the view is enough
        if(!ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_layouting) && !ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_layoutedFramePrevResetFlag))
        {
            d->layoutedFramePrev = d->layoutedFrame;
        }
        ZFBitUnset(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_layoutedFramePrevResetFlag);
        if(d->layoutedFrame != rect)
        {
            d->layoutedFrame = rect;
            ZFPROTOCOL_ACCESS(ZFUIView)->viewFrameSet(this, ZFUIRectApplyScale(rect, this->scaleFixed()));
        }
    }
    ZFBitUnset(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_layoutRequested);
}
ZFMETHOD_DEFINE_0(ZFUIView, void, layoutIfNeed)
{
    this->layout(this->layoutedFrame());
}
ZFMETHOD_DEFINE_0(ZFUIView, const ZFUIRect &, layoutedFrame)
{
    return d->layoutedFrame;
}
ZFMETHOD_DEFINE_0(ZFUIView, const ZFUIRect &, layoutedFramePrev)
{
    return d->layoutedFramePrev;
}
ZFMETHOD_DEFINE_0(ZFUIView, void, layoutedFramePrevReset)
{
    d->layoutedFramePrev = ZFUIRectZero();
    ZFBitSet(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_layoutedFramePrevResetFlag);
}
ZFMETHOD_DEFINE_1(ZFUIView, void, layoutedFrameFixedT,
                  ZFMP_OUT(ZFUIRect &, ret))
{
    ret = d->layoutedFrame;
    ZFUIView *viewParent = d->viewParent;
    if(d->viewDelegateParent != zfnull)
    {
        ZFUIView *viewDelegateParent = d->viewDelegateParent;
        do
        {
            ZFUIRect tmp = ret;
            viewDelegateParent->layoutedFrameFixedOnUpdateForChild(ret, tmp);
            viewParent = viewDelegateParent->d->viewParent;
            viewDelegateParent = viewDelegateParent->d->viewDelegateParent;
        } while(viewDelegateParent != zfnull);
    }
    if(viewParent != zfnull)
    {
        viewParent->layoutedFrameFixedOnUpdateForChild(ret, this->layoutedFrame());
    }
}
ZFMETHOD_DEFINE_0(ZFUIView, ZFUIRect, layoutedFrameFixed)
{
    ZFUIRect ret = ZFUIRectZero();
    this->layoutedFrameFixedT(ret);
    return ret;
}

void ZFUIView::layoutOnLayoutRequest(ZF_IN ZFUIView *requestByView)
{
    if(ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_layoutOnLayoutRequest)
        || ZFBitTest(_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_layoutOnLayoutRequest))
    {
        this->observerNotify(ZFUIView::EventViewLayoutOnLayoutRequest(), requestByView);
    }
}
void ZFUIView::layoutOnLayout(ZF_IN const ZFUIRect &bounds)
{
    for(zfindex i = 0; i < this->childCount(); ++i)
    {
        ZFUIView *child = this->childAtIndex(i);
        child->layout(
            ZFUIViewLayoutParam::layoutParamApply(
                bounds,
                child,
                child->layoutParam()));
    }
}

// ============================================================
// child views
ZFMETHOD_DEFINE_3(ZFUIView, ZFUIView *, childFindById,
                  ZFMP_IN(const zfchar *, viewId),
                  ZFMP_IN_OPT(zfbool, findRecursively, zftrue),
                  ZFMP_IN_OPT(zfbool, includeInternalViews, zffalse))
{
    if(viewId == zfnull || *viewId == '\0')
    {
        return zfnull;
    }

    if(!findRecursively)
    {
        for(zfindex i = 0; i < d->layerNormal.views.count(); ++i)
        {
            if(zfscmpTheSame(d->layerNormal.views[i]->viewId().cString(), viewId))
            {
                return d->layerNormal.views[i];
            }
        }
        if(includeInternalViews)
        {
            for(zfindex i = 0; i < d->layerInternalImpl.views.count(); ++i)
            {
                if(zfscmpTheSame(d->layerInternalImpl.views[i]->viewId().cString(), viewId))
                {
                    return d->layerInternalImpl.views[i];
                }
            }
            for(zfindex i = 0; i < d->layerInternalBg.views.count(); ++i)
            {
                if(zfscmpTheSame(d->layerInternalBg.views[i]->viewId().cString(), viewId))
                {
                    return d->layerInternalBg.views[i];
                }
            }
            for(zfindex i = 0; i < d->layerInternalFg.views.count(); ++i)
            {
                if(zfscmpTheSame(d->layerInternalFg.views[i]->viewId().cString(), viewId))
                {
                    return d->layerInternalFg.views[i];
                }
            }
        }
        return zfnull;
    }

    ZFCoreArrayPOD<ZFUIView *> toFind;
    toFind.add(this);
    while(!toFind.isEmpty())
    {
        ZFUIView *view = toFind.get(0);
        toFind.remove(0);
        if(zfscmpTheSame(view->viewId().cString(), viewId))
        {
            return view;
        }
        toFind.addFrom(view->childArray());
        if(includeInternalViews)
        {
            toFind.addFrom(view->d->layerInternalImpl.views);
            toFind.addFrom(view->d->layerInternalBg.views);
            toFind.addFrom(view->d->layerInternalFg.views);
        }
    }
    return zfnull;
}

ZFMETHOD_DEFINE_3(ZFUIView, void, childAdd,
                  ZFMP_IN(ZFUIView *, view),
                  ZFMP_IN_OPT(ZFUIViewLayoutParam *, layoutParam, zfnull),
                  ZFMP_IN_OPT(zfindex, atIndex, zfindexMax()))
{
    if(d->viewDelegate)
    {
        d->viewDelegate->childAdd(view, layoutParam, atIndex);
    }
    else
    {
        d->childAdd(this, ZFUIViewChildLayer::e_Normal, d->layerNormal, view, layoutParam, atIndex);
    }
}
ZFMETHOD_DEFINE_3(ZFUIView, void, childAdd,
                  ZFMP_IN(ZFUIView *, view),
                  ZFMP_IN(const ZFUISizeParam &, sizeParam),
                  ZFMP_IN_OPT(ZFUIAlignFlags const &, layoutAlign, ZFUIAlign::e_LeftInner | ZFUIAlign::e_TopInner))
{
    this->childAdd(view);
    ZFUIViewLayoutParam *lp = view->layoutParam();
    lp->sizeParamSet(sizeParam);
    lp->layoutAlignSet(layoutAlign);
}
ZFMETHOD_DEFINE_1(ZFUIView, void, childRemove,
                  ZFMP_IN(ZFUIView *, view))
{
    if(d->viewDelegate)
    {
        d->viewDelegate->childRemove(view);
    }
    else
    {
        d->childRemove(this, ZFUIViewChildLayer::e_Normal, d->layerNormal, view);
    }
}
ZFMETHOD_DEFINE_1(ZFUIView, void, childRemoveAtIndex,
                  ZFMP_IN(zfindex, index))
{
    if(d->viewDelegate)
    {
        d->viewDelegate->childRemoveAtIndex(index);
    }
    else
    {
        d->childRemoveAtIndex(this, ZFUIViewChildLayer::e_Normal, d->layerNormal, index);
    }
}
ZFMETHOD_DEFINE_0(ZFUIView, void, childRemoveAll)
{
    if(d->viewDelegate)
    {
        d->viewDelegate->childRemoveAll();
    }
    else
    {
        d->childRemoveAll(this, ZFUIViewChildLayer::e_Normal, d->layerNormal);
    }
}

ZFMETHOD_DEFINE_2(ZFUIView, void, childMove,
                  ZFMP_IN(zfindex, fromIndex),
                  ZFMP_IN(zfindex, toIndexOrIndexMax))
{
    if(d->viewDelegate)
    {
        d->viewDelegate->childMove(fromIndex, toIndexOrIndexMax);
    }
    else
    {
        d->childMove(this, ZFUIViewChildLayer::e_Normal, d->layerNormal, fromIndex, toIndexOrIndexMax);
    }
}
ZFMETHOD_DEFINE_2(ZFUIView, void, childMove,
                  ZFMP_IN(ZFUIView *, child),
                  ZFMP_IN(zfindex, toIndexOrIndexMax))
{
    if(d->viewDelegate)
    {
        d->viewDelegate->childMove(child, toIndexOrIndexMax);
    }
    else
    {
        d->childMove(this, ZFUIViewChildLayer::e_Normal, d->layerNormal, child, toIndexOrIndexMax);
    }
}
ZFMETHOD_DEFINE_2(ZFUIView, void, childReplaceAtIndex,
                  ZFMP_IN(zfindex, atIndex),
                  ZFMP_IN(ZFUIView *, toReplace))
{
    if(d->viewDelegate)
    {
        d->viewDelegate->childReplaceAtIndex(atIndex, toReplace);
    }
    else
    {
        d->childReplace(this, ZFUIViewChildLayer::e_Normal, d->layerNormal, atIndex, toReplace);
    }
}

ZFMETHOD_DEFINE_0(ZFUIView, zfindex, childCount)
{
    if(d->viewDelegate)
    {
        return d->viewDelegate->childCount();
    }
    else
    {
        return d->childCount(d->layerNormal);
    }
}
ZFMETHOD_DEFINE_1(ZFUIView, ZFUIView *, childAtIndex,
                  ZFMP_IN(zfindex, index))
{
    if(d->viewDelegate)
    {
        return d->viewDelegate->childAtIndex(index);
    }
    else
    {
        return d->childAtIndex(d->layerNormal, index);
    }
}
ZFMETHOD_DEFINE_1(ZFUIView, zfindex, childFind,
                  ZFMP_IN(ZFUIView *, view))
{
    if(d->viewDelegate)
    {
        return d->viewDelegate->childFind(view);
    }
    else
    {
        return d->childFind(d->layerNormal, view);
    }
}
ZFMETHOD_DEFINE_0(ZFUIView, ZFCoreArrayPOD<ZFUIView *>, childArray)
{
    if(d->viewDelegate)
    {
        return d->viewDelegate->childArray();
    }
    else
    {
        return d->layerNormal.views;
    }
}
ZFMETHOD_DEFINE_0(ZFUIView, ZFUIViewChildLayerEnum, viewLayer)
{
    return d->viewLayer;
}
ZFMETHOD_DEFINE_0(ZFUIView, ZFCoreArrayPOD<ZFUIView *>, childRawArray)
{
    ZFCoreArrayPOD<ZFUIView *> ret;
    ret.capacitySet(
        d->layerInternalBg.views.count()
        + d->layerInternalBg.views.count()
        + this->childCount()
        + d->layerInternalFg.views.count());
    ret.addFrom(d->layerInternalImpl.views);
    ret.addFrom(d->layerInternalBg.views);
    ret.addFrom(this->childCount());
    ret.addFrom(d->layerInternalFg.views);
    return ret;
}

// ============================================================
// events
void ZFUIView::viewChildOnChange(void)
{
    if(ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_viewChildOnChange)
        || ZFBitTest(_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_viewChildOnChange))
    {
        this->observerNotify(ZFUIView::EventViewChildOnChange());
    }
}
void ZFUIView::viewChildOnAdd(ZF_IN ZFUIView *child,
                              ZF_IN ZFUIViewChildLayerEnum layer)
{
    if(ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_viewChildOnAdd)
        || ZFBitTest(_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_viewChildOnAdd))
    {
        zfblockedAlloc(ZFUIViewChildLayer, t, layer);
        this->observerNotify(ZFUIView::EventViewChildOnAdd(), child, t);
    }
}
void ZFUIView::viewChildOnRemove(ZF_IN ZFUIView *child,
                                 ZF_IN ZFUIViewChildLayerEnum layer)
{
    if(ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_viewChildOnRemove)
        || ZFBitTest(_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_viewChildOnRemove))
    {
        zfblockedAlloc(ZFUIViewChildLayer, t, layer);
        this->observerNotify(ZFUIView::EventViewChildOnRemove(), child, t);
    }
}
void ZFUIView::viewOnAddToParent(ZF_IN ZFUIView *parent)
{
    if(ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_viewOnAddToParent)
        || ZFBitTest(_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_viewOnAddToParent))
    {
        this->observerNotify(ZFUIView::EventViewOnAddToParent(), parent);
    }
}
void ZFUIView::viewOnRemoveFromParent(ZF_IN ZFUIView *parent)
{
    if(ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_viewOnRemoveFromParent)
        || ZFBitTest(_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_viewOnRemoveFromParent))
    {
        this->observerNotify(ZFUIView::EventViewOnRemoveFromParent(), parent);
    }
}

// ============================================================
// internal impl views
ZFMETHOD_DEFINE_3(ZFUIView, void, internalImplViewAdd,
                  ZFMP_IN(ZFUIView *, view),
                  ZFMP_IN_OPT(ZFUIViewLayoutParam *, layoutParam, zfnull),
                  ZFMP_IN_OPT(zfbool, addAsTopMost, zftrue))
{
    d->childAdd(this, ZFUIViewChildLayer::e_InternalImpl, d->layerInternalImpl, view, layoutParam, (addAsTopMost ? zfindexMax() : 0));
}
ZFMETHOD_DEFINE_1(ZFUIView, void, internalImplViewRemove,
                  ZFMP_IN(ZFUIView *, view))
{
    d->childRemove(this, ZFUIViewChildLayer::e_InternalImpl, d->layerInternalImpl, view);
}
ZFMETHOD_DEFINE_0(ZFUIView, ZFCoreArrayPOD<ZFUIView *>, internalImplViewArray)
{
    return d->layerInternalImpl.views;
}
void ZFUIView::internalImplViewOnLayout(ZF_IN const ZFUIRect &bounds)
{
    for(zfindex i = 0; i < d->childCount(d->layerInternalImpl); ++i)
    {
        ZFUIView *child = d->childAtIndex(d->layerInternalImpl, i);
        if(this->internalViewShouldLayout(child))
        {
            child->layout(
                ZFUIViewLayoutParam::layoutParamApply(
                    bounds,
                    child,
                    d->childLayoutParamAtIndex(d->layerInternalImpl, i)));
        }
    }
}

// ============================================================
// internal background views
ZFMETHOD_DEFINE_3(ZFUIView, void, internalBgViewAdd,
                  ZFMP_IN(ZFUIView *, view),
                  ZFMP_IN_OPT(ZFUIViewLayoutParam *, layoutParam, zfnull),
                  ZFMP_IN_OPT(zfbool, addAsTopMost, zftrue))
{
    d->childAdd(this, ZFUIViewChildLayer::e_InternalBg, d->layerInternalBg, view, layoutParam, (addAsTopMost ? zfindexMax() : 0));
}
ZFMETHOD_DEFINE_1(ZFUIView, void, internalBgViewRemove,
                  ZFMP_IN(ZFUIView *, view))
{
    d->childRemove(this, ZFUIViewChildLayer::e_InternalBg, d->layerInternalBg, view);
}
ZFMETHOD_DEFINE_0(ZFUIView, ZFCoreArrayPOD<ZFUIView *>, internalBgViewArray)
{
    return d->layerInternalBg.views;
}
void ZFUIView::internalBgViewOnLayout(ZF_IN const ZFUIRect &bounds)
{
    for(zfindex i = 0; i < d->childCount(d->layerInternalBg); ++i)
    {
        ZFUIView *child = d->childAtIndex(d->layerInternalBg, i);
        if(this->internalViewShouldLayout(child))
        {
            child->layout(
                ZFUIViewLayoutParam::layoutParamApply(
                    bounds,
                    child,
                    d->childLayoutParamAtIndex(d->layerInternalBg, i)));
        }
    }
}

// ============================================================
// internal foreground views
ZFMETHOD_DEFINE_3(ZFUIView, void, internalFgViewAdd,
                  ZFMP_IN(ZFUIView *, view),
                  ZFMP_IN_OPT(ZFUIViewLayoutParam *, layoutParam, zfnull),
                  ZFMP_IN_OPT(zfbool, addAsTopMost, zftrue))
{
    d->childAdd(this, ZFUIViewChildLayer::e_InternalFg, d->layerInternalFg, view, layoutParam, (addAsTopMost ? zfindexMax() : 0));
}
ZFMETHOD_DEFINE_1(ZFUIView, void, internalFgViewRemove,
                  ZFMP_IN(ZFUIView *, view))
{
    d->childRemove(this, ZFUIViewChildLayer::e_InternalFg, d->layerInternalFg, view);
}
ZFMETHOD_DEFINE_0(ZFUIView, ZFCoreArrayPOD<ZFUIView *>, internalFgViewArray)
{
    return d->layerInternalFg.views;
}
void ZFUIView::internalFgViewOnLayout(ZF_IN const ZFUIRect &bounds)
{
    for(zfindex i = 0; i < d->childCount(d->layerInternalFg); ++i)
    {
        ZFUIView *child = d->childAtIndex(d->layerInternalFg, i);
        if(child != d->viewDelegate && this->internalViewShouldLayout(child))
        {
            child->layout(
                ZFUIViewLayoutParam::layoutParamApply(
                    bounds,
                    child,
                    d->childLayoutParamAtIndex(d->layerInternalFg, i)));
        }
    }
}

// ============================================================
// other internal view logic
ZFMETHOD_DEFINE_1(ZFUIView, void, internalViewAutoSerializeTagAdd,
                  ZFMP_IN(const zfchar *, tag))
{
    if(zfsIsEmpty(tag))
    {
        return ;
    }
    d->internalViewAutoSerializeTags[tag] = zftrue;
}
ZFMETHOD_DEFINE_1(ZFUIView, void, internalViewAutoSerializeTagRemove,
                  ZFMP_IN(const zfchar *, tag))
{
    if(zfsIsEmpty(tag))
    {
        return ;
    }
    d->internalViewAutoSerializeTags.erase(tag);
}
ZFMETHOD_DEFINE_0(ZFUIView, void, internalViewAutoSerializeTagRemoveAll)
{
    d->internalViewAutoSerializeTags.clear();
}
ZFMETHOD_DEFINE_1(ZFUIView, void, internalViewAutoSerializeTagGetAllT,
                  ZFMP_OUT(ZFCoreArray<zfstring> &, ret))
{
    ret.capacitySet(ret.count() + d->internalViewAutoSerializeTags.size());
    for(_ZFP_ZFUIViewInternalViewAutoSerializeTagMapType::iterator it = d->internalViewAutoSerializeTags.begin();
        it != d->internalViewAutoSerializeTags.end();
        ++it)
    {
        ret.add(it->first.c_str());
    }
}
ZFMETHOD_DEFINE_0(ZFUIView, ZFCoreArray<zfstring>, internalViewAutoSerializeTagGetAll)
{
    ZFCoreArray<zfstring> ret;
    this->internalViewAutoSerializeTagGetAllT(ret);
    return ret;
}

// ============================================================
// events
ZFMETHOD_DEFINE_1(ZFUIView, void, viewEventSend,
                  ZFMP_IN(ZFUIEvent *, event))
{
    if(event == zfnull)
    {
        return ;
    }

    zfRetain(this);
    zfblockedRelease(this);

    this->viewEventOnEvent(event);
    this->observerNotify(ZFUIView::EventViewOnEvent(), event);
}
void ZFUIView::viewEventOnEvent(ZF_IN ZFUIEvent *event)
{
    const ZFClass *eventClass = event->classData();
    if(eventClass->classIsTypeOf(ZFUIMouseEvent::ClassData()))
    {
        ZFUIMouseEvent *mouseEvent = ZFCastZFObjectUnchecked(ZFUIMouseEvent *, event);
        this->viewEventOnMouseEvent(mouseEvent);
        mouseEvent->eventResolvedSet(zftrue);
    }
    else if(eventClass->classIsTypeOf(ZFUIKeyEvent::ClassData()))
    {
        ZFUIKeyEvent *eventTmp = ZFCastZFObjectUnchecked(ZFUIKeyEvent *, event);
        this->viewEventOnKeyEvent(eventTmp);
        {
            ZFUIView *view = this->viewParent();
            while(view != zfnull && !event->eventResolved())
            {
                view->viewEventOnKeyEvent(eventTmp);
                view = view->viewParent();
            }
        }
    }
    else if(eventClass->classIsTypeOf(ZFUIWheelEvent::ClassData()))
    {
        ZFUIWheelEvent *eventTmp = ZFCastZFObjectUnchecked(ZFUIWheelEvent *, event);
        this->viewEventOnWheelEvent(eventTmp);
        {
            ZFUIView *view = this->viewParent();
            while(view != zfnull && !event->eventResolved())
            {
                view->viewEventOnWheelEvent(eventTmp);
                view = view->viewParent();
            }
        }
    }
}

void ZFUIView::viewEventOnMouseEvent(ZF_IN ZFUIMouseEvent *mouseEvent)
{
    if(this->viewUIEnable())
    {
        if(mouseEvent->mouseAction == ZFUIMouseAction::e_MouseUp
            && this->viewFocusable()
            && this->viewFocusObtainWhenClick())
        {
            this->viewFocusRequest(zftrue);
        }
        mouseEvent->eventResolvedSet(zftrue);
    }
}

void ZFUIView::viewEventOnKeyEvent(ZF_IN ZFUIKeyEvent *keyEvent)
{
    this->viewEventOnKeyEventResolveFocus(keyEvent);
}
void ZFUIView::viewEventOnKeyEventResolveFocus(ZF_IN ZFUIKeyEvent *keyEvent)
{
    ZFUIViewFocusResolveKeyEvent(this, keyEvent);
}

void ZFUIView::viewEventOnWheelEvent(ZF_IN ZFUIWheelEvent *wheelEvent)
{
    // nothing to do
}

// ============================================================
// view property async update
ZFMETHOD_DEFINE_0(ZFUIView, void, viewPropertyUpdateRequest)
{
    ZFThreadTaskRequest(
        zfHint("task")ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewListenerHolder)->viewPropertyOnUpdateListener,
        zfHint("userData")this->objectHolder(),
        zfHint("param0")zfnull,
        zfHint("param1")zfnull,
        zfHint("owner")this,
        zfHint("mergeCallback")ZFThreadTaskRequestMergeCallbackIgnoreOldTask());
}

void ZFUIView::viewPropertyOnUpdate(void)
{
    if(ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_viewPropertyOnUpdate)
        || ZFBitTest(_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_viewPropertyOnUpdate))
    {
        this->observerNotify(ZFUIView::EventViewPropertyOnUpdate());
    }
}

// ============================================================
// override
void ZFUIView::styleableOnCopyFrom(ZF_IN ZFStyleable *anotherStyleable)
{
    zfsuperI(ZFStyleable)::styleableOnCopyFrom(anotherStyleable);
    ZFUIView *ref = ZFCastZFObject(ZFUIView *, anotherStyleable);
    if(ref == zfnull || this->childCount() != 0)
    {
        return ;
    }
    for(zfindex i = 0; i < ref->childCount(); ++i)
    {
        zfautoObject child = ref->childAtIndex(i)->copy();
        zfautoObject childLayoutParam = ref->childAtIndex(i)->layoutParam()->copy();
        this->childAdd(child, childLayoutParam);
    }
}

void ZFUIView::observerOnAdd(ZF_IN zfidentity eventId)
{
    zfsuper::observerOnAdd(eventId);
    if(eventId == ZFUIView::EventViewPropertyOnUpdate())
    {
        ZFBitSet(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_viewPropertyOnUpdate);
    }
    else if(eventId == ZFUIView::EventViewLayoutOnLayoutRequest())
    {
        ZFBitSet(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_layoutOnLayoutRequest);
    }
    else if(eventId == ZFUIView::EventViewChildOnChange())
    {
        ZFBitSet(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_viewChildOnChange);
    }
    else if(eventId == ZFUIView::EventViewChildOnAdd())
    {
        ZFBitSet(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_viewChildOnAdd);
    }
    else if(eventId == ZFUIView::EventViewChildOnRemove())
    {
        ZFBitSet(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_viewChildOnRemove);
    }
    else if(eventId == ZFUIView::EventViewOnAddToParent())
    {
        ZFBitSet(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_viewOnAddToParent);
    }
    else if(eventId == ZFUIView::EventViewOnRemoveFromParent())
    {
        ZFBitSet(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_viewOnRemoveFromParent);
    }
}
void ZFUIView::observerOnRemove(ZF_IN zfidentity eventId)
{
    zfsuper::observerOnRemove(eventId);
    if(eventId == ZFUIView::EventViewPropertyOnUpdate())
    {
        ZFBitUnset(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_viewPropertyOnUpdate);
    }
    else if(eventId == ZFUIView::EventViewLayoutOnLayoutRequest())
    {
        ZFBitUnset(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_layoutOnLayoutRequest);
    }
    else if(eventId == ZFUIView::EventViewChildOnChange())
    {
        ZFBitUnset(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_viewChildOnChange);
    }
    else if(eventId == ZFUIView::EventViewChildOnAdd())
    {
        ZFBitUnset(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_viewChildOnAdd);
    }
    else if(eventId == ZFUIView::EventViewChildOnRemove())
    {
        ZFBitUnset(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_viewChildOnRemove);
    }
    else if(eventId == ZFUIView::EventViewOnAddToParent())
    {
        ZFBitUnset(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_viewOnAddToParent);
    }
    else if(eventId == ZFUIView::EventViewOnRemoveFromParent())
    {
        ZFBitUnset(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_viewOnRemoveFromParent);
    }
}

ZF_NAMESPACE_GLOBAL_END

