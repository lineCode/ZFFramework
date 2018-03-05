@echo off
setlocal

set WORK_DIR=%~dp0
set DST_PATH=%~1%
set FILE1_PATH=%~2%

if not defined DST_PATH goto :usage
if not defined FILE1_PATH goto :usage
goto :run
:usage
echo usage:
echo   file_merge.bat DST_PATH FILE1_PATH [FILE2_PATH ...]
exit /b 1
:run

more "%FILE1_PATH%" > "%DST_PATH%.tmp"

shift
:all_file
shift
set FILEN_PATH=%~1%
if defined FILEN_PATH (
    more "%FILEN_PATH%" >> "%DST_PATH%.tmp"
    goto :all_file
)

move /y "%DST_PATH%.tmp" "%DST_PATH%" >nul 2>&1

