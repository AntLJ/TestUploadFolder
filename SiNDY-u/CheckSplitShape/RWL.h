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

#include "LayerBase.h"

/**
 * @brief RWL�̃O���[�v�����Ǘ����邽�߂̍\����
 */
struct RWLGROUP {
	long eGroupID;                   //!< �O���[�vID
	std::vector<long> eVecDivID;     //!< �O���[�v�ɏ�������DIVID�Q�i�i�[����DIVID���j
	std::vector<LONLAT> eVecLonLat;  //!< DIVID�Q�ŃO���[�v�����ꂽ�`��
};

/**
 * @brief RWL�̃��R�[�h�P�ʂ̓����N���X
 */
class CRWLDiv : public CRecodeBase {
public:
	CRWLDiv(){}
	~CRWLDiv(){}
public:
	virtual bool SetData(const CString& strTableName, IFeaturePtr ipFeature, const std::map<long,CString>& mapFieldIndex2Name, const std::map<CString,long>& mapFieldName2Index, const std::map<CString,long>& mapAttrName2Index);

public:
	UNIQID eFromNodeID;
	UNIQID eToNodeID;
};


/**
 * @brief RWL�f�[�^�����N���X
 */
class CRWLAll: public CTableBase {
public:
	CRWLAll(){}
	~CRWLAll(){}

public:
	/**
	 * @brief RWL�p�f�[�^�ǂݍ���
	 * @retval true ��������
	 * @retval true �������s
	 */
	virtual bool LoadData();

	/**
	 * @brief �O���[�v��
	 * @note �����O�f�[�^�p�̊֐�
	 * @retval true ��������
	 * @retval true �������s
	 */
	bool ConstractGroupBefore();

	/**
	 * @brief �O���[�v��
	 * @note ������f�[�^�p�̊֐�
	 * @param mapBeforeDivGroup  [in] �����O�O���[�v���
	 * @retval true ��������
	 * @retval true �������s
	 */
	bool ConstractGroupAfter(const std::map<long,std::vector<RWLGROUP>>& mapBeforeDivGroup);

private:
	/**
	 * @brief �O���[�v��������Ԃ̌`��擾
	 * @param lOID            [in] �擾�Ώۂ�OID
	 * @param vecDivID        [in] �擾�Ώۂ̃O���[�v��������ԁiDIVID�Q�j
	 * @param vecGroupLonLat  [out] �擾�`��
	 * @retval true ��������
	 * @retval true �������s
	 */
	bool ConstractGroupLonLat(long lOID, const std::vector<long>& vecDivID, std::vector<LONLAT>& vecGroupLonLat);

public:
	std::map<UNIQID,CRWLDiv> m_mapData;                   //!< DIVID�P�ʂ̃f�[�^���
	std::map<UNIQID,std::set<UNIQID>> m_mapNode2Link;     //!< �m�[�hID�ƃ����NID�̃}�b�v
	std::map<long,std::set<long>> m_mapDivOID;            //!< �I�u�W�F�N�gID�ɏ�������DIVID�̃O���[�v
	std::map<long,std::vector<RWLGROUP>> m_mapDivGroup;   //!< �O���[�v���iOID,�O���[�vID,�O���[�v���j

	std::map<std::set<LONLAT>,UNIQID> m_mapWalkHCSegment;     //!< �l�ԋ��p���s�҃����N�̃Z�O�����g���X�g�i���s�҃����N�ŕ��s�҃����N��ʂ��l�ԋ��p�̏ꍇ�j
	std::map<std::set<LONLAT>,UNIQID> m_mapRoadWalkHCSegment; //!< �l�ԋ��p�����N�̃Z�O�����g���X�g�i���s�҃����N�ŕ��s�҃����N��ʂ��l�ԋ��p�̏ꍇ�A�������́Awalkable_c���l�ԋ��p�̏ꍇ�j
};
