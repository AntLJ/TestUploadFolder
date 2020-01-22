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

// VerifyLog.cpp: CVerifyLog クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VerifyLog.h"
#include <crd_cnv.h>


CVerifyLog::CVerifyLog()
{
	m_UpdateFiledDomain		= NULL;	//!< 更新日フィールドドメイン
}

CVerifyLog::~CVerifyLog()
{
	if( m_UpdateFiledDomain )
		m_UpdateFiledDomain->clear();

}
BOOL CVerifyLog::Initialize()
{
	// ルールの初期化
	if( !m_pISiNDYRuleObj )
		m_pISiNDYRuleObj.CreateInstance( CLSID_SiNDYRuleObj );

	// [Bug4820]ログインせずに内容確認ボタンを2回押すと落ちるので、ログインしているか
	// どうか確認しておく
	VARIANT_BOOL vaLogined = VARIANT_FALSE;
	m_pISiNDYRuleObj->IsLogined( &vaLogined );
	// オペレータ名を取得
	if( vaLogined == VARIANT_TRUE )
	{
		CComBSTR bstrOperator;
		m_pISiNDYRuleObj->get_OperatorName( &bstrOperator );
		m_strOperator = CString( bstrOperator );
	}

	// 現在の年月日を取得
	::GetLocalTime( &m_SystemTime);

	// ホスト名を取得
	if( m_strHostName.GetLength() == 0 ){
		DWORD	nMaxLen = MAX_COMPUTERNAME_LENGTH+1;
		TCHAR	chCompName[MAX_COMPUTERNAME_LENGTH+1];
		if( !::GetComputerName( chCompName, &nMaxLen ) )
			return FALSE;

		chCompName[nMaxLen+1] = NULL;
		m_strHostName = chCompName;
	}

	// SiNDY-eインストールディレクトリを取得
	if( m_strInstallDir.GetLength() == 0 ){
		CInifileHelper cCInifileHelper;
		if( !cCInifileHelper.GetInstallRoot( m_strInstallDir )) 
			return FALSE;
	}
	
	// ログ格納ディレクトリの存在確認
	if( m_strLogDirName.GetLength() == 0 ){
		m_strLogDirName.Format( _T("%slog\\"), m_strInstallDir );

		if( !CFile::FileExists( m_strLogDirName ) ){
			if( !::CreateDirectory( m_strLogDirName, NULL ) ){
				m_strLogDirName = _T("");	// クリア
				return FALSE;
			}
		}
	}

	// ファイル名を作成
	if( m_strLogFileName.GetLength() == 0 )
		m_strLogFileName.Format( _T("%s%s_%s_%d_%02d%02d.log"),
								m_strLogDirName,	// ログ格納ディレクトリ
								m_strHostName,		// ホスト名
								m_strOperator,		// オペレータ名
								m_SystemTime.wYear,	// 確認年
								m_SystemTime.wMonth,	// 確認月
								m_SystemTime.wDay		// 確認日
								);

	if( !CFile::FileExists( m_strLogFileName ) ){
		// 存在していない場合は、新規にファイルを作成する
		if( !m_VerifyLogFile.Create( m_strLogFileName, GENERIC_READ | GENERIC_WRITE ) ){
			m_strLogFileName = _T("");	// 作成できない場合はクリアしておく
			return FALSE;
		}
		// ログタイプを書き込む
		CString strLogType;
		strLogType.Format( _T("#%s\r\n"), _T("VERIFYLOG") );

		m_VerifyLogFile.Write( strLogType );
	}else{
		// 存在している場合は該当ファイルがオープン済がチェック
		if( HANDLE( m_VerifyLogFile ) == INVALID_HANDLE_VALUE ){
			// 未オープンの場合はファイルをオープンする
			if( !m_VerifyLogFile.Open( m_strLogFileName, GENERIC_READ | GENERIC_WRITE ) )
				return FALSE;

			// 既存ファイルの末尾に追加するためファイルポインタをファイルの終わり（EOF）に設定。
			m_VerifyLogFile.Seek( 0, FILE_END );
		}
	}
	return TRUE;
}

BOOL CVerifyLog::AppendVerifyLog( IMap* ipMap, _IRowPtr ipRow )
{
	if( !Initialize() )
		return FALSE;

	CString	strLineBuffer;

	// - 1)チェック済フラグ
	strLineBuffer += _T("0");	// 常に0
	strLineBuffer += DELIMITA;

	// - 2)レイヤ名
	CString strLayerName = AheGetFeatureClassName( ipRow );
	strLineBuffer += strLayerName + DELIMITA;

	// - 3)オブジェクトＩＤ
	long nOID;
	ipRow->get_OID( &nOID );
	CString strTemp;
	strTemp.Format( _T("%d"), nOID );
	strLineBuffer += strTemp;
	strLineBuffer += DELIMITA;

	if( m_strPrevLayerName == strLayerName && m_nPrevOID == nOID ){
		AheMessageBox( NULL, AheLoadString( IDS_SAMEITEM ), AheLoadString( IDS_CAPTION ), MB_OK|MB_ICONWARNING|MB_TASKMODAL, WARNLEVEL_NONE );
		// 確認作業の頻度（更新間隔はそれほど短くないであろう）とログファイルの重要さから毎回クローズしたほうが良いかな？
		m_VerifyLogFile.Close();
		return FALSE;
	}

	m_strPrevLayerName = strLayerName;
	m_nPrevOID = nOID;

	// 画面中心緯度経度から2次メッシュコードと都市地図メッシュコードを取得する
	//IMapPtr ipMap( pArcHelper->GetMap() );
	IActiveViewPtr ipActiveView( ipMap );

	IEnvelopePtr ipScreenEnv = NULL;
	ipActiveView->get_Extent( &ipScreenEnv );	// スクリーンの矩形と幅・高さ取得

	IPointPtr ipPoint = NULL;
	((IAreaPtr)ipScreenEnv)->get_Centroid( &ipPoint );

	DOUBLE dLongitude, dLatitude;
	ipPoint->get_Y( &dLatitude );
	ipPoint->get_X( &dLongitude );

	// 座標変換
	crd_cnv cnv;
	int nMesh = 0, nX = 0, nY = 0;

	// - 4)2次メッシュコード
    cnv.LLtoMesh( dLongitude, dLatitude, 2, &nMesh, &nX, &nY );
	strTemp.Format( _T("%d"), nMesh );
	strLineBuffer += strTemp + DELIMITA;

	// - 5)都市地図メッシュコード
    cnv.LLtoMesh( dLongitude, dLatitude, 64, &nMesh, &nX, &nY );
	strTemp.Format( _T("%d"), nMesh );
	strLineBuffer += strTemp + DELIMITA;

	// - 6)確認作業時のユーザ名 
	// ユーザ名を取得
	strLineBuffer += m_strOperator + DELIMITA;
	
	// - 7)確認作業時のＰＣ名
	strLineBuffer += m_strHostName + DELIMITA;

	// - 8)確認作業時のボタンを押した日時(2003/06/26 12:00:00)
	strTemp.Format( _T("%d/%02d/%02d %02d:%02d:%02d"), 
						m_SystemTime.wYear, 
						m_SystemTime.wMonth, 
						m_SystemTime.wDay, 
						m_SystemTime.wHour, 
						m_SystemTime.wMinute, 
						m_SystemTime.wSecond );
	strLineBuffer += strTemp + DELIMITA;

	// - 9)確認作業時のMXDファイル名
	CComBSTR bstrMXDFile;
	m_pISiNDYRuleObj->get_MXDFileName( &bstrMXDFile );
	strLineBuffer += CString( bstrMXDFile ) + DELIMITA;

	// - 10)確認作業時のツールのバージョン(episode???)
	// [HKEY_LOCAL_MACHINE\SOFTWARE\INCREMENT P CORPORATION\SiNDY-e\Version] "Version"="Release1"
	// からツールバージョンを取得
	if( m_strToolVer.GetLength() == 0 ){
		CString strRegOpenKey( _T("SOFTWARE\\INCREMENT P CORPORATION\\SiNDY-e\\Version" ) );

		HKEY hOpenKey = EmxCreateRegKey( strRegOpenKey, HKEY_LOCAL_MACHINE );
		if ( hOpenKey ) {
			TCHAR strToolVersion[_MAX_PATH];

			if( EmxGetRegString( _T("Version"), strToolVersion, _MAX_PATH, hOpenKey ) == TRUE )
				m_strToolVer = CString( strToolVersion ) + DELIMITA;
			else
				m_strToolVer += DELIMITA;

			RegCloseKey( hOpenKey );
		}
	}
	strLineBuffer += m_strToolVer;

	// - 11)最終変更時のユーザ名
	CComVariant vTemp = AheGetAttribute( ipRow, OPERATOR_FIELD );
	vTemp.ChangeType( VT_BSTR );
	strLineBuffer +=  CString( vTemp.bstrVal ) + DELIMITA;

	// - 12)最終変更時の作業目的
	vTemp = AheGetAttribute( ipRow, PURPOSE_C_FIELD );
	CComBSTR bstrName;
	m_pISiNDYRuleObj->GetOperatingPurposeFromID( vTemp.lVal, &bstrName );
	strLineBuffer +=  CString( bstrName ) + DELIMITA;

	// - 13)最終変更日時
	vTemp = AheGetAttribute( ipRow, MODIFYDATE_FIELD );
	vTemp.ChangeType( VT_BSTR );
	strLineBuffer +=  CString( vTemp.bstrVal ) + DELIMITA;

	// - 14)最終変更時の変更内容
	vTemp = AheGetAttribute( ipRow, UPDATETYPE_CFIELD );

	if( m_UpdateFiledDomain == NULL ){
		IFieldsPtr ipFields;
		ipRow->get_Fields( &ipFields );
		long lFiledIndex;
		ipFields->FindField( CComBSTR( UPDATETYPE_CFIELD ), &lFiledIndex );

		IFieldPtr ipField;
		ipFields->get_Field( lFiledIndex, &ipField );

		// フィールド情報を取得
		m_UpdateFiled.SetFieldDef( strLayerName, UPDATETYPE_CFIELD );	
		m_UpdateFiled.SetRowBuffer( lFiledIndex, ipField, ipRow );

		m_UpdateFiledDomain = m_UpdateFiled.GetDomain();
	}
	for( DOMAINMAP::iterator l = m_UpdateFiledDomain->begin(); l != m_UpdateFiledDomain->end(); l++ ) {
		if( l->code == vTemp ) {
			strTemp = l->code_discription;
			break;
		}
	}

	strLineBuffer += strTemp + DELIMITA;

	// - 15)プログラム最終変更日時
	vTemp = AheGetAttribute( ipRow, _T("PROGMODIFYDATE") );
	vTemp.ChangeType( VT_BSTR );
	strLineBuffer +=  CString( vTemp.bstrVal ) + DELIMITA;

	// - 16)プログラム最終変更時のプログラム名
	vTemp = AheGetAttribute( ipRow, _T("MODIFYPROGNAME") );
	vTemp.ChangeType( VT_BSTR );
	strLineBuffer +=  CString( vTemp.bstrVal ) + DELIMITA;

	// - 17)その他コメント行
	strLineBuffer += _T("\r\n");

	// 確認内容を書き込む
	m_VerifyLogFile.Write( strLineBuffer );

	// 確認作業の頻度（更新間隔はそれほど短くないであろう）とログファイルの重要さから毎回クローズしたほうが良いかな？
	m_VerifyLogFile.Close();


	return TRUE;
}
