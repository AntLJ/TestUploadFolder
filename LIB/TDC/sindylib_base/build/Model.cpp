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
#include <boost/foreach.hpp>
#include "Model.h"

#define foreach BOOST_FOREACH

CModel::CModel()
{
}

CModel::~CModel()
{
}

// モデルのトップから末端までをシリアルに取得する
std::list<CString> CModel::GetOrderdNameList( LPCTSTR lpcszTopLevelParentName ) const
{
	std::list<CString> ret; // 戻り値
	std::set<CString> for_find; // 検索済み親の検索用

	TYPEDEF_MODEL model( m_model );
	for( TYPEDEF_MODEL::iterator it = model.begin(); it != model.end(); )
	{
		if( ( it->second.ParentName.IsEmpty() && it->second.Name == lpcszTopLevelParentName ) ||// 親がいない＝トップレベル
			for_find.end() != for_find.find( it->second.ParentName ) ) // 親が既に戻り値として登録済み
		{
			CString strName( it->first ); // 消されるので保持
			ret.push_back( strName );
			for_find.insert( strName );
			// モデルから削除する
			model.erase( it );
			it = model.begin(); // もう一度最初から
		}
		else
			++it;
	}

	return ret;
}

// 親に当たるモデル名を取得する
std::list<CString> CModel::GetParent( LPCTSTR lpcszName ) const
{
	std::list<CString> ret; // 戻り値

	foreach( const CModel::TYPEDEF_MODEL_PAIR& m, std::make_pair( m_model.find( lpcszName ), m_model.end() ) )
		ret.push_back( m.second.ParentName );

	_ASSERT_EXPR( ! ret.empty(), _T("モデル名が登録されていない") );

	return ret;
}

// 子にあたるモデル名を取得する
std::list<CString> CModel::GetChild( LPCTSTR lpcszName ) const
{
	std::list<CString> ret; // 戻り値

	if( ! lpcszName )
		return ret;

	foreach( const CModel::TYPEDEF_MODEL_PAIR& m, m_model )
	{
		if( 0 != m.second.ParentName.CompareNoCase( lpcszName ) )
			continue;

		ret.push_back( m.first );
	}
	return ret;
}

// モデルを追加する
CModel::TYPEDEF_MODEL::iterator CModel::AddModel( LPCTSTR lpcszChild, LPCTSTR lpcszChildKey, LPCTSTR lpcszParent, LPCTSTR lpcszParentKey )
{
	TYPEDEF_MODEL::iterator it = m_model.insert( std::make_pair(CString(lpcszChild),stModel()) );
	stModel& model = it->second;
	model.Name = lpcszChild;
	model.OuterKey = lpcszChildKey;
	model.ParentName = lpcszParent;
	model.ParentKey = lpcszParentKey;
	return it;
}

void CModel::Add( LPCTSTR lpcszChild, LPCTSTR lpcszChildKey, LPCTSTR lpcszParent, LPCTSTR lpcszParentKey, LPCTSTR lpcszParentStrictKey, const CComVariant& vaStrictValue )
{
	// 結合フィールドが2つの場合用（SiNDY-i の FEATURE_RELとINFO_POINT,INFO_LINE,INFO_POLYGONなど）
	TYPEDEF_MODEL::iterator it = AddModel( lpcszChild, lpcszChildKey, lpcszParent, lpcszParentKey );
	it->second.ParentStrictKey = lpcszParentStrictKey;
	it->second.ParentStrictValue = vaStrictValue;
}

// 子のモデル名からキー等を取得する
std::list<stModel> CModel::GetKeys( LPCTSTR lpcszName ) const
{
	std::list<stModel> listRet; // 戻り値

	foreach( const CModel::TYPEDEF_MODEL_PAIR& m, std::make_pair( m_model.find( lpcszName ), m_model.end() ) )
	{
		if( m.second.Name != lpcszName )
			break;
		listRet.push_back( m.second );
	}

	return listRet;
}

// 親のモデル名からキー等を取得する
std::list<stModel> CModel::GetKeysByParentName( LPCTSTR lpcszName ) const
{
	std::list<stModel> listRet; // 戻り値

	foreach( const CModel::TYPEDEF_MODEL_PAIR& m, m_model )
	{
		if( 0 != m.second.ParentName.CompareNoCase( lpcszName ) )
			continue;

		listRet.push_back( m.second );
	}

	return listRet;

}

// 親子のモデル名からキー等を取得する
stModel CModel::GetKey( LPCTSTR lpcszName, LPCTSTR lpcszParent ) const
{
	foreach( const CModel::TYPEDEF_MODEL_PAIR& m, std::make_pair( m_model.find( lpcszName ), m_model.end() ) )
	{
		if( m.second.ParentName == lpcszParent )
			return m.second;
	}
	return stModel();
}
