WORK_DIR=$(cd "$(dirname "$0")"; pwd)

ZF_ROOT_PATH=$WORK_DIR/../..
ZF_TOOLS_PATH=$ZF_ROOT_PATH/tools

_OLD_DIR=$(pwd)
cd "$ZF_ROOT_PATH"
pod lib lint --allow-warnings --use-libraries --skip-import-validation
_RESULT="$?"
cd "$_OLD_DIR"

exit $_RESULT

