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

// CheckTelTPError.cpp: CheckTelTPError クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CheckTelTPError.h"
#include <WinLib/VersionInfo.h>

#include "TelTPHelper.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

CheckTelTPError::CheckTelTPError()
{
}

CheckTelTPError::~CheckTelTPError()
{
}

// 使用法
void CheckTelTPError::PrintUsage()
{
	std::cout << "[TelTPPoint過去トラチェックツール]\n"
			  << "(Usage)CheckTelTPError.exe [入力ファイル] [出力ファイル]\n"
			  << "・環境変数について\n"
			  << "  DB_ADDRESS       ... 住所系接続プロパティ\n"
			  << "  FC_TELTP_POINT   ... TelTPポイントフィーチャクラス" << std::endl;
}

// 初期化
bool CheckTelTPError::Init( int cArgc, _TCHAR* cArgv[] )
{
	if( cArgc < 3 )
	{
		cerr << "引数が足りません" << endl;
		return false;
	}

	int i = 1;

	// 入力ファイルのオープン
	if( !m_cListHandle.loadData(cArgv[i]) )
	{
		cerr << "入力ファイルを開けません: " << CT2CA(cArgv[i]) << endl;
		return false;
	}

	// 出力ファイルのオープン・ヘッダ出力
	m_ofs.open( static_cast<CT2CW>(cArgv[++i]) );
	if( !m_ofs.is_open() )
	{
		cerr << "出力ファイルを開けません:" << CT2CA(cArgv[i]) << endl;
		return false;
	}

	// 環境変数から各種設定
	if( !fnSetInfoFromEnv() )
		return false;


	// ログヘッダー出力
	CVersion cVer;
	CString strMsg;
	strMsg.Format( _T("#FREESTYLELOG\n")
				   _T("#%s FILEVERSION %s PRODUCTVERSION %s\n")
				   _T("FLAG\tクレームNo.\tObjectID\tシーケンス\tMESH\tX\tY\t座標付与種別\tユーザクレームフラグ\t電話番号\t掲載住所\t住所コード\t方書\t掲載名称\t調査結果コード\t情報ソース\tメッセージ")
				   _T("\t1:MESH\t1:X\t1:Y\tCOMMENT"),
				   cVer.GetOriginalFilename(), cVer.GetFileVersion(), cVer.GetProductVersion() );
	m_ofs << CT2CA(strMsg) << std::endl;

	return	true;
}

// 実行
bool CheckTelTPError::Execute()
{
	// ヘルパークラス初期化
	CTelTPHelper	cTelTPHelper( m_cTelTPPointFC );

	CLList& listRec = m_cListHandle.getRecodList();
	CLList::const_iterator itrE = listRec.end();
	for( CLList::iterator itr = listRec.begin(); itr != itrE; ++itr )
	{
		// メッシュコード・フィールドが空なら付与モードへ
		if( itr->m_cMXY.m_nMesh == 0 )
		{
			m_bAddMode = true;
			cout << CT2CA(itr->m_strSequence) << ": 座標付与中..." << endl;
		}
		else
		{
			m_bAddMode = false;
			cout << CT2CA(itr->m_strSequence) << ": チェック中..." << endl;
		}
		
		// TEL_TP_POINT から検索
		EResultStat emStatus = cTelTPHelper.findPoint(*itr, m_bAddMode);

		outputLog( emStatus, *itr );	// 出力
	}
	return true;
}



//////////////////////////////////
//	内部関数
//////////////////////////////////

// 環境変数取得
bool CheckTelTPError::fnSetInfoFromEnv()
{

#ifdef _DEBUG
//	_tputenv( _T("DB_ADDRESS=SiNDY2009B/SiNDY2009B/SDE.DEFAULT/5151/topaz2") );
//	_tputenv( _T("FC_TELTP_POINT=TelTP_Point") );
	_tputenv( _T("DB_ADDRESS=prodeng2/prodeng2/SDE.DEFAULT/5151/coral2") );
	_tputenv( _T("FC_TELTP_POINT=prodeng2.TelTP_Point") );
#endif

	// 住所DB接続
	CString strDBProp;
	if( !strDBProp.GetEnvironmentVariable(_T("DB_ADDRESS")) )
	{
		cerr << "環境変数 DB_ADDRESS が設定されていません" << endl;
		return false;
	}
	if( !m_cWorkspace.Connect(strDBProp) )
	{
		cerr << "住所DBへの接続に失敗 : " << CT2CA(strDBProp) << endl;
		return false;
	}

	// フィーチャクラス取得
	CString strFCName;
	if( !strFCName.GetEnvironmentVariable(_T("FC_TELTP_POINT")) )
	{
		cerr << "環境変数 FC_TELTP_POINT が設定されていません" << endl;
		return false;
	}
	m_cTelTPPointFC.SetObject( m_cWorkspace.OpenTable(strFCName) );
	if( NULL == m_cTelTPPointFC )
	{
		cerr << "TelTPポイントフィーチャクラスの取得に失敗 : " << CT2CA(strFCName) << std::endl;
		return false;
	}
	return true;
}

// ログ出力
void CheckTelTPError::outputLog( EResultStat emStatus, CLRec &cRec )
{
	CString strLog;

	// FLAG クレームNo. ObjectID シーケンス
	strLog.Format( _T("0\t%s\t%s\t%s\t"), cRec.m_strClaimNo, cRec.m_strObjectID, cRec.m_strSequence );

	// MESH X Y
	if( cRec.m_cMXY.m_nMesh == 0 )
		strLog.Append( _T("\t\t\t") );
	else
		strLog.AppendFormat( _T("%d\t%d\t%d\t"), cRec.m_cMXY.m_nMesh, cRec.m_cMXY.m_nX, cRec.m_cMXY.m_nY );

	// 座標付与種別(ADDXY_C) ユーザクレームフラグ 電話番号 掲載住所 住所コード 方書 掲載名称 調査結果コード 情報ソース
	strLog.AppendFormat( _T("%d\t%d\t%s\t%s\t%s\t%s\t%s\t%d\t%s\t"), cRec.m_lAddXY, cRec.m_lUserClaimF, cRec.m_strTelNum, cRec.m_strAddrName, 
								cRec.m_strAddrCode, cRec.m_strKatagaki, cRec.m_strKeisaiName, cRec.m_lPointStat, 
								cRec.m_strSource );
	// メッセージ
	if( !cRec.m_strAddComment.IsEmpty() && !m_bAddMode )
		strLog.AppendFormat( _T("%s%s\t"), GetResultMsg(emStatus), cRec.m_strAddComment );
	else
		strLog.AppendFormat( _T("%s\t"), GetResultMsg(emStatus) );
	
	/* ドメイン名使うなら下記を利用
	// 座標付与種別(ADDXY_C) 電話番号 掲載住所 住所コード 方書 掲載名称 調査結果コード 情報ソース メッセージ
	strLog.AppendFormat( _T("%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t"), static_cast<CString>((cTelTPHelper.getDomain( _T("ADDXY_C"), cRec.m_lAddXY )).c_str()), 
								cRec.m_strTelNum, cRec.m_strAddrName, cRec.m_strAddrCode, cRec.m_strKatagaki, cRec.m_strKeisaiName, 
								static_cast<CString>((cTelTPHelper.getDomain( _T("POINTSTAT_C"), cRec.m_lPointStat )).c_str()), cRec.m_strSource, GetResultMsg(emStatus) );
*/
	// 1:MESH　1:X　1:Y
	if( cRec.m_cMXY_Ref.m_nMesh == 0 )
		strLog.Append( _T("\t\t\t") );
	else
		strLog.AppendFormat( _T("%d\t%d\t%d\t"), cRec.m_cMXY_Ref.m_nMesh, cRec.m_cMXY_Ref.m_nX, cRec.m_cMXY_Ref.m_nY );

	m_ofs << CT2CA(strLog) << std::endl;
}




