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

// 初期化
bool CAddrEdtInitialDataCreator::init()
{
	// インポートタイプに応じて生成するインスタンスを変える
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

	// 初期化
	return (*m_dataCreator).init();
}

// 実行
bool CAddrEdtInitialDataCreator::execute()
{
	// 初期化
	if( !init() )
		return false;

	// データ生成
	return (*m_dataCreator).create();
}



