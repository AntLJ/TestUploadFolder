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

#include <SiNDYLib/FeatureClass.h>

/**
 * @class	CTelTPHelper
 * @brief	TelTP�|�C���g�w���p�[�����N���X
 * @note	TelTP�|�C���g�ɑ΂���F�X�ȏ������s��
 */
class CTelTPHelper
{
public:
	CTelTPHelper( CFeatureClass& cTelTPPointFC ) : 
		m_cTelTPPointFC(cTelTPPointFC)
	{}
	~CTelTPHelper(void){};

	/**
	 * @brief	TelTP�|�C���g����
	 * @param	cRec		[in,out]	���X�g���R�[�h
	 * @param	bAddMode	[in]		�t�^���[�h�H
	 * @return	EResultStat(�`�F�b�N���ʃX�e�[�^�X)
	 */
	EResultStat findPoint( CLRec& cRec, bool bAddMode );

	/**
	 * @brief	TelTP�|�C���g�̏��擾
	 * @param	cRec	[in, out]	���X�g���R�[�h
	 * @param	ipFeature	[in]	TelTP��ipFeaturePtr
	 * @return	bool
	 */
	bool getInfo( CLRec& cRec, CRow cTelTPPoint );

	/**
	 * @brief	�w���ʂ̃h���C���擾(���󖢎g�p�A�g�p���邽�߂ɂ�CheckTelTPError::outputLog��CTelTPHelper��ǉ�����B)
	 * @param	lpszDomainName	[in]	�擾�������h���C���̃t�B�[���h��
	 * @param	lDomainCode		[in]	�擾�������h���C���̃R�[�h�l
	 * @return	CString					�w���ʂ̃R�[�h�l�ɑΉ�����h���C����
	 */
	const std::string CTelTPHelper::getDomain(LPCTSTR lpszDomainName, long lDomainCode) const;

private:

	CFeatureClass	m_cTelTPPointFC;		//!< TelTP�|�C���g�t�B�[�`���N���X
	IFeaturePtr		m_ipFeature;			//!< TelTP�|�C���g�̃t�B�[�`��
};
