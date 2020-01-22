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

//
// 2002.06.05
// �c�[���̏������y�ѐݒ�Ɋւ��Ă̏������s���B
#include "stdafx.h"
#include "prog_init_cls.h"

/**
	���b�V�����X�g�̓ǂݍ���
	���w�肵�����b�V�����P�P�ǂݍ���ł���
	@return �����@0�@���s�@-1
*/
int		mk_input_list::
read_mesh_list	(	int		c_iMesh	)	///	���b�V���R�[�h
{
	crd_cnv			crd;

	int	cur_digit	= 0;
	cur_digit	= crd.GetMeshDigit( c_iMesh );
	if( cur_digit == -1 ) {
		return	( -1 );
	}
	
	// ���X�g�ɒǉ�
	mesh_set.insert	( c_iMesh );
	mesh_digit		= cur_digit;	// �����̊l��
	return	( 0 );
}

/**
	���b�V�����X�g�̓ǂݍ���
	�����I�Ƀ��b�V���R�[�h�̌������Ⴄ�Ǝ��s�����ƂȂ�
	@return �����@0�@���s�@-1
*/
int		mk_input_list::
read_mesh_list	(	char	*c_cFile	)	// �ǂݍ��݃t�@�C����
{
	const	int	bmax	= 128;
	FILE		*p_RFp;		
	char		p_RBuf[bmax];
	int			a_StrLen = 0;	

	p_RFp	= fopen(c_cFile, "r");
	if(p_RFp == NULL) {
		fprintf	(stderr, "���b�V�����X�g�t�@�C�� <%s> ���J���܂���!!\n", c_cFile);
		return	(-1);
	}

	crd_cnv			crd;
	int	pre_digit	= 0;
	int	first_flag	= 0;
	int	fail_flag	= 0;	// ���삪���������̔���Ɏg�p
	while(fgets(p_RBuf, bmax, p_RFp) != NULL)
	{
		int			cur_digit	= 0;
		a_StrLen	= strlen(p_RBuf);
		// �Ō��"\n"����菜��
		if(p_RBuf[a_StrLen - 1] == '\n') {
			p_RBuf[a_StrLen - 1] = '\0';
		}

		// �R�����g�����΂�
		if(p_RBuf[0] == '#' || p_RBuf[0] == '\0') {
			continue;
		}

		int			meshcode	= 0;
		meshcode	=  atoi(p_RBuf);

		cur_digit	= crd.GetMeshDigit(meshcode);
		if(first_flag == 0) {
			pre_digit	= cur_digit; 
			first_flag	= 1;
		}
		if(pre_digit != cur_digit) {
			fprintf		(stderr, "�ǂݍ��񂾃��b�V�����X�g�̌����ɐH���Ⴄ���̂�����܂�<%s>\n", p_RBuf);
			fail_flag	= 1;
			break;
		}
		// ���X�g�ɒǉ�
		mesh_set.insert	(meshcode);
	}

	fclose(p_RFp);

	if(fail_flag == 1) {
		return	(-1);
	}
	mesh_digit	= pre_digit;	// �����̊l��
	return(0);
}

/** 2007.06.28 �ǉ�
	�����񃌃R�[�h�̒ǉ�
*/
void	mk_input_list::
put_str_record	(	char	*c_cStr	)
{
	std::string	put_str( c_cStr );

	// ���X�g�ɒǉ�
	data_set.insert	( put_str );
}

/**
	�f�[�^���X�g�̓ǂݍ���
	�ǂݍ��ݕ�����̒���������Ă��悢�d�l�ɂ��Ă���
	@return �����@0�@���s�@-1
*/
int		mk_input_list::
read_data_list	(	char	*c_cFile	)	// �ǂݍ��݃t�@�C����
{
	const	int	bmax		= 512;
	FILE		*p_RFp;		
	char		p_RBuf[bmax];
	int			a_StrLen	= 0;	

	p_RFp	= fopen ( c_cFile, "r" );
	if(p_RFp == NULL) {
		fprintf	( stderr, "���X�g�t�@�C�� <%s> ���J���܂���!!\n", c_cFile );
		return	( -1 );
	}

	crd_cnv			crd;
	while ( fgets ( p_RBuf, bmax, p_RFp ) != NULL )
	{
		a_StrLen	= strlen(p_RBuf);
		// �Ō��"\n"����菜��
		if(p_RBuf[a_StrLen - 1] == '\n') {
			p_RBuf[a_StrLen - 1] = '\0';
		}

		// �R�����g�����΂�
		if( p_RBuf[0] == '#' || p_RBuf[0] == '\0' ) {
			continue;
		}

		// ���������m�ۂ���
	//	char*	put_str	= NULL;
	//	put_str	= new char[a_StrLen+1];
	//	if( put_str == NULL ) {
	//		fprintf	( stderr, "<%s>�̓ǂݍ��݃f�[�^���X�g���쐬����ۂɁA�������m�ۂɎ��s���܂����B\n", c_cFile );
	//		return	( -1 );
	//	}

		// �f�[�^���R�s�[
		//strcpy	( put_str, p_RBuf );
		//put_str	= p_RBuf;

		// ���X�g�ɒǉ�
		put_str_record ( p_RBuf );
	}

	fclose	( p_RFp );

	return	( 0 );
}

/**
	�������t�@�C���̓ǂݍ���
	�����I�Ƀ��b�V���R�[�h�̌������Ⴄ�Ǝ��s�����ƂȂ�
	@return �����@0�@���s�@-1
*/
int		prog_init::
read_init_file	(	char	*c_cFile,	// �ǂݍ��݃t�@�C����
 					int		c_iMode	)	// �ǂݍ��݃��[�h 0=SDE�p 1=SiNDY-C�p
{
	const	int	bmax	= 256;
	FILE		*p_RFp;		
	char		p_RBuf[bmax];
	int			a_StrLen = 0;

	p_RFp	= fopen(c_cFile, "r");
	if(p_RFp == NULL) {
		fprintf	(stderr, "�v���O�����������t�@�C�� <%s> ���J���܂���!!\n", c_cFile);
		return	(-1);
	}

	crd_cnv		crd;
	int			first_flag	= 0;
	int			fail_flag	= 0;		// ���삪���������̔���Ɏg�p
	char*		cut_str		= "\t ";	// �^�u�ƃX�y�[�X�ŃJ�b�g
	while(fgets(p_RBuf, bmax, p_RFp) != NULL)
	{
		int			cur_digit	= 0;
		a_StrLen	= strlen(p_RBuf);
		// �Ō��"\n"����菜��
		if(p_RBuf[a_StrLen - 1] == '\n') {
			p_RBuf[a_StrLen - 1] = '\0';
		}

		// �R�����g�����΂�
		if(p_RBuf[0] == '#' || p_RBuf[0] == '\0') {
			continue;
		}

		// �^�u�ł���
		char*	field[3]	= {NULL, NULL, NULL};

		field[0]	= strtok(p_RBuf, cut_str);
		field[1]	= strtok(NULL, cut_str);
		field[2]	= strtok(NULL, cut_str);
		
		if(field[0] == NULL) {
			fprintf	(stderr, "�������t�@�C��<%s>�̃t�H�[�}�b�g���Ⴂ�܂�\n", c_cFile);
			return	(-1);
		}

		std::string		fstr[2];
		fstr[0]	= field[0];

		// �啶���� 2007.10.26 �ǉ�
		//-------------------------------------------------------------
		int	c = 0;
		while(fstr[0][c] != '\0' ){
			fstr[0][c]	= (char)toupper(fstr[0][c]);
			c++;
		}
		//-------------------------------------------------------------

		fstr[1]	= field[1];
		if(DB_MAP == fstr[0]){	///< �n�}�nDB�ڑ�
			if( c_iMode == 0 ) {	// 2007.03.01
				if(!get_Connect_Info	( field[1],	&e_sUserName_Map, &e_sPassword_Map, &e_sVersion_Map, &e_sInstance_Map, &e_sServer_Map )) {
					fprintf	( stderr, "�ڑ����̎w�肪�Ԉ���Ă��܂�\n");
					return	( -1 );
				}
			}else {
				e_sDataFile_Path_Map	= field[1];	// 2007.03.01
			}
		}else if(DB_ADDRESS == fstr[0]){	///< �Z���nDB�ڑ�
			if( c_iMode == 0 ) {	// 2007.03.01
				if(!get_Connect_Info	( field[1],	&e_sUserName_Address, &e_sPassword_Address, &e_sVersion_Address, &e_sInstance_Address, &e_sServer_Address )) {
					fprintf	( stderr, "�ڑ����̎w�肪�Ԉ���Ă��܂�\n");
					return	( -1 );
				}
			}else {
				e_sDataFile_Path_Address	= field[1];	// 2007.03.01
			}
		}else if(DB_BASE == fstr[0]){	///< ���k��DB�ڑ�
			if( c_iMode == 0 ) {	// 2007.03.01
				if(!get_Connect_Info	( field[1],	&e_sUserName_Base, &e_sPassword_Base, &e_sVersion_Base, &e_sInstance_Base, &e_sServer_Base )) {
					fprintf	( stderr, "�ڑ����̎w�肪�Ԉ���Ă��܂�\n");
					return	( -1 );
				}
			}else {
				e_sDataFile_Path_Base	= field[1];	// 2007.03.01			
			}
		}else if(DB_ROAD == fstr[0]){	///< ���H�nDB�ڑ� [bug 7066]�ւ̑Ή� 2009.07.02
			if( c_iMode == 0 ) {
				if(!get_Connect_Info	( field[1],	&e_sUserName_Road, &e_sPassword_Road, &e_sVersion_Road, &e_sInstance_Road, &e_sServer_Road )) {
					fprintf	( stderr, "�ڑ����̎w�肪�Ԉ���Ă��܂�\n");
					return	( -1 );
				}
			}else {
				e_sDataFile_Path_Road	= field[1];		
			}
		}else if(DB_ANNO == fstr[0]){	///< ���L�nDB�ڑ� city��anno,poi�̕����Ή� 2014/05/07
			if( c_iMode == 0 ) {
				if(!get_Connect_Info	( field[1],	&e_sUserName_Anno, &e_sPassword_Anno, &e_sVersion_Anno, &e_sInstance_Anno, &e_sServer_Anno )) {
					fprintf	( stderr, "�ڑ����̎w�肪�Ԉ���Ă��܂�\n");
					return	( -1 );
				}
			}else {
				e_sDataFile_Path_Anno	= field[1];		
			}
		}else if(DB_ANY == fstr[0]){	///< �ėpDB�ڑ�
			if( c_iMode == 0 ) {	// 2007.03.01
				if(!get_Connect_Info	( field[1],	&e_sUserName, &e_sPassword, &e_sVersion, &e_sInstance, &e_sServer )) {
					fprintf	( stderr, "�ڑ����̎w�肪�Ԉ���Ă��܂�\n");
					return	( -1 );			
				}
			}else {
				e_sDataFile_Path		= field[1];	// 2007.03.01			
			}
		}else
		if(Add_FC(INI_UN) == fstr[0]) {	///< ���[�U��
			e_sUserName	= fstr[1];
		}else
		if(Add_FC(INI_PW) == fstr[0]) {	///<�p�X���[�h
			e_sPassword	= fstr[1];
		}else
		if(Add_FC(INI_IT) == fstr[0]) {	///< �C���X�^���X
			e_sInstance	= fstr[1];
		}else
		if(Add_FC(INI_SV) == fstr[0]) {		///< �T�[�o��
			e_sServer	= fstr[1];
		}else
		if(Add_FC(INI_VER) == fstr[0]) {	///< �o�[�W����
			e_sVersion	= fstr[1];
		}else
	//	if(Add_FC(INI_OPATH) == fstr[0]) {	///< �f�[�^�o�̓p�X��
	//		e_sOutput_Path	= fstr[1];
	//	}else
		if(Add_FC(INI_CMESH) == fstr[0]) {	///< �s�s�n�}���b�V�����C��
			e_sCity_Mesh	= fstr[1];
		}else
		if(Add_FC(INI_CNOTE) == fstr[0]) {	///< �s�s�n�}���L�|�C���g���C��
			e_sCity_Note	= fstr[1];
		}else
		if(Add_FC(INI_CNLINE) == fstr[0]) {	///< �s�s�n�}���L���C�����C��
			e_sCity_Note_Line	= fstr[1];
		}else
		if(Add_FC(INI_CHIBAN) == fstr[0]) {	///< �n�Ԓ��L���C��
			e_sChiban			= fstr[1];
		}else
		if(Add_FC(INI_CLINE) == fstr[0]) {	///< �s�s�n�}�w�i�A�[�N���C��
			e_sCity_Line	= fstr[1];
		}else
		if(Add_FC(INI_CRWAY) == fstr[0]) {	///< �s�s�n�}�S�����C��
			e_sCity_RailWay	= fstr[1];
		}else
		if(Add_FC(INI_CSITE) == fstr[0]) {	///< �s�s�n�}�X�惌�C��
			e_sCity_Site	= fstr[1];
		}else
		if(Add_FC(INI_CSTA) == fstr[0]) {	///< �s�s�n�}�w�Ƀ��C��
			e_sCity_Station	= fstr[1];
		}else
		if(Add_FC(INI_BLD) == fstr[0]) {	///< �s�s�n�}�������C��
			e_sCity_Building	= fstr[1];
		}else
		if(Add_FC(INI_BLDL) == fstr[0]) {	///< �s�s�n�}�����A�[�N���C��
			e_sCity_Build_Line	= fstr[1];
		}else
		if(Add_FC(INI_STEP) == fstr[0]) {	///< �s�s�n�}�i���`�󃌃C��
			e_sCity_Build_Step	= fstr[1];
		}else
		if(Add_FC(INI_ROOF) == fstr[0]) {	///< �s�s�n�}��������{�݌`�󃌃C��
			e_sCity_Building_Roof	= fstr[1];
		}else
		if(Add_FC(INI_TUNNEL) == fstr[0]) {	///< �s�s�n�}�g���l���B���`�󃌃C��
			e_sCity_Tunnel	= fstr[1];
		}else
		if(Add_FC(INI_GOU) == fstr[0]) {	///< ���|�C���g
			e_sGou_Point		= fstr[1];
		}else
		if(Add_FC(INI_ADM) == fstr[0]) {	///< �s���E���C��
			e_sAdmin			= fstr[1];
		}else
		if(Add_FC(INI_BADM) == fstr[0]) {	///< ���k�ڍs���E���C��
			e_sBase_Admin		= fstr[1];
		}else
		if(Add_FC(INI_CADM) == fstr[0]) {	///< �s�s�n�}�s���E���C��
			e_sCity_Admin		= fstr[1];
		}else
		if(Add_FC(INI_BMESH) == fstr[0]) {	///< ���k�ڃ��b�V�����C��
			e_sBase_Mesh		= fstr[1];
		}else
		if(Add_FC(INI_BNL1) == fstr[0]) {	///< ���k�ڒ��L���C�����C���P
			e_sBase_Note_Line1	= fstr[1];
		}else
		if(Add_FC(INI_BNL2) == fstr[0]) {	///< ���k�ڒ��L���C�����C���Q
			e_sBase_Note_Line2	= fstr[1];
		}else
		if(Add_FC(INI_BNL3) == fstr[0]) {	///< ���k�ڒ��L���C�����C���R
			e_sBase_Note_Line3	= fstr[1];
		}else
		if(Add_FC(INI_BNL4) == fstr[0]) {	///< ���k�ڒ��L���C�����C���S
			e_sBase_Note_Line4	= fstr[1];
		}else
		if(Add_FC(INI_BNOTE) == fstr[0]) {	///< ���k�ڒ��L
			e_sBase_Note	= fstr[1];
		}else
		if(Add_FC(INI_BCNT) == fstr[0]) {	///< ���k�ڒi�ʃ��C��
			e_sBase_Contour	= fstr[1];
		}else
		if(Add_FC(INI_BLINE) == fstr[0]) {	///< ���k�ڔw�i�A�[�N���C��
			e_sBase_Line	= fstr[1];
		}else
		if(Add_FC(INI_BRWAY) == fstr[0]) {	///< ���k�ړS�����C��
			e_sBase_Railway	= fstr[1];
		}else
		if(Add_FC(INI_BSITE) == fstr[0]) {	///< ���k�ڔw�i�A�{�݃��C��
			e_sBase_Site	= fstr[1];
		}else
		if(Add_FC(INI_BSTA) == fstr[0]) {	///< ���k�ډw�Ƀ��C��
			e_sBase_Station	= fstr[1];
		}else
		if(Add_FC(INI_MMESH) == fstr[0]) {	///< �~�h�����b�V�����C��
			e_sMiddle_Mesh		= fstr[1];
		}else
		if(Add_FC(INI_MNL1) == fstr[0]) {	///< �~�h�����L���C�����C���P
			e_sMiddle_Note_Line1	= fstr[1];
		}else
		if(Add_FC(INI_MNL2) == fstr[0]) {	///< �~�h�����L���C�����C���Q
			e_sMiddle_Note_Line2	= fstr[1];
		}else
		if(Add_FC(INI_MNL3) == fstr[0]) {	///< �~�h�����L���C�����C���R
			e_sMiddle_Note_Line3	= fstr[1];
		}else
		if(Add_FC(INI_MNL4) == fstr[0]) {	///< �~�h�����L���C�����C���S
			e_sMiddle_Note_Line4	= fstr[1];
		}else
		if(Add_FC(INI_MNOTE) == fstr[0]) {	///< �~�h�����L
			e_sMiddle_Note	= fstr[1];
		}else
		if(Add_FC(INI_MCNT) == fstr[0]) {	///< �~�h���i�ʃ��C��
			e_sMiddle_Contour	= fstr[1];
		}else
		if(Add_FC(INI_MLINE) == fstr[0]) {	///< �~�h���w�i�A�[�N���C��
			e_sMiddle_Line	= fstr[1];
		}else
		if(Add_FC(INI_MSITE) == fstr[0]) {	///< �~�h���w�i�A�{�݃��C��
			e_sMiddle_Site	= fstr[1];
		}else
		if(Add_FC(INI_TMESH) == fstr[0]) {	///< �g�b�v���b�V�����C��
			e_sTop_Mesh		= fstr[1];
		}else
		// �g�b�v�}�b�v���L�p 2004.10.26 �ǉ�
		if(Add_FC(INI_TNL1) == fstr[0]) {	///< �g�b�v���L���C�����C���P
			e_sTop_Note_Line1	= fstr[1];
		}else
		if(Add_FC(INI_TNL2) == fstr[0]) {	///< �g�b�v���L���C�����C���Q
			e_sTop_Note_Line2	= fstr[1];
		}else
		if(Add_FC(INI_TNL3) == fstr[0]) {	///< �g�b�v���L���C�����C���R
			e_sTop_Note_Line3	= fstr[1];
		}else
		if(Add_FC(INI_TNL4) == fstr[0]) {	///< �g�b�v���L���C�����C���S
			e_sTop_Note_Line4	= fstr[1];
		}else
		if(Add_FC(INI_TNOTE) == fstr[0]) {	///< �g�b�v���L
			e_sTop_Note	= fstr[1];
		}else
		// �g�b�v�}�b�v�p 2003.01.05 �ǉ�
		if(Add_FC(INI_TCNT) == fstr[0]) {	///< �g�b�v�i�ʃ��C��
			e_sTop_Contour	= fstr[1];
		}else
		if(Add_FC(INI_TLINE) == fstr[0]) {	///< �g�b�v�w�i�A�[�N���C��
			e_sTop_Line		= fstr[1];
		}else
		if(Add_FC(INI_TSITE) == fstr[0]) {	///< �g�b�v�w�i�A�{�݃��C��
			e_sTop_Site		= fstr[1];
		}else
		//�@�g�b�v�}�b�v�p�����܂�
		if(Add_FC(INI_RLINK) == fstr[0]) {	///< ���H�����N���C��
			e_sRoad_Link	= fstr[1];		
		}else
		if(Add_FC(INI_PAREA) == fstr[0]) {	///< PAREA��\�_���C��
			e_sParea		= fstr[1];		
		}else
		if(Add_FC(INI_BRP) == fstr[0]) {	///< ���k��Addr�|�C���g���C��
			e_sBase_Rep_Point= fstr[1];		
		}else
		if(Add_FC(INI_BACK_ATTR) == fstr[0]) {	///< �w�i��ʗp�e�[�u��
			e_sBackground_Class	= fstr[1];
		}else
		if(Add_FC(INI_ANNO_ATTR) == fstr[0]) {	///< ���L��ʗp�e�[�u��
			e_sAnnotation_Class	= fstr[1];
		}else
		if(Add_FC(INI_POI) == fstr[0]) {
			e_sPOI_Point = fstr[1];
		}
	}

	fclose(p_RFp);

	if(fail_flag == 1) {
		return	(-1);
	}
	return(0);
}

/**
	�v���O�������������ϐ��̓ǂݍ���
	���ϐ�����������o��
	@return �����@0�@���s�@-1
*/
int		prog_init::
read_init_env	( 	int		c_iMode	)		// �ǂݍ��݃��[�h 0=SDE�p 1=SiNDY-C�p
{

	char*		a_cpEnvStr	= NULL;		// ���ϐ��̃|�C���^
	int			cur_digit	= 0;
	
	if( (a_cpEnvStr = getenv(DB_MAP)) != NULL ) {	///< �n�}�nDB�ڑ�
		if( c_iMode == 0 ) {	// 2007.03.01
			if(!get_Connect_Info	( a_cpEnvStr,	&e_sUserName_Map, &e_sPassword_Map, &e_sVersion_Map, &e_sInstance_Map, &e_sServer_Map )) {
				fprintf	( stderr, "�ڑ����̎w�肪�Ԉ���Ă��܂�\n");
				return	( -1 );
			}
		}else {
			e_sDataFile_Path_Map	= a_cpEnvStr;	// 2007.03.01
		}
	}
	if( (a_cpEnvStr = getenv(DB_BASE)) != NULL ) {	///< ���k�ڌnDB�ڑ�
		if( c_iMode == 0 ) {	// 2007.03.01
			if(!get_Connect_Info	( a_cpEnvStr,	&e_sUserName_Base, &e_sPassword_Base, &e_sVersion_Base, &e_sInstance_Base, &e_sServer_Base )) {
				fprintf	( stderr, "�ڑ����̎w�肪�Ԉ���Ă��܂�\n");
				return	( -1 );
			}
		}else {
			e_sDataFile_Path_Base	= a_cpEnvStr;	// 2007.03.01
		}
	}
	if( (a_cpEnvStr = getenv(DB_ROAD)) != NULL ) {	///< ���H�nDB�ڑ� [bug 7066]�Ή� 2009.07.07
		if( c_iMode == 0 ) {	// 2007.03.01
			if(!get_Connect_Info	( a_cpEnvStr,	&e_sUserName_Road, &e_sPassword_Road, &e_sVersion_Road, &e_sInstance_Road, &e_sServer_Road )) {
				fprintf	( stderr, "�ڑ����̎w�肪�Ԉ���Ă��܂�\n");
				return	( -1 );
			}
		}else {
			e_sDataFile_Path_Road	= a_cpEnvStr;	// 2007.03.01
		}
	}
	if( (a_cpEnvStr = getenv(DB_ANNO)) != NULL ) {	///< ���L�nDB�ڑ� city��anno,poi�̕����Ή� 2014/05/07
		if( c_iMode == 0 ) {
			if(!get_Connect_Info	( a_cpEnvStr,	&e_sUserName_Anno, &e_sPassword_Anno, &e_sVersion_Anno, &e_sInstance_Anno, &e_sServer_Anno )) {
				fprintf	( stderr, "�ڑ����̎w�肪�Ԉ���Ă��܂�\n");
				return	( -1 );
			}
		}else {
			e_sDataFile_Path_Anno	= a_cpEnvStr;
		}
	}
	if( (a_cpEnvStr = getenv(DB_ADDRESS)) != NULL ) {	///< �Z���nDB�ڑ�
		if( c_iMode == 0 ) {	// 2007.03.01
			if(!get_Connect_Info	( a_cpEnvStr,	&e_sUserName_Address, &e_sPassword_Address, &e_sVersion_Address, &e_sInstance_Address, &e_sServer_Address )) {
				fprintf	( stderr, "�ڑ����̎w�肪�Ԉ���Ă��܂�\n");
				return	( -1 );
			}
		}else {
			e_sDataFile_Path_Address	= a_cpEnvStr;	// 2007.03.01
		}
	}
	if( (a_cpEnvStr = getenv(DB_ANY)) != NULL ) {	///< �ėpDB�ڑ�
		if( c_iMode == 0 ) {	// 2007.03.01
			if(!get_Connect_Info	( a_cpEnvStr,	&e_sUserName, &e_sPassword, &e_sVersion, &e_sInstance, &e_sServer )) {
				fprintf	( stderr, "�ڑ����̎w�肪�Ԉ���Ă��܂�\n");
				return	( -1 );			
			}
		}else {
			e_sDataFile_Path		= a_cpEnvStr;	// 2007.03.01
		}
	}
//	if( (a_cpEnvStr = getenv( Add_FC(INI_UN))) != NULL ) {	///< ���[�U��
//		e_sUserName	= a_cpEnvStr;
//	}
//	if( (a_cpEnvStr = getenv( Add_FC(INI_PW))) != NULL ) {	///<�p�X���[�h
//		e_sPassword	= a_cpEnvStr;
//	}
//	if( (a_cpEnvStr = getenv( Add_FC(INI_IT))) != NULL ) {	///<�C���X�^���X
//		e_sInstance	= a_cpEnvStr;
//	}
//	if( (a_cpEnvStr = getenv( Add_FC(INI_SV ))) != NULL ) {	///<�T�[�o��
//		e_sServer	= a_cpEnvStr;
//	}
//	if( (a_cpEnvStr = getenv( Add_FC(INI_VER))) != NULL ) {	///<�o�[�W����
//		e_sVersion	= a_cpEnvStr;
//	}
//	if( (a_cpEnvStr = getenv( Add_FC(INI_OPATH))) != NULL ) {///<�f�[�^�o�̓p�X��
//		e_sOutput_Path	= a_cpEnvStr;
//	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_CMESH))) != NULL ) {	///<�s�s�n�}���b�V�����C��
		e_sCity_Mesh	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_CNOTE))) != NULL ) {	///<�s�s�n�}���L�|�C���g���C��
		e_sCity_Note	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_CNLINE))) != NULL ) {	///<�s�s�n�}���L���C�����C��
		e_sCity_Note_Line	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_CHIBAN))) != NULL ) {	///<�n�Ԓ��L���C��
		e_sChiban	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_CLINE))) != NULL ) {	///<�s�s�n�}�w�i�A�[�N���C��
		e_sCity_Line	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_CRWAY))) != NULL ) {	///<�s�s�n�}�S�����C��
		e_sCity_RailWay	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_CSITE))) != NULL ) {	///<�s�s�n�}�X�惌�C��
		e_sCity_Site	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_CSTA))) != NULL ) {	///<�s�s�n�}�w�Ƀ��C��
		e_sCity_Station	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_BLD))) != NULL ) {	///<�s�s�n�}�������C��
		e_sCity_Building	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_BLDL))) != NULL ) {	///<�s�s�n�}�����A�[�N���C��
		e_sCity_Build_Line	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_STEP))) != NULL ) {	///<�s�s�n�}�i���`�󃌃C��
		e_sCity_Build_Step	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_ROOF))) != NULL ) {	///<�s�s�n�}��������{�݌`�󃌃C��
		e_sCity_Building_Roof	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_TUNNEL))) != NULL ) {	///<�s�s�n�}�g���l���B���`�󃌃C��
		e_sCity_Tunnel	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_GOU))) != NULL ) {	///<���|�C���g���C�� 2003.7.18
		e_sGou_Point		= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_ADM))) != NULL ) {	///<�s���E���C��
		e_sAdmin	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_BADM))) != NULL ) {	///<���k�ڍs���E���C��
		e_sBase_Admin	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_CADM))) != NULL ) {	///<�s�s�n�}�s���E���C��
		e_sCity_Admin	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_BMESH))) != NULL ) {///<���k�ڃ��b�V�����C��
		e_sBase_Mesh	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_BNL1))) != NULL ) {///<���k�ڒ��L���C�����C���P
		e_sBase_Note_Line1	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_BNL2))) != NULL ) {///<���k�ڒ��L���C�����C���Q
		e_sBase_Note_Line2	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_BNL3))) != NULL ) {///<���k�ڒ��L���C�����C���R
		e_sBase_Note_Line3	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_BNL4))) != NULL ) {///<���k�ڒ��L���C�����C���S
		e_sBase_Note_Line4	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_BNL4))) != NULL ) {///<���k�ڒ��L���C�����C���S
		e_sBase_Note_Line4	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_BNOTE))) != NULL ) {	///< ���k�ڒ��L
		e_sBase_Note	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_BCNT))) != NULL ) {	///< ���k�ڒi�ʃ��C��
		e_sBase_Contour	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_BLINE))) != NULL ) {	///< ���k�ڔw�i�A�[�N���C��
		e_sBase_Line	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_BRWAY))) != NULL ) {	///< ���k�ړS�����C��
		e_sBase_Railway	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_BSITE))) != NULL ) {	///< ���k�ڔw�i�A�{�݃��C��
		e_sBase_Site	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_BSTA))) != NULL ) {	///< ���k�ډw�Ƀ��C��
		e_sBase_Station	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_MMESH))) != NULL ) {	///< �~�h�����b�V�����C��
		e_sMiddle_Mesh		= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_MNL1))) != NULL ) {	///< �~�h�����L���C�����C���P
		e_sMiddle_Note_Line1	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_MNL2))) != NULL ) {	///< �~�h�����L���C�����C���Q
		e_sMiddle_Note_Line2	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_MNL3))) != NULL ) {	///< �~�h�����L���C�����C���R
		e_sMiddle_Note_Line3	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_MNL4))) != NULL ) {	///< �~�h�����L���C�����C���S
		e_sMiddle_Note_Line4	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_MNOTE))) != NULL ) {	///< �~�h�����L
		e_sMiddle_Note	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_MCNT))) != NULL ) {		///< �~�h���i�ʃ��C��
		e_sMiddle_Contour	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_MLINE))) != NULL ) {	///< �~�h���w�i�A�[�N���C��
		e_sMiddle_Line	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_MSITE))) != NULL ) {	///< �~�h���w�i�A�{�݃��C��
		e_sMiddle_Site	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_TMESH))) != NULL ) {	///< �~�h�����b�V�����C��
		e_sTop_Mesh		= a_cpEnvStr;
	}
	// �g�b�v�}�b�v�p 2003.01.05�ǉ�
	if( (a_cpEnvStr = getenv( Add_FC(INI_TCNT))) != NULL ) {		///< �g�b�v�i�ʃ��C��
		e_sTop_Contour	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_TLINE))) != NULL ) {	///< �g�b�v�w�i�A�[�N���C��
		e_sTop_Line		= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_TSITE))) != NULL ) {	///< �g�b�v�w�i�A�{�݃��C��
		e_sTop_Site		= a_cpEnvStr;
	}
	//�@�g�b�v�}�b�v�p�����܂�
	if( (a_cpEnvStr = getenv( Add_FC(INI_RLINK))) != NULL ) {	///< ���H�����N���C��
		e_sRoad_Link	= a_cpEnvStr;		
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_PAREA))) != NULL ) {	///< PAREA��\�_���C��
		e_sParea		= a_cpEnvStr;		
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_BRP))) != NULL ) {		///< ���k��Addr�|�C���g���C��
		e_sBase_Rep_Point= a_cpEnvStr;		
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_BACK_ATTR))) != NULL ) {	///< �w�i��ʗp�e�[�u��
		e_sBackground_Class	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_ANNO_ATTR))) != NULL ) {	///< ���L��ʗp�e�[�u��
		e_sAnnotation_Class	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_POI))) != NULL ) {	///< POI_POINT�e�[�u��
		e_sPOI_Point	= a_cpEnvStr;
	}
	return(0);
}

/**	2007.06.27 �ǉ� from CrossRoadsNameChk ����
	������������̓ǂݍ���
	@return �����@0�@���s�@-1
*/
bool	prog_init::
read_init_str	(	char	*c_cpConnect_Str	)	///< �ڑ�������
{
	if( !get_Connect_Info ( c_cpConnect_Str,
							&e_sUserName, &e_sPassword, &e_sVersion,
							&e_sInstance, &e_sServer ) ) {
		return	( false );
	}
	return	( true );
}

/**
	�ڑ����̎擾
	@return	����	true
	@return	���s	false
*/
bool	prog_init::
get_Connect_Info (	char*			c_cpConnect_Str,	///< �ڑ�������
					std::string*	c_spUserName,		///< ���[�U��[out]
					std::string*	c_spPassword,		///< �p�X���[�h[out]
					std::string*	c_spVersion,		///< �o�[�W����[out]
					std::string*	c_spInstance,		///< �C���X�^���X[out]
					std::string*	c_spServer )		///< �T�[�o��[out]
{
	char*		cut_str		= "/";	// �X���b�V���ŃJ�b�g

	// �^�u�ł���
	char*	field[5]	= {NULL, NULL, NULL, NULL, NULL};

	field[0]	= strtok(c_cpConnect_Str, cut_str);	///< ���[�U��
	field[1]	= strtok(NULL, cut_str);			///< �p�X���[�h
	field[2]	= strtok(NULL, cut_str);			///< �C���X�^���X
	field[3]	= strtok(NULL, cut_str);			///< �T�[�o��
	field[4]	= strtok(NULL, cut_str);			///< �o�[�W����	

	int	i = 0;
	for(i = 0; i < 5; i++)
	{
		if(field[i] == NULL) {
			return	( false );
		}
	}

	*c_spUserName	= field[0];
	*c_spPassword	= field[1];
	*c_spVersion	= field[2];
	*c_spInstance	= field[3];
	*c_spServer		= field[4];

	return	( true );
}
