
@echo off 
set URL="https://sourceforge.net/projects/luabinaries/files/5.4.2/Windows%%20Libraries/Static/lua-5.4.2_Win64_vc16_lib.zip/download"
echo Downloading from %URL%
mkdir Lua\lib
cd Lua\lib

curl -o lua.zip -L %URL% > NUL
tar -xf lua.zip > NUL
move /Y include\* . > NUL
rmdir include
del lua.zip
cd ..