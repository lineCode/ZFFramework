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

import java.lang.ref.WeakReference;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
import com.ZFFramework.Android.NativeUtil.ZFAndroidRect;
import com.ZFFramework.Android.NativeUtil.ZFAndroidSize;
import com.ZFFramework.Android.NativeUtil.ZFAndroidUI;
import com.ZFFramework.Android.ZF_impl.ZFMainEntry;
import android.app.Activity;
import android.app.Application;
import android.graphics.Rect;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.View;
import android.view.ViewTreeObserver.OnGlobalLayoutListener;
import android.view.Window;

public class ZFUIOnScreenKeyboardState {
    private static Application.ActivityLifecycleCallbacks _activityLifecycleCallback = new Application.ActivityLifecycleCallbacks() {
        private Handler _attachDelay = new Handler(new Handler.Callback() {
            @Override
            public boolean handleMessage(Message msg) {
                ZFUIOnScreenKeyboardState.keyboardStateRegister((Activity)msg.obj);
                return true;
            }
        });
        @Override
        public void onActivityCreated(Activity activity, Bundle savedInstanceState) {
            _attachDelay.sendMessage(Message.obtain(_attachDelay, 0, activity));
        }
        @Override
        public void onActivityDestroyed(Activity activity) {
            ZFUIOnScreenKeyboardState.keyboardStateUnregister(activity);
        }

        @Override public void onActivityStarted(Activity activity) {}
        @Override public void onActivityResumed(Activity activity) {}
        @Override public void onActivityPaused(Activity activity) {}
        @Override public void onActivityStopped(Activity activity) {}
        @Override public void onActivitySaveInstanceState(Activity activity, Bundle outState) {}
    };
    protected static void native_keyboardStaticInit() {
        ZFMainEntry.app().registerActivityLifecycleCallbacks(_activityLifecycleCallback);
        _activityLifecycleCallback.onActivityCreated(ZFMainEntry.mainEntryActivity(), null);
    }
    protected static void native_keyboardStaticCleanup() {
        ZFMainEntry.app().unregisterActivityLifecycleCallbacks(_activityLifecycleCallback);

        Iterator<Map.Entry<Activity, _RegisterFlagData> > entries = _keyboardStateRegisterFlag.entrySet().iterator();
        while(entries.hasNext()) {
            Map.Entry<Activity, _RegisterFlagData> entry = entries.next();
            ZFUIOnScreenKeyboardState.keyboardStateUnregister(entry.getKey());
        }
    }

    private static class _WindowData {
        public boolean keyboardStateDelaying = true;
        public final ZFAndroidRect keyboardFrame = new ZFAndroidRect();
    }
    private static Map<Window, _WindowData> _keyboardState = new HashMap<Window, _WindowData>();

    private static Handler _keyboardStateUpdater = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            Window window = (Window)msg.obj;
            _WindowData windowData = _keyboardState.get(window);
            if(windowData != null) {
                windowData.keyboardStateDelaying = false;
                ZFUIOnScreenKeyboardState.notifyKeyboardStateOnChange(window);
            }
        }
    };
    private static class _RegisterFlagData {
        public View activityRootView = null;
        public _OnGlobalLayoutListener globalLayoutListener = null;
        public _RegisterFlagData(View activityRootView, _OnGlobalLayoutListener globalLayoutListener) {
            this.activityRootView = activityRootView;
            this.globalLayoutListener = globalLayoutListener;
        }
    }
    private static Map<Activity, _RegisterFlagData> _keyboardStateRegisterFlag = new HashMap<Activity, _RegisterFlagData>();
    private static class _OnGlobalLayoutListener implements OnGlobalLayoutListener {
        private WeakReference<Activity> _activity = null;
        private WeakReference<View> _activityRootView = null;
        public _OnGlobalLayoutListener(Activity activity, View activityRootView) {
            _activity = new WeakReference<Activity>(activity);
            _activityRootView = new WeakReference<View>(activityRootView);
        }
        private int _heightDiffOld = 0;
        @Override
        public void onGlobalLayout() {
            Window window = _activity.get().getWindow();
            _WindowData windowData = _keyboardState.get(window);
            if(windowData == null) {
                windowData = new _WindowData();
                _keyboardState.put(window, windowData);
            }
            else {
                windowData.keyboardStateDelaying = true;
            }

            Rect r = new Rect();
            _activityRootView.get().getWindowVisibleDisplayFrame(r);

            int heightDiff = _activityRootView.get().getRootView().getHeight() - (r.bottom - r.top);
            ZFAndroidSize screenSize = ZFAndroidUI.screenSize(ZFMainEntry.appContext());
            if((heightDiff >= screenSize.height / 5) != (_heightDiffOld >= screenSize.height / 5)) {
                _heightDiffOld = heightDiff;
                _keyboardStateUpdater.sendMessage(Message.obtain(_keyboardStateUpdater, 0, window));
            }
        }
    };
    public static void keyboardStateRegister(Activity activity) {
        View activityRootView = ZFAndroidUI.activityRootView(activity);
        _OnGlobalLayoutListener listener = new _OnGlobalLayoutListener(activity, activityRootView);
        _keyboardStateRegisterFlag.put(activity, new _RegisterFlagData(activityRootView, listener));
        activityRootView.getViewTreeObserver().addOnGlobalLayoutListener(listener);
        _keyboardState.put(activity.getWindow(), new _WindowData());
    }
    public static void keyboardStateUnregister(Activity activity) {
        View activityRootView = ZFAndroidUI.activityRootView(activity);
        _RegisterFlagData flagData = _keyboardStateRegisterFlag.remove(activity);
        if(flagData != null) {
            activityRootView.getViewTreeObserver().removeOnGlobalLayoutListener(flagData.globalLayoutListener);
            _keyboardState.remove(activity.getWindow());
        }
    }

    public static boolean native_keyboardShowing() {
        return (_keyboardFrame.height > 0);
    }

    private static ZFAndroidRect _keyboardFrame = new ZFAndroidRect();
    public static Object native_keyboardFrame() {
        return _keyboardFrame;
    }

    /**
     * @brief for special conditions, such as ZFFramework's view tree was embeded to native view,
     *   you should manually notify keyboard changing state
     */
    public static void notifyKeyboardStateOnChange(Window window) {
        if(window == null) {
            _keyboardFrame.set(0, 0, 0, 0);
            return ;
        }
        _WindowData windowData = _keyboardState.get(window);
        if(windowData == null) {
            int old = _keyboardFrame.height;
            keyboardFrameUpdate(window, _keyboardFrame);
            if(_keyboardFrame.height != old) {
                ZFUIOnScreenKeyboardState.native_notifyKeyboardStateOnChange();
            }
        }
        else if(windowData.keyboardStateDelaying) {
            _keyboardFrame.set(windowData.keyboardFrame);
        }
        else {
            int old = windowData.keyboardFrame.height;
            keyboardFrameUpdate(window, windowData.keyboardFrame);
            _keyboardFrame.set(windowData.keyboardFrame);
            if(_keyboardFrame.height != old) {
                ZFUIOnScreenKeyboardState.native_notifyKeyboardStateOnChange();
            }
        }
    }
    public static void keyboardFrameUpdate(Window window, ZFAndroidRect outKeyboardFrame) {
        Rect rect = new Rect();
        window.getDecorView().getWindowVisibleDisplayFrame(rect);
        ZFAndroidSize screenSize = ZFAndroidUI.screenSize(ZFMainEntry.appContext());
        outKeyboardFrame.set(0, rect.bottom, screenSize.width,
            screenSize.height - rect.bottom);
    }
    private static native void native_notifyKeyboardStateOnChange();
}
