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

#include "stdafx.h"
#include "AddrEdtInitialDataCreator.h"
#include "LayerInfoManager.h"
#include "AddrDataCreator.h"
#include "PlaceNameRepCreator.h"
#include "Utility.h"

using namespace std;
using namespace sindy;

// ������
bool CAddrEdtInitialDataCreator::init()
{
	// �C���|�[�g�^�C�v�ɉ����Đ�������C���X�^���X��ς���
	if( Utility::ExecuteType::AddrPoly == m_args.m_impType ||
		Utility::ExecuteType::AddrPoint == m_args.m_impType ||
		Utility::ExecuteType::AddrAll == m_args.m_impType )
	{
		m_dataCreator = std::make_shared<AddrDataCreator>(m_args);
	}
	else if( Utility::ExecuteType::PlaceNameRep == m_args.m_impType )
	{
		m_dataCreator = std::make_shared<PlaceNameRepCreator>(m_args);
	}

	// ������
	return (*m_dataCreator).init();
}

// ���s
bool CAddrEdtInitialDataCreator::execute()
{
	// ������
	if( !init() )
		return false;

	// �f�[�^����
	return (*m_dataCreator).create();
}



