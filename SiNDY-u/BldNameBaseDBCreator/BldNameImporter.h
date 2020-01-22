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
#include "common.h"
#include "MatchingProcessor.h"
#include "SurveyPointManager.h"

class BldNameImporter
{
public:
	BldNameImporter(
		const std::vector<long>& srcPriListPrior,
		const std::vector<long>& srcPriListMatch,
		const std::vector<CString>& poiPriList,
		const std::vector<CString>& supPoiPriList,
		const std::vector<StrPair>& replaceList, 
		const std::set<long>& bldExcList,
		const std::map<long, long>& condList,
		const std::map<CString, long>& bldSrcIndeces,
		const std::map<CString, long>& bldAddInfoIndeces,
		const std::map<CString, long>& bldIndeces,
		const std::map<CString, long>& twnpageIndeces,
		const std::map<CString, long>& poiPtOrgIndeces,
		const std::map<CString, long>& bldNamePtIndeces,
		const std::map<CString, long>& relBldNameIndeces,
		const IFeatureClassPtr& ipBldSrcPt,
		const ITablePtr& ipBldAddInfo,
		const IFeatureClassPtr& ipBld,
		const IFeatureClassPtr& ipCityAdm,
		const IFeatureClassPtr& ipTwnpage,
		const IFeatureClassPtr& ipPoiPtOrg,
		const IFeatureClassPtr& ipBldNamePt,
		const ITablePtr& ipRelBldName,
		bool isUpdate,
		bool isSurvey,
		SurveyPointManager& surveyPtMgr);

	~BldNameImporter(void);

	/**
	 * @brief �������֐�
	 * @retval true ����
	 * @retval false ���s
	 */
	bool Init();

	/**
	 * @brief �C���|�[�g�֐�
	 * @param addrCode [in] �����Ώۍs���E�R�[�h
	 * @retval true ����
	 * @retval false ���s
	 */
	bool Import(const CString& addrCode);

private:
	/**
	 * @brief �s���E�t�B�[�`���擾�֐�
	 * @note �^����ꂽ�s���E(5��)�R�[�h��ێ�����s���E�̃}�[�W���擾����B
	 * @param addrCode [in] �s���E�R�[�h
	 * @param tgtAdmGeometry [out] 5���s���E
	 * @retval true ����
	 * @retval false ���s
	 */
	bool GetTgtCityAdms(const CString& addrCode, IGeometryPtr& tgtAdmGeometry);

	/**
	 * @brief �������擾�֐�
	 * @note �^����ꂽ�s���E��intersect���錚���t�B�[�`����overlap���錚��ID���擾����B
	 * @param admFeatre [in] �s���E�t�B�[�`��
	 * @param intersectBldFeatures [out] intersect���������t�B�[�`��
	 * @param overrapBldIDs [out] overrapBldIDs overlap��������ID���X�g
	 */
	bool GetTgtBldInfos(const IGeometryPtr& ipAdmGeometry, IDFeatureMap* intersectBldFeatures, std::set<long>* overlapBldIDs);

	/**
	 * @brief �����r�����̃|�C���g�E�����r�����̕R�Â����R�[�h�擾�֐�
	 * @param bldID [in] �����������OBJECTID
	 * @param ipBldFeature [in] �����`��
	 * @param overlapBldIDs [in] �s���E��overlap���Ă��錚��ID���X�g
	 * @retval true ����
	 * @retval false ���s
	 */
	bool GetImportRecordInfos(long bldID, const IFeaturePtr& ipBldFeature, const std::set<long>& overlapBldIDs);

	/**
	 * @brief DB�X�V�����֐�
	 * @param flushAll [in] �S�����f���[�h
	 * @retval true ����
	 * @retval false ���s
	 */
	bool DBUpdate(bool flushAll);

	/**
	 * @brief ���������r�����̃|�C���g���݃`�F�b�N
	 * @note  �o�͐挚���r�����̃|�C���g�e�[�u���ɁA�폜�Ώۃ|�C���g�����݂��Ȃ����`�F�b�N����B
	 *        �ˏ����̊�ƂȂ錚���ɕR�Â��A�����w�M���x�R�[�h��1(�����ɂ���Ēǉ����ꂽ����)�ȊO�̏ꍇ
	 * @param bldID					[in] �����������OBJECTID
	 * @param eixstEditPoint		[out] �M���x�R�[�h��1�̃|�C���g�����݂��邩�ǂ��� true:���݂��� false:���݂��Ȃ� 
	 */
	bool ChkExistBldNamePt(long bldID, bool* existEditPoint);

	/**
	 * @brief  �����ߍσ|���S���̎擾
	 * @note   �Ώۃ|���S����ExteriorRing����ł��邱�Ƃ�����
	 * @param  ipTgtFeature		[in]  �Ώۃ|���S���t�B�[�`��
	 * @param  ipFilledPolygon	[out] �����ߍσ|���S��
	 * @retval true ����
	 * @retval false ���s
	 */
	bool GetFilledPolygon(IFeaturePtr ipTgtFeature, IPolygonPtr& ipFilledPolygon);

	/**
     * @brief ������ɑ��݂���f�ރ|�C���g�擾�֐�
	 * @param ipFilledBldPolygon	[in] �����ߍό����`��
	 * @param tgtSrcPtFeatures		[out] ������ɑ��݂���f�ރ|�C���g�Q
	 * @retval true ����
	 * @retval false ���s 
	 */
	bool GetTgtSrcPt(IPolygonPtr ipFilledBldPolygon, IDFeatureMap* tgtSrcPtFeatures);

	/**
     * @brief ������ɑ��݂��钍�LPOI�|�C���g�擾�֐�
	 * @param ipFilledBldPolygon	[in] �����ߍό����`��
	 * @param tgtPoiPtFeatures		[out] ������ɑ��݂��钍�LPOI�|�C���g�Q
	 * @retval true ����
	 * @retval false ���s 
	 */
	bool GetTgtPoiPt(IPolygonPtr ipFilledBldPolygon, IDFeatureMap* tgtPoiPtFeatures);

	/**
	 *  @brief �����ߍό����`���ɑ��݂���t�B�[�`���擾�֐�
	 *	@param bldID				[in] �����������OBJECTID
	 *	@param ipInfillBldGeometry	[in] ����������`��(�����ߍ�)
	 *	@param ipFeatureClass		[in] �擾�Ώۃt�B�[�`���N���X
	 *	@param tableName			[in] �e�[�u����
	 *	@param tgtFeatures			[out] ������ɑ��݂���t�B�[�`���Q
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool GetTgtAtInfillGeo(long bldID, const IGeometryPtr& ipInfillBldGeometry, const IFeatureClassPtr& ipFeatureClass, LPCTSTR tableName, IDFeatureMap* tgtFeatures);

	/**
	 * @brief�@���������r�����̃|�C���g�폜�֐�
	 * @retval true ����
	 * @retval false ���s
	 */
	bool DeleteExistRecord();

	/**
	 * @brief �����r�����̃|�C���g�o�^�֐�
	 * @param bldNamePtInfo		[in] ���R�[�h�o�^���
	 * @param bldNameAddInfo	[in] �t����񃌃R�[�h���
	 * @param ipBldNameCursor	[in] �o�^�p�t�B�[�`���J�[�\��
	 * @param bldNamePtID		[out] �o�^�����ۂ�obejctid
	 * @retval true ����
	 * @retval false ���s
	 */
	bool InsertBldNamePt(const BldNamePtInfo& bldNamePtInfo, const BldNameAddInfo& bldNameAddInfo, IFeatureCursorPtr& ipBldNamePtCursor, long* bldNamePtID);

	/**
	 * @brief �����r�����̕R�Â����R�[�h�o�^�֐�
	 * @param bldNamePtID			[in] �R�Â������r�����̃|�C���gobjectid
	 * @param bldSrcPtID			[in] �R�Â��f��ID
	 * @param ipRelBldNameCursor	[in] �o�^�p�t�B�[�`���J�[�\��
	 * @retval ture  ����
	 * @retval false ���s
	 */
	bool InsertRelBldName(long bldNamePtID, long bldSrcPtID, _ICursorPtr& ipRelBldNameCursor);

	/**
	 * @brief �����r�����̕t����񃌃R�[�h�擾�֐�
	 * @param bldNamePtInfo		[in] �����r�����̃|�C���g�o�^���
	 * @param bldNameAddInfo	[out] �����r�����̕t����񃌃R�[�h���
	 * @retval ture  ����
	 * @retval false ���s
	 */
	bool GetAddInfo(const BldNamePtInfo& bldNamePtInfo, BldNameAddInfo& bldNameAddInfo);

private:
	std::set<long> m_bldExcList;	//!< �����ΏۊO������ʃ��X�g

	const std::map<CString, long>	m_bldIndeces;			//!< building�e�[�u���t�B�[���h�C���f�b�N�X���
	const std::map<CString, long>	m_bldSrcIndeces;		//!< buildingname_src_point�e�[�u���t�B�[���h�C���f�b�N�X���
	const std::map<CString, long>	m_bldAddInfoIndeces;	//!< buildingname_src_addinfo�e�[�u���t�B�[���h�C���f�b�N�X���
	const std::map<CString, long>	m_twnpageIndeces;		//!< townpage�e�[�u���C���f�b�N�X���
	const std::map<CString, long>	m_poiPtOrgIndeces;		//!< poi_point_org�e�[�u���C���f�b�N�X���
	const std::map<CString, long>	m_bldNamePtIndeces;		//!< buildingname_point�e�[�u���t�B�[���h�C���f�b�N�X���
	const std::map<CString, long>	m_relBldNameIndeces;	//!< rel_buildingname�e�[�u���t�B�[���h�C���f�b�N�X���

	const IFeatureClassPtr	m_ipBldSrcPt;		//!< buildingname_src_point�t�B�[�`���N���X
	const ITablePtr			m_ipBldAddInfo;		//!< buildingname_src_addinfo�e�[�u��
	const IFeatureClassPtr	m_ipBld;			//!< building�t�B�[�`���N���X
	const IFeatureClassPtr	m_ipCityAdm;		//!< city_admin�t�B�[�`���N���X
	const IFeatureClassPtr	m_ipTwnpage;		//!< townpage�t�B�[�`���N���X
	const IFeatureClassPtr	m_ipPoiPtOrg;		//!< poi_point_org�t�B�[�`���N���X
	IFeatureClassPtr		m_ipBldNamePt;		//!< buildingname_point�t�B�[�`���N���X
	ITablePtr				m_ipRelBldName;		//!< rel_buildingname�e�[�u��

	std::set<long>		m_cnvdOverlapBldIDs;	//!< �ϊ��ό���ID�i�[�R���e�i(�s���E�|���S����overlap������̂̂ݑΏ�)

	std::set<long>		m_srcTypeCodedValues;	//!< �o�T��ʃR�[�h�l���X�g

	std::map<CString, long>	m_mapAddrCodeCounter;	//!< �u�h�ΑΏە����v�|�C���g�E20���Z���R�[�h�J�E���g���X�g

	MatchingProcessor*	m_matchingProcessor;	//!< �}�b�`���O�����N���X

	const bool	m_isUpdate;		//!< DB�X�V���[�h
	const bool	m_isSuvey;		//!< �����Ώۃ|�C���g���X�g�쐬���[�h

	SurveyPointManager&	m_surveyPtMgr;	//!< �����Ώۃ|�C���g���X�g�Ǘ��I�u�W�F�N�g

	std::map<long, BldNamePtInfo>		m_bldNamePtInfos;		//!< �����r�����̃|�C���g�o�^���X�g
	std::map<long, std::set<long>>		m_relBldNameInfos;		//!< �����r�����̕R�t�����R�[�h�o�^���X�g
	std::set<long>						m_deleteBldNamePtIDs;	//!< �����r�����̃|�C���g�폜�Ώ�ID���X�g
};
