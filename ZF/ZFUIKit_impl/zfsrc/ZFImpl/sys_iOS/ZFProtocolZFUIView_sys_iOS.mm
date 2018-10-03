/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFImpl_sys_iOS_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIView.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIViewFocus.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIKeyboardState.h"

#if ZF_ENV_sys_iOS

#import <objc/runtime.h>

@interface _ZFP_ZFUIViewImpl_sys_iOS_View : UIView
@property (nonatomic, assign) ZFUIView *_ZFP_ownerZFUIView;
@property (nonatomic, strong) UIView *_ZFP_nativeImplView;
@property (nonatomic, assign) CGRect _ZFP_nativeImplViewFrame;
@property (nonatomic, assign) CGRect _ZFP_frame;
@property (nonatomic, strong) NSMutableArray *_ZFP_mouseRecords; // UITouch
@property (nonatomic, assign) BOOL _ZFP_uiEnable;
@property (nonatomic, assign) BOOL _ZFP_layoutRequested;
@property (nonatomic, assign) BOOL _ZFP_ZFUIViewFocus_viewFocusable;
@end
@implementation _ZFP_ZFUIViewImpl_sys_iOS_View
// ============================================================
// init and dealloc
- (id)init
{
    self = [super init];

    self._ZFP_mouseRecords = [NSMutableArray array];

    self.autoresizesSubviews = NO;
    self.multipleTouchEnabled = YES;
    self.exclusiveTouch = YES;
    self.clipsToBounds = YES;

    // status init
    self._ZFP_uiEnable = YES;
    self._ZFP_layoutRequested = NO;

    self._ZFP_ZFUIViewFocus_viewFocusable = NO;

    return self;
}
- (void)dealloc
{
    zfCoreAssert(self._ZFP_nativeImplView == nil);
}

// ============================================================
// ui and tree enable
- (UIView *)hitTest:(CGPoint)point withEvent:(UIEvent *)event
{
    UIView *tmp = [super hitTest:point withEvent:event];
    if(!self._ZFP_uiEnable && tmp == self)
    {
        return nil;
    }
    else
    {
        return tmp;
    }
}

// ============================================================
// frame and layout
- (void)set_ZFP_frame:(CGRect)newFrame ZFImpl_sys_iOS_overrideProperty
{
    self->__ZFP_frame = newFrame;
    self.frame = newFrame;
}
- (void)setNeedsLayout
{
    if(!self._ZFP_layoutRequested)
    {
        self._ZFP_layoutRequested = YES;
        if(self._ZFP_ownerZFUIView != zfnull && self._ZFP_ownerZFUIView->viewParent() == zfnull)
        {
            ZFPROTOCOL_ACCESS(ZFUIView)->notifyNeedLayout(self._ZFP_ownerZFUIView);
        }
    }
    [super setNeedsLayout];
}
- (CGSize)sizeThatFits:(CGSize)size
{
    return self._ZFP_frame.size;
}
- (void)layoutSubviews
{
    self._ZFP_layoutRequested = NO;
    [super layoutSubviews];
    if(self._ZFP_ownerZFUIView != zfnull && self._ZFP_ownerZFUIView->viewParent() == zfnull)
    {
        ZFPROTOCOL_ACCESS(ZFUIView)->notifyLayoutRootView(self._ZFP_ownerZFUIView, ZFImpl_sys_iOS_ZFUIKit_impl_ZFUIRectFromCGRect(self.frame));
    }

    NSArray *children = self.subviews;
    for(NSUInteger i = 0; i < [children count]; ++i)
    {
        UIView *child = [children objectAtIndex:i];
        if(child == self._ZFP_nativeImplView)
        {
            child.frame = self._ZFP_nativeImplViewFrame;
        }
        else if([child isKindOfClass:[_ZFP_ZFUIViewImpl_sys_iOS_View class]])
        {
            child.frame = ((_ZFP_ZFUIViewImpl_sys_iOS_View *)child)._ZFP_frame;
        }
        else
        {
            child.frame = self.bounds;
        }
    }
}

// ============================================================
// touches
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    // should not called otherwise touch would be passed to parent for a empty UIView
    // [super touchesBegan:touches withEvent:event];

    if(self._ZFP_ownerZFUIView == zfnull)
    {
        return ;
    }

    for(UITouch *touch in touches)
    {
        [self._ZFP_mouseRecords addObject:touch];

        zfautoObject evHolder = ZFUIMouseEvent::cacheHolder()->cacheGet(ZFUIMouseEvent::ClassData());
        ZFUIMouseEvent *ev = evHolder;
        ev->eventResolvedSet(zffalse);
        ev->mouseId = (zfidentity)[touch hash];
        ev->mouseAction = ZFUIMouseAction::e_MouseDown;
        ev->mousePoint = ZFImpl_sys_iOS_ZFUIKit_impl_ZFUIPointFromCGPoint([touch locationInView:self]);
        ev->mouseButton = ZFUIMouseButton::e_MouseButtonLeft;
        ZFPROTOCOL_ACCESS(ZFUIView)->notifyUIEvent(self._ZFP_ownerZFUIView, ev);
    }
}
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    // should not called otherwise touch would be passed to parent for a empty UIView
    // [super touchesMoved:touches withEvent:event];

    if(self._ZFP_ownerZFUIView == zfnull)
    {
        return ;
    }

    for(UITouch *touch in touches)
    {
        if([self._ZFP_mouseRecords indexOfObject:touch] == NSNotFound)
        {
            [self._ZFP_mouseRecords addObject:touch];
        }
    }

    for(UITouch *touch in self._ZFP_mouseRecords)
    {
        zfautoObject evHolder = ZFUIMouseEvent::cacheHolder()->cacheGet(ZFUIMouseEvent::ClassData());
        ZFUIMouseEvent *ev = evHolder;
        ev->eventResolvedSet(zffalse);
        ev->mouseId = (zfidentity)[touch hash];
        ev->mouseAction = ZFUIMouseAction::e_MouseMove;
        ev->mousePoint = ZFImpl_sys_iOS_ZFUIKit_impl_ZFUIPointFromCGPoint([touch locationInView:self]);
        ev->mouseButton = ZFUIMouseButton::e_MouseButtonLeft;
        ZFPROTOCOL_ACCESS(ZFUIView)->notifyUIEvent(self._ZFP_ownerZFUIView, ev);
    }
}
- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    // should not called otherwise touch would be passed to parent for a empty UIView
    // [super touchesEnded:touches withEvent:event];

    if(self._ZFP_ownerZFUIView == zfnull)
    {
        return ;
    }

    for(UITouch *touch in touches)
    {
        [self._ZFP_mouseRecords removeObject:touch];

        zfautoObject evHolder = ZFUIMouseEvent::cacheHolder()->cacheGet(ZFUIMouseEvent::ClassData());
        ZFUIMouseEvent *ev = evHolder;
        ev->eventResolvedSet(zffalse);
        ev->mouseId = (zfidentity)[touch hash];
        ev->mouseAction = ZFUIMouseAction::e_MouseUp;
        ev->mousePoint = ZFImpl_sys_iOS_ZFUIKit_impl_ZFUIPointFromCGPoint([touch locationInView:self]);
        ev->mouseButton = ZFUIMouseButton::e_MouseButtonLeft;
        ZFPROTOCOL_ACCESS(ZFUIView)->notifyUIEvent(self._ZFP_ownerZFUIView, ev);
    }
}
- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
    // should not called otherwise touch would be passed to parent for a empty UIView
    // [super touchesCancelled:touches withEvent:event];

    if(self._ZFP_ownerZFUIView == zfnull)
    {
        return ;
    }

    for(UITouch *touch in touches)
    {
        [self._ZFP_mouseRecords removeObject:touch];

        zfautoObject evHolder = ZFUIMouseEvent::cacheHolder()->cacheGet(ZFUIMouseEvent::ClassData());
        ZFUIMouseEvent *ev = evHolder;
        ev->eventResolvedSet(zffalse);
        ev->mouseId = (zfidentity)[touch hash];
        ev->mouseAction = ZFUIMouseAction::e_MouseCancel;
        ev->mousePoint = ZFImpl_sys_iOS_ZFUIKit_impl_ZFUIPointFromCGPoint([touch locationInView:self]);
        ev->mouseButton = ZFUIMouseButton::e_MouseButtonLeft;
        ZFPROTOCOL_ACCESS(ZFUIView)->notifyUIEvent(self._ZFP_ownerZFUIView, ev);
    }
}

// ============================================================
// ZFUIViewFocus
- (BOOL)canBecomeFirstResponder
{
    if(self._ZFP_nativeImplView != nil)
    {
        return [self._ZFP_nativeImplView canBecomeFirstResponder];
    }
    else
    {
        return self._ZFP_ZFUIViewFocus_viewFocusable;
    }
}
- (BOOL)becomeFirstResponder
{
    if(self._ZFP_nativeImplView != nil)
    {
        return [self._ZFP_nativeImplView becomeFirstResponder];
    }
    else
    {
        return [super becomeFirstResponder];
    }
}
- (BOOL)resignFirstResponder
{
    if(self._ZFP_nativeImplView != nil)
    {
        return [self._ZFP_nativeImplView resignFirstResponder];
    }
    else
    {
        return [super resignFirstResponder];
    }
}
@end

static void _ZFP_ZFUIViewImpl_sys_iOS_notifyViewFocusChanged(ZF_IN UIView *nativeImplView)
{
    _ZFP_ZFUIViewImpl_sys_iOS_View *nativeView = nil;
    if([nativeImplView isKindOfClass:[_ZFP_ZFUIViewImpl_sys_iOS_View class]])
    {
        nativeView = (_ZFP_ZFUIViewImpl_sys_iOS_View *)nativeImplView;
    }
    else if([nativeImplView.superview isKindOfClass:[_ZFP_ZFUIViewImpl_sys_iOS_View class]])
    {
        nativeView = (_ZFP_ZFUIViewImpl_sys_iOS_View *)nativeImplView.superview;
    }
    else if([nativeImplView.superview.superview isKindOfClass:[_ZFP_ZFUIViewImpl_sys_iOS_View class]])
    {
        nativeView = (_ZFP_ZFUIViewImpl_sys_iOS_View *)nativeImplView.superview.superview;
    }

    if(nativeView._ZFP_ownerZFUIView != zfnull)
    {
        ZFProtocolZFUIViewFocus_notifyViewFocusChanged(nativeView._ZFP_ownerZFUIView);
    }
}

@implementation UIResponder (_ZFP_ZFUIViewImpl_sys_iOS_MethodSwizzling)
- (BOOL)_ZFP_ZFUIViewImpl_sys_iOS_methodSwizzling_becomeFirstResponder
{
    BOOL old = self.isFirstResponder;
    BOOL ret = [self _ZFP_ZFUIViewImpl_sys_iOS_methodSwizzling_becomeFirstResponder];
    if(!old && self.isFirstResponder && [self isKindOfClass:[UIView class]])
    {
        _ZFP_ZFUIViewImpl_sys_iOS_notifyViewFocusChanged((UIView *)self);
    }
    return ret;
}
- (BOOL)_ZFP_ZFUIViewImpl_sys_iOS_methodSwizzling_resignFirstResponder
{
    BOOL old = self.isFirstResponder;
    BOOL ret = [self _ZFP_ZFUIViewImpl_sys_iOS_methodSwizzling_resignFirstResponder];
    if(old && !self.isFirstResponder && [self isKindOfClass:[UIView class]])
    {
        _ZFP_ZFUIViewImpl_sys_iOS_notifyViewFocusChanged((UIView *)self);
    }
    return ret;
}
@end

static void _ZFP_ZFUIViewImpl_sys_iOS_methodSwizzlePrepare(void)
{
    {
        Method methodOrg =  class_getInstanceMethod([UIResponder class], @selector(becomeFirstResponder));
        Method methodNew = class_getInstanceMethod([UIResponder class], @selector(_ZFP_ZFUIViewImpl_sys_iOS_methodSwizzling_becomeFirstResponder));
        method_exchangeImplementations(methodOrg, methodNew);
    }
    {
        Method methodOrg =  class_getInstanceMethod([UIResponder class], @selector(resignFirstResponder));
        Method methodNew = class_getInstanceMethod([UIResponder class], @selector(_ZFP_ZFUIViewImpl_sys_iOS_methodSwizzling_resignFirstResponder));
        method_exchangeImplementations(methodOrg, methodNew);
    }
}

static __weak id _ZFP_ZFUIViewImpl_sys_iOS_firstResponderRef = nil;
@interface UIResponder(_ZFP_ZFUIViewImpl_sys_iOS_FirstResponder)
+ (id)_ZFP_ZFUIViewImpl_sys_iOS_firstResponder;
@end
@implementation UIResponder(_ZFP_ZFUIViewImpl_sys_iOS_FirstResponder)
+ (id)_ZFP_ZFUIViewImpl_sys_iOS_firstResponder
{
     _ZFP_ZFUIViewImpl_sys_iOS_firstResponderRef = nil;
     [[UIApplication sharedApplication] sendAction:@selector(_ZFP_ZFUIViewImpl_sys_iOS_firstResponderFind:) to:nil from:nil forEvent:nil];
     return _ZFP_ZFUIViewImpl_sys_iOS_firstResponderRef;
}
-(void)_ZFP_ZFUIViewImpl_sys_iOS_firstResponderFind:(id)sender
{
    _ZFP_ZFUIViewImpl_sys_iOS_firstResponderRef = self;
}
@end

ZF_NAMESPACE_GLOBAL_BEGIN
ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIViewImpl_sys_iOS, ZFUIView, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("iOS:UIView")

public:
    zfoverride
    virtual void protocolOnInit(void)
    {
        zfsuper::protocolOnInit();
        static zfbool _needMethodSwizzling = zftrue;
        if(_needMethodSwizzling)
        {
            _needMethodSwizzling = zffalse;
            _ZFP_ZFUIViewImpl_sys_iOS_methodSwizzlePrepare();
        }
    }
    zfoverride
    virtual void protocolOnInitFinish(void)
    {
        zfsuper::protocolOnInitFinish();
        ZFUIKeyboardStateBuiltinImplRegister();
    }
    zfoverride
    virtual void protocolOnDeallocPrepare(void)
    {
        ZFUIKeyboardStateBuiltinImplUnregister();
        zfsuper::protocolOnDeallocPrepare();
    }

public:
    virtual zfbool nativeViewCacheOnSave(ZF_IN void *nativeView)
    {
        _ZFP_ZFUIViewImpl_sys_iOS_View *nativeViewTmp = (__bridge _ZFP_ZFUIViewImpl_sys_iOS_View *)nativeView;

        nativeViewTmp.hidden = NO;
        nativeViewTmp.alpha = 1;
        nativeViewTmp._ZFP_uiEnable = YES;
        nativeViewTmp.userInteractionEnabled = YES;
        nativeViewTmp.backgroundColor = nil;
        nativeViewTmp._ZFP_ZFUIViewFocus_viewFocusable = NO;

        [nativeViewTmp._ZFP_mouseRecords removeAllObjects];
        nativeViewTmp._ZFP_ownerZFUIView = zfnull;
        return zftrue;
    }
    virtual void nativeViewCacheOnRestore(ZF_IN ZFUIView *view,
                                          ZF_IN void *nativeView)
    {
        _ZFP_ZFUIViewImpl_sys_iOS_View *nativeViewTmp = (__bridge _ZFP_ZFUIViewImpl_sys_iOS_View *)nativeView;
        nativeViewTmp._ZFP_ownerZFUIView = view;
    }
    virtual void *nativeViewCreate(ZF_IN ZFUIView *view)
    {
        _ZFP_ZFUIViewImpl_sys_iOS_View *nativeView = [_ZFP_ZFUIViewImpl_sys_iOS_View new];
        nativeView._ZFP_ownerZFUIView = view;
        return (__bridge_retained void *)nativeView;
    }
    virtual void nativeViewDestroy(ZF_IN void *nativeView)
    {
        _ZFP_ZFUIViewImpl_sys_iOS_View *nativeViewTmp = (__bridge_transfer _ZFP_ZFUIViewImpl_sys_iOS_View *)nativeView;
        nativeViewTmp._ZFP_ownerZFUIView = zfnull;
        nativeViewTmp = nil;
    }

    virtual void nativeImplViewSet(ZF_IN ZFUIView *view,
                                   ZF_IN void *nativeImplViewOld,
                                   ZF_IN void *nativeImplView,
                                   ZF_IN zfindex virtualIndex)
    {
        _ZFP_ZFUIViewImpl_sys_iOS_View *nativeView = (__bridge _ZFP_ZFUIViewImpl_sys_iOS_View *)view->nativeView();
        if(nativeView._ZFP_nativeImplView != nil)
        {
            [nativeView._ZFP_nativeImplView removeFromSuperview];
        }
        nativeView._ZFP_nativeImplView = (__bridge UIView *)nativeImplView;
        if(nativeView._ZFP_nativeImplView != nil)
        {
            [nativeView insertSubview:nativeView._ZFP_nativeImplView atIndex:virtualIndex];
        }
    }
    virtual void nativeImplViewFrameSet(ZF_IN ZFUIView *view,
                                        ZF_IN const ZFUIRect &rect)
    {
        _ZFP_ZFUIViewImpl_sys_iOS_View *nativeView = (__bridge _ZFP_ZFUIViewImpl_sys_iOS_View *)view->nativeView();
        nativeView._ZFP_nativeImplViewFrame = ZFImpl_sys_iOS_ZFUIKit_impl_ZFUIRectToCGRect(rect);
    }
    virtual zffloat nativeViewScaleForImpl(ZF_IN void *nativeView)
    {
        return 1;
    }
    virtual zffloat nativeViewScaleForPhysicalPixel(ZF_IN void *nativeView)
    {
        UIScreen *ownerScreen = ((__bridge UIView *)nativeView).window.screen;
        return (zffloat)((ownerScreen == nil) ? [UIScreen mainScreen].scale : ownerScreen.scale);
    }

    // ============================================================
    // properties
public:
    virtual void viewVisibleSet(ZF_IN ZFUIView *view,
                                ZF_IN zfbool viewVisible)
    {
        ((__bridge _ZFP_ZFUIViewImpl_sys_iOS_View *)view->nativeView()).hidden = !viewVisible;
    }
    virtual void viewAlphaSet(ZF_IN ZFUIView *view,
                              ZF_IN zffloat viewAlpha)
    {
        ((__bridge _ZFP_ZFUIViewImpl_sys_iOS_View *)view->nativeView()).alpha = viewAlpha;
    }
    virtual void viewUIEnableSet(ZF_IN ZFUIView *view,
                                 ZF_IN zfbool viewUIEnable)
    {
        ((__bridge _ZFP_ZFUIViewImpl_sys_iOS_View *)view->nativeView())._ZFP_uiEnable = viewUIEnable;
    }
    virtual void viewUIEnableTreeSet(ZF_IN ZFUIView *view,
                                     ZF_IN zfbool viewUIEnableTree)
    {
        ((__bridge _ZFP_ZFUIViewImpl_sys_iOS_View *)view->nativeView()).userInteractionEnabled = viewUIEnableTree;
    }
    virtual void viewBackgroundColorSet(ZF_IN ZFUIView *view,
                                        ZF_IN const ZFUIColor &viewBackgroundColor)
    {
        ((__bridge _ZFP_ZFUIViewImpl_sys_iOS_View *)view->nativeView()).backgroundColor = ZFImpl_sys_iOS_ZFUIKit_impl_ZFUIColorToUIColor(viewBackgroundColor);
    }

public:
    virtual void childAdd(ZF_IN ZFUIView *parent,
                          ZF_IN ZFUIView *child,
                          ZF_IN zfindex virtualIndex,
                          ZF_IN ZFUIViewChildLayerEnum childLayer,
                          ZF_IN zfindex childLayerIndex)
    {
        [(__bridge _ZFP_ZFUIViewImpl_sys_iOS_View *)parent->nativeView()
            insertSubview:(__bridge UIView *)child->nativeView()
            atIndex:virtualIndex];
    }
    virtual void childRemove(ZF_IN ZFUIView *parent,
                             ZF_IN ZFUIView *child,
                             ZF_IN zfindex virtualIndex,
                             ZF_IN ZFUIViewChildLayerEnum childLayer,
                             ZF_IN zfindex childLayerIndex)
    {
        [(__bridge UIView *)child->nativeView() removeFromSuperview];
    }
    virtual void childRemoveAllForDealloc(ZF_IN ZFUIView *parent)
    {
        _ZFP_ZFUIViewImpl_sys_iOS_View *nativeView = (__bridge _ZFP_ZFUIViewImpl_sys_iOS_View *)parent->nativeView();
        NSArray *children = nativeView.subviews;
        if(children.count > 0
            && !(nativeView._ZFP_nativeImplView != nil && children.count == 1))
        {
            for(NSUInteger i = children.count - 1; i != (NSUInteger)-1; --i)
            {
                UIView *child = [children objectAtIndex:i];
                if(child != nativeView._ZFP_nativeImplView)
                {
                    [child removeFromSuperview];
                }
            }
        }
    }

public:
    virtual void viewFrameSet(ZF_IN ZFUIView *view,
                              ZF_IN const ZFUIRect &rect)
    {
        ((__bridge _ZFP_ZFUIViewImpl_sys_iOS_View *)view->nativeView())._ZFP_frame =
            ZFImpl_sys_iOS_ZFUIKit_impl_ZFUIRectToCGRect(rect);
    }

    virtual void layoutRequest(ZF_IN ZFUIView *view)
    {
        // iOS needs to setNeedsLayout recursively
        UIView *nativeView = (__bridge UIView *)view->nativeView();
        do
        {
            [nativeView setNeedsLayout];
            nativeView = nativeView.superview;
        } while(nativeView != nil);
    }

    virtual void measureNativeView(ZF_OUT ZFUISize &ret,
                                   ZF_IN void *nativeView,
                                   ZF_IN const ZFUISize &sizeHint)
    {
        ret = ZFUISizeMake(zfmMax(sizeHint.width, 0), zfmMax(sizeHint.height, 0));
        ZFImpl_sys_iOS_ZFUIKit_impl_ZFUISizeFromCGSizeT(ret,
            [(__bridge UIView *)nativeView sizeThatFits:ZFImpl_sys_iOS_ZFUIKit_impl_ZFUISizeToCGSize(ret)]);
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIViewImpl_sys_iOS)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFUIViewImpl_sys_iOS)

// ============================================================
// ZFUIViewFocus
ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIViewFocusImpl_sys_iOS, ZFUIViewFocus, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("iOS:UIView")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIView, "iOS:UIView")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()
public:
    virtual void viewFocusableSet(ZF_IN ZFUIView *view,
                                  ZF_IN zfbool viewFocusable)
    {
        _ZFP_ZFUIViewImpl_sys_iOS_View *nativeView = (__bridge _ZFP_ZFUIViewImpl_sys_iOS_View *)view->nativeView();
        nativeView._ZFP_ZFUIViewFocus_viewFocusable = viewFocusable;
        if(!viewFocusable)
        {
            [nativeView resignFirstResponder];
        }
    }
    virtual zfbool viewFocused(ZF_IN ZFUIView *view)
    {
        _ZFP_ZFUIViewImpl_sys_iOS_View *nativeView = (__bridge _ZFP_ZFUIViewImpl_sys_iOS_View *)view->nativeView();
        return (nativeView.isFirstResponder
            || (nativeView._ZFP_nativeImplView != nil && nativeView._ZFP_nativeImplView.isFirstResponder));
    }
    virtual ZFUIView *viewFocusFind(ZF_IN ZFUIView *view)
    {
        _ZFP_ZFUIViewImpl_sys_iOS_View *nativeView = (__bridge _ZFP_ZFUIViewImpl_sys_iOS_View *)view->nativeView();
        id focused = [UIResponder _ZFP_ZFUIViewImpl_sys_iOS_firstResponder];
        if(focused == nil || ![focused isKindOfClass:[UIView class]])
        {
            return zfnull;
        }
        ZFUIView *ret = zfnull;
        UIView *t = (UIView *)focused;
        while(t != nil)
        {
            if(ret == zfnull && [t isKindOfClass:[_ZFP_ZFUIViewImpl_sys_iOS_View class]])
            {
                ret = ((_ZFP_ZFUIViewImpl_sys_iOS_View *)t)._ZFP_ownerZFUIView;
            }
            if(t == nativeView)
            {
                return ret;
            }
            t = t.superview;
        }
        return zfnull;
    }
    virtual void viewFocusRequest(ZF_IN ZFUIView *view,
                                  ZF_IN zfbool viewFocus)
    {
        UIView *nativeView = (__bridge UIView *)view->nativeView();
        if(viewFocus)
        {
            [nativeView becomeFirstResponder];
        }
        else
        {
            [nativeView resignFirstResponder];
        }
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIViewFocusImpl_sys_iOS)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFUIViewFocusImpl_sys_iOS)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_iOS

