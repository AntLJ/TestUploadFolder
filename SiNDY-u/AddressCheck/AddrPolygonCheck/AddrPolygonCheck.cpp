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
#include "AddrPolygonCheck.h"

// 初期化
bool AddrPolygonCheck::initialize( const SettingFile& settingFile, TableFinder& tableFinder, ErrLogger& errorLog, const CString& textAddressMaster )
{
	printProgress( getMsg( iAC_CheckInit ), getLayerCheckName());

	m_textAddressMaster.setDirPath( textAddressMaster );

	m_errorLog = &errorLog;	
	m_ipAddressPolygon =  tableFinder.findTable( DB_ADDRESS, addr_polygon::kTableName );
	m_addressFieldsMap = gf::createFieldMap( m_ipAddressPolygon );
	m_ipCityPolygon =  tableFinder.findTable( DB_ADDRESS, city_polygon::kTableName);
	m_ipPlaseNameRepPoint = tableFinder.findTable( DB_ADDRESS, placename_rep_point::kTableName);
	m_prohibitStr = settingFile.getLayerSettingItem(getName(), NG_CHAR);
	return true;
}

// チェック
void AddrPolygonCheck::check( unsigned long ulMeshCode, const IGeometryPtr& ipMeshGeometry, long meshID)
{
	m_curMeshCode = ulMeshCode;

	IFeatureCursorPtr ipAddressCursor;
	if( !select( ipAddressCursor, ipMeshGeometry ) )  
	{
		// Error code:0x01004003のチエック(メッシュ図郭内に、行政界が全く存在しない)
		if( m_errorIdSet.find( ErrorIdInfo( ulMeshCode, kAC_AddrPolygonNoPolygonInMesh ) ) != m_errorIdSet.end() ) return;

		m_errorLog->write( kAC_AddrPolygonNoPolygonInMesh, citymesh::kTableName, meshID, NULL, _T("%d"), ulMeshCode );
		m_errorIdSet.insert( ErrorIdInfo( ulMeshCode, kAC_AddrPolygonNoPolygonInMesh ) );

		return;
	}

	IGeometryCollectionPtr ipGeometryCollection( CLSID_GeometryBag );
	ISpatialReferencePtr	ipSpatialReference;

	map<FieldGroupKey, FieldGroupValue> differentAddrCodeMap; //!< 市区町村コード、住所コード、住所コード２、街区符号の組み合わせ
	map<FieldGroupKey2, map<long,IGeometryPtr>> touchGeoMap;  //!< 市区町村コード、住所コード、住所コード2、街区符号、拡張名称（公称）、拡張名称（通称）、住所整備種別コードの組み合わせ

	IFeaturePtr ipAddressFeature;
	// オブジェクトごとの処理
	while( S_OK == ipAddressCursor->NextFeature(&ipAddressFeature) && ipAddressFeature )
	{
		IGeometryPtr ipAddressGeometry; 
		if( ( S_OK != ipAddressFeature->get_Shape( &ipAddressGeometry ) ) || !ipAddressGeometry )
		{
			printProgress( getMsg( eFeailGetShape ), ulMeshCode );
			continue;
		}

		// 市区町村コード
		CComVariant cityCode = gf::getValue( ipAddressFeature, m_addressFieldsMap[addr_polygon::kCityCode] );
		// 住所コード
		CComVariant addrCode1 = gf::getValue( ipAddressFeature, m_addressFieldsMap[addr_polygon::kAddrCode1] );
		CComVariant addrCode2 = gf::getValue( ipAddressFeature, m_addressFieldsMap[addr_polygon::kAddrCode2] );
		// 住所整備種別コード
		CComVariant addrType = gf::getValue( ipAddressFeature, m_addressFieldsMap[addr_polygon::kAddrType] );
		// 拡張名称
		CComVariant extName1 = gf::getValue( ipAddressFeature, m_addressFieldsMap[addr_polygon::kExtName1] );
		CComVariant extName2 = gf::getValue( ipAddressFeature, m_addressFieldsMap[addr_polygon::kExtName2] );
		// 街区符号
		CComVariant gaikufugo = gf::getValue( ipAddressFeature, m_addressFieldsMap[addr_polygon::kGaikufugo] );
		// 拡張読み（通称）
		CComVariant extYomi2 = gf::getValue( ipAddressFeature, m_addressFieldsMap[addr_polygon::kExtYomi2] );

		CString addrTypeStr = ToString(addrType);
		long addrTypeCode = _ttol( addrTypeStr );
		long gaikufugoCode = _ttol( ToString(gaikufugo) );
		CString cityCodeStr = ToString(cityCode);
		CString addrCodeStr = ToString(addrCode1);
		CString addrCode2Str = ToString(addrCode2);
		CString extName1Str = ToString(extName1);
		CString extName2Str = ToString(extName2);

		long oID(0L);
		ipAddressFeature->get_OID( &oID );	

		// 空間参照の取得
		if( !ipSpatialReference )
		{
			ipAddressGeometry->get_SpatialReference( &ipSpatialReference );
			IGeometryPtr(ipGeometryCollection)->putref_SpatialReference( ipSpatialReference );
		}
		ipGeometryCollection->AddGeometry( ipAddressGeometry );
		
		// 重なるチェック
		CString query, info;
		query.Format(_T(" %s = '%s'"), addr_polygon::kCityCode, cityCodeStr );
		info = cityCodeStr + addrCodeStr;
		checkOverlap(ipAddressGeometry, ipAddressFeature, query, info, oID );	

		// 禁則文字のチエック
		checkProhibitChar(oID, ipAddressGeometry, extName1Str);

		// 地名代表点のチエック
		checkRepresentPoint(oID, ipAddressGeometry, cityCodeStr, addrCodeStr);

		// フィールドが等しい住所整備種別の異なるのチエック
		checkSameAttrAddrTypeC( oID,ipAddressGeometry,ipSpatialReference, addrTypeStr,addrCodeStr,addrCode2Str,cityCodeStr,gaikufugoCode, differentAddrCodeMap );

		// フィールドのチエック
		checkField( oID,ipAddressGeometry,addrType,cityCode,addrCode1,addrCode2,gaikufugo,extName1,extName2,extYomi2 );

		// マスタ情報を通じてのチエック
		checkWithMasterInfo( oID, ipAddressGeometry, cityCodeStr,addrCodeStr,extName1Str );

		// 同じ属性のジオメトリを収集する
		FieldGroupKey fieldGroup( addrCodeStr, addrCode2Str, cityCodeStr, gaikufugoCode );
		collectSameAttrGeometry( oID, ipAddressGeometry, fieldGroup, addrTypeCode, extName1Str, extName2Str, touchGeoMap );
	}

	// Error code:0x01004002のチエック(住所ポリゴンに抜けがある)
	if ( !m_relationCheck.checkOutArea( ipMeshGeometry, ipGeometryCollection, ipSpatialReference, ErrorIdInfo( m_curMeshCode, kAC_AddrPolygonCheckMissing), this, ErrorIdInfo( ulMeshCode, kAC_AddrPolygonNoPolygonInMesh ), citymesh::kTableName, meshID ) ) printProgress( getMsg( eFailMissing ) );

	// フィールドが等しい行政界が隣接しているのチエック
	checkSameAttrShape( touchGeoMap );

	// 飛び地のチエック
	checkEnclave( ipSpatialReference, differentAddrCodeMap );
}

// 同じ属性の形を集める
void AddrPolygonCheck::collectSameAttrGeometry( long oID, const IGeometryPtr& ipAddressGeometry, const FieldGroupKey& fieldGroup, long addrTypeCode, const CString& extName1Str, const CString& extName2Str, map<FieldGroupKey2, map<long,IGeometryPtr>>& touchGeoMap )
{
	// 住所整備種別コードが「0：町丁目界未整備」以外の場合,同じ属性の形を集める
	if( addrTypeCode )
	{
		FieldGroupKey2 fieldGroup2(addrTypeCode, extName1Str, extName2Str, fieldGroup);
		auto &itr = touchGeoMap.find(fieldGroup2);
		if( itr == touchGeoMap.end() )
		{
			map<long,IGeometryPtr> geoMap;
			geoMap[oID] = ipAddressGeometry;
			touchGeoMap[fieldGroup2] = geoMap;
		}else
		{
			itr->second[oID] = ipAddressGeometry;
		}
	}
	return;
}

// マスタ情報を通じてのチエック
void AddrPolygonCheck::checkWithMasterInfo(long oID, const IGeometryPtr& ipAddressGeometry, const CString&cityCodeStr, const CString& addrCodeStr, const CString& extName1Str)
{
	// 拡張名称(公称)がNULL以外場合チエックする。
	if( extName1Str.IsEmpty() ) return;

	// Error code:0x01004011のチエック(「8桁住所コードの名称＋拡張名称（公称）」の文字列を生成する。その文字列が、住所マスタ上、11桁住所として存在し、かつ該当の住所コードのステータスが「2：廃止」以外)
	CString searchMasterCode = cityCodeStr + addrCodeStr.Left(3) + _T("000");
	const auto master = m_textAddressMaster.getTextAddressMaster( searchMasterCode );
	CString searchMasterStr = CString( master.addressName ).TrimRight(_T("\t")) + _T("\t") + extName1Str;
	const auto &addressMasterCode = m_textAddressMaster.getTextAddressMasterCode(searchMasterStr);
	if( ( addressMasterCode.status != ABOLITION_CODE ) && (!addressMasterCode.addressCode.IsEmpty()) && ( m_errorIdSet.find( ErrorIdInfo( oID, kAC_AddrPolygonExtNameExistMaster ) ) == m_errorIdSet.end() ) )
	{
		m_errorLog->write( kAC_AddrPolygonExtNameExistMaster, getName(), oID, NULL, _T("%d\t%s\t%s"), m_curMeshCode, LPCTSTR(cityCodeStr + addrCodeStr), LPCTSTR(searchMasterStr) );
		m_errorIdSet.insert( ErrorIdInfo( oID, kAC_AddrPolygonExtNameExistMaster ) );
	}

	return;
}

// 地名代表点のチエック
void AddrPolygonCheck::checkRepresentPoint(long oID, const IGeometryPtr& ipAddressGeometry, const CString& cityCodeStr, const CString& addrCodeStr )
{
	// Error code:0x01004009のチエック(行政界の持つ11桁住所コードを持つ地名代表点ポイントが存在しない)
	long count(INVALID_VALUE);
	CString whereClause;
	whereClause.Format(_T("(%s='%s') AND (%s='%s')"), placename_rep_point::kCityCode, cityCodeStr, placename_rep_point::kAddrCode, addrCodeStr);
	_ICursorPtr cursor = gf::SelectByAttr(m_ipPlaseNameRepPoint, &count, whereClause);
	if( !cursor && ( m_errorIdSet.find( ErrorIdInfo( oID, kAC_AddrPolygonNoPoint ) ) == m_errorIdSet.end() ) )
	{
		m_errorLog->write( kAC_AddrPolygonNoPoint, getName(), oID, NULL, _T("%d\t%s"), m_curMeshCode, cityCodeStr + addrCodeStr );
		m_errorIdSet.insert( ErrorIdInfo( oID, kAC_AddrPolygonNoPoint ) );
	}

	return;
}

// 飛び地のチエック
void AddrPolygonCheck::checkEnclave(const ISpatialReferencePtr& ipSpatialReference,  map<FieldGroupKey, FieldGroupValue>& differentAddrCodeMap)
{
	// Error code:0x01003002のチエック(市区町村コード、住所コード、住所コード2、街区符号の組み合わせが等しい行政界ポリゴンをマージした結果、複数ポリゴンできる（飛び地ができる）)
	for(auto& itr: differentAddrCodeMap )
	{
		IPolygonPtr geoMerge =  gf::constructUnion( itr.second.ipGeometryCollection, ipSpatialReference );
		if( !geoMerge)
		{
			printProgress(getMsg( eFailConstructUnion ));
			return;
		}

		long geoCount(0L);
		IGeometryCollectionPtr(geoMerge)->get_GeometryCount( &geoCount );
		// 全て隣接するポリゴン場合、チエックしない
		if( geoCount < 2 )			continue;

		vector<long>oId;
		oId.resize(geoCount);
		// 飛び地のobjectIDを取得する
		for(unsigned short index = 0; index < geoCount; ++index)
		{
			IGeometryPtr ipGeo;
			IGeometryCollectionPtr( geoMerge)->get_Geometry(index, &ipGeo );
			if( !ipGeo ) continue;

			ipGeo->putref_SpatialReference(ipSpatialReference);
			IGeometryPtr ipMergeGeoItem(ipGeo);

			esriGeometryType esriType;
			ipGeo->get_GeometryType( &esriType );
			if(esriType != esriGeometryPolygon)
			{
				IPolygonPtr ipPolygon( CLSID_Polygon );
				IGeometryCollectionPtr(ipPolygon)->AddGeometry(ipGeo);		
				ipMergeGeoItem = ipPolygon;
			}

			for( const auto& geoItr:itr.second.geoMap )
			{
				if( !gf::IsContains( ipMergeGeoItem, geoItr.second ) ) continue;

				oId[index] = geoItr.first;
				break;
			}
		}
		//　出力Error
		for( const auto& idItr:oId )
		{
			if( idItr == oId[0] ) continue;

			if ( m_errorIdSet.find( ErrorIdInfo( idItr, kAC_AddrPolygonCheckAddrTypeGeoError1 ) ) != m_errorIdSet.end() ) continue;

			m_errorLog->write( kAC_AddrPolygonCheckAddrTypeGeoError1, getName(), oId[0], NULL, _T( "%d\t%d" ), m_curMeshCode, idItr );
			m_errorIdSet.insert( ErrorIdInfo( idItr, kAC_AddrPolygonCheckAddrTypeGeoError1 ) );
		}
	}
	return;
}

// フィールドが等しい住所整備種別の異なるのチエック
void AddrPolygonCheck::checkSameAttrAddrTypeC(long oID, IGeometryPtr& ipAddressGeometry, const ISpatialReferencePtr& ipSpatialReference, const CString& addrTypeStr, const CString& addrCodeStr, const CString& addrCode2Str, const CString& cityCodeStr, long gaikufugoCode, map<FieldGroupKey, FieldGroupValue>& differentAddrCodeMap)
{
	// Error code:0x01003001のチエック(市区町村コード、住所コード、住所コード２、街区符号の組み合わせが等しいが、住所整備種別の異なる住所ポリゴンが存在している)
	FieldGroupKey fieldGroup( addrCodeStr, addrCode2Str, cityCodeStr, gaikufugoCode );

	auto itr = differentAddrCodeMap.find( fieldGroup );
	if( itr == differentAddrCodeMap.end() )
	{
		CString oidStr;
		oidStr.Format( _T("%d"), oID );

		FieldGroupValue fieldValue;
		fieldValue.addrTypeMap[addrTypeStr] = oidStr;
		fieldValue.geoMap[oID] = ipAddressGeometry;

		fieldValue.ipGeometryCollection.CreateInstance(CLSID_GeometryBag) ;

		IGeometryPtr(fieldValue.ipGeometryCollection)->putref_SpatialReference( ipSpatialReference );
		fieldValue.ipGeometryCollection->AddGeometry( ipAddressGeometry );


		differentAddrCodeMap[fieldGroup] = fieldValue;
	}else
	{
		itr->second.ipGeometryCollection->AddGeometry( ipAddressGeometry );
		itr->second.geoMap[oID] = ipAddressGeometry;
		// 住所整備種別の異なる住所ポリゴンが存在している場合
		if( ( itr->second.addrTypeMap.find( addrTypeStr ) == itr->second.addrTypeMap.end() )  && ( m_errorIdSet.find( ErrorIdInfo( oID, kAC_AddrPolygonCheckAddrTypeError12 ) ) == m_errorIdSet.end() ) )
		{
			m_errorLog->write( kAC_AddrPolygonCheckAddrTypeError12, getName(), oID, NULL, _T("%d\t%s\t%s"), m_curMeshCode, addrTypeStr, itr->second.addrTypeMap.begin()->second );
			m_errorIdSet.insert( ErrorIdInfo( oID, kAC_AddrPolygonCheckAddrTypeError12 ) );
		}
	}
	return;
}

// フィールドが等しい行政界が隣接しているのチエック
void AddrPolygonCheck::checkSameAttrShape(const map<FieldGroupKey2, map<long,IGeometryPtr>>& touchGeoMap)
{
	// Error code:0x01003003のチエック(住所整備種別コードが「0：町丁目界未整備」以外で、市区町村コード、住所コード、住所コード2、街区符号、拡張名称（公称）、拡張名称（通称）、住所整備種別コードが等しい行政界が隣接している)
	for( const auto&itrGroup2: touchGeoMap )
	{
		for( const auto& itrCompare1:itrGroup2.second )
		{
			long oId1 = itrCompare1.first;

			// 相互隣接となるポリゴンが1回だけ出力する。
			IGeometryPtr ipGeo1 = itrCompare1.second;
			bool isFindGeo1(false);
			for( const auto& itrCompare2:itrGroup2.second )
			{
				if( oId1 == itrCompare2.first )
				{
					isFindGeo1 = true;
					continue;
				}

				if( !isFindGeo1 ) continue;

				IGeometryPtr ipGeo2 = itrCompare2.second;

				if( !gf::isTouches( ipGeo1, ipGeo2 )  || ( m_errorIdSet.find( ErrorIdInfo( oId1, kAC_AddrPolygonCheckAddrTypeGeoError2 ) ) != m_errorIdSet.end() )) 
					continue;

				m_errorLog->write( kAC_AddrPolygonCheckAddrTypeGeoError2, getName(), oId1, NULL, _T( "%d\t%d" ), m_curMeshCode, itrCompare2.first );
				m_errorIdSet.insert( ErrorIdInfo( oId1, kAC_AddrPolygonCheckAddrTypeGeoError2 ) );
			}
		}
	}
	return;
}

// 禁則文字のチエック
void AddrPolygonCheck::checkProhibitChar(long oID, const IGeometryPtr& ipAddressGeometry, const CString& extName1Str)
{
	// Error code:0x01001001のチエック(拡張名称（公称）に禁則文字が使用されている。)
	for( int index = 0; index < extName1Str.GetLength(); ++index )
	{
		CString charTmp= extName1Str[index];
		if( m_prohibitStr.Find(charTmp) < 0 )	continue;

		if( m_errorIdSet.find( ErrorIdInfo( oID, kAC_AddrPolygonExistProhibitChar ) ) != m_errorIdSet.end() ) continue;

		m_errorLog->write( kAC_AddrPolygonExistProhibitChar, getName(), oID, NULL, _T("%d\t%s"), m_curMeshCode, extName1Str );
		m_errorIdSet.insert( ErrorIdInfo( oID, kAC_AddrPolygonExistProhibitChar ) );

		break;
	}
	return;
}

// フィールドのチエック
void AddrPolygonCheck::checkField(long oID, const IGeometryPtr& ipAddressGeometry, const CComVariant& addrType, const CComVariant& cityCode, const CComVariant& addrCode1, const CComVariant& addrCode2, const CComVariant& gaikufugo, const CComVariant& extName1, const CComVariant& extName2, const CComVariant& extYomi2)
{
	long addrTypeCode = _ttol( ToString(addrType) );
	CString gaikufugoStr = ToString(gaikufugo);
	long gaikufugoCode = _ttol( gaikufugoStr );
	CString cityCodeStr = ToString(cityCode);
	CString addrCodeStr = ToString(addrCode1);
	CString addrCode2Str = ToString(addrCode2);
	CString extName1Str = ToString(extName1);
	CString extName2Str = ToString(extName2);

	// Error code: 0x01001011のチエック(住所コード(公称)と住所コード(通称)が同じ)
	if( (addrCodeStr.Compare(addrCode2Str) == 0) &&  (m_errorIdSet.find( ErrorIdInfo( oID, kAC_AddrPolygonSameAddrCode ) ) == m_errorIdSet.end()) )
	{
		m_errorLog->write( kAC_AddrPolygonSameAddrCode, getName(), oID, NULL, _T("%d\t%s\t%s"), m_curMeshCode, addrCodeStr, addrCode2Str );
		m_errorIdSet.insert( ErrorIdInfo( oID, kAC_AddrPolygonSameAddrCode ) );
	}

	// Error code: 0x01001003のチエック(住所コード(通称)に、別荘地「999999」が設定されている)
	if( (addrCode2Str.Compare(VILLA) == 0) && (m_errorIdSet.find( ErrorIdInfo( oID, kAC_AddrPolygonAddrCode2Villa ) ) == m_errorIdSet.end()) )
	{
		m_errorLog->write( kAC_AddrPolygonAddrCode2Villa, getName(), oID, NULL, _T("%d\t%s"), m_curMeshCode, cityCodeStr+addrCode2Str );
		m_errorIdSet.insert( ErrorIdInfo( oID, kAC_AddrPolygonAddrCode2Villa ) );
	}

	// Error code:0x01001002のチエック(住所整備種別コードがNULL、またはコード値ドメイン規定値以外の値を格納している。)
	if( ( isNullOrEmpty( addrType ) || ( (addr_polygon::addr_type::kAreaUndefined != addrTypeCode) && (addr_polygon::addr_type::kDefinedWithChiban != addrTypeCode) && 
		( addr_polygon::addr_type::kDefinedWithDispAddr != addrTypeCode) && (addr_polygon::addr_type::kUndefinedWithChiban != addrTypeCode) ) ) &&
		( m_errorIdSet.find( ErrorIdInfo( oID, kAC_AddrPolygonCheckAddrTypeError11 ) ) == m_errorIdSet.end() ) )
	{
		m_errorLog->write( kAC_AddrPolygonCheckAddrTypeError11, getName(), oID, NULL, _T("%d\t%d"), m_curMeshCode, addrTypeCode );
		m_errorIdSet.insert( ErrorIdInfo( oID, kAC_AddrPolygonCheckAddrTypeError11 ) );
	}

	// Error code:0x01002010のチエック(「住所コード（通称）がNotNULLの時、拡張名称（公称）と拡張名称（通称）の名称が異なる)
	if( !addrCode2Str.IsEmpty() && ( extName1Str != extName2Str ) && ( m_errorIdSet.find( ErrorIdInfo( oID, kAC_AddrPolygonCheckAddrTypeError10 ) ) == m_errorIdSet.end() ) )
	{
		m_errorLog->write( kAC_AddrPolygonCheckAddrTypeError10, getName(), oID, NULL, _T("%d\t%s\t%s"), m_curMeshCode, extName1Str, extName2Str );
		m_errorIdSet.insert( ErrorIdInfo( oID, kAC_AddrPolygonCheckAddrTypeError10 ) );
	}

	// Error code:0x01004004のチエック(市区町村コード＋住所コードの１１桁住所コードが住所マスタに存在しない。)
	if( ( ( m_textAddressMaster.getTextAddressMaster( cityCodeStr + addrCodeStr ) ).status == INVALID_VALUE ) && 
		( m_errorIdSet.find( ErrorIdInfo( oID, kAC_AddrPolygonCheckAddrCodeError1 ) ) == m_errorIdSet.end() ) )
	{
		m_errorLog->write( kAC_AddrPolygonCheckAddrCodeError1, getName(), oID, NULL, _T("%d\t%s"), m_curMeshCode, cityCodeStr + addrCodeStr );
		m_errorIdSet.insert( ErrorIdInfo( oID, kAC_AddrPolygonCheckAddrCodeError1 ) );
	}

	// Error code:0x01004006のチエック(住所コード(公称)が「000000」以外、かつ、同一11桁住所コードのテキスト住所マスタのステータスが「2」)
	if( ( addrCodeStr != DEFAULT_ADDRCODE ) && ( ( m_textAddressMaster.getTextAddressMaster( cityCodeStr + addrCodeStr ) ).status == ABOLITION_CODE ) &&
		m_errorIdSet.find( ErrorIdInfo( oID, kAC_AddrPolygonCheckAddrCodeError3 ) ) == m_errorIdSet.end() )	
	{
		m_errorLog->write( kAC_AddrPolygonCheckAddrCodeError3, getName(), oID, NULL, _T("%d\t%s"), m_curMeshCode, cityCodeStr + addrCodeStr );
		m_errorIdSet.insert( ErrorIdInfo( oID, kAC_AddrPolygonCheckAddrCodeError3 ) );
	}

	// 住所コード（通称）がNot Null場合
	if( !isNullOrEmpty(addrCode2) )
	{
		// Error code:0x01004005のチエック(市区町村コード＋住所コード2の１１桁住所コードが住所マスタに存在しない。)
		if( ( m_textAddressMaster.getTextAddressMaster( cityCodeStr + addrCode2Str ) ).status == INVALID_VALUE  && 
			m_errorIdSet.find( ErrorIdInfo( oID, kAC_AddrPolygonCheckAddrCodeError2 ) ) == m_errorIdSet.end() )
		{
			m_errorLog->write( kAC_AddrPolygonCheckAddrCodeError2, getName(), oID, NULL, _T("%d\t%s"), m_curMeshCode, cityCodeStr + addrCode2Str );
			m_errorIdSet.insert( ErrorIdInfo( oID, kAC_AddrPolygonCheckAddrCodeError2 ) );				
		}

		// Error code:0x01004007のチエック(住所コード(通称)が「000000」以外、かつ、同一11桁住所コードのテキスト住所マスタのステータスが「2」)
		if( ( addrCode2Str != DEFAULT_ADDRCODE ) && ( ( m_textAddressMaster.getTextAddressMaster( cityCodeStr + addrCode2Str ) ).status == ABOLITION_CODE ) &&
			m_errorIdSet.find( ErrorIdInfo( oID, kAC_AddrPolygonCheckAddrCodeError4 ) ) == m_errorIdSet.end() )
		{
			m_errorLog->write( kAC_AddrPolygonCheckAddrCodeError4, getName(), oID, NULL, _T("%d\t%s"), m_curMeshCode, cityCodeStr + addrCode2Str );
			m_errorIdSet.insert( ErrorIdInfo( oID, kAC_AddrPolygonCheckAddrCodeError4 ) );
		}
	}

	// 住所整備種別コードが「0：町丁目界未整備」の場合
	if( addr_polygon::addr_type::kAreaUndefined == addrTypeCode )
	{
		// Error code:0x01002001のチエック(住所整備種別コードが「0：町丁目界未整備」で、市区町村コードが「00000」以外、または住所コードまたは住所コード2が「000000」以外)
		if( ( cityCodeStr != DEFAULT_CITYCODE || addrCodeStr != DEFAULT_ADDRCODE || addrCode2Str != DEFAULT_ADDRCODE ) &&
			( m_errorIdSet.find( ErrorIdInfo( oID, kAC_AddrPolygonCheckAddrTypeError1 ) ) == m_errorIdSet.end() ) )
		{
			m_errorLog->write( kAC_AddrPolygonCheckAddrTypeError1, getName(), oID, NULL, _T("%d\t%s\t%s"), m_curMeshCode, cityCodeStr+addrCodeStr, cityCodeStr+addrCode2Str );
			m_errorIdSet.insert( ErrorIdInfo( oID, kAC_AddrPolygonCheckAddrTypeError1 ) );
		}

		// Error code:0x01002003のチエック(行政界の住所整備種別が「0：町丁目界未整備」で、街区符号または拡張名称(公称)がNot Null)
		if( ( !isNullOrEmpty(gaikufugo) || !isNullOrEmpty(extName1) ) && m_errorIdSet.find( ErrorIdInfo( oID, kAC_AddrPolygonCheckAddrTypeError3 ) ) == m_errorIdSet.end() )
		{
			m_errorLog->write( kAC_AddrPolygonCheckAddrTypeError3, getName(), oID, NULL, _T("%d\t%s\t%s"), m_curMeshCode, gaikufugoStr, extName1Str);
			m_errorIdSet.insert( ErrorIdInfo( oID, kAC_AddrPolygonCheckAddrTypeError3 ) );
		}
	}else
	// 住所整備種別コードが「0：町丁目界未整備」以外の場合
	{
		// Error code:0x01002002のチエック(住所整備種別コードが「0：町丁目界未整備」以外で、市区町村コードが「00000」、または住所コードまたは住所コード2が「000000」である)
		if( ( cityCodeStr == DEFAULT_CITYCODE || addrCodeStr == DEFAULT_ADDRCODE || addrCode2Str == DEFAULT_ADDRCODE ) && 
			( m_errorIdSet.find( ErrorIdInfo( oID, kAC_AddrPolygonCheckAddrTypeError2 ) ) == m_errorIdSet.end() ) )
		{
			m_errorLog->write( kAC_AddrPolygonCheckAddrTypeError2, getName(), oID, NULL, _T("%d\t%s\t%s"), m_curMeshCode, cityCodeStr+addrCodeStr, cityCodeStr+addrCode2Str );
			m_errorIdSet.insert( ErrorIdInfo( oID, kAC_AddrPolygonCheckAddrTypeError2 ) );
		}

		// Error code:0x01002004のチエック(住所整備種別が「１:住居表示整備済み」または「2:地番整備済み」で、街区符号と拡張名称（公称）がどちらもNot Nullである)
		if( ( ( addrTypeCode == addr_polygon::addr_type::kDefinedWithDispAddr || addrTypeCode == addr_polygon::addr_type::kDefinedWithChiban ) && !isNullOrEmpty(gaikufugo) && !isNullOrEmpty(extName1) ) && 
			( m_errorIdSet.find( ErrorIdInfo( oID, kAC_AddrPolygonCheckAddrTypeError4 ) ) == m_errorIdSet.end() ) )
		{
			m_errorLog->write( kAC_AddrPolygonCheckAddrTypeError4, getName(), oID, NULL, _T("%d\t%s\t%s"), m_curMeshCode, gaikufugoStr, extName1Str );
			m_errorIdSet.insert( ErrorIdInfo( oID, kAC_AddrPolygonCheckAddrTypeError4 ) );
		}

		// Error code:0x01002005のチエック(住所整備種別が「1：住居表示整備済み」または「2：地番整備済み」で、街区符号が「0」)
		if( ( addrTypeCode == addr_polygon::addr_type::kDefinedWithDispAddr || addrTypeCode == addr_polygon::addr_type::kDefinedWithChiban ) && gaikufugoStr == DEFAULT_BLOCKCODE && 
			( m_errorIdSet.find( ErrorIdInfo( oID, kAC_AddrPolygonCheckAddrTypeError5 ) ) == m_errorIdSet.end() ) )
		{
			m_errorLog->write( kAC_AddrPolygonCheckAddrTypeError5, getName(), oID, NULL, _T("%d"), m_curMeshCode );
			m_errorIdSet.insert( ErrorIdInfo( oID, kAC_AddrPolygonCheckAddrTypeError5 ) );
		}

		// Error code:0x01002006のチエック(住所整備種別コードが「1：住居表示整備済み」で、街区符号が3桁以上の数字)
		if( ( addrTypeCode == addr_polygon::addr_type::kDefinedWithDispAddr ) && ( gaikufugoCode >= 100 ) &&
			( m_errorIdSet.find( ErrorIdInfo( oID, kAC_AddrPolygonCheckAddrTypeError6 ) ) == m_errorIdSet.end() ) )
		{
			m_errorLog->write( kAC_AddrPolygonCheckAddrTypeError6, getName(), oID, NULL, _T("%d\t%s"), m_curMeshCode, gaikufugoStr );
			m_errorIdSet.insert( ErrorIdInfo( oID, kAC_AddrPolygonCheckAddrTypeError6 ) );
		}

		// Error code:0x01002007のチエック(住所整備種別コードが「3；住居表示・地番未整備」で、街区符号がNot Null)
		if( addrTypeCode == addr_polygon::addr_type::kUndefinedWithChiban && !isNullOrEmpty( gaikufugo ) &&
			m_errorIdSet.find( ErrorIdInfo( oID, kAC_AddrPolygonCheckAddrTypeError7 ) ) == m_errorIdSet.end() )
		{
			m_errorLog->write( kAC_AddrPolygonCheckAddrTypeError7, getName(), oID, NULL, _T("%d\t%s"), m_curMeshCode, gaikufugoStr );
			m_errorIdSet.insert( ErrorIdInfo( oID, kAC_AddrPolygonCheckAddrTypeError7 ) );
		}

		// Error code:0x01002008のチエック(住所整備種別コードが「1：住居表示整備済み」または「2：地番整備済み」で、街区符号と拡張名称1がNull)
		if( ( addrTypeCode == addr_polygon::addr_type::kDefinedWithDispAddr || addrTypeCode == addr_polygon::addr_type::kDefinedWithChiban ) && isNullOrEmpty( gaikufugo ) && isNullOrEmpty( extName1 ) &&
			m_errorIdSet.find( ErrorIdInfo( oID, kAC_AddrPolygonCheckAddrTypeError8 ) ) == m_errorIdSet.end() )
		{
			m_errorLog->write( kAC_AddrPolygonCheckAddrTypeError8, getName(), oID, NULL, _T("%d"), m_curMeshCode );
			m_errorIdSet.insert( ErrorIdInfo( oID, kAC_AddrPolygonCheckAddrTypeError8 ) );
		}

		// Error code:0x01002009のチエック(住所整備種別が「２：地番整備済み」かつ、拡張名称（公称）がNot Null)
		if( ( addrTypeCode == addr_polygon::addr_type::kDefinedWithChiban ) && !isNullOrEmpty( extName1 ) &&
			m_errorIdSet.find( ErrorIdInfo( oID, kAC_AddrPolygonCheckAddrTypeError9 ) ) == m_errorIdSet.end() )
		{
			m_errorLog->write( kAC_AddrPolygonCheckAddrTypeError9, getName(), oID, NULL, _T("%d\t%s\t%s"), m_curMeshCode, cityCodeStr + addrCodeStr, extName1Str );
			m_errorIdSet.insert( ErrorIdInfo( oID, kAC_AddrPolygonCheckAddrTypeError9 ) );
		}

		// Error code:0x01002012のチエック(住所整備種別が「1：住居表示整備済み」または「2：地番整備済み」で、住所コード（通称）、拡張名称（通称）、拡張読み（通称）がNot Null)
		if( ( ( addrTypeCode == addr_polygon::addr_type::kDefinedWithDispAddr ) || ( addrTypeCode == addr_polygon::addr_type::kDefinedWithChiban ) ) &&
			( ( !isNullOrEmpty(addrCode2) ) || ( !isNullOrEmpty(extName2) ) || ( !isNullOrEmpty(extYomi2) ) ) &&
			( m_errorIdSet.find( ErrorIdInfo( oID, kAC_AddrPolygonCheckAddrTypeError13 ) ) == m_errorIdSet.end() ) )
		{
			m_errorLog->write( kAC_AddrPolygonCheckAddrTypeError13, getName(), oID, NULL, _T("%d\t%s\t%s"), m_curMeshCode, cityCodeStr + addrCode2Str, extName2Str );
			m_errorIdSet.insert( ErrorIdInfo( oID, kAC_AddrPolygonCheckAddrTypeError13 ) );
		}
	}
	return;
}

// 重なるチェック
void AddrPolygonCheck::checkOverlap( const IGeometryPtr& ipGeometry, const IFeaturePtr& ipFeature, const CString& query, const CString& info, long oID )
{
	// Error code:0x01004001のチエック(同一の市区町村コードを持つ市区町村ポリゴンと一つも重なっていない)
	IFeatureCursorPtr ipCityCursor;
	long count = AheSelectByShapeAndGetCount( ipGeometry, m_ipCityPolygon, &ipCityCursor, esriSpatialRelIntersects, NULL, query );

	if( ( !ipCityCursor || count == 0 ) && ( m_errorIdSet.find( ErrorIdInfo( oID, kAC_AddrPolygonCheckOverLap ) ) == m_errorIdSet.end() ) )
	{
		m_errorLog->write( kAC_AddrPolygonCheckOverLap, getName(), oID, NULL, _T("%d\t%s"), m_curMeshCode, info );
		m_errorIdSet.insert( ErrorIdInfo( oID, kAC_AddrPolygonCheckOverLap ) );
	}

	return;
}

// 空間検索
bool AddrPolygonCheck::select( IFeatureCursorPtr& ipCursor, const IGeometryPtr& ipGeometry)
{
	long featureCount = AheSelectByShapeAndGetCount( ipGeometry, m_ipAddressPolygon, &ipCursor );
	if( !ipCursor || featureCount == 0 )
	{
		printProgress( getMsg( eFailAddressPolyData ));
		return false;
	}

	return true;
}

