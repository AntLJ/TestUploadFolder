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
 * @brief �����r�����̃f�[�^�ϊ��N���X
 */
class BDNCooker
{
public:
	BDNCooker(const FieldIndexMap* bldNamePtIndeces);
	~BDNCooker(void);

	/**
	 * @brief  BDN�ϊ��f�[�^�쐬
	 * @param  ipMeshGeometry [in] ���b�V����`
	 * @param  bldFeatures [in] �������
	 * @param  bldNamePtFeatures [in] �������̃|�C���g���
	 * @param  bldNamePtIDs [in, out] �����Ώی������̃|�C���gID�Q
	 * @param  cnvInfo [out] BDN�ϊ����
	 * @param  existWrongData [out] �s���ȃf�[�^�����݂��邩�ǂ���(true: ���݂���, false: ���݂��Ȃ�)
	 * @retval true ����
	 * @retval false ���s
	 */
	bool cookBDN(
		const IGeometryPtr& ipMeshGeometry, 
		const IDFeatureMap& bldFeatures, 
		const IDFeatureMap& bldNamePtFeatures,
		IDFeatureMap* bldNamePtIDs, 
		IDRecordMap* cnvInfo,
		bool* existWrongData
		);

private:
	/**
	 * @brief  �����ߌ`��쐬�֐�
	 * @param  bldinfo [in] �������
	 * @param  ipInfillBldGeometry [out] �����ߌ`��
	 * @retval true ����
	 * @retval false ���s
	 */
	bool makeInfillBldGeometry(const IDFeatureMap::value_type& bldInfo, IGeometryPtr& ipInfillBldGeometry);

	/**
	 * @brief  �����Ɋ֘A���錚���r�����̎擾
	 * @param  bldID [in] ������OBJECTID
	 * @param  ipInfillBldGeometry [in] �����ߌ����`��
	 * @param  bldNamePtFeature [in] �����r�����̌Q	
	 * @param  meshMin [in] �����Ώۃ��b�V����`�ŏ����W
	 * @param  meshMax [in] �����Ώۃ��b�V����`�ő���W
	 * @param  bldNamePtIDs [in, out] �����Ώی������̃|�C���gID�Q
	 * @param  cnvInfo [out] BDN�ϊ����
	 * @param  relExist [out] �֘A���錚���r�����̂����݂������ǂ���(ture: ���݂��� false: ���݂��Ȃ�����)
	 * @param  posChk [out] �R�Â��������r�����̂������`���ɑ��݂������̃`�F�b�N(ture: ���݂��� false: ���݂��Ȃ�����)
	 * @retval true ����
	 * @retval false ���s
	 */
	bool getRelatedBldNamePt(
		long bldID,
		const IGeometryPtr& ipInfillBldGeometry,
		const IDFeatureMap& bldNamePtFeatures,
		const LONLAT& meshMin,
		const LONLAT& meshMax,
		IDFeatureMap* bldNamePtIDs, 
		IDRecordMap* cnvInfo,
		bool* relExist,
		bool* posChk
		);

	/**
	 * @brief  IRingPtr �� IPolygonPtr�ϊ��֐�(�����O��ExteriorRing�ł��邱������)
	 * @param  bldID [in] ������objectid
	 * @param  ipRing [in] �Ώ�IRingPtr
	 * @param  ipSpRef [in] ��ԎQ��
	 * @param  ipPolygon [out] �|���S���`��
	 * @retval true ����
	 * @retval false ���s
	 */
	bool ring2Polygon(long bldID, const IRingPtr& ipRing, const ISpatialReferencePtr& ipSpRef, IPolygonPtr& ipPolygon);

	/**
	 * @biref  �����r�����̗p�|�C���g���ϊ��Ώۂ��`�F�b�N����B
	 * @note   �ϊ������F�|�C���g�����b�V����`�̒��Ɋ܂܂��B���b�V�����E����ɑ��݂���ꍇ�́A�������ق��̃��b�V���ŕϊ�����B
	 * @param  tableName [in] �e�[�u����
	 * @param  id [in] �Ώۃ|�C���g��OBJECTID
	 * @param  ipPtGeometry [in] �|�C���g�`��
	 * @param  meshMin [in] �����Ώۃ��b�V����`�ŏ����W
	 * @param  meshMax [in] �����Ώۃ��b�V����`�ő���W
	 * @param  isCnv [out] �ϊ��Ώۂ��ǂ���
	 * @retval true ����
	 * @retval false ���s
	 */
	bool chkPointCnv(LPCTSTR tableName, long id, const IGeometryPtr& ipPtGeometry, const LONLAT& meshMin, const LONLAT& meshMax, bool* isCnv);

private:
	const FieldIndexMap* m_bldNamePtIndeces;	//!< �����r�����̃|�C���g�E�t�B�[���h�C���f�b�N�X�Ή��\
};
