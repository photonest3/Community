@echo off
set _upx="E:\voy\102\PhotoNest\tests\cefclient\Release\FUPX_64bit_PORTABLE\upx\400\upx64.exe"
set Params=
%_upx% %Params% "E:\voy\102\PhotoNest\tests\cefclient\Release\bin\nest1.exe"
%_upx% %Params% "E:\voy\102\PhotoNest\tests\cefclient\Release\bin\nest2.exe"
%_upx% %Params% "E:\voy\102\PhotoNest\tests\cefclient\Release\bin\scheme.dll"
%_upx% %Params% "E:\voy\102\PhotoNest\tests\cefclient\Release\bin\locales.dll"
%_upx% %Params% "E:\voy\102\PhotoNest\tests\cefclient\Release\bin\templ.dll"
%_upx% %Params% "E:\voy\102\PhotoNest\tests\cefclient\Release\bin\photol.dll"
%_upx% %Params% "E:\voy\102\PhotoNest\tests\cefclient\Release\bin\inscl.exe"
%_upx% %Params% "E:\voy\102\PhotoNest\tests\cefclient\Release\bin\paint.exe"
%_upx% %Params% "E:\voy\102\PhotoNest\tests\cefclient\Release\bin\cutout.exe"
%_upx% %Params% "E:\voy\102\PhotoNest\tests\cefclient\Release\bin\viewer.exe"
pause
