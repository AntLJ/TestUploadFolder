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

#include <shapelib/DBFHandle.h>
#include "Moji_Chk.h"

using namespace std;

struct	ShpRecord
{
	// �`�F�b�N�Ŏg���̂͂�����
	wstring	wsNameKanji;		// ��������(UTF16)
	wstring	wsNameYomi;			// ���~����(UTF16)
	wstring	wsNameEng;			// �p�ꖼ��(UTF16)

	ShpRecord(){ }
	~ShpRecord(){ }

	bool	operator=(const ShpRecord temp_ShpRecord ) const
	{
		if( wsNameKanji == temp_ShpRecord.wsNameKanji && wsNameYomi == temp_ShpRecord.wsNameYomi )
			return true;
		else
			return false;
	}
};


class	CCheckMultiLanguageRoad
{
public:
	// �R���X�g���N�^
	CCheckMultiLanguageRoad() : m_bIsError( false )
	{
	}
	// �f�N�X�g���N�^
	~CCheckMultiLanguageRoad()
	{
	}

	/**
	 * @brief	�g�p�@
	 */
	static void	printUsage();

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

	/**
	 * @brief	�G���[�����݂������ǂ���
	 * @return	bool
	 */
	bool	IsError() { return m_bIsError; };


private:

	/**
	 * @brief	�����`�F�b�N
	 * @param	argc	[in]	�����̐�
	 * @param	argv	[in]	����
	 * @return	bool
	 */
	bool	arg_check( int argc, char* argv[] );

	/**
	 * @brief	���X�g�t�@�C���ǂݍ���
	 * @param	lpszFile	[in]	�ǂݍ��݂������X�g��
	 * @param	VectorList	[out]	�Ǎ���Vector
	 * @return	bool
	 */
	bool	loadlist( LPCTSTR lpszFile, vector<string> &VectorList );

	/**
	 * @brief	���b�V���P�ʃt�@�C�����̍쐬
	 * @param	strMesh		[in]	���b�V���R�[�h
	 * @param	strPath		[in]	�t�@�C����_�p�X
	 * @param	strLayer	[in]	���C����
	 * @return	string		[out]	�t�@�C���̃t���p�X
	 */
	string	make_mesh_fname( string strMesh, string strPath, string strLayer );

	/**
	 * @brief	dbf�t�@�C����ǂݍ��݁A�`�F�b�N���s��
	 * @param	strMesh			[in]	�ϊ����b�V��
	 * @param	strInFullPath	[in]	���̓t�@�C���̃t���p�X
	 * @return	bool
	 */	
	bool	CheckDBF( string strMesh, string strInFullPath );

	/**
	 * @brief	dbf�t�@�C����1���R�[�h��ǂݍ���
	 * @param	cDBFHdl			[in]	�Y��dbf��dbfHandle�N���X
	 * @param	iRecNum			[in]	�ǂݍ��݂������R�[�h�ԍ�
	 * @param	stShpRecord		[out]	�ǂݍ��ݐ�̍\����
	 * @return	bool
	 */	
	bool	LoadRecord( CDBFHandle cDBFHdl, int iRecNum, ShpRecord &stShpRecord );

	/**
	 * @brief	�����񃋁[���`�F�b�N
	 * @param	stShpRecord		[in]	shape��1���R�[�h(�����A�ǂ݁A�|��̂�)	
	 */	
	void	CCheckMultiLanguageRoad::MojiStrChk( ShpRecord &stShpRecord );

	/**
	 * @brief	������֘A���`�F�b�N
	 * @param	stShpRecord		[in]	shape��1���R�[�h(�����A�ǂ݁A�|��̂�)	
	 */	
	void	CCheckMultiLanguageRoad::MojiRelationChk( ShpRecord &stShpRecord );

	/**
	 * @brief	������֘A���`�F�b�N
	 * @param	stShpRecord		[in]	shape��1���R�[�h(�����A�ǂ݁A�|��̂�)	
	 */	
	void	CCheckMultiLanguageRoad::OutputError( ShpRecord stShpRecord, string sErrorMessage );

private:

	vector<string>		m_vsMesh;				// ���b�V�����X�g1�s(���b�V���R�[�h+�G���A�R�[�h)���i�[
	string				m_Meshlistname;			// ���b�V�����X�g��
	string				m_InLayer;				// ���̓��C����

	string				m_InFilePath;			// �f�[�^���̓p�X��

	ofstream			m_ErrorLog;				// ���O�o�͗p
	SYSTEMTIME			m_st;					// �J�n�E�I�����Ԏ擾�p

	Moji_Chk			e_sChk;					// �`�F�b�N�p�N���X(�{�̂�MultiNoteShpChk��)

	bool				m_bIsError;				// �G���[�����݂������ǂ���
};
