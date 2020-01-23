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
// ���֎���H�������X�g������B
//////////////////////////////////////////////////////////////////////
bool CMakeList::create( IFeatureClass* ipLinkClass, const std::vector<CAdapt<IFeatureClassPtr>>& vecFeatureClass, bool bShape, std::ofstream& ofp, LPCTSTR lpcszShapeDir, LPCTSTR lpcszLinkDir )
{
	// �擾�����t�B�[�`���N���X�Ń��[�v������
	m_lCount = 0;
	m_lFeatureClassSize = vecFeatureClass.size();
	m_strShapeDir = lpcszShapeDir;
	m_strLinkDir = lpcszLinkDir;

	std::cerr << "�����֎���H�����X�g�擾������...\n";
	for( std::vector<CAdapt<IFeatureClassPtr>>::const_iterator it = vecFeatureClass.begin(); it != vecFeatureClass.end(); ++it )
	{
		m_lCount++;
		std::cerr << "\t��IFeatureClass�� " << m_lCount << " / " << m_lFeatureClassSize <<  " ��\n";
		// shape�̏���
		if( bShape )
		{
			CComBSTR bstrName;
			IDatasetPtr(it->m_T)->get_Name( &bstrName );
			bool bFlag = false;	// �ŏd�v�H�����ǂ����̓t�B�[�`���N���X���Ŕ��ʁB�t�B�[�`���N���X�擾���ɔ��ʂ��Ă������񂾂��ǂ�...
			if( _tcsicmp( CString(bstrName), NoParkingRegR ) == 0 )
				bFlag = false;	// �d�v�H��
			else if( _tcsicmp( CString(bstrName), NoParkingRegRImp ) == 0 )
				bFlag = true;	// �ŏd�v�H��
			else
			{
				// �����ɗ��邱�Ƃ͂Ȃ��͂������ǁA�ꉞ
				std::cerr << "# ERROR\t"<< CT2A(CString(bstrName)) <<"�F�z��O�̃��C�����擾���Ă��܂��B\n";
				return false;
			}

			if( !SearchClass( ipLinkClass, it->m_T, _T(""), ofp, bFlag ) )
				return false;
		}
		// PGDB�̏���
		else
		{
			// �ŏd�_����H��
			if( !SearchClass( ipLinkClass, it->m_T, _T("AttrCode = '911@'"), ofp, true ) )
				return false;

			// �d�_����H��
			if( !SearchClass( ipLinkClass, it->m_T, _T("AttrCode = '912@'"), ofp, false ) )
				return false;
		}
	}
	std::cerr << "�����֎���H�����X�g�擾�����I��\n";

//#ifdef _DEBUG
	// ���֎���H�����X�g��shape�o��
	if( !m_mapNoParkingLink.empty() && !m_strShapeDir.IsEmpty() )
	{
		std::cerr << "�����֎���H�����X�gshape�o�͒�...\n";
		if( OutputShape( ipLinkClass ) )
			std::cerr << "�����֎���H�����X�gshape�o�͏I��\n";
		else
		{
			std::cerr << "�����֎���H�����X�gshape�o�͏I��\n";
			return false;
		}
	}


	// ���ԋ֎~����G���A�̃��C����shape�ɏo��
	// �s���{���ʂ�shape�ɂȂ��Ă���̂Ō��f�[�^���܂Ƃ߂ďo�͂�����
	// PGDB�̏ꍇ�͂��܂�Ӗ����Ȃ�
	if( !m_vecBaseLink.empty() && !m_strLinkDir.IsEmpty() )
	{
		std::cerr << "�����ԋ֎~�����Ԃ̃��C����shape�o�͒�...\n";
		OutputBaseLink( m_vecBaseLink );
		std::cerr << "�����ԋ֎~�����Ԃ̃��C����shape�o�͏I��\n";
	}
//#endif
	return true;
}

//////////////////////////////////////////////////////////////////////
// ���ԋ֎~���擾�������s���B
//////////////////////////////////////////////////////////////////////
bool CMakeList::SearchClass( IFeatureClass* ipLinkClass, IFeatureClass* ipNoParkClass, LPCTSTR lpcszWhereClause, std::ofstream &ofp, bool bFlag )
{
	IQueryFilterPtr ipQFilter(CLSID_QueryFilter);
	if( !CString(lpcszWhereClause).IsEmpty() )	// WhereClause���Ȃ���΃X���[
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
				std::cerr << "\t\t" << ((bFlag) ? "�ŏd�_����H��" : "�d�_����H��") << "�쐬��...\t"<< lCount  << " / " << lTotalCount << " ��\r";

				IGeometryPtr ipGeom = ipFeature->GetShapeCopy();
				if( ipGeom )
				{
					// �t�B�[�`���擾�i���Ƃ�shape�o�͂���̂Łj
					m_vecBaseLink.push_back( ipFeature );

					// 20m�o�b�t�@���쐬����
					ISpatialReferencePtr ipRef =((IGeoDatasetPtr)ipLinkClass)->GetSpatialReference();
					if( ipRef )
					{
						IGeometryPtr ipBuffer = GetBuffer( ipGeom );
						if( ipBuffer )
						{
							// �o�b�t�@�Ɋ܂܂铹�H�����N���擾����B
							long lNoParking_c = (bFlag) ? 1 : 2;
							if( !CompData( ipLinkClass, ipBuffer, ofp, lNoParking_c ) )
								return false;
						}
						else
						{
							std::cout << "# ERROR\tSearchClass()\t�t�B�[�`���`��o�b�t�@�쐬���s\n";
							return false;
						}
					}
					else
					{
						std::cout << "# ERROR\tSearchClass()\t��ԎQ�Ǝ擾���s\n";
						return false;
					}
				}
				else
				{
					std::cout << "# ERROR\tSearchClass()\t�t�B�[�`���R�s�[�쐬���s\n";
					return false;
				}
			}
		}
		else
		{
			std::cout << "# ERROR\tSearchClass()\t�t�B�[�`���������s\n";
			return false;
		}
	}
	else
	{
		std::cout << "# ERROR\tSearchClass()\t�t�B�[�`���J�E���g���擾���s\n";
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

	// IGeometry::Project()������O�ɁA��ԎQ�Ƃ��Z�b�g����Ă��邩�`�F�b�N����
	ISpatialReferencePtr ipCheckSpRef;
	ipCloneGeom->get_SpatialReference(&ipCheckSpRef);
	long lFcCode = 0;
	ipCheckSpRef->get_FactoryCode(&lFcCode);
	if(lFcCode == 0){
		// ��ԎQ�Ƃ��Z�b�g����Ă��Ȃ��`��̏ꍇ�̓f�t�H���g�l���w�肵�Ă�����
		ISpatialReferencePtr ipDefSpRef;
		ipSpRefFct->CreateSpatialReference(esriSRGeoCS_Tokyo, &ipDefSpRef);
		ipCloneGeom->putref_SpatialReference(ipDefSpRef);
	}
	ipCloneGeom->Project(ipSpRef);

	return ((IGeometryPtr)ipClone);
}

//////////////////////////////////////////////////////////////////////
// �����N�ɉ�����20m�o�b�t�@���쐬����B
//////////////////////////////////////////////////////////////////////
IGeometryPtr CMakeList::GetBuffer(IGeometry* ipGeom)
{
	IGeometryPtr ipLinkBuffer;
	ITopologicalOperatorPtr ipTopo( get_Tokyo10CloneGeom(ipGeom) );
	ipTopo->Buffer( 20, &ipLinkBuffer );

	// Arc10.3.1�ł͍\���_�����Ȃ��Ȑ������p���ꂽ�o�b�t�@���쐬����邽�߁A
	// Densify()�����{����Arc10.1�ɋ߂Â���
	// Densify()�̑�2�p�����[�^�́A�ŏ�(2 * XYResolution)�ƃf�t�H���g(2 * XYResolution * 100)
	// �̊Ԃ����A(2 * XYResolution * 10)�Ƃ���
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
// �o�b�t�@�ɂЂ������郊���N�����X�g�A�b�v����B
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
	// �����̏����A����Ȃ��ˁH�H
	// �o�b�t�@�Ɋ��S����̃����N�𒊏o����B
	IFeatureCursorPtr ipContainCursor;
	if( S_OK == ipLinkClass->Search( ipSpatialFilter, VARIANT_FALSE, &ipContainCursor ) ) 
	{
		IFeaturePtr ipContainLink;
		while( ipContainCursor->NextFeature( &ipContainLink ) == S_OK )
		{
			CComVariant	vaClass	= ipContainLink->GetValue( ipContainLink->Fields->_FindField(road_link::kRoadClass) );
			// �L���ł͂Ȃ���{���H�ȊO�͑ΏۊO
			if( vaClass.lVal == 0 || vaClass.lVal > 7 )
				continue;
			setOID.insert(ipContainLink->GetOID());
		}
	}
	else
	{
		std::cout << "# ERROR\tCompData()\t�o�b�t�@�Ɋ��S����̃����N�������s\n";
		return false;
	}
	///////////////////////////////////////////////

	// �o�b�t�@�ɏd�Ȃ��Ă��郊���N�𒊏o����B
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
			// �L���ł͂Ȃ���{���H�ȊO�͑ΏۊO
			if( vaClass.lVal == 0 || vaClass.lVal > 7 )
				continue;

			// �ŏd�_�Ώۂ͏d�_�Ώۂ��珜�O����
			std::map<long,CNoParkingLink>::const_iterator iter_linkid = m_mapNoParkingLink.find(lOID);
			if( iter_linkid != m_mapNoParkingLink.end() )
				continue;

			// �����N�`��̎擾
			IGeometryPtr ipGeo = ipIntersectLink->GetShapeCopy();
			std::set<long>::const_iterator iter_containid = setOID.find(lOID);

			// �o�b�t�@�Ɋ��S�����Ă��Ȃ������N�̓����N���Ŕ��f����B
			if( iter_containid == setOID.end())
			{
				// ���̃����N��
				double dLen = GetLinkLen(ipGeo);

				// �o�b�t�@�Ɋ܂܂�郊���N��
				double dBuffLen = GetIntersecLinkLen( ipGeo, ipBuff, lOID );
	
				// �����N��8�����܂܂�Ă��Ȃ��ꍇ�͑ΏۊO
				if( dBuffLen / dLen * 100 < 80 ) 
					continue;

			}

			// �ŏd�_�H����ΏۂɑI�񂾏ꍇ�̓I�u�W�F�N�gID���i�[����
	//		if( lClass == 1 )
				//m_LinkOIDSet.insert(lOID);
			CNoParkingLink cNoParkingLink;
			cNoParkingLink.ipLink = ipIntersectLink;
			cNoParkingLink.lNoParkingClass = lClass;
			m_mapNoParkingLink.insert( std::pair<long,CNoParkingLink>( lOID, cNoParkingLink ) );

			// �Ώۃ����N�����݂��郁�b�V���R�[�h���擾����B
			long lMeshCode = GetExistMesh(ipGeo);

			// ���X�g���e���o�͂���B
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
		std::cout << "# ERROR\tCompData()\t�o�b�t�@�ɏd�Ȃ��Ă��郊���N�������s\n";
		return false;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////
// �����N�����v������B
//////////////////////////////////////////////////////////////////////
double CMakeList::GetLinkLen( IGeometry* ipLinkGeom )
{
	double dLength = 0;

	IPolylinePtr ipLink(get_Tokyo10CloneGeom(ipLinkGeom));
	dLength = ipLink->GetLength();

	return dLength;
}

//////////////////////////////////////////////////////////////////////
// �o�b�t�@�Ɋ܂܂�郊���N�����v������B
//////////////////////////////////////////////////////////////////////
double CMakeList::GetIntersecLinkLen( IGeometry* ipLinkGeo, IGeometry* ipPolygon, long lOID )
{
	HRESULT hr;
	IGeometryPtr ipNewGeometry = get_Tokyo10CloneGeom(ipLinkGeo);
	IGeometryPtr ipPoly = get_Tokyo10CloneGeom(ipPolygon);

	ITopologicalOperatorPtr ipTopo(ipNewGeometry);
	IGeometryPtr ipIntersect = 0;

	// �o�b�t�@�Ɋ܂܂�Ă��郊���N�`����擾����B
	if (FAILED(hr = ipTopo->Intersect(ipPoly, esriGeometry1Dimension, &ipIntersect)))
	{
		IErrorInfoPtr ipErrorInfo = 0;
		::GetErrorInfo(0, &ipErrorInfo);
		if (ipErrorInfo != 0)
		{
			CComBSTR strDescription = 0;
			ipErrorInfo->GetDescription( &strDescription );
		}
		std::cout << "# WARNING\t�����N�̃N���b�v�Ɏ��s�I\t" << lOID << std::endl;
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

	// �擾�����`�󂻂ꂼ��̒��������v����B(����ѐ؂�W���ł��ЂƂ̃|�����C���Ō`������Ă���̂ŕK�v�Ȃ��B)
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
// �����N���݃��b�V���R�[�h���擾����B
///////////////////////////////////////////////////////////
long CMakeList::GetExistMesh(IGeometry* ipLink)
{
	IPointCollectionPtr ipPointCo(ipLink);
	long lPointCount = ipPointCo->GetPointCount();

	IPointPtr ipFromPoint	= ipPointCo->GetPoint(0);
	IPointPtr ipToPoint		= ipPointCo->GetPoint(1);

	// ��1�\���_�Ƒ�2�\���_�̒��ԓ_�̃��b�V���R�[�g�������N���b�V���Ƃ���B
	double dX = (ipFromPoint->GetX() + ipToPoint->GetX()) / 2;
	double dY = (ipFromPoint->GetY() + ipToPoint->GetY()) / 2;

	crd_cnv crd;
	int iMeshCode = 0, iX = 0, iY = 0;
	crd.LLtoMesh(dX, dY, 2, &iMeshCode, &iX, &iY, 1);

	return iMeshCode;
}

//////////////////////////////////////////////////////////////////////
// 10�i����16�i���ւ̕ϊ�
//////////////////////////////////////////////////////////////////////
void CMakeList::SetObjectID(CString &rCstrID, int rID)
{
	CString cstrID, cstrTmp;
	cstrID.Format(_T("%x"), rID);

	// �󂢂�����0��}��
	for(int iLength = 0; iLength < (8 - cstrID.GetLength()); ++iLength)
		cstrTmp += "0";

	rCstrID = _T("0x") + cstrTmp + cstrID;
}

bool CMakeList::OutputShape( IFeatureClass* ipLinkClass )
{
	USES_CONVERSION;
	// Shape�̃p�X�ݒ�
	CString strShapeFile;
	strShapeFile.Format( _T("%s\\noparking_link"), m_strShapeDir );

	CSHPHdl cSHPHandle;
	CDBFHdl cDBFHdl;
	// �`��f�[�^�쐬�i�t�B�[�`���N���X�쐬�j
	cSHPHandle.create( T2CA(strShapeFile), SHPT_ARC );

	// �t�B�[���h�f�[�^�쐬�i�t�B�[�`���N���X�̃t�B�[���h�쐬�j
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
			std::cout << "# ERROR\t" << CT2A(sindy::schema::road_link::kFromNodeID) << " �t�B�[���h�擾���s\n";
			return false;
		}
		if( lTo < 0 )
		{
			std::cout << "# ERROR\t" << CT2A(sindy::schema::road_link::kToNodeID) << " �t�B�[���h�擾���s\n";			
			return false;
		}
	}
	else
	{
		std::cout << "# ERROR\t���H�����N�N���X����t�B�[���h�擾���s\n";
		return false;
	}

	// shape�f�[�^�쐬
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
		// shape�`��ɂ���
		CSHPObj aArcObj( SHPT_ARC, aPointArray );
		// �t�B�[�`���N���X�Ƀt�B�[�`����ǉ��ishape�f�[�^��˂����ށj
		nRet = cSHPHandle.writeObject( aArcObj, -1 );
	}
	return nRet;
}

void CMakeList::OutputBaseLink( const std::vector<CAdapt<IFeaturePtr>>& vecBaseLink )
{
	USES_CONVERSION;
	// Shape�̃p�X�ݒ�
	CString strShapeFile;
	strShapeFile.Format( _T("%s\\baselink"), m_strLinkDir );

	CSHPHdl cSHPHandle;
	CDBFHdl cDBFHdl;
	// �`��f�[�^�쐬�i�t�B�[�`���N���X�쐬�j
	cSHPHandle.create( T2CA(strShapeFile), SHPT_ARC );

	// �t�B�[���h�f�[�^�쐬�i�t�B�[�`���N���X�̃t�B�[���h�쐬�j
	cDBFHdl.create( T2CA(strShapeFile) );
	long lOID = cDBFHdl.addField( "OBJECTID", FTInteger, 9, 0 );

	// shape�f�[�^�쐬
	long lSize = vecBaseLink.size();
	long lCount = 0;
	for( std::vector<CAdapt<IFeaturePtr>>::const_iterator it = vecBaseLink.begin(); it != vecBaseLink.end(); ++it )
	{
		lCount++;
		std::cerr << lCount << " / " << lSize << " ��\r";
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
