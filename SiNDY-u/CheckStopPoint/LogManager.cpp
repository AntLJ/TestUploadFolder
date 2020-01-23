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
#include "LogManager.h"
#include "ErrorLevelDef.h"
#include "CheckStopPoint.h"

using namespace std;

// コンストラクタ
CLogManager::CLogManager(void)
{
}

// デストラクタ
CLogManager::~CLogManager(void)
{
	Close();
}

// 初期設定
bool CLogManager::Init( const CString& logPath )
{
	return Open( logPath );
}

// ログファイルオープン
bool CLogManager::Open( const CString& logPath )
{
	m_ofs.open( logPath );
	if( !m_ofs || !m_ofs.is_open() ){
		cerr << "#ERROR ログファイルが開けません：" << CStringA( logPath ) << endl;
		return false;
	}
	return true;
}

// ログファイルクローズ
void CLogManager::Close()
{
	if( m_ofs.is_open() )
	{
		WriteInfo( "#終了時間 : %s", GetTime() );
		m_ofs.close();
	}
}

// 現在時刻取得
CStringA CLogManager::GetTime()
{
	SYSTEMTIME st;
	::GetLocalTime( &st );
	CStringA strTime;
	strTime.Format( "%d/%02d/%02d %02d:%02d:%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond );
	return strTime;
}

// 抽出モードのログヘッダー出力
void CLogManager::WriteExtractLogHeader()
{
	WriteInfo( "#FREESTYLELOG" );
	CStringA header;
	header = "FLAG\tLAYER\tOBJECTID\tMESHCODE\tERRORLEVEL\tMESSAGE";
	// 日本だと七差路が最大？SiNDY上だとまた違うかもしれないけど10あれば足りるだろう
	for( long i = 1; i <= 10; ++i )
	{
		CStringA str;
		str.Format( "\tROAD_LINK%d", i );
		header += str;
		str.Format( "\tROADCLASS_C%d", i );
		header += str;
	}
	WriteInfo( header );
	WriteInfo( "#開始時間 : %s", GetTime() );
}

// チェックモードのログヘッダー出力
void CLogManager::WriteCheckLogHeader()
{
	WriteInfo( "#FREESTYLELOG" );
	CStringA header;
	header = "FLAG\tLAYER\tOBJECTID\tLON\tLAT\tERRORLEVEL\tERRORCODE\tMESSAGE";
	// Info1～3のカラム名を追加
	for( int i=1; i<=3; ++i )
	{
		header.AppendFormat("\tInfo%d", i);
	}
	header.AppendFormat("\tCOMMENT");
	WriteInfo( header );
	WriteInfo( "#開始時間 : %s", GetTime() );
}

// ログに任意の文字列出力
void CLogManager::WriteInfo( LPCSTR message, ... )
{
	va_list vaList = NULL;
	va_start( vaList, message );

	CStringA strLog;
	strLog.FormatV( message, vaList );
	m_ofs << strLog << endl;
}

// 抽出ログ出力
void CLogManager::WriteExtractLog( const CStringA& layer, long oid, long meshcode, const CStringA& errlevel, const CStringA& message, const CStringA& linkInfo )
{
	m_ofs << "0\t" << layer << "\t" << oid << "\t" << meshcode << "\t" << errlevel << "\t" << message << "\t" << linkInfo << endl;
}

/**
 * @brief エラーレベルから文字列を取得
 * @param elevel [in] エラーレベル
 * @return エラーレベルを表す文字列
 */
inline CStringA convErrorLevel2Str(ERROR_LEVEL elevel)
{
	switch(elevel)
	{
	case kError:
		return "ERROR";
	case kWarn:
		return "WARNING";
	default:
		break;
	}
	return "";
}

// チェックログ出力
void CLogManager::WriteCheckLog( const CStringA& layer, long oid, long errorCode, double lon, double lat, long meshcode, long linkID, long nodeID )
{
	// 経緯度格納用文字列
	CStringA strLon, strLat;
	//経緯度は小数点以下を7桁で出力
	strLon.Format("%.7lf", lon);
	strLat.Format("%.7lf", lat);

	m_ofs << "0\t" << layer << "\t" << oid << "\t" << strLon << "\t" << strLat << "\t";

	// エラーコードとメッセージを出力
	switch(errorCode)
	{
#define ERROR_CODE(code,level,...) case code: m_ofs << convErrorLevel2Str(level) << "\t" << code << "\t" << #__VA_ARGS__; break;
#include "ErrorCode.h"
#undef ERROR_CODE
		default: m_ofs << "このエラーコードには対応していません\tERROR\t" << errorCode << "\t"; break;
	}
	m_ofs << "\t" << meshcode << "\t" << linkID << "\t" << nodeID << "\t" << endl;
}
