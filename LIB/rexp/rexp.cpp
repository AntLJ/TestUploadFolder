/***
 * rexp.c
 *
 *  regular expression main routine
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
 * 0.01 1990/10/20 speed up
 * 0.02 1990/10/21 modify ^ and $ operators
 * 0.03 1990/10/21 BM-Search mode for non-regular expression
 * 0.04 1990/10/25 Delete auto structure initialization
 * 0.05 1990/10/26 Add BOL and EOL to isrexp
 * 0.06 1990/10/27 Modify BOL and EOL handling (delete ANY_S)
 * 0.07 1990/10/27 Add re_compv() for multi pattern
 * 0.08 1991/06/03 Previous expression checking bug fix (for BM-Search)
 * 0.09 1991/06/03 Modify pre_search for re_ignore_case
 * 0.10 1991/06/03 Speed up pre_search by BM
 * 0.11 1991/06/04 Refer to previous expression if given pattern is ""
 * 0.12 1991/06/05 Combine pre_search and bm_exec into bm_search
 * 0.13 1991/06/05 Modify re_ignore_case and support it in CCL
 * 0.14 1991/06/05 Substitute is_kanji2 for nth_ctype and chk_ctype
 *
 *****************************************************************************/

/* This file(rexp.cpp) was modified from the original rexp.c.
The modifications are as follows.

(1)Renamed "rexp.cpp". 

(2)Used <ctype.h> instead of <jctype.h>
  
(3)According to iskanji(),iskanji2() function, the following macros
   were defined in "rexpdef.h" 

   #define iskanji(c)  ((( 0x81 <= (c)) && ((c) <= 0x9f)) ||
                        (( 0xe0 <= (c)) && ((c) <= 0xef)))
   #define iskanji2(c) ((( 0x40 <= (c)) && ((c) <= 0x7e)) ||
                        (( 0x80 <= (c)) && ((c) <= 0xfc)))

(4)In order for compiling, added some uchar(unsigned char) casts 
 
									 2001/07/14 Tomoichi Ebata
*/ 

#include <string.h>
#include <stdlib.h>
#include <limits.h>
//#include <jctype.h>
#include <ctype.h>
#include "rexpdef.h"

int re_ignore_case = 0;
int re_errno = 0;
jmp_buf re_begin;



static char* comp_errmsg[] = {"No previous regular expression",
				"Character class buffer overflow",
				"Character class parenthes not balanced",
				"Out of Memory",
				"Parenthes not balanced",
				"Syntax error in regular expression"
};

#define DMAPVAL 881
struct dfa_stat {
	unsigned long id;
	int next_st;
	struct dfa_stat* next;
};
static struct dfa_stat* index[DMAPVAL];	/* index of direct mapping cache */
static int x_num = 0;
static int x_ovflw = 0;
static struct dfa_stat x_tbl[DFA_TRANS_MAX];

static int use_bm = 0;

static uchar* bm_search( uchar* );
static int bm_search_word_len;
static uchar bm_search_word[RE_MAX];

static int is_kanji2(unsigned char* string, register int len)
{
	register int flg;
	register unsigned char*p = string + len;

	if(!iskanji2(*p--))
	    return 0;
	flg = 0;
	while( --len >= 0 ) {
	    if(!iskanji(*p--))
		break;
	    flg = !flg;
	}
	return flg;
}

static int match(wchar_t c, int s )
{
	struct states* st;

	switch((st=re_state(s))->type) {
	case ANY:
		return  1;
	case CHAR:
		if(re_ignore_case)
		    return (c >= 0x100 ? c : toupper(c)) == (wchar_t)st->value;
		else
		    return c == (wchar_t)st->value;
	case CCL:
		if(re_ignore_case)
		    return re_is_ccl_member(c >= 0x100 ? c : toupper(c), st->value);
		else
		    return re_is_ccl_member(c,st->value);
	case NCCL:
		if(re_ignore_case)
		    return !re_is_ccl_member(c >= 0x100 ? c : toupper(c) ,st->value);
		else
		    return !re_is_ccl_member(c,st->value);
	default:
		longjmp(re_begin, RE_ERR_UNKNOWN);
	}
}

static struct dfa_stat* stalloc( void )
{
	if(x_num >= DFA_TRANS_MAX-1)
		x_ovflw = 1;
	return x_tbl + x_num++;
}

static int add_DFA_state( int s1, wchar_t ch, int s2 )
{
	unsigned long id;
	int h;
	register struct dfa_stat *p;

	if(x_ovflw)
		return s2;
#if INT_MAX == SHRT_MAX
	id = *(unsigned long*)&s1;
#else
	id = ((unsigned long)ch << 16L) + (unsigned long)s1;
#endif
	if( (p = index[h = (int)(id % DMAPVAL)]) == NULL ) {
		if((p = index[h] = stalloc()) == NULL)
			return s2;
		p->id = id;
		p->next = NULL;
		return p->next_st = s2;
	}
	for( ; ; p = p->next ) {
		if(p->id == id)
			return s2;
		else if( p->next  == NULL ) {
			if((p->next = stalloc()) == NULL)
				return s2;
			p->next->id = id;
			p->next->next = NULL;
			return p->next->next_st = s2;
		}
	}
}

int _cdecl re_exec( uchar* str )
{
	wchar_t c;
	int cur_S, DFA_S[NFA_MAX];
	register uchar* p;
	unsigned long id;
	register struct dfa_stat* sp;
	register int i,*N_ptr;
	unsigned final = 0;

	if(use_bm)
		return *bm_search( str ) != '\0';
	if((re_errno = setjmp(re_begin)) == 0) {
		cur_S = 0;
		for( p = bm_search_word_len ? bm_search(str) : str ; *p ; ++p ) {
			if(iskanji(*p)) {
				c = ((wchar_t)*p << 8) + *(p+1);
				if(*++p == 0)
					break;
			}
			else if( *p == '\n' ) {
				final |= isfinal(cur_S);
				cur_S = 0;
				continue;
			}
			else
				c = *p;
			id = ((unsigned long)c << 16L) + (unsigned long)cur_S;
			sp = index[id % DMAPVAL];
			for( ; sp ; sp = sp->next )
				if(sp->id == id) {
					cur_S = sp->next_st;
					break;
				}
			if(sp == NULL) {
				for(i = 0, N_ptr = re_N_top(cur_S); *N_ptr != NIL ; ++N_ptr)
					if(match(c,*N_ptr))
						DFA_S[i++] = *N_ptr;
				DFA_S[i] = NIL;
				cur_S = add_DFA_state(cur_S,c,re_state_num(DFA_S));
			}
		}
		return isfinal(cur_S) | final;
        }
	else
		return 0;
}

static void regexp_init( void )
{
	x_num = 0;
	x_ovflw = 0;
	memset(index,0,sizeof(index));
}

#define ismeta( c ) (c == ANY || c == LBR || c == UNI || c == CLOS || c == CLOSP || c == ZRONE || c == CCL || c == BOL || c == EOL)

#define ispostfixop( c )    (c == CLOS || c == CLOSP || c == ZRONE)

static int isrexp( uchar* pat )
{
	for( ; *pat ; ++pat ) {
		if(ismeta(*pat))
			return 1;
		else if( *pat == ESC ) {
			if(*++pat == 0)
				return 0;
			else if(iskanji(*pat)) {
				if(*++pat == 0)
					return 0;
			}
		}
		else {
			if(iskanji(*pat))
				if(*++pat == 0)
					return 0;
		}
	}
	return 0;
}

static int pat_shift[0x100];

static void bm_comp( uchar* pat )
{
    int i;
    int len = strlen((char *)pat);
    uchar* p;

    bm_search_word_len = len;
    strcpy((char *)bm_search_word, (char *)pat );
    for (i = 0; i < sizeof(pat_shift)/sizeof(pat_shift[0]); i++)
	pat_shift[i] = len;
    for (p = pat ; *p ; p++)
	pat_shift[*p] = --len;
}

static void pre_comp( uchar* pat)
{
	int i;
	register uchar * p;
	int len;

	for( i = 0, p = pat ; *p ; ++p ) {
		if(ismeta(*p)) {
			if(strchr((char *)p,UNI))
				i = 0;
			else if(ispostfixop(*p))
				--i;
			break;
		}
		else if( *p == ESC ) {
			if(*++p == 0)
				break;
			else if(iskanji(*p)) {
				bm_search_word[i++] = *p;
				bm_search_word[i++] = *++p;
				if(*p == 0)
					break;
			}
			else
				bm_search_word[i++] = re_ignore_case ? toupper(*p) : *p;
		}
		else {
			if(iskanji(*p)) {
				bm_search_word[i++] = *p;
				bm_search_word[i++] = *++p;
				if(*p == 0)
					break;
			}
			else
				bm_search_word[i++] = re_ignore_case ? toupper(*p) : *p;
		}
	}
	if( i <= 0 )
		bm_search_word_len = 0;
	else {
		bm_search_word[i] = 0;
		if(bm_search_word_len = i) {
		    len = bm_search_word_len;		/* same as bm_comp */
		    for (i = 0; i < sizeof(pat_shift)/sizeof(pat_shift[0]); i++)
			pat_shift[i] = len;
		    for (p = bm_search_word ; *p ; p++)
			pat_shift[*p] = --len;
		}
	}
}

#ifdef toupper
#undef toupper
#endif

static uchar* bm_search( uchar* line )
{
    uchar * pos = line + bm_search_word_len - 1;
    uchar * eop = line + strlen((char *)line) - 1;
    uchar * p, * q;
    int skip;

    if(re_ignore_case) {
        while( 1 ) {
	    while (pos <= eop && (skip = pat_shift[toupper(*pos)]) > 0)
		pos += skip;
	    if (pos > eop)
		return NULL;
	    for (skip = bm_search_word_len - 1, p = pos, q = bm_search_word + skip ; skip != 0 && toupper(*--p) == *--q ; --skip)
		;
	    if (skip == 0 && !is_kanji2(line, p-line))
		return p;
	    if ((p += pat_shift[toupper(*p)]) > pos)
		pos = p;
    	    else
		pos++;
	}
    }
    else {
        while( 1 ) {
	    while (pos <= eop && (skip = pat_shift[*pos]) > 0)
		pos += skip;
	    if (pos > eop)
		return NULL;
	    for (skip = bm_search_word_len - 1, p = pos, q = bm_search_word + skip ; skip != 0 && *--p == *--q ; --skip)
		;
	    if (skip == 0 && !is_kanji2(line, p-line))
		return p;
	    if ((p += pat_shift[*p]) > pos)
		pos = p;
	    else
		pos++;
	}
    }
}

static int pat_num;

void _cdecl re_init_pattern( void )
{
	pat_num = 0;
	re_parse_init();
	re_state_init();
	regexp_init();
	use_bm = 0;
}

unsigned _cdecl re_add_pattern( uchar* pat )
{
	if(pat_num == 0)
		pre_comp( pat );
	++pat_num;
	return re_parse_add(pat);
}

void _cdecl re_end_pattern( void )
{
	int S0[2];

	if(pat_num > 1)
		bm_search_word_len = 0;
	S0[0] = 0;
	S0[1] = NIL;
	re_state_num(S0);
}

uchar* _cdecl re_comp( uchar* pat )
{
	static int compiled = 0;
	uchar* msg = NULL;

	if(pat == NULL || *pat == '\0')
		msg = compiled ? NULL : (uchar*)comp_errmsg[0];
	else {
		if(isrexp(pat)) {
			re_init_pattern();
			re_add_pattern( pat );
			re_end_pattern();
			if(re_errno)
				msg = (uchar*)comp_errmsg[re_errno];
			else
				compiled = 1;
		}
		else {
			bm_comp(pat);
			compiled = 1;
			use_bm = 1;
		}
	}
	return msg;
}

uchar* _cdecl re_compv( uchar** patv )
{
	static int compiled = 0;
	uchar* msg = NULL;

	if(patv == NULL)
		msg = compiled ? NULL : (uchar*)comp_errmsg[0];
	else {
		re_init_pattern();
		for( ; *patv ; ++patv ) {
			re_add_pattern(*patv);
			if(re_errno)
				break;
		}
		re_end_pattern();
		if(re_errno)
			msg = (uchar*)comp_errmsg[re_errno];
		else
			compiled = 1;
	}
	return msg;
}
