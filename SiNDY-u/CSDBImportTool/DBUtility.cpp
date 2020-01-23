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

// DBUtility.cpp: CDBUtility �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DBUtility.h"
#include "globalfunc.h"
#include <sindy/workspace.h> 
namespace DBUtility
{



IWorkspacePtr OpenSDEWorkspace(LPCTSTR lpcszUser, LPCTSTR lpcszPassword, LPCTSTR lpcszVersion, LPCTSTR lpcszInstance, LPCTSTR lpcszServer)
{
	CString connectStr;
	connectStr.Format(_T("%s@%s(%s)"),	lpcszUser, lpcszServer, lpcszVersion);
	
	return sindy::create_workspace(connectStr);
}

IFeatureDatasetPtr OpenDataset(IWorkspacePtr ipWorkspace, LPCTSTR lpcszName)
{
	IFeatureWorkspacePtr ipFeatureWorkspace(ipWorkspace);
	IFeatureDatasetPtr ipDataset;
	if( ipFeatureWorkspace )
		ipFeatureWorkspace->OpenFeatureDataset( CComBSTR( lpcszName ), &ipDataset );
	return ipDataset;
}

IDatasetPtr CreateDataset( IWorkspacePtr ipWorkspace, LPCTSTR lpcszName)
{
	IFeatureDatasetPtr ipDataset;

	// ��ԎQ�Ƃ�CityMesh����q��
	ISpatialReferencePtr ipSpRef = GetSpatialReference( OpenFeatureClass( ipWorkspace, _T("CityMesh") ) );
	IFeatureWorkspacePtr ipFeatureWorkspace(ipWorkspace);
	if( ipFeatureWorkspace )
		ipFeatureWorkspace->CreateFeatureDataset( CComBSTR( lpcszName ), ipSpRef, &ipDataset );
	// ��ԎQ�Ƃ��쐬
	//ISpatialReferenceFactoryPtr ipSpRefFactory(CLSID_SpatialReferenceEnvironment);
	//IGeographicCoordinateSystemPtr ipGeoCoordinateSystem;
	//if(SUCCEEDED(ipSpRefFactory->CreateGeographicCoordinateSystem(esriSRGeoCS_Tokyo, &ipGeoCoordinateSystem))) {
	//	ISpatialReferencePtr ipSpRef(ipGeoCoordinateSystem);
	//	ipSpRef->SetDomain(122.0, 155.0, 19.0, 50.0);			// �ܓx�o�x�͈̔͂����
	//	ipSpRef->SetFalseOriginAndUnits(122.0, 19.0, 64000000);	// �ܓx�o�x�̐��x�����
	//	ipSpRef->SetZFalseOriginAndUnits(0, 1);					// Z�l�̐ݒ�
	//	ipSpRef->SetMFalseOriginAndUnits(0, 1);					// M�l�̐ݒ�

	//	IFeatureWorkspacePtr ipFeatureWorkspace(ipWorkspace);
	//	if( ipFeatureWorkspace )
	//		ipFeatureWorkspace->CreateFeatureDataset( CComBSTR( lpcszName ), ipSpRef, &ipDataset );
	//}
	return ipDataset;
	
}

IFeatureClassPtr OpenFeatureClass(IWorkspacePtr ipWorkspace, LPCTSTR lpcszName)
{
	IFeatureWorkspacePtr ipFeatureWorkspace(ipWorkspace);
	IFeatureClassPtr ipFeatureClass;
	if( ipFeatureWorkspace )
		ipFeatureWorkspace->OpenFeatureClass(CComBSTR(lpcszName), &ipFeatureClass);

	return ipFeatureClass;
}

ITablePtr OpenTable(IWorkspacePtr ipWorkspace, LPCTSTR lpcszName)
{
	IFeatureWorkspacePtr ipFeatureWorkspace(ipWorkspace);
	ITablePtr ipTable;
	ipFeatureWorkspace->OpenTable(CComBSTR(lpcszName), &ipTable);
	return ipTable;
}

ISpatialReferencePtr GetSpatialReference(IFeatureClassPtr ipFeatureClass)
{
	IGeoDatasetPtr ipGeoDataset(ipFeatureClass);
	ISpatialReferencePtr ipSpatialReference;
	ipGeoDataset->get_SpatialReference(&ipSpatialReference);
	return ipSpatialReference;
}

DOUBLE CalcDistance(IPointPtr ipPoint1, IPointPtr ipPoint2)
{
	DOUBLE dDistance = 0.0;
	IProjectedCoordinateSystemPtr ipProjectedCoordinateSystem;
	ISpatialReferenceFactory2Ptr ipSpatialReferenceFactory(CLSID_SpatialReferenceEnvironment);
	ipSpatialReferenceFactory->CreateProjectedCoordinateSystem(esriSRProjCS_TokyoJapan10, &ipProjectedCoordinateSystem);
	ipPoint1->Project(ipProjectedCoordinateSystem);
	ipPoint2->Project(ipProjectedCoordinateSystem);
	((IProximityOperatorPtr)ipPoint1)->ReturnDistance(ipPoint2, &dDistance);
	return dDistance;
}

IWorkspacePtr CreateEditVersion( IWorkspacePtr ipWorkspace, LPCTSTR strVersionName )
{
	IVersionPtr ipNewVersion;
	if( ipWorkspace )
	{
		// ���ɓ����o�[�W�����������������
		IVersionPtr ipOldVer;
		if( SUCCEEDED( (( IVersionedWorkspacePtr )ipWorkspace)->FindVersion( CComBSTR( strVersionName ), &ipOldVer ) ) )
		{
			CComBSTR bstrOldName;
			ipOldVer->get_VersionName( &bstrOldName );
			if( SUCCEEDED( ipOldVer->Delete() ) ){
				CString strMsg;
				strMsg.Format( _T("�����̃o�[�W����%s�������܂����B\r\n"), bstrOldName);
				_tprintf( strMsg );
			}
		}

		// �o�[�W�����V�K�쐬
		if( SUCCEEDED( ((IVersionPtr)ipWorkspace)->CreateVersion( CComBSTR(strVersionName), &ipNewVersion ) ) )
		{
			CComBSTR bstrNewVerName;
			if( SUCCEEDED( ipNewVersion->get_VersionName( &bstrNewVerName ) ) ){
				CString strMsg;
				strMsg.Format( _T("�o�[�W����%s���쐬���܂����B\r\n"), bstrNewVerName);
				_tprintf( strMsg );
			}
		}

	}
	return ipNewVersion;
}


IFeatureClassPtr ImitateFeatureClass( LPCTSTR lpcszFeatureClassName, IFeatureClass* ipImitativeFeature, IWorkspace* ipTargetWorkspace, LPCTSTR lpcszDatasetname /*= NULL*/ )
{
	if( !ipTargetWorkspace )
		ipTargetWorkspace = GetWorkspace( ipImitativeFeature );

	// ���ɑ��݂��Ă���t�B�[�`���N���X�����������
	IFeatureClassPtr ipRetFeatureClass = OpenFeatureClass( ipTargetWorkspace, lpcszFeatureClassName );
	if( ipRetFeatureClass )
	{
		DeleteFeatureClass( ipRetFeatureClass );
		ReOpenWorkspace( GetWorkspace( ipRetFeatureClass ) );
		ipRetFeatureClass = NULL;
		//return ipRetFeatureClass;
	}

	if( ipImitativeFeature )
	{
		// �f�[�^�Z�b�g���擾�B�w�肪�Ȃ���Γ����f�[�^�Z�b�g
		IFeatureDatasetPtr ipDataset;
		if( lpcszDatasetname != NULL )
			ipDataset = GetDataset( GetWorkspace( ipImitativeFeature ), lpcszDatasetname );
		else
			ipImitativeFeature->get_FeatureDataset( &ipDataset );


		// �t�B�[���h�\�����R�s�[
		IFieldsPtr ipFields;
		ipImitativeFeature->get_Fields( &ipFields );

		if( ipDataset )
			ipDataset->CreateFeatureClass(CComBSTR(lpcszFeatureClassName), ipFields, 0, 0, esriFTSimple, CComBSTR(_T("SHAPE")), CComBSTR(_T("")), &ipRetFeatureClass );
		else
		{
			// �f�[�^�Z�b�g�������ꍇ
			IFeatureWorkspacePtr ipFeatureWS( ipTargetWorkspace );
			if( ipFeatureWS )
				ipFeatureWS->CreateFeatureClass(CComBSTR(lpcszFeatureClassName), ipFields, 0, 0, esriFTSimple, CComBSTR(_T("SHAPE")), CComBSTR(_T("REFERENCE_UNI")), &ipRetFeatureClass ); 
		}

		// ModelInfo��t���Ă���
		((IModelInfoPtr)ipRetFeatureClass )->put_ModelName( CComBSTR( lpcszFeatureClassName ) );
	}
	return ipRetFeatureClass;
}

IWorkspacePtr GetWorkspace( IFeatureClass* ipFeatureClass )
{
	IWorkspacePtr ipRetWorkspace;
	IDatasetPtr ipDataset(ipFeatureClass);
	if(ipDataset)
		ipDataset->get_Workspace( &ipRetWorkspace );

	//if( ipFeatureClass )
	//{
	//	IFeatureDatasetPtr ipDataset;
	//	// �f�[�^�Z�b�g�������ꍇ��NULL��Ԃ�
	//	if( SUCCEEDED( ipFeatureClass->get_FeatureDataset(&ipDataset) ) && ipDataset )
	//	{
	//		ipDataset->get_Workspace(&ipRetWorkspace);
	//	}
	//}
	return ipRetWorkspace;
}

IDatasetPtr GetDataset( IWorkspace* ipWorkspace, LPCTSTR lpcszDatasetName )
{
	IDatasetPtr ipRetDataset;

	// �����̃f�[�^�Z�b�g��T���ĕԂ�
	IEnumDatasetPtr ipEnumDataset;
	ipWorkspace->get_Datasets( esriDTFeatureDataset, &ipEnumDataset );
	ipEnumDataset->Reset();
	IDatasetPtr ipDataset;
	while( ipEnumDataset->Next( &ipDataset ) == S_OK && ipDataset )
	{
		CComBSTR bstrDatasetName;
		ipDataset->get_Name( &bstrDatasetName );
		CString strDatasetName(bstrDatasetName);
		if( strDatasetName.Find( lpcszDatasetName ) != -1 )
		{
			 ipRetDataset = ipDataset;
			 break;
		}
	}
	return ipRetDataset;
}

void DeleteFeatureClass( IFeatureClass* ipFeatureClass )
{
	if( ipFeatureClass )
	{
		// �Ώۂ̃t�B�[�`���N���XID�𒲂ׂĂ���
		long lID=0;
		ipFeatureClass->get_FeatureClassID( &lID );
		IFeatureDatasetPtr ipDataset;
		// �t�B�[�`���N���X�̊܂܂��f�[�^�Z�b�g���擾
		if( SUCCEEDED( ipFeatureClass->get_FeatureDataset( &ipDataset ) ) && ipDataset )
		{
			// �T�u�Z�b�g������
			IEnumDatasetPtr ipSubsets;
			IDatasetPtr ipSubset;
			ipDataset->get_Subsets( &ipSubsets );
			ipSubsets->Reset();
			while( ipSubsets->Next( &ipSubset ) == S_OK && ipSubset )
			{
				// �T�u�Z�b�g���t�B�[�`���N���X��������ID�ƍ����đΏۂ�����
				IFeatureClassPtr ipTgtFeatureClass( ipSubset );
				if( ipTgtFeatureClass )
				{
					long lTgtID=0;
					ipTgtFeatureClass->get_FeatureClassID( &lTgtID );
					if( lID == lTgtID )
					{
						ipSubset->Delete();
						break;
					}
				}
			}
		}
		else
		{
			// ���[�N�X�y�[�X�ɒ��̃X�^���h�A�����t�B�[�`���N���X�̏ꍇ
			IDatasetPtr ipFCDataset( ipFeatureClass );
			ipFCDataset->Delete();
		}
	}
}

void ReOpenWorkspace( IWorkspace* ipWorkspace )
{
	if( ipWorkspace )
	{
		IPropertySetPtr ipPropSet;
		ipWorkspace->get_ConnectionProperties( &ipPropSet );
		ipWorkspace = NULL;

		IWorkspaceFactoryPtr ipWorkspaceFactory(CLSID_SdeWorkspaceFactory);
		ipWorkspaceFactory->Open(ipPropSet, 0, &ipWorkspace);

	}
}

IFeatureClassPtr CreateFeatureClass(IWorkspacePtr ipWorkspace, IDataset* ipDataset, LPCTSTR lpcszLayerName, IFieldsPtr ipFields, esriFeatureType eType )
{
	IFeatureWorkspacePtr ipFeatureWorkspace(ipWorkspace);
	IFeatureClassPtr ipFeatureClass;
	
	IUIDPtr ipCLSID( CLSID_UID );
	ipCLSID->put_Value( CComVariant( _T("esriGeoDatabase.Feature" ) ) );
	
	if( !ipDataset )
		ipFeatureWorkspace->CreateFeatureClass( CComBSTR(lpcszLayerName), ipFields, ipCLSID, 0, eType, CComBSTR(_T("SHAPE")), CComBSTR(_T("")), &ipFeatureClass );
	else
	{
		HRESULT hr = ((IFeatureDatasetPtr)ipDataset)->CreateFeatureClass( CComBSTR(lpcszLayerName), ipFields, ipCLSID, 0, eType, CComBSTR(_T("SHAPE")), CComBSTR(_T("REFERENCE_UNI")), &ipFeatureClass );
		ATLTRACE(_T("!!"));
	}

	return ipFeatureClass;
}

IFeatureClassPtr CreateFeatureClassInDataset(IDatasetPtr ipDataset, LPCTSTR lpcszLayerName, IFieldsPtr ipFields, esriFeatureType eType )
{
	IFeatureDatasetPtr ipFeatureDataset( ipDataset );
	IFeatureClassPtr ipFeatureClass;
	//if( ipFeatureWorkspace )
		HRESULT hr = ipFeatureDataset->CreateFeatureClass( CComBSTR(lpcszLayerName), ipFields, 0, 0, eType, CComBSTR(_T("SHAPE")), CComBSTR(_T("")), &ipFeatureClass );

	return ipFeatureClass;
}

IFieldPtr CreateField( esriFieldType cType, CString cFieldName, LPCTSTR cAliasName, long cLength, long cPrecision, long cScale, VARIANT_BOOL cIsNullable, VARIANT_BOOL cEditable, CString& strErr )
{
	IFieldEditPtr ipFieldEdit(CLSID_Field);

	bool bSucsess = true;

	if( FAILED( ipFieldEdit->put_Name( CComBSTR( cFieldName ) ) ) ) 
		bSucsess = false;

	if( esriFieldTypeOID != cType ){
		if( FAILED( ipFieldEdit->put_AliasName( CComBSTR(cAliasName) ) ) ||
			FAILED( ipFieldEdit->put_Type( cType ) ) ||
			FAILED( ipFieldEdit->put_IsNullable( cIsNullable ) ) || 
			FAILED( ipFieldEdit->put_Editable( cEditable ) ) ||
			FAILED( ipFieldEdit->put_DomainFixed(VARIANT_TRUE) ) 
			//FAILED( ipFieldEdit->put_Required(VARIANT_FALSE) )  )
			)
			bSucsess = false;

		HRESULT hr;
		switch( cType )
		{
		case esriFieldTypeString: 
			hr = ipFieldEdit->put_Length( cLength ); break;
		case esriFieldTypeDouble:
			hr = ipFieldEdit->put_Scale( cScale ); // break���Ȃ�
		case esriFieldTypeSmallInteger:
		case esriFieldTypeInteger:
			hr = ipFieldEdit->put_Precision( cLength ); break;
		default: break;
		}
		if( hr != S_OK)
		{
			ATLTRACE(_T("!!"));
		}
		//if(  cType == esriFieldTypeString && cLength > 0 ){
		//	if( FAILED( ipFieldEdit->put_Length( cLength ) ) ) 
		//	bSucsess = false;
		//}


		//if( cPrecision > 0 ){
		//	if( FAILED( ipFieldEdit->put_Precision( cPrecision ) ) ) 
		//	bSucsess = false;
		//}
		//if( cScale > 0 ){
		//	if( FAILED( ipFieldEdit->put_Precision( cLength ) ) || FAILED( ipFieldEdit->put_Scale( cScale ) ) ) 
		//	bSucsess = false;
		//}
	}
	else{
		if( FAILED( ipFieldEdit->put_Type( cType ) ) ) 
			bSucsess = false;
	}

	strErr += bSucsess ? _T("") : _T("�t�B�[���h�쐬�G���[�F ") + cFieldName + _T("\n");
	return bSucsess ? ipFieldEdit : NULL;
}

//Shape�t�B�[���h�̍쐬
IFieldPtr CreateShapeField(LPCTSTR lpcszFieldName, esriGeometryType eGeometryType, bool bHasM, bool bHasZ )
{
	HRESULT hr;

	IFieldEditPtr ipFieldEdit(CLSID_Field);
	
	ISpatialReferenceFactoryPtr ipSpRefFactory(CLSID_SpatialReferenceEnvironment);
	IGeographicCoordinateSystemPtr ipGeoCoordinateSystem;
	if(SUCCEEDED(ipSpRefFactory->CreateGeographicCoordinateSystem(esriSRGeoCS_Tokyo, &ipGeoCoordinateSystem))) {
		ISpatialReferencePtr ipSpRef(ipGeoCoordinateSystem);
		ipSpRef->SetDomain(122.0, 155.0, 19.0, 50.0);			// �ܓx�o�x�͈̔͂����
		ipSpRef->SetFalseOriginAndUnits(122.0, 19.0, 64000000);	// �ܓx�o�x�̐��x�����
		ipSpRef->SetZFalseOriginAndUnits(0, 1);					// Z�l�̐ݒ�
		ipSpRef->SetMFalseOriginAndUnits(0, 1);					// M�l�̐ݒ�
	//}


	// �W�I���g���̖���
	if (ipFieldEdit->put_Name(_bstr_t(lpcszFieldName)) != S_OK)
		return false;

	// �t�B�[���h�̃^�C�v��esriFieldTypeGeometry
	if (ipFieldEdit->put_Type(esriFieldTypeGeometry) != S_OK)
		return false;

	// NULL�͋����Ȃ�
	if (ipFieldEdit->put_IsNullable(VARIANT_FALSE) != S_OK)
		return false;

	
	if (ipFieldEdit->put_Required(VARIANT_FALSE) != S_OK)
		return false;

	IGeometryDefEditPtr ipGeometryDef(CLSID_GeometryDef);
	// �`��^�Ƌ�ԎQ�Ƃ�ݒ�
	if ( FAILED( hr = ipGeometryDef->put_GeometryType(eGeometryType)) ) return hr;
	if ( FAILED( hr = ipGeometryDef->put_HasM(bHasM?VARIANT_TRUE:VARIANT_FALSE)) ) return hr;
	if ( FAILED( hr = ipGeometryDef->put_HasZ(bHasZ?VARIANT_TRUE:VARIANT_FALSE)) ) return hr;
	if ( FAILED( hr = ipGeometryDef->put_GridCount(1)) ) return hr;
	if ( FAILED( hr = ipGeometryDef->put_GridSize(0, 0.05)) ) return hr;	// ����Q�������O���b�h�T�C�Y
	//if ( FAILED( hr = ipGeometryDef->putref_SpatialReference(ISpatialReferencePtr(ipGeoCoordinateSystem))) ) return hr;
	//if ( FAILED( hr = ipGeometryDef->putref_SpatialReference( ipSpRef )) ) return hr;
	if ( FAILED( hr = ipFieldEdit->putref_GeometryDef(ipGeometryDef) ) ) return hr;
	}

	return ipFieldEdit;
}

IFieldsPtr CreateFieldsFromSchema( tFieldSchema tFieldinfo[], int nSize )
{
	IFieldsEditPtr ipFields( CLSID_Fields );

	CString strErr;

	// ObjectID �t�B�[���h
	IFieldEditPtr ipOIDField( CLSID_Field );
	if( FAILED( ipOIDField->put_Type( esriFieldTypeOID ) ) || 
		FAILED( ipOIDField->put_Name( CComBSTR( _T("OBJECTID") ) ) ) ||
		//FAILED( ipOIDField->put_Required( VARIANT_TRUE ) ) ||
		//FAILED( ipOIDField->put_IsNullable(VARIANT_FALSE) ) ||
		FAILED( ipFields->AddField( ipOIDField ) ) )
	{
		strErr += _T("OBJECTID�t�B�[���h���쐬�ł��Ȃ�\n");
	}

	for( int i=0; i<nSize; ++i)
	{
		IFieldPtr ipField =
			CreateField( tFieldinfo[i].emType,
						 tFieldinfo[i].strFieldName, 
						 tFieldinfo[i].strAlias, 
						 tFieldinfo[i].lLength, 
						 0, 
						 tFieldinfo[i].lScale, 
						 tFieldinfo[i].vbNullable, 
						 VARIANT_TRUE,
						 strErr						);
		if( !(ipField && SUCCEEDED( ipFields->AddField( ipField ) ) ) )
		{
			strErr = _T("�t�B�[���h�ǉ��G���[: ") + tFieldinfo[i].strFieldName + _T("\n");
			_tprintf( strErr );
		}
	}

	// �W�I���g���t�B�[���h
	//IFieldPtr ipShpField;
	//HRESULT hr = CreateShapeField(  _T("SHAPE"), esriGeometryPoint, false, false, &ipShpField );
	//hr = ipFields->AddField( ipShpField );
	//HRESULT hr = ipFields->AddField( CreateShapeField( _T("SHAPE"), esriGeometryPoint, false, false ) );

	//IFieldCheckerPtr ipCheck;
	//IEnumFieldErrorPtr ipEnumErr;
	//IFieldsPtr ipFixedFields;
	//ipCheck->Validate( (IFieldsPtr)ipFields, &ipEnumErr, &ipFixedFields );
	//ipEnumErr->Reset();
	//IFieldErrorPtr ipErrField;
	//while( ipEnumErr->Next( &ipErrField) && ipErrField )
	//{
	//	esriFieldNameErrorType eType;
	//	ipErrField->get_FieldError( &eType );
	//	if( eType > 0 )
	//	{
	//		_tprintf( _T("!"));
	//	}
	//}

	return ipFields;
}

IFieldsPtr CreateFCFields( tFieldSchema tFieldinfo[] )
{
	IFieldsEditPtr ipFields( CLSID_Fields );

	CString strErr;

	// ObjectID �t�B�[���h
	IFieldEditPtr ipOIDField( CLSID_Field );
	if( FAILED( ipOIDField->put_Type( esriFieldTypeOID ) ) || 
		FAILED( ipOIDField->put_Name( CComBSTR( _T("OBJECTID") ) ) ) ||
		//FAILED( ipOIDField->put_Required( VARIANT_TRUE ) ) ||
		//FAILED( ipOIDField->put_IsNullable(VARIANT_FALSE) ) ||
		FAILED( ipFields->AddField( ipOIDField ) ) )
	{
		strErr += _T("OBJECTID�t�B�[���h���쐬�ł��Ȃ�\n");
	}

	// �����e�[�u���R�t���p�t�B�[���h
	IFieldPtr ipField = CreateField( esriFieldTypeInteger, _T("ATTRTBLID"), _T("�����e�[�u��ID"), 0, 0, 0, VARIANT_FALSE, VARIANT_TRUE, strErr );
	ipFields->AddField( ipField );

	// ID�t�B�[���h
	ipField =  CreateField( 
				tFieldinfo[ID_INDEX].emType,
				tFieldinfo[ID_INDEX].strFieldName, 
				tFieldinfo[ID_INDEX].strAlias, 
				tFieldinfo[ID_INDEX].lLength, 
				0, 
				tFieldinfo[ID_INDEX].lScale, 
				tFieldinfo[ID_INDEX].vbNullable, 
				VARIANT_TRUE,
				strErr						);
	ipFields->AddField( ipField );

	// �W�I���g���t�B�[���h
	IFieldPtr ipShpField = CreateShapeField( _T("SHAPE"), esriGeometryPoint, false, false );
	ipFields->AddField( ipShpField );

	return ipFields;
}

}// namespace DBUtility
