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

#pragma once

#include <map>
#include <string>


/**
 * @class	CIDPoint
 * @brief	ID���X�g�̃��R�[�h
 */
struct CIDPoint
{
	LONG	m_lOID;							/// �I�u�W�F�N�g�h�c
	std::map<CString, CString>	m_mapVal;	/// �f�t�H���g�l
};

/**
 * @class	CMXYPoint
 * @brief	MXY���X�g�̃��R�[�h
 */
struct CMXYPoint
{
	int		m_nMeshCode;					/// ���b�V���R�[�h
	int		m_nMeshX;						/// ���b�V���w
	int		m_nMeshY;						/// ���b�V���x
	std::map<CString, CString>	m_mapVal;	/// �f�t�H���g�l
};

/**
 * @class	CListHandle
 * @brief	�e�탊�X�g�n���h�����O�N���X
 */
class CListHandle
{
public:
	CListHandle(void);
	~CListHandle(void);

	/**
	 * @brief	�h�c���X�g�ǂݍ���
	 * @param	lpszFile	[in]	���X�g�t�@�C��
	 * @param	rVecList	[out]	�h�c���X�g�i�[�R���e�i
	 * @return	bool
	 */
	bool loadIDList( LPCTSTR lpszFile, std::vector<CIDPoint>& rVecList );

	/**
	 * @brief	MeshXY���X�g�ǂݍ���
	 * @param	lpszFile	[in]	���X�g�t�@�C��
	 * @param	rVecList	[out]	MeshXY���X�g�i�[�R���e�i
	 * @return	bool
	 */
	bool loadMXYList( LPCTSTR lpszFile, std::vector<CMXYPoint>& rVecList );

private:

	/**
	 * @brief	�����n�Z�b�g
	 * @param	strData	[in]	�f�[�^
	 * @param	rMapVal	[out]	�����l�i�[�R���e�i
	 * @retval	true : ����
	 * @retval	false : ���s
	 */
	bool setMapVal( const std::string& strData, std::map<CString, CString>& rMapVal );

};
