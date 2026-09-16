@echo off
rem compile.cmd -- Build ZeroPoint for ArcaOS using GCC + SDL2
rem Usage: compile.cmd  (run from this directory on ArcaOS)

set LOGFILE=compile.cmd.log

rem -- EMXOMFLD / wlink linker setup
set EMXOMFLD_TYPE=WLINK
set EMXOMFLD_LINKER=wl.exe
set EMXOMFLD_PRELINK=0

rem -- Create output directory
if not exist bin md bin

echo === ZeroPoint SDL2 Build === | tee -a %LOGFILE%
echo. | tee -a %LOGFILE%

make -f makefile.gcc 2>&1 | tee -a %LOGFILE%

echo. | tee -a %LOGFILE%
echo Done. See %LOGFILE% for full output. | tee -a %LOGFILE%
