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

// AttrTelTpPointDlg.cpp : DLL �G�N�X�|�[�g�̎����ł��B


#include "stdafx.h"
#include "AttrTelTpPointDlg.h"

/// �E�B���h�E�}�l�[�W���z�u�}�b�v�i�s�s�n�}�ƌ`�p�j
BEGIN_WINDOW_MAP(TELTPPOINT_MAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(2,2))
		BEGINROWS(WRCT_REST,0,0)
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_ADDRCODE)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_ADDRCODE)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_ADDRNAME)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_ADDRNAME)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_KATAGAKI)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_KATAGAKI)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_KEISAINAME)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_KEISAINAME)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_POINTSTAT)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_POINTSTAT)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_ADDXY)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_ADDXY)
			ENDGROUP()
			RCSPACE(10)
		ENDGROUP()
		BEGINROWS(WRCT_TOFIT, 0, RCMARGINS(4,4))
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
 * @retval CAttrTelTpPointDlg��Ԃ��܂��B
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CAttrTelTpPointDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrTelTpPointDlg �����o�֐�
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrTelTpPointDlg::CAttrTelTpPointDlg()
 * @brief �R���X�g���N�^
 *
 * �u���V�̏����������Ă����܂��B
 */
CAttrTelTpPointDlg::CAttrTelTpPointDlg() 
	: ATTR_BASE_CLASS(TELTPPOINT_MAP, IDD_ATTRTELTPPOINTDLG),
		m_wnd0( _T("EDIT"), this, IDC_EDIT_ADDRCODE ),
		m_wnd1( _T("EDIT"), this, IDC_EDIT_ADDRNAME ),
		m_wnd2( _T("EDIT"), this, IDC_EDIT_KATAGAKI ),
		m_wnd3( _T("EDIT"), this, IDC_EDIT_KEISAINAME ),
		m_wnd4( _T("COMBO"), this, IDC_COMBO_POINTSTAT ),
		m_wnd5( _T("EDIT"), this, IDC_EDIT_SOURCE ),
		m_wnd6( _T("COMBO"), this, IDC_COMBO_ADDXY )
{
	// �t�H���g�쐬
	m_hFont = CreateFont(
		16, 0, 0, 0, 100,		// �����C���C�w�p�x�C�x�p�x�C����
		FALSE, FALSE, FALSE,    // �ΆC�����C�ŏ���
		SHIFTJIS_CHARSET,       // �L�����N�^�ݒ�
		OUT_DEFAULT_PRECIS,     // �o�͐��x
		CLIP_DEFAULT_PRECIS,    // �N���b�v���x
		DEFAULT_QUALITY,        // �i��
		FIXED_PITCH | FF_DONTCARE,// �s�b�`
		_T("�l�r�o�S�V�b�N"));   // �t�H���g���
	// �t�H���g�쐬
	m_hStaticFont = CreateFont(
		12, 0, 0, 0, FW_BOLD,		// �����C���C�w�p�x�C�x�p�x�C����
		FALSE, TRUE, FALSE,    // �ΆC�����C�ŏ���
		SHIFTJIS_CHARSET,       // �L�����N�^�ݒ�
		OUT_DEFAULT_PRECIS,     // �o�͐��x
		CLIP_DEFAULT_PRECIS,    // �N���b�v���x
		DEFAULT_QUALITY,        // �i��
		FIXED_PITCH | FF_DONTCARE,// �s�b�`
		_T("�l�r�o�S�V�b�N"));   // �t�H���g���
}

/** 
 * @fn HWND CAttrTelTpPointDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief �q�_�C�A���O���쐬���܂��B
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)�Ń_�C�A���O���쐬���܂��B
 *
 * @param hWndParent	[in]	�e�E�B���h�E�n���h��
 * @param dwInitParam	[in]	�_�C�A���O�������p�����[�^�i���ݓ��Ɏg�p���Ă��܂���j
 *
 * @retval �쐬���ꂽ�q�_�C�A���O�̃n���h����Ԃ��܂��B
 */
HWND CAttrTelTpPointDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	m_hChildWnd = ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
	return m_hChildWnd;
}

/**
 * @fn void CAttrTelTpPointDlg::Delete()
 * @brief �쐬�����q�_�C�A���O���폜���܂��B
 *
 * �_�C�A���O���폜����Ƃ��ɕK���Ă�ŉ������B�Ă΂Ȃ��ƃ��������[�N���Ă��܂��܂��B
 */
void CAttrTelTpPointDlg::Delete()
{
	ClearFeatureDefs();
	::DestroyWindow(m_hChildWnd);
    delete this; // �K��new�Ŋm�ۂ���O��
}

/**
 * @fn BOOL CAttrTelTpPointDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
 * @brief ���ݕ\�������ׂ��t�B�[�`����FeatureDef�C���f�b�N�X��n���܂��B
 * ����ɁA�t�B�[�`���̑����Ƃ͊֌W�Ȃ��R���g���[���̐�������Ă����܂��B
 * �_�C�A���O���J�����Ƃ��ɍŏ��Ƀt�H�[�J�X�̓������Ă��ė~�����R���g���[���̐ݒ�����Ă����܂��B
 * ���e�ԍ��̐ݒ�����Ă����܂��B
 *
 * @param lFeatureIndex	[in]	�t�B�[�`����`�N���X�̃C���f�b�N�X�ԍ�
 * @param lTableIndex	[in]	�e�[�u����`�N���X�̃C���f�b�N�X�ԍ�
 * @param lRowIndex		[in]	���R�[�h��`�N���X�̃C���f�b�N�X�ԍ�
 * @param bForce		[in]	�����I�ɕҏW���邩�ǂ���
 * @param bEditable		[in]	�ҏW�\���ǂ���
 *
 * @retval �Ӗ����Ȃ��iTRUE�����Ԃ��Ȃ��j
 */

BOOL CAttrTelTpPointDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
    BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	// ���ڃt�B�[�`���Ɋ֌W�Ȃ��G�f�B�b�g�{�b�N�X(��F�Q�Ɨp�̘H�����V�[�P���X�⌴�e�ԍ�����)�Ȃǂ𐧌䂷��
	// �ҏW�J�n�O�ƕҏW�J�n��ł��ҏW�s�̂Ƃ���DISABLE�ɂ���
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		if( CheckSamePoint() )
		{
			// �ҏW�J�n����Ă���Ƃ��ɂ��łɁASetFocus���Ă���
			::SetFocus( GetDlgItem(IDC_COMBO_POINTSTAT) );
		}
		else
		{
			AheMessageBox( AheLoadString( IDS_CANTEDIT ), AheLoadString( IDS_CAPTION ), MB_OK );
			::PostMessage( GetParent(), WM_CLOSE, 0, 0 ); 
			return false;
		}
	}

	// �ҏW�s�ɂ��Ă���
	SetDlgStatus(IDC_EDIT_ADDRCODE, FALSE, TRUE );
	SetDlgStatus(IDC_EDIT_ADDRNAME, FALSE, TRUE );
	SetDlgStatus(IDC_EDIT_KATAGAKI, FALSE, TRUE );
	SetDlgStatus(IDC_EDIT_KEISAINAME, FALSE, TRUE );
	::EnableWindow( GetDlgItem( IDC_COMBO_ADDXY ), FALSE);

    return bRet;
}

/**
 * @fn void CAttrBaseRailwayDlg::CreateControlRelation()
 * @brief �q�_�C�A���O�����ۂɍ쐬���܂��B
 *
 * ���ۂ̍쐬��ControlRel�ɔC���܂��B
 */
void CAttrTelTpPointDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// �e�E�C���h�E�n���h����ݒ肵�Ȃ���
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );
		// �d�b�ԍ��Z���|�C���g�p

		// �Z���R�[�h
		m_cControlRel.SetControl( IDC_EDIT_ADDRCODE,	TYPE_EDITSTRING,0,	0,  _T("ADDRCODE"),	NULL, _T("TELTP_POINT") );
		// �Z����
		m_cControlRel.SetControl( IDC_EDIT_ADDRNAME,	TYPE_EDITSTRING,0,	0,  _T("ADDRNAME"),	NULL, _T("TELTP_POINT") );
		// ����
		m_cControlRel.SetControl( IDC_EDIT_KATAGAKI,	TYPE_EDITSTRING,0,	0,  _T("KATAGAKINAME"),	NULL, _T("TELTP_POINT") );
		// �f�ږ�
		m_cControlRel.SetControl( IDC_EDIT_KEISAINAME,	TYPE_EDITSTRING,0,	0,  _T("KEISAINAME"),	NULL, _T("TELTP_POINT") );
		// �������ʃR�[�h
		m_cControlRel.SetControl( IDC_COMBO_POINTSTAT,	TYPE_COMBO,		0,	0,  _T("POINTSTAT_C"),	NULL, _T("TELTP_POINT") );
		// ���W�t�^��ʃR�[�h
		m_cControlRel.SetControl( IDC_COMBO_ADDXY,	TYPE_COMBO,		0,	0,  _T("ADDXY_C"),	NULL, _T("TELTP_POINT") );
		// ���\�[�X
		m_cControlRel.SetControl( IDC_EDIT_SOURCE,		TYPE_EDITSTRING,0,	0,  _T("SOURCE"),		NULL, _T("TELTP_POINT") );
	}

}
bool CAttrTelTpPointDlg::CheckSamePoint()
{
	CFeatureDef *pCurrentFeatureDef = GetCurrentFeatureDef();
	IFeaturePtr ipCurrentFeature = pCurrentFeatureDef->GetFeature();
	if( !ipCurrentFeature )
		return false;

	long lCurrentOID = 0;
	ipCurrentFeature->get_OID( &lCurrentOID );
	ITablePtr ipCurrentTable;
	ipCurrentFeature->get_Table( &ipCurrentTable );
	IGeometryPtr ipOrgGeom;
	ipCurrentFeature->get_Shape( &ipOrgGeom );
	IRelationalOperatorPtr ipRel( ipOrgGeom );
	for( std::list<CFeatureDef>::iterator it = m_pFeatureDefList->begin(); it != m_pFeatureDefList->end(); it++ )
	{
		IFeaturePtr temp = it->GetFeature();
		if( !temp )
			continue;
		LONG lOID = 0;
		temp->get_OID( &lOID );
		ITablePtr ipTable;
		temp->get_Table( &ipTable );
		if( ipTable != ipCurrentTable )
			continue;
		if(lOID == lCurrentOID)
			continue;

		// �����܂ł��ď��߂ă`�F�b�N�Ώ�
		IGeometryPtr ipTempGeom;
		temp->get_Shape( &ipTempGeom );
		VARIANT_BOOL vaEqual;
		ipRel->Equals( ipTempGeom, &vaEqual );
		// �������W�łȂ�������I��
		if( vaEqual == VARIANT_FALSE )
			return false;
	}
	return true;
}

void CAttrTelTpPointDlg::CopyOtherFeature(CString strCopyFieldName)
{
	// �Ƃ肠�����A���ݕێ����Ă��郊�X�g���ׂĂɂ����ă��[�v���s��
	CFeatureDef *pCurrentFeatureDef = GetCurrentFeatureDef();
	IFeaturePtr ipCurrentFeature = pCurrentFeatureDef->GetFeature();
	if( !ipCurrentFeature )
		return;

	long lCurrentOID = 0;
	ipCurrentFeature->get_OID( &lCurrentOID );
	ITablePtr ipCurrentTable;
	ipCurrentFeature->get_Table( &ipCurrentTable );
	IGeometryPtr ipOrgGeom;
	ipCurrentFeature->get_Shape( &ipOrgGeom );
	IRelationalOperatorPtr ipRel( ipOrgGeom );

	for( std::list<CFeatureDef>::iterator it = m_pFeatureDefList->begin(); it != m_pFeatureDefList->end(); it++ )
	{
		IFeaturePtr temp = it->GetFeature();
		if( !temp )
			continue;
		LONG lOID;
		temp->get_OID( &lOID );
		ITablePtr ipTable;
		temp->get_Table( &ipTable );
		if( ipTable != ipCurrentTable )
			continue;
		if(lOID == lCurrentOID)
			continue;
		IGeometryPtr ipTempGeom;
		temp->get_Shape( &ipTempGeom );
		VARIANT_BOOL vaEqual;
		ipRel->Equals( ipTempGeom, &vaEqual );
		if( vaEqual == VARIANT_FALSE )
			continue;

		CFeatureDef *pFeatureDef = &(*it);
		if( pFeatureDef != NULL )
		{
			// ���ׂẴt�B�[���h�l���`�F�b�N
			for( std::list<CFieldDef>::iterator itField = GetCurrentFeatureDef()->begin(); itField != GetCurrentFeatureDef()->end(); itField++ )
			{
				CString strFieldName = itField->m_strFieldName;

				// �ȉ�if���̒��g�̓R�s�[���Ă��܂��Ă͂����Ȃ�����
				if( strFieldName == strCopyFieldName)
				{
					// ��L��if���̏����ȊO�̓R�s�[����
					CFieldDef *pFieldDef;
					pFieldDef = pFeatureDef->GetFieldDef( _T("TELTP_POINT"), itField->m_strFieldName );
					VARIANT vaValue;
					pFeatureDef->GetFeature()->get_Value( itField->m_lFieldIndex, &vaValue );
					if( itField->m_vaValue != vaValue )
					{
						pFieldDef->m_vaValue = itField->m_vaValue;
						pFieldDef->m_bChanged = TRUE;
					}
					else{
						pFieldDef->m_vaValue = itField->m_vaValue;
						pFieldDef->m_bChanged = FALSE;
					}
				}
			}
		}
	}
}
