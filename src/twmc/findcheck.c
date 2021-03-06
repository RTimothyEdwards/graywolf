/* ----------------------------------------------------------------- 
FILE:	    findcheck.c                                       
DESCRIPTION:final check
CONTENTS:   finalcheck()
DATE:	    Jan 30, 1988 
REVISIONS:  Jul 21, 1988 - fixed softcell code.
	    Oct 25, 1988 - remove weights from funccost
	    Mar 30, 1989 - changed tile datastructure.
----------------------------------------------------------------- */
#ifndef lint
static char SccsId[] = "@(#) findcheck.c version 3.3 9/5/90" ;
#endif

#include <custom.h>
#include <yalecad/relpos.h>
#include <yalecad/debug.h>


finalcheck()
{

NETBOXPTR netptr ;
PINBOXPTR pinptr ;
CELLBOXPTR ptr ;
INT cell , net ;
INT x , y , xc , yc ;
INT orient ;

fcostG  = 0 ;
fwireG  = 0 ;
fwirexG = 0 ;
fwireyG = 0 ;
for( cell = 1 ; cell <= totalcellsG; cell++ ) {
    ptr = cellarrayG[cell] ;
    orient = ptr->orient ;
    xc = ptr->xcenter ;
    yc = ptr->ycenter ;
    for( pinptr = ptr->pinptr ; pinptr ; pinptr = pinptr->nextpin ) {
	/* rel position is a macro which calculates absolute pin loc */
	/* defined in relpos.h */
	REL_POS( orient, 
	    pinptr->xpos, pinptr->ypos,               /* result */ 
	    pinptr->txpos, pinptr->typos,             /* cell relative */
	    xc, yc ) ;                                /*  cell center  */
    }

}

for( net = 1 ; net <= numnetsG ; net++ ) {
    netptr =  netarrayG[net] ;
    if( netptr->skip ) {
	continue ;
    }
    for( pinptr = netptr->pins ;pinptr ; pinptr = pinptr->next ) {
	if( pinptr ) {
	    break ;
	}
	if( pinptr->skip ) {
	    continue ;
	}
	netptr->xmin = netptr->xmax = pinptr->xpos ;
	netptr->ymin = netptr->ymax = pinptr->ypos ;
	pinptr = pinptr->next ;
	break ;
    }
    for( ; pinptr ; pinptr = pinptr->next ) {
	if( pinptr->skip ) {
	    continue ;
	}
	x = pinptr->xpos ;
	y = pinptr->ypos ;

	if( x < netptr->xmin ) {
	    netptr->xmin = x ;
	} else if( x > netptr->xmax ) {
	    netptr->xmax = x ;
	}
	if( y < netptr->ymin ) {
	    netptr->ymin = y ;
	} else if( y > netptr->ymax ) {
	    netptr->ymax = y ;
	}
    }
    fwirexG += netptr->xmax - netptr->xmin ;
    fwireyG += netptr->ymax - netptr->ymin ;
    fwireG  += netptr->xmax - netptr->xmin + netptr->ymax - netptr->ymin;
    fcostG += ( netptr->xmax - netptr->xmin ) +
	    ( netptr->ymax - netptr->ymin ) ;
}

return;
}



initcheck()
{

NETBOXPTR netptr ;

INT net ;

icostG  = 0 ;
iwireG  = 0 ;
iwirexG = 0 ;
iwireyG = 0 ;

for( net = 1 ; net <= numnetsG ; net++ ) {
    netptr =  netarrayG[net] ;
    iwirexG += netptr->xmax - netptr->xmin ;
    iwireyG += netptr->ymax - netptr->ymin ;
    iwireG  += netptr->xmax - netptr->xmin + netptr->ymax - netptr->ymin;
    icostG += ( netptr->xmax - netptr->xmin) +
	     ( netptr->ymax - netptr->ymin) ;
}
return;
}
