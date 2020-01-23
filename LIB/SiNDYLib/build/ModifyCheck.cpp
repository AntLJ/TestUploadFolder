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
	LPCTSTR SDE_CONNECT_FAILED     = _T("SDE�ւ̐ڑ������s���܂���");
	LPCTSTR EXIST_WHEN_MIGRATING   = _T("�ڍs������");
	LPCTSTR CREATING_NEW           = _T("�V�K�쐬");
	LPCTSTR DELETE_IT              = _T("�폜");
	LPCTSTR SHAPE_CHANGE           = _T("�`��ύX");
	LPCTSTR ATTRIBUTE_MODIFICATION = _T("�����ύX");
	LPCTSTR SHANGHAI_DATA_IMPORT   = _T("��C�f�[�^�C���|�[�g");
	LPCTSTR DATA_IMPORT            = _T("�f�[�^�C���|�[�g");
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
TCHAR g_strModifyCheckProcedure[48] = _T("ModifyCheck");	//!< ModifyCheck�v���V�[�W����
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
	m_strTime = m_strTime.Left( m_strTime.GetLength() - 1 ); // asctime�͉��s�����Ă����c
}

CModifyCheck::CModifyCheck()
{
	m_bUseProcedure = true;
	m_bUncheck = false;
	m_bExistProcedure = true;	// ��x�v���V�[�W�������邩�ǂ����`�F�b�N����̂ł܂���true

	CRegKey cRegKey;
	if( ERROR_SUCCESS == cRegKey.Open( HKEY_CURRENT_USER, _T("Software\\INCREMENT P CORPORATION\\SiNDY-e\\Tools\\Common") ) )
	{
		DWORD dwValue;

		// �v���V�[�W�����g�p���邩�ǂ��������W�X�g������ǂݍ���
		if( ERROR_SUCCESS == cRegKey.QueryValue( dwValue, _T("UseProcedure") ) )
			m_bUseProcedure = ( dwValue == 1 ) ? true : false;		

		// ModifyCheck���g�p�����ǂ��������W�X�g������ǂݍ���
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
	TRACEMESSAGE(_T("%s : �v���V�[�W���F%s ModifyCheck�F%s\n"), __func__, m_bUseProcedure ? _T("�g�p����") : _T("�g�p���Ȃ�"), m_bUncheck ? _T("���s���Ȃ�") : _T("���s����") );
#endif // ifdef _DEBUG_FOR_ESRIJ

	bool bRet = true;	// �f�t�H���g�́u���ɕҏW����Ă���v�Ƃ���

	if( ! m_bUncheck )
	{
#ifdef _DEBUG
		bool bChecked = false;
		DWORD DWSTART = GetTickCount();
#endif
		USES_CONVERSION;

		// OBJECTID�擾
		long lOID = cRow.GetOID();

#ifdef _DEBUG_FOR_ESRIJ
		TRACEMESSAGE(_T("%s : �`�F�b�N�Ώ�OID : %d\n"), __func__, lOID );
#endif // ifdef _DEBUG_FOR_ESRIJ

		// [bug 3258] �o�[�W������Ή��̏ꍇ�̓X���[����
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
				// ���W�X�g���̃L�[���L���ŁA���v���V�[�W��������Ƃ��ɂ̂݃v���V�[�W���� ModifyCheck
				// �v���V�[�W���̗L�������ۂɃR�[�����邱�ƂŊm�F���邽�߁A����� m_bExistProcedure = TRUE
MODIFYCHECK:
				if ( m_bUseProcedure && m_bExistProcedure )
				{
#ifdef _DEBUG_FOR_ESRIJ
					TRACEMESSAGE(_T("%s : �v���V�[�W���F%s\n"), __func__, m_bExistProcedure ? _T("���݂���Ɖ���") : _T("���݂��Ȃ�") );
#endif // ifdef _DEBUG_FOR_ESRIJ

					// Modify Check �� PL/SQL �Ŏ��s����ꍇ
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
						MessageBox( GetActiveWindow(), sSql, _T("�v���V�[�W���R�[��"), MB_OK|MB_ICONINFORMATION );
#endif // ifdef _DEBUG_FOR_ESRIJ*/

						HRESULT hr = ipWorkspace->ExecuteSQL(CComBSTR(sSql));

						if ( SUCCEEDED(hr)) {
#ifdef _DEBUG_FOR_ESRIJ
							TRACEMESSAGE(_T("%s : ipWorkspace->ExecuteSQL() : �Ԃ�l : %ld\n"), __func__, hr );
#endif // ifdef _DEBUG_FOR_ESRIJ
							bRet = false;
						} else {
#ifdef _DEBUG_FOR_ESRIJ
							TRACEMESSAGE(_T("%s : ipWorkspace->ExecuteSQL() : �Ԃ�l : %ld\n"), __func__, hr );
#endif // ifdef _DEBUG_FOR_ESRIJ
							/// �Փ˂����m���ꂽ
							cInfo.m_bIsModified = true;

							IFeatureWorkspacePtr ipFeatureWorkspace(ipVersionedWorkspace);

							ITablePtr ipTable;
							ipFeatureWorkspace->OpenTable(CComBSTR(_T("REFERENCE.MODIFY_RESULT")), &ipTable);
#ifdef _DEBUG_FOR_ESRIJ
							TRACEMESSAGE(_T("%s : ���ʂ̎擾�e�[�u�� : %s : OPEN[%s]\n"), __func__, _T("REFERENCE.MODIFY_RESULT"), ( NULL != ipTable ) ? _T("����") : _T("���s") );
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
												/// �폜���ꂽ�ꍇ
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
												//sMsg.Format(_T("[SERVER] �I�u�W�F�N�g %d �� %s �ɂ���� %s �Ɂw%s�x�̍�ƖړI�� %s ���Ă��܂��BTABLE[A%s] STATE[%d]\n"), vOID.lVal, CString(CComBSTR(vOperator.bstrVal)), sTime, sPurpose, sUpdateType, sTable, vState.lVal);
											}

											// �ǉ����ꂽ�f�[�^�͍폜���Ă�������
											if( SUCCEEDED( ipWorkspace->ExecuteSQL(CComBSTR(_T("DELETE FROM REFERENCE.MODIFY_RESULT"))) ) )
											{
#ifdef _DEBUG_FOR_ESRIJ
												TRACEMESSAGE(_T("%s : ipWorkspace->ExecuteSQL(CComBSTR(_T(\"DELETE FROM REFERENCE.MODIFY_RESULT\"))) : ����\n"), __func__ );
#endif // ifdef _DEBUG_FOR_ESRIJ
											}
											else {
#ifdef _DEBUG_FOR_ESRIJ
												TRACEMESSAGE(_T("%s : ipWorkspace->ExecuteSQL(CComBSTR(_T(\"DELETE FROM REFERENCE.MODIFY_RESULT\"))) : ����\n"), __func__ );
#endif // ifdef _DEBUG_FOR_ESRIJ
												_ASSERTE(false);
											}
											//*sMessage = T2BSTR(sMsg);
											//*bModified = VARIANT_TRUE;

											//return true;
										}
									}
#ifdef _DEBUG_FOR_ESRIJ
									TRACEMESSAGE(_T("%s : �擾�ł����X�e�[�gID : %ld�i-1�̏ꍇ�́A�v���V�[�W�������ŃG���[�A���̓v���V�[�W�������݂��Ȃ��j\n"), __func__, cInfo.m_lStateID );
#endif // ifdef _DEBUG_FOR_ESRIJ
								}
							} else {

								m_bExistProcedure = FALSE;

								// �ēx���[�`���𐳂����ʂ����߂ɂ�ނ𓾂� goto ���g�p����
								// ���őO�̂悤�ɏ����������Ă��ǂ����A��̏������ύX�ɂȂ����Ƃ���
								// ���̏�����ύX���Y��� ModifyCheck ���X���[���Ȃ��悤�ɂ��邽��
								goto MODIFYCHECK;

//									*sMessage = T2BSTR(_T("REFERENCE.MODIFY_RESULT ��������܂���B�v���V�[�W�����������C���X�g�[������Ă��邩�m�F���Ă��������B"));
//									*bModified = VARIANT_TRUE;
//	
//									return S_FALSE;		
							}
						}
					}
				}
				else {
#ifdef _MODIFYCHECK_CREATE_NEW_CONNECTION
					// �ڑ��i�����̂��̂�����ꍇ�͂����T���j
					CString strVersion, strDatasetName, strConnStr;
					if( CreateConnection( cRow, strVersion, strDatasetName, strConnStr ) == FALSE )
					{
						_ASSERTE( FALSE );
						//return S_FALSE;
					}

					// �Y�� OBJECT_ID ��T��
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
					// �����R�l�N�V�������擾
					SE_CONNECTION conn = NULL;
					ISdeWorkspaceImplPtr ipSdeWorkspace( cRow.GetWorkspace() );
					if( NULL != ipSdeWorkspace )
					{
						ipSdeWorkspace->get_ConnectionHandle( (long*)&conn );
						// �e�[�u���̃��X�g���擾
						GetTableList(conn, CComVariant(cRow.GetNameString()->GetOwnerName()));
					}
					bRet = IsModifiedByLocal( conn, cRow.GetNameString()->GetOwnerTableName(), cRow.GetNameString()->GetVersionName(), cRow.GetOID(), cInfo );
#endif // ifdef _MODIFYCHECK_CREATE_NEW_CONNECTION
				}
			}
			else
				bRet = false;	// �p�[�\�i���E�W�I�f�[�^�x�[�X��V�F�[�v�t�@�C���̏ꍇ
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

// �ύX�`�F�b�N�Ώۂ̃I�u�W�F�N�g��ID
bool CModifyCheck::IsModifiedByLocal( SE_CONNECTION conn, LPCTSTR lpcszTable, LPCTSTR lpcszVersion, long lOID, CModifyInfo& cInfo )
{
	USES_CONVERSION;

	// lpcszVersion �� �o�[�W�������ƃI�[�i�[�ɕ�����
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

	// �����̕ҏW�n�����擾
	m_strEditStateChain = GetEditStateChain( conn, strVer );

	// �e�[�u�����L���b�V�����猟��
	std::map<CString, CString>::iterator it = g_mapTabls.find(lpcszTable);

	if ( it != g_mapTabls.end() )
	{
		// �����̕ҏW�n�������O���āAA�e�[�u���̃X�e�[�g���擾
		std::list<CModifyInfo> listAdds;
		CString strIDs = GetAStates( conn, it->second, lOID, listAdds, m_strEditStateChain );

		if( ! strIDs.IsEmpty() )
		{
			// �擾�����X�e�[�g�������Ă��邩�ǂ������`�F�b�N
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
						//sMessage.Format(_T("[CLIENT] �I�u�W�F�N�g %d �́A%s �� %s �Ɂw%s�x�̍�ƖړI�� %s ���Ă��܂��BTABLE[A%s] STATE[%d]"), lOID, itAdds->Operator, strDate, itAdds->Purpose, itAdds->UpdateType, it->second, lActiveState );
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

		// �����̕ҏW�n�������O���āAA�e�[�u���̃X�e�[�g���擾
		CString strDIDs = GetDStates( conn, it->second, lOID, m_strEditStateChain );

		if( ! strDIDs.IsEmpty() )
		{
			// �擾�����X�e�[�g�������Ă��邩�ǂ������`�F�b�N
			INT nActiveType = -1;
			LONG lActiveState = IsActiveStates( conn, strVer, strDIDs, nActiveType );
			if( lActiveState >= 0 )
			{
				cInfo.m_bIsModified = true;
				//sMessage.Format(_T("[CLIENT] �I�u�W�F�N�g %d �͍폜����܂����BTABLE[D%s] STATE[%d]"), lOID, it->second, lActiveState );
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
 * @brief �X�e�[�g�������Ă��邩�ǂ���
 *
 * @param conn	[in]	�Z�b�V����
 * @param lpcszVer		[in]	�ҏW���̃o�[�W������
 * @param lpcszIDs		[in]	�����Ώۂ̃X�e�[�g������i�J���}��؂�j
 * @param nActiveType	[out]	-1:Active�ł͂Ȃ� 0:�ҏW�����̓|�X�g��̃X�e�[�g 1:���[�J���o�[�W�����݂̂ɕۑ�����Ă���X�e�[�g
 *
 * @retval -1 �X�e�[�g�͐����ĂȂ�
 * @retval -1�ȊO �X�e�[�g�͐����Ă���i�X�e�[�g�ԍ��j
 */
LONG CModifyCheck::IsActiveStates( SE_CONNECTION conn, LPCTSTR lpcszVer, LPCTSTR lpcszIDs, INT& nActiveType )
{
	USES_CONVERSION;

	nActiveType = -1;	// ������
	if( lpcszVer != NULL && lpcszIDs != NULL )
	{
//		CComBSTR bstrOperator;
//		CString strOperator;
//		m_ipRule->get_OperatorName( &bstrOperator );
//		strOperator = bstrOperator;

		// �N�G���쐬
		CString strSQL;
		// strIDs��STATE_LOCKS�Ɉ�ł��܂܂�Ă����ꍇ�A�ҏW���̃X�e�[�g�Ȃ̂ŕҏW�s��
		// strIDs�����ݕҏW���̃o�[�W�����̐e�o�[�W�����̃X�e�[�g�܂ł̌n���Ɋ܂܂�Ă���̂ł���΁A�ۑ���̃X�e�[�g�Ȃ̂ŁA�ҏW�s��
		strSQL.Format(_T("SELECT STATE_ID FROM SDE.STATE_LOCKS WHERE STATE_ID IN (%s) UNION \
						  SELECT STATE_ID FROM SDE.STATES WHERE STATE_ID IN (%s) AND STATE_ID IN ( \
							SELECT STATE_ID FROM SDE.STATES START WITH STATE_ID = ( \
								SELECT STATE_ID FROM SDE.VERSIONS WHERE NAME = ( \
									SELECT PARENT_NAME FROM SDE.VERSIONS WHERE NAME = '%s') ) \
							CONNECT BY STATE_ID = PRIOR PARENT_STATE_ID AND NOT STATE_ID = 0 )"), lpcszIDs, lpcszIDs, lpcszVer );
		LONG lStateID = -1;
		// �N�G���I�u�W�F�N�g�̍쐬
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
							// �G���[
							_ASSERTE( SE_SUCCESS == errcode );
							break;
						}

						// �Ȃ���DOUBLE�ŕԂ��Ă���H�H
						// SDE API �̃o�O�Ǝv���邪�A�����C������邱�Ƃ��l����LONG�ł�����悤�ɂ��Ă���
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

			// stream�J��
			if( SE_SUCCESS != ( errcode = SSE_stream_free( stream ) ) )
				_ASSERTE( SE_SUCCESS == errcode );
		}
		else
			_ASSERTE( SE_SUCCESS == errcode );

		if( lStateID > 0 )
		{
			nActiveType = 0;	// �ҏW���̃X�e�[�g�y�сA�|�X�g��̃X�e�[�g
			return lStateID;
		}
		else {
			// strIDs�̃X�e�[�g�������Ă��邩�ǂ���
			// = ���̃X�e�[�g���疖�[�܂ł̌n���ɐ����Ă���o�[�W���������݂��邩�ǂ���
			std::list<CString> listIDs;
			AheSepStr2listString( lpcszIDs, _T(","), listIDs );
			for( std::list<CString>::const_iterator itIDs = listIDs.begin(); itIDs != listIDs.end(); ++itIDs )
			{
				// �N�G���쐬
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
									// �G���[
									_ASSERTE( SE_SUCCESS == errcode );
									break;
								}

								// �Ȃ���DOUBLE�ŕԂ��Ă���H�H
								// SDE API �̃o�O�Ǝv���邪�A�����C������邱�Ƃ��l����LONG�ł�����悤�ɂ��Ă���
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

					// stream�J��
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
 * @brief A�e�[�u������t�B�[�`���̃X�e�[�g���擾����
 *
 * @param conn		[in]			�Z�b�V����
 * @param lpcszTableID		[in]			SDE�ɓo�^����Ă���e�[�u��ID
 * @param lObjectID			[in]			�X�e�[�g���擾����t�B�[�`����OBJECTID
 * @param listAddsData		[out]			�擾�����X�e�[�g���X�g
 * @param lpcszExcludeIDs	[in,optional]	���O���X�g
 *
 * @return �擾�����X�e�[�g��ID�i�J���}��؂�j
 */
CString CModifyCheck::GetAStates( SE_CONNECTION conn, LPCTSTR lpcszTableID, LONG lObjectID, std::list<CModifyInfo>& listAdds, LPCTSTR lpcszExcludeIDs/* = NULL*/ )
{
	USES_CONVERSION;

	CString strIDs;
	TCHAR strID[15];
	// �����`�F�b�N
	if( lpcszTableID != NULL && lObjectID >= 0 )
	{
		// �N�G���쐬
		CString strSQL;
		// ���O���X�g������ꍇ�͓����
		if( lpcszExcludeIDs != NULL )
		{
			strSQL.Format(_T("SELECT OPERATOR, PURPOSE_C, MODIFYDATE, UPDATETYPE_C, SDE_STATE_ID, PROGMODIFYDATE, MODIFYPROGNAME FROM A%s \
				WHERE OBJECTID = %d AND ( SDE_STATE_ID NOT IN (%s) )"), lpcszTableID, lObjectID, lpcszExcludeIDs );
		}
		else 
			strSQL.Format(_T("SELECT OPERATOR, PURPOSE_C, MODIFYDATE, UPDATETYPE_C, SDE_STATE_ID, PROGMODIFYDATE, MODIFYPROGNAME FROM A%s WHERE OBJECTID = %d"), lpcszTableID, lObjectID );

		// �N�G���I�u�W�F�N�g�̍쐬
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
							// �G���[
							_ASSERTE( SE_SUCCESS == errcode );
							break;
						}
						CModifyInfo cInfo;

						// OPERATOR�isindy�̏ꍇ��MODIFYPROGNAME������j
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

						// PURPOSE_C�i�擾�ł��Ȃ��ꍇ��0�ɂ��Ă����j
						if( SE_SUCCESS != ( errcode = SSE_stream_get_integer( stream, 2, &cInfo.m_lPurposeID ) ) ) cInfo.m_lPurposeID = 0;

/*						// ��ƖړIID->��ƖړI���ɕϊ�
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

						// UpdateType_C�i���Ȃ��ꍇ��-1�ɂ��Ă����j
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

						// STATE_ID�̕�������쐬���Ă���
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
	
			// stream�J��
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
 * @brief D�e�[�u������t�B�[�`���̃X�e�[�g���擾����
 *
 * @param conn		[in]			�Z�b�V����
 * @param lpcszTableID		[in]			SDE�ɓo�^����Ă���e�[�u��ID
 * @param lObjectID			[in]			�X�e�[�g���擾����t�B�[�`����OBJECTID
 * @param lpcszExcludeIDs	[in,optional]	���O���X�g
 *
 * @return �擾�����X�e�[�g��ID�i�J���}��؂�j
 */
CString CModifyCheck::GetDStates( SE_CONNECTION conn, LPCTSTR lpcszTableID, LONG lObjectID, LPCTSTR lpcszExcludeIDs/* = NULL*/ )
{
	USES_CONVERSION;

	CString strIDs;
	TCHAR strID[15];
	// �����`�F�b�N
	if( lpcszTableID != NULL && lObjectID >= 0 )
	{
		// �N�G���쐬
		CString strSQL;
		// ���O���X�g������ꍇ�͓����
		if( lpcszExcludeIDs != NULL )
		{
			strSQL.Format(_T("SELECT DELETED_AT FROM D%s WHERE SDE_DELETES_ROW_ID = %d AND DELETED_AT NOT IN ( \
				SELECT SDE_STATE_ID FROM A%s WHERE OBJECTID = %d ) AND DELETED_AT NOT IN (%s)"), lpcszTableID, lObjectID, lpcszTableID, lObjectID, lpcszExcludeIDs );
		}
		else {
			strSQL.Format(_T("SELECT DELETED_AT FROM D%s WHERE SDE_DELETES_ROW_ID = %d AND DELETED_AT NOT IN ( \
				SELECT SDE_STATE_ID FROM A%s WHERE OBJECTID = %d)"), lpcszTableID, lObjectID, lpcszTableID, lObjectID );
		}

		// �N�G���I�u�W�F�N�g�̍쐬
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
							// �G���[
							_ASSERTE( SE_SUCCESS == errcode );
							break;
						}
						LONG lStateID = -1;
						if( SE_SUCCESS != ( errcode = SSE_stream_get_integer( stream, 1, &lStateID ) ) )
							_ASSERTE( FALSE );

						// STATE_ID�̕�������쐬���Ă���
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
	
			// stream�J��
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
 * @brief �����̕ҏW�n�����J���}��؂�Ŏ擾����
 *
 * @param conn	[in]	�Z�b�V����
 * @param lpcszVer		[in]	�o�[�W�������i�ڑ����[�U���͂��Ȃ��j
 *
 * @retval CString
 */
CString CModifyCheck::GetEditStateChain( SE_CONNECTION conn, LPCTSTR lpcszVer )
{
	USES_CONVERSION;

	CString strIDs;
	TCHAR strID[15];
	// �����`�F�b�N
	if( lpcszVer != NULL )
	{	
		// �N�G���쐬
		CString strSQL;
		strSQL.Format(_T("SELECT STATE_ID FROM SDE.STATES START WITH STATE_ID = ( SELECT STATE_ID FROM SDE.VERSIONS WHERE NAME = '%s' ) CONNECT BY STATE_ID = PRIOR PARENT_STATE_ID AND NOT STATE_ID = 0 \
			UNION SELECT STATE_ID FROM SDE.STATE_LOCKS WHERE SDE_ID = ( SELECT SDE_ID FROM SDE.OBJECT_LOCKS WHERE OBJECT_ID = ( SELECT VERSION_ID FROM SDE.VERSIONS WHERE NAME = '%s' ) )"), lpcszVer, lpcszVer );

		// �N�G���I�u�W�F�N�g�̍쐬
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
							// �G���[
							_ASSERTE( SE_SUCCESS == errcode );
							break;
						}

						// �Ȃ���DOUBLE�ŕԂ��Ă���H�H
						// SDE API �̃o�O�Ǝv���邪�A�����C������邱�Ƃ��l����LONG�ł�����悤�ɂ��Ă���
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

						// �����ɕϊ����Ċi�[
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

			// stream�J��
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

/*	// ��Ǝ҂̎擾
	if( m_ipRule == NULL ) m_ipRule.CreateInstance( CLSID_SiNDYRuleObj );
	m_ipRule->get_OperatorName( &m_bstrOperator );*/
	
//	LONG lOID = cRow.GetOID();

	IDatasetPtr ipDataset( cRow.GetTable() );
	if ( ipDataset )
	{
		// Workspace �̎擾
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
				/// �ڑ����̎擾
				pIPropertySet->GetProperty( CComBSTR(_T("USER")), &vaUser );
				pIPropertySet->GetProperty( CComBSTR(_T("INSTANCE")), &vaInstance );
				pIPropertySet->GetProperty( CComBSTR(_T("SERVER")), &vaServer );
			} else {
				ATLASSERT(FALSE);
				return FALSE;
			}

			/// Version ���
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
		/// Workspace ���擾�ł��Ȃ��E�E�E
		return FALSE;
	}

	USES_CONVERSION;

	/// �ڑ�����ӂɔ��f���邽�߂̕�����
	strConnStr = vaServer.bstrVal;
	strConnStr += vaInstance.bstrVal;
	strConnStr += vaUser.bstrVal;

	if ( g_mapConnection.find(strConnStr) == g_mapConnection.end() ) {
		/// SDE �ɐڑ�
		SE_CONNECTION conn = NULL;
		//conn = new SdeConnect;

		// Direct Connection �̏ꍇ�̓p�X���[�h�� password@�l�b�g�T�[�r�X���ɕύX����
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

			ATLTRACE(_T("�ڑ�����\n"));
		} else {
			MessageBox( GetActiveWindow(), SDE_CONNECT_FAILED, _T("ModifyCheck"), MB_OK|MB_ICONWARNING);
			return FALSE;
		}

		// �e�[�u���̃��X�g���擾
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

	/// SDE.TABLE_REGISTRY ���猻�݂̃��C���̑Ή����� A �e�[�u���AD �e�[�u���Ɏg�� ID ���擾
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

	// �N�G���I�u�W�F�N�g�̍쐬
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
						// �G���[
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
										/// A �e�[�u���AD �e�[�u�������p�̃L���b�V���ɒǉ�
										g_mapTabls[CString(strTable)] = strNum;
									}
								}
								break;
							case SE_NSTRING_TYPE:
								{
									WCHAR strTable[256] = L"";
									if( SE_SUCCESS == ( errcode = SSE_stream_get_nstring( stream, 2, strTable ) ) ) {
										/// A �e�[�u���AD �e�[�u�������p�̃L���b�V���ɒǉ�
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

		// stream�J��
		if( SE_SUCCESS != ( errcode = SSE_stream_free( stream ) ) )
			_ASSERTE( SE_SUCCESS == errcode );
	}
	else
		return FALSE;

	return TRUE;
}

} // sindy
