WORK_DIR=$(cd "$(dirname "$0")"; pwd)
iOS_SRCROOT=$1
if test "x-$iOS_SRCROOT" = "x-" ; then
    echo usage:
    echo   ZFCompileSource.sh SRCROOT
    exit 1
fi

ZF_ROOT_PATH=${iOS_SRCROOT}/../../../../../ZFFramework
ZF_TOOLS_PATH=$ZF_ROOT_PATH/tools
COMPILE_DST_PATH=${iOS_SRCROOT}/ProjectFiles

sh $ZF_TOOLS_PATH/common/unity_build.sh "${COMPILE_DST_PATH}/zfgensrc_ZFFramework_test.mm" "${iOS_SRCROOT}/../../../zfsrc"

