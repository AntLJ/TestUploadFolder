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

#ifndef	_SINDY2TOLLGATELANE_H_
#define	_SINDY2TOLLGATELANE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SINDY2INHWY.h"


// tollgate_laneinfo.txt��1���R�[�h��\���\����
struct TollGateLaneInfo
{
	long		m_HwyNodeOID;		// �Ή�����n�C�E�F�C�m�[�h��ObjectID�i�f�o�b�O�p�̏��j
	long		m_MeshCode;			// 2�����b�V���R�[�h
	long		m_TollGateNodeID;	// �������m�[�hID
	long		m_ApproachNodeID;	// �����m�[�hID
	long		m_LaneNum;			// ���[����
	std::string	m_LaneStr;			// ���[�����
};
// tollgate_laneinfo.txt�̃��R�[�h�̃\�[�g��Ƃ���t�@���N�^
struct lessTollGateLaneInfo : public std::binary_function<TollGateLaneInfo,TollGateLaneInfo,bool>
{
	bool operator ()(const TollGateLaneInfo& lhs, const TollGateLaneInfo& rhs) const
	{
		return
			lhs.m_MeshCode != rhs.m_MeshCode				? lhs.m_MeshCode < rhs.m_MeshCode :
			lhs.m_TollGateNodeID != rhs.m_TollGateNodeID	? lhs.m_TollGateNodeID < rhs.m_TollGateNodeID :
			lhs.m_ApproachNodeID != rhs.m_ApproachNodeID	? lhs.m_ApproachNodeID < rhs.m_ApproachNodeID :
			lhs.m_LaneNum != rhs.m_LaneNum					? lhs.m_LaneNum < rhs.m_LaneNum :
															  lhs.m_LaneStr < rhs.m_LaneStr;
	}
};
// tollgate_laneinfo.txt�̃��R�[�h�Q���i�[����R���e�i
typedef std::set<TollGateLaneInfo, lessTollGateLaneInfo> TollGateLaneInfoArray;

// tollgate_laneinfo.txt���쐬���邽�߂�SiNDY����̃f�[�^�̎擾�A�f�[�^�̕ێ��A�t�@�C���ւ̏o�͂�S������N���X
class SINDY2TOLLGATELANE : public SINDY2INHWY
{
public:
	SINDY2TOLLGATELANE(SINDY2INHWYINFO *cHwyInfo);
	~SINDY2TOLLGATELANE();

	HRESULT Convert(void);
	HRESULT Write(std::string cFileName);

	HRESULT GetMeshCode(IFeaturePtr cpHwyNode, long& crMesh);
	HRESULT GetTollGateNodeID(IFeaturePtr cpHwyNode, long& crNodeID);
	HRESULT GetApproachNodeID(IFeaturePtr cpHwyNode, long& crNodeID);
	HRESULT GetLaneStr(IFeaturePtr cpHwyNode, std::string& crLaneStr);
private:
	TollGateLaneInfoArray m_TollGateLaneInfoArray;

public:
	typedef TollGateLaneInfoArray::const_iterator Iter;

	/// �f�[�^��̐擪���w��const_iterator��Ԃ�
	Iter begin() const { return m_TollGateLaneInfoArray.begin(); }

	/// �f�[�^��̖������w��const_iterator��Ԃ�
	Iter end() const { return m_TollGateLaneInfoArray.end(); }

	/// �f�[�^��̃T�C�Y��Ԃ�
	std::size_t size() const { return m_TollGateLaneInfoArray.size(); }
};


#endif	// _SINDY2TOLLGATELANE_H_
