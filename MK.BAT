@ECHO OFF
if %1. == ?. goto usage
if %2. == l.    goto dvi
call v.bat 


nmake.exe -f t_exe.mk   /D cur_tsk=%1  /NOLOGO 
goto done

:dvi
nmake.exe -f t_dvi.mk    /D cur_tsk=%1  /NOLOGO

goto done
:usage

echo  to build exe             - mk mkTex
echo  to build latex doc (dvi) - mk mkTex l

:done
