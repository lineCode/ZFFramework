/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
/**
 * @file ZFProtocolZFUISysWindowEmbedNativeView.h
 * @brief protocol for ZFUISysWindow
 */

#ifndef _ZFI_ZFProtocolZFUISysWindowEmbedNativeView_h_
#define _ZFI_ZFProtocolZFUISysWindowEmbedNativeView_h_

#include "ZFCore/ZFProtocol.h"
#include "ZFUIKit/ZFUISysWindow.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for ZFUISysWindow
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFUISysWindowEmbedNativeView)
public:
    /**
     * @brief used to attach window's root view to native container
     *
     * the attached child must fill parent,
     * and change frame accorrding to parent's size automatically
     */
    virtual void nativeViewAdd(ZF_IN void *parent, ZF_IN void *child) zfpurevirtual;
    /**
     * @brief remove native view, see #nativeViewAdd
     */
    virtual void nativeViewRemote(ZF_IN void *parent, ZF_IN void *child) zfpurevirtual;
ZFPROTOCOL_INTERFACE_END(ZFUISysWindowEmbedNativeView)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFUISysWindowEmbedNativeView_h_

