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

// LQAttrTurnRegDlg.cpp : CLQAttrTurnRegDlg �̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LQAttrTurnRegDlg.h"

/// �E�B���h�E�}�l�[�W���z�u�}�b�v�i�E���܋֎~�p�j
BEGIN_WINDOW_MAP(TURNREG_MAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(0,0))
		RCSPACE(20)
		BEGINROWS(WRCT_TOFIT, 0, RCMARGINS(0,2))
			BEGINCOLS(WRCT_TOFIT, IDC_STATIC_TURNREG1, RCMARGINS(4,8))
				BEGINROWS(WRCT_TOFIT, 0, RCMARGINS(0,2))
					BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(0,2))
						RCTOFIT(IDC_BUTTON_TUUNEN1)
						RCTOFIT(IDC_BUTTON_SYUJITU1)
						RCTOFIT(IDC_CHECK_ZEROGATU1)
						RCTOFIT(IDC_CHECK1)
						RCSPACE(100)
						RCTOFIT(IDC_BUTTON_DELETE1)				
					ENDGROUP()
					BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(0,2))
						RCTOFIT(IDC_EDIT_TURNREGSMON1)
						RCTOFIT(IDC_STATIC_TURNREGSMON1)
						RCTOFIT(IDC_EDIT_TURNREGSDAY1)
						RCTOFIT(IDC_STATIC_TURNREGSDAY1)
						RCTOFIT(IDC_STATIC_TURNREGMD1)
						RCTOFIT(IDC_EDIT_TURNREGEMON1)
						RCTOFIT(IDC_STATIC_TURNREGEMON1)
						RCTOFIT(IDC_EDIT_TURNREGEDAY1)
						RCTOFIT(IDC_STATIC_TURNREGEDAY1)
						RCSPACE(4)
						RCTOFIT(IDC_EDIT_TURNREGSHOUR1)
						RCTOFIT(IDC_STATIC_TURNREGSHM1)
						RCTOFIT(IDC_EDIT_TURNREGSMIN1)
						RCTOFIT(IDC_STATIC_TURNREGHM1)
						RCTOFIT(IDC_EDIT_TURNREGEHOUR1)
						RCTOFIT(IDC_STATIC_TURNREGEHM1)
						RCTOFIT(IDC_EDIT_TURNREGEMIN1)
						RCREST(IDC_COMBO_TURNREGW1)
					ENDGROUP()
				ENDGROUP()
			ENDGROUP()
			BEGINCOLS(WRCT_TOFIT, IDC_STATIC_TURNREG2, RCMARGINS(4,8))
				BEGINROWS(WRCT_TOFIT, 0, RCMARGINS(0,2))
					BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(0,2))
						RCTOFIT(IDC_BUTTON_TUUNEN2)
						RCTOFIT(IDC_BUTTON_SYUJITU2)
						RCTOFIT(IDC_CHECK_ZEROGATU2)
						RCSPACE(100)
						RCTOFIT(IDC_BUTTON_DELETE2)	
					ENDGROUP()
					BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(0,2))
						RCTOFIT(IDC_EDIT_TURNREGSMON2)
						RCTOFIT(IDC_STATIC_TURNREGSMON2)
						RCTOFIT(IDC_EDIT_TURNREGSDAY2)
						RCTOFIT(IDC_STATIC_TURNREGSDAY2)
						RCTOFIT(IDC_STATIC_TURNREGMD2)
						RCTOFIT(IDC_EDIT_TURNREGEMON2)
						RCTOFIT(IDC_STATIC_TURNREGEMON2)
						RCTOFIT(IDC_EDIT_TURNREGEDAY2)
						RCTOFIT(IDC_STATIC_TURNREGEDAY2)
						RCSPACE(4)
						RCTOFIT(IDC_EDIT_TURNREGSHOUR2)
						RCTOFIT(IDC_STATIC_TURNREGSHM2)
						RCTOFIT(IDC_EDIT_TURNREGSMIN2)
						RCTOFIT(IDC_STATIC_TURNREGHM2)
						RCTOFIT(IDC_EDIT_TURNREGEHOUR2)
						RCTOFIT(IDC_STATIC_TURNREGEHM2)
						RCTOFIT(IDC_EDIT_TURNREGEMIN2)
						RCREST(IDC_COMBO_TURNREGW2)
					ENDGROUP()
				ENDGROUP()
			ENDGROUP()
			BEGINCOLS(WRCT_TOFIT, IDC_STATIC_TURNREG3, RCMARGINS(4,8))
				BEGINROWS(WRCT_TOFIT, 0, RCMARGINS(0,2))
					BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(0,2))
						RCTOFIT(IDC_BUTTON_TUUNEN3)
						RCTOFIT(IDC_BUTTON_SYUJITU3)
						RCTOFIT(IDC_CHECK_ZEROGATU3)
						RCSPACE(100)
						RCTOFIT(IDC_BUTTON_DELETE3)	
					ENDGROUP()
					BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(0,2))
						RCTOFIT(IDC_EDIT_TURNREGSMON3)
						RCTOFIT(IDC_STATIC_TURNREGSMON3)
						RCTOFIT(IDC_EDIT_TURNREGSDAY3)
						RCTOFIT(IDC_STATIC_TURNREGSDAY3)
						RCTOFIT(IDC_STATIC_TURNREGMD3)
						RCTOFIT(IDC_EDIT_TURNREGEMON3)
						RCTOFIT(IDC_STATIC_TURNREGEMON3)
						RCTOFIT(IDC_EDIT_TURNREGEDAY3)
						RCTOFIT(IDC_STATIC_TURNREGEDAY3)
						RCSPACE(4)
						RCTOFIT(IDC_EDIT_TURNREGSHOUR3)
						RCTOFIT(IDC_STATIC_TURNREGSHM3)
						RCTOFIT(IDC_EDIT_TURNREGSMIN3)
						RCTOFIT(IDC_STATIC_TURNREGHM3)
						RCTOFIT(IDC_EDIT_TURNREGEHOUR3)
						RCTOFIT(IDC_STATIC_TURNREGEHM3)
						RCTOFIT(IDC_EDIT_TURNREGEMIN3)
						RCREST(IDC_COMBO_TURNREGW3)
					ENDGROUP()
				ENDGROUP()
			ENDGROUP()
			BEGINCOLS(WRCT_TOFIT, IDC_STATIC_TURNREG4, RCMARGINS(4,8))
				BEGINROWS(WRCT_TOFIT, 0, RCMARGINS(0,2))
					BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(0,2))
						RCTOFIT(IDC_BUTTON_TUUNEN4)
						RCTOFIT(IDC_BUTTON_SYUJITU4)	
						RCTOFIT(IDC_CHECK_ZEROGATU4)
						RCSPACE(100)
						RCTOFIT(IDC_BUTTON_DELETE4)	
					ENDGROUP()
					BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(0,2))
						RCTOFIT(IDC_EDIT_TURNREGSMON4)
						RCTOFIT(IDC_STATIC_TURNREGSMON4)
						RCTOFIT(IDC_EDIT_TURNREGSDAY4)
						RCTOFIT(IDC_STATIC_TURNREGSDAY4)
						RCTOFIT(IDC_STATIC_TURNREGMD4)
						RCTOFIT(IDC_EDIT_TURNREGEMON4)
						RCTOFIT(IDC_STATIC_TURNREGEMON4)
						RCTOFIT(IDC_EDIT_TURNREGEDAY4)
						RCTOFIT(IDC_STATIC_TURNREGEDAY4)
						RCSPACE(4)
						RCTOFIT(IDC_EDIT_TURNREGSHOUR4)
						RCTOFIT(IDC_STATIC_TURNREGSHM4)
						RCTOFIT(IDC_EDIT_TURNREGSMIN4)
						RCTOFIT(IDC_STATIC_TURNREGHM4)
						RCTOFIT(IDC_EDIT_TURNREGEHOUR4)
						RCTOFIT(IDC_STATIC_TURNREGEHM4)
						RCTOFIT(IDC_EDIT_TURNREGEMIN4)
						RCREST(IDC_COMBO_TURNREGW4)
					ENDGROUP()
				ENDGROUP()
			ENDGROUP()
		ENDGROUP()
		RCREST(-1)
		RCTOFIT(IDC_STATIC_SOURCESEP)
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(0,2))
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
 * @retval CLQAttrTurnRegDlg��Ԃ��܂��B
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CLQAttrTurnRegDlg;
}

/////////////////////////////////////////////////////////////////////////////
//
// CLQAttrTurnRegDlg �����o�֐�
//
/////////////////////////////////////////////////////////////////////////////
CLQAttrTurnRegDlg::CLQAttrTurnRegDlg() 
	: LQATTR_COMMON_BASE_CLASS(TURNREG_MAP, IDD_LQATTRTURNREGDLG),
	m_wndSMonth1( _T("EDIT"), this, IDC_EDIT_TURNREGSMON1 ),
	m_wndSMonth2( _T("EDIT"), this, IDC_EDIT_TURNREGSMON2 ),
	m_wndSMonth3( _T("EDIT"), this, IDC_EDIT_TURNREGSMON3 ),
	m_wndSMonth4( _T("EDIT"), this, IDC_EDIT_TURNREGSMON4 ),
	m_wndEMonth1( _T("EDIT"), this, IDC_EDIT_TURNREGEMON1 ),
	m_wndEMonth2( _T("EDIT"), this, IDC_EDIT_TURNREGEMON2 ),
	m_wndEMonth3( _T("EDIT"), this, IDC_EDIT_TURNREGEMON3 ),
	m_wndEMonth4( _T("EDIT"), this, IDC_EDIT_TURNREGEMON4 ),
	m_wndSDay1( _T("EDIT"), this, IDC_EDIT_TURNREGSDAY1 ),
	m_wndSDay2( _T("EDIT"), this, IDC_EDIT_TURNREGSDAY2 ),
	m_wndSDay3( _T("EDIT"), this, IDC_EDIT_TURNREGSDAY3 ),
	m_wndSDay4( _T("EDIT"), this, IDC_EDIT_TURNREGSDAY4 ),
	m_wndEDay1( _T("EDIT"), this, IDC_EDIT_TURNREGEDAY1 ),
	m_wndEDay2( _T("EDIT"), this, IDC_EDIT_TURNREGEDAY2 ),
	m_wndEDay3( _T("EDIT"), this, IDC_EDIT_TURNREGEDAY3 ),
	m_wndEDay4( _T("EDIT"), this, IDC_EDIT_TURNREGEDAY4 ),
	m_wndSHour1( _T("EDIT"), this, IDC_EDIT_TURNREGSHOUR1 ),
	m_wndSHour2( _T("EDIT"), this, IDC_EDIT_TURNREGSHOUR2 ),
	m_wndSHour3( _T("EDIT"), this, IDC_EDIT_TURNREGSHOUR3 ),
	m_wndSHour4( _T("EDIT"), this, IDC_EDIT_TURNREGSHOUR4 ),
	m_wndEHour1( _T("EDIT"), this, IDC_EDIT_TURNREGEHOUR1 ),
	m_wndEHour2( _T("EDIT"), this, IDC_EDIT_TURNREGEHOUR2 ),
	m_wndEHour3( _T("EDIT"), this, IDC_EDIT_TURNREGEHOUR3 ),
	m_wndEHour4( _T("EDIT"), this, IDC_EDIT_TURNREGEHOUR4 ),
	m_wndSMin1( _T("EDIT"), this, IDC_EDIT_TURNREGSMIN1 ),
	m_wndSMin2( _T("EDIT"), this, IDC_EDIT_TURNREGSMIN2 ),
	m_wndSMin3( _T("EDIT"), this, IDC_EDIT_TURNREGSMIN3 ),
	m_wndSMin4( _T("EDIT"), this, IDC_EDIT_TURNREGSMIN4 ),
	m_wndEMin1( _T("EDIT"), this, IDC_EDIT_TURNREGEMIN1 ),
	m_wndEMin2( _T("EDIT"), this, IDC_EDIT_TURNREGEMIN2 ),
	m_wndEMin3( _T("EDIT"), this, IDC_EDIT_TURNREGEMIN3 ),
	m_wndEMin4( _T("EDIT"), this, IDC_EDIT_TURNREGEMIN4 ),
	m_wndWeek1( _T("COMBOBOX"), this, IDC_COMBO_TURNREGW1 ),
	m_wndWeek2( _T("COMBOBOX"), this, IDC_COMBO_TURNREGW2 ),
	m_wndWeek3( _T("COMBOBOX"), this, IDC_COMBO_TURNREGW3 ),
	m_wndWeek4( _T("COMBOBOX"), this, IDC_COMBO_TURNREGW4 ),
	m_wndTButton1( _T("BUTTON"), this, IDC_BUTTON_TUUNEN1 ),
	m_wndTButton2( _T("BUTTON"), this, IDC_BUTTON_TUUNEN2 ),
	m_wndTButton3( _T("BUTTON"), this, IDC_BUTTON_TUUNEN3 ),
	m_wndTButton4( _T("BUTTON"), this, IDC_BUTTON_TUUNEN4 ),
	m_wndSButton1( _T("BUTTON"), this, IDC_BUTTON_SYUJITU1 ),
	m_wndSButton2( _T("BUTTON"), this, IDC_BUTTON_SYUJITU2 ),
	m_wndSButton3( _T("BUTTON"), this, IDC_BUTTON_SYUJITU3 ),
	m_wndSButton4( _T("BUTTON"), this, IDC_BUTTON_SYUJITU4 ),
	m_wndDButton1( _T("BUTTON"), this, IDC_BUTTON_DELETE1 ),
	m_wndDButton2( _T("BUTTON"), this, IDC_BUTTON_DELETE2 ),
	m_wndDButton3( _T("BUTTON"), this, IDC_BUTTON_DELETE3 ),
	m_wndDButton4( _T("BUTTON"), this, IDC_BUTTON_DELETE4 ),
	m_wndZButton1( _T("BUTTON"), this, IDC_CHECK_ZEROGATU1 ),
	m_wndZButton2( _T("BUTTON"), this, IDC_CHECK_ZEROGATU2),
	m_wndZButton3( _T("BUTTON"), this, IDC_CHECK_ZEROGATU3 ),
	m_wndZButton4( _T("BUTTON"), this, IDC_CHECK_ZEROGATU4 ),
	m_wndSource( _T("EDIT"), this, IDC_EDIT_SOURCE )
{
}

/** 
 * @fn HWND CLQAttrTurnRegDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief �q�_�C�A���O���쐬���܂��B
 *
 * LQATTR_COMMON_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)�Ń_�C�A���O���쐬���܂��B
 *
 * @param hWndParent	[in]	�e�E�B���h�E�n���h��
 * @param dwInitParam	[in]	�_�C�A���O�������p�����[�^�i���ݓ��Ɏg�p���Ă��܂���j
 *
 * @retval �쐬���ꂽ�q�_�C�A���O�̃n���h����Ԃ��܂��B
 */
HWND CLQAttrTurnRegDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{
	// �_�C�A���O�̍쐬��LQATTR_COMMON_BASE_CLASS�ɔC���܂�
	m_hChildWnd = LQATTR_COMMON_BASE_CLASS::Create(hWndParent,dwInitParam);
	return m_hChildWnd;
}

/**
 * @fn void CLQAttrTurnRegDlg::Delete()
 * @brief �쐬�����q�_�C�A���O���폜���܂��B
 *
 * �_�C�A���O���폜����Ƃ��ɕK���Ă�ŉ������B�Ă΂Ȃ��ƃ��������[�N���Ă��܂��܂��B
 */
void CLQAttrTurnRegDlg::Delete()
{
	::DestroyWindow(m_hChildWnd);
    delete this;
}

/**
 * @fn BOOL CLQAttrTurnRegDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
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
BOOL CLQAttrTurnRegDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
	// �u�����v�������ꂽ��ԂŃc���[�őI���t�B�[�`����؂�ւ����ꍇ�A��ԁiToggle = On / Edit = Disable�j
	//  �����Z�b�g����Ȃ��̂Ŗ����I�ɂ���Ă����K�v������B(bug 6292)
	for( int i = KISEIJIKAN1; i <= KISEIJIKAN4; ++i )
	{ 
		CRegulationTime cReg( m_hWnd, (EKiseiNum)i );
		cReg.UnsetZeroGatu();
	}

	BOOL bRet = LQATTR_COMMON_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	// �ŏ��ɕ���0��������00�ɃZ�b�g���Ă���
	for( int i = KISEIJIKAN1; i <= KISEIJIKAN4; ++i )
	{
		CRegulationTime cReg( m_hWnd, (EKiseiNum)i );
		cReg.DisplayZeroZero();
	}

	m_bIsEditable = FALSE;
	//[Bug 8399] �֐�IsEditableFeature��Editable�݂̂𔻒f����̂���
	//�V�K�ł���ꍇ�Am_bIsEditable��TRUE��ݒ肷��K�v������B(�ϐ�bEditable��Editable��Ԃ𔻒f���邱�Ƃ��ł���B)
	//Button��Event�֐��̒��ɁA����Flag�l�ɂ�艞�����邩�ǂ����𔻒f����B
	if(m_cArcHelper.IsStartEditing() && bEditable )
	{
			::SetFocus( GetDlgItem( IDC_EDIT_TURNREGSMON1 ) );
			m_bIsEditable = TRUE;
	}
	else
		::SendMessage( GetDlgItem(IDC_EDIT_TURNREGSMON1), WM_KILLFOCUS, NULL, NULL );

    return bRet;
}

/**
 * @fn void CLQAttrTurnRegDlg::CreateControlRelation()
 * @brief �q�_�C�A���O�����ۂɍ쐬���܂��B
 *
 * ���ۂ̍쐬��ControlRel�ɔC���܂��B
 */
void CLQAttrTurnRegDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// �e�E�C���h�E�n���h����ݒ肵�Ȃ���
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		CString strInfoTableName = CLoadQueueInfo::GetLoadQueueInfo( sindyeRoadQueueTurnreg, sindyeLoadQueueInfoInfoTableName );
		m_cControlRel.SetHWND( m_hWnd );
		// �E���܋֎~�p
		m_cControlRel.SetControl( IDC_EDIT_TURNREGSMON1,	TYPE_EDIT,	0,  0,	_T("START_MONTH1"),		NULL, strInfoTableName  );
		m_cControlRel.SetControl( IDC_EDIT_TURNREGSDAY1,	TYPE_EDIT,	0,  0,	_T("START_DAY1"),		NULL, strInfoTableName  );
		m_cControlRel.SetControl( IDC_EDIT_TURNREGEMON1,	TYPE_EDIT,	0,  0,	_T("END_MONTH1"),		NULL, strInfoTableName  );
		m_cControlRel.SetControl( IDC_EDIT_TURNREGEDAY1,	TYPE_EDIT,	0,  0,	_T("END_DAY1"),			NULL, strInfoTableName  );
		m_cControlRel.SetControl( IDC_EDIT_TURNREGSHOUR1,	TYPE_EDIT,	0,  0,	_T("START_HOUR1"),		NULL, strInfoTableName  );
		m_cControlRel.SetControl( IDC_EDIT_TURNREGSMIN1,	TYPE_EDIT,	0,  0,	_T("START_MIN1"),		NULL, strInfoTableName  );
		m_cControlRel.SetControl( IDC_EDIT_TURNREGEHOUR1,	TYPE_EDIT,	0,  0,	_T("END_HOUR1"),		NULL, strInfoTableName  );
		m_cControlRel.SetControl( IDC_EDIT_TURNREGEMIN1,	TYPE_EDIT,	0,  0,	_T("END_MIN1"),			NULL, strInfoTableName  );
		m_cControlRel.SetControl( IDC_COMBO_TURNREGW1,		TYPE_COMBO,	0,  0,	_T("DAYOFWEEK1_C"),		NULL, strInfoTableName  );
		m_cControlRel.SetControl( IDC_EDIT_TURNREGSMON2,	TYPE_EDIT,	0,  0,	_T("START_MONTH2"),		NULL, strInfoTableName  );
		m_cControlRel.SetControl( IDC_EDIT_TURNREGSDAY2,	TYPE_EDIT,	0,  0,	_T("START_DAY2"),		NULL, strInfoTableName  );
		m_cControlRel.SetControl( IDC_EDIT_TURNREGEMON2,	TYPE_EDIT,	0,  0,	_T("END_MONTH2"),		NULL, strInfoTableName  );
		m_cControlRel.SetControl( IDC_EDIT_TURNREGEDAY2,	TYPE_EDIT,	0,  0,	_T("END_DAY2"),			NULL, strInfoTableName  );
		m_cControlRel.SetControl( IDC_EDIT_TURNREGSHOUR2,	TYPE_EDIT,	0,  0,	_T("START_HOUR2"),		NULL, strInfoTableName  );
		m_cControlRel.SetControl( IDC_EDIT_TURNREGSMIN2,	TYPE_EDIT,	0,  0,	_T("START_MIN2"),		NULL, strInfoTableName  );
		m_cControlRel.SetControl( IDC_EDIT_TURNREGEHOUR2,	TYPE_EDIT,	0,  0,	_T("END_HOUR2"),		NULL, strInfoTableName  );
		m_cControlRel.SetControl( IDC_EDIT_TURNREGEMIN2,	TYPE_EDIT,	0,  0,	_T("END_MIN2"),			NULL, strInfoTableName  );
		m_cControlRel.SetControl( IDC_COMBO_TURNREGW2,		TYPE_COMBO,	0,  0,	_T("DAYOFWEEK2_C"),		NULL, strInfoTableName  );
		m_cControlRel.SetControl( IDC_EDIT_TURNREGSMON3,	TYPE_EDIT,	0,  0,	_T("START_MONTH3"),		NULL, strInfoTableName  );
		m_cControlRel.SetControl( IDC_EDIT_TURNREGSDAY3,	TYPE_EDIT,	0,  0,	_T("START_DAY3"),		NULL, strInfoTableName  );
		m_cControlRel.SetControl( IDC_EDIT_TURNREGEMON3,	TYPE_EDIT,	0,  0,	_T("END_MONTH3"),		NULL, strInfoTableName  );
		m_cControlRel.SetControl( IDC_EDIT_TURNREGEDAY3,	TYPE_EDIT,	0,  0,	_T("END_DAY3"),			NULL, strInfoTableName  );
		m_cControlRel.SetControl( IDC_EDIT_TURNREGSHOUR3,	TYPE_EDIT,	0,  0,	_T("START_HOUR3"),		NULL, strInfoTableName  );
		m_cControlRel.SetControl( IDC_EDIT_TURNREGSMIN3,	TYPE_EDIT,	0,  0,	_T("START_MIN3"),		NULL, strInfoTableName  );
		m_cControlRel.SetControl( IDC_EDIT_TURNREGEHOUR3,	TYPE_EDIT,	0,  0,	_T("END_HOUR3"),		NULL, strInfoTableName  );
		m_cControlRel.SetControl( IDC_EDIT_TURNREGEMIN3,	TYPE_EDIT,	0,  0,	_T("END_MIN3"),			NULL, strInfoTableName  );
		m_cControlRel.SetControl( IDC_COMBO_TURNREGW3,		TYPE_COMBO,	0,  0,	_T("DAYOFWEEK3_C"),		NULL, strInfoTableName  );
		m_cControlRel.SetControl( IDC_EDIT_TURNREGSMON4,	TYPE_EDIT,	0,  0,	_T("START_MONTH4"),		NULL, strInfoTableName  );
		m_cControlRel.SetControl( IDC_EDIT_TURNREGSDAY4,	TYPE_EDIT,	0,  0,	_T("START_DAY4"),		NULL, strInfoTableName  );
		m_cControlRel.SetControl( IDC_EDIT_TURNREGEMON4,	TYPE_EDIT,	0,  0,	_T("END_MONTH4"),		NULL, strInfoTableName  );
		m_cControlRel.SetControl( IDC_EDIT_TURNREGEDAY4,	TYPE_EDIT,	0,  0,	_T("END_DAY4"),			NULL, strInfoTableName  );
		m_cControlRel.SetControl( IDC_EDIT_TURNREGSHOUR4,	TYPE_EDIT,	0,  0,	_T("START_HOUR4"),		NULL, strInfoTableName  );
		m_cControlRel.SetControl( IDC_EDIT_TURNREGSMIN4,	TYPE_EDIT,	0,  0,	_T("START_MIN4"),		NULL, strInfoTableName  );
		m_cControlRel.SetControl( IDC_EDIT_TURNREGEHOUR4,	TYPE_EDIT,	0,  0,	_T("END_HOUR4"),		NULL, strInfoTableName  );
		m_cControlRel.SetControl( IDC_EDIT_TURNREGEMIN4,	TYPE_EDIT,	0,  0,	_T("END_MIN4"),			NULL, strInfoTableName  );
		m_cControlRel.SetControl( IDC_COMBO_TURNREGW4,		TYPE_COMBO,	0,  0,	_T("DAYOFWEEK4_C"),		NULL, strInfoTableName  );
		m_cControlRel.SetControl( IDC_EDIT_SOURCE,			TYPE_EDITSTRING,	0,	0,	_T("SOURCE"),			NULL, strInfoTableName  );
	}
}

void CLQAttrTurnRegDlg::SetTuunen(INT nControlID)
{
	if( IsStartEditing() && m_bIsEditable )
	{
		CRegulationTime cReg( m_hWnd, CRegulationTime::CtrlID2KiseiNum(nControlID) );
		cReg.UnsetZeroGatu();	// �u�����v��������Ă������������
		cReg.SetRegDate(1, 1, 12, 31);
	}
}

void CLQAttrTurnRegDlg::SetSyujitu(INT nControlID)
{
	if( IsStartEditing() && m_bIsEditable )
	{
		CRegulationTime cReg( m_hWnd, CRegulationTime::CtrlID2KiseiNum(nControlID) );
		cReg.SetRegTime(0, 0, 24, 0);
	}
}

LRESULT CLQAttrTurnRegDlg::OnClickedZerogatu(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( IsStartEditing() && m_bIsEditable )
	{
		CButton cBtn( GetDlgItem(wID) );
		CRegulationTime cReg( m_hWnd, CRegulationTime::CtrlID2KiseiNum(wID) );
		cReg.ToggleZeroGatu( cBtn.GetCheck() );
	}
	bHandled = FALSE;
	return 0;
}

LRESULT CLQAttrTurnRegDlg::OnUpdateRegMonth(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	CRegulationTime cReg( m_hWnd, CRegulationTime::CtrlID2KiseiNum(wID) );
	CString strErr;
	if( !cReg.CheckZeroGatu( hWndCtl, strErr, m_bFinishInit ) )
		AttrMessageBox( strErr, AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_HIGH );
	bHandled = FALSE;
	return 0;
}

void CLQAttrTurnRegDlg::DeleteReg(INT nControlID)
{
	if( IsStartEditing() && m_bIsEditable )
	{
		// �Ԃ��폜���ꂽ��l�߂�B3�Ԗڂ܂łȂ玟�̋K�����Ԃ��R�s�[���A4�Ԗڂ͕K���폜�����B
		// �K�����Ԃ�4�����ł����ʂɋ󗓂��R�s�[����邪�A�F�X�l����Ɩʓ|�Ȃ��ƂɂȂ�̂ŁA�A�A
		for( int i = CRegulationTime::CtrlID2KiseiNum(nControlID); i <= KISEIJIKAN3; ++i )
		{
			CRegulationTime cRegCur( m_hWnd, (EKiseiNum)i ), cRegNext( m_hWnd, (EKiseiNum)(i+1));
			cRegCur.UnsetZeroGatu();
			cRegCur.SetRegTime( cRegNext.GetRegTime() );
		}
		CRegulationTime cRegLast( m_hWnd, KISEIJIKAN4 );
		cRegLast.Clear();
	}
}

BOOL CLQAttrTurnRegDlg::ErrorCheck()
{
	BOOL bRet = TRUE;
	CString strInfTableName = m_cLQInfo.GetLoadQueueInfo( sindyeRoadQueueTurnreg, sindyeLoadQueueInfoInfoTableName );

	CString strMsg;

	for( std::list<CLQRowDef>::iterator it = m_pFeatureDefList->begin(); it != m_pFeatureDefList->end(); it++ )
	{
		if( strInfTableName.CompareNoCase( it->GetTableName() ) == 0 )
		{
			LONG lOID;
			CFieldDef* pOIDFieldDef = it->GetFieldDef( strInfTableName, _T("OBJECTID"));
			lOID = pOIDFieldDef->m_vaValue.lVal;

			std::vector<TIME_REG_DATA> vecTimeRegData;
			std::vector<TIME_REG_DATA> vecConnectTimeRegData;
			TIME_REG_DATA tRow;
			TIME_REG_DATA tConnectRow;

			// �����̂̂P�`�S�܂ł�����ׂ�
			CString strStartMonth,strStartDay,strStartHour,strStartMin,strEndMonth,strEndDay,strEndHour,strEndMin,strDayOfWeek;
			BOOL bIsBlank = FALSE;
			LONG lRegNum = 0;
			for( LONG i = 1; i <= 4; i++ )
			{
				strStartMonth.Format(_T("START_MONTH%d"), i );
				strStartDay.Format(_T("START_DAY%d"), i );
				strStartHour.Format(_T("START_HOUR%d"),i);
				strStartMin.Format(_T("START_MIN%d"),i);
				strEndMonth.Format(_T("END_MONTH%d"), i );
				strEndDay.Format(_T("END_DAY%d"), i );
				strEndHour.Format(_T("END_HOUR%d"),i);
				strEndMin.Format(_T("END_MIN%d"),i);
				strDayOfWeek.Format(_T("DAYOFWEEK%d_C"),i);

				CFieldDef* pStartMonthFieldDef	= it->GetFieldDef( strInfTableName, strStartMonth );
				CFieldDef* pStartDayFieldDef	= it->GetFieldDef( strInfTableName, strStartDay );
				CFieldDef* pStartHourFieldDef	= it->GetFieldDef( strInfTableName, strStartHour );
				CFieldDef* pStartMinFieldDef	= it->GetFieldDef( strInfTableName, strStartMin );
				CFieldDef* pEndMonthFieldDef	= it->GetFieldDef( strInfTableName, strEndMonth );
				CFieldDef* pEndDayFieldDef		= it->GetFieldDef( strInfTableName, strEndDay );
				CFieldDef* pEndHourFieldDef		= it->GetFieldDef( strInfTableName, strEndHour );
				CFieldDef* pEndMinFieldDef		= it->GetFieldDef( strInfTableName, strEndMin );
				CFieldDef* pDayOfWeekFieldDef	= it->GetFieldDef( strInfTableName, strDayOfWeek );

				if( !pStartMonthFieldDef || !pStartDayFieldDef || !pStartHourFieldDef || !pStartMinFieldDef ||
					!pEndMonthFieldDef || !pEndDayFieldDef || !pEndHourFieldDef || !pEndMinFieldDef || !pDayOfWeekFieldDef )
				{
					_ASSERTE( pStartMonthFieldDef == NULL || pStartDayFieldDef == NULL || pStartHourFieldDef == NULL || pStartMinFieldDef == NULL ||
					pEndMonthFieldDef == NULL || pEndDayFieldDef == NULL || pEndHourFieldDef == NULL || pEndMinFieldDef == NULL || pDayOfWeekFieldDef == NULL );
					continue;
				}
				// �Ԃɋ�̍s���Ȃ����ǂ�������ׂ�
				if( bIsBlank ==  TRUE )
				{
					if( pStartMonthFieldDef->m_vaValue.vt != VT_NULL ||
					pStartDayFieldDef->m_vaValue.vt != VT_NULL ||
					pStartHourFieldDef->m_vaValue.vt != VT_NULL ||
					pStartMinFieldDef->m_vaValue.vt != VT_NULL ||
					pEndMonthFieldDef->m_vaValue.vt != VT_NULL ||
					pEndDayFieldDef->m_vaValue.vt != VT_NULL ||
					pEndHourFieldDef->m_vaValue.vt != VT_NULL ||
					pEndMinFieldDef->m_vaValue.vt != VT_NULL ||
					pDayOfWeekFieldDef->m_vaValue.vt != VT_NULL)
					{
						strMsg.Format( AheLoadString( IDS_BLANKROW ), strInfTableName, lOID );
						AttrMessageBox( strMsg, AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_HIGH );

						bRet = FALSE;
						break;
					}
				}

				if( ! ( ( pStartMonthFieldDef->m_vaValue.vt == VT_NULL &&
					pStartDayFieldDef->m_vaValue.vt == VT_NULL &&
					pStartHourFieldDef->m_vaValue.vt == VT_NULL &&
					pStartMinFieldDef->m_vaValue.vt == VT_NULL &&
					pEndMonthFieldDef->m_vaValue.vt == VT_NULL &&
					pEndDayFieldDef->m_vaValue.vt == VT_NULL &&
					pEndHourFieldDef->m_vaValue.vt == VT_NULL &&
					pEndMinFieldDef->m_vaValue.vt == VT_NULL &&
					pDayOfWeekFieldDef->m_vaValue.vt == VT_NULL) ||
					( pStartMonthFieldDef->m_vaValue.vt != VT_NULL &&
					pStartDayFieldDef->m_vaValue.vt != VT_NULL &&
					pStartHourFieldDef->m_vaValue.vt != VT_NULL &&
					pStartMinFieldDef->m_vaValue.vt != VT_NULL &&
					pEndMonthFieldDef->m_vaValue.vt != VT_NULL &&
					pEndDayFieldDef->m_vaValue.vt != VT_NULL &&
					pEndHourFieldDef->m_vaValue.vt != VT_NULL &&
					pEndMinFieldDef->m_vaValue.vt != VT_NULL &&
					pDayOfWeekFieldDef->m_vaValue.vt != VT_NULL) ) )
				{
					strMsg.Format( AheLoadString( IDS_BLANKCELL ), strInfTableName, lOID, i );
					AttrMessageBox( strMsg, AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_HIGH );

					bRet = FALSE;
					break;
				}

				// ��̍s����������A�����Ńt���O�𗧂ĂĂ���
				if( pStartMonthFieldDef->m_vaValue.vt == VT_NULL &&
					pStartDayFieldDef->m_vaValue.vt == VT_NULL &&
					pStartHourFieldDef->m_vaValue.vt == VT_NULL &&
					pStartMinFieldDef->m_vaValue.vt == VT_NULL &&
					pEndMonthFieldDef->m_vaValue.vt == VT_NULL &&
					pEndDayFieldDef->m_vaValue.vt == VT_NULL &&
					pEndHourFieldDef->m_vaValue.vt == VT_NULL &&
					pEndMinFieldDef->m_vaValue.vt == VT_NULL &&
					pDayOfWeekFieldDef->m_vaValue.vt == VT_NULL )
					bIsBlank = TRUE;

				// �ȉ��A���ԋK���̃`�F�b�N
				if( !bIsBlank )
				{
					// �X�^�[�g�����Q�b�g
					tRow.DATA[1] = pStartMonthFieldDef->m_vaValue.lVal;	
					tConnectRow.DATA[1] = 100*pStartMonthFieldDef->m_vaValue.lVal;

					// �X�^�[�g�����Q�b�g
					tRow.DATA[2] = pStartDayFieldDef->m_vaValue.lVal;	
					tConnectRow.DATA[2] = pStartDayFieldDef->m_vaValue.lVal;

					// �X�^�[�g�����Q�b�g
					tRow.DATA[3] = pStartHourFieldDef->m_vaValue.lVal;
					tConnectRow.DATA[3] = 100*pStartHourFieldDef->m_vaValue.lVal;

					// �X�^�[�g�����Q�b�g
					tRow.DATA[4] = pStartMinFieldDef->m_vaValue.lVal;
					tConnectRow.DATA[4] = pStartMinFieldDef->m_vaValue.lVal;

					// �G���h�����Q�b�g
					tRow.DATA[5] = pEndMonthFieldDef->m_vaValue.lVal;
					tConnectRow.DATA[5] = 100*pEndMonthFieldDef->m_vaValue.lVal;
					// �����N�z�������Ă����ꍇ��ConnectRow��1200�𑫂�
					if( tRow.DATA[5] < tRow.DATA[1] )
						tConnectRow.DATA[5] = (tConnectRow.DATA[5].lVal)+1200;

					// �G���h�����Q�b�g
					tRow.DATA[6] = pEndDayFieldDef->m_vaValue.lVal;
					tConnectRow.DATA[6] = pEndDayFieldDef->m_vaValue.lVal;

					// �G���h�����Q�b�g
					tRow.DATA[7] = pEndHourFieldDef->m_vaValue.lVal;
					tConnectRow.DATA[7] = 100*pEndHourFieldDef->m_vaValue.lVal;
					// ���Ԃ������ׂ��ł���ꍇ��ConnectRow��2400�𑫂�
					if( tRow.DATA[7] < tRow.DATA[3] )
						tConnectRow.DATA[7] = (tConnectRow.DATA[7].lVal)+2400;

					// �G���h�����Q�b�g
					tRow.DATA[8] = pEndMinFieldDef->m_vaValue.lVal;	
					tConnectRow.DATA[8] = pEndMinFieldDef->m_vaValue.lVal;
			
					// �j�����Q�b�g
					tRow.DATA[9] = pDayOfWeekFieldDef->m_vaValue.lVal;
					tConnectRow.DATA[9] = pDayOfWeekFieldDef->m_vaValue.lVal;

					if( tRow.DATA[3].lVal == tRow.DATA[7].lVal && tRow.DATA[4].lVal == tRow.DATA[8].lVal )
					{
						strMsg.Format(AheLoadString( IDS_SAMESTARTEND ), strInfTableName, lOID, i );
						AttrMessageBox( strMsg, AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_HIGH );

						bRet = FALSE;
						break;
					}

					// [bug 6296] �u�����v�̎��������Ȃ����`�F�b�N
					if( ( tRow.DATA[1].lVal == 1 || tRow.DATA[1].lVal == 0 ) && tRow.DATA[2].lVal == 1 && tRow.DATA[3].lVal == 0 && tRow.DATA[4].lVal == 0 &&
						( tRow.DATA[5].lVal == 12 || tRow.DATA[1].lVal == 0 ) && tRow.DATA[6].lVal == 31 && tRow.DATA[7].lVal == 24 && tRow.DATA[8].lVal == 0 && tRow.DATA[9].lVal == 15)
					{
						strMsg.Format( AheLoadString( IDS_ENDLESSDAY ), strInfTableName, lOID, i );
						AttrMessageBox( strMsg, AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_HIGH );

						bRet = FALSE;
						break;
					}

					vecTimeRegData.push_back(tRow);
					vecConnectTimeRegData.push_back(tConnectRow);

					// �K���̌����J�E���g
					lRegNum++;
				}
			}

			enum DAY
			{
				ALL_DAY = 15,
				WEEKDAY = 5
			};

			if( lRegNum > 1)
			{
				for(INT i = 0; i < lRegNum - 1; i++ )
				{
					for(INT j = i+1; j < lRegNum; j++ )
					{
						BOOL IsSameTimeReg = TRUE;
						// �������ԋK�����Ȃ����ǂ����̃`�F�b�N
						for( INT k = 0; k < 10; k++ )
						{
							if( !(vecTimeRegData[i].DATA[k].lVal == vecTimeRegData[j].DATA[k].lVal) )
								IsSameTimeReg = FALSE;
						}

						// ���ԋK���ɏd�Ȃ肪�Ȃ����̃`�F�b�N

						// [bug 6296] �u0���v�̋K���� 1���`12�� && �u�S�j���v�̋K���ɓ��ɂ����ԁA���Ԃ���܂����ꍇ��NG
						//				 NG��j
						//					1��1���`12��31���A0:00�`9:00�A�u�S�j���v
						//					0��1���`0��15���A7:00�`9:00�A�u�S�j���v

						CRegulationTime cReg1( m_hWnd, (EKiseiNum)i ), cReg2( m_hWnd, (EKiseiNum)j );
						if( cReg1.IsEveryMonth() && cReg2.IsZeroGatu() )
						{
							if( cReg1.IsContainDays( cReg2.GetRegTime() ) && cReg1.IsContainTime( cReg2.GetRegTime() ) )
							{
								strMsg.Format( AheLoadString( IDS_DUPLITIME ), strInfTableName, lOID, i+1, j+1 );
								AttrMessageBox( strMsg, AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_HIGH );

								bRet = FALSE;
								break;
							}
						}
						if( cReg1.IsZeroGatu() && cReg2.IsEveryMonth() )
						{
							if( cReg2.IsContainDays( cReg1.GetRegTime() ) && cReg2.IsContainTime( cReg1.GetRegTime() ) )
							{
								strMsg.Format( AheLoadString( IDS_DUPLITIME ), strInfTableName, lOID, i+1, j+1 );
								AttrMessageBox( strMsg, AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_HIGH );

								bRet = FALSE;
								break;
							}
						}

						// �S�j��������Ƃ��A�܂��͓����j���̎��Ƀ`�F�b�N
						if( vecConnectTimeRegData[i].DATA[9].lVal == ALL_DAY || vecConnectTimeRegData[j].DATA[9].lVal == ALL_DAY ||
							(vecConnectTimeRegData[i].DATA[9].lVal == vecConnectTimeRegData[j].DATA[9].lVal) )
						{

							// ���A���Ԃ�4���ɕϊ�����
							// ��j�@12��31���Ȃ�1231�A25:15�Ȃ�2515�ƕϊ�
							// �����̑召��r�Ŏ��ԋK���̏d�Ȃ���`�F�b�N����
							LONG lStartDay1,lStartDay2,lStartHour1,lStartHour2;
							LONG lEndDay1,lEndDay2,lEndHour1,lEndHour2;
							lStartDay1 = vecConnectTimeRegData[i].DATA[1].lVal + vecConnectTimeRegData[i].DATA[2].lVal;
							lStartDay2 = vecConnectTimeRegData[j].DATA[1].lVal + vecConnectTimeRegData[j].DATA[2].lVal;
							lEndDay1 = vecConnectTimeRegData[i].DATA[5].lVal + vecConnectTimeRegData[i].DATA[6].lVal;
							lEndDay2 = vecConnectTimeRegData[j].DATA[5].lVal + vecConnectTimeRegData[j].DATA[6].lVal;
							lStartHour1 = vecConnectTimeRegData[i].DATA[3].lVal + vecConnectTimeRegData[i].DATA[4].lVal;
							lStartHour2 = vecConnectTimeRegData[j].DATA[3].lVal + vecConnectTimeRegData[j].DATA[4].lVal;
							lEndHour1 = vecConnectTimeRegData[i].DATA[7].lVal + vecConnectTimeRegData[i].DATA[8].lVal;
							lEndHour2 = vecConnectTimeRegData[j].DATA[7].lVal + vecConnectTimeRegData[j].DATA[8].lVal;


							// �����j���������Ƃ�
							if( vecConnectTimeRegData[i].DATA[9].lVal == vecConnectTimeRegData[j].DATA[9].lVal )
							{
								if( (lStartDay1 <= lStartDay2 && lEndDay1 >= lEndDay2 && lStartHour1 <= lStartHour2 && lEndHour1 >= lEndHour2 ) ||
									(lStartDay1 >= lStartDay2 && lEndDay1 <= lEndDay2 && lStartHour1 >= lStartHour2 && lEndHour1 <= lEndHour2 ) )
								{
									strMsg.Format( AheLoadString( IDS_DUPLITIME ), strInfTableName, lOID, i+1, j+1 );
									AttrMessageBox( strMsg, AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_HIGH );

									bRet = FALSE;
									break;
								}
							}
							// �S�j���̎�
							if( vecConnectTimeRegData[i].DATA[9].lVal == ALL_DAY || vecConnectTimeRegData[j].DATA[9].lVal == ALL_DAY )
							{

								// ��r�����ق����S�j���������ꍇ���l���A���Ԃ����ւ���
								// ��)�@�@ 0:00 �`�@24:00 ���j��
								//	�@�A�@ 0:00 �`  23:00 �S�j��
								// �@�̂悤�ȂƂ��ɁA�A�����C���ɍl���Ĕ�r���Ȃ���΂����Ȃ��̂ŁA
								// �@��L�̗�̎��ɂ́A�ꉞ�s���Ȏ��ԋK���ł͂Ȃ��Ƃ���B
								if( vecConnectTimeRegData[j].DATA[9].lVal == ALL_DAY )
								{
									LONG tmp;
									tmp = lStartDay1;
									lStartDay1 = lStartDay2;
									lStartDay2 = tmp;

									tmp = lEndDay1;
									lEndDay1 = lEndDay2;
									lEndDay2 = tmp;

									tmp = lStartHour1;
									lStartHour1 = lStartHour2;
									lStartHour2 = tmp;

									tmp = lEndHour1;
									lEndHour1 = lEndHour2;
									lEndHour2 = tmp;
								}

								if(lStartDay1 <= lStartDay2 && lEndDay1 >= lEndDay2 && lStartHour1 <= lStartHour2 && lEndHour1 >= lEndHour2 )
								{
									strMsg.Format( AheLoadString( IDS_DUPLITIME ), strInfTableName, lOID, i+1, j+1 );
									AttrMessageBox( strMsg, AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_HIGH );

									bRet = FALSE;
									break;
								}
							}
						}

						if( IsSameTimeReg )
						{
							strMsg.Format( AheLoadString( IDS_SAMETIME ), strInfTableName, lOID, i+1, j+1 );
							AttrMessageBox( strMsg, AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_HIGH );

							bRet = FALSE;
							break;
						}
					}
				}
			}
		}
	}
	
	return bRet;
}
