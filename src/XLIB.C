//   ver. 1.3   x_pasteChars have been added 
//   ver. 1.2.  x_addStr have been added
//   ver. 1.1.  x_strCmp have been added
//

//#include <alloc.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "sysdef.h"


# ifdef X_DEBUG
   FILE    * x_out= 0;
   ulong	 x_call = 0;
   char 	 x_buf[x_BUF_SZ + 1];
   int		 x_debug = 0;
   const boolean  x_isDebug = true;
# else
   const boolean  x_isDebug = false;
# endif

double    x_uLong (uLong x)
{
double rc = x;
return rc;
}

ulong    
x_parmValue (
      char * value, 
      const uint valueSz, 
      const char * line, 
      const char * parm,
      const char   separator)
{
ulong rc = _WPAR;
if (line && parm){
    uint i = 0,  pSz = strlen(parm), lSz = strlen(line);
    int l = 0;
    char * ch = 0;
    rc = _NOTITEM;
    if (pSz > lSz) // if parameter bigger than line 
        return rc; // then notitem
    for (; i  <= lSz - pSz ; i++){
        l = x_strCmp(line + i, parm);
        if (l==0){       // value of parm is ""
            rc = _OK;
            *value = 0;
            return rc;
        } 
//        printf ("\n parm =%s, line=%s, l=%d", parm, line + i,l);
        if (l < 0)  
            l = -l; 
        if (l > (int)pSz ){ 
             l += i-1;
             for(i = 0; line[l+i] != 0 && line[l+i] != separator ; i++) {
                 if (i < valueSz) 
                     value[i] = line[l+i];
                 else 
                     break;
             }
             if (i < valueSz) {
                 value[i]=0;
                 rc = _OK;
             }
             else  
                 rc = _NOTMEM;
             break;
        }
    }
}
return rc;
}

long 
x_strCmp( const char *s1, const char *s2 )
{
    long i;
    for( i = 0 ; s1[i] == s2[i] && s1[i] ; i++ );
    if( s1[i] == s2[i] )
        i=0;
    else
        i++;
    return i;
}

void         // no null at the end
x_pasteChars(char * str,  const char * chars)
{
if (str && chars){
    while (*chars) {
        *str = *chars;
        str++;
        chars++;
    }
    // don't copy the 0
}
}

char * 
x_rplcStr(char * old, const char * new)
{
x_free(old);
if(new){
    const uint	 xLen = strlen(new)+1;
    old = x_malloc(xLen);
    if(old)
    	strcpy(old, new);
}
return old;
}

char * 
x_addStr(char * old, const char * tail)
{
uint tL;
if(tail && (tL = strlen(tail)) > 0){
    char * tmp = 0;
    uint oL = 0;
    if (old && (oL = strlen(old)) > 0) {
        tmp = x_malloc (oL + tL + 1);
        if (tmp) {
            strcpy (tmp, old);
        }
    }
    else  {         // in this case (oL==0) I could to do x_rplcStr simply.
        tmp = x_malloc( tL + 1);
    }
    if (tmp) {
        strcpy(tmp+oL, tail);
    }
    x_free(old);
    return tmp;
}
return old;
}



char  *  x_empty(const char * str)
{
static char * _x = "nill";
if(str)
	return (char *)str;
return _x;
}

static char _toTime[10]; // hh:mm:ss dd.mm.yy

char *     x_curTime(){
struct tm *t;
time_t tt;
   tt = time(NULL) ;
   t  = localtime(&tt);
   sprintf(_toTime, "%02d:%02d:%02d", t->tm_hour, t->tm_min, t->tm_sec);
return _toTime;
}

char *     x_curDate(){
struct tm *t;
time_t tt;
   tt = time(NULL) ;
   t  = localtime(&tt);
   sprintf(_toTime, "%02d.%02d.%02d", t->tm_mday, t->tm_mon+1,  t->tm_year%100);
return _toTime;
}



# ifdef X_DEBUG

   void * x_calloc(uint nitems, uint size){
   void * x = calloc(nitems, size);
   x_call++;
   if(x_debug & x_ALLOC)
		fprintf(x_out, "\n%10lu: calloc(mem)          call:%lu sz:%lu",
				   (ulong) x, x_call, (ulong)( size * nitems));
   return x;
   }



   void * x_malloc(uint size){
   void * x = malloc(size);
   x_call++;
   if(x_debug & x_ALLOC)
		fprintf(x_out, "\n%10lu: malloc(mem)          call:%lu sz:%lu",
						(ulong) x,					  x_call, (ulong) size);
   return x;
   }


   void * x_realloc(void * block, uint size){
   void * x ;
   if(block && (x_debug & x_ALLOC)){
	   x_call++;
		fprintf(x_out, "\n%10lu: realloc - free(mem)  call:%lu",
						(ulong) block,					   x_call);
		}
   x = realloc (block, size);
   x_call++;
   if(x_debug & x_ALLOC)
		fprintf(x_out, "\n%10lu: realloc -malloc(mem) call:%lu sz:%lu",
						  (ulong) x,				   x_call, (ulong) size);
   return x;
   }

   char * x_tempnam(char * dir, char * prefix){
   char * x  = tempnam(dir, prefix);

   x_call++;
   if(x_debug & x_ALLOC)
        fprintf(x_out, "\n%10lu: tempnam -malloc(mem)  call:%lu",
                   (ulong) x, x_call);
   return x;
   }

   void   x_free_  (void * block){
	if(block) {
		free(block);
		x_call++;
		if(x_debug & x_ALLOC)
			fprintf(x_out, "\n%10lu: free(mem)            call:%lu",
					  (ulong) block,					  x_call);
	}
   }
# endif
