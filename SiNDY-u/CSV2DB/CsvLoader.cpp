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
#include "CsvLoader.h"
#include "Logger.h"
#include "globalfunc.h"
#include <TDC/sindylib_base/TableContainer.h>
#include <TDC/sindylib_core/FieldMap.h>
#include <boost/filesystem.hpp>

const CString LAT_FIELD = _T("LAT");       // 経度
const CString LON_FIELD = _T("LON");       // 緯度

using namespace sindy;
using namespace sindy::schema;

FieldInfo::FieldInfo( long csvIdx, long destIdx, const CString& field, bool opt ):
	csvIndex(csvIdx),
	destIndex(destIdx),
	fieldName(field),
	forceOutput(opt),
	isChanged(false)
{
}

// コンストラクタ
RecordInfo::RecordInfo():
	isCreate(false),
	oid(-1),
	hasShape(false),
	latlon()
{
}

// []オペレータ
CComVariant& RecordInfo::operator [](const FieldInfo& fi)
{
	// フィールド名で探す
	auto& it = boost::find_if( m_fieldValues, [&fi](const fieldValue& fldVal){
		return fldVal.first.fieldName == fi.fieldName;
	});

	// 既に持っていればそれを返す
	if( it != m_fieldValues.end() )
		return it->second;

	// なければ追加する
	m_fieldValues.push_back(std::make_pair(fi, CComVariant()));
	return m_fieldValues.back().second;
}

bool RecordInfo::setShape( const CString& fieldName, CComVariant& val )
{
	if( FAILED( val.ChangeType(VT_R8) ) )
		return false;

	if( fieldName.CompareNoCase( LON_FIELD ) == 0 )
		latlon.X = val.dblVal;
	else if( fieldName.CompareNoCase( LAT_FIELD ) == 0 )
		latlon.Y = val.dblVal;

	hasShape = true;

	return true;
}

void CsvLoader::init( const CString& path, const sindy::CFieldMap& destField, const CString& optFields )
{
	TryOpenCSV( path );

	// 「OBJECTID」フィールドは必須
	const auto& csvField = m_adoCur.fields();
	auto it = boost::find_if( csvField, [&](const CADOCursor::Field& field){
		return ( FieldInfo::isOidField(field.name) );
	});
	if( csvField.end() == it )
	{
		throw std::runtime_error( "入力ファイルにOBJECTIDフィールドが存在しません。" );
	}
	
	// 更新フィールド存在チェック
	CStringA noFields;
	bool hasLat(false), hasLon(false);
	for( const auto& field : csvField )
	{
		if( FieldInfo::isLatField( field.name ) )
		{
			hasLat = true;
			continue;
		}
		
		if( FieldInfo::isLonField( field.name ) )
		{
			hasLon = true;
			continue;
		}

		if( (destField.HasField( field.name )) < 0 )
			noFields.AppendFormat( "[%s] ", CT2A(field.name) );
	}
	if( !noFields.IsEmpty() )
	{
		std::string msg = "更新先のテーブルに存在しないフィールドです： " + noFields;
		throw std::runtime_error( msg );
	}

	// LAN,LOT不揃いチェック
	if( hasLat ^ hasLon )
	{
		throw std::runtime_error( "LON または LAT フィールドが存在しません。" );
	}

	// 強制出力フィールド名存在チェック
	auto optFieldLst = uh::str_util::split( optFields, _T(",") );
	for( const auto& optFld : optFieldLst )
	{
		auto exist = boost::find_if( csvField, [&optFld](const CADOCursor::Field& filed ){
			return (optFld.CompareNoCase( filed.name ) == 0); 
		});

		if( exist == csvField.end() )
			noFields.AppendFormat( "[%s] ", CT2A(optFld) );
	}
	if( !noFields.IsEmpty() )
	{
		std::string msg = "--force_output オプションのフィールドが不正です： " + noFields;
		throw std::runtime_error( msg );
	}

	// フィールドの情報を確保
	long srcIdx = 0;
	for( auto itFld = csvField.begin(); itFld != csvField.end(); ++itFld, ++srcIdx )
	{
		m_fieldInfos.push_back( FieldInfo( 
			srcIdx,
			destField.HasField( itFld->name ),
			itFld->name,
			boost::find( optFieldLst, itFld->name ) != optFieldLst.end() )
		);
	}
}

// Load()で呼ばれる非メンバ関数のプロトタイプ宣言
bool ValidateData( int lineNum, 
				   CComVariant& val, 
				   const FieldInfo& fi, 
				   const CFieldMap& fm, 
				   std::vector<CString>& errs );

void CsvLoader::Load( const CString& path, const CString& optFields, const sindy::CTableContainer& table )
{
	boost::timer t;
	RUNLOGGER.Info( uh::str_util::format(
		_T("入力ファイル読み込み... : %s"), path), true);

	auto fm = *table.GetFieldMap();
	init( path, fm, optFields );

	// 全レコードを検査しながら取得
	long lineNum = 1;     // 行数カウンタ。ヘッダを考慮して1から
	bool hasErr = false;
	for( auto it = m_adoCur.begin(); it != m_adoCur.end(); ++it, ++lineNum )
	{
		// フィールドごとの値を取得する
		RecordInfo ri;
		for( const auto& fi : GetFieldInfos() )
		{
			CComVariant& csvVal = (*it)[fi.csvIndex];   // CSVで指定された値
			
			// フィールド値チェック
			std::vector<CString> errs;
			if( !ValidateData( lineNum, csvVal, fi, fm, errs ) )
			{
				RUNLOGGER.Log( uh::str_util::join(errs, _T("\n")) + _T("\n"), false );
				hasErr = true;
				continue;
			}
			
			if( fi.isOidField() )
			{
				if( uh::variant_util::isNullOrEmpty(csvVal) )
					ri.isCreate = true;
				else
					ri.oid = csvVal.lVal;
			}
			else if( fi.isLatLonField() )
			{
				ri.setShape( fi.fieldName, csvVal );
			}

			// フィールドの値を取得
			ri[fi] = csvVal;
		}

		// 更新対象なら対象レコードの存在を確認しておく
		if( !ri.isCreate &&
			!table.GetRow( ri.oid ) )
		{
			RUNLOGGER.Log( uh::str_util::format( 
				_T("Line:%ld\t\t\t更新対象のオブジェクトが存在しません（OID:%ld）\n"), lineNum, ri.oid ),
				false );

			hasErr = true;
			continue;
		}

		// 更新対象
		Add( lineNum, ri );
	}

	if( hasErr )
	{
		throw std::runtime_error( "入力データにエラーがあります。" );
	}

	RUNLOGGER.Info( 
		uh::str_util::format( _T("完了: %ld records. ( %lf[s] )\n"), m_records.size(), t.elapsed() ), true);
}

// 追加
void CsvLoader::Add( long lineNum, const RecordInfo& record )
{
	m_records.push_back(
		std::make_pair( lineNum, record )
	);
}

// OIDで検索
const RecordInfo& CsvLoader::find_by_oid( long oid ) const
{
	auto it = boost::find_if( m_records, [&oid]( const CsvRecord& record ){
		return (record.second.oid == oid);
	});

	if( it == m_records.end() )
	{
		throw std::runtime_error( uh::str_util::formatA(
			_T("【内部エラー】CSVのキャッシュ中に存在しないレコードです（OID:%ld）"), oid ) );
	}
	return it->second;
}

// レコード読み込み
void CsvLoader::TryOpenCSV( const CString& path )
{
	boost::filesystem::path fullPath = path.GetString();
	
	if( !m_ado.connect( fullPath.c_str() ) )
	{
		std::string msg = "入力ファイルオープンエラー： " + CStringA(path);
		throw std::runtime_error( msg );
	}
	
	// ここでフルパス指定すると日本語フォルダを含むパスだと開けなくなるのでファイル名だけ。
	// なら↑ではこのフォルダまででいいの？と思ったがそういうわけでもないらしい。。
	CStringA query = "SELECT * FROM " + CStringA(fullPath.filename().c_str());
	m_adoCur = m_ado.search( query );
	if( S_OK != m_adoCur.err() )
	{
		std::string msg = "入力ファイル読み込みエラー： " + CStringA(m_adoCur.GetErrorMessage());
		throw std::runtime_error( msg );
	}

}

// 更新対象のOID取得
std::vector< long > CsvLoader::GetUpdateIDs() const
{
	std::vector< long > ids;
	for( const auto& row : m_records )
	{
		const RecordInfo& ri = row.second;

		if( ri.isCreate )
			continue;

		ids.push_back( ri.oid );
	}

	return ids;
}


std::vector< CsvLoader::CsvRecords > CsvLoader::GetNewRecordsList() const
{
	RUNLOGGER.Info( _T("新規作成リスト取得... "), true );
	long count = 0;
	boost::timer t;

	std::vector< CsvRecords > recList;
	CsvRecords records;
	for( const auto& row : m_records )
	{
		const RecordInfo& ri = row.second;

		if( !ri.isCreate )
			continue;

		records.push_back( ri );
		if( records.size() >= 10000 )
		{
			recList.push_back( records );
			records.clear();

			std::cerr << count << "records. " << t.elapsed() << "[s]" <<std::endl;
			t.restart();
		}

		++count;
	}
	
	recList.push_back( records );
	RUNLOGGER.Info( _T("OK.\n"), true );
	return recList;
}

//////////////////////
// 非メンバ関数の実装
//////////////////////


bool ValidateData( int lineNum, 
				   CComVariant& val,             // ChangeType()で型が変わる可能性がある
				   const FieldInfo& fi, 
				   const CFieldMap& fm, 
				   std::vector<CString>& errs )
{
	using namespace uh::variant_util;

	// 値が指定されていなければノーチェック
	if( isNullOrEmpty(val) )
		return true;

	// エラーメッセージ書き出し用
	auto setErrMsg = [&]( const CString& msg ){
		using namespace uh::str_util;
		// <行数> <フィールド名> <値> <内容>
		CString strLine = uh::str_util::format( _T("Line:%ld"), lineNum );
		val.ChangeType( VT_BSTR );
		std::list<CString> err = boost::assign::list_of
			(strLine)(fi.fieldName)(val)(msg);

		errs.push_back( uh::str_util::join( err, _T("\t") ) );
	};

	// LAT,LONフィールドはSHAPEに読み替え
	CFieldMap::_FIELD fieldAttr = {};
	if( fi.isLatLonField() )
	{
		if( fm.GetShapeIndex() < 0 )
		{
			setErrMsg( _T("対象テーブルに形状フィールドが存在しません") );
			return false;
		}
		fieldAttr = fm.GetFieldAttr( fm.GetShapeIndex() );
	}
	else
	{
		fieldAttr = fm.GetFieldAttr( fi.destIndex );
	}


	// 型チェック
	{
		static const std::map< esriFieldType, VARTYPE > typeChecker = boost::assign::map_list_of
			(esriFieldTypeOID, VT_I4)
			(esriFieldTypeInteger, VT_I4)
			(esriFieldTypeDouble, VT_R8)
			(esriFieldTypeString, VT_BSTR)
			(esriFieldTypeDate, VT_DATE)
			(esriFieldTypeGeometry, VT_R8);  // Geometryはポイント（LAT,LON）のみサポート

		auto it = typeChecker.find( fieldAttr.type );
		if( it == typeChecker.end() )
		{
			setErrMsg( _T("未サポートのデータタイプです") );
		}

		// TODO: ADOでCSVを扱う場合、最初の数行からフィールドのデータ型が推測され、
		//       合わないものはフェッチの時点でVT_NULLにされるので、このチェックはあまり意味ないかも
		if( FAILED( val.ChangeType(it->second) ) )
		{
			setErrMsg( uh::str_util::format( 
				_T("不正なデータ型です（%s）"), toTypeString(val) ) );
		}
	}
	
	// 型ごとの詳細チェック
	switch( fieldAttr.type )
	{
	case esriFieldTypeDouble:
	case esriFieldTypeInteger:
		{
			// 全桁数
			long precision = CStringW(uh::str_util::ToString(val)).GetLength();
			if( fieldAttr.precision < precision )
			{
				setErrMsg( uh::str_util::format( 
					_T("桁数がオーバーしています。[%ld]（制限：%ld）"), precision, fieldAttr.precision ) );
			}
		}
		break;
	case esriFieldTypeString:
		{
			// 長さ
			long length = CStringW(uh::str_util::ToString(val)).GetLength();
			if( fieldAttr.length < length )
			{
				setErrMsg( uh::str_util::format( 
					_T("文字列数がオーバーしています。[%ld]（制限：%ld）"), length, fieldAttr.length ) );
			}
		}
		break;
	default:
		break;
	}

	// NULL許可チェック
	if( !fieldAttr.nullable && 
		uh::str_util::ToString(val) == NULL_VALUE )
	{
		setErrMsg( _T("当該フィールドはnull不許可です。") );
	}

	// コード値チェック
	if( !fieldAttr.rdomain.empty() && 
		fieldAttr.rdomain.find(val.lVal) == fieldAttr.rdomain.end() )
	{
		setErrMsg( _T("存在しないコード値です。") );
	}

	return errs.empty();
}
