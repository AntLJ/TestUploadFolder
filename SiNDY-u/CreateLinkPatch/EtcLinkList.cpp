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

// EtcLinkList.cpp: EtcLinkList クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EtcLinkList.h"
#include <fstream>
#include <string>

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

EtcLinkList::EtcLinkList()
{

}

EtcLinkList::~EtcLinkList()
{

}


/**
 *	@brief	書き込み
 *	@note	指定されたストリームにデータを出力する
 **/
void EtcLinkList::write( std::ostream& crStream )
{
	for (Data::const_iterator aIt = mData.begin(); aIt != mData.end(); ++aIt)
	{
		// 分類コード
		crStream << aIt->eType;

		// メッシュコード
		crStream << "\t";
		crStream << aIt->eMeshCode;

		// 始点ノードID
		crStream << "\t";
		crStream << aIt->eFromNodeID;

		// 中点ノードID列
		for (EtcLink::NodeList::const_iterator aNodeIt = aIt->eNodeList.begin(); aNodeIt != aIt->eNodeList.end(); ++aNodeIt)
		{
			crStream << "\t";
			crStream << *aNodeIt;
		}

		// 終点ノードID
		crStream << "\t";
		crStream << aIt->eToNodeID;

		crStream << std::endl;
	}
}


/**
 *	@brief	書き込み
 *	@note	ファイル名と文字コード（改行コードのみ考慮）を指定して書き込み
 **/
void EtcLinkList::write(const char* cFilePath, bool cUseEUC)
{
	std::string aFunc = "EtcLinkList::write()";

	// EUCの場合はバイナリモードで開く
	std::ios::openmode aOpenMode;
	if (cUseEUC) { aOpenMode = std::ios::out | std::ios::binary; }
	else		 { aOpenMode = std::ios::out; }

	std::ofstream aFout( cFilePath, aOpenMode );
	if (!aFout) { throw std::runtime_error( "R1F:" + aFunc + " : スマートIC用リンクリストファイルオープンエラー" ); }
	
	write( aFout );

	aFout.close();
	
}
