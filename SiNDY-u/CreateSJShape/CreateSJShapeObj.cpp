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
#include "CreateSJShapeObj.h"
#include "Workspace.h"
#include "AheLayerFunctions.h"


using namespace std;

FILE* g_pFILE = NULL;

void OpenLogFile(const CString& strFile)
{
	CString strLogFile;
	if( strFile.IsEmpty() )
	{
		TCHAR cBuf[ _MAX_PATH+1 ];
		if( !GetModuleFileName( NULL, cBuf, _MAX_PATH ) )
			return;

		strLogFile.Format( _T("%s"), cBuf );
		long lidx = strLogFile.ReverseFind( '\\' );
		if( lidx == -1 )
			return;

		strLogFile = strLogFile.Left( lidx );
	}
	else
		strLogFile = strFile;

	CString cFileName;
	CTime cTime = CTime::GetCurrentTime();
	long lYear = cTime.GetYear();
	long lMonth = cTime.GetMonth();
	long lDay = cTime.GetDay();
	long lHour = cTime.GetHour();
	long lMinute = cTime.GetMinute();
	long lSecond = cTime.GetSecond();

	cFileName.Format( _T("\\CreateSJShape%04d%02d%02d%02d%02d%02d.log"), lYear, lMonth, lDay, lHour, lMinute, lSecond );
	strLogFile += cFileName;

	fopen_s(&g_pFILE, CT2CA( strLogFile ), "a" );
}

void WriteLog(const CString& msg, bool bDiplayLog = true )
{
	USES_CONVERSION;
	if( bDiplayLog )
		cerr << T2A( msg ) << endl;

	if( !g_pFILE )
		return;

	if( msg.IsEmpty() )
		return;

	fprintf( g_pFILE, CT2CA( msg ) );
	fprintf( g_pFILE, "\n" );
	fflush( g_pFILE );
}

void ReleaseLogFile()
{
	if( g_pFILE )
	{
		fclose( g_pFILE );
		g_pFILE = NULL;
	}
}


CCreateSJShapeObj::CCreateSJShapeObj(void)
{
}

CCreateSJShapeObj::~CCreateSJShapeObj(void)
{
}

bool Usage()
{
	std::cerr << "SJ�p�����[�X�f�[�^�쐬�c�[��" << std::endl;
	std::cerr << "Usage  : CreateSJShape -s <SJ�f�[�^�T�[�o��> -m <���b�V���T�[�o��> -l <���b�V�����X�g> -r <�����[�X�f�[�^�o�̓t�H���_> -c <�s�s�G���APGDB> -o <���O�o�̓t�H���_>" << std::endl;
	return false;
}

// �������֐�
bool CCreateSJShapeObj::Init(int argc, _TCHAR **argv)
{
	if (argc <= 1)
		return Usage();
	while (--argc)
	{
		_TCHAR *s = *++argv;
		switch (*s)
		{
		case '-':
			switch (*++s)
			{
			case 's':
				{
					CString strConnectServer = *++argv;
					ConnectServer( strConnectServer, false );
					--argc;
				}
				break;
			case 'm':
				{
					CString strConnectServer = *++argv;
					ConnectServer( strConnectServer, true );
					--argc;
				}
				break;
			case 'l':
				{
					ifstream file( *++argv );
					if (! file.is_open())
					{
						WriteLog( _T("���b�V�����X�g���J���܂���ł���") );
						return false;
					}
					while (file.good())
					{
						unsigned int m = 0;
						file >> m;
						if (! m) continue;
						m_listMesh.push_back(m);
					}
					--argc;
				}
				break;
			case 'o':
				{
					m_strLogFileName = *++argv;
					--argc;
				}
				break;
			case 'r':
				{
					m_strOutputDir = *++argv;
					--argc;
				}
				break;
			case 'c':
				{
					CString strCityAreaPGDB = *++argv;
					sindy::CWorkspace cWorkspace;
					cWorkspace.Connect( strCityAreaPGDB );
					((IFeatureWorkspacePtr)(IWorkspace*)cWorkspace)->OpenFeatureClass(_bstr_t(_T("CITYAREA")), &m_ipCityArea);
					--argc;
					break;
				}
			default:
				return Usage();
			}
			break;
		default:
			break;
		}
	}

	return true;
}

// �T�[�o�ɐڑ����ĕK�v�ȃt�B�[�`���N���X���擾����
void CCreateSJShapeObj::ConnectServer( const CString& strConnectServer, bool bIsMesh )
{
	sindy::CWorkspace cWorkspace;
	cWorkspace.Connect( strConnectServer );

	// �T�[�o�ڑ�
	IFeatureDatasetPtr ipFeatureDataset;
	if( bIsMesh )
		((IFeatureWorkspacePtr)(IWorkspace*)cWorkspace)->OpenFeatureDataset(_bstr_t(_T("WRD_REF_MESH")), &ipFeatureDataset);
	else
		((IFeatureWorkspacePtr)(IWorkspace*)cWorkspace)->OpenFeatureDataset(_bstr_t(_T("WRD_CITY")), &ipFeatureDataset);

	if( ipFeatureDataset )
	{
		IEnumDatasetPtr ipSubSetsDataset;
		ipFeatureDataset->get_Subsets( &ipSubSetsDataset );
		ipSubSetsDataset->Reset();
		IDatasetPtr ipSubDataset;
		while( ipSubSetsDataset->Next( &ipSubDataset ) == S_OK )
		{
			CString strDatasetName = AheGetFeatureClassName( ipSubDataset );
			if( bIsMesh )
			{
				if( strDatasetName == _T("TERTIARYMESH") )
				{
					m_ipTertiaryMesh = ipSubDataset;
					break;
				}
			}
			else
			{
				m_listSJFeatureClass.push_back( (IFeatureClassPtr)ipSubDataset );
			}
		}
	}
}

// ���C���֐�
bool CCreateSJShapeObj::Run()
{
	OpenLogFile( m_strLogFileName );

	if( !m_ipCityArea )
	{
		WriteLog( _T("CITYAREA���擾�ł��܂���ł���\n") );
		ReleaseLogFile();
		return false;
	}

	for( list< unsigned long >::iterator it = m_listMesh.begin(); it != m_listMesh.end(); ++it )
	{
		// IN�łȂ��ōŏ��ɑS�������Ă���������ق��������̂ŁA���Ƃŗv����
		CString strMeshQuery;
		strMeshQuery.Format( _T("MESHCODE = %u"), *it );
		IFeatureCursorPtr ipTertiaryMeshCursor;
		m_ipTertiaryMesh->Search( AheInitQueryFilter( NULL, NULL, strMeshQuery ), VARIANT_FALSE, &ipTertiaryMeshCursor );
		
		if( !ipTertiaryMeshCursor )
		{
			CString strLog;
			strLog.Format( _T("%u:���b�V�����擾�ł��܂���ł���"), *it );
			WriteLog(strLog);
			ReleaseLogFile();
			return false;
		}
		int nMeshCount = 0;
		IFeaturePtr ipTertiaryMeshFeature;
		// ��{1�����Ȃ��͂�
		while( ipTertiaryMeshCursor->NextFeature( &ipTertiaryMeshFeature ) == S_OK )
		{
			nMeshCount++;
			IGeometryPtr ipTertiaryMeshGeom;
			ipTertiaryMeshFeature->get_ShapeCopy( &ipTertiaryMeshGeom );
			CreateSJShape( *it, ipTertiaryMeshGeom );
		}
		if( nMeshCount != 1 )
		{
			CString strLog;
			if( nMeshCount == 0 )
				strLog.Format( _T("%u:���b�V�����擾�ł��܂���ł���"), *it );
			else
				strLog.Format( _T("%u:���b�V��������������܂���"), *it );
			WriteLog(strLog);
			ReleaseLogFile();
			return false;
		}
	}

	ReleaseLogFile();
	return true;
}

void CCreateSJShapeObj::CreateSJShape( unsigned long lMeshCode, IGeometryPtr ipTertiaryMeshGeom )
{
	for( list< CAdapt< IFeatureClassPtr > >::iterator it = m_listSJFeatureClass.begin(); it != m_listSJFeatureClass.end(); ++it )
	{
		ISpatialReferencePtr ipSpRef;
		((IGeoDatasetPtr)it->m_T)->get_SpatialReference( &ipSpRef );
		CString strFeatureClassName = AheGetFeatureClassName( it->m_T );
		IFeatureClassPtr ipShapeFeatureClass = CreateShapeFeatureClass( lMeshCode, strFeatureClassName, ipSpRef );
		if( ipShapeFeatureClass )
		{
			// ���b�V���`���SDE�t�B�[�`���N���X��Ԍ���
			IFeatureCursorPtr ipFeatureCursor;
			it->m_T->Search( AheInitQueryFilter( AheInitSpatialFilter( NULL, ipTertiaryMeshGeom ), NULL, NULL ), VARIANT_FALSE, &ipFeatureCursor );

			IFeatureCursorPtr	ipShapeInsertCursor;
			ipShapeFeatureClass->Insert(VARIANT_TRUE, &ipShapeInsertCursor);

			IFieldsPtr ipShapeFields, ipFields;
			ipShapeFeatureClass->get_Fields( &ipShapeFields );
			it->m_T->get_Fields( &ipFields );
			map< long, long > mapCorrespondFieldIndex = GetCorrespondFieldIndex( strFeatureClassName, ipShapeFields, ipFields );

			IFeaturePtr ipFeature;
			while( ipFeatureCursor->NextFeature( &ipFeature ) == S_OK )
			{
				IGeometryPtr ipGeom;
				ipFeature->get_ShapeCopy( &ipGeom );
				IGeometryPtr ipIntersectGeom;
				if( strFeatureClassName == _T("HIG_A") || strFeatureClassName == _T("FRM_A") )
					((ITopologicalOperatorPtr)ipGeom)->Intersect( ipTertiaryMeshGeom, esriGeometry1Dimension, &ipIntersectGeom );
				else
					((ITopologicalOperatorPtr)ipGeom)->Intersect( ipTertiaryMeshGeom, esriGeometry2Dimension, &ipIntersectGeom );

				long lOID = 0;
				ipFeature->get_OID( &lOID );

#ifdef _DEBUG
				if( strFeatureClassName == _T("DEC_P") && lOID == 5754 )
				{
					IPointCollectionPtr ipPointCol = ipIntersectGeom;
					long lPointCol = 0;
					ipPointCol->get_PointCount( &lPointCol );
					for( int i = 0; i < lPointCol; ++i )
					{
						IPointPtr ipPoint;
						ipPointCol->get_Point( i, &ipPoint );
						double dX, dY;
						ipPoint->QueryCoords( &dX, &dY );
						CString str;
						str.Format( _T("%ld X=%.20lf Y=%.20lf\n"), i, dX, dY );
						OutputDebugString( str );
					}
				}
#endif
				// ���̃X�i�b�v������ׂ�������ׂ�����Ȃ����͂悭�킩��Ȃ�
				// �Ƃ͂����A������Intersect�������_�ł��łɁA��ԎQ�Ƃ��킳�ꂿ����Ă���H������
				// ���̃X�i�b�v�ɈӖ��͂Ȃ���������Ȃ��B�Ȃ��Ȃ�A���̃X�i�b�v�O��ō��W������Ȃ��̂�
				ipIntersectGeom->putref_SpatialReference( ipSpRef );
				ipIntersectGeom->SnapToSpatialReference();
				// ���b�V�����E�Ő؂�Ă���悤�Ȍ`��i���̃v���W�F�N�g�ɂ͂Ȃ��͂������j���ƁA�`��NULL�ɂȂ����Ⴄ�̂�
				// �ꉞ�`�F�b�N���Ă͂����Ă���
				VARIANT_BOOL vaEmpty = VARIANT_FALSE;
				ipIntersectGeom->get_IsEmpty( &vaEmpty );
				if( vaEmpty )
					continue;

				IFeatureBufferPtr	ipFeatureBuffer;
				ipShapeFeatureClass->CreateFeatureBuffer(&ipFeatureBuffer);
				ipFeatureBuffer->putref_Shape( ipIntersectGeom );
				for( map< long, long >::iterator itField = mapCorrespondFieldIndex.begin(); itField != mapCorrespondFieldIndex.end(); ++itField )
				{
					CComVariant vaValue;
					ipFeature->get_Value( itField->first, &vaValue );
					ipFeatureBuffer->put_Value( itField->second, vaValue );

					// �_�|���S���̃`�F�b�N
					// �_�|���S���Ƃ́ACITYAREA���ɂ���s���S�G���A�̂���
					// �����l60�i�s���S�G���A�j�n�[�h�R�[�f�B���O����������
					if( strFeatureClassName == _T("FRM_P") && vaValue.lVal == 60 )
					{
						IFeatureCursorPtr ipCityAreaCursor;
						m_ipCityArea->Search( AheInitQueryFilter( AheInitSpatialFilter( NULL, ipGeom, _T("Shape"), esriSearchOrderSpatial, esriSpatialRelWithin ), NULL, NULL ), VARIANT_FALSE, &ipCityAreaCursor );
						IFeaturePtr ipCityArea;
						while( ipCityAreaCursor->NextFeature( &ipCityArea ) == S_OK )
						{
							IGeometryPtr ipCityAreaGeom;
							ipCityArea->get_ShapeCopy( &ipCityAreaGeom );
							IRelationalOperatorPtr ipRelOp = ipGeom;
							VARIANT_BOOL vaWithin = VARIANT_FALSE;
							ipRelOp->Within( ipCityAreaGeom, &vaWithin );
							if( vaWithin )
							{
								IFieldsPtr ipShapeFields;
								ipShapeFeatureClass->get_Fields( &ipShapeFields );
								long lSourceIndex = -1;
								ipShapeFields->FindField( _bstr_t( _T("Source") ), &lSourceIndex );
								ipFeatureBuffer->put_Value( lSourceIndex, _variant_t(_bstr_t(_T("cloud"))) );
							}
						}
					}
				}

				CComVariant va;
				HRESULT hr = ipShapeInsertCursor->InsertFeature(ipFeatureBuffer, &va);
				if( !( hr == S_OK && va.lVal >= 0 ) )
				{
					CString strLog;
					strLog.Format( _T("%u\t%s\t�����r����InsertFeature�Ɏ��s���܂���"), lMeshCode, strFeatureClassName );
					WriteLog( strLog );
				}
			}
			HRESULT hr = ipShapeInsertCursor->Flush();
			
			if( hr == S_OK )
			{
				CString strLog;
				strLog.Format( _T("%u\t%s\t��������I��"), lMeshCode, strFeatureClassName );
				WriteLog( strLog );
			}
			else
			{
				CString strLog;
				strLog.Format( _T("%u\t%s\t�����s���iFlush�Ɏ��s�j"), lMeshCode, strFeatureClassName );
				WriteLog( strLog );
			}
		}
	}
}

IFeatureClassPtr CCreateSJShapeObj::CreateShapeFeatureClass( unsigned long lMeshCode, CString strFeatureClassName, ISpatialReferencePtr ipSpRef )
{
	USES_CONVERSION;
	CString strMeshCode, strPrimary, strSecondary;
	strMeshCode.Format( _T("%lu"), lMeshCode );
	strPrimary.Format( _T("%.6s"), strMeshCode );
	strSecondary.Format( _T("%.8s"), strMeshCode );

	CString strPrimaryDir, strSecondaryDir;
	strPrimaryDir = m_strOutputDir + _T("\\") + strPrimary;
	strSecondaryDir = m_strOutputDir + _T("\\") + strPrimary + _T("\\") + strSecondary;
	bool bExist = true;
	if( !PathFileExists( m_strOutputDir ) )
	{
		if( !CreateDirectory( m_strOutputDir, NULL ) )
			bExist = false;
	}
	if( !bExist )
	{
		CString strLog;
		strLog.Format( _T("%s ���쐬�ł��܂���ł���"), m_strOutputDir );
		WriteLog( strLog );
		return false;
	}
	if( !PathFileExists( strPrimaryDir ) )
	{
		if( !CreateDirectory( strPrimaryDir, NULL ) )
			bExist = false;
	}
	if( !bExist )
	{
		CString strLog;
		strLog.Format( _T("%s ���쐬�ł��܂���ł���"), strPrimaryDir );
		WriteLog( strLog );
		return false;
	}
	if( !PathFileExists( strSecondaryDir ) )
	{
		if( !CreateDirectory( strSecondaryDir, NULL ) )
			bExist = false;
	}
	if( !bExist )
	{
		CString strLog;
		strLog.Format( _T("%s ���쐬�ł��܂���ł���"), strSecondaryDir );
		WriteLog( strLog );
		return false;
	}

	// shape�t�@�C�����쐬
	CString strShapeFileName;
	strShapeFileName.Format( _T("%u%s"), lMeshCode, strFeatureClassName );
	// �������ɓ���
	strShapeFileName.MakeLower();

	//-------------------------------------------------------------------------------
	// Shape�t�@�C���̃I�[�v������	
	//-------------------------------------------------------------------------------
	// ���[�N�X�y�[�X���I�[�v��
	IPropertySetPtr			ipPropertySet(CLSID_PropertySet);
	ipPropertySet->SetProperty(_bstr_t(_T("DATABASE")), _variant_t( _bstr_t(strSecondaryDir) ) );
	// ���[�N�X�y�[�X�̃I�[�v��
	IWorkspaceFactoryPtr	ipWSFactory(CLSID_ShapefileWorkspaceFactory);
	IWorkspacePtr			ipWorkspace;
	ipWSFactory->Open(ipPropertySet, 0, &ipWorkspace);
	if( !ipWorkspace) {
		CString strLog;
		strLog.Format( _T("%s �̃��[�N�X�y�[�X�i�f�B���N�g���j�̃I�[�v���Ɏ��s���܂���"), strPrimaryDir );
		WriteLog( strLog );
		return	NULL;
	}

	// �t�B�[�`���N���X�̃I�[�v��
	IFeatureWorkspacePtr	ipFeatureWorkspace(ipWorkspace);
	IFeatureClassPtr		ipFeatureClass;
	ipFeatureWorkspace->OpenFeatureClass(_bstr_t(strShapeFileName), &ipFeatureClass);

	if(ipFeatureClass != NULL) {
		CString strLog;
		strLog.Format( _T("%s.shp:���łɓ�����shape�t�@�C�������݂��Ă��܂�"), strShapeFileName );
		WriteLog( strLog );
		return	NULL;
	}


	// �W�I���g��(Shape)�E�t�B�[���h�ǉ�
	//-------------------------------------------------------------
	IFieldsPtr		ipFields(CLSID_Fields);
	IFieldsEditPtr	ipFieldsEdit(ipFields);

	IFieldPtr		ipFieldObj(CLSID_Field);
	IFieldEditPtr	ipFieldEditObj(ipFieldObj);
	ipFieldEditObj->put_Name(T2BSTR(_T("ObjectID")));
	ipFieldEditObj->put_Type(esriFieldTypeInteger);
	ipFieldEditObj->put_IsNullable(VARIANT_FALSE);

	if(ipFieldsEdit->AddField(ipFieldEditObj) != S_OK) 
		return NULL;

	if( strFeatureClassName == _T("FRM_P") || strFeatureClassName == _T("FRM_A") )
	{
		IFieldPtr		ipField(CLSID_Field);
		IFieldEditPtr	ipFieldEdit(ipField);
		ipFieldEdit->put_Name(T2BSTR(_T("BgCls")));
		ipFieldEdit->put_Type(esriFieldTypeInteger);
		ipFieldEdit->put_IsNullable(VARIANT_FALSE);

		if(ipFieldsEdit->AddField(ipFieldEdit) != S_OK) 
			return NULL;

		if( strFeatureClassName == _T("FRM_P") )
		{
			IFieldPtr		ipField2(CLSID_Field);
			IFieldEditPtr	ipFieldEdit2(ipField2);
			ipFieldEdit2->put_Name(T2BSTR(_T("Source")));
			ipFieldEdit2->put_Type(esriFieldTypeString);
			if(ipFieldsEdit->AddField(ipFieldEdit2) != S_OK) 
				return NULL;
		}
	}
	else if( strFeatureClassName == _T("HIG_P") || strFeatureClassName == _T("HIG_A") )
	{
		IFieldPtr		ipField(CLSID_Field);
		IFieldEditPtr	ipFieldEdit(ipField);
		ipFieldEdit->put_Name(T2BSTR(_T("BldCls")));
		ipFieldEdit->put_Type(esriFieldTypeInteger);
		ipFieldEdit->put_IsNullable(VARIANT_FALSE);

		if(ipFieldsEdit->AddField(ipFieldEdit) != S_OK) 
			return NULL;

		if( strFeatureClassName == _T("HIG_P") )
		{
			IFieldPtr		ipField2(CLSID_Field);
			IFieldEditPtr	ipFieldEdit2(ipField2);
			ipFieldEdit2->put_Name(T2BSTR(_T("CorridorF")));
			ipFieldEdit2->put_Type(esriFieldTypeInteger);
			ipFieldEdit2->put_IsNullable(VARIANT_FALSE);

			if(ipFieldsEdit->AddField(ipFieldEdit2) != S_OK) 
				return NULL;
		}
	}
	else if( strFeatureClassName == _T("DEC_P") )
	{
		IFieldPtr		ipField(CLSID_Field);
		IFieldEditPtr	ipFieldEdit(ipField);
		ipFieldEdit->put_Name(T2BSTR(_T("DecCls")));
		ipFieldEdit->put_Type(esriFieldTypeInteger);
		ipFieldEdit->put_IsNullable(VARIANT_FALSE);

		if(ipFieldsEdit->AddField(ipFieldEdit) != S_OK) 
			return NULL;

		IFieldPtr		ipField2(CLSID_Field);
		IFieldEditPtr	ipFieldEdit2(ipField2);
		ipFieldEdit2->put_Name(T2BSTR(_T("RooftopF")));
		ipFieldEdit2->put_Type(esriFieldTypeInteger);
		ipFieldEdit2->put_IsNullable(VARIANT_FALSE);

		if(ipFieldsEdit->AddField(ipFieldEdit2) != S_OK) 
			return NULL;
	}
	else if( strFeatureClassName == _T("ZLV_P") )
	{
		IFieldPtr		ipField(CLSID_Field);
		IFieldEditPtr	ipFieldEdit(ipField);
		ipFieldEdit->put_Name(T2BSTR(_T("ZLevelCls")));
		ipFieldEdit->put_Type(esriFieldTypeInteger);
		ipFieldEdit->put_IsNullable(VARIANT_FALSE);

		if(ipFieldsEdit->AddField(ipFieldEdit) != S_OK) 
			return NULL;
	}


	IFieldPtr		ipField(CLSID_Field);
	IFieldEditPtr	ipFieldEditShape(ipField);
	if( strFeatureClassName == _T("HIG_A") || strFeatureClassName == _T("FRM_A") )
	{
		ipFieldEditShape->put_Name(T2BSTR(_T("Shape")));
		ipFieldEditShape->put_Type(esriFieldTypeGeometry);
		ipFieldEditShape->put_IsNullable(VARIANT_FALSE);

		IGeometryDefPtr			ipGDef(CLSID_GeometryDef);
		IGeometryDefEditPtr		ipGDefEdit(ipGDef);
		// �i�[����Shape�̃W�I���g���^�C�v��ݒ�
		ipGDefEdit->put_GeometryType(esriGeometryPolyline);

		// Shape�ɂ���ԎQ�Ə[�Ă�
//		ISpatialReferenceTolerancePtr ipTole = ipSpRef;
//		double dTolerance = 0;
		// ����������W���o�͂��邽�߂̂��܂��Ȃ�
//		ipTole->put_XYTolerance( 0.00000003125 );
//		ipTole->get_XYTolerance( &dTolerance );
//		ISpatialReferenceResolutionPtr ipResol = ipSpRef;
		// ����������W���o�͂��邽�߂̂��܂��Ȃ�
//		ipResol->put_XYResolution( VARIANT_FALSE, 0.000000015625 );
		ipGDefEdit->putref_SpatialReference( ipSpRef );
		ipFieldEditShape->putref_GeometryDef(ipGDefEdit);
	}
	else
	{
		ipFieldEditShape->put_Name(T2BSTR(_T("Shape")));
		ipFieldEditShape->put_Type(esriFieldTypeGeometry);
		ipFieldEditShape->put_IsNullable(VARIANT_FALSE);

		IGeometryDefPtr			ipGDef(CLSID_GeometryDef);
		IGeometryDefEditPtr		ipGDefEdit(ipGDef);
		// �i�[����Shape�̃W�I���g���^�C�v��ݒ�
		ipGDefEdit->put_GeometryType(esriGeometryPolygon);

		// Shape�ɂ���ԎQ�Ə[�Ă�
		ISpatialReferenceTolerancePtr ipTole = ipSpRef;
		// ���e�l�Ɛ��x��^���Ă��^���Ȃ��Ă��A���ǂ����̂ŁA�^���Ȃ����Ƃɂ���
		// ���e�l0.0000003125,���x0.000000015625��^����ƃ��b�V�����E�Ő����l��
		// �o�����Ƃ��ł��邪�A���ꂪ�d�v�ł͂Ȃ�
		// ��L���e�l��^����ƁASDE�̃f�[�^�Ə����_9���ڂ��炢����o�͂������W�������
		// �����炭���e�l�͈͓����炢�Ȃ̂��낤�Ǝv����
		// �^���Ȃ��ꍇ�ɂ́ASDE�̒l���珬���_15���ڂ��炢�ł����
		// ����͂����炭double�l�̒l�̂��ꂾ�낤�Ǝv����
		// ����SJ�v���W�F�N�g�ł́A���b�V�����E�t�߂Ő����l���o���Ȃ��Ƃ������Ƃ�
		// ���ɖ��ɂ͂Ȃ�Ȃ��͂��iSJ�ɃT���v����2���b�V���o�������Ɠ�����ԁj
//		double dTolerance = 0;
//		ipTole->get_XYTolerance( &dTolerance );
		// ����������W���o�͂��邽�߂̂��܂��Ȃ�
//		ipTole->put_XYTolerance( 0.00000003125 );
//		ISpatialReferenceResolutionPtr ipResol = ipSpRef;
//		double dReso = 0;
//		ipResol->get_XYResolution( VARIANT_FALSE, &dReso );
		// ����������W���o�͂��邽�߂̂��܂��Ȃ�
//		ipResol->put_XYResolution( VARIANT_TRUE, dTolerance / 2.0 );
//		ipResol->put_XYResolution( VARIANT_FALSE, 0.000000015625 );
		ipGDefEdit->putref_SpatialReference( ipSpRef );
		ipFieldEditShape->putref_GeometryDef(ipGDefEdit);
	}

	if(ipFieldsEdit->AddField(ipFieldEditShape) != S_OK) 
		return NULL;

	//-------------------------------------------------------------


	ipFeatureWorkspace->CreateFeatureClass(_bstr_t( strShapeFileName ), ipFields,
		NULL, NULL, esriFTSimple, _bstr_t(_T("Shape")), CComBSTR(""), &ipFeatureClass);

	return ipFeatureClass;
}

map< long, long > CCreateSJShapeObj::GetCorrespondFieldIndex( CString strFeatureClassName, IFieldsPtr ipShapeFields, IFieldsPtr ipFields )
{
	map< long, long > mapFieldIndex;
	long lShapeIndex = -1, lIndex = -1;
	ipFields->FindField( _T("ObjectID"), &lIndex );
	ipShapeFields->FindField( _T("ObjectID"),  &lShapeIndex );
	mapFieldIndex[ lIndex ] = lShapeIndex;

	if( strFeatureClassName == _T("FRM_P") || strFeatureClassName == _T("FRM_A") )
	{
		ipFields->FindField( _T("BgCls"), &lIndex );
		ipShapeFields->FindField( _T("BgCls"),  &lShapeIndex );
		mapFieldIndex[ lIndex ] = lShapeIndex;
	}
	else if(  strFeatureClassName == _T("HIG_P") || strFeatureClassName == _T("HIG_A") )
	{
		ipFields->FindField( _T("BldCls"), &lIndex );
		ipShapeFields->FindField( _T("BldCls"),  &lShapeIndex );
		mapFieldIndex[ lIndex ] = lShapeIndex;
		if( strFeatureClassName == _T("HIG_P") )
		{
			ipFields->FindField( _T("CorFlag"), &lIndex );
			ipShapeFields->FindField( _T("CorridorF"),  &lShapeIndex );
			mapFieldIndex[ lIndex ] = lShapeIndex;
		}
	}
	else if(  strFeatureClassName == _T("DEC_P") )
	{
		ipFields->FindField( _T("DecCls"), &lIndex );
		ipShapeFields->FindField( _T("DecCls"),  &lShapeIndex );
		mapFieldIndex[ lIndex ] = lShapeIndex;
		ipFields->FindField( _T("RooftopFlag"), &lIndex );
		ipShapeFields->FindField( _T("RooftopF"),  &lShapeIndex );
		mapFieldIndex[ lIndex ] = lShapeIndex;

	}
	else if(  strFeatureClassName == _T("ZLV_P") )
	{
		ipFields->FindField( _T("ZLevelCls"), &lIndex );
		ipShapeFields->FindField( _T("ZLevelCls"),  &lShapeIndex );
		mapFieldIndex[ lIndex ] = lShapeIndex;
	}

	return mapFieldIndex;
}
