@echo off
set DuelEnginePath=%cd%

if exist .\boost_build_log.log del .\boost_build_log.log /q
if exist .\glew_build_log.log del .\glew_build_log.log /q
if exist .\freetype_build_log.log del .\freetype_build_log.log /q
if exist .\OIS_build_log.log del .\OIS_build_log.log /q

@echo Building boost...
cd .\..\..\External\boost\
call build_vc11.bat -> "%DuelEnginePath%\boost_build_log.log"
@echo Build completed, see boost_build_log.log

call "%VS110COMNTOOLS%vsvars32.bat"

cd "%DuelEnginePath%"
@echo Buiding glew... 
devenv.exe ".\..\..\External\glew-1.10.0\build\vc11\glew.sln" /Project glew_shared /Build "Debug|Win32" /Out glew_build_log.log
devenv.exe ".\..\..\External\glew-1.10.0\build\vc11\glew.sln" /Project glew_shared /Build "Release|Win32" /Out glew_build_log.log
devenv.exe ".\..\..\External\glew-1.10.0\build\vc11\glew.sln" /Project glew_shared /Build "Debug MX|Win32" /Out glew_build_log.log
devenv.exe ".\..\..\External\glew-1.10.0\build\vc11\glew.sln" /Project glew_shared /Build "Release MX|Win32" /Out glew_build_log.log
devenv.exe ".\..\..\External\glew-1.10.0\build\vc11\glew.sln" /Project glew_shared /Clean "Debug|Win32" /Out glew_build_log.log
devenv.exe ".\..\..\External\glew-1.10.0\build\vc11\glew.sln" /Project glew_shared /Clean "Release|Win32" /Out glew_build_log.log
devenv.exe ".\..\..\External\glew-1.10.0\build\vc11\glew.sln" /Project glew_shared /Clean "Debug MX|Win32" /Out glew_build_log.log
devenv.exe ".\..\..\External\glew-1.10.0\build\vc11\glew.sln" /Project glew_shared /Clean "Release MX|Win32" /Out glew_build_log.log
@echo Build completed, see glew_build_log.log

@echo Building freetype... 
devenv.exe ".\..\..\External\freetype-2.5.0.1\builds\win32\vc2012\freetype.sln" /Project freetype /Build "Debug|Win32" /Out freetype_build_log.log
devenv.exe ".\..\..\External\freetype-2.5.0.1\builds\win32\vc2012\freetype.sln" /Project freetype /Build "Debug Singlethreaded|Win32" /Out freetype_build_log.log
devenv.exe ".\..\..\External\freetype-2.5.0.1\builds\win32\vc2012\freetype.sln" /Project freetype /Build "Debug Multithreaded|Win32" /Out freetype_build_log.log
devenv.exe ".\..\..\External\freetype-2.5.0.1\builds\win32\vc2012\freetype.sln" /Project freetype /Build "Release|Win32" /Out freetype_build_log.log
devenv.exe ".\..\..\External\freetype-2.5.0.1\builds\win32\vc2012\freetype.sln" /Project freetype /Build "Release Singlethreaded|Win32" /Out freetype_build_log.log
devenv.exe ".\..\..\External\freetype-2.5.0.1\builds\win32\vc2012\freetype.sln" /Project freetype /Build "Release Multithreaded|Win32" /Out freetype_build_log.log
devenv.exe ".\..\..\External\freetype-2.5.0.1\builds\win32\vc2012\freetype.sln" /Project freetype /Clean "Debug|Win32" /Out freetype_build_log.log
devenv.exe ".\..\..\External\freetype-2.5.0.1\builds\win32\vc2012\freetype.sln" /Project freetype /Clean "Debug Singlethreaded|Win32" /Out freetype_build_log.log
devenv.exe ".\..\..\External\freetype-2.5.0.1\builds\win32\vc2012\freetype.sln" /Project freetype /Clean "Debug Multithreaded|Win32" /Out freetype_build_log.log
devenv.exe ".\..\..\External\freetype-2.5.0.1\builds\win32\vc2012\freetype.sln" /Project freetype /Clean "Release|Win32" /Out freetype_build_log.log
devenv.exe ".\..\..\External\freetype-2.5.0.1\builds\win32\vc2012\freetype.sln" /Project freetype /Clean "Release Singlethreaded|Win32" /Out freetype_build_log.log
devenv.exe ".\..\..\External\freetype-2.5.0.1\builds\win32\vc2012\freetype.sln" /Project freetype /Clean "Release Multithreaded|Win32" /Out freetype_build_log.log
@echo Build completed, see freetype_build_log.log


@echo Building OIS... 
devenv.exe ".\..\..\External\ois-v1-3\Win32\ois_vc11.sln" /Project OIS /Build "OIS_DebugDll|Win32" /Out OIS_build_log.log
devenv.exe ".\..\..\External\ois-v1-3\Win32\ois_vc11.sln" /Project OIS /Build "OIS_ReleaseDll|Win32" /Out OIS_build_log.log
devenv.exe ".\..\..\External\ois-v1-3\Win32\ois_vc11.sln" /Project OIS /Clean "OIS_DebugDll|Win32" /Out OIS_build_log.log
devenv.exe ".\..\..\External\ois-v1-3\Win32\ois_vc11.sln" /Project OIS /Clean "OIS_ReleaseDll|Win32" /Out OIS_build_log.log
@echo Build completed, see OIS_build_log.log
