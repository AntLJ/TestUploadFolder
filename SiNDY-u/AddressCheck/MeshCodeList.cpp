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

#include "StdAfx.h"
#include "MeshCodeList.h"

// メッシュリスト読み込み
bool MeshCodeList::load( const CString& path )
{
	ifstream ifs( static_cast<CT2CW>(path) );
	if( !ifs.is_open() )
		return false;

	copy( istream_iterator<unsigned long>(ifs), 
		istream_iterator<unsigned long>(), 
		back_inserter(m_listMeshCode) );

	return ifs.eof();
}

// メッシュリスト取得
const std::list<unsigned long>& MeshCodeList::getMeshCodeList() const
{
	return m_listMeshCode;
}