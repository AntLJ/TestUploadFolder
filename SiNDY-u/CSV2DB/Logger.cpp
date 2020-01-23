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

#include <WinLib/VersionInfo.h>
#include <atltime.h>

#include "Logger.h"
//#include "globalfunc.h"
#include "CsvLoader.h"

#include <TDC/sindylib_base/TableContainer.h>
#include <TDC/sindylib_base/RowContainer.h>

// useful_headers
#include <TDC/useful_headers/str_util.h>

// boost
#include <boost/filesystem/path.hpp>

const CString LAYER_FIELD = _T("LAYER");   // レイヤ名
const CString UPDATE_TYPE = _T("更新内容");  // 新規or更新
const CString COLMUNS     = _T("#cols#");  // ヘッダの列数覚えとく用

const CString ORG_FIELD = _T("（前）");   // レイヤ名
const CString MODIFY_FIELD = _T("（後）");   // レイヤ名

// シングルトンオブジェクトのスレッド制御用
std::mutex CRunLogger::s_mutex;
std::mutex CErrLogger::s_mutex;

// ファイルオープン
bool CLogger::Initialize( const uh::tstring& strDirPath )
{
	if( ! m_Log || m_Log.is_open() )
		return false;

	// フォルダパスとファイル名を連結
	//boost::filesystem::path filepath = boost::filesystem::path(uh::toStr( strDirPath.c_str()) + uh::toStr( getFileName() ));

	m_Log.open( strDirPath );
	if( ! m_Log )
		return false;

	writeHeader();

	return true;
}

// ログへメッセージ出力
void CLogger::Log( const uh::tstring& strMessage, bool std_out )
{
	m_Log << uh::toStr( strMessage );
	if( std_out )
		std::cout << uh::toStr( strMessage ) << std::endl;
}


// ログへメッセージ出力
void CLogger::Log( const TCHAR* format, ... )
{
	CString cstr;
	if( format )
	{
		va_list args;
		va_start( args, format );
		cstr.FormatV( format, args );
		va_end(args);
	}
	Log( uh::toTStr( cstr.GetString() ), false );
}


// << オペレータのオーバーロード
CLogger& CLogger::operator<<( const uh::tstring& strMessage )
{
	m_Log << uh::toStr( strMessage );
	return *this;
}



// エラーレベル付きのメッセージ出力
void CRunLogger::Fatal( const uh::tstring& strMessage, bool std_err )
{
	Log( _T("%s FATAL -- : %s\n"), log_utility::GetDateTimeInfo().c_str(), strMessage.c_str() );
	if( std_err )
		std::cerr << uh::toStr( strMessage ) << std::endl;
	//throw std::runtime_error( uh::toStr( strMessage ) );
}

void CRunLogger::Error( const uh::tstring& strMessage )
{
	Log( _T("%s ERROR -- : %s\n"), log_utility::GetDateTimeInfo().c_str(), strMessage.c_str() );
}

void CRunLogger::Warn( const uh::tstring& strMessage )
{
	Log( _T("%s WARN  -- : %s\n"), log_utility::GetDateTimeInfo().c_str(), strMessage.c_str() );
}

void CRunLogger::Info( const uh::tstring& strMessage, bool std_out )
{
	Log( _T("%s INFO  -- : %s\n"), log_utility::GetDateTimeInfo().c_str(), strMessage.c_str() );
	if( std_out )
		std::cout << uh::toStr( strMessage ) << std::endl;
}

void CRunLogger::Debug( const uh::tstring& strMessage )
{
	Log( _T("%s DEBUG -- : %s\n"), log_utility::GetDateTimeInfo().c_str(),  strMessage.c_str() );
}


// 実行ログファイル名を返す
uh::tstring CRunLogger::getFileName()
{
	return log_utility::GetLogfileNamePrefix() + _T("run.log");
}


void CRunLogger::writeHeader()
{
	// ツール名
	CVersion version;
	Log( _T("%s PRODUCTVERSION %s\n"), version.GetInternalName(), version.GetProductVersion() );
}

// 結果出力
void CRunLogger::WriteResult( const CsvLoader& csv )
{
	long all = csv.size();
	long create =std::count_if( csv.cbegin(), csv.cend(), []( const CsvLoader::CsvRecord& record ){
			return record.second.isCreate;
	});
	long update = all - create;

	CString result = 
		_T("\n")
		_T("####################\n")
		_T("# 処理結果\n")
		_T("####################\n")
		_T("入力レコード数 ： %ld\n")
		_T("--\n")
		_T(" 更新レコード数： %ld\n")
		_T(" 追加レコード数： %ld\n");

	Log( uh::str_util::format( result, all, update, create ), true );
}

// エラーログファイル名を返す
uh::tstring CErrLogger::getFileName()
{
	return log_utility::GetLogfileNamePrefix() + _T("err.log");
}


void CErrLogger::writeHeader()
{
	Log( _T( "#FREESTYLELOG\n"), false );
}

std::map<CString, long> getColumnIndex( const CsvLoader& csv )
{
	using namespace sindy::schema;

	static std::map<CString, long> columnIdx;
	if( !columnIdx.empty() )
		return columnIdx;
	
	// 固定列
	std::vector<CString> columns = boost::assign::list_of
		(LAYER_FIELD)             // LAYER
		(kObjectID)               // OBJECTID
		(UPDATE_TYPE);            // 更新内容（新規or更新）
	
	bool hasShapeField = false;
	for( const auto& fi : csv.GetFieldInfos() )
	{
		if( fi.isOidField() )
			continue;

		if( fi.isLatLonField() )
		{
			hasShapeField = true;
			continue;
		}

		columnIdx[ fi.fieldName ] = columns.size();

		columns.push_back( fi.fieldName + ORG_FIELD );
		columns.push_back( fi.fieldName + MODIFY_FIELD );
	}

	// 緯経度フィールドを最後に積む
	if( hasShapeField )
	{
		columnIdx[ ipc_feature::kShape ] = columns.size();
		columns.push_back( LON_FIELD + ORG_FIELD );
		columns.push_back( LAT_FIELD + ORG_FIELD );
		columns.push_back( LON_FIELD + MODIFY_FIELD );
		columns.push_back( LAT_FIELD + MODIFY_FIELD );
	}

	// 列数を覚えておく
	columnIdx[ COLMUNS ] = columns.size();

	// ヘッダ行書き出し
	ERRLOGGER.Log( uh::str_util::join( columns, _T("\t") ) + _T("\n"), false );

	return columnIdx;
}

void CErrLogger::WriteLog( const CsvLoader& csv, 
	                       const sindy::CTableContainer& destT )
{
	using namespace sindy;
	using namespace sindy::schema;

	// ヘッダ名のインデクスを取得
	std::map<CString, long> columnIdx = getColumnIndex( csv );

	// 緯経度出力用
	auto toLatLon = [&columnIdx]( const CRowContainer& row, bool before, bool after, std::vector<CString>& columunVal)
	{
		long logIdx = columnIdx[ ipc_feature::kShape ];
		if( before )
		{
			IPointPtr orgP = row.CRowBase::GetShapeCopy(enumOriginal);
			if( !orgP )
			{
				// 前にNULL形状はあり得る（bug 11640）
				columunVal[ logIdx ]     = _T("<null>");
				columunVal[ logIdx + 1 ] = _T("<null>");
			}
			else
			{
				double org_x(0), org_y(0);
				orgP->QueryCoords( &org_x, &org_y );
				columunVal[ logIdx ]     = uh::str_util::ToString(org_x, _T("%lf"));   // LON(前)
				columunVal[ logIdx + 1 ] = uh::str_util::ToString(org_y, _T("%lf"));   // LAT(前)
			}
		}
		
		if( after )
		{
			IPointPtr newP = row.CRowBase::GetShapeCopy();
			if( !newP )
			{
                // NULL形状にするような機能はなく、もともとNULL形状のものを弄らなかった場合は
                // ログ出力対象にならない（差分なし）のでここでNULL形状となることはないはず
				throw std::runtime_error( uh::str_util::formatA( 
					_T("【内部エラー】形状の取得に失敗（OID: %ld"), row.GetOID() ) );
			}
			double x(0), y(0);
			newP->QueryCoords( &x, &y );
			columunVal[ logIdx + 2 ]     = uh::str_util::ToString(x, _T("%lf"));   // LON(前)
			columunVal[ logIdx + 3 ] = uh::str_util::ToString(y, _T("%lf"));       // LAT(前)
		}
	};
	
	for( const auto& con : destT) 
	{
		const auto& row = *CAST_ROWC(con);

		// 1行分書き出し
		std::vector<CString> columunVal(columnIdx[ COLMUNS ]);
		columunVal[0] = destT.GetTableName();
		columunVal[1] = uh::str_util::ToString(row.GetOID());
		columunVal[2] = row.IsCreated() ? _T("新規") : _T("更新");

		// 新規ならすべて（後）に書き出し
		if( row.IsCreated() )
		{
			for( const auto& col : columnIdx )
			{
				if( col.first.Compare( COLMUNS ) == 0 )
					continue;

				// 形状なら
				const CString& fieldName = col.first;
				if( fieldName == ipc_feature::kShape )
				{
					toLatLon( row, false, true, columunVal );
					continue;
				}

				// 新規の場合、OBJECTIDはGetValueで拾えない。。
				CString val = uh::str_util::ToString(
					( FieldInfo::isOidField( fieldName ) ) ? row.GetOID() : row.GetValue( fieldName ) );
				
				// （後）なので +1
				long logIdx = col.second;
				columunVal[ logIdx + 1 ] = val;
			}
		}
		else
		{
			const RecordInfo& ri = csv.find_by_oid( row.GetOID() );
			for( const auto& fldVal : ri )
			{
				const FieldInfo& fi = fldVal.first;
				
				// 形状なら
				if( fi.isLatLonField() )
				{
					toLatLon( row, (ri.hasShape || fi.forceOutput), ri.hasShape, columunVal );
					continue;
				}

				// 変更があれば（前）（後）
				long logIdx = columnIdx[ fi.fieldName ];
				if( row.CRowBase::Changed( fi.fieldName ))
				{
					columunVal[ logIdx ] = uh::str_util::ToString( row.GetOriginalValue( fi.fieldName ) );
					columunVal[ logIdx + 1 ] = uh::str_util::ToString( row.GetValue( fi.fieldName ) );
					continue;
				}

				// 強制出力オプションで指定されたフィールドなら変更がなくても（前）
				if( fi.forceOutput )
					columunVal[ logIdx ] = uh::str_util::ToString( row.GetOriginalValue( fi.fieldName ) );
			}
		}

		// 1行分書き出し
		ERRLOGGER.Log( uh::str_util::join( columunVal, _T("\t") ) + _T("\n"), false );
	}
}

namespace log_utility
{
	// エラーログファイル名を取得
	static uh::tstring GetLogfileNamePrefix()
	{
		// ツール名取得
		CVersion version;
		CString strToolName = version.GetInternalName();
		// 拡張子があれば削除
		if( strToolName.Find( _T('.') ) != -1 )
			strToolName = strToolName.Left( strToolName.ReverseFind(_T('.')) );;

		// 現在の日時を"YYYYMMDDHHDDSS" で取得
		CTime cTime = CTime::GetCurrentTime();
		const CString strTime = cTime.Format( _T("%Y%m%d%H%M%S") );

		return strToolName + _T("_") + strTime.GetString() + _T("_");
	}

	// 日時の情報を文字列で返す
	static uh::tstring GetDateTimeInfo()
	{
		CTime cTime = CTime::GetCurrentTime();
		const CString strTime = cTime.Format( _T("[%Y-%m-%d %H:%M:%S]") );

		return uh::toTStr( strTime.GetString() );
	}
}
