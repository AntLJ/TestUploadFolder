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

#include "TransParam.h"


CTransParam::CTransParam(void)
{
	m_bNoShape = true;
}


CTransParam::~CTransParam(void)
{
}

// 要求されているオプションの確認
bool CTransParam::check_required_option(const po::variables_map& vm, LPCSTR lpszOption, LPCSTR lpszDesc)
{
	if(vm.count(lpszOption) == 0) {
		std::cout << lpszDesc << "が未設定です。" << std::endl;
		std::cerr << lpszDesc << "が未設定です。" << std::endl;
		return false;
	}
	return true;
}

bool CTransParam::parse(int argc, _TCHAR* argv[])
{
	po::options_description desc("How to use");

	desc.add_options()
		( g_strTaregetDB, tvalue<tstring>(&m_tstrTargetDB), "[必須]変換対象DB" )
		( g_strLayer, tvalue<tstring>(&m_tstrLayer), "[必須]レイヤ名" )
		( g_strMeshDB, tvalue<tstring>(&m_tstrMesh), "[選択]メッシュDB(二次メッシュ)" )
		( g_strMeshSchema, tvalue<tstring>(&m_tstrMeshSchema), "[選択]メッシュレイヤのあるスキーマ" )
		( g_strMeshlist, tvalue<tstring>(&m_tstrMeshlist), "[選択]メッシュリスト" )
		( g_strOutDir, tvalue<tstring>(&m_tstrOutDir), "[選択]シェープファイル出力ディレクトリ" )
		( g_strIdentifier, tvalue<tstring>(&m_tstrIdentifier), "[選択]出力シェープファイルの識別子" )
		( g_strOutFileName, tvalue<tstring>(&m_tstrFileName), "[選択]出力ファイル名(拡張子なし)" )
		( g_strColumnlist, tvalue<tstring>(&m_tstrColumnlist), "[必須]変換フィールドの対応表" )
		( g_strNoShape, "[選択]形状を出力させない" )
		( g_strWhereQuery, tvalue<tstring>(&m_tstrWhereQuery), "[選択]QueryFilter用Where句" )
		( g_strWhereLoop, tvalue<tstring>(&m_tstrWhereLoop), "[選択]フィーチャ取得ループ内用Where句" )
		( g_Runlog, tvalue<tstring>(&m_tstrRunLog), "[選択]Runログファイル名" )
		( g_Errlog, tvalue<tstring>(&m_tstrErrLog), "[選択]Errログファイル名" );

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	// 必須パラメータのチェック
	bool bAllow = true;
	bAllow &= check_required_option(vm, g_strTaregetDB, 	"[必須]変換対象DB");
	bAllow &= check_required_option(vm, g_strLayer, 	"[必須]レイヤ名");
	bAllow &= check_required_option(vm, g_strColumnlist, 	"[必須]変換フィールドの対応表");
	// メッシュの指定がある場合は、出力ディレクトリの指定が必須
	if( !m_tstrMesh.empty() )
	{
		bAllow &= check_required_option(vm, g_strMeshDB, 	"[必須]メッシュDB(二次メッシュ)");
		bAllow &= check_required_option(vm, g_strMeshlist, 	"[必須]メッシュリスト");
		if( m_tstrOutDir.empty() && m_tstrFileName.empty() )
		{
			// メッシュ指定あり、シェープファイル出力ディレクトリ or 出力ファイル名の指定がない場合
			bAllow &= check_required_option(vm, "", 	"[必須]シェープファイル出力ディレクトリまたは、出力ファイル名の指定が必要です");
		}
	}
	else if( !m_tstrFileName.empty() )
	{
		bAllow &= check_required_option(vm, g_strOutFileName, 	"[必須]出力ファイル名(拡張子なし)");
	}
	else
		bAllow &= check_required_option(vm, "", 	"[必須]メッシュDBまたは、出力ファイル名の指定が必要です");

	m_bNoShape = (vm.count(g_strNoShape) > 0);

	if( !bAllow )
	{
		std::cerr << desc << std::endl;
		return false;
	}

	return true;
}

std::string CTransParam::GetOption()
{
	std::string strOption = "[option]\n";
	if( !m_tstrTargetDB.empty() )
		strOption.append( GetOptionLine( g_strTaregetDB, uh::toStr( m_tstrTargetDB ) ) );

	if( !m_tstrLayer.empty() )
		strOption.append( GetOptionLine( g_strLayer, uh::toStr( m_tstrLayer ) ) );

	if( !m_tstrMesh.empty() )
		strOption.append( GetOptionLine( g_strMeshDB, uh::toStr( m_tstrMesh ) ) );

	if( !m_tstrMeshSchema.empty() )
		strOption.append( GetOptionLine( g_strMeshSchema, uh::toStr( m_tstrMeshSchema ) ) );

	if( !m_tstrMeshlist.empty() )
		strOption.append( GetOptionLine( g_strMeshlist, uh::toStr( m_tstrMeshlist ) ) );

	if( !m_tstrColumnlist.empty() )
		strOption.append( GetOptionLine( g_strColumnlist, uh::toStr( m_tstrColumnlist ) ) );

	if( !m_tstrOutDir.empty() )
		strOption.append( GetOptionLine( g_strOutDir, uh::toStr( m_tstrOutDir ) ) );

	if( !m_tstrFileName.empty() )
		strOption.append( GetOptionLine( g_strOutFileName, uh::toStr( m_tstrFileName ) ) );

	if( !m_tstrWhereQuery.empty() )
		strOption.append( GetOptionLine( g_strWhereQuery, uh::toStr( m_tstrWhereQuery ) ) );

	if( !m_tstrWhereLoop.empty() )
		strOption.append( GetOptionLine( g_strWhereLoop, uh::toStr( m_tstrWhereLoop ) ) );

	if( !m_tstrRunLog.empty() )
		strOption.append( GetOptionLine( g_Runlog, uh::toStr( m_tstrRunLog ) ) );

	if( !m_tstrErrLog.empty() )
		strOption.append( GetOptionLine( g_Errlog, uh::toStr( m_tstrErrLog ) ) );

	return strOption;
}

std::string CTransParam::GetOptionLine(const char* const cParam, std::string strValue)
{
	std::string strOptionLine = "   --";
	strOptionLine.append( cParam );
	strOptionLine.append( " " );
	strOptionLine.append( strValue );
	strOptionLine.append( "\n" );

	return strOptionLine;
}
