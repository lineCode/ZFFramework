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

import com.ZFFramework.Android.NativeUtil.ZFAndroidNinePatch;
import android.graphics.Bitmap;
import android.graphics.Bitmap.Config;
import android.graphics.Canvas;
import android.graphics.drawable.BitmapDrawable;

public final class ZFUIImageIO {
    @SuppressWarnings("deprecation")
    public static Object native_imageApplyScale(float imageScale,
                                                Object nativeImage,
                                                int newSize_width, int newSize_height,
                                                int ninePatch_left, int ninePatch_top, int ninePatch_right, int ninePatch_bottom) {
        Bitmap bmp = Bitmap.createBitmap(newSize_width, newSize_height, Config.ARGB_8888);
        Canvas canvas = new Canvas(bmp);
        ZFAndroidNinePatch.drawNinePatch(
            ((BitmapDrawable)nativeImage).getBitmap(),
            1, // src and dst always have same scale
            ninePatch_left, ninePatch_top, ninePatch_right, ninePatch_bottom,
            canvas,
            newSize_width, newSize_height);
        return new BitmapDrawable(bmp);
    }
    @SuppressWarnings("deprecation")
    public static Object native_imageLoadFromColor(int color,
                                                   int size_width, int size_height) {
        Bitmap bmp = Bitmap.createBitmap(size_width, size_height, Config.ARGB_8888);
        Canvas canvas = new Canvas(bmp);
        canvas.drawColor(color);
        return new BitmapDrawable(bmp);
    }
}
