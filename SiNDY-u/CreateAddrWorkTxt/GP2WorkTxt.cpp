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
#include "GP2WorkTxt.h"

#include <AddrLib/GouPoints.h>


// 初期化
bool CGP2WorkTxt::Init()
{
	// 実行対象市区町村コード生成
	m_cAddrCodeList.SetObject( m_rParam.m_cCityAdminFC );

	if( m_rParam.m_pairArea.first == _T("-ca")  )
	{
		if( !m_cAddrCodeList.MakeCodeListAll(5) )
		{
			std::wcerr << L"#Error 全国分の市区町村コードリスト生成に失敗" << std::endl;
			return false;
		}
	}
	else if( m_rParam.m_pairArea.first == _T("-cl")  )
	{
		if( !m_cAddrCodeList.AddFromFile(m_rParam.m_pairArea.second, 5) )
		{
			std::wcerr << L"#Error 市区町村リストからの市区町村コードリスト生成に失敗 : " << m_rParam.m_pairArea.second << std::endl;
			return false;
		}
	}
	else if( m_rParam.m_pairArea.first == _T("-cr") )
	{
		CString strRange( m_rParam.m_pairArea.second );
		int nPos = strRange.Find( _T('-') ) ;
		if( nPos > 0 )
		{
			if( !m_cAddrCodeList.MakeCodeListFromRange(strRange.Left(nPos), strRange.Mid(nPos+1)) )
			{
				std::wcerr << L"#Error 市区町村リストからの市区町村コードリスト生成に失敗 : " << m_rParam.m_pairArea.second << std::endl;
				return false;
			}
		}
		else
		{
			std::wcerr << L"#Error 市区町村コード指定フォーマットが不正です。(start-end) : " << m_rParam.m_pairArea.second << std::endl;
			return false;
		}
	}
	else if( m_rParam.m_pairArea.first == _T("-cs") )
	{
		m_cAddrCodeList.Add( m_rParam.m_pairArea.second );
	}
	else
	{
		std::wcerr << L"#Error 実行エリアが特定できません : " << m_rParam.m_pairArea.first << std::endl;
		return false;
	}

	return true;
}

// データ出力
bool CGP2WorkTxt::WriteData()
{
	CGouPoints cGous( m_rParam.m_cCityAdminFC );
	for( CAddrCodeList::const_iterator itCode = m_cAddrCodeList.begin(); itCode != m_cAddrCodeList.end(); ++itCode )
	{
		std::cout << "#" << CT2CA(*itCode) << std::endl;

		cGous.SelectByAddrCode( *itCode );
		if(! cGous.empty(cGous.GetTableType()) )
		{
			CString strFile;
			strFile.Format( _T("%s\\gpw%s.txt"), m_rParam.m_strOutDir, *itCode );
			std::ofstream ofs( strFile, std::ios::binary );
			if( ofs )
			{
				long lCount = 1;
				for( CCityAdmin::const_rows_iterator itAdmin = cGous.begin(cGous.GetTableType()); itAdmin != cGous.end(cGous.GetTableType()); ++itAdmin, lCount++ )
				{
					// 行政界内にある号ポイント取得
					const boost::shared_ptr<CCityAdmin> pAdmin = boost::static_pointer_cast<CCityAdmin>(*itAdmin);

					// 住居表示整備済で街区符号がNULLになっているエリアはデータ化しない
					if( pAdmin->GetGaikuFugo() == 0 
						&& (pAdmin->GetAddrClass() == city_admin::addr_class::kChibanMaintenance || pAdmin->GetAddrClass() == city_admin::addr_class::kResidenceIndicationMaintenance) )
					{
						PrintError( pAdmin.get(), _T("NOTICE"), _T("住居表示整備済で街区符号がNULLなのでデータ化しません"), pAdmin->GetAddrCode() );
						continue;
					}

					for( CGouPoint::const_rows_iterator itGou = pAdmin->begin(sindyTableType::gou_point); itGou != pAdmin->end(sindyTableType::gou_point); ++itGou )
					{
						const boost::shared_ptr<CGouPoint> pGou = boost::static_pointer_cast<CGouPoint>(*itGou);

						// 住所コード確認
						//   ・20桁化できないものは、無視(地番なし住所等)
						//   ・大字・字コード[999999]のものは、無視(bug7975)
						CString strAddrCode = pGou->GetAddrCode();
						if( strAddrCode.IsEmpty() )
						{
							CString strInfo;
							strInfo.Format( _T("行政界コード:%s ＋ 号番号:%s"), pAdmin->GetAddrCode(), pGou->GetGouNo() );
							PrintError( pGou.get(), _T("WARNING"), _T("20桁コード化失敗"), strInfo );
							continue;
						}
						else if( strAddrCode.Mid(5, 6) == _T("999999") )
						{
							PrintError( pGou.get(), _T("NOTICE"), _T("大字・字コード[999999]の行政界エリアの為、データ化しません"), strAddrCode );
							continue;
						}

						// メッシュコード・経度・緯度取得
						WKSPoint point;
						IGeometryPtr ipGeo;
						((IFeature*)*pGou)->get_Shape( &ipGeo );
						((IPointPtr)ipGeo)->QueryCoords( &point.X, &point.Y );

						// データ書き出し
						// [ObjectID],[住所コード],[号タイプ],[行政界エリア種別],[優先フラグ],[メッシュコード],[10進経度],[10進緯度]
						CString strData;
						strData.Format( _T("%d,%s,%c,%d,%d,%d,%.12f,%.12f"), 
										pGou->GetOID(),								// OID
										strAddrCode,								// 住所コード
										pGou->GetGouType() == 1? _T('g') : _T('t'),	// GouType
										pAdmin->GetAreaClass(),						// 行政界エリア種別
										pGou->IsPrior()? 1 : 0,						// 優先号
										GetMeshCode(point.X, point.Y), point.X, point.Y	);				// メッシュコード座標
						ofs << CT2CA(strData) << "\n";

					}
					ofs.flush();
					std::cout << lCount << " / " << cGous.size(cGous.GetTableType()) << " 行政界\r";
				}
				std::cout << std::endl;
			}
			else
				std::cerr << "#Error 出力ファイルの作成に失敗 : " << CT2CA(strFile) << std::endl;
		}
	}
	return true;
}
