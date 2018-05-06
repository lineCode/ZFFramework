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

import com.ZFFramework.Android.NativeUtil.ZFAndroidRect;
import android.view.View;

public class ZFUIViewPositionOnScreen {
    private static int[] _viewPositionOnScreenCache = new int[2];
    public static Object native_viewPositionOnScreen(Object nativeView) {
        View nativeViewTmp = (View)nativeView;
        nativeViewTmp.getLocationOnScreen(_viewPositionOnScreenCache);
        return new ZFAndroidRect(_viewPositionOnScreenCache[0], _viewPositionOnScreenCache[1], nativeViewTmp.getWidth(), nativeViewTmp.getHeight());
    }
}
