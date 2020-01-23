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

/**
 * @file SpatialIndex.h
 * @brief <b>CSpatialIndex�N���X��`�t�@�C��</b>\n
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * @vertion $Id$
 */
#ifndef _SPATIALINDEX_H_
#define _SPATIALINDEX_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AheWKSFunctions.h"
#include "ErrorCode.h"
#include <list>

namespace sindy {

/**
 * @class CSpatialIndex
 * @brief <b>��ԃC���f�b�N�X�N���X</b>\n
 * ���̃N���X�͋[���I�ȋ�ԃC���f�b�N�X�̍쐬�E�Ǘ���
 * ����N���X�ł��B
 * ArcObjects�̋�ԃC���f�b�N�X�Ƃ͊֌W����܂���B
 */
class SINDYLIB_API CSpatialIndex
{
public:
	explicit CSpatialIndex()
	{
		m_cRect.XMin = m_cRect.YMin = m_cRect.XMax = m_cRect.YMax = 0;
		m_nDivideX = m_nDivideY = 0;
		m_pIndex = NULL;
	}
	virtual ~CSpatialIndex()
	{
		DeleteIndex();
	}

	/**
	 * @brief <b>��ԃC���f�b�N�X�̃T�C�Y���w�肷��</b>\n
	 * �����̃C���f�b�N�X�͍폜����܂��B
	 * @param nMeshCode	[in]	���b�V���R�[�h
	 * @param nDivideX	[in]	��`�̉�������
	 * @param nDivideY	[in]	��`�̏c������
	 */
	void Init( int nMeshCode, int nDivideX, int nDivideY );

	/**
	 * @brief <b>��ԃC���f�b�N�X�̃T�C�Y���w�肷��</b>\n
	 * �����̃C���f�b�N�X�͍폜����܂��B
	 * @param cRect		[in]	�C���f�b�N�X�쐬�Ώۋ�`
	 * @param nDivideX	[in]	�C���f�b�N�X�쐬�Ώۋ�`�̉�������
	 * @param nDivideY	[in]	�C���f�b�N�X�쐬�Ώۋ�`�̏c������
	 */
	void Init( const WKSEnvelope& cRect, int nDivideX, int nDivideY )
	{
		DeleteIndex();
		m_cRect = cRect;
		m_nDivideX = nDivideX;
		m_nDivideY = nDivideY;
		CreateIndex();
	}

	/**
	 * @brief <b>�C���f�b�N�X�ɗv�f��ǉ�����</b>\n
	 * @param cRect		[in]	�C���f�b�N�X���쐬�������`����܂����`
	 * @param lID		[in]	�C���f�b�N�X���쐬�������`���ID
	 */
	errorcode::sindyErrCode Add( const WKSEnvelope& cRect, long lID );

	/**
	 * @brief <b>�C���f�b�N�X����ID����������</b>\n
	 * @param cRect		[in]	������`
	 * @param listIDs	[out]	�C���f�b�N�X�Ɉ�����������ID�̃��X�g
	 * @retval listIDs�̌�
	 */
	long Search( const WKSEnvelope& cRect, std::list<long>& listIDs ) const;
#ifdef _DEBUG
	void Trace() const
	{
		for( int i = 0; i < m_nDivideX; ++i )
		{
			for( int j = 0; j < m_nDivideY; ++j )
				ATLTRACE(_T("m_pIndex[%d][%d] = %d\n"), i, j, m_pIndex[i][j].size() );
		}
	}
#endif // ifdef _DEBUG
private:
	/**
	 * @brief <b>�����C���f�b�N�X���쐬����</b>\n
	 */
	void CreateIndex();

	/**
	 * @brief <b>�C���f�b�N�X���폜����</b>\n
	 */
	void DeleteIndex();

	/**
	 * @brief <b>�C���f�b�N�X���[�v��XY�̊J�n�E�I���C���f�b�N�X�ԍ����擾����</b>\n
	 * @param d			[in]	��r�Ώ�
	 * @param bIsX		[in]	X���W�l���ǂ���
	 * @param bFlag		[in]	true�Ȃ�C���f�b�N�X�̋��ł���ꍇ�ɏ������ق���Ԃ�
	 * @retval -1	�Ǘ����b�V���ΏۊO
	 * @retval 0�ȏ� �C���f�b�N�X���[�v�̃C���f�b�N�X�ԍ�
	 */
	int GetIndex( const double& d, const bool bIsX, const bool bFlag, double* dMin = NULL, double* dMax = NULL, int* nStart = NULL, int* nEnd = NULL ) const;

private:
	int					m_nDivideX;		//!< �C���f�b�N�X�쐬�Ώۋ�`�̉�������
	int					m_nDivideY;		//!< �C���f�b�N�X�쐬�Ώۋ�`�̏c������
	WKSEnvelope			m_cRect;		//!< �C���f�b�N�X�쐬�Ώۋ�`
	std::list<long>**	m_pIndex;		//!< �C���f�b�N�X���X�g
};

} // sindy

#endif // ifndef _SPATIALINDEX_H_
