/*
 * 	rexpDLL.c
 *	DLL interface for rexp
 *	1993 Sey
 *	
 ***********************************************************************
	このファイルは、DUDEさんのrexpパッケージをWINDOWS DLLとして使うため
	のインターフェースです。ご覧のように単にDLLとして呼び出せるようにし
	ただけです。rexpはPublicDomainであり、このファイルもPublicDomainとし
	ます。
	
	付属のmakefileはオリジナルのままで、通常のDOSのlibファイルを作るため
	のものです。DLLを作る場合は、
	bcc -WDE -ml rexpdll.c parse.c rexp.c state.c token.c    
	としてください。
	リンク時にデータ領域が64Kを超えるというエラーが出るようなら、rexpdef.
	hの中の DFA_TRANS_MAX マクロの値を減らしてください。
	
	VisualBasicからこのDLLを使う場合は、次のような宣言をしてください。

	Declare Function ReComp Lib "rexpdll.dll" (ByVal pat As String) As Integer
	Declare Sub ReInitPattern Lib "rexpdll.dll" ()
	Declare Function ReAddPattern Lib "rexpdll.dll" (ByVal pat As String) As Integer
	Declare Sub ReEndPattern Lib "rexpdll.dll" ()
	Declare Function ReExec Lib "rexpdll.dll" (ByVal text As String) As Integer
	Declare Sub ReIgnoreCase Lib "rexpdll.dll" (ByVal value As Integer)
	Declare Function ReErrno Lib "rexpdll.dll" () As Integer
	
	各ルーティンの使い方はREXP.MANを見てください。VBからCのchar **argvの
	形の引数をDLLへ渡すことができないので、re_compvは使えません。
 ***********************************************************************
 */

#include <windows.h>
#include "rexpdef.h"

int ReComp( char* pat )
{
	return re_comp(pat) != NULL;
}

/* end ReComp */

int  ReCompv( char** patv )
{
	return re_compv(patv) != NULL;
}
/* end ReCompv */

void ReInitPattern(void)
{
	re_init_pattern();
}
/* end ReInitPattern */

unsigned ReAddPattern( char* pat )
{
	return re_add_pattern(pat);
}
/* end ReAddPattern */

void ReEndPattern(void)
{
	re_end_pattern();
}
/* end ReEndPattern */

int ReExec(char* str)
{
	return re_exec(str);
}
/* end ReExec */

void ReIgnoreCase(int value)
{
	re_ignore_case = value;
}
/* end ReIgnoreCase */

int ReErrno(void)
{
	return re_errno;
}
/* end ReErrno */

int FAR PASCAL LibMain(HANDLE hInstance, WORD wDataSeg, WORD  cbHeapSize,
					   LPSTR lpCmdLine)
{
    return 1;
}
/* end LibMain */

int FAR PASCAL WEP(int nParameter)
	{
    return 1;
	}
/* end WEP */

/* end source rexpdll.c */
