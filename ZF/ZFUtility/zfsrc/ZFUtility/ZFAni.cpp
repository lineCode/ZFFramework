#include "ZFAni.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFMETHOD_FUNC_DEFINE_3(zfautoObjectT<ZFAnimationTimeLine *>, ZFAni,
                       ZFMP_IN(ZFObject *, target),
                       ZFMP_IN(const ZFListener &, aniImpl),
                       ZFMP_IN_OPT(ZFObject *, userData, zfnull))
{
    zfblockedAlloc(ZFAnimationTimeLine, ani);
    ZFLISTENER_LAMBDA_2(aniImplWrap
            , zfautoObject, target
            , ZFListener, aniImpl
        , {
            aniImpl.execute(ZFListenerData(zfidentityInvalid(), target, listenerData.param0()), userData);
        })
    ani->observerAdd(ZFAnimationTimeLine::EventAniTimeLineOnUpdate(), aniImplWrap, userData);
    return ani;
}

// ============================================================
static zfautoObjectT<ZFAnimationTimeLine *> _ZFP_ZFAni(ZF_IN ZFObject *target,
                                                       ZF_IN const ZFMethod *setterMethod,
                                                       ZF_IN const ZFMethod *getterMethod,
                                                       ZF_IN ZFObject *from,
                                                       ZF_IN ZFObject *to)
{
    if(setterMethod == zfnull)
    {
        if(ZFClass::classForName(getterMethod->methodReturnTypeId()) == zfnull
            || ZFCastZFObject(ZFProgressable *, getterMethod->methodGenericInvoke(target)) == zfnull)
        { // ani by getter is valid only for retain property
            return zfnull;
        }
        ZFLISTENER_LAMBDA_3(aniImpl
                , const ZFMethod *, getterMethod
                , zfautoObject, from
                , zfautoObject, to
            , {
                ZFProgressable *value = getterMethod->methodGenericInvoke(listenerData.sender());
                if(value != zfnull)
                {
                    value->progressUpdate(from, to, listenerData.param0<v_zffloat *>()->zfv);
                }
            })
        return ZFAni(target, aniImpl);
    }
    const ZFTypeInfo *typeInfo = ZFTypeInfoForName(getterMethod->methodReturnTypeId());
    if(typeInfo == zfnull)
    {
        return zfnull;
    }
    ZFLISTENER_LAMBDA_5(aniImpl
            , const ZFTypeInfo *, typeInfo
            , const ZFMethod *, setterMethod
            , const ZFMethod *, getterMethod
            , zfautoObject, from
            , zfautoObject, to
        , {
            zfautoObject valueHolder;
            if(!typeInfo->typeIdWrapper(valueHolder))
            {
                return ;
            }
            ZFProgressable *value = valueHolder;
            if(value == zfnull || !value->progressUpdate(from, to, listenerData.param0<v_zffloat *>()->zfv))
            {
                return ;
            }
            setterMethod->methodGenericInvoke(listenerData.sender(), valueHolder);
        })
    return ZFAni(target, aniImpl);
}
static zfbool _ZFP_ZFAniPrepare(ZF_OUT const ZFMethod *&setterMethod,
                                ZF_OUT const ZFMethod *&getterMethod,
                                ZF_IN ZFObject *target,
                                ZF_IN const zfchar *name)
{
    if(target == zfnull)
    {
        return zffalse;
    }
    const ZFProperty *property = target->classData()->propertyForName(name);
    if(property != zfnull)
    {
        setterMethod = property->setterMethod();
        getterMethod = property->getterMethod();
    }
    else
    {
        setterMethod = target->classData()->propertySetterForName(name);
        getterMethod = target->classData()->propertyGetterForName(name);
    }
    if(setterMethod->methodPrivilegeType() != ZFMethodPrivilegeTypePublic)
    {
        setterMethod = zfnull;
    }
    if(getterMethod->methodPrivilegeType() == ZFMethodPrivilegeTypePrivate)
    {
        getterMethod = zfnull;
    }
    return (getterMethod != zfnull);
}
ZFMETHOD_FUNC_DEFINE_4(zfautoObjectT<ZFAnimationTimeLine *>, ZFAni,
                       ZFMP_IN(ZFObject *, target),
                       ZFMP_IN(const zfchar *, name),
                       ZFMP_IN(const zfchar *, from),
                       ZFMP_IN(const zfchar *, to))
{
    const ZFMethod *setterMethod = zfnull;
    const ZFMethod *getterMethod = zfnull;
    if(!_ZFP_ZFAniPrepare(setterMethod, getterMethod, target, name))
    {
        return zfnull;
    }
    const zfchar *typeId = getterMethod->methodReturnTypeId();
    const ZFClass *typeClass = ZFClass::classForName(typeId);
    if(typeClass != zfnull)
    {
        if(!typeClass->classIsTypeOf(ZFSerializable::ClassData()))
        {
            return zfnull;
        }
        zfautoObject fromHolder = typeClass->newInstance();
        if(!fromHolder.to<ZFSerializable *>()->serializeFromString(from))
        {
            return zfnull;
        }
        zfautoObject toHolder = typeClass->newInstance();
        if(!toHolder.to<ZFSerializable *>()->serializeFromString(to))
        {
            return zfnull;
        }
        return _ZFP_ZFAni(target, setterMethod, getterMethod, fromHolder, toHolder);
    }
    const ZFTypeInfo *typeInfo = ZFTypeInfoForName(typeId);
    if(typeInfo == zfnull)
    {
        return zfnull;
    }
    zfautoObject fromHolder;
    zfautoObject toHolder;
    if(!typeInfo->typeIdWrapper(fromHolder) || !typeInfo->typeIdWrapper(toHolder))
    {
        return zfnull;
    }
    if(!fromHolder->to<ZFTypeIdWrapper *>()->wrappedValueFromString(from))
    {
        return zfnull;
    }
    if(!toHolder->to<ZFTypeIdWrapper *>()->wrappedValueFromString(to))
    {
        return zfnull;
    }
    return _ZFP_ZFAni(target, setterMethod, getterMethod, fromHolder, toHolder);
}
ZFMETHOD_FUNC_DEFINE_4(zfautoObjectT<ZFAnimationTimeLine *>, ZFAni,
                       ZFMP_IN(ZFObject *, target),
                       ZFMP_IN(const zfchar *, name),
                       ZFMP_IN(ZFObject *, from),
                       ZFMP_IN(ZFObject *, to))
{
    const ZFMethod *setterMethod = zfnull;
    const ZFMethod *getterMethod = zfnull;
    if(!_ZFP_ZFAniPrepare(setterMethod, getterMethod, target, name))
    {
        return zfnull;
    }
    return _ZFP_ZFAni(target, setterMethod, getterMethod, from, to);
}

ZF_NAMESPACE_GLOBAL_END

