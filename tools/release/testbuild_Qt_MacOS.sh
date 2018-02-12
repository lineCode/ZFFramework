WORK_DIR=$(cd "$(dirname "$0")"; pwd)
PROJ_NAME=$1
PROJ_PATH=$2
BUILD_TARGET_ONLY=$3
if test "x-$PROJ_NAME" = "x-" || test "x-$PROJ_PATH" = "x-" ; then
    echo usage:
    echo   testbuild_Qt_MacOS.sh PROJ_NAME PROJ_PATH [BUILD_TARGET_ONLY]
    exit 1
fi

ZF_ROOT_PATH=$WORK_DIR/../..
ZF_TOOLS_PATH=$ZF_ROOT_PATH/tools
_TMP_PATH=$ZF_ROOT_PATH/_tmp/$PROJ_NAME/Qt_MacOS/release

if test "x-$BUILD_TARGET_ONLY" = "x-" ; then
    chmod +x $WORK_DIR/release_Qt_MacOS_check.sh
    $WORK_DIR/release_Qt_MacOS_check.sh
fi

_OLD_DIR=$(pwd)
cd "$PROJ_PATH"
_PROJ_PATH=$(pwd)
mkdir -p "$_TMP_PATH" >/dev/null 2>&1
cd "$_TMP_PATH"
qmake "$_PROJ_PATH/$PROJ_NAME.pro" CONFIG+=release
make -j2
_RESULT="$?"
cd "$_OLD_DIR"

exit $_RESULT

