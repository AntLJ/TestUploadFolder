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
#include "AddrDataCreator.h"
#include "Utility.h"
#include "Logger.h"
#include "GsSchema.h"

#include <sindy/workspace.h>
#include <TDC/sindylib_core/FieldMap.h>
#include <TDC/useful_headers/str_util.h>
#include <TDC/useful_headers/text_util.h>
#include <sindy/workspace.h>
#include <WinLib/Str2.h>

using namespace std;
using namespace sindy;
using namespace sindy::schema;
using namespace Utility;
using namespace gs::schema;

AddrDataCreator::AddrDataCreator(const Arguments& arg) : BaseDataCreator(arg) {}

bool AddrDataCreator::init()
{
	bool ret = true;

	if( !BaseDataCreator::initBase() )
		return false;

	// 実行ログへオプション情報を書込み
	RUNLOGGER.Log(uh::str_util::format(_T("-%s\t%s\n"), 
		(CString)arg::k_imp_type, m_args.m_impTypeStr), false);
	RUNLOGGER.Log(uh::str_util::format(_T("-%s\t%s\n"),
		(CString)arg::k_edt_addr_db, m_args.m_edtAddrDb), false);
	RUNLOGGER.Log(uh::str_util::format(_T("-%s\t%s\n"), 
		(CString)arg::k_gs_addr_db, m_args.m_gsAddrDb), false);
	RUNLOGGER.Log(uh::str_util::format(_T("-%s\t%s\n"),
		(CString)arg::k_run_log, m_args.m_runLog), false);
	RUNLOGGER.Log(uh::str_util::format(_T("-%s\t%s\n"), 
		(CString)arg::k_citylist, m_args.m_cityList), false);
	RUNLOGGER.Log(uh::str_util::format(_T("-%s\t%s\n"), 
		(CString)arg::k_extcode_list, m_args.m_extcodeList), false);
	RUNLOGGER.Log(_T("\n"), false);

	writeMessage(
			uh::str_util::format(_T("データベース接続・フィーチャクラス取得:%s\n"),
			m_args.m_edtAddrDb ),
			RunLogMsgStatus::Info,
			true);
		
		// SiNDY（住所・編集）接続
	m_edtWorkspace = create_workspace(m_args.m_edtAddrDb);
	if (!m_edtWorkspace)
	{
		writeMessage(
				uh::str_util::format(_T("SiNDY（住所・編集）への接続に失敗：%s\n"),
				m_args.m_edtAddrDb ),
				RunLogMsgStatus::Error,
				true);
		ret = false;
	}

	// GS納品データ(住所）接続
	auto gsAddrWorkspace = create_workspace(m_args.m_gsAddrDb);
	if (!gsAddrWorkspace)
	{
		writeMessage(
				uh::str_util::format(_T("GS納品データ(住所）への接続に失敗：%s\n"),
				m_args.m_gsAddrDb ),
				RunLogMsgStatus::Error,
				true);
		ret = false;
	}

	if( !ret )
	{
		writeMessage(uh::str_util::format(_T("エラー終了\n")), Utility::RunLogMsgStatus::Error, false);
		return ret;
	}

	// 各種フィーチャクラス・テーブル取得

	m_edtLayerInfos.emplace(addr_polygon::kTableName, true);
	if( !m_edtLayerInfos[addr_polygon::kTableName].setFeatureClassInfo(
												m_edtWorkspace,
												addr_polygon::kTableName) )
	{
		writeMessage(
				uh::str_util::format(_T("SiNDY（住所・編集）の%sフィーチャクラス取得に失敗\n"),
				addr_polygon::kTableName ),
				RunLogMsgStatus::Error,
				true);
		ret = false;
	}

	m_baseLayerInfos.emplace(gs_addr_polygon::kTableName, true);
	if( !m_baseLayerInfos[gs_addr_polygon::kTableName].setFeatureClassInfo(
													gsAddrWorkspace,
													gs_addr_polygon::kTableName) )
	{
		writeMessage(
				uh::str_util::format(_T("GS納品データ(住所）の%sフィーチャクラス取得に失敗\n"),
				gs_addr_polygon::kTableName ),
				RunLogMsgStatus::Error,
				true);
		ret = false;
	}

	// インポートタイプが「addr_poly」以外の場合、住所ポイント関連のフィーチャクラスも取得
	if(Utility::ExecuteType::AddrPoly != m_args.m_impType)
	{
		m_edtLayerInfos.emplace(addr_point::kTableName, true);
		if( !m_edtLayerInfos[addr_point::kTableName].setFeatureClassInfo(
													m_edtWorkspace,
													addr_point::kTableName) )
		{
			writeMessage(
					uh::str_util::format(_T("SiNDY（住所・編集）の%sフィーチャクラス取得に失敗\n"),
					addr_point::kTableName ),
					RunLogMsgStatus::Error,
					true);
			ret = false;
		}

		m_baseLayerInfos.emplace(gs_addr_point::kTableName, true);
		if( !m_baseLayerInfos[gs_addr_point::kTableName].setFeatureClassInfo(
													gsAddrWorkspace,
													gs_addr_point::kTableName) )
		{
			writeMessage(
					uh::str_util::format(_T("GS納品データ(住所）の%sフィーチャクラス取得に失敗\n"),
					gs_addr_point::kTableName ),
					RunLogMsgStatus::Error,
					true);
			ret = false;
		}
	}

	writeMessage(
			uh::str_util::format(_T("市区町村コードリスト読み込み:%s\n"),
			m_args.m_cityList ),
			RunLogMsgStatus::Info,
			true);

	// 市区町村コードリスト読込み
	m_citycodeList = uh::text_util::ToTStrList((uh::tstring)m_args.m_cityList);
	if( m_citycodeList.empty() )
	{
		writeMessage(
				_T("市区町村コードリストが空\n"),
				RunLogMsgStatus::Error,
				true);
		ret = false;
	}

	// 拡張コードコードリスト読込み
	if( !getExtcodeList(m_args.m_extcodeList))
		ret = false;

	if( !ret )
		writeMessage(uh::str_util::format(_T("エラー終了\n")), Utility::RunLogMsgStatus::Error, false);

	return ret;

}

bool AddrDataCreator::setExtcodeInfo(
		const CString& kcode,
		EdtAddrPolygonRec& addrPolygonRec,
		bool isKcode1
	)
{
	CString fourteenCode, compStr;
	// 拡張コードが1か2かで参照フィールドが異なる
	if( isKcode1 )
		fourteenCode = addrPolygonRec.citycode + addrPolygonRec.addrcode1;
	else
		fourteenCode = addrPolygonRec.citycode + addrPolygonRec.addrcode2;

	// 無効値の場合、何も設定しない（EdtAddrPolygonRecの初期値のまま）
	// 無効値は、拡張コード1・拡張コード2で共通
	if( 0 == kcode.CompareNoCase( _T("000") ) )
		return true;

	fourteenCode += kcode;

	// 無効値以外で、拡張コードリストに存在しない場合、エラー
	auto excodeListRecIt = m_extcodeListMap.find(fourteenCode);
	if( m_extcodeListMap.end() == excodeListRecIt )
		return false;

	// 拡張コードリストから取得した名称・読みをそれぞれ設定
	// 拡張コード1か2かで、格納先のフィールドが異なる
	auto extcodeListRec = (*excodeListRecIt).second;

	if( isKcode1 )
	{
		addrPolygonRec.extName1 = extcodeListRec.extName;
		addrPolygonRec.extYomi1 = extcodeListRec.extYomi;
	}
	else
	{
		addrPolygonRec.extName2 = extcodeListRec.extName;
		addrPolygonRec.extYomi2 = extcodeListRec.extYomi;
	}
	
	return true;
}

bool AddrDataCreator::setEdtAddrPolygonRec(
	const IFeaturePtr& feature,
	EdtAddrPolygonRec& polyRec
	)
{
	bool ret = true;

	auto& baseFieldMap = m_baseLayerInfos[gs_addr_polygon::kTableName].getFieldMap();

	CComVariant citycode, addrcode1, addrcode2, k_code1, k_code2, gaikufugo, addrClass;
	if ( !getValue(feature, baseFieldMap.FindField(gs_addr_polygon::kCityCode), citycode) ||
	     !getValue(feature, baseFieldMap.FindField(gs_addr_polygon::kAddrCode1), addrcode1) || 
	     !getValue(feature, baseFieldMap.FindField(gs_addr_polygon::kAddrCode2), addrcode2) || 
	     !getValue(feature, baseFieldMap.FindField(gs_addr_polygon::kKCode1), k_code1) || 
	     !getValue(feature, baseFieldMap.FindField(gs_addr_polygon::kKCode2), k_code2) || 
	     !getValue(feature, baseFieldMap.FindField(gs_addr_polygon::kGaikuFugo), gaikufugo) || 
	     !getValue(feature, baseFieldMap.FindField(gs_addr_polygon::kAddrClass), addrClass) || 
	     FAILED(feature->get_OID(&polyRec.oid) ) )
	{
			writeErrMsg( gs_addr_polygon::kTableName, polyRec.oid,
						_T("ERROR"), _T("Featureからのgetに失敗"), _T(""));
			return false;
	}
	
	polyRec.citycode = citycode.bstrVal;
	polyRec.addrcode1 = addrcode1.bstrVal;
	polyRec.setAddrcode2(addrcode2.bstrVal);
	polyRec.gaikuFugo = ( 0 == (gaikufugo.lVal) ) ? _T("") : uh::str_util::ToString(gaikufugo.lVal);
	polyRec.addrType = addrClass.lVal;
	polyRec.colorCode = 0;
	polyRec.geo = feature->GetShapeCopy();

	CString kcode1 = k_code1.bstrVal;
	if( !setExtcodeInfo(kcode1, polyRec, true) )
	{
		std::vector<CString> infos = boost::assign::list_of
			( polyRec.citycode )
			( polyRec.addrcode1 )
			( kcode1 );

		writeErrMsg(
				gs_addr_polygon::kTableName,
				polyRec.oid,
				_T("ERROR"), 
				_T("拡張コード1に対する名称・読みの取得に失敗"),
				infos
			);

		ret = false;
	}

	// 住所コード2が未設定の場合のみ拡張名称（通称）・拡張読み（通称）を行う
	CString kcode2 = k_code2.bstrVal;
	if( !polyRec.addrcode2.IsEmpty() && !setExtcodeInfo(kcode2, polyRec, false) )
	{
		std::vector<CString> infos = boost::assign::list_of
			( polyRec.citycode )
			( polyRec.addrcode2 )
			( kcode2 );

		writeErrMsg(
				gs_addr_polygon::kTableName,
				polyRec.oid,
				_T("ERROR"), 
				_T("拡張コード2に対する名称・読みの取得に失敗"),
				infos
			);

		ret = false;
	}

	return ret;
}

bool AddrDataCreator::setConfirmC(
	EdtAddrPointRec& addrPointRec,
	const CString& sourceCd
	)
{
	// 対応表に存在しない場合、エラー
	auto confirmIt = convertMap.find(sourceCd);
	if( convertMap.end() == confirmIt )
		return false;

	addrPointRec.confirmC = (*confirmIt).second;
	return true;
}

CString AddrDataCreator::toHalfGouNo(
	const CString& str
	)
{
	// AttrGouPointDlg.hからそのまま流用

	LPTSTR lpszOriginalText = (LPTSTR)((LPCTSTR)str);
	INT nTextLen = lstrlen( lpszOriginalText ) + 1;
	LPTSTR lpszText = (LPTSTR)_alloca( nTextLen * sizeof(TCHAR) * 2 );	// 2倍の領域を確保
	// 全て全角にしてしまう
	LCMapString( GetUserDefaultLCID(), LCMAP_FULLWIDTH, lpszOriginalText, nTextLen, lpszText, nTextLen * 2 );

	CString strText( lpszText );
	CString strRet;
	LONG lLen = CString_GetLength2( strText );

	for( LONG i = 0; i < lLen; i++ )
	{
		CString str = CString_GetAt2( strText, i );
		LPSTR lpstr = CT2A( (LPTSTR)((LPCTSTR)str) );
		LPTSTR lptstr = (LPTSTR)((LPCTSTR)str);

		if( ( 0x81 == (unsigned char)((lpstr[0])&0xff) ) && ( 0x94 == (unsigned char)((lpstr[1])&0xff) ) )
			strRet += _T("#");
		else if( ( 0x81 == (unsigned char)((lpstr[0])&0xff) ) && ( 0x96 == (unsigned char)((lpstr[1])&0xff) ) )
			strRet += _T("*");
		else if( ( 0x81 == (unsigned char)((lpstr[0])&0xff) ) && ( 0x7c == (unsigned char)((lpstr[1])&0xff) ) )
			strRet += _T("-");
		else if( ( 0x81 == (unsigned char)((lpstr[0])&0xff) ) && ( 0x40 == (unsigned char)((lpstr[1])&0xff) ) )	// [bug7749]スペースの入力はできないようにする
			strRet += _T("");
		// 数字の場合には半角に直す
		else if( ( 0x82 == (unsigned char)((lpstr[0])&0xff) ) && ( 0x4f <= (unsigned char)((lpstr[1])&0xff) ) && ( (unsigned char)((lpstr[1])&0xff) <= 0x58 ) )
		{
			LPTSTR lpszNumberText = (LPTSTR)_alloca( 2 * sizeof(TCHAR) );
			// 半角にしてしまう
			LCMapString( GetUserDefaultLCID(), LCMAP_HALFWIDTH, lptstr, 2, lpszNumberText, 2 );

			strRet += lpszNumberText;
		}
		else
		{
			// 先頭が#であるならば、文字そのままくっつける
			if( 0x30 <= (unsigned char)((lpstr[0])&0xff) && (unsigned char)(lpstr[0]&0xff) <= 0x39 )
				strRet += lpstr;
			else
			{
				if( strRet.Find( _T("#") ) == 0 )
					strRet += lpstr;
				else
				{
					// 文字入力の際に#が入っていなかったら先頭に#を付ける
					CString strTemp = strRet;
					strRet = _T("#");
					strRet += strTemp;
					strRet += lpstr;
				}
			}
		}
	}
	return strRet;
}

void AddrDataCreator::setGouNo(
	EdtAddrPointRec& addrPointRec,
	const std::vector<CString>& addressList,
	const CString& shinraiF,
	const CString& hyouki
	)
{
	CString gouNo;
	for(const auto& address : addressList)
	{
		// 格納値が初期値（半角スペース）の場合、スキップ
		if( 0 == address.Compare(_T(" ")) )
			continue;

		// 半角ハイフンで前から連結
		gouNo += ( gouNo.IsEmpty() ) ? address : ( _T("-") + address);
	}

	// この時点で値が空なら特殊処理、値があれば連結した文字列を号番号に設定
	if( gouNo.IsEmpty() )
	{
		// 信頼フラグが「2」（図面で確認し附属建物と判断した場合）または、
		// 表記が「2」（図面で確認し住所を持たなくてよい家形と判断した場合）の場合、
		// ポイント整備種別を「5」（付属建物）にする。
		// そうでなければ、号番号「*」にする
		if( 0 == shinraiF.Compare(_T("2")) || 0 == hyouki.Compare(_T("2")))
		{
			addrPointRec.pointType = 5;
			addrPointRec.gouNo = _T("");
		}
		else
			addrPointRec.gouNo = _T("*");
	}
	else
		addrPointRec.gouNo = toHalfGouNo(gouNo);
}

bool AddrDataCreator::setEdtAddrPointRec(
	const IFeaturePtr& feature,
	const EdtAddrPolygonRec& addrPolygonRec,
	EdtAddrPointRec& addrPointRec
	)
{
	bool ret = true;

	auto& fieldMap = m_baseLayerInfos[gs_addr_point::kTableName].getFieldMap();

	CComVariant placeCd, addr1, addr2, addr3, geoBfId, ipcBjId, sourceCd;
	CComVariant shinraiF, hyouki, adoptF;
	if( !getValue(feature, fieldMap.FindField(gs_addr_point::kPlaceCd), placeCd) || 
	    !getValue(feature, fieldMap.FindField(gs_addr_point::kAddress1), addr1) || 
	    !getValue(feature, fieldMap.FindField(gs_addr_point::kAddress2), addr2) || 
	    !getValue(feature, fieldMap.FindField(gs_addr_point::kAddress3), addr3) || 
	    !getValue(feature, fieldMap.FindField(gs_addr_point::kGeoBFid), geoBfId) || 
	    !getValue(feature, fieldMap.FindField(gs_addr_point::kIpcObjId), ipcBjId) || 
	    !getValue(feature, fieldMap.FindField(gs_addr_point::kSourceCd), sourceCd) || 
	    !getValue(feature, fieldMap.FindField(gs_addr_point::kShinrai), shinraiF) || 
	    !getValue(feature, fieldMap.FindField(gs_addr_point::kHyoki), hyouki) || 
	    !getValue(feature, fieldMap.FindField(gs_addr_point::kAdopt), adoptF) || 
	    FAILED(feature->get_OID(&addrPointRec.oid)) )		
	{
			writeErrMsg( gs_addr_polygon::kTableName, addrPointRec.oid,
						_T("ERROR"), _T("Featureからのgetに失敗"), _T(""));
			ret = false;
	}

	std::vector<CString> addressList = boost::assign::list_of
							(addr1.bstrVal)
							(addr2.bstrVal)
							(addr3.bstrVal);

	// 市区町村コード、住所コード
	CString citycode = CString(placeCd).Mid(0, 5);
	CString addrcode = CString(placeCd).Mid(5, 6);

	// 住所ポリゴンと住所ポイントの市区町村コード、住所コード1のいずれかが異なる場合
	if( 0 != addrPolygonRec.citycode.Compare(citycode) ||
		0 != addrPolygonRec.addrcode1.Compare(addrcode) )
	{
		// 住所ポイントの市区町村コード、住所コードを設定
		addrPointRec.citycode = citycode;
		addrPointRec.addrcode = addrcode;
	}
	else
	{
		// 住所ポリゴンと住所ポイントの市区町村コード、住所コード1が両方一致する場合

		// 比較用のADDRESS1から半角スペースを除去
		CString address1 = addr1.bstrVal;
		address1.Replace(_T(" "), _T(""));

		// 住所ポリゴンの街区符号または拡張コード1に対する名称のいずれかが、
		// 住所ポイントのADDRESS1値と一致する場合
		if( ( 0 == addrPolygonRec.gaikuFugo.Compare(uh::str_util::ToHalf(address1)) ||
			  0 == addrPolygonRec.extName1.Compare(address1) ) &&
			  !address1.IsEmpty()
			)
		{
			// 号番号に設定する値がADDRESS2とADDRESS3のみ
			addressList = boost::assign::list_of
					(addr2.bstrVal)
					(addr3.bstrVal);
		}
		else
		{
			// 街区符号または拡張コード1が存在する場合
			if( !address1.IsEmpty() &&
				( !addrPolygonRec.gaikuFugo.IsEmpty() ||
				  !addrPolygonRec.extName1.IsEmpty() ) )
				// 街区符号無効化フラグを1にする
				addrPointRec.ignoreF = 1;
			}
	}

	// ADDRESSの格納値検査
	bool beforeAddressExist = ( 0 != addressList[0].Compare(_T(" ")) );
	for(unsigned int i = 1; i < addressList.size(); ++i)
	{
		bool isAddressExist = ( 0 != addressList[i].Compare(_T(" ")) );
		if( !beforeAddressExist && isAddressExist )
		{
			writeErrMsg(
					gs_addr_point::kTableName,
					addrPointRec.oid,
					_T("ERROR"), 
					_T("ADDRESSフィールドが歯抜け（GS納品データ）"),
					addressList
				);

			ret = false;
			break;
		}
		beforeAddressExist = isAddressExist;
	}

	// 号番号
	setGouNo(addrPointRec, addressList, CString(shinraiF.bstrVal), CString(hyouki.bstrVal));

	// 優先フラグ（一律「0」）
	addrPointRec.priorityF = 0;
		
	// Geospace建物ID
	addrPointRec.geoBldId = geoBfId.bstrVal;

	// iPC建物ID（取得した文字列の先頭2文字は除去して設定）
	CString ipcBldId = ipcBjId.bstrVal;
	addrPointRec.ipcBldId = ( 0 == ipcBldId.Compare(_T(" ")) ) ? _T("") : ipcBldId.Mid(2);

	// 形状
	addrPointRec.geo = feature->GetShapeCopy();

	// 確認ステータスコード
	if( !setConfirmC(addrPointRec, CString(sourceCd.bstrVal)) )
	{
		writeErrMsg(
				gs_addr_point::kTableName,
				addrPointRec.oid,
				_T("ERROR"), 
				_T("出典コード値が不正（GS納品データ）"),
				CString(sourceCd.bstrVal)
			);
		ret = false;
	}

	// ポイント整備種別コード

	// 採用フラグがnullの場合、擬似的に0にして判定する
	long adopt = ( adoptF.vt == VT_NULL ) ? 0 : adoptF.lVal;
	if( 0 != adopt ) // 採用フラグが空ではない場合
		addrPointRec.pointType = 2;
	else if( 0 == addrPointRec.pointType )
		addrPointRec.pointType = ( addrPointRec.ipcBldId.IsEmpty() ) ? 3 : 1;

	return ret;
}

bool AddrDataCreator::getExtcodeList(
		const CString& extcodeListPath
	)
{
	static std::map<int, CString> extFieldIdxMap = boost::assign::map_list_of
		( 0, _T("14桁コード"))
		( 1, _T("名称"))
		( 2, _T("読み"));
	const int EXTCODE_COL_NUM = extFieldIdxMap.size();

	// ファイルから1行ずつ読み込んで格納
	std::ifstream ifs( extcodeListPath );
	if( !ifs.is_open() )
	{
		writeMessage(
				_T("拡張コードリストが読み込めません\n"),
				RunLogMsgStatus::Error,
				true);
		return false;
	}

	bool ret = true;
	std::string line;
	int lineCount = 0;
	while(std::getline(ifs, line))
	{
		++lineCount;

		// 空行は取得対象外
		if( line.empty() )
			continue;

		auto splitValues = uh::str_util::split(CString(line.c_str()), _T("\t"), true);
		if( EXTCODE_COL_NUM != splitValues.size() )
		{
			writeMessage(
					uh::str_util::format(_T("拡張コードリストの列数が%d列ではありません（%d行目 列数:%d）\n"),
					EXTCODE_COL_NUM, lineCount, splitValues.size() ),
					RunLogMsgStatus::Error,
					true);
			ret = false;
			continue;
		}

		for(unsigned int i = 0; i < splitValues.size(); ++i)
		{
			if( splitValues[i].IsEmpty() )
			{
				writeMessage(
						uh::str_util::format(_T("拡張コードリストの列値が空です（%d行目 列名:%s）\n"),
						lineCount, extFieldIdxMap[i] ),
						RunLogMsgStatus::Error,
						true);
				ret = false;
			}
		}
		if( !ret )
			continue;

		m_extcodeListMap.emplace(
				std::piecewise_construct,
				std::forward_as_tuple(splitValues[0]),
				std::forward_as_tuple(splitValues[0], splitValues[1], splitValues[2]));
	}

	return ret;
}

bool AddrDataCreator::getAddrPolygon(
	std::vector<EdtAddrPolygonRec>& addrPolygonRecs,
	const CString& citycode,
	CountInfo& countInfo
	)
{
	bool ret = true;

	// 市区町村コードを指定して該当オブジェクトを取得
	CString whereClause = uh::str_util::format(_T("%s = '%s'"), gs_addr_polygon::kCityCode, citycode);
	auto& cursor = searchByWhereClause(m_baseLayerInfos[gs_addr_polygon::kTableName], whereClause);

	if( !cursor )
	{
		writeMessage(
				uh::str_util::format(_T("GS納品データ(住所：%s）の検索に失敗（市区町村コード：%s）\n"),
				gs_addr_polygon::kTableName, citycode ),
				RunLogMsgStatus::Error,
				true);
		return false;
	}

	IFeaturePtr feature;
	while (S_OK == cursor->NextFeature(&feature) && feature)
	{
		++(countInfo.totalCount);

		// EdtAddrPolygonRecに値を設定
		EdtAddrPolygonRec polyRec;
		ret &= setEdtAddrPolygonRec(feature, polyRec);

		addrPolygonRecs.push_back(polyRec);
	}
	return ret;
}

bool AddrDataCreator::getAddrPoint(
	const std::vector<EdtAddrPolygonRec>& addrPolygonRecs,
	std::vector<EdtAddrPointRec>& addrPointRecs,
	CountInfo& countInfo
	)
{
	bool ret = true;

	// 取得した住所ポリゴン分、処理を行う
	for(auto& polyRec : addrPolygonRecs)
	{
		std::vector<long> touchOIds;

		// 住所ポリゴンの境界上に存在する住所ポイントを先に取得
		if( !getBoundaryTouchPoints(polyRec, touchOIds) )
			return false;

		// 処理対象の住所ポリゴンに包含される住所ポイントを取得
		ISpatialFilterPtr ipSpatialFilter(CLSID_SpatialFilter);
		ipSpatialFilter->putref_Geometry(polyRec.geo);
		ipSpatialFilter->PutSpatialRel(esriSpatialRelEnum::esriSpatialRelIntersects);

		IFeatureCursorPtr cursor;
		if (FAILED(m_baseLayerInfos[gs_addr_point::kTableName].
						getFeatureClass()->Search(ipSpatialFilter, VARIANT_FALSE, &cursor)))
		{
			writeMessage(
					uh::str_util::format(_T("GS納品データ(住所：%s）の検索に失敗（住所ポリゴンOID：%ld）\n"),
					gs_addr_point::kTableName, polyRec.oid ),
					RunLogMsgStatus::Error,
					true);
			return false;
		}

		IFeaturePtr pointFeature;
		while (S_OK == cursor->NextFeature(&pointFeature) && pointFeature)
		{
			++(countInfo.totalCount);

			CComVariant sourceCd;
			pointFeature->get_Value(
					m_baseLayerInfos[gs_addr_point::kTableName].getFieldMap().FindField(gs_addr_point::kSourceCd), &sourceCd);

			// 出典フラグが「9」（追加調査必要）の場合、インポート対象外
			if( 0 == CString(sourceCd.bstrVal).Compare(_T("9")) )
			{
				++(countInfo.exceptCount);
				continue;
			}

			EdtAddrPointRec pointRec;
			ret &= setEdtAddrPointRec(pointFeature, polyRec, pointRec);

			// 取得した住所ポイントがポリゴンの境界上に存在する場合、フラグを立てておく
			if( touchOIds.end() != std::find(touchOIds.begin(), touchOIds.end(), pointRec.oid ) )
				pointRec.onAddrPolyBoundary = true;

			addrPointRecs.push_back(pointRec);
		}

	}
	return ret;
}

bool AddrDataCreator::getBoundaryTouchPoints(
		const EdtAddrPolygonRec& addrPolygonRec,
		std::vector<long>& touchOIds
	)
{
	IGeometryPtr boundaryGeo = ((ITopologicalOperatorPtr)addrPolygonRec.geo)->GetBoundary();

	// 処理対象の住所ポリゴンの外周に接触する住所ポイントを取得
	ISpatialFilterPtr ipSpatialFilter(CLSID_SpatialFilter);
	ipSpatialFilter->putref_Geometry(boundaryGeo);
	ipSpatialFilter->PutSpatialRel(esriSpatialRelEnum::esriSpatialRelIntersects);

	IFeatureCursorPtr cursor;
	if (FAILED(m_baseLayerInfos[gs_addr_point::kTableName].
					getFeatureClass()->Search(ipSpatialFilter, VARIANT_FALSE, &cursor)))
	{
		writeMessage(
				uh::str_util::format(_T("GS納品データ(住所：%s）の検索に失敗（住所ポリゴンOID：%ld）\n"),
				gs_addr_point::kTableName, addrPolygonRec.oid ),
				RunLogMsgStatus::Error,
				true);
		return false;
	}

	IFeaturePtr feature;
	while (S_OK == cursor->NextFeature(&feature) && feature)
	{
		long oid(0);
		if( FAILED(feature->get_OID(&oid)) )
			return false;

		touchOIds.push_back(oid);
	}
	return true;
}
bool AddrDataCreator::insertAddrPolygon(
	const std::vector<EdtAddrPolygonRec>& addrPolygonRecs,
	CountInfo& countInfo
	)
{
	bool ret = true;

	IFeatureCursorPtr insertCursor;
	if( FAILED(m_edtLayerInfos[addr_polygon::kTableName].getFeatureClass()->Insert(VARIANT_TRUE, &insertCursor)) )
	{
		writeErrMsg( addr_polygon::kTableName, 0L, _T("ERROR"), _T("Insertに失敗"), _T(""));
		return false;
	}

	IFeatureBufferPtr featureBuf;
	if( FAILED(m_edtLayerInfos[addr_polygon::kTableName].getFeatureClass()->CreateFeatureBuffer(&featureBuf)) )
	{
		writeErrMsg( addr_polygon::kTableName, 0L, _T("ERROR"), _T("CreateFeatureBufferに失敗"), _T(""));
		return false;
	}

	auto& fieldMap = m_edtLayerInfos[addr_polygon::kTableName].getFieldMap();
	for(const auto& addrPolyRec : addrPolygonRecs)
	{
		// 共通属性値設定
		for (const auto& attr : m_commonAttrMap)
		{
			if( !putValue(featureBuf, fieldMap.FindField(attr.first), attr.second) )
			{
				writeErrMsg( gs_addr_polygon::kTableName, addrPolyRec.oid,
							_T("ERROR"), _T("FeatureBufferへのputに失敗"), _T(""));
				ret = false;
			}
		}

		// レイヤ個別属性値設定
		if( !putValue(featureBuf, fieldMap.FindField(addr_polygon::kCityCode), addrPolyRec.citycode) || 
			!putValue(featureBuf, fieldMap.FindField(addr_polygon::kAddrCode1), addrPolyRec.addrcode1) || 
			!putValue(featureBuf, fieldMap.FindField(addr_polygon::kAddrCode2), addrPolyRec.addrcode2) || 
			!putValue(featureBuf, fieldMap.FindField(addr_polygon::kExtName1), addrPolyRec.extName1) || 
			!putValue(featureBuf, fieldMap.FindField(addr_polygon::kExtYomi1), addrPolyRec.extYomi1) || 
			!putValue(featureBuf, fieldMap.FindField(addr_polygon::kExtName2), addrPolyRec.extName2) || 
			!putValue(featureBuf, fieldMap.FindField(addr_polygon::kExtYomi2), addrPolyRec.extYomi2) || 
			!putValue(featureBuf, fieldMap.FindField(addr_polygon::kGaikufugo),_ttol(addrPolyRec.gaikuFugo), 
									( 0 == addrPolyRec.gaikuFugo.Compare(_T("") )) ? true : false ) || 
			!putValue(featureBuf, fieldMap.FindField(addr_polygon::kAddrType), addrPolyRec.addrType) || 
			!putValue(featureBuf, fieldMap.FindField(addr_polygon::kColorCode), 0) || 
			FAILED(featureBuf->putref_Shape(addrPolyRec.geo)) )
		{
			writeErrMsg( gs_addr_polygon::kTableName, addrPolyRec.oid,
						_T("ERROR"), _T("FeatureBufferへのputに失敗"), _T(""));
			ret = false;
		}

		CComVariant oid(0);
		if( FAILED(insertCursor->InsertFeature(featureBuf, &oid)) )
		{
			writeErrMsg( addr_polygon::kTableName, addrPolyRec.oid,
						_T("ERROR"), _T("InsertFeatureに失敗"), _T("") );
			ret = false;
			continue;
		}
		++(countInfo.importCount);
	}

	return ret;
}

bool AddrDataCreator::insertAddrPoint(
	const std::vector<EdtAddrPointRec>& addrPointRecs,
	CountInfo& countInfo
	)
{
	bool ret = true;

	IFeatureCursorPtr insertCursor;
	if( FAILED(m_edtLayerInfos[addr_point::kTableName].getFeatureClass()->Insert(VARIANT_TRUE, &insertCursor) ) )
	{
		writeErrMsg( addr_point::kTableName, 0L, _T("ERROR"), _T("Insertに失敗"), _T(""));
		return false;
	}

	auto fieldMap = m_edtLayerInfos[addr_point::kTableName].getFieldMap();

	IFeatureBufferPtr featureBuf;
	if( FAILED(m_edtLayerInfos[addr_point::kTableName].getFeatureClass()->CreateFeatureBuffer(&featureBuf) ) )
	{
		writeErrMsg( addr_point::kTableName, 0L, _T("ERROR"), _T("CreateFeatureBufferに失敗"), _T(""));
		return false;
	}

	for(auto& pointRec : addrPointRecs)
	{
		// 共通属性値設定
		for (const auto& attr : m_commonAttrMap)
		{
			if( !putValue(featureBuf, fieldMap.FindField(attr.first), attr.second) )
			{
				writeErrMsg( gs_addr_point::kTableName, pointRec.oid,
							_T("ERROR"), _T("FeatureBufferへのputに失敗"), _T(""));
				ret = false;
			}
		}

		// レイヤ個別属性値設定
		if ( !putValue(featureBuf, fieldMap.FindField(addr_point::kCityCode), pointRec.citycode) || 
			 !putValue(featureBuf, fieldMap.FindField(addr_point::kAddrCode), pointRec.addrcode) ||
			 !putValue(featureBuf, fieldMap.FindField(addr_point::kGouNo), pointRec.gouNo) ||
			 !putValue(featureBuf, fieldMap.FindField(addr_point::kYomi), pointRec.yomi) ||
			 !putValue(featureBuf, fieldMap.FindField(addr_point::kPriority), pointRec.priorityF) ||
			 !putValue(featureBuf, fieldMap.FindField(addr_point::kIgnore), pointRec.ignoreF) ||
			 !putValue(featureBuf, fieldMap.FindField(addr_point::kPointType), pointRec.pointType) ||
			 !putValue(featureBuf, fieldMap.FindField(addr_point::kConfirm), pointRec.confirmC) ||
			 !putValue(featureBuf, fieldMap.FindField(addr_point::kGeoBldID), pointRec.geoBldId) ||
			 !putValue(featureBuf, fieldMap.FindField(addr_point::kIPCBldID), pointRec.ipcBldId) ||
			 FAILED(featureBuf->putref_Shape(pointRec.geo)) )
		{
			writeErrMsg( gs_addr_point::kTableName, pointRec.oid,
						_T("ERROR"), _T("FeatureBufferへのputに失敗"), _T(""));
			ret = false;
		}

		CComVariant oid(0);
		if( FAILED(insertCursor->InsertFeature(featureBuf, &oid)) )
		{
			writeErrMsg( gs_addr_point::kTableName, pointRec.oid,
						_T("ERROR"), _T("InsertFeatureに失敗"), _T("") );
			ret = false;
			continue;
		}
		++(countInfo.importCount);

		// 該当住所ポイントが住所ポリゴンの境界上に存在する場合、エラーログに情報として出力
		if( pointRec.onAddrPolyBoundary )
		{
			writeErrMsg(
					addr_point::kTableName,
					(long)oid.lVal,
					_T("WARNING"), 
					_T("住所ポリゴンの境界上に存在する（編集データ）"),
					CString()
				);
		}

	}
	return ret;
}

bool AddrDataCreator::create()
{
	bool result = true;

	// 件数カウント情報
	std::map<CString, CountInfo> addrPolyCntInfos;
	std::map<CString, CountInfo> addrPointCntInfos;

	// 市区町村コード単体で処理を実施
	for(auto& citycode : m_citycodeList)
	{
		// 空の場合スキップ
		if( citycode.empty() )
			continue;

		addrPolyCntInfos.emplace(std::piecewise_construct,
									std::make_tuple(citycode.c_str()),
									std::make_tuple(addr_polygon::kTableName));
		addrPointCntInfos.emplace(std::piecewise_construct,
									std::make_tuple(citycode.c_str()),
									std::make_tuple(addr_point::kTableName));

		// 市区町村コードが不正な場合
		if( !std::regex_match(CStringW(citycode.c_str()).GetString(), citycode_re) )
		{
			writeMessage(uh::str_util::format(_T("市区町村コードが不正:%s\n"), citycode.c_str()),
					RunLogMsgStatus::Error, false);
			result &= false;
			continue;
		}

		writeMessage(
				uh::str_util::format(_T("市区町村コード：%s\n"),
				citycode.c_str() ),
				RunLogMsgStatus::Info,
				true);
	
		// 住所ポリゴン取得
		writeMessage(
				_T("GS納品レイヤからデータ取得\n"), RunLogMsgStatus::Info, true);

		std::vector<EdtAddrPolygonRec> addrPolygonRecs;
		if( !getAddrPolygon(addrPolygonRecs,
							citycode.c_str(),
							addrPolyCntInfos[citycode.c_str()]) )
		{
			result &= false;
			continue;
		}

		std::vector<EdtAddrPointRec> addrPointRecs;

		// インポートタイプが「addr_poly」以外の場合、住所ポイントも処理対象にする
		if( Utility::ExecuteType::AddrPoly != m_args.m_impType )
		{
			if( !getAddrPoint(addrPolygonRecs, addrPointRecs, addrPointCntInfos[citycode.c_str()]) )
			{
				result &= false;
				continue;
			}
		}

		writeMessage(_T("インポート処理\n"), RunLogMsgStatus::Info, true);

		// インポート処理
		if( !startEdit() )
		{
			writeMessage(_T("編集開始に失敗\n"), RunLogMsgStatus::Error, true);
			result &= false;
			continue;
		}

		bool ret = true;

		// インポートタイプが「addr_point」以外の場合、住所ポリゴンのインポート処理を行う
		if( Utility::ExecuteType::AddrPoint != m_args.m_impType )
		{
			if( !insertAddrPolygon(addrPolygonRecs, addrPolyCntInfos[citycode.c_str()]) )
				ret = false;
		}

		// インポートタイプが「addr_poly」以外の場合、住所ポイントのインポート処理を行う
		if( Utility::ExecuteType::AddrPoly != m_args.m_impType )
		{
			if( !insertAddrPoint(addrPointRecs, addrPointCntInfos[citycode.c_str()]) )
				ret = false;
		}

		// インポート処理結果に応じて編集終了/破棄を切り分け
		if( ret )
		{
			// 編集終了
			if ( !stopEdit(ret) )
			{
				writeMessage(_T("編集終了に失敗\n"), RunLogMsgStatus::Error, true);
				addrPolyCntInfos[citycode.c_str()].resetImportCount();
				addrPointCntInfos[citycode.c_str()].resetImportCount();
			}
		}
		else
		{
			// 編集破棄
			abort();
			addrPolyCntInfos[citycode.c_str()].resetImportCount();
			addrPointCntInfos[citycode.c_str()].resetImportCount();
		}
		result &= ret;
	}

	// テーブル統計情報更新
	if( Utility::ExecuteType::AddrPoint != m_args.m_impType )
	{
		if( !tableAnalyze(addr_polygon::kTableName) )
		{
			writeMessage(uh::str_util::format(_T("テーブル統計情報更新に失敗:%s\n"),
							addr_polygon::kTableName), RunLogMsgStatus::Error, true);
			result = false;
		}
	}

	if( Utility::ExecuteType::AddrPoly != m_args.m_impType )
	{
		if( !tableAnalyze(addr_point::kTableName) )
		{
			writeMessage(uh::str_util::format(_T("テーブル統計情報更新に失敗:%s\n"),
							addr_point::kTableName), RunLogMsgStatus::Error, true);
			result = false;
		}
	}
				
	if( result )
		writeMessage(uh::str_util::format(_T("正常終了\n")), RunLogMsgStatus::Info, false);
	else
		writeMessage(uh::str_util::format(_T("エラー終了\n")), RunLogMsgStatus::Error, false);

	writeMessage(uh::str_util::format(_T("\n市区町村コード\tレイヤ名\t全件数\tインポート件数\t除外件数\n")),
					RunLogMsgStatus::Normal, false);
	for(const auto& citycode : m_citycodeList)
	{
		if( Utility::ExecuteType::AddrPoint != m_args.m_impType )
			writeMessage(uh::str_util::format(_T("%s\t%s"),
								citycode.c_str(),
							addrPolyCntInfos[citycode.c_str()].getLogMsg()),
							RunLogMsgStatus::Normal, false);

		if( Utility::ExecuteType::AddrPoly != m_args.m_impType )
			writeMessage(uh::str_util::format(_T("%s\t%s"),
								citycode.c_str(),
							addrPointCntInfos[citycode.c_str()].getLogMsg()),
							RunLogMsgStatus::Normal, false);
	}

	return result;
}

