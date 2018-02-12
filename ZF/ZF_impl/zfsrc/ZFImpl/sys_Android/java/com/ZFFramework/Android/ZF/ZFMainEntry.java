/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
package com.ZFFramework.Android.ZF;

import java.lang.ref.WeakReference;
import java.util.ArrayList;
import java.util.List;
import com.ZFFramework.Android.NativeUtil.ZFAndroidLog;
import android.app.Activity;
import android.app.Application;
import android.content.Context;
import android.content.res.AssetManager;
import android.content.res.Configuration;
import android.os.Bundle;
import android.util.AttributeSet;
import android.view.View;
import android.view.ViewGroup.LayoutParams;
import android.view.Window;
import android.view.WindowManager;
import android.widget.FrameLayout;

/**
 * @brief main entry as a Activity in Android
 *
 * under Android, start ZFMainEntry as a Activity
 * to startup main entry of ZFFramework,
 * ZFMainEntry must be single task and only one instance is allowed\n
 * before use, you must declare in your AndroidManifest.xml like this:
 * @code
 *   <activity
 *       android:name="com.ZFFramework.Android.ZFCore.ZFMainEntry"
 *       android:label="ZFFramework"
 *       android:launchMode="singleTask" >
 *   </activity>
 * @endcode
 * \n
 * typical start steps:
 * -  start your launcher activity
 * -  load any so files of ZFFramework and your app's cpp code
 * -  start ZFMainEntry as single task activity
 * -  finish your launcher activity
 *
 * if you do not want the extra main entry activity,
 * you may use ZFMainEntry.mainEntryAttach/ZFMainEntry.mainEntryDetach
 * to attach to your own existing activity\n
 * \n
 * note, typically, ZFMainEntry only used as entry point and state storage,
 * a ZFUISysWindow would be created to hold actual UI,
 * see #ZFUISysWindow::nativeWindowEmbed
 */
public final class ZFMainEntry extends Activity {
    // ============================================================
    // main entry setup
    public static boolean mainEntryAttached() {
        return (_mainEntryActivity != null && _mainEntryActivity.get() != null);
    }
    public static void mainEntryAttach(Activity activity) {
        _mainEntryActivity = new WeakReference<Activity>(activity);
        ZFFrameworkInit();
        ZFMainExecute(null);
    }
    public static void mainEntryAttach(Activity activity, String[] params) {
        _mainEntryActivity = new WeakReference<Activity>(activity);
        ZFFrameworkInit();
        ZFMainExecute(params);
    }
    public static void mainEntryDetach() {
        ZFFrameworkCleanup();
        _mainEntryActivity = null;
    }

    // ============================================================
    // debug mode
    private static boolean _debugMode = false;
    public static void debugModeSet(boolean value) {
        _debugMode = value;
        ZFMainEntry.native_debugModeSet(value);
    }
    public static boolean debugMode() {
        return _debugMode;
    }
    private native static void native_debugModeSet(boolean value);

    // ============================================================
    // global state
    private static WeakReference<Application> _app = null;
    public static Application app() {
        if(_app == null || _app.get() == null) {
            _app = new WeakReference<Application>(mainEntryActivity().getApplication());
        }
        return _app.get();
    }
    public static Context appContext() {
        return _mainEntryActivity.get().getApplicationContext();
    }

    public static AssetManager assetManager() {
        return ZFMainEntry.appContext().getAssets();
    }

    private static WeakReference<Activity> _mainEntryActivity = null;
    public static Activity mainEntryActivity() {
        return _mainEntryActivity.get();
    }

    // ============================================================
    // activity entry
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);

        if(!ZFMainEntry.mainEntryAttached()) {
            ZFMainEntry.mainEntryAttach(this);
        }
    }
    @Override
    protected void onDestroy() {
        super.onDestroy();
        if(ZFMainEntry.mainEntryAttached() && ZFMainEntry.mainEntryActivity() == this) {
            ZFMainEntry.mainEntryDetach();
        }
    }

    // ============================================================
    // native
    private static boolean _mainEntryIsStarted = false;
    private static void ZFFrameworkInit() {
        if(!_mainEntryIsStarted) {
            _mainEntryIsStarted = true;
            native_ZFFrameworkInit();
        }
    }
    private static void ZFFrameworkCleanup() {
        if(_mainEntryIsStarted) {
            _mainEntryIsStarted = false;
            native_ZFFrameworkCleanup();
        }
    }
    private static int ZFMainExecute(String[] params) {
        return native_ZFMainExecute(params);
    }

    // ============================================================
    // native communication
    protected static Object native_appContext() {
        return ZFMainEntry.appContext();
    }
    protected static Object native_assetManager() {
        return ZFMainEntry.assetManager();
    }
    protected static Object native_mainEntryActivity() {
        return ZFMainEntry.mainEntryActivity();
    }
    private native static void native_ZFFrameworkInit();
    private native static void native_ZFFrameworkCleanup();
    private native static int native_ZFMainExecute(String[] params);
}
