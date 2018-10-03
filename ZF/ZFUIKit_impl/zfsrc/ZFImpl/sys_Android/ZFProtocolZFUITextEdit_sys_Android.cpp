/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFImpl_sys_Android_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUITextEdit.h"

#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN

#define ZFImpl_sys_Android_JNI_ID_ZFUITextEdit ZFImpl_sys_Android_JNI_ID(ZFUIKit_1impl_ZFUITextEdit)
#define ZFImpl_sys_Android_JNI_NAME_ZFUITextEdit ZFImpl_sys_Android_JNI_NAME(ZFUIKit_impl.ZFUITextEdit)

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUITextEditImpl_sys_Android, ZFUITextEdit, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("Android:EditText")

public:
    zfoverride
    virtual void protocolOnInit(void)
    {
        zfsuper::protocolOnInit();
        JNIEnv *jniEnv = JNIGetJNIEnv();
        jobject tmp = zfnull;

        tmp = JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_ZFUITextEdit).c_str());
        this->jclsOwner = (jclass)JNIUtilNewGlobalRef(jniEnv, tmp);
        JNIUtilDeleteLocalRef(jniEnv, tmp);
    }
    zfoverride
    virtual void protocolOnDealloc(void)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        JNIUtilDeleteGlobalRef(jniEnv, this->jclsOwner);
        zfsuper::protocolOnDealloc();
    }

public:
    virtual void *nativeTextEditCreate(ZF_IN ZFUITextEdit *textEdit)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_nativeTextEditCreate",
            JNIGetMethodSig(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object), JNIParamTypeContainer()
                .add(JNIPointerJNIType)
            ).c_str());
        jobject tmp = JNIUtilCallStaticObjectMethod(jniEnv, this->jclsOwner, jmId,
            JNIConvertZFObjectToJNIType(jniEnv, textEdit));
        jobject ret = JNIUtilNewGlobalRef(jniEnv, tmp);
        JNIUtilDeleteLocalRef(jniEnv, tmp);
        return ZFCastStatic(void *, ret);
    }
    virtual void nativeTextEditDestroy(ZF_IN ZFUITextEdit *textEdit,
                                       ZF_IN void *nativeTextEdit)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_nativeTextEditDestroy",
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());
        jobject nativeTextEditTmp = ZFCastStatic(jobject, nativeTextEdit);
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId, nativeTextEditTmp);
        JNIUtilDeleteGlobalRef(jniEnv, nativeTextEditTmp);
    }

    // ============================================================
    // properties
public:
    virtual void textEditEnableSet(ZF_IN ZFUITextEdit *textEdit,
                                   ZF_IN zfbool textEditEnable)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_textEditEnableSet",
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_boolean)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId,
            ZFCastStatic(jobject, textEdit->nativeImplView()),
            (jboolean)textEditEnable);
    }
    virtual void textEditSecuredSet(ZF_IN ZFUITextEdit *textEdit,
                                    ZF_IN zfbool textEditSecured)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_textEditSecuredSet",
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_boolean)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId,
            ZFCastStatic(jobject, textEdit->nativeImplView()),
            (jboolean)textEditSecured);
    }
    virtual void textEditKeyboardTypeSet(ZF_IN ZFUITextEdit *textEdit,
                                         ZF_IN ZFUITextEditKeyboardTypeEnum textEditKeyboardType)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_textEditKeyboardTypeSet",
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_int)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId,
            ZFCastStatic(jobject, textEdit->nativeImplView()),
            (jint)textEditKeyboardType);
    }
    virtual void textEditKeyboardReturnTypeSet(ZF_IN ZFUITextEdit *textEdit,
                                               ZF_IN ZFUITextEditKeyboardReturnTypeEnum textEditKeyboardReturnType)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_textEditKeyboardReturnTypeSet",
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_int)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId,
            ZFCastStatic(jobject, textEdit->nativeImplView()),
            (jint)textEditKeyboardReturnType);
    }

    virtual void textSelectRange(ZF_IN ZFUITextEdit *textEdit, ZF_OUT ZFIndexRange &textSelectRange)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId_start = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_textSelectRange_start",
            JNIGetMethodSig(JNIType::S_int, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());
        static jmethodID jmId_count = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_textSelectRange_count",
            JNIGetMethodSig(JNIType::S_int, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());
        textSelectRange.start = JNIUtilCallStaticIntMethod(jniEnv, this->jclsOwner, jmId_start,
            ZFCastStatic(jobject, textEdit->nativeImplView()));
        textSelectRange.count = JNIUtilCallStaticIntMethod(jniEnv, this->jclsOwner, jmId_count,
            ZFCastStatic(jobject, textEdit->nativeImplView()));
    }
    virtual void textSelectRangeSet(ZF_IN ZFUITextEdit *textEdit, ZF_IN const ZFIndexRange &textSelectRange)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_textSelectRangeSet",
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_int)
                .add(JNIType::S_int)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId,
            ZFCastStatic(jobject, textEdit->nativeImplView()),
            (jint)textSelectRange.start,
            (jint)textSelectRange.count);
    }

public:
    virtual void textSet(ZF_IN ZFUITextEdit *textEdit,
                         ZF_IN const zfchar *text)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_textSet",
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId,
            ZFCastStatic(jobject, textEdit->nativeImplView()),
            ZFCastStatic(jobject, ZFImpl_sys_Android_zfstringToString(text)));
    }
    virtual void textAppearanceSet(ZF_IN ZFUITextEdit *textEdit,
                                   ZF_IN ZFUITextAppearanceEnum const &textAppearance)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_textAppearanceSet",
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_int)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId,
            ZFCastStatic(jobject, textEdit->nativeImplView()),
            ZFCastStatic(jint, textAppearance));
    }
    virtual void textAlignSet(ZF_IN ZFUITextEdit *textEdit,
                              ZF_IN ZFUIAlignFlags const &textAlign)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_textAlignSet",
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_int)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId,
            ZFCastStatic(jobject, textEdit->nativeImplView()),
            ZFCastStatic(jint, textAlign));
    }
    virtual void textColorSet(ZF_IN ZFUITextEdit *textEdit,
                              ZF_IN ZFUIColor const &textColor)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_textColorSet",
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_int)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId,
            ZFCastStatic(jobject, textEdit->nativeImplView()),
            ZFImpl_sys_Android_ZFUIKit_impl_ZFUIColorToColor(textColor));
    }
    virtual void textShadowColorSet(ZF_IN ZFUITextEdit *textEdit,
                                    ZF_IN ZFUIColor const &textShadowColor)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_textShadowColorSet",
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_int)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId,
            ZFCastStatic(jobject, textEdit->nativeImplView()),
            ZFImpl_sys_Android_ZFUIKit_impl_ZFUIColorToColor(textShadowColor));
    }
    virtual void textShadowOffsetSet(ZF_IN ZFUITextEdit *textEdit,
                                     ZF_IN ZFUISize const &textShadowOffset)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_textShadowOffsetSet",
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_int)
                .add(JNIType::S_int)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId,
            ZFCastStatic(jobject, textEdit->nativeImplView()),
            ZFCastStatic(jint, textShadowOffset.width),
            ZFCastStatic(jint, textShadowOffset.height));
    }
    virtual void textSizeSet(ZF_IN ZFUITextEdit *textEdit,
                             ZF_IN zfint textSize)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_textSizeSet",
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_int)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId,
            ZFCastStatic(jobject, textEdit->nativeImplView()),
            ZFCastStatic(jint, textSize));
    }

    // ============================================================
    // layout
public:
    virtual ZFUISize measureNativeTextEdit(ZF_IN ZFUITextEdit *textEdit,
                                           ZF_IN const ZFUISize &sizeHint,
                                           ZF_IN zfint textSize)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_measureNativeTextEdit",
            JNIGetMethodSig(JNIType::S_array(JNIType::S_int), JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_int)
                .add(JNIType::S_int)
                .add(JNIType::S_int)
            ).c_str());
        jintArray jobjSize = (jintArray)JNIUtilCallStaticObjectMethod(jniEnv, this->jclsOwner, jmId,
            ZFCastStatic(jobject, textEdit->nativeImplView()),
            ZFCastStatic(jint, sizeHint.width),
            ZFCastStatic(jint, sizeHint.height),
            ZFCastStatic(jint, textSize));
        jint *jarrSize = JNIUtilGetIntArrayElements(jniEnv, jobjSize, NULL);
        ZFUISize ret = ZFUISizeMake((zfint)jarrSize[0], (zfint)jarrSize[1]);
        JNIUtilReleaseIntArrayElements(jniEnv, jobjSize, jarrSize, JNI_ABORT);
        JNIUtilDeleteLocalRef(jniEnv, jobjSize);
        return ret;
    }

    // ============================================================
    // edit
public:
    virtual void textEditBegin(ZF_IN ZFUITextEdit *textEdit)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_textEditBegin",
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId,
            ZFCastStatic(jobject, textEdit->nativeImplView()));
    }
    virtual void textEditEnd(ZF_IN ZFUITextEdit *textEdit)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_textEditEnd",
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId,
            ZFCastStatic(jobject, textEdit->nativeImplView()));
    }

private:
    jclass jclsOwner;
ZFPROTOCOL_IMPLEMENTATION_END(ZFUITextEditImpl_sys_Android)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFUITextEditImpl_sys_Android)

ZF_NAMESPACE_GLOBAL_END

JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFUITextEdit,
                         jboolean, native_1notifyCheckTextShouldChange,
                         JNIPointer zfjniPointerOwnerZFUITextEdit,
                         jobject jobjNewString)
{
    return (jboolean)ZFPROTOCOL_ACCESS(ZFUITextEdit)->notifyCheckTextShouldChange(
        ZFCastZFObject(ZFUITextEdit *, JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUITextEdit)),
        ZFImpl_sys_Android_zfstringFromString(ZFCastStatic(jstring, jobjNewString)));
}
JNI_METHOD_DECLARE_END()
JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFUITextEdit,
                         void, native_1notifyTextChange,
                         JNIPointer zfjniPointerOwnerZFUITextEdit,
                         jobject jobjNewString)
{
    ZFPROTOCOL_ACCESS(ZFUITextEdit)->notifyTextChange(
        ZFCastZFObject(ZFUITextEdit *, JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUITextEdit)),
        ZFImpl_sys_Android_zfstringFromString(ZFCastStatic(jstring, jobjNewString)));
}
JNI_METHOD_DECLARE_END()
JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFUITextEdit,
                         void, native_1notifyTextSelectRangeChange,
                         JNIPointer zfjniPointerOwnerZFUITextEdit)
{
    ZFPROTOCOL_ACCESS(ZFUITextEdit)->notifyTextSelectRangeChange(
        ZFCastZFObject(ZFUITextEdit *, JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUITextEdit)));
}
JNI_METHOD_DECLARE_END()
JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFUITextEdit,
                         void, native_1notifyTextReturnClicked,
                         JNIPointer zfjniPointerOwnerZFUITextEdit)
{
    ZFPROTOCOL_ACCESS(ZFUITextEdit)->notifyTextReturnClicked(
        ZFCastZFObject(ZFUITextEdit *, JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUITextEdit)));
}
JNI_METHOD_DECLARE_END()
JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFUITextEdit,
                         void, native_1notifyTextEditBegin,
                         JNIPointer zfjniPointerOwnerZFUITextEdit)
{
    ZFPROTOCOL_ACCESS(ZFUITextEdit)->notifyTextEditBegin(
        ZFCastZFObject(ZFUITextEdit *, JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUITextEdit)));
}
JNI_METHOD_DECLARE_END()
JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFUITextEdit,
                         void, native_1notifyTextEditEnd,
                         JNIPointer zfjniPointerOwnerZFUITextEdit)
{
    ZFPROTOCOL_ACCESS(ZFUITextEdit)->notifyTextEditEnd(
        ZFCastZFObject(ZFUITextEdit *, JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUITextEdit)));
}
JNI_METHOD_DECLARE_END()

#endif // #if ZF_ENV_sys_Android

