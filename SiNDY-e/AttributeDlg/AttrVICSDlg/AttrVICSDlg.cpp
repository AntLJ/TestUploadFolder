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

// AttrVICSDlg.cpp : DLL �A�v���P�[�V�����p�̃G���g�� �|�C���g���`���܂��B
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AttrVICSDlg.h"

/// �E�B���h�E�}�l�[�W���z�u�}�b�v�iVICS�p�j
BEGIN_WINDOW_MAP(VICS_MAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(2,2))
		BEGINROWS(WRCT_PCT,0,90)
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_MESH)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_MESH)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_VICSCLASS)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_VICSCLASS)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_VICSID)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_VICSID)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_BUTTON_DRAW)
				RCTOFIT(-1)
				RCTOFIT(IDC_STATIC_SAMEVICS)
			ENDGROUP()
		ENDGROUP()
		BEGINROWS(WRCT_PCT, 0, 10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(0,0))
				RCREST(IDC_STATIC_VAR)
			ENDGROUP()
			RCSPACE(5)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_SOURCE)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_SOURCE)
			ENDGROUP()
		ENDGROUP()
	ENDGROUP()
END_WINDOW_MAP()

/**
 * �N���X�̖����I�����N���s�����߂ɁA���̃N���X��CExportDlg�N���X���p�����A�����_�C�A���O����CExportDlg���쐬���邱�Ƃɂ��
 * �����_�C�A���O������t���ɂ����邱�Ƃ��ł���N���X�{�̂��쐬����܂��B
 * �ڂ�����http://ptvi.hp.infoseek.co.jp/vc/p_0000010.html���Q�Ƃ̂���
 *
 * @retval CAttrVICSDlg��Ԃ��܂��B
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CAttrVICSDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrVICSDlg �����o�֐�
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrVICSDlg::CAttrVICSDlg()
 * @brief �R���X�g���N�^
 *
 * �u���V�̏����������Ă����܂��B
 */
CAttrVICSDlg::CAttrVICSDlg()
	: ATTR_BASE_CLASS(VICS_MAP, IDD_ATTRVICSDLG),
		m_wnd0( _T("EDIT"),		this, IDC_EDIT_MESH ),
		m_wnd1( _T("COMBOBOX"), this, IDC_COMBO_VICSCLASS ),
		m_wnd2( _T("EDIT"),		this, IDC_EDIT_VICSID ),
		m_wnd3( _T("EDIT"),		this, IDC_EDIT_SOURCE ),
		m_wnd4( _T("BUTTON"),		this, IDC_BUTTON_DRAW )
{
}

/** 
 * @fn HWND CAttrVICSDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief �q�_�C�A���O���쐬���܂��B
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)�Ń_�C�A���O���쐬���܂��B
 *
 * @param hWndParent	[in]	�e�E�B���h�E�n���h��
 * @param dwInitParam	[in]	�_�C�A���O�������p�����[�^�i���ݓ��Ɏg�p���Ă��܂���j
 *
 * @retval �쐬���ꂽ�q�_�C�A���O�̃n���h����Ԃ��܂��B
 */
HWND CAttrVICSDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	m_hChildWnd = ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
	return m_hChildWnd;
}

/**
 * @fn void CAttrVICSDlg::Delete()
 * @brief �쐬�����q�_�C�A���O���폜���܂��B
 *
 * �_�C�A���O���폜����Ƃ��ɕK���Ă�ŉ������B�Ă΂Ȃ��ƃ��������[�N���Ă��܂��܂��B
 */
void CAttrVICSDlg::Delete()
{
	ClearFeatureDefs();
	::DestroyWindow(m_hChildWnd);
    delete this; // �K��new�Ŋm�ۂ���O��
}

/**
 * @fn BOOL CAttrVICSDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
 * @brief ���ݕ\�������ׂ��t�B�[�`����FeatureDef�C���f�b�N�X��n���܂��B
 * ����ɁA�t�B�[�`���̑����Ƃ͊֌W�Ȃ��R���g���[���̐�������Ă����܂��B
 * �_�C�A���O���J�����Ƃ��ɍŏ��Ƀt�H�[�J�X�̓������Ă��ė~�����R���g���[���̐ݒ�����Ă����܂��B
 *
 * @param lFeatureIndex	[in]	�t�B�[�`����`�N���X�̃C���f�b�N�X�ԍ�
 * @param lTableIndex	[in]	�e�[�u����`�N���X�̃C���f�b�N�X�ԍ�
 * @param lRowIndex		[in]	���R�[�h��`�N���X�̃C���f�b�N�X�ԍ�
 * @param bForce		[in]	�����I�ɕҏW���邩�ǂ���
 * @param bEditable		[in]	�ҏW�\���ǂ���
 *
 * @retval �Ӗ����Ȃ��iTRUE�����Ԃ��Ȃ��j
 */
BOOL CAttrVICSDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
    BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );
	CWindow staticName( GetDlgItem(IDC_STATIC_SAMEVICS) );
	staticName.SetWindowText( (LPCTSTR)(_T("")) );

    return bRet;
}

/**
 * @fn void CAttrVICSDlg::CreateControlRelation()
 * @brief �q�_�C�A���O�����ۂɍ쐬���܂��B
 *
 * ���ۂ̍쐬��ControlRel�ɔC���܂��B
 */
void CAttrVICSDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// �e�E�C���h�E�n���h����ݒ肵�Ȃ���
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {

		m_cControlRel.SetHWND( m_hWnd );

		// VICS
		// ���b�V���R�[�h
		m_cControlRel.SetControl( IDC_EDIT_MESH,		TYPE_EDIT,	0,	0,	_T("MESHCODE"),		NULL, m_strFeatureClassName);
		// VICS���
		m_cControlRel.SetControl( IDC_COMBO_VICSCLASS,	TYPE_COMBO,	0,	0,	_T("VICS_CLASS"),	NULL, m_strFeatureClassName);
		// VICS_ID
		m_cControlRel.SetControl( IDC_EDIT_VICSID,		TYPE_EDIT,	0,	0,	_T("VICS_ID"),		NULL, m_strFeatureClassName);
		// ���\�[�X
		m_cControlRel.SetControl( IDC_EDIT_SOURCE,		TYPE_EDITSTRING,0,	0,	_T("VICSINFO"),	NULL, m_strFeatureClassName );
	}
}

/**
 * @fn void CAttrVICSDlg::DrawSameVICS(IRowPtr ipVICSRow)
 * @brief ����VICS���n�C���C�g�\�����܂�
 *
 * @param ipVICSRow	[in]	�`�悵����VICS�t�B�[�`��
 */
void CAttrVICSDlg::DrawSameVICS(_IRowPtr ipVICSRow)
{
	if( !ipVICSRow )
		return;

	// �\������Ă���VICS�̃��b�V���R�[�h�A��ʁAVICSID���擾����
	IFieldsPtr ipFields;
	LONG	lIndex = 0;
	ipVICSRow->get_Fields( &ipFields );
	
	LONG lOID = 0;
	ipVICSRow->get_OID( &lOID );

	CComVariant vaValue;
	ipFields->FindField( CComBSTR(_T("MESHCODE")),&lIndex );
	ipVICSRow->get_Value( lIndex, &vaValue );
	long lMeshCode = vaValue.lVal;

	ipFields->FindField( CComBSTR(_T("VICS_CLASS")),&lIndex );
	ipVICSRow->get_Value( lIndex, &vaValue );
	long lVICSClass = vaValue.lVal;

	ipFields->FindField( CComBSTR(_T("VICS_ID")),&lIndex );
	ipVICSRow->get_Value( lIndex, &vaValue );
	long lVICSID = vaValue.lVal;

	ITablePtr		ipTable;
	_ICursorPtr		ipCursor;
	IQueryFilterPtr ipQueryFilter( CLSID_QueryFilter );
	CString strWhereClause, strSubFields;
        
	ipVICSRow->get_Table( &ipTable );

	// ���b�V���R�[�h�Ō���
	strWhereClause.Format(_T("MESHCODE = %d AND VICS_CLASS = %d AND VICS_ID = %d"), lMeshCode, lVICSClass, lVICSID );
	strSubFields.Format(_T("OBJECTID,VICS_CLASS,VICS_ID,SHAPE"));
	ipQueryFilter->put_WhereClause( CComBSTR(strWhereClause) );
	ipQueryFilter->put_SubFields( CComBSTR(strSubFields) );
	ipTable->Search( ipQueryFilter, VARIANT_FALSE, &ipCursor );

	_IRowPtr ipRow;
	LONG lCounter=0;
	if( ipCursor )
	{
		IGeometryPtr ipVICSGeometry;
		while( ipCursor->NextRow( &ipRow ) == S_OK )
		{
			IFeaturePtr ipFeature = ipRow;
			if( !ipVICSGeometry )
				ipFeature->get_ShapeCopy( &ipVICSGeometry );
			else
			{
				IGeometryPtr ipGeomMaster;
				IGeometryPtr ipGeomSlave;
				ipFeature->get_ShapeCopy( &ipGeomMaster );

				ITopologicalOperatorPtr ipTopo(ipVICSGeometry);
				if( SUCCEEDED( ipTopo->Union( ipGeomMaster, &ipGeomSlave)))
				{
					if(ipGeomSlave )
					{
						ipVICSGeometry = ipGeomSlave;
						lCounter++;
					}
				}		
			}
		}
		m_ipVICSGeom = ipVICSGeometry;
	}

	// VICS�̎��肾���ĕ`��
	IEnvelopePtr ipFeatEnv;
	if( m_ipVICSGeom )
	{
		m_ipVICSGeom->get_Envelope( &ipFeatEnv );
		m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, ipFeatEnv );
	}
}

/**
 * @fn void CAttrVICSDlg::Refresh()
 * @brief �`�撆��VICS�̎�����ĕ`�悵�܂�
 */
void CAttrVICSDlg::Refresh()
{
	if( m_ipVICSGeom )
		AheDrawArrowline( m_cArcHelper.GetScreenDisplay(), m_ipVICSGeom, RGB( 0, 250, 0), 3, esriSLSSolid);
}
