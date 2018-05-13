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
 * @file ZFMethodGenericInvoker.h
 * @brief generic invoker declare for #ZFMethod::methodGenericInvoker
 */

#ifndef _ZFI_ZFMethodGenericInvoker_h_
#define _ZFI_ZFMethodGenericInvoker_h_

#include "ZFPropertyTypeFwd.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassFwd ZFMethod;
/* ZFMETHOD_MAX_PARAM */
/**
 * @brief generic invoker for advanced reflection, see #ZFMethod::methodGenericInvoker
 */
typedef zfbool (*ZFMethodGenericInvoker)(ZF_IN const ZFMethod *invokerMethod
                                         , ZF_IN ZFObject *invokerObject
                                         , ZF_OUT_OPT zfstring *errorHint
                                         , ZF_OUT zfautoObject &ret
                                         , ZF_IN_OPT ZFObject *param0 /* = ZFMethodGenericInvokerDefaultParam() */
                                         , ZF_IN_OPT ZFObject *param1 /* = ZFMethodGenericInvokerDefaultParam() */
                                         , ZF_IN_OPT ZFObject *param2 /* = ZFMethodGenericInvokerDefaultParam() */
                                         , ZF_IN_OPT ZFObject *param3 /* = ZFMethodGenericInvokerDefaultParam() */
                                         , ZF_IN_OPT ZFObject *param4 /* = ZFMethodGenericInvokerDefaultParam() */
                                         , ZF_IN_OPT ZFObject *param5 /* = ZFMethodGenericInvokerDefaultParam() */
                                         , ZF_IN_OPT ZFObject *param6 /* = ZFMethodGenericInvokerDefaultParam() */
                                         , ZF_IN_OPT ZFObject *param7 /* = ZFMethodGenericInvokerDefaultParam() */
                                         );

/**
 * @brief util macro to expand params for #ZFMethodGenericInvoker
 */
#define ZFMETHOD_GENERIC_INVOKER_PARAMS \
    ZF_IN const ZFMethod *invokerMethod \
    , ZF_IN ZFObject *invokerObject \
    , ZF_OUT_OPT zfstring *errorHint \
    , ZF_OUT zfautoObject &ret \
    , ZF_IN_OPT ZFObject *param0 /* = ZFMethodGenericInvokerDefaultParam() */ \
    , ZF_IN_OPT ZFObject *param1 /* = ZFMethodGenericInvokerDefaultParam() */ \
    , ZF_IN_OPT ZFObject *param2 /* = ZFMethodGenericInvokerDefaultParam() */ \
    , ZF_IN_OPT ZFObject *param3 /* = ZFMethodGenericInvokerDefaultParam() */ \
    , ZF_IN_OPT ZFObject *param4 /* = ZFMethodGenericInvokerDefaultParam() */ \
    , ZF_IN_OPT ZFObject *param5 /* = ZFMethodGenericInvokerDefaultParam() */ \
    , ZF_IN_OPT ZFObject *param6 /* = ZFMethodGenericInvokerDefaultParam() */ \
    , ZF_IN_OPT ZFObject *param7 /* = ZFMethodGenericInvokerDefaultParam() */ \
    ZFM_EMPTY()

extern ZF_ENV_EXPORT ZFObject *_ZFP_ZFMethodGenericInvokerDefaultParamRef;
/**
 * @brief mark the object as default param
 */
#define ZFMethodGenericInvokerDefaultParam() ((ZFObject * const &)_ZFP_ZFMethodGenericInvokerDefaultParamRef)

extern ZF_ENV_EXPORT zfautoObject _ZFP_ZFMethodGenericInvokerDefaultParamHolderRef;
/**
 * @brief holder of #ZFMethodGenericInvokerDefaultParam
 */
#define ZFMethodGenericInvokerDefaultParamHolder() ((zfautoObject const &)_ZFP_ZFMethodGenericInvokerDefaultParamHolderRef)

/**
 * @brief util to call #ZFMethodGenericInvoker
 */
extern ZF_ENV_EXPORT zfbool ZFMethodGenericInvoke(ZF_IN const ZFMethod *invokerMethod
                                                  , ZF_IN ZFObject *invokerObject
                                                  , ZF_OUT_OPT zfstring *errorHint
                                                  , ZF_OUT zfautoObject &ret
                                                  , ZF_IN_OPT ZFObject *param0 = ZFMethodGenericInvokerDefaultParam()
                                                  , ZF_IN_OPT ZFObject *param1 = ZFMethodGenericInvokerDefaultParam()
                                                  , ZF_IN_OPT ZFObject *param2 = ZFMethodGenericInvokerDefaultParam()
                                                  , ZF_IN_OPT ZFObject *param3 = ZFMethodGenericInvokerDefaultParam()
                                                  , ZF_IN_OPT ZFObject *param4 = ZFMethodGenericInvokerDefaultParam()
                                                  , ZF_IN_OPT ZFObject *param5 = ZFMethodGenericInvokerDefaultParam()
                                                  , ZF_IN_OPT ZFObject *param6 = ZFMethodGenericInvokerDefaultParam()
                                                  , ZF_IN_OPT ZFObject *param7 = ZFMethodGenericInvokerDefaultParam()
                                                  );

// ============================================================
template<typename T_Dummy, int n>
zfclassNotPOD _ZFP_MtdGICk
{
};
template<typename T_Dummy>
zfclassNotPOD _ZFP_MtdGICk<T_Dummy, 1>
{
public:
    typedef int TypeMustRegisterByZFPROPERTY;
};
extern ZF_ENV_EXPORT zfbool _ZFP_MtdGIParamCheck(ZF_OUT_OPT zfstring *errorHint,
                                                 ZF_IN zfbool accessAvailable,
                                                 ZF_IN const ZFMethod *invokerMethod,
                                                 ZF_IN zfindex paramIndex,
                                                 ZF_IN const zfchar *paramType,
                                                 ZF_IN ZFObject *param);
#define _ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_DECLARE_EXPAND(N, ParamType) \
    typedef ParamType _T##N; \
    typedef zftTraits<ParamType>::TrNoRef _TR##N; \
    typedef _ZFP_MtdGICk< \
            _TR##N, \
            ZFPropertyTypeIdData<_TR##N>::PropertyRegistered \
        >::TypeMustRegisterByZFPROPERTY _Ck##N;
#define _ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_PREPARE_EXPAND(N, DefaultExpandOrEmpty, ParamType, param) \
    _ZFP_MtdGIParamCheck( \
        errorHint, \
        ZFPropertyTypeIdData<_TR##N>::Value<_T##N>::accessAvailable(param), \
        invokerMethod, \
        N, \
        ZFM_TOSTRING(ParamType), \
        param)
#define _ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_ACCESS_EXPAND(N, DefaultExpandOrEmpty, ParamType, param) \
    ZFPropertyTypeIdData<_TR##N>::Value<_T##N>::access( \
            DefaultExpandOrEmpty(param != ZFMethodGenericInvokerDefaultParam() ?) \
            param \
            DefaultExpandOrEmpty(: pDef##N().toObject()) \
        )
#define _ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS(N, DefaultExpandOrEmpty, ParamType, DefaultValueFix) \
    DefaultExpandOrEmpty( \
        static zfautoObject pDef##N(void) \
        { \
            zftValue<zftTraits<ParamType>::TrNoRef> paramDefault; \
            zfautoObject ret; \
            ZFPropertyTypeIdData<zftTraits<ParamType>::TrNoRef>::ValueStore( \
                ret, \
                (paramDefault.zfv DefaultValueFix())); \
            return ret; \
        } \
    )

// ============================================================
extern ZF_ENV_EXPORT void _ZFP_MtdGIRetError(ZF_OUT_OPT zfstring *errorHint,
                                             ZF_IN const zfchar *returnTypeId,
                                             ZF_IN const zfchar *returnValueInfo);

template<typename T_ReturnType>
zfclassNotPOD _ZFP_MtdGIFix
{
public:
    typedef T_ReturnType (*Ivk)(ZF_IN const ZFMethod *invokerMethod
                                , ZF_IN ZFObject *invokerObject
                                , ZF_IN ZFObject *param0
                                , ZF_IN ZFObject *param1
                                , ZF_IN ZFObject *param2
                                , ZF_IN ZFObject *param3
                                , ZF_IN ZFObject *param4
                                , ZF_IN ZFObject *param5
                                , ZF_IN ZFObject *param6
                                , ZF_IN ZFObject *param7
                                );
public:
    static zfbool action(ZF_IN Ivk invoke
                         , ZF_IN const ZFMethod *invokerMethod
                         , ZF_IN ZFObject *invokerObject
                         , ZF_OUT_OPT zfstring *errorHint
                         , ZF_OUT_OPT zfautoObject &ret
                         , ZF_IN ZFObject *param0
                         , ZF_IN ZFObject *param1
                         , ZF_IN ZFObject *param2
                         , ZF_IN ZFObject *param3
                         , ZF_IN ZFObject *param4
                         , ZF_IN ZFObject *param5
                         , ZF_IN ZFObject *param6
                         , ZF_IN ZFObject *param7
                         )
    {
        T_ReturnType retTmp = invoke(invokerMethod, invokerObject
                , param0
                , param1
                , param2
                , param3
                , param4
                , param5
                , param6
                , param7
            );
        typedef typename zftTraits<T_ReturnType>::TrNoRef T_ReturnTypeTmp;
        if(ZFPropertyTypeIdData<T_ReturnTypeTmp>::ValueStore(ret, retTmp))
        {
            return zftrue;
        }
        else
        {
            zfstring info;
            ZFCoreElementInfoGetter<T_ReturnTypeTmp>::elementInfoGetter(info, retTmp);
            _ZFP_MtdGIRetError(errorHint,
                ZFPropertyTypeIdData<T_ReturnTypeTmp>::PropertyTypeId(),
                info);
            return zffalse;
        }
    }
};
template<>
zfclassNotPOD _ZFP_MtdGIFix<void>
{
public:
    typedef void (*Ivk)(ZF_IN const ZFMethod *invokerMethod
                        , ZF_IN ZFObject *invokerObject
                        , ZF_IN ZFObject *param0
                        , ZF_IN ZFObject *param1
                        , ZF_IN ZFObject *param2
                        , ZF_IN ZFObject *param3
                        , ZF_IN ZFObject *param4
                        , ZF_IN ZFObject *param5
                        , ZF_IN ZFObject *param6
                        , ZF_IN ZFObject *param7
                        );
public:
    static zfbool action(ZF_IN Ivk invoke
                         , ZF_IN const ZFMethod *invokerMethod
                         , ZF_IN ZFObject *invokerObject
                         , ZF_OUT_OPT zfstring *errorHint
                         , ZF_OUT_OPT zfautoObject &ret
                         , ZF_IN ZFObject *param0
                         , ZF_IN ZFObject *param1
                         , ZF_IN ZFObject *param2
                         , ZF_IN ZFObject *param3
                         , ZF_IN ZFObject *param4
                         , ZF_IN ZFObject *param5
                         , ZF_IN ZFObject *param6
                         , ZF_IN ZFObject *param7
                         )
    {
        invoke(invokerMethod, invokerObject
                , param0
                , param1
                , param2
                , param3
                , param4
                , param5
                , param6
                , param7
            );
        return zftrue;
    }
};

// ============================================================
#define _ZFP_ZFMETHOD_GENERIC_INVOKER_DECLARE( \
        ReturnType \
        , ParamExpandOrEmpty0, ParamType0, param0_, DefaultExpandOrEmpty0, DefaultValueFix0 \
        , ParamExpandOrEmpty1, ParamType1, param1_, DefaultExpandOrEmpty1, DefaultValueFix1 \
        , ParamExpandOrEmpty2, ParamType2, param2_, DefaultExpandOrEmpty2, DefaultValueFix2 \
        , ParamExpandOrEmpty3, ParamType3, param3_, DefaultExpandOrEmpty3, DefaultValueFix3 \
        , ParamExpandOrEmpty4, ParamType4, param4_, DefaultExpandOrEmpty4, DefaultValueFix4 \
        , ParamExpandOrEmpty5, ParamType5, param5_, DefaultExpandOrEmpty5, DefaultValueFix5 \
        , ParamExpandOrEmpty6, ParamType6, param6_, DefaultExpandOrEmpty6, DefaultValueFix6 \
        , ParamExpandOrEmpty7, ParamType7, param7_, DefaultExpandOrEmpty7, DefaultValueFix7 \
    ) \
    private: \
        ParamExpandOrEmpty0(_ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_DECLARE_EXPAND(0, ParamType0)) \
        ParamExpandOrEmpty1(_ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_DECLARE_EXPAND(1, ParamType1)) \
        ParamExpandOrEmpty2(_ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_DECLARE_EXPAND(2, ParamType2)) \
        ParamExpandOrEmpty3(_ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_DECLARE_EXPAND(3, ParamType3)) \
        ParamExpandOrEmpty4(_ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_DECLARE_EXPAND(4, ParamType4)) \
        ParamExpandOrEmpty5(_ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_DECLARE_EXPAND(5, ParamType5)) \
        ParamExpandOrEmpty6(_ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_DECLARE_EXPAND(6, ParamType6)) \
        ParamExpandOrEmpty7(_ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_DECLARE_EXPAND(7, ParamType7)) \
    public: \
        ParamExpandOrEmpty0(_ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS(0, DefaultExpandOrEmpty0, ParamType0, DefaultValueFix0)) \
        ParamExpandOrEmpty1(_ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS(1, DefaultExpandOrEmpty1, ParamType1, DefaultValueFix1)) \
        ParamExpandOrEmpty2(_ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS(2, DefaultExpandOrEmpty2, ParamType2, DefaultValueFix2)) \
        ParamExpandOrEmpty3(_ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS(3, DefaultExpandOrEmpty3, ParamType3, DefaultValueFix3)) \
        ParamExpandOrEmpty4(_ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS(4, DefaultExpandOrEmpty4, ParamType4, DefaultValueFix4)) \
        ParamExpandOrEmpty5(_ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS(5, DefaultExpandOrEmpty5, ParamType5, DefaultValueFix5)) \
        ParamExpandOrEmpty6(_ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS(6, DefaultExpandOrEmpty6, ParamType6, DefaultValueFix6)) \
        ParamExpandOrEmpty7(_ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS(7, DefaultExpandOrEmpty7, ParamType7, DefaultValueFix7)) \
    public: \
        static zfbool GI(ZF_IN const ZFMethod *invokerMethod \
                         , ZF_IN ZFObject *invokerObject \
                         , ZF_OUT_OPT zfstring *errorHint \
                         , ZF_OUT_OPT zfautoObject &ret \
                         , ZF_IN ZFObject *param0 \
                         , ZF_IN ZFObject *param1 \
                         , ZF_IN ZFObject *param2 \
                         , ZF_IN ZFObject *param3 \
                         , ZF_IN ZFObject *param4 \
                         , ZF_IN ZFObject *param5 \
                         , ZF_IN ZFObject *param6 \
                         , ZF_IN ZFObject *param7 \
                         ) \
        { \
            ParamExpandOrEmpty0( \
                if( \
                    !_ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_PREPARE_EXPAND(0, DefaultExpandOrEmpty0, ParamType0, param0) \
                    ParamExpandOrEmpty1(|| !_ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_PREPARE_EXPAND(1, DefaultExpandOrEmpty1, ParamType1, param1)) \
                    ParamExpandOrEmpty2(|| !_ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_PREPARE_EXPAND(2, DefaultExpandOrEmpty2, ParamType2, param2)) \
                    ParamExpandOrEmpty3(|| !_ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_PREPARE_EXPAND(3, DefaultExpandOrEmpty3, ParamType3, param3)) \
                    ParamExpandOrEmpty4(|| !_ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_PREPARE_EXPAND(4, DefaultExpandOrEmpty4, ParamType4, param4)) \
                    ParamExpandOrEmpty5(|| !_ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_PREPARE_EXPAND(5, DefaultExpandOrEmpty5, ParamType5, param5)) \
                    ParamExpandOrEmpty6(|| !_ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_PREPARE_EXPAND(6, DefaultExpandOrEmpty6, ParamType6, param6)) \
                    ParamExpandOrEmpty7(|| !_ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_PREPARE_EXPAND(7, DefaultExpandOrEmpty7, ParamType7, param7)) \
                    ) \
                { \
                    return zffalse; \
                } \
            ) \
            zfbool _ret = _ZFP_MtdGIFix<ReturnType>::action(I, invokerMethod, invokerObject, errorHint, ret \
                    , param0 \
                    , param1 \
                    , param2 \
                    , param3 \
                    , param4 \
                    , param5 \
                    , param6 \
                    , param7 \
                ); \
            return _ret; \
        } \
    private: \
        static ReturnType I(ZF_IN const ZFMethod *invokerMethod \
                            , ZF_IN ZFObject *invokerObject \
                            , ZF_IN ZFObject *param0 \
                            , ZF_IN ZFObject *param1 \
                            , ZF_IN ZFObject *param2 \
                            , ZF_IN ZFObject *param3 \
                            , ZF_IN ZFObject *param4 \
                            , ZF_IN ZFObject *param5 \
                            , ZF_IN ZFObject *param6 \
                            , ZF_IN ZFObject *param7 \
                            ) \
        { \
            return invokerMethod->execute<ReturnType \
                    ParamExpandOrEmpty0(ZFM_COMMA() ParamType0) \
                    ParamExpandOrEmpty1(ZFM_COMMA() ParamType1) \
                    ParamExpandOrEmpty2(ZFM_COMMA() ParamType2) \
                    ParamExpandOrEmpty3(ZFM_COMMA() ParamType3) \
                    ParamExpandOrEmpty4(ZFM_COMMA() ParamType4) \
                    ParamExpandOrEmpty5(ZFM_COMMA() ParamType5) \
                    ParamExpandOrEmpty6(ZFM_COMMA() ParamType6) \
                    ParamExpandOrEmpty7(ZFM_COMMA() ParamType7) \
                >(invokerObject \
                    ParamExpandOrEmpty0(ZFM_COMMA() _ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_ACCESS_EXPAND(0, DefaultExpandOrEmpty0, ParamType0, param0)) \
                    ParamExpandOrEmpty1(ZFM_COMMA() _ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_ACCESS_EXPAND(1, DefaultExpandOrEmpty1, ParamType1, param1)) \
                    ParamExpandOrEmpty2(ZFM_COMMA() _ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_ACCESS_EXPAND(2, DefaultExpandOrEmpty2, ParamType2, param2)) \
                    ParamExpandOrEmpty3(ZFM_COMMA() _ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_ACCESS_EXPAND(3, DefaultExpandOrEmpty3, ParamType3, param3)) \
                    ParamExpandOrEmpty4(ZFM_COMMA() _ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_ACCESS_EXPAND(4, DefaultExpandOrEmpty4, ParamType4, param4)) \
                    ParamExpandOrEmpty5(ZFM_COMMA() _ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_ACCESS_EXPAND(5, DefaultExpandOrEmpty5, ParamType5, param5)) \
                    ParamExpandOrEmpty6(ZFM_COMMA() _ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_ACCESS_EXPAND(6, DefaultExpandOrEmpty6, ParamType6, param6)) \
                    ParamExpandOrEmpty7(ZFM_COMMA() _ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_ACCESS_EXPAND(7, DefaultExpandOrEmpty7, ParamType7, param7)) \
                ); \
        } \
    public:
#define _ZFP_ZFMETHOD_GENERIC_INVOKER_ADDR(owner) \
    owner::GI
#define _ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS_ADDR(owner, DefaultExpandOrEmpty, N) \
    (zfnull DefaultExpandOrEmpty(ZFM_EMPTY(), owner::pDef##N))

// ============================================================
extern ZF_ENV_EXPORT void _ZFP_ZFMethodGenericInvokeError(ZF_IN const ZFMethod *method,
                                                          ZF_IN ZFObject *obj,
                                                          ZF_IN zfint pos,
                                                          ZF_IN_OPT const zfchar *errorHint = zfnull);

#define _ZFP_ZFMethodGenericInvoke_REPEAT1(N) \
        zfautoObject _p##N; \
        typedef typename zftTraits<Type##N>::TrNoRef _Type##N; \
        if(!ZFPropertyTypeIdData<_Type##N>::ValueStore(_p##N, param##N)) \
        { \
            _ZFP_ZFMethodGenericInvokeError(method, obj, N); \
        }
#define _ZFP_ZFMethodGenericInvoke_REPEAT2(N) _p##N
#define _ZFP_ZFMethodGenericInvoke_REPEAT3(N) \
        _ZFP_MtdGII_P<Type##N>::p(param##N, _p##N);

#define _ZFP_ZFMethodGenericInvoke_DECLARE(N) \
    template<typename T_ReturnType ZFM_REPEAT(N, ZFM_REPEAT_TEMPLATE, ZFM_COMMA, ZFM_COMMA)> \
    T_ReturnType _ZFP_MtdGII(ZF_IN const ZFMethod *method, ZF_IN ZFObject *obj ZFM_REPEAT(N, ZFM_REPEAT_PARAM, ZFM_COMMA, ZFM_COMMA)) \
    { \
        ZFM_REPEAT(N, _ZFP_ZFMethodGenericInvoke_REPEAT1, ZFM_EMPTY, ZFM_EMPTY) \
        zfautoObject _ret; \
        zfstring errorHint; \
        if(!ZFMethodGenericInvoke(method, obj, &errorHint, _ret ZFM_REPEAT(N, _ZFP_ZFMethodGenericInvoke_REPEAT2, ZFM_COMMA, ZFM_COMMA))) \
        { \
            _ZFP_ZFMethodGenericInvokeError(method, obj, -1, errorHint); \
        } \
        ZFM_REPEAT(N, _ZFP_ZFMethodGenericInvoke_REPEAT3, ZFM_EMPTY, ZFM_EMPTY) \
        return _ZFP_MtdGII_R<T_ReturnType>::r(method, obj, _ret); \
    }

template<typename T_ParamType, typename T_Fix = void>
zfclassNotPOD _ZFP_MtdGII_P
{
public:
    static inline void p(ZF_IN_OUT T_ParamType p, ZF_IN const zfautoObject &h)
    {
    }
};
template<typename T_ParamType>
zfclassNotPOD _ZFP_MtdGII_P<T_ParamType const &>
{
public:
    static inline void p(ZF_IN_OUT T_ParamType const &p, ZF_IN const zfautoObject &h)
    {
    }
};
template<typename T_ParamType>
zfclassNotPOD _ZFP_MtdGII_P<const T_ParamType *>
{
public:
    static inline void p(ZF_IN_OUT const T_ParamType *p, ZF_IN const zfautoObject &h)
    {
    }
};
template<typename T_ParamType>
zfclassNotPOD _ZFP_MtdGII_P<T_ParamType &>
{
public:
    static void p(ZF_IN_OUT T_ParamType &p, ZF_IN const zfautoObject &h)
    {
        typedef typename zftTraits<T_ParamType &>::TrNoRef _Type;
        p = ZFPropertyTypeIdData<_Type>::template Value<T_ParamType &>::access(h);
    }
};
template<typename T_ParamType>
zfclassNotPOD _ZFP_MtdGII_P<T_ParamType *, zftEnableIf<!zftIsZFObject(T_ParamType)>>
{
public:
    static void p(ZF_IN_OUT T_ParamType *p, ZF_IN const zfautoObject &h)
    {
        typedef typename zftTraits<T_ParamType *>::TrNoRef _Type;
        if(p)
        {
            *p = ZFPropertyTypeIdData<_Type>::template Value<T_ParamType const &>::access(h);
        }
    }
};

template<typename T_ReturnType>
zfclassNotPOD _ZFP_MtdGII_R
{
public:
    static T_ReturnType r(ZF_IN const ZFMethod *method, ZF_IN ZFObject *obj, ZF_IN const zfautoObject &ret)
    {
        typedef typename zftTraits<T_ReturnType>::TrNoRef _T_ReturnType;
        if(!ZFPropertyTypeIdData<_T_ReturnType>::template Value<T_ReturnType>::accessAvailable(ret))
        {
            _ZFP_ZFMethodGenericInvokeError(method, obj, -2);
        }
        return ZFPropertyTypeIdData<_T_ReturnType>::template Value<T_ReturnType>::access(ret);
    }
};
template<>
zfclassNotPOD _ZFP_MtdGII_R<void>
{
public:
    static void r(ZF_IN const ZFMethod *method, ZF_IN ZFObject *obj, ZF_IN const zfautoObject &ret)
    {
    }
};

_ZFP_ZFMethodGenericInvoke_DECLARE(0)
_ZFP_ZFMethodGenericInvoke_DECLARE(1)
_ZFP_ZFMethodGenericInvoke_DECLARE(2)
_ZFP_ZFMethodGenericInvoke_DECLARE(3)
_ZFP_ZFMethodGenericInvoke_DECLARE(4)
_ZFP_ZFMethodGenericInvoke_DECLARE(5)
_ZFP_ZFMethodGenericInvoke_DECLARE(6)
_ZFP_ZFMethodGenericInvoke_DECLARE(7)
_ZFP_ZFMethodGenericInvoke_DECLARE(8)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFMethodGenericInvoker_h_

