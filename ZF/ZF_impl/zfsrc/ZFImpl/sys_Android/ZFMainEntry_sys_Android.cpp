/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFImpl_sys_Android_ZF_impl.h"
#include "ZFCore/ZFMainEntry.h"

#if ZF_ENV_sys_Android

#define ZFImpl_sys_Android_JNI_ID_ZFMainEntry ZFImpl_sys_Android_JNI_ID(ZF_1impl_ZFMainEntry)
#define ZFImpl_sys_Android_JNI_NAME_ZFMainEntry ZFImpl_sys_Android_JNI_NAME(ZF_impl.ZFMainEntry)

// ============================================================
// main entry
JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFMainEntry,
                         void, native_1ZFFrameworkInit)
{
    ZFFrameworkInit();
}
JNI_METHOD_DECLARE_END()
JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFMainEntry,
                         void, native_1ZFFrameworkCleanup)
{
    ZFFrameworkCleanup();
}
JNI_METHOD_DECLARE_END()

JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFMainEntry,
                         jint, native_1ZFMainExecute,
                         jobjectArray params)
{
    ZFCoreArray<zfstring> paramsTmp;
    if(params != NULL)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        jsize count = JNIUtilGetArrayLength(jniEnv, params);
        for(jsize i = 0; i < count; ++i)
        {
            jobject e = JNIUtilGetObjectArrayElement(jniEnv, params, i);
            paramsTmp.add(ZFImpl_sys_Android_zfstringFromString(e));
            JNIUtilDeleteLocalRef(jniEnv, e);
        }
    }
    return ZFMainExecute(paramsTmp);
}
JNI_METHOD_DECLARE_END()

JNI_ONLOAD_ENTRY(vm, reserved)
{
    if(!JNIInit(vm, JNI_VERSION_1_6))
    {
        return -1;
    }

    return JNIGetDesiredVersion();
}

ZF_NAMESPACE_GLOBAL_BEGIN
// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFMainEntry_sys_Android_DataHolder, ZFLevelZFFrameworkStatic)
{
    this->_ownerJClass = zfnull;
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFMainEntry_sys_Android_DataHolder)
{
    if(this->_ownerJClass != zfnull)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        JNIUtilDeleteGlobalRef(jniEnv, _ownerJClass);
        this->_ownerJClass = zfnull;
    }
}
private:
    jclass _ownerJClass;
public:
    jclass getOwnerJClass(void)
    {
        if(this->_ownerJClass == zfnull)
        {
            JNIEnv *jniEnv = JNIGetJNIEnv();
            jobject tmp = JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_ZFMainEntry).c_str());
            this->_ownerJClass = (jclass)JNIUtilNewGlobalRef(jniEnv, tmp);
            JNIUtilDeleteLocalRef(jniEnv, tmp);
        }
        return this->_ownerJClass;
    }
ZF_GLOBAL_INITIALIZER_END(ZFMainEntry_sys_Android_DataHolder)
#define _ZFP_ZFMainEntry_sys_Android_ownerJClass \
    (ZF_GLOBAL_INITIALIZER_INSTANCE(ZFMainEntry_sys_Android_DataHolder)->getOwnerJClass())

// ============================================================
JNIObjectHolder ZFImpl_sys_Android_appContext(void)
{
    JNIEnv *jniEnv = JNIGetJNIEnv();
    static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, _ZFP_ZFMainEntry_sys_Android_ownerJClass, zfTextA("native_appContext"),
        JNIGetMethodSig(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object), JNIParamTypeContainer()
        ).c_str());
    jobject tmp = JNIUtilCallStaticObjectMethod(jniEnv,
        _ZFP_ZFMainEntry_sys_Android_ownerJClass,
        jmId);
    JNIObjectHolder ret(tmp);
    JNIUtilDeleteLocalRef(jniEnv, tmp);
    return ret;
}
JNIObjectHolder ZFImpl_sys_Android_assetManager(void)
{
    JNIEnv *jniEnv = JNIGetJNIEnv();
    static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, _ZFP_ZFMainEntry_sys_Android_ownerJClass, zfTextA("native_assetManager"),
        JNIGetMethodSig(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object), JNIParamTypeContainer()
        ).c_str());
    jobject tmp = JNIUtilCallStaticObjectMethod(jniEnv,
        _ZFP_ZFMainEntry_sys_Android_ownerJClass,
        jmId);
    JNIObjectHolder ret(tmp);
    JNIUtilDeleteLocalRef(jniEnv, tmp);
    return ret;
}
JNIObjectHolder ZFImpl_sys_Android_mainEntryActivity(void)
{
    JNIEnv *jniEnv = JNIGetJNIEnv();
    static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, _ZFP_ZFMainEntry_sys_Android_ownerJClass, zfTextA("native_mainEntryActivity"),
        JNIGetMethodSig(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object), JNIParamTypeContainer()
        ).c_str());
    jobject tmp = JNIUtilCallStaticObjectMethod(jniEnv, _ZFP_ZFMainEntry_sys_Android_ownerJClass, jmId);
    JNIObjectHolder ret(tmp);
    JNIUtilDeleteLocalRef(jniEnv, tmp);
    return ret;
}
ZF_NAMESPACE_GLOBAL_END

#include "ZFCore/ZFLogLevel.h"
JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFMainEntry,
                         void, native_1debugModeSet,
                         jboolean value)
{
    if(value)
    {
        ZFLogLevelSet(ZFLogLevel::e_Verbose);
    }
    else
    {
        ZFLogLevelSet(ZFLogLevel::EnumDefault());
    }
}
JNI_METHOD_DECLARE_END()

#endif

