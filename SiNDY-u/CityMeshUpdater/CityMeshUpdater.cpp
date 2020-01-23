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

// CityMeshUpdater.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "ConnectDlg.h"
#include "grobalfunc.h"
#include <locale.h>
#include <map>
#include "CSV.h"
#include "arctl/coinitializer.h"

COutput g_cOutput;

void Run()
{
	CConnectDlg cConnectDlg;
	if( cConnectDlg.DoModal() == IDOK )
	{
		// 更新元、更新先の各ワークスペースを取得
		if( cConnectDlg.Connect( WORKING_VERSIONNAME ) )
		{
			// TMP_CITYMESHの全行を取得
			_tprintf( _T(" 更新元CSVを取得中...") );
			CCSV cCsv( cConnectDlg.GetCSVPath() );
			_tprintf( _T("%d [Records]\r\n"), cCsv.GetRecords() );

			if( cConnectDlg.IsVersioned() )
			{
				// 更新元バージョンとデフォルトバージョンとの差分取得
				MESHCODEMAP mapDiff;
				gf::DiffMapBetweenVersion( cConnectDlg.GetWorkspace( VER_SRC ), cConnectDlg.DestFeatureClassName(), mapDiff );

				// メッシュコードがかぶるものは削除
				gf::DeleteDuplicateKeys( mapDiff, cCsv.m_RowMap );

				// WORKING_VERSIONNAME バージョンの CITYMESHテーブルを更新
				IFeatureClassPtr ipFeatClass;
				if( SUCCEEDED( ((IFeatureWorkspacePtr)cConnectDlg.GetWorkspace( VER_DST ))->OpenFeatureClass( CComBSTR( cConnectDlg.DestFeatureClassName() ), &ipFeatClass ) ) ){			
					IWorkspaceEditPtr ipWorkspaceEdit( cConnectDlg.GetWorkspace( VER_DST ) );
					if(ipWorkspaceEdit)
					{
						int nUpdate=0;

						ipWorkspaceEdit->StartEditing( VARIANT_TRUE );
						ipWorkspaceEdit->StartEditOperation();

						g_cOutput.OutputMsg( _T("(b)を実更新先バージョンに更新...") );
						nUpdate += gf::UpdateTable( mapDiff, gf::CreateMapFromQuery( ipFeatClass, mapDiff ) );
						g_cOutput.OutputMsg( _T("(c)\r\n\r\n") );

						// CITYMESHテーブルをを全行取得
						_tprintf( _T(" CITYMESHテーブルを取得中...") );
						MESHCODEMAP CityMeshMap = gf::Table2Map( cConnectDlg.GetWorkspace( VER_DST ), cConnectDlg.DestFeatureClassName(), MESHCORD_FIELDNAME );
						_tprintf( _T("\r\n") );

						g_cOutput.OutputMsg( _T("(c)とTMP_CITYMESHとの差分を(c)に更新...") );
						nUpdate += gf::UpdateTable( cCsv.m_RowMap, cCsv.m_vecField, CityMeshMap  );
						g_cOutput.OutputMsg( _T("\r\n\r\n") );
						

						if( FAILED( ipWorkspaceEdit->StopEditOperation() ) )
						{
							g_cOutput.OutputMsg( _T("# FATAL StopEditOperation() 失敗 \n") );
							ipWorkspaceEdit->AbortEditOperation();
							ipWorkspaceEdit->StopEditing( VARIANT_TRUE );
							return;
						}

						if( FAILED( ipWorkspaceEdit->StopEditing( VARIANT_TRUE ) ) )
						{
							g_cOutput.OutputMsg( _T("# FATAL StopEditing() 失敗 \n") );
							return;
						}

						CString strMsg;
						strMsg.Format( _T("計%d件が更新されました。\r\n"), nUpdate );
						g_cOutput.OutputMsg( strMsg );
					}
				}
			}
			// ノンバージョン処理
			else
			{
				// CITYMESHテーブルをを全行取得
				_tprintf( _T(" CITYMESHテーブルを取得中...") );
				MESHCODEMAP CityMeshMap = gf::Table2Map( cConnectDlg.GetWorkspace( VER_DST ), cConnectDlg.DestFeatureClassName(), MESHCORD_FIELDNAME );
				_tprintf( _T("\r\n") );


				IWorkspaceEditPtr ipWorkspaceEdit( cConnectDlg.GetWorkspace( VER_DST ) );
				HRESULT hr = ipWorkspaceEdit->StartEditing( VARIANT_FALSE );
				hr = ipWorkspaceEdit->StartEditOperation();


				g_cOutput.OutputMsg( _T("CITYMESHテーブルを更新...") );
				int nUpdate = gf::UpdateTable( cCsv.m_RowMap, cCsv.m_vecField, CityMeshMap );
				g_cOutput.OutputMsg( _T("\r\n\r\n") );

				
				if( FAILED( ipWorkspaceEdit->StopEditOperation() ) )
				{
					g_cOutput.OutputMsg( _T("# FATAL StopEditOperation() 失敗 \n") );
					ipWorkspaceEdit->AbortEditOperation();
					ipWorkspaceEdit->StopEditing( VARIANT_TRUE );
					return;
				}

				if( FAILED( ipWorkspaceEdit->StopEditing( VARIANT_TRUE ) ) )
				{
					g_cOutput.OutputMsg( _T("# FATAL StopEditing() 失敗 \n") );
					return;
				}

				CString strMsg;
				strMsg.Format( _T("%d件が更新されました。\r\n"), nUpdate );
				g_cOutput.OutputMsg( strMsg );
			}

			// 統計情報更新
			ITablePtr ipTable;
			((IFeatureWorkspacePtr)cConnectDlg.GetWorkspace( VER_DST ))->OpenTable( CComBSTR( cConnectDlg.DestFeatureClassName() ), &ipTable );
			IDatasetAnalyzePtr ipAnalyze(ipTable);
			long n=-1;
			g_cOutput.OutputMsg( _T("統計情報を更新中。。") );
			ipAnalyze->get_AllowableComponents( &n );
			HRESULT hr = ipAnalyze->Analyze( n );
			if( SUCCEEDED(hr) )
				g_cOutput.OutputMsg( _T(" > OK\r\n") );
			else
			{
				CString strMsg;
				strMsg.Format( _T("失敗（%d）\r\n"), hr );
				g_cOutput.OutputMsg( strMsg );
			}
		}
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	_tsetlocale ( LC_ALL, _T("") );

	
	const arctl::coinitializer coinit;
	Run();

	_tprintf( _T("Press Enter Key\r\n") );
	getchar();

	return 0;
}

