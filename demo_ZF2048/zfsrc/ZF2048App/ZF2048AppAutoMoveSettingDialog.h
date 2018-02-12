/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#ifndef _ZFI_ZF2048AppAutoMoveSettingDialog_h_
#define _ZFI_ZF2048AppAutoMoveSettingDialog_h_

#include "ZF2048AppDialog.h"
#include "ZF2048AppAutoMoveAction.h"
ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd _ZFP_ZF2048AppAutoMoveSettingDialogPrivate;
zfclass ZF2048AppAutoMoveSettingDialog : zfextends ZF2048AppDialog
{
    ZFOBJECT_DECLARE(ZF2048AppAutoMoveSettingDialog, ZF2048AppDialog)

    ZFOBSERVER_EVENT(AutoMoveSettingOnChange)

public:
    ZFCoreArrayPOD<ZF2048AppAutoMoveActionEnum> autoMoves;

protected:
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);

    zfoverride
    virtual void autoMoveSettingOnChange(void)
    {
        this->observerNotify(zfself::EventAutoMoveSettingOnChange());
    }

private:
    _ZFP_ZF2048AppAutoMoveSettingDialogPrivate *d;
    friend zfclassFwd _ZFP_ZF2048AppAutoMoveSettingDialogPrivate;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZF2048AppAutoMoveSettingDialog_h_

