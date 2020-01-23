@echo off

rem ���\�[�X�X�N���v�g�W�J
for /f "usebackq delims=" %%I IN (`find "Comments" ..\*.rc`) do set PROJECTNAMERC=%%I
for /f "usebackq delims=" %%I IN (`find "InternalName" ..\*.rc`) do set FILENAMERC=%%I
for /f "usebackq delims=" %%I IN (`find "FILEVERSION" ..\*.rc`) do set FILEVERSIONRC=%%I
for /f "usebackq delims=" %%I IN (`find "PRODUCTVERSION" ..\*.rc`) do set PRODUCTVERSIONRC=%%I
set PROJECTNAMERC=%PROJECTNAMERC:\0=%
set FILENAMERC=%FILENAMERC:\0=%
set FILEVERSIONRC=%FILEVERSIONRC:,=.%
set PRODUCTVERSIONRC=%PRODUCTVERSIONRC:,=.%

rem ���ϐ��ݒ�
set DEVFILENAMEPOSTFIX=(develop)

set PROJECTNAME=%PROJECTNAMERC:~31,-1%
set FILEVERSION=%FILEVERSIONRC:~13%
set PRODUCTVERSION=%PRODUCTVERSIONRC:~16%

set RELEASEFILENAME=%FILENAMERC:~35,-1%
set DEVFILENAME=%RELEASEFILENAME%%DEVFILENAMEPOSTFIX%

rem �o�[�W��������荞��
call version.bat

rem make���s
nmake %*
