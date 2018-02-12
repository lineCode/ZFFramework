WORK_DIR=$(cd "$(dirname "$0")"; pwd)
PROJ_NAME=$1
PROJ_PATH=$2
BUILD_TARGET_ONLY=$3
if test "x-$PROJ_NAME" = "x-" || test "x-$PROJ_PATH" = "x-" ; then
    echo usage:
    echo   testbuild_iOS.sh PROJ_NAME PROJ_PATH [BUILD_TARGET_ONLY]
    exit 1
fi

ZF_ROOT_PATH=$WORK_DIR/../..
ZF_TOOLS_PATH=$ZF_ROOT_PATH/tools

if test "x-$BUILD_TARGET_ONLY" = "x-" ; then
    chmod +x $WORK_DIR/release_iOS_check.sh
    $WORK_DIR/release_iOS_check.sh
fi

_OLD_DIR=$(pwd)
cd "$PROJ_PATH"
xcodebuild -showsdks | grep iphonesimulator | cut -f3 | xargs xcodebuild build -project $PROJ_NAME.xcodeproj -scheme $PROJ_NAME CODE_SIGNING_REQUIRED=NO
_RESULT="$?"
cd "$_OLD_DIR"

exit $_RESULT

