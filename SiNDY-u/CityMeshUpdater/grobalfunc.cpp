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
#include "grobalfunc.h"
#include <list>
#include <boost/regex.hpp>
#include <boost/tokenizer.hpp>
#include <sindy/workspace.h>

namespace gf {
	namespace gf_private{
		MESHCODEMAP Cursor2Map( IDifferenceCursor* ipCursor, LPCTSTR strKey )
		{
			MESHCODEMAP retMap;

			_IRowPtr ipRow;
			long lCityMeshCodeIndex = -1;
			if(ipCursor)
			{
				long lOID=0;
				while( ipCursor->Next( &lOID, &ipRow ) == S_OK )
				{
					if( !ipRow ) break;

					if( lCityMeshCodeIndex == -1 )
					{
						IFieldsPtr ipFields;
						ipRow->get_Fields( &ipFields );
						ipFields->FindField( CComBSTR(strKey), &lCityMeshCodeIndex );
					}
					CComVariant vaValue;
					ipRow->get_Value( lCityMeshCodeIndex, &vaValue );
					retMap[vaValue.lVal] = ipRow;
				}
			}
			return retMap;
		}

		// �e�[�u������S�s�S�t�B�[���h�E���Ă���J�[�\��
		_ICursorPtr GetTableCursor( IWorkspace* ipWorkspace, LPCTSTR strTableName )
		{
			_ICursorPtr ipCursor;

			if(ipWorkspace)
			{
				ITablePtr ipTable;
				((IFeatureWorkspacePtr)ipWorkspace)->OpenTable( CComBSTR( strTableName ), &ipTable );
				if(ipTable)
				{
					IQueryFilterPtr ipQueryFilter( CLSID_QueryFilter );
					ipQueryFilter->put_SubFields( CComBSTR(_T("*")) );
					ipTable->Search( ipQueryFilter, VARIANT_FALSE, &ipCursor );
				}
			}
			return ipCursor;
		}


		// MESHCODE�ň����N�G���[�����
		void CreateQuery( MESHCODEMAP::const_iterator it, MESHCODEMAP::const_iterator& itEnd, std::list<CString>& lstStrQuery )
		{
			CString strQuery = _T("MESHCODE IN (");
			for( int i=0; it != itEnd; it++, i++)
			{
				CString strTmp;
				strTmp.Format( _T("%ld,"), it->first );
				strQuery += strTmp;
				// 1000�������甲����
				if( i==100 )
					break;
			}
			// �Ō�� , �������đ̍ق𐮂���
			strQuery=strQuery.TrimRight( _T(",") ) + _T(")");
			lstStrQuery.push_back( strQuery );

			// �}�b�v��S��������܂ōċA
			if( it != itEnd )
				CreateQuery( ++it, itEnd, lstStrQuery );
		}


		// �t�B�[���h�C���f�b�N�X�̑Ή��}�b�v�����
		std::map<long, long> IndexMatching( const std::vector<CString> &vecRowKeyField, _IRow* ipValueRow )
		{
			std::map<long, long> retMap;
			IFieldsPtr ipFields;
			ipValueRow->get_Fields(&ipFields);
			if(ipFields)
			{
				for( int i=0; i < vecRowKeyField.size(); ++i )
				{
					// ���b�V���R�[�h��OID�t�B�[���h�͍X�V�ΏۊO
					if( vecRowKeyField[i].CompareNoCase( _T("OBJECTID") ) != 0 && vecRowKeyField[i].CompareNoCase( MESHCORD_FIELDNAME ) != 0 )
					{
						long lValueIndex = -1;
						ipFields->FindField( CComBSTR(vecRowKeyField[i]), &lValueIndex );
						if(lValueIndex > -1)
							retMap[i] = lValueIndex;
					}
				}
			}

			CString strMsg = _T("\r\n��r�Ώۃt�B�[���h�F");
			for( std::map<long, long>::const_iterator it = retMap.begin(); it != retMap.end(); ++it )
			{
				strMsg += _T("[") + vecRowKeyField[ it->first ] + _T("]");
			}
			g_cOutput.OutputMsg( strMsg + _T("\r\n") );

			return retMap;
		}

		std::map<long, long> IndexMatching( _IRow* ipKeyRow, _IRow* ipValueRow )
		{
			std::map<long, long> retMap;
			if(ipKeyRow && ipValueRow )
			{
				IFieldsPtr ipKeyFields, ipValFields;
				ipKeyRow->get_Fields( &ipKeyFields );
				ipValueRow->get_Fields( &ipValFields );
				if(ipKeyFields && ipValFields)
				{
					long lFields=0;
					ipKeyFields->get_FieldCount( &lFields );
					// �L�[�ɂȂ�ق��̃t�B�[���h���ł܂킷
					for( long i=0; i<lFields; i++)
					{
						IFieldPtr ipKeyField;
						if( SUCCEEDED( ipKeyFields->get_Field( i, &ipKeyField ) ) )
						{
							CComBSTR bstrFieldName;
							ipKeyField->get_Name( &bstrFieldName );
							// OBJECTID �� MESHCODE�t�B�[���h�͍X�V�ΏۊO
							if( bstrFieldName != _T("OBJECTID") && bstrFieldName != MESHCORD_FIELDNAME)
							{
								long lValueIndex = -1;
								ipValFields->FindField( bstrFieldName, &lValueIndex );
								if(lValueIndex > -1)
									retMap[i] = lValueIndex;
							}

						}
					}
				}
			}
			return retMap;
		}

		int exception_filter(unsigned int code, struct _EXCEPTION_POINTERS *ep) {

			if (code == EXCEPTION_ACCESS_VIOLATION)
				return EXCEPTION_EXECUTE_HANDLER;

			else
				return EXCEPTION_CONTINUE_SEARCH;
		}
		HRESULT IDataset_getWorkspace( IDataset* ipDataset, IWorkspace** ppWork )
		{
			if( ! ipDataset || ! ppWork )
				return E_POINTER;

			// �Ԃт�������(bug 1765)
			// �]����try...catch�ł͈���������Ȃ��̂ŁA�\������O���g�p
			__try {
				return ipDataset->get_Workspace( ppWork );
			}
			__except( exception_filter( GetExceptionCode(), GetExceptionInformation() ) ) {
				ATLTRACE(_T("��O : ExceptionCode[%X]\r\n"), GetExceptionCode() );
			}
			return E_FAIL;
		}

		IWorkspacePtr GetWorkspaceFromRow( _IRow* ipRow )
		{
			IWorkspacePtr ipRetWS;

			if( ipRow )
			{
				ITablePtr ipTable;
				ipRow->get_Table( &ipTable );
				IDatasetPtr ipDataset(ipTable);
				IDataset_getWorkspace( ipDataset, &ipRetWS );
			}
			return ipRetWS;
		}

	} //namespace gf_private

std::vector<CString> Tokenize2Vector( const CString & strSrc, LPCTSTR lpcszTorkens, LPCTSTR lpcszTrim )
{
	std::string str = CT2A(strSrc);
	std::vector<CString> retVec;

    typedef boost::char_separator<char> char_separator;
    typedef boost::tokenizer<char_separator> tokenizer;

    char_separator sep(",", "", boost::keep_empty_tokens);
    tokenizer tokens(str, sep);

    for (tokenizer::iterator tok_iter = tokens.begin(); tok_iter != tokens.end(); ++tok_iter) {
		retVec.push_back( CString( CA2T( (*tok_iter).c_str() ) ) );
    }


	return retVec;
}


IWorkspacePtr CreateNewVersion( IWorkspacePtr ipWorkspace, LPCTSTR strVersionName )
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
				g_cOutput.OutputMsg( strMsg );
			}
		}

		// �o�[�W�����V�K�쐬
		if( SUCCEEDED( ((IVersionPtr)ipWorkspace)->CreateVersion( CComBSTR(strVersionName), &ipNewVersion ) ) )
		{
			CComBSTR bstrNewVerName;
			if( SUCCEEDED( ipNewVersion->get_VersionName( &bstrNewVerName ) ) ){
				CString strMsg;
				strMsg.Format( _T("�o�[�W����%s���쐬���܂����B\r\n"), bstrNewVerName);
				g_cOutput.OutputMsg( strMsg );
			}
		}

	}
	return ipNewVersion;
}

IWorkspacePtr OpenWorkSpace( LPCTSTR strServername, LPCTSTR strUsername, LPCTSTR strPassword, LPCTSTR strVersionName )
{
	CString strConnectionInfo;
	strConnectionInfo.Format(_T("%s/%s/%s/%s"), strUsername, strPassword, strVersionName, strServername );

	IWorkspacePtr ipRetWorkSpace = sindy::create_workspace(strConnectionInfo);

	if( !ipRetWorkSpace ){
		CString strMsg;
		strMsg.Format( _T("�ڑ��Ɏ��s���܂����B %s@%s(%s)\r\n"), strUsername, strServername, strVersionName );
		g_cOutput.OutputMsg( strMsg );
	}

	return ipRetWorkSpace;
}

bool DiffMapBetweenVersion( IWorkspacePtr ipCurrentWorkspace, LPCTSTR strTableName, MESHCODEMAP& DiffMap )
{
	g_cOutput.OutputMsg( _T("CITYMESH�e�[�u����SDE.DEFAULT�ƍX�V���o�[�W�����̍������擾...") );

	MESHCODEMAP RetMap;

	// �f�t�H���g�o�[�W�������擾
	IVersionPtr ipDefaultVer;
	int nDiffRecords = 0;
	if( SUCCEEDED( ((IVersionedWorkspacePtr)ipCurrentWorkspace)->get_DefaultVersion( &ipDefaultVer ) ) )
	{
		// �f�t�H���g�ƌ��݂̃o�[�W�������炻�ꂼ��e�[�u�����擾
		IFeatureClassPtr ipDefaultbl, ipDstTable;
		if( SUCCEEDED( ((IFeatureWorkspacePtr)((IFeatureWorkspacePtr)ipDefaultVer))->OpenFeatureClass( CComBSTR( strTableName ), &ipDefaultbl ) ) &&
			SUCCEEDED( ((IFeatureWorkspacePtr)ipCurrentWorkspace)->OpenFeatureClass( CComBSTR( strTableName ), &ipDstTable ) ) )
		{
			// ���݂̃o�[�W�����ƃf�t�H���g�o�[�W�����̍��������
			IDifferenceCursorPtr ipCursor;
			HRESULT hr = ((IVersionedTablePtr)ipDstTable)->Differences( (ITablePtr)ipDefaultbl, esriDifferenceTypeUpdateNoChange, NULL, &ipCursor );
			if( SUCCEEDED( hr ) || FDO_E_SE_STATES_ARE_SAME == hr )
			{
				// �������Ȃ�����OK
				DiffMap = gf_private::Cursor2Map( ipCursor, MESHCORD_FIELDNAME );

				CString strMsg;
				strMsg.Format( _T("%d [Records] (a)\r\n"), DiffMap.size() );
				g_cOutput.OutputMsg( strMsg );
				return true;
			}else{
				g_cOutput.OutputMsg( _T("\r\n�f�t�H���g�o�[�W�����Ƃ̍����擾�Ɏ��s���܂����B\r\n") ); }
		}else{
			g_cOutput.OutputMsg( _T("\r\n�f�t�H���g�܂��͍X�V���o�[�W�����̃e�[�u���擾�Ɏ��s���܂����B\r\n") ); }
	}else{
		g_cOutput.OutputMsg(_T("\r\n�f�t�H���g�o�[�W�����̎擾�Ɏ��s���܂����B\r\n")); }

	return false;
}

MESHCODEMAP Table2Map( IWorkspace* ipWorkspace, LPCTSTR strTableName, LPCTSTR strKey )
{
	MESHCODEMAP retMap;

	_ICursorPtr ipCursor = gf_private::GetTableCursor( ipWorkspace, strTableName );
	_IRowPtr ipRow;
	long lCityMeshCodeIndex = -1;
	if(ipCursor)
	{
		while( ipCursor->NextRow( &ipRow ) == S_OK )
		{
			if( lCityMeshCodeIndex == -1 )
			{
				IFieldsPtr ipFields;
				ipRow->get_Fields( &ipFields );
				ipFields->FindField( CComBSTR(strKey), &lCityMeshCodeIndex );
			}
			CComVariant vaValue;
			ipRow->get_Value( lCityMeshCodeIndex, &vaValue );
			retMap[vaValue.lVal] = ipRow;
		}
	}
	_tprintf( _T("%d [Records]"), retMap.size() );
	return retMap;
}


int DeleteDuplicateKeys( MESHCODEMAP& TargetMap, const MESHCODEVECMAP& CompareMap )
{
	g_cOutput.OutputMsg( _T("(a)����TMP_CITYMESH�Ƃ̏d�����R�[�h���폜...") );

	int retNum=0;
	for( MESHCODEVECMAP::const_iterator it = CompareMap.begin(); it != CompareMap.end(); it++ )
	{
		if( TargetMap.erase( it->first ) )
			retNum++;
	}

	CString strMsg;
	strMsg.Format( _T("%d [delete](b)\r\n"), retNum );
	g_cOutput.OutputMsg( strMsg );
	return retNum;
}


// �e�[�u���Ɠ���MESHCODE�̃��R�[�h���t�B�[�`���N���X����Ђ��ς��Ă���
MESHCODEMAP CreateMapFromQuery( IFeatureClass* ipFeatClass, const MESHCODEMAP& mapTmpCityMesh )
{
	std::map< long, CAdapt< _IRowPtr > > retMap;

	// IN���1000���������邽�߂̉�����
	std::list<CString> strQueryList;
	gf::gf_private::CreateQuery( mapTmpCityMesh.begin(), mapTmpCityMesh.end(), strQueryList );

	IQueryFilterPtr ipQueryFilter( CLSID_QueryFilter );
	ipQueryFilter->put_SubFields( CComBSTR(_T("*")) );
	int i=0;
	for( std::list<CString>::iterator it = strQueryList.begin(); it != strQueryList.end(); it++, i++ )
	{
		// Where MESHCODE In( �c, )
		ipQueryFilter->put_WhereClause( CComBSTR(*it) );
		IFeatureCursorPtr ipFeatCursor;
		ipFeatClass->Search( ipQueryFilter, VARIANT_FALSE, &ipFeatCursor );
		if(ipFeatCursor)
		{
			IFeaturePtr ipFeature;
			long lCityMeshCodeIndex = -1;
			while( ipFeatCursor->NextFeature( &ipFeature ) == S_OK )
			{
				if( lCityMeshCodeIndex == -1 )
				{
					// ���񂾂��t�B�[���h�C���f�b�N�X������
					IFieldsPtr ipFields;
					ipFeature->get_Fields( &ipFields );
					ipFields->FindField( CComBSTR( MESHCORD_FIELDNAME ), &lCityMeshCodeIndex );
				}
				CComVariant vaValue;
				ipFeature->get_Value( lCityMeshCodeIndex, &vaValue );
				retMap[vaValue.lVal] = ipFeature;
			}
		}
	}

	return retMap;
}


int UpdateTable( const MESHCODEVECMAP& SrcMap, const std::vector<CString>& vecFdield, MESHCODEMAP& DstMap )
{
	std::map<long, long> mapIndex = gf_private::IndexMatching( vecFdield, DstMap.begin()->second.m_T );

	int nNum=0;
	int nTmpRecord=0;
	for( MESHCODEVECMAP::const_iterator itSrc = SrcMap.begin(); itSrc != SrcMap.end(); itSrc++ )
	{
		_IRowPtr ipDstRow = DstMap[itSrc->first].m_T;

		if( !ipDstRow )
		{
			CString strErr;
			strErr.Format( _T("%d�F���݂��Ȃ����b�V���R�[�h�ł��B\r\n"), itSrc->first );
			g_cOutput.OutputMsg( strErr );
			continue;
		}

		std::map<long, CComVariant> mapUpdateVal;
		bool bIsUpdate = false;
		for( std::map<long, long>::iterator it = mapIndex.begin(); it != mapIndex.end(); it++)
		{
			CComVariant vaSrcVal, vaDstVal;

			if( itSrc->second[it->first].IsEmpty() )
				vaSrcVal.ChangeType( VT_NULL );
			else
				vaSrcVal = itSrc->second[it->first];

			ipDstRow->get_Value( mapIndex[it->first], &vaDstVal );

			CString strSrcField = vecFdield[it->first];

			if( vaDstVal.vt != VT_NULL )
				vaSrcVal.ChangeType( vaDstVal.vt );

			// �l���Ⴄ���͍̂X�V�Ώ�
			if( vaSrcVal != vaDstVal)
			{
				bIsUpdate = true;
				mapUpdateVal[ mapIndex[it->first] ] = vaSrcVal;
			}
		}

		// �S�t�B�[���h�l���������������͍X�V���Ȃ�
		if(bIsUpdate)
		{
			for( std::map<long, CComVariant>::iterator itUpdate = mapUpdateVal.begin(); itUpdate != mapUpdateVal.end(); itUpdate++ )
			{
				ipDstRow->put_Value( itUpdate->first, itUpdate->second );
			}
			if( SUCCEEDED( ipDstRow->Store() ) )
				nNum++;
		}

		nTmpRecord++;
	}

	CString strMsg;
	strMsg.Format( _T("%d [Update]"), nNum );
	g_cOutput.OutputMsg( strMsg );

	return nNum;
}

int UpdateTable( const MESHCODEMAP& SrcMap, MESHCODEMAP& DstMap )
{
	if( SrcMap.empty() || DstMap.empty() )
		return 0;

	// �e�[�u���ƃt�B�[�`���N���X�̃t�B�[���h�C���f�b�N�X�̑Ή��\�����
	std::map<long, long> mapIndex = gf_private::IndexMatching( SrcMap.begin()->second.m_T, DstMap.begin()->second.m_T );

	int nNum=0;

	// �ҏW�J�n
	//IWorkspaceEditPtr ipWorkspace( gf_private::GetWorkspaceFromRow( DstMap.begin()->second.m_T ) );
	//ipWorkspace->StartEditing( VARIANT_TRUE );
	//ipWorkspace->StartEditOperation();

	for( MESHCODEMAP::const_iterator itSrc = SrcMap.begin(); itSrc != SrcMap.end(); itSrc++ )
	{
		_IRowPtr ipSrcRow = itSrc->second.m_T; //mapTmpCityMesh[itSrc->first].m_T;	// �R�s�[��
		_IRowPtr ipDstRow = DstMap[itSrc->first].m_T;					// �R�s�[��

		std::map<long, CComVariant> mapUpdateVal;
		bool bIsUpdate = false;
		for( std::map<long, long>::iterator it = mapIndex.begin(); it != mapIndex.end(); it++)
		{
			CComVariant vaSrcVal, vaDstVal;
			ipSrcRow->get_Value( it->first, &vaSrcVal );
			ipDstRow->get_Value( mapIndex[it->first], &vaDstVal );

			// �l���Ⴄ���͍̂X�V�Ώ�
			if( vaSrcVal != vaDstVal)
			{
				bIsUpdate = true;
				mapUpdateVal[ mapIndex[it->first] ] = vaSrcVal;
			}
		}

		// �S�t�B�[���h�l���������������͍X�V���Ȃ�
		if(bIsUpdate)
		{
			for( std::map<long, CComVariant>::iterator itUpdate = mapUpdateVal.begin(); itUpdate != mapUpdateVal.end(); itUpdate++ )
			{
				ipDstRow->put_Value( itUpdate->first, itUpdate->second );

			}
			if( SUCCEEDED( ipDstRow->Store() ) )
				nNum++;
		}
	}
	//ipWorkspace->StopEditOperation();
	//ipWorkspace->StopEditing( VARIANT_TRUE );

	CString strMsg;
	strMsg.Format( _T("%d [Update]"), nNum );
	g_cOutput.OutputMsg( strMsg );

	return nNum;
}

} // namespace gf
