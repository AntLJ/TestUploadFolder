/****
 * rexp.h
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

/* This file(rexp.h) was modified from the original rexp.h.
The modifications are as follows.

(1)use extern  __declspec(dllimport)  for the import functions.

									 2001/07/14 Tomoichi Ebata
*/

#if !(_MSC_VER >= 600)
#define _cdecl
#endif

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

#ifdef REXPDLL_EXPORTS
#define REXPLIB_API __declspec(dllimport)
#else
#define REXPLIB_API
#endif

extern  REXPLIB_API unsigned char* _cdecl re_comp( unsigned char* pat );

extern  REXPLIB_API unsigned char* _cdecl re_compv( unsigned char** patv );

extern  REXPLIB_API void _cdecl re_init_pattern( void );
extern  REXPLIB_API unsigned _cdecl re_add_pattern( unsigned char* pat );
extern  REXPLIB_API void _cdecl re_end_pattern( void );

extern  REXPLIB_API int _cdecl re_exec( unsigned char* str );

extern int re_ignore_case;
extern int re_errno;
