/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
package com.ZFFramework.Android.ZFUIKit_impl;

import android.view.View;

public class ZFUIViewPositionOnScreen {
    private static int[] _viewPositionOnScreenCache = new int[4];
    public static int[] native_viewPositionOnScreen(Object nativeView) {
        View nativeViewTmp = (View)nativeView;
        nativeViewTmp.getLocationOnScreen(_viewPositionOnScreenCache);
        _viewPositionOnScreenCache[2] = nativeViewTmp.getWidth();
        _viewPositionOnScreenCache[3] = nativeViewTmp.getHeight();
        return _viewPositionOnScreenCache;
    }
}
