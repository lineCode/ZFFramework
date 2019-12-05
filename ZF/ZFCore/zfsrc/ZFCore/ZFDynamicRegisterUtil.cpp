#include "ZFDynamicRegisterUtil.h"

#include "ZFSTLWrapper/zfstl_string.h"
#include "ZFSTLWrapper/zfstl_map.h"

ZF_NAMESPACE_GLOBAL_BEGIN
/* ZFMETHOD_MAX_PARAM */

// ============================================================
ZFOBJECT_REGISTER(ZFDynamicMethodData)

ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFDynamicMethodData, const ZFMethod *, invokerMethod)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFDynamicMethodData, ZFObject *, invokerObject)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFDynamicMethodData, zfstring, errorHint)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFDynamicMethodData, zfautoObject, ret)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFDynamicMethodData, zfautoObject, param0)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFDynamicMethodData, zfautoObject, param1)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFDynamicMethodData, zfautoObject, param2)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFDynamicMethodData, zfautoObject, param3)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFDynamicMethodData, zfautoObject, param4)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFDynamicMethodData, zfautoObject, param5)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFDynamicMethodData, zfautoObject, param6)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFDynamicMethodData, zfautoObject, param7)

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
    zfstlstringZ regTag;
    const ZFClass *cls;
    zfstring methodNamespace;
    zfstring enumClassName;
    zfuint enumDefault;
    zfbool enumIsFlags;
    zfuint enumValueNext;
    ZFCoreArrayPOD<zfuint> enumValues;
    ZFCoreArray<zfstring> enumNames;
    ZFCoreArrayPOD<const ZFClass *> allClass;
    ZFCoreArrayPOD<const ZFClass *> allEnum;
    ZFCoreArrayPOD<const ZFMethod *> allMethod;
    ZFCoreArrayPOD<const ZFProperty *> allProperty;
    ZFCoreArrayPOD<zfidentity> allEvent;
    ZFCoreArray<ZFOutput> errorCallbackList;
public:
    _ZFP_ZFDynamicPrivate(void)
    : refCount(1)
    , errorOccurred(zffalse)
    , regTag()
    , cls(zfnull)
    , methodNamespace()
    , enumClassName()
    , enumDefault(ZFEnumInvalid())
    , enumIsFlags(zffalse)
    , enumValueNext(0)
    , enumValues()
    , enumNames()
    , allClass()
    , allEnum()
    , allMethod()
    , allProperty()
    , allEvent()
    , errorCallbackList()
    {
        this->attachGlobal();
    }
    ~_ZFP_ZFDynamicPrivate(void)
    {
        this->detachGlobal();
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
            s += "\n";

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
            this->error("have you forgot classEnd?");
            return zffalse;
        }
        else if(!this->methodNamespace.isEmpty())
        {
            this->error("have you forgot NSEnd?");
            return zffalse;
        }
        else if(!this->enumClassName.isEmpty())
        {
            this->error("have you forgot enumEnd?");
            return zffalse;
        }
        else
        {
            return zftrue;
        }
    }
public:
    void attachGlobal(void);
    void detachGlobal(void);
    void removeByGlobal(void)
    {
        this->allClass.removeAll();
        this->allEnum.removeAll();
        this->allMethod.removeAll();
        this->allProperty.removeAll();
        this->allEvent.removeAll();
    }
};

// ============================================================
ZF_STATIC_INITIALIZER_INIT(ZFDynamicDataHolder)
{
}
public:
    ZFCoreArrayPOD<_ZFP_ZFDynamicPrivate *> allImpl;
    ZFCoreArrayPOD<const ZFClass *> allClass;
    ZFCoreArrayPOD<const ZFClass *> allEnum;
    ZFCoreArrayPOD<const ZFMethod *> allMethod;
    ZFCoreArrayPOD<const ZFProperty *> allProperty;
    ZFCoreArrayPOD<zfidentity> allEvent;
public:
    void removeAll(void)
    {
        if(!this->allImpl.isEmpty())
        {
            ZFCoreArrayPOD<_ZFP_ZFDynamicPrivate *> allImpl = this->allImpl;
            this->allImpl = ZFCoreArrayPOD<_ZFP_ZFDynamicPrivate *>();
            for(zfindex i = allImpl.count() - 1; i != zfindexMax(); --i)
            {
                allImpl[i]->removeByGlobal();
            }
        }

        if(!this->allEvent.isEmpty())
        {
            ZFCoreArrayPOD<zfidentity> allEvent = this->allEvent;
            this->allEvent = ZFCoreArrayPOD<zfidentity>();
            for(zfindex i = allEvent.count() - 1; i != zfindexMax(); --i)
            {
                ZFIdMapDynamicUnregister(allEvent[i]);
            }
        }

        if(!this->allMethod.isEmpty())
        {
            ZFCoreArrayPOD<const ZFMethod *> allMethod = this->allMethod;
            this->allMethod = ZFCoreArrayPOD<const ZFMethod *>();
            for(zfindex i = allMethod.count() - 1; i != zfindexMax(); --i)
            {
                ZFMethodDynamicUnregister(allMethod[i]);
            }
        }

        if(!this->allProperty.isEmpty())
        {
            ZFCoreArrayPOD<const ZFProperty *> allProperty = this->allProperty;
            this->allProperty = ZFCoreArrayPOD<const ZFProperty *>();
            for(zfindex i = allProperty.count() - 1; i != zfindexMax(); --i)
            {
                ZFPropertyDynamicUnregister(allProperty[i]);
            }
        }

        if(!this->allEnum.isEmpty())
        {
            ZFCoreArrayPOD<const ZFClass *> allEnum = this->allEnum;
            this->allEnum = ZFCoreArrayPOD<const ZFClass *>();
            for(zfindex i = allEnum.count() - 1; i != zfindexMax(); --i)
            {
                ZFEnumDynamicUnregister(allEnum[i]);
            }
        }

        if(!this->allClass.isEmpty())
        {
            ZFCoreArrayPOD<const ZFClass *> allClass = this->allClass;
            this->allClass = ZFCoreArrayPOD<const ZFClass *>();
            for(zfindex i = allClass.count() - 1; i != zfindexMax(); --i)
            {
                ZFClassDynamicUnregister(allClass[i]);
            }
        }
    }
ZF_STATIC_INITIALIZER_END(ZFDynamicDataHolder)

void _ZFP_ZFDynamicPrivate::attachGlobal(void)
{
    zfCoreMutexLocker();
    ZF_STATIC_INITIALIZER_INSTANCE(ZFDynamicDataHolder)->allImpl.add(this);
}
void _ZFP_ZFDynamicPrivate::detachGlobal(void)
{
    zfCoreMutexLocker();
    ZF_STATIC_INITIALIZER_INSTANCE(ZFDynamicDataHolder)->allImpl.removeElement(this);
}

// ============================================================
static zfstlmap<zfstlstringZ, ZFDynamic> &_ZFP_ZFDynamicRegTagMap(void)
{
    static zfstlmap<zfstlstringZ, ZFDynamic> m;
    return m;
}

// ============================================================
ZFDynamic::ZFDynamic(void)
: d(zfnew(_ZFP_ZFDynamicPrivate))
{
}
ZFDynamic::ZFDynamic(ZF_IN const zfchar *regTag)
: d(zfnew(_ZFP_ZFDynamicPrivate))
{
    this->regTag(regTag);
}
ZFDynamic::ZFDynamic(ZF_IN const ZFDynamic &ref)
: d(ref.d)
{
    ++(d->refCount);
}
ZFDynamic::~ZFDynamic(void)
{
    --(d->refCount);
    if(d->refCount == 0)
    {
        zfdelete(d);
    }
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

void ZFDynamic::exportTag(ZF_IN_OUT const ZFOutput &output)
{
    if(!output.callbackIsValid())
    {
        return ;
    }

    ZFCoreArrayPOD<const ZFClass *> allClass;
    ZFClassGetAllT(allClass);

    ZFCoreArrayPOD<const ZFMethod *> allMethod;
    ZFMethodGetAllT(allMethod);

    ZFCoreArrayPOD<const ZFTypeInfo *> allTypeId;
    ZFTypeInfoGetAllT(allTypeId);

    ZFCoreArrayPOD<const zfchar *> allNamespace;
    ZFNamespaceGetAllT(allNamespace);

    zfstlmap<zfstlstringZ, zfbool> tags;
    const zfchar *zfpFix = "_ZFP_";
    zfindex zfpFixLen = zfslen(zfpFix);

    for(zfindex i = 0; i < allClass.count(); ++i)
    {
        const ZFClass *t = allClass[i];
        if(zfsncmp(t->className(), zfpFix, zfpFixLen) != 0)
        {
            tags[t->className()] = zftrue;
        }
    }
    for(zfindex i = 0; i < allMethod.count(); ++i)
    {
        const ZFMethod *t = allMethod[i];
        if(zfsncmp(t->methodName(), zfpFix, zfpFixLen) != 0)
        {
            tags[t->methodName()] = zftrue;
        }
    }
    for(zfindex i = 0; i < allTypeId.count(); ++i)
    {
        const ZFTypeInfo *t = allTypeId[i];
        if(!zfsIsEmpty(t->typeId())
            && zfsncmp(t->typeId(), zfpFix, zfpFixLen) != 0)
        {
            tags[t->typeId()] = zftrue;
        }
    }
    for(zfindex i = 0; i < allNamespace.count(); ++i)
    {
        tags[allNamespace[i]] = zftrue;
    }

    for(zfstlmap<zfstlstringZ, zfbool>::iterator it = tags.begin(); it != tags.end(); ++it)
    {
        output << it->first.c_str() << "\n";
    }
}

ZFDynamic &ZFDynamic::regTag(ZF_IN const zfchar *regTag)
{
    zfstlmap<zfstlstringZ, ZFDynamic> &m = _ZFP_ZFDynamicRegTagMap();
    if(!d->regTag.empty())
    {
        m.erase(d->regTag);
    }

    if(regTag == zfnull)
    {
        d->regTag.clear();
        return *this;
    }

    zfstlmap<zfstlstringZ, ZFDynamic>::iterator it = m.find(regTag);
    if(it != m.end() && it->second != *this)
    {
        // remove may cause unexpect dealloc, retain once
        ZFDynamic holder = it->second;
        holder.removeAll();
    }
    d->regTag = regTag;
    m[regTag] = *this;
    return *this;
}
const zfchar *ZFDynamic::regTag(void) const
{
    return (d->regTag.empty() ? zfnull : d->regTag.c_str());
}

void ZFDynamic::removeAll(void)
{
    zfCoreMutexLocker();
    ZF_STATIC_INITIALIZER_CLASS(ZFDynamicDataHolder) *g = ZF_STATIC_INITIALIZER_INSTANCE(ZFDynamicDataHolder);

    if(!d->allEvent.isEmpty())
    {
        ZFCoreArrayPOD<zfidentity> allEvent = d->allEvent;
        d->allEvent = ZFCoreArrayPOD<zfidentity>();
        for(zfindex i = allEvent.count() - 1; i != zfindexMax(); --i)
        {
            g->allEvent.removeElement(allEvent[i]);
            ZFIdMapDynamicUnregister(allEvent[i]);
        }
    }

    if(!d->allMethod.isEmpty())
    {
        ZFCoreArrayPOD<const ZFMethod *> allMethod = d->allMethod;
        d->allMethod = ZFCoreArrayPOD<const ZFMethod *>();
        for(zfindex i = allMethod.count() - 1; i != zfindexMax(); --i)
        {
            g->allMethod.removeElement(allMethod[i]);
            ZFMethodDynamicUnregister(allMethod[i]);
        }
    }

    if(!d->allProperty.isEmpty())
    {
        ZFCoreArrayPOD<const ZFProperty *> allProperty = d->allProperty;
        d->allProperty = ZFCoreArrayPOD<const ZFProperty *>();
        for(zfindex i = allProperty.count() - 1; i != zfindexMax(); --i)
        {
            g->allProperty.removeElement(allProperty[i]);
            ZFPropertyDynamicUnregister(allProperty[i]);
        }
    }

    if(!d->allEnum.isEmpty())
    {
        ZFCoreArrayPOD<const ZFClass *> allEnum = d->allEnum;
        d->allEnum = ZFCoreArrayPOD<const ZFClass *>();
        for(zfindex i = allEnum.count() - 1; i != zfindexMax(); --i)
        {
            g->allEnum.removeElement(allEnum[i]);
            ZFEnumDynamicUnregister(allEnum[i]);
        }
    }

    if(!d->allClass.isEmpty())
    {
        ZFCoreArrayPOD<const ZFClass *> allClass = d->allClass;
        d->allClass = ZFCoreArrayPOD<const ZFClass *>();
        for(zfindex i = allClass.count() - 1; i != zfindexMax(); --i)
        {
            g->allClass.removeElement(allClass[i]);
            ZFClassDynamicUnregister(allClass[i]);
        }
    }

    d->errorOccurred = zffalse;
    if(!d->regTag.empty())
    {
        _ZFP_ZFDynamicRegTagMap().erase(d->regTag);
        d->regTag.clear();
    }
    d->cls = zfnull;
    d->enumClassName.removeAll();
    d->enumDefault = ZFEnumInvalid();
    d->enumIsFlags = zffalse;
    d->enumValueNext = 0;
    d->enumValues.removeAll();
    d->enumNames.removeAll();
    d->errorCallbackList.removeAll();
}

const ZFCoreArrayPOD<const ZFClass *> &ZFDynamic::allClass(void) const
{
    return d->allClass;
}
const ZFCoreArrayPOD<const ZFClass *> &ZFDynamic::allEnum(void) const
{
    return d->allEnum;
}
const ZFCoreArrayPOD<const ZFMethod *> &ZFDynamic::allMethod(void) const
{
    return d->allMethod;
}
const ZFCoreArrayPOD<const ZFProperty *> &ZFDynamic::allProperty(void) const
{
    return d->allProperty;
}
const ZFCoreArrayPOD<zfidentity> &ZFDynamic::allEvent(void) const
{
    return d->allEvent;
}

ZFDynamic &ZFDynamic::classBegin(ZF_IN const zfchar *classNameFull,
                                 ZF_IN_OPT const ZFClass *classParent /* = ZFObject::ClassData() */,
                                 ZF_IN_OPT ZFObject *classDynamicRegisterUserData /* = zfnull */)
{
    if(d->errorOccurred) {return *this;}
    if(!d->scopeBeginCheck()) {return *this;}
    d->cls = ZFClass::classForName(classNameFull);
    if(d->cls == zfnull)
    {
        zfstring errorHint;
        const ZFClass *dynClass = ZFClassDynamicRegister(
            classNameFull, classParent, classDynamicRegisterUserData, &errorHint);
        if(dynClass == zfnull)
        {
            d->error("unable to register class: %s, reason: %s",
                classNameFull,
                errorHint.cString());
        }
        else
        {
            d->allClass.add(dynClass);
            ZF_STATIC_INITIALIZER_INSTANCE(ZFDynamicDataHolder)->allClass.add(dynClass);
            d->cls = dynClass;
        }
    }
    return *this;
}
ZFDynamic &ZFDynamic::classBegin(ZF_IN const zfchar *classNameFull,
                                 ZF_IN const zfchar *parentClassNameFull,
                                 ZF_IN_OPT ZFObject *classDynamicRegisterUserData /* = zfnull */)
{
    if(d->errorOccurred) {return *this;}
    if(!d->scopeBeginCheck()) {return *this;}
    if(zfsIsEmpty(parentClassNameFull))
    {
        return this->classBegin(classNameFull, ZFObject::ClassData(), classDynamicRegisterUserData);
    }
    else
    {
        const ZFClass *classParent = ZFClass::classForName(parentClassNameFull);
        if(classParent == zfnull)
        {
            d->error("no such classParent: %s", parentClassNameFull);
            return *this;
        }
        else
        {
            return this->classBegin(classNameFull, classParent, classDynamicRegisterUserData);
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
        d->error("null class");
    }
    return *this;
}
ZFDynamic &ZFDynamic::classEnd(void)
{
    if(d->errorOccurred) {return *this;}
    if(d->cls == zfnull)
    {
        d->error("no paired classBegin");
    }
    d->cls = zfnull;
    return *this;
}

zfclass _ZFP_I_ZFDynamicOnInitData : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_I_ZFDynamicOnInitData, ZFObject)
public:
    ZFListener callback;
    zfautoObject userData;
};
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFDynamicOnInit, ZFLevelZFFrameworkEssential)
{
    this->classOnInitListener = ZFCallbackForFunc(zfself::classOnInit);
    this->classOnDeallocAttachListener = ZFCallbackForFunc(zfself::classOnDeallocAttach);
    this->classOnDeallocListener = ZFCallbackForFunc(zfself::classOnDealloc);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFDynamicOnInit)
{
    if(this->classOnChangeListener.callbackIsValid())
    {
        ZFClassDataChangeObserver.observerRemove(
            ZFGlobalEvent::EventClassDataChange(),
            this->classOnChangeListener);
    }
}
zfstlmap<const ZFClass *, zfbool> classOnInitMap;
zfstlmap<const ZFClass *, zfbool> classOnDeallocMap;
ZFListener classOnChangeListener;
ZFListener classOnInitListener;
ZFListener classOnDeallocAttachListener;
ZFListener classOnDeallocListener;
void checkAttach(void)
{
    if(!this->classOnChangeListener.callbackIsValid())
    {
        this->classOnChangeListener = ZFCallbackForFunc(zfself::classOnChange);
        ZFClassDataChangeObserver.observerAdd(
            ZFGlobalEvent::EventClassDataChange(),
            this->classOnChangeListener);
    }
}
static ZFLISTENER_PROTOTYPE_EXPAND(classOnChange)
{
    const ZFClassDataChangeData *data = listenerData.param0<ZFPointerHolder *>()->holdedDataPointer<const ZFClassDataChangeData *>();
    if(data->changeType == ZFClassDataChangeTypeDetach && data->changedClass != zfnull)
    {
        ZF_GLOBAL_INITIALIZER_CLASS(ZFDynamicOnInit) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFDynamicOnInit);
        d->classOnInitMap.erase(data->changedClass);
        d->classOnDeallocMap.erase(data->changedClass);
    }
}
static ZFLISTENER_PROTOTYPE_EXPAND(classOnInit)
{
    _ZFP_I_ZFDynamicOnInitData *data = userData->to<_ZFP_I_ZFDynamicOnInitData *>();
    if(data->callback.callbackIsValid())
    {
        data->callback.execute(listenerData, data->userData);
    }
}
static ZFLISTENER_PROTOTYPE_EXPAND(classOnDeallocAttach)
{
    listenerData.sender()->observerAdd(ZFObserverAddParam()
        .eventId(ZFObject::EventObjectBeforeDealloc())
        .observer(ZF_GLOBAL_INITIALIZER_INSTANCE(ZFDynamicOnInit)->classOnDeallocListener)
        .userData(userData)
        .observerLevel(ZFLevelZFFrameworkPostHigh)
        );
}
static ZFLISTENER_PROTOTYPE_EXPAND(classOnDealloc)
{
    _ZFP_I_ZFDynamicOnInitData *data = userData->to<_ZFP_I_ZFDynamicOnInitData *>();
    if(data->callback.callbackIsValid())
    {
        data->callback.execute(listenerData, data->userData);
    }
}
ZF_GLOBAL_INITIALIZER_END(ZFDynamicOnInit)

ZFDynamic &ZFDynamic::classOnInit(ZF_IN const ZFListener &classOnInitCallback,
                                  ZF_IN_OPT ZFObject *userData /* = zfnull */)
{
    if(d->errorOccurred) {return *this;}
    if(d->cls == zfnull)
    {
        d->error("have you forgot classBegin?");
        return *this;
    }
    if(!d->cls->classIsDynamicRegister())
    {
        d->error("only dynamic registered class can attach custom classOnInit");
        return *this;
    }
    if(!classOnInitCallback.callbackIsValid())
    {
        d->error("invalid callback");
        return *this;
    }
    ZF_GLOBAL_INITIALIZER_CLASS(ZFDynamicOnInit) *g = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFDynamicOnInit);
    if(g->classOnInitMap.find(d->cls) != g->classOnInitMap.end())
    {
        d->error("class %s already register a custom classOnInit",
            d->cls->classNameFull());
        return *this;
    }
    g->classOnInitMap[d->cls] = zftrue;
    zfblockedAlloc(_ZFP_I_ZFDynamicOnInitData, data);
    data->callback = classOnInitCallback;
    data->userData = userData;
    d->cls->instanceObserverAdd(g->classOnInitListener, data, zfnull, ZFLevelZFFrameworkHigh);
    g->checkAttach();
    return *this;
}
ZFDynamic &ZFDynamic::classOnDealloc(ZF_IN const ZFListener &classOnDeallocCallback,
                                     ZF_IN_OPT ZFObject *userData /* = zfnull */)
{
    if(d->errorOccurred) {return *this;}
    if(d->cls == zfnull)
    {
        d->error("have you forgot classBegin?");
        return *this;
    }
    if(!d->cls->classIsDynamicRegister())
    {
        d->error("only dynamic registered class can attach custom classOnDealloc");
        return *this;
    }
    if(!classOnDeallocCallback.callbackIsValid())
    {
        d->error("invalid callback");
        return *this;
    }
    ZF_GLOBAL_INITIALIZER_CLASS(ZFDynamicOnInit) *g = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFDynamicOnInit);
    if(g->classOnDeallocMap.find(d->cls) != g->classOnDeallocMap.end())
    {
        d->error("class %s already register a custom classOnDealloc",
            d->cls->classNameFull());
        return *this;
    }
    g->classOnDeallocMap[d->cls] = zftrue;
    zfblockedAlloc(_ZFP_I_ZFDynamicOnInitData, data);
    data->callback = classOnDeallocCallback;
    data->userData = userData;
    d->cls->instanceObserverAdd(g->classOnDeallocAttachListener, data, zfnull, ZFLevelZFFrameworkHigh);
    g->checkAttach();
    return *this;
}

ZFDynamic &ZFDynamic::NSBegin(ZF_IN_OPT const zfchar *methodNamespace /* = ZF_NAMESPACE_GLOBAL_NAME */)
{
    if(d->errorOccurred) {return *this;}
    if(!d->scopeBeginCheck()) {return *this;}
    if(zfsIsEmpty(methodNamespace) || zfscmpTheSame(methodNamespace, ZF_NAMESPACE_GLOBAL_ABBR_NAME))
    {
        d->methodNamespace = ZF_NAMESPACE_GLOBAL_NAME;
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
        d->error("no paired NSBegin");
    }
    d->methodNamespace.removeAll();
    return *this;
}

ZFDynamic &ZFDynamic::enumBegin(ZF_IN const zfchar *enumClassName)
{
    if(d->errorOccurred) {return *this;}
    if(!d->scopeBeginCheck()) {return *this;}
    d->enumClassName = enumClassName;
    return *this;
}
ZFDynamic &ZFDynamic::enumIsFlags(ZF_IN zfbool enumIsFlags)
{
    if(d->errorOccurred) {return *this;}
    if(d->enumClassName.isEmpty())
    {
        d->error("have you forgot enumBegin?");
        return *this;
    }
    d->enumIsFlags = enumIsFlags;
    return *this;
}
ZFDynamic &ZFDynamic::enumValue(ZF_IN const zfchar *enumName,
                                ZF_IN_OPT zfuint enumValue /* = ZFEnumInvalid() */)
{
    if(d->errorOccurred) {return *this;}
    if(d->enumClassName.isEmpty())
    {
        d->error("have you forgot enumBegin?");
        return *this;
    }
    if(enumValue == ZFEnumInvalid())
    {
        enumValue = d->enumValueNext;
        ++(d->enumValueNext);
    }
    else
    {
        if(d->enumValueNext <= enumValue)
        {
            d->enumValueNext = enumValue + 1;
        }
    }
    d->enumValues.add(enumValue);
    d->enumNames.add(enumName);
    return *this;
}
ZFDynamic &ZFDynamic::enumEnd(ZF_IN_OPT zfuint enumDefault /* = ZFEnumInvalid() */)
{
    if(d->errorOccurred) {return *this;}
    if(d->enumClassName.isEmpty())
    {
        d->error("have you forgot enumBegin?");
        return *this;
    }

    zfstring errorHint;
    const ZFClass *enumClass = ZFEnumDynamicRegister(
        d->enumClassName,
        d->enumValues,
        d->enumNames,
        d->enumDefault,
        d->enumIsFlags,
        &errorHint);
    if(enumClass == zfnull)
    {
        d->error("unable to register enum, reason: %s", errorHint.cString());
        return *this;
    }
    d->allEnum.add(enumClass);
    ZF_STATIC_INITIALIZER_INSTANCE(ZFDynamicDataHolder)->allEnum.add(enumClass);
    d->enumClassName.removeAll();
    d->enumDefault = ZFEnumInvalid();
    d->enumIsFlags = zffalse;
    d->enumValueNext = 0;
    d->enumValues.removeAll();
    d->enumNames.removeAll();
    return *this;
}

static zfbool _ZFP_ZFDynamicEventGI(ZFMETHOD_GENERIC_INVOKER_PARAMS)
{
    ret = invokerMethod->methodDynamicRegisterUserData();
    return zftrue;
}
ZFDynamic &ZFDynamic::event(ZF_IN const zfchar *eventName)
{
    if(d->errorOccurred) {return *this;}
    if(!d->enumClassName.isEmpty())
    {
        d->error("can not be called within enumBegin");
        return *this;
    }
    if(zfsIsEmpty(eventName))
    {
        d->error("empty event name");
        return *this;
    }
    zfstring idName;
    if(d->cls != zfnull)
    {
        idName += d->cls->classNameFull();
    }
    else
    {
        if(d->methodNamespace.isEmpty())
        {
            idName += ZF_NAMESPACE_GLOBAL_NAME;
        }
        else
        {
            idName += d->methodNamespace;
        }
    }
    idName += "::Event";
    idName += eventName;
    zfidentity idValue = ZFIdMapIdForName(idName);
    if(idValue != zfidentityInvalid())
    {
        d->error("%s already exists", idName.cString());
        return *this;
    }
    idValue = ZFIdMapDynamicRegister(idName);
    d->allEvent.add(idValue);
    ZF_STATIC_INITIALIZER_INSTANCE(ZFDynamicDataHolder)->allEvent.add(idValue);

    zfblockedAlloc(v_zfidentity, t);
    t->zfv = idValue;
    const ZFMethod *method = ZFMethodDynamicRegister(ZFMethodDynamicRegisterParam()
            .methodGenericInvoker(_ZFP_ZFDynamicEventGI)
            .methodDynamicRegisterUserData(t)
            .methodOwnerClass(d->cls)
            .methodNamespace(d->methodNamespace)
            .methodName(zfstringWithFormat("Event%s", eventName))
            .methodReturnTypeId(ZFTypeId_zfidentity())
        );
    zfCoreAssert(method != zfnull);
    d->allMethod.add(method);
    ZF_STATIC_INITIALIZER_INSTANCE(ZFDynamicDataHolder)->allMethod.add(method);

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
    d->param0 = paramList[0];
    d->param1 = paramList[1];
    d->param2 = paramList[2];
    d->param3 = paramList[3];
    d->param4 = paramList[4];
    d->param5 = paramList[5];
    d->param6 = paramList[6];
    d->param7 = paramList[7];

    _ZFP_I_ZFDynamicMethodTask *task = ZFCastZFObjectUnchecked(_ZFP_I_ZFDynamicMethodTask *, invokerMethod->methodDynamicRegisterUserData());
    task->methodCallback.execute(ZFListenerData().param0(d), task->methodCallbackUserData);
    ret = d->ret;
    if(errorHint != zfnull)
    {
        *errorHint += d->errorHint;
    }
    if(d->invokeSuccess)
    {
        paramList[0] = d->param0;
        paramList[1] = d->param1;
        paramList[2] = d->param2;
        paramList[3] = d->param3;
        paramList[4] = d->param4;
        paramList[5] = d->param5;
        paramList[6] = d->param6;
        paramList[7] = d->param7;
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
        d->error("invalid methodCallback");
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
            .methodDynamicRegisterUserData(methodDynamicRegisterUserData)
            .methodOwnerClass(d->cls)
            .methodNamespace(d->methodNamespace)
            .methodGenericInvoker(methodGenericInvoker)
            .methodName(methodName)
            .methodReturnTypeId(methodReturnTypeId)
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
    if(!d->enumClassName.isEmpty())
    {
        d->error("can not be called within enumBegin");
        return *this;
    }
    zfstring errorHint;
    const ZFMethod *dynMethod = ZFMethodDynamicRegister(param, &errorHint);
    if(dynMethod == zfnull)
    {
        d->error("unable to register method, reason: %s", errorHint.cString());
    }
    else
    {
        d->allMethod.add(dynMethod);
        ZF_STATIC_INITIALIZER_INSTANCE(ZFDynamicDataHolder)->allMethod.add(dynMethod);
    }
    return *this;
}

static zfautoObject _ZFP_ZFDynamicPropertyInit(ZF_IN const ZFProperty *property)
{
    ZFCopyable *copyable = property->propertyDynamicRegisterUserData()->toAny();
    if(copyable != zfnull)
    {
        return copyable->copy();
    }
    ZFStyleable *styleable = property->propertyDynamicRegisterUserData()->toAny();
    if(styleable != zfnull)
    {
        zfautoObject ret = styleable->classData()->newInstance();
        ret.to<ZFStyleable *>()->styleableCopyFrom(styleable);
        return ret;
    }
    return zfnull;
}
ZFDynamic &ZFDynamic::property(ZF_IN const zfchar *propertyTypeId,
                               ZF_IN const zfchar *propertyName,
                               ZF_IN_OPT ZFObject *propertyInitValue /* = zfnull */,
                               ZF_IN_OPT ZFMethodPrivilegeType setterPrivilegeType /* = ZFMethodPrivilegeTypePublic */,
                               ZF_IN_OPT ZFMethodPrivilegeType getterPrivilegeType /* = ZFMethodPrivilegeTypePublic */)
{
    if(d->errorOccurred) {return *this;}
    const ZFClass *cls = ZFClass::classForName(propertyTypeId);
    if(cls != zfnull)
    {
        return this->property(cls, propertyName, propertyInitValue, setterPrivilegeType, getterPrivilegeType);
    }
    ZFPropertyDynamicRegisterParam param;
    param.propertyOwnerClass(d->cls);
    param.propertyTypeId(propertyTypeId);
    param.propertyClassOfRetainProperty(ZFClass::classForName(propertyTypeId));
    param.propertyName(propertyName);
    param.propertySetterType(setterPrivilegeType);
    param.propertyGetterType(getterPrivilegeType);
    if(propertyInitValue != zfnull)
    {
        ZFTypeIdWrapper *propertyInitValueWrapper = ZFCastZFObject(ZFTypeIdWrapper *, propertyInitValue);
        if(propertyInitValueWrapper == zfnull)
        {
            d->error("assign property's type must be %s: %s",
                ZFTypeIdWrapper::ClassData()->classNameFull(),
                propertyInitValue->objectInfo().cString());
            return *this;
        }
        param.propertyInitValueCallback(_ZFP_ZFDynamicPropertyInit);
        param.propertyDynamicRegisterUserData(propertyInitValue);
    }
    return this->property(param);
}
ZFDynamic &ZFDynamic::property(ZF_IN const ZFClass *propertyClassOfRetainProperty,
                               ZF_IN const zfchar *propertyName,
                               ZF_IN_OPT ZFObject *propertyInitValue /* = zfnull */,
                               ZF_IN_OPT ZFMethodPrivilegeType setterPrivilegeType /* = ZFMethodPrivilegeTypePublic */,
                               ZF_IN_OPT ZFMethodPrivilegeType getterPrivilegeType /* = ZFMethodPrivilegeTypePublic */)
{
    if(d->errorOccurred) {return *this;}
    if(propertyClassOfRetainProperty == zfnull)
    {
        d->error("propertyClassOfRetainProperty not set");
        return *this;
    }
    ZFPropertyDynamicRegisterParam param;
    param.propertyOwnerClass(d->cls);
    param.propertyTypeId(propertyClassOfRetainProperty->classNameFull());
    param.propertyName(propertyName);
    param.propertyClassOfRetainProperty(propertyClassOfRetainProperty);
    param.propertySetterType(setterPrivilegeType);
    param.propertyGetterType(getterPrivilegeType);
    if(propertyInitValue != zfnull)
    {
        if(!propertyInitValue->classData()->classIsTypeOf(propertyClassOfRetainProperty))
        {
            d->error("init value %s is not type of %s",
                propertyInitValue->objectInfo().cString(),
                propertyClassOfRetainProperty->classNameFull());
            return *this;
        }
        if(ZFCastZFObject(ZFCopyable *, propertyInitValue) == zfnull
            && ZFCastZFObject(ZFStyleable *, propertyInitValue) == zfnull)
        {
            d->error("init value %s is not type of %s or %s",
                propertyInitValue->objectInfo().cString(),
                ZFCopyable::ClassData()->classNameFull(),
                ZFStyleable::ClassData()->classNameFull());
            return *this;
        }
        param.propertyInitValueCallback(_ZFP_ZFDynamicPropertyInit);
        param.propertyDynamicRegisterUserData(propertyInitValue);
    }
    return this->property(param);
}
ZFDynamic &ZFDynamic::property(ZF_IN const ZFPropertyDynamicRegisterParam &param)
{
    if(d->errorOccurred) {return *this;}
    if(!d->enumClassName.isEmpty())
    {
        d->error("can not be called within enumBegin");
        return *this;
    }
    if(d->cls == zfnull)
    {
        d->error("have you forgot classBegin?");
        return *this;
    }
    zfstring errorHint;
    const ZFProperty *dynProperty = ZFPropertyDynamicRegister(param, &errorHint);
    if(dynProperty == zfnull)
    {
        d->error("unable to register property, reason: %s", errorHint.cString());
    }
    else
    {
        d->allProperty.add(dynProperty);
        ZF_STATIC_INITIALIZER_INSTANCE(ZFDynamicDataHolder)->allProperty.add(dynProperty);
    }
    return *this;
}

ZFDynamic &ZFDynamic::errorCallbackAdd(ZF_IN const ZFOutput &errorCallback /* = ZFOutputDefault() */)
{
    if(errorCallback.callbackIsValid())
    {
        d->errorCallbackList.add(errorCallback);
    }
    return *this;
}
ZFDynamic &ZFDynamic::errorCallbackRemove(ZF_IN const ZFOutput &errorCallback)
{
    d->errorCallbackList.removeElement(errorCallback);
    return *this;
}
zfindex ZFDynamic::errorCallbackCount(void) const
{
    return d->errorCallbackList.count();
}
const ZFOutput &ZFDynamic::errorCallbackAtIndex(ZF_IN zfindex index) const
{
    return d->errorCallbackList[index];
}
void ZFDynamic::errorCallbackNotify(ZF_IN const zfchar *errorHint) const
{
    d->error("%s", errorHint);
}

// ============================================================
ZFTYPEID_ACCESS_ONLY_DEFINE(ZFDynamic, ZFDynamic)

// ============================================================
ZFMETHOD_FUNC_DEFINE_0(void, ZFDynamicRemoveAll)
{
    ZF_STATIC_INITIALIZER_INSTANCE(ZFDynamicDataHolder)->removeAll();
}
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFDynamicRemoveAllAutoNotify, ZFLevelZFFrameworkNormal)
{
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFDynamicRemoveAllAutoNotify)
{
    ZFDynamicRemoveAll();
}
ZF_GLOBAL_INITIALIZER_END(ZFDynamicRemoveAllAutoNotify)

// ============================================================
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_STATIC_1(ZFDynamic, v_ZFDynamic, void, exportTag, ZFMP_IN_OUT(const ZFOutput &, output))
ZFOBJECT_ON_INIT_USER_REGISTER_1({
        invokerObject->to<v_ZFDynamic *>()->zfv.regTag(regTag);
    }, v_ZFDynamic
    , ZFMP_IN(const zfchar *, regTag)
    )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFDynamic, ZFDynamic &, regTag, ZFMP_IN(const zfchar *, regTag))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFDynamic, const zfchar *, regTag)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFDynamic, void, removeAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFDynamic, const ZFCoreArrayPOD<const ZFClass *> &, allClass)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFDynamic, const ZFCoreArrayPOD<const ZFClass *> &, allEnum)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFDynamic, const ZFCoreArrayPOD<const ZFMethod *> &, allMethod)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFDynamic, const ZFCoreArrayPOD<const ZFProperty *> &, allProperty)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFDynamic, const ZFCoreArrayPOD<zfidentity> &, allEvent)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_3(v_ZFDynamic, ZFDynamic &, classBegin, ZFMP_IN(const zfchar *, classNameFull), ZFMP_IN_OPT(const ZFClass *, classParent, ZFObject::ClassData()), ZFMP_IN_OPT(ZFObject *, classDynamicRegisterUserData, zfnull))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_3(v_ZFDynamic, ZFDynamic &, classBegin, ZFMP_IN(const zfchar *, classNameFull), ZFMP_IN(const zfchar *, parentClassNameFull), ZFMP_IN_OPT(ZFObject *, classDynamicRegisterUserData, zfnull))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFDynamic, ZFDynamic &, classBegin, ZFMP_IN(const ZFClass *, cls))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFDynamic, ZFDynamic &, classEnd)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFDynamic, ZFDynamic &, classOnInit, ZFMP_IN(const ZFListener &, classOnInitCallback), ZFMP_IN_OPT(ZFObject *, userData, zfnull))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFDynamic, ZFDynamic &, classOnDealloc, ZFMP_IN(const ZFListener &, classOnDeallocCallback), ZFMP_IN_OPT(ZFObject *, userData, zfnull))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFDynamic, ZFDynamic &, NSBegin, ZFMP_IN_OPT(const zfchar *, methodNamespace, ZF_NAMESPACE_GLOBAL_NAME))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFDynamic, ZFDynamic &, NSEnd)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFDynamic, ZFDynamic &, enumBegin, ZFMP_IN(const zfchar *, enumClassName))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFDynamic, ZFDynamic &, enumIsFlags, ZFMP_IN(zfbool, enumIsFlags))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFDynamic, ZFDynamic &, enumValue, ZFMP_IN(const zfchar *, enumName), ZFMP_IN_OPT(zfuint, enumValue, ZFEnumInvalid()))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFDynamic, ZFDynamic &, enumEnd, ZFMP_IN_OPT(zfuint, enumDefault, ZFEnumInvalid()))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFDynamic, ZFDynamic &, event, ZFMP_IN(const zfchar *, eventName))
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
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_5(v_ZFDynamic, ZFDynamic &, property, ZFMP_IN(const zfchar *, propertyTypeId), ZFMP_IN(const zfchar *, propertyName), ZFMP_IN_OPT(ZFObject *, propertyInitValue, zfnull), ZFMP_IN_OPT(ZFMethodPrivilegeType, setterPrivilegeType, ZFMethodPrivilegeTypePublic), ZFMP_IN_OPT(ZFMethodPrivilegeType, getterPrivilegeType, ZFMethodPrivilegeTypePublic))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_5(v_ZFDynamic, ZFDynamic &, property, ZFMP_IN(const ZFClass *, propertyClassOfRetainProperty), ZFMP_IN(const zfchar *, propertyName), ZFMP_IN_OPT(ZFObject *, propertyInitValue, zfnull), ZFMP_IN_OPT(ZFMethodPrivilegeType, setterPrivilegeType, ZFMethodPrivilegeTypePublic), ZFMP_IN_OPT(ZFMethodPrivilegeType, getterPrivilegeType, ZFMethodPrivilegeTypePublic))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFDynamic, ZFDynamic &, property, ZFMP_IN(const ZFPropertyDynamicRegisterParam &, param))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFDynamic, ZFDynamic &, errorCallbackAdd, ZFMP_IN_OPT(const ZFOutput &, errorCallback, ZFOutputDefault()))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFDynamic, ZFDynamic &, errorCallbackRemove, ZFMP_IN(const ZFOutput &, errorCallback))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFDynamic, zfindex, errorCallbackCount)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFDynamic, const ZFOutput &, errorCallbackAtIndex, ZFMP_IN(zfindex, index))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFDynamic, void, errorCallbackNotify, ZFMP_IN(const zfchar *, errorHint))

ZF_NAMESPACE_GLOBAL_END

