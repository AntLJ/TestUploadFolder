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
#include "Arguments.h"
#include <iostream>
#include <TDC/useful_headers/directory_util.h>
#include <TDC/useful_headers/boost_program_options.h>
#include "TDC/useful_headers/text_util.h"
#include "TDC/sindylib_base/sindylib_base_global.h"
#include <regex>

using namespace std;
using namespace uh;
using namespace meshtype;
namespace po = boost::program_options;

// コマンドライン引数の解析
bool Arguments::parse(int argc, _TCHAR* argv[])
{
	const string k_base = "base";
	const string k_comp = "comp";
	const string k_mesh = "mesh";
	const string k_date = "date";
	const string k_runlog = "runlog";
	const string k_errlog = "errlog";

	po::options_description desc("How to use");

	try
	{
		tstring strModDate;

		desc.add_options()
			(k_base.c_str(),		tvalue<tstring>(&m_BaseDB),		"[必須]比較元バージョンのDB")
			(k_comp.c_str(),		tvalue<tstring>(&m_CompDB),		"[必須]比較先バージョンのDB")
			(k_mesh.c_str(),		tvalue<tstring>(&m_MeshList),	"[必須]メッシュリスト")
			(k_date.c_str(),		tvalue<tstring>(&strModDate),	"[任意]最終更新日")
			(k_runlog.c_str(),		tvalue<tstring>(&m_Runlog),		"[必須]実行ログ")
			(k_errlog.c_str(),		tvalue<tstring>(&m_Errlog),		"[必須]エラーログ")
			;

		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);

		// 必須パラメータのチェック
		bool good = true;
		if(m_BaseDB.empty())
		{
			wcerr << L"比較元バージョンのDBを指定してください" << endl;
			good = false;
		}

		if(m_CompDB.empty())
		{
			wcerr << L"比較先バージョンのDBを指定してください" << endl;
			good = false;
		}

		if(m_MeshList.empty())
		{
			wcerr << L"メッシュリストを指定してください" << endl;
			good = false;
		}

		if(m_Runlog.empty())
		{
			wcerr << L"実行ログを指定してください" << endl;
			good = false;
		}

		if(m_Errlog.empty())
		{
			wcerr << L"エラーログを指定してください" << endl;
			good = false;
		}
		
		if(!good)
		{
			cerr << desc << endl;
			return false;
		}

		// 最終更新日の閾値が指定されていれば設定
		if( !strModDate.empty() )
		{
			m_ModDate = strModDate.c_str();
			if( FAILED( m_ModDate.ChangeType(VT_DATE) ) )
			{
				wcerr << L"指定された最終更新日が不正です" << endl;
				return false;
			}
		}
	}
	catch(const std::exception& e)
	{
		cerr << e.what() << endl;
		cerr << desc << endl;
		return false;
	}
	return true;
}

bool Arguments::parseMeshes( vector<long>& meshes, map<int, CString>& mapInvalidRow )
{
	using namespace uh::str_util;
		
	// メッシュリストの解析
	int rowCnt = 0;
	const auto& listStrMesh = text_util::ToTStrList(m_MeshList);
	for( const auto& strMesh : listStrMesh )
	{
		++rowCnt;

		try
		{
			// メッシュコードとして正しいかチェック
			// 最初の桁は3～6, 6桁の場合は下2桁が0～7, 8桁の場合は下4桁が0～7
			const tstring strRegEx( _T("^[3-6]\\d{3}([0-7]{2}|[0-7]{4})$") );
			bool isValidMeshCode = regex_match( toStr(strMesh), smatch(), regex( toStr(strRegEx) ) );
			if( !isValidMeshCode )
				throw std::exception();

			// long型にキャスト
			long mesh = boost::lexical_cast<long>( strMesh );

			// 6桁か8桁のコードしか格納されないはず
			meshes.emplace_back( mesh );
		}
		catch( ... )
		{
			// キャストできない場合、メッシュコードとして不正な場合、
			// 処理対象のメッシュでない場合には、行番号と紐付けて保持
			mapInvalidRow[rowCnt] = strMesh.c_str();
		}
		
	}
	
	// 不正文字があったらここで終了
	if( !mapInvalidRow.empty() )
	{
		m_MeshType = kInvalid;
		return false;
	}

	// メッシュリストの1行目の桁数から、処理メッシュ単位を判定
	int frontMeshDigit = g_cnv.GetMeshDigit( meshes.front() );
	switch (frontMeshDigit)
	{
	case 6:
		m_MeshType = kBasemesh;
		break;
	case 8:
		m_MeshType = kCitymesh;
		break;
	default:
		assert(false);
		break;
	}

	int meshCnt = 0;
	for( const auto& mesh : meshes )
	{
		++meshCnt;

		// 1行目と桁数が同じであればOK
		if( frontMeshDigit == g_cnv.GetMeshDigit( mesh ) )
			continue;

		// 桁数が異なる場合は、行番号と紐付けて保持
		mapInvalidRow[meshCnt] = format( _T("%d"), mesh );
	}

	// 不正な文字列がなければtrue
	return mapInvalidRow.empty();
}
