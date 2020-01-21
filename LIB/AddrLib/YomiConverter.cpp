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
#include "YomiConverter.h"
#include <WinLib/charutil.h>
#include <WinLib/Str2.h>

namespace addr
{
	// 変換テーブルスキーマ
	namespace yomi_schema
	{
		namespace common
		{
			const TCHAR kKanji[]	= _T("Kanji");
			const TCHAR kYomi[]	= _T("Yomi");
		}

		namespace kanji2yomi
		{
			const TCHAR kTableName[]	= _T("KanjiToYomi");
			using namespace yomi_schema::common;		
		}

		namespace object2yomi
		{
			const TCHAR kTableName[]	= _T("ObjectToYomi");
			const TCHAR kLayer[]		= _T("Layer");
			const TCHAR kObjectID[]	= _T("ObjectID");
			using namespace yomi_schema::common;
		}
	}

using namespace yomi_schema;

// 初期化
bool CYomiConverter::Init( const _TCHAR* lpcszCovnertTablePath )
{
	try
	{
		CADOCursor cKanjiYomiTable, cObjectYomiTable;

		CString strConvertDB;
		strConvertDB.Format( _T("Provider=Microsoft.JET.OLEDB.4.0;Data source = %s"), lpcszCovnertTablePath );
		_ConnectionPtr ipConnection(__uuidof(Connection));
		if( SUCCEEDED(ipConnection->Open(_bstr_t(strConvertDB), "", "", adConnectUnspecified)) )
		{
			// 2つの変換テーブルを取得する
			CADOBase ipConvertDB(ipConnection);
			CString strSQL;
			strSQL.Format( _T("select * from %s"), kanji2yomi::kTableName );
			cKanjiYomiTable = ipConvertDB.search( CT2CA(strSQL) );
			strSQL.Format( _T("select * from %s"), object2yomi::kTableName );
			cObjectYomiTable = ipConvertDB.search( CT2CA(strSQL) );

			m_cSubConvert.Init();
			SetKanjiYomiTableItems( cKanjiYomiTable );		// 漢字読み変換テーブルのレコードリストを取得
			SetObjectYomiTableItems( cObjectYomiTable );	// オブジェクトID読み変換テーブルのレコードリストを取得
			return true;
		}
		else
			return false;
	}
	catch( _com_error& e )
	{
		m_strError.Format( _T("ヨミテーブルオープン時にCOMエラー発生, %s"), COLE2T(e.Description()) );
		return false;
	}
}

// オブジェクト読み変換テーブルのレコード取得
void CYomiConverter::SetObjectYomiTableItems( CADOCursor& cObjectYomiCursor )
{
	// オブジェクトID読みテーブルのレコードリストを取得する
	long lCount = 1;
	const std::vector<CADOCursor::Field>& vecField = cObjectYomiCursor.fields();
	for( CADOCursor::iterator it = cObjectYomiCursor.begin(); it != cObjectYomiCursor.end(); ++it, ++lCount )
	{
		std::vector<CComVariant> vecValue = it.dereference();
		std::vector<CComVariant>::iterator itVal = vecValue.begin();

		// １レコード全て取得
		GouOrCityAddrLayer emLayer = None_Layer;
		long lOID = 0;
		CString strKanji, strYomi;
		for( std::vector<CADOCursor::Field>::const_iterator itField = vecField.begin(); itField != vecField.end(); ++itField )
		{
			if( itField->name.CompareNoCase(object2yomi::kLayer) == 0  )
				emLayer = (VT_I2 == itVal->vt || VT_I4 == itVal->vt && (itVal->lVal == 1 || itVal->lVal == 2) )?
						  static_cast<GouOrCityAddrLayer>(itVal->lVal) : None_Layer;
			else if( itField->name.CompareNoCase(object2yomi::kObjectID) == 0 )
				lOID = (VT_I2 == itVal->vt || VT_I4 == itVal->vt)? itVal->lVal : -1;
			else if( itField->name.CompareNoCase(object2yomi::kKanji) == 0 )
				strKanji = (VT_BSTR == itVal->vt)? CString(itVal->bstrVal) : _T("");
			else if( itField->name.CompareNoCase(object2yomi::kYomi) == 0 )
				strYomi = (VT_BSTR == itVal->vt)? CString(itVal->bstrVal) : _T("");

			++itVal;
		}

		if( emLayer != None_Layer && lOID > 0 && !strKanji.IsEmpty() && !strYomi.IsEmpty() )
		{
			// チェックは一切行わない(CharAddrCheckで実施しているので)
			m_mapObjectYomiTable.insert( Object2YomiRec( ObjectLayerPair( (CityAdmin_Layer == emLayer)? CityAdmin_Layer : GouPoint_Layer, lOID ), 
														 KanjiYomiPair( strKanji, strYomi ) ) );
		}
	}
}

// 漢字読み変換テーブルのレコード取得
void CYomiConverter::SetKanjiYomiTableItems( CADOCursor& cKanjiYomiCursor )
{
	// 漢字読みテーブルのレコードリストを取得する
	long lCount = 1;
	const std::vector<CADOCursor::Field>& vecField = cKanjiYomiCursor.fields();
	for( CADOCursor::iterator it = cKanjiYomiCursor.begin(); it != cKanjiYomiCursor.end(); ++it, ++lCount )
	{
		std::vector<CComVariant> vecValue = it.dereference();
		std::vector<CComVariant>::iterator itVal = vecValue.begin();
		CString strKanji, strYomi;
		for( std::vector<CADOCursor::Field>::const_iterator itField = vecField.begin(); itField != vecField.end(); ++itField )
		{
			if( itField->name.CompareNoCase(kanji2yomi::kKanji) == 0 )
				strKanji = SUCCEEDED(itVal->ChangeType(VT_BSTR))? CString(itVal->bstrVal) : _T("");	// KANJIフィールドは、バイナリ型で格納しているのでVT_BSTRに変換後
			else if( itField->name.CompareNoCase(kanji2yomi::kYomi) == 0 )
				strYomi = (VT_BSTR == itVal->vt)? CString(itVal->bstrVal) : _T("");
			itVal++;
		}

		if( !strKanji.IsEmpty() && !strYomi.IsEmpty() )
			m_mapKanjiYomiTable.insert( std::pair<CString, CString>( strKanji, strYomi ) );
	}
}

// 文字付き住所をオブジェクトID読み変換テーブルで変換して出力
CString CYomiConverter::GetYomiFromCharAddr_Object( GouOrCityAddrLayer LayerType, long lOID, const _TCHAR* lpcszCharAddr, bool& bRet ) const
{
	// 指定オブジェクトがオブジェクトID読みテーブルにあるかチェック
	std::map<ObjectLayerPair,KanjiYomiPair>::const_iterator it = m_mapObjectYomiTable.find( ObjectLayerPair(LayerType, lOID) );
	if( it != m_mapObjectYomiTable.end() )
	{
		// オブジェクトID読みテーブルにあってもレコード内の名称が文字付き住所名称と違う場合があるのでここでチェック
		// 例）テーブルには[OID:123456, Layer:gou_point, Name:高橋]と入っているが、実際にチェックしたいOID:123456の号ポイントからは「高橋１２３」という名称である場合。
		//      本来ならば[OID:123456, Layer:gou_point, Name:高橋１２３]となっていなければならない。
		//      おそらく作業者が間違えて入力してしまったのが原因。
		bRet = it->second.first == lpcszCharAddr;
		return bRet? it->second.second : _T("");
	}
	else
	{
		bRet = false;
		return _T("");
	}
}

// 文字付き住所を漢字読み変換テーブルで変換して出力
CString CYomiConverter::GetYomiFromCharAddr_Kanji( const _TCHAR* lpcszCharAddr, bool& bRet )
{
	// 文字付き住所の文字列を単語毎に分割してリスト化したものを漢字読みに変換
	std::list<std::pair<YomiType, CString>> listCharAddr;
	SetCharAddrList( lpcszCharAddr, listCharAddr );
	return GetYomi(listCharAddr, bRet);
}

// 文字付き住所の文字列を単語毎に分割したリストを取得
void CYomiConverter::SetCharAddrList( const _TCHAR* lpcszCharAddr, std::list<std::pair<YomiType, CString>>& rList )
{
	// 文字列を漢字かなカナ、英字、数字に分解する処理
	// 便利なライブラリ（LIB/jis）があったので使用。。。
	std::vector<std::string> vecCharAddr;
	parse_info<> aInfo = 
		parse( (const char*)(CT2A(lpcszCharAddr)), *( 
			(+m_cSubConvert.getKanjiKana())[push_back_a(vecCharAddr)] |
			(+m_cSubConvert.getNumber())[push_back_a(vecCharAddr)] |
			(+m_cSubConvert.getAlphabet())[push_back_a(vecCharAddr)] |
			(+m_cSubConvert.getHyphen())[push_back_a(vecCharAddr)] |
			+anychar_p ) );

	// 分解した文字列を更に「かなカナ、アルファベット、数字、漢字読み」に分ける
	for( std::vector<std::string>::const_iterator it = vecCharAddr.begin(); it != vecCharAddr.end(); ++it )
	{
		// 漢字かなカナか
		parse_info<> aInfoKanjiKana = parse( it->c_str(), *(m_cSubConvert.getKanjiKana()) );
		// 数字か
		parse_info<> aInfoNumber = parse( it->c_str(), *(m_cSubConvert.getNumber()) );
		// 英字か
		parse_info<> aInfoAlphabet = parse( it->c_str(), *(m_cSubConvert.getAlphabet()) );

		// 漢字とかなカナかチェック
		if( aInfoKanjiKana.full )
		{
			parse_info<> aInfoKana = parse( it->c_str(), *(m_cSubConvert.getKana()) );
			rList.push_back( std::pair<YomiType, CString>( aInfoKana.full? HiraKataType : KanjiYomiTableType, CString(it->c_str()) ) );	// かなカナのみかそれ以外か
		}
		// 数字だけかチェック
		else if( aInfoNumber.full )
			rList.push_back( std::pair<YomiType, CString>( NumberType, CString(it->c_str()) ) );
		// アルファベットだけかチェック
		else if( aInfoAlphabet.full )
		{
			CString str(it->c_str());
			rList.push_back( std::pair<YomiType, CString>( CString_GetLength2(str) > 2? KanjiYomiTableType : AlphabetType, str ) );	// アルファベットも3文字以上は漢字読み変換の対象となる
		}
		// 上記以外
		else
			rList.push_back( std::pair<YomiType, CString>( NoneType, CString(it->c_str()) ) );
	}
}

CString CYomiConverter::GetYomiFromKanjiYomiTable( const CString& strCharAddr, bool& bRet ) const
{
	// 漢字読みテーブルから読みを取得
	std::map<CString, CString>::const_iterator it = m_mapKanjiYomiTable.find( strCharAddr );
	if( it == m_mapKanjiYomiTable.end() )
	{
		bRet = false;
		return strCharAddr;
	}
	else
		return it->second;
}

// サブテーブルから読みを取得
CString CYomiConverter::GetYomiFromSubTable( const std::wstring& strCharAddr, bool& bRet ) const
{
	// サブテーブルから読みを取得
	std::map<std::wstring, std::wstring>::const_iterator it = m_cSubConvert.GetSubtable().find( strCharAddr );
	if( it == m_cSubConvert.GetSubtable().end() )
	{
		bRet = false;
		return CString(strCharAddr.c_str());
	}
	else
		return CString(it->second.c_str());
}

// 文字付き住所を漢字読み変換テーブルで変換して出力
CString CYomiConverter::GetYomi( const std::list<std::pair<YomiType, CString>>& listCharAddr, bool& bRet ) const
{
	// リストにはこんな感じで入ってます
	// 例)元データ　　　　　：ABあいう-12C高橋EFG34
	// 　 元データ（リスト）：AB(アルファベット),あいう(かなカナ),-(なし),12(数字),C(アルファベット),高橋(漢字読み),EFG(漢字読み),34(数字)
	// 　 出力データ　　　　：エービーアイウ－１２シータカハシイーエフジー３４
	CString strRet;
	bool bRetYomi = true;
	for( std::list<std::pair<YomiType, CString>>::const_iterator it = listCharAddr.begin(); it != listCharAddr.end(); ++it )
	{
		switch( it->first )
		{
		// 数字
		case NumberType:
			{
				// 全部数字
#ifdef _UNICODE
				long lSize = it->second.GetLength()*2+1;
#else	// MBCS
				long lSize = it->second.GetLength()+1;
#endif
				TCHAR *lpzBuf = new TCHAR[lSize];
				ZeroMemory(lpzBuf, lSize);
				// 数字部分を全角に変換
				LCMapString( GetUserDefaultLCID(), LCMAP_FULLWIDTH, it->second, lSize, lpzBuf, lSize );
				strRet.Format( _T("%s"), lpzBuf );
				delete[] lpzBuf;
			}
			break;
		// アルファベット
		case AlphabetType:
			{
				// アルファベット2文字まではサブテーブルで読みを付与
				int nLen = CString_GetLength2( it->second );
				for( int i = 0; i < nLen; ++i )
					strRet+= GetYomiFromSubTable( std::wstring(CT2CW(CString_GetAt2(it->second, i))), bRetYomi );
				break;
			}
		// かなカナ
		case HiraKataType:
			{
				// 全部数字
#ifdef _UNICODE
				long lSize = it->second.GetLength()*2+1;
#else	// MBCS
				long lSize = it->second.GetLength()+1;
#endif
				TCHAR *lpzBuf = new TCHAR[lSize];
				ZeroMemory(lpzBuf, lSize);
				// 全角かなを全角カナに変換
				LCMapString( GetUserDefaultLCID(), LCMAP_KATAKANA, it->second, lSize, lpzBuf, lSize );
				strRet.Format( _T("%s"), lpzBuf );
				delete[] lpzBuf;
				// [bug6542]修正
				strRet.Replace(_T("ヰ"),_T("イ"));
				strRet.Replace(_T("ヱ"),_T("エ"));
			}
			break;
		// 漢字読みテーブル
		case KanjiYomiTableType:
			strRet += GetYomiFromKanjiYomiTable( it->second, bRetYomi );
			break;
		// 該当なし
		case NoneType:
			if( it->second == _T("-") )
				strRet += _T("－");
			else
				bRetYomi = false;
			break;
		default:
			bRetYomi = false;
			break;
		}
	}
	bRet = bRetYomi;

	return strRet;
}

}	// namespace addr
