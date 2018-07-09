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
 * @file ZFStyleLoad.h
 * @brief style load utility
 */

#ifndef _ZFI_ZFStyleLoad_h_
#define _ZFI_ZFStyleLoad_h_

#include "ZFFile.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief util to load multiple styles from directory
 *
 * pathInfo must points to a directory that contains styles,
 * example:
 * @code
 *   ~/
 *       MyStyle/
 *           button.xml
 *           label.json
 *           image.png
 *           image.xml
 * @endcode
 * \n
 * all styles are loaded by #ZFObjectIOLoad,
 * and the file path name without file ext would be used as styleKey for #ZFStyleSet
 * (`~/path1/path2/mybutton.xml` would result `path1/path2/mybutton` as styleKey),
 * if two item with same name exists,
 * the later one would override the first one
 * (but which one is later, is not ensured)\n
 * the filters can be supplied to ignore some files by file name,
 * by default, these files or dirs would be ignored:
 * -  files or dirs whose name start with '.' or '_' (e.g. '.ignored.xml')
 * -  files whose name contains '._.' (e.g. 'ignored._.xml')
 *
 * @note load new style would append or replace current one
 */
ZFMETHOD_FUNC_DECLARE_3(zfbool, ZFStyleLoad,
                        ZFMP_IN(const ZFPathInfo &, pathInfo),
                        ZFMP_IN_OPT(const ZFFilterForString *, fileNameFilter, zfnull),
                        ZFMP_IN_OPT(const ZFFilterForString *, dirNameFilter, zfnull))

/**
 * @brief load multiple style from serializableData
 *
 * data should contains:
 * @code
 *   <node>
 *       <Style1 name="styleKey1" ... />
 *       <Style2 name="styleKey2" ... />
 *   </node>
 * @endcode
 */
ZFMETHOD_FUNC_DECLARE_1(zfbool, ZFStyleLoad,
                        ZFMP_IN(const ZFSerializableData &, serializableData))

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFStyleLoad_h_

