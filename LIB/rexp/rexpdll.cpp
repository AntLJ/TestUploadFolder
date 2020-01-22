/*
 * 	rexpDLL.c
 *	DLL interface for rexp
 *	1993 Sey
 *	
 ***********************************************************************
	���̃t�@�C���́ADUDE�����rexp�p�b�P�[�W��WINDOWS DLL�Ƃ��Ďg������
	�̃C���^�[�t�F�[�X�ł��B�����̂悤�ɒP��DLL�Ƃ��ČĂяo����悤�ɂ�
	�������ł��Brexp��PublicDomain�ł���A���̃t�@�C����PublicDomain�Ƃ�
	�܂��B
	
	�t����makefile�̓I���W�i���̂܂܂ŁA�ʏ��DOS��lib�t�@�C������邽��
	�̂��̂ł��BDLL�����ꍇ�́A
	bcc -WDE -ml rexpdll.c parse.c rexp.c state.c token.c    
	�Ƃ��Ă��������B
	�����N���Ƀf�[�^�̈悪64K�𒴂���Ƃ����G���[���o��悤�Ȃ�Arexpdef.
	h�̒��� DFA_TRANS_MAX �}�N���̒l�����炵�Ă��������B
	
	VisualBasic���炱��DLL���g���ꍇ�́A���̂悤�Ȑ錾�����Ă��������B

	Declare Function ReComp Lib "rexpdll.dll" (ByVal pat As String) As Integer
	Declare Sub ReInitPattern Lib "rexpdll.dll" ()
	Declare Function ReAddPattern Lib "rexpdll.dll" (ByVal pat As String) As Integer
	Declare Sub ReEndPattern Lib "rexpdll.dll" ()
	Declare Function ReExec Lib "rexpdll.dll" (ByVal text As String) As Integer
	Declare Sub ReIgnoreCase Lib "rexpdll.dll" (ByVal value As Integer)
	Declare Function ReErrno Lib "rexpdll.dll" () As Integer
	
	�e���[�e�B���̎g������REXP.MAN�����Ă��������BVB����C��char **argv��
	�`�̈�����DLL�֓n�����Ƃ��ł��Ȃ��̂ŁAre_compv�͎g���܂���B
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
