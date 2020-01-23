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
#include "MakeList.h"

using namespace sindy::schema;

CMakeList::CMakeList(void)
{
}

CMakeList::~CMakeList(void)
{
}

//////////////////////////////////////////////////////////////////////
// 駐禁取締路線をリスト化する。
//////////////////////////////////////////////////////////////////////
bool CMakeList::create( IFeatureClass* ipLinkClass, const std::vector<CAdapt<IFeatureClassPtr>>& vecFeatureClass, bool bShape, std::ofstream& ofp, LPCTSTR lpcszShapeDir, LPCTSTR lpcszLinkDir )
{
	// 取得したフィーチャクラスでループさせる
	m_lCount = 0;
	m_lFeatureClassSize = vecFeatureClass.size();
	m_strShapeDir = lpcszShapeDir;
	m_strLinkDir = lpcszLinkDir;

	std::cerr << "◆駐禁取締路線リスト取得処理中...\n";
	for( std::vector<CAdapt<IFeatureClassPtr>>::const_iterator it = vecFeatureClass.begin(); it != vecFeatureClass.end(); ++it )
	{
		m_lCount++;
		std::cerr << "\t◇IFeatureClass数 " << m_lCount << " / " << m_lFeatureClassSize <<  " 件\n";
		// shapeの処理
		if( bShape )
		{
			CComBSTR bstrName;
			IDatasetPtr(it->m_T)->get_Name( &bstrName );
			bool bFlag = false;	// 最重要路線かどうかはフィーチャクラス名で判別。フィーチャクラス取得時に判別してもいいんだけどね...
			if( _tcsicmp( CString(bstrName), NoParkingRegR ) == 0 )
				bFlag = false;	// 重要路線
			else if( _tcsicmp( CString(bstrName), NoParkingRegRImp ) == 0 )
				bFlag = true;	// 最重要路線
			else
			{
				// ここに来ることはないはずだけど、一応
				std::cerr << "# ERROR\t"<< CT2A(CString(bstrName)) <<"：想定外のレイヤを取得しています。\n";
				return false;
			}

			if( !SearchClass( ipLinkClass, it->m_T, _T(""), ofp, bFlag ) )
				return false;
		}
		// PGDBの処理
		else
		{
			// 最重点取締路線
			if( !SearchClass( ipLinkClass, it->m_T, _T("AttrCode = '911@'"), ofp, true ) )
				return false;

			// 重点取締路線
			if( !SearchClass( ipLinkClass, it->m_T, _T("AttrCode = '912@'"), ofp, false ) )
				return false;
		}
	}
	std::cerr << "◆駐禁取締路線リスト取得処理終了\n";

//#ifdef _DEBUG
	// 駐禁取締路線リストをshape出力
	if( !m_mapNoParkingLink.empty() && !m_strShapeDir.IsEmpty() )
	{
		std::cerr << "◆駐禁取締路線リストshape出力中...\n";
		if( OutputShape( ipLinkClass ) )
			std::cerr << "◆駐禁取締路線リストshape出力終了\n";
		else
		{
			std::cerr << "◆駐禁取締路線リストshape出力終了\n";
			return false;
		}
	}


	// 駐車禁止取締エリアのラインをshapeに出力
	// 都道府県別のshapeになっているので元データをまとめて出力させる
	// PGDBの場合はあまり意味がない
	if( !m_vecBaseLink.empty() && !m_strLinkDir.IsEmpty() )
	{
		std::cerr << "◆駐車禁止取締区間のラインのshape出力中...\n";
		OutputBaseLink( m_vecBaseLink );
		std::cerr << "◆駐車禁止取締区間のラインのshape出力終了\n";
	}
//#endif
	return true;
}

//////////////////////////////////////////////////////////////////////
// 駐車禁止情報取得処理を行う。
//////////////////////////////////////////////////////////////////////
bool CMakeList::SearchClass( IFeatureClass* ipLinkClass, IFeatureClass* ipNoParkClass, LPCTSTR lpcszWhereClause, std::ofstream &ofp, bool bFlag )
{
	IQueryFilterPtr ipQFilter(CLSID_QueryFilter);
	if( !CString(lpcszWhereClause).IsEmpty() )	// WhereClauseがなければスルー
		ipQFilter->put_WhereClause( CComBSTR(lpcszWhereClause) );

	long lCount = 0, lTotalCount = -1;
	HRESULT hr = ITablePtr(ipNoParkClass)->RowCount( ipQFilter, &lTotalCount );
	if( S_OK == ipNoParkClass->FeatureCount( ipQFilter, &lTotalCount ) && !( lTotalCount < 0 ) )
	{
		IFeatureCursorPtr ipFeatureCursor;
		if( S_OK == ipNoParkClass->Search( ipQFilter, VARIANT_FALSE, &ipFeatureCursor ) )
		{
			IFeaturePtr ipFeature;
			while( S_OK == ipFeatureCursor->NextFeature( &ipFeature ) )
			{
				++lCount;
				std::cerr << "\t\t" << ((bFlag) ? "最重点取締路線" : "重点取締路線") << "作成中...\t"<< lCount  << " / " << lTotalCount << " 件\r";

				IGeometryPtr ipGeom = ipFeature->GetShapeCopy();
				if( ipGeom )
				{
					// フィーチャ取得（あとでshape出力するので）
					m_vecBaseLink.push_back( ipFeature );

					// 20mバッファを作成する
					ISpatialReferencePtr ipRef =((IGeoDatasetPtr)ipLinkClass)->GetSpatialReference();
					if( ipRef )
					{
						IGeometryPtr ipBuffer = GetBuffer( ipGeom );
						if( ipBuffer )
						{
							// バッファに含まる道路リンクを取得する。
							long lNoParking_c = (bFlag) ? 1 : 2;
							if( !CompData( ipLinkClass, ipBuffer, ofp, lNoParking_c ) )
								return false;
						}
						else
						{
							std::cout << "# ERROR\tSearchClass()\tフィーチャ形状バッファ作成失敗\n";
							return false;
						}
					}
					else
					{
						std::cout << "# ERROR\tSearchClass()\t空間参照取得失敗\n";
						return false;
					}
				}
				else
				{
					std::cout << "# ERROR\tSearchClass()\tフィーチャコピー作成失敗\n";
					return false;
				}
			}
		}
		else
		{
			std::cout << "# ERROR\tSearchClass()\tフィーチャ検索失敗\n";
			return false;
		}
	}
	else
	{
		std::cout << "# ERROR\tSearchClass()\tフィーチャカウント数取得失敗\n";
		return false;
	}
	std::cerr << "\n";

	return true;
}

IGeometryPtr CMakeList::get_Tokyo10CloneGeom(IGeometry* ipGeom)
{
	ISpatialReferenceFactory2Ptr ipSpRefFct(CLSID_SpatialReferenceEnvironment);
	ISpatialReferencePtr ipSpRef;
	ipSpRefFct->CreateSpatialReference(esriSRProjCS_TokyoJapan10, &ipSpRef);

	IClonePtr ipClone;
	((IClonePtr)ipGeom)->Clone(&ipClone);
	IGeometryPtr ipCloneGeom(ipClone);

	// IGeometry::Project()をする前に、空間参照がセットされているかチェックする
	ISpatialReferencePtr ipCheckSpRef;
	ipCloneGeom->get_SpatialReference(&ipCheckSpRef);
	long lFcCode = 0;
	ipCheckSpRef->get_FactoryCode(&lFcCode);
	if(lFcCode == 0){
		// 空間参照がセットされていない形状の場合はデフォルト値を指定してあげる
		ISpatialReferencePtr ipDefSpRef;
		ipSpRefFct->CreateSpatialReference(esriSRGeoCS_Tokyo, &ipDefSpRef);
		ipCloneGeom->putref_SpatialReference(ipDefSpRef);
	}
	ipCloneGeom->Project(ipSpRef);

	return ((IGeometryPtr)ipClone);
}

//////////////////////////////////////////////////////////////////////
// リンクに沿って20mバッファを作成する。
//////////////////////////////////////////////////////////////////////
IGeometryPtr CMakeList::GetBuffer(IGeometry* ipGeom)
{
	IGeometryPtr ipLinkBuffer;
	ITopologicalOperatorPtr ipTopo( get_Tokyo10CloneGeom(ipGeom) );
	ipTopo->Buffer( 20, &ipLinkBuffer );

	// Arc10.3.1では構成点が少なく曲線が多用されたバッファが作成されるため、
	// Densify()を実施してArc10.1に近づける
	// Densify()の第2パラメータは、最少(2 * XYResolution)とデフォルト(2 * XYResolution * 100)
	// の間を取り、(2 * XYResolution * 10)とする
	ISpatialReferencePtr ipSpRef;
	ipLinkBuffer->get_SpatialReference(&ipSpRef);
	double dXYResolution = 0.0;
	ISpatialReferenceResolutionPtr(ipSpRef)->get_XYResolution(VARIANT_FALSE, &dXYResolution);
	double dMaxDeviation = 2 * dXYResolution * 10;
	if (FAILED(IPolygonPtr(ipLinkBuffer)->Densify(0.0, dMaxDeviation)))
		return NULL;

	return ipLinkBuffer;
}

//////////////////////////////////////////////////////////////////////
// バッファにひっかかるリンクをリストアップする。
//////////////////////////////////////////////////////////////////////
bool CMakeList::CompData( IFeatureClass* ipLinkClass, IGeometry* ipBuff, std::ofstream &ofp, long lClass )
{
	ISpatialFilterPtr ipSpatialFilter(CLSID_SpatialFilter);
	ipSpatialFilter->putref_Geometry(ipBuff);
	ipSpatialFilter->put_SpatialRel( esriSpatialRelContains );

	IFeatureCursorPtr	ipCursor, ipCursor2;
	IFeaturePtr			ipLink, ipLink2;
	std::set<long>		setOID;

	///////////////////////////////////////////////
	// ここの処理、いらなくね？？
	// バッファに完全内包のリンクを抽出する。
	IFeatureCursorPtr ipContainCursor;
	if( S_OK == ipLinkClass->Search( ipSpatialFilter, VARIANT_FALSE, &ipContainCursor ) ) 
	{
		IFeaturePtr ipContainLink;
		while( ipContainCursor->NextFeature( &ipContainLink ) == S_OK )
		{
			CComVariant	vaClass	= ipContainLink->GetValue( ipContainLink->Fields->_FindField(road_link::kRoadClass) );
			// 有料ではない基本道路以外は対象外
			if( vaClass.lVal == 0 || vaClass.lVal > 7 )
				continue;
			setOID.insert(ipContainLink->GetOID());
		}
	}
	else
	{
		std::cout << "# ERROR\tCompData()\tバッファに完全内包のリンク検索失敗\n";
		return false;
	}
	///////////////////////////////////////////////

	// バッファに重なっているリンクを抽出する。
	ipSpatialFilter->put_SpatialRel( esriSpatialRelIntersects );
	IFeatureCursorPtr ipIntersectCursor;
	if( S_OK == ipLinkClass->Search( ipSpatialFilter, VARIANT_FALSE, &ipIntersectCursor ) )
	{
		IFeaturePtr ipIntersectLink;
		while( ipIntersectCursor->NextFeature( &ipIntersectLink ) == S_OK )
		{
			CComVariant	vaClass	= ipIntersectLink->GetValue(ipIntersectLink->Fields->_FindField(road_link::kRoadClass));
			CComVariant	vaFrom	= ipIntersectLink->GetValue(ipIntersectLink->Fields->_FindField(road_link::kFromNodeID));
			CComVariant	vaTo	= ipIntersectLink->GetValue(ipIntersectLink->Fields->_FindField(road_link::kToNodeID));
			CComVariant vaLink	= ipIntersectLink->GetValue(ipIntersectLink->Fields->_FindField(road_link::kMainLinkClass));

			long lOID = ipIntersectLink->GetOID();
			// 有料ではない基本道路以外は対象外
			if( vaClass.lVal == 0 || vaClass.lVal > 7 )
				continue;

			// 最重点対象は重点対象から除外する
			std::map<long,CNoParkingLink>::const_iterator iter_linkid = m_mapNoParkingLink.find(lOID);
			if( iter_linkid != m_mapNoParkingLink.end() )
				continue;

			// リンク形状の取得
			IGeometryPtr ipGeo = ipIntersectLink->GetShapeCopy();
			std::set<long>::const_iterator iter_containid = setOID.find(lOID);

			// バッファに完全内包されていないリンクはリンク長で判断する。
			if( iter_containid == setOID.end())
			{
				// 元のリンク長
				double dLen = GetLinkLen(ipGeo);

				// バッファに含まれるリンク長
				double dBuffLen = GetIntersecLinkLen( ipGeo, ipBuff, lOID );
	
				// リンクの8割が含まれていない場合は対象外
				if( dBuffLen / dLen * 100 < 80 ) 
					continue;

			}

			// 最重点路線を対象に選んだ場合はオブジェクトIDを格納する
	//		if( lClass == 1 )
				//m_LinkOIDSet.insert(lOID);
			CNoParkingLink cNoParkingLink;
			cNoParkingLink.ipLink = ipIntersectLink;
			cNoParkingLink.lNoParkingClass = lClass;
			m_mapNoParkingLink.insert( std::pair<long,CNoParkingLink>( lOID, cNoParkingLink ) );

			// 対象リンクが存在するメッシュコードを取得する。
			long lMeshCode = GetExistMesh(ipGeo);

			// リスト内容を出力する。
#ifdef _DEBUG
			ofp << lOID << std::endl;
#else
/*			CString aFromID, aToID;
			SetObjectID(aFromID, vaFrom.lVal);
			SetObjectID(aToID, vaTo.lVal);*/
			ofp << lMeshCode << "\t" << vaFrom.lVal << "\t" << vaTo.lVal << "\t" << lClass << std::endl;
#endif
		}
	}
	else
	{
		std::cout << "# ERROR\tCompData()\tバッファに重なっているリンク検索失敗\n";
		return false;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////
// リンク長を計測する。
//////////////////////////////////////////////////////////////////////
double CMakeList::GetLinkLen( IGeometry* ipLinkGeom )
{
	double dLength = 0;

	IPolylinePtr ipLink(get_Tokyo10CloneGeom(ipLinkGeom));
	dLength = ipLink->GetLength();

	return dLength;
}

//////////////////////////////////////////////////////////////////////
// バッファに含まれるリンク長を計測する。
//////////////////////////////////////////////////////////////////////
double CMakeList::GetIntersecLinkLen( IGeometry* ipLinkGeo, IGeometry* ipPolygon, long lOID )
{
	HRESULT hr;
	IGeometryPtr ipNewGeometry = get_Tokyo10CloneGeom(ipLinkGeo);
	IGeometryPtr ipPoly = get_Tokyo10CloneGeom(ipPolygon);

	ITopologicalOperatorPtr ipTopo(ipNewGeometry);
	IGeometryPtr ipIntersect = 0;

	// バッファに含まれているリンク形状を取得する。
	if (FAILED(hr = ipTopo->Intersect(ipPoly, esriGeometry1Dimension, &ipIntersect)))
	{
		IErrorInfoPtr ipErrorInfo = 0;
		::GetErrorInfo(0, &ipErrorInfo);
		if (ipErrorInfo != 0)
		{
			CComBSTR strDescription = 0;
			ipErrorInfo->GetDescription( &strDescription );
		}
		std::cout << "# WARNING\tリンクのクリップに失敗！\t" << lOID << std::endl;
		return 0;
	}

	double dTotalLength;
	std::map<long,double>::const_iterator iter_len = m_LinkLength.find(lOID);
	if( iter_len != m_LinkLength.end() )
		dTotalLength = iter_len->second;
	else
		dTotalLength = 0;

	dTotalLength += GetLinkLen(ipIntersect);
	m_LinkLength[lOID] = dTotalLength;

	// 取得した形状それぞれの長さを合計する。(ちょび切れ集合でもひとつのポリラインで形成されているので必要なし。)
/*	IGeometryCollectionPtr ipGeoCo = ipIntersect;
	long lGeoCount = ipGeoCo->GetGeometryCount();
	double dTotalLength = 0;
	for( int iCount = 0; iCount < lGeoCount; ++ iCount )
	{
		IGeometryPtr ipTmpGeo = ipGeoCo->GetGeometry(iCount);
		esriGeometryType eType = ipTmpGeo->GetGeometryType();
		double dLength = GetLinkLen(ipTmpGeo);
		dTotalLength += dLength;
	}*/

	return dTotalLength;
}

///////////////////////////////////////////////////////////
// リンク存在メッシュコードを取得する。
///////////////////////////////////////////////////////////
long CMakeList::GetExistMesh(IGeometry* ipLink)
{
	IPointCollectionPtr ipPointCo(ipLink);
	long lPointCount = ipPointCo->GetPointCount();

	IPointPtr ipFromPoint	= ipPointCo->GetPoint(0);
	IPointPtr ipToPoint		= ipPointCo->GetPoint(1);

	// 第1構成点と第2構成点の中間点のメッシュコートをリンクメッシュとする。
	double dX = (ipFromPoint->GetX() + ipToPoint->GetX()) / 2;
	double dY = (ipFromPoint->GetY() + ipToPoint->GetY()) / 2;

	crd_cnv crd;
	int iMeshCode = 0, iX = 0, iY = 0;
	crd.LLtoMesh(dX, dY, 2, &iMeshCode, &iX, &iY, 1);

	return iMeshCode;
}

//////////////////////////////////////////////////////////////////////
// 10進数→16進数への変換
//////////////////////////////////////////////////////////////////////
void CMakeList::SetObjectID(CString &rCstrID, int rID)
{
	CString cstrID, cstrTmp;
	cstrID.Format(_T("%x"), rID);

	// 空いた桁に0を挿入
	for(int iLength = 0; iLength < (8 - cstrID.GetLength()); ++iLength)
		cstrTmp += "0";

	rCstrID = _T("0x") + cstrTmp + cstrID;
}

bool CMakeList::OutputShape( IFeatureClass* ipLinkClass )
{
	USES_CONVERSION;
	// Shapeのパス設定
	CString strShapeFile;
	strShapeFile.Format( _T("%s\\noparking_link"), m_strShapeDir );

	CSHPHdl cSHPHandle;
	CDBFHdl cDBFHdl;
	// 形状データ作成（フィーチャクラス作成）
	cSHPHandle.create( T2CA(strShapeFile), SHPT_ARC );

	// フィールドデータ作成（フィーチャクラスのフィールド作成）
	cDBFHdl.create( T2CA(strShapeFile) );
	long lOID = cDBFHdl.addField( "OBJECTID", FTInteger, 9, 0 );
	long lClass = cDBFHdl.addField( "NoParkingClass", FTInteger, 9, 0 );
	long lFromNode = cDBFHdl.addField( "FromNodeID", FTInteger, 9, 0 );
	long lToNode = cDBFHdl.addField( "ToNodeID", FTInteger, 9, 0 );

	IFieldsPtr ipFields;
	ipLinkClass->get_Fields( &ipFields );
	long lFrom = -1, lTo = -1;
	if( ipFields )
	{
		ipFields->FindField( CComBSTR(sindy::schema::road_link::kFromNodeID), &lFrom );
		ipFields->FindField( CComBSTR(sindy::schema::road_link::kToNodeID), &lTo );
		if( lFrom < 0 )
		{
			std::cout << "# ERROR\t" << CT2A(sindy::schema::road_link::kFromNodeID) << " フィールド取得失敗\n";
			return false;
		}
		if( lTo < 0 )
		{
			std::cout << "# ERROR\t" << CT2A(sindy::schema::road_link::kToNodeID) << " フィールド取得失敗\n";			
			return false;
		}
	}
	else
	{
		std::cout << "# ERROR\t道路リンククラスからフィールド取得失敗\n";
		return false;
	}

	// shapeデータ作成
	for( std::map<long,CNoParkingLink>::const_iterator it = m_mapNoParkingLink.begin(); it != m_mapNoParkingLink.end(); ++it )
	{
		IGeometryPtr ipGeom;
		it->second.ipLink->get_ShapeCopy( &ipGeom );
		int nID = CreateOneShape( cSHPHandle, ipGeom );
		if( nID >= 0 )
		{
			CComVariant vaFromNode, vaToNode;
			it->second.ipLink->get_Value( lFrom, &vaFromNode );
			it->second.ipLink->get_Value( lTo, &vaToNode );
			cDBFHdl.writeIntegerAttribute( nID, lOID, it->first );
			cDBFHdl.writeIntegerAttribute( nID, lClass, it->second.lNoParkingClass );
			cDBFHdl.writeIntegerAttribute( nID, lFromNode, vaFromNode.lVal );
			cDBFHdl.writeIntegerAttribute( nID, lToNode, vaToNode.lVal );
		}
	}
	std::cerr << "\n";

	cSHPHandle.close();
	cDBFHdl.close();

	return true;
}

int CMakeList::CreateOneShape( CSHPHdl& cSHPHandle, IGeometry* ipGeom )
{
	int nRet = -1;
	
	IPointCollectionPtr ipPointCollection(ipGeom);
	if( ipPointCollection )
	{
		long lPointCount = 0;
		ipPointCollection->get_PointCount( &lPointCount );
		CSHPPointArray aPointArray;
		for( long l = 0; l < lPointCount; ++l )
		{
			IPointPtr ipPoint;
			ipPointCollection->get_Point( l, &ipPoint );
			double dX = 0.0, dY = 0.0;
			ipPoint->QueryCoords( &dX, &dY );
			CSHPPoint aSHPPoint( dX, dY );
			aPointArray.push_back(aSHPPoint);
		}
		// shape形状にする
		CSHPObj aArcObj( SHPT_ARC, aPointArray );
		// フィーチャクラスにフィーチャを追加（shapeデータを突っ込む）
		nRet = cSHPHandle.writeObject( aArcObj, -1 );
	}
	return nRet;
}

void CMakeList::OutputBaseLink( const std::vector<CAdapt<IFeaturePtr>>& vecBaseLink )
{
	USES_CONVERSION;
	// Shapeのパス設定
	CString strShapeFile;
	strShapeFile.Format( _T("%s\\baselink"), m_strLinkDir );

	CSHPHdl cSHPHandle;
	CDBFHdl cDBFHdl;
	// 形状データ作成（フィーチャクラス作成）
	cSHPHandle.create( T2CA(strShapeFile), SHPT_ARC );

	// フィールドデータ作成（フィーチャクラスのフィールド作成）
	cDBFHdl.create( T2CA(strShapeFile) );
	long lOID = cDBFHdl.addField( "OBJECTID", FTInteger, 9, 0 );

	// shapeデータ作成
	long lSize = vecBaseLink.size();
	long lCount = 0;
	for( std::vector<CAdapt<IFeaturePtr>>::const_iterator it = vecBaseLink.begin(); it != vecBaseLink.end(); ++it )
	{
		lCount++;
		std::cerr << lCount << " / " << lSize << " 件\r";
		IGeometryPtr ipGeom;
		it->m_T->get_ShapeCopy( &ipGeom );
		int nID = CreateOneShape( cSHPHandle, ipGeom );
		if( nID >= 0 )
		{
			long lOIDs = 0;
			it->m_T->get_OID( &lOIDs );
			cDBFHdl.writeIntegerAttribute( nID, lOID, lOIDs );
		}
	}
	std::cerr << "\n";

	cSHPHandle.close();
	cDBFHdl.close();
}
