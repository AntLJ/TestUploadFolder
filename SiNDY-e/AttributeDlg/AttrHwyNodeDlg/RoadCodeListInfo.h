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

#include <string>
#include <set>

// Road_Code_List����擾���������i�[����\����
struct RoadCodeListRecord
{
	long		m_ObjectID;		///< �I�u�W�F�N�gID
	long		m_RoadCode;		///< �H���R�[�h
	std::string	m_NameKanji;	///< �H�����̊���
	std::string	m_NameYomi;		///< �H�����̓ǂ�
	long		m_LineClass;	///< �H����ʃR�[�h
};

// �{�ݖ��̓ǂ݁{�H���R�[�h��RoadCodeListRecord���\�[�g���邽�߂̃t�@���N�^
struct lessYomiAndRoadCode : public std::binary_function<RoadCodeListRecord, RoadCodeListRecord, bool>
{
	bool operator() (const RoadCodeListRecord& lhs, const RoadCodeListRecord& rhs) const
	{
		return
			lhs.m_NameYomi != rhs.m_NameYomi ? lhs.m_NameYomi < rhs.m_NameYomi :
											   lhs.m_RoadCode < rhs.m_RoadCode;
	}
};

// RoadCodeListRecord���i�[����R���e�i
typedef std::set<RoadCodeListRecord, lessYomiAndRoadCode> RoadCodeListArray;

