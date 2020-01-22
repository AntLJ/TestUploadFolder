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

// LQAttrGuideDlg.cpp : CLQAttrGuideDlg �̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LQAttrGuideDlg.h"

/// �E�B���h�E�}�l�[�W���z�u�}�b�v�i�����U���p�j
BEGIN_WINDOW_MAP(GUIDE_MAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(0,0))
		RCSPACE(20)
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,10))
			RCTOFIT(IDC_STATIC_GUIDECLASS)
			RCREST(IDC_COMBO_GUIDECLASS)
		ENDGROUP()
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
 * @retval CLQAttrGuideDlg��Ԃ��܂��B
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CLQAttrGuideDlg;
}

/////////////////////////////////////////////////////////////////////////////
//
// CLQAttrGuideDlg �����o�֐�
//
/////////////////////////////////////////////////////////////////////////////
CLQAttrGuideDlg::CLQAttrGuideDlg() 
	: LQATTR_COMMON_BASE_CLASS(GUIDE_MAP, IDD_LQATTRGUIDEDLG),
	m_wndGuideClass( _T("COMBOBOX"), this, IDC_COMBO_GUIDECLASS ),
	m_wndSource( _T("EDIT"), this, IDC_EDIT_SOURCE )
{
}

/** 
 * @fn HWND CLQAttrGuideDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief �q�_�C�A���O���쐬���܂��B
 *
 * LQATTR_COMMON_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)�Ń_�C�A���O���쐬���܂��B
 *
 * @param hWndParent	[in]	�e�E�B���h�E�n���h��
 * @param dwInitParam	[in]	�_�C�A���O�������p�����[�^�i���ݓ��Ɏg�p���Ă��܂���j
 *
 * @retval �쐬���ꂽ�q�_�C�A���O�̃n���h����Ԃ��܂��B
 */
HWND CLQAttrGuideDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{
	// �_�C�A���O�̍쐬��LQATTR_COMMON_BASE_CLASS�ɔC���܂�
	m_hChildWnd = LQATTR_COMMON_BASE_CLASS::Create(hWndParent,dwInitParam);
	return m_hChildWnd;
}

/**
 * @fn void CLQAttrGuideDlg::Delete()
 * @brief �쐬�����q�_�C�A���O���폜���܂��B
 *
 * �_�C�A���O���폜����Ƃ��ɕK���Ă�ŉ������B�Ă΂Ȃ��ƃ��������[�N���Ă��܂��܂��B
 */
void CLQAttrGuideDlg::Delete()
{
	::DestroyWindow(m_hChildWnd);
    delete this;
}

/**
 * @fn BOOL CLQAttrGuideDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
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
BOOL CLQAttrGuideDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
    BOOL bRet = LQATTR_COMMON_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
		::SetFocus( GetDlgItem(IDC_COMBO_GUIDECLASS) );
	else
		::SendMessage( GetDlgItem(IDC_COMBO_GUIDECLASS), WM_KILLFOCUS, NULL, NULL );

    return bRet;
}

/**
 * @fn void CLQAttrGuideDlg::CreateControlRelation()
 * @brief �q�_�C�A���O�����ۂɍ쐬���܂��B
 *
 * ���ۂ̍쐬��ControlRel�ɔC���܂��B
 */
void CLQAttrGuideDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// �e�E�C���h�E�n���h����ݒ肵�Ȃ���
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		CString strInfoTableName = CLoadQueueInfo::GetLoadQueueInfo( sindyeRoadQueueGuide, sindyeLoadQueueInfoInfoTableName );
		m_cControlRel.SetHWND( m_hWnd );
		// �����U���p
		m_cControlRel.SetControl( IDC_COMBO_GUIDECLASS,	TYPE_COMBO,			0, 0,	_T("GUIDETYPE_C"),	NULL, strInfoTableName );
		m_cControlRel.SetControl( IDC_EDIT_SOURCE,		TYPE_EDITSTRING,	0, 0,	_T("SOURCE"),		NULL, strInfoTableName );
	}
}


BOOL CLQAttrGuideDlg::ErrorCheck()
{
	BOOL bRet = TRUE;
	CString strInfTableName = m_cLQInfo.GetLoadQueueInfo( sindyeRoadQueueGuide, sindyeLoadQueueInfoInfoTableName );
	CString strMsg;
	for( std::list<CLQRowDef>::iterator it = m_pFeatureDefList->begin(); it != m_pFeatureDefList->end(); it++ )
	{
		if( strInfTableName.CompareNoCase( it->GetTableName() ) == 0 )
		{
			LONG lOID;
			CFieldDef* pOIDFieldDef = it->GetFieldDef( strInfTableName, _T("OBJECTID"));
			lOID = pOIDFieldDef->m_vaValue.lVal;

			CFieldDef* pGuideFieldDef = it->GetFieldDef( strInfTableName, _T("GUIDETYPE_C") );

			if( !pGuideFieldDef )
			{
				_ASSERTE( pGuideFieldDef != NULL );
				continue;
			}

			CString strMsg;
			// �ē���ʃR�[�h�� NULL �������Ă��Ȃ����ǂ���
			if( pGuideFieldDef->m_vaValue.lVal == 0 )
			{
				strMsg.Format( AheLoadString( IDS_ERRORGUIDE ), strInfTableName, lOID);
				AttrMessageBox( strMsg, AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_HIGH );

				bRet = FALSE;
				break;
			}
		}
	}
	
	return bRet;
}
