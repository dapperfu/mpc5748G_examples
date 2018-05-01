#!/bin/sh



find . -name "*.c" | xargs --max-procs=8 -n1 dos2unix
find . -name "*.c" | xargs --max-procs=8 -n1 clang-format-6.0 -i -style=file


