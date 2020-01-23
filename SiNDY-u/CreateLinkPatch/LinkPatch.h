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
 *	@file LinkPatch.h
 *	@brief リンクパッチクラス 定義部
 *	@author	F.Adachi
 *	@date	2005/11/9		新規作成
 *	$ID$
 */

#ifndef ___LINKPATCH___
#define ___LINKPATCH___

#include <boost/utility.hpp>
#include <vector>

/// <b>リンクパッチのレコードクラス</b>
struct LinkPatchRecord
{
	std::string			m_Attribute;		//< 修正属性
	std::string			m_RevisedValue;		//< 修正値（文字列）
	long				m_MeshCode;			//< 対象２次メッシュコード
	std::vector<long>	m_NodeIDCollection;	//< 対象ノードID列(２以上)

	LinkPatchRecord(void);
	~LinkPatchRecord();

	typedef std::vector<long>::const_iterator NodeIDIter;
};


/// <b>リンクパッチクラス</b>
class LinkPatch : boost::noncopyable
{
public:
	LinkPatch(void);
	~LinkPatch();

	void assign( LinkPatchRecord cRecord );
	void clear(void);
	void write( std::ostream& crStream );
	void write( const char* cFilePath, bool cUseEUC );

	typedef std::vector<LinkPatchRecord>::const_iterator LinkPatchIter;
private:
	std::vector<LinkPatchRecord>	m_Data;		//< リンクパッチのレコード列

};

#endif
