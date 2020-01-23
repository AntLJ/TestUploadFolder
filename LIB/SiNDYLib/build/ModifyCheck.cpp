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

/**
 * @file ModifyCheck.cpp
 */
#include "stdafx.h"
#include "ModifyCheck.h"
#include "Row.h"
#include "Workspace.h"
#include "GlobalFunctions.h"
#include "RuleCommunity.h"
#include <sdeerno.h>
#include "sde_ondemand.h"
#include "../../LIB/ArcMapUIBase/esriHidingInterface.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace {
#ifndef SINDY_FOR_ENGLISH
	LPCTSTR SDE_CONNECT_FAILED     = _T("SDEへの接続が失敗しました");
	LPCTSTR EXIST_WHEN_MIGRATING   = _T("移行時既存");
	LPCTSTR CREATING_NEW           = _T("新規作成");
	LPCTSTR DELETE_IT              = _T("削除");
	LPCTSTR SHAPE_CHANGE           = _T("形状変更");
	LPCTSTR ATTRIBUTE_MODIFICATION = _T("属性変更");
	LPCTSTR SHANGHAI_DATA_IMPORT   = _T("上海データインポート");
	LPCTSTR DATA_IMPORT            = _T("データインポート");
#else
	LPCTSTR SDE_CONNECT_FAILED     = _T("Failed to connect to SDE");
	LPCTSTR EXIST_WHEN_MIGRATING   = _T("Exist when migrating");
	LPCTSTR CREATING_NEW           = _T("Create new");
	LPCTSTR DELETE_IT              = _T("Delete");
	LPCTSTR SHAPE_CHANGE           = _T("Shape change");
	LPCTSTR ATTRIBUTE_MODIFICATION = _T("Attribute modification");
	LPCTSTR SHANGHAI_DATA_IMPORT   = _T("Shanghai data import");
	LPCTSTR DATA_IMPORT            = _T("Data import");
#endif // SINDY_FOR_ENGLISH
}


using namespace winutil;
//#define _DEBUG_FOR_ESRIJ
namespace sindy {
#ifdef _MODIFYCHECK_CREATE_NEW_CONNECTION
std::map<CString, SE_CONNECTION>	CModifyCheck::g_mapConnection;
#endif // if _MODIFYCHECK_CREATE_NEW_CONNECTION
std::map<CString, CString>			CModifyCheck::g_mapTabls;

#ifdef _TEST_NEW_MODIFYCHECK
TCHAR g_strModifyCheckProcedure[48] = _T("ModifyCheck");	//!< ModifyCheckプロシージャ名
#endif // ifdef _TEST_NEW_MODIFYCHECK

CModifyCheck::CModifyInfo::CModifyInfo()
{
	m_lOID = m_lPurposeID = m_lStateID = m_lUpdateTypeID = -1;
	m_bIsLocal = m_bDeleted = m_bUsefulTime = m_bIsModified = false;
}

CModifyCheck::CModifyInfo::~CModifyInfo()
{
}

CModifyCheck::CModifyInfo& CModifyCheck::CModifyInfo::operator=( const CModifyInfo& cInfo )
{
	m_bIsModified = cInfo.m_bIsModified;

	m_lOID = cInfo.m_lOID;
	m_lPurposeID = cInfo.m_lPurposeID;
	m_lStateID = cInfo.m_lStateID;
	m_lUpdateTypeID = cInfo.m_lUpdateTypeID;
	m_bIsLocal = cInfo.m_bIsLocal;
	m_bDeleted = cInfo.m_bDeleted;
	m_bUsefulTime = cInfo.m_bUsefulTime;
	SetOperator( cInfo.GetOperator() );
	SetTime( cInfo.GetTime() );
	SetTableName( cInfo.GetTableName() );
	SetUpdateType( cInfo.GetUpdateType() );
	SetPurpose( cInfo.GetPurpose() );

	return *this;
}

void CModifyCheck::CModifyInfo::SetOperator( LPCTSTR lpcszName ){ m_strOperator = lpcszName; }
void CModifyCheck::CModifyInfo::SetTime( LPCTSTR lpcszTime ){ m_strTime = lpcszTime; }
void CModifyCheck::CModifyInfo::SetTableName( LPCTSTR lpcszName ){ m_strTableName = lpcszName; }
void CModifyCheck::CModifyInfo::SetUpdateType( LPCTSTR lpcszType ){ m_strUpdateType = lpcszType; }
void CModifyCheck::CModifyInfo::SetPurpose( LPCTSTR lpcszPurpose ){ m_strPurpose = lpcszPurpose; }
void CModifyCheck::CModifyInfo::SetTime( LPSYSTEMTIME lpsysTime )
{
	m_strTime = ::GetTime( lpsysTime );
}
void CModifyCheck::CModifyInfo::SetTime( const tm& tmTime )
{
	m_strTime = _tasctime( &tmTime );
	m_strTime = m_strTime.Left( m_strTime.GetLength() - 1 ); // asctimeは改行を入れてくれる…
}

CModifyCheck::CModifyCheck()
{
	m_bUseProcedure = true;
	m_bUncheck = false;
	m_bExistProcedure = true;	// 一度プロシージャがあるかどうかチェックするのでまずはtrue

	CRegKey cRegKey;
	if( ERROR_SUCCESS == cRegKey.Open( HKEY_CURRENT_USER, _T("Software\\INCREMENT P CORPORATION\\SiNDY-e\\Tools\\Common") ) )
	{
		DWORD dwValue;

		// プロシージャを使用するかどうかをレジストリから読み込む
		if( ERROR_SUCCESS == cRegKey.QueryValue( dwValue, _T("UseProcedure") ) )
			m_bUseProcedure = ( dwValue == 1 ) ? true : false;		

		// ModifyCheckを使用すかどうかをレジストリから読み込む
		dwValue = 0;
		if( ERROR_SUCCESS == cRegKey.QueryValue( dwValue, _T("UnuseModifyCheck") ) )
			m_bUncheck = ( dwValue == 1 ) ? true : false;
	}
}

CModifyCheck::~CModifyCheck()
{
}

bool CModifyCheck::IsModified( const CRow& cRow, CModifyInfo& cInfo )
{
#ifdef _DEBUG_FOR_ESRIJ
	TRACEMESSAGE(_T("%s : プロシージャ：%s ModifyCheck：%s\n"), __func__, m_bUseProcedure ? _T("使用する") : _T("使用しない"), m_bUncheck ? _T("実行しない") : _T("実行する") );
#endif // ifdef _DEBUG_FOR_ESRIJ

	bool bRet = true;	// デフォルトは「既に編集されている」とする

	if( ! m_bUncheck )
	{
#ifdef _DEBUG
		bool bChecked = false;
		DWORD DWSTART = GetTickCount();
#endif
		USES_CONVERSION;

		// OBJECTID取得
		long lOID = cRow.GetOID();

#ifdef _DEBUG_FOR_ESRIJ
		TRACEMESSAGE(_T("%s : チェック対象OID : %d\n"), __func__, lOID );
#endif // ifdef _DEBUG_FOR_ESRIJ

		// [bug 3258] バージョン非対応の場合はスルーする
		IDatasetPtr ipDataset = cRow.GetTable();
		if( NULL != ipDataset )
		{
			IWorkspacePtr ipWorkspace;
			ipDataset->get_Workspace( &ipWorkspace );
			IVersionedWorkspacePtr ipVersionedWorkspace = ipWorkspace;
			if( NULL != ipVersionedWorkspace )
			{
#ifdef _DEBUG
				bChecked = true;
#endif // ifdef _DEBUG
				// レジストリのキーが有効で、かつプロシージャがあるときにのみプロシージャで ModifyCheck
				// プロシージャの有無を実際にコールすることで確認するため、初回は m_bExistProcedure = TRUE
MODIFYCHECK:
				if ( m_bUseProcedure && m_bExistProcedure )
				{
#ifdef _DEBUG_FOR_ESRIJ
					TRACEMESSAGE(_T("%s : プロシージャ：%s\n"), __func__, m_bExistProcedure ? _T("存在すると仮定") : _T("存在しない") );
#endif // ifdef _DEBUG_FOR_ESRIJ

					// Modify Check を PL/SQL で実行する場合
					CTableNameString& cNames( *cRow.GetNameString().get() );
					LPCTSTR strLayerName = cNames.GetTableName();
					LPCTSTR strOwnerName = cNames.GetOwnerName();

					CComBSTR sVersion;
					IVersionPtr ipVersion(ipVersionedWorkspace);
				
					if ( ipVersion ) {
						ipVersion->get_VersionName(&sVersion);
						CString sVer(sVersion);

						int nPos = sVer.Find(_T("."));

						if ( nPos != -1 ) {
							sVer = sVer.Mid(nPos + 1);
						}

						CString sSql;
#ifdef _TEST_NEW_MODIFYCHECK
						sSql.Format(_T("BEGIN %s.%s(%d, '%s', '%s', '%s'); END;"), strOwnerName, g_strModifyCheckProcedure, lOID, strOwnerName, strLayerName, sVer);
#ifdef _DEBUG_FOR_ESRIJ
						TRACEMESSAGE(_T("%s : SQL : %s\n"), __func__, sSql );
#endif // ifdef _DEBUG_FOR_ESRIJ
#else
						sSql.Format(_T("BEGIN %s.%s(%d, '%s', '%s', '%s'); END;"), strOwnerName, _T("ModifyCheck"), lOID, strOwnerName, strLayerName, sVer);
#endif // ifdef _TEST_NEW_MODIFYCHECK

/*#ifdef _DEBUG_FOR_ESRIJ
						MessageBox( GetActiveWindow(), sSql, _T("プロシージャコール"), MB_OK|MB_ICONINFORMATION );
#endif // ifdef _DEBUG_FOR_ESRIJ*/

						HRESULT hr = ipWorkspace->ExecuteSQL(CComBSTR(sSql));

						if ( SUCCEEDED(hr)) {
#ifdef _DEBUG_FOR_ESRIJ
							TRACEMESSAGE(_T("%s : ipWorkspace->ExecuteSQL() : 返り値 : %ld\n"), __func__, hr );
#endif // ifdef _DEBUG_FOR_ESRIJ
							bRet = false;
						} else {
#ifdef _DEBUG_FOR_ESRIJ
							TRACEMESSAGE(_T("%s : ipWorkspace->ExecuteSQL() : 返り値 : %ld\n"), __func__, hr );
#endif // ifdef _DEBUG_FOR_ESRIJ
							/// 衝突が検知された
							cInfo.m_bIsModified = true;

							IFeatureWorkspacePtr ipFeatureWorkspace(ipVersionedWorkspace);

							ITablePtr ipTable;
							ipFeatureWorkspace->OpenTable(CComBSTR(_T("REFERENCE.MODIFY_RESULT")), &ipTable);
#ifdef _DEBUG_FOR_ESRIJ
							TRACEMESSAGE(_T("%s : 結果の取得テーブル : %s : OPEN[%s]\n"), __func__, _T("REFERENCE.MODIFY_RESULT"), ( NULL != ipTable ) ? _T("成功") : _T("失敗") );
#endif // ifdef _DEBUG_FOR_ESRIJ

							if ( ipTable ) {
								IQueryFilterPtr pIQueryFilter( CLSID_QueryFilter );
								_ICursorPtr pICursor;

								ipTable->Search( pIQueryFilter, VARIANT_FALSE, &pICursor );

								if ( pICursor ) {
									_IRowPtr pIRowTmp;
									CComVariant vOID, vOperator, vDate, vPurpose, vUpdateType, vTableID, vState;
								
									while( pICursor->NextRow( &pIRowTmp ) == S_OK ) {
										if ( pIRowTmp) {
											CString sMsg, sTable;

											pIRowTmp->get_Value(0, &vOID);				/// OID
											vOID.ChangeType(VT_I4);
											pIRowTmp->get_Value(5, &vTableID);			/// Table ID
											sTable.Format(_T("%d\n"), vTableID.lVal);

											pIRowTmp->get_Value(6, &vState);			/// State
											vState.ChangeType(VT_I4);

											pIRowTmp->get_Value(1, &vOperator);		/// Operator
											
											if ( VT_NULL == vOperator.vt )
											{
												/// 削除された場合
												cInfo.m_lOID = vOID.lVal;
												cInfo.SetTableName( sTable );
												cInfo.m_lStateID = vState.lVal;
												cInfo.m_bIsLocal = false;
												cInfo.m_bDeleted = true;
											} 
											else {
												pIRowTmp->get_Value(2, &vDate);			/// Date
												pIRowTmp->get_Value(3, &vPurpose);			/// Purpose
												vPurpose.ChangeType(VT_I4);
												pIRowTmp->get_Value(4, &vUpdateType);		/// UpdateType
												vUpdateType.ChangeType(VT_I4);


												CString sTime, sUpdateType;
												CComBSTR sPurpose;
												SYSTEMTIME st;
					
												VariantTimeToSystemTime(vDate.dblVal, &st);
				
												sTime.Format( _T( "%04d/%02d/%02d %02d:%02d:%02d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
//													m_ipRule->GetOperatingPurposeFromID(vPurpose.lVal, &sPurpose);

												switch ( vUpdateType.lVal ) {
													case 0:	sUpdateType = EXIST_WHEN_MIGRATING;		break;
													case 1:	sUpdateType = CREATING_NEW;				break;
													case 2:	sUpdateType = DELETE_IT;				break;
													case 3:	sUpdateType = SHAPE_CHANGE;				break;
													case 4:	sUpdateType = ATTRIBUTE_MODIFICATION;	break;
													case 5: sUpdateType = SHANGHAI_DATA_IMPORT;		break;
													case 6: sUpdateType = DATA_IMPORT;				break;
												}

												cInfo.m_lOID = vOID.lVal;
												cInfo.m_lStateID = vState.lVal;
												cInfo.m_lPurposeID = vPurpose.lVal;
												cInfo.SetOperator( OLE2T(vOperator.bstrVal) );
												cInfo.SetTime( (LPSYSTEMTIME)&st );
												cInfo.SetUpdateType( sUpdateType );
												cInfo.SetTableName( sTable );
												cInfo.m_bDeleted = false;
												cInfo.m_bIsLocal = false;
												//sMsg.Format(_T("[SERVER] オブジェクト %d は %s によって %s に『%s』の作業目的で %s しています。TABLE[A%s] STATE[%d]\n"), vOID.lVal, CString(CComBSTR(vOperator.bstrVal)), sTime, sPurpose, sUpdateType, sTable, vState.lVal);
											}

											// 追加されたデータは削除しておくこと
											if( SUCCEEDED( ipWorkspace->ExecuteSQL(CComBSTR(_T("DELETE FROM REFERENCE.MODIFY_RESULT"))) ) )
											{
#ifdef _DEBUG_FOR_ESRIJ
												TRACEMESSAGE(_T("%s : ipWorkspace->ExecuteSQL(CComBSTR(_T(\"DELETE FROM REFERENCE.MODIFY_RESULT\"))) : 成功\n"), __func__ );
#endif // ifdef _DEBUG_FOR_ESRIJ
											}
											else {
#ifdef _DEBUG_FOR_ESRIJ
												TRACEMESSAGE(_T("%s : ipWorkspace->ExecuteSQL(CComBSTR(_T(\"DELETE FROM REFERENCE.MODIFY_RESULT\"))) : 成功\n"), __func__ );
#endif // ifdef _DEBUG_FOR_ESRIJ
												_ASSERTE(false);
											}
											//*sMessage = T2BSTR(sMsg);
											//*bModified = VARIANT_TRUE;

											//return true;
										}
									}
#ifdef _DEBUG_FOR_ESRIJ
									TRACEMESSAGE(_T("%s : 取得できたステートID : %ld（-1の場合は、プロシージャ内部でエラー、又はプロシージャが存在しない）\n"), __func__, cInfo.m_lStateID );
#endif // ifdef _DEBUG_FOR_ESRIJ
								}
							} else {

								m_bExistProcedure = FALSE;

								// 再度ルーチンを正しく通すためにやむを得ず goto を使用する
								// 下で前のように条件を書いても良いが、上の条件が変更になったときに
								// 下の条件を変更し忘れて ModifyCheck がスルーしないようにするため
								goto MODIFYCHECK;

//									*sMessage = T2BSTR(_T("REFERENCE.MODIFY_RESULT が見つかりません。プロシージャが正しくインストールされているか確認してください。"));
//									*bModified = VARIANT_TRUE;
//	
//									return S_FALSE;		
							}
						}
					}
				}
				else {
#ifdef _MODIFYCHECK_CREATE_NEW_CONNECTION
					// 接続（既存のものがある場合はそれを探す）
					CString strVersion, strDatasetName, strConnStr;
					if( CreateConnection( cRow, strVersion, strDatasetName, strConnStr ) == FALSE )
					{
						_ASSERTE( FALSE );
						//return S_FALSE;
					}

					// 該当 OBJECT_ID を探す
					CString sMes;
					std::map<CString, SE_CONNECTION>::iterator it = g_mapConnection.find(T2CA(strConnStr));

					if ( it != g_mapConnection.end() )
					{
						bRet = IsModifiedByLocal( it->second, strDatasetName, strVersion, lOID, cInfo );
					}
					else {
						_ASSERTE( it != g_mapConnection.end() );
						//return false;
					}
#else
					// 既存コネクションを取得
					SE_CONNECTION conn = NULL;
					ISdeWorkspaceImplPtr ipSdeWorkspace( cRow.GetWorkspace() );
					if( NULL != ipSdeWorkspace )
					{
						ipSdeWorkspace->get_ConnectionHandle( (long*)&conn );
						// テーブルのリストを取得
						GetTableList(conn, CComVariant(cRow.GetNameString()->GetOwnerName()));
					}
					bRet = IsModifiedByLocal( conn, cRow.GetNameString()->GetOwnerTableName(), cRow.GetNameString()->GetVersionName(), cRow.GetOID(), cInfo );
#endif // ifdef _MODIFYCHECK_CREATE_NEW_CONNECTION
				}
			}
			else
				bRet = false;	// パーソナル・ジオデータベースやシェープファイルの場合
		}
#ifdef _DEBUG
		if( bChecked )
			ATLTRACE(_T("CModifyCheck::IsModified() : %.4lf\n"), ( (DOUBLE)GetTickCount() - (DOUBLE)DWSTART )/(DOUBLE)1000.0 );
#endif
	}
	else
		bRet = false;

	return bRet;
}

// 変更チェック対象のオブジェクトのID
bool CModifyCheck::IsModifiedByLocal( SE_CONNECTION conn, LPCTSTR lpcszTable, LPCTSTR lpcszVersion, long lOID, CModifyInfo& cInfo )
{
	USES_CONVERSION;

	// lpcszVersion を バージョン名とオーナーに分ける
	CString strOwner, strVer;
	std::list<CString> listVersion;
	AheSepStr2listString( lpcszVersion, _T("."), listVersion );
	if( listVersion.size() == 2 )
	{
		std::list<CString>::iterator itVer = listVersion.begin();
		strOwner = *itVer;
		strVer = *(++itVer);
	}
	else
		_ASSERTE( listVersion.size() == 2 );

	// 自分の編集系統を取得
	m_strEditStateChain = GetEditStateChain( conn, strVer );

	// テーブルをキャッシュから検索
	std::map<CString, CString>::iterator it = g_mapTabls.find(lpcszTable);

	if ( it != g_mapTabls.end() )
	{
		// 自分の編集系統を除外して、Aテーブルのステートを取得
		std::list<CModifyInfo> listAdds;
		CString strIDs = GetAStates( conn, it->second, lOID, listAdds, m_strEditStateChain );

		if( ! strIDs.IsEmpty() )
		{
			// 取得したステートが生きているかどうかをチェック
			INT nActiveType = -1;
			LONG lActiveState = IsActiveStates( conn, strVer, strIDs, nActiveType );
			if( lActiveState >= 0 )
			{
				for( std::list<CModifyInfo>::const_iterator itAdds = listAdds.begin(); itAdds != listAdds.end(); itAdds++ ) 
				{
					if( itAdds->m_lStateID == lActiveState )
					{
						CString strDate;
						if( itAdds->m_bUsefulTime == TRUE )
						{
							strDate = itAdds->GetTime();
						}
						else
							strDate = _T("NULLTIME");

						cInfo.m_bIsModified = true;
						//sMessage.Format(_T("[CLIENT] オブジェクト %d は、%s が %s に『%s』の作業目的で %s しています。TABLE[A%s] STATE[%d]"), lOID, itAdds->Operator, strDate, itAdds->Purpose, itAdds->UpdateType, it->second, lActiveState );
						cInfo = *itAdds;
						cInfo.m_lOID = lOID;
						cInfo.SetTime( strDate );
						cInfo.SetTableName( it->second );
						cInfo.m_lStateID = lActiveState;
						cInfo.m_bDeleted = false;
						cInfo.m_bIsLocal = true;

						return true;
					}
				}
			}
		}

		// 自分の編集系統を除外して、Aテーブルのステートを取得
		CString strDIDs = GetDStates( conn, it->second, lOID, m_strEditStateChain );

		if( ! strDIDs.IsEmpty() )
		{
			// 取得したステートが生きているかどうかをチェック
			INT nActiveType = -1;
			LONG lActiveState = IsActiveStates( conn, strVer, strDIDs, nActiveType );
			if( lActiveState >= 0 )
			{
				cInfo.m_bIsModified = true;
				//sMessage.Format(_T("[CLIENT] オブジェクト %d は削除されました。TABLE[D%s] STATE[%d]"), lOID, it->second, lActiveState );
				cInfo.m_lOID = lOID;
				cInfo.SetTableName( it->second );
				cInfo.m_lStateID = lActiveState;
				cInfo.m_bDeleted = true;
				cInfo.m_bIsLocal = true;

				return true;
			}
		}
	}
	
	return false;
}

/** 
 * @brief ステートが生きているかどうか
 *
 * @param conn	[in]	セッション
 * @param lpcszVer		[in]	編集中のバージョン名
 * @param lpcszIDs		[in]	検査対象のステート文字列（カンマ区切り）
 * @param nActiveType	[out]	-1:Activeではない 0:編集中又はポスト後のステート 1:ローカルバージョンのみに保存されているステート
 *
 * @retval -1 ステートは生きてない
 * @retval -1以外 ステートは生きている（ステート番号）
 */
LONG CModifyCheck::IsActiveStates( SE_CONNECTION conn, LPCTSTR lpcszVer, LPCTSTR lpcszIDs, INT& nActiveType )
{
	USES_CONVERSION;

	nActiveType = -1;	// 初期化
	if( lpcszVer != NULL && lpcszIDs != NULL )
	{
//		CComBSTR bstrOperator;
//		CString strOperator;
//		m_ipRule->get_OperatorName( &bstrOperator );
//		strOperator = bstrOperator;

		// クエリ作成
		CString strSQL;
		// strIDsがSTATE_LOCKSに一つでも含まれていた場合、編集中のステートなので編集不可
		// strIDsが現在編集中のバージョンの親バージョンのステートまでの系統に含まれているのであれば、保存後のステートなので、編集不可
		strSQL.Format(_T("SELECT STATE_ID FROM SDE.STATE_LOCKS WHERE STATE_ID IN (%s) UNION \
						  SELECT STATE_ID FROM SDE.STATES WHERE STATE_ID IN (%s) AND STATE_ID IN ( \
							SELECT STATE_ID FROM SDE.STATES START WITH STATE_ID = ( \
								SELECT STATE_ID FROM SDE.VERSIONS WHERE NAME = ( \
									SELECT PARENT_NAME FROM SDE.VERSIONS WHERE NAME = '%s') ) \
							CONNECT BY STATE_ID = PRIOR PARENT_STATE_ID AND NOT STATE_ID = 0 )"), lpcszIDs, lpcszIDs, lpcszVer );
		LONG lStateID = -1;
		// クエリオブジェクトの作成
		LONG errcode = SE_SUCCESS;
		SE_STREAM stream = NULL;
		if ( SE_SUCCESS == ( errcode = SSE_stream_create( conn, &stream ) ) )
		{
			if( SE_SUCCESS == ( errcode = SSE_stream_prepare_sql( stream, T2CA(strSQL) ) ) )
			{
				if( SE_SUCCESS == ( errcode = SSE_stream_execute( stream ) ) )
				{
					while( SE_FINISHED != ( errcode = SSE_stream_fetch( stream ) ) )
					{
						if( SE_SUCCESS != errcode )
						{
							// エラー
							_ASSERTE( SE_SUCCESS == errcode );
							break;
						}

						// なぜかDOUBLEで返ってくる？？
						// SDE API のバグと思われるが、将来修正されることを考えてLONGでも取れるようにしておく
						SE_COLUMN_DEF column_def = {0};
						if( SE_SUCCESS == ( errcode = SSE_stream_describe_column( stream, 1, &column_def ) ) )
						{
							switch( column_def.sde_type )
							{
								case SE_INTEGER_TYPE:
									if( SE_SUCCESS != ( errcode = SSE_stream_get_integer( stream, 1, &lStateID ) ) )
										_ASSERTE( SE_SUCCESS == errcode );
									break;
								case SE_DOUBLE_TYPE:
								{ 
									DOUBLE dStateID = 0.0;
									if( SE_SUCCESS != ( errcode = SSE_stream_get_double( stream, 1, &dStateID ) ) )
										_ASSERTE( SE_SUCCESS == errcode );
									lStateID = (LONG)dStateID;
									break;
								}
								case SE_FLOAT_TYPE:
								{ 
									float fStateID = 0.0;
									if( SE_SUCCESS != ( errcode = SSE_stream_get_float( stream, 1, &fStateID ) ) )
										_ASSERTE( SE_SUCCESS == errcode );
									lStateID = (LONG)fStateID;
									break;
								}
								default:
									_ASSERTE( FALSE );
									break;
							}
							break;
						}
						else
							_ASSERTE( SE_SUCCESS == errcode );
					}
				}
				else
					_ASSERTE( SE_SUCCESS == errcode );
			}
			else
				_ASSERTE( SE_SUCCESS != errcode );

			// stream開放
			if( SE_SUCCESS != ( errcode = SSE_stream_free( stream ) ) )
				_ASSERTE( SE_SUCCESS == errcode );
		}
		else
			_ASSERTE( SE_SUCCESS == errcode );

		if( lStateID > 0 )
		{
			nActiveType = 0;	// 編集中のステート及び、ポスト後のステート
			return lStateID;
		}
		else {
			// strIDsのステートが生きているかどうか
			// = そのステートから末端までの系統に生きているバージョンが存在するかどうか
			std::list<CString> listIDs;
			AheSepStr2listString( lpcszIDs, _T(","), listIDs );
			for( std::list<CString>::const_iterator itIDs = listIDs.begin(); itIDs != listIDs.end(); ++itIDs )
			{
				// クエリ作成
				strSQL.Format(_T("SELECT OBJECT_ID FROM SDE.OBJECT_LOCKS WHERE OBJECT_ID IN ( \
									SELECT VERSION_ID FROM SDE.VERSIONS WHERE STATE_ID IN ( \
										SELECT STATE_ID FROM SDE.STATES START WITH STATE_ID = %s CONNECT BY PRIOR STATE_ID = PARENT_STATE_ID))"), *itIDs );

				LONG errcode = SE_SUCCESS;
				SE_STREAM stream = NULL;
				if ( SE_SUCCESS == ( errcode = SSE_stream_create( conn, &stream ) ) )
				{
					if( SE_SUCCESS == ( errcode = SSE_stream_prepare_sql( stream, T2CA(strSQL) ) ) )
					{
						if( SE_SUCCESS == ( errcode = SSE_stream_execute( stream ) ) )
						{
							while( SE_FINISHED != ( errcode = SSE_stream_fetch( stream ) ) )
							{
								if( SE_SUCCESS != errcode )
								{
									// エラー
									_ASSERTE( SE_SUCCESS == errcode );
									break;
								}

								// なぜかDOUBLEで返ってくる？？
								// SDE API のバグと思われるが、将来修正されることを考えてLONGでも取れるようにしておく
								SE_COLUMN_DEF column_def = {0};
								if( SE_SUCCESS == ( errcode = SSE_stream_describe_column( stream, 1, &column_def ) ) )
								{
									switch( column_def.sde_type )
									{
										case SE_INTEGER_TYPE:
											if( SE_SUCCESS != ( errcode = SSE_stream_get_integer( stream, 1, &lStateID ) ) )
												_ASSERTE( SE_SUCCESS == errcode );
											break;
										case SE_DOUBLE_TYPE:
										{ 
											DOUBLE dStateID = 0.0;
											if( SE_SUCCESS != ( errcode = SSE_stream_get_double( stream, 1, &dStateID ) ) )
												_ASSERTE( SE_SUCCESS == errcode );
											lStateID = (LONG)dStateID;
											break;
										}
										case SE_FLOAT_TYPE:
										{ 
											float fStateID = 0.0;
											if( SE_SUCCESS != ( errcode = SSE_stream_get_float( stream, 1, &fStateID ) ) )
												_ASSERTE( SE_SUCCESS == errcode );
											lStateID = (LONG)fStateID;
											break;
										}
										default:
											_ASSERTE( FALSE );
											break;
									}
									lStateID = _ttol( *itIDs );
									break;
								}
								else
									_ASSERTE( SE_SUCCESS == errcode );
							}
						}
						else
							_ASSERTE( SE_SUCCESS == errcode );
					}
					else
						_ASSERTE( SE_SUCCESS == errcode );

					// stream開放
					if( SE_SUCCESS != ( errcode = SSE_stream_free( stream ) ) )
						_ASSERTE( SE_SUCCESS == errcode );
				}

				if( lStateID >= 0 ) break;
			}
			if( lStateID >= 0 )
			{
				nActiveType = 1;
				return lStateID;
			}
		}
	}
	return -1;
}

/**
 * @brief Aテーブルからフィーチャのステートを取得する
 *
 * @param conn		[in]			セッション
 * @param lpcszTableID		[in]			SDEに登録されているテーブルID
 * @param lObjectID			[in]			ステートを取得するフィーチャのOBJECTID
 * @param listAddsData		[out]			取得したステートリスト
 * @param lpcszExcludeIDs	[in,optional]	除外リスト
 *
 * @return 取得したステートのID（カンマ区切り）
 */
CString CModifyCheck::GetAStates( SE_CONNECTION conn, LPCTSTR lpcszTableID, LONG lObjectID, std::list<CModifyInfo>& listAdds, LPCTSTR lpcszExcludeIDs/* = NULL*/ )
{
	USES_CONVERSION;

	CString strIDs;
	TCHAR strID[15];
	// 引数チェック
	if( lpcszTableID != NULL && lObjectID >= 0 )
	{
		// クエリ作成
		CString strSQL;
		// 除外リストがある場合は入れる
		if( lpcszExcludeIDs != NULL )
		{
			strSQL.Format(_T("SELECT OPERATOR, PURPOSE_C, MODIFYDATE, UPDATETYPE_C, SDE_STATE_ID, PROGMODIFYDATE, MODIFYPROGNAME FROM A%s \
				WHERE OBJECTID = %d AND ( SDE_STATE_ID NOT IN (%s) )"), lpcszTableID, lObjectID, lpcszExcludeIDs );
		}
		else 
			strSQL.Format(_T("SELECT OPERATOR, PURPOSE_C, MODIFYDATE, UPDATETYPE_C, SDE_STATE_ID, PROGMODIFYDATE, MODIFYPROGNAME FROM A%s WHERE OBJECTID = %d"), lpcszTableID, lObjectID );

		// クエリオブジェクトの作成
		LONG errcode = SE_SUCCESS;
		SE_STREAM stream = NULL;
		if ( SE_SUCCESS == ( errcode = SSE_stream_create( conn, &stream ) ) )
		{
			if( SE_SUCCESS == ( errcode = SSE_stream_prepare_sql( stream, T2CA(strSQL) ) ) )
			{
				if( SE_SUCCESS == ( errcode = SSE_stream_execute( stream ) ) )
				{
					while( SE_FINISHED != ( errcode = SSE_stream_fetch( stream ) ) )
					{
						if( SE_SUCCESS != errcode )
						{
							// エラー
							_ASSERTE( SE_SUCCESS == errcode );
							break;
						}
						CModifyInfo cInfo;

						// OPERATOR（sindyの場合はMODIFYPROGNAMEを入れる）
						SE_COLUMN_DEF column_def = {0};
						if( SE_SUCCESS == ( errcode = SSE_stream_describe_column( stream, 1, &column_def ) ) ) {
							switch( column_def.sde_type )
							{
								case SE_STRING_TYPE:
									{
										CHAR strOperator[256] = "";
										if( SE_SUCCESS == ( errcode = SSE_stream_get_string( stream, 1, strOperator ) ) )
											cInfo.SetOperator( A2CT(strOperator) );
										else
											_ASSERTE( SE_SUCCESS == errcode );
									}
									break;
								case SE_NSTRING_TYPE:
									{
										WCHAR strOperator[256] = L"";
										if( SE_SUCCESS == ( errcode = SSE_stream_get_nstring( stream, 1, strOperator ) ) )
											cInfo.SetOperator( strOperator );
										else
											_ASSERTE( SE_SUCCESS == errcode );
									}
									break;
								default:
									break;
							}
						}

						if( lstrcmpi( cInfo.GetOperator(), _T("sindy") ) == 0 )
						{
							SE_COLUMN_DEF column_def = {0};
							if( SE_SUCCESS == ( errcode = SSE_stream_describe_column( stream, 7, &column_def ) ) ) {
								switch( column_def.sde_type )
								{
									case SE_STRING_TYPE:
										{
											CHAR strOperator[256] = "";
											if( SE_SUCCESS == ( errcode = SSE_stream_get_string( stream, 7, strOperator ) ) )
												cInfo.SetOperator( A2CT(strOperator) );
											else
												_ASSERTE( SE_SUCCESS == errcode );
										}
										break;
									case SE_NSTRING_TYPE:
										{
											WCHAR strOperator[256] = L"";
											if( SE_SUCCESS == ( errcode = SSE_stream_get_nstring( stream, 7, strOperator ) ) )
												cInfo.SetOperator( strOperator );
											else
												_ASSERTE( SE_SUCCESS == errcode );
										}
										break;
									default:
										break;
								}
							}
						}

						// PURPOSE_C（取得できない場合は0にしておく）
						if( SE_SUCCESS != ( errcode = SSE_stream_get_integer( stream, 2, &cInfo.m_lPurposeID ) ) ) cInfo.m_lPurposeID = 0;

/*						// 作業目的ID->作業目的名に変換
						CComBSTR bstrPurpose;
						m_ipRule->GetOperatingPurposeFromID( cInfo.m_lPurposeID, &bstrPurpose );
						stAddsData.Purpose = bstrPurpose;*/

						// MODIFYDATE
						tm tmTime;
						if( SE_SUCCESS != ( errcode = SSE_stream_get_date( stream, 3, &tmTime ) ) )
						{
							if( SE_SUCCESS != ( errcode = SSE_stream_get_date( stream, 6, &tmTime ) ) )
								cInfo.m_bUsefulTime = false;
							else {
								cInfo.m_bUsefulTime = true;
								cInfo.SetTime(tmTime);
							}
						}
						else {
							cInfo.m_bUsefulTime = true;
							cInfo.SetTime(tmTime);
						}

						// UpdateType_C（取れない場合は-1にしておく）
						if( SE_SUCCESS != ( errcode = SSE_stream_get_integer( stream, 4, &cInfo.m_lUpdateTypeID ) ) ) cInfo.m_lUpdateTypeID = -1;

						switch ( cInfo.m_lUpdateTypeID )
						{
							case 0:	cInfo.SetUpdateType( EXIST_WHEN_MIGRATING );	break;
							case 1:	cInfo.SetUpdateType( CREATING_NEW );			break;
							case 2:	cInfo.SetUpdateType( DELETE_IT );				break;
							case 3:	cInfo.SetUpdateType( SHAPE_CHANGE );			break;
							case 4:	cInfo.SetUpdateType( ATTRIBUTE_MODIFICATION );	break;
							case 5: cInfo.SetUpdateType( SHANGHAI_DATA_IMPORT );	break;
							case 6: cInfo.SetUpdateType( DATA_IMPORT );				break;
						}

						// SDE_STATE_ID
						if( SE_SUCCESS != ( errcode = SSE_stream_get_integer( stream, 5, &cInfo.m_lStateID ) ) )
							_ASSERTE( SE_SUCCESS == errcode );

						// STATE_IDの文字列を作成しておく
						_ltot( cInfo.m_lStateID, strID, 10 );
						if( strIDs.IsEmpty() )
							strIDs = strID;
						else {
							strIDs += _T(",");
							strIDs += strID;
						}

						listAdds.push_back( cInfo );
					}
				}
				else
					_ASSERTE( SE_SUCCESS == errcode );
			} else
				_ASSERTE( SE_SUCCESS == errcode );
	
			// stream開放
			if( SE_SUCCESS != ( errcode = SSE_stream_free( stream ) ) )
				_ASSERTE( SE_SUCCESS == errcode );
		} 
		else
			_ASSERTE( SE_SUCCESS == errcode );
	}
	else
		_ASSERTE( lpcszTableID != NULL && lObjectID >= 0 );

	return strIDs;
}

/**
 * @brief Dテーブルからフィーチャのステートを取得する
 *
 * @param conn		[in]			セッション
 * @param lpcszTableID		[in]			SDEに登録されているテーブルID
 * @param lObjectID			[in]			ステートを取得するフィーチャのOBJECTID
 * @param lpcszExcludeIDs	[in,optional]	除外リスト
 *
 * @return 取得したステートのID（カンマ区切り）
 */
CString CModifyCheck::GetDStates( SE_CONNECTION conn, LPCTSTR lpcszTableID, LONG lObjectID, LPCTSTR lpcszExcludeIDs/* = NULL*/ )
{
	USES_CONVERSION;

	CString strIDs;
	TCHAR strID[15];
	// 引数チェック
	if( lpcszTableID != NULL && lObjectID >= 0 )
	{
		// クエリ作成
		CString strSQL;
		// 除外リストがある場合は入れる
		if( lpcszExcludeIDs != NULL )
		{
			strSQL.Format(_T("SELECT DELETED_AT FROM D%s WHERE SDE_DELETES_ROW_ID = %d AND DELETED_AT NOT IN ( \
				SELECT SDE_STATE_ID FROM A%s WHERE OBJECTID = %d ) AND DELETED_AT NOT IN (%s)"), lpcszTableID, lObjectID, lpcszTableID, lObjectID, lpcszExcludeIDs );
		}
		else {
			strSQL.Format(_T("SELECT DELETED_AT FROM D%s WHERE SDE_DELETES_ROW_ID = %d AND DELETED_AT NOT IN ( \
				SELECT SDE_STATE_ID FROM A%s WHERE OBJECTID = %d)"), lpcszTableID, lObjectID, lpcszTableID, lObjectID );
		}

		// クエリオブジェクトの作成
		LONG errcode = SE_SUCCESS;
		SE_STREAM stream = NULL;
		if ( SE_SUCCESS == ( errcode = SSE_stream_create( conn, &stream ) ) )
		{
			if( SE_SUCCESS == ( errcode = SSE_stream_prepare_sql( stream, T2CA(strSQL) ) ) )
			{
				if( SE_SUCCESS == ( errcode = SSE_stream_execute( stream ) ) )
				{
					while( SE_FINISHED != ( errcode = SSE_stream_fetch( stream ) ) )
					{
						if( SE_SUCCESS != errcode )
						{
							// エラー
							_ASSERTE( SE_SUCCESS == errcode );
							break;
						}
						LONG lStateID = -1;
						if( SE_SUCCESS != ( errcode = SSE_stream_get_integer( stream, 1, &lStateID ) ) )
							_ASSERTE( FALSE );

						// STATE_IDの文字列を作成しておく
						_ltot( lStateID, strID, 10 );
						if( strIDs.IsEmpty() )
							strIDs = strID;
						else {
							strIDs += _T(",");
							strIDs += strID;
						}
					}
				} 
				else
					_ASSERTE( SE_SUCCESS == errcode );
			}
			else
				_ASSERTE( SE_SUCCESS == errcode );
	
			// stream開放
			if( SE_SUCCESS != ( errcode = SSE_stream_free( stream ) ) )
				_ASSERTE( SE_SUCCESS == errcode );
		}
		else
			_ASSERTE( SE_SUCCESS == errcode );
	}
	else
		_ASSERTE( lpcszTableID != NULL && lObjectID >= 0 );

	return strIDs;
}

/**
 * @brief 自分の編集系統をカンマ区切りで取得する
 *
 * @param conn	[in]	セッション
 * @param lpcszVer		[in]	バージョン名（接続ユーザ名はつかない）
 *
 * @retval CString
 */
CString CModifyCheck::GetEditStateChain( SE_CONNECTION conn, LPCTSTR lpcszVer )
{
	USES_CONVERSION;

	CString strIDs;
	TCHAR strID[15];
	// 引数チェック
	if( lpcszVer != NULL )
	{	
		// クエリ作成
		CString strSQL;
		strSQL.Format(_T("SELECT STATE_ID FROM SDE.STATES START WITH STATE_ID = ( SELECT STATE_ID FROM SDE.VERSIONS WHERE NAME = '%s' ) CONNECT BY STATE_ID = PRIOR PARENT_STATE_ID AND NOT STATE_ID = 0 \
			UNION SELECT STATE_ID FROM SDE.STATE_LOCKS WHERE SDE_ID = ( SELECT SDE_ID FROM SDE.OBJECT_LOCKS WHERE OBJECT_ID = ( SELECT VERSION_ID FROM SDE.VERSIONS WHERE NAME = '%s' ) )"), lpcszVer, lpcszVer );

		// クエリオブジェクトの作成
		LONG errcode = SE_SUCCESS;
		SE_STREAM stream = NULL;
		if ( SE_SUCCESS == ( errcode = SSE_stream_create( conn, &stream ) ) )
		{
			if( SE_SUCCESS == ( errcode = SSE_stream_prepare_sql( stream, T2CA(strSQL) ) ) )
			{
				if( SE_SUCCESS == ( errcode = SSE_stream_execute( stream ) ) )
				{
					ATLTRACE(_T("EDITSTATE:\n"));
					while( SE_FINISHED != ( errcode = SSE_stream_fetch( stream ) ) )
					{
						if( SE_SUCCESS != errcode )
						{
							// エラー
							_ASSERTE( SE_SUCCESS == errcode );
							break;
						}

						// なぜかDOUBLEで返ってくる？？
						// SDE API のバグと思われるが、将来修正されることを考えてLONGでも取れるようにしておく
						LONG lStateID = -1;
						SE_COLUMN_DEF column_def = {0};
						if( SE_SUCCESS == ( errcode = SSE_stream_describe_column( stream, 1, &column_def ) ) )
						{
							switch( column_def.sde_type )
							{
								case SE_INTEGER_TYPE:
									if( SE_SUCCESS != ( errcode = SSE_stream_get_integer( stream, 1, &lStateID ) ) )
										_ASSERTE( SE_SUCCESS == errcode );
									break;
								case SE_DOUBLE_TYPE:
								{ 
									DOUBLE dStateID = 0.0;
									if( SE_SUCCESS != ( errcode = SSE_stream_get_double( stream, 1, &dStateID ) ) )
										_ASSERTE( SE_SUCCESS == errcode );
									lStateID = (LONG)dStateID;
									break;
								}
								case SE_FLOAT_TYPE:
								{ 
									float fStateID = 0.0;
									if( SE_SUCCESS != ( errcode = SSE_stream_get_float( stream, 1, &fStateID ) ) )
										_ASSERTE( SE_SUCCESS == errcode );
									lStateID = (LONG)fStateID;
									break;
								}
								default:
									_ASSERTE( FALSE );
									break;
							}
						}
						else
							_ASSERTE( SE_SUCCESS == errcode );

						// 文字に変換して格納
						_ltot( lStateID, strID, 10 );
						if( strIDs.IsEmpty() )
						{
							strIDs = strID;
							ATLTRACE(_T("%s"), strID );
						}
						else {
							strIDs += _T(",");
							strIDs += strID;
							ATLTRACE(_T(",%s"), strID );
						}
					}
					ATLTRACE(_T("%s"), _T("\n\n") );
				} 
				else
					_ASSERTE( SE_SUCCESS == errcode );
			} 
			else
				_ASSERTE( SE_SUCCESS == errcode );

			// stream開放
			if( SE_SUCCESS != ( errcode = SSE_stream_free( stream ) ) )
				_ASSERTE( SE_SUCCESS == errcode );
		}
		else
			_ASSERTE( SE_SUCCESS == errcode );
	}
	else
		_ASSERTE( lpcszVer != NULL );

	return strIDs;
}
#if 0
BOOL CModifyCheck::CreateConnection( const CRow& cRow, CString& strVersion, CString& strDatasetName, CString& strConnStr )
{
	CComBSTR sDatasetName, bVersion;
	IWorkspacePtr pIWorkspace;
	CComVariant vaUser, vaInstance, vaServer;

/*	// 作業者の取得
	if( m_ipRule == NULL ) m_ipRule.CreateInstance( CLSID_SiNDYRuleObj );
	m_ipRule->get_OperatorName( &m_bstrOperator );*/
	
//	LONG lOID = cRow.GetOID();

	IDatasetPtr ipDataset( cRow.GetTable() );
	if ( ipDataset )
	{
		// Workspace の取得
		ipDataset->get_Workspace(&pIWorkspace);
		ipDataset->get_Name(&sDatasetName);

		if ( sDatasetName.Length() != 0 ) {
			sDatasetName.ToUpper();
		}
	}

	if ( pIWorkspace )
	{
		CComBSTR sPath;
		IPropertySetPtr pIPropertySet;

		pIWorkspace->get_PathName(&sPath);

		if ( sPath.Length() == 0)  {
			/// SDE 
			pIWorkspace->get_ConnectionProperties(&pIPropertySet);

			if ( pIPropertySet ) {
				/// 接続情報の取得
				pIPropertySet->GetProperty( CComBSTR(_T("USER")), &vaUser );
				pIPropertySet->GetProperty( CComBSTR(_T("INSTANCE")), &vaInstance );
				pIPropertySet->GetProperty( CComBSTR(_T("SERVER")), &vaServer );
			} else {
				ATLASSERT(FALSE);
				return FALSE;
			}

			/// Version 情報
			IVersionedWorkspacePtr pIVersionedWorkspace;
			pIVersionedWorkspace = pIWorkspace;


			IVersionPtr pIVersion;
			pIVersion = pIWorkspace;
			pIVersion->get_VersionName(&bVersion);
		} else {
			/// Personal GeoDB, & Others
			return FALSE;
		}
	} else {
		/// Workspace が取得できない・・・
		return FALSE;
	}

	USES_CONVERSION;

	/// 接続を一意に判断するための文字列
	strConnStr = vaServer.bstrVal;
	strConnStr += vaInstance.bstrVal;
	strConnStr += vaUser.bstrVal;

	if ( g_mapConnection.find(strConnStr) == g_mapConnection.end() ) {
		/// SDE に接続
		SE_CONNECTION conn = NULL;
		//conn = new SdeConnect;

		// Direct Connection の場合はパスワードを password@ネットサービス名に変更する
		CString strInstance = vaInstance.bstrVal, strServer = vaServer.bstrVal, strPassword;
		if( 0 == strInstance.Find( _T("sde:oracle"), 0 ) )
		{
			int nPos = strServer.Find(_T('.'), 0 );
			if( nPos > 0 )
				strPassword.Format(_T("%s@%s.tdc"), OLE2T(vaUser.bstrVal), strServer.Left(nPos) );
			else
				strPassword.Format(_T("%s@%s.tdc"), OLE2T(vaUser.bstrVal), strServer );
		}
		else
			strPassword = OLE2T(vaUser.bstrVal);

		SE_ERROR err = {0};
		if ( SE_SUCCESS == SSE_connection_create(
			OLE2A(vaServer.bstrVal),
			OLE2A(vaInstance.bstrVal),
			"",
			OLE2A(vaUser.bstrVal),
			T2CA(strPassword),
			&err,
			&conn ) )
		{
			g_mapConnection[strConnStr] = conn;

			ATLTRACE(_T("接続成功\n"));
		} else {
			MessageBox( GetActiveWindow(), SDE_CONNECT_FAILED, _T("ModifyCheck"), MB_OK|MB_ICONWARNING);
			return FALSE;
		}

		// テーブルのリストを取得
		GetTableList(conn, vaUser);
	}

	strVersion = bVersion;
	strDatasetName = sDatasetName.m_str;

	return TRUE;
}
#endif // if 0
BOOL CModifyCheck::GetTableList(SE_CONNECTION conn, const VARIANT& vaUser )
{
	ATLTRACE(_T("CModifyCheck::GetTableList\n"));

	CComBSTR sSelect, sFrom, sWhere, sSql, sTmp;

	/// SDE.TABLE_REGISTRY から現在のレイヤの対応する A テーブル、D テーブルに使う ID を取得
	sSelect = _T("SELECT REGISTRATION_ID, OWNER || '.' || TABLE_NAME ");
	sFrom = _T("FROM SDE.TABLE_REGISTRY ");
	sWhere = _T("WHERE OWNER = '");
	sTmp = vaUser.bstrVal;
	sTmp.ToUpper();
	sWhere.Append(sTmp);
	sWhere.Append(_T("'"));

	USES_CONVERSION;

	sSql = sSelect;
	sSql.AppendBSTR(sFrom);
	sSql.AppendBSTR(sWhere);

	// クエリオブジェクトの作成
	LONG errcode = SE_SUCCESS;
	SE_STREAM stream = NULL;
	if ( SE_SUCCESS == ( errcode = SSE_stream_create( conn, &stream ) ) )
	{
		if( SE_SUCCESS == ( errcode = SSE_stream_prepare_sql( stream, OLE2CA(sSql) ) ) )
		{
			if( SE_SUCCESS == ( errcode = SSE_stream_execute( stream ) ) )
			{
				long lVal = -1;
				CString strNum;
				while( SE_FINISHED != ( errcode = SSE_stream_fetch( stream ) ) )
				{
					if( SE_SUCCESS != errcode )
					{
						// エラー
						_ASSERTE( SE_SUCCESS == errcode );
						break;
					}

					if( SE_SUCCESS != ( errcode = SSE_stream_get_integer( stream, 1, &lVal ) ) )
						break;

					strNum.Format(_T("%d"), lVal);

					SE_COLUMN_DEF column_def = {0};
					if( SE_SUCCESS == ( errcode = SSE_stream_describe_column( stream, 2, &column_def ) ) ) {
						switch( column_def.sde_type )
						{
							case SE_STRING_TYPE:
								{
									CHAR strTable[256] = "";
									if( SE_SUCCESS == ( errcode = SSE_stream_get_string( stream, 2, strTable ) ) ) {
										/// A テーブル、D テーブル検索用のキャッシュに追加
										g_mapTabls[CString(strTable)] = strNum;
									}
								}
								break;
							case SE_NSTRING_TYPE:
								{
									WCHAR strTable[256] = L"";
									if( SE_SUCCESS == ( errcode = SSE_stream_get_nstring( stream, 2, strTable ) ) ) {
										/// A テーブル、D テーブル検索用のキャッシュに追加
										g_mapTabls[CString(strTable)] = strNum;
									}
								}
								break;
							default:
								break;
						}
					}
				}
			}
			else
				return FALSE;
		}
		else
			return FALSE;

		// stream開放
		if( SE_SUCCESS != ( errcode = SSE_stream_free( stream ) ) )
			_ASSERTE( SE_SUCCESS == errcode );
	}
	else
		return FALSE;

	return TRUE;
}

} // sindy
