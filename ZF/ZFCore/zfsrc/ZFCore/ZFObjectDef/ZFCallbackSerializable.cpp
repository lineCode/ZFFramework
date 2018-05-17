/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFCallbackSerializable.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// serialize routine
ZFTYPEID_DEFINE_BY_SERIALIZABLE_CONVERTER(ZFCallback, ZFCallback, {
        { // custom serialize logic
            const zfchar *customType = ZFSerializableUtil::checkAttribute(serializableData, ZFSerializableKeyword_ZFCallback_callbackType);
            if(customType != zfnull)
            {
                const ZFSerializableData *customData = ZFSerializableUtil::checkElementByCategory(serializableData, ZFSerializableKeyword_ZFCallback_callbackData);
                if(customData == zfnull)
                {
                    ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
                        zfText("missing %s"), ZFSerializableKeyword_ZFCallback_callbackData);
                    return zffalse;
                }

                _ZFP_ZFCallbackSerializeCustomCallback serializeCallback = _ZFP_ZFCallbackSerializeCustomTypeGet(customType);
                if(serializeCallback == zfnull)
                {
                    ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
                        zfText("no such callback custom serialize type: %s"), customType);
                    return zffalse;
                }
                if(!serializeCallback(v, *customData, outErrorHint, outErrorPos))
                {
                    return zffalse;
                }
                v.callbackSerializeCustomTypeSet(customType);
                v.callbackSerializeCustomDataSet(customData);

                serializableData.resolveMark();
                return zftrue;
            }
        }

        if(zfscmpTheSame(serializableData.itemClass(), ZFSerializableKeyword_null))
        {
            v = ZFCallbackNull();
            serializableData.resolveMark();
            return zftrue;
        }
        if(ZFSerializableUtil::requireSerializableClass(ZFTypeId_ZFCallback(), serializableData, outErrorHint, outErrorPos) == zfnull)
        {
            return zffalse;
        }

        const ZFSerializableData *methodData = ZFSerializableUtil::requireElementByCategory(serializableData, ZFSerializableKeyword_ZFCallback_method, outErrorHint, outErrorPos);
        if(methodData == zfnull)
        {
            return zffalse;
        }
        const ZFMethod *method = zfnull;
        if(!ZFMethodFromData(method, *methodData, outErrorHint, outErrorPos))
        {
            return zffalse;
        }

        if(method->methodOwnerClass() == zfnull)
        {
            v = ZFCallbackForMethod(method);
        }
        else if(method->methodType() == ZFMethodTypeStatic)
        {
            v = ZFCallbackForMethod(method);
        }
        else
        {
            ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
                zfText("member method callback is not supported"));
            return zffalse;
        }

        serializableData.resolveMark();
        return zftrue;
    }, {
        if(v.callbackSerializeCustomType() != zfnull)
        {
            if(zfscmpTheSame(v.callbackSerializeCustomType(), ZFCallbackSerializeCustomTypeDisable))
            {
                ZFSerializableUtil::errorOccurred(outErrorHint, zfText("callback was marked as not serializable"));
                return zffalse;
            }

            if(v.callbackSerializeCustomData() == zfnull)
            {
                ZFSerializableUtil::errorOccurred(outErrorHint, zfText("missing callback serialize custom data"));
                return zffalse;
            }
            serializableData.itemClassSet(ZFTypeId_ZFCallback());

            serializableData.attributeSet(ZFSerializableKeyword_ZFCallback_callbackType, v.callbackSerializeCustomType());

            ZFSerializableData customData = v.callbackSerializeCustomData()->copy();
            customData.categorySet(ZFSerializableKeyword_ZFCallback_callbackData);
            serializableData.elementAdd(customData);

            return zftrue;
        }

        switch(v.callbackType())
        {
            case ZFCallbackTypeDummy:
                serializableData.itemClassSet(ZFSerializableKeyword_null);
                break;
            case ZFCallbackTypeMethod:
            {
                serializableData.itemClassSet(ZFTypeId_ZFCallback());
                ZFSerializableData methodData;
                if(!ZFMethodToData(methodData, v.callbackMethod(), outErrorHint))
                {
                    return zffalse;
                }
                methodData.categorySet(ZFSerializableKeyword_ZFCallback_method);
                serializableData.elementAdd(methodData);
            }
                break;
            case ZFCallbackTypeMemberMethod:
            {
                ZFSerializableUtil::errorOccurred(outErrorHint,
                    zfText("member method callback is not supported"));
                return zffalse;
            }
                break;
            case ZFCallbackTypeRawFunction:
                serializableData.itemClassSet(ZFTypeId_ZFCallback());
                ZFSerializableUtil::errorOccurred(outErrorHint,
                    zfText("raw function is not supported"));
                return zffalse;
            default:
                zfCoreCriticalShouldNotGoHere();
                return zffalse;
        }

        return zftrue;
    })

// ============================================================
ZFTYPEID_ALIAS_DEFINE(ZFCallback, ZFCallback, ZFListener, ZFListener)
ZFTYPEID_ALIAS_DEFINE(ZFCallback, ZFCallback, ZFIOCallback, ZFIOCallback)
ZFTYPEID_ALIAS_DEFINE(ZFCallback, ZFCallback, ZFOutput, ZFOutput)
ZFTYPEID_ALIAS_DEFINE(ZFCallback, ZFCallback, ZFInput, ZFInput)

// ============================================================
// custom serialize logic
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFCallbackSerializeCustomDataHolder, ZFLevelZFFrameworkStatic)
{
}
public:
    ZFCoreMap datas; // _ZFP_ZFCallbackSerializeCustomCallback *
ZF_GLOBAL_INITIALIZER_END(ZFCallbackSerializeCustomDataHolder)
void _ZFP_ZFCallbackSerializeCustomTypeRegister(ZF_IN const zfchar *customType,
                                                ZF_IN _ZFP_ZFCallbackSerializeCustomCallback serializeCallback)
{
    ZFCoreMap &m = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFCallbackSerializeCustomDataHolder)->datas;
    zfCoreAssertWithMessage(m.get(customType) == zfnull, zfTextA("custom callback serialize type \"%s\" already registered"), zfsCoreZ2A(customType));
    zfCoreAssert(customType != zfnull && *customType != '\0' && serializeCallback != zfnull);

    m.set(customType, ZFCorePointerForObject<_ZFP_ZFCallbackSerializeCustomCallback *>(zfnew(_ZFP_ZFCallbackSerializeCustomCallback, serializeCallback)));
}
void _ZFP_ZFCallbackSerializeCustomTypeUnregister(ZF_IN const zfchar *customType)
{
    ZF_GLOBAL_INITIALIZER_INSTANCE(ZFCallbackSerializeCustomDataHolder)->datas.remove(customType);
}
_ZFP_ZFCallbackSerializeCustomCallback _ZFP_ZFCallbackSerializeCustomTypeGet(ZF_IN const zfchar *customType)
{
    ZFCorePointerBase *value = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFCallbackSerializeCustomDataHolder)->datas.get(customType);
    if(value == zfnull)
    {
        return zfnull;
    }
    else
    {
        return *(value->pointerValueT<const _ZFP_ZFCallbackSerializeCustomCallback *>());
    }
}

ZF_NAMESPACE_GLOBAL_END

