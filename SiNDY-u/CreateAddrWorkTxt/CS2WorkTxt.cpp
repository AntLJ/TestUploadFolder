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
#include "CS2WorkTxt.h"

#include <AddrLib/CSAddrPoint.h>

using namespace addr;

// データ出力
bool CCS2WorkTxt::WriteData()
{
	long lAllCount = m_rParam.m_cPointFC.FeatureCount( NULL );
	IFeatureCursorPtr ipCursor( m_rParam.m_cPointFC.Search(NULL, VARIANT_FALSE) );
	if( lAllCount > 0 && ipCursor )
	{
		// 出力ファイルオープン
		std::ofstream ofs( m_rParam.m_strOutFile, std::ios::out | std::ios::binary );
		if( !ofs )
		{
			std::cerr << "#Error 出力ファイルのオープン失敗 : " << CT2CA(m_rParam.m_strOutFile) << std::endl;
			return false;
		}


		CSPFieldMap cFields( m_rParam.m_cPointFC.GetFieldMap() );
		CSPTableNameString cNames( m_rParam.m_cPointFC.GetNameString() );

		long lCount = 1;
		IFeaturePtr ipFeature;
		while( ipCursor->NextFeature(&ipFeature) == S_OK )
		{
			CCSAddrPoint cCSPoint( ipFeature, sindyTableType::cs_addr_point, false, cFields, cNames );

			const CCityAdmin* pAdmin = cCSPoint.GetCityAdmin( (ITable*)m_rParam.m_cCityAdminFC );
			if( pAdmin )
			{
				city_admin::area_class::ECode eAreaClass = pAdmin->GetAreaClass();
				if( eAreaClass == city_admin::area_class::kSea )
				{
					// 海なのでエラー
					continue;
				}

				// メッシュコード・経度・緯度取得
				WKSPoint point;
				IGeometryPtr ipGeo;
				((IFeature*)cCSPoint)->get_Shape( &ipGeo );
				((IPointPtr)ipGeo)->QueryCoords( &point.X, &point.Y );

				CString strData;
				strData.Format( _T("%d,%s,%d,%d,%.12f,%.12f"), 
								cCSPoint.GetOID(),			// ObjectID
								cCSPoint.GetAddrCode(),		// 20桁住所コード
								eAreaClass,					// 行政界エリア種別
								GetMeshCode(point.X, point.Y), point.X, point.Y );	// メッシュコード座標

				ofs << CT2CA(strData) << "\n";
				ofs.flush();
			}
			else
				PrintError( &cCSPoint, _T("ERROR"), _T("行政界が取得できない or 境界上にある") );

			std::cout << "#" << lCount++ << " / " << lAllCount << " 件完了\r";
		}		

		return true;
	}
	else
		return false;
}
