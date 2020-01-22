/****
 * parse.c
 *
 *  parse regular expression
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
 * 0.01 1990/10/26 Add empty expression
 * 0.02 1990/10/27 Add parsev() for multi pattern
 *
 *****************************************************************************/

/* This file(parse.cpp) was modified from the original prase.c.
    The modifications are as follows.

  (1)Renamed "parse.cpp". 
									 2001/07/14 Tomoichi Ebata
*/ 

#include <stdlib.h>
#include "rexpdef.h"

static int token;
extern int re_tok_value;

static int expression( void );

static int factor( void )
{
	int node;

	if( token == LBR ) {
		token = re_get_token();
		if(token == RBR)
			node = re_mkleaf(NIL,NIL);
		else {
			node = expression();
			if(token != RBR)
				longjmp(re_begin,RE_ERR_PAR);
		}
	}
	else if(isliteral(token))
		node = re_mkleaf(token,re_tok_value);
	else if(token == 0)
		node = re_mkleaf(NIL,NIL);
	else
		longjmp(re_begin,RE_ERR_SYNTAX);
	while( (token = re_get_token()) == CLOS || token == CLOSP || token == ZRONE )
		node = re_mknode(token, node, 0);
	return node;
}

static int term( void )
{
	int node;

	node = factor();
	while( token == LBR || isliteral(token))
		node = re_mknode(CAT,node,term());
	return node;
}

static int expression( void )
{
	int node;

	if(token == UNI) {
		token = re_get_token();
		if( token == RBR )
			node = re_mkleaf(NIL,NIL);
		else
			node = re_mknode(UNI,re_mkleaf(NIL,NIL),expression());
	}
	else
		node = term();
	while( token == UNI) {
		token = re_get_token();
		if( token == RBR )
			node = re_mknode(UNI,node,re_mkleaf(NIL,NIL));
		else
			node = re_mknode(UNI,node,expression());
	}
	return node;
}
static int parse( uchar* pat, int id )
{
	wchar_t* p;
	int node;

	p = re_token_init(pat);
	if((re_errno = setjmp(re_begin)) == 0) {
		token = re_get_token();
		node = re_mknode(CAT,expression(),re_mkleaf(NIL,id));
		if(token)
			longjmp(re_begin,RE_ERR_SYNTAX);
	}
	free(p);
	return node;
}

static int node;
static int patno;

void re_parse_init( void )
{
	node = -1;
	patno = 0;
}

unsigned re_parse_add( uchar* pat )
{
	unsigned patid = 1 << patno++;

	if(node == -1)
		node = parse( pat, patid );
	else
		node = re_mknode(BRANCH,node,parse(pat,patid));
	return patid;
}
