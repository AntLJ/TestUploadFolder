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

// ������
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

// �`�F�b�N
void AddrPolygonCheck::check( unsigned long ulMeshCode, const IGeometryPtr& ipMeshGeometry, long meshID)
{
	m_curMeshCode = ulMeshCode;

	IFeatureCursorPtr ipAddressCursor;
	if( !select( ipAddressCursor, ipMeshGeometry ) )  
	{
		// Error code:0x01004003�̃`�G�b�N(���b�V���}�s���ɁA�s���E���S�����݂��Ȃ�)
		if( m_errorIdSet.find( ErrorIdInfo( ulMeshCode, kAC_AddrPolygonNoPolygonInMesh ) ) != m_errorIdSet.end() ) return;

		m_errorLog->write( kAC_AddrPolygonNoPolygonInMesh, citymesh::kTableName, meshID, NULL, _T("%d"), ulMeshCode );
		m_errorIdSet.insert( ErrorIdInfo( ulMeshCode, kAC_AddrPolygonNoPolygonInMesh ) );

		return;
	}

	IGeometryCollectionPtr ipGeometryCollection( CLSID_GeometryBag );
	ISpatialReferencePtr	ipSpatialReference;

	map<FieldGroupKey, FieldGroupValue> differentAddrCodeMap; //!< �s�撬���R�[�h�A�Z���R�[�h�A�Z���R�[�h�Q�A�X�敄���̑g�ݍ��킹
	map<FieldGroupKey2, map<long,IGeometryPtr>> touchGeoMap;  //!< �s�撬���R�[�h�A�Z���R�[�h�A�Z���R�[�h2�A�X�敄���A�g�����́i���́j�A�g�����́i�ʏ́j�A�Z��������ʃR�[�h�̑g�ݍ��킹

	IFeaturePtr ipAddressFeature;
	// �I�u�W�F�N�g���Ƃ̏���
	while( S_OK == ipAddressCursor->NextFeature(&ipAddressFeature) && ipAddressFeature )
	{
		IGeometryPtr ipAddressGeometry; 
		if( ( S_OK != ipAddressFeature->get_Shape( &ipAddressGeometry ) ) || !ipAddressGeometry )
		{
			printProgress( getMsg( eFeailGetShape ), ulMeshCode );
			continue;
		}

		// �s�撬���R�[�h
		CComVariant cityCode = gf::getValue( ipAddressFeature, m_addressFieldsMap[addr_polygon::kCityCode] );
		// �Z���R�[�h
		CComVariant addrCode1 = gf::getValue( ipAddressFeature, m_addressFieldsMap[addr_polygon::kAddrCode1] );
		CComVariant addrCode2 = gf::getValue( ipAddressFeature, m_addressFieldsMap[addr_polygon::kAddrCode2] );
		// �Z��������ʃR�[�h
		CComVariant addrType = gf::getValue( ipAddressFeature, m_addressFieldsMap[addr_polygon::kAddrType] );
		// �g������
		CComVariant extName1 = gf::getValue( ipAddressFeature, m_addressFieldsMap[addr_polygon::kExtName1] );
		CComVariant extName2 = gf::getValue( ipAddressFeature, m_addressFieldsMap[addr_polygon::kExtName2] );
		// �X�敄��
		CComVariant gaikufugo = gf::getValue( ipAddressFeature, m_addressFieldsMap[addr_polygon::kGaikufugo] );
		// �g���ǂ݁i�ʏ́j
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

		// ��ԎQ�Ƃ̎擾
		if( !ipSpatialReference )
		{
			ipAddressGeometry->get_SpatialReference( &ipSpatialReference );
			IGeometryPtr(ipGeometryCollection)->putref_SpatialReference( ipSpatialReference );
		}
		ipGeometryCollection->AddGeometry( ipAddressGeometry );
		
		// �d�Ȃ�`�F�b�N
		CString query, info;
		query.Format(_T(" %s = '%s'"), addr_polygon::kCityCode, cityCodeStr );
		info = cityCodeStr + addrCodeStr;
		checkOverlap(ipAddressGeometry, ipAddressFeature, query, info, oID );	

		// �֑������̃`�G�b�N
		checkProhibitChar(oID, ipAddressGeometry, extName1Str);

		// �n����\�_�̃`�G�b�N
		checkRepresentPoint(oID, ipAddressGeometry, cityCodeStr, addrCodeStr);

		// �t�B�[���h���������Z��������ʂ̈قȂ�̃`�G�b�N
		checkSameAttrAddrTypeC( oID,ipAddressGeometry,ipSpatialReference, addrTypeStr,addrCodeStr,addrCode2Str,cityCodeStr,gaikufugoCode, differentAddrCodeMap );

		// �t�B�[���h�̃`�G�b�N
		checkField( oID,ipAddressGeometry,addrType,cityCode,addrCode1,addrCode2,gaikufugo,extName1,extName2,extYomi2 );

		// �}�X�^����ʂ��Ẵ`�G�b�N
		checkWithMasterInfo( oID, ipAddressGeometry, cityCodeStr,addrCodeStr,extName1Str );

		// ���������̃W�I���g�������W����
		FieldGroupKey fieldGroup( addrCodeStr, addrCode2Str, cityCodeStr, gaikufugoCode );
		collectSameAttrGeometry( oID, ipAddressGeometry, fieldGroup, addrTypeCode, extName1Str, extName2Str, touchGeoMap );
	}

	// Error code:0x01004002�̃`�G�b�N(�Z���|���S���ɔ���������)
	if ( !m_relationCheck.checkOutArea( ipMeshGeometry, ipGeometryCollection, ipSpatialReference, ErrorIdInfo( m_curMeshCode, kAC_AddrPolygonCheckMissing), this, ErrorIdInfo( ulMeshCode, kAC_AddrPolygonNoPolygonInMesh ), citymesh::kTableName, meshID ) ) printProgress( getMsg( eFailMissing ) );

	// �t�B�[���h���������s���E���אڂ��Ă���̃`�G�b�N
	checkSameAttrShape( touchGeoMap );

	// ��ђn�̃`�G�b�N
	checkEnclave( ipSpatialReference, differentAddrCodeMap );
}

// ���������̌`���W�߂�
void AddrPolygonCheck::collectSameAttrGeometry( long oID, const IGeometryPtr& ipAddressGeometry, const FieldGroupKey& fieldGroup, long addrTypeCode, const CString& extName1Str, const CString& extName2Str, map<FieldGroupKey2, map<long,IGeometryPtr>>& touchGeoMap )
{
	// �Z��������ʃR�[�h���u0�F�����ڊE�������v�ȊO�̏ꍇ,���������̌`���W�߂�
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

// �}�X�^����ʂ��Ẵ`�G�b�N
void AddrPolygonCheck::checkWithMasterInfo(long oID, const IGeometryPtr& ipAddressGeometry, const CString&cityCodeStr, const CString& addrCodeStr, const CString& extName1Str)
{
	// �g������(����)��NULL�ȊO�ꍇ�`�G�b�N����B
	if( extName1Str.IsEmpty() ) return;

	// Error code:0x01004011�̃`�G�b�N(�u8���Z���R�[�h�̖��́{�g�����́i���́j�v�̕�����𐶐�����B���̕����񂪁A�Z���}�X�^��A11���Z���Ƃ��đ��݂��A���Y���̏Z���R�[�h�̃X�e�[�^�X���u2�F�p�~�v�ȊO)
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

// �n����\�_�̃`�G�b�N
void AddrPolygonCheck::checkRepresentPoint(long oID, const IGeometryPtr& ipAddressGeometry, const CString& cityCodeStr, const CString& addrCodeStr )
{
	// Error code:0x01004009�̃`�G�b�N(�s���E�̎���11���Z���R�[�h�����n����\�_�|�C���g�����݂��Ȃ�)
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

// ��ђn�̃`�G�b�N
void AddrPolygonCheck::checkEnclave(const ISpatialReferencePtr& ipSpatialReference,  map<FieldGroupKey, FieldGroupValue>& differentAddrCodeMap)
{
	// Error code:0x01003002�̃`�G�b�N(�s�撬���R�[�h�A�Z���R�[�h�A�Z���R�[�h2�A�X�敄���̑g�ݍ��킹���������s���E�|���S�����}�[�W�������ʁA�����|���S���ł���i��ђn���ł���j)
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
		// �S�ėאڂ���|���S���ꍇ�A�`�G�b�N���Ȃ�
		if( geoCount < 2 )			continue;

		vector<long>oId;
		oId.resize(geoCount);
		// ��ђn��objectID���擾����
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
		//�@�o��Error
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

// �t�B�[���h���������Z��������ʂ̈قȂ�̃`�G�b�N
void AddrPolygonCheck::checkSameAttrAddrTypeC(long oID, IGeometryPtr& ipAddressGeometry, const ISpatialReferencePtr& ipSpatialReference, const CString& addrTypeStr, const CString& addrCodeStr, const CString& addrCode2Str, const CString& cityCodeStr, long gaikufugoCode, map<FieldGroupKey, FieldGroupValue>& differentAddrCodeMap)
{
	// Error code:0x01003001�̃`�G�b�N(�s�撬���R�[�h�A�Z���R�[�h�A�Z���R�[�h�Q�A�X�敄���̑g�ݍ��킹�����������A�Z��������ʂ̈قȂ�Z���|���S�������݂��Ă���)
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
		// �Z��������ʂ̈قȂ�Z���|���S�������݂��Ă���ꍇ
		if( ( itr->second.addrTypeMap.find( addrTypeStr ) == itr->second.addrTypeMap.end() )  && ( m_errorIdSet.find( ErrorIdInfo( oID, kAC_AddrPolygonCheckAddrTypeError12 ) ) == m_errorIdSet.end() ) )
		{
			m_errorLog->write( kAC_AddrPolygonCheckAddrTypeError12, getName(), oID, NULL, _T("%d\t%s\t%s"), m_curMeshCode, addrTypeStr, itr->second.addrTypeMap.begin()->second );
			m_errorIdSet.insert( ErrorIdInfo( oID, kAC_AddrPolygonCheckAddrTypeError12 ) );
		}
	}
	return;
}

// �t�B�[���h���������s���E���אڂ��Ă���̃`�G�b�N
void AddrPolygonCheck::checkSameAttrShape(const map<FieldGroupKey2, map<long,IGeometryPtr>>& touchGeoMap)
{
	// Error code:0x01003003�̃`�G�b�N(�Z��������ʃR�[�h���u0�F�����ڊE�������v�ȊO�ŁA�s�撬���R�[�h�A�Z���R�[�h�A�Z���R�[�h2�A�X�敄���A�g�����́i���́j�A�g�����́i�ʏ́j�A�Z��������ʃR�[�h���������s���E���אڂ��Ă���)
	for( const auto&itrGroup2: touchGeoMap )
	{
		for( const auto& itrCompare1:itrGroup2.second )
		{
			long oId1 = itrCompare1.first;

			// ���ݗאڂƂȂ�|���S����1�񂾂��o�͂���B
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

// �֑������̃`�G�b�N
void AddrPolygonCheck::checkProhibitChar(long oID, const IGeometryPtr& ipAddressGeometry, const CString& extName1Str)
{
	// Error code:0x01001001�̃`�G�b�N(�g�����́i���́j�ɋ֑��������g�p����Ă���B)
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

// �t�B�[���h�̃`�G�b�N
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

	// Error code: 0x01001011�̃`�G�b�N(�Z���R�[�h(����)�ƏZ���R�[�h(�ʏ�)������)
	if( (addrCodeStr.Compare(addrCode2Str) == 0) &&  (m_errorIdSet.find( ErrorIdInfo( oID, kAC_AddrPolygonSameAddrCode ) ) == m_errorIdSet.end()) )
	{
		m_errorLog->write( kAC_AddrPolygonSameAddrCode, getName(), oID, NULL, _T("%d\t%s\t%s"), m_curMeshCode, addrCodeStr, addrCode2Str );
		m_errorIdSet.insert( ErrorIdInfo( oID, kAC_AddrPolygonSameAddrCode ) );
	}

	// Error code: 0x01001003�̃`�G�b�N(�Z���R�[�h(�ʏ�)�ɁA�ʑ��n�u999999�v���ݒ肳��Ă���)
	if( (addrCode2Str.Compare(VILLA) == 0) && (m_errorIdSet.find( ErrorIdInfo( oID, kAC_AddrPolygonAddrCode2Villa ) ) == m_errorIdSet.end()) )
	{
		m_errorLog->write( kAC_AddrPolygonAddrCode2Villa, getName(), oID, NULL, _T("%d\t%s"), m_curMeshCode, cityCodeStr+addrCode2Str );
		m_errorIdSet.insert( ErrorIdInfo( oID, kAC_AddrPolygonAddrCode2Villa ) );
	}

	// Error code:0x01001002�̃`�G�b�N(�Z��������ʃR�[�h��NULL�A�܂��̓R�[�h�l�h���C���K��l�ȊO�̒l���i�[���Ă���B)
	if( ( isNullOrEmpty( addrType ) || ( (addr_polygon::addr_type::kAreaUndefined != addrTypeCode) && (addr_polygon::addr_type::kDefinedWithChiban != addrTypeCode) && 
		( addr_polygon::addr_type::kDefinedWithDispAddr != addrTypeCode) && (addr_polygon::addr_type::kUndefinedWithChiban != addrTypeCode) ) ) &&
		( m_errorIdSet.find( ErrorIdInfo( oID, kAC_AddrPolygonCheckAddrTypeError11 ) ) == m_errorIdSet.end() ) )
	{
		m_errorLog->write( kAC_AddrPolygonCheckAddrTypeError11, getName(), oID, NULL, _T("%d\t%d"), m_curMeshCode, addrTypeCode );
		m_errorIdSet.insert( ErrorIdInfo( oID, kAC_AddrPolygonCheckAddrTypeError11 ) );
	}

	// Error code:0x01002010�̃`�G�b�N(�u�Z���R�[�h�i�ʏ́j��NotNULL�̎��A�g�����́i���́j�Ɗg�����́i�ʏ́j�̖��̂��قȂ�)
	if( !addrCode2Str.IsEmpty() && ( extName1Str != extName2Str ) && ( m_errorIdSet.find( ErrorIdInfo( oID, kAC_AddrPolygonCheckAddrTypeError10 ) ) == m_errorIdSet.end() ) )
	{
		m_errorLog->write( kAC_AddrPolygonCheckAddrTypeError10, getName(), oID, NULL, _T("%d\t%s\t%s"), m_curMeshCode, extName1Str, extName2Str );
		m_errorIdSet.insert( ErrorIdInfo( oID, kAC_AddrPolygonCheckAddrTypeError10 ) );
	}

	// Error code:0x01004004�̃`�G�b�N(�s�撬���R�[�h�{�Z���R�[�h�̂P�P���Z���R�[�h���Z���}�X�^�ɑ��݂��Ȃ��B)
	if( ( ( m_textAddressMaster.getTextAddressMaster( cityCodeStr + addrCodeStr ) ).status == INVALID_VALUE ) && 
		( m_errorIdSet.find( ErrorIdInfo( oID, kAC_AddrPolygonCheckAddrCodeError1 ) ) == m_errorIdSet.end() ) )
	{
		m_errorLog->write( kAC_AddrPolygonCheckAddrCodeError1, getName(), oID, NULL, _T("%d\t%s"), m_curMeshCode, cityCodeStr + addrCodeStr );
		m_errorIdSet.insert( ErrorIdInfo( oID, kAC_AddrPolygonCheckAddrCodeError1 ) );
	}

	// Error code:0x01004006�̃`�G�b�N(�Z���R�[�h(����)���u000000�v�ȊO�A���A����11���Z���R�[�h�̃e�L�X�g�Z���}�X�^�̃X�e�[�^�X���u2�v)
	if( ( addrCodeStr != DEFAULT_ADDRCODE ) && ( ( m_textAddressMaster.getTextAddressMaster( cityCodeStr + addrCodeStr ) ).status == ABOLITION_CODE ) &&
		m_errorIdSet.find( ErrorIdInfo( oID, kAC_AddrPolygonCheckAddrCodeError3 ) ) == m_errorIdSet.end() )	
	{
		m_errorLog->write( kAC_AddrPolygonCheckAddrCodeError3, getName(), oID, NULL, _T("%d\t%s"), m_curMeshCode, cityCodeStr + addrCodeStr );
		m_errorIdSet.insert( ErrorIdInfo( oID, kAC_AddrPolygonCheckAddrCodeError3 ) );
	}

	// �Z���R�[�h�i�ʏ́j��Not Null�ꍇ
	if( !isNullOrEmpty(addrCode2) )
	{
		// Error code:0x01004005�̃`�G�b�N(�s�撬���R�[�h�{�Z���R�[�h2�̂P�P���Z���R�[�h���Z���}�X�^�ɑ��݂��Ȃ��B)
		if( ( m_textAddressMaster.getTextAddressMaster( cityCodeStr + addrCode2Str ) ).status == INVALID_VALUE  && 
			m_errorIdSet.find( ErrorIdInfo( oID, kAC_AddrPolygonCheckAddrCodeError2 ) ) == m_errorIdSet.end() )
		{
			m_errorLog->write( kAC_AddrPolygonCheckAddrCodeError2, getName(), oID, NULL, _T("%d\t%s"), m_curMeshCode, cityCodeStr + addrCode2Str );
			m_errorIdSet.insert( ErrorIdInfo( oID, kAC_AddrPolygonCheckAddrCodeError2 ) );				
		}

		// Error code:0x01004007�̃`�G�b�N(�Z���R�[�h(�ʏ�)���u000000�v�ȊO�A���A����11���Z���R�[�h�̃e�L�X�g�Z���}�X�^�̃X�e�[�^�X���u2�v)
		if( ( addrCode2Str != DEFAULT_ADDRCODE ) && ( ( m_textAddressMaster.getTextAddressMaster( cityCodeStr + addrCode2Str ) ).status == ABOLITION_CODE ) &&
			m_errorIdSet.find( ErrorIdInfo( oID, kAC_AddrPolygonCheckAddrCodeError4 ) ) == m_errorIdSet.end() )
		{
			m_errorLog->write( kAC_AddrPolygonCheckAddrCodeError4, getName(), oID, NULL, _T("%d\t%s"), m_curMeshCode, cityCodeStr + addrCode2Str );
			m_errorIdSet.insert( ErrorIdInfo( oID, kAC_AddrPolygonCheckAddrCodeError4 ) );
		}
	}

	// �Z��������ʃR�[�h���u0�F�����ڊE�������v�̏ꍇ
	if( addr_polygon::addr_type::kAreaUndefined == addrTypeCode )
	{
		// Error code:0x01002001�̃`�G�b�N(�Z��������ʃR�[�h���u0�F�����ڊE�������v�ŁA�s�撬���R�[�h���u00000�v�ȊO�A�܂��͏Z���R�[�h�܂��͏Z���R�[�h2���u000000�v�ȊO)
		if( ( cityCodeStr != DEFAULT_CITYCODE || addrCodeStr != DEFAULT_ADDRCODE || addrCode2Str != DEFAULT_ADDRCODE ) &&
			( m_errorIdSet.find( ErrorIdInfo( oID, kAC_AddrPolygonCheckAddrTypeError1 ) ) == m_errorIdSet.end() ) )
		{
			m_errorLog->write( kAC_AddrPolygonCheckAddrTypeError1, getName(), oID, NULL, _T("%d\t%s\t%s"), m_curMeshCode, cityCodeStr+addrCodeStr, cityCodeStr+addrCode2Str );
			m_errorIdSet.insert( ErrorIdInfo( oID, kAC_AddrPolygonCheckAddrTypeError1 ) );
		}

		// Error code:0x01002003�̃`�G�b�N(�s���E�̏Z��������ʂ��u0�F�����ڊE�������v�ŁA�X�敄���܂��͊g������(����)��Not Null)
		if( ( !isNullOrEmpty(gaikufugo) || !isNullOrEmpty(extName1) ) && m_errorIdSet.find( ErrorIdInfo( oID, kAC_AddrPolygonCheckAddrTypeError3 ) ) == m_errorIdSet.end() )
		{
			m_errorLog->write( kAC_AddrPolygonCheckAddrTypeError3, getName(), oID, NULL, _T("%d\t%s\t%s"), m_curMeshCode, gaikufugoStr, extName1Str);
			m_errorIdSet.insert( ErrorIdInfo( oID, kAC_AddrPolygonCheckAddrTypeError3 ) );
		}
	}else
	// �Z��������ʃR�[�h���u0�F�����ڊE�������v�ȊO�̏ꍇ
	{
		// Error code:0x01002002�̃`�G�b�N(�Z��������ʃR�[�h���u0�F�����ڊE�������v�ȊO�ŁA�s�撬���R�[�h���u00000�v�A�܂��͏Z���R�[�h�܂��͏Z���R�[�h2���u000000�v�ł���)
		if( ( cityCodeStr == DEFAULT_CITYCODE || addrCodeStr == DEFAULT_ADDRCODE || addrCode2Str == DEFAULT_ADDRCODE ) && 
			( m_errorIdSet.find( ErrorIdInfo( oID, kAC_AddrPolygonCheckAddrTypeError2 ) ) == m_errorIdSet.end() ) )
		{
			m_errorLog->write( kAC_AddrPolygonCheckAddrTypeError2, getName(), oID, NULL, _T("%d\t%s\t%s"), m_curMeshCode, cityCodeStr+addrCodeStr, cityCodeStr+addrCode2Str );
			m_errorIdSet.insert( ErrorIdInfo( oID, kAC_AddrPolygonCheckAddrTypeError2 ) );
		}

		// Error code:0x01002004�̃`�G�b�N(�Z��������ʂ��u�P:�Z���\�������ς݁v�܂��́u2:�n�Ԑ����ς݁v�ŁA�X�敄���Ɗg�����́i���́j���ǂ����Not Null�ł���)
		if( ( ( addrTypeCode == addr_polygon::addr_type::kDefinedWithDispAddr || addrTypeCode == addr_polygon::addr_type::kDefinedWithChiban ) && !isNullOrEmpty(gaikufugo) && !isNullOrEmpty(extName1) ) && 
			( m_errorIdSet.find( ErrorIdInfo( oID, kAC_AddrPolygonCheckAddrTypeError4 ) ) == m_errorIdSet.end() ) )
		{
			m_errorLog->write( kAC_AddrPolygonCheckAddrTypeError4, getName(), oID, NULL, _T("%d\t%s\t%s"), m_curMeshCode, gaikufugoStr, extName1Str );
			m_errorIdSet.insert( ErrorIdInfo( oID, kAC_AddrPolygonCheckAddrTypeError4 ) );
		}

		// Error code:0x01002005�̃`�G�b�N(�Z��������ʂ��u1�F�Z���\�������ς݁v�܂��́u2�F�n�Ԑ����ς݁v�ŁA�X�敄�����u0�v)
		if( ( addrTypeCode == addr_polygon::addr_type::kDefinedWithDispAddr || addrTypeCode == addr_polygon::addr_type::kDefinedWithChiban ) && gaikufugoStr == DEFAULT_BLOCKCODE && 
			( m_errorIdSet.find( ErrorIdInfo( oID, kAC_AddrPolygonCheckAddrTypeError5 ) ) == m_errorIdSet.end() ) )
		{
			m_errorLog->write( kAC_AddrPolygonCheckAddrTypeError5, getName(), oID, NULL, _T("%d"), m_curMeshCode );
			m_errorIdSet.insert( ErrorIdInfo( oID, kAC_AddrPolygonCheckAddrTypeError5 ) );
		}

		// Error code:0x01002006�̃`�G�b�N(�Z��������ʃR�[�h���u1�F�Z���\�������ς݁v�ŁA�X�敄����3���ȏ�̐���)
		if( ( addrTypeCode == addr_polygon::addr_type::kDefinedWithDispAddr ) && ( gaikufugoCode >= 100 ) &&
			( m_errorIdSet.find( ErrorIdInfo( oID, kAC_AddrPolygonCheckAddrTypeError6 ) ) == m_errorIdSet.end() ) )
		{
			m_errorLog->write( kAC_AddrPolygonCheckAddrTypeError6, getName(), oID, NULL, _T("%d\t%s"), m_curMeshCode, gaikufugoStr );
			m_errorIdSet.insert( ErrorIdInfo( oID, kAC_AddrPolygonCheckAddrTypeError6 ) );
		}

		// Error code:0x01002007�̃`�G�b�N(�Z��������ʃR�[�h���u3�G�Z���\���E�n�Ԗ������v�ŁA�X�敄����Not Null)
		if( addrTypeCode == addr_polygon::addr_type::kUndefinedWithChiban && !isNullOrEmpty( gaikufugo ) &&
			m_errorIdSet.find( ErrorIdInfo( oID, kAC_AddrPolygonCheckAddrTypeError7 ) ) == m_errorIdSet.end() )
		{
			m_errorLog->write( kAC_AddrPolygonCheckAddrTypeError7, getName(), oID, NULL, _T("%d\t%s"), m_curMeshCode, gaikufugoStr );
			m_errorIdSet.insert( ErrorIdInfo( oID, kAC_AddrPolygonCheckAddrTypeError7 ) );
		}

		// Error code:0x01002008�̃`�G�b�N(�Z��������ʃR�[�h���u1�F�Z���\�������ς݁v�܂��́u2�F�n�Ԑ����ς݁v�ŁA�X�敄���Ɗg������1��Null)
		if( ( addrTypeCode == addr_polygon::addr_type::kDefinedWithDispAddr || addrTypeCode == addr_polygon::addr_type::kDefinedWithChiban ) && isNullOrEmpty( gaikufugo ) && isNullOrEmpty( extName1 ) &&
			m_errorIdSet.find( ErrorIdInfo( oID, kAC_AddrPolygonCheckAddrTypeError8 ) ) == m_errorIdSet.end() )
		{
			m_errorLog->write( kAC_AddrPolygonCheckAddrTypeError8, getName(), oID, NULL, _T("%d"), m_curMeshCode );
			m_errorIdSet.insert( ErrorIdInfo( oID, kAC_AddrPolygonCheckAddrTypeError8 ) );
		}

		// Error code:0x01002009�̃`�G�b�N(�Z��������ʂ��u�Q�F�n�Ԑ����ς݁v���A�g�����́i���́j��Not Null)
		if( ( addrTypeCode == addr_polygon::addr_type::kDefinedWithChiban ) && !isNullOrEmpty( extName1 ) &&
			m_errorIdSet.find( ErrorIdInfo( oID, kAC_AddrPolygonCheckAddrTypeError9 ) ) == m_errorIdSet.end() )
		{
			m_errorLog->write( kAC_AddrPolygonCheckAddrTypeError9, getName(), oID, NULL, _T("%d\t%s\t%s"), m_curMeshCode, cityCodeStr + addrCodeStr, extName1Str );
			m_errorIdSet.insert( ErrorIdInfo( oID, kAC_AddrPolygonCheckAddrTypeError9 ) );
		}

		// Error code:0x01002012�̃`�G�b�N(�Z��������ʂ��u1�F�Z���\�������ς݁v�܂��́u2�F�n�Ԑ����ς݁v�ŁA�Z���R�[�h�i�ʏ́j�A�g�����́i�ʏ́j�A�g���ǂ݁i�ʏ́j��Not Null)
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

// �d�Ȃ�`�F�b�N
void AddrPolygonCheck::checkOverlap( const IGeometryPtr& ipGeometry, const IFeaturePtr& ipFeature, const CString& query, const CString& info, long oID )
{
	// Error code:0x01004001�̃`�G�b�N(����̎s�撬���R�[�h�����s�撬���|���S���ƈ���d�Ȃ��Ă��Ȃ�)
	IFeatureCursorPtr ipCityCursor;
	long count = AheSelectByShapeAndGetCount( ipGeometry, m_ipCityPolygon, &ipCityCursor, esriSpatialRelIntersects, NULL, query );

	if( ( !ipCityCursor || count == 0 ) && ( m_errorIdSet.find( ErrorIdInfo( oID, kAC_AddrPolygonCheckOverLap ) ) == m_errorIdSet.end() ) )
	{
		m_errorLog->write( kAC_AddrPolygonCheckOverLap, getName(), oID, NULL, _T("%d\t%s"), m_curMeshCode, info );
		m_errorIdSet.insert( ErrorIdInfo( oID, kAC_AddrPolygonCheckOverLap ) );
	}

	return;
}

// ��Ԍ���
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

