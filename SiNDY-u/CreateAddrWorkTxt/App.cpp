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
#include "App.h"

#include <SiNDYLib/Workspace.h>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <WinLib/VersionInfo.h>

#include "CS2WorkTxt.h"
#include "GP2WorkTxt.h"
#include "KP2WorkTxt.h"

/**
 * @brief	現在の時刻を返す
 * @return	時刻文字列(yyyy/mm/dd hh/mm/ss)
 */
inline CString GetCurTime()
{
	SYSTEMTIME	st;
	GetLocalTime( &st );
	CString strTime;
	strTime.Format( _T("%04d/%02d/%02d %02d:%02d:%02d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond );
	return strTime;
}

// 使用法
void CApp::Usage()
{
	std::cerr << "[住所中間データ作成ツール]\n" 
			  << "(Usage)CreateAddrWorkTxt.exe [作成モードオプション] [各種オプション]\n"
			  << "■作成モードオプション ... いずれか必須指定\n"
			  << " -gpw ... 号ポイント中間データ作成モード\n"
			  << " -kpw ... 家形リンクポイント中間データ作成モード\n"
			  << " -csw ... CSポイント中間データ作成モード\n"
			  << "■各種オプション\n"
			  << " -out ... 出力先(必須) 号・家形リンク->出力ディレクトリ CS->出力ファイル名\n"
			  << " -log ... ログファイル名(必須)\n"
			  << " -jdb ... 住所DB接続先(家形リンク時のみ必須)\n"
			  << " ・実行エリア指定オプション(未指定は、全国)\n"
			  << "     -cl <市区町村コードリスト>\n"
			  << "     -cs <市区町村コード>\n"
			  << "     -cr <開始市区町村コード>-<終了市区町村コード>\n"
			  << "■環境変数について(全て必須)\n"
			  << " DB_ADDRESS      ... 住所系SDE接続プロパティ\n"
			  << " FC_TARGET_POINT ... ターゲット住所ポイントテーブル名(号 or 家形リンク or CS)\n"
			  << " FC_CITY_ADMIN   ... 都市地図行政界テーブル名\n"
			  << std::endl;
}

// 初期化
bool CApp::Init( int argc, _TCHAR* argv[] )
{
	if( argc < 5 )
	{
		std::cerr << "#Error 引数の数が不正です" << std::endl;
		return false;
	}

	// 作成モード確認
	if( _tcscmp(argv[1], _T("-gpw")) == 0 )		m_emMode = kGPMode;
	else if( _tcscmp(argv[1], _T("-kpw")) == 0 )	m_emMode = kKPMode;
	else if( _tcscmp(argv[1], _T("-csw")) == 0 )	m_emMode = kCSMode;
	else
	{
		std::cerr << "#Error 不明な作成モードオプション : " << CT2CA(argv[1]) << std::endl;
		return false;
	}

	// 引数解析
	for( int i = 2; i < argc - 1; ++i )
	{
		if( _tcscmp(argv[i], _T("-out")) == 0 )
		{
			boost::filesystem::wpath out( argv[++i] );
			switch( m_emMode )
			{
			case kGPMode:
			case kKPMode:
				if( !boost::filesystem::is_directory(out) )
				{
					std::cerr << "#Error 号・家形リンクの場合は、出力先【ディレクトリ】を指定して下さい : " << CT2CA(argv[i]) << std::endl;
					return false;
				}
				m_cParam.m_strOutDir = out.string().c_str();
				break;
			case kCSMode:
				if( boost::filesystem::exists(out) && boost::filesystem::is_directory(out) )
				{
					std::cerr << "#Error CSの場合は、出力【ファイル】を指定して下さい : " << CT2CA(argv[i]) << std::endl;
					return false;
				}
				m_cParam.m_strOutFile = out.string().c_str();
				break;
			default:
				return false;
			}
		}
		else if( _tcscmp(argv[i], _T("-log")) == 0 )
		{			
			m_ofs.open( static_cast<CT2CW>(argv[++i]) );
			if( !m_ofs.is_open() )
			{
				std::cerr << "#Error ログファイルのオープンに失敗 : " << CT2CA(argv[i]) << std::endl;
				return false;
			}
			m_ofs.imbue( std::locale(std::locale("Japanese"), "C", std::locale::numeric) );			
		}
		else if( _tcscmp(argv[i], _T("-cl")) == 0 || _tcscmp(argv[i], _T("-cr")) == 0 || _tcscmp(argv[i], _T("-cs")) == 0 )
		{
			m_cParam.m_pairArea.first = argv[i];
			m_cParam.m_pairArea.second = argv[++i];
		}
		else if( _tcscmp(argv[i], _T("-jdb")) == 0 )
		{
			m_cParam.m_strJDBProp = argv[++i];
		}
	}

	// 出力先指定チェック
	if( m_cParam.m_strOutDir.IsEmpty() && m_cParam.m_strOutFile.IsEmpty() )
	{
		std::cerr << "#Error データ出力先(-out)が未指定" << std::endl;
		return false;
	}

	// ログファイル指定チェック
	if( !m_ofs.is_open() )
	{
		std::cerr << "#Error ログファイル(-log)が未指定" << std::endl;
		return false;
	}

	// 住所DB指定チェック(家形リンク時)
	if( kKPMode == m_emMode && m_cParam.m_strJDBProp.IsEmpty() )
	{
		std::cerr << "#Error 住所DB接続先(-jdb)が未指定" << std::endl;
		return false;
	}

	// SDE接続
	CString strEnv;
	if( !strEnv.GetEnvironmentVariable(_T("DB_ADDRESS")) )
	{
		std::cerr << "#Error 環境変数 DB_ADDRESS が設定されていない" << std::endl;
		return false;
	}
	
	CWorkspace cWorkspace;
	if( !cWorkspace.Connect(strEnv) )
	{
		std::cerr << "#Error SDE接続に失敗 : " << CT2CA(strEnv) << std::endl;
		return false;
	}

	// ターゲット住所ポイントテーブルオープン
	if( !strEnv.GetEnvironmentVariable(_T("FC_TARGET_POINT")) )
	{
		std::cerr << "#Error 環境変数 FC_TARGET_POINT が設定されていない" << std::endl;
		return false;
	}

	m_cParam.m_cPointFC.SetObject( cWorkspace.OpenTable(strEnv) );
	if( !m_cParam.m_cPointFC )
	{
		std::cerr << "#Error 住所ポイントフィーチャクラスのオープン失敗 : " << CT2CA(strEnv) << std::endl;
		return false;
	}

	// 都市地図行政界フィーチャクラスオープン
	if( !strEnv.GetEnvironmentVariable(_T("FC_CITY_ADMIN")) )
	{
		std::cerr << "#Error 環境変数 FC_CITY_ADMIN が設定されていない" << std::endl;
		return false;
	}

	m_cParam.m_cCityAdminFC.SetObject( cWorkspace.OpenTable(strEnv) );
	if( !m_cParam.m_cCityAdminFC )
	{
		std::cerr << "#Error 都市地図行政界フィーチャクラスのオープン失敗 : " << CT2CA(strEnv) << std::endl;
		return false;
	}

	return true;
}

// 実行
bool CApp::Run()
{
	// ログヘッダー出力
	CVersion cVer;
	CString strHeader;
	strHeader.Format( _T("#SINDYSTDLOG\n#%s FILEVERSION %s PRODUCTVERSION %s\n#開始時刻 %s"), cVer.GetOriginalFilename(), cVer.GetFileVersion(), cVer.GetProductVersion(), GetCurTime() );
	m_ofs << CT2CA(strHeader) << std::endl;

	// 中間データ生成インスタンス取得
	std::tr1::shared_ptr<CWorkTxtImpl> pWorkTxt;
	switch( m_emMode )
	{
	case kCSMode:	pWorkTxt = std::tr1::shared_ptr<CWorkTxtImpl>(new CCS2WorkTxt(m_cParam, m_ofs));	break;
	case kGPMode:	pWorkTxt = std::tr1::shared_ptr<CWorkTxtImpl>(new CGP2WorkTxt(m_cParam, m_ofs));	break;
	case kKPMode:	pWorkTxt = std::tr1::shared_ptr<CWorkTxtImpl>(new CKP2WorkTxt(m_cParam, m_ofs));	break;
	default:
		return false;
	}

	// 初期化
	if( pWorkTxt->Init() )
	{
		// 出力開始
		if( !pWorkTxt->WriteData() )
		{
			std::cerr << "#Error データ出力中にエラーが発生しました" << std::endl;
			return false;
		}
	}
	else
	{
		std::cerr << "#Error 出力クラスの初期化に失敗" << std::endl;
		return false;
	}


	// 終了時刻
	m_ofs << "#終了時刻 : " << CT2CA(GetCurTime()) << std::endl;

	return true;
}
