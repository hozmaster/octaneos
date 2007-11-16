@echo off

set BOCHS_HOME="C:\Program Files\Bochs-2.2.pre4"

cd %BOCHS_HOME%
%BOCHS_HOME%\bochs.exe -q -f bochs\.bochsrc
