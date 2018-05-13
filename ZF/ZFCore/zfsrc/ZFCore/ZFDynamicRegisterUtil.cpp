/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFDynamicRegisterUtil.h"

ZF_NAMESPACE_GLOBAL_BEGIN
/* ZFMETHOD_MAX_PARAM */

// ============================================================
ZFOBJECT_REGISTER(ZFDynamicMethodData)

ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFDynamicMethodData, const ZFMethod *, invokerMethod)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFDynamicMethodData, ZFObject *, invokerObject)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFDynamicMethodData, zfstring, errorHint)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFDynamicMethodData, zfautoObject, ret)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFDynamicMethodData, ZFObject *, param0)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFDynamicMethodData, ZFObject *, param1)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFDynamicMethodData, ZFObject *, param2)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFDynamicMethodData, ZFObject *, param3)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFDynamicMethodData, ZFObject *, param4)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFDynamicMethodData, ZFObject *, param5)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFDynamicMethodData, ZFObject *, param6)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFDynamicMethodData, ZFObject *, param7)

// ============================================================
ZFOBJECT_REGISTER(ZFDynamicPropertyData)

ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFDynamicPropertyData, zfautoObject, ret)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFDynamicPropertyData, const ZFProperty *, property)

// ============================================================
zfclassNotPOD _ZFP_ZFDynamicPrivate
{
public:
    zfuint refCount;
    zfbool errorOccurred;
    const ZFClass *cls;
    zfstring methodNamespace;
    ZFCoreArrayPOD<const ZFClass *> allClass;
    ZFCoreArrayPOD<const ZFMethod *> allMethod;
    ZFCoreArrayPOD<const ZFProperty *> allProperty;
    ZFCoreArray<ZFOutputCallback> errorCallbackList;
public:
    _ZFP_ZFDynamicPrivate(void)
    : refCount(1)
    , errorOccurred(zffalse)
    , cls(zfnull)
    , methodNamespace()
    , allClass()
    , allMethod()
    , allProperty()
    {
    }
public:
    void error(ZF_IN const zfchar *errorHint, ...)
    {
        this->errorOccurred = zftrue;
        if(!this->errorCallbackList.isEmpty())
        {
            zfstring s;
            va_list vaList;
            va_start(vaList, errorHint);
            zfstringAppendV(s, errorHint, vaList);
            va_end(vaList);
            s += zfText("\n");

            for(zfindex i = 0; i < this->errorCallbackList.count(); ++i)
            {
                this->errorCallbackList[i].execute(s.cString());
            }
        }
    }
public:
    zfbool scopeBeginCheck(void)
    {
        if(this->cls != zfnull)
        {
            this->error(zfText("[ZFDynamic] have you forgot classEnd?"));
            return zffalse;
        }
        else if(!this->methodNamespace.isEmpty())
        {
            this->error(zfText("[ZFDynamic] have you forgot NSEnd?"));
            return zffalse;
        }
        else
        {
            return zftrue;
        }
    }
};

// ============================================================
/** @cond ZFPrivateDoc */
ZFDynamic::ZFDynamic(void)
: d(zfnew(_ZFP_ZFDynamicPrivate))
{
}
ZFDynamic::ZFDynamic(ZF_IN const ZFDynamic &ref)
: d(ref.d)
{
    ++(d->refCount);
}
ZFDynamic &ZFDynamic::operator = (ZF_IN const ZFDynamic &ref)
{
    ++(ref.d->refCount);
    --(d->refCount);
    if(d->refCount == 0)
    {
        zfdelete(d);
    }
    d = ref.d;
    return *this;
}
zfbool ZFDynamic::operator == (ZF_IN const ZFDynamic &ref) const
{
    return (d == ref.d);
}
/** @endcond */

void ZFDynamic::removeAll(void)
{
    ZFCoreArrayPOD<const ZFClass *> allClass = d->allClass;
    d->allClass = ZFCoreArrayPOD<const ZFClass *>();
    ZFCoreArrayPOD<const ZFMethod *> allMethod = d->allMethod;
    d->allMethod = ZFCoreArrayPOD<const ZFMethod *>();
    ZFCoreArrayPOD<const ZFProperty *> allProperty = d->allProperty;
    d->allProperty = ZFCoreArrayPOD<const ZFProperty *>();

    for(zfindex i = 0; i < allMethod.count(); ++i)
    {
        ZFMethodDynamicUnregister(allMethod[i]);
    }
    for(zfindex i = 0; i < allProperty.count(); ++i)
    {
        ZFPropertyDynamicUnregister(allProperty[i]);
    }
    for(zfindex i = 0; i < allClass.count(); ++i)
    {
        ZFClassDynamicUnregister(allClass[i]);
    }
}
const ZFCoreArrayPOD<const ZFClass *> &ZFDynamic::allClass(void) const
{
    return d->allClass;
}
const ZFCoreArrayPOD<const ZFMethod *> &ZFDynamic::allMethod(void) const
{
    return d->allMethod;
}
const ZFCoreArrayPOD<const ZFProperty *> &ZFDynamic::allProperty(void) const
{
    return d->allProperty;
}

ZFDynamic &ZFDynamic::classBegin(ZF_IN const zfchar *className,
                                 ZF_IN_OPT const ZFClass *parentClass /* = ZFObject::ClassData() */)
{
    if(d->errorOccurred) {return *this;}
    if(!d->scopeBeginCheck()) {return *this;}
    d->cls = ZFClass::classForName(className);
    if(d->cls == zfnull)
    {
        if(parentClass == zfnull)
        {
            d->error(zfText("[ZFDynamic] no such class: %s"), className);
        }
        else
        {
            zfstring errorHint;
            const ZFClass *dynClass = ZFClassDynamicRegister(className, parentClass, &errorHint);
            if(dynClass == zfnull)
            {
                d->error(zfText("[ZFDynamic] unable to register class: %s, reason: %s"),
                    className,
                    errorHint.cString());
            }
            else
            {
                d->allClass.add(dynClass);
                d->cls = dynClass;
            }
        }
    }
    return *this;
}
ZFDynamic &ZFDynamic::classBegin(ZF_IN const zfchar *className,
                                 ZF_IN const zfchar *parentClassName)
{
    if(d->errorOccurred) {return *this;}
    if(!d->scopeBeginCheck()) {return *this;}
    if(zfsIsEmpty(parentClassName))
    {
        return this->classBegin(className, ZFObject::ClassData());
    }
    else
    {
        const ZFClass *parentClass = ZFClass::classForName(parentClassName);
        if(parentClass == zfnull)
        {
            d->error(zfText("[ZFDynamic] no such parentClass: %s"), parentClassName);
            return *this;
        }
        else
        {
            return this->classBegin(className, parentClass);
        }
    }
}
ZFDynamic &ZFDynamic::classBegin(ZF_IN const ZFClass *cls)
{
    if(d->errorOccurred) {return *this;}
    if(!d->scopeBeginCheck()) {return *this;}
    d->cls = cls;
    if(d->cls == zfnull)
    {
        d->error(zfText("[ZFDynamic] null class"));
    }
    return *this;
}
ZFDynamic &ZFDynamic::classEnd(void)
{
    if(d->errorOccurred) {return *this;}
    if(d->cls == zfnull)
    {
        d->error(zfText("[ZFDynamic] no paired classBegin"));
    }
    d->cls = zfnull;
    return *this;
}

ZFDynamic &ZFDynamic::NSBegin(ZF_IN_OPT const zfchar *methodNamespace /* = ZFMethodFuncNamespaceGlobal */)
{
    if(d->errorOccurred) {return *this;}
    if(!d->scopeBeginCheck()) {return *this;}
    if(zfsIsEmpty(methodNamespace))
    {
        d->methodNamespace = ZFMethodFuncNamespaceGlobal;
    }
    else
    {
        d->methodNamespace = methodNamespace;
    }
    return *this;
}
ZFDynamic &ZFDynamic::NSEnd(void)
{
    if(d->errorOccurred) {return *this;}
    if(d->methodNamespace.isEmpty())
    {
        d->error(zfText("[ZFDynamic] no paired NSBegin"));
    }
    d->methodNamespace.removeAll();
    return *this;
}

zfclass _ZFP_I_ZFDynamicMethodTask : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_I_ZFDynamicMethodTask, ZFObject)
public:
    ZFListener methodCallback;
    zfautoObject methodCallbackUserData;
};
static zfbool _ZFP_ZFDynamicMethodGI(ZFMETHOD_GENERIC_INVOKER_PARAMS)
{
    zfblockedAlloc(ZFDynamicMethodData, d);
    d->invokerMethod = invokerMethod;
    d->invokerObject = invokerObject;
    d->param0 = param0;
    d->param1 = param1;
    d->param2 = param2;
    d->param3 = param3;
    d->param4 = param4;
    d->param5 = param5;
    d->param6 = param6;
    d->param7 = param7;

    _ZFP_I_ZFDynamicMethodTask *task = ZFCastZFObjectUnchecked(_ZFP_I_ZFDynamicMethodTask *, invokerMethod->methodDynamicRegisterUserData());
    task->methodCallback.execute(ZFListenerData().param0Set(d), task->methodCallbackUserData);
    ret = d->ret;
    if(errorHint != zfnull)
    {
        *errorHint += d->errorHint;
    }
    return d->invokeSuccess;
}
ZFDynamic &ZFDynamic::method(ZF_IN const ZFListener &methodCallback
                             , ZF_IN ZFObject *methodCallbackUserData
                             , ZF_IN const zfchar *methodReturnTypeId
                             , ZF_IN const zfchar *methodName
                             , ZF_IN_OPT const zfchar *methodParamTypeId0 /* = zfnull */
                             , ZF_IN_OPT const zfchar *methodParamTypeId1 /* = zfnull */
                             , ZF_IN_OPT const zfchar *methodParamTypeId2 /* = zfnull */
                             , ZF_IN_OPT const zfchar *methodParamTypeId3 /* = zfnull */
                             , ZF_IN_OPT const zfchar *methodParamTypeId4 /* = zfnull */
                             , ZF_IN_OPT const zfchar *methodParamTypeId5 /* = zfnull */
                             , ZF_IN_OPT const zfchar *methodParamTypeId6 /* = zfnull */
                             , ZF_IN_OPT const zfchar *methodParamTypeId7 /* = zfnull */
                             )
{
    if(d->errorOccurred) {return *this;}
    if(!methodCallback.callbackIsValid())
    {
        d->error(zfText("invalid methodCallback"));
        return *this;
    }

    zfblockedAlloc(_ZFP_I_ZFDynamicMethodTask, task);
    task->methodCallback = methodCallback;
    task->methodCallbackUserData = methodCallbackUserData;
    return this->method(_ZFP_ZFDynamicMethodGI, task, methodReturnTypeId, methodName
            , methodParamTypeId0
            , methodParamTypeId1
            , methodParamTypeId2
            , methodParamTypeId3
            , methodParamTypeId4
            , methodParamTypeId5
            , methodParamTypeId6
            , methodParamTypeId7
        );
}
ZFDynamic &ZFDynamic::method(ZF_IN ZFMethodGenericInvoker methodGenericInvoker
                             , ZF_IN ZFObject *methodDynamicRegisterUserData
                             , ZF_IN const zfchar *methodReturnTypeId
                             , ZF_IN const zfchar *methodName
                             , ZF_IN_OPT const zfchar *methodParamTypeId0 /* = zfnull */
                             , ZF_IN_OPT const zfchar *methodParamTypeId1 /* = zfnull */
                             , ZF_IN_OPT const zfchar *methodParamTypeId2 /* = zfnull */
                             , ZF_IN_OPT const zfchar *methodParamTypeId3 /* = zfnull */
                             , ZF_IN_OPT const zfchar *methodParamTypeId4 /* = zfnull */
                             , ZF_IN_OPT const zfchar *methodParamTypeId5 /* = zfnull */
                             , ZF_IN_OPT const zfchar *methodParamTypeId6 /* = zfnull */
                             , ZF_IN_OPT const zfchar *methodParamTypeId7 /* = zfnull */
                             )
{
    if(d->errorOccurred) {return *this;}
    return this->method(ZFMethodDynamicRegisterParam()
            .methodDynamicRegisterUserDataSet(methodDynamicRegisterUserData)
            .methodOwnerClassSet(d->cls)
            .methodNamespaceSet(d->methodNamespace)
            .methodGenericInvokerSet(methodGenericInvoker)
            .methodNameSet(methodName)
            .methodReturnTypeIdSet(methodReturnTypeId)
            .methodParamAdd(methodParamTypeId0)
            .methodParamAdd(methodParamTypeId1)
            .methodParamAdd(methodParamTypeId2)
            .methodParamAdd(methodParamTypeId3)
            .methodParamAdd(methodParamTypeId4)
            .methodParamAdd(methodParamTypeId5)
            .methodParamAdd(methodParamTypeId6)
            .methodParamAdd(methodParamTypeId7)
        );
}
ZFDynamic &ZFDynamic::method(ZF_IN const ZFMethodDynamicRegisterParam &param)
{
    if(d->errorOccurred) {return *this;}
    zfstring errorHint;
    const ZFMethod *dynMethod = ZFMethodDynamicRegister(param, &errorHint);
    if(dynMethod == zfnull)
    {
        d->error(zfText("[ZFDynamic] unable to register method, reason: %s"), errorHint.cString());
    }
    else
    {
        d->allMethod.add(dynMethod);
    }
    return *this;
}

zfclass _ZFP_I_ZFDynamicPropertyTask : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_I_ZFDynamicPropertyTask, ZFObject)
public:
    ZFListener initValueCallback;
    ZFObject *initValueCallbackUserData;
};
static zfautoObject _ZFP_ZFDynamicPropertyInit(ZF_IN const ZFProperty *property)
{
    _ZFP_I_ZFDynamicPropertyTask *task = ZFCastZFObjectUnchecked(_ZFP_I_ZFDynamicPropertyTask *, property->propertyDynamicRegisterUserData());
    zfblockedAlloc(ZFDynamicPropertyData, d);
    d->property = property;
    task->initValueCallback.execute(ZFListenerData().param0Set(d), task->initValueCallbackUserData);
    return d->ret;
}
static void _ZFP_ZFDynamicPropertyInitSetup(ZF_IN_OUT ZFPropertyDynamicRegisterParam &param,
                                            ZF_IN const ZFListener &initValueCallback,
                                            ZF_IN ZFObject *initValueCallbackUserData)
{
    if(initValueCallback.callbackIsValid())
    {
        zfblockedAlloc(_ZFP_I_ZFDynamicPropertyTask, task);
        task->initValueCallback = initValueCallback;
        task->initValueCallbackUserData = initValueCallbackUserData;
        param.propertyInitValueCallbackSet(_ZFP_ZFDynamicPropertyInit);
        param.propertyDynamicRegisterUserDataSet(task);
    }
}
ZFDynamic &ZFDynamic::property(ZF_IN const zfchar *propertyTypeId,
                               ZF_IN const zfchar *propertyName,
                               ZF_IN_OPT const ZFListener &initValueCallback /* = ZFCallbackNull() */,
                               ZF_IN_OPT ZFObject *initValueCallbackUserData /* = zfnull */)
{
    if(d->errorOccurred) {return *this;}
    ZFPropertyDynamicRegisterParam param;
    param.propertyOwnerClassSet(d->cls);
    param.propertyTypeIdSet(propertyTypeId);
    param.propertyNameSet(propertyName);
    _ZFP_ZFDynamicPropertyInitSetup(param, initValueCallback, initValueCallbackUserData);
    return this->property(param);
}
ZFDynamic &ZFDynamic::property(ZF_IN const ZFClass *propertyClassOfRetainProperty,
                               ZF_IN const zfchar *propertyName,
                               ZF_IN_OPT const ZFListener &initValueCallback /* = ZFCallbackNull() */,
                               ZF_IN_OPT ZFObject *initValueCallbackUserData /* = zfnull */)
{
    if(d->errorOccurred) {return *this;}
    if(propertyClassOfRetainProperty == zfnull)
    {
        d->error(zfText("[ZFDynamic] propertyClassOfRetainProperty not set"));
        return *this;
    }
    ZFPropertyDynamicRegisterParam param;
    param.propertyOwnerClassSet(d->cls);
    param.propertyTypeIdSet(ZFPropertyTypeId_ZFObject());
    param.propertyNameSet(propertyName);
    param.propertyClassOfRetainPropertySet(propertyClassOfRetainProperty);
    _ZFP_ZFDynamicPropertyInitSetup(param, initValueCallback, initValueCallbackUserData);
    return this->property(param);
}
ZFDynamic &ZFDynamic::property(ZF_IN const ZFPropertyDynamicRegisterParam &param)
{
    if(d->errorOccurred) {return *this;}
    if(d->cls == zfnull)
    {
        d->error(zfText("[ZFDynamic] have you forgot classBegin?"));
        return *this;
    }
    zfstring errorHint;
    const ZFProperty *dynProperty = ZFPropertyDynamicRegister(param, &errorHint);
    if(dynProperty == zfnull)
    {
        d->error(zfText("[ZFDynamic] unable to register property, reason: %s"), errorHint.cString());
    }
    else
    {
        d->allProperty.add(dynProperty);
    }
    return *this;
}

ZFDynamic &ZFDynamic::errorCallbackAdd(ZF_IN const ZFOutputCallback &errorCallback /* = ZFOutputCallbackDefault() */)
{
    if(errorCallback.callbackIsValid())
    {
        d->errorCallbackList.add(errorCallback);
    }
    return *this;
}
ZFDynamic &ZFDynamic::errorCallbackRemove(ZF_IN const ZFOutputCallback &errorCallback)
{
    d->errorCallbackList.removeElement(errorCallback);
    return *this;
}
zfindex ZFDynamic::errorCallbackCount(void) const
{
    return d->errorCallbackList.count();
}
const ZFOutputCallback &ZFDynamic::errorCallbackAtIndex(ZF_IN zfindex index) const
{
    return d->errorCallbackList[index];
}
void ZFDynamic::errorCallbackNotify(ZF_IN const zfchar *errorHint) const
{
    d->error(zfText("%s"), errorHint);
}

// ============================================================
ZFPROPERTY_TYPE_ACCESS_ONLY_DEFINE(ZFDynamic, ZFDynamic)

ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFDynamic, void, removeAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFDynamic, const ZFCoreArrayPOD<const ZFClass *> &, allClass)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFDynamic, const ZFCoreArrayPOD<const ZFMethod *> &, allMethod)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFDynamic, const ZFCoreArrayPOD<const ZFProperty *> &, allProperty)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFDynamic, ZFDynamic &, classBegin, ZFMP_IN(const zfchar *, className), ZFMP_IN_OPT(const ZFClass *, parentClass, ZFObject::ClassData()))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFDynamic, ZFDynamic &, classBegin, ZFMP_IN(const zfchar *, className), ZFMP_IN(const zfchar *, parentClassName))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFDynamic, ZFDynamic &, classBegin, ZFMP_IN(const ZFClass *, cls))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFDynamic, ZFDynamic &, classEnd)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFDynamic, ZFDynamic &, NSBegin, ZFMP_IN_OPT(const zfchar *, methodNamespace, ZFMethodFuncNamespaceGlobal))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFDynamic, ZFDynamic &, NSEnd)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_8(v_ZFDynamic, ZFDynamic &, method
    , ZFMP_IN(ZFMethodGenericInvoker, methodGenericInvoker)
    , ZFMP_IN(ZFObject *, methodDynamicRegisterUserData)
    , ZFMP_IN(const zfchar *, methodReturnTypeId)
    , ZFMP_IN(const zfchar *, methodName)
    , ZFMP_IN_OPT(const zfchar *, methodParamTypeId0, zfnull)
    , ZFMP_IN_OPT(const zfchar *, methodParamTypeId1, zfnull)
    , ZFMP_IN_OPT(const zfchar *, methodParamTypeId2, zfnull)
    , ZFMP_IN_OPT(const zfchar *, methodParamTypeId3, zfnull)
    /* ZFMETHOD_MAX_PARAM , ZFMP_IN_OPT(const zfchar *, methodParamTypeId4, zfnull) */
    /* ZFMETHOD_MAX_PARAM , ZFMP_IN_OPT(const zfchar *, methodParamTypeId5, zfnull) */
    /* ZFMETHOD_MAX_PARAM , ZFMP_IN_OPT(const zfchar *, methodParamTypeId6, zfnull) */
    /* ZFMETHOD_MAX_PARAM , ZFMP_IN_OPT(const zfchar *, methodParamTypeId7, zfnull) */
    )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_8(v_ZFDynamic, ZFDynamic &, method
    , ZFMP_IN(const ZFListener &, methodCallback)
    , ZFMP_IN(ZFObject *, methodCallbackUserData)
    , ZFMP_IN(const zfchar *, methodReturnTypeId)
    , ZFMP_IN(const zfchar *, methodName)
    , ZFMP_IN_OPT(const zfchar *, methodParamTypeId0, zfnull)
    , ZFMP_IN_OPT(const zfchar *, methodParamTypeId1, zfnull)
    , ZFMP_IN_OPT(const zfchar *, methodParamTypeId2, zfnull)
    , ZFMP_IN_OPT(const zfchar *, methodParamTypeId3, zfnull)
    /* ZFMETHOD_MAX_PARAM , ZFMP_IN_OPT(const zfchar *, methodParamTypeId4, zfnull) */
    /* ZFMETHOD_MAX_PARAM , ZFMP_IN_OPT(const zfchar *, methodParamTypeId5, zfnull) */
    /* ZFMETHOD_MAX_PARAM , ZFMP_IN_OPT(const zfchar *, methodParamTypeId6, zfnull) */
    /* ZFMETHOD_MAX_PARAM , ZFMP_IN_OPT(const zfchar *, methodParamTypeId7, zfnull) */
    )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFDynamic, ZFDynamic &, method, ZFMP_IN(const ZFMethodDynamicRegisterParam &, param))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_4(v_ZFDynamic, ZFDynamic &, property, ZFMP_IN(const zfchar *, propertyTypeId), ZFMP_IN(const zfchar *, propertyName), ZFMP_IN_OPT(const ZFListener &, initValueCallback, ZFCallbackNull()), ZFMP_IN_OPT(ZFObject *, initValueCallbackUserData, zfnull))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_4(v_ZFDynamic, ZFDynamic &, property, ZFMP_IN(const ZFClass *, propertyClassOfRetainProperty), ZFMP_IN(const zfchar *, propertyName), ZFMP_IN_OPT(const ZFListener &, initValueCallback, ZFCallbackNull()), ZFMP_IN_OPT(ZFObject *, initValueCallbackUserData, zfnull))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFDynamic, ZFDynamic &, property, ZFMP_IN(const ZFPropertyDynamicRegisterParam &, param))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFDynamic, ZFDynamic &, errorCallbackAdd, ZFMP_IN_OPT(const ZFOutputCallback &, errorCallback, ZFOutputCallbackDefault()))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFDynamic, ZFDynamic &, errorCallbackRemove, ZFMP_IN(const ZFOutputCallback &, errorCallback))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFDynamic, zfindex, errorCallbackCount)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFDynamic, const ZFOutputCallback &, errorCallbackAtIndex, ZFMP_IN(zfindex, index))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFDynamic, void, errorCallbackNotify, ZFMP_IN(const zfchar *, errorHint))

ZF_NAMESPACE_GLOBAL_END

