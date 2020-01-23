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

#include "chk_attr_value_tbl.h"

/**
	���O�t�@�C���̐ݒ�
*/
bool	chk_attr_value_tbl::
Log_Init	(	FILE*	c_fpLogFp	)
{
	if( c_fpLogFp != NULL ) {
		e_fpLog	= c_fpLogFp;
	}else {
		return	( false );
	}
	return	( true );
}

/**
	�Ή��e�[�u����ǂݍ���
*/
bool	chk_attr_value_tbl::
read_table	( char*	c_cpRFName )
{
	const int	BUFMAX	= 4096;

	FILE		*a_fpRFp;		
	char		a_cpRBuf[BUFMAX];
	string		a_sDivStr[8];	// ��������������
	int			a_iStrLen = 0;	

	a_fpRFp	= fopen(c_cpRFName, "r");
	if(a_fpRFp == NULL) {
		fprintf	( e_fpLog, "�Ή��e�[�u���t�@�C�� <%s> ���J���܂���!!\n", c_cpRFName );
		return	( false );
	}

	while(fgets(a_cpRBuf, BUFMAX, a_fpRFp) != NULL)
	{

		attr_record	a_aCurRec;

		a_iStrLen	= strlen(a_cpRBuf);
		// �Ō��"\n"����菜��
		if(a_cpRBuf[a_iStrLen - 1] == '\n') {
			a_cpRBuf[a_iStrLen - 1] = '\0';
		}
		// �R�����g�����΂�
		if(a_cpRBuf[0] == '#' || a_cpRBuf[0] == '\0') {
			continue;
		}

		// �������','�ŕ�������
		e_uCu.DivbyToken(a_cpRBuf, "\t", 1, 8, a_sDivStr);

		a_aCurRec.e_iMeshDigit	= atoi(a_sDivStr[0].c_str());
		a_aCurRec.e_sLayerName	= a_sDivStr[1].c_str();
		a_aCurRec.e_sFieldName	= a_sDivStr[2].c_str();
		a_aCurRec.e_sDomainType	= a_sDivStr[3].c_str();
		a_aCurRec.e_sDomainName	= a_sDivStr[4].c_str();
		a_aCurRec.e_sAttrName	= a_sDivStr[5].c_str();
		a_aCurRec.e_lMin		= atoi(a_sDivStr[6].c_str());
		a_aCurRec.e_lMax		= atoi(a_sDivStr[7].c_str());

		int	c = 0;
		while(a_aCurRec.e_sDomainType[c] != '\0' ){
			a_aCurRec.e_sDomainType[c]	= (char)toupper(a_aCurRec.e_sDomainType[c]);
			c++;
		}

		if( a_aCurRec.e_sDomainType != "RANGE" ) {
			a_aCurRec.e_lValue	= a_aCurRec.e_lMin;
		}else {
			a_aCurRec.e_lValue	= 0;
		}

		// ���X�g�ɒǉ�
		e_sAttr_Value_Tbl.insert(a_aCurRec);
	}

	fclose(a_fpRFp);

	return(true);
}

/**
	�Ή��e�[�u����ǂݍ���
*/
bool	chk_attr_value_tbl::
read_note_disp_rule	( char*	c_cpRFName )
{
	const int	BUFMAX	= 4096;

	FILE		*a_fpRFp;		
	char		a_cpRBuf[BUFMAX];
	string		a_sDivStr[20];	// ��������������
	int			a_iStrLen = 0;	

	a_fpRFp	= fopen(c_cpRFName, "r");
	if(a_fpRFp == NULL) {
		fprintf	( e_fpLog, "�Ή��e�[�u���t�@�C�� <%s> ���J���܂���!!\n", c_cpRFName );
		return	( false );
	}

	while(fgets(a_cpRBuf, BUFMAX, a_fpRFp) != NULL)
	{

		note_disp_rule	a_aCurRec;

		a_iStrLen	= strlen(a_cpRBuf);
		// �Ō��"\n"����菜��
		if(a_cpRBuf[a_iStrLen - 1] == '\n') {
			a_cpRBuf[a_iStrLen - 1] = '\0';
		}
		// �R�����g�����΂�
		if(a_cpRBuf[0] == '#' || a_cpRBuf[0] == '\0') {
			continue;
		}

		// �������','�ŕ�������
		e_uCu.DivbyToken(a_cpRBuf, "\t", 1, 20, a_sDivStr);

		int		a_iDispInfo[4]	= {0,0,0,0};

		a_aCurRec.e_lValue		= atoi(a_sDivStr[0].c_str());
		a_aCurRec.e_sName		= a_sDivStr[1].c_str();
		a_aCurRec.e_iDispType	= atoi(a_sDivStr[2].c_str());

		// �\���t���O���o��
		a_iDispInfo[0]			= atoi(a_sDivStr[3].c_str());	// �g�b�v
		a_iDispInfo[1]			= atoi(a_sDivStr[4].c_str());	// �~�h��
		a_iDispInfo[2]			= atoi(a_sDivStr[5].c_str());	// �x�[�X
		a_iDispInfo[3]			= atoi(a_sDivStr[6].c_str());	// �s�s�n�}

		// �t���O�쐬
		if( a_iDispInfo[0] != 0 ) {	a_aCurRec.e_iDispFlag |= DISP_TOP; }
		if( a_iDispInfo[1] != 0 ) {	a_aCurRec.e_iDispFlag |= DISP_MID; }
		if( a_iDispInfo[2] != 0 ) {	a_aCurRec.e_iDispFlag |= DISP_BASE; }
		if( a_iDispInfo[3] != 0 ) {	a_aCurRec.e_iDispFlag |= DISP_CITY; }

		// �\�������o��
		int i = 0;
		for( i = 0; i < 4; i++ )
		{
			a_aCurRec.e_lTopDispRule.e_lDispRule[i]	= atoi(a_sDivStr[7+i].c_str());
			a_aCurRec.e_lMidDispRule.e_lDispRule[i]	= atoi(a_sDivStr[11+i].c_str());
			a_aCurRec.e_lBaseDispRule.e_lDispRule[i]= atoi(a_sDivStr[15+i].c_str());
			a_aCurRec.e_lCityDispRule.e_lDispRule[i]= atoi(a_sDivStr[19].c_str());	// �S���������ɂ���i���܂�Ӗ��͂Ȃ����j
		}

		// ���X�g�ɒǉ�
		e_sNote_Disp_Rule.insert(a_aCurRec);
	}

	fclose(a_fpRFp);

	return(true);
}

bool	chk_attr_value_tbl::
find_data	(	int				c_iMeshDigit,	///< �f�[�^���Y�����郁�b�V������[IN]
				string&			c_sLayerName,	///< ���C����[IN]
				string&			c_sFieldName,	///< �t�B�[���h����[IN]
				int				c_iValue,		///< �����l[IN]
				attr_record*	c_apAttRec )	///< �������R�[�h[OUT]
{
	attr_record	a_sSKey;	// �T�[�`�L�[
	a_sSKey.e_iMeshDigit	= c_iMeshDigit;
	a_sSKey.e_sLayerName	= c_sLayerName;
	a_sSKey.e_sFieldName	= c_sFieldName;
	a_sSKey.e_sDomainType	= "CODED";
	a_sSKey.e_lValue		= c_iValue;

	is_attr_record	a_iIndx;

	a_iIndx	= e_sAttr_Value_Tbl.find ( a_sSKey );
	if( a_iIndx != e_sAttr_Value_Tbl.end() ) {
		*c_apAttRec	= *a_iIndx;
		return	( true );
	}
	// coded�ł݂���Ȃ��ꍇ��range��
	a_sSKey.e_sDomainType	= "RANGE";
	a_sSKey.e_lValue		= 0;		// �_�~�[�ő�����0��
	a_iIndx	= e_sAttr_Value_Tbl.find ( a_sSKey );
	if( a_iIndx != e_sAttr_Value_Tbl.end() ) {
		attr_record	a_aSearched	= *a_iIndx;
		// �͈͓������`�F�b�N����
		if( a_aSearched.e_lMin <= c_iValue && c_iValue <= a_aSearched.e_lMax ) {
			*c_apAttRec	= a_aSearched;
			return	( true );
		}
	}
	return	( false );
}

/**
	�Y�����钍�L�\�����[�������o��
*/
bool	chk_attr_value_tbl::
find_rel	(	int				c_iMeshDigit,	///< �f�[�^���Y�����郁�b�V������[IN]
				int				c_iValue,		///< �����l[IN]
				note_disp_rule*	c_apAttRec )	///< �������R�[�h[OUT]
{
	note_disp_rule		a_sSKey;	// �T�[�`�L�[
	a_sSKey.e_lValue	= c_iValue;

	int	a_iDispFlag	= 0;	// �\���t���O
	// ���b�V����������t���O���쐬
	switch ( c_iMeshDigit )
	{
	case	2:
		a_iDispFlag	= DISP_TOP;
		break;
	case	4:
		a_iDispFlag	= DISP_MID;
		break;
	case	6:
		a_iDispFlag	= DISP_BASE;
		break;
	case	8:
		a_iDispFlag	= DISP_CITY;
		break;
	}

	pair< ims_note_disp_rule, ims_note_disp_rule >	a_piIndx_pair;

	ims_note_disp_rule	a_iIndx;

	a_piIndx_pair	= e_sNote_Disp_Rule.equal_range ( a_sSKey );
	
	for( a_iIndx = a_piIndx_pair.first; a_iIndx != a_piIndx_pair.second; a_iIndx++ )
	{
		if( (*a_iIndx).e_iDispFlag & a_iDispFlag ) {
			*c_apAttRec	= *a_iIndx;
			return ( true );
		}
	}
	return	( false );
}
