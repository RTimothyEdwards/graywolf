/* ----------------------------------------------------------------- 
FILE:	    program.c                                       
DESCRIPTION:miscellaneous program initialization functions
CONTENTS:   
	    YinitProgram(char *, char *, INT (*function) )
	    YexitPgm(int)
	    char *YgetProgName()
DATE:	    Jan 29, 1988 
REVISIONS:  May 04, 1988 - updated initProgram to include 
			introTextFunction. Added remove_lblanks,
			directoryExists functions.
	    Sep 26, 1988 - split into three files for archive.
	    Sep 29, 1988 - removed argument from introTextfunction.
	    Jan 25, 1989 - added error for nonzero status for exitPgm.
	    Jan 27, 1989 - changed msg to YmsgG so user knows its global.
	    Apr 27, 1989 - changed to Y prefix.
	    May  8, 1990 - removed \n from message format.
	    Oct  4, 1990 - added Ytimer_start to initialization.
	    Fri Feb 15 15:38:11 EST 1991 - renamed message functions
		and added timer.
	    Fri Feb 22 23:39:39 EST 1991 - added newline character
		at exit.
----------------------------------------------------------------- */
#ifndef lint
static char SccsId[] = "@(#) program.c version 3.8 3/4/92" ;
#endif

#include <yalecad/base.h>
#include <yalecad/message.h>
#include <yalecad/program.h>
 
static char programName[LRECL];
static char progVersion[LRECL];
static char progDate[LRECL];

/* ----------------------------------------------------------------- 
   Program control routines                    
   
   initProgram gets the date of compile of program 
   introTextFunction always uses YmsgG (actually a global note G suffix ).
   The user may perform various initialization in introTextFunction. 
   Returns string with program name, version, and compile date.
   ----------------------------------------------------------------- */
char *YinitProgram(name,version,introTextFunction)
     char *name ;
     char *version ;
     VOID (*introTextFunction)() ;
{
  char    *date ,
  *getCompileDate() ;
  
  Ytimer_start() ;   /* start the elapsed timer */
  sprintf(programName,"%s",name);
  sprintf(progVersion,"%s",version);
  
  if ( date = getCompileDate() ){
    sprintf(progDate,"%s",date);
  } else {
    sprintf(progDate,"unknown") ;
  }
  sprintf( YmsgG, "%s version:%s date:%s",
	  programName,progVersion,progDate) ;
  
  /* call the users intro text if available */
  /* Intro Text function may use YmsgG as a global variable */
  if( introTextFunction ){
    (*introTextFunction)() ;
  }
  return( YmsgG ) ;
  
} /* end initProgram */

/* exit program gracefully */
YexitPgm(status)
INT status ;
{

    INT     errorCount, 
	    warningCount,
	    mode ;
    char    message[LRECL] ,
	    *name ;

    warningCount = Ymessage_get_warncount() ;
    errorCount = Ymessage_get_errorcount() ;
    if( status != 0 && errorCount == 0 ){
	/* if we have an error status but no recorded error record error*/
	errorCount++ ;
    }
    mode = Ymessage_get_mode() ;
    if( errorCount != 0 || mode != M_SILENT ){
	/* make sure we see errors */
	Ymessage_mode( M_VERBOSE ) ;
    }

    if( name = YgetProgName() ){
	if( errorCount ){
	    sprintf(message,"\n%s terminated abnormally with %d error[s] and %d warning[s]\n\n",
		name,errorCount,warningCount) ;
	} else {
	    sprintf(message,"\n%s terminated normally with no errors and %d warning[s]\n\n",
		name,warningCount) ;
	}
    } else {
	M(WARNMSG,"exitPgm","Unable to get program name.  Probably initProgram not used.\n") ;
	sprintf(message,"Program terminated abnormally with %d error[s] and %d warning[s]\n\n",
		errorCount,++warningCount) ;
    }
    M(MSG,NULL,message) ;
    /* now write debug file if desired */
    YdebugWrite() ;
    Ymessage_close();	/* Added by Tim, 5/4/11 */
    exit(status) ;

} /* end exitPgm */
 
    
char *YgetProgName()
{
 
    return(programName);
}
/*---------------end program control routines --------------------- */
