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

// LQAttrRouteDlg.cpp: CLQAttrRouteDlg �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LQAttrRouteDlg.h"

using namespace std;

/// �E�B���h�E�}�l�[�W���z�u�}�b�v�i�A�N�Z�X/���p�����p�j
BEGIN_WINDOW_MAP(ROUTE_MAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(0,0))
		RCSPACE(10)
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,10))
			RCTOFIT(IDC_STATIC_ROUTECLASS)
			RCREST(IDC_COMBO_ROUTECLASS)
		ENDGROUP()
		RCSPACE(7)
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,10))
			RCTOFIT(IDC_STATIC_SOURCENAME)
			RCREST(IDC_EDIT_SOURCENAME)
		ENDGROUP()
		RCSPACE(7)
		RCREST(-1)
		RCTOFIT(IDC_STATIC_SOURCESEP)
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,10))
			RCTOFIT(IDC_STATIC_SOURCE)
			RCREST(IDC_EDIT_SOURCE)
		ENDGROUP()
	ENDGROUP()
END_WINDOW_MAP()

/**
 * �N���X�̖����I�����N���s�����߂ɁA���̃N���X��CExportDlg�N���X���p�����A�����_�C�A���O����CExportDlg���쐬���邱�Ƃɂ��
 * �����_�C�A���O������t���ɂ����邱�Ƃ��ł���N���X�{�̂��쐬����܂��B
 * �ڂ�����http://ptvi.hp.infoseek.co.jp/vc/p_0000010.html���Q�Ƃ̂���
 *
 * @retval CLQAttrRouteDlg��Ԃ��܂��B
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CLQAttrRouteDlg;
}

/////////////////////////////////////////////////////////////////////////////
//
// CLQAttrRouteDlg �����o�֐�
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CLQAttrRouteDlg::CLQAttrDefaultDlg()
 * @brief �R���X�g���N�^
 *
 * �Ȃɂ����܂���
 */
CLQAttrRouteDlg::CLQAttrRouteDlg() 
	: LQATTR_COMMON_BASE_CLASS(ROUTE_MAP, IDD_LQATTRROUTEDLG),
	m_wndRouteClass( _T("COMBOBOX"), this, IDC_COMBO_ROUTECLASS ),
	m_wndSource( _T("EDIT"), this, IDC_EDIT_SOURCE ),
	m_wndSourceName( _T("EDIT"), this, IDC_EDIT_SOURCENAME )
{
}

/** 
 * @fn HWND CLQAttrRouteDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief �q�_�C�A���O���쐬���܂��B
 *
 * LQATTR_COMMON_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)�Ń_�C�A���O���쐬���܂��B
 *
 * @param hWndParent	[in]	�e�E�B���h�E�n���h��
 * @param dwInitParam	[in]	�_�C�A���O�������p�����[�^�i���ݓ��Ɏg�p���Ă��܂���j
 *
 * @retval �쐬���ꂽ�q�_�C�A���O�̃n���h����Ԃ��܂��B
 */
HWND CLQAttrRouteDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{
	// �_�C�A���O�̍쐬��LQATTR_COMMON_BASE_CLASS�ɔC���܂�
	m_hChildWnd = LQATTR_COMMON_BASE_CLASS::Create(hWndParent,dwInitParam);
	return m_hChildWnd;
}

/**
 * @fn void CLQAttrRouteDlg::Delete()
 * @brief �쐬�����q�_�C�A���O���폜���܂��B
 *
 * �_�C�A���O���폜����Ƃ��ɕK���Ă�ŉ������B�Ă΂Ȃ��ƃ��������[�N���Ă��܂��܂��B
 */
void CLQAttrRouteDlg::Delete()
{
	::DestroyWindow(m_hChildWnd);
    delete this;
}

/**
 * @fn BOOL CLQAttrRouteDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
 * @brief ���ݕ\�������ׂ��t�B�[�`����FeatureDef�C���f�b�N�X��n���܂��B
 *
 * @param lFeatureIndex	[in]	�t�B�[�`����`�N���X�̃C���f�b�N�X�ԍ�
 * @param lTableIndex	[in]	�e�[�u����`�N���X�̃C���f�b�N�X�ԍ�
 * @param lRowIndex		[in]	���R�[�h��`�N���X�̃C���f�b�N�X�ԍ�
 * @param bForce		[in]	�����I�ɕҏW���邩�ǂ���
 * @param bEditable		[in]	�ҏW�\���ǂ���
 *
 * @retval �Ӗ����Ȃ��iTRUE�����Ԃ��Ȃ��j
 */
BOOL CLQAttrRouteDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
    BOOL bRet = LQATTR_COMMON_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
		::SetFocus( GetDlgItem(IDC_COMBO_ROUTECLASS) );
	else
		::SendMessage( GetDlgItem(IDC_COMBO_ROUTECLASS), WM_KILLFOCUS, NULL, NULL );

    return bRet;
}

/**
 * @fn void CLQAttrRouteDlg::CreateControlRelation()
 * @brief �q�_�C�A���O�����ۂɍ쐬���܂��B
 *
 * ���ۂ̍쐬��ControlRel�ɔC���܂��B
 */
void CLQAttrRouteDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// �e�E�C���h�E�n���h����ݒ肵�Ȃ���
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		CString strInfoTableName = CLoadQueueInfo::GetLoadQueueInfo( sindyeRoadQueueRoute, sindyeLoadQueueInfoInfoTableName );
		m_cControlRel.SetHWND( m_hWnd );
		// VICS�p
		m_cControlRel.SetControl( IDC_COMBO_ROUTECLASS,	TYPE_COMBO,		0, 0,	_T("ROUTECLASS_C"),	NULL, strInfoTableName );
		m_cControlRel.SetControl( IDC_EDIT_SOURCE,		TYPE_EDITSTRING,0, 0,	_T("SOURCE"),		NULL, strInfoTableName );
		m_cControlRel.SetControl( IDC_EDIT_SOURCENAME,	TYPE_EDITSTRING,0, 0,	_T("SOURCE_NAME"),	NULL, strInfoTableName );
	}
}

BOOL CLQAttrRouteDlg::IsAlreadyExistOverlapedRoute( LONG lCurrentRouteClass, BOOL bErrEnd/* = TRUE*/ )
{
	// ���H�����NID���擾
	list<ROWDEF> listRoads;
	LONG lDrawMode = CLoadQueueInfo::GetLoadQueueDrawMode( sindyeRoadQueueRoute );
	CString strInfTableName = CLoadQueueInfo::GetLoadQueueInfo( sindyeRoadQueueRoute, sindyeLoadQueueInfoInfoTableName );
	CString strLQTableName = CLoadQueueInfo::GetLoadQueueInfo( sindyeRoadQueueRoute, sindyeLoadQueueInfoLQTableName );
	CLQRowDef* pInfoRow = GetCurrentFeatureDef();
	CRelationalTableDef* pRelTable = NULL;

	if( pInfoRow )
		pRelTable = pInfoRow->GetRelationalTableDef( strLQTableName );
	if( pRelTable )
	{
		for( CRelationalTableDef::iterator it = pRelTable->begin(); it != pRelTable->end(); it++ )
		{
			CFieldDef* pFieldDef = it->GetFieldDef( LINK_ID_FIELD );
			if( pFieldDef )
			{
				if( pFieldDef->m_vaValue.vt == VT_I4 )
				{
					ROWDEF def;
					def.flg = lDrawMode;
					def.oid = pFieldDef->m_vaValue.lVal;
					listRoads.push_back( def );
				}
			}
		}
	}
	_ASSERTE( listRoads.size() > 0 );
	
	// ���H�����NID����LQ������
	list<ROWDEF> listLQRows;
	list<LONG> listInfoIDs;
	CString strRoadRelFieldName = CLoadQueueInfo::GetLoadQueueInfo( sindyeRoadQueueRoute, sindyeLoadQueueInfoLQRFRoadFieldName );
	AheSelectLQRowByRoadIDs( listRoads, pRelTable->GetTable(), strRoadRelFieldName, lDrawMode, listLQRows, listInfoIDs );
	// InfoIDs����Info������
	list< CAdapt<_IRowPtr> > listIRow;
	_IRowPtr ipRow = pInfoRow->GetFeature();
	ITablePtr ipInfoTable;
	LONG lOID = -1;
	if( ipRow )
	{
		ipRow->get_Table( &ipInfoTable );
		ipRow->get_OID( &lOID );
	}
	if( ipInfoTable )
	{
		for( list<LONG>::iterator it = listInfoIDs.begin(); it != listInfoIDs.end(); it++ )
		{
			_IRowPtr ipInfoRow;
			LONG lOIDTmp = -1;

			ipInfoTable->GetRow( *it, &ipInfoRow );
			// ���݂̃����N��͔�΂��Ċi�[
			if( ipInfoRow != NULL ) ipInfoRow->get_OID( &lOIDTmp );	
			if( lOID != lOIDTmp ) 
			{
				// �����N�񑮐��ҏW�c�[���Ɋ��ɓǂݍ��܂�Ă����炻��������l���擾
				list<CLQRowDef>::iterator itFeatureDefList;
				for( itFeatureDefList = m_pFeatureDefList->begin(); itFeatureDefList != m_pFeatureDefList->end(); itFeatureDefList++ )
				{
					_IRowPtr ipListInfoRow = itFeatureDefList->GetFeature();
					LONG lListInfoID = -1;

					ipListInfoRow->get_OID( &lListInfoID );
					if( lOIDTmp == lListInfoID )
						break;
				}
				CComVariant vaRouteClass;
				if( itFeatureDefList != m_pFeatureDefList->end() )
				{
					CFieldDef* pRouteClassDef    = itFeatureDefList->GetFieldDef( strInfTableName, ROUTECLASS_FIELD );
					if( pRouteClassDef != NULL && pRouteClassDef->m_vaValue.vt == VT_I4 && lCurrentRouteClass == pRouteClassDef->m_vaValue.lVal )
						listIRow.push_back( ipInfoRow );
				}
				// �����N�񑮐��ҏW�c�[���Ɋ��ɓǂݍ��܂�Ă��Ȃ������猟���������̂���l���擾
				else {
					vaRouteClass = AheGetAttribute( ipInfoRow, ROUTECLASS_FIELD );
					if( vaRouteClass.vt == VT_I4 && lCurrentRouteClass == vaRouteClass.lVal )
						listIRow.push_back( ipInfoRow );
				}
			}
		}
	}
	// ������ʂŕ���������LQ��T��
	listInfoIDs.clear();
	for( list<ROWDEF>::iterator it = listLQRows.begin(); it != listLQRows.end(); it++ )
	{
		LONG lOriID = it->oid;
		for( list< CAdapt<_IRowPtr> >::iterator itIRow = listIRow.begin(); itIRow != listIRow.end(); itIRow++ )
		{
			// ��Ɋi�[����IRow�Ɠ���ID��ROUTE_ID�ɂ����̂̂݃`�F�b�N
			LONG lRowID = -1, lRouteID = -1;
			itIRow->m_T->get_OID( &lRowID );
			CComVariant vaInfoID = AheGetAttribute( it->obj, INF_ID_FIELD );
			if( vaInfoID.vt == VT_I4 ) lRouteID = vaInfoID.lVal;
			if( lRowID == lRouteID )
			{
				for( CRelationalTableDef::iterator itRelTable = pRelTable->begin(); itRelTable != pRelTable->end(); itRelTable++ )
				{
					CFieldDef* vaTempID = itRelTable->GetFieldDef( _T("OBJECTID") );
					LONG lTempID = vaTempID->m_vaValue.lVal;
					CComVariant vaInfoID = AheGetAttribute( it->obj, INF_ID_FIELD );
					if( vaInfoID.vt == VT_I4 )
						listInfoIDs.push_back( vaInfoID.lVal );
				}
			}	
		}
	}
	listInfoIDs.sort();
	listInfoIDs.unique();
	if( listInfoIDs.size() > 0 )
	{
		CString strMsg( AheLoadString( IDS_DUPLIROUTE ) );
		for( list<LONG>::iterator it = listInfoIDs.begin(); it != listInfoIDs.end(); it++ )
		{
			if( lOID == *it ) break;
			CString strMsgTmp;
			strMsgTmp.Format(_T(" OBJECTID=%d "), *it );
			strMsg += strMsgTmp;
		}
		if( bErrEnd )
			strMsg += AheLoadString( IDS_CONTINUE );

		if( bErrEnd )
		{
			if( AttrMessageBox( strMsg, AheLoadString(IDS_CAPTION), MB_YESNO|MB_ICONWARNING, WARNLEVEL_HIGH  ) == IDNO )
				return FALSE;
		}
		else {
			AttrMessageBox( strMsg, AheLoadString(IDS_CAPTION), MB_OK|MB_ICONSTOP, WARNLEVEL_HIGH  );
			return FALSE;
		}
	}

	return TRUE;
}
BOOL CLQAttrRouteDlg::ErrorCheck()
{
	BOOL bRet = TRUE;
	CString strInfTableName = m_cLQInfo.GetLoadQueueInfo( sindyeRoadQueueRoute, sindyeLoadQueueInfoInfoTableName );
	CString strMsg;
	for( list<CLQRowDef>::iterator it = m_pFeatureDefList->begin(); it != m_pFeatureDefList->end(); it++ )
	{
		if( strInfTableName.CompareNoCase( it->GetTableName() ) == 0 )
		{
			LONG lOID;
			CFieldDef* pOIDFieldDef = it->GetFieldDef( strInfTableName, _T("OBJECTID"));
			lOID = pOIDFieldDef->m_vaValue.lVal;

			CFieldDef* pRouteFieldDef = it->GetFieldDef( strInfTableName, _T("ROUTECLASS_C") );

			if( !pRouteFieldDef )
			{
				_ASSERTE( pRouteFieldDef != NULL );
				continue;
			}

			CString strMsg;
			// �ē���ʃR�[�h�� NULL �������Ă��Ȃ����ǂ���
			LONG ltest = pRouteFieldDef->m_vaValue.lVal;
			if( pRouteFieldDef->m_vaValue.lVal == 0 )
			{
				strMsg.Format(AheLoadString( IDS_NOROUTECLASS ), strInfTableName, lOID);
				AttrMessageBox( strMsg, AheLoadString(IDS_CAPTION), MB_OK, WARNLEVEL_HIGH );

				bRet = FALSE;
				break;
			}
		}
	}
	
	return bRet;
}
