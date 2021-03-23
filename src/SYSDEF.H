#ifndef  __SYSDEF_H
#define  __SYSDEF_H

#include <windows.h>


#define WINDOWS
#define _export

//#define		 X_DEBUG
# if !defined(_INC_TIME)
# include  <time.h>
# endif


# define   _OK		   0
# define   _NOTITEM        1
# define   _NOTMEM	   2
# define   _WPAR	   3
# define   _WVRSN	   4
# define   _ERR 	   5
# define   _NRD	           6

# define   _BUF_SZ     100
# define   _BIGBUF_SZ     500



#define true  TRUE
#define false FALSE
/*
typedef enum boolean {          //  type of columns of table
    false,
    true
}
boolean;
*/
# define x_updNOTITEM(rc,x) ((x)==_NOTITEM?(rc):(x)) 
#define x_bool(x)  ((x)?"true":"false")
#define x_trigger(x)  ((x)?0:1)
typedef unsigned char      uchar;
typedef unsigned int       uint;
typedef unsigned short     ushort;
typedef unsigned long      ulong;
typedef double             uLong;


//							replace bc-value
//# pragma  warn -dup
# define	MAXDRIVE   256
# define	MAXDIR	   256
# define	MAXFILE    256
# define	MAXEXT	   256
# define	MAXPATH    260
//# pragma  warn +dup

# ifdef X_DEBUG

#	if !defined (__STDIO_H )
#	   include "stdio.h"
#	endif

#	define	 x_BUF_SZ	 512
#	define	 x_ALLOC	0x0002
#	define   x_LOCK     0x0004
	extern	 int	   x_debug;
	extern	 char	   x_buf[];
	extern	 ulong	   x_call;
	extern	 FILE	 * x_out;
# endif

extern  const boolean x_isDebug;

# ifdef __cplusplus
	  extern "C"    {
# endif
double    x_uLong (uLong x);
ulong    
x_parmValue (
      char * value, 
      const uint valueSz, 
      const char * line, 
      const char * parm,
      const char   separator);
long      x_strCmp( const char *str1, const char *str2 );
char *    x_rplcStr (char * oldStr, const char * newStr);
char *    x_curTime (void);
char *    x_curDate (void);
char *    x_empty   (const char * str);
char *    x_substrAddr(char * str, uint substrN); // start with 1 but not 0
time_t    x_str2time(const char * s );


#	 ifdef X_DEBUG
	 void * x_calloc (uint nitems, uint size);
	 void * x_malloc (uint size);
	 void	x_free_  (void * block);
	 void * x_realloc(void * block, uint size);
         char * x_tempnam(char * dir, char * prefix);
#		 define x_free(a);	 {x_free_(((void*)a)); (a)=(a)=0;}

#	 else
#		 define x_calloc  calloc
#		 define x_malloc  malloc
#		 define x_realloc realloc
#        define x_tempnam tempnam
#		 define	   x_free(a);	   if((a)){\
    		                             free(((void *)a));\
    					     (a)=(a)=0;\
    					   }


# endif

#ifdef __cplusplus
   }
#endif


#endif

