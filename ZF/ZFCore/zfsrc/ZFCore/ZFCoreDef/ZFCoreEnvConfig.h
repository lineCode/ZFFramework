/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
/**
 * @file ZFCoreEnvConfig.h
 * @brief global definition
 *
 * this file would be included by all files before any other files,
 * so put things here ONLY if necessary
 */
#ifndef _ZFI_ZFCoreEnvConfig_h_
#define _ZFI_ZFCoreEnvConfig_h_

/* ZFTAG_ADD_MODULE */
/**
 * @def ZF_ENV_ZFVERSION_MAIN
 * @brief see #ZF_ENV_ZFVERSION
 * @def ZF_ENV_ZFVERSION_SUB
 * @brief see #ZF_ENV_ZFVERSION
 * @def ZF_ENV_ZFVERSION_MINOR
 * @brief see #ZF_ENV_ZFVERSION
 * @def ZF_ENV_ZFVERSION
 * @brief version of ZFFramework, e.g. '0.1.0'
 */
#define ZF_ENV_ZFVERSION_MAIN()       0
#define ZF_ENV_ZFVERSION_SUB()        7
#define ZF_ENV_ZFVERSION_MINOR()      2

#define ZF_ENV_ZFVERSION() _ZFP_ZF_ENV_ZFVERSION(ZF_ENV_ZFVERSION_MAIN(), ZF_ENV_ZFVERSION_SUB(), ZF_ENV_ZFVERSION_MINOR())
#define _ZFP_ZF_ENV_ZFVERSION(vMain, vSub, vMinor) ZFM_TOSTRING(vMain) "." ZFM_TOSTRING(vSub) "." ZFM_TOSTRING(vMinor)

#if 0
// ZFTAG_ADD_MODULE
/*
 * to make doc with Doxygen, you should
 * specify INPUT and INCLUDE_PATH to the "zfsrc" dir of each module,
 * and:
 * ============================================================ */
FULL_PATH_NAMES        = NO
SORT_MEMBER_DOCS       = NO
MAX_INITIALIZER_LINES  = 0
RECURSIVE              = YES
HTML_FOOTER            = doc_footer.html
GENERATE_LATEX         = NO
ENABLE_PREPROCESSING   = YES
MACRO_EXPANSION        = YES
SKIP_FUNCTION_MACROS   = YES
EXPAND_ONLY_PREDEF     = YES
EXTRACT_PRIVATE        = NO
EXTRACT_LOCAL_CLASSES  = NO
EXTRACT_LOCAL_METHODS  = NO
AUTOLINK_SUPPORT       = YES
USE_MDFILE_AS_MAINPAGE = ../../../README.md

INPUT                  =
INPUT                  += ../../../README.md
INPUT                  += ../docs
INPUT                  += ../../../ZF/ZFAlgorithm/zfsrc
INPUT                  += ../../../ZF/ZFAlgorithm_impl/zfsrc
INPUT                  += ../../../ZF/ZFCore/zfsrc
INPUT                  += ../../../ZF/ZFCore_impl/zfsrc
INPUT                  += ../../../ZF/ZFLua/zfsrc
INPUT                  += ../../../ZF/ZFLua_impl/zfsrc
INPUT                  += ../../../ZF/ZFUIKit/zfsrc
INPUT                  += ../../../ZF/ZFUIKit_impl/zfsrc
INPUT                  += ../../../ZF/ZFUIWebKit/zfsrc
INPUT                  += ../../../ZF/ZFUIWebKit_impl/zfsrc
INPUT                  += ../../../ZF/ZFUIWidget/zfsrc
INPUT                  += ../../../ZF/ZFUtility/zfsrc
INPUT                  += ../../../ZF/ZF_impl/zfsrc

FILE_PATTERNS          = *.h *.hpp

INCLUDE_PATH           =
INCLUDE_PATH           += ../../../ZF/ZFAlgorithm/zfsrc
INCLUDE_PATH           += ../../../ZF/ZFAlgorithm_impl/zfsrc
INCLUDE_PATH           += ../../../ZF/ZFCore/zfsrc
INCLUDE_PATH           += ../../../ZF/ZFCore_impl/zfsrc
INCLUDE_PATH           += ../../../ZF/ZFLua/zfsrc
INCLUDE_PATH           += ../../../ZF/ZFLua_impl/zfsrc
INCLUDE_PATH           += ../../../ZF/ZFUIKit/zfsrc
INCLUDE_PATH           += ../../../ZF/ZFUIKit_impl/zfsrc
INCLUDE_PATH           += ../../../ZF/ZFUIWebKit/zfsrc
INCLUDE_PATH           += ../../../ZF/ZFUIWebKit_impl/zfsrc
INCLUDE_PATH           += ../../../ZF/ZFUIWidget/zfsrc
INCLUDE_PATH           += ../../../ZF/ZFUtility/zfsrc
INCLUDE_PATH           += ../../../ZF/ZF_impl/zfsrc

PREDEFINED            += ZF_DOXYGEN_FLAG
PREDEFINED            += ZF_NAMESPACE_GLOBAL_BEGIN:=
PREDEFINED            += ZF_NAMESPACE_GLOBAL_END:=
PREDEFINED            += ZF_NAMESPACE_GLOBAL_USE:=
PREDEFINED            += _ZFP_ZFIMPLEMENTS_DECLARE(...):=
PREDEFINED            += ZFIMPLEMENTS_DECLARE(...):=

EXPAND_AS_DEFINED     += ZF_NAMESPACE_BEGIN
EXPAND_AS_DEFINED     += ZF_NAMESPACE_END
EXPAND_AS_DEFINED     += ZF_NAMESPACE_USE
EXPAND_AS_DEFINED     += ZF_ENV_EXPORT
EXPAND_AS_DEFINED     += ZF_IN
EXPAND_AS_DEFINED     += ZF_IN_OPT
EXPAND_AS_DEFINED     += ZF_OUT
EXPAND_AS_DEFINED     += ZF_OUT_OPT
EXPAND_AS_DEFINED     += ZF_IN_OUT
EXPAND_AS_DEFINED     += ZF_IN_OUT_OPT
EXPAND_AS_DEFINED     += ZFT_INT_WEAK
EXPAND_AS_DEFINED     += ZFT_INT_STRONG
EXPAND_AS_DEFINED     += zfclass
EXPAND_AS_DEFINED     += zfclassFwd
EXPAND_AS_DEFINED     += zfclassPOD
EXPAND_AS_DEFINED     += zfclassLikePOD
EXPAND_AS_DEFINED     += zfclassNotPOD
EXPAND_AS_DEFINED     += zfabstract
EXPAND_AS_DEFINED     += zfextends
EXPAND_AS_DEFINED     += zfextendsPOD
EXPAND_AS_DEFINED     += zfextendsLikePOD
EXPAND_AS_DEFINED     += zfextendsNotPOD
EXPAND_AS_DEFINED     += zfoverride
EXPAND_AS_DEFINED     += zffinal
EXPAND_AS_DEFINED     += zfinterface
EXPAND_AS_DEFINED     += zfimplements
EXPAND_AS_DEFINED     += ZFCLASS_DISALLOW_COPY_CONSTRUCTOR
EXPAND_AS_DEFINED     += ZFCORETYPE_STRING_CONVERTER_DECLARE
EXPAND_AS_DEFINED     += ZFCOMPARER_DEFAULT_DECLARE
EXPAND_AS_DEFINED     += ZFCOMPARER_DEFAULT_DECLARE_ALIAS
EXPAND_AS_DEFINED     += ZFCOMPARER_DEFAULT_DECLARE_EXPLICIT
EXPAND_AS_DEFINED     += ZFCORE_POD_COMPARER_DECLARE
EXPAND_AS_DEFINED     += ZFCOREPOINTER_DECLARE
EXPAND_AS_DEFINED     += ZFM_CLASS_HAS_MEMBER_DECLARE
EXPAND_AS_DEFINED     += ZFFILTER_DECLARE
EXPAND_AS_DEFINED     += ZFPROTOCOL_INTERFACE_OPTIONAL
EXPAND_AS_DEFINED     += ZFPROTOCOL_INTERFACE_CLASS
EXPAND_AS_DEFINED     += ZFPROTOCOL_INTERFACE_BEGIN
EXPAND_AS_DEFINED     += ZFPROTOCOL_INTERFACE_END
EXPAND_AS_DEFINED     += ZFOBJECT_DECLARE
EXPAND_AS_DEFINED     += ZFOBJECT_DECLARE_WITH_CUSTOM_CTOR
EXPAND_AS_DEFINED     += ZFOBJECT_DECLARE_ABSTRACT
EXPAND_AS_DEFINED     += ZFOBJECT_DECLARE_ABSTRACT_WITH_CUSTOM_CTOR
EXPAND_AS_DEFINED     += _ZFP_ZFINTERFACE_DECLARE
EXPAND_AS_DEFINED     += ZFINTERFACE_DECLARE
EXPAND_AS_DEFINED     += ZFINTERFACE_DECLARE_WITH_CUSTOM_CTOR
EXPAND_AS_DEFINED     += ZFINTERFACE_ON_INIT_DECLARE
EXPAND_AS_DEFINED     += ZFINTERFACE_ON_INIT_DEFINE
EXPAND_AS_DEFINED     += ZFINTERFACE_ON_DEALLOC_DECLARE
EXPAND_AS_DEFINED     += ZFINTERFACE_ON_DEALLOC_DEFINE
EXPAND_AS_DEFINED     += ZFAUTOOBJECT_DECLARE
EXPAND_AS_DEFINED     += ZFAUTOOBJECT_DECLARE_WITH_CALLER_INFO
EXPAND_AS_DEFINED     += ZFIDMAP
EXPAND_AS_DEFINED     += ZFIDMAP_DETAIL
EXPAND_AS_DEFINED     += ZFIDMAP_GLOBAL
EXPAND_AS_DEFINED     += ZFIDMAP_GLOBAL_DETAIL
EXPAND_AS_DEFINED     += ZFOBSERVER_EVENT
EXPAND_AS_DEFINED     += ZFOBSERVER_EVENT_GLOBAL
EXPAND_AS_DEFINED     += ZFCLASS_SINGLETON_DECLARE
EXPAND_AS_DEFINED     += ZFCLASS_SINGLETON_DECLARE_DETAIL
EXPAND_AS_DEFINED     += ZFOBJECT_SINGLETON_DECLARE
EXPAND_AS_DEFINED     += ZFOBJECT_SINGLETON_DECLARE_DETAIL
EXPAND_AS_DEFINED     += _ZFP_ZFMETHOD_INVOKER
EXPAND_AS_DEFINED     += ZFMETHOD_INLINE_0
EXPAND_AS_DEFINED     += ZFMETHOD_INLINE_DETAIL_0
EXPAND_AS_DEFINED     += ZFMETHOD_DECLARE_0
EXPAND_AS_DEFINED     += ZFMETHOD_DECLARE_DETAIL_0
EXPAND_AS_DEFINED     += ZFMETHOD_INLINE_1
EXPAND_AS_DEFINED     += ZFMETHOD_INLINE_DETAIL_1
EXPAND_AS_DEFINED     += ZFMETHOD_DECLARE_1
EXPAND_AS_DEFINED     += ZFMETHOD_DECLARE_DETAIL_1
EXPAND_AS_DEFINED     += ZFMETHOD_INLINE_2
EXPAND_AS_DEFINED     += ZFMETHOD_INLINE_DETAIL_2
EXPAND_AS_DEFINED     += ZFMETHOD_DECLARE_2
EXPAND_AS_DEFINED     += ZFMETHOD_DECLARE_DETAIL_2
EXPAND_AS_DEFINED     += ZFMETHOD_INLINE_3
EXPAND_AS_DEFINED     += ZFMETHOD_INLINE_DETAIL_3
EXPAND_AS_DEFINED     += ZFMETHOD_DECLARE_3
EXPAND_AS_DEFINED     += ZFMETHOD_DECLARE_DETAIL_3
EXPAND_AS_DEFINED     += ZFMETHOD_INLINE_4
EXPAND_AS_DEFINED     += ZFMETHOD_INLINE_DETAIL_4
EXPAND_AS_DEFINED     += ZFMETHOD_DECLARE_4
EXPAND_AS_DEFINED     += ZFMETHOD_DECLARE_DETAIL_4
EXPAND_AS_DEFINED     += ZFMETHOD_INLINE_5
EXPAND_AS_DEFINED     += ZFMETHOD_INLINE_DETAIL_5
EXPAND_AS_DEFINED     += ZFMETHOD_DECLARE_5
EXPAND_AS_DEFINED     += ZFMETHOD_DECLARE_DETAIL_5
EXPAND_AS_DEFINED     += ZFMETHOD_INLINE_6
EXPAND_AS_DEFINED     += ZFMETHOD_INLINE_DETAIL_6
EXPAND_AS_DEFINED     += ZFMETHOD_DECLARE_6
EXPAND_AS_DEFINED     += ZFMETHOD_DECLARE_DETAIL_6
EXPAND_AS_DEFINED     += ZFMETHOD_INLINE_7
EXPAND_AS_DEFINED     += ZFMETHOD_INLINE_DETAIL_7
EXPAND_AS_DEFINED     += ZFMETHOD_DECLARE_7
EXPAND_AS_DEFINED     += ZFMETHOD_DECLARE_DETAIL_7
EXPAND_AS_DEFINED     += ZFMETHOD_INLINE_8
EXPAND_AS_DEFINED     += ZFMETHOD_INLINE_DETAIL_8
EXPAND_AS_DEFINED     += ZFMETHOD_DECLARE_8
EXPAND_AS_DEFINED     += ZFMETHOD_DECLARE_DETAIL_8
EXPAND_AS_DEFINED     += ZFMETHOD_FUNC_DECLARE_0
EXPAND_AS_DEFINED     += ZFMETHOD_FUNC_DECLARE_INLINE_0
EXPAND_AS_DEFINED     += ZFMETHOD_FUNC_DECLARE_1
EXPAND_AS_DEFINED     += ZFMETHOD_FUNC_DECLARE_INLINE_1
EXPAND_AS_DEFINED     += ZFMETHOD_FUNC_DECLARE_2
EXPAND_AS_DEFINED     += ZFMETHOD_FUNC_DECLARE_INLINE_2
EXPAND_AS_DEFINED     += ZFMETHOD_FUNC_DECLARE_3
EXPAND_AS_DEFINED     += ZFMETHOD_FUNC_DECLARE_INLINE_3
EXPAND_AS_DEFINED     += ZFMETHOD_FUNC_DECLARE_4
EXPAND_AS_DEFINED     += ZFMETHOD_FUNC_DECLARE_INLINE_4
EXPAND_AS_DEFINED     += ZFMETHOD_FUNC_DECLARE_5
EXPAND_AS_DEFINED     += ZFMETHOD_FUNC_DECLARE_INLINE_5
EXPAND_AS_DEFINED     += ZFMETHOD_FUNC_DECLARE_6
EXPAND_AS_DEFINED     += ZFMETHOD_FUNC_DECLARE_INLINE_6
EXPAND_AS_DEFINED     += ZFMETHOD_FUNC_DECLARE_7
EXPAND_AS_DEFINED     += ZFMETHOD_FUNC_DECLARE_INLINE_7
EXPAND_AS_DEFINED     += ZFMETHOD_FUNC_DECLARE_8
EXPAND_AS_DEFINED     += ZFMETHOD_FUNC_DECLARE_INLINE_8
EXPAND_AS_DEFINED     += ZFMETHOD_GENERIC_INVOKER_PARAMS
EXPAND_AS_DEFINED     += ZFOBJECT_ON_INIT_INLINE_1
EXPAND_AS_DEFINED     += ZFOBJECT_ON_INIT_DECLARE_1
EXPAND_AS_DEFINED     += ZFOBJECT_ON_INIT_INLINE_2
EXPAND_AS_DEFINED     += ZFOBJECT_ON_INIT_DECLARE_2
EXPAND_AS_DEFINED     += ZFOBJECT_ON_INIT_INLINE_3
EXPAND_AS_DEFINED     += ZFOBJECT_ON_INIT_DECLARE_3
EXPAND_AS_DEFINED     += ZFOBJECT_ON_INIT_INLINE_4
EXPAND_AS_DEFINED     += ZFOBJECT_ON_INIT_DECLARE_4
EXPAND_AS_DEFINED     += ZFOBJECT_ON_INIT_INLINE_5
EXPAND_AS_DEFINED     += ZFOBJECT_ON_INIT_DECLARE_5
EXPAND_AS_DEFINED     += ZFOBJECT_ON_INIT_INLINE_6
EXPAND_AS_DEFINED     += ZFOBJECT_ON_INIT_DECLARE_6
EXPAND_AS_DEFINED     += ZFOBJECT_ON_INIT_INLINE_7
EXPAND_AS_DEFINED     += ZFOBJECT_ON_INIT_DECLARE_7
EXPAND_AS_DEFINED     += ZFOBJECT_ON_INIT_INLINE_8
EXPAND_AS_DEFINED     += ZFOBJECT_ON_INIT_DECLARE_8
EXPAND_AS_DEFINED     += ZFCALLBACK_DECLARE
EXPAND_AS_DEFINED     += ZFCALLBACK_DECLARE_BEGIN
EXPAND_AS_DEFINED     += ZFCALLBACK_DECLARE_END
EXPAND_AS_DEFINED     += _ZFP_ZFCALLBACK_DECLARE_END_NO_ALIAS
EXPAND_AS_DEFINED     += ZFCALLBACK_LOCAL_BEGIN
EXPAND_AS_DEFINED     += ZFCALLBACK_LOCAL_END
EXPAND_AS_DEFINED     += _ZFP_ZFCALLBACK_INVOKER
EXPAND_AS_DEFINED     += ZFLISTENER_INLINE
EXPAND_AS_DEFINED     += ZFLISTENER_DECLARE
EXPAND_AS_DEFINED     += ZFLISTENER_PROTOTYPE_EXPAND
EXPAND_AS_DEFINED     += ZFLISTENER_LOCAL_BEGIN
EXPAND_AS_DEFINED     += ZFLISTENER_LOCAL_END
EXPAND_AS_DEFINED     += ZFCORE_PARAM_DECLARE_SELF
EXPAND_AS_DEFINED     += ZFCORE_PARAM
EXPAND_AS_DEFINED     += ZFCORE_PARAM_WITH_INIT
EXPAND_AS_DEFINED     += ZFCORE_PARAM_RETAIN
EXPAND_AS_DEFINED     += ZFCORE_PARAM_RETAIN_WITH_INIT
EXPAND_AS_DEFINED     += _ZFP_ZFCORE_PARAM_RETAIN_DECLARE
EXPAND_AS_DEFINED     += ZFPROPERTY_RETAIN
EXPAND_AS_DEFINED     += ZFPROPERTY_RETAIN_WITH_INIT
EXPAND_AS_DEFINED     += ZFPROPERTY_RETAIN_READONLY
EXPAND_AS_DEFINED     += ZFPROPERTY_RETAIN_DETAIL
EXPAND_AS_DEFINED     += ZFPROPERTY_ASSIGN
EXPAND_AS_DEFINED     += ZFPROPERTY_ASSIGN_WITH_INIT
EXPAND_AS_DEFINED     += ZFPROPERTY_ASSIGN_READONLY
EXPAND_AS_DEFINED     += ZFPROPERTY_ASSIGN_DETAIL
EXPAND_AS_DEFINED     += ZFPROPERTY_OVERRIDE_ON_INIT_DECLARE
EXPAND_AS_DEFINED     += ZFPROPERTY_OVERRIDE_ON_INIT_DECLARE_NO_AUTO_INIT
EXPAND_AS_DEFINED     += ZFPROPERTY_OVERRIDE_ON_INIT_INLINE
EXPAND_AS_DEFINED     += ZFPROPERTY_OVERRIDE_ON_INIT_INLINE_NO_AUTO_INIT
EXPAND_AS_DEFINED     += ZFPROPERTY_OVERRIDE_ON_DEALLOC_DECLARE
EXPAND_AS_DEFINED     += ZFPROPERTY_OVERRIDE_ON_DEALLOC_INLINE
EXPAND_AS_DEFINED     += ZFPROPERTY_OVERRIDE_ON_VERIFY_DECLARE
EXPAND_AS_DEFINED     += ZFPROPERTY_OVERRIDE_ON_VERIFY_INLINE
EXPAND_AS_DEFINED     += ZFPROPERTY_OVERRIDE_ON_ATTACH_DECLARE
EXPAND_AS_DEFINED     += ZFPROPERTY_OVERRIDE_ON_ATTACH_INLINE
EXPAND_AS_DEFINED     += ZFPROPERTY_OVERRIDE_ON_DETACH_DECLARE
EXPAND_AS_DEFINED     += ZFPROPERTY_OVERRIDE_ON_DETACH_INLINE
EXPAND_AS_DEFINED     += ZFTYPEID_DECLARE
EXPAND_AS_DEFINED     += ZFTYPEID_DECLARE_WITH_CUSTOM_WRAPPER
EXPAND_AS_DEFINED     += ZFTYPEID_ACCESS_ONLY_DECLARE
EXPAND_AS_DEFINED     += ZFTYPEID_ALIAS_DECLARE
EXPAND_AS_DEFINED     += ZFTYPEID_ALIAS_DECLARE_SPECIALIZATION
EXPAND_AS_DEFINED     += ZFTYPEID_DISABLE
EXPAND_AS_DEFINED     += ZFTYPEID_PROGRESS_DECLARE
EXPAND_AS_DEFINED     += ZFTYPEID_PROGRESS_DECLARE_BY_VALUE
EXPAND_AS_DEFINED     += ZFSTYLE_DEFAULT_DECLARE
EXPAND_AS_DEFINED     += ZFENUM_BEGIN
EXPAND_AS_DEFINED     += ZFENUM_VALUE
EXPAND_AS_DEFINED     += ZFENUM_VALUE_WITH_INIT
EXPAND_AS_DEFINED     += ZFENUM_SEPARATOR
EXPAND_AS_DEFINED     += ZFENUM_SEPARATOR_ALLOW_DUPLICATE_VALUE
EXPAND_AS_DEFINED     += ZFENUM_VALUE_REGISTER
EXPAND_AS_DEFINED     += ZFENUM_VALUE_REGISTER_WITH_NAME
EXPAND_AS_DEFINED     += ZFENUM_END
EXPAND_AS_DEFINED     += ZFENUM_END_WITH_DEFAULT
EXPAND_AS_DEFINED     += ZFENUM_END_FLAGS
EXPAND_AS_DEFINED     += ZFENUM_END_FLAGS_WITH_DEFAULT
EXPAND_AS_DEFINED     += ZFOUTPUT_TYPE
EXPAND_AS_DEFINED     += ZFOUTPUT_TYPE_TEMPLATE
EXPAND_AS_DEFINED     += ZFOUTPUT_TYPE_DECLARE
EXPAND_AS_DEFINED     += ZFEXPORT_VAR_DECLARE
EXPAND_AS_DEFINED     += ZFEXPORT_VAR_ALIAS_DECLARE
EXPAND_AS_DEFINED     += ZFEXPORT_VAR_VALUEREF_DECLARE
EXPAND_AS_DEFINED     += ZFEXPORT_VAR_READONLY_DECLARE
EXPAND_AS_DEFINED     += ZFEXPORT_VAR_READONLY_ALIAS_DECLARE
EXPAND_AS_DEFINED     += ZFEXPORT_VAR_READONLY_VALUEREF_DECLARE
EXPAND_AS_DEFINED     += _ZFP_ZFValue_method_DECLARE
EXPAND_AS_DEFINED     += _ZFP_ZFValueEditable_method_DECLARE

EXCLUDE_PATTERNS      += */*.java
EXCLUDE_PATTERNS      += */_repo/*
EXCLUDE_PATTERNS      += */_tmp/*

EXCLUDE_SYMBOLS       += _ZFP_*
EXCLUDE_SYMBOLS       += _ZFI_*
EXCLUDE_SYMBOLS       += _ZFT_*
EXCLUDE_SYMBOLS       += *zfstl_impl*
/* ============================================================ */
#endif

// ignore MSVC warnings
#if defined(_MSC_VER)
    // reference type warnings
    #pragma warning(disable:4180)

    // placement new for POD with no-param-constructor:
    //   new addr Type();
    #pragma warning(disable:4345)

    // array member init
    #pragma warning(disable:4351)

    // unsafe APIs
    #pragma warning(disable:4996)
#endif

#endif // #ifndef _ZFI_ZFCoreEnvConfig_h_

