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
 * @file ZFImpl_ZFLua.h
 * @brief global header for lua impl
 */

#ifndef _ZFI_ZFImpl_ZFLua_h_
#define _ZFI_ZFImpl_ZFLua_h_

#include "ZFLua.h"

#include "../_repo/ELuna.h"

ZF_NAMESPACE_GLOBAL_BEGIN

extern ZF_ENV_EXPORT lua_State *_ZFP_ZFImpl_ZFLua_luaState(void);
/**
 * @brief global lua state for lua impl
 *
 * lua state would be initialized during #ZFFrameworkInit as level #ZFLevelAppEssential\n
 * \n
 * if you want to setup existing lua_State instead of #ZFImpl_ZFLua_luaState,
 * you must first invoke #ZFImpl_ZFLua_luaStateAttach\n
 * \n
 * \n
 * ADVANCED: how the impl works:
 * -  all attach steps can be registered by #ZFImpl_ZFLua_implSetupCallback_DEFINE,
 *   when #ZFImpl_ZFLua_luaStateAttach,
 *   all necessary register steps would be performed
 * -  in ZFLua, all types are wrapped by #zfautoObject
 *   (wrapped directly for ZFObject type,
 *   wrapped by #ZFTypeIdWrapper for non-ZFObject type)
 * -  all global variables are also wrapped by #zfautoObject
 * -  class and topmost namespace are wrapped by lua raw string value as a global variable,
 *   and should be registered by #ZFImpl_ZFLua_implSetupScope\n
 *   these things are equal:
 *   -  `MyClass.myFunc(param);`
 *   -  `zfl_callStatic("MyClass.myFunc", param);`
 *   -  `local cls = MyClass; cls.myFunc(param);`
 * -  member functions are dispatched as `obj:myFunc(param)`,
 *   which equals to `obj.myFunc(obj, param)`
 * -  the method dispatch can be modified by registering #ZFImpl_ZFLua_implDispatch_DEFINE,
 *   if registered, zfl_call and zfl_callStatic would first check #ZFImpl_ZFLua_implDispatch
 * -  all #zfautoObject must be registered by #ZFImpl_ZFLua_implSetupObject,
 *   and all methods would be dispatched internally,
 *   you should not modify #zfautoObject's "__index" by other lua bind tools
 */
#define ZFImpl_ZFLua_luaState _ZFP_ZFImpl_ZFLua_luaState()

/**
 * @brief change #ZFImpl_ZFLua_luaState, can not undo
 */
extern ZF_ENV_EXPORT void ZFImpl_ZFLua_luaStateChange(ZF_IN lua_State *L);

/**
 * @brief see #ZFImpl_ZFLua_luaState
 *
 * must not be attached more than one time
 */
extern ZF_ENV_EXPORT void *ZFImpl_ZFLua_luaStateOpen(void);
/**
 * @brief see #ZFImpl_ZFLua_luaState
 */
extern ZF_ENV_EXPORT void ZFImpl_ZFLua_luaStateClose(ZF_IN lua_State *L);

/**
 * @brief see #ZFImpl_ZFLua_luaState
 *
 * must not be attached more than one time
 */
extern ZF_ENV_EXPORT void ZFImpl_ZFLua_luaStateAttach(ZF_IN lua_State *L);
/**
 * @brief see #ZFImpl_ZFLua_luaStateAttach
 *
 * note, this method only detach L from ZFFramework,
 * all methods registered to L can not be undo
 */
extern ZF_ENV_EXPORT void ZFImpl_ZFLua_luaStateDetach(ZF_IN lua_State *L);

/**
 * @brief return all lua_State that currently registered, for impl or debug use only
 */
extern ZF_ENV_EXPORT const ZFCoreArrayPOD<lua_State *> &ZFImpl_ZFLua_luaStateList(void);
/**
 * @brief see #ZFImpl_ZFLua_luaStateList
 */
extern ZF_ENV_EXPORT void ZFImpl_ZFLua_luaStateListT(ZF_IN_OUT ZFCoreArray<lua_State *> &ret);

// ============================================================
typedef void (*_ZFP_ZFImpl_ZFLua_ImplSetupCallback)(ZF_IN_OUT lua_State *L);
extern ZF_ENV_EXPORT void _ZFP_ZFImpl_ZFLua_implSetupCallbackRegister(ZF_IN _ZFP_ZFImpl_ZFLua_ImplSetupCallback setupAttachCallback,
                                                                      ZF_IN _ZFP_ZFImpl_ZFLua_ImplSetupCallback setupDetachCallback);
extern ZF_ENV_EXPORT void _ZFP_ZFImpl_ZFLua_implSetupCallbackUnregister(ZF_IN _ZFP_ZFImpl_ZFLua_ImplSetupCallback setupAttachCallback,
                                                                        ZF_IN _ZFP_ZFImpl_ZFLua_ImplSetupCallback setupDetachCallback);

/**
 * @brief for impl to add attach and detach actions
 *
 * usage:
 * @code
 *   ZFImpl_ZFLua_implSetupCallback_DEFINE(YourSetupSig, {
 *           // your own attach action, callback proto type:
 *           //   zfbool implSetupAttach(ZF_IN_OUT lua_State *L);
 *       }, {
 *           // your own detach action, callback proto type:
 *           //   zfbool implSetupDetach(ZF_IN_OUT lua_State *L);
 *       })
 * @endcode
 */
#define ZFImpl_ZFLua_implSetupCallback_DEFINE(SetupSig, setupAttachAction, setupDetachAction) \
    ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFImpl_ZFLua_implSetupAction_##SetupSig, ZFLevelZFFrameworkLow) \
    { \
        _ZFP_ZFImpl_ZFLua_implSetupCallbackRegister(zfself::implSetupAttach, zfself::implSetupDetach); \
    } \
    ZF_GLOBAL_INITIALIZER_DESTROY(ZFImpl_ZFLua_implSetupAction_##SetupSig) \
    { \
        _ZFP_ZFImpl_ZFLua_implSetupCallbackUnregister(zfself::implSetupAttach, zfself::implSetupDetach); \
    } \
    public: \
        static void implSetupAttach(ZF_IN_OUT lua_State *L) \
        { \
            setupAttachAction \
        } \
        static void implSetupDetach(ZF_IN_OUT lua_State *L) \
        { \
            setupDetachAction \
        } \
    ZF_GLOBAL_INITIALIZER_END(ZFImpl_ZFLua_implSetupAction_##SetupSig)

// ============================================================
/** @brief see #ZFImpl_ZFLua_luaState */
extern ZF_ENV_EXPORT void ZFImpl_ZFLua_implSetupScope(ZF_IN_OUT lua_State *L, ZF_IN const zfchar *scopeName);
/** @brief see #ZFImpl_ZFLua_luaState */
extern ZF_ENV_EXPORT void ZFImpl_ZFLua_implSetupScope(ZF_IN_OUT lua_State *L, ZF_IN const zfchar **scopeNameList);
/** @brief see #ZFImpl_ZFLua_luaState */
extern ZF_ENV_EXPORT void ZFImpl_ZFLua_implSetupObject(ZF_IN_OUT lua_State *L, ZF_IN_OPT int objIndex = -1);

// ============================================================
/** @brief see #ZFImpl_ZFLua_implDispatch_DEFINE */
typedef enum {
    ZFImpl_ZFLua_ImplDispatchResultForward, /**< @brief not dispatched, function should be further dispatched */
    ZFImpl_ZFLua_ImplDispatchResultSuccess, /**< @brief successfully dispatched, function call end */
    ZFImpl_ZFLua_ImplDispatchResultError, /**< @brief error occurred, #ZFImpl_ZFLua_ImplDispatchInfo::errorHint should be set */
} ZFImpl_ZFLua_ImplDispatchResult;

extern ZF_ENV_EXPORT const zfautoObject &_ZFP_ZFImpl_ZFLua_implDispatchReturnValueNotSet(void);
/**
 * @brief dummy object to indicate whether the dispatch has return value, see #ZFImpl_ZFLua_implDispatch_DEFINE
 */
#define ZFImpl_ZFLua_implDispatchReturnValueNotSet _ZFP_ZFImpl_ZFLua_implDispatchReturnValueNotSet()

/** @brief see #ZFImpl_ZFLua_implDispatch_DEFINE */
zfclassLikePOD ZF_ENV_EXPORT ZFImpl_ZFLua_ImplDispatchInfo
{
public:
    lua_State *L; /**< @brief the lua state */
    int luaParamOffset;  /**< @brief lua offset to first method param */
    zfbool isStatic; /**< @brief whether the method called as static method */
    const zfchar *classOrNamespace; /**< @brief class or namespace text */
    const ZFClass *classOrNull; /**< @brief class if able to find */
    ZFObject *objectOrNull; /**< @brief null for static method or non-null for instance method */
    const zfchar *methodName; /**< @brief method name */
    zfautoObject (&paramList)[ZFMETHOD_MAX_PARAM]; /**< @brief param list */
    zfindex paramCount; /**< @brief param count */
    zfautoObject returnValue; /**< @brief #ZFImpl_ZFLua_implDispatchReturnValueNotSet if no return value, see #returnValueCustom */
    int returnValueCustom; /**< @brief -1 by default, set non-negative to show custom return value specified, see #returnValue */
    zfstring errorHint; /**< @brief optional error hint to append */
    ZFImpl_ZFLua_ImplDispatchResult dispatchResult; /**< @brief dispatch result */
public:
    /** @brief main constructor */
    explicit ZFImpl_ZFLua_ImplDispatchInfo(ZF_IN_OUT lua_State *L,
                                           ZF_IN int luaParamOffset,
                                           ZF_IN zfbool isStatic,
                                           ZF_IN const zfchar *classOrNamespace,
                                           ZF_IN const ZFClass *classOrNull,
                                           ZF_IN ZFObject *objectOrNull,
                                           ZF_IN const zfchar *methodName,
                                           ZF_IN_OUT zfautoObject (&paramList)[ZFMETHOD_MAX_PARAM],
                                           ZF_IN zfindex paramCount)
    : L(L)
    , luaParamOffset(luaParamOffset)
    , isStatic(isStatic)
    , classOrNamespace(classOrNamespace)
    , classOrNull(classOrNull)
    , objectOrNull(objectOrNull)
    , methodName(methodName)
    , paramList(paramList)
    , paramCount(paramCount)
    , returnValue(ZFImpl_ZFLua_implDispatchReturnValueNotSet)
    , returnValueCustom(-1)
    , errorHint()
    , dispatchResult(ZFImpl_ZFLua_ImplDispatchResultForward)
    {
    }
public:
    /** @brief see #ZFImpl_ZFLua_implDispatch_DEFINE */
    void dispatchForward(void)
    {
        this->dispatchResult = ZFImpl_ZFLua_ImplDispatchResultForward;
    }
    /** @brief see #ZFImpl_ZFLua_implDispatch_DEFINE */
    void dispatchSuccess(void)
    {
        this->dispatchResult = ZFImpl_ZFLua_ImplDispatchResultSuccess;
    }
    /** @brief see #ZFImpl_ZFLua_implDispatch_DEFINE */
    void dispatchError(ZF_IN const zfchar *fmt, ...)
    {
        va_list vaList;
        va_start(vaList, fmt);
        zfstringAppendV(this->errorHint, fmt, vaList);
        va_end(vaList);

        this->dispatchResult = ZFImpl_ZFLua_ImplDispatchResultError;
    }
};

/**
 * @brief see #ZFImpl_ZFLua_implDispatch_DEFINE
 */
#define ZFImpl_ZFLua_implDispatchAll zfText("-")

/** @brief see #ZFImpl_ZFLua_implDispatch_DEFINE */
extern ZF_ENV_EXPORT void ZFImpl_ZFLua_implDispatch(ZF_IN_OUT ZFImpl_ZFLua_ImplDispatchInfo &dispatchInfo);

typedef void (*_ZFP_ZFImpl_ZFLua_ImplDispatchCallback)(ZF_IN_OUT ZFImpl_ZFLua_ImplDispatchInfo &dispatchInfo);
extern ZF_ENV_EXPORT void _ZFP_ZFImpl_ZFLua_implDispatchRegister(ZF_IN _ZFP_ZFImpl_ZFLua_ImplDispatchCallback invokerCallback,
                                                                 ZF_IN_OPT const zfchar *classOrNamespace = zfnull,
                                                                 ZF_IN_OPT const zfchar *methodName = zfnull);
extern ZF_ENV_EXPORT void _ZFP_ZFImpl_ZFLua_implDispatchUnregister(ZF_IN _ZFP_ZFImpl_ZFLua_ImplDispatchCallback invokerCallback,
                                                                   ZF_IN_OPT const zfchar *classOrNamespace = zfnull,
                                                                   ZF_IN_OPT const zfchar *methodName = zfnull);
/**
 * @brief register custom method dispatch callback
 *
 * by default, all method would be dispatched by #ZFMethod reflection for #ZFObject type\n
 * for non-ZFObject type, you may want to register custom dispatch callback
 * to implement `obj:myFunc()` or `MyClass.myStaticFunc()`\n
 * to achieve this, the object or namespace must first be registered by
 * #ZFImpl_ZFLua_implSetupObject or #ZFImpl_ZFLua_implSetupScope,
 * then register the callback by this macro\n
 * \n
 * usage:
 * @code
 *   ZFImpl_ZFLua_implDispatch_DEFINE(YourRegisterSig, yourClassOrNamespace, yourMethodName, {
 *           // dispatch by your own logic, callback proto type:
 *           //   void implDispatch(ZF_IN_OUT ZFImpl_ZFLua_ImplDispatchInfo &dispatchInfo);
 *           if(someErrorCond)
 *           {
 *               return dispatchInfo.dispatchError(yourErrorHint);
 *           }
 *           if(someCond)
 *           {
 *               dispatchInfo.returnValue = yourRet; // specify return value if exist
 *               return dispatchInfo.dispatchSuccess();
 *           }
 *           return dispatchInfo.dispatchForward();
 *       })
 * @endcode
 * \n
 * params:
 * -  classOrNamespace : null or empty or #ZF_NAMESPACE_GLOBAL_NAME or #ZF_NAMESPACE_GLOBAL_ABBR_NAME for global scope function,
 *   otherwise, should be the class name or namespace
 *   registered by #ZFImpl_ZFLua_implSetupScope
 * -  methodName : the method name
 *
 * there are different methods to register the dispatch callback:
 * -  methodName is null
 *   -  classOrNamespace is #ZFImpl_ZFLua_implDispatchAll : all method would be dispatch to this
 *   -  classOrNamespace is not #ZFImpl_ZFLua_implDispatchAll : all method with the specified scope name would be dispatched
 * -  methodName is not null
 *   -  classOrNamespace is #ZFImpl_ZFLua_implDispatchAll : dispatch specified methodName to all class
 *   -  classOrNamespace is not #ZFImpl_ZFLua_implDispatchAll : exactly dispatch (note that there may be overloaded methods)
 *
 * \n
 * the dispatch callback:\n
 * return false if not dispatched, and should be further dispatch by default #ZFMethod reflection,
 * return true if successfully processed
 */
#define ZFImpl_ZFLua_implDispatch_DEFINE(RegisterSig, classOrNamespace_, methodName_, dispatchAction) \
    ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFImpl_ZFLua_implDispatchDefine_##RegisterSig, ZFLevelZFFrameworkLow) \
    { \
        _ZFP_ZFImpl_ZFLua_implDispatchRegister(zfself::implDispatch, \
            classOrNamespace_, \
            methodName_); \
    } \
    ZF_GLOBAL_INITIALIZER_DESTROY(ZFImpl_ZFLua_implDispatchDefine_##RegisterSig) \
    { \
        _ZFP_ZFImpl_ZFLua_implDispatchUnregister(zfself::implDispatch, \
            classOrNamespace_, \
            methodName_); \
    } \
    static void implDispatch(ZF_IN_OUT ZFImpl_ZFLua_ImplDispatchInfo &dispatchInfo) \
    { \
        dispatchAction \
    } \
    ZF_GLOBAL_INITIALIZER_END(ZFImpl_ZFLua_implDispatchDefine_##RegisterSig)
/** @brief util macro to #ZFImpl_ZFLua_implDispatch_DEFINE for short */
#define ZFImpl_ZFLua_implDispatch_DEFINE_DEFAULT(ClassSig, methodNameSig, dispatchAction) \
    ZFImpl_ZFLua_implDispatch_DEFINE( \
        ClassSig##_##methodNameSig, \
        ZFM_TOSTRING(ClassSig), \
        ZFM_TOSTRING(methodNameSig), \
        dispatchAction)

/** @brief util macro for impl */
#define ZFImpl_ZFLua_implDispatch_AssertParamCount(N) \
    do { \
        if(dispatchInfo.paramCount != N) \
        { \
            return dispatchInfo.dispatchError( \
                zfText("%s::%s expect %zi param, got %zi"), \
                dispatchInfo.classOrNamespace, dispatchInfo.methodName, \
                N, dispatchInfo.paramCount); \
        } \
    } while(zffalse)
/** @brief util macro for impl */
#define ZFImpl_ZFLua_implDispatch_AssertParamCountRange(N0, N1) \
    do { \
        if(dispatchInfo.paramCount < N0 || dispatchInfo.paramCount > N1) \
        { \
            return dispatchInfo.dispatchError( \
                zfText("%s::%s expect %zi ~ %zi param, got %zi"), \
                dispatchInfo.classOrNamespace, dispatchInfo.methodName, \
                N0, N1, dispatchInfo.paramCount); \
        } \
    } while(zffalse)
/** @brief util macro for impl */
#define ZFImpl_ZFLua_implDispatch_AssertIsStaticMethod() \
    do { \
        if(!dispatchInfo.isStatic) \
        { \
            return dispatchInfo.dispatchError( \
                zfText("%s::%s is static method, called as object instance method"), \
                dispatchInfo.classOrNamespace, dispatchInfo.methodName); \
        } \
    } while(zffalse)
/** @brief util macro for impl */
#define ZFImpl_ZFLua_implDispatch_AssertNotStaticMethod() \
    if(dispatchInfo.isStatic) \
    { \
        return dispatchInfo.dispatchError( \
            zfText("%s::%s is object instance method, called as static method"), \
            dispatchInfo.classOrNamespace, dispatchInfo.methodName); \
    }
/** @brief util macro for impl */
#define ZFImpl_ZFLua_implDispatch_AssertClassExist() \
    do { \
        if(dispatchInfo.classOrNull == zfnull) \
        { \
            return dispatchInfo.dispatchError( \
                zfText("%s::%s no such class"), \
                dispatchInfo.classOrNamespace, dispatchInfo.methodName); \
        } \
    } while(zffalse)
/** @brief util macro for impl */
#define ZFImpl_ZFLua_implDispatch_AccessParamAllowEmpty(N, desiredClass, paramName) \
    desiredClass *paramName = ZFCastZFObject(desiredClass *, dispatchInfo.paramList[N]->toObject()); \
    zfautoObject _ZFP_##paramName; \
    do { \
        if(paramName == zfnull && dispatchInfo.paramList[N]->toObject() != zfnull) \
        { \
            ZFDI_Wrapper *t = ZFCastZFObject(ZFDI_Wrapper *, dispatchInfo.paramList[N]->toObject()); \
            if(t != zfnull && desiredClass::ClassData()->classIsTypeOf(ZFTypeIdWrapper::ClassData())) \
            { \
                zfblockedAlloc(desiredClass, t2); \
                if(t2->to<ZFTypeIdWrapper *>()->wrappedValueFromString(t->zfv)) \
                { \
                    _ZFP_##paramName = t2; \
                    paramName = t2; \
                    break; \
                } \
            } \
            return dispatchInfo.dispatchError( \
                zfText("%s::%s param type mismatch, expect %s, got %s"), \
                dispatchInfo.classOrNamespace, dispatchInfo.methodName, \
                desiredClass::ClassData()->classNameFull(), \
                ZFObjectInfo(dispatchInfo.paramList[N]->toObject()).cString()); \
        } \
    } while(zffalse)
/** @brief util macro for impl */
#define ZFImpl_ZFLua_implDispatch_AccessParam(N, desiredClass, paramName) \
    desiredClass *paramName = ZFCastZFObject(desiredClass *, dispatchInfo.paramList[N]->toObject()); \
    zfautoObject _ZFP_##paramName; \
    do { \
        if(paramName == zfnull) \
        { \
            ZFDI_Wrapper *t = ZFCastZFObject(ZFDI_Wrapper *, dispatchInfo.paramList[N]->toObject()); \
            if(t != zfnull && desiredClass::ClassData()->classIsTypeOf(ZFTypeIdWrapper::ClassData())) \
            { \
                zfblockedAlloc(desiredClass, t2); \
                if(t2->to<ZFTypeIdWrapper *>()->wrappedValueFromString(t->zfv)) \
                { \
                    _ZFP_##paramName = t2; \
                    paramName = t2; \
                    break; \
                } \
            } \
            return dispatchInfo.dispatchError( \
                zfText("%s::%s param type mismatch, expect %s, got %s"), \
                dispatchInfo.classOrNamespace, dispatchInfo.methodName, \
                desiredClass::ClassData()->classNameFull(), \
                ZFObjectInfo(dispatchInfo.paramList[N]->toObject()).cString()); \
        } \
    } while(zffalse)
/** @brief util macro for impl */
#define ZFImpl_ZFLua_implDispatch_AccessOwnerObject(OwnerObjectType, WrapperType, objName) \
    ZFImpl_ZFLua_implDispatch_AssertNotStaticMethod() \
    WrapperType *_ZFP_##objName = ZFCastZFObject(WrapperType *, dispatchInfo.objectOrNull); \
    if(_ZFP_##objName == zfnull) \
    { \
        return dispatchInfo.dispatchError( \
            zfText("%s::%s owner object type mismatch, expect %s, got %s"), \
            dispatchInfo.classOrNamespace, dispatchInfo.methodName, \
            WrapperType::ClassData()->classNameFull(), \
            ZFObjectInfo(dispatchInfo.objectOrNull).cString()); \
    } \
    OwnerObjectType objName = _ZFP_##objName->zfv
/** @brief util macro for impl */
#define ZFImpl_ZFLua_implDispatch_AccessOwnerZFObject(OwnerZFObjectType, objName) \
    ZFImpl_ZFLua_implDispatch_AssertNotStaticMethod() \
    OwnerZFObjectType *objName = ZFCastZFObject(OwnerZFObjectType *, dispatchInfo.objectOrNull); \
    do { \
        if(objName == zfnull) \
        { \
            return dispatchInfo.dispatchError( \
                zfText("%s::%s owner object type mismatch, expect %s, got %s"), \
                dispatchInfo.classOrNamespace, dispatchInfo.methodName, \
                OwnerZFObjectType::ClassData()->classNameFull(), \
                ZFObjectInfo(dispatchInfo.objectOrNull).cString()); \
        } \
    } while(zffalse)

// ============================================================
/** @brief class prefix for #ZFTypeIdWrapper impl */
#define ZFImpl_ZFLua_PropTypePrefix ZFTypeIdWrapperPrefixName
/** @brief see #ZFImpl_ZFLua_PropTypePrefix */
#define ZFImpl_ZFLua_PropTypePrefixLen ZFTypeIdWrapperPrefixNameLen

// ============================================================
/**
 * @brief run lua code, L must be first initialized by #ZFImpl_ZFLua_luaStateAttach
 */
extern ZF_ENV_EXPORT zfbool ZFImpl_ZFLua_execute(ZF_IN lua_State *L,
                                                 ZF_IN const zfchar *buf,
                                                 ZF_IN_OPT zfindex bufLen = zfindexMax(),
                                                 ZF_OUT_OPT zfautoObject *luaResult = zfnull,
                                                 ZF_IN_OPT const ZFCoreArray<zfautoObject> *luaParams = zfnull,
                                                 ZF_OUT_OPT zfstring *errHint = zfnull,
                                                 ZF_IN_OPT const zfchar *chunkInfo = zfnull);

// ============================================================
// utils
/** @brief see #ZFImpl_ZFLua_luaObjectInfo */
extern ZF_ENV_EXPORT void ZFImpl_ZFLua_luaObjectInfoT(ZF_OUT zfstring &ret,
                                                      ZF_IN lua_State *L,
                                                      ZF_IN int luaStackOffset,
                                                      ZF_IN_OPT zfbool printLuaType = zffalse);
/**
 * @brief get raw lua object info
 */
inline zfstring ZFImpl_ZFLua_luaObjectInfo(ZF_IN lua_State *L,
                                           ZF_IN int luaStackOffset,
                                           ZF_IN_OPT zfbool printLuaType = zffalse)
{
    zfstring ret;
    ZFImpl_ZFLua_luaObjectInfoT(ret, L, luaStackOffset, printLuaType);
    return ret;
}

/**
 * @brief get params from lua
 */
extern ZF_ENV_EXPORT zfbool ZFImpl_ZFLua_toObject(ZF_OUT zfautoObject &param,
                                                  ZF_IN lua_State *L,
                                                  ZF_IN int luaStackOffset);

/**
 * @brief get params from lua
 *
 * supports these types:
 * -  zfautoObject
 * -  any types that can be converted by #ZFImpl_ZFLua_toCallback
 * -  any type that can be converted to string by #ZFImpl_ZFLua_toString,
 *   result would be stored as #ZFDI_Wrapper,
 *   and would be converted to #ZFTypeIdWrapper during function call
 */
extern ZF_ENV_EXPORT zfbool ZFImpl_ZFLua_toGeneric(ZF_OUT zfautoObject &param,
                                                   ZF_IN lua_State *L,
                                                   ZF_IN int luaStackOffset);

/**
 * @brief get params from lua
 *
 * supports these types:
 * -  #v_ZFCallback
 * -  lua function, converted by ZFCallbackForLua (available in lua code only)
 */
extern ZF_ENV_EXPORT zfbool ZFImpl_ZFLua_toCallback(ZF_OUT zfautoObject &param,
                                                    ZF_IN lua_State *L,
                                                    ZF_IN int luaStackOffset);

/**
 * @brief get params from lua
 *
 * support types:
 * -  normal lua string
 * -  #ZFString
 * -  #v_zfstring
 */
extern ZF_ENV_EXPORT zfbool ZFImpl_ZFLua_toString(ZF_IN_OUT zfstring &s,
                                                  ZF_IN lua_State *L,
                                                  ZF_IN int luaStackOffset,
                                                  ZF_IN_OPT zfbool allowEmpty = zffalse,
                                                  ZF_OUT_OPT const ZFClass **holderCls = zfnull);
/** @brief see #ZFImpl_ZFLua_toString */
extern ZF_ENV_EXPORT zfbool ZFImpl_ZFLua_toString(ZF_IN_OUT zfstring &s,
                                                  ZF_IN ZFObject *obj,
                                                  ZF_IN_OPT zfbool allowEmpty = zffalse,
                                                  ZF_OUT_OPT const ZFClass **holderCls = zfnull);

/** @brief see #ZFImpl_ZFLua_toNumber */
extern ZF_ENV_EXPORT zfbool ZFImpl_ZFLua_toNumberT(ZF_OUT zfautoObject &ret,
                                                   ZF_IN lua_State *L,
                                                   ZF_IN int luaStackOffset,
                                                   ZF_IN_OPT zfbool allowEmpty = zffalse,
                                                   ZF_OUT_OPT const ZFClass **holderCls = zfnull);
/**
 * @brief get params from lua
 *
 * support types:
 * -  normal lua number
 * -  #ZFValue
 * -  #v_zfbool
 * -  #v_zfindex
 * -  #v_zfint
 * -  #v_zfuint
 * -  #v_zffloat
 * -  #v_zfdouble
 * -  #v_zflongdouble
 * -  #v_zfbyte
 * -  #v_zftimet
 * -  #v_zfflags
 * -  #v_zfidentity
 * -  all #ZFEnum types
 *
 * return proper #ZFValue if success, or empty if fail\n
 * if allowEmpty, a #ZFValue::intValueCreate would be returned
 */
inline zfautoObject ZFImpl_ZFLua_toNumber(ZF_IN lua_State *L,
                                          ZF_IN int luaStackOffset,
                                          ZF_IN_OPT zfbool allowEmpty = zffalse,
                                          ZF_OUT_OPT const ZFClass **holderCls = zfnull)
{
    zfautoObject ret;
    ZFImpl_ZFLua_toNumberT(ret, L, luaStackOffset, allowEmpty, holderCls);
    return ret;
}

/** @brief see #ZFImpl_ZFLua_toNumber */
extern ZF_ENV_EXPORT zfbool ZFImpl_ZFLua_toNumberT(ZF_OUT zfautoObject &ret,
                                                   ZF_IN ZFObject *obj,
                                                   ZF_IN_OPT zfbool allowEmpty = zffalse,
                                                   ZF_OUT_OPT const ZFClass **holderCls = zfnull);

/**
 * @brief convert native type to lua type
 *
 * support all types that can be converted by #ZFImpl_ZFLua_toString
 * and #ZFImpl_ZFLua_toNumber\n
 * if success, push result value to top of lua's stack and return true,
 * otherwise, push nothing and return false\n
 * if allowEmpty, push 0 number if obj is null
 */
extern ZF_ENV_EXPORT zfbool ZFImpl_ZFLua_toLuaValue(ZF_IN lua_State *L,
                                                    ZF_IN ZFObject *obj,
                                                    ZF_IN_OPT zfbool allowEmpty = zffalse);

/**
 * @brief append with va_arg, see #ZFLuaExecute
 */
extern ZF_ENV_EXPORT zfbool ZFImpl_ZFLua_zfstringAppend(ZF_IN lua_State *L,
                                                        ZF_IN_OUT zfstring &s,
                                                        ZF_IN_OPT int luaParamOffset = 1);

// ============================================================
// wrapper for impl
/** @brief util for impl */
inline lua_State *ZFImpl_ZFLua_luaOpen(void)
{
    return ELuna::openLua();
}
/** @brief util for impl */
inline void ZFImpl_ZFLua_luaClose(ZF_IN lua_State *L)
{
    ELuna::closeLua(L);
}

/** @brief util for impl */
template<typename T>
inline void ZFImpl_ZFLua_luaClassRegister(ZF_IN lua_State *L, ZF_IN const zfchar *name)
{
    ELuna::registerClass<T>(L, zfsCoreZ2A(name), ELuna::constructor<zfautoObject>);
}
/** @brief util for impl */
template<typename F>
inline void ZFImpl_ZFLua_luaFunctionRegister(ZF_IN lua_State *L, ZF_IN const zfchar *name, ZF_IN F f)
{
    ELuna::registerFunction(L, zfsCoreZ2A(name), f);
}
/** @brief util for impl */
inline void ZFImpl_ZFLua_luaCFunctionRegister(ZF_IN lua_State *L, ZF_IN const zfchar *name, ZF_IN int (*f)(lua_State *))
{
    lua_register(L, zfsCoreZ2A(name), f);
}

/** @brief util for impl */
inline void ZFImpl_ZFLua_luaPush(ZF_IN lua_State *L, ZF_IN zfautoObject &v)
{
    ELuna::convert2LuaType<zfautoObject>::convertType(L, v);
    ZFImpl_ZFLua_implSetupObject(L);
}
/** @brief util for impl */
inline void ZFImpl_ZFLua_luaPush(ZF_IN lua_State *L, ZF_IN const zfautoObject &v)
{
    zfautoObject t = v;
    ZFImpl_ZFLua_luaPush(L, t);
}
/** @brief util for impl */
inline const zfautoObject &ZFImpl_ZFLua_luaGet(ZF_IN lua_State *L, ZF_IN int luaStackOffset)
{
    return ELuna::convert2CppType<zfautoObject const &>::convertType(L, luaStackOffset);
}
/** @cond ZFPrivateDoc */
#define ZFImpl_ZFLua_dummyError "ZFImpl_ZFLua_dummyError"
/** @endcond */
/** @brief util for impl */
extern ZF_ENV_EXPORT int ZFImpl_ZFLua_luaError(ZF_IN lua_State *L);

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFImpl_ZFLua_h_

