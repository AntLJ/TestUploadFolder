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

// CityMeshUpdater.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
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
		// �X�V���A�X�V��̊e���[�N�X�y�[�X���擾
		if( cConnectDlg.Connect( WORKING_VERSIONNAME ) )
		{
			// TMP_CITYMESH�̑S�s���擾
			_tprintf( _T(" �X�V��CSV���擾��...") );
			CCSV cCsv( cConnectDlg.GetCSVPath() );
			_tprintf( _T("%d [Records]\r\n"), cCsv.GetRecords() );

			if( cConnectDlg.IsVersioned() )
			{
				// �X�V���o�[�W�����ƃf�t�H���g�o�[�W�����Ƃ̍����擾
				MESHCODEMAP mapDiff;
				gf::DiffMapBetweenVersion( cConnectDlg.GetWorkspace( VER_SRC ), cConnectDlg.DestFeatureClassName(), mapDiff );

				// ���b�V���R�[�h�����Ԃ���͍̂폜
				gf::DeleteDuplicateKeys( mapDiff, cCsv.m_RowMap );

				// WORKING_VERSIONNAME �o�[�W������ CITYMESH�e�[�u�����X�V
				IFeatureClassPtr ipFeatClass;
				if( SUCCEEDED( ((IFeatureWorkspacePtr)cConnectDlg.GetWorkspace( VER_DST ))->OpenFeatureClass( CComBSTR( cConnectDlg.DestFeatureClassName() ), &ipFeatClass ) ) ){			
					IWorkspaceEditPtr ipWorkspaceEdit( cConnectDlg.GetWorkspace( VER_DST ) );
					if(ipWorkspaceEdit)
					{
						int nUpdate=0;

						ipWorkspaceEdit->StartEditing( VARIANT_TRUE );
						ipWorkspaceEdit->StartEditOperation();

						g_cOutput.OutputMsg( _T("(b)�����X�V��o�[�W�����ɍX�V...") );
						nUpdate += gf::UpdateTable( mapDiff, gf::CreateMapFromQuery( ipFeatClass, mapDiff ) );
						g_cOutput.OutputMsg( _T("(c)\r\n\r\n") );

						// CITYMESH�e�[�u������S�s�擾
						_tprintf( _T(" CITYMESH�e�[�u�����擾��...") );
						MESHCODEMAP CityMeshMap = gf::Table2Map( cConnectDlg.GetWorkspace( VER_DST ), cConnectDlg.DestFeatureClassName(), MESHCORD_FIELDNAME );
						_tprintf( _T("\r\n") );

						g_cOutput.OutputMsg( _T("(c)��TMP_CITYMESH�Ƃ̍�����(c)�ɍX�V...") );
						nUpdate += gf::UpdateTable( cCsv.m_RowMap, cCsv.m_vecField, CityMeshMap  );
						g_cOutput.OutputMsg( _T("\r\n\r\n") );
						

						if( FAILED( ipWorkspaceEdit->StopEditOperation() ) )
						{
							g_cOutput.OutputMsg( _T("# FATAL StopEditOperation() ���s \n") );
							ipWorkspaceEdit->AbortEditOperation();
							ipWorkspaceEdit->StopEditing( VARIANT_TRUE );
							return;
						}

						if( FAILED( ipWorkspaceEdit->StopEditing( VARIANT_TRUE ) ) )
						{
							g_cOutput.OutputMsg( _T("# FATAL StopEditing() ���s \n") );
							return;
						}

						CString strMsg;
						strMsg.Format( _T("�v%d�����X�V����܂����B\r\n"), nUpdate );
						g_cOutput.OutputMsg( strMsg );
					}
				}
			}
			// �m���o�[�W��������
			else
			{
				// CITYMESH�e�[�u������S�s�擾
				_tprintf( _T(" CITYMESH�e�[�u�����擾��...") );
				MESHCODEMAP CityMeshMap = gf::Table2Map( cConnectDlg.GetWorkspace( VER_DST ), cConnectDlg.DestFeatureClassName(), MESHCORD_FIELDNAME );
				_tprintf( _T("\r\n") );


				IWorkspaceEditPtr ipWorkspaceEdit( cConnectDlg.GetWorkspace( VER_DST ) );
				HRESULT hr = ipWorkspaceEdit->StartEditing( VARIANT_FALSE );
				hr = ipWorkspaceEdit->StartEditOperation();


				g_cOutput.OutputMsg( _T("CITYMESH�e�[�u�����X�V...") );
				int nUpdate = gf::UpdateTable( cCsv.m_RowMap, cCsv.m_vecField, CityMeshMap );
				g_cOutput.OutputMsg( _T("\r\n\r\n") );

				
				if( FAILED( ipWorkspaceEdit->StopEditOperation() ) )
				{
					g_cOutput.OutputMsg( _T("# FATAL StopEditOperation() ���s \n") );
					ipWorkspaceEdit->AbortEditOperation();
					ipWorkspaceEdit->StopEditing( VARIANT_TRUE );
					return;
				}

				if( FAILED( ipWorkspaceEdit->StopEditing( VARIANT_TRUE ) ) )
				{
					g_cOutput.OutputMsg( _T("# FATAL StopEditing() ���s \n") );
					return;
				}

				CString strMsg;
				strMsg.Format( _T("%d�����X�V����܂����B\r\n"), nUpdate );
				g_cOutput.OutputMsg( strMsg );
			}

			// ���v���X�V
			ITablePtr ipTable;
			((IFeatureWorkspacePtr)cConnectDlg.GetWorkspace( VER_DST ))->OpenTable( CComBSTR( cConnectDlg.DestFeatureClassName() ), &ipTable );
			IDatasetAnalyzePtr ipAnalyze(ipTable);
			long n=-1;
			g_cOutput.OutputMsg( _T("���v�����X�V���B�B") );
			ipAnalyze->get_AllowableComponents( &n );
			HRESULT hr = ipAnalyze->Analyze( n );
			if( SUCCEEDED(hr) )
				g_cOutput.OutputMsg( _T(" > OK\r\n") );
			else
			{
				CString strMsg;
				strMsg.Format( _T("���s�i%d�j\r\n"), hr );
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

