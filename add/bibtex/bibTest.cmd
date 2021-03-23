@echo off

set MKT=G:\agp\96\mkTex\bld\mkTex.exe
dot.exe      -Tps  mkDoc.dot  -o.mkDoc.ps  
epstopdf.exe .mkDoc.ps --outfile=mkDoc.pdf


rem     
%MKT% class=article;letter;preamble=12 <bibTest.txt > .bibTest.tex

rem   
pdflatex.exe -interaction=batchmode  -terminal=oem  .bibTest.tex
rem   
makeindex  >.makeindex.log .bibTest.idx -o .bibTest.ind
rem echo on
rem  
bibtex -include-directory=./bibDB  .bibTest   1>.bibTex.log
rem  
pdflatex.exe -interaction=batchmode  -terminal=oem  .bibTest.tex
rem  
pdflatex.exe -interaction=batchmode  -terminal=oem  .bibTest.tex

