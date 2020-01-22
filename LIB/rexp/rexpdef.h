/****
 * rexpdef.h
 *
 *  Fast regular expression library for Japanese
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
 *
 *****************************************************************************/


/* This file(rexpdef.h) was modified from the original rexpdef.h.
The modifications are as follows.

(1)According to iskanji(),iskanji2() function, the following macros
   were defined in "rexpdef.h" 

   #define iskanji(c)  ((( 0x81 <= (c)) && ((c) <= 0x9f)) ||
                        (( 0xe0 <= (c)) && ((c) <= 0xef)))
   #define iskanji2(c) ((( 0x40 <= (c)) && ((c) <= 0x7e)) ||
                        (( 0x80 <= (c)) && ((c) <= 0xfc)))

(2)use __declspec(dllexport) for the export functions.

									 2001/07/14 Tomoichi Ebata
*/ 

#include <stdio.h>
#include <setjmp.h>

#if !(_MSC_VER >= 600)
#define _cdecl
#endif

#define iskanji(c)  ((( 0x81 <= (c)) && ((c) <= 0x9f)) || (( 0xe0 <= (c)) && ((c) <= 0xef)))
#define iskanji2(c) ((( 0x40 <= (c)) && ((c) <= 0x7e)) || (( 0x80 <= (c)) && ((c) <= 0xfc)))


#define ANY	'.'
#define CCL	'['
#define RCCL	']'
#define NCCL	'^'
#define RANGE	'-'
#define ESC	'\\'
#define LBR	'('
#define RBR	')'
#define UNI	'|'
#define CLOS	'*'
#define CLOSP	'+'
#define ZRONE	'?'
#define BOL	'^'
#define EOL	'$'

#define CHAR	1
#define CAT		2
#define BRANCH	3
#define isliteral(x)	((x) == CHAR || (x) == ANY || (x) == CCL || (x) == NCCL)

#define RE_MAX		256		/* max size of regexp. */
#define NFA_MAX		(RE_MAX*2)	/* max num of NFA states */
#define DFA_MAX 	512		/* max num of DFA states */
#define TAB_MAX		2048		/* max size of NFA -> DFA table */
#define DFA_TRANS_MAX	4096		/* max size of DFA transition table */

#define NIL	(-1)

struct states {
	int last;
	int lson;
	int rson;
	int type;
	int value;
};

#define wchar_t		unsigned short		/* for TC++ */
typedef unsigned char  uchar;

enum re_err {	RE_ERR_NONE,		/* No error */
		RE_ERR_CCL_NUM,		/* Too many chars in CCL */
		RE_ERR_CCL_TERM,	/* CCL not properly terminated */
		RE_ERR_MEMORY,		/* Out of memory */
		RE_ERR_PAR,		/* Parentheses not balanced */
		RE_ERR_SYNTAX,		/* Syntax error */
		RE_ERR_TABLE,		/* Overflow NFA -> DFA table */
		RE_ERR_DFA_STAT,	/* Too many DFA states */
		RE_ERR_UNKNOWN,		/* Unknown error */
};

wchar_t* re_token_init(uchar* pat);
void re_state_init( void );
int re_get_token( void );

void re_parse_init( void );
unsigned re_parse_add( uchar* pat );

struct states* re_state(int st_num);
int re_mkleaf(int typ, int val);
int re_mknode( int op, int left, int right );

int re_is_ccl_member(wchar_t c, int id);
#define isfinal( state )    re_F_state[state]
int re_state_num( int* S );
int* re_N_top( int state );

extern jmp_buf re_begin;
extern unsigned re_F_state[];

/* for public */

__declspec(dllexport)  uchar* _cdecl re_comp( uchar* pat );

__declspec(dllexport)  uchar* _cdecl re_compv( uchar** patv );

__declspec(dllexport)  void _cdecl re_init_pattern( void );
__declspec(dllexport)  unsigned _cdecl re_add_pattern( uchar* pat );
__declspec(dllexport)  void _cdecl re_end_pattern( void );

__declspec(dllexport)  int _cdecl re_exec( uchar* str );

extern int re_ignore_case;
extern int re_errno;
