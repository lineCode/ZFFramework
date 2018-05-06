WORK_DIR=$(cd "$(dirname "$0")"; pwd)
CONFIG_FILE_PATH=$1
DST_PATH=$2
if test "x-$CONFIG_FILE_PATH" = "x-" ; then
    echo "usage:"
    echo "  zfproj_creator.sh CONFIG_FILE_PATH [ZF_OUTPUT]"
    echo ""
    echo "config file format:"
    echo "  these are required:"
    echo "    ZF_NAME = YourProjName"
    echo "    ZF_TYPE = app/lib/impl"
    echo "    ZF_LIB += ZFCore"
    echo "    ZF_LIB += ZFAlgorithm"
    echo "    ZF_IMPL += ZFCore"
    echo "    ZF_EXT += https://github.com/ZFFramework/ZFModuleDemo_lib ZFModuleDemo_lib master"
    echo "  these are optional: (ignored when ZF_OUTPUT specified in command line)"
    echo "    ZF_OUTPUT += output_path"
    echo "    ZF_INPLACE += whether_update_inplace"
    echo "    ZF_INPLACE_SRC += relative_path_in_template_output"
    echo "  these are optional:"
    echo "    ZF_EXCLUDE += file_or_dirs_to_exclude"
    echo ""
    echo "when ZF_INPLACE specified,"
    echo "  we would update project files inplace,"
    echo "  ignoring source/resource/icon files"
    echo "  ZF_INPLACE_SRC can be specified to update sub dirs only,"
    echo "  at this case, the ZF_OUTPUT must be specified properly"
    echo "  the output dir would like:"
    echo "    ~/    <= ZF_OUTPUT"
    echo "        YourProjName/"
    echo "            YourProjName/"
    echo "                zfproj/"
    echo "        ZFModule/"
    echo "            ZF/"
    echo "                YourProjName/"
    echo "                    zfproj/"
    exit 1
fi
if ! test -e "$CONFIG_FILE_PATH" ; then
    echo "config file not exist: $CONFIG_FILE_PATH"
    exit 1
fi

# ============================================================
ZF_ROOT_PATH=$WORK_DIR/../../../ZFFramework

# state
_IS_MAC=`uname -a | grep -i -o "darwin"`
if ! test "x-$_IS_MAC" = "x-" ; then
    _IS_MAC=1
else
    _IS_MAC=
fi

# format and read config file
if ! test -e "$CONFIG_FILE_PATH" ; then
    echo "config file not exist: $CONFIG_FILE_PATH"
    exit 1
fi
_CONFIG_FILE_PATH=$ZF_ROOT_PATH/_tmp/zfproj_creator.tmp
mkdir -p "${_CONFIG_FILE_PATH%[/\\]*}" >/dev/null 2>&1
cp "$CONFIG_FILE_PATH" "$_CONFIG_FILE_PATH"
cat "$_CONFIG_FILE_PATH" \
    | sed -E 's#^ +##g' \
    | sed -E 's# +$##g' \
    | sed -E 's# +([\+=]+)#\1#g' \
    | sed -E 's#([\+=]+) +#\1#g' \
    | sed -E 's# +#\|#g' \
    | sed -E 's#^([a-zA-Z_0-9]+)\+=(.*)$#\1=\"\$\1 \2\"#g' \
      >"$_CONFIG_FILE_PATH"
source "$_CONFIG_FILE_PATH"
rm "$_CONFIG_FILE_PATH" >/dev/null 2>&1

echo "============================================================"
echo "generating project from config: $CONFIG_FILE_PATH"
echo "============================================================"

# parse config to text template params
if test "x-$ZF_NAME" = "x-" ; then
    echo "ZF_NAME not set"
    exit 1
fi
export ZFTT_R_proj_name=$ZF_NAME

if ! test "x-$DST_PATH" = "x-" ; then
    export ZF_INPLACE=
    export ZF_INPLACE_SRC=
else
    DST_PATH=$ZF_OUTPUT
fi
if test "x-$DST_PATH" = "x-" ; then
    echo "ZF_OUTPUT not set"
    exit 1
fi
_OLD_DIR=$(pwd)
cd "${CONFIG_FILE_PATH%[/\\]*}"
mkdir -p "$DST_PATH" >/dev/null 2>&1
cd "$DST_PATH"
DST_PATH=$(pwd)
cd "$_OLD_DIR"

if test "x-$ZF_TYPE" = "x-app" ; then
    export ZFTT_C_app_proj=1
elif test "x-$ZF_TYPE" = "x-lib" ; then
    export ZFTT_C_lib_proj=1
elif test "x-$ZF_TYPE" = "x-impl" ; then
    export ZFTT_C_impl_proj=1
else
    echo "ZF_TYPE not set"
    exit 1
fi

libIndex=0
for lib in $ZF_LIB ; do
    export ZFTT_C_lib_require_${libIndex}=1
    export ZFTT_R_lib_name_${libIndex}=$lib
    libIndex=$((libIndex+1))
done
implIndex=0
for impl in $ZF_IMPL ; do
    export ZFTT_C_impl_require_${implIndex}=1
    export ZFTT_R_impl_name_${implIndex}=$impl
    implIndex=$((implIndex+1))

    _HAS_UIKit=`echo $impl | grep "UI"`
    if ! test "x-$_HAS_UIKit" = "x-" ; then
        export ZFTT_C_needUIKit=1
    fi
    _HAS_UIWebKit=`echo $impl | grep "UIWeb"`
    if ! test "x-$_HAS_UIWebKit" = "x-" ; then
        export ZFTT_C_needUIKit=1
        export ZFTT_C_needUIWebKit=1
    fi
done
extIndex=0
for ext in $ZF_EXT ; do
    ext=`echo "$ext" | sed -E 's#\|# #g'`
    export ZFTT_C_ext_require_${extIndex}=1
    export ZFTT_R_ext_git_${extIndex}="$ext"
    extIndex=$((extIndex+1))
done
if test "x-$ZF_TYPE" = "x-impl" ; then
    if test "x-$ZF_NAME" = "x-ZF_impl" ; then
        export ZFTT_C_needUIKit=1
    fi
    _HAS_UIKit=`echo $ZF_NAME | grep "UI"`
    if ! test "x-$_HAS_UIKit" = "x-" ; then
        export ZFTT_C_needUIKit=1
    fi
    _HAS_UIWebKit=`echo $ZF_NAME | grep "UIWeb"`
    if ! test "x-$_HAS_UIWebKit" = "x-" ; then
        export ZFTT_C_needUIKit=1
        export ZFTT_C_needUIWebKit=1
    fi
fi
if test 1 = 1 ; then
    echo "configs:"
    echo "    ZFTT_C_app_proj=$ZFTT_C_app_proj"
    echo "    ZFTT_C_lib_proj=$ZFTT_C_lib_proj"
    echo "    ZFTT_C_impl_proj=$ZFTT_C_impl_proj"
    echo "    ZFTT_R_proj_name=$ZFTT_R_proj_name"
    echo "    ZFTT_R_proj_name=$ZFTT_R_proj_name"
    echo "    ZFTT_C_lib_require_0=$ZFTT_C_lib_require_0 \t ZFTT_R_lib_name_0=$ZFTT_R_lib_name_0"
    echo "    ZFTT_C_lib_require_1=$ZFTT_C_lib_require_1 \t ZFTT_R_lib_name_1=$ZFTT_R_lib_name_1"
    echo "    ZFTT_C_lib_require_2=$ZFTT_C_lib_require_2 \t ZFTT_R_lib_name_2=$ZFTT_R_lib_name_2"
    echo "    ZFTT_C_lib_require_3=$ZFTT_C_lib_require_3 \t ZFTT_R_lib_name_3=$ZFTT_R_lib_name_3"
    echo "    ZFTT_C_lib_require_4=$ZFTT_C_lib_require_4 \t ZFTT_R_lib_name_4=$ZFTT_R_lib_name_4"
    echo "    ZFTT_C_lib_require_5=$ZFTT_C_lib_require_5 \t ZFTT_R_lib_name_5=$ZFTT_R_lib_name_5"
    echo "    ZFTT_C_lib_require_6=$ZFTT_C_lib_require_6 \t ZFTT_R_lib_name_6=$ZFTT_R_lib_name_6"
    echo "    ZFTT_C_lib_require_7=$ZFTT_C_lib_require_7 \t ZFTT_R_lib_name_7=$ZFTT_R_lib_name_7"
    echo "    ..."
    echo "    ZFTT_C_impl_require_0=$ZFTT_C_impl_require_0 \t ZFTT_R_impl_name_0=$ZFTT_R_impl_name_0"
    echo "    ZFTT_C_impl_require_1=$ZFTT_C_impl_require_1 \t ZFTT_R_impl_name_1=$ZFTT_R_impl_name_1"
    echo "    ZFTT_C_impl_require_2=$ZFTT_C_impl_require_2 \t ZFTT_R_impl_name_2=$ZFTT_R_impl_name_2"
    echo "    ZFTT_C_impl_require_3=$ZFTT_C_impl_require_3 \t ZFTT_R_impl_name_3=$ZFTT_R_impl_name_3"
    echo "    ZFTT_C_impl_require_4=$ZFTT_C_impl_require_4 \t ZFTT_R_impl_name_4=$ZFTT_R_impl_name_4"
    echo "    ZFTT_C_impl_require_5=$ZFTT_C_impl_require_5 \t ZFTT_R_impl_name_5=$ZFTT_R_impl_name_5"
    echo "    ZFTT_C_impl_require_6=$ZFTT_C_impl_require_6 \t ZFTT_R_impl_name_6=$ZFTT_R_impl_name_6"
    echo "    ZFTT_C_impl_require_7=$ZFTT_C_impl_require_7 \t ZFTT_R_impl_name_7=$ZFTT_R_impl_name_7"
    echo "    ..."
    echo "    ZFTT_C_ext_require_0=$ZFTT_C_ext_require_0 \t ZFTT_R_ext_git_0=$ZFTT_R_ext_git_0"
    echo "    ZFTT_C_ext_require_1=$ZFTT_C_ext_require_1 \t ZFTT_R_ext_git_1=$ZFTT_R_ext_git_1"
    echo "    ZFTT_C_ext_require_2=$ZFTT_C_ext_require_2 \t ZFTT_R_ext_git_2=$ZFTT_R_ext_git_2"
    echo "    ZFTT_C_ext_require_3=$ZFTT_C_ext_require_3 \t ZFTT_R_ext_git_3=$ZFTT_R_ext_git_3"
    echo "    ZFTT_C_ext_require_4=$ZFTT_C_ext_require_4 \t ZFTT_R_ext_git_4=$ZFTT_R_ext_git_4"
    echo "    ZFTT_C_ext_require_5=$ZFTT_C_ext_require_5 \t ZFTT_R_ext_git_5=$ZFTT_R_ext_git_5"
    echo "    ZFTT_C_ext_require_6=$ZFTT_C_ext_require_6 \t ZFTT_R_ext_git_6=$ZFTT_R_ext_git_6"
    echo "    ZFTT_C_ext_require_7=$ZFTT_C_ext_require_7 \t ZFTT_R_ext_git_7=$ZFTT_R_ext_git_7"
    echo "    ..."
    echo "    ZFTT_C_needUIKit=$ZFTT_C_needUIKit"
    echo "    ZFTT_C_needUIWebKit=$ZFTT_C_needUIWebKit"
fi

# tmp dir
echo "copying files..."
_TMP_DIR=$ZF_ROOT_PATH/_tmp/_zfproj_tmp_$ZF_NAME
rm -rf "$_TMP_DIR" >/dev/null 2>&1
if test "x-$ZF_TYPE" = "x-app" ; then
    _TMP_DIR_SRC="$WORK_DIR/private/app/."
    _TMP_DIR_DST="$_TMP_DIR/$ZFTT_R_proj_name/"
elif test "x-$ZF_TYPE" = "x-lib" ; then
    _TMP_DIR_SRC="$WORK_DIR/private/lib/."
    _TMP_DIR_DST="$_TMP_DIR/ZFModule/ZF/$ZFTT_R_proj_name/"
elif test "x-$ZF_TYPE" = "x-impl" ; then
    _TMP_DIR_SRC="$WORK_DIR/private/lib/."
    _TMP_DIR_DST="$_TMP_DIR/ZFModule/ZF/$ZFTT_R_proj_name/"
fi
mkdir -p "$_TMP_DIR_DST" >/dev/null 2>&1
_SYNC_EXCLUDE=
if ! test "x-$_IS_MAC" = "x-1" ; then
    _SYNC_EXCLUDE="$_SYNC_EXCLUDE --exclude=\"project.pbxproj\""
fi
for i in "$ZF_EXCLUDE" ; do
    _SYNC_EXCLUDE="$_SYNC_EXCLUDE --exclude=\"$i\""
done
if ! test "x-$ZF_INPLACE" = "x-" ; then
    _SYNC_EXCLUDE="$_SYNC_EXCLUDE --exclude=\"zfsrc\""
    _SYNC_EXCLUDE="$_SYNC_EXCLUDE --exclude=\"zfres\""
    _SYNC_EXCLUDE="$_SYNC_EXCLUDE --exclude=\"*.png\""
    _SYNC_EXCLUDE="$_SYNC_EXCLUDE --exclude=\"*.icns\""
    _SYNC_EXCLUDE="$_SYNC_EXCLUDE --exclude=\"*.ico\""
fi
eval "rsync $_SYNC_EXCLUDE -r \"$_TMP_DIR_SRC\" \"$_TMP_DIR_DST\" >/dev/null 2>&1"

# modify file names
echo "modifying file names..."
while ((1)) ; do
    changed=0
    # condition
    for f in `find $_TMP_DIR -name "*\{ZFTT_C_*"` ; do
        if ! test -e "$f" ; then
            continue
        fi
        fileName=${f##*[/\\]}
        fileNameNew=$fileName
        while ((1)) ; do
            condName=`echo "$fileNameNew" | sed -E 's#.*\{(ZFTT_C_[a-zA-Z_0-9]+)\}.*\{\ZFTT_CE\}.*#\1#g'`
            if test "x-$condName" = "x-" || test "x-$condName" = "x-$fileNameNew" ; then
                break
            fi
            cond="echo \$$condName"
            cond=`eval $cond`
            if test "x-$cond" = "x-1" ; then
                fileNameNew=`echo "$fileNameNew" | sed -E "s#(.*)\{$condName\}(.*)\{ZFTT_CE\}(.*)#\1\2\3#"`
            else
                fileNameNew=`echo "$fileNameNew" | sed -E "s#(.*)\{$condName\}(.*)\{ZFTT_CE\}(.*)#\1\3#"`
            fi
        done
        if ! test "x-$fileName" = "x-$fileNameNew" ; then
            changed=1
            if test "x-$fileNameNew" = "x-" ; then
                rm -rf "${f%[/\\]*}/$fileName" >/dev/null 2>&1
            else
                mv "${f%[/\\]*}/$fileName" "${f%[/\\]*}/$fileNameNew" >/dev/null 2>&1
            fi
        fi
    done
    # replace
    for f in `find $_TMP_DIR -name "*\{ZFTT_R_*"` ; do
        if ! test -e "$f" ; then
            continue
        fi
        fileName=${f##*[/\\]}
        fileNameNew=$fileName
        while ((1)) ; do
            condName=`echo "$fileNameNew" | sed -E 's#.*\{(ZFTT_R_[a-zA-Z_0-9]+)\}.*#\1#g'`
            if test "x-$condName" = "x-" || test "x-$condName" = "x-$fileNameNew" ; then
                break
            fi
            cond="echo \$$condName"
            cond=`eval $cond`
            fileNameNew=`echo "$fileNameNew" | sed -E "s#(.*)\{$condName\}(.*)#\1$cond\2#"`
        done
        if ! test "x-$fileName" = "x-$fileNameNew" ; then
            changed=1
            mv "${f%[/\\]*}/$fileName" "${f%[/\\]*}/$fileNameNew" >/dev/null 2>&1
        fi
    done
    # check
    if ! test "x-$changed" = "x-1" ; then
        break
    fi
done

# modify file contents
echo "processing file contents..."
while ((1)) ; do
    changed=0
    for f in `find $_TMP_DIR -type f` ; do
        fileName=${f##*[/\\]}
        # condition
        while ((1)) ; do
            exist=`grep -o "\{ZFTT_C_[a-zA-Z_0-9]\+\}" "$f" | uniq`
            if test "x-$exist" = "x-" ; then
                break
            fi
            condNameList=`echo "$exist" | sed -E 's#.*\{(ZFTT_C_[a-zA-Z_0-9]+)\}.*#\1#g'`
            for condName in $condNameList ; do
                printf "\r%s    processing: $fileName\t$condName" $(tput el)
                cond="echo \$$condName"
                cond=`eval $cond`
                if test "x-$cond" = "x-1" ; then
                    sedPattern="s#\{$condName\}([^%]*)%#\1#g"
                else
                    sedPattern="s#\{$condName\}([^%]*)%##g"
                fi
                cat "$f" \
                    | sed -E "s#@#__zfproj_esc1__#g" \
                    | tr "\n" "@" \
                    | sed -E "s#%#__zfproj_esc2__#g" \
                    | sed -E "s#\{ZFTT_CE\}#%#g" \
                    | sed -E "$sedPattern" \
                    | sed -E "s#%#\{ZFTT_CE\}#g" \
                    | tr -d "\n" \
                    | tr "@" "\n" \
                    | sed -E "s#__zfproj_esc1__#@#g" \
                    | sed -E "s#__zfproj_esc2__#%#g" \
                      >"$f.tmp"
                mv "$f.tmp" "$f"
            done
        done
        # replace
        while ((1)) ; do
            exist=`grep -o "\{ZFTT_R_[a-zA-Z_0-9]\+\}" "$f" | uniq`
            if test "x-$exist" = "x-" ; then
                break
            fi
            condNameList=`echo "$exist" | sed -E 's#.*\{(ZFTT_R_[a-zA-Z_0-9]+)\}.*#\1#g'`
            for condName in $condNameList ; do
                printf "\r%s    processing: $fileName\t$condName" $(tput el)
                cond="echo \$$condName"
                cond=`eval $cond`
                cat "$f" \
                    | sed -E "s#\{$condName\}#$cond#g" \
                      >"$f.tmp"
                mv "$f.tmp" "$f"
            done
        done
    done
    # check
    if ! test "x-$changed" = "x-1" ; then
        break
    fi
done
printf "\r%s" $(tput el)

# xUnique if necessary
_IS_MAC=`uname -a | grep -i -o "darwin"`
if ! test "x-$_IS_MAC" = "x-" ; then
    echo "unique iOS project..."
    sh "$ZF_ROOT_PATH/tools/spec/iOS/unique_proj_recursive.sh" "$_TMP_DIR"
fi

# finally sync to dst
echo "sync to target"
eval "rsync $_SYNC_EXCLUDE -ruc \"$_TMP_DIR/$ZF_INPLACE_SRC/\" \"$DST_PATH/\" >/dev/null 2>&1"

rm -rf "$_TMP_DIR" >/dev/null 2>&1
echo "done"

