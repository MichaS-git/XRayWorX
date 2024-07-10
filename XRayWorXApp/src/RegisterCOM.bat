@echo off

echo RegisterCOM.bat
echo.
echo Additional information:
echo Make sure the following files and folders exist in the executable path:
echo   "<ExecutablePath>\ErroCodes\ErrorCodes.csv"
echo   "<ExecutablePath>\Config\XBaseConfig.xml"
echo   "<ExecutablePath>\Config\XProcedureConfig.xml"
echo.
echo Copy the following files to the project directory:
echo   BR.AN.PVIServices.dll
echo   Microsoft.Win32.Registry.dll
echo   System.IO.FileSystem.AccessControll.dll
echo   System.Security.AccessControl.dll
echo   System.Security.Principal.Windows.dll
echo   TwinCAT.Ads.dll
echo   X-Lab.Commands.dll
echo   X-Lab.Framework.dll
echo   X-Lab.ProcedureLib.dll
echo   X-Lab.Reflection.dll
echo   X-Lab.Utils.dll
echo   XRAYWorX.Files.dll
echo   XRAYWorX.SystemExtensions.dll
echo   XRAYWorX.Threading.dll
echo   XRAYWorX.Utils.Cryptography.dll
echo   XRAYWorX.Utils.Interfaces.dll
echo   XRAYWorX.Utils.Plc.Contracts.dll
echo   XRAYWorX.Utils.Plc.dll
echo   XRAYWorX.Utils.Serialization.dll
echo   XRAYWorX.Utils.System.dll
echo   XRAYWorX.Utils.Web.dll
echo   XRAYWorX.Base.Config.dll
echo   XRAYWorXBase.dll
echo   XRAYWorXBase.Loader.dll
echo   XRAYWorXBase.Misc.dll
echo   XRAYWorXBase.Tube.dll
echo   XRAYWorXBaseCOM.dll
echo.
echo Make sure to use the matching regasm.exe (x86/x64) for the target system.
echo.

IF EXIST "%WINDIR%\Microsoft.NET\Framework64" GOTO x64

path="%WINDIR%\Microsoft.NET\Framework\v4.0.30319\"
echo %path%
GOTO endSetPath

:x64
path="%WINDIR%\Microsoft.NET\Framework64\v4.0.30319\"
echo %path%

:endSetPath

regasm.exe "%~dp0\XRAYWorXBase.dll" /tlb /codebase
echo.
regasm.exe "%~dp0\XRAYWorXBaseCOM.dll" /tlb /codebase

pause