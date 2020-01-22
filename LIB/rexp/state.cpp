/****
 * state.c
 *
 * finite state machine (construct NFA and DFA)
 *
 * This software is public domain. You can use this for both personal and
 * bussines purpose. But if you revise this source and want to put it on
 * view to the general public, please leave my name and discribe revisions.
 * I do not take the responsibility of revising, and do not guarantee for
 * any damage by using this software.
 *
 *  Copyleft 1990 by Y.Iizuka(DUDE).
 *
 * 0.00 1990/10/18 Initial revision
 * 0.01 1990/10/21 speed up state_num()
 * 0.02 1990/10/27 Modify mknode() and F_state[] for multi pattern
 * 0.03 1990/06/04 change isfinal() function to macro
 *
 *****************************************************************************/

/* This file(state.cpp) was modified from the original state.c.
The modifications are as follows.

(1)Renamed "state.cpp". 
									 2001/07/14 Tomoichi Ebata
*/ 

#include <stdlib.h>
#include <string.h>
#include "rexpdef.h"

static struct states St[NFA_MAX];
static int St_num;

static int S_body[TAB_MAX];
static int* S_ptr;
static int DFA_num;

static int* S_state[DFA_MAX];
static int* N_state[DFA_MAX];
unsigned re_F_state[DFA_MAX];

/*
 *	construct NFA
 */

#define add_nil_state()			add_state(NIL,NIL,NIL,NIL,NIL)

struct states* re_state( int state_num )				/* for external */
{
	return St+state_num;
}
#define re_state(state_num)		(St + state_num)	/* for this source */

static int add_state( int type, int value, int lson, int rson, int last )
{
	St[St_num].type = type;
	St[St_num].value = value;
	St[St_num].lson = lson;
	St[St_num].rson = rson;
	St[St_num].last = last;
	return St_num++;
}

int re_mkleaf(int typ, int val)
{
	if(St_num == 0)
		add_nil_state();				/* make root state */
	return St[0].lson = add_state(typ,val,NIL,NIL,St_num);
										/*		  ^ last state is itself */
}

int re_mknode( int op_type, int left, int right )
{
	int start_state,end_state;

	switch( op_type ) {
	case CAT:							/*  -> left -> right ------->	*/
		start_state = left;
		St[St[left].last].lson = right;
		St[start_state].last = St[right].last;
		break;
	case UNI:							/* -> (node) -> left -> (node) ->	*/
										/*      |	        	  ^			*/
										/*      +---> right ------+			*/
		if(St[St[left].last].type == NIL)
			end_state = St[St[right].last].lson = St[left].last;
		else if(St[St[right].last].type == NIL)
			end_state = St[St[left].last].lson = St[right].last;
		else
			end_state = St[St[left].last].lson = St[St[right].last].lson = add_nil_state();
		if(St[left].type == NIL && St[left].rson == NIL) {
			start_state = left;
			St[left].rson = right;
			St[left].last = end_state;
		}
		else if(St[right].type == NIL && St[right].rson == NIL) {
			start_state = right;
			St[right].rson = left;
			St[right].last = end_state;
		}
		else
			start_state = add_state(NIL,NIL,left,right,end_state);
		break;
	case CLOS:							/*     +---> left				*/
										/*     |      |					*/
										/* -> node <--+					*/
										/*     |      					*/
										/*     +------------> node ->	*/
		end_state = add_nil_state();
		start_state = St[St[left].last].lson = add_state(NIL,NIL,left,end_state ,end_state);
		break;
	case CLOSP:							/* -> left <--+					*/
										/*     |      |					*/
										/*     +---> node --> node ->	*/
		start_state = left;
		St[St[left].last].lson = add_state(NIL,NIL,left,end_state = add_nil_state(),NIL);
		St[start_state].last = end_state;
		break;
	case ZRONE:							/* -> node ------------+ 		*/
										/*     |               v		*/
										/*     +---> left --> node ->	*/
		if(St[St[left].last].type == NIL)
			end_state = St[left].last;
		else
			end_state = add_nil_state();
		St[St[left].last].lson = end_state;
		start_state = add_state(NIL,NIL,left,end_state,end_state);
		break;
	case BRANCH:						/* -> node -> left ------->    	*/
										/*      |	        	   		*/
										/*      +---> right ------>    	*/
		start_state = add_state(NIL,NIL,left,right,NIL);
		break;
	}
	return St[0].lson = start_state;
}

/*
 *	trace NFA then construct DFA
 */

#if 0
unsigned isfinal( int state_num )
{
	return re_F_state[state_num];
}
#endif

int* re_N_top( int state_num )
{
	return N_state[state_num];
}

static void save_state( int state_num )
{
	if( S_ptr - S_body >= TAB_MAX)
		longjmp(re_begin,RE_ERR_TABLE);
	*S_ptr++ = state_num;
}

static int* add_S( register int* S )
{
	int* start = S_ptr;

	do
		save_state(*S);
	while( *S++ != NIL );
	return start;
}

static int N_record[NFA_MAX];
static int* next_r;

static void N_expandto( int stat )
{
	struct states* ss;
	register int* pi;

	for( pi = N_record,*next_r = stat ; *pi != stat ; ++pi )
		;									/* Is stat already recorded ? */
	if(pi != next_r)						/* If so, do nothing */
		return;
	++next_r;
	if((ss = re_state(stat))->type == NIL) {	/* epsilon transition */
		if(ss->lson == NIL && ss->rson == NIL)
			re_F_state[DFA_num] |= ss->value;	/* pattern id */
		else {
			if(ss->lson != NIL)
				N_expandto(ss->lson);
			if(ss->rson != NIL)
				N_expandto(ss->rson);
		}
	}
	else									/* reachable leaf from S */
		save_state(stat);
}

static int* add_N( int* S )
{
	struct states* ss;
	int* start = S_ptr;

	next_r = N_record;
	for( ; *S != NIL ; ++S ) {
		if((ss = re_state(*S))->lson == NIL && ss->rson == NIL)
			re_F_state[DFA_num] |= ss->value;	/* pattern id */
		else {
			if(ss->lson != NIL)
				N_expandto(ss->lson);
			if(ss->rson != NIL)
				N_expandto(ss->rson);
		}
	}
	save_state(NIL);
	return start;
}

int re_state_num( int* S )
{
	register int n,*s,*t;

	if(*S != NIL) {							/* Reverse states */
		t = S;
		while( *++t != NIL )
			;
		for( s = S,--t ; s < t ; ++s,--t) {
			n = *s;
			*s = *t;
			*t = n;
		}
	}
	for(n = 0 ; n < DFA_num ; ++n ) {		/* Is S already resigterd ? */
		for( s = S, t = S_state[n]; *s == *t ; ++s,++t)
			if(*s == NIL)
				return n;
	}
	if(DFA_num >= DFA_MAX)
		longjmp(re_begin,RE_ERR_DFA_STAT);
	N_state[DFA_num] = add_N(S_state[DFA_num] = add_S(S));
	return DFA_num++;
}

void re_state_init( void )
{
	memset(re_F_state,0,sizeof(re_F_state));
	St_num = 0;
	S_ptr = S_body;
	DFA_num = 0;
}
