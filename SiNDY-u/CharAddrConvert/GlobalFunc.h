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

/**
 * @file	GlobalFunc.h
 * @brief	�ǂ��g���֐��S��`�t�@�C��
 */

// ��ʓI�Ȋ֐�
namespace gf
{
	/**
	 * @brief	���[�N�X�y�[�X�擾(���̂��ڑ����s���邱�Ƃ�����̂�5�񃊃g���C)
	 * @note	SDE PGDB SHAPE ��������n�j
	 * @param	lpcszDBProp	[in]	�c�a�ڑ��v���p�e�B
	 * @return	���[�N�X�y�[�X
	 */
	IWorkspacePtr GetWorkspace( LPCTSTR lpcszDBProp );

	/**
	 * @brief	��Ԍ��� (+ ��������)
	 * @param	ipGeo			[in]	�Ώۂ̃W�I���g��
	 * @param	ipFeatureClass	[in]	�������̃t�B�[�`���N���X
	 * @param	pCount			[out]	�������R�[�h��([Bug 6175]�����擾���Ȃ��Ȃ�ANULL�w��)
	 * @param	spatialRel		[in]	��������
	 * @param	lpszWhereClause	[in]	��������������Ȃ�w�肷��(�f�t�H���g��NULL)
	 * @param	lpszSubFields	[in]	�K�v�ȃt�B�[���h(�J���}��؂�)
	 * @return	�t�B�[�`���J�[�\��(�������R�[�h�� = 0�Ȃ�ANULL�Ԃ�)
	 */
	IFeatureCursorPtr SelectByShape( IGeometryPtr ipGeo, IFeatureClassPtr ipFeatureClass, long* pCount = NULL,
									 esriSpatialRelEnum spatialRel = esriSpatialRelIntersects,
									 LPCTSTR lpszWhereClause = NULL, LPCTSTR lpszSubFields = NULL );

	/**
	 * @brief	��������
	 * @param	ipTable			[in]	�������̃t�B�[�`���N���X
	 * @param	pCount			[out]	�������R�[�h��([Bug 6175]�����擾���Ȃ��Ȃ�ANULL�w��)
	 * @param	lpszWhereClause	[in]	��������������Ȃ�w�肷��(�f�t�H���g��NULL)
	 * @param	lpszSubFields	[in]	�K�v�ȃt�B�[���h(�J���}��؂�)
	 * @return	�t�B�[�`���J�[�\��(�������R�[�h�� = 0�Ȃ�ANULL�Ԃ�)
	 */
	_ICursorPtr SelectByAttr( ITablePtr ipTable, long* pCount = NULL, LPCTSTR lpszWhereClause = NULL, LPCTSTR lpszSubFields = NULL );
}

// �Z���n�ɓ���������ʓI�Ȋ֐�
namespace addr_func
{
	static crd_cnv g_cCrdCnv;

	/**
	 * @brief	���p����?
	 * @param	strData	[in]	�f�[�^
	 * @return	bool
	 */
	bool IsNumStr( const CString& strData );

	/**
	 * @brief	�ʏ�̒n��?
	 * @param	strPnt	[in]	�n�Ԗ���
	 * @return	bool
	 */
	inline bool IsNormalPnt( const CString& strPnt ){ return IsNumStr(strPnt) && strPnt != _T("0") && _tstoi(strPnt) > 0 && _tstoi(strPnt) < 100000; }

	/**
	 * @brief	�ʏ�̍��ԍ�?
	 * @param	strGou	[in]	���ԍ�
	 * @return	bool
	 */
	inline bool IsNormalGou( const CString& strGou ){ return IsNumStr(strGou) && strGou != _T("0") && _tstoi(strGou) > 0 && _tstoi(strGou) < 10000; }

	/**
	 * @brief	CGouRec��r�֐�(��\�Z���I��p)
	 * @param	lhs	[in]	�����R�[�h1
	 * @param	rhs	[in]	�����R�[�h2
	 * @return	bool
	 */
	bool CompareGouRec( const CGouRec& lhs, const CGouRec& rhs );

	/**
	 * @brief	�����R�[�h�ʒu�I�ȗD��x����֐�
	 * @note	��蓌�E���k�ɂ�����̂�D��
	 * @param	lhs	[in]	GouRec1
	 * @param	rhs	[in]	GouRec2
	 * @retval	-1 : ������D��
	 * @retval	 0 : ����ʒu
	 * @retval	 1 : �E����D��
	 */
	int CompareGouRecPos( const CGouRec& lhs, const CGouRec& rhs );

	/**
	 * @brief	�����R�[�h�Ԃ̋����Z�o
	 * @param	lhs	[in]	GouRec1
	 * @param	rhs	[in]	GouRec2
	 * @return	����
	 */
	inline double GetDistGouRec( const CGouRec& lhs, const CGouRec& rhs )
	{
		return g_cCrdCnv.GetMeshDist( lhs.m_cMXY.m_nMeshCode, lhs.m_cMXY.m_nX, lhs.m_cMXY.m_nY, rhs.m_cMXY.m_nMeshCode, rhs.m_cMXY.m_nX, rhs.m_cMXY.m_nY );
	}

	/**
	 * @brief	���p�p������S�p��
	 * @param	lpcszData	[in]	�f�[�^
	 * @return	�ϊ��f�[�^(���s������A���̂܂ܕԂ�)
	 */
	inline CString GetZenkakuData( LPCTSTR lpcszData )
	{
		TCHAR szConvData[100] = _T("");
		int nRet = ::LCMapString( ::GetUserDefaultLCID(), LCMAP_FULLWIDTH, lpcszData, _tcslen(lpcszData), szConvData, sizeof(szConvData) );
		return nRet? szConvData : lpcszData;
	}

}
