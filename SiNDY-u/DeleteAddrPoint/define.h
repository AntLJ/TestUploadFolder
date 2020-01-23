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
 * @file	define.h
 * @brief	�e���`
 */

using namespace std;

#define PROGRAM_NAME	_T("CheckTelTPError")	// �v���O������

/**
 * @struct	CMeshXY
 * @brief	���b�V��XY
 */
struct CMeshXY
{
	int			m_nMesh;	//!< ���b�V���R�[�h
	int			m_nX;		//!< ���b�V��X
	int			m_nY;		//!< ���b�V��Y

	CMeshXY() : m_nMesh(0), m_nX(0), m_nY(0){}

	/**
	 * @brief	== �̃I�[�o�[���[�h
	 * @note	�����ł́A���b�V���R�[�h���������@���� ���K��XY��100�����̍��ł���Γ������Ƃ���
	 * @return	bool
	 */
	bool operator==( const CMeshXY& ob )
	{
		return (m_nMesh == ob.m_nMesh) && (abs(m_nX - ob.m_nX) < 100) && (abs(m_nY - ob.m_nY) < 100);
	}

};
