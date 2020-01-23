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

#include <SiNDYLib/FieldMap.h>

#include "Arguments.h"
#include "FeatureReader.h"
#include "ChildVerEditWorkspace.h"

/**
 * @brief   ���C���N���X
 */
class CCreateRoadLinkage
{
public:
	/**
	 * @brief  ���H�����N�|�Ԑ������N�֘A�e�[�u���̏��
	 * @note   �e�[�u���ւ̃��R�[�h�ǉ��p
	 */
	struct InsertRecord{
		long		 roadOid;	// ���H�����N��OID
		CString      laneGid;	// �Ԑ������NGID
		int			 roadDC;	// ���H�����N�����R�[�h

		InsertRecord() : roadOid(0), roadDC(-1){}
		InsertRecord(long road_oid, CString lane_gid, int road_dc )
			: roadOid(road_oid), laneGid(lane_gid), roadDC(road_dc){ }
	};

	/**
	 * @brief  ���H�����N�|�Ԑ������N�֘A�e�[�u������폜����t�B�[�`���[���
	 * @note   �G���[���O�֏o�͂���ۂɕK�v�ɂȂ�OID,X,Y,�⑫���b�Z�[�W������
	 */
	struct Info4ErrLog{
		long		 laneOid;	//!< �Ԑ������N��OID
		double		 x;			//!< �o�x
		double		 y;			//!< �ܓx
		CString		 msg;		//!< �⑫���b�Z�[�W

		Info4ErrLog() : laneOid(-1), x(0.0), y(0.0){}
		Info4ErrLog(long lOid, double dX, double dY, CString strMsg )
			: laneOid(lOid), x(dX), y(dY), msg(strMsg){}
	};

public:
	/**
	 * @brief   �R���X�g���N�^
	 * @param   args [in]  ����
	 */
	CCreateRoadLinkage( const Arguments& args ) : m_Args( args ){}

	/**
	 * @brief   �f�X�g���N�^
	 */
	virtual ~CCreateRoadLinkage(){};

	/**
	 * @brief   ���s
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool execute();

private:
	/**
	 * @brief   ���O������
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool initLog() const;

	/**
	 * @brief   �e�[�u���̃I�[�v��
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool openTable();

	/**
	 * @brief   �ҏW�J�n
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool startEdit();

	/**
	 * @brief   �ҏW�I��
	 */
	void stopEdit();

	/**
	 * @brief   �ҏW���f
	 */
	void abortEdit();

	/**
	 * @brief   �����N�R�t���쐬����
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool createLinkage();

	/**
	 * @brief   ����FGDB���g��Ȃ����C������
	 * @param[in]	mesh				���b�V�����
	 * @param[out]	lanelinkFeatures	�R�t���Ώۂ̎Ԑ������N
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool run4NotUseDiff( const std::pair<long, IGeometryPtr>& mesh, std::vector<IFeaturePtr>& lanelinkFeatures ) const;

	/**
	 * @brief   ����FGDB���g�������C������
	 * @note	�ȉ����s��
				�E�ĕR�t���Ώۂ̑I��
				�E�������R�[�h��������̊֘A���폜���A�폜�����G���[���O�o��
	 * @param[in]	mesh				���b�V�����
	 * @param[out]	lanelinkFeatures	�R�t���Ώۂ̎Ԑ������N
	 * @param[out]	lane2Road			�폜�����֘A���R�[�h
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool run4UseDiff(
		const std::pair<long, IGeometryPtr>& mesh,
		std::vector<IFeaturePtr>& lanelinkFeatures,
		std::multimap<CString, long>& lane2Road
	) const;

	/**
	 * @brief   ����FGDB���獷���f�[�^���擾����
	 * @note	�擾����f�[�^��1���b�V�����̃f�[�^�Ɍ��肷��
	 * @param[out]	diffMap		�����f�[�^�i�L�[�F�����^�C�v�A�l�F�t�B�[�`���j
	 * @param[in]	mesh		���b�V��
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool getDiff( std::map<long, std::vector<IFeaturePtr>>& diffMap, const IGeometryPtr& mesh ) const;

	/**
	 * @brief   �`�󍷕������H�����N�ł��邩
	 * @note	m_DiffFM��GLOBALID�̃C���f�b�N�X�������Ă��邩�ۂ��Ŕ��肵�Ă���
	 * @retval  true  ���H�����N
	 * @retval  false �Ԑ������N
	 */
	bool isRoadDiff() const;

	/**
	 * @brief   �Ԑ������N�̌`�󍷕��𕪐͂���
	 * @param[in]	diffMap				�����}�b�v
	 * @param[in]	baseLane			1���b�V�����̎Ԑ������N
	 * @param[out]	lanelinkFeatures	�R�t���Ώۂ̎Ԑ������N
	 * @param[out]	lane4DeleteRel		�Ԑ������N�Ə��i�֘A�폜�Ώہj
	 */
	void analyzeLaneDiff(
		const std::map<long, std::vector<IFeaturePtr>>& diffMap,
		const std::map<CString, IFeaturePtr>& baseLane,
		std::vector<IFeaturePtr>& lanelinkFeatures,
		std::map<CString, Info4ErrLog>& lane4DeleteRel
	) const;

	/**
	 * @brief   ���H�����N�̌`�󍷕��𕪐͂���
	 * @note	���͂�搂��Ă��邪�A��OID���擾���邾���ɐ��艺�����Ă���
				�ꉞ�����̎�ނ����Ă���̂ƁA�Ԑ������N���Ɠ��ꂷ��Ӑ}�����߂�analyze�Ƃ��Ă���
	 * @param[in]	diffMap				�����}�b�v
	 * @param[out]	orgOids				��OID
	 */
	void analyzeRoadDiff(
		const std::map<long, std::vector<IFeaturePtr>>& diffMap,
		std::vector<long>& orgOids
	) const;

	/**
	 * @brief   �֘A�e�[�u������֘A���폜����
	 * @param[in]	wheres			WHERE��iIN��1000�v�f��؂�j
	 * @param[out]	lane2Road		�폜�����֘A���R�[�h
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool deleteRel( const std::vector<CString>& wheres, std::multimap<CString, long>& lane2Road ) const;

	/**
	 * @brief   �R�t���Ώۂ̎Ԑ������N�ƁA�폜���ꂽ�Ԑ������N�̏����擾����
	 * @note	���H�����̒��ԏ����������Ɋ֐��ɂ܂Ƃ߂�����
	 * @param[in]	lane2Road			�폜�����֘A���R�[�h
	 * @param[in]	meshCode			���b�V���R�[�h
	 * @param[out]	lanelinkFeatures	�R�t���Ώۂ̎Ԑ������N
	 * @param[out]	lane4DeleteRel		�Ԑ������N�Ə��i�֘A�폜�Ώہj
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool getLinkInfo4CreateAndDeleted(
		const std::multimap<CString, long>& lane2Road,
		long meshCode,
		std::vector<IFeaturePtr>& lanelinkFeatures,
		std::map<CString, Info4ErrLog>& lane4DeleteRel
	) const;

	/**
	 * @brief	�폜�����֘A�ɂ��ăG���[���O�ɏo��
	 * @param[in]	lane4DeleteRel	�Ԑ������N�Ə��i�֘A�폜�Ώہj
	 * @param[in]	lane2Road		�폜�����֘A���R�[�h
	 * @param[in]	meshCode		���b�V���R�[�h
	 */
	void writeDeletedRelInfo( const std::map<CString, Info4ErrLog>& lane4DeleteRel, const std::multimap<CString, long>& lane2Road, long meshCode ) const;

	/**
	 * @brief   �֘A�e�[�u���ւ̏d���C���|�[�g�h�~
	 * @param[in,out]	lanelinkFeatures	�R�t���Ώۂ̎Ԑ������N
	 * @param[out]		existRel			�֘A�e�[�u���ɑ��݂���֘A�iKey�F�Ԑ������N�AValue�F���H�����N�j
	 */
	void preventDuplication( std::vector<IFeaturePtr>& lanelinkFeatures, std::multimap<CString, long>& existRel ) const;

	/**
	 * @brief   ���H�����N�|�Ԑ������N�֘A�e�[�u���ɕR�t�������݂��Ȃ��Ԑ������N�݂̂ɍi�荞��
	 * @param[in,out] lane_features  �Ԑ������N�Q
	 */
	void extractNotLinkedFeature( std::vector<IFeaturePtr>& lane_features ) const;

	/**
	 * @brief   �֘A�e�[�u���ɕR�t�������Ԑ������N�̊֘A���擾����
	 * @param[in]	lane_features	�Ԑ������N�Q
	 * @param[in]	existGids		�Ԑ������N��GID
	 * @param[in]	exsistRels		�Ԑ������N�Ɠ��H�����N�̑Ή��\
	 */
	void getExistRel( const std::vector<IFeaturePtr>& lane_features, std::set<CString>& existGids, std::multimap<CString, long>& exsistRel ) const;

	/**
	 * @brief  �����l�Q�ƌ����t�B�[���h���w�肵��IN������Where����擾����
	 * @param [in] vals        �����l�Q
	 * @param [in] field_name  �����t�B�[���h
	 * @return Where��Q (IN��̒��g��1000���𒴂���ꍇ�A������Where��ɕ����ĕԂ�)
	 */
	std::vector<CString> getWhereClauseINSearch( const std::set<CString>& vals, const CString& field_name ) const;
	std::vector<CString> getWhereClauseINSearch( const std::vector<long>& vals, const CString& field_name ) const;
	std::vector<CString> getWhereClauseINSearch( const std::list<CString>& vals, const CString& field_name ) const;

	/**
	 * @brief	�R�t���Ώۂ̎Ԑ������N���g���āA�֘A�����
	 * @param [in] lanelinkFeatures	�R�t���Ώۂ̎Ԑ������N
	 * @param [in] lane2Road		�폜�����֘A�iKey�F�Ԑ������N�AValue�F���H�����N�j
	 * @param [in] existRel			�֘A�e�[�u���ɑ��݂���֘A�iKey�F�Ԑ������N�AValue�F���H�����N�j
	 * @param [in] meshCode			���b�V���R�[�h
	 * @return Where��Q (IN��̒��g��1000���𒴂���ꍇ�A������Where��ɕ����ĕԂ�)
	 */
	bool createRel(
		const std::vector<IFeaturePtr>& lanelinkFeatures,
		const std::multimap<CString, long> lane2Road,
		const std::multimap<CString, long> existRel,
		long meshCode
	) const;

	/**
	 * @brief  �Ԑ������N(lane_link)�ɕR�Â����H�����N�𓹘H�����N�Q(road_links)����擾����
	 * @param[in]	lane_link		�R�t�����̎Ԑ������N
	 * @param[in]	road_links		�R�t���Ώۂ̓��H�����N�Q
	 * @param[in]	lane2Road		�폜�����֘A�iKey�F�Ԑ������N�AValue�F���H�����N�j
	 * @param[in]	existRel		�֘A�e�[�u���ɑ��݂���֘A�iKey�F�Ԑ������N�AValue�F���H�����N�j
	 * @param[in]	bash_mesh		�x�[�X���b�V���ԍ�
	 * @param[out]	insert_records	�R�t����񃊃X�g
	 */
	void matchLaneLinkToRoadLinks(
		const IFeaturePtr& lane_link,
		const std::vector<IFeaturePtr> road_links,
		const std::multimap<CString, long>& lane2Road,
		const std::multimap<CString, long>& existRel,
		long base_mesh,
		std::vector<InsertRecord>& insert_records
	) const;

	/**
	 * @brief   ���H�����N�|�Ԑ������N�֘A�e�[�u���ւ̏o�͏���
	 * @param[in] insert_records  �o�͂�����
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool insertRecord(const std::vector<InsertRecord>& insert_records ) const;

	/**
	 * @brief	���b�V���|���S���擾
	 * @param[in]	meshList	 ���b�V�����X�g
	 * @param[out]	meshGeoMap	 ���b�V���W�I���g���}�b�v
	 * @retval	true  ����
	 * @retval	false ���s
	 */
	bool getMeshPolygon( const std::vector<long>& meshList, std::map<long, IGeometryPtr>& meshGeoMap ) const;

	/**
	 * @brief   ���݂̎��Ԃ𕶎���Ŏ擾����
	 * @return  ���݂̎��Ԃ����������� (YYYY/MM/DD HH:MM:SS)
	 */
	CString getCurrentTime() const;

	/**
	 * @brief   �㏈�� 
	 * @param [in] isSucceeded ����I���������ۂ�
	 */
	void finish(bool isSucceeded) const;

	/**
	 * @brief �q�o�[�W��������Ԃ�
	 * @return �q�o�[�W������
	 */
	CString getChildVersionName() const;

private:
	Arguments m_Args;      //!< ����

	ChildVerEditWorkspace m_Workspace;		//!< �ҏW�Z�b�V��������

	ITablePtr m_LaneLinkTable;				//!< �Ԑ������N�e�[�u��
	ITablePtr m_RoadLinkTable;				//!< ���H�����N�e�[�u��
	ITablePtr m_BaseMeshTable;				//!< �x�[�X���b�V���e�[�u��
	ITablePtr m_RelRoadLinkLaneLinkTable;	//!< ���H�����N�|�Ԑ������N�֘A�e�[�u��
	ITablePtr m_DiffTable;					//!< ���H�����Nor�Ԑ������N�e�[�u���i�`�󍷕��j

	sindy::CFieldMap m_LaneLinkFM;				//!< �Ԑ������N�e�[�u���̃t�B�[���h�}�b�v
	sindy::CFieldMap m_RelRoadLinkLaneLinkFM;	//!< ���H�����N�|�Ԑ������N�֘A�e�[�u���̃t�B�[���h�}�b�v
	sindy::CFieldMap m_BaseMeshFM;				//!< �x�[�X���b�V���e�[�u���̃t�B�[���h�}�b�v
	sindy::CFieldMap m_DiffFM;					//!< ���H�����Nor�Ԑ������N�e�[�u���i�`�󍷕��j�̃t�B�[���h�}�b�v

	ISpatialReferencePtr m_LaneSpRef;			//!< �Ԑ������N�e�[�u���̋�ԎQ��
	ISpatialReferencePtr m_DiffSpRef;			//!< ���H�����Nor�Ԑ������N�e�[�u���i�`�󍷕��j�̋�ԎQ��
};

