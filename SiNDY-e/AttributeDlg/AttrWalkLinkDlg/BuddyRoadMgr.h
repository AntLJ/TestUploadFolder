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


/**
* @brief �R�t�����H�����N�Ǘ��p�N���X�ibug 11283�j
*/
class CBuddyRoad : private boost::operators<CBuddyRoad>
{
	CBuddyRoad(){}
public:
	CBuddyRoad( const IFeaturePtr& roadLink, const _IRowPtr& linkRel );
	~CBuddyRoad();

	long m_objectID;		//!< LINK_RELATION�e�[�u�����̂̃I�u�W�F�N�gID
	long m_roadID;          //!< RoadLink��OID
	IGeometryPtr m_geom;    //!< RoadLink�̌`��
	IFeaturePtr m_roadLink; //!< RoadLink
	_IRowPtr m_linkRel;     //!< LINK_RELATION�̃��R�[�h
	bool m_addF;            //!< �ǉ��t���O
	bool m_deleteF;         //!< �폜�t���O


	/**
	* @brief �ύX�`�F�b�N
	* @note �ǉ��܂��͍폜�̂ǂ��炩��������������Ă�����ύX�L��ƌ��Ȃ�
	*       �����͋��i���ۂ͒ǉ����폜�����Ȃ��Ŗ����j
	* @return �l���ύX����Ă����true
	*/
	bool IsChanged() const { return (m_addF ^ m_deleteF); }

	/// LINK_RELATION�e�[�u�����̂̃I�u�W�F�N�gID�Ɠ��H�����NID�Ŕ���
	/// �I�u�W�F�N�gID�������ƁA�V�K�ǉ����ɓ����Ɣ��肳��A
	/// ���H�����NID�������ƁA�d�����ē����Ă���ꍇ�ɓ����Ɣ��肳��邽��
	bool operator==(const CBuddyRoad& other) const { 
		return m_objectID == other.m_objectID && m_roadID == other.m_roadID;
	}
};

typedef std::vector<CBuddyRoad> RoadList; //!< ���H�̃��X�g��`


/**
* @brief �R�t�����H�����N�Q�Ǘ��p�N���X
*/
class CBuddyRoadMgr
{
public:
	CBuddyRoadMgr();
	~CBuddyRoadMgr();
	
	/**
	* @brief ������
	*
	* @note walkLink�Ɠ������[�U��LINK_RELATION�e�[�u������A���ɕR�t���Ă��铹�H�����N��T���܂�
	* @param [in] walkLink �Ώۂ̕��s�҃����N
	* @param [in] ah ArcHelper
	*/
	void Init( std::list<CFeatureDef>* featureDefs, const CArcHelper& ah );
	
	/**
	* @brief ���H�R�t������
	*
	* LINK_RELATION�e�[�u������A���ɕR�t���Ă��铹�H�����N��T���A���X�g�{�b�N�X�ɕ\�����܂�
	*/
	void AddFromSearchedRoads( long walkID );

	/**
	* @brief �R�t���Ă��邩
	* @param [in] walkID ���s�҃����NID
	* @param [in] roadID ���H�����NID
	* @return walkID��roadID���R�t���Ă����true
	*/
	bool IsBuddy( long walkID, long roadID );

	/**
	* @brief �R�t�����H�����N�����݂��邩
	* @note �폜�t���O�̗����Ă��铹�H�����N�̓J�E���g�ΏۂɂȂ�܂���
	* @param [in] walkID �Ώۂ̕��s�҃����NID
	* @return �R�t�����H�����N���P�ł����݂����true
	*/
	bool HasBuddy( long walkID );
	
	/**
	* @brief �ύX�`�F�b�N
	* @param [in] walkID �Ώۂ̕��s�҃����NID
	* @return �R�t�����H�����N�Q���ύX����Ă����true
	*/
	bool IsChanged( long walkID );
	
	/**
	* @brief �_���`�F�b�N
	* @param [out] stringID �G���[���b�Z�[�W�̃��\�[�XID
	* @retval true �G���[�Ȃ�
	* @retval false �G���[����
	*/
	bool CheckLogic( UINT& stringID );
	
	/**
	* @brief LINK_RELATION�e�[�u�����X�V����
	* @note �ύX���̃��R�[�h�ǉ��A�폜���s���AStore�܂ōs���܂�
	* @return �ۑ��ɐ��������true
	*/
	bool UpdateLinkRelTable();

	/**
	* @brief buddy�ҏW���e�̔j��
	*
	* @note m_buddyRoads���N���A���܂��B
	*   buddyRoads�̕ύX��ۑ��E�j��������ɌĂ΂Ȃ��ƁA�O��̕ҏW���c���Ă��܂����Ƃ�����܂��B
	*/
	void ClearLinkMap();

	/**
	* @brief ���ӌ���
	* @note ���ӂ̓��H�����N���������A���ʂ͓����ł��ۑ����܂�
	* @param [in] walkLink �Ώۂ̕��s�҃����N
	* @param [in] dRange �����͈́i���[�g���j
	* @return �������ʂ̓��H�����N�Q
	*/
	RoadList& ArroundSearch( IFeaturePtr walkLink, double dRange);
	
	/**
	* @brief �R�t���Ă��铹�H�����N�擾
	* @note �폜�t���O�͍l������܂���
	* @param [in] walkID �Ώۂ̕��s�҃����NID
	* @return Buddy�֌W�ɂ��铹�H�����N�Q
	*/
	RoadList& GetBuddyRoads( long walkID );
	
	/**
	* @brief ���ӌ����ςݓ��H�����N�擾
	* @return �������ʂ̓��H�����N�Q
	*/
	RoadList& GetSearchedRoads(){ return m_searchedRoads; }

private:
	ITablePtr m_LinkRelT;                  //!< LINK_RALATION�e�[�u��
	IFeatureClassPtr m_roadLinkT;          //!< ���H�����N�e�[�u��
	std::map<long, RoadList> m_buddyRoads; //!< key:WalkLink��ID val:�R�t�����H�̃��X�g
	RoadList m_searchedRoads;              //!< ���ӌ������ꂽ���H�����N�Q
};

