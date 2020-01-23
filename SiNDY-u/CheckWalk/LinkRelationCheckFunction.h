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
#include "util.h"
#include "OutputLog.h"

class LinkRelationCheckFunction
{
public:
	/**
	 * @brief �R���X�g���N�^
	 * @param out                   [in] ���O�o�͊Ǘ��N���X�̎Q��
	 * @param modufyFilter          [in] �ҏW�����t�B���^
	 * @param buddy_dist_threshold  [in] �����N�R�t�`�F�b�N�p����臒l
	 * @param buddy_angle_threshold [in] �����N�R�t�`�F�b�N�p�p�x臒l
	 */
	LinkRelationCheckFunction(
		COutPut& out
		, CComVariant modifyFilter
		, double buddy_dist_threshold
		, double buddy_angle_threshold
		):
		m_output(out)
		, m_modifyFilter(modifyFilter)
		, m_buddy_dist_threshold(buddy_dist_threshold)
		, m_buddy_angle_threshold(buddy_angle_threshold)
	{}

	/**
	 * @brief �����N�R�t�e�[�u���̑S�`�F�b�N
	 * @param meshcode        [in] �`�F�b�N���̓s�s�n�}���b�V���R�[�h
	 * @param ipMeshGeo       [in] �`�F�b�N���̓s�s�n�}���b�V���̌`��
	 * @param roadLink        [in] ����9���b�V���̓��H�����N�Q�N���X
	 * @param walkLink        [in] ����9���b�V���̕��s�҃����N�Q�N���X
	 * @param linkRelation    [in] �����N�R�t���e�[�u���̑S���R�[�h
	 * @param bLinkRelation   [in] link_relation���[�h�Ŏ��s���邩�ۂ�
	 */
	void checkLinkRelation(
		const CString& meshcode,
		const IGeometryPtr& ipMeshGeo,
		const FeaturesPack& roadLink,
		const FeaturesPack& walkLink,
		const MultiRowsPack& linkRelation,
		bool bLinkRelation
		);

	/** 
	 * @brief humancar,walkable���[�h�p�̃����N�R�t�e�[�u��
	 * @param meshcode        [in] �`�F�b�N���̓s�s�n�}���b�V���R�[�h
	 * @param ipMeshGeo       [in] �`�F�b�N���̓s�s�n�}���b�V���̌`��
	 * @param roadLink        [in] ����9���b�V���̓��H�����N�Q�N���X
	 * @param walkLink        [in] ����9���b�V���̕��s�҃����N�Q�N���X
	 * @param linkRelation    [in] �����N�R�t���e�[�u���̑S���R�[�h
	 */
	void checkHumanCarOrWalkable(
		const CString& meshcode,
		const IGeometryPtr& ipMeshGeo,
		const FeaturesPack& roadLink,
		const FeaturesPack& walkLink,
		const MultiRowsPack& linkRelationMap
		);

	// TODO: checkLinkRelation�Ƌ@�\���܂Ƃ߂���Ƃ���͂܂Ƃ߂�
	/**
	 * @brief release���[�h�p�̃����N�R�t�e�[�u���`�F�b�N
	 * @param ipMeshGeo       [in] �`�F�b�N���̓s�s�n�}���b�V���̌`��
	 * @param roadLink        [in] ����9���b�V���̓��H�����N�Q�N���X
	 * @param walkLink        [in] ����9���b�V���̕��s�҃����N�Q�N���X
	 * @param linkRelation    [in] �����N�R�t���e�[�u���̑S���R�[�h
	 */
	void checkRelease(
		const IGeometryPtr& ipMeshGeo,
		const FeaturesPack& roadLink,
		const FeaturesPack& walkLink,
		const MultiRowsPack& linkRelationMap
		);

private:

	/**
	 * @brief �����N���m�̋��������ꂷ���Ă��Ȃ����`�F�b�N
	 * @note �G���[�R�[�h:1206
	 * @param walkLinkObj   [in] �`�F�b�N�Ώۂ̕��s�҃����N�̃I�u�W�F�N�g
	 * @param roadLinkID    [in] �`�F�b�N�Ώۂ̕��s�҃����N�ɕR�t�����H�����N��OID
	 * @param roadLinkObj   [in] �`�F�b�N�Ώۂ̓��H�����N�̃I�u�W�F�N�g
	 * @param objectId      [in] �`�F�b�N���̃����N�R�t���e�[�u���̃��R�[�h�̏��
	 */
	void checkLinkDistance(
		const GeoClass& walkLinkObj,
		const long roadLinkID,
		const GeoClass& roadLinkObj,
		const long objectId);

	/**
	 * @brief �����N���m�̊p�x���傫�����Ȃ����`�F�b�N
	 * @note �G���[�R�[�h:1207
	 * @param walkLinkObj   [in] �`�F�b�N�Ώۂ̕��s�҃����N�̃I�u�W�F�N�g
	 * @param roadLinkID    [in] �`�F�b�N�Ώۂ̕��s�҃����N�ɕR�t�����H�����N��OID
	 * @param roadLinkObj   [in] �`�F�b�N�Ώۂ̓��H�����N�̃I�u�W�F�N�g
	 * @param objectId      [in] �`�F�b�N���̃����N�R�t���e�[�u���̃��R�[�h�̏��
	 */
	void checkLinkAngle(
		const GeoClass& walkLinkObj,
		const long roadLinkID,
		const GeoClass& roadLinkObj,
		const long objectId );
	/**
	 * @brief	�����N�R�t�e�[�u���̊֘A�����N�Q�`�F�b�N
	 * @param	meshcode	[in]	���b�V���R�[�h
	 * @param	baseLinkObj	[in]	������N
	 * @param   fieldMap    [in]    ���H�����N�t�B�[���h�}�b�v
	 * @param	relLinks	[in]	�֘A�����N�̃��X�g
	 * @param	maxDistance	[in/out]	������N�A�֘A�����N�Ԃ̍ő勗��
	 * @param   bRelease    [in] release���[�h�Ŏ��s����Ă��邩�ۂ�
	 */
	void checkLinkRelationGroup(
		const GeoClass& baseLinkObj,
		const sindy::CFieldMap& fieldMap,
		const std::map<long, IGeometryPtr>& relLinks,
		double& maxDistance,
		bool bRelease
		);


	/**
	 * @brief	�w�肳�ꂽ2��GeoClass���ڐG���Ă��Ȃ���
	 * @param	geo1	[in]	GeoClass1
	 * @param	geo2	[in]	GeoClass2
	 * @param	distance	[in]	GeoClass1�̃o�b�t�@����(m)
	 * @retval	true	�ڐG���Ă��Ȃ�
	 * @retval	false	�ڐG���Ă���
	 */
	bool IsDisjoint(
		const GeoClass& geo1,
		const GeoClass& geo2,
		double distance
		);

	/**
	 * @brief	�w�肳�ꂽmap��key���J���}��؂�Ō�������B
	 * @param	target	[in]	�����Ώۂ�map
	 * @retval	�J���}��؂��key�̕�����
	 */
	CString JoinKeyValue(
		const std::map<long, IGeometryPtr>& target
		);

	/**
	 * @brief	�w�肳�ꂽ�����N�̃��X�g���Ȃ����Ă��邩
	 * @param	links	[in]	�����N�̃��X�g
	 * @retval	true	�Ȃ����Ă���
	 * @retval	false	�Ȃ����Ă��Ȃ�
	 */
	bool ConnectLinks(
		const std::map<long, IGeometryPtr>& links
		);

	/**
	 * @brief	�w�肳�ꂽPolyline��FROM/TO�݂̂�Line�Ƃ݂Ȃ��A2��Polyline�̊p�x�������߂�
	 * @param	geo1	[in]	Polyline��GeoClass1
	 * @param	geo2	[in]	Polyline��GeoClass2
	 * @param	angle	[out]	�p�x��
	 * @retval	true	����
	 * @retval	false	���s
	 */
	bool ContainAngluarDiff(
		const GeoClass& geo1,
		const GeoClass& geo2,
		double angle
		);

	/**
	 * @brief	�w�肳�ꂽPolyline��FROM/TO�݂̂�Line�Ƃ݂Ȃ��p�x�����߂�
	 * @param	ipGeometry	[in]	Polyline
	 * @retval	�p�x(-180���`180��)
	 */
	double GetAngle(
		const IGeometryPtr& ipGeometry
		);

	/**
	* @brief	buddy�������s�֎~�̓��H�����N�̃G���[�o��
	* @param	walkableErrRoadLinks	[in]	�o�͑Ώۂ̓��H�����NID�Z�b�g
	* @param    roadLink    [in]    ���H�����N�Q�N���X
	* @param	meshcode	[in]	���b�V���R�[�h
	*/
	void outputWalkableErr(
		const std::set<long>& walkableErrRoadLinks,
		const FeaturesPack& roadLink,
		const CString& meshcode );

protected:
	COutPut& m_output; //!< ���O�o�͗p�C���X�^���X�ւ̎Q��
	CComVariant m_modifyFilter; //!< �ҏW�����t�B���^
	double m_buddy_dist_threshold; //!< �����N�R�t�`�F�b�N�p����臒l
	double m_buddy_angle_threshold; //!< �����N�R�t�`�F�b�N�p�p�x臒l

};
