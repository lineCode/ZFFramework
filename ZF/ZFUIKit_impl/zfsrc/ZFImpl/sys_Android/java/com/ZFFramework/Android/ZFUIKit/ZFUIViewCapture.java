/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
package com.ZFFramework.Android.ZFUIKit;

import android.graphics.Bitmap;
import android.graphics.Bitmap.Config;
import android.graphics.Color;
import android.graphics.drawable.BitmapDrawable;
import android.view.View;

public class ZFUIViewCapture {
    @SuppressWarnings("deprecation")
    public static Object native_viewCapture(Object nativeView) {
        View nativeViewTmp = (View)nativeView;
        boolean drawingCacheEnabledSaved = nativeViewTmp.isDrawingCacheEnabled();
        int drawingCacheBackgroundColorSaved = nativeViewTmp.getDrawingCacheBackgroundColor();
        int drawingCacheQualitySaved = nativeViewTmp.getDrawingCacheQuality();
        nativeViewTmp.setDrawingCacheBackgroundColor(Color.TRANSPARENT);
        nativeViewTmp.setDrawingCacheQuality(View.DRAWING_CACHE_QUALITY_HIGH);
        nativeViewTmp.setDrawingCacheEnabled(true);
        nativeViewTmp.buildDrawingCache();
        Bitmap bmp = nativeViewTmp.getDrawingCache().copy(Config.ARGB_8888, true);
        nativeViewTmp.setDrawingCacheBackgroundColor(drawingCacheBackgroundColorSaved);
        nativeViewTmp.setDrawingCacheQuality(drawingCacheQualitySaved);
        nativeViewTmp.setDrawingCacheEnabled(drawingCacheEnabledSaved);
        nativeViewTmp.destroyDrawingCache();
        return new BitmapDrawable(bmp);
    }
}
