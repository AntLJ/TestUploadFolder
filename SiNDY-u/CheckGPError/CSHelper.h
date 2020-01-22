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
 * @class	CCSHelper
 * @brief	CS�|�C���g�w���p�[�����N���X
 * @note	CS�|�C���g�ɑ΂���F�X�ȏ������s��
 */
class CCSHelper
{
public:
	CCSHelper( CFeatureClass& cCSPointFC, const CFeatureClass& cCityAdminFC) : 
		m_cCSPointFC(cCSPointFC),
		m_cCityAdminFC(cCityAdminFC)
	{}
	~CCSHelper(void){};

	/**
	 * @brief	CS�|�C���g����
	 * @param	cRec		[in,out]	���X�g���R�[�h
	 * @param	bAddMode	[in]		�t�^���[�h�H
	 * @return	EResultStat(�`�F�b�N���ʃX�e�[�^�X)
	 */
	EResultStat findPoint( CLRec& cRec, bool bAddMode );

	/**
	 * @brief	CS�|�C���g�ړ�
	 * @param	cRec	[in, out]	���X�g���R�[�h
	 * @return	EResultStat(�ړ��������ʃX�e�[�^�X)
	 */
	EResultStat movePoint( const CLRec& cRec );

	/**
	 * @brief	CS�|�C���g�ǉ�
	 * @param	cRec	[in, out]	���X�g���R�[�h
	 * @return	EResultStat(�ړ��������ʃX�e�[�^�X)
	 */
	EResultStat insertPoint( CLRec& cRec );

private:

	CFeatureClass	m_cCSPointFC;		//!< CS�|�C���g�t�B�[�`���N���X
	const CFeatureClass	m_cCityAdminFC;		//!< �s�s�n�}�s���E�t�B�[�`���N���X
};
