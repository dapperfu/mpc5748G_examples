@ECHO OFF
ECHO ##### Bootstrapping MinGW Distro - nuwen.net #####
call %~dp0bootstrap_MinGW\bootstrap_mingw.bat
pause
ECHO ##### Bootstrapping CMake #####
call %~dp0bootstrap_CMake\bootstrap_cmake.bat
pause
ECHO ##### Bootstrapping LLVM #####
call %~dp0bootstrap_LLVM\bootstrap_llvm.bat
pause
ECHO ###### Bootstrapping WinPython-64bit-3.6 #####
SET WINPYTHON_URL=https://github.com/winpython/winpython/releases/download/1.9.20171031/WinPython-64bit-3.6.3.0Qt5.exe
call %~dp0bootstrap_WinPython\bootstrap_winpython.bat
pause
set S32DS=C:\NXP\S32DS_Power_v2017.R1
echo ##### Copying compiler. #####
if not exist %~dp0powerpc-eabivle-4_9 Robocopy.exe %S32DS%\Cross_Tools\powerpc-eabivle-4_9 C:\Projects\mpc5748G_examples\powerpc-eabivle-4_9 /MIR
if not exist %~dp0e200_ewl2 Robocopy.exe %S32DS%\S32DS\e200_ewl2 C:\Projects\mpc5748G_examples\e200_ewl2 /MIR

set PATH=%PATH%;%~dp0powerpc-eabivle-4_9\bin