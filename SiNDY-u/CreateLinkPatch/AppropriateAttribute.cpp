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

/**
 *	@file AppropriateAttribute.cpp
 *	@brief 路線コード指定ファイルクラス 実装部
 *	@author	F.Adachi
 *	@date	2005/08/10		新規作成
 *	$ID$
 */

#include "StdAfx.h"
#include "AppropriateAttribute.h"
#include "ZCL/ZSepByTab.h"
#include <sstream>

/// コンストラクタ
AppropriateAttribute::AppropriateAttribute(void)
{
}

/// デストラクタ
AppropriateAttribute::~AppropriateAttribute()
{
}

/**
 *	@brief	路線コード指定ファイルの読み込み
 *	@param	cpFilePath	[in]	路線コード指定ファイルのパス
 **/
void AppropriateAttribute::read( const char* cpFilePath )
{
	std::string aFunc = "AppropriateAttribute::read()";

	ifstream aFin( cpFilePath );
	if (!aFin) {
		throw std::runtime_error( "R1F:" + aFunc + " : 路線コード指定ファイルオープンエラー : " + cpFilePath );
	}

	std::string aLineBuff;
	int aLineCount = 0;
	while ( std::getline( aFin, aLineBuff ) ) {
		++aLineCount;
		ZSepByTab	aSepByTab;
		aSepByTab.analyze( aLineBuff.c_str() );
		
		if (aSepByTab.getCount() != 2) {
			std::ostringstream aMessage;
			aMessage << aFunc << " : 不正なレコード " << cpFilePath << " LINE " << aLineCount;
			throw std::runtime_error( "R1F:" + aMessage.str() );
			continue;
		}

		m_Table.insert( std::pair<int, int>( atoi(aSepByTab[0]), atoi(aSepByTab[1]) ) );

	}

#ifdef	_DEBUG
	std::cout << cpFilePath << " : " << aLineCount << "行読み込み完了" << std::endl;
#endif

	aFin.close();
}


/**
 *	@brief	テーブル全体をストリーム出力
 *	@param	crStream	[in]	出力ストリーム
 **/
void AppropriateAttribute::disp( std::ostream& crStream ) const
{
	for (std::map<long, long>::const_iterator aIter = m_Table.begin();	aIter != m_Table.end();	++aIter ) {
		crStream << aIter->first << "\t" << aIter->second << std::endl;
	}
}


/**
 *	@brief	道路標示種別の取得
 *	@note	指定された路線コードに対応する道路標示種別をバッファに格納して返す
 *	@param	cRoadCode	[in]	路線コード
 **/
long AppropriateAttribute::getRoadClass( long cRoadCode ) const
{
	std::string aFunc = "AppropriateAttribute::getRoadClass()";

	std::map<long, long>::const_iterator aIter = m_Table.find( cRoadCode );
	if ( aIter == m_Table.end() ) {
		std::ostringstream aMessage;
		aMessage << aFunc << " : 路線コード指定ファイルに存在しない路線コード [" << cRoadCode << "]";
		throw std::runtime_error( "R1F:" + aMessage.str() );
	}

	return aIter->second;
}

