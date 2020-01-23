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
#include "KP2WorkTxt.h"

#include <string>

#include <ArcHelperEx/GlobalFunctions.h>

// 初期化
bool CKP2WorkTxt::Init()
{
	// 処理対象エリア取得
	if( m_rParam.m_pairArea.first == _T("-ca") || m_rParam.m_pairArea.first == _T("-cr") )
	{
		IQueryFilterPtr ipQuery( CLSID_QueryFilter );
		CString strWhere(_T("OBJECTID IS NOT NULL"));
		if( m_rParam.m_pairArea.first == _T("-cr") )
		{
			// 範囲を取得
			int nPos = m_rParam.m_pairArea.second.Find('-');
			if( nPos > 0 )
			{
				strWhere.Format( _T("%s between '%s00000000000000000' and '%s99999999999999999'"), 
								 klink_point::kAddrCodeOrg, m_rParam.m_pairArea.second.Left(nPos), m_rParam.m_pairArea.second.Mid(nPos+1) );
			}
			else
			{
				std::cerr << "#Error 市区町村コード指定フォーマットが不正です。(start-end) : " << CT2CA(m_rParam.m_pairArea.second) << std::endl;
				return false;
			}
		}

		ipQuery->put_WhereClause( _bstr_t(strWhere) );
		ipQuery->put_SubFields( _bstr_t(klink_point::kAddrCodeOrg) );

		CSPFieldMap cFields( m_rParam.m_cPointFC.GetFieldMap() );
		CSPTableNameString cNames( m_rParam.m_cPointFC.GetNameString() );

		_ICursorPtr ipCursor( m_rParam.m_cPointFC.Search(ipQuery, VARIANT_FALSE) );
		if( ipCursor )
		{
			_IRowPtr ipRow;
			while( ipCursor->NextRow(&ipRow) == S_OK )
			{
				CKLinkPoint cKPPoint( ipRow, sindyTableType::klink_point, false, cFields, cNames );
				m_setCode.insert( cKPPoint.GetAddrCodeOrg().Left(5) );
			}
		}
	}
	else if( m_rParam.m_pairArea.first == _T("-cl")  )
	{
		if( !LoadCityCodeList(m_rParam.m_pairArea.second) )
		{
			std::cerr << "#Error 市区町村リストからの市区町村コードリスト生成に失敗 : " << CT2CA(m_rParam.m_pairArea.second) << std::endl;
			return false;
		}
	}
	else if( m_rParam.m_pairArea.first == _T("-cs") )
	{
		m_setCode.insert( m_rParam.m_pairArea.second );
	}
	else
	{
		std::cerr << "#Error 実行エリアが特定できません : " << CT2CA(m_rParam.m_pairArea.first) << std::endl;
		return false;
	}

	// 住所マスタへ接続
	return m_cACHelper.Connect(m_rParam.m_strJDBProp);
}

// データ出力
bool CKP2WorkTxt::WriteData()
{
	// 家形リンクポイントのフィールドインデックス等をキャッシュ
	CSPFieldMap cFields( m_rParam.m_cPointFC.GetFieldMap() );
	CSPTableNameString cNames( m_rParam.m_cPointFC.GetNameString() );

	for( std::set<CString>::const_iterator itCode = m_setCode.begin(); itCode != m_setCode.end(); ++itCode )
	{
		m_setWritedOID.clear();
		std::cout << "#" << CT2CA(*itCode) << std::endl;

		// 出力ファイルオープン
		CString strFile;
		strFile.Format( _T("%s\\kpw%s.txt"), m_rParam.m_strOutDir, *itCode );
		std::ofstream ofs( strFile, std::ios::binary );
		if( !ofs )
		{
			std::cerr << "#Error 出力ファイルの作成に失敗 : " << CT2CA(strFile) << std::endl;
			continue;
		}

		// 指定5桁内の行政界検索
		IQueryFilterPtr ipFilter( CLSID_QueryFilter );
		CString strWhere;
		strWhere.Format( _T("%s = '%s'"), city_admin::kCityCode, *itCode );
		ipFilter->put_WhereClause( _bstr_t(strWhere) );

		long lAdminCount = m_rParam.m_cCityAdminFC.FeatureCount( ipFilter );
		IFeatureCursorPtr ipAdminCursor( m_rParam.m_cCityAdminFC.Search(ipFilter, false) );
		IFeaturePtr ipAdminFeature;
		long lCount = 1;
		while( ipAdminCursor && ipAdminCursor->NextFeature(&ipAdminFeature) == S_OK )
		{
			// 行政界内にある家形リンクポイント取得
			CCityAdmin cAdmin( ipAdminFeature, sindyTableType::city_admin, false, m_rParam.m_cCityAdminFC.GetFieldMap(), m_rParam.m_cCityAdminFC.GetNameString() );

			ISpatialFilterPtr ipSpFIlter( AheInitSpatialFilter(NULL, cAdmin.GetShapeCopy()) );
			if( ipSpFIlter )
			{				
				IFeatureCursorPtr ipCursor( m_rParam.m_cPointFC.Search(ipSpFIlter, VARIANT_FALSE) );
				IFeaturePtr ipFeature;
				while( ipCursor && ipCursor->NextFeature(&ipFeature) == S_OK )
				{
					CKLinkPoint cKPPoint( ipFeature, sindyTableType::klink_point, false, cFields, cNames );

					// まだデータ化していない and 行政界と同一5桁を持っている → 中間データ化
					if( m_setWritedOID.find(cKPPoint.GetOID()) == m_setWritedOID.end()
						&& cAdmin.GetAddrCode().Left(5) == cKPPoint.GetAddrCodeOrg().Left(5) )
					{
						// 住所コードの最新状態を取得
						CString strNewAddrCode( GetNewestCode(cKPPoint.GetAddrCodeOrg().Left(11)) + cKPPoint.GetAddrCodeOrg().Mid(11) );
						if( strNewAddrCode.GetLength() == 22 )
						{
							// 住所コードが廃止or無効であればデータ化しない(Bug7975)
							addrCodeStatus acStatus = m_cACHelper.GetCodeStatus( strNewAddrCode.Left(11) );
							if( kShiko == acStatus || kTmpShiko == acStatus )
							{
								// 行政界との整合確認
								long lEqualF = 0;
								if( GetEqualF(strNewAddrCode, NULL, &cAdmin, &lEqualF) && lEqualF != 1 )
								{
									WriteData( ofs, cKPPoint, strNewAddrCode, lEqualF );	// データ書き出し
									m_setWritedOID.insert( cKPPoint.GetOID() );				// 処理済みOIDリストに登録
								}
							}
						}
						else
							PrintError( &cKPPoint, _T("ERROR"), _T("最新状態の住所コードの取得に失敗"), cKPPoint.GetAddrCodeOrg() );
					}					
				}
			}
			std::cout << lCount++ << " / " << lAdminCount << " 行政界\r";
		}

		std::cout << std::endl;

		// 指定5桁が行政界内に存在しない場合を考慮して家形リンクポイントからも検索
		strWhere.Format( _T("%s like '%s%%'"), klink_point::kAddrCodeOrg, *itCode );
		ipFilter->put_WhereClause( _bstr_t(strWhere) );

		long lAllCount = m_rParam.m_cPointFC.FeatureCount( ipFilter );
		IFeatureCursorPtr ipCursor( m_rParam.m_cPointFC.Search(ipFilter, VARIANT_FALSE) );
		if( lAllCount > 0 && ipCursor )
		{
			lCount = 1;
			IFeaturePtr ipFeature;
			while( ipCursor->NextFeature(&ipFeature) == S_OK )
			{
				CKLinkPoint cKPPoint( ipFeature, sindyTableType::klink_point, false, cFields, cNames );
				
				// 行政界からの中間データ化ができないものだけここでカバーする
				if( m_setWritedOID.find(cKPPoint.GetOID()) == m_setWritedOID.end() )
				{
					// 住所コードの最新状態を取得
					CString strNewAddrCode( GetNewestCode(cKPPoint.GetAddrCodeOrg().Left(11)) + cKPPoint.GetAddrCodeOrg().Mid(11) );
					if( strNewAddrCode.GetLength() == 22 )
					{
						// 住所コードが廃止or無効であればデータ化しない(Bug7975)
						addrCodeStatus acStatus = m_cACHelper.GetCodeStatus( strNewAddrCode.Left(11) );
						if( kShiko == acStatus || kTmpShiko == acStatus )
						{
							// 行政界との整合情報取得
							IGeometryPtr ipGeo;
							((IFeature*)cKPPoint)->get_Shape( &ipGeo );
							long lEqualF = 0;
							if( !GetEqualF(strNewAddrCode, ipGeo, NULL, &lEqualF) )
							{						
								PrintError( &cKPPoint, _T("ERROR"), _T("行政界が取得できない") );				
								lEqualF = 1;
							}

							WriteData( ofs, cKPPoint, strNewAddrCode, lEqualF );	// データ書き出し
						}
					}
					else
						PrintError( &cKPPoint, _T("ERROR"), _T("最新状態の住所コードの取得に失敗"), cKPPoint.GetAddrCodeOrg() );
				}
				std::cout << lCount++ << " / " << lAllCount << " 件\r";
			}
			std::cout << std::endl;
		}
	}
	return true;
}

/////////////////////////////////////////////////
//	非公開メンバ関数
/////////////////////////////////////////////////

// 市区町村コードリスト読み込み
bool CKP2WorkTxt::LoadCityCodeList( const _TCHAR* lpcszFile )
{
	std::ifstream ifs( static_cast<CT2CW>(lpcszFile) );
	if( ifs )
	{
		while( !ifs.eof() )
		{
			std::string strBuff;
			std::getline( ifs, strBuff );
			if( !strBuff.empty() )
				m_setCode.insert( CString(strBuff.c_str()) );
		}
		return true;		
	}
	else
		return false;
}

// 最新11桁住所コード取得
CString CKP2WorkTxt::GetNewestCode( const _TCHAR* lpcszAddrCode )
{
	std::map<CString, CString>::const_iterator it = m_mapNewestAddrCode.find( lpcszAddrCode );
	if( it == m_mapNewestAddrCode.end() )
	{
		CString strSQL;
		strSQL.Format( _T("select getnewestcode( '%s' ) from dual"), lpcszAddrCode );
		CJusyoRecordSet cRecord;
		m_cACHelper.Select( strSQL, cRecord );
		if( cRecord.GetRecordCount() == 1 )
		{
			CString strNewCode( cRecord.GetStringValue((long)0) );
			if( strNewCode.GetLength() == 11 )
			{
				m_mapNewestAddrCode.insert( std::pair<CString, CString>(lpcszAddrCode, strNewCode) );
				return strNewCode;
			}
			else
				return _T("");
		}
		else
			return _T("");
	}
	else
		return it->second;

}

// 情報提供元コード変換
const _TCHAR* CKP2WorkTxt::ConvInfoSrc( klink_point::info_src::ECode eInfoSrc )
{
	switch( eInfoSrc )
	{
	case klink_point::info_src::kUnlooked:				return _T("AA");	// 未調査(座標なし)
	case klink_point::info_src::kWallLinkData:			return _T("03");	// 家形リンクデータ
	case klink_point::info_src::kGouMaintenanceData:	return _T("13");	// 号整備データ
	case klink_point::info_src::kWorking:				return _T("BB");	// 作業中
	default:	return _T("");
	}
}

// 入力用座標精度コード変換
const _TCHAR CKP2WorkTxt::ConvInputSeido( klink_point::input_seido::ECode eInputSeido )
{
	switch( eInputSeido )
	{
	case klink_point::input_seido::kNone:						return _T('A');		// 未調査
	case klink_point::input_seido::kNoVerification:				return _T('B');		// 未検証
	case klink_point::input_seido::kMultipleWallCounterData:	return _T('C');		// 対応家形複数(データ)
	case klink_point::input_seido::kNotWallCounterData:			return _T('D');		// 対応家形なし(データ)
	case klink_point::input_seido::kAddrClericalMistake:		return _T('E');		// 住所名誤記
	case klink_point::input_seido::kUnmatchedName:				return _T('G');		// 名称不一致
	case klink_point::input_seido::kUnmatchedShape:				return _T('H');		// 形状不一致
	case klink_point::input_seido::kMultipleWallCounterSrc:		return _T('I');		// 対応家形複数(資料) 
	case klink_point::input_seido::kNotWallCounterSrc:			return _T('J');		// 対応家形なし(資料) 
	case klink_point::input_seido::kUnmatchedBlock:				return _T('K');		// 街区不一致
	case klink_point::input_seido::kOther:						return _T('F');		// その他
	default:													return _T('0');
	}
}

// リリース用座標精度コード変換
const _TCHAR CKP2WorkTxt::ConvReleaseSeido( klink_point::release_seido::ECode eReleaseSeido )
{
	switch( eReleaseSeido )
	{
	case klink_point::release_seido::kNone:					return _T('A');		// 未調査
	case klink_point::release_seido::kMunicipalityBorder:	return _T('1');		// 市区郡町村界
	case klink_point::release_seido::kOazaBorder:			return _T('2');		// 町・大字界
	case klink_point::release_seido::kAzaBorder:			return _T('3');		// 丁目・字(大字)界
	case klink_point::release_seido::kBlock:				return _T('4');		// 街区(地番)
	case klink_point::release_seido::kPinpoint:				return _T('5');		// ピンポイント
	case klink_point::release_seido::kWall:					return _T('6');		// 家形
	case klink_point::release_seido::kUnmatchedBlock:		return _T('K');		// 街区不一致
	case klink_point::release_seido::kOther:				return _T('F');		// その他
	default:	return _T('*');
	}
}

// 行政界との整合情報取得
bool CKP2WorkTxt::GetEqualF( const CString& strKPAddrCode, IGeometry* ipGeo, const CCityAdmin* pAdmin, long* pEqualF )
{
	// 地番が[00000]で号がnot[0000]な不正データを無効化
	if( strKPAddrCode.Mid(11, 5) == _T("00000") && strKPAddrCode.Mid(16, 4) != _T("0000") )
	{
		*pEqualF = 2;
		return true;
	}

	// 行政界取得(境界上にあるときは、いずれかと整合合えばＯＫとする)
	if( !pAdmin )
	{
		// まだ紐付けてないなら検索
		ISpatialFilterPtr ipSpFIlter( AheInitSpatialFilter(NULL, ipGeo) );
		if( ipSpFIlter )
		{
			long lCount = m_rParam.m_cCityAdminFC.FeatureCount( ipSpFIlter );
			if( lCount > 0 )
			{
				IFeatureCursorPtr ipFeatureCursor = m_rParam.m_cCityAdminFC.Search( ipSpFIlter, false );
				IFeaturePtr ipFeature;
				while( ipFeatureCursor && ipFeatureCursor->NextFeature(&ipFeature) == S_OK )
				{
					CCityAdmin cAdmin( ipFeature, sindyTableType::city_admin, false, m_rParam.m_cCityAdminFC.GetFieldMap(), m_rParam.m_cCityAdminFC.GetNameString() );
				
					// 字・丁目コードが 001 ～ 099 なら11桁一致、その他は8桁一致確認
					if( 0 < _tstoi(cAdmin.GetAddrCode().Mid(8, 3)) && _tstoi(cAdmin.GetAddrCode().Mid(8, 3)) < 100 )
					{
						if( strKPAddrCode.Left(11) == cAdmin.GetAddrCode().Left(11) )
						{
							*pEqualF = 0;
							return true;
						}
					}
					else
					{
						if( strKPAddrCode.Left(8) == cAdmin.GetAddrCode().Left(8) )
						{
							*pEqualF = 0;
							return true;
						}
					}
				}
				*pEqualF = 1;
				return true;
			}
		}
	}
	else
	{
		// 既に紐付く行政界わかってるとき

		// 字・丁目コードが 001 ～ 099 なら11桁一致、その他は8桁一致確認
		if( 0 < _tstoi(pAdmin->GetAddrCode().Mid(8, 3)) && _tstoi(pAdmin->GetAddrCode().Mid(8, 3)) < 100 )
		{
			if( strKPAddrCode.Left(11) == pAdmin->GetAddrCode().Left(11) )
			{
				*pEqualF = 0;
				return true;
			}
		}
		else
		{
			if( strKPAddrCode.Left(8) == pAdmin->GetAddrCode().Left(8) )
			{
				*pEqualF = 0;
				return true;
			}
		}
		*pEqualF = 1;
		return true;
	}

	// 行政界取得できないならエラー
	*pEqualF = 1;
	return false;	
}

// データ書き出し
void CKP2WorkTxt::WriteData( std::ofstream& stream, const CKLinkPoint& cKPPoint, const CString& strAddrCode, long lEqualF )
{
	// メッシュコード・経度・緯度取得
	WKSPoint point;
	((IPointPtr)cKPPoint.GetShapeCopy())->QueryCoords( &point.X, &point.Y );

	// データ書き出し
	CString strData;
	strData.Format( _T("%d,%s,%s,%c,%c,%d,%d,%s,%s,%d,%s,%d,%.12f,%.12f"), 
					cKPPoint.GetOID(),									// OID
					strAddrCode,									// 住所コード
					ConvInfoSrc(cKPPoint.GetInfoSrc()),				// 情報提供元コード
					ConvInputSeido(cKPPoint.GetInputSeido()),		// 入力用座標精度コード
					ConvReleaseSeido(cKPPoint.GetReleaseSeido()),	// リリース用座標精度コード
					lEqualF,										// 行政界との整合状況
					cKPPoint.GetFloor(),							// テナント階数
					cKPPoint.GetKeisaiName(),						// 掲載名称
					cKPPoint.GetKatagakiName(),						// 方書名称
					cKPPoint.GetBuildInfoSrc(),						// ビル情報提供元コード
					cKPPoint.GetBuildName(),						// ビル名称
					GetMeshCode(point.X, point.Y), point.X, point.Y	);				// メッシュコード座標
	stream << CT2CA(strData) << "\n";

	stream.flush();
}
