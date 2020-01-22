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
#include "LogSys.h"
#include "SurveyPointManager.h"

// �v�f ���̂̒���&objectid&����(���̂̒������Z�����Ɋi�[�E���̂̒����������̏ꍇ��objectid��)
/**
 * @brief POI��񃌃R�[�h
 */
struct POIINFO
{
	long e_nameLength;		//!< ���̂̒���
	long e_objectid;		//!< objectid
	CString e_name;			//!< ����
	CString e_srcID;		//!< �o�T����ID
	CAdapt<IGeometryPtr> e_ipGeometry; //!< �`��

	bool operator == (const POIINFO& poiInfo) const {
		return (e_nameLength == poiInfo.e_nameLength && e_objectid == poiInfo.e_objectid && e_name == poiInfo.e_name);
	}

	bool operator < (const POIINFO& poiInfo) const {
		if (e_nameLength != poiInfo.e_nameLength) return (e_nameLength < poiInfo.e_nameLength);
		return (e_objectid < poiInfo.e_objectid);
	}
};

/**
 * @brief �}�b�`���O��񃌃R�[�h
 */
struct MATCHINGINFO
{
	long	e_perfectMatchCount;			//!< ���S��v��
	long	e_replacePerfectMatchCount;		//!< ���H�㊮�S��v��
	double	e_maxMatchPer;					//!< �ő�F�}�b�`���O��
	long	e_srcType;						//!< �o�T�R�[�h
	CString	e_srcName;						//!< �o�T��
	CString	e_srcID;						//!< �o�T����ID
	CString	e_anyplaceID;					//!< AnyPlaceID
	std::map<long, double> e_allMatchPer;	//!< �ʑf�ނƂ̃}�b�`���O�����X�g�i�L�[�F��r��f�ނ�objectid �v�f�F�}�b�`���O���j
	CString	e_name;							//!< ����

	MATCHINGINFO() : e_perfectMatchCount(0), e_replacePerfectMatchCount(0), e_maxMatchPer(0.0), e_srcType(0) {}
};

/**
 * @brief �D��x����L�[�i�D�旘�p��ʗp�j
 */
struct SrcPriorKey
{
	long	e_srcType;	//!< �o�T�R�[�h
	long	e_adptCode;	//!< �̗p�R�[�h
	long	e_nameLen;	//!< ���̒���
	long	e_objectID;	//!< �I�u�W�F�N�gID

	SrcPriorKey(const std::vector<long>& srcPriListPrior) :
		m_srcPriListPrior(srcPriListPrior) {}
	SrcPriorKey(const std::vector<long>& srcPriListPrior, long strType, long adptCode, long nameLen, long objectID) :
		e_srcType(strType), e_adptCode(adptCode), e_nameLen(nameLen), e_objectID(objectID), m_srcPriListPrior(srcPriListPrior) {}
	~SrcPriorKey() {}

	bool operator < (const SrcPriorKey& rec) const
	{
		// �܂��A���ꂼ��̗D�揇�ʂ��擾����r
		int idx1 = std::distance(m_srcPriListPrior.begin(), std::find(m_srcPriListPrior.begin(), m_srcPriListPrior.end(), e_srcType));
		int idx2 = std::distance(m_srcPriListPrior.begin(), std::find(m_srcPriListPrior.begin(), m_srcPriListPrior.end(), rec.e_srcType));
		if (idx1 != idx2) return (idx1 < idx2);
		// �Ǝ������̏ꍇ�͍̗p�R�[�h���r
		if (sindy::schema::sj::buildingname_src_point::source_type::kOwnSurvey == e_srcType && e_adptCode != rec.e_adptCode) return (e_adptCode < rec.e_adptCode);
		if (e_nameLen != rec.e_nameLen) return (e_nameLen < rec.e_nameLen);
		return (e_objectID < rec.e_objectID);
	}

private:
	const std::vector<long>& m_srcPriListPrior;	//!< �f�ޗD��x��ʃ��X�g�i�D�旘�p�j
};

/**
 * @brief �D��x����}�b�v�i�D�旘�p��ʗp�j
 */
typedef std::map<SrcPriorKey, CAdapt<IFeaturePtr>> PriorFeatureMap;

/**
 * @brief �}�b�`���O�����N���X
 */
class MatchingProcessor
{
public:
	MatchingProcessor(
		const std::vector<long>& srcPriListPrior,
		const std::vector<long>& srcPriListMatch,
		const std::vector<CString>& poiPriList,
		const std::vector<CString>& supPoiPriList,
		const std::vector<StrPair>& replaceList,
		const std::map<long, long>& condList,
		const std::map<CString, long>& bldSrcIndeces,
		const std::map<CString, long>& poiPtOrgIndeces,
		const std::map<CString, long>& mapAddrCodeCounter,
		bool isSurvey,
		SurveyPointManager& surveyPtMgr
		);

	~MatchingProcessor(void);

	/**
	 * @brief �}�b�`���O�����֐�
	 * @param tgtSrcPtFeature		[in,out] �}�b�`���O�Ώۑf�ރ|�C���g�Q
	 * @param tgtPoiPtFeature		[in]  �}�b�`���O�Ώے��LPOI�|�C���g�Q
	 * @param bldID					[in]  �Ώی���ID
	 * @param ipFilledBldPolygon	[in]  �����ߍό����`��
	 * @param bldNamePtInfo			[out] �������̃|�C���g���R�[�h�o�^���
	 * @param relIDs				[out] rel_buildingname�ɓo�^�����\�_ID�Q
	 * @param result				[out] �}�b�`���O���� true:����(��\�_����) false:���s(��\�_�ƂȂ�|�C���g�����j
	 * @param excepted				[out] �u���O�����v�ɂ�鏜�O�������ǂ���
	 * @param selected				[out] �����Ώۃ|�C���g���ݒ�ς݂��ǂ���
	 */
	bool MatchingProc(
		IDFeatureMap& tgtSrcPtFeatures,
		const IDFeatureMap& tgtPoiPtFeatures,
		long bldID,
		IPolygonPtr ipFilledBldPolygon,
		BldNamePtInfo *bldNamePtInfo,
		std::set<long> *relIDs,
		bool *result,
		bool *excepted,
		bool *selected
		);

private:
	/**
	 * @brief  ����f�ރ|�C���g���݊m�F�֐�
	 * @note   �u���O�����v�u�Ǝ������v�̑f�ރ|�C���g�̃`�F�b�N
	 * @param  tgtSrcPtFeature	[in]  �}�b�`���O�Ώۑf�ރ|�C���g�Q
	 * @param  bldID			[in]  �Ώی���ID
	 * @param  bldNamePtInfo	[out] �������̃|�C���g���R�[�h�o�^���
	 * @param  result			[out] �}�b�`���O����
	 * @param  excepted			[out] �u���O�����v�ɂ�鏜�O�������ǂ���
	 * @param  selected			[out] �����Ώۃ|�C���g���ݒ�ς݂��ǂ����i�����Ώۃ|�C���g�����}���̂��߂Ɏg�p�j
	 * @retval true  ����
	 * @retval false ���s
	 */
	bool CheckSpecialSrcPt(const IDFeatureMap& tgtSrcPtFeatures, long bldID, BldNamePtInfo *bldNamePtInfo, bool *result, bool *excepted, bool *selected);

	/**
	 * @brief  �Ǝ������|�C���g�̍폜
	 * @note   �u�Ǝ������v�̑f�ރ|�C���g���}�b�`���O�Ώۑf�ރ|�C���g�Q����폜
	 * @param  tgtSrcPtFeature	[in,out]  �}�b�`���O�Ώۑf�ރ|�C���g�Q
	 */
	void DeleteOwnSurveyPts(IDFeatureMap& tgtSrcPtFeatures);

	/**
	 * @brief  ���LPOI�|�C���g�o�^���擾�֐�
	 * @note   ���LPOI�|�C���g�����݂��Ȃ��ꍇ��poiSrcID=-1�ƂȂ�B
	 *         �������݂���ꍇ�́A���LPOI�D��x��ʃ��X�g(m_poiPriList)�̒��ŗD��x��������ʂ�ێ����Ă�����̂Ƃ���B
	 *         �˓�����ʂ̃|�C���g���������݂����ꍇ�́A������̒Z������D��
	 *         �˕�����������̏ꍇ�́Aobjectid��
	 * @param  tgtSrcPtFeature	[in]  �}�b�`���O�Ώۑf�ރ|�C���g�Q
	 * @param  bldID			[in]  �Ώی���ID
	 * @param  bldNamePtInfo	[out] �������̃|�C���g���R�[�h�o�^��� 
	 * @param  existPOI			[out] ���LPOI�|�C���g�̑��ݗL�� true:���݂��� false:���݂��Ȃ�
	 * @retval true  ����
	 * @retval false ���s
	 */
	bool GetPoiSrcPtID(const IDFeatureMap& tgtSrcPtFeatures, long bldID, BldNamePtInfo* bldNamePtInfo, bool* existPOI);
	
	/**
	 * @brief  �P�ƃ|�C���g�}�b�`���O����
	 * @note   ������ɑf�ރ|�C���g������������ꍇ�̃}�b�`���O����
	 * @param  tgtSrcID				[in]  �f�ރ|�C���gID
	 * @param  ipTgtSrcFeature		[in]  �f�ރ|�C���g�t�B�[�`��
	 * @param  bldID				[in]  �Ώی���ID
	 * @param  ipFilledBldPolygon	[in]  �����ߍό����`��
	 * @param  isPart				[in]  ������ɓ���ʂ̑f�ނ����݂��A���̒��̈�ɑ΂���}�b�`���O������ true: ������ɓ���ʂ̑f�ނ����݂���B flase:������ɂ͖{�����ōs���Ă���f�ނ������݂��Ȃ��B
	 * @param  bldNamePtInfo		[out] �������̃|�C���g���R�[�h�o�^���
	 * @param  result				[out] �}�b�`���O���� true:����(��\�_����) false:���s(��\�_�ƂȂ�|�C���g�����j
	 * @retval true  ����
	 * @retval false ���s
	 */
	bool MatchingOnePoint(
		long tgtSrcID,
		const IFeaturePtr& ipTgtSrcFeature,
		long bldID, 
		IPolygonPtr ipFilledBldPolygon, 
		bool isPart,
		BldNamePtInfo* bldNamePtInfo, 
		bool* result
		);

	/**
	 *	@brief �����|�C���g�}�b�`���O����
	 *	@note  ��������g�p�����}�b�`���O
	 *	@param tgtSrcPtFeatures [in] �}�b�`���O�Ώۑf�ރ|�C���g�Q
	 *	@param bldNamePtInfo [in] �f�ރ|�C���gID
	 *	@param relIDs [in] �Ώی���ID
	 *	@param result [out] �}�b�`���O���� true:����(��\�_����) false:���s(��\�_�ƂȂ�|�C���g����)
	 *	@return true  :����
	 *	@return false :���s
	 */
	bool MatchingPoints(
		const IDFeatureMap& tgtSrcPtFeatures, 
		long bldID,
		BldNamePtInfo *bldNamePtInfo,
		bool *result
		);

	/**
	 *	@brief ����f�ރ|�C���g�}�b�`���O����
	 *	@param sameSrcBldNamePtInfo [in] ����f�ރr�����̏��
	 *	@return ��\�_ID
	 */
	long SelectSameSrcBldNamePtInfo(const std::map<long, BldNamePtInfo>& sameSrcBldNamePtInfo);

	/**
	 *	@brief �P�Ɩh�ΑΏە��̗p�`�F�b�N
	 *  @param ipTgtSrcFeature [in] �f�ރ|�C���g�t�B�[�`��
	 *  @param result [out] �̗p���� true:�̗p false:�s�̗p
	 *  @return true�@:����
	 *  @return false :���s
	 */
	bool AdoptFirePrevChk(const IFeaturePtr& ipTgtSrcFeature, bool* result);

	/**
	 *  @brief �s���Y���̗p�`�F�b�N
	 *  @param tgtSrcID [in] �f�ރ|�C���gID
	 *	@param ipTgtSrcFeature [in] �f�ރ|�C���g�t�B�[�`��
	 *	@param ipInfillBldGeometry [in] �����ߍό����`��
	 *  @param bldID [in] �Ώی���ID
	 *	@param result [out] �̗p���� true:�̗p false:�s�̗p
	 *  @return true  :����
	 *  @return false :���s
	 */
	bool AdoptImmovablesChk(long tgtSrcID, const IFeaturePtr& ipTgtSrcFeature, IPolygonPtr ipFilledBldPolygon, long bldID, bool *result);

	/**
	 *	@biref �e�f�ނƂ̃}�b�`���O�֐�
	 *  @param tgtSrcPtFeatures [in] �f�ރ|�C���g�t�B�[�`���Q
	 *	@param resultMatch [out] �}�b�`���O����
	 *	@param resultMatchSortPerfectMatch [out] ���S��v���Ń\�[�g���ꂽ�}�b�`���O����
	 *  @param resultMatchSortReplacePerfectMatch [out] ���H�㊮�S��v���Ń\�[�g���ꂽ�}�b�`���O����
	 *  @param resultMatchSortMaxMatchPer [out] �ő�}�b�`���O���Ń\�[�g���ꂽ�}�b�`���O����
	 */
	bool GetEachMatchingInfo(
		const IDFeatureMap& tgtSrcPtFeatures, 
		std::map<long, MATCHINGINFO>* resultMatch, 
		std::map<long, std::map<long, MATCHINGINFO*>>* resultMatchSortPerfectMatch,
		std::map<long, std::map<long, MATCHINGINFO*>>* resultMatchSortReplacePerfectMatch,
		std::map<double, std::map<long, MATCHINGINFO*>>* resultMatchSortMaxMatchPer
		);

	/**
	 *	@brief ������u���֐�
	 *	@param orgName [in]
	 *	@return �u���㕶����
	 */
	CString ReplaceStr(const CString& orgName);

	/**
	 *	@brief ������}�b�`���O�֐�
	 *	@param str1 [in] ������1
	 *	@param str2 [in] ������2
	 *  @return �}�b�`���O��
	 */
	double MatchingStr(const CString& str1, const CString& str2);

	/**
	 *	@brief ��\�_ID�E�M���x�擾�֐�
	 *	@param resultMatchingSortPerfectMatch [in] ���S��v���Ń\�[�g���ꂽ�}�b�`���O����
	 *	@param resultMatchingSortReplacePerfectMatch [in] ���H�㊮�S��v���Ń\�[�g���ꂽ�}�b�`���O����
	 *  @param resultMatchingSortMaxMatchPer [in] �}�b�`���O���Ń\�[�g���ꂽ�}�b�`���O����
	 *  @param headID [out] ��\�_ID
	 *  @param priority [out] �M���x�R�[�h
	 *	@return true ����
	 *	@return false ���s
	 */
	bool GetHeadPt(
		const std::map<long, std::map<long, MATCHINGINFO*>>& resultMatchingSortPerfectMatch,
		const std::map<long, std::map<long, MATCHINGINFO*>>& resultMatchingSortReplacePerfectMatch,
		const std::map<double, std::map<long, MATCHINGINFO*>>& resultMatchingSortMaxMatchPer,
		long *headID,
		long *priority
		);
	
	/**
	 *	@brief ���S��v�� or ���H�㊮�S��v����p������\�_ID�E�M���x�擾�֐�
	 *	@param maxPerfectMatchInfo [in] ���S��v�����ő�̃}�b�`���O����
	 *  @param maxReplacePerfectMatchInfo [in] ���H�㊮�S��v�H�����ő�̃}�b�`���O����
	 *  @param maxPerfectMatchCount ���S��v��
	 *  @param headID [out] ��\�_ID
	 *  @param priority [out] �M���x�R�[�h
	 *  @return true ����
	 *  @return false ���s
	 */
	bool GetHeadPtByPerfectMatch(
		const std::map<long, MATCHINGINFO*>& maxPerfectMatchInfo,
		const std::map<long, MATCHINGINFO*>& maxReplacePerfectMatchInfo,
		long maxPerfectMatchCount,
		long *headID,
		long *priority
		);

	/**
	 *	@brief �ő�}�b�`���O����p������\�_�E�M���x�擾�֐�
	 *  @param maxMatchPerInfo [in] �}�b�`���O���Ń\�[�g���ꂽ�}�b�`���O����
	 *  @param headID [out] ��\�_ID
	 *	@param priority [out] �M���x�R�[�h
	 *  @return true ����
	 *  @return false ���s
	 */
	bool GetHeadPtByMatchPer(const std::map<long, MATCHINGINFO*>& maxMatchPerInfo, long *headID, long *priority);

	/**
	 *	@brief �D��x�������}�b�`���O���擾�֐�
	 *  @param matchingInfos [in] �}�b�`���O���Q
	 *  @return �D��x���ő�̃}�b�`���O���Q
	 */
	std::map<long, MATCHINGINFO*> GetMatchInfoBySrcPri(const std::map<long, MATCHINGINFO*>& matchingInfos);

	/**
	 *  @brief �����񂪒Z���}�b�`���O���擾�֐�
	 *  @param matchingInfos [in] �}�b�`���O���Q
	 *  @return �����񂪈�ԒZ���}�b�`���O���Q
	 */
	std::map<long, MATCHINGINFO*> GetMatchInfoByStrLen(const std::map<long, MATCHINGINFO*>& matchingInfos);

	/**
	 *	@brief rel_buildingname�o�^ID�Z�b�g�֐�
	 *  @note headID(��\�_ID)����܂��Ă��邱�Ƃ�����
	 *  @param tgtSrcPtFeature [in] �}�b�`���O�Ώۑf�ރ|�C���g�Q
	 *	@param headID [in] ��\�_ID
	 *	@param relIDs [out] rel_buildingname�ɓo�^�����\�_ID�Q
	 */
	void SetRelIDs(const IDFeatureMap& tgtSrcPtFeatures, long headID, std::set<long>* relIDs);

	/**
	 *  @brief ��������ꕶ���Â�������֐�
	 *  @param str [in] ������
	 *	@return ������𕪉����Ċi�[�����R���e�i
	 */
	std::set<CString> Str2Pieces(const CString& str);

	/**
	 * @brief  �⊮�p���LPOI�|�C���g�ɂ��⊮�̎��{
	 * @param  tgtPoiPtFeatures	[in]  �}�b�`���O�Ώے��LPOI�|�C���g�Q
	 * @param  bldID			[in]  �Ώی���ID
	 * @param  bldNamePtInfo	[out] �������̃|�C���g���R�[�h�o�^���
	 * @param  result			[out] �}�b�`���O����
	 * @retval true  ����
	 * @retval false ���s
	 */
	bool SupplyWithPoiPtOrg(const IDFeatureMap& tgtPoiPtFeatures, long bldID, BldNamePtInfo *bldNamePtInfo, bool *result);

private:
	const std::vector<long>			m_srcPriListPrior;	//!< �f�ޗD��x��ʃ��X�g�i�D�旘�p�j
	const std::vector<long>			m_srcPriListMatch;	//!< �f�ޗD��x��ʃ��X�g�i�}�b�`���O�j
	const std::vector<CString>		m_poiPriList;		//!< ���LPOI�D��x���X�g
	const std::vector<CString>		m_supPoiPriList;	//!< �⊮�p���LPOI�D��x���X�g
	const std::vector<StrPair>		m_replaceList;		//!< �u�������񃊃X�g
	const std::map<long, long>		m_condList;			//!< �̗p�����L�q���X�g
	const std::map<CString, long>	m_bldSrcIndeces;	//!< �f�ރ|�C���g�̃t�B�[���h�C���f�b�N�X
	const std::map<CString, long>	m_poiPtOrgIndeces;	//!< ���LPOI�|�C���g�̃t�B�[���h�C���f�b�N�X

	const std::map<CString, long>&	m_mapAddrCodeCounter;	//!< �f�ރ|�C���g�E20���Z���R�[�h�J�E���g���X�g

	const bool	m_isSuvey;	//!< �����Ώۃ|�C���g���X�g�o�̓��[�h���ǂ���

	SurveyPointManager&	m_surveyPtMgr;	//!< �����Ώۃ|�C���g���X�g�Ǘ��I�u�W�F�N�g
};
