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
 *	@file LinkPatch.cpp
 *	@brief リンクパッチクラス 実装部
 *	@author	F.Adachi
 *	@date	2005/11/9		新規作成
 *	$ID$
 */

#include "StdAfx.h"
#include "LinkPatch.h"
//#include <sstream>
#include <fstream>
#include <string>

LinkPatchRecord::LinkPatchRecord(void) :
	m_MeshCode(0)
{
}

LinkPatchRecord::~LinkPatchRecord()
{
}

LinkPatch::LinkPatch(void)
{
}

LinkPatch::~LinkPatch()
{
}

/**
 *	@brief	格納
 **/
void LinkPatch::assign( LinkPatchRecord cRecord )
{
	m_Data.push_back( cRecord );
}


/**
 *	@brief	全削除
 **/
void LinkPatch::clear(void)
{
	m_Data.clear();
}

/**
 *	@brief	書き込み
 *	@note	指定されたストリームにリンクパッチの内容を出力する
 **/
void LinkPatch::write( std::ostream& crStream )
{
	for( LinkPatchIter aLinkPatchIter = m_Data.begin(); aLinkPatchIter != m_Data.end(); ++aLinkPatchIter ) {
		crStream << "( ";
		crStream << aLinkPatchIter->m_Attribute << ", ";
		crStream << aLinkPatchIter->m_RevisedValue << ", ";
		crStream << aLinkPatchIter->m_MeshCode;
		
		for( LinkPatchRecord::NodeIDIter aNodeIDIter = aLinkPatchIter->m_NodeIDCollection.begin(); 
			 aNodeIDIter != aLinkPatchIter->m_NodeIDCollection.end();
			 ++aNodeIDIter )
		{
			crStream << ", " << *aNodeIDIter;
		}
		
		crStream << " )," << "\n";
	}
}


/**
 *	@brief	書き込み
 *	@note	ファイル名と文字コード（改行コードのみ考慮）を指定して書き込み
 *	@note	リンクパッチのレコードには日本語(2Byteコード)が含まれないことが前提
 **/
void LinkPatch::write( const char* cFilePath, bool cUseEUC )
{
	std::string aFunc = "LinkPatch::write()";

	// EUCの場合はバイナリモードで開く
	std::ios::openmode aOpenMode;
	if (cUseEUC) { aOpenMode = std::ios::out | std::ios::binary; }
	else		 { aOpenMode = std::ios::out; }

	std::ofstream aFout( cFilePath, aOpenMode );
	if (!aFout) { throw std::runtime_error( "R1F:" + aFunc + " : リンクパッチファイルオープンエラー" ); }
	
	write( aFout );

	aFout.close();
}

