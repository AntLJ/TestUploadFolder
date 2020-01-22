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
#include <AddrLib/GouPoints.h>

using namespace addr;

/**
 * @class	CGPHelper
 * @brief	���|�C���g�w���p�[�����N���X
 * @note	���|�C���g�ɑ΂���F�X�ȏ������s��
 */
class CGPHelper
{
public:
	CGPHelper( CFeatureClass& cGouPointFC, CFeatureClass& cGouPointFC_Ref, const CFeatureClass& cCityAdminFC, const CFeatureClass& cCityAdminFC_Ref ) : 
		m_cGouPointFC(cGouPointFC),
		m_cCityAdminFC(cCityAdminFC),
		m_cGous(cCityAdminFC),
		m_cGouPointFC_Ref(cGouPointFC_Ref),
		m_cCityAdminFC_Ref(cCityAdminFC_Ref)
	{}
	~CGPHelper(void){}

	/**
	 * @brief	���|�C���g����
	 * @param	cRec		[in,out]		���X�g���R�[�h
	 * @param	bAddMode	[in]			�t�^���[�h�H
	 * @return	EResultStat(�`�F�b�N���ʃX�e�[�^�X)
	 */
	EResultStat findPoint( CLRec& cRec, bool bAddMode );

	/**
	 * @brief	���|�C���g�ړ�
	 * @param	cRec	[in, out]	���X�g���R�[�h
	 * @param	bPriorF	[in]		�ړ��|�C���g�́A�D��t���O�����Ă�H
	 * @return	EResultStat(�ړ��������ʃX�e�[�^�X)
	 */
	EResultStat movePoint( CLRec& cRec, bool bPriorF );

	/**
	 * @brief	���|�C���g�ǉ�
	 * @param	cRec		[in, out]	���X�g���R�[�h
	 * @return	EResultStat(�ړ��������ʃX�e�[�^�X)
	 */
	EResultStat insertPoint( CLRec& cRec );

private:

	/**
	 * @brief	�`�F�b�N�^�C�v���Ƃ̍s���EWhere��쐬
	 * @param	cRec	[in]	���X�g���R�[�h
	 * @param	emCType	[in]	�`�F�b�N�^�C�v
	 * @return	Where��
	 */
	CString MakeWhereClause( const CLRec& cRec, ECheckType emCType );

	/**
	 * @brief	�w��s���E���̍��|�C���g�m�F
	 * @param	cRec	[in]	���X�g���R�[�h
	 * @param	rGou	[out]	���|�C���g���
	 * @param	emCType	[in]	�`�F�b�N�^�C�v
	 * @retval	kO_NOT_FOUND      : ������Ȃ�(�ꎞ�X�e�[�^�X)
	 * @retval	kO_MATCH_PRIOR_GP : Gou�ƗD��t���O����|�C���g�ň�v(�ꎞ�X�e�[�^�X)
	 * @retval	kO_MATCH_GP       : Gou�ƗD��t���O�Ȃ��|�C���g�ň�v(�ꎞ�X�e�[�^�X)
	 * @retval	kW_GP_DUPLI_PRIOR : �y�v�m�F�z����20���R�[�h�ŗD��t���O�����Ă�����̂���������	
	 */
	EResultStat findGPByAdmin( const CLRec& cRec, CSPGouPoint& rGou, ECheckType emCType );

	/**
	 * @brief	���R�[�h�ƈ�v�������|�C���g�̊֌W����ڍ׃X�e�[�^�X�擾
	 * @param	cRec		[in,out]	���R�[�h
	 * @param	pGou		[in]		��v�������|�C���g
	 * @param	bAddMode	[in]		�t�^���[�h�H
	 * @return	EResultStat(�`�F�b�N���ʃX�e�[�^�X)
	 */
	EResultStat getDetailStatus( CLRec& cRec, const CSPGouPoint& pGou, bool bAddMode );
	
	/**
	 * @brief	�ړ��E�ǉ��\��|�C���g�ƎQ��DB�̃|�C���g�Ƃ̔�r
	 * @param	cRec		[in]	���R�[�h
	 * @param	pGou		[in]	�ړ��E�ǉ��\��̃|�C���g
	 * @param   bIsMovePoint	[in]	pGou���ړ��|�C���g���ǂ���
	 * @return	�ړ��E�ǉ��̉� (true�F��)
	 */
	bool checkGPByReferenceDB( CLRec& cRec, CCityAdmin& cAdmin, CGouPoint& cGou, bool bIsMovePoint );

private:

	CGouPoints			m_cGous;			//!< ���|�C���g���X�g�Ǘ��N���X

	CFeatureClass		m_cGouPointFC;		//!< ���|�C���g�t�B�[�`���N���X
	CFeatureClass		m_cGouPointFC_Ref;	//!< �Q�Ɨp���|�C���g�t�B�[�`���N���X(bug9300)
	const CFeatureClass	m_cCityAdminFC;		//!< �s�s�n�}�s���E�t�B�[�`���N���X
	const CFeatureClass	m_cCityAdminFC_Ref;	//!< �Q�Ɨp�s�s�n�}�s���E�t�B�[�`���N���X(bug9300)
};
