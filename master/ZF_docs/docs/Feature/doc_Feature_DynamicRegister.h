/**
 * @page DocTag_Feature_DynamicRegister Dynamic class/method/property register
 *
 * unlike other C++ framework use static template specialization
 * to achieve reflection,
 * ZFFramework use fully dynamic way to achieve reflection,
 * this may cause less performance,
 * but are much more convenient for high level app coding\n
 * \n
 * example:
 * @code
 *   ZFLuaExecute(zfText(" \
 *       ZFDynamic() \
 *       .classBegin('MyClass', 'ZFUIView') \
 *           .property('zfstring', 'myProp') \
 *       .classEnd() \
 *       "));
 * @endcode
 *
 * as you can see, we are able to register class/method/property in script language\n
 * further more, the registered contents can be combined
 * with all other features in C++ world:
 * @code
 *   zfautoObject obj = ZFClass::newInstanceForName(zfText("MyClass"));
 *   obj->invoke(zfText("myPropSet"), zflineAlloc(v_zfstring, zfText("myValue")));
 *   ZFObjectToXml(ZFOutputForConsole(), obj);
 * @endcode
 */

