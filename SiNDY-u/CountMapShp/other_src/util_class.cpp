/*
 * Copyright (C) INCREMENT P CORP. All Rights Reserved.
 *
 * THIS SOFTWARE IS PROVIDED BY INCREMENT P CORP., WITHOUT WARRANTY OF
 * ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT.
 *
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDER BE LIABLE FOR ANY
 * CLAIM, DAMAGES SUFFERED BY LICENSEE AS A RESULT OF USING, MODIFYING
 * OR DISTRIBUTING THIS SOFTWARE OR ITS DERIVATIVES.
*/

#include "util_class.h"

//------------------------------------------------------
// ����ŕ�����𕪊�����i���̕�����𕪊����Ȃ��j
// @return  ���o����������̌�
//------------------------------------------------------
int util_class::
DivbyToken(	char		*c_cpString,	///< �����Ώە�����
			char		*c_cpToken,		///< ��؂蕶����
			int			c_iStrNum,		///< �擪���牽�Ԗڂ̕����񂩂���o����
			int			c_iN,			///< ���̕���������o����
			string*		c_cpStrPtr)		///< ���o����������̃|�C���^
{
	const int	bmax = 512;
	char	*pStr;                          // ������̃|�C���^
	char	pCopyBuf[bmax];                 // ���o������������R�s�[����o�b�t�@
	int		i=1;                            // ���[�v�̃J�E���g
	int		j=0;                            // ���o����������̃J�E���g
	char	*tmp_str	= NULL;             // �f�[�^���R�s�[���Ă���
	int		len			= strlen(c_cpString);  // ������

	tmp_str     = new char[len+1];
	memset      (tmp_str, '\0', len+1);
	strcpy      (tmp_str, c_cpString);

	while(1){

		// ��������o��
		if(i == 1){
			pStr = strtok(tmp_str, c_cpToken);
		}else{
			pStr = strtok(NULL, c_cpToken);
		}
		if(pStr == NULL){
			delete [] tmp_str;
			return(j);  // ���o����������̌���Ԃ�
		}
		strcpy(pCopyBuf, pStr);     // strtok���Ԃ���������R�s�[����

		if(j == c_iN){
			delete [] tmp_str;
			return(j);  // ���o����������̌���Ԃ�
		}
		if(i < c_iStrNum){    // �擪����m�Ԗڂ���̃f�[�^�̎��o�����s�Ȃ�
			i++;
			continue;
		}
		c_cpStrPtr[j]	= pCopyBuf;
	//	strcpy(c_cpStrPtr[j].eStr, pCopyBuf);  //  ���o������������O���̔z��փR�s�[����
		i++;
		j++;
	}
}

/**
	UNICODE��SJIS�ɕϊ�����
*/
string	util_class::
Str_to_SJIS		(	CComBSTR&	c_CStr,		///< ������(Unicode)
			 		int			c_iMode	)	///< �����ϊ����[�h
{

	USES_CONVERSION;

	string		ret_str;

	if( c_iMode == SJIS ) {
		_bstr_t		a_bTmp1	( c_CStr, false );
		if( (char *)a_bTmp1 == NULL ) {
			ret_str	= "";
		}else {
			ret_str	= (char *)a_bTmp1;
		}
	}else {
		if (c_CStr != 0) {
			ret_str = OLE2T(c_CStr);
		}
	}
	return	(ret_str);
}

//-----------------------------------------------------------
// ���b�V���R�[�h�̃��X�g���쐬����
// @return	����	true
// @return	���s	false
//-----------------------------------------------------------
bool	util_class::
MakeMeshList(	char*		c_cpRFName,	///< ���b�V�����X�g�t�@�C����[IN]
				set<int>*	c_spMList)	///< ���b�V�����X�g�ւ̃|�C���^[OUT]
{
	const int	bmax = 512;
	FILE		*a_fpRFp;		
	char		a_cpRBuf[bmax];
	int			a_iMeshCode;		// ���b�V���R�[�h
	string		a_sDivStr[2];		// ��������������
	int			a_iStrLen = 0;	

	a_fpRFp	= fopen(c_cpRFName, "r");
	if(a_fpRFp == NULL) {
		fprintf	( stderr, "���b�V���e�[�u���t�@�C�� <%s> ���J���܂���!!\n", c_cpRFName);
		return	( false );
	}

	while(fgets(a_cpRBuf, bmax, a_fpRFp) != NULL)
	{
		a_iStrLen	= strlen(a_cpRBuf);
		// �Ō��"\n"����菜��
		if(a_cpRBuf[a_iStrLen - 1] == '\n') {
			a_cpRBuf[a_iStrLen - 1] = '\0';
		}

		// �R�����g�����΂�
		if(a_cpRBuf[0] == '#' || a_cpRBuf[0] == '\0') {
			continue;
		}

		// ���b�V���R�[�h
		a_iMeshCode	= atoi((char*)a_cpRBuf);
		// ���X�g�ɒǉ�
		c_spMList->insert(a_iMeshCode);
	}
	fclose	( a_fpRFp );
	return	( true );
}
