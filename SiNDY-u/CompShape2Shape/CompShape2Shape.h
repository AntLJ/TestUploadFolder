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

#include "stdafx.h"


using namespace std;



// Shape�f�[�^�ێ��p�̃N���X(1���R�[�h���Ƃ�1��)
class	shp1
{
public:
	// �R���X�g���N�^
	shp1()
	{
		shp_ID		= 0;
		point_num	= 0;
		check_flag	= 0;
		shp_dX		= 0;
		shp_dY		= 0;
		iMeshCode	= 0;
		iField_num	= 0;
	}

	// �f�N�X�g���N�^
	~shp1()
	{
	}

	long					shp_ID;					// �I�u�W�F�N�gID or FID�iFID���[�h�w�肵�����j
	int						point_num;				// ��ԓ_��
	double					shp_dX;					// �|�C���g��X���W�l
	double					shp_dY;					// �|�C���g��Y���W�l
	int						check_flag;				// �`�F�b�N���ꂽ���ǂ����̃t���O
	int						iMeshCode;				// ���b�V���R�[�h
	int						iField_num;				// �t�B�[���h��
	vector<string>			vsField_name;			// �t�B�[���h�������ԂɊi�[
	map<string, wstring>	Fields;					// �t�B�[���h���������Ă��i�[�ł���悤��map�Ńt�B�[���h���Ɨv�f�Ŋi�[
		

	bool operator < (const shp1& right) const{	//�I�y���[�^�ݒ�
		bool	ret	= false;
		if( shp_ID != right.shp_ID ) {			//�I�u�W�F�N�gID�̏����ŕ��ёւ���
			if( shp_ID < right.shp_ID ) {
				ret	= true;
			}
		}
		return	( ret );
	}
};

typedef multimap<long, shp1>			mmShp1;		// shp1��1���b�V�����ێ��̂��߂�multimap
typedef	multimap<long, shp1>::iterator	itrmmShp1;

// �o�C�i�����[�h����Shape�f�[�^�ێ��p�̃N���X(1mesh���Ƃ�1��)
class	shp2
{
public:
	shp2()
	{
		iRec_num			= 0;
		iHeadSize			= 0;
		iRecSize			= 0;
		iMeshCode			= 0;
	}

	~shp2()
	{
	}

	int									iRec_num;			// ���R�[�h��
	int									iHeadSize;			// �w�b�_�T�C�Y
	int									iRecSize;			// ���R�[�h�T�C�Y
	int									iMeshCode;			// ���b�V���R�[�h
	vector<string>						vsField_name;		// �t�B�[���h�������ԂɊi�[
	//vector<string>					vsBinData;			// 1���R�[�h���Ƃ̃o�C�i���f�[�^�����ԂɊi�[
	multimap<int, pair<int, string>>	msBinData;			// 1���R�[�h���Ƃ̃o�C�i���f�[�^���I�u�W�F�N�gID��
															//�L�[�Ƀ`�F�b�N�ς݃t���O�ƂƂ���multimap�Ɋi�[
};

// �����p�N���X
class	CCompShape2Shape
{
public:
	// �R���X�g���N�^
	CCompShape2Shape()
	{
		m_iExeMode		= 0;
		m_dDistBorder	= 0;
		m_bAttr			= false;
		m_bShp			= false;
	}
	
	// �f�N�X�g���N�^
	~CCompShape2Shape()
	{
	}

	/**
	 * @brief	�g�p�@
	 */
	void	printUsage();

	/**
	 * @brief	�����`�F�b�N
	 * @param	argc	[in]	�����̐�
	 * @param	argv	[in]	����
	 * @return	bool
	 */
	bool	arg_check( int argc, char* argv[] );

	/**
	 * @brief	�o�[�W�����o��
	 */
	void	print_ver();

	/**
	 * @brief	Shape�t�@�C����_�p�X�o��
	 */
	void	print_pass();

	/**
	 * @brief	�J�n���ԏo��
	 */
	void	print_begin_time();

	/**
	 * @brief	�I�����ԏo��
	 */
	void	print_end_time();

	/**
	 * @brief	���O�w�b�_(�t�B�[���h��)�o��
	 */
	void	print_log_head();

	/**
	 * @brief	���s
	 * @return	bool
	 */
	int	execute();

private:
	/**
	 * @brief	���X�g�t�@�C���ǂݍ���
	 * @param	lpszFile	[in]	�ǂݍ��݂������X�g��
	 * @param	vsVectorList	[out]	�Ǎ���Vector(1�t�B�[���h����)
	 * @return	bool
	 */
	bool	loadlist( LPCTSTR lpszFile, vector<string>& vsVectorList );

	/**
	 * @brief	���b�V���P�ʃt�@�C�����̍쐬
	 * @param	iMeshCode	[in]	���b�V���R�[�h
	 * @param	cPathName	[in]	�t�@�C����_�p�X
	 * @return	string		[out]	�t�@�C���̃t���p�X
	 */
	string	make_mesh_fname( int iMeshCode,	char* cpPathName );

	/**
	 * @brief	Shape�t�@�C����ǂݍ��݁Amultimap<ID,shp1>�ɗ��Ƃ�����
	 * @param	sFullPass	[in]	�ǂݍ���Shape�t�@�C���̃t���p�X
	 * @param	mmShp1		[out]	Shape�t�@�C�����g�i�[�p�N���X�W���R���e�i
	 * @param	iMesh		[in]	���b�V���R�[�h
	 * @return	bool
	 */
	bool	load_shape_info( string sFullPass, int iMesh, mmShp1& mmShp1 );

	/**
	 * @brief	���Shape�t�@�C���̑����ƕ�ԓ_�����r���A�قȂ�΃G���[�o��
	 * @param	mmShpOri	[in]	��r��Shape�t�@�C�����g�i�[�p�N���X�W���R���e�i<ID, shp1�{��>
	 * @param	mmShpDest	[in]	��r��Shape�t�@�C�����g�i�[�p�N���X�W���R���e�i<ID, shp1�{��>
	 */
	bool	comp_shape_info( mmShp1 mmShpOri, mmShp1 mmShpDest );

	/**
	 * @brief	Shape�t�@�C�����o�C�i���œǂݍ��݁Ashp2�ɗ��Ƃ�����
	 * @param	sFullPass	[in]	�ǂݍ���Shape�t�@�C���̃t���p�X
	 * @param	iMesh		[in]	���b�V���R�[�h
	 * @param	cShp2		[out]	Shape�t�@�C�����g�i�[�p�N���X�i�o�C�i���j
	 * @return	bool
	 */
	bool	load_shape_info_bin( string sFullPass, int iMesh, shp2& cShp2 );

	/**
	 * @brief	���Shape�t�@�C���̃��R�[�h���o�C�i���Ŕ�r���A�قȂ�΃G���[�o��
	 * @param	cShp2Ori	[in]	��r��Shape�t�@�C�����g�i�[�p�N���X�i�o�C�i���j
	 * @param	cShp2Dest	[in]	��r��Shape�t�@�C�����g�i�[�p�N���X�i�o�C�i���j
	 */
	bool	comp_shape_info_bin( shp2 cShp2Ori, shp2 cShp2Dest );


	int					m_iExeMode;					// ON_FID_MODE�̂Ƃ�FID�ŕR�t���A����ȊO��OBJECTID�ŕR�t��
	vector<string>		m_vsMesh;					// ���b�V�����X�g1�s(���b�V���R�[�h+�G���A�R�[�h)���i�[
	string				m_sOriFPass;				// ��r��Shape�t�@�C���p�X
	string				m_sDestFPass;				// ��r��Shape�t�@�C���p�X
	string				m_sLayerName;				// ��r�Ώۃ��C����					
	string				m_sDataSet;					// �f�[�^�Z�b�g��(Citymap��)
	bool				m_bAttr;					// �����`�F�b�N���s�������f����bool�l
	bool				m_bShp;						// �`��`�F�b�N���s�������f����bool�l
	double				m_dDistBorder;				// �`��`�F�b�N�̍ہA�����̋��e�덷
	vector<string>		m_vsFSpecify;				// ��r�t�B�[���h�w�莞�A�w��t�B�[���h���i�[�p

	ofstream			m_ofErrorLog;				// ���O�o�͗p

	crd_cnv				m_cCrd;						
	
	SYSTEMTIME			m_st;						// �J�n�E�I�����Ԏ擾�p
	string				m_sVersion;					// �o�[�W�������o�͗p

//	shp1				m_clOriShp;					// ��r��Shape�n���h���N���X
//	shp1				m_clDestShp;				// ��r��Shape�n���h���N���X

};
