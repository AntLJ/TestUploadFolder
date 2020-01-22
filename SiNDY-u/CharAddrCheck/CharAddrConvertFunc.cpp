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
#include "CharAddrConvertFunc.h"

#include <sindy/workspace.h> 
#include <sindy/schema.h>

#include "ADOBase.h"
#include <TDC/common_check/NameYomiChk.h>

using namespace boost::spirit;
using namespace jis::x0208::spirit;

std::multimap<long,CString> CCharAddrConvert::Init( LPCTSTR lpcszCovnertTablePath )
{
	// エラー出力を突っ込む
	std::multimap<long,CString> mapError;

	ITablePtr ipKanjiYomiTable, ipObjectYomiTable;
	CADOCursor cKanjiYomiTable;
	using namespace sindy;
	using namespace sindy::schema;
	if( !CString(lpcszCovnertTablePath).IsEmpty() )
	{
		INamePtr ipName(create_workspace_name(lpcszCovnertTablePath));
		if( ipName )
		{
			IWorkspacePtr ipConvertWorkspace(ipName->_Open());
			if( ipConvertWorkspace )
			{
				CString strConvertDB;
				strConvertDB.Format( _T("Provider=Microsoft.JET.OLEDB.4.0;Data source = %s"), lpcszCovnertTablePath );
				_ConnectionPtr ipConnection(__uuidof(Connection));
				if( ipConnection )
				{
					ipConnection->Open(_bstr_t(strConvertDB), "", "", adConnectUnspecified);
					// 読み変換テーブルを取得する
					IFeatureWorkspacePtr ipConvertFWorkspace(ipConvertWorkspace);
					CADOBase ipConvertDB(ipConnection);
					cKanjiYomiTable = ipConvertDB.search( _T("select * from KanjiToYomi") );
					ipConvertFWorkspace->OpenTable(CComBSTR(_T("ObjectToYomi")), &ipObjectYomiTable);
					//if( cKanjiYomiTable.begin() == cKanjiYomiTable.end() ) // これは正しい判別ではないが、NULLってたら読込めなかったと判断
					//	mapError.insert( std::pair<long,CString>( 1, _T("テーブルオープン失敗\t漢字読み変換テーブル") ) );
					if( !ipObjectYomiTable )
						mapError.insert( std::pair<long,CString>( 2, _T("テーブルオープン失敗\tオブジェクトID読み変換テーブル") ) );
				}
				else
					mapError.insert( std::pair<long,CString>( 1, _T("PGDB接続失敗") ) );
			}
			else
				mapError.insert( std::pair<long,CString>( 1, _T("変換テーブルのIWorkspaceをオープンできません。") ) );
		}
		else
			mapError.insert( std::pair<long,CString>( 1, _T("PGDB接続失敗") ) );
	}
	else
		mapError.insert( std::pair<long,CString>( 1, _T("テーブル名がNULLってます。") ) );

	if( mapError.empty() )
	{
		m_cSubConvert.Init();
		// オブジェクトID読み変換テーブルのレコードリストを取得
		m_mapObjectYomiTable = GetObjectYomiTableItems( ipObjectYomiTable, mapError );
		
		// 漢字読み変換テーブルのレコードリストを取得
		m_mapKanjiYomiTable = GetKanjiYomiTableItems( cKanjiYomiTable, mapError );
		
		if( mapError.empty() )
		{
			// サブテーブルのレコードリストを取得する（アルファベット読み）
			m_mapSubTable = m_cSubConvert.GetSubtable();
		}
	}
	return mapError;
}

std::map<ObjectLayerPair,KanjiYomiPair> CCharAddrConvert::GetObjectYomiTableItems( ITable* ipTable, std::multimap<long,CString>& mapError )
{
	std::map<ObjectLayerPair,KanjiYomiPair> mapRet;

	// オブジェクトID読みテーブルのレコードリストを取得する
	IQueryFilterPtr ipQFilter(CLSID_QueryFilter);
	_ICursorPtr ipCursor;
	if( S_OK == ipTable->Search( ipQFilter, VARIANT_FALSE, &ipCursor ) && ipCursor )
	{
		_IRowPtr ipRow;
		long lCount = 0;
		while( S_OK == ipCursor->NextRow( &ipRow ) && ipRow )
		{
			lCount++;
			CComVariant vaOID = ipRow->GetValue( ipTable->_FindField( _bstr_t(_T("OBJECTID")) ) );
			CComVariant vaLayer = ipRow->GetValue( ipTable->_FindField( _bstr_t(_T("LAYER")) ) );
			CComVariant vaKanji = ipRow->GetValue( ipTable->_FindField( _bstr_t(_T("KANJI")) ) );
			CComVariant vaYomi = ipRow->GetValue( ipTable->_FindField( _bstr_t(_T("YOMI")) ) );
			long lLayerValue = vaLayer.lVal;
			if( lLayerValue == 1 || lLayerValue == 2 )
			{
				CString strKanji = vaKanji.bstrVal;
				CString strYomi = vaYomi.bstrVal;
				if( !strKanji.IsEmpty() && !strYomi.IsEmpty() )
				{
					// 値が正しいレコードをリストに突っ込む
					std::string lpwKanji = CT2A(strKanji);
					std::string lpwYomi = CT2A(strYomi);
					// [bug6542]修正
					bool bName = IsCheckYomi( lpwKanji, ObjectNameType );
					bool bYomi = IsCheckYomi( lpwYomi, ObjectYomiType );

					if( !bName )
						mapError.insert( std::pair<long,CString>( vaOID.lVal, _T("オブジェクトID読みテーブル\t漢字不正\t") + strKanji ) );
					if( !bYomi )
						mapError.insert( std::pair<long,CString>( vaOID.lVal, _T("オブジェクトID読みテーブル\t読み不正\t") + strYomi ) );

					if( bName && bYomi )
					{
						// [bug7333]オブジェクトID読みテーブルの漢字と読みが一致しているかチェック
						if(IsCheckKanjiYomi(strKanji, strYomi, vaOID.lVal, mapError))
						{
							if( lLayerValue == 1 )
								mapRet.insert( std::pair<ObjectLayerPair,KanjiYomiPair>( ObjectLayerPair( CityAdmin_Layer, vaOID.lVal ), KanjiYomiPair( lpwKanji, lpwYomi ) ) );
							else if( lLayerValue == 2 )
								mapRet.insert( std::pair<ObjectLayerPair,KanjiYomiPair>( ObjectLayerPair( GouPoint_Layer, vaOID.lVal ), KanjiYomiPair( lpwKanji, lpwYomi ) ) );
						}
					}
				}
				else
				{
					// 値が正しくないものはエラー出力リストに突っ込む
					if( strKanji.IsEmpty() && !strYomi.IsEmpty() )
						mapError.insert( std::pair<long,CString>( vaOID.lVal, _T("オブジェクトID読みテーブル\t漢字がNULL\t") + strYomi ) );
					else if( !strKanji.IsEmpty() && strYomi.IsEmpty() )
						mapError.insert( std::pair<long,CString>( vaOID.lVal, _T("オブジェクトID読みテーブル\t読みがNULL\t") + strKanji ) );
					else
						mapError.insert( std::pair<long,CString>( vaOID.lVal, _T("オブジェクトID読みテーブル\tどちらもNULL") ) );
				}
			}
			else
				mapError.insert( std::pair<long,CString>( vaOID.lVal, _T("LAYER値に0,1以外が設定されている") ) );
		}
	}
	return mapRet;
}

std::map<std::string,std::string> CCharAddrConvert::GetKanjiYomiTableItems( CADOCursor& cKanjiYomiCursor, std::multimap<long,CString>& mapError )
{
	std::map<std::string,std::string> mapRet;

	// 漢字読みテーブルのレコードリストを取得する
	long lCount = 0;
	std::vector<CADOCursor::Field> vecField = cKanjiYomiCursor.fields();
	for( CADOCursor::iterator it = cKanjiYomiCursor.begin(); it != cKanjiYomiCursor.end(); ++it )
	{
		lCount++;
		std::vector<CComVariant> vecValue = it.dereference();
		std::vector<CComVariant>::iterator itVal = vecValue.begin();
		CString strKanji, strYomi;
		for( std::vector<CADOCursor::Field>::iterator itField = vecField.begin(); itField != vecField.end(); ++itField )
		{
			CString strFieldName = itField->name;
			if( _tcsicmp( strFieldName, _T("KANJI") ) == 0 )
				strKanji = *itVal;
			else if( _tcsicmp( strFieldName, _T("YOMI") ) == 0 )
				strYomi = *itVal;
			itVal++;
		}
		if( !strKanji.IsEmpty() && !strYomi.IsEmpty() )
		{
			// 値が正しいレコードをリストに突っ込む
			std::string lpwKanji = CT2A(strKanji);
			std::string lpwYomi = CT2A(strYomi);
			// [bug6542]修正
			bool bName = IsCheckYomi( lpwKanji, KanjiNameType );
			bool bYomi = IsCheckYomi( lpwYomi, KanjiYomiType );
			if( !bName )
				mapError.insert( std::pair<long,CString>( lCount, _T("漢字読みテーブル\t漢字不正\t") + strKanji ) );
			if( !bYomi )
				mapError.insert( std::pair<long,CString>( lCount, _T("漢字読みテーブル\t読み不正\t") + strYomi ) );

			if( bName && bYomi )
				mapRet.insert( std::pair<std::string,std::string>( lpwKanji, lpwYomi ) );
		}
		else
		{
			// 値が正しくないものはエラー出力リストに突っ込む
			if( strKanji.IsEmpty() && !strYomi.IsEmpty() )
				mapError.insert( std::pair<long,CString>( lCount, _T("漢字読みテーブル\t漢字がNULL\t") + strYomi ) );
			else if( !strKanji.IsEmpty() && strYomi.IsEmpty() )
				mapError.insert( std::pair<long,CString>( lCount, _T("漢字読みテーブル\t読みがNULL\t") + strKanji ) );
			else
				mapError.insert( std::pair<long,CString>( lCount, _T("漢字読みテーブル\tどちらもNULL") ) );
		}
	}
	return mapRet;
}

std::string CCharAddrConvert::GetYomiFromCharAddr_Object( GouOrCityAddrLayer LayerType, long lOID, LPCTSTR lpcszCharAddr, bool& bRet )
{
	std::string strRet;

	// 指定オブジェクトがオブジェクトID読みテーブルにあるかチェック
	std::map<ObjectLayerPair,KanjiYomiPair>::const_iterator it = m_mapObjectYomiTable.find( ObjectLayerPair( LayerType, lOID ) );
	if( it != m_mapObjectYomiTable.end() )
	{
		// オブジェクトID読みテーブルにあってもレコード内の名称が文字付き住所名称と違う場合があるのでここでチェック
		// 例）テーブルには[OID:123456, Layer:gou_point, Name:高橋]と入っているが、実際にチェックしたいOID:123456の号ポイントからは「高橋１２３」という名称である場合。
		//      本来ならば[OID:123456, Layer:gou_point, Name:高橋１２３]となっていなければならない。
		//      おそらく作業者が間違えて入力してしまったのが原因。
		std::string strTemp = CT2A(lpcszCharAddr);
		if( it->second.first.compare( strTemp ) == 0 )
		{
			strRet = it->second.second;
			bRet = true;
		}
		else
			bRet = false;
	}
	else
		bRet = false;

	return strRet;
}

std::string CCharAddrConvert::GetYomiFromCharAddr_Kanji( LPCTSTR lpcszCharAddr, bool& bRet )
{
	std::string strRet;

	// 文字付き住所の文字列を単語毎に分割してリストにする
	std::list<std::pair<YomiType,std::string>> listRet = GetCharAddrList( lpcszCharAddr );

	// リスト化したものを漢字読み変換する
	strRet = GetYomi( listRet, bRet );

	return strRet;
}

std::list<std::pair<YomiType,std::string>> CCharAddrConvert::GetCharAddrList( LPCTSTR lpcszCharAddr, const sjis_ranges& sjisSeparate )
{
	std::list<std::pair<YomiType,std::string>> listCharAddr;
	std::vector<std::string> vecCharAddr;

	std::string strCharAddr = CT2A(lpcszCharAddr);

	// 文字列を漢字かなカナ、英字、数字に分解する処理
	// 便利なライブラリ（LIB/jis）があったので使用。。。
	parse_info<> aInfo = 
		parse( strCharAddr.c_str(), *( 
			(+m_cSubConvert.getKanjiKana())[push_back_a(vecCharAddr)] |
			(+m_cSubConvert.getNumber())[push_back_a(vecCharAddr)] |
			(+m_cSubConvert.getAlphabet())[push_back_a(vecCharAddr)] |
			(+sjisSeparate)[push_back_a(vecCharAddr)] |
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
			// かなカナのみかそれ以外か
			if( aInfoKana.full )
				listCharAddr.push_back( std::pair<YomiType,std::string>( HiraKataType, *it ) );
			else
				listCharAddr.push_back( std::pair<YomiType,std::string>( KanjiYomiTableType, *it ) );
		}
		// 数字だけかチェック
		else if( aInfoNumber.full )
			listCharAddr.push_back( std::pair<YomiType,std::string>( NumberType, *it ) );
		// アルファベットだけかチェック
		else if( aInfoAlphabet.full )
		{
			std::wstring strWStr = CA2W(it->c_str());
			// アルファベットも3文字以上は漢字読み変換の対象となる
			if( strWStr.size() > 2 )
				listCharAddr.push_back( std::pair<YomiType,std::string>( KanjiYomiTableType, *it ) );
			else
				listCharAddr.push_back( std::pair<YomiType,std::string>( AlphabetType, *it ) );
		}
		// 上記以外
		else
			listCharAddr.push_back( std::pair<YomiType,std::string>( NoneType, *it ) );
	}

	return listCharAddr;
}

std::list<std::pair<YomiType,std::string>> CCharAddrConvert::GetCharAddrList2( LPCTSTR lpcszCharAddr, const sjis_ranges& sjisSeparate )
{
	std::list<std::pair<YomiType,std::string>> listCharAddr;
	std::vector<std::string> vecCharAddr;

	std::string strCharAddr = CT2A(lpcszCharAddr);

	// 文字列を漢字かなカナ英字、数字に分解する処理
	// 便利なライブラリ（LIB/jis）があったので使用。。。
	parse_info<> aInfo = 
		parse( strCharAddr.c_str(), *( 
			(+m_cSubConvert.getKanjiKana().add(m_cSubConvert.getAlphabet()))[push_back_a(vecCharAddr)] |
			(+m_cSubConvert.getNumber())[push_back_a(vecCharAddr)] |
			(+sjisSeparate)[push_back_a(vecCharAddr)] |
			+anychar_p ) );

	// 分解した文字列を更に「かなカナ、アルファベット、数字、漢字読み」に分ける
	for( std::vector<std::string>::const_iterator it = vecCharAddr.begin(); it != vecCharAddr.end(); ++it )
	{
		// 漢字かなカナ英字か
		parse_info<> aInfoKanjiKanaAlpha = parse( it->c_str(), *(m_cSubConvert.getKanjiKana().add(m_cSubConvert.getAlphabet())) );
		// 数字か
		parse_info<> aInfoNumber = parse( it->c_str(), *(m_cSubConvert.getNumber()) );

		// 英字か
		parse_info<> aInfoAlphabet = parse( it->c_str(), *(m_cSubConvert.getAlphabet()) );

		// 漢字とかなカナ英字かチェック
		if( aInfoKanjiKanaAlpha.full )
		{
			parse_info<> aInfoKana = parse( it->c_str(), *(m_cSubConvert.getKana()) );
			// かなカナのみかそれ以外か
			if( aInfoKana.full )
				listCharAddr.push_back( std::pair<YomiType,std::string>( HiraKataType, *it ) );
			else
			{
				// 英字だけか
				if(aInfoAlphabet.full)
				{
					std::wstring strWStr = CA2W(it->c_str());
					// アルファベットも3文字以上は漢字読み変換の対象となる
					if( strWStr.size() >= 3 )
						listCharAddr.push_back( std::pair<YomiType,std::string>( KanjiYomiTableType, *it ) );
					else
						listCharAddr.push_back( std::pair<YomiType,std::string>( AlphabetType, *it ) );
				}
				else
					listCharAddr.push_back( std::pair<YomiType,std::string>( KanjiYomiTableType, *it ) );
			}
		}
		// 数字だけかチェック
		else if( aInfoNumber.full )
			listCharAddr.push_back( std::pair<YomiType,std::string>( NumberType, *it ) );
		// 上記以外
		else
			listCharAddr.push_back( std::pair<YomiType,std::string>( NoneType, *it ) );
	}

	return listCharAddr;
}

std::string CCharAddrConvert::GetYomiFromKanjiYomiTable( const std::string& strCharAddr, bool& bRet )
{
	std::string strRet;
	// 漢字読みテーブルから読みを取得
	std::map<std::string,std::string>::const_iterator it = m_mapKanjiYomiTable.find( strCharAddr );
	if( it == m_mapKanjiYomiTable.end() )
	{
		strRet = strCharAddr;
		bRet = false;
	}
	else
		strRet = it->second;
	return strRet;
}

std::string CCharAddrConvert::GetYomiFromSubTable( const std::wstring& strCharAddr, bool& bRet )
{
	std::string strRet;
	// サブテーブルから読みを取得
	std::map<std::wstring,std::wstring>::const_iterator it = m_mapSubTable.find( strCharAddr );
	if( it == m_mapSubTable.end() )
	{
		strRet = CW2A(strCharAddr.c_str());
		bRet = false;
	}
	else
		strRet = CW2A(it->second.c_str());
	return strRet;
}

std::string CCharAddrConvert::GetYomi( const std::list<std::pair<YomiType,std::string>>& listCharAddr, bool& bRet )
{
	// リストにはこんな感じで入ってます
	// 例)元データ　　　　　：ABあいう-12C高橋EFG34
	// 　 元データ（リスト）：AB(アルファベット),あいう(かなカナ),-(なし),12(数字),C(アルファベット),高橋(漢字読み),EFG(漢字読み),34(数字)
	// 　 出力データ　　　　：エービーアイウ－１２シータカハシイーエフジー３４
	std::string strRet;
	bool bRetYomi = true;
	for( std::list<std::pair<YomiType,std::string>>::const_iterator it = listCharAddr.begin(); it != listCharAddr.end(); ++it )
	{
		switch( it->first )
		{
		// 数字
		case NumberType:
			{
				// 全部数字
				// 数字部分を全角に変換
				strRet.append(ConvertHanNumToZenNum(it->second));
			}
			break;
		// アルファベット
		case AlphabetType:
			{
				std::wstring strWStr = CA2W(it->second.c_str());
				// アルファベット2文字まではサブテーブルで読みを付与
				for( int i = 0; i < int(strWStr.size()); ++i )
					strRet.append( GetYomiFromSubTable( strWStr.substr( i, i+1 ), bRetYomi ) );
				break;
			}
		// かなカナ
		case HiraKataType:
			{
				// 全部かなカナ
				// かなをカナへ変換
				strRet.append(ConvertHiraToKata(it->second));
			}
			break;
		// 漢字読みテーブル
		case KanjiYomiTableType:
			strRet.append( GetYomiFromKanjiYomiTable( it->second, bRetYomi ) );
			break;
		// 該当なし
		case NoneType:
			if( it->second == "-" )
				strRet.append( "－" );
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

bool CCharAddrConvert::IsCheckYomi( const std::string& strStr, CharAddrType nType )
{
	bool bRet = false;

	switch( nType )
	{
	case ObjectNameType:
		// まずは使用OKの文字だけを使っているかチェック
		// sjis第一、第二水準漢字＋全角かなカナ＋全角英字＋半角数字＋[ー－-々]
		if(parse( strStr.c_str(), *(m_cSubConvert.getObjectName()) ).full)
		{
			// 単語毎に分割処理
			std::list<std::pair<YomiType,std::string>> listStr = GetCharAddrList( strStr.c_str() );
			// 最初と最後が「-」だったらNG
			if(_tcscmp(CA2T(listStr.begin()->second.c_str()), _T("-")) == 0 || _tcscmp(CA2T(listStr.rbegin()->second.c_str()), _T("-")) == 0)
				break;

			bool bKanji = false, bOther = false;
			for(std::list<std::pair<YomiType,std::string>>::iterator it = listStr.begin(); it != listStr.end(); ++it)
			{
				switch(it->first)
				{
				case AlphabetType:
					if(std::wstring(CA2W(it->second.c_str())).length() >= 3)	// 3文字以上なら漢字と同じ扱い
						bKanji = true;
					break;
				case KanjiYomiTableType:
					bKanji = true;
					break;
				case NoneType:
					if(_tcscmp(it->second.c_str(), _T("-")) != 0)	// 「-」でない場合はNG
						bOther = true;
				case NumberType:
				case HiraKataType:
					break;
				default:
					bOther = true;
					break;
				}
			}
			// 漢字を使っていればOK、ひらカタ数字アルファベット「-」のみはNG
			if(!bOther && bKanji)
				bRet = true;
		}
		break;
	case ObjectYomiType:
		// まずは使用OKの文字だけを使っているかチェック
		// 全角数字カナ（ヰヱ以外）＋[ー－]
		if(parse( strStr.c_str(), *(m_cSubConvert.getObjectYomi()) ).full)
		{
			// 単語毎に分割処理
			std::list<std::pair<YomiType,std::string>> listStr = GetCharAddrList( strStr.c_str(), sjis_ranges().add(sjis_range(0x817C,0x817C)) );
			// 最初と最後が「－」だったらNG
			if(_tcscmp(listStr.begin()->second.c_str(), _T("－")) == 0 || _tcscmp(listStr.rbegin()->second.c_str(), _T("－")) == 0)
				break;

			bool bHiraKata = false, bOther = false;
			for(std::list<std::pair<YomiType,std::string>>::iterator it = listStr.begin(); it != listStr.end(); ++it)
			{
				switch(it->first)
				{
				case NoneType:
					if(_tcscmp(it->second.c_str(), _T("－")) != 0)	// 「－」でない場合はNG
						bOther = true;
					break;
				case NumberType:
					break;
				case HiraKataType:
					// カタカナ部分の並びが適正なときのみ、カタカナを使っていると判定する
					if( CheckFirstLargeKana( it->second.c_str() ) && CheckYomiCharacter( it->second.c_str() ) ){
						bHiraKata = true;
					}
					break;
				default:
					bOther = true;
					break;
				}
			}
			// カタカナを使っていればOK、数字「－」のみはNG
			if(!bOther && bHiraKata)
				bRet = true;
		}
		break;
	case KanjiNameType:
		// まずは使用OKの文字だけを使っているかチェック
		// sjis第一、第二水準漢字＋全角英字＋全角かなカナ＋[ー々]
		if( parse( strStr.c_str(), *(m_cSubConvert.getKanjiName()) ).full )
		{
			// 単語毎に分割処理
			std::list<std::pair<YomiType,std::string>> listStr = GetCharAddrList( strStr.c_str() );
			// 分割できるようならNG
			if(listStr.size() == 1)
			{
				bool bKanji = false, bOther = false;
				for(std::list<std::pair<YomiType,std::string>>::iterator it = listStr.begin(); it != listStr.end(); ++it)
				{
					switch(it->first)
					{
					case AlphabetType:
						if(std::wstring(CA2W(it->second.c_str())).length() >= 3)	// 3文字以上なら漢字と同じ扱い
							bKanji = true;
						break;
					case KanjiYomiTableType:
						bKanji = true;
						break;
					default:
						bOther = true;
						break;
					}
				}
				// カタカナを使っていればOK、数字のみはNG
				if(!bOther && bKanji)
					bRet = true;
			}
		}
		break;
	case KanjiYomiType:
		// １文字目が適正な大文字カタカナであり、全体が適正なカナであるかチェック (Bug 9249)
		bRet = ( CheckFirstLargeKana( strStr.c_str() ) && CheckYomiCharacter( strStr.c_str() ) );
		break;
	}

	return bRet;
}

bool CCharAddrConvert::IsCheckKanjiYomi(LPCTSTR lpcszKanji, LPCTSTR lpcszYomi, long lOID, std::multimap<long,CString>& mapError)
{
	bool bRet = true;

	// GetCharAddrList()だと漢字かなカナ、英字、数字、「-」で分かれるので
	// GetCharAddrList2()を指定。これで漢字かなカナ英字、数字、「-」で分かれる
	std::list<std::pair<YomiType,std::string>> listKanji = GetCharAddrList2(lpcszKanji);
	std::list<std::pair<YomiType,std::string>> listYomi = GetCharAddrList2(lpcszYomi, sjis_ranges().add(sjis_range(0x817C,0x817C)));

	// 分割数が違ったらエラー
	if(listKanji.size() == listYomi.size())	// listのsize()は余り使いたくないけど、絶対数が少ないから気にしない
	{
		std::list<std::pair<YomiType,std::string>>::const_iterator itYomi = listYomi.begin();
		for(std::list<std::pair<YomiType,std::string>>::const_iterator itKanji = listKanji.begin(); itKanji != listKanji.end(); ++itKanji)
		{
			// [漢字:漢字<->読み:ひらカタ] [漢字:アルファベット<->読み:ひらカタ]
			// 上記の組合せの場合はスルー
			if((itKanji->first == KanjiYomiTableType && itYomi->first == HiraKataType) ||
				(itKanji->first == AlphabetType && itYomi->first == HiraKataType))
			{
			}
			// ひらカタ同士なら漢字をカタカタに変換して同じかチェックする
			else if(itKanji->first == HiraKataType && itYomi->first == HiraKataType)
			{
				std::string strKanjiKata = ConvertHiraToKata(itKanji->second);
				if(_tcscmp(strKanjiKata.c_str(), itYomi->second.c_str()) != 0)
				{
					mapError.insert( std::pair<long,CString>( lOID, _T("オブジェクトID読みテーブル\t漢字と読みが不一致\t") + CString(lpcszKanji) + _T("\t") + CString(lpcszYomi) ) );
					bRet = false;
					break;
				}
			}
			// 数字同士の組合せの場合をチェック
			else if(itKanji->first == NumberType && itYomi->first == NumberType)
			{
				// 同じだったらダメでしょう（漢字->数字は半角：読み->数字は全角）
				// この処理の前でチェックしているのでここで引っかかることはないと思うけど、、、
				if( _tcscmp(itKanji->second.c_str(), itYomi->second.c_str()) != 0)
				{
					// 漢字->数字を全角に変換
					CString strKanji(ConvertHanNumToZenNum(itKanji->second).c_str());
					// 全角数字変換したもので漢字と読みを比較、違ったらエラー
					if( _tcscmp(strKanji, itYomi->second.c_str()) != 0 )
					{
						mapError.insert( std::pair<long,CString>( lOID, _T("オブジェクトID読みテーブル\t漢字と読みが不一致\t") + CString(lpcszKanji) + _T("\t") + CString(lpcszYomi) ) );
						bRet = false;
						break;
					}
				}
				else
				{
					mapError.insert( std::pair<long,CString>( lOID, _T("オブジェクトID読みテーブル\t漢字と読みが不一致\t") + CString(lpcszKanji) + _T("\t") + CString(lpcszYomi) ) );
					bRet = false;
					break;
				}
			}
			// その他同士の組合せの場合をチェック
			else if(itKanji->first == NoneType && itYomi->first == NoneType)
			{
				// KANJIのNoneTypeは「-」のみ。「--」のようになる場合はエラー
				// YOMIのNoneTypeは「－」のみ。「－－」のようになる場合はエラー
				bool bNoneType = true;
				if(_tcscmp(itKanji->second.c_str(), _T("-")) != 0)
				{
					mapError.insert( std::pair<long,CString>( lOID, _T("オブジェクトID読みテーブル\t漢字不正\t") + CString(lpcszKanji) ) );
					bNoneType = false;
				}
				if(_tcscmp(itYomi->second.c_str(), _T("－")) != 0)
				{
					mapError.insert( std::pair<long,CString>( lOID, _T("オブジェクトID読みテーブル\t読み不正\t") + CString(lpcszYomi) ) );
					bNoneType = false;
				}
				if(!bNoneType)
					break;
			}
			// それ以外の組合せの場合はエラー
			else
			{
				mapError.insert( std::pair<long,CString>( lOID, _T("オブジェクトID読みテーブル\t漢字と読みが不一致\t") + CString(lpcszKanji) + _T("\t") + CString(lpcszYomi) ) );
				bRet = false;
				break;
			}
			itYomi++;
		}
	}
	else
	{
		mapError.insert( std::pair<long,CString>( lOID, _T("オブジェクトID読みテーブル\t漢字と読みが不一致\t") + CString(lpcszKanji) + _T("\t") + CString(lpcszYomi) ) );
		bRet = false;
	}

	return bRet;
}

std::string CCharAddrConvert::ConvertHiraToKata(std::string strHira)
{
	std::string strRet;
	int iSize = int(strHira.size())*2+1;
	TCHAR *lpzBuf = new TCHAR[iSize];
	ZeroMemory(lpzBuf, iSize);
	// 全角かなを全角カナに変換
	LCMapString( GetUserDefaultLCID(), LCMAP_KATAKANA, CA2CT(strHira.c_str()), iSize, lpzBuf, iSize );
	CString strBuf;
	strBuf.Format( _T("%s"), lpzBuf );
	delete [] lpzBuf;
	// [bug6542]修正
	strBuf.Replace(_T("ヰ"),_T("イ"));
	strBuf.Replace(_T("ヱ"),_T("エ"));
	strRet.append( CT2A(strBuf) );
	return strRet;
}

std::string CCharAddrConvert::ConvertHanNumToZenNum(std::string strHanNum)
{
	std::string strRet;
	int iSize = int(strHanNum.size())*2+1;
	TCHAR *lpzBuf = new TCHAR[iSize];
	ZeroMemory(lpzBuf, iSize);
	// 数字部分を全角に変換
	LCMapString( GetUserDefaultLCID(), LCMAP_FULLWIDTH, CA2CT(strHanNum.c_str()), iSize, lpzBuf, iSize );
	CString strBuf;
	strBuf.Format( _T("%s"), lpzBuf );
	delete [] lpzBuf;
	strRet.append( CT2A(strBuf) );
	return strRet;
}
