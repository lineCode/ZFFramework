WORK_DIR=$(cd "$(dirname "$0")"; pwd)
PROJ_PATH=$1
if test "x-$PROJ_PATH" = "x-" ; then
    echo "usage:"
    echo "  unique_proj.sh PROJ_PATH"
    exit 1
fi

ZF_ROOT_PATH=$WORK_DIR/../../..
ZF_TOOLS_PATH=$ZF_ROOT_PATH/tools

sh "$ZF_TOOLS_PATH/common/zfsh.sh" "$WORK_DIR/zf3rd/zfautoscript_zf3rd_setup_xUnique.zfsh"
rm -rf "$PROJ_PATH.zftmp" >/dev/null 2>&1
cp -r "$PROJ_PATH" "$PROJ_PATH.zftmp" >/dev/null 2>&1
python "$WORK_DIR/zf3rd/_repo/xUnique/xUnique.py" "$PROJ_PATH.zftmp"

if test -e "$PROJ_PATH.zftmp/project.pbxproj.ubak" ; then
    mv "$PROJ_PATH.zftmp/project.pbxproj.ubak" "$PROJ_PATH.zftmp/project.pbxproj"
fi

if test -e "$PROJ_PATH.zftmp/project.pbxproj.sbak" ; then
    mv "$PROJ_PATH.zftmp/project.pbxproj.sbak" "$PROJ_PATH.zftmp/project.pbxproj"
fi

rsync -ruc "$PROJ_PATH.zftmp/project.pbxproj" "$PROJ_PATH/project.pbxproj" >/dev/null 2>&1

rm -rf "$PROJ_PATH.zftmp" >/dev/null 2>&1

exit 0

