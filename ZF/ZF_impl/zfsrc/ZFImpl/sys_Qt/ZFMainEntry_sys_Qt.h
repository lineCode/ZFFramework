/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#ifndef _ZFI_ZFMainEntry_sys_Qt_h_
#define _ZFI_ZFMainEntry_sys_Qt_h_

#include "ZFCore.h"
#include "ZFImpl_sys_Qt_ZF_impl.h"

#if ZF_ENV_sys_Qt

#include <QWidget>
#include <QLibrary>

zfclassNotPOD ZF_ENV_EXPORT ZFImpl_sys_Qt_Window : public QWidget
{
    Q_OBJECT

public:
    ZFImpl_sys_Qt_Window(void);
};

ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief main entry for Qt
 *
 * main entry steps:
 * -# load libraries by ZFImpl_sys_Qt_requireLib
 * -# call ZFMainEntry_sys_Qt and return
 *
 * @code
 *   int main(int argc, char **argv)
 *   {
 *       // load your lib
 *       ZFImpl_sys_Qt_requireLib(YourLibName)
 *
 *       // common entry, setup root window and start ZF
 *       return ZFMainEntry_sys_Qt(argc, argv);
 *   }
 * @endcode
 * \n
 * if you want to attach to your existing UI:
 * @code
 *   int main(int argc, char **argv)
 *   {
 *       // your own app and window
 *       QApplication app(argc, argv);
 *       QWidget w;
 *       w.show();
 *       w.setLayout(new YourWindowLayout());
 *
 *       // load your lib
 *       ZFImpl_sys_Qt_requireLib(YourLibName)
 *
 *       // attach
 *       // note, during attach, a root view would be added to the rootWindow,
 *       // by QWidget::layout::addWidget,
 *       // you must ensure:
 *       // -  the QWidget::layout has been set properly
 *       // -  the root view would be layouted properly (typically to match full size of the window)
 *       int ret = ZFMainEntry_sys_Qt_attach(&w, argc, argv);
 *       if(ret != 0)
 *       {
 *           return ret;
 *       }
 *       // app exe
 *       ret = app.exec();
 *       // detach
 *       ZFMainEntry_sys_Qt_detach();
 *       // return
 *       return ret;
 *   }
 * @endcode
 */
extern ZF_ENV_EXPORT int ZFMainEntry_sys_Qt(int argc, char **argv);
#define ZFImpl_sys_Qt_requireLib(libName) \
    QLibrary _lib##libName(#libName); \
    _lib##libName.load();

extern ZF_ENV_EXPORT QWidget *ZFImpl_sys_Qt_rootWindow(void);

extern ZF_ENV_EXPORT int ZFMainEntry_sys_Qt_attach(ZF_IN QWidget *rootWindow,
                                                   ZF_IN_OPT int argc = 0,
                                                   ZF_IN_OPT char **argv = NULL);
extern ZF_ENV_EXPORT void ZFMainEntry_sys_Qt_detach(void);

ZF_NAMESPACE_GLOBAL_END
#endif
#endif // #ifndef _ZFI_ZFMainEntry_sys_Qt_h_

