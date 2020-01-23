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
#include <string>
#include <stdio.h>
#include <algorithm>
#define _USE_SHAPELIB_STATIC
#include <libshape.h>
#include <DBFHandle.h>
#include <shapelib\SHPHandle.h>

#define ON_DEFAULT			0x00
#define	ON_NORMAL_MODE		0x01
#define	ON_CITYCODE_MODE	0x02
#define	ON_ROADCODE_MODE	0x04

using namespace std;

struct FieldInfo
{
	int				iID;		// �t�B�[���h�ԍ�
	DBFFieldType	dType;		// �t�B�[���h�^�C�v(0:FTString,1:FTInteger,2:FTDouble,3:FTLogical,4:FTInvalid)
	string			sName;		// �t�B�[���h��
	int				iWidth;		// �t�B�[���h�T�C�Y
	int				iDecimals;	// �t�B�[���h���x
	int				iIgnoreFlag;	// �ϊ������t�B�[���h(1:string�^�ł��ϊ����Ȃ�)

	FieldInfo()
	{
		iID			= 0;
		dType		= FTInvalid;
		iWidth		= 0;
		iDecimals	= 0;
		iIgnoreFlag	= 0;
	}
	~FieldInfo()
	{
	}

	bool	operator<(const FieldInfo temp_FieldInfo) const
	{
		if( iID < temp_FieldInfo.iID )
			return true;
		else
			return false;
	}
	bool	operator=(const FieldInfo temp_FieldInfo) const
	{
		if( iID == temp_FieldInfo.iID )
			return true;
		else
			return false;
	}
};

class	CConvSJIStoUTF8
{

public:

	// �R���X�g���N�^
	CConvSJIStoUTF8()
	{
		m_iExeMode	= ON_DEFAULT;
		m_DbfMode = false;
	}

	// �f�N�X�g���N�^
	~CConvSJIStoUTF8()
	{
	}

	/**
	 * @brief	�g�p�@
	 */
	static void	printUsage();

	/**
	 * @brief	�����`�F�b�N
	 * @param	argc	[in]	�����̐�
	 * @param	argv	[in]	����
	 * @return	bool
	 */
	bool	arg_check( int argc, char* argv[] );

	/**
	 * @brief	������
	 * @param	argc	[in]	�����̐�
	 * @param	argv	[in]	����
	 * @return	bool
	 */
	bool	init( int argc, char* argv[] );

	/**
	 * @brief	���s
	 * @return	bool
	 */
	bool	execute();

private:

	/**
	 * @brief	���X�g�t�@�C���ǂݍ���
	 * @param	lpszFile		[in]	�ǂݍ��݂������X�g��
	 * @param	vsVectorList	[out]	�Ǎ���Vector(1�t�B�[���h����)
	 * @return	bool
	 */
	bool	loadlist( LPCTSTR lpszFile, vector<string>& vsVectorList );

	/**
	 * @brief	dbf�t�@�C����UTF8�ϊ�
	 * @param	���b�V��	[in]	�ϊ����b�V��
	 * @param	�p�X		[in]	���̓t�@�C���̃t���p�X
	 * @param	�p�X		[in]	�o�̓t�@�C���̃t���p�X
	 * @return	bool
	 */	
	bool	ConvertDBF( string Mesh, string fullpass, string destfullpass );

	/**
	 * @brief	���b�V���P�ʃt�@�C�����̍쐬
	 * @param	MeshCode	[in]	���b�V���R�[�h
	 * @param	cPathName	[in]	�t�@�C����_�p�X
	 * @return	�t�@�C���̃t���p�X
	 */
	string	make_mesh_fname( string MeshCode,	char* cpPathName );

	/**
	 * @brief	�ϊ��Ώۃ��C������A������t�B�[���h�̃C���f�b�N�X���擾
	 * @param	fullpass	[in]	�t�@�C���̃t���p�X
	 * @return	bool
	 */
	bool	GetFieldInfo( string sFullPass );

	/**
	 * @brief	�����R�[�h��Shift-JIS�����肷��B
	 * @param	cChars	[in]	������
	 * @return	bool
	 */
	bool	CheckShiftJIS( const char* cChars );

	string				m_sFilePass;				// �ϊ��Ώ�Shape�t�@�C���p�X(��_)
	string				m_sDestFilePass;			// �ϊ���Shape�t�@�C���p�X

	string				m_sLayerName;				// �����Ώۃ��C����
	string				m_sDataSet;					// �f�[�^�Z�b�g��(Citymap,Road��)

	vector<string>		m_vsMesh;					// ���b�V�����X�g1�s���i�[
	vector<string>		m_vsAllLayer;				// 05Shape���݃��C���S�Ă��i�[
	vector<string>		m_vsIgnoreField;			// �ϊ��������C���i�[(string�����A���l(+,)���������Ȃ��t�B�[���h)

	vector<FieldInfo>	m_vfFieldInfo;				// �t�B�[���h�ԍ�,�t�B�[���h�^�C�v,�t�B�[���h��,�t�B�[���h�T�C�Y,���x,�ϊ������t���O�����\���̂�vector

	int					m_iExeMode;					// ���s���[�h(citycode, roadcode, DBF�̂�, �ʏ탌�C��)

	ofstream			m_ErrorLog;					// ���O�o�͗p
	SYSTEMTIME			m_st;						// �J�n�E�I�����Ԏ擾�p

	bool				m_DbfMode;					// dbf�ϊ����[�h(�ʏ��shape�ł��邱�ƑO��)
};
