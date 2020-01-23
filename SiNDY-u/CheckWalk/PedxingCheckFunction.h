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
#include "DatabaseControl.h"
#include "Properties.h"

class CCheckWalk;
class PedxingCheckFunction
{
public:
	/**
	 * @brief �R���X�g���N�^
	 * @param [in] out                       ���O�o�͊Ǘ��N���X�̎Q��
	 * @param [in] modufyFilter              �ҏW�����t�B���^
	 * @param [in] dataType                  �`�F�b�N�Ώۂ̃f�[�^�^�C�v
	 * @param [in] sindyTolerance_threshold  SiNDY���e�l(m)
	 */
	PedxingCheckFunction(
		COutPut& out
		, CComVariant modifyFilter
		, sindy::schema::walk_type::type_class::ECode dataType
		, double sindyTolerance_threshold):
		m_output( out )
		, m_modifyFilter(modifyFilter)
		, m_dataType(dataType)
		, m_sindyTolerance_threshold(sindyTolerance_threshold)
	{}

	/**
	 * @brief ���s�҃����N�̑S�Ẵ`�F�b�N�����s
	 * @param [in] meshcode            �������̓s�s�n�}���b�V���R�[�h
	 * @param [in] ipMeshGeo           �s�s�n�}���b�V���`��
	 * @param [in] pedxing             ���f�|�C���g�Q�N���X
	 * @param [in] walkLink            ���s�҃����N�Q�N���X
	 * @param [in] walkNode            ���s�҃m�[�h�Q�N���X
	 * @param [in] roadLink            ���H�����N�Q�N���X
	 * @param [in] roadNode            ���H�m�[�h�Q�N���X
	 * @param [in] scrambleArea        �X�N�����u���G���A�Q�N���X
	 * @param [in] heightNode          �d�Ȃ胊���N�̍����m�[�h�Q�N���X
	 * @param [in] pedxingRTree        ���f�|�C���gRTree
	 * @param [in] walkLinkRTree       ���s�҃����NRTree
	 * @param [in] walkNodeRTree       ���s�҃m�[�hRTree
	 * @param [in] roadLinkRTree       ���H�����NRTree
	 * @param [in] roadNodeRTree       ���H�m�[�hRTree	 
	 * @param [in] scrambleAreaRTree   �X�N�����u���G���ARTree
	 * @param [in] heightNodeRTree     �d�Ȃ胊���N�̍����m�[�h��RTree
	 * @param [in] checktarget         �`�F�b�N�Ώ�
	 */
	void check( 
		const CString& meshcode,
		const IGeometryPtr& ipMeshGeo,
		const FeaturesPack& pedxing,
		const FeaturesPack& walkLink,
		const FeaturesPack& walkNode,
		const FeaturesPack& roadLink,
		const FeaturesPack& roadNode,
		const FeaturesPack& scrambleArea,
		const FeaturesPack& heightNode,
		const BGRTree& pedxingRTree,
		const BGRTree& walkLinkRTree,
		const BGRTree& walkNodeRTree,
		const BGRTree& roadLinkRTree,
		const BGRTree& roadNodeRTree,		
		const BGRTree& scrambleAreaRTree,
		const BGRTree& heightNodeRTree,
		properties::check_target checktarget
		) const ;	

	/**
	 * @brief ��O���[�h�p�̃`�F�b�N
	 * @param [in] pedxing             ���f�|�C���g�Q�N���X
	 */
	void checkException(
		const FeaturesPack& pedxing
		);

protected:
		
	/**
	 * @brief ���f�|�C���g�`�󂪕s���ł��邩�`�F�b�N
	 * @note �G���[�R�[�h:138
	 * @param [in] ipTable  ���f�|�C���g�e�[�u��
	 */
	void checkPedxingGeometry( const ITablePtr& ipTable ) const;

	/**
	 * @brief ���f�|�C���g���X�N�����u���G���A���ɂ��邩�`�F�b�N
	 * @param [in] pedxingObject      �`�F�b�N�Ώۂ� ���f�|�C���g
	 * @param [in] scrambleArea       �X�N�����u���G���A�Q�N���X
	 * @param [in] scrambleAreaRTree  �X�N�����u���G���ARTree
	 */
	void checkPedxingOnScrambleArea(
		const GeoClass& pedxingObject,
		const FeaturesPack& scrambleArea,
		const BGRTree& scrambleAreaRTree	
	) const;

	/**
	 * @brief ���f�|�C���g�����H�����N��ɖ������`�F�b�N
	 * @param [in] pedxingObject      �`�F�b�N�Ώۂ� ���f�|�C���g
	 * @param [in] roadLink           ���H�����N�Q�N���X
	 * @param [in] roadLinkRTree      ���H�����NRTree
	 */
	void checkPedxingOnRoadLink(
		const GeoClass& pedxingObject,
		const FeaturesPack& roadLink,
		const BGRTree& roadLinkRTree	
		) const;

	/**
	 * @brief	�����ӏ��̃|�C���g���擾����B
	 * @param	[in] baseGeoClass	���GeoClass
	 * @param   [in] crossObjects   �����Ȃ�|���S���̃��X�g
	 * @retval true  �����ӏ��̋�����SiNDY���e�l��菬�����̃|�C���g�����݂���
	 * @retval false �����ӏ��̋�����SiNDY���e�l��菬�����̃|�C���g�͑��݂��Ȃ�
	 */
	bool getCrossPoint(
		const GeoClass& baseGeoClass,
		const std::vector<GeoClass>& crossObjects
		) const;

	/**
	 * @brief ���f�|�C���g�����H�����N�����ӏ��ɂ��邩�`�F�b�N
	 * @param [in] pedxingObject      �`�F�b�N�Ώۂ� ���f�|�C���g
	 * @param [in] roadLink           ���H�����N�Q�N���X
	 * @param [in] roadLinkRTree      ���H�����NRTree
	 * @param [in] roadNodeMap        ���H�m�[�h�Q���
	 * @param [in] roadNodeRTree      ���H�m�[�hRTree
	 */
	void checkPedxingCrossRoadLink(
		const GeoClass& pedxingObject,
		const FeaturesPack& roadLink,
		const BGRTree& roadLinkRTree,
		const std::map<long, GeoClass>& roadNodeMap,
		const BGRTree& roadNodeRTree
		) const;

	/**
	* @brief ���f�|�C���g�ɏd�Ȃ�m�[�h������
	* @param pedxingObject [in] �`�F�b�N�Ώۂ̉��f�|�C���g�̏��
	* @param nodeMap       [in] �m�[�h�̏��
	* @param nodeRTree     [in] �m�[�h��RTree
	* @retval true  ���s�҃m�[�h�ɏd�Ȃ�m�[�h�����݂���
	* @retval false ���s�҃m�[�h�ɏd�Ȃ�m�[�h�͑��݂��Ȃ�
	*/
	bool findOverlapNode(
		const GeoClass& pedxingObject,
		const std::map<long, GeoClass>& nodeMap,
		const BGRTree& nodeRTree
		) const;

	/**
	 * @brief ���f�|�C���g�����s�҃����N�����ӏ��ɂ��邩�`�F�b�N
	 * @param [in] pedxingObject      �`�F�b�N�Ώۂ� ���f�|�C���g
	 * @param [in] walkLink           ���s�҃����N�Q�N���X
	 * @param [in] walkLinkRTree      ���s�҃����NRTree
	 * @param [in] walkNodeMap        ���s�҃m�[�h�Q���
	 * @param [in] walkNodeRTree      ���s�҃m�[�hRTree
	 */
	void checkPedxingCrossWalkLink(
		const GeoClass& pedxingObject,
		const FeaturesPack& walkLink,
		const BGRTree& walkLinkRTree,
		const std::map<long, GeoClass>& walkNodeMap,
		const BGRTree& walkNodeRTree
		) const;

	/**
	 * @brief ���f�|�C���g�Ɋ֌W�Ȃ����s�҃����N�����݂��邩�`�F�b�N
	 * @param [in] pedxingObject      �`�F�b�N�Ώۂ� ���f�|�C���g
	 * @param [in] walkLink           ���s�҃����N�Q�N���X
	 * @param [in] walkLinkRTree      ���s�҃����NRTree
	 * @param [in] pedxingClass       ���s�҃m�[�h���
	 */
	void checkPedxingWithWalkLink(
		const GeoClass& pedxingObject,
		const FeaturesPack& walkLink,
		const BGRTree& walkLinkRTree,
		long  pedxingClass
		) const;

	/**
	 * @brief ���f�^�C�v�R�[�h���s���Ȓl���`�F�b�N
	 * @param [in] pedxingClass       ���s�҃m�[�h���
	 */
	void checkPedxingClass(
		long  pedxingClass
		) const;

	/**
	 * @brief �|�C���g���m�iPEDXING�AHEIGHT_NODE�j�̋������߂����`�F�b�N
	 * @param [in] pedxingObject      �`�F�b�N�Ώۂ� ���f�|�C���g
	 * @param [in] pedxing            ���f�|�C���g�Q�N���X
	 * @param [in] heightNode         �d�Ȃ胊���N�̍����m�[�h�Q�N���X
	 * @param [in] pedxingRTree       ���f�|�C���gRTree
	 * @param [in] heightNodeRTree    �d�Ȃ胊���N�̍����m�[�h��RTree
	 */
	void checkNearPoint(
		const GeoClass& pedxingObject,
		const FeaturesPack& pedxing,	
		const FeaturesPack& heightNode,
		const BGRTree& pedxingRTree,
		const BGRTree& heightNodeRTree
		) const;

	/**
	 * @brief �Ӗ��Ȃ�PEDXING���`�F�b�N
	 * @param [in] pedxingObject      �`�F�b�N�Ώۂ� ���f�|�C���g
	 * @param [in] roadLink           ���H�����N�Q�N���X
	 * @param [in] walkLink           ���s�҃����N�Q�N���X
	 * @param [in] roadLinkRTree      ���H�����NRTree
	 * @param [in] walkLinkRTree      ���s�҃����NRTree
	 */
	void checkCrossRoadLink(
		const GeoClass& pedxingObject,
		const FeaturesPack& roadLink,
		const FeaturesPack& walkLink,
		const BGRTree& roadLinkRTree,
		const BGRTree& walkLinkRTree
		) const;

protected:
	COutPut& m_output;                                                   //!< ���O�o�͗p�C���X�^���X�ւ̎Q��
	CComVariant m_modifyFilter;                                          //!< �ҏW�����t�B���^
	sindy::schema::walk_type::type_class::ECode m_dataType;              //!< �f�[�^�^�C�v
	double	m_sindyTolerance_threshold;                                  //!< SiNDY���e�l(m)
};
