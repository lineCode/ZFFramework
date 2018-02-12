@echo off
setlocal

set WORK_DIR=%~dp0
set PROJ_NAME=%~1%
set PROJ_PATH=%~2%
set BUILD_TARGET_ONLY=%~3%

if not defined PROJ_NAME goto :usage
if not defined PROJ_PATH goto :usage
goto :run
:usage
echo usage:
echo   testbuild_Qt_MacOS.bat PROJ_NAME PROJ_PATH [BUILD_TARGET_ONLY]
exit /b 1
:run

set ZF_ROOT_PATH=%WORK_DIR%\..\..
set ZF_TOOLS_PATH=%ZF_ROOT_PATH%\tools
set _TMP_PATH=%ZF_ROOT_PATH%\_tmp\%PROJ_NAME%\Qt_Windows\release

if not defined BUILD_TARGET_ONLY call "%WORK_DIR%\release_Qt_Windows_check.bat"

set _OLD_DIR=%cd%
cd "%PROJ_PATH%"
set _PROJ_PATH=%cd%
mkdir "%_TMP_PATH%" >nul 2>&1
cd "%_TMP_PATH%"
qmake "%_PROJ_PATH%\%PROJ_NAME%.pro" CONFIG+=release
mingw32-make -j2
set _RESULT=%errorlevel%
cd "%_OLD_DIR%"

exit /b %_RESULT%

