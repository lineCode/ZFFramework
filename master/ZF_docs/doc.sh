WORK_DIR=$(cd "$(dirname "$0")"; pwd)
ZF_ROOT_PATH=$WORK_DIR/../..
ZF_TOOLS_PATH=$ZF_ROOT_PATH/tools

if test "x-$ZFDOC_KEY" = "x-" ; then
    echo "ZFDOC_KEY not set"
    exit 1
fi

OUTPUT_PATH="$WORK_DIR/../../_tmp/DoxygenDoc"
mkdir -p "$OUTPUT_PATH"

_OLD_DIR=$(pwd)
cd "$WORK_DIR/Doxygen"
doxygen
if ! test "$?" = "0" ; then
    exit 1
fi
cd "$_OLD_DIR"

rm -rf "$OUTPUT_PATH/html/index.hhc"
rm -rf "$OUTPUT_PATH/html/index.hhk"
rm -rf "$OUTPUT_PATH/html/index.hhp"

DOC_REPO_PATH="$OUTPUT_PATH/../DoxygenDoc_repo"
mkdir -p "$DOC_REPO_PATH"

sh "$ZF_TOOLS_PATH/common/git_check.sh" "https://github.com/ZFFramework/zfframework.github.com" master "$DOC_REPO_PATH"

_OLD_DIR=$(pwd)
cd "$DOC_REPO_PATH"
git config user.email "z@zsaber.com"
git config user.name "ZSaberLv0"
git config push.default "simple"
git checkout .
git reset --hard
git clean -xdf
git filter-branch --force --index-filter 'git rm --cached --ignore-unmatch *' --prune-empty --tag-name-filter cat -- --all
cd "$_OLD_DIR"

cp -r "$WORK_DIR/doc_repo/." "$DOC_REPO_PATH/"
rm -rf "$DOC_REPO_PATH/doc"
mkdir "$DOC_REPO_PATH/doc"
cp -r "$OUTPUT_PATH/html/." "$DOC_REPO_PATH/doc/"

_OLD_DIR=$(pwd)
cd "$DOC_REPO_PATH"
git add -A
git commit -a -m "update doc"
git push --force "https://ZSaberLv0:$ZFDOC_KEY@github.com/ZFFramework/zfframework.github.com"
cd "$_OLD_DIR"

