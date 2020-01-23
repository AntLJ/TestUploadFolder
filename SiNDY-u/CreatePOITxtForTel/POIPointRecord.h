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
class POIPointRecord
{
public:
	POIPointRecord() : m_nMeshCode(0), m_nX(0), m_nY(0), m_lGrpCode(0), m_lChainCode(0), m_nAddXYC(0), m_buildingID(0), m_deleteCode(0){}

	bool operator<( const POIPointRecord& ob ) const
	{
		if( m_nAddXYC == 0)
			return	m_strTel < ob.m_strTel;
		else
			return	m_strTel + m_strName < ob.m_strTel + ob.m_strName;
	}

	/**
	 * @brief	���x�D��t��
	 * @param	strAccCode	[in]	���x�R�[�h
	 * @return	�D��x(�Ⴂ�قǗD��x��) 1S(0) > 3A(1) > 1A(2) > UK(3)
	 */
	int GetPriorValueFromAcc() const;

	long	m_lOID;			//!< OBJECTID
	CString	m_strAccCode;	//!< ���x�R�[�h

	int		m_nMeshCode;	//!< 2�����b�V���R�[�h
	int		m_nX;			//!< MeshX
	int		m_nY;			//!< MeshY
	long	m_lGrpCode;		//!< ���ރR�[�h
	long	m_lChainCode;	//!< �X�܃R�[�h
	CString	m_strSeq;		//!< �f�[�^�V�[�P���X
	CString	m_strName;		//!< ����
	CString	m_strTel;		//!< �d�b�ԍ�
	CString	m_strAddrCode1;	//!< �Z���R�[�h1
	CString	m_strAddrCode2;	//!< �Z���R�[�h2
	int		m_nAddXYC;		//!< �}�b�`���O���
	long	m_buildingID;	//!< ����OBJECTID
	long	m_deleteCode;	//!< �폜�R�[�h 0:���� 1:�폜 2:�폜�ۗ�
};

