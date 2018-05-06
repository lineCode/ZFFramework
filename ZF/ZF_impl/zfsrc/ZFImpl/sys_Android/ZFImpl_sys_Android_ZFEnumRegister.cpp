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

#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN

#define ZFImpl_sys_Android_JNI_ID_ZFEnum ZFImpl_sys_Android_JNI_ID(ZF_1impl_ZFEnum)
#define ZFImpl_sys_Android_JNI_NAME_ZFEnum ZFImpl_sys_Android_JNI_NAME(ZF_impl.ZFEnum)

ZF_NAMESPACE_GLOBAL_END

JNI_METHOD_DECLARE(jint, ZFImpl_sys_Android_JNI_ID_ZFEnum, native_1rawEnumValue,
                   JNIEnv *jniEnv, jclass jniCls,
                   jstring rawEnumValueName)
{
    const zfcharA *rawEnumValueNameT = JNIUtilGetStringUTFChars(jniEnv, rawEnumValueName, zfnull);
    zfuint ret = ZFEnumInvalid();
    do
    {
        const ZFMethod *method = ZFMethodFuncGet(ZFMethodFuncNamespaceGlobal, ZFStringA2Z(rawEnumValueNameT));
        if(method == zfnull) {break;}
        ret = method->executeStatic<zfuint>();
    } while(zffalse);
    JNIUtilReleaseStringUTFChars(jniEnv, rawEnumValueName, rawEnumValueNameT);
    return (jint)ret;
}
JNI_METHOD_DECLARE(jint, ZFImpl_sys_Android_JNI_ID_ZFEnum, native_1enumDefault,
                   JNIEnv *jniEnv, jclass jniCls,
                   jstring enumClassName)
{
    const zfcharA *enumClassNameT = JNIUtilGetStringUTFChars(jniEnv, enumClassName, zfnull);
    zfuint ret = ZFEnumInvalid();
    do
    {
        const ZFClass *cls = ZFClass::classForName(ZFStringA2Z(enumClassNameT));
        if(cls == zfnull || !cls->classIsTypeOf(ZFEnum::ClassData())) {break;}
        const ZFMethod *method = cls->methodForName(zfText("EnumDefault"));
        if(method == zfnull) {break;}
        ret = method->executeStatic<zfuint>();
    } while(zffalse);
    JNIUtilReleaseStringUTFChars(jniEnv, enumClassName, enumClassNameT);
    return (jint)ret;
}
JNI_METHOD_DECLARE(jint, ZFImpl_sys_Android_JNI_ID_ZFEnum, native_1enumValue,
                   JNIEnv *jniEnv, jclass jniCls,
                   jstring enumClassName,
                   jstring enumValueName)
{
    const zfcharA *enumClassNameT = JNIUtilGetStringUTFChars(jniEnv, enumClassName, zfnull);
    const zfcharA *enumValueNameT = JNIUtilGetStringUTFChars(jniEnv, enumValueName, zfnull);
    zfuint ret = ZFEnumInvalid();
    do
    {
        const ZFClass *cls = ZFClass::classForName(ZFStringA2Z(enumClassNameT));
        if(cls == zfnull || !cls->classIsTypeOf(ZFEnum::ClassData())) {break;}
        const ZFMethod *method = cls->methodForName(zfText("EnumValueForName"));
        if(method == zfnull) {break;}
        ret = method->executeStatic<zfuint, const zfchar *>(ZFStringA2Z(enumValueNameT));
    } while(zffalse);
    JNIUtilReleaseStringUTFChars(jniEnv, enumClassName, enumClassNameT);
    JNIUtilReleaseStringUTFChars(jniEnv, enumValueName, enumValueNameT);
    return (jint)ret;
}
JNI_METHOD_DECLARE(jstring, ZFImpl_sys_Android_JNI_ID_ZFEnum, native_1enumName,
                   JNIEnv *jniEnv, jclass jniCls,
                   jstring enumClassName,
                   jint enumValue)
{
    const zfcharA *enumClassNameT = JNIUtilGetStringUTFChars(jniEnv, enumClassName, zfnull);
    const zfchar *ret = ZFEnumNameInvalid();
    do
    {
        const ZFClass *cls = ZFClass::classForName(ZFStringA2Z(enumClassNameT));
        if(cls == zfnull || !cls->classIsTypeOf(ZFEnum::ClassData())) {break;}
        const ZFMethod *method = cls->methodForName(zfText("EnumNameForValue"));
        if(method == zfnull) {break;}
        ret = method->executeStatic<const zfchar *, zfuint>((zfuint)enumValue);
    } while(zffalse);
    JNIUtilReleaseStringUTFChars(jniEnv, enumClassName, enumClassNameT);
    return JNIUtilNewStringUTF(jniEnv, ZFStringZ2A(ret));
}

#endif // #if ZF_ENV_sys_Android

