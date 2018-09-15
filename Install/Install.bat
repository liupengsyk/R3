@echo off
setlocal EnableDelayedExpansion
for /f "tokens=2,*" %%i in ('reg query "HKEY_CURRENT_USER\Software\Microsoft\VisualStudio\10.0" /v "VisualStudioLocation"') do (
call set var=%%j\Wizards
)

echo !var!
md "%var%"

set a1=VSWIZARD 7.0
echo %a1%>.\InstallFiles\TccmWizard.vsz

set a2=Wizard=VsWizard.VsWizardEngine.10.0
echo %a2%>>.\InstallFiles\TccmWizard.vsz

set a3=Param="WIZARD_NAME = TccmWizard"
echo %a3%>>.\InstallFiles\TccmWizard.vsz

set a4=Param="ABSOLUTE_PATH = %var%\TccmWizard"
echo %a4%>>.\InstallFiles\TccmWizard.vsz

set a5=Param="FALLBACK_LCID = 2052"
echo %a5%>>.\InstallFiles\TccmWizard.vsz


copy .\InstallFiles\*.ico   "%var%" /y 
copy .\InstallFiles\*.vsdir "%var%" /y 
copy .\InstallFiles\*.vsz   "%var%" /y 

xcopy .\InstallFiles\HTML      "%var%\TccmWizard\HTML\"      /y /e 
xcopy .\InstallFiles\Images    "%var%\TccmWizard\Images\"    /y /e 
xcopy .\InstallFiles\Scripts   "%var%\TccmWizard\Scripts\"   /y /e 
xcopy .\InstallFiles\Templates "%var%\TccmWizard\Templates\" /y /e
xcopy .\InstallFiles\2052      "%var%\TccmWizard\2052\"      /y /e


echo.
echo .............安装成功.................
echo.
echo ..........按任意键退出.................
pause>nul
exit
