@echo off
chcp 936 >nul

title Easy2D ��װ��

echo ��װ���������...

:: ��ȡ����ԱȨ��
cd /d "%~dp0"
cacls.exe "%SystemDrive%\System Volume Information" >nul 2>nul
if %errorlevel%==0 goto Main
if exist "%temp%\getadmin.vbs" del /f /q "%temp%\getadmin.vbs"
echo Set RequestUAC = CreateObject^("Shell.Application"^)>"%temp%\getadmin.vbs"
echo RequestUAC.ShellExecute "%~s0","","","runas",1 >>"%temp%\getadmin.vbs"
echo WScript.Quit >>"%temp%\getadmin.vbs"
"%temp%\getadmin.vbs" /f
if exist "%temp%\getadmin.vbs" del /f /q "%temp%\getadmin.vbs"
exit

:Main

:: ����·������
setlocal enabledelayedexpansion
set vsregpath=
set existvs2015=0
set existvs2017=0
set existvs2019=0
set existvs2022=0
set vs2015path=0
set vs2017path=0
set vs2019path=0
set vs2022path=0
set vs2015include=0
set vs2015x86lib=0
set vs2015x64lib=0
set vs2017include=0
set vs2017x86lib=0
set vs2017x64lib=0
set vs2019include=0
set vs2019x86lib=0
set vs2019x64lib=0
set vs2022include=0
set vs2022x86lib=0
set vs2022x64lib=0

:: ��ѯ����ϵͳ��32λ����64λ
2>nul reg query "HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Session Manager\Environment" /v "PROCESSOR_ARCHITECTURE" | findstr "AMD64">nul
if %errorlevel% EQU 0 (
    set vsregpath=WOW6432Node\
)
set vsregpath=HKLM\Software\%vsregpath%Microsoft\VisualStudio\SxS\VS7

:: ��ѯVS��װ·��
for /f "tokens=1,2*" %%a in ('2^>nul reg query "%vsregpath%" /v "14.0"') do ( set vs2015path=%%cVC\ )

for /f "tokens=1" %%a in ('2^>nul reg query "HKLM\Software\WOW6432Node\Microsoft\Windows\CurrentVersion\Uninstall"') do (
    for /f "tokens=1,2*" %%b in ('2^>nul reg query %%a /v "DisplayName"') do (
        set temp=""%%d
        if not "x!temp!"=="x!temp:Visual Studio Community 2017=!" ( set vs2017regitem=%%a )
        if not "x!temp!"=="x!temp:Visual Studio Professional 2017=!" ( set vs2017regitem=%%a )
        if not "x!temp!"=="x!temp:Visual Studio Enterprise 2017=!" ( set vs2017regitem=%%a )

        if not "x!temp!"=="x!temp:Visual Studio Community 2019=!" ( set vs2019regitem=%%a )
        if not "x!temp!"=="x!temp:Visual Studio Professional 2019=!" ( set vs2019regitem=%%a )
        if not "x!temp!"=="x!temp:Visual Studio Enterprise 2019=!" ( set vs2019regitem=%%a )

        if not "x!temp!"=="x!temp:Visual Studio Community 2022=!" ( set vs2022regitem=%%a )
        if not "x!temp!"=="x!temp:Visual Studio Professional 2022=!" ( set vs2022regitem=%%a )
        if not "x!temp!"=="x!temp:Visual Studio Enterprise 2022=!" ( set vs2022regitem=%%a )
    )
)

if defined vs2017regitem (
    for /f "tokens=1,2*" %%a in ('2^>nul reg query !vs2017regitem! /v "InstallLocation"') do (
        set vs2017path=%%c
    )
)

if defined vs2019regitem (
    for /f "tokens=1,2*" %%a in ('2^>nul reg query !vs2019regitem! /v "InstallLocation"') do (
        set vs2019path=%%c
    )
)

if defined vs2022regitem (
    for /f "tokens=1,2*" %%a in ('2^>nul reg query !vs2022regitem! /v "InstallLocation"') do (
        set vs2022path=%%c
    )
)

:: ����VS����Ŀ¼�Ϳ�Ŀ¼
if exist "%vs2015path%" (
    set existvs2015=1
    set vs2015include=!vs2015path!include\
    set vs2015x86lib=!vs2015path!lib\
    set vs2015x64lib=!vs2015path!lib\amd64\
)
if exist "%vs2017path%" (
    :: ��ȡVS2017���߼��汾��
    set vs2017toolver=!vs2017path!\VC\Auxiliary\Build\Microsoft.VCToolsVersion.default.txt
    if exist !vs2017toolver! (
        for /f "usebackq" %%a in ("!vs2017toolver!") do (
            set vs2017path=!vs2017path!\VC\Tools\MSVC\%%a\
        )
        set existvs2017=1
        set vs2017include=!vs2017path!include\
        set vs2017x86lib=!vs2017path!lib\x86\
        set vs2017x64lib=!vs2017path!lib\x64\
    )
)
if exist "%vs2019path%" (
    :: ��ȡVS2019���߼��汾��
    set vs2019toolver=!vs2019path!\VC\Auxiliary\Build\Microsoft.VCToolsVersion.default.txt
    if exist !vs2019toolver! (
        for /f "usebackq" %%a in ("!vs2019toolver!") do (
            set vs2019path=!vs2019path!\VC\Tools\MSVC\%%a\
        )
        set existvs2019=1
        set vs2019include=!vs2019path!include\
        set vs2019x86lib=!vs2019path!lib\x86\
        set vs2019x64lib=!vs2019path!lib\x64\
    )
)
if exist "%vs2022path%" (
    :: ��ȡVS2022���߼��汾��
    set vs2022toolver=!vs2022path!\VC\Auxiliary\Build\Microsoft.VCToolsVersion.default.txt
    if exist !vs2022toolver! (
        for /f "usebackq" %%a in ("!vs2022toolver!") do (
            set vs2022path=!vs2022path!\VC\Tools\MSVC\%%a\
        )
        set existvs2022=1
        set vs2022include=!vs2022path!include\
        set vs2022x86lib=!vs2022path!lib\x86\
        set vs2022x64lib=!vs2022path!lib\x64\
    )
)


:Start
:: ��ʾ��װ����
cls
echo ��ӭʹ�� Easy2D ��װ��
echo ���򵼽��������� Easy2D �Ŀ��ļ���������ӦĿ¼��
echo.
echo [1] �Զ���װ
echo [2] ��װ��ָ���� Visual Studio �汾��

choice /c 12 /n /m ������ѡ�
set userchoice=%errorlevel%

if %userchoice%==1 goto AUTOINSTALL
if %userchoice%==2 goto CUSTOMINSTALL
exit

:AUTOINSTALL
:: �������VS�汾����װ
if %existvs2015% == 1 (
    echo.
    echo ���ڰ�װ VS2015 �汾��
    xcopy include "!vs2015include!" /e /c /f /y
    xcopy output\v140\x86 "!vs2015x86lib!" /e /c /f /y
    xcopy output\v140\x64 "!vs2015x64lib!" /e /c /f /y
    echo.
    echo VS2015 �汾�ⰲװ���
)
if %existvs2017% == 1 (
    echo.
    echo ���ڰ�װ VS2017 �汾��
    xcopy include "!vs2017include!" /e /c /f /y
    xcopy output\v141\x86 "!vs2017x86lib!" /e /c /f /y
    xcopy output\v141\x64 "!vs2017x64lib!" /e /c /f /y
    echo.
    echo VS2017 �汾�ⰲװ���
)
if %existvs2019% == 1 (
    echo.
    echo ���ڰ�װ VS2019 �汾��
    xcopy include "!vs2019include!" /e /c /f /y
    xcopy output\v142\x86 "!vs2019x86lib!" /e /c /f /y
    xcopy output\v142\x64 "!vs2019x64lib!" /e /c /f /y
    echo.
    echo VS2019 �汾�ⰲװ���
)
if %existvs2022% == 1 (
    echo.
    echo ���ڰ�װ VS2022 �汾��
    xcopy include "!vs2022include!" /e /c /f /y
    xcopy output\v143\x86 "!vs2022x86lib!" /e /c /f /y
    xcopy output\v143\x64 "!vs2022x64lib!" /e /c /f /y
    echo.
    echo VS2022 �汾�ⰲװ���
)
echo ��������˳� & pause>nul
exit

:CUSTOMINSTALL
echo.
echo ��ѡ��װ�汾��
echo [1] Visual Studio 2015
echo [2] Visual Studio 2017
echo [3] Visual Studio 2019
echo [4] Visual Studio 2022

choice /c 12345 /n /m ������ѡ�

if %errorlevel%==1 goto SETUP2015
if %errorlevel%==2 goto SETUP2017
if %errorlevel%==3 goto SETUP2019
if %errorlevel%==4 goto SETUP2022

:SETUP2015
if %existvs2015% == 1 (
    echo.
    echo ���ڰ�װ VS2015 �汾��
    xcopy include "!vs2015include!" /e /c /f /y
    xcopy output\v140\x86 "!vs2015x86lib!" /e /c /f /y
    xcopy output\v140\x64 "!vs2015x64lib!" /e /c /f /y
    echo.
    echo VS2015 �汾�ⰲװ���
) else (
    echo.
    echo �����������Ҳ��� Visual Studio 2015
)
echo ��������˳� & pause>nul
exit

:SETUP2017
if %existvs2017% == 1 (
    echo.
    echo ���ڰ�װ VS2017 �汾��
    xcopy include "!vs2017include!" /e /c /f /y
    xcopy output\v141\x86 "!vs2017x86lib!" /e /c /f /y
    xcopy output\v141\x64 "!vs2017x64lib!" /e /c /f /y
    echo.
    echo VS2017 �汾�ⰲװ���
) else (
    echo.
    echo �����������Ҳ��� Visual Studio 2017
)
echo ��������˳� & pause>nul
exit

:SETUP2019
if %existvs2019% == 1 (
    echo.
    echo ���ڰ�װ VS2019 �汾��
    xcopy include "!vs2019include!" /e /c /f /y
    xcopy output\v142\x86 "!vs2019x86lib!" /e /c /f /y
    xcopy output\v142\x64 "!vs2019x64lib!" /e /c /f /y
    echo.
    echo VS2019 �汾�ⰲװ���
) else (
    echo.
    echo �����������Ҳ��� Visual Studio 2019
)

:SETUP2022
if %existvs2022% == 1 (
    echo.
    echo ���ڰ�װ VS2022 �汾��
    xcopy include "!vs2022include!" /e /c /f /y
    xcopy output\v143\x86 "!vs2022x86lib!" /e /c /f /y
    xcopy output\v143\x64 "!vs2022x64lib!" /e /c /f /y
    echo.
    echo VS2022 �汾�ⰲװ���
) else (
    echo.
    echo �����������Ҳ��� Visual Studio 2022
)
echo ��������˳� & pause>nul
exit
