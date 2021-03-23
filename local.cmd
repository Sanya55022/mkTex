rem SET PATH=g:\bin\texLive\bin\win32;G:\bin\Graphviz\bin\;%PATH%
rem SET PATH=C:\bin\texLive\bin\win32;G:\bin\Graphviz2008\bin\;%PATH%


rem SET R=/r:Logger.cs.dll
rem SET R=/r:ClassLibrary1.dll 
rem     public enum IMPORTANCELEVEL { Spam, Debug, Warning, Stats, Error, FatalError, Info,  Ignore };
SET R=/r:Logger.cs.dll /r:args.dll 

del /s *.exe *.obj .* *.log *.pdb 

SET NAMEZIP=mkTex
echo %NAMEZIP%

del    ./smp/_text.*


rem 
