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
#include <TDC/useful_headers/directory_util.h>
#include <TDC/useful_headers/boost_program_options.h>

using namespace std;
using namespace uh;
namespace po = boost::program_options;

// コマンドライン引数の解析
bool Arguments::parse(int argc, _TCHAR* argv[])
{
	// TODO: 引数名を記載
	const char * const k_bl_name_db   = "bl_name_db";
	const char * const k_building_db  = "building_db";
	const char * const k_anno_db      = "anno_db";
	const char * const k_mesh_db      = "mesh_db";
	const char * const k_output       = "output";
	const char * const k_meshlist     = "meshlist";
	const char * const k_ng_build     = "ng_build";
	const char * const k_err_word     = "err_word";
	const char * const k_warn_word    = "warn_word";
	const char * const k_bldc_ok_word = "bldc_ok_word";
	const char * const k_bldc_ng_word = "bldc_ng_word";
	const char * const k_chk_fixclass = "chk_fixclass";
	const char * const k_replace_word = "replace_word";
	const char * const k_join_anno    = "join_anno";
	const char * const k_exclude_mark = "exclude_mark";
	const char * const k_buffer_range = "buffer_range";

	po::options_description desc("How to use");

	try{
		// TODO: 引数の受け取る変数と説明を記載
		desc.add_options()
			(k_bl_name_db,   tvalue<tstring>(&m_strBlNameDB),    "[必須]ビル名称DB接続先")
			(k_building_db,  tvalue<tstring>(&m_strBuildingDB),  "[必須]都市建物DB接続先")
			(k_anno_db,      tvalue<tstring>(&m_strAnnoDB),      "[必須]注記DB接続先")
			(k_mesh_db,      tvalue<tstring>(&m_strMeshDB),      "[必須]メッシュDB接続先")
			(k_output,       tvalue<tstring>(&m_strOutput),      "[必須]アウトプット")
			(k_meshlist,     tvalue<tstring>(&m_strMeshList),    "[必須]都市地図メッシュリスト")
			(k_ng_build,     tvalue<tstring>(&m_strNgBuild),     "[必須]NG建物種別リスト")
			(k_err_word,     tvalue<tstring>(&m_strErrWord),     "[必須]NG文字列リスト(error)")
			(k_warn_word,    tvalue<tstring>(&m_strWarnWord),    "[必須]NG文字列リスト(warning)")
			(k_bldc_ok_word, tvalue<tstring>(&m_strBldcOKWord),  "[必須]建物種別コード別OKワードリスト")
			(k_bldc_ng_word, tvalue<tstring>(&m_strBldcNGWord),  "[必須]建物種別コード別NGワードリスト")
			(k_chk_fixclass, tvalue<bool>   (&m_chkFixclass),    "建物種別コード別ワードチェックを種別確定コードが未確認（0）のデータに対してのみ行うかどうか:default true")
			(k_replace_word, tvalue<tstring>(&m_strReplaceWord), "[必須]文字列比較用置換リスト")
			(k_join_anno,    tvalue<tstring>(&m_strJoinAnno),    "[必須]注記比較用結合文字列リスト")
			(k_exclude_mark, tvalue<tstring>(&m_strExclude_mark),"分割数チェック時除外記号リスト")
			(k_buffer_range, tvalue<double>(&m_bufferRange),     "バッファで捕まえる範囲(m):default 50m")
			;

		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);

		// 必須パラメータのチェック
		// TODO: 引数に応じて変更
		bool good = true;
		good &= CheckParam( k_bl_name_db,   m_strBlNameDB )    &&
			    CheckParam( k_building_db,  m_strBuildingDB )  &&
			    CheckParam( k_anno_db,      m_strAnnoDB )      &&
			    CheckParam( k_mesh_db,      m_strMeshDB )      &&
			    CheckParam( k_output,       m_strOutput )      &&
			    CheckParam( k_meshlist,     m_strMeshList )    &&
			    CheckParam( k_ng_build,     m_strNgBuild )     &&
			    CheckParam( k_err_word,     m_strErrWord )     &&
				CheckParam( k_warn_word,    m_strWarnWord )    &&
				CheckParam( k_bldc_ok_word, m_strBldcOKWord )  &&
			    CheckParam( k_bldc_ng_word, m_strBldcNGWord )  &&
			    CheckParam( k_replace_word, m_strReplaceWord ) &&
			    CheckParam( k_join_anno,    m_strJoinAnno );

		if( ! good ){
			cerr << desc << endl;
			return false;
		}
	}
	catch(const std::exception& e){
		cerr << e.what() << endl;
		cerr << desc << endl;
		return false;
	}
	return true;
}

// コマンドライン引数の確認
bool Arguments::CheckParam( const char* const optName, const tstring& optValue, bool bFile /* = false */ )
{
	if( optValue.empty() ){	
		cerr << optName << "オプションは必須です" << endl;
		return false;
	}

	if( bFile ){
		if( ! isFile( optValue.c_str() )){
			cerr << toStr( optValue ) << "はファイルではありません" << endl;
			return false;
		}
	}

	return true;
}
