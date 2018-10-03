/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFCallerInfo.h"

ZF_NAMESPACE_GLOBAL_BEGIN

void _ZFP_ZF_CALLER_FILE_TO_NAME(ZF_IN_OUT zfstring &ret,
                                 ZF_IN const zfchar *filePath)
{
    if(filePath == zfnull)
    {
        return ;
    }
    const zfchar *p = filePath + zfslen(filePath);
    while(p > filePath)
    {
        --p;
        if(*p == '/' || *p == '\\')
        {
            ++p;
            break;
        }
    }
    ret += p;
}
zfstring _ZFP_ZF_CALLER_FILE_TO_NAME(ZF_IN const zfchar *filePath)
{
    zfstring ret;
    _ZFP_ZF_CALLER_FILE_TO_NAME(ret, filePath);
    return ret;
}

const ZFCallerInfo &_ZFP_ZFCallerInfoEmpty(void)
{
    static ZFCallerInfo d;
    return d;
}

ZF_NAMESPACE_GLOBAL_END

