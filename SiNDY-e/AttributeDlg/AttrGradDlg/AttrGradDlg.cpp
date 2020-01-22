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

// AttrGradDlg.cpp: CAttrGradDlg �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AttrGradDlg.h"

/// �E�B���h�E�}�l�[�W���z�u�}�b�v�i�X�Ηp�j
BEGIN_WINDOW_MAP(GRADMAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(2,2))
		BEGINROWS(WRCT_PCT,0,90)
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(0,0))
				RCTOFIT(IDC_GRAD_STATIC1)
				RCTOFIT(-1)
				RCREST(IDC_GRAD_EDIT1)
			ENDGROUP()
		ENDGROUP()
		BEGINROWS(WRCT_PCT,0,10)
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
 * @retval CAttrGradDlg��Ԃ��܂��B
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CAttrGradDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrGradDlg �����o�֐�
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrGradDlg::CAttrGradDlg()
 * @brief �R���X�g���N�^
 *
 * �u���V�̏����������Ă����܂��B
 */
CAttrGradDlg::CAttrGradDlg() 
	: ATTR_BASE_CLASS(GRADMAP, IDD_ATTRGRADDLG),
		m_wnd0( _T("EDIT"), this, IDC_GRAD_EDIT1 ),
		m_wnd1( _T("EDIT"), this, IDC_EDIT_SOURCE )
{
}

/** 
 * @fn HWND CAttrGradDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief �q�_�C�A���O���쐬���܂��B
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)�Ń_�C�A���O���쐬���܂��B
 *
 * @param hWndParent	[in]	�e�E�B���h�E�n���h��
 * @param dwInitParam	[in]	�_�C�A���O�������p�����[�^�i���ݓ��Ɏg�p���Ă��܂���j
 *
 * @retval �쐬���ꂽ�q�_�C�A���O�̃n���h����Ԃ��܂��B
 */
HWND CAttrGradDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	m_hChildWnd = ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
	return m_hChildWnd;
}

/**
 * @fn void CAttrGradDlg::Delete()
 * @brief �쐬�����q�_�C�A���O���폜���܂��B
 *
 * �_�C�A���O���폜����Ƃ��ɕK���Ă�ŉ������B�Ă΂Ȃ��ƃ��������[�N���Ă��܂��܂��B
 */
void CAttrGradDlg::Delete()
{
	ClearFeatureDefs();
	::DestroyWindow(m_hChildWnd);
    delete this; // �K��new�Ŋm�ۂ���O��
}

/**
 * @fn BOOL CAttrGradDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
 * @brief ���ݕ\�������ׂ��t�B�[�`����FeatureDef�C���f�b�N�X��n���܂��B
 * ����ɁA�t�B�[�`���̑����Ƃ͊֌W�Ȃ��R���g���[���̐�������Ă����܂��B
 * �_�C�A���O���J�����Ƃ��ɍŏ��Ƀt�H�[�J�X�̓������Ă��ė~�����R���g���[���̐ݒ�����Ă����܂��B
 * 2�����b�V�����ׂ��Őڂ��Ă���X�΂��I������Ă���ꍇ�ɂ́A�֘A����X�΂��m�ۂ���
 *
 * @param lFeatureIndex	[in]	�t�B�[�`����`�N���X�̃C���f�b�N�X�ԍ�
 * @param lTableIndex	[in]	�e�[�u����`�N���X�̃C���f�b�N�X�ԍ�
 * @param lRowIndex		[in]	���R�[�h��`�N���X�̃C���f�b�N�X�ԍ�
 * @param bForce		[in]	�����I�ɕҏW���邩�ǂ���
 * @param bEditable		[in]	�ҏW�\���ǂ���
 *
 * @retval �Ӗ����Ȃ��iTRUE�����Ԃ��Ȃ��j
 */
BOOL CAttrGradDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
    BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	// 2�����b�V�����E�̂��ߒǉ�
	m_pGradOnMeshList.clear();

	// 2�����b�V�����E�ׂ��ł��邩�ǂ����t���O�̏�����
	m_bIsRoadOnMesh = FALSE;

	IFeaturePtr	ipFeature = GetCurrentFeatureDef()->GetFeature();

	CTargetItems cTargetItems;
	cTargetItems.clear();

	// 2�����b�V�����ׂ��ł��邩�ǂ������`�F�b�N���A�ׂ��ł���ΐڑ����Ă���X�΂��擾
	if( AheIsFeatureCrossMesh( m_cArcHelper.GetMap(), ipFeature, cTargetItems ) )
	{
		// 2�����b�V�����ׂ��ł���̂Ńt���O�ύX
		m_bIsRoadOnMesh = TRUE;

		// �I�����ꂽ�A2�����b�V���֘A�̓��H��o�^����
		for( CTargetItems::iterator it = cTargetItems.begin(); it != cTargetItems.end(); it++ )
		{
			IFeaturePtr ipFeatureOnMesh = it->GetFeature();
			CFeatureDef featDef;
			featDef.SetArcHelper( m_cArcHelper.GetApp() );
			featDef.SetFeature( ipFeatureOnMesh );
			m_pGradOnMeshList.push_back(featDef);
		}
	}

	// ���ڃt�B�[�`���Ɋ֌W�Ȃ��G�f�B�b�g�{�b�N�X(��F�Q�Ɨp�̘H�����V�[�P���X�⌴�e�ԍ�����)�Ȃǂ𐧌䂷��
	// �ҏW�J�n�O�ƕҏW�J�n��ł��ҏW�s�̂Ƃ���DISABLE�ɂ���
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		// �ҏW�J�n����Ă���Ƃ��ɂ��łɁASetFocus���Ă���
		::SetFocus( GetDlgItem(IDC_GRAD_EDIT1) );
	}

    return bRet;
}

/**
 * @fn void CAttrGradDlg::CreateControlRelation()
 * @brief �q�_�C�A���O�����ۂɍ쐬���܂��B
 *
 * ���ۂ̍쐬��ControlRel�ɔC���܂��B
 */
void CAttrGradDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// �e�E�C���h�E�n���h����ݒ肵�Ȃ���
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );
		// �X�Ηp

		// �X�Βl
		m_cControlRel.SetControl( IDC_GRAD_EDIT1,		TYPE_EDIT,		0,	0,	_T("GRAD_VALUE"),	NULL, GRAD_TABLE_NAME );
		// ���\�[�X
		m_cControlRel.SetControl( IDC_EDIT_SOURCE,		TYPE_EDITSTRING,0,	0,	_T("SOURCE"),		NULL, GRAD_TABLE_NAME );
	}
}

/**
 * @fn BOOL CAttrGradDlg::CheckGradValue()
 * @brief �X�Βl���������Z�b�g����Ă��邩�`�F�b�N
 *
 * �X�Βl��-40,-30,-20,-10,0,10,20,30,40�̂����ꂩ�łȂ��Ƃ����܂���
 *
 * @return	�������X�Βl�ł�������TRUE, �s���ȌX�Βl�ł�������FALSE 
 */
BOOL CAttrGradDlg::CheckGradValue()
{
	CEdit	cEditGrad( GetDlgItem(IDC_GRAD_EDIT1) );
	int		iLineLength = cEditGrad.LineLength();

	LPTSTR lpszGradValue = (LPTSTR)_alloca( ( iLineLength + 1 ) * sizeof(int) );
	cEditGrad.GetWindowText( lpszGradValue, iLineLength + 1 );

	LONG lGradValue = _ttol( lpszGradValue );

	CFeatureDef	*pFeatureDef;
	pFeatureDef = GetCurrentFeatureDef();
	
	if( lGradValue == -40 || lGradValue == -30 || lGradValue == -20 || lGradValue == -10 || lGradValue == 0 ||
		lGradValue == 10 || lGradValue == 20 || lGradValue == 30 || lGradValue == 40)
	{
		pFeatureDef->m_bError = FALSE;
		return TRUE;
	}

	m_bErrorCheck = TRUE;

	AttrMessageBox( AheLoadString( IDS_GRADRANGE ), AheLoadString( IDS_CAPTION ), MB_OK|MB_TASKMODAL, WARNLEVEL_HIGH );

	m_bErrorCheck = FALSE;

	pFeatureDef->m_bError = TRUE;
	return FALSE;
}

/**
 * @fn BOOL CAttrGradDlg::CheckReturnKeyItem(INT nClassID)
 * @brief ���^�[���L�[�������ꂽ���̓���
 *
 * @param	INT		nClassID	[i]		���^�[���L�[�������ꂽ����ItemID
 *
 * @return	TRUE : ���^�[���L�[�Ƃ��Ă̓���(�K�p����)�� , FALSE : ���^�[���L�[�Ƃ��Ă̓���s�� 
 */
BOOL CAttrGradDlg::CheckReturnKeyItem(INT nClassID)
{
	// 2�����b�V�����E�����ׂ��ł��铹�H�̏ꍇ�͔��Α��̓��H�����������ɂ��Ȃ��Ă͂����Ȃ�����
	// ���̃R�s�[�����͂�邵���Ȃ��̂ł���������Ă͐�΂ɂ����Ȃ��I
	CopyMeshGrad();

	if( !CheckGradValue() )
		return FALSE;
	return TRUE;
}
void CAttrGradDlg::CopyMeshGrad()
{
	// 2�����b�V�����E�����ׂ��ł��铹�H�̏ꍇ�͔��Α��̓��H�����������ɂ��Ȃ��Ă͂����Ȃ�����
	// �ȉ��̏������s��
	if( m_bIsRoadOnMesh )
	{
		// �Ƃ肠�����A���ݕێ����Ă��郊�X�g���ׂĂɂ����ă��[�v���s��
		for( std::list<CFeatureDef>::iterator it = m_pFeatureDefList->begin(); it != m_pFeatureDefList->end(); it++ )
		{
			// ���ݑI�𒆂�2�����b�V�����ׂ��ł��铹�H�����N�Ɋ֘A���Ă��郊�X�g�Ƃ̏ƍ�
			CFeatureDef *pFeatureDef = NULL;
			for( std::list<CFeatureDef>::iterator iAttr = m_pGradOnMeshList.begin(); iAttr != m_pGradOnMeshList.end(); iAttr++ )
			{
				if( iAttr->GetFeature() == it->GetFeature() )
				{
					pFeatureDef = &(*it);
					break;
				}
			}

			if( pFeatureDef )
			{
				// ���ׂẴt�B�[���h�l���`�F�b�N
				for( std::list<CFieldDef>::iterator itField = GetCurrentFeatureDef()->begin(); itField != GetCurrentFeatureDef()->end(); itField++ )
				{
					CString strFieldName = itField->m_strFieldName;

					// �ȉ����e���R�s�[����
					if( strFieldName == _T("GRAD_VALUE") || strFieldName == _T("SOURCE") )
					{
						// ��L��if���̏����ȊO�̓R�s�[����
						CFieldDef *pFieldDef;
						pFieldDef = pFeatureDef->GetFieldDef( GRAD_TABLE_NAME, itField->m_strFieldName );
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
}
/**
 * @fn BOOL CAttrGradDlg::CheckEscKeyItem(INT nClassID)
 * @brief ESC�L�[�������ꂽ���̓���
 *
 * m_bErrorCheck��TRUE�ɂ��Ă����Ȃ��ƁAESC�L�[�������ă_�C�A���O�����Ƃ��ɁA�G���[�`�F�b�N�������Ă��܂����߂ɂ����Ő���
 * 
 * @param	nClassID	[in]		ESC�L�[�������ꂽ����ItemID
 *
 * @return	�K��TRUE���Ԃ�i�Ӗ��̂Ȃ��l�j
 */
BOOL CAttrGradDlg::CheckEscKeyItem(INT nClassID)
{
	m_bErrorCheck = TRUE;	
	return TRUE;
}

/**
 * @fn BOOL CAttrRoadLinkDlg::ErrorCheck()
 * @brief �_�C�A���O��������Ƃ��ɓ��H�����N�̃G���[�`�F�b�N���s��
 *
 * @return �G���[�łȂ������� TRUE�A�G���[�������� FALSE
 */
BOOL CAttrGradDlg::ErrorCheck()
{
	for( std::list<CFeatureDef>::iterator it = m_pFeatureDefList->begin(); it != m_pFeatureDefList->end(); it++ )
	{
		CString strTableName = it->GetTableName();

		if( strTableName == GRAD_TABLE_NAME )
		{
			// ���ݕҏW���̒l���K��
			CFieldDef* pFieldDef;
			CComVariant vaGradValue;
			pFieldDef = it->GetFieldDef( GRAD_TABLE_NAME, _T("GRAD_VALUE") );

			LONG lGradValue = pFieldDef->m_vaValue.lVal;

			// �t�B�[�`���ƃI�u�W�F�N�gID�擾
			IFeaturePtr ipFeature(it->GetFeature());
			LONG lOID = 0;
			ipFeature->get_OID( &lOID );

			if( !(lGradValue == -40 || lGradValue == -30 || lGradValue == -20 || lGradValue == -10 || lGradValue == 0 ||
				lGradValue == 10 || lGradValue == 20 || lGradValue == 30 || lGradValue == 40) )
			{
				CString strMessage;
				strMessage.Format( AheLoadString( IDS_GRADRANGE2 ) ,lOID );
				AttrMessageBox( strMessage,  AheLoadString( IDS_CAPTION ), MB_OK|MB_TASKMODAL, WARNLEVEL_HIGH );
				return FALSE;
			}
		}
	}
	
	return TRUE;
}
