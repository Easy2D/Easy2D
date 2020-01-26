@echo off
chcp 65001 >nul

title Easy2D 安装向导

echo 安装程序加载中...

:: 获取管理员权限
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

:: 创建路径变量
setlocal enabledelayedexpansion
set vsregpath=
set existvs2013=0
set existvs2015=0
set existvs2017=0
set existvs2019=0
set vs2013path=0
set vs2015path=0
set vs2017path=0
set vs2019path=0
set vs2013include=0
set vs2013x86lib=0
set vs2013x64lib=0
set vs2015include=0
set vs2015x86lib=0
set vs2015x64lib=0
set vs2017include=0
set vs2017x86lib=0
set vs2017x64lib=0
set vs2019include=0
set vs2019x86lib=0
set vs2019x64lib=0

:: 查询操作系统是32位还是64位
2>nul reg query "HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Session Manager\Environment" /v "PROCESSOR_ARCHITECTURE" | findstr "AMD64">nul
if %errorlevel% EQU 0 (set vsregpath=WOW6432Node\)
set vsregpath=HKLM\Software\%vsregpath%Microsoft\VisualStudio\SxS\VS7

:: 查询VS安装路径
for /f "tokens=1,2*" %%a in ('2^>nul reg query "%vsregpath%" /v "11.0"') do (set vs2012path=%%cVC\)
for /f "tokens=1,2*" %%a in ('2^>nul reg query "%vsregpath%" /v "12.0"') do (set vs2013path=%%cVC\)
for /f "tokens=1,2*" %%a in ('2^>nul reg query "%vsregpath%" /v "14.0"') do (set vs2015path=%%cVC\)

for /f "tokens=1" %%a in ('2^>nul reg query "HKLM\Software\WOW6432Node\Microsoft\Windows\CurrentVersion\Uninstall"') do (
    for /f "tokens=1,2*" %%b in ('2^>nul reg query %%a /v "DisplayName"') do (
        set temp=""%%d
        if not "x!temp!"=="x!temp:Visual Studio Community 2017=!" ( set vs2017regitem=%%a )
        if not "x!temp!"=="x!temp:Visual Studio Professional 2017=!" ( set vs2017regitem=%%a )
        if not "x!temp!"=="x!temp:Visual Studio Enterprise 2017=!" ( set vs2017regitem=%%a )

        if not "x!temp!"=="x!temp:Visual Studio Community 2019=!" ( set vs2019regitem=%%a )
        if not "x!temp!"=="x!temp:Visual Studio Professional 2019=!" ( set vs2019regitem=%%a )
        if not "x!temp!"=="x!temp:Visual Studio Enterprise 2019=!" ( set vs2019regitem=%%a )
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

:: 设置VS包含目录和库目录
if exist "%vs2013path%" (
    set existvs2013=1
    set vs2013include=!vs2013path!include\
    set vs2013x86lib=!vs2013path!lib\
    set vs2013x64lib=!vs2013path!lib\amd64\
)
if exist "%vs2015path%" (
    set existvs2015=1
    set vs2015include=!vs2015path!include\
    set vs2015x86lib=!vs2015path!lib\
    set vs2015x64lib=!vs2015path!lib\amd64\
)
if exist "%vs2017path%" (
    :: 读取VS2017工具集版本号
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
    :: 读取VS2019工具集版本号
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


:Start
:: 显示安装界面
cls
echo 欢迎使用 Easy2D 安装向导
echo 该向导将帮助您将 Easy2D 的库文件拷贝到相应目录中
echo.
echo [1] 自动安装
echo [2] 安装在指定的 Visual Studio 版本上

choice /c 12 /n /m 请输入选项：
set userchoice=%errorlevel%

if %userchoice%==1 goto ONE
if %userchoice%==2 goto TWO
exit

:ONE
:: 检测所有VS版本并安装
if %existvs2013% == 1 (
    echo.
    echo 正在安装 VS2013 版本库
    xcopy include "!vs2013include!" /e /c /f /y
    xcopy output\v120\x86 "!vs2013x86lib!" /e /c /f /y
    xcopy output\v120\x64 "!vs2013x64lib!" /e /c /f /y
    echo VS2013 版本库安装完成
)
if %existvs2015% == 1 (
    echo.
    echo 正在安装 VS2015 版本库
    xcopy include "!vs2015include!" /e /c /f /y
    xcopy output\v140\x86 "!vs2015x86lib!" /e /c /f /y
    xcopy output\v140\x64 "!vs2015x64lib!" /e /c /f /y
    echo VS2015 版本库安装完成
)
if %existvs2017% == 1 (
    echo.
    echo 正在安装 VS2017 版本库
    xcopy include "!vs2017include!" /e /c /f /y
    xcopy output\v141\x86 "!vs2017x86lib!" /e /c /f /y
    xcopy output\v141\x64 "!vs2017x64lib!" /e /c /f /y
    echo VS2017 版本库安装完成
)
if %existvs2019% == 1 (
    echo.
    echo 正在安装 VS2019 版本库
    xcopy include "!vs2019include!" /e /c /f /y
    xcopy output\v142\x86 "!vs2019x86lib!" /e /c /f /y
    xcopy output\v142\x64 "!vs2019x64lib!" /e /c /f /y
    echo VS2019 版本库安装完成
)
echo.
echo 按任意键退出 & pause>nul
exit

:TWO
echo.
echo 请选择安装版本：
echo [1] Visual Studio 2013
echo [2] Visual Studio 2015
echo [3] Visual Studio 2017
echo [3] Visual Studio 2019

choice /c 1234 /n /m 请输入选项：

if %errorlevel%==1 goto SETUP2013
if %errorlevel%==2 goto SETUP2015
if %errorlevel%==3 goto SETUP2017
if %errorlevel%==4 goto SETUP2019

:SETUP2013
if %existvs2013% == 1 (
    echo.
    echo 正在安装 VS2013 版本库
    xcopy include "!vs2013include!" /e /c /f /y
    xcopy output\v120\x86 "!vs2013x86lib!" /e /c /f /y
    xcopy output\v120\x64 "!vs2013x64lib!" /e /c /f /y
    echo VS2013 版本库安装完成
) else (
    echo.
    echo 在您电脑上找不到 Visual Studio 2013
)
echo.
echo 按任意键退出 & pause>nul
exit

:SETUP2015
if %existvs2015% == 1 (
    echo.
    echo 正在安装 VS2015 版本库
    xcopy include "!vs2015include!" /e /c /f /y
    xcopy output\v140\x86 "!vs2015x86lib!" /e /c /f /y
    xcopy output\v140\x64 "!vs2015x64lib!" /e /c /f /y
    echo VS2015 版本库安装完成
) else (
    echo.
    echo 在您电脑上找不到 Visual Studio 2015
)
echo.
echo 按任意键退出 & pause>nul
exit

:SETUP2017
if %existvs2017% == 1 (
    echo.
    echo 正在安装 VS2017 版本库
    xcopy include "!vs2017include!" /e /c /f /y
    xcopy output\v141\x86 "!vs2017x86lib!" /e /c /f /y
    xcopy output\v141\x64 "!vs2017x64lib!" /e /c /f /y
    echo VS2017 版本库安装完成
) else (
    echo.
    echo 在您电脑上找不到 Visual Studio 2017
)
echo.
echo 按任意键退出 & pause>nul
exit

:SETUP2019
if %existvs2019% == 1 (
    echo.
    echo 正在安装 VS2019 版本库
    xcopy include "!vs2019include!" /e /c /f /y
    xcopy output\v142\x86 "!vs2019x86lib!" /e /c /f /y
    xcopy output\v142\x64 "!vs2019x64lib!" /e /c /f /y
    echo VS2019 版本库安装完成
) else (
    echo.
    echo 在您电脑上找不到 Visual Studio 2019
)
echo.
echo 按任意键退出 & pause>nul
exit
