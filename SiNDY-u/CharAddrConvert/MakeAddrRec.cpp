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
#include "MakeAddrRec.h"
#include <algorithm>

// 代表点プロットNGな都市背景種別一覧
const static long g_lNGSiteClass[] =
{
	city_site::bg_class::kRiver,                    //!< 河川
	city_site::bg_class::kLake,                     //!< 池・湖
	city_site::bg_class::kPool,                     //!< プール
	city_site::bg_class::kSea,                      //!< 海
	city_site::bg_class::kInterCityTollExpressway,  //!< 都市間高速道(有料)
	city_site::bg_class::kUrbanTollExpressway,      //!< 都市高速(有料)
	city_site::bg_class::kTollRoad,                 //!< 有料道路
	city_site::bg_class::kNationalHighway,          //!< 国道
	city_site::bg_class::kPrincipalPrefecturalRoad, //!< 主要地方道
	city_site::bg_class::kPrefectureNormalRoad,     //!< 一般都道府県道
	city_site::bg_class::kOtherArterialRoad,        //!< その他幹線道路
	city_site::bg_class::kSideway,                  //!< 歩道
	city_site::bg_class::kGardenPath,               //!< 庭園路
	city_site::bg_class::kInterCityFreeExpressway,  //!< 都市間高速道(無料)
	city_site::bg_class::kUrbanFreeExpressway,      //!< 都市高速(無料)
	city_site::bg_class::kBreakwater,               //!< 防波堤
	city_site::bg_class::kIncompleteArea,           //!< 不完全エリア
	city_site::bg_class::kPublicRoadStairs,         //!< 公道階段
	city_site::bg_class::kRunway,                   //!< 滑走路
	city_site::bg_class::kUnderRepairRoad,          //!< 工事中道路
	city_site::bg_class::kTollGateLane,             //!< 料金所レーン
	-1                                              //!< ループ終わり判定用
};

// 初期化
bool CMakeAddrRec::Init( const IFeatureClassPtr& ipCityAdminFC, const IFeatureClassPtr& ipCitySiteFC, const IFeatureClassPtr& ipBuildingFC, const IFeatureClassPtr& ipBuildingRoofFC, const IFeatureClassPtr& ipCityMeshFC, LPCTSTR lpcszCodeConvDB, LPCTSTR lpcszJusyoDB )
{
	m_ipCityAdminFC = ipCityAdminFC;
	m_ipCitySiteFC = ipCitySiteFC;
	m_ipBuildingFC = ipBuildingFC;
	m_ipBuildingRoofFC = ipBuildingRoofFC;
	m_ipCityMeshFC = ipCityMeshFC;

	m_ipCitySiteFC->FindField( _bstr_t(city_site::kBgClass), &m_lBGClass_FIdx );
	m_ipCityMeshFC->FindField( _bstr_t(citymesh::kCreateYear), &m_lCreateYear_FIdx );

	// コード変換DB
	if( !m_cConvTable.OpenDB(lpcszCodeConvDB) )
		return false;

	// 接続プロパティ構文チェック
	CString str( lpcszJusyoDB );
	int nSlaPos = str.Find( '/' ), nAtPos = str.Find( '@' );
	if( nSlaPos == -1 || nAtPos == -1 )
		return progress_out::ProgressError<bool>( false, kError, "住所マスタへの接続に失敗 : %s", lpcszJusyoDB );

	// 住所DBから郵便番号リスト生成
	int nPos = 0;
	CString strUser		= str.Tokenize( "/", nPos );
	CString strPass		= str.Tokenize( "@", nPos );
	CString strDBName	= str.Tokenize( "@", nPos );

	OStartup();
	ODatabase cDBSource;
	if( OSUCCESS == cDBSource.Open(strDBName, strUser, strPass) )
	{
		CString strSQL = _T("select ken_code || shi_code || oaza_code || aza_code, nvl(zip_code, '0000000') from areacode_master ")
						 _T("where oaza_code != '000' and haishi_date = '000000'");
		ODynaset cResult( cDBSource, strSQL );
		if( cResult.GetRecordCount() > 0 )
		{
			char szBuff1[12] = "", szBuff2[8] = "";
			do
			{
				cResult.GetFieldValue( 0, szBuff1, sizeof(szBuff1) );	cResult.GetFieldValue( 1, szBuff2, sizeof(szBuff2) );
				m_ZipMap.insert( std::pair<CString, CString>(szBuff1, szBuff2) );
				cResult.MoveNext();
			}while( !cResult.IsEOF() );
		}
	}
	else
		return progress_out::ProgressError<bool>( false, kError, "住所マスタへの接続に失敗 : %s", lpcszJusyoDB );

	OShutdown();

	if( m_ZipMap.empty() )
		return progress_out::ProgressError<bool>( false, kError, "郵便番号リストの作成に失敗 : %s", lpcszJusyoDB );
	else
		return true;

}

// 行政界から地番代表点生成
void CMakeAddrRec::MakePntRecordByAdmin( const CAdminInfo& cAdmin, PNTMAP& rContainer )
{
	// 既に行政界から代表点が生成されていれば問題なし
	CString strTmpCode = cAdmin.m_strAddrCode + m_cConvTable.GetPntCode( cAdmin.m_strAddrCode, cAdmin.m_strExtGaiku, false );
	if( rContainer.find(strTmpCode) == rContainer.end() )
	{
		// 同一16桁となる行政界が他にも存在するか確認
		CString strWhere;
		strWhere.Format( _T("%s = '%s' and %s = '%s' and %s = '%s'"), 
						 city_admin::kCityCode, cAdmin.m_strAddrCode.Left(5), city_admin::kAddrCode, cAdmin.m_strAddrCode.Mid(5), city_admin::kExtGaikuFugo, cAdmin.m_strExtGaiku );
		IQueryFilterPtr ipQuery( CLSID_QueryFilter );
		ipQuery->put_WhereClause( CComBSTR(strWhere) );

		long lCount = 0;
		m_ipCityAdminFC->FeatureCount( ipQuery, &lCount );
		IGeometryPtr ipTargetGeo;
		if( lCount == 1 )		// 引数のものをそのまま使用
		{
			ipTargetGeo = cAdmin.GetGeometry();
		}
		else if( lCount > 1 )	// 複数 -> [エリア内 > 面積広] の優先度で生成する
		{
			ITableSortPtr ipTableSort( CLSID_TableSort );
			ipTableSort->putref_Table( (ITablePtr)m_ipCityAdminFC );
			ipTableSort->put_Fields( CComBSTR(_T("AREACLASS_C, SHAPE.AREA")) );
			ipTableSort->put_Ascending( _bstr_t(_T("AREACLASS_C")), VARIANT_FALSE );	// エリア内優先
			ipTableSort->put_Ascending( _bstr_t(_T("SHAPE.AREA")), VARIANT_TRUE );		// 面積広い方優先
			ipTableSort->putref_QueryFilter( ipQuery );
			ipTableSort->Sort( NULL );

			_ICursorPtr ipCursor;
			ipTableSort->get_Rows( &ipCursor );
			IFeaturePtr ipFeature;
			if( ipCursor && ((IFeatureCursorPtr)ipCursor)->NextFeature(&ipFeature) == S_OK )
				ipFeature->get_Shape( &ipTargetGeo );
		}

		if( ipTargetGeo )
		{
			// 行政界から代表点生成
			CPntRec cRepPnt;
			cRepPnt.m_strAddrCode = cAdmin.m_strAddrCode;
			cRepPnt.m_lRepDomariF = 1;	// 代表点-エリア 地番止まり-OFF
			cRepPnt.m_strPntName = cAdmin.m_strExtGaiku;
			cRepPnt.m_strPntYomi = cAdmin.m_strExtGaikuYomi;
			cRepPnt.m_strZipCode = GetZipCode( cAdmin.m_strAddrCode );	// 該当11桁コードから郵便番号も取得

			WKSEnvelope wksEnv;
			ipTargetGeo->Envelope->QueryWKSCoords( &wksEnv );

			IPointPtr ipRepPoint( CLSID_Point ), ipKeepPoint;
			bool bSucceeded = false;
			int nDiv = 1;
			while( nDiv < 32 )
			{		
				double dXRange = (wksEnv.XMax - wksEnv.XMin) / (double)(nDiv * 2.0);
				double dYRange = (wksEnv.YMax - wksEnv.YMin) / (double)(nDiv * 2.0);
				for( int i = 0; i < nDiv; ++i )		// Y方向
				{
					WKSPoint p;
					p.Y = (i == 0)? wksEnv.YMin + dYRange : wksEnv.YMin + (dYRange * (i * 2 + 1));
					for( int j = 0; j < nDiv; ++j )	// X方向
					{
						p.X = (j == 0)? wksEnv.XMin + dXRange : wksEnv.XMin + (dXRange * (j * 2 + 1));
						ipRepPoint->PutCoords( p.X, p.Y );

						// 背景チェック
						if( !IsNGSite(ipRepPoint) )
						{
							cRepPnt.m_strAddrCode += m_cConvTable.GetPntCode( cAdmin.m_strAddrCode, cAdmin.m_strExtGaiku );
							cRepPnt.SetCoord( ipRepPoint );
							rContainer.insert( std::pair<CString, CPntRec>(cRepPnt.m_strAddrCode, cRepPnt) );
							return;
						}
						else
						{
							// 最初の矩形内中点は、背景がNGの場所でもそれ以外で妥当なものがなかったら採用するので保持
							if( nDiv == 1 )
								ipKeepPoint = ipRepPoint;
						}
					}
				}
				nDiv *= 2;	// 矩形分割数増やす
			}
			// 妥当なものなかったら、最初の矩形内中点を代表点に採用
			if( ipKeepPoint )
			{
				cRepPnt.m_strAddrCode += m_cConvTable.GetPntCode( cAdmin.m_strAddrCode, cAdmin.m_strExtGaiku );
				cRepPnt.SetCoord( ipKeepPoint );
				rContainer.insert( std::pair<CString, CPntRec>(cRepPnt.m_strAddrCode, cRepPnt) );
			}
		}
		else
			progress_out::ProgressError<int>( 0, kFatal, "街区符号から地番代表点生成できません : %d", cAdmin.GetOID() );
	}
}

// 号レコードから地番代表点生成
void CMakeAddrRec::MakePntRecordByGou( const GOUMAP& rGouRec, PNTMAP& rContainer )
{
	CPntRec cPntRec;	// 一時変数
	GOULIST listGou, listTpg;	// GOUポイント, TPポイント
	CString strBaseCode;	// 処理対象16桁コード
	for( GOUMAP::const_iterator itrGou = rGouRec.begin(); itrGou != rGouRec.end(); ++itrGou )
	{
		// 既に地番代表点生成されている or 数字の地番のときは、無視
		if( !addr_func::IsNumStr(itrGou->first.Mid(11, 5)) && rContainer.find(itrGou->first.Left(16)) == rContainer.end() )
		{
			if( strBaseCode != itrGou->first.Left(16) )
			{
				if( !strBaseCode.IsEmpty() )
				{
					// 住所コード切り替わりのタイミングなので、地番代表点生成
					CGouRec cGouRec;
					long lRepDomariF = SelectPntFromGouList( listGou, listTpg, cGouRec );
					rContainer.insert( std::pair<CString, CPntRec>(strBaseCode, CPntRec(cGouRec, lRepDomariF)) );	// 号レコードから地番レコードに変換して登録
				}

				// リセット
				strBaseCode = itrGou->first.Left(16);
				listGou.clear(); listTpg.clear();
			}

			// GOU or TPGで分ける。
			if( 1 == itrGou->second.m_lGouType )	listGou.push_back( itrGou->second );
			else									listTpg.push_back( itrGou->second );
		}
	}

	// 最後に残ったレコードからも地番代表点生成
	if( !listGou.empty() || !listTpg.empty() )
	{
		CGouRec cGouRec;
		long lRepDomariF = SelectPntFromGouList( listGou, listTpg, cGouRec );
		rContainer.insert( std::pair<CString, CPntRec>(strBaseCode, CPntRec(cGouRec, lRepDomariF)) );
	}
}

// 号レコード作成
void CMakeAddrRec::MakeGouRecordByDB( const CAdminInfo& cAdmin, const CGouInfo& cGou, GOUMAP& rContainer )
{
	// 号番号が空 or ヨミが空(ヨミ取得失敗) or [*](号なし住所) なら無視
	if( cGou.GetGouNo() != _T("") && cGou.GetGouNoYomi() != _T("") && cGou.GetGouNo() != _T("*") )
	{
		// 文字付き住所？
		const EADDR_STATUS emAddrStatus = IsCharAddr( cAdmin, cGou );
		if( emAddrStatus != kNormal )
		{
			CGouRec cRec;
			cRec.m_lOID = cGou.GetOID();
			cRec.m_lGouType = cGou.m_lGouType;
			cRec.m_strAddrCode = cAdmin.m_strAddrCode;
			cRec.m_bPriorF = cGou.m_bPriorF;												// 優先
			cRec.m_bInArea = (cAdmin.m_lAreaClass_C == city_admin::area_class::kInArea);	// エリア内外

			// 20桁コード化開始
			if( !cGou.m_bExGouF && !cAdmin.m_strExtGaiku.IsEmpty() )	// 拡張街区符号あり(但し拡張号番号でないとき)
			{
				// 地番 : 拡張街区符号
				// 号   : 号番号
				cRec.m_strAddrCode.AppendFormat( _T("%05s%04s"), 
							 					 m_cConvTable.GetPntCode(cAdmin.m_strAddrCode, cAdmin.m_strExtGaiku),
												 (emAddrStatus == kExtGaiku)? cGou.GetGouNo() : m_cConvTable.GetGouCode(cAdmin.m_strAddrCode, cGou.GetGouNo()) );

				cRec.m_strPntName = cAdmin.m_strExtGaiku;
				cRec.m_strPntYomi = cAdmin.m_strExtGaikuYomi;
				cRec.m_strGouName = cGou.GetGouNo();
				cRec.m_strGouYomi = cGou.GetGouNoYomi();
			}
			else
			{
				if( !cGou.m_bExGouF && cAdmin.m_lGaikuFugo > 0 )	// 街区符号あり
				{
					// 地番 : 街区符号// 号   : 号番号
					cRec.m_strAddrCode.AppendFormat( _T("%05d%04s"), cAdmin.m_lGaikuFugo, m_cConvTable.GetGouCode(cAdmin.m_strAddrCode, cGou.GetGouNo()) );
					CString strGaikuFugo;
					strGaikuFugo.Format( _T("%d"), cAdmin.m_lGaikuFugo );
					bool bRet = false;
					cRec.m_strPntName = strGaikuFugo;
					cRec.m_strPntYomi = strGaikuFugo;
					cRec.m_strGouName = cGou.GetGouNo();
					cRec.m_strGouYomi = cGou.GetGouNoYomi();
				}
				else												// 街区符号なし
				{
					// 地番 : 号番号(区切りあるときは、最初の区切りまで)
					// 号   : 号番号(区切りあるときは、最初の区切り以降)
					CString strPntName = cGou.GetTokenCount() == 1? cGou.GetGouNo() : cGou.GetGouNo(0, 1);
					CString strGouName = cGou.GetTokenCount() == 1? _T("") : cGou.GetGouNo(1);
					
					cRec.m_strAddrCode.AppendFormat( _T("%05s%04s"), 
												addr_func::IsNormalPnt(strPntName)? strPntName : m_cConvTable.GetPntCode(cAdmin.m_strAddrCode, strPntName), 
												(addr_func::IsNormalGou(strGouName) || strGouName.IsEmpty())? strGouName : m_cConvTable.GetGouCode(cAdmin.m_strAddrCode, strGouName) );

					cRec.m_strPntName = strPntName;
					cRec.m_strPntYomi = cGou.GetTokenCount() == 1? cGou.GetGouNoYomi() : cGou.GetGouNoYomi(0, 1);
					cRec.m_strGouName = strGouName;
					cRec.m_strGouYomi = cGou.GetTokenCount() == 1? _T("") : cGou.GetGouNoYomi(1);
				}		
			}

			cRec.SetCoord( cGou.GetGeometry() );
			cRec.m_dKakeiArea = GetContainsBldArea( cGou.GetGeometry() );	// 号ポイントが家形内にあるかも調べる
			cRec.m_strZipCode = GetZipCode( cAdmin.m_strAddrCode );			// 該当11桁コードから郵便番号も取得
			rContainer.insert( std::pair<CString, CGouRec>(cRec.m_strAddrCode, cRec) );		// レコード追加

			// 通称行政界のとき、ADDRCODE2でもう一つ作成
			if( cAdmin.m_strOazaAzaCode2 != _T("000000") )
			{
				// [Bug7068]11桁コードで再度コード割り当てを行う。
				CString strTmpAdminCode = cRec.m_strAddrCode.Left(5) + cAdmin.m_strOazaAzaCode2;

				cRec.m_strAddrCode.Format( _T("%s%05s%04s"), 
										   strTmpAdminCode, 
										   addr_func::IsNormalPnt(cRec.m_strPntName)? cRec.m_strPntName : m_cConvTable.GetPntCode(strTmpAdminCode, cRec.m_strPntName), 
										   (addr_func::IsNormalGou(cRec.m_strGouName) || cRec.m_strGouName.IsEmpty())? cRec.m_strGouName : m_cConvTable.GetGouCode(strTmpAdminCode, cRec.m_strGouName) );
				cRec.m_bTsushoConv = true;
				rContainer.insert( std::pair<CString, CGouRec>(cRec.m_strAddrCode, cRec) );	// レコード追加
			}
		}
	}
}

// 号レコード一覧から代表住所にフィルタリング
void CMakeAddrRec::SelectRepGouRecord( const GOUMAP& rGouMap, GOUMAP& rContainer )
{
	typedef GOUMAP::const_iterator GOUMAP_ITR;
	for( GOUMAP_ITR itr = rGouMap.begin(); itr != rGouMap.end(); ++itr )
	{
		// 同一20桁コード何件あるか確認
		if( rContainer.find(itr->first) == rContainer.end() )
		{
			if( rGouMap.count(itr->first) == 1 )
			{
				rContainer.insert( std::pair< CString, CGouRec>(itr->first, itr->second) );	// 1件ならそれが代表住所
			}
			else
			{
				GOULIST listPrior, listGou;
				std::pair< GOUMAP_ITR, GOUMAP_ITR> itrPair = rGouMap.equal_range( itr->first );
				for( ; itrPair.first != itrPair.second; ++itrPair.first )
				{
					// 優先号(通称行政界での生成は優先としない)と優先でない号で場合わけ
					if( itrPair.first->second.m_bPriorF && !itrPair.first->second.m_bTsushoConv )
						listPrior.push_back( itrPair.first->second );
					else
						listGou.push_back( itrPair.first->second );	
				}

				// 優先号の数によって処理分ける
				switch( listPrior.size() )
				{
				case 1:	// 優先フラグ一つならそれが代表住所
					rContainer.insert( std::pair<CString, CGouRec>(itr->first, *(listPrior.begin())) );
					break;
				case 0:	// ソート後、一番先頭に来たものが代表住所
					std::sort( listGou.begin(), listGou.end(), addr_func::CompareGouRec );
					rContainer.insert( std::pair<CString, CGouRec>(itr->first, *(listGou.begin())) );
					break;
				default:	// 優先号複数のときは、エラー
					for( GOULIST::const_iterator itr = listPrior.begin(); itr != listPrior.end(); ++itr )
					{
						object_out::ObjectError( gou_point::kTableName, itr->m_lOID, _T("同一20桁で優先号が複数\t%s"), itr->m_strAddrCode );
					}
					break;
				}

				// メインイテレータも同一20桁数分スキップ(同じ処理を再度やらなくてよいので)
				std::advance( itr, listPrior.size() + listGou.size() -1 );
			}
		}
	}
}

//////////////////////////////////////////////
//	内部関数
//////////////////////////////////////////////

// 文字付き住所？
EADDR_STATUS CMakeAddrRec::IsCharAddr( const CAdminInfo& cAdmin, const CGouInfo& cGou )
{
	// 号番号の先頭[#]なら文字付き
	if( cGou.m_bMoji )
		return !cGou.m_bExGouF && !cAdmin.m_strExtGaiku.IsEmpty()? kMojiGou_ExtGaiku : kMojiGou;

	if( !cGou.m_bExGouF && !cAdmin.m_strExtGaiku.IsEmpty() )	// 拡張街区符号あり
	{
		// 区切りが2つ以上なら文字付き号
		if( cGou.GetTokenCount() > 1 )
			return kSepOver_ExtGaiku;

		// 号が10,000以上なら文字付き号
		return (_tstol(cGou.GetGouNo(0, 1)) > 9999)? kNoOver_ExtGaiku : kExtGaiku;
	}
	else														// 拡張街区符号なし
	{
		// 拡張号でない+街区符号ありで区切りが2つ以上なら文字付きとして扱う
		// 拡張号で区切りが2つ以上なら文字付きとして扱う(Bug7287)
		if( (!cGou.m_bExGouF && cAdmin.m_lGaikuFugo > 0 && cGou.GetTokenCount() > 1) || (cGou.m_bExGouF && cGou.GetTokenCount() > 2) )
			return kSepOver;

		// 街区符号なしで区切りが3つ以上なら文字付きとして扱う
		if( cAdmin.m_lGaikuFugo == 0 && cGou.GetTokenCount() > 2 )
			return kSepOver;

		// 最後に地番が100,000以上 号が10,000以上なら文字付きとして扱う
		if( !cGou.m_bExGouF && cAdmin.m_lGaikuFugo > 0 )	// 街区符号あり
		{
			return (_tstol(cGou.GetGouNo(0, 1)) > 9999)? kNoOver : kNormal;
		}
		else												// 街区符号なし
		{
			return (_tstol(cGou.GetGouNo(0, 1)) > 99999) 
					|| (cGou.GetTokenCount() > 1 && _tstol(cGou.GetGouNo(1, 2)) > 9999) ?
					kNoOver : kNormal;
		}
	}
}

// プロットNGな背景？
bool CMakeAddrRec::IsNGSite( IPoint* ipPoint )
{
	CComVariant vaValue;
	IFeaturePtr ipFeature;
	long lSiteCount = 0;
	IFeatureCursorPtr ipSiteCursor = gf::SelectByShape( ipPoint, m_ipCitySiteFC, &lSiteCount, esriSpatialRelWithin, NULL, city_site::kBgClass );
	if( lSiteCount > 0 && ipSiteCursor )
	{
		while( ipSiteCursor->NextFeature(&ipFeature) == S_OK )
		{
			ipFeature->get_Value( m_lBGClass_FIdx, &vaValue );
			for( int  i = 0; g_lNGSiteClass[i] != -1; ++i )
			{
				if( g_lNGSiteClass[i] == vaValue.lVal )
					return true;
			}
		}
		return false;
	}
	else
	{
		// 都市地図エリア内なら細街路なので、NG
		long lMeshCount = 0;
		IFeatureCursorPtr ipMeshCursor = gf::SelectByShape( ipPoint, m_ipCityMeshFC, &lMeshCount, esriSpatialRelWithin, NULL, citymesh::kCreateYear );
		if( lMeshCount > 0 && ipMeshCursor )
		{
			while( ipMeshCursor->NextFeature(&ipFeature) == S_OK )
			{
				ipFeature->get_Value( m_lCreateYear_FIdx, &vaValue ); 
				if( vaValue.vt != VT_NULL )
					return true;	// 細街路になるのでNG
			}
		}
		return false;
	}
}

// 家形ポリゴンから屋上ポリゴンの面積取得
double CMakeAddrRec::GetRoofArea( const IGeometryPtr& ipBldGeometry )
{	
	double roofArea = 0;
	IFeatureCursorPtr ipFeaturecursor = gf::SelectByShape( ipBldGeometry, m_ipBuildingRoofFC );
	if( ipFeaturecursor )
	{
		IFeaturePtr ipFeature;
		while( ipFeaturecursor->NextFeature(&ipFeature) == S_OK && ipFeature )
		{
			IGeometryPtr ipGeo;
			ipFeature->get_Shape( &ipGeo );
			
			double tmpArea = 0;
			((IAreaPtr)ipGeo)->get_Area( &tmpArea );
			roofArea += tmpArea;
		}
	}

	return roofArea;
}

// 号ポイントが存在する家形の面積取得
double CMakeAddrRec::GetContainsBldArea( const IPointPtr& ipPoint )
{
	if( ipPoint )
	{
		IFeatureCursorPtr ipFeatureCursor = gf::SelectByShape( ipPoint, m_ipBuildingFC, NULL, esriSpatialRelWithin );
		if( ipFeatureCursor )
		{
			double dTmpArea = 0.0, dMaxArea = 0.0, dRoofArea = 0.0;
			IFeaturePtr ipFeature;
			while( ipFeatureCursor->NextFeature(&ipFeature) == S_OK && ipFeature )
			{
				IGeometryPtr ipGeo;
				ipFeature->get_Shape( &ipGeo );
				
				((IAreaPtr)ipGeo)->get_Area( &dTmpArea );
				// もしBuilding_Roofがあればその分の面積は引かなくてはいけない
				dRoofArea = GetRoofArea( ipGeo );
				dTmpArea -= dRoofArea;
				if( dMaxArea < dTmpArea )
					dMaxArea = dTmpArea;
			}
			return dMaxArea;
		}
	}
	return 0.0;
}

// 号レコードから地番代表点となりうる号ポイント選択
long CMakeAddrRec::SelectPntFromGouList( const GOULIST& listGou, const GOULIST& listTpg, CGouRec& rGouRec, bool bOazaIgnore /* = true */ )
{
	const GOULIST& listTarget = (!listGou.empty())? listGou : listTpg;
	GOULIST listPriorChiban, listChiban, listNotChiban, listOazaMode;
	
	// 地番止まりフラグポイントを取得
	for( GOULIST::const_iterator itr = listTarget.begin(); itr != listTarget.end(); ++itr )
	{
		// [Bug7068] 通称行政界による生成は地番代表点生成に基本的に使用しない
		if( ! itr->m_bTsushoConv || ! bOazaIgnore )
		{
			if( itr->m_strAddrCode.Right(4) == _T("0000") )
			{
				// 地番止まり内をさらに優先号で場合わけ
				if( itr->m_bPriorF )	listPriorChiban.push_back( *itr );
				else					listChiban.push_back( *itr );
			}
			else
				listNotChiban.push_back( *itr );
		}
		else
			listOazaMode.push_back( *itr );
	}

	long lChibanCount    = (signed)listChiban.size() + (signed)listPriorChiban.size(); // 地番止まり件数
	long lNotChibanCount = (signed)listNotChiban.size();                               // 地番止まりでない件数
	long lOazaModeCount  = (signed)listOazaMode.size();                                // 大字モード件数
	long lTpgCount       = 0;                                                          // TPG由来の件数(号由来が存在する場合のみ使用)

	// 号由来とTPG由来が混在する際、号由来のみでフラグ判定を行っているため不整合となっている問題解決のため、TPG由来の件数を取得
	if( !listGou.empty() )
		lTpgCount = (signed)listTpg.size();

	if( lChibanCount == 1 )	// 地番止まり1点
	{
		// 該当ポイントを選択
		rGouRec = !listChiban.empty()? *(listChiban.begin()) : *(listPriorChiban.begin());

		// 地番フラグ設定時は、listOazaModeとTPG由来の件数も考慮しないとおかしくなるため、そのように修正 2013/01/23
		return (lNotChibanCount > 0 || lOazaModeCount > 0 || lTpgCount > 0 )? 4 : 2;	// 同一16桁で複数あり[代表点-複数 地番止まり-ON]・同一16桁で複数なし[代表点-OFF  地番止まり-ON]
	}
	else if( lChibanCount > 1 )	// 地番止まり複数
	{
		if( !listPriorChiban.empty() )	// 優先ありならそれを選択(代表点-複数 地番止まり-ON)
		{
			rGouRec = *(listPriorChiban.begin());
			return 4;

		}
		else	// 優先なしなら地番止まりポイント郡・地番止まりでないポイント郡(号)から代表点を決定する( 代表点-複数 地番止まり-ON )
		{
			if( lNotChibanCount == 0 )
			{
				if( lChibanCount == 2 )	// 地番止 2点 地番止でない 0点
				{
					rGouRec = addr_func::CompareGouRecPos(*(listChiban.begin()), *(++listChiban.begin())) <= 0? *(listChiban.begin()) : *(++listChiban.begin());
					return 4;
				}
				else					// 地番止 3点以上 地番止でない 0点
				{
					SelectPntFromGouDist( listChiban, listNotChiban, rGouRec );	// 距離から算出
					return 4;
				}
			}
			else
			{
				// 地番止 複数 地番止でない 1点以上
				SelectPntFromGouDist( listChiban, listNotChiban, rGouRec );	// 距離から算出
				return 4;
			}
		}
	}
	else	// 地番止まりポイントなし
	{
		if( lNotChibanCount == 1 )	// 地番止でない 1点
		{
			// 当該ポイント選択
			rGouRec = *(listNotChiban.begin());

			// 地番フラグ設定時は、listOazaModeとTPG由来の件数も考慮しないとおかしくなるため、そのように修正 2013/01/23
			return (lOazaModeCount > 0 || lTpgCount > 0)? 5 : 3;	// 同一16桁で複数あり[代表点-複数 地番止まり-OFF]・同一16桁で複数なし[代表点-OFF  地番止まり-OFF]
		}
		else if( lNotChibanCount == 2 )		// 地番止でない 2点
		{
			// 地番止まりでないポイント郡(号)から代表点を決定する( 代表点-複数 地番止まり-OFF )
			rGouRec = addr_func::CompareGouRecPos( *(listNotChiban.begin()), *(++listNotChiban.begin())) <= 0? *(listNotChiban.begin()) : *(++listNotChiban.begin());
			return 5;
		}
		else if( lNotChibanCount > 2 )		// 地番止でない 3点以上
		{
			// 地番止まりでないポイント郡(号)から代表点を決定する( 代表点-複数 地番止まり-OFF )
			SelectPntFromGouDist( listChiban, listNotChiban, rGouRec );	// 距離から算出
			return 5;
		}
		else	// 地番止でない 0点
		{
			// 大字も生成によるポイント郡(号)から地番代表点決める
			return SelectPntFromGouList( listOazaMode, listTpg, rGouRec, false );
		}
	}
}

// 号レコードの位置関係から地番代表点となりうる号ポイント選択
void CMakeAddrRec::SelectPntFromGouDist( const GOULIST& listChiban, const GOULIST& listNotChiban, CGouRec& rGouRec )
{
	// 地番止まりあればそれを優先
	const GOULIST& listTarget = !listChiban.empty()? listChiban : listNotChiban;

	// １点を元に各点間距離の総和が一番小さいものがより中央にあるとして代表点にする
	double dMinDist = -1.0;
	for( GOULIST::const_iterator itrTarget = listTarget.begin(); itrTarget != listTarget.end(); ++itrTarget )
	{
		double dSumDist = 0.0;
		// 地番止まりポイント郡
		for( GOULIST::const_iterator itrChiban = listChiban.begin(); itrChiban != listChiban.end(); ++itrChiban )
		{
			if( itrTarget->m_lOID != itrChiban->m_lOID )	dSumDist += addr_func::GetDistGouRec( *itrTarget, *itrChiban );
		}

		// 地番止まりでないポイント郡
		for( GOULIST::const_iterator itrChiban = listNotChiban.begin(); itrChiban != listNotChiban.end(); ++itrChiban )
		{
			if( itrTarget->m_lOID != itrChiban->m_lOID )	dSumDist += addr_func::GetDistGouRec( *itrTarget, *itrChiban );
		}

		if( dMinDist == -1.0 || dMinDist >= dSumDist )
		{
			// もし等しいときは、より東・より北を優先
			if( dMinDist != dSumDist || (dMinDist == dSumDist && addr_func::CompareGouRecPos(rGouRec, *itrTarget) == 1) )
			{				
				dMinDist = dSumDist;
				rGouRec = *itrTarget;
			}
		}
	}
}
