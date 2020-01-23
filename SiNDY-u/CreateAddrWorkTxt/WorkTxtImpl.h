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

#include "Parameter.h"
#include <crd_cnv/coord_converter.h>

/**
 * @class	CWorkTxtImple
 * @brief	���ԃf�[�^�o�͊��N���X
 * @note	���̃N���X���p�����āA�e�|�C���g���Ƃ̎������s��
 */
class CWorkTxtImpl
{
public:
	CWorkTxtImpl( const CParameter& rParam, std::ofstream& ofs ) : m_rParam(rParam), m_ofsLog(ofs){}
	virtual ~CWorkTxtImpl(void){}

	/**
	 * @brief	������
	 * @note	�p����ŕK������
	 * @return	bool
	 */
	virtual bool Init() = 0;

	/**
	 * @brief	�f�[�^�o��
	 * @note	�p����ŕK������
	 * @return	bool
	 */
	virtual bool WriteData() = 0;

protected:

	/**
	 * @brief	�ܓx�E�o�x����iPC�s�s�n�}���b�V���R�[�h�擾
	 * @param	dLon	[in]	�o�x
	 * @param	dLat	[in]	�ܓx
	 * @return	���b�V���R�[�h(���s������-1)
	 */
	int GetMeshCode( double dLon, double dLat )
	{
		static crd_cnv g_cCrdCnv;
		int nMeshcode = 0, nX = 0, nY = 0;
		return g_cCrdCnv.LLtoMesh(dLon, dLat, 64, &nMeshcode, &nX, &nY)  == 0 ? nMeshcode : -1;
	}

	/**
	 * @brief	�G���[���O�o��
	 * @param	pRow		[in]	CRow
	 * @param	lpcszLevel	[in]	�G���[���x��
	 * @param	lpcszMsg	[in]	�G���[���b�Z�[�W
	 * @param	lpcszInfo	[in]	�ǉ����
	 */
	void PrintError( const CRow* pRow, const _TCHAR* lpcszLevel, const _TCHAR* lpcszMsg, const _TCHAR* lpcszInfo = NULL )
	{
		CString strLog;
		strLog.Format( _T("0\t%s\t%d\t\t\t%s\t\t%s\t%s"),
					   pRow->GetTableName(), pRow->GetOID(), lpcszLevel, lpcszMsg, lpcszInfo? lpcszInfo : _T("") );
		m_ofsLog << CT2CA(strLog) << std::endl;
	}

protected:

	const CParameter&	m_rParam;			//!< ���s���

private:

	std::ofstream&		m_ofsLog;			//!< ���O�X�g���[��
};
