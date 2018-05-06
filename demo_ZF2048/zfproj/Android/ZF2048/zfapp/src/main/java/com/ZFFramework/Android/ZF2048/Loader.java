/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
package com.ZFFramework.Android.ZF2048;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import com.ZFFramework.Android.ZF_impl.ZFMainEntry;

public class Loader extends Activity {
    static {
        System.loadLibrary("ZF2048");
    }
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if(BuildConfig.DEBUG) {
            ZFMainEntry.debugModeSet(true);
        }
    }
    private boolean _firstTime = true;
    @Override
    protected void onResume() {
        super.onResume();
        if(_firstTime) {
            _firstTime = false;
            Intent intent = new Intent(Loader.this, ZFMainEntry.class);
            startActivity(intent);
            Loader.this.finish();
        }
    }
}

