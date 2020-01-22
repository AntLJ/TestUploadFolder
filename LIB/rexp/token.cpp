/****
 * token.c
 *
 *  break regular expression into tokens
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
 * 0.01 1990/10/21 add ANY_S (ANY | '\n')
 * 0.02 1990/10/27 modify BOL and EOL handling (delete ANY_S)
 * 0.03 1991/05/13 CCL bug fix
 * 0.04 1991/05/13 bug fix in kanji handling of CCL
 * 0.05 1991/06/05 bug fix in re_ignore_case of CCL
 *
 *****************************************************************************/

/* This file(token.cpp) was modified from the original token.c.
The modifications are as follows.

(1)Renamed "token.cpp". 

(2)Used <ctype.h> instead of <jctype.h>
									 2001/07/14 Tomoichi Ebata
*/ 


#include <stdlib.h>
#include <string.h>
//#include <jctype.h>
#include <ctype.h>
#include "rexpdef.h"

static wchar_t* pat;

int re_tok_value;

#define CCLMAX 512

static struct ccl {
	int type;
	wchar_t ch1;
	wchar_t ch2;
	} chars[CCLMAX];

static int ccl_next;

static void addccl( int type, wchar_t c1, wchar_t c2 )
{
	if( ccl_next >= CCLMAX - 1)
		longjmp(re_begin,RE_ERR_CCL_NUM);
	chars[ccl_next].type = type;
	chars[ccl_next].ch1 = c1;
	chars[ccl_next].ch2 = c2;
	++ccl_next;
}

int re_is_ccl_member(wchar_t c, register int id)
{
	struct ccl* cp;

	for( cp = chars + id ; cp->type != NIL ; ++cp ) {
		if(cp->type == CHAR) {
			if(cp->ch1 == c)
				return 1;
		}
		else {
			if(cp->ch1 <= c && cp->ch2 >= c)
				return 1;
		}
	}
	return 0;
}

static int genccl( int* id )
{
	int token;
	wchar_t ch;
	register wchar_t* p;

	p = pat;
	*id = ccl_next;
	if(*p == NCCL) {
		token = NCCL;
		p++;
	}
	else
		token = CCL;
	if(*p == RCCL)
		addccl(CHAR,*p++,0);
	for( ; *p != RCCL && *p != 0 ; ++p ) {
		if(*p == ESC && *(p+1) != 0)
			if((ch = *++p) == 'n')
				addccl(CHAR,'\n',0);
			else
				addccl(CHAR,ch,0);
		else if(*(p+1) != RANGE || *(p+2) == RCCL)
			addccl(CHAR,*p,0);
		else {
			addccl(RANGE, *p, *(p+2) );
			p += 2;
		}
	}
	if(*p++ != RCCL)
		longjmp(re_begin,RE_ERR_CCL_TERM);
	pat = p;
	addccl(NIL,0,0);
	return token;
}

int re_get_token( void )
{
	int c;
	static char escs[] = "ftbv\0\f\t\b\v";
	uchar* p;

	switch( c = *pat++) {
	case ANY:
	case LBR:
	case RBR:
	case UNI:
	case CLOSP:
	case CLOS:
	case ZRONE:
	case 0:
		return c;
	case CCL:
		return genccl(&re_tok_value);
	case ESC:
		if((c = *pat) == 0)
			return 0;
		++pat;
		if( c == 'n' )
			return '\n';
		if(p = (uchar*)strchr(escs,c))
			c = *(p+(sizeof(escs)+1)/2);
	default:
		re_tok_value = c;
		return CHAR;
	}
}

wchar_t* re_token_init(register uchar* str)
{
	wchar_t *p;

	ccl_next = 0;
	if((p = pat = (unsigned short *)malloc((strlen((char *)str)+20)*sizeof(wchar_t))) == NULL)
		longjmp(re_begin,RE_ERR_MEMORY);
	if(*str == BOL)
		++str;
	else {
		*p++ = ANY;
		*p++ = CLOS;
	}
	*p++ = LBR;
	while( *str != 0 ) {
		if(iskanji(*str)){
			*p = ((wchar_t)*str << 8) + *(str+1);
			if(*++str == 0) {
				*p = 0;
				break;
			}
			++str;
			++p;
		}
		else {
			*p++ = re_ignore_case ? toupper(*str) : *str;
			++str;
		}
	}
	if(*(str-1) == EOL)
		*(p-1) = RBR;
	else {
		*p++ = RBR;
		*p++ = ANY;
		*p++ = CLOS;
	}
	*p = 0;
	return pat;
}
