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

/*
**	SiINClassTable
**
**		IN class table class
**
**		Programmed by H.Miyahara
*/
#ifdef WIN32
#pragma warning(disable:4786)
#endif

//#include "stdafx.h"
#include "SiINClassTable.h"
#include "Smp_db_obj_attr.h"


/*
**	static variables
*/

multiset<SiINClassTable::Peace, SiINClassTable::lessC>	SiINClassTable::mTable;


/*
**	inner data
*/
/*
struct INClass
{
	char	eSiLayerName[11];	///< SiNDY���C������
	int		eSiClass;			///< SiNDY��ʃR�[�h
	int		eINClass;			///< IN���
	char*	eName;
};
*/


/**
	��ʖ��̂����o��
*/
/*
const char* SiINClassTable::
GetName	(	char*	cFieldName,	/// SiNDY�t�B�[���h����
			int		cClass)		///<�@��ʃR�[�h
{
	/*
	if(0 == mTable.size())
	{
		for(int i=0 ; gClasses[i].eSiClass ; ++i)
		{
			mTable.insert(Peace(gClasses[i].eSiClass, i));
		}
	}
	*//*
	Peace	aSearch_Tmp;
	aSearch_Tmp.setData	(cFieldName, cClass);
#ifdef	WIN32
	set<Peace, lessC>::iterator	i = mTable.find(aSearch_Tmp);
#else
	set<Peace>::iterator			i = mTable.find(aSearch_Tmp);
#endif
	if(i == mTable.end()) return 0;

	//return gClasses[(*i).eIndex].eName;
	Peace	ret_tbl	= *i;			// �Ԃ��e�[�u��
	return	(ret_tbl.eName);
}
*/


/**
	�t�B�[�`������f�[�^���Z�b�g����
	���S�e�[�u�� = 0, �s�s�n�} = 3x, �x�[�X = 2x, �~�h�� = 1x
	@return	����	true
	@return	���s	false
*/
bool	SiINClassTable::Peace::
Set_Data	(	_IRowPtr	c_Feature,			///< �t�F�b�`���Ă����t�B�[�`��[IN]
			 	int			c_iData_Level	)	///< �f�[�^�̃��x��
{
	LONG			a_Col_Size	= 0; 
	IFieldsPtr		a_Col_Defs;					// �J�����̒�`
	c_Feature->get_Fields		( &a_Col_Defs );
	a_Col_Defs->get_FieldCount	( &a_Col_Size );
	
	LONG		i = 0;
	for( i = 0 ; i < a_Col_Size ; i++ )
	{
		IFieldPtr	a_Col_Def;
		CComBSTR	a_Col_Name;
		CComVariant	a_Value;
		a_Col_Defs->get_Field	( i, &a_Col_Def );
		a_Col_Name.Empty();

		esriFieldType	a_Col_Type;				// �t�B�[���h�^�C�v
		a_Col_Def->get_Type	( &a_Col_Type );	// �J�����^�C�v
		a_Col_Def->get_Name	( &a_Col_Name );	// �J��������
		c_Feature->get_Value( i, &a_Value );	// �����l�̎擾

		a_Col_Name.ToUpper(); // postgre�Ή�

		if(a_Col_Name == "OBJECTID") {			// �I�u�W�F�N�gID
			eIndex			= a_Value.intVal;
		}else
		if(a_Col_Name == "CLASS"){				// SiNDY���
			eSiClass		= a_Value.intVal;
		}else
		if(a_Col_Name == "CLASS_NAME") {		// ��ʖ���
			string			a_sName_Str;
			a_sName_Str		= Str_to_SJIS ((CComBSTR *)(&a_Value.bstrVal));
			strcpy_s		( eName, &a_sName_Str[0] );
		}else
		if(a_Col_Name == "LAYERID") {			// ���C��ID
			int	a_iLID		= a_Value.intVal;
			int a_iModLID	= a_iLID / 10;
			if( c_iData_Level != 0 && (a_iModLID != c_iData_Level) ) {	// �ړI�̃f�[�^���x���ȊO�͔�΂�
				// �g�b�v�}�b�v�p 2003.01.09
				if( !(c_iData_Level == top && a_iModLID == 0 ) ){
					// �g�b�v�}�b�v�p�����܂�
					return	( false );
				}
			}
			/////////////////////////////////////////
			string		a_sFName;
			int			a_iTmpType;
			if( !Get_Field_DType	( a_iLID, &a_sFName, &a_iTmpType ) ) {
			//	fprintf	( stderr, "�����擾�Ɏ��s\n");
			//	fprintf	( stderr, "���Ή��̃��C���ł�<%s>\n", &a_stmpstr[0]);
				return	( false );
			}
			eData_Type		= a_iTmpType;
			eSiFieldName	= a_sFName;
			// 2010.08.18 �ǉ�
			switch (c_iData_Level)
			{
			case	top:	m_Top_ExF	= 1;	break;
			case	mid:	m_Mid_ExF	= 1;	break;
			case	base:	m_Base_ExF	= 1;	break;
			case	city:	m_City_ExF	= 1;	break;
			}

			/////////////////////////////////////////
		/*	IDomainPtr	a_ipDomain;
			a_Col_Def->get_Domain(&a_ipDomain);
			ICodedValueDomainPtr ipCodeDomain(a_ipDomain);
			long		aCount;
			ipCodeDomain->get_CodeCount(&aCount);
			for(int j = 0; j < aCount; j++)
			{
				CComVariant	a_cTmpValue;
				CComBSTR	aName;
				string		a_sFName;
				int			a_iTmpType;

				ipCodeDomain->get_Value	( j, &a_cTmpValue );
				ipCodeDomain->get_Name	( j, &aName );
				if( a_iLID == a_cTmpValue.intVal ) {
					string	a_stmpstr	= Str_to_SJIS ( (CComBSTR *)(&aName) );
					if( !Get_Field_DType	( a_iLID, &a_sFName, &a_iTmpType ) ) {
					//	fprintf	( stderr, "�����擾�Ɏ��s\n");
						fprintf	( stderr, "���Ή��̃��C���ł�<%s>\n", &a_stmpstr[0]);
						return	( false );
					}
					eData_Type		= a_iTmpType;
					eSiFieldName	= a_sFName;
					break;
				}
			}*/
			////////////////////////////////////////
		}else
		if(a_Col_Name == "UNDERGROUND_F") {		// �n��n���t���O
			eUnder_Flag		= a_Value.intVal;
		}else
		if(a_Col_Name == "IN_CLASS") {			// IN���
			string			a_sIN_Class_Str;
			char**			gomistr = NULL;
			a_sIN_Class_Str	= Str_to_SJIS ((CComBSTR *)(&a_Value.bstrVal));
			eINClass		= strtoul(&a_sIN_Class_Str[0], gomistr, 16);
			// 0x00000000 �̎�ʂ̂��̂̓e�[�u���ɂ͒ǉ����Ȃ� 2003.04.14
			if( eINClass == 0x00000000 ) {
			//	fprintf	( stderr, "��������:<%s>\t0x%08x\tIN�����l�������̂łƂ΂��܂�\n", &eName[0], eINClass ); 
			//	return	( false ); 2007.01.11 �R�����g�A�E�g
			}
		}else
		if(a_Col_Name == "TOP_FLAG") {
			m_Top_ExF	=	a_Value.intVal;
		}else
		if(a_Col_Name == "MIDDLE_FLAG") {
			m_Mid_ExF	=	a_Value.intVal;
		}else
		if(a_Col_Name == "BASE_FLAG") {
			m_Base_ExF	=	a_Value.intVal;
		}else
		if(a_Col_Name == "CITY_FLAG") {
			m_City_ExF	=	a_Value.intVal;
		}

#ifdef	DB_ROT_RULE	// 2006.10.12
		else	// 2006.10.11 ���L�̉�]���[����DB����擾���邽�ߏC��
		if(a_Col_Name == "ROT_RULE" ) {
			string			a_sRot_Rule;
			a_sRot_Rule		= Str_to_SJIS ((CComBSTR *)(&a_Value.bstrVal));
			if( a_sRot_Rule.size() != 12 ) {
				return	( false );
			}
			strncpy ( eRot_Rule, (char*)a_sRot_Rule.c_str(), 12 );
		}
#endif
	}
	return	( true );
}

/**
	Unicode�̕����񂩂�SJIS�ɂ���
*/
string	SiINClassTable::Peace::
Str_to_SJIS		(	CComBSTR*	c_CStr	)		// ������(Unicode)
{
	string		ret_str;

	USES_CONVERSION;
	if (*c_CStr != 0) {
		_bstr_t		a_bTmp1	( (*c_CStr), false );
		if( (char *)a_bTmp1 == NULL ) {
			ret_str	= "";
		}else {
			ret_str	= (char *)a_bTmp1;
		}
	}
	return	(ret_str);
}

/**
	���C��ID����A�����̃t�B�[���h���ƃf�[�^�^�C�v�l������
	@return	����	true
	@return	���s	false
*/
bool	SiINClassTable::Peace::
Get_Field_DType	(	int			c_sLayer_ID,	///< ���C��ID[IN]
					string*		c_spFName,		///< �t�B�[���h��[OUT]
					int*		c_ipDType	)	///< �f�[�^�^�C�v[OUT]
{
	switch	( c_sLayer_ID )
	{
	case	0:		// "common"
		*c_spFName	= "";
		*c_ipDType	= 0;
		break;
	case	1:		// "top_line" 2003.01.05 �ǉ�
	case	11:		// "middle_line"
	case	21:		// "base_line" 
	case	31:		// "city_line"
		*c_spFName	= BG_C;
		*c_ipDType	= TYPE_LINE;
		break;
	case	12:		// "middle_railway"
	case	22:		// "base_railway"
	case	32:		// "city_railway" 
		*c_spFName	= RAIL_C;
		*c_ipDType	= TYPE_LINE;
		break;
	case	13:		// "middle_station"
	case	23:		// "base_station"
	case	33:		// "city_station" 
		*c_spFName	= STAT_C;
		*c_ipDType	= TYPE_POLY;
		break;
	case	4:		// "top_site" 2003.01.05 �ǉ�
	case	14:		// "middle_site"
	case	24:		// "base_site"
	case	34:		// "city_site"
		*c_spFName	= BG_C;
		*c_ipDType	= TYPE_POLY;
		break;
	case	5:		// "top_contour" 2003.01.05 �ǉ�
	case	15:		// "middle_contour"
	case	25:		// "base_contour"
		*c_spFName	= CNT_C;
		*c_ipDType	= TYPE_POLY;
		break;
	case	35:		// "building"
		*c_spFName	= BLD_C;
		*c_ipDType	= TYPE_POLY;
		break;
	case	36:		// building_line
		*c_spFName	= BLD_C;
		*c_ipDType	= TYPE_LINE;
		break;
	default:
		return	( false );
	}
	return	( true );
}

//===================================================================
// ��������@SiINClassTable�̒�`
//===================================================================

/**
	��ʖ��̂����o��
*/
const int SiINClassTable::
GetINClassCode	(	string	cFieldName,	// SiNDY�t�B�[���h����
					int		cData_Type,	// �f�[�^�^�C�v
					int		cUnder_Flag,// �n��E�n���t���O(0:�n��A1:�n���j
					int		cSiClass )	// ��ʃR�[�h
{
	/*
	if(0 == mTable.size())	{
		for(int i=0 ; gClasses[i].eSiClass ; ++i)
		{
			mTable.insert(Peace(gClasses[i].eSiClass, i));
		}
	}
	*/
	Peace	aSearch_Tmp;
	/*char	pName[256];
	int		lsize = cName.size();
	if(lsize > 256) {
		return 0;
	}
	strncpy(pName, &cName[0], 256);
	*/
	 aSearch_Tmp.setData	(cFieldName, cData_Type, cUnder_Flag, cSiClass);
#ifdef	WIN32
	multiset<Peace, lessC>::iterator	i = mTable.find(aSearch_Tmp);
#else
	multiset<Peace>::iterator			i = mTable.find(aSearch_Tmp);
#endif
	if(i == mTable.end()) {
		return 0;
	}
	Peace	ret_tbl	= *i;			// �Ԃ��e�[�u��
	return	(ret_tbl.eINClass);
}

/**
	SiNDY�̏�񂩂�IN�����̑Ή��������o��
*/
SiINClassTable::Peace		SiINClassTable::
GetSiToINInfo	(	string	cFieldName,	// SiNDY�t�B�[���h����
					int		cData_Type,	// �f�[�^�^�C�v
					int		cUnder_Flag,// �n��E�n���t���O(0:�n��A1:�n���j
					int		cSiClass )	// ��ʃR�[�h
{
	Peace	aSearch_Tmp;
	aSearch_Tmp.setData	(cFieldName, cData_Type, cUnder_Flag, cSiClass);
#ifdef	WIN32
	multiset<Peace, lessC>::iterator	i = mTable.find(aSearch_Tmp);
#else
	multiset<Peace>::iterator			i = mTable.find(aSearch_Tmp);
#endif
	Peace	ret_tbl;			// �Ԃ��e�[�u��
	if(i != mTable.end()) {
		// 2007.01.10
		ret_tbl	= *i;
	}
	return	(ret_tbl);
}

/**
	�Ή��e�[�u���ǂݍ���
*/

/**
	Sindy�AIN�����Ή��e�[�u���̔z����쐬����
	@return �Ԃ�l	���� = 0, ���s = -1
*/
int		SiINClassTable::
Read_Table	(	char	*c_RFName)	// Sindy�AIN�����Ή��e�[�u���t�@�C����
{
	const	int		bmax		= 256;
	FILE			*pRFp;		
	char			pRBuf[bmax];
	int				aStrLen		= 0;

	pRFp	= fopen(c_RFName, "r");
	if(pRFp == NULL) {
		fprintf	(stderr, "Sindy�AIN�����Ή��e�[�u���t�@�C�� <%s> ���J���܂���!!\n", c_RFName);
		return	(-1);
	}
/**
	if(0 == mTable.size())
	{
		for(int i=0 ; gClasses[i].eSiClass ; ++i)
		{
			mTable.insert(Peace(gClasses[i].eSiClass, i));
		}
	}
	*/
	
	int		i = 0;
	char*	cut_str	= ",\t";
	while(fgets(pRBuf, bmax, pRFp) != NULL)
	{
#ifdef	DB_ROT_RULE	// 2006.10.12
		char		*eDivStr[7] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL};	// SiNDY��̃��C����
#else
		char		*eDivStr[6] = {NULL, NULL, NULL, NULL, NULL, NULL};	// SiNDY��̃��C����
#endif
		Peace		cur_tbl;	// �ǉ����鑮���e�[�u��

		aStrLen		= strlen(pRBuf);
		// �Ō��"\n"����菜��
		if(pRBuf[aStrLen - 1] == '\n') {
			pRBuf[aStrLen - 1] = '\0';
		}

		// �R�����g�����΂�
		if(pRBuf[0] == '#' || pRBuf[0] == '\0') {
			continue;
		}

		// �������','�ŕ�������
		char**				gomistr = NULL;
		eDivStr[0]			= strtok	(pRBuf, cut_str);
		eDivStr[1]			= strtok	(NULL, cut_str);
		eDivStr[2]			= strtok	(NULL, cut_str);
		eDivStr[3]			= strtok	(NULL, cut_str);
		eDivStr[4]			= strtok	(NULL, cut_str);
		eDivStr[5]			= strtok	(NULL, cut_str);
#ifdef	DB_ROT_RULE	// 2006.10.12
		eDivStr[6]			= strtok	(NULL, cut_str);
#endif

	//	strcpy				(cur_tbl.eSiFieldName, eDivStr[0]);
		cur_tbl.eSiFieldName	= eDivStr[0];
		if(strcmp(eDivStr[1], "POLYGON") == 0) {
			cur_tbl.eData_Type	= TYPE_POLY;
		}else
		if(strcmp(eDivStr[1], "LINE") == 0) {
			cur_tbl.eData_Type	= TYPE_LINE;
		}else
		if(strcmp(eDivStr[1], "POINT") == 0) {
			cur_tbl.eData_Type	= TYPE_POINT;
		}else{
			fprintf	(stderr, "�����Ή���`�t�@�C���t�H�[�}�b�g�G���[\n");
			return	(-1);
		}
		cur_tbl.eUnder_Flag	= atoi(eDivStr[2]);
		cur_tbl.eSiClass	= atoi(eDivStr[3]);
		cur_tbl.eINClass	= strtoul(eDivStr[4], gomistr, 16);
		strcpy				(cur_tbl.eName, eDivStr[5]);
#ifdef	DB_ROT_RULE	// 2006.10.13
		if( eDivStr[6] != NULL ) {
			strcpy				(cur_tbl.eRot_Rule, eDivStr[6]);
		}
#endif
		cur_tbl.eIndex		= i;

		// �Z�b�g�ɒǉ�
		mTable.insert	(cur_tbl);
	}
	fclose(pRFp);
	return(0);
}

/**
	DB���瑮���Ή��e�[�u����ǂݍ���
	Sindy�AIN�����Ή��e�[�u���̔z����쐬����
	���f�[�^���x�����w�肵�ĕK�v�Ƃ��郌�R�[�h�����ǂݍ���
	�@�S�e�[�u�� = 0, �s�s�n�} = 3x, �x�[�X = 2x, �~�h�� = 1x
	@return �Ԃ�l	���� = 0, ���s = -1
*/
int	SiINClassTable::
Read_DB_Table	(	char*					c_cpLayer_Name,		///< DB�̃��C����
					IFeatureWorkspacePtr	c_ipWorkSpace,		///< �t�B�[�`�����[�N�X�y�[�X
					int						c_iData_Level	)	///< �f�[�^�̃��x��
{
	IFeatureWorkspacePtr	a_F_Workspace	( c_ipWorkSpace );	// �t�B�[�`�����[�N�X�y�[�X
//	IFeatureClassPtr		a_ipF_Class;
	ITablePtr				a_ipTable;

	CComBSTR	a_bOpen_Layer_Name	( c_cpLayer_Name );

	// ���C���̃I�[�v��
	if( FAILED(a_F_Workspace->OpenTable ( a_bOpen_Layer_Name, &a_ipTable )) ) {
		fprintf	(stderr,	"<%s> ���C���̃I�[�v���Ɏ��s\n", c_cpLayer_Name);
		return	(false);
	}else {
		fprintf	(stdout,	"<%s> ���C�����I�[�v��\n", c_cpLayer_Name);
	}

	char		a_tmpstr[30];	//�@�����͂Ȃ�Ƃ��������C������

	sprintf	(	a_tmpstr, "OBJECTID <> 0");		// ���b�V���R�[�h�𕶎����

	_bstr_t		a_Where_Str ( a_tmpstr );

	IQueryFilterPtr	a_Sp_Filter		( CLSID_SpatialFilter );		// ��Ԍ����t�B���^
	a_Sp_Filter->put_WhereClause	( a_Where_Str );				// Where��̐ݒ�
	
	// �N���b�v��`�쐬
	IEnvelopePtr	a_ipMesh_Rect;	// ���b�V����`

	// �T�[�`���Ă���
//	IFeatureCursorPtr	a_F_Cursor;
	_ICursorPtr			a_ipCursor;
	if( FAILED	(a_ipTable->Search	( a_Sp_Filter, VARIANT_FALSE, &a_ipCursor )) ) {
		fprintf	(stderr, "�f�[�^�̑��������Ɏ��s\n");
		return	( -1 );
	}else {
		_IRowPtr				a_ipRow;
		long					a_lSp_InitFlag	= 0;	// ��ԎQ�Ƃ��������������ǂ����̃t���O
		int i = 0;
		for(i=0;;i++)
		{
			if( a_ipCursor->NextRow ( &a_ipRow ) != S_OK) {	// �f�[�^���ЂƂ��o��
				break;
			}

			Peace	cur_attr;
			
			//�@�����ƁA�t�F�b�`�����V�F�C�v���Ƃ��Ă���
			if( !cur_attr.Set_Data	( a_ipRow, c_iData_Level ) ) continue;
			// ���L�L���p�̏���
			if( cur_attr.eData_Type	== TYPE_POINT ) {
				cur_attr.eSiFieldName	= AN_CLS_C;	// ���L�L���p�̃t�B�[���h����ǉ����Ă���
			}
			// �Z�b�g�ɒǉ�
			mTable.insert	(cur_attr);
		}
	}

	// �s�s�n�}�w�i�̂Ƃ�����
	if ( c_iData_Level == city ) {
		// �X��p�̃e�[�u����ǉ��ŗp��
		int i = 0;
		for( i = 0; i < 6; i++ )
		{
			Peace					a_ptmpattr;
			a_ptmpattr.eSiFieldName	= "BGCLASS_C";
			a_ptmpattr.eIndex		= i+100;
			a_ptmpattr.eINClass		= 0x01015000 + 0x100*(i+1);
			strcpy (a_ptmpattr.eName, "�X��F");
			a_ptmpattr.eData_Type	= TYPE_POLY;
			a_ptmpattr.eSiClass		= i+1;
			a_ptmpattr.eUnder_Flag	= 0;
			mTable.insert ( a_ptmpattr );
		}
	}

	// �x�[�X�}�b�v�̏ꍇ�t�F���[�p�̃e�[�u�����쐬 2003.08.02
	if ( c_iData_Level == base ) {
		int	i = 0;
		for ( i = 2; i < 5; i++ )
		{
			Peace					a_ptmpattr;
			a_ptmpattr.eSiFieldName	= "ROADCLASS_C";
			a_ptmpattr.eIndex		= i+300;	// �l�̓_�~�[
			a_ptmpattr.eINClass		= 0x0500f100;
			strcpy (a_ptmpattr.eName, "");
			a_ptmpattr.eData_Type	= TYPE_LINE;
			a_ptmpattr.eSiClass		= 200+i;
			a_ptmpattr.eUnder_Flag	= 0;
			mTable.insert ( a_ptmpattr );
		}
	}

	//�s���E�p�_�~�[
	{
		Peace					a_ptmpattr;
		a_ptmpattr.eSiFieldName	= "D_ADMCLASS_C";
		a_ptmpattr.eIndex		= 350;	// �l�̓_�~�[
		a_ptmpattr.eINClass		= 0x00000000;
		strcpy (a_ptmpattr.eName, "�s�撬���E�^���E�����ڊE");
		a_ptmpattr.eData_Type	= TYPE_POLY;
		a_ptmpattr.eSiClass		= 0;
		a_ptmpattr.eUnder_Flag	= 0;
		mTable.insert ( a_ptmpattr );
	}

	// �����������p�_�~�[
	{
		Peace					a_ptmpattr;
		a_ptmpattr.eSiFieldName	= "D_DCLCLASS_C";
		a_ptmpattr.eIndex		= 360;	// �l�̓_�~�[
		a_ptmpattr.eINClass		= 0x00000000;
		strcpy (a_ptmpattr.eName, "����������");
		a_ptmpattr.eData_Type	= TYPE_LINE;
		a_ptmpattr.eSiClass		= 0;
		a_ptmpattr.eUnder_Flag	= 0;
		mTable.insert ( a_ptmpattr );
	}

	// �����p
	{
		Peace					a_ptmpattr;
		a_ptmpattr.eSiFieldName	= "BLDCLASS_C";
		a_ptmpattr.eIndex		= 370;	// �l�̓_�~�[
		a_ptmpattr.eINClass		= 0x00000000;
		strcpy (a_ptmpattr.eName, "�������E��ƒ�");
		a_ptmpattr.eData_Type	= TYPE_POLY;
		a_ptmpattr.eSiClass		= 0;
		a_ptmpattr.eUnder_Flag	= 0;
		mTable.insert ( a_ptmpattr );

	}

	return	( a_ipMesh_Rect );
}
