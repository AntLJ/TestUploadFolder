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
#include "VersionedWorkspaceDiff.h"
#include "Row.h"
#include "GlobalFunctions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace {
#ifdef SINDY_FOR_ENGLISH
	LPCTSTR DIFF_EXTRACTION           = _T("[Difference extraction]:Category[%s] Table[%s]\n");
	LPCTSTR FAIL_LOAD_TABLE_AND_SKIP  = _T("%s : Failed to load the table. A process is skipped.(The reason:%s)\n");
	LPCTSTR DIFF_EXTRACTION_SKIP      = _T("[Difference extraction]:Category[%s] Table[%s]: It is skipped because it is out of target\n");
#else
	LPCTSTR DIFF_EXTRACTION           = _T("[差分抽出]：カテゴリ[%s] テーブル[%s]\n");
	LPCTSTR FAIL_LOAD_TABLE_AND_SKIP  = _T("%s : テーブルの読み込みに失敗しました。処理を飛ばします。（理由：%s）\n");
	LPCTSTR DIFF_EXTRACTION_SKIP      = _T("[差分抽出]：カテゴリ[%s] テーブル[%s] : 差分出力対象外なので飛ばします。\n");
#endif	// SINDY_FOR_ENGLISH
} // namespace


namespace sindy {
using namespace errorcode;

CVersionedWorkspaceDiff::CVersionedWorkspaceDiff()
{
}

CVersionedWorkspaceDiff::~CVersionedWorkspaceDiff()
{
	clear();
}

CVersionedWorkspaceDiff::const_iterator CVersionedWorkspaceDiff::end( esriDifferenceType emType ) const
{
	CDiffRowMap::const_iterator it = m_mapDiff.find( emType );
	if( it == m_mapDiff.end() )
	{
		// 入れ物を作ってやる
		CDiffRowMap* pMapDiff = const_cast<CDiffRowMap*>(&m_mapDiff);
		CRowList tmp = (*pMapDiff)[emType];

		it = m_mapDiff.find( emType );
		_ASSERTE( it != m_mapDiff.end() ); // 必ずある
	}

	return it->second.end();
}
CVersionedWorkspaceDiff::const_iterator CVersionedWorkspaceDiff::begin( esriDifferenceType emType ) const
{
	CDiffRowMap::const_iterator it = m_mapDiff.find( emType );
	if( it == m_mapDiff.end() )
	{
		// 入れ物を作ってやる
		CDiffRowMap* pMapDiff = const_cast<CDiffRowMap*>(&m_mapDiff);
		CRowList tmp = (*pMapDiff)[emType];

		it = m_mapDiff.find( emType );
		_ASSERTE( it != m_mapDiff.end() ); // 必ずある
	}

	return it->second.begin();
}

long CVersionedWorkspaceDiff::DiffCur2Map( esriDifferenceType emType, IDifferenceCursor* ipCursor, IFields* ipFields )
{
	long lCount = 0;
	if( ipCursor )
	{
		long lOID = -1;
		_IRowPtr ipRow;

		// 共有オブジェクト作成
		boost::shared_ptr<CFieldMap> spFields( new CFieldMap( ipFields ) );
		boost::shared_ptr<CTableNameString> spNameString;

		while( S_OK == ipCursor->Next( &lOID, &ipRow ) )
		{
			if( NULL == ipRow ) break;

			// 最初の一つで初期化
			if( ! spNameString )
				spNameString.reset( new CTableNameString( ipRow ) );

			CRow cRow( ipRow );
			cRow.CreateCache( true, spFields, spNameString );
			cRow.SetLongValue( 0, lOID );

			//m_mapDiff.insert( std::map<esriDifferenceType,CRow>::value_type(emType,cRow) );
			m_mapDiff[emType].push_back( cRow );
			//TCHAR strOpe[16];
			//TRACEMESSAGE(_T("\tOID[%ld] OPERATOR[%s]\n"), lOID, cRow.GetStringValueByFieldName( kOperator, 16, strOpe ) );
			++lCount;
		}
	}

	return lCount;
}

sindyErrCode CVersionedWorkspaceDiff::Differences( IEnumDatasetName* ipDatasetNames, IFeatureWorkspace* ipWork, sindyDifferenceType emDiffType, IProgressor* ipProgress/* = NULL*/ )
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

	LOGASSERTEERR_IF( NULL != ipDatasetNames && SUCCEEDED( ipDatasetNames->Reset() ), sindyErr_COMFunctionFailed )
	{
		IDatasetNamePtr ipDatasetName;
		while( S_OK == ipDatasetNames->Next( &ipDatasetName ) )
		{
			if( NULL == ipDatasetName ) break;

			CComBSTR bstrCategory, bstrName;
			ipDatasetName->get_Category( &bstrCategory );
			ipDatasetName->get_Name( &bstrName );

			esriDatasetType emType = esriDTAny;
			LOGASSERTERR_IF( SUCCEEDED( ipDatasetName->get_Type( &emType ) ), sindyErr_COMFunctionFailed )
			{
				TRACEMESSAGE( ipProgress, -1, false, false, DIFF_EXTRACTION, COLE2CT(bstrCategory), COLE2CT(bstrName) );
				ATLTRACE(_T("[差分抽出]：カテゴリ[%s] テーブル[%s]\n"), COLE2CT(bstrCategory), COLE2CT(bstrName) );

				switch( emType )
				{
					case esriDTFeatureDataset:
					{
						ATLTRACE( _T("\n"));

						IEnumDatasetNamePtr ipDatasetNames2;
						LOGASSERTEERR_IF( SUCCEEDED( ipDatasetName->get_SubsetNames( &ipDatasetNames2 ) ), sindyErr_COMFunctionFailed )
						{
							emErr = Differences( ipDatasetNames2, ipWork, emDiffType, ipProgress );
						}
						break;
					}
					case esriDTFeatureClass:
					case esriDTTable:
					{
						IUnknownPtr ipUnk;
						if( SUCCEEDED( ((INamePtr)ipDatasetName)->Open( &ipUnk ) ) )
						{
							ITablePtr ipTable( ipUnk );
							if( NULL != ipTable )
							{
								// 同じ物を別のワークスペースでオープン
								// ここでS_FALSEが返ってくるテーブルは差分比較に失敗するため、SUCCEEDEDでなく、
								// S_OKかどうかで判定(SINDYBユーザのテーブルではS_FALSEが返ってくることを確認)
								ITablePtr ipAnotherTable;
								if( S_OK == ipWork->OpenTable(bstrName, &ipAnotherTable) )
								{
									IVersionedTablePtr ipAnotherVTable( ipAnotherTable );
									LOGASSERTEERR_IF( NULL != ipAnotherVTable, sindyErr_COMQueryInterfaceFailed )
									{
										IFieldsPtr ipFieldsCopy;	// CRowのキャッシュを作成するのに必要
										bool bBreak = false;		// 飛ばす用フラグ
										for( int i = 0; i < 6; ++i )
										{
											sindyDifferenceType emNowDiffType = (sindyDifferenceType)0;
											switch( i )
											{
												case esriDifferenceTypeInsert:			emNowDiffType = ( emDiffType & sindyDifferenceTypeInsert			) ? sindyDifferenceTypeInsert			: (sindyDifferenceType)0; break;
												case esriDifferenceTypeDeleteNoChange:	emNowDiffType = ( emDiffType & sindyDifferenceTypeDeleteNoChange	) ? sindyDifferenceTypeDeleteNoChange	: (sindyDifferenceType)0; break;
												case esriDifferenceTypeUpdateNoChange:	emNowDiffType = ( emDiffType & sindyDifferenceTypeUpdateNoChange	) ? sindyDifferenceTypeUpdateNoChange	: (sindyDifferenceType)0; break;
												case esriDifferenceTypeUpdateUpdate:	emNowDiffType = ( emDiffType & sindyDifferenceTypeUpdateUpdate		) ? sindyDifferenceTypeUpdateUpdate		: (sindyDifferenceType)0; break;
												case esriDifferenceTypeUpdateDelete:	emNowDiffType = ( emDiffType & sindyDifferenceTypeUpdateDelete		) ? sindyDifferenceTypeUpdateDelete		: (sindyDifferenceType)0; break;
												case esriDifferenceTypeDeleteUpdate:	emNowDiffType = ( emDiffType & sindyDifferenceTypeDeleteUpdate		) ? sindyDifferenceTypeDeleteUpdate		: (sindyDifferenceType)0; break;
												default: LOGASSERT( 0 <= i && i < 6, sindyErr_AlgorithmFailed ); break;
											}

											if( 0 < emNowDiffType )
											{
												HRESULT hr = 0;
												IDifferenceCursorPtr ipDiffCursor;
												if( SUCCEEDED( hr = ipAnotherVTable->Differences( ipTable, (esriDifferenceType)i, NULL, &ipDiffCursor ) ) )
												{
													if( NULL == ipFieldsCopy )
													{
														IFieldsPtr ipFields;
														ipTable->get_Fields( &ipFields );
														ipFieldsCopy = AheCreateClone( ipFields );
													}
													long lDiffCount = DiffCur2Map( (esriDifferenceType)i, ipDiffCursor, ipFieldsCopy );
													ATLTRACE( _T("[変更%s]"), ( 0 < lDiffCount ) ? _T("あり") : _T("なし"));
												}
												else {
													switch( hr )
													{
														case FDO_E_SE_TABLE_NOT_MULTIVERSION:	// バージョン対応ではないので飛ばす
															ATLTRACE( _T("[バージョン非対応]"));
															bBreak = true;
															break;
														case FDO_E_SE_STATES_ARE_SAME:			// 変更が無かったので飛ばす
															ATLTRACE( _T("[変更なし]"));
															break;
														default:
															SUCCEEDED_IF( hr, emErr = sindyErr_COMFunctionFailed, _T("HRESULT[%ld(0x%X)] CATEGORY[%s] TABLE[%s] DIFFTYPE[%d]"), hr, hr, OLE2T(bstrCategory), OLE2T(bstrName), emNowDiffType );
															break;
													}
												}
											}
											if( bBreak ) break;
										}
										ATLTRACE( _T("\n"));
									}

								}
								else
									TRACEMESSAGE( FAIL_LOAD_TABLE_AND_SKIP, __func__, GETCOMERRORSTRING() );
							}
							else
								ATLTRACE( _T("\n") );
						}
						else
							TRACEMESSAGE( FAIL_LOAD_TABLE_AND_SKIP, __func__, GETCOMERRORSTRING() );
						break;
					}
					default:
					{
						TRACEMESSAGE( ipProgress, -1, false, false, DIFF_EXTRACTION_SKIP, COLE2CT(bstrCategory), COLE2CT(bstrName) );
						break;
					}
				}
			}
		}
	}

	return emErr;
}

// ワークスペース間での差分を取得する
sindyErrCode CVersionedWorkspaceDiff::Differences( IFeatureWorkspace* ipWork, sindyDifferenceType emDiffType, IProgressor* ipProgress/* = NULL*/ )
{
	return Differences( GetDatasetNames( esriDTAny ), ipWork, emDiffType, ipProgress );
}

// 取得差分にコンフリクトが含まれるかどうかをチェックする
bool CVersionedWorkspaceDiff::HasConflict() const
{
	const_iterator it;
	for( it = begin( esriDifferenceTypeDeleteUpdate ); it != end( esriDifferenceTypeDeleteUpdate ); ++it ) return true;
	for( it = begin( esriDifferenceTypeUpdateUpdate ); it != end( esriDifferenceTypeUpdateUpdate ); ++it ) return true;
	for( it = begin( esriDifferenceTypeUpdateDelete ); it != end( esriDifferenceTypeUpdateDelete ); ++it ) return true;

	return false;
}

void CVersionedWorkspaceDiff::clear()
{
	m_mapDiff.clear();
}

} // sindy
