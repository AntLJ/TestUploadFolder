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

		// テーブルから全行全フィールド拾ってくるカーソル
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


		// MESHCODEで引くクエリーを作る
		void CreateQuery( MESHCODEMAP::const_iterator it, MESHCODEMAP::const_iterator& itEnd, std::list<CString>& lstStrQuery )
		{
			CString strQuery = _T("MESHCODE IN (");
			for( int i=0; it != itEnd; it++, i++)
			{
				CString strTmp;
				strTmp.Format( _T("%ld,"), it->first );
				strQuery += strTmp;
				// 1000個書いたら抜ける
				if( i==100 )
					break;
			}
			// 最後の , を消して体裁を整える
			strQuery=strQuery.TrimRight( _T(",") ) + _T(")");
			lstStrQuery.push_back( strQuery );

			// マップを全走査するまで再帰
			if( it != itEnd )
				CreateQuery( ++it, itEnd, lstStrQuery );
		}


		// フィールドインデックスの対応マップを作る
		std::map<long, long> IndexMatching( const std::vector<CString> &vecRowKeyField, _IRow* ipValueRow )
		{
			std::map<long, long> retMap;
			IFieldsPtr ipFields;
			ipValueRow->get_Fields(&ipFields);
			if(ipFields)
			{
				for( int i=0; i < vecRowKeyField.size(); ++i )
				{
					// メッシュコードとOIDフィールドは更新対象外
					if( vecRowKeyField[i].CompareNoCase( _T("OBJECTID") ) != 0 && vecRowKeyField[i].CompareNoCase( MESHCORD_FIELDNAME ) != 0 )
					{
						long lValueIndex = -1;
						ipFields->FindField( CComBSTR(vecRowKeyField[i]), &lValueIndex );
						if(lValueIndex > -1)
							retMap[i] = lValueIndex;
					}
				}
			}

			CString strMsg = _T("\r\n比較対象フィールド：");
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
					// キーになるほうのフィールド数でまわす
					for( long i=0; i<lFields; i++)
					{
						IFieldPtr ipKeyField;
						if( SUCCEEDED( ipKeyFields->get_Field( i, &ipKeyField ) ) )
						{
							CComBSTR bstrFieldName;
							ipKeyField->get_Name( &bstrFieldName );
							// OBJECTID と MESHCODEフィールドは更新対象外
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

			// 赤びっくり回避(bug 1765)
			// 従来のtry...catchでは引っかからないので、構造化例外を使用
			__try {
				return ipDataset->get_Workspace( ppWork );
			}
			__except( exception_filter( GetExceptionCode(), GetExceptionInformation() ) ) {
				ATLTRACE(_T("例外 : ExceptionCode[%X]\r\n"), GetExceptionCode() );
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
		// 既に同名バージョンがあったら消す
		IVersionPtr ipOldVer;
		if( SUCCEEDED( (( IVersionedWorkspacePtr )ipWorkspace)->FindVersion( CComBSTR( strVersionName ), &ipOldVer ) ) )
		{
			CComBSTR bstrOldName;
			ipOldVer->get_VersionName( &bstrOldName );
			if( SUCCEEDED( ipOldVer->Delete() ) ){
				CString strMsg;
				strMsg.Format( _T("既存のバージョン%sを消しました。\r\n"), bstrOldName);
				g_cOutput.OutputMsg( strMsg );
			}
		}

		// バージョン新規作成
		if( SUCCEEDED( ((IVersionPtr)ipWorkspace)->CreateVersion( CComBSTR(strVersionName), &ipNewVersion ) ) )
		{
			CComBSTR bstrNewVerName;
			if( SUCCEEDED( ipNewVersion->get_VersionName( &bstrNewVerName ) ) ){
				CString strMsg;
				strMsg.Format( _T("バージョン%sを作成しました。\r\n"), bstrNewVerName);
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
		strMsg.Format( _T("接続に失敗しました。 %s@%s(%s)\r\n"), strUsername, strServername, strVersionName );
		g_cOutput.OutputMsg( strMsg );
	}

	return ipRetWorkSpace;
}

bool DiffMapBetweenVersion( IWorkspacePtr ipCurrentWorkspace, LPCTSTR strTableName, MESHCODEMAP& DiffMap )
{
	g_cOutput.OutputMsg( _T("CITYMESHテーブルのSDE.DEFAULTと更新元バージョンの差分を取得...") );

	MESHCODEMAP RetMap;

	// デフォルトバージョンを取得
	IVersionPtr ipDefaultVer;
	int nDiffRecords = 0;
	if( SUCCEEDED( ((IVersionedWorkspacePtr)ipCurrentWorkspace)->get_DefaultVersion( &ipDefaultVer ) ) )
	{
		// デフォルトと現在のバージョンからそれぞれテーブルを取得
		IFeatureClassPtr ipDefaultbl, ipDstTable;
		if( SUCCEEDED( ((IFeatureWorkspacePtr)((IFeatureWorkspacePtr)ipDefaultVer))->OpenFeatureClass( CComBSTR( strTableName ), &ipDefaultbl ) ) &&
			SUCCEEDED( ((IFeatureWorkspacePtr)ipCurrentWorkspace)->OpenFeatureClass( CComBSTR( strTableName ), &ipDstTable ) ) )
		{
			// 現在のバージョンとデフォルトバージョンの差分を取る
			IDifferenceCursorPtr ipCursor;
			HRESULT hr = ((IVersionedTablePtr)ipDstTable)->Differences( (ITablePtr)ipDefaultbl, esriDifferenceTypeUpdateNoChange, NULL, &ipCursor );
			if( SUCCEEDED( hr ) || FDO_E_SE_STATES_ARE_SAME == hr )
			{
				// 差分がない時もOK
				DiffMap = gf_private::Cursor2Map( ipCursor, MESHCORD_FIELDNAME );

				CString strMsg;
				strMsg.Format( _T("%d [Records] (a)\r\n"), DiffMap.size() );
				g_cOutput.OutputMsg( strMsg );
				return true;
			}else{
				g_cOutput.OutputMsg( _T("\r\nデフォルトバージョンとの差分取得に失敗しました。\r\n") ); }
		}else{
			g_cOutput.OutputMsg( _T("\r\nデフォルトまたは更新元バージョンのテーブル取得に失敗しました。\r\n") ); }
	}else{
		g_cOutput.OutputMsg(_T("\r\nデフォルトバージョンの取得に失敗しました。\r\n")); }

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
	g_cOutput.OutputMsg( _T("(a)からTMP_CITYMESHとの重複レコードを削除...") );

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


// テーブルと同じMESHCODEのレコードをフィーチャクラスからひっぱってくる
MESHCODEMAP CreateMapFromQuery( IFeatureClass* ipFeatClass, const MESHCODEMAP& mapTmpCityMesh )
{
	std::map< long, CAdapt< _IRowPtr > > retMap;

	// IN句で1000件ずつ投げるための下準備
	std::list<CString> strQueryList;
	gf::gf_private::CreateQuery( mapTmpCityMesh.begin(), mapTmpCityMesh.end(), strQueryList );

	IQueryFilterPtr ipQueryFilter( CLSID_QueryFilter );
	ipQueryFilter->put_SubFields( CComBSTR(_T("*")) );
	int i=0;
	for( std::list<CString>::iterator it = strQueryList.begin(); it != strQueryList.end(); it++, i++ )
	{
		// Where MESHCODE In( …, )
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
					// 初回だけフィールドインデックスを引く
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
			strErr.Format( _T("%d：存在しないメッシュコードです。\r\n"), itSrc->first );
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

			// 値が違うものは更新対象
			if( vaSrcVal != vaDstVal)
			{
				bIsUpdate = true;
				mapUpdateVal[ mapIndex[it->first] ] = vaSrcVal;
			}
		}

		// 全フィールド値が同じだった時は更新しない
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

	// テーブルとフィーチャクラスのフィールドインデックスの対応表を作る
	std::map<long, long> mapIndex = gf_private::IndexMatching( SrcMap.begin()->second.m_T, DstMap.begin()->second.m_T );

	int nNum=0;

	// 編集開始
	//IWorkspaceEditPtr ipWorkspace( gf_private::GetWorkspaceFromRow( DstMap.begin()->second.m_T ) );
	//ipWorkspace->StartEditing( VARIANT_TRUE );
	//ipWorkspace->StartEditOperation();

	for( MESHCODEMAP::const_iterator itSrc = SrcMap.begin(); itSrc != SrcMap.end(); itSrc++ )
	{
		_IRowPtr ipSrcRow = itSrc->second.m_T; //mapTmpCityMesh[itSrc->first].m_T;	// コピー元
		_IRowPtr ipDstRow = DstMap[itSrc->first].m_T;					// コピー先

		std::map<long, CComVariant> mapUpdateVal;
		bool bIsUpdate = false;
		for( std::map<long, long>::iterator it = mapIndex.begin(); it != mapIndex.end(); it++)
		{
			CComVariant vaSrcVal, vaDstVal;
			ipSrcRow->get_Value( it->first, &vaSrcVal );
			ipDstRow->get_Value( mapIndex[it->first], &vaDstVal );

			// 値が違うものは更新対象
			if( vaSrcVal != vaDstVal)
			{
				bIsUpdate = true;
				mapUpdateVal[ mapIndex[it->first] ] = vaSrcVal;
			}
		}

		// 全フィールド値が同じだった時は更新しない
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
