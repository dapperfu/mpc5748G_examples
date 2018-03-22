@ECHO OFF
call %~dp0bootstrap_toolchain.bat

make --version

cmake --version

gcc --version

powerpc-eabivle-gcc --version

clang --version

cmd /k