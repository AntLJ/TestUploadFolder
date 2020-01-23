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
#include "PathFinder.h"
#include "PathUtility.h"
#include "Notifier.h"
#include <TDC/useful_headers/directory_util.h>
#include <TDC/useful_headers/boost_program_options.h>
#include <TDC/useful_headers/text_util.h>

using namespace std;
using namespace uh;
namespace po = boost::program_options;

// コマンドライン引数の解析
bool Arguments::parse(int argc, _TCHAR* argv[])
{
	const char * const k_input					= "input";
	const char * const k_output					= "output";
	const char * const k_input_owner			= "input_owner";
	const char * const k_output_owner			= "output_owner";
	const char * const k_mesh                   = "mesh";
	const char * const k_mesh_db                = "mesh_db";
	const char * const k_mesh_table_owner       = "mesh_table_owner";
	const char * const k_mesh_layer_name        = "mesh_layer_name";
	const char * const k_merge                  = "merge";
	const char * const k_convert                = "convert";
	const char * const k_crdcnv                 = "crdcnv";
	const char * const k_clip                   = "clip";
	const char * const k_runlog					= "runlog";
	const char * const k_errlog					= "errlog";
	const char * const k_thread                 = "thread";
	const char * const k_value                  = "value";
	const char * const k_dissolve_multi         = "dissolve_multi";
	const char * const k_output_ondemand_search = "output_ondemand_search";
// TODO: split_by_mesh メッシュで切るオプション

	po::options_description desc("How to use");

	try{
		desc.add_options()
			(k_input,					tvalue<tstring>(&m_input),				  "[必須]インプットDB情報")
			(k_output,					tvalue<tstring>(&m_output),				  "[必須]アウトプットDB情報")
			(k_input_owner,				tvalue<tstring>(&m_input_owner),		  "[任意]インプット側オーナー名")
			(k_output_owner,			tvalue<tstring>(&m_output_owner),		  "[任意]アウトプット側オーナー名")
			(k_mesh,					tvalue<tstring>(&m_mesh_list_str),		  "[任意]メッシュ/リスト/ファイル")
			(k_mesh_db,	                tvalue<tstring>(&m_meshDB),               "[必須]メッシュDB")
			(k_mesh_table_owner,	    tvalue<tstring>(&m_meshTableOwner),       "[任意]メッシュDBオーナー")                // TODO: 必須パラメータの調整すること
			(k_mesh_layer_name,	        tvalue<tstring>(&m_meshLayerName),        "[必須]メッシュDB参照レイヤ名")
			(k_merge,					tvalue<tstring>(&m_mapDefFile[s2sModeMerge]),   "[必須]マージ処理定義ファイルパス")
			(k_convert,					tvalue<tstring>(&m_mapDefFile[s2sModeConvert]), "[必須]コンバート処理定義ファイルパス")
			(k_crdcnv,					tvalue<tstring>(&m_mapDefFile[s2sModeCrdcnv]),  "[必須]測地系変換処理定義ファイルパス")
			(k_clip,					tvalue<tstring>(&m_mapDefFile[s2sModeClip]),    "[必須]クリップ処理定義ファイルパス")
			(k_runlog,					tvalue<tstring>(&m_runLog),				  "[必須]実行ログファイルパス")
			(k_errlog,					tvalue<tstring>(&m_errLog),				  "[必須]エラーログファイルパス")
			(k_thread,					tvalue<long>(&m_thread),				  "[任意]実行スレッド数")
			(k_value,                   tvalue<std::vector<tstring>>(&m_values),  "[任意]定義中で使用する値の指定(複数可)")
			(k_dissolve_multi,          tvalue<bool>(&m_dissolveMulti),           "[任意]処理後にマルチパート形状を解消するか（デフォルト：true）")
			(k_output_ondemand_search,  tvalue<bool>(&m_output_ondemand_search),  "[任意][コンバート処理]アウトプット側の更新対象フィーチャは都度検索する（デフォルト：false）")
			;

		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);

		// 必須パラメータのチェック
		bool good = true;
		good &= CheckParam( k_input,  m_input);
		good &= CheckParam( k_output, m_output );

		if( ! good ){
			cerr << "必須パラメータが足りません" << std::endl;
			cerr << desc << endl;
			return false;
		}

		// どれかは最低一つ設定されている必要がある
		bool hasMode = ( [&]()
		{
			for( const auto& path : m_mapDefFile )
			{
				if( !path.second.empty() )
					return true;
			}
			return false;
		}() );
		if( ! hasMode ) {
			cerr << "モードパラメータが一つも指定されていません" << std::endl;
			cerr << desc << endl;
			return false;
		}

		// 入力がPG(FG)DBの場合
		if( isPGDBorFGDB( m_input ) )
		{
			// PG(FG)DBのディレクトリパス群を取得
			m_paths = getPGFGDBPath(m_input);
		}

		// メッシュコードが設定されているか
		if(!m_mesh_list_str.empty())
		{
			// メッシュコードをパース
			for( auto meshcode : text_util::ToLongList(m_mesh_list_str) )
			{
				std::stringstream ss;
				ss << meshcode;

				m_meshes.push_back( uh::toTStr(ss.str()) );
			}
		}
		else
		{
			// メッシュコードが省略されていて、入力がPG(FG)DBの場合
			// PG(FG)DBのファイル名称からメッシュコードを取得
			if( isPGDBorFGDB(m_input) )
			{
				m_meshes = getMeshCodesFromPaths(m_paths);
			}
			else
			{
				// SDEの場合はエラー??
			}
		}

		if( !m_values.empty() )
		{
			// 「:」で分割する
			uh::tstring::size_type pos = uh::tstring::npos;
			for( const auto& value : m_values )
			{
				uh::tstring::size_type pos = value.find(_T(":"));
				if( uh::tstring::npos == pos )
				{
					cerr << "valueパラメータの区切り文字がありません：" << toStr(value) << std::endl;
					return false;
				}
				m_mapValue[value.substr( 0, pos )] = CComVariant(value.substr(pos+1).c_str());
			}
		}
	}
	catch(const std::exception& e){
		cerr << e.what() << endl;
		cerr << desc << endl;
		return false;
	}
	return true;
}

bool Arguments::CheckParam( const char* const optName, const tstring& optValue, bool bFile /* = false */ )
{
	if( optValue.empty() ){
		cerr << optName<< "オプションは必須です" << endl;
		return false;
	}
	return true;
}

bool Arguments::isPGDBorFGDB(const uh::tstring& str)
{
	return uh::isFile(str.c_str()) || uh::isDirectory(str.c_str());
}

// インプット/アウトプット側のオーナー名補完テーブル名を返す
uh::tstring Arguments::getInputTableName( const uh::tstring& name ) const
{
	if( !m_input_owner.empty() )
		return m_input_owner + _T(".") + name;

	return name;
}

uh::tstring Arguments::getOutputTableName( const uh::tstring& name ) const
{
	if( !m_output_owner.empty() )
		return m_output_owner + _T(".") + name;

	return name;
}

// オプションで指定された値を取得する
CComVariant Arguments::GetValue( LPCTSTR name ) const
{
	// TODO: --value name1:value1 --value name2:value2 ... みたいな感じで指定できるようにする
	const auto& it = m_mapValue.find(name);
	if( it != m_mapValue.end() )
		return it->second;
	else {
		std::wstringstream ss;
		ss << _T("定義ファイルで指定されている値がオプションで指定されていません：") << name;
		notifier::critical::versatile(ss.str());
	}
	return CComVariant();
}
// 識別子に対応するシステム変数を取得
uh::tstring Arguments::GetSystemValue( SYSTEM_VALUE identifier, const uh::tstring& nullValue /*= uh::tstring()*/ ) const
{
	auto it = m_systemValue.find(identifier);
	if(m_systemValue.end()==it) return nullValue;
	return it->second;
}

std::vector<uh::tstring> Arguments::getPGFGDBPath(const uh::tstring& directoryPath)
{
	// inputのPG(FG)DBのパスを取得
	PathFinder finder;
	std::vector<uh::tstring> filter;
	filter.push_back(_T("gdb"));
	filter.push_back(_T("mdb"));
	std::vector<uh::tstring> vPaths( finder.getFilePathByExtension(directoryPath, filter ));
	return vPaths;
}

std::vector<uh::tstring> Arguments::getMeshCodesFromPaths(const std::vector<uh::tstring>& vPaths)
{
	std::vector<uh::tstring> ret;
	for (const auto& p : vPaths )
	{
		ret.push_back(PathUtillity::getStem(p));
	}
	return ret;
}
