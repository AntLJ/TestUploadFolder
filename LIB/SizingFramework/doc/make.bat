@echo off

set FIND=C:\WINNT\SYSTEM32\find

rem for %%I IN ( ..\*.rc ) do iconv -f CP932 -t UTF-8 %%I > %%I2

rem リソーススクリプト展開
for /f "usebackq delims=" %%I IN (`%FIND% "FileDescription" ..\*.rc`) do set PROJECTNAMERC=%%I
for /f "usebackq delims=" %%I IN (`%FIND% "InternalName" ..\*.rc`) do set FILENAMERC=%%I
for /f "usebackq delims=" %%I IN (`%FIND% "FILEVERSION" ..\*.rc`) do set FILEVERSIONRC=%%I
for /f "usebackq delims=" %%I IN (`%FIND% "PRODUCTVERSION" ..\*.rc`) do set PRODUCTVERSIONRC=%%I
set PROJECTNAMERC=%PROJECTNAMERC:\0=%
set FILENAMERC=%FILENAMERC:\0=%
set FILEVERSIONRC=%FILEVERSIONRC:,=.%
set PRODUCTVERSIONRC=%PRODUCTVERSIONRC:,=.%
rem set PROJECTNAMERC="Sizing Framework"
rem set FILENAMERC="winmgr.lib"
rem set FILEVERSIONRC="1.0.0.0"
rem set PRODUCTVERSIONRC="1.0.0.0"

rem 環境変数設定
set DEVFILENAMEPOSTFIX=(develop)

set PROJECTNAME=%PROJECTNAMERC:~38,-1%
set FILEVERSION=%FILEVERSIONRC:~13%
set PRODUCTVERSION=%PRODUCTVERSIONRC:~16%

set RELEASEFILENAME=%FILENAMERC:~35,-1%
set DEVFILENAME=%RELEASEFILENAME%%DEVFILENAMEPOSTFIX%

rem バージョン等取り込み
call version.bat

rem make実行
nmake %*
