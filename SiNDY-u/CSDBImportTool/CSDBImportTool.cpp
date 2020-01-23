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

// CSDBImportTool.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include "glovalclass.h"
#include "globalfunc.h"
#include "DBUtility.h"
#include "ImportData.h"
#include "ConectInfoDlg.h"
#include "PlogressDlg.h"
#include "ErrorHandler.h"
#include <arctl/coinitializer.h>



void SetValue( _IRow* ipNewRow, CString& strID, std::map<long, CComVariant>& mapVal, CConectInfoDlg& cConnectDlg, CImportData& cImportData, CErrorHandler& cErr)
{			
	IFieldsPtr ipFields;
	ipNewRow->get_Fields( &ipFields );
	typedef std::pair<long, CComVariant> idx_val;
	foreach( idx_val val, mapVal )
	{
		IFieldPtr ipField;
		ipFields->get_Field(val.first, &ipField);
		CString strErrFields;

		//if( VT_BSTR == val.second.vt && CString(val.second).IsEmpty() )
		//{
		//	val.second.ChangeType( VT_NULL );
		//}
		
		gf::CheckFieldValue( ipField, val.second, strErrFields );
			cErr.SetResult( ipNewRow->put_Value( val.first, val.second ), strID, val.first, ERR_FIELD );
		
		//if( !strErrFields.IsEmpty() )
		//	cErr.FieldErr( CString(val.second), strErrFields );
	}

	// �����e�[�u���ɃX�g�A
	cErr.SetResult( ipNewRow->Store(), strID, -1, ERR_STORE );

	// OID�擾�i�R�t���p�j
	long lTableOID = -1;
	ipNewRow->get_OID( &lTableOID );

	// ID�Ɋ֘A����`�󃊃X�g���擾
	CImportData::POINTSMAP mapPoints = cImportData.GetPoints( strID );
	typedef std::pair<CString, WKSPoint> name_point;
	foreach( name_point point, mapPoints )
	{
		// �`����쐬�B�t�B�[�`���N���X�̋�ԎQ�Ƃƍ��킹��
		IPointPtr ipPointGeom( CLSID_Point );
		ISpatialReferencePtr ipSPRef;
		((IGeoDatasetPtr)cConnectDlg.GetFC())->get_SpatialReference( &ipSPRef );
		ipPointGeom->putref_SpatialReference( ipSPRef );
		ipPointGeom->PutCoords( point.second.X, point.second.Y );

		// �t�B�[�`��������Č`���ݒ�
		IFeaturePtr ipNewFeature;
		cConnectDlg.GetFC()->CreateFeature( &ipNewFeature );
		cErr.SetResult( ipNewFeature->putref_Shape( ipPointGeom ), strID, -1, ERR_SHAPE );

		// �e�[�u����ID��ݒ�
		ipNewFeature->put_Value( cConnectDlg.AttTblIndex(), CComVariant( lTableOID ) );

		// ID��ݒ�
		ipNewFeature->put_Value( cConnectDlg.IDIndex(), CComVariant(strID) );

		ipNewFeature->Store();

	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	const arctl::coinitializer aCoInitializer;

	CConectInfoDlg cConnectDlg( argv[1] );
	if( cConnectDlg.DoModal() == IDOK && cConnectDlg.CheckValue() )
	{
		if( !cConnectDlg.Connect() )
		{
			// �ڑ����s
			//MessageBox( 0, _T("�t�B�[�`���N���X�܂��̓e�[�u���̍쐬�Ɏ��s���܂����B"), _T("�T�[�o�G���["), MB_OK|MB_ICONERROR );
			return 0;
		}

		ITablePtr ipTable = cConnectDlg.GetTable();

		IFieldsPtr ipFields;
		ipTable->get_Fields( &ipFields );
		CImportData cImportData( cConnectDlg.GetSrcFilePath(), cConnectDlg.GetSheetName(), ipFields );



		CErrorHandler cErr( cConnectDlg.GetLogFilePath() );

		((IWorkspaceEditPtr)cConnectDlg.m_ipWorkspace)->StartEditing( VARIANT_FALSE );

		// �C���|�[�g�����X�g�擾
		CImportData::RECORDSMAP& mapRecords = cImportData.GetReCords();
		
		// �v���O���X�o�[�̏���i�C���|�[�g����t�@�C���̃T�C�Y�j���Z�b�g
		CPlogressDlg cProgDlg;
		cProgDlg.Create(NULL);
		cProgDlg.ShowWindow(SW_SHOW);
		cProgDlg.SetRange( 0, mapRecords.size() );

		// �X�V
		std::list<CString> lstIDs;
		gf::IDs2Str( cImportData.GetIDList(), lstIDs, _T("'") );
		CString strQuery;
		strQuery.Format( _T("ID IN (%s)"), *lstIDs.begin() );

		IQueryFilterPtr ipQuery(CLSID_QueryFilter);
		ipQuery->put_WhereClause( CComBSTR(strQuery) );
		ipQuery->put_SubFields(_T("*"));

		std::set<CString> setUpdates;
		_ICursorPtr ipCursor;
		ipTable->Search( ipQuery , VARIANT_FALSE, &ipCursor );
		if( ipCursor )
		{
			long lIDIndex = -1;
			ipCursor->FindField( L"ID", &lIDIndex );
			_IRowPtr ipRow;
			while( ipCursor->NextRow( &ipRow ) == S_OK )
			{
				CComVariant vaID;
				ipRow->get_Value( lIDIndex, &vaID );
				CImportData::RECORDSMAP::iterator it = mapRecords.find( CString(vaID) );
				if( it != mapRecords.end() )
				{
					cProgDlg.SetMsg( CString(vaID) );

					// ���W���������Ă���ꍇ�͊����̃|�C���g�������Ă����ibug 8885�j
					if( ! cImportData.GetPoints( CString(vaID) ).empty() )
					{
						long lOID = -1;
						ipRow->get_OID( &lOID );
						CString strGeomQuery;
						strGeomQuery.Format( _T("ATTRTBLID = %ld"), lOID );
						IQueryFilterPtr ipGeomQuery(CLSID_QueryFilter);
						ipGeomQuery->put_WhereClause( CComBSTR(strGeomQuery) );
						IFeatureCursorPtr ipGeomCursor;
						cConnectDlg.GetFC()->Search( ipGeomQuery, VARIANT_FALSE, &ipGeomCursor );
						IFeaturePtr ipFeature;
						while( ipGeomCursor->NextFeature( &ipFeature ) == S_OK )
						{
							cErr.SetResult( ipFeature->Delete(), cConnectDlg.GetFCAliass(), lOID, ERR_DELETE );
						}
					}

					// �����e�[�u�����X�V
					SetValue( ipRow, CString(vaID), it->second, cConnectDlg, cImportData, cErr);

					setUpdates.insert( CString(vaID) );
					cProgDlg.OffsetPos(1);
				}
			}
		}


		// �V�K�쐬
		typedef std::pair< CString, std::map<long, CComVariant> > _id_val;
		foreach( _id_val id_val, mapRecords )
		{
			// �X�V����Ȃ��������̂͐V�K�쐬�Ώ�
			if( setUpdates.find( id_val.first) != setUpdates.end() )
				continue;

			cProgDlg.SetMsg( id_val.first );

			// �C���T�[�g
			_IRowPtr ipNewRow;
			HRESULT hr = ipTable->CreateRow( &ipNewRow );

			SetValue( ipNewRow, id_val.first, id_val.second, cConnectDlg, cImportData, cErr);

			cProgDlg.OffsetPos(1);
		}
		((IWorkspaceEditPtr)cConnectDlg.m_ipWorkspace)->StopEditing( VARIANT_TRUE );
		cProgDlg.DestroyWindow();
	}

	return 0;
}

