#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "sysdef.h"

boolean debug = false;

int _pri = 0;

int    r_lvl = 4;
#define  R_LVL_MIN 1
#define  R_LVL_MAX 9

void r_lvl_up   (){ if ( r_lvl < 9) r_lvl++; }
void r_lvl_down (){ if ( r_lvl > 1) r_lvl--; }


char  _buf[2*_BIGBUF_SZ+1];
char _ver[] = "1.26.1";

char *_size="normalsize"; 
char *_verbatimSize="small"; 

char * _sizes[] = {
      "tiny",
      "scriptsize",
      "footnotesize",
      "small",
      "normalsize",      // 4
      "large",
      "Large",
      "LARGE",
      "huge",
      "Huge"
};

char * sizes (int i) {
  char * ret = _sizes[4];
  if ( i >=0 && i < sizeof(_sizes)/sizeof(char*))
    return _sizes[i];

return ret;
}

void    usage(char *, char *);

char *
skipPrm(char * in, char cmd)
{
  char * s = in;
  _pri = 0;
  while (*in== ' ')
    in++;              // смотрели на пробел, теперь на первый символ
if (debug )
    printf("\n....skipPrm is here %c", *in);
  if (*in == cmd){
    _pri=1;
    while (*in != ' ')
      in++;
  }
  else
     in = s;
  return in;
}

void
printLabel (char *p1)
{
  static char buf [_BIGBUF_SZ+1];
  int b = 0;
  if (_pri!=0)  {
    while (*p1 == ' ')
      p1++;              // смотрели на пробел, теперь на первый символ
    while (*p1 != ' ') {
      buf[b++] = *p1++;  // перекопировали метку
    }
    buf[b++] = 0;
    printf("\n\\label{%s}\n", buf+1);
  }
}



char *
out(
    char * in, boolean modify
)
{ char * p= _buf;
*p=0;
if (debug )
    printf("\n....out is here %c/%d", *in, modify);

if (modify){
    while (*in){
         char * ot_=0;  //output text
        if (debug)    printf("\n....out: it is > %c <", *in);
        switch (*in) {                                           // see 492473-a_gentle_intoduction_to_latex.pdf
        case '\\' :  
            if (*(in+1) == '\\'){
                ot_ = "$\\backslash$";
                strcpy(p, ot_); 
                p  += strlen(ot_)-1;
                in ++;
            }
            else if (*(in+1) == '{'){
                ot_ = "$\\{$";
                strcpy(p, ot_); 
                p  += strlen(ot_)-1;
                in ++;
            }
            else if (*(in+1) == '}'){
                ot_ = "$\\}$";
                strcpy(p, ot_); 
                p  += strlen(ot_)-1;
                in ++;
            }
            else if (*(in+1) == '%'){
                ot_ = "\\%";
                strcpy(p, ot_); 
                p  += strlen(ot_)-1;
                in ++;
            }
            else if (*(in+1) == '&'){
                ot_ = "\\&";
                strcpy(p, ot_); 
                p  += strlen(ot_)-1;
                in ++;
            }
            else if (*(in+1) == '~'){
                ot_ = "\\~{}";
                strcpy(p, ot_); 
                p  += strlen(ot_)-1;
                in ++;
            }
            else if (*(in+1) == '$'){
                ot_ = "\\$";
                strcpy(p, ot_); 
                p  += strlen(ot_)-1;
                in ++;
            }
            else if (*(in+1) == '^'){
                ot_ = "\\^{}";
                strcpy(p, ot_); 
                p  += strlen(ot_)-1;
                in ++;
            }
            else if (*(in+1) == '_'){
                ot_ = "\\_{}";
                strcpy(p, ot_); 
                p  += strlen(ot_)-1;
                in ++;
            }
            else if (*(in+1) == '#'){
                ot_ = "\\#";
                strcpy(p, ot_); 
                p  += strlen(ot_)-1;
                in ++;
            }
            else if (* (in+1) == '@' && * (in+2) == 'D'){
                strcpy(p, "\\number\\day");
                p  += strlen("\\number\\day")-1;
                in += 2;
            }
            else if (* (in+1) == '@' && * (in+2) == 'Y'){
                strcpy(p, "\\number\\year");
                p  += strlen("\\number\\year")-1;
                in += 2;
            }
            else if (* (in+1) == '@' && * (in+2) == 'M'){
# define _M "\\ifcase\\month\\or 01\\or 02\\or 03\\or 04\\or 05\\or 06\\or 07\\or 08\\or 09\\or 10\\or 11\\or 12\\fi"
                strcpy(p, _M);
                p  += strlen(_M)-1;
                in += 2;
            }
            else 
                *p = *in;
            break;
//        case '^' :   
//            *p++ = '\\'; 
//            *p++ = '^'; 
//            *p = '~'; 
//            break;
//        case '<' :   
//            *p++ = '$'; 
//            *p++ = '<'; 
//            *p = '$'; 
//            break;
        case '|' :   
            *p++ = '$'; 
            *p++ = '|'; 
            *p = '$'; 
            break;
//        case '>' :   
//            *p++ = '$'; 
//            *p++ = '>'; 
//            *p = '$'; 
//            break;
//        case '#' :   
//        case '$' :   
//        case '%' :   
//        case '&' :
//        case '{' :   
//        case '}' :  
//        case '_' :  
            *p = '\\'; p++;  
        default :
            *p = *in;
        }
        p++;
        in++;
    }
    *p = *in;
    return _buf;
}
return in;
}

//  if x_fgets(..) < 0 => only blanks 
//  if x_fgets(..) = 0 => eof
//  if len (string of file) >= bSz => x_fgets would skip rest of line
//  if last symbol of line is \ => x_fgets would join current and next line 
//   together
int
x_fgets (
    char    * b,
    int       bSz,
    FILE    * fp
){
char c, cn;
int  i, ic;
int  blank  = -1;

for (i=0; i< bSz; ) {
    ic = getc(fp);
    c = ic;
    if (ic == EOF ){
        b[i] = 0;
        return i * blank;
    }
    else if (c == '\n' && i > 0){
        b[i] = 0;
        return i * blank;
    }
    else if (c == '\n' && i == 0){ // строка началась с ньюлайна
        return -1;
    }
    else if (c == '\\'){
        cn = getc(fp);
        if (cn == '\n') {
            continue;
        }
        else
            ungetc(cn,fp);
    }
    switch (c) {
//
//    to switch on the skipping of blank line you should uncommnt next lines
//    case ' ' :  break;
//    case '\t':  break;
    default :
        blank = 1;                                                   
    }
    b[i++] = c;
}
b[i--] = 0;
//    to skip rest of line
while((c = getc(fp)) != EOF ){
    if( c == '\n')
        break;
}
return i * blank; 
}


int
fGetStr(
    char    * buf,
    int       n,
    FILE    * fp
)
{
int l;
    do{
        l = x_fgets(buf,n,fp);
    }
    while(l < 0);
return l;
}

int
GetStr(
    char    * buf
)
{
return(fGetStr(buf, _BIGBUF_SZ, stdin));
}                 




int
main(
    int        argc,
    char    ** argv
)
{
boolean  first   = false,
         miktex  = false,
         russian = false,
         letter  = false,
         doxygen = false,
         isBib   = false,
         isNote  = false,
         skipAppendix = false,
         modify  = true,
         rsl     = false,
         emph    = false;

char     *class  = 0;
char     buf[_BIGBUF_SZ+1];
char     *p, *p1;
int      isItem = 0;

int      fnote  = 0,
         desc   = 0,
         rc     = _OK ;

printf("%%this file has been generated by mkTex, version '%s' \n", _ver );
printf("%%http://agp1.adr.com.ua/mkTex.html \n");

if (argc == 2) {
   if (argv[1][0] == '?')
       usage(*argv, _ver);
   else if (argv[1][0] == '-')
       usage(*argv, _ver);
   else if (argv[1][0] == '/')
       usage(*argv, _ver);
   for (p = argv[1]; *p; p++)  {
       *p=tolower(*p);
   }
   rc = x_parmValue(buf, _BIGBUF_SZ, argv[1], "preambule", ';');
   if (rc == _OK || x_parmValue(buf, _BIGBUF_SZ, argv[1], "preamble", ';') == _OK) {
       first = true;
       p = buf + 1; // to skip =
       rc = atoi(p);
       if (rc == 1)
           russian = true;
       else if (rc == 2) {
           miktex = true;
           russian = true;
       }
       else if (rc == 10) {
           doxygen = true;
       }
       else if (rc == 12) {
           doxygen = true;
           russian = true;
       }
   }

   rc = x_parmValue(buf, _BIGBUF_SZ, argv[1], "tsize=", ';');
   if (rc == _OK) {
       rc = atoi(buf);
       _size = 0;
       _size = x_rplcStr(_size, sizes(rc)); 
       printf("\\%s\n", _size);           

   }
   rc = x_parmValue(buf, _BIGBUF_SZ, argv[1], "vsize=", ';');
   if (rc == _OK) {
       rc = atoi(buf);
       _verbatimSize = 0;
       _verbatimSize = x_rplcStr(_verbatimSize, sizes(rc)); 
   }
   
   rc = x_parmValue(buf, _BIGBUF_SZ, argv[1], "class=", ';');
   if (rc == _OK) {
       class = x_rplcStr(class,buf); // to skip 
   }
   rc = x_parmValue(buf, _BIGBUF_SZ, argv[1], "rslenv", ';');

   if (rc == _OK)
       rsl = true;
   rc = x_parmValue(buf, _BIGBUF_SZ, argv[1], "skipappendix", ';');

   if (rc == _OK)
       skipAppendix = true;

   rc = x_parmValue(buf, _BIGBUF_SZ, argv[1], "letter", ';');

   if (rc == _OK)
       letter = true;
   rc = x_parmValue(buf, _BIGBUF_SZ, argv[1], "doxygen", ';');

   if (rc == _OK)
       doxygen = true;


   rc = x_parmValue(buf, _BIGBUF_SZ, argv[1], "dbg", ';');

   if (rc == _OK)
       debug = true;


}
else if (argc > 2)
       usage(*argv, _ver);
if (first) {
//    printf("\\documentclass[11pt]{report}\n");
  printf("\\documentclass[%spaper,oneside]{%s}\n", letter?"letter":"a4", class?class:"article");
  printf("\\usepackage{verbatim} \n");
  if ((russian && miktex) || (russian && doxygen))
      printf("\\usepackage[russian]{babel}\n");
  printf("\\usepackage{makeidx}     \n");
    printf("\\usepackage[dvips]{graphicx} \n");

  if(doxygen) {
    printf("\\usepackage[cp1251]{inputenc}\n");
    printf("\\usepackage{fancyhdr} \n");
    printf("\\usepackage{multicol} \n");
    printf("\\usepackage{float}    \n");
    printf("\\usepackage{textcomp} \n");
    printf("\\usepackage{alltt}    \n");
    printf("\\usepackage{times}    \n");
    printf("\\ifx\\pdfoutput\\undefined \n");
    printf("\\usepackage[ps2pdf,    \n");
    printf("         pagebackref=true, \n");
    printf("         colorlinks=true, \n");
    printf("         linkcolor=blue,  \n");
    printf("         unicode=true  \n");    
    printf("         ]{hyperref}     \n");
    printf("\\usepackage{pspicture}  \n");
    printf("\\else                   \n");
    printf("\\usepackage[pdftex,     \n");
    printf("          pagebackref=true, \n");
    printf("          colorlinks=true,  \n");
    printf("          linkcolor=blue,    \n");
    printf("          unicode=true  \n");    
    printf("          ]{hyperref}       \n");
    printf("\\fi                        \n");
    printf("\\setcounter{secnumdepth}{4}   \n");
    printf("\\setcounter{tocdepth}{4}   \n");
    printf("\\renewcommand{\\footrulewidth}{0.4pt}\n");

  }
  else {
    printf("\\usepackage[dvips]{color} \n");
    if (rsl)
      printf("\\usepackage{rslenv}  \n");
    if (russian && miktex == false) {
        printf("\\renewcommand{\\chaptername}{Раздел} \n");
        printf("\\renewcommand{\\appendixname}{Приложение} \n");
        printf("\\renewcommand{\\contentsname}{Оглавление} \n");
        printf("\\renewcommand{\\indexname}{Индекс} \n");
        printf("\\renewcommand{\\bibname}{ПЕРЕЧЕНЬ ССЫЛОЧНЫХ ДОКУМЕНТОВ} \n");
        printf("\\pagestyle{myheadings} \n");
    }
    if (russian && miktex) {
//        printf("\\renewcommand{\\chaptername}{╨рчфхы} \n");
//        printf("\\renewcommand{\\appendixname}{╧Ёшыюцхэшх} \n");
//        printf("\\renewcommand{\\contentsname}{╬уыртыхэшх} \n");
//        printf("\\renewcommand{\\indexname}{╚эфхъё}  \n");
//        printf("\\renewcommand{\\bibname}{╧┼╨┼╫┼═▄ ╤╤█╦╬╫═█╒ ─╬╩╙╠┼═╥╬┬} \n");
        printf("\\pagestyle{myheadings} \n");
    }
    printf("\\setlength{\\topmargin}{0cm} \n");
    printf("\\addtolength{\\textheight}{4cm} \n");
  }
  printf("\\makeindex              \n");
  printf("\\begin{document} \n");
//    printf("\\itshape         \n");
  printf("\\sffamily        \n");

}

while (GetStr(buf)){
    p = buf;
    if (debug )printf("\n....first symbol %c", *p);
    if (*p == '.'){
        p++;
        switch(tolower(*p)) {
        case '<': 
            p++;
            while (*p == ' ')
                p++;
            { char * pp = p;
                while (*pp){
                    if (debug)   fprintf(stderr, "\n ???%s???", pp);
                    *pp = tolower(*pp);
                    pp++;
                }
            }
            if (x_strCmp("index", p)==0)
                printf("\n\\printindex \n");
            else {
              //  p = out(p);
              if (modify)
                printf("\\input {%s}\n",p);  
              else        {
                char *pp = p;
               
                printf("---- File:%s\n",p);  
                printf("\\end{verbatim}\n");
                printf("\\index{");
                while (*pp){
                  if (*pp == '_')
                    printf("\\");
                  printf("%c", *pp++);
                }
                printf("}\n");
                
                printf("\\verbatiminput {%s}\n",p);  

                printf("\\begin{verbatim}\n");  

                printf("---- End Of File:%s\n",p);  

              }

            }
            break;  //включить файл
        case ';':  
            p++;
            switch (*p){
            case '-' : if (modify) modify = false ; else modify = true; break; 
                           // не менять выводимых символов.
            case '#' : isBib = true; break; // включить библиографию
            case 'u'  :  
            case 'U'  :  
                        r_lvl_up   ();         break;
            case 'd'  : 
            case 'D'  : 
                        r_lvl_down ();         break;
            case 'c':  
                p++;
                p = out(p, modify);
                printf("\\cite{%s}\n",p);           
                break;  // новая ссылка на литературу
            case '+':  
                p++;
                p = out(p, modify);
                printf("\\ref{%s}~",p);           
                break;  // новая ссылка на метку
            case 'p':  
                p++;
                while(*p==' ')p++;
                p = out(p, modify);
                printf("\\pageref{%s}~",p);           
                break; 
            case 'a':       // caption
                p++;
                while(*p==' ')p++;
                p = out(p, modify);
                printf("\\caption{%s}~",p);           
                break; 
            case 'l':  
                p++;
                while(*p==' ')p++;
                p = out(p, modify);
                printf("\\label{%s}~",p);           
                break; 
            case '}'  : 
            case '\\' : 
            
            printf ("%s\n",p); break;  // убрать комментарий 
            default:
            printf("%% %s\n",p);           // поставить комментарий
           }
 
        break;  // коментарий 
        case '%':            break;  // нумерация страниц 
        case 's':            break;  // размер страницы
        case 'h':            break;  // размер страницы
        case 'i':            break;  // размер страницы
        case '$': 
            p++;
            printf("\n\\tableofcontents\n", p);
            /*
            if (class==0 || *class == 'a') // article
              printf("\\addcontentsline{toc}{part}{CONTENTS}\n");
            else if (*class == 'r' || *class == 'b') // report or book
              printf("\\addcontentsline{toc}{chapter}{CONTENTS}\n");
            */
            printf("\\thispagestyle{myheadings}\n", p);
            break;   
        case '=':   
            p++;
            if (desc==0){
                desc = 1;
                p = out(p, modify);
                printf("\\begin{description}\n\\item[%s]~\n", p);
                printf("\\medskip\n\\linebreak\n");
            }
            else {
                desc = 0;
                printf("\\end{description}\n");
                if (strlen (p) > 2 )  {
                    desc = 1;
                    p = out(p, modify);
                    printf("\\begin{description}\n\\item[%s]~\n", p);
                    printf("\\medskip\n\\linebreak\n");
                }
            }
            break;              // не нумеровать

        case '{':  
            printf("\\begin{description}\n");
            break;              // не нумеровать
        case '?':   
            p++;
                p = out(p, modify);
            printf("\\item[%s]\n", p);
            break;              // не нумеровать
        case '}':  
            printf("\\end{description}\n");
            break;              // не нумеровать

        case '+':   
            p++;
            p = out(p, modify);
            if (isBib){
                while (*p==' ' || *p=='\t')
                    p++;
                printf("\n\\bibitem{%s}\n\n", p);
                emph=true;
            }
            else 
//                printf("\\item[%s]\n", p);
                printf("\\item\n");
            break;              // не нумеровать
        case '#':   
        case '@':   
            p++;
            p = out(p, modify);
//            printf("\\item[%s]\n", p);
            printf("\\item\n");
            break;              
        case '(':   
            if (isBib)   {
                if (class==0 || *class == 'a') // article
                  printf("\\renewcommand{\\refname}{BIBLIOGRAPHY\\ }\n");

                else if (*class == 'r' || *class == 'b') // report or book
                  printf("\\renewcommand{\\bibname}{BIBLIOGRAPHY\\ }\n");

                printf("\\begin{thebibliography}{99}\n");

                if (class==0 || *class == 'a') // article
                  printf("\\addcontentsline{toc}{part}{\\refname}\n");
                else if (*class == 'r' || *class == 'b') // report or book
                  printf("\\addcontentsline{toc}{chapter}{\\bibname}\n");
                printf("\\thispagestyle{myheadings}\n", p);


            }
            else    {
                printf("\\begin{itemize}\n");
                isItem++;
            }
            break;              // не нумеровать
        case ')':   
            if (isBib)    {
                printf("\\end{thebibliography}\n");
                isBib = false;
            }
            else   {
                printf("\\end{itemize}\n");
                isItem--;
                if (isItem < 0) isItem=0;
            }
            break;              
        case '-':   
            p++;
            p = out(p, modify);
            if (isItem <= 0) {
//                printf("\\medskip\n\\linebreak\n");
                printf("\\newline\n\\linebreak\n");
                printf("--");
            }
            else {
                printf("\\item\n");
            }
            printf(" %s", p);

            break;              // не нумеровать
        case '[':   
            printf("\\begin{enumerate}\n");
            break;              

        case ']':   
            printf("\\end{enumerate}\n");
            break;              

        case '\'': 
        case '"': 
            p++;    // пропустил тект
            p1 = p; p = skipPrm(p, '+');
            p = out(p, modify);
            if (skipAppendix == false){
                printf("\n\\appendix");
                skipAppendix = true;
            }
            printf("\n\\chapter{%s}\n", p);
            printLabel( p1);
            printf("\\thispagestyle{myheadings}\n", p);
            break;   

        case '0': 
            p++;      // 
            p1 = p; p = skipPrm(p, '+');
            p = out(p, modify);
              printf("\n\\part{%s}\n", p);
            printLabel( p1);
            break;  // центрировать текст
        case '1': 
            p++;
            p1 = p; p = skipPrm(p, '+');
   //         p = out(p, modify);
            if (class==0 || *class == 'a') // article
              printf("\n\\part{%s}\n", p);
            else if (*class == 'r' || *class == 'b') // report or book
              printf("\n\\chapter{%s}\n", p);
            printLabel( p1);
            printf("\\thispagestyle{myheadings}\n");
            break;   
        case '2':            
            p++;
            p1 = p; p = skipPrm(p, '+');
            p = out(p, modify);
            printf("\n\\section{%s}\n", p);
            printLabel( p1);
            break;   
        case '3':            
            p++;
            p1 = p; p = skipPrm(p, '+');
            p = out(p, modify);
            printf("\n\\subsection{%s}\n", p);
            printLabel(p1);
            break;   
        case '4':            
            p++;
            p1 = p; p = skipPrm(p, '+');
            p = out(p, modify);
            printf("\n\\subsubsection{%s}\n", p);
            printLabel(p1);
            break;   
        case '5':            
            p++;
            p1 = p; p = skipPrm(p, '+');
            p = out(p, modify);
            printf("\n\\paragraph{%s\\\\}\n", p);
            printLabel(p1);
            break;   
        case '6':           // это началась паранойя          
            p++;
            p1 = p; p = skipPrm(p, '+');
            p = out(p, modify);
            printf("\n\\subparagraph{%s\\\\}\n", p);
            printLabel(p1);
        case '7':                     
        case '8':                     
            p++;
            p1 = p; p = skipPrm(p, '+');
            p = out(p, modify);
            printf("\n\n %s \n\n", p);
            printLabel(p1);
            break;   
        case '9': 
            p++;

            switch (r_lvl){
              case 1: 
                p1 = p; p = skipPrm(p, '+');
                printf("\n\\chapter{%s}\n", p);
                printf("\\thispagestyle{myheadings}\n");
                printLabel(p1);
              break;
              case 2: 
                p1 = p; p = skipPrm(p, '+');
                p = out(p, modify);
                printf("\n\\section{%s}\n", p);
                printLabel(p1);
              break;
              case 3: 
                p1 = p; p = skipPrm(p, '+');
                p = out(p, modify);
                printf("\n\\subsection{%s}\n", p);
                printLabel(p1);
              break;
              case 4: 
                p1 = p; p = skipPrm(p, '+');
                p = out(p, modify);
                printf("\n\\subsubsection{%s}\n", p);
                printLabel( p1);
              break;
              case 5: 
                p1 = p; p = skipPrm(p, '+');
                p = out(p, modify);
                printf("\n\\paragraph{%s}\n", p);
                printLabel(p1);
              break;
              case 6: 
                p1 = p; p = skipPrm(p, '+');
                p = out(p, modify);
                printf("\n\\subparagraph{%s}\n", p);
                printLabel(p1);
              break;
              case 7: 
              case 8: 
              case 9: 
                p1 = p; p = skipPrm(p, '+');
                p = out(p, modify);
                printf("\n\n %s \n\n", p);
                printLabel(p1);
              break;
            }
            break;

        case '_': 
            p++;
            p = out(p, modify);
            printf("\\pagestyle{myheadings}\n");
            printf("\\markright{%s}\n", p);

            break;   

        case 'b':  
            p++;
            {  int x;
               x = atoi(p);
               if (x < 1)  x = 1;
               if (x > 10) x = 10;
               while(x--){
                   printf("\n\\medskip\n");
               }
            }
        break;  // пропуск строчек
        case 'c': 
            p++;
            p1 = p; p = skipPrm(p, '+');
            p = out(p, modify);
            printf("\n\\chapter{%s}\n", p);
            printLabel(p1);
            printf("\\thispagestyle{myheadings}\n", p);
            break;   

        case 'd':
            p++;
            while (*p == ' ')
                p++;
            p = out(p, modify);
            { char * pp = p;

                while (*pp!=' ' && *pp){
                    pp++;
                }
                if (*pp == ' '){
                    *pp = 0;
                    pp++;
                    printf("\\newcommand{\\%s}{%s\\ }\n",p, pp);
                }
                else {
                    printf("!!!error!!!== {\\%s}{%s}",p, pp);
                }
            }
            break;  // 
 
        case 'f':          
            modify = true;
            printf("\\end{verbatim}\n");           
            printf("\\%s\n", _size);           
            break;  // 
 
        case 'n':          
            modify = false;
            printf("\\%s\n",_verbatimSize);           
            printf("\n \n\\begin{verbatim}\n");           
            break;  // 

        case 'p':            
            printf("\\newpage\n");           
            break;  // новая страница 
        case 'x':
            p++;
            p = out(p, false);
            while (*p== ' ')
              p++;              // смотрели на пробел, теперь на первый символ
            if (*p=='+') {
              p1=++p;
              while (*p!= ' ') p++; *p++=0;  //p1 показывает на метку. p на слово
              if (doxygen)
                printf(" \\hypertarget{%s}{\\index{%s}}\\label{%s}\n",p1,p,p1);           
              else
                printf(" \\index{%s}\n",p);           
            }
            else if (*p=='?'){
               p1 = ++p;
               while (*p!= ' ') p++;              
               *p++=0;         // слово
               if (doxygen) 
                 printf(" \\hyperlink{%s}{%s}\\index{%s}\n",p1,p,p);           
               else
                  printf(" \\index{%s}\n %s",p,p);           


            }
            else {
              printf("\\index{%s}\n",p);           
            }
            break;  // занести в индекс 
              
        case '|':            
            printf("\\newline ");
//            printf("\n\\vskip 1 pt\n");
 //           printf("\n\\smallskip\n");
            break;  // новый абзац 

        case '^':            
            printf("\n\\medskip\n\n\n");
            break;  // новый абзац 
        case ':': 
            p++;
            p = out(p, modify);
            printf("\\begin{Large}\n");   
            printf("\n\\begin{center} \n%s \n\\end{center}\n",p);           
            printf("\\end{Large}\n");   
//            printf("\\addcontentsline{toc}{chapter}{%s}\n",p);           

            break;  // центрировать текст

        case '!': 

            if (isNote) {
                isNote = false;
                printf("\n\\end{quotation}");           
            }
            else {
                isNote = true;
                p++;
                p = out(p, modify);
                printf("\n\\begin{center} \n%s \n\\end{center}\n",p);           
                printf("\n\\begin{quotation}\n");           
            }
            break;
        case ' ':
        case '\t':
        case '\n':
                printf(". ");           
            break; 
        default: 
            printf("!!wrong command: %s!!", p);
        }
    }
    else {
        if (debug )printf("\n....before out");
        p = out(buf, modify);
        if (emph)  {
            printf("\\emph{%s}\n", p);
            emph = false;
        }
        else
            printf("%s\n", p);
    }
    if (debug )printf("\n....after if");
 }

if (first) {
    printf("\\end{document}        \n  ");
}
printf("%%% to avoid problem at the end of file  \n%%   \n%%    \n%%   ");


return 0;
}

void usage(char *name, char *ver){


    char szFilePath[4096], *p;
    szFilePath[0] = 0;  
    GetModuleFileName(NULL, szFilePath, sizeof(szFilePath));
    p = strrchr(szFilePath, '\\');
    if (!p) p=strrchr(szFilePath, '/');
    if (p) *p=0;

printf("\n xxxx: >%s<", szFilePath );

printf("\n usage LaTex generator version of %s:", ver );
printf("\n %s [preamble[={1|2|10|12}]][;class=xxx][;rsl][;skipAppendix][;dbg][;letter;doxygen]  <input >output", name);
printf("\n \twhere");
printf("\n preamble    - create preamble");
printf("\n preamble=1  - create preamble and initial for cp866(emtex)");
printf("\n preamble=2  - create preamble and initial for cp1251(miktex) and add");
printf("\n      '\\usepackage[russian]{babel}'");
printf("\n preamble=10 - create preamble for pdflatex(as doxygen makes)");
printf("\n preamble=12 - as 10 and add babel package");
printf("\n class=xxx   - xxx isin {article,report,book,etc}");
printf("\n rslenv      - to use package for rsl");
printf("\n doxygen     - to use crossreferens. additinal command: .x ?metka word");
printf("\n tsize=N     - size of normal text. O<=N<10 (tiny, small, normalsize:4, large, Large:6, etc)");
printf("\n vsize=N     - size of verbatim text");

printf("\n there is the special case of rio comments:");
printf("\n .;# - swith on the bibliography instead the items (.(, .+, .))");
printf("\n .;\\c ref - +> .\\cite{ref} - make the reference to bibliography");
printf("\n .;\\bla bla bla - just copy \\bla bla bla to a tex text");

exit(2);
}





