/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFDynamicInvoker.h"
#include "ZFString.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFOBJECT_REGISTER(ZFDI_Wrapper)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFDI_Wrapper, zfstring, zfv)

// ============================================================
const zfchar *ZFDI_toString(ZF_IN ZFObject *obj)
{
    if(obj == zfnull)
    {
        return zfnull;
    }
    {
        v_zfstring *t = ZFCastZFObject(v_zfstring *, obj);
        if(t != zfnull)
        {
            return t->zfv;
        }
    }
    {
        ZFString *t = ZFCastZFObject(ZFString *, obj);
        if(t != zfnull)
        {
            return t->stringValue();
        }
    }
    {
        ZFDI_Wrapper *t = ZFCastZFObject(ZFDI_Wrapper *, obj);
        if(t != zfnull)
        {
            return t->zfv;
        }
    }
    return zfnull;
}

const ZFClass *ZFDI_classForName(ZF_IN const zfchar *className,
                                 ZF_IN const zfchar *NS)
{
    if(className == zfnull)
    {
        return zfnull;
    }
    const ZFClass *cls = ZFClass::classForName(className, NS);
    if(cls != zfnull)
    {
        return cls;
    }
    zfindex dotPos = zfstringFindReversely(className, zfindexMax(), ZFNamespaceSeparator(), ZFNamespaceSeparatorLen());
    if(dotPos == zfindexMax())
    {
        zfstring classNameTmp;
        classNameTmp += ZFTypeIdWrapperPrefixName;
        classNameTmp += className;
        cls = ZFClass::classForName(classNameTmp, NS);
    }
    else
    {
        zfstring classNameTmp;
        classNameTmp.append(className, dotPos + ZFNamespaceSeparatorLen());
        classNameTmp += ZFTypeIdWrapperPrefixName;
        classNameTmp += className + dotPos + ZFNamespaceSeparatorLen();
        cls = ZFClass::classForName(classNameTmp, NS);
    }
    return cls;
}

static void _ZFP_ZFDI_paramInfo(ZF_IN_OUT zfstring &ret,
                                ZF_IN ZFObject *param)
{
    if(param == zfnull)
    {
        ret += ZFTOKEN_zfnull;
    }
    else
    {
        ret += "(";
        ret += param->classData()->className();
        ret += ")";
        param->objectInfoT(ret);
    }
}
void ZFDI_paramInfo(ZF_IN_OUT zfstring &ret
                    , ZF_IN_OPT ZFObject *param0 /* = ZFMethodGenericInvokerDefaultParam() */
                    , ZF_IN_OPT ZFObject *param1 /* = ZFMethodGenericInvokerDefaultParam() */
                    , ZF_IN_OPT ZFObject *param2 /* = ZFMethodGenericInvokerDefaultParam() */
                    , ZF_IN_OPT ZFObject *param3 /* = ZFMethodGenericInvokerDefaultParam() */
                    , ZF_IN_OPT ZFObject *param4 /* = ZFMethodGenericInvokerDefaultParam() */
                    , ZF_IN_OPT ZFObject *param5 /* = ZFMethodGenericInvokerDefaultParam() */
                    , ZF_IN_OPT ZFObject *param6 /* = ZFMethodGenericInvokerDefaultParam() */
                    , ZF_IN_OPT ZFObject *param7 /* = ZFMethodGenericInvokerDefaultParam() */
                    )
{
    if(param0 == zfnull || param0 == ZFMethodGenericInvokerDefaultParam()) {return ;}
    ret += '[';
    do
    {
        _ZFP_ZFDI_paramInfo(ret, param0);

        #define _ZFP_ZFDI_paramInfo_loop(N) \
            if(param##N == zfnull || param##N == ZFMethodGenericInvokerDefaultParam()) \
            { \
                break; \
            } \
            else \
            { \
                ret += ", "; \
                _ZFP_ZFDI_paramInfo(ret, param##N); \
            }
        _ZFP_ZFDI_paramInfo_loop(1)
        _ZFP_ZFDI_paramInfo_loop(2)
        _ZFP_ZFDI_paramInfo_loop(3)
        _ZFP_ZFDI_paramInfo_loop(4)
        _ZFP_ZFDI_paramInfo_loop(5)
        _ZFP_ZFDI_paramInfo_loop(6)
        _ZFP_ZFDI_paramInfo_loop(7)
    } while(zffalse);
    ret += ']';
}

// ============================================================
zfbool ZFDI_invoke(ZF_OUT zfautoObject &ret
                   , ZF_OUT_OPT zfstring *errorHint
                   , ZF_IN_OPT ZFObject *obj
                   , ZF_IN_OPT const zfchar *NS
                   , ZF_IN ZFObject *type
                   , ZF_IN zfindex paramCount
                   , ZF_IN_OPT ZFObject *param0 /* = ZFMethodGenericInvokerDefaultParam() */
                   , ZF_IN_OPT ZFObject *param1 /* = ZFMethodGenericInvokerDefaultParam() */
                   , ZF_IN_OPT ZFObject *param2 /* = ZFMethodGenericInvokerDefaultParam() */
                   , ZF_IN_OPT ZFObject *param3 /* = ZFMethodGenericInvokerDefaultParam() */
                   , ZF_IN_OPT ZFObject *param4 /* = ZFMethodGenericInvokerDefaultParam() */
                   , ZF_IN_OPT ZFObject *param5 /* = ZFMethodGenericInvokerDefaultParam() */
                   , ZF_IN_OPT ZFObject *param6 /* = ZFMethodGenericInvokerDefaultParam() */
                   , ZF_IN_OPT ZFObject *param7 /* = ZFMethodGenericInvokerDefaultParam() */
                   /* ZFMETHOD_MAX_PARAM */)
{
    if(type == zfnull)
    {
        if(errorHint != zfnull)
        {
            zfstringAppend(errorHint, "null methodName or ClassName");
        }
        return zffalse;
    }

    if(obj == zfnull)
    {
        v_ZFClass *clsWrapper = ZFCastZFObject(v_ZFClass *, type);
        if(clsWrapper != zfnull)
        {
            if(clsWrapper->zfv == zfnull)
            {
                if(errorHint != zfnull)
                {
                    zfstringAppend(errorHint, "null class");
                }
                return zffalse;
            }
            else
            {
                return ZFDI_alloc(ret, errorHint, clsWrapper->zfv, paramCount, param0, param1, param2, param3, param4, param5, param6, param7);
            }
        }
    }

    const zfchar *methodName = ZFDI_toString(type);
    zfstring NSHolder;
    if(methodName != zfnull && ZFNamespaceSkipGlobal(NS) == zfnull)
    {
        zfindex dotPos = zfstringFindReversely(methodName, zfindexMax(), ZFNamespaceSeparator(), ZFNamespaceSeparatorLen());
        if(dotPos != zfindexMax())
        {
            NSHolder.append(methodName, dotPos);
            NS = NSHolder;
            methodName = methodName + dotPos + ZFNamespaceSeparatorLen();
        }
    }
    if(methodName != zfnull && obj == zfnull)
    {
        const ZFClass *cls = ZFDI_classForName(methodName, NS);
        if(cls != zfnull)
        {
            return ZFDI_alloc(ret, errorHint, cls, paramCount, param0, param1, param2, param3, param4, param5, param6, param7);
        }
    }

    ZFCoreArrayPOD<const ZFMethod *> methodList;
    if(methodName == zfnull)
    {
        v_ZFMethod *methodWrapper = ZFCastZFObject(v_ZFMethod *, type);
        if(methodWrapper != zfnull)
        {
            methodList.add(methodWrapper->zfv);
        }
        else
        {
            if(errorHint != zfnull)
            {
                zfstringAppend(errorHint, "unable to detect methodName from: \"%s\"",
                    ZFObjectInfo(type).cString());
            }
            return zffalse;
        }
    }
    else
    {
        if(obj != zfnull)
        {
            obj->classData()->methodForNameGetAllT(methodList, methodName);
        }
        else
        {
            ZFMethodFuncGetAllT(methodList, NS, methodName);
            if(methodList.isEmpty())
            {
                const ZFClass *cls = ZFDI_classForName(NS, zfnull);
                if(cls != zfnull)
                {
                    cls->methodForNameGetAllT(methodList, methodName);
                }
            }
        }
        if(methodList.isEmpty())
        {
            if(errorHint != zfnull)
            {
                if(obj != zfnull)
                {
                    zfstringAppend(errorHint, "no such method \"%s\" for object: %s",
                        methodName, obj->objectInfoOfInstance().cString());
                }
                else
                {
                    zfstringAppend(errorHint, "no such method \"%s\" in scope \"%s\"",
                        methodName, NS);
                }
            }
            return zffalse;
        }
    }

    // try to invoke each method
    zfstring _errorHintTmp;
    zfstring *errorHintTmp = errorHint ? &_errorHintTmp : zfnull;
    const ZFMethod *methodLast = zfnull;
    for(zfindex iMethod = 0; iMethod < methodList.count(); ++iMethod)
    {
        const ZFMethod *method = methodList[iMethod];
        if(paramCount < method->methodParamCountMin() || paramCount > method->methodParamCount())
        {
            continue;
        }
        methodLast = method;
        _errorHintTmp.removeAll();
        zfautoObject paramList[ZFMETHOD_MAX_PARAM] = {
            param0, param1, param2, param3, param4, param5, param6, param7,
        };
        zfbool paramConvertSuccess = zftrue;
        for(zfindex iParam = 0; iParam < paramCount; ++iParam)
        {
            ZFDI_Wrapper *wrapper = ZFCastZFObject(ZFDI_Wrapper *, paramList[iParam]);
            if(wrapper != zfnull)
            {
                if(!ZFDI_paramConvert(
                    paramList[iParam], method->methodParamTypeIdAtIndex(iParam), wrapper, errorHintTmp))
                {
                    if(errorHint != zfnull)
                    {
                        paramConvertSuccess = zffalse;
                        break;
                    }
                }
            }
        }
        if(!paramConvertSuccess)
        {
            continue;
        }
        if(method->methodGenericInvoker()(method, obj, errorHintTmp, ret, paramList))
        {
            return zftrue;
        }
    }
    if(errorHint != zfnull)
    {
        zfstringAppend(errorHint, "no matching method to call, last error reason: %s, for method: %s",
            errorHintTmp->cString(),
            methodLast ? methodLast->objectInfo().cString() : ZFTOKEN_zfnull);

        if(param0 != zfnull && param0 != ZFMethodGenericInvokerDefaultParam())
        {
            *errorHint += ", with params: ";
            ZFDI_paramInfo(*errorHint
                    , param0
                    , param1
                    , param2
                    , param3
                    , param4
                    , param5
                    , param6
                    , param7
                );
        }
    }
    return zffalse;
}

zfbool ZFDI_alloc(ZF_OUT zfautoObject &ret
                  , ZF_OUT_OPT zfstring *errorHint
                  , ZF_IN_OPT const zfchar *NS
                  , ZF_IN ZFObject *type
                  , ZF_IN zfindex paramCount
                  , ZF_IN_OPT ZFObject *param0 /* = ZFMethodGenericInvokerDefaultParam() */
                  , ZF_IN_OPT ZFObject *param1 /* = ZFMethodGenericInvokerDefaultParam() */
                  , ZF_IN_OPT ZFObject *param2 /* = ZFMethodGenericInvokerDefaultParam() */
                  , ZF_IN_OPT ZFObject *param3 /* = ZFMethodGenericInvokerDefaultParam() */
                  , ZF_IN_OPT ZFObject *param4 /* = ZFMethodGenericInvokerDefaultParam() */
                  , ZF_IN_OPT ZFObject *param5 /* = ZFMethodGenericInvokerDefaultParam() */
                  , ZF_IN_OPT ZFObject *param6 /* = ZFMethodGenericInvokerDefaultParam() */
                  , ZF_IN_OPT ZFObject *param7 /* = ZFMethodGenericInvokerDefaultParam() */
                  /* ZFMETHOD_MAX_PARAM */)
{
    if(type == zfnull)
    {
        if(errorHint != zfnull)
        {
            zfstringAppend(errorHint, "null class name");
        }
        return zffalse;
    }
    const ZFClass *cls = zfnull;
    v_ZFClass *clsWrapper = ZFCastZFObject(v_ZFClass *, type);
    if(clsWrapper != zfnull)
    {
        cls = clsWrapper->zfv;
        if(cls == zfnull)
        {
            if(errorHint != zfnull)
            {
                zfstringAppend(errorHint, "null class");
            }
            return zffalse;
        }
    }
    else
    {
        cls = ZFDI_classForName(ZFDI_toString(type), NS);
    }
    if(cls == zfnull)
    {
        if(errorHint != zfnull)
        {
            zfstringAppend(errorHint, "no such class \"%s\" in scope \"%s\"",
                ZFObjectInfo(type).cString(), NS);
        }
        return zffalse;
    }
    else
    {
        return ZFDI_alloc(ret, errorHint, cls, paramCount, param0, param1, param2, param3, param4, param5, param6, param7);
    }
}
zfbool ZFDI_alloc(ZF_OUT zfautoObject &ret
                  , ZF_OUT_OPT zfstring *errorHint
                  , ZF_IN const ZFClass *cls
                  , ZF_IN zfindex paramCount
                  , ZF_IN_OPT ZFObject *param0 /* = ZFMethodGenericInvokerDefaultParam() */
                  , ZF_IN_OPT ZFObject *param1 /* = ZFMethodGenericInvokerDefaultParam() */
                  , ZF_IN_OPT ZFObject *param2 /* = ZFMethodGenericInvokerDefaultParam() */
                  , ZF_IN_OPT ZFObject *param3 /* = ZFMethodGenericInvokerDefaultParam() */
                  , ZF_IN_OPT ZFObject *param4 /* = ZFMethodGenericInvokerDefaultParam() */
                  , ZF_IN_OPT ZFObject *param5 /* = ZFMethodGenericInvokerDefaultParam() */
                  , ZF_IN_OPT ZFObject *param6 /* = ZFMethodGenericInvokerDefaultParam() */
                  , ZF_IN_OPT ZFObject *param7 /* = ZFMethodGenericInvokerDefaultParam() */
                  )
{
    if(cls == zfnull)
    {
        if(errorHint != zfnull)
        {
            zfstringAppend(errorHint, "null class");
        }
        return zffalse;
    }
    if(paramCount == 0)
    {
        ret = cls->newInstance();
        if(ret == zfnull)
        {
            if(errorHint != zfnull)
            {
                zfstringAppend(errorHint, "unable to alloc class \"%s\"", cls->classNameFull());
            }
            return zffalse;
        }
        else
        {
            return zftrue;
        }
    }
    ZFCoreArrayPOD<const ZFMethod *> methodList;
    cls->methodForNameGetAllT(methodList, "objectOnInit");
    if(methodList.isEmpty())
    {
        if(errorHint != zfnull)
        {
            zfstringAppend(errorHint, "class \"%s\" has no reflectable objectOnInit", cls->classNameFull());
        }
        return zffalse;
    }

    ZFToken token = cls->newInstanceGenericBegin();
    if(token == zfnull)
    {
        if(errorHint != zfnull)
        {
            zfstringAppend(errorHint, "unable to alloc class \"%s\"", cls->classNameFull());
        }
        return zffalse;
    }

    zfstring _errorHintTmp;
    zfstring *errorHintTmp = errorHint ? &_errorHintTmp : zfnull;
    for(zfindex iMethod = 0; iMethod < methodList.count(); ++iMethod)
    {
        const ZFMethod *method = methodList[iMethod];
        if(paramCount < method->methodParamCountMin() || paramCount > method->methodParamCount())
        {
            continue;
        }
        _errorHintTmp.removeAll();
        zfautoObject paramList[ZFMETHOD_MAX_PARAM] = {
            param0, param1, param2, param3, param4, param5, param6, param7,
        };
        zfbool paramConvertSuccess = zftrue;
        for(zfindex iParam = 0; iParam < paramCount; ++iParam)
        {
            ZFDI_Wrapper *wrapper = ZFCastZFObject(ZFDI_Wrapper *, paramList[iParam]);
            if(wrapper != zfnull)
            {
                if(!ZFDI_paramConvert(
                    paramList[iParam], method->methodParamTypeIdAtIndex(iParam), wrapper, errorHintTmp))
                {
                    if(errorHint != zfnull)
                    {
                        paramConvertSuccess = zffalse;
                        break;
                    }
                }
            }
        }
        if(!paramConvertSuccess)
        {
            continue;
        }
        if(cls->newInstanceGenericCheck(token, methodList[iMethod], paramList))
        {
            ret = cls->newInstanceGenericEnd(token, zftrue);
            return zftrue;
        }
    }
    cls->newInstanceGenericEnd(token, zffalse);
    if(errorHint != zfnull)
    {
        zfstringAppend(errorHint, "no matching objectOnInit to call for class %s",
            cls->className());
        if(!errorHintTmp->isEmpty())
        {
            zfstringAppend(errorHint, ", last error reason: %s",
                errorHintTmp->cString());
        }
        if(param0 != zfnull && param0 != ZFMethodGenericInvokerDefaultParam())
        {
            *errorHint += ", with params: ";
            ZFDI_paramInfo(*errorHint
                    , param0
                    , param1
                    , param2
                    , param3
                    , param4
                    , param5
                    , param6
                    , param7
                );
        }
    }
    return zffalse;
}

zfbool ZFDI_paramConvert(ZF_OUT zfautoObject &ret,
                         ZF_IN const zfchar *typeId,
                         ZF_IN ZFDI_Wrapper *wrapper,
                         ZF_OUT_OPT zfstring *errorHint /* = zfnull */)
{
    const ZFClass *cls = ZFClass::classForName(typeId);
    if(cls != zfnull)
    {
        if(!ZFSerializeFromString(ret, cls, wrapper->zfv, wrapper->zfv.length()))
        {
            if(errorHint != zfnull)
            {
                zfstringAppend(errorHint, "%s unable to convert from string \"%s\"",
                    typeId, wrapper->zfv.cString());
            }
            return zffalse;
        }
        else
        {
            return zftrue;
        }
    }

    const ZFTypeIdBase *typeIdData = ZFTypeIdGet(typeId);
    if(typeIdData == zfnull || !typeIdData->typeIdWrapper(ret))
    {
        if(errorHint != zfnull)
        {
            zfstringAppend(errorHint, "%s can not be converted from string automatically", typeId);
        }
        return zffalse;
    }
    if(ret != zfnull)
    {
        ZFTypeIdWrapper *typeWrapper = ret;
        if(typeWrapper != zfnull && typeWrapper->wrappedValueFromString(wrapper->zfv, wrapper->zfv.length()))
        {
            return zftrue;
        }
    }
    if(errorHint != zfnull)
    {
        zfstringAppend(errorHint, "%s can not be converted from string \"%s\"",
                typeId,
                wrapper->zfv.cString()
            );
    }
    return zffalse;
}

ZF_NAMESPACE_GLOBAL_END

