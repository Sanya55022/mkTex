
SET GOAL=text
echo on
call  ../params.%COMPUTERNAME%.cmd
echo off

SET BIB=.\bibDB


rem tex utility

SET LATEX=%P%\pdflatex.exe 
SET MKIDX=%P%\makeindex.exe 
SET BIBTEX=%P%\bibtex.exe 
SET EPS2PDF=%P%\epstopdf.exe
 
SET B=ascii
rem     my utility
set MKT=.\bin\mkTex.exe
rem graphViz utility
set DOT=.\bin\dot.exe

echo on

copy %BIB%\*.bib .               



%MKT% class=article;letter;preamble=12 <%GOAL%.txt > .%GOAL%.tex
%MKT% class=article;letter <%GOAL%.r > .%GOAL%.r


%LATEX% -interaction=batchmode    .%GOAL%.tex


%MKIDX%  >.makeindex.log .%GOAL%.idx -o .%GOAL%.ind


%BIBTEX%  .%GOAL%   1>.%GOAL%.log

%LATEX% -interaction=batchmode   .%GOAL%.tex
%LATEX% -interaction=batchmode   .%GOAL%.tex
exit                            


