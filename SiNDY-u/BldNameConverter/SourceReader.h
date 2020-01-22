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
#include "LogSys.h"
#include "common.h"

/**
 * @brief �ϊ����f�[�^�ǂݍ��݃N���X
 */
class SourceReader
{
public:
	SourceReader(
		const IFeatureClassPtr& ipBldNamePt,
		const IFeatureClassPtr& ipBld,
		const FieldIndexMap* bldNamePtIndeces,
		const FieldIndexMap* bldIndeces,
		const std::set<long>* bldExcList
		);
	~SourceReader(void);

	/**
	 * @brief  �ϊ����f�[�^�ǂݍ��݊֐�
	 * @param  ipMeshGeometry [in] �Ώۃ��b�V���`��
	 * @param  bldFeatures [out] �������
	 * @param  bldNamePtFeatures [out] �������̏��
	 * @param  bldNamePtOIDs [out] 
	 * @param  existWrongData [out] �s���ȃf�[�^�����݂������ǂ���
	 * @retval true ����
	 * @retval false ���s
	 */
	bool readSource(const IGeometryPtr& ipMeshGeometry, IDFeatureMap* bldFeatures, IDFeatureMap* bldNamePtFeatures, IDFeatureMap* bldNamePtOIDs, bool* existWrongData);

private:
	/**
	 * @brief  �����Ώی����擾&�擾����������S�Ċ܂ދ�`�擾
	 * @param  ipMeshGeometry [in] ���b�V����`
	 * @param  bldFeatures [out] �����Ώی���
	 * @param  ipExtentGemetry�@[out] ������S�Ċ܂ދ�`
	 * @retval true ����
	 * @retval false ���s
	 */
	bool getTgtBldAndExtentGeometry(const IGeometryPtr ipMeshGeometry, IDFeatureMap* bldFeatures, IGeometryPtr& ipExtentGemetry);

	/**
     * @brief  �����Ώی������̎擾
	 * @note   �������̂����݂��郁�b�V���ł����ϊ��͂���Ȃ����A�����ɕR�Â��������̂����݂��邩�̃`�F�b�N���K�v�ł��邽�߁A�G�N�X�e���g������`�Ŏ擾
	 * @param  ipExtentGeometr [in] ���b�V�����G�N�X�e���g������`
	 * @param  bldNamePtFeatures [out] �����Ώی�������
	 * @param  bldNamePtIDs [out] �����Ώی�������OBJECTID���X�g
	 * @retval true ����
	 * @retval false ���s
	 */
	bool getTgtBldNamePt(const IGeometryPtr ipExtentGeometry, IDFeatureMap* bldNamePtFeatures, IDFeatureMap* bldNamePtOIDs, bool* existWrongData);

private:
	const IFeatureClassPtr	m_ipBldNamePt;		//!< �����r�����̃|�C���g�E�t�B�[�`���N���X
	const IFeatureClassPtr	m_ipBld;			//!< �����|���S���E�t�B�[�`���N���X

	const FieldIndexMap*	m_bldNamePtIndeces;	//!< �����r�����̃|�C���g�E�t�B�[���h�C���f�b�N�X�Ή��\
	const FieldIndexMap*	m_bldIndeces;		//!< �����|���S���E�t�B�[���h�C���f�b�N�X�Ή��\

	const std::set<long>*	m_bldExcList;		//!< �����ΏۊO������ʃ��X�g
};
