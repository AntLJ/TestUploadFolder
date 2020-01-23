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

// CreateList.h: CCreateList �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CREATELIST_H__17BD2DE6_7C62_45EF_A27D_B864FECBE3CE__INCLUDED_)
#define AFX_CREATELIST_H__17BD2DE6_7C62_45EF_A27D_B864FECBE3CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

using namespace std;

/**
 * @class	CCreateList
 */
class CCreateList  
{
public:
	CCreateList();
	~CCreateList();

	/**
	 * @brief	�g�p�@
	 */
	static void printUsage();

	/**
	 * @brief	������
	 * @param	�R�}���h���C���������̂܂�
	 * @return	true: ����	false: ���s
	 */
	bool init( int argc, char* argv[] );

	/**
	 * @brief	���s
	 */
	bool execute();

private:
	/**
	 * @breif	�N���|�����X�g�쐬
	 * @param	ipFeature	[in]	�N���[���|���t�B�[�`��
	 */
	void makeClaimPolyList( IFeature* ipFeature );

	/**
	 * @brief	�N���|�����o��
	 * @param	lOID		[in]	�I�u�W�F�N�gID
	 * @param	lpcszClaimNo	[in]	�N���[���ԍ�
	 * @param	nUselessF	[in]	�s�v�t���O
	 */
	void printInfo( long lOID, const _TCHAR* lpcszClaimNo, long lUselessF )
	{
		m_fout << lOID << "," << lpcszClaimNo << "," << lUselessF << endl;
	}

/////////////////////////////
//	�����o�ϐ�
/////////////////////////////
private:

	std::ofstream		m_fout;					//!< �o�̓X�g���[��

	IFeatureClassPtr	m_ipClaimPolyClass;		//!< �N���|���t�B�[�`���N���X	

	long				m_lClaimNumberFID;		//!<! �N���[���ԍ��t�B�[���h�C���f�b�N�X
	long				m_lUselessF_FID;		//!<! �s�v�t���O�t�B�[���h�C���f�b�N�X
};

#endif // !defined(AFX_CREATELIST_H__17BD2DE6_7C62_45EF_A27D_B864FECBE3CE__INCLUDED_)
