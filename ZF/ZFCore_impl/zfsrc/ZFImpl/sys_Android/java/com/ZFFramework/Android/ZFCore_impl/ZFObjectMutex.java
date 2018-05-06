/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
package com.ZFFramework.Android.ZFCore_impl;

import java.util.concurrent.locks.ReentrantLock;

public final class ZFObjectMutex {
    public static Object native_nativeMutexCreate() {
        return new ReentrantLock();
    }
    public static void native_mutexLock(Object nativeMutex) {
        ((ReentrantLock)nativeMutex).lock();
    }
    public static boolean native_mutexTryLock(Object nativeMutex) {
        return ((ReentrantLock)nativeMutex).tryLock();
    }
    public static void native_mutexUnlock(Object nativeMutex) {
        ((ReentrantLock)nativeMutex).unlock();
    }
}
