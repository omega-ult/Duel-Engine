@echo Start building boost for Duel Engine
bjam.exe --toolset=msvc-10.0 --stagedir=./x86 address-model=32 --with-date_time --with-system --with-thread link=shared runtime-link=shared threading=multi define=_CRT_SECURE_NO_DEPRECATE define=_SCL_SECURE_NO_DEPRECATE stage debug release
xcopy .\x86\lib\*.lib .\..\Lib\x86\ /e/s/y
xcopy .\x86\lib\*.dll .\..\..\Bin\x86\ /e/s/y
bjam.exe --toolset=msvc-10.0 --stagedir=./x86 address-model=32 --with-date_time --with-system --with-thread link=shared runtime-link=shared threading=multi define=_CRT_SECURE_NO_DEPRECATE define=_SCL_SECURE_NO_DEPRECATE stage clean
del .\bin.v2\ /s/q
rd .\bin.v2 /s/q
rd .\x86 /s/q