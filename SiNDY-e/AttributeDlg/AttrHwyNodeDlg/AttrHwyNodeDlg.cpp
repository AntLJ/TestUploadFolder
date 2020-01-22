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

// AttrHwyNodeDlg.cpp: CAttrHwyNodeDlg �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AttrHwyNodeDlg.h"
#include <sindy/schema.h>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/assign.hpp>
#include <boost/array.hpp>
#include <TDC/useful_headers/str_util.h>
#include <WinLib/type_convert.h>

using namespace sindy::schema;

/// �E�B���h�E�}�l�[�W���z�u�}�b�v�ihwynode�Atollnode���p�j
BEGIN_WINDOW_MAP(HWYTOLLNODEMAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(2,2))
		BEGINROWS(WRCT_PCT,0,90)
			BEGINROWS(WRCT_REST, IDC_TAB_HWYNODE, RCMARGINS(2,2))
				RCSPACE(22)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))		// �H����ʂƃn�C�E�F�C���[�h�t���O
					RCTOFIT(IDC_STATIC_ROADCLASS_REF)
					RCSPACE(5)
					RCREST(IDC_COMBO_HWYNODE_ROADCLASS_REF)
					RCSPACE(15)
					RCTOFIT(IDC_CHECK_ISHWMODE)
				ENDGROUP()
				RCSPACE(4)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))		// �H������
					RCTOFIT(IDC_STATIC_ROADNAME_REF)
					RCSPACE(5)
					RCREST(IDC_COMBO_HWYNODE_ROADNAME_REF)
				ENDGROUP()
				RCSPACE(4)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))		// �{�ݖ���
					RCTOFIT(IDC_STATIC_SEQUENCE)
					RCSPACE(5)
					RCREST(IDC_COMBO_SEQUENCE)
				ENDGROUP()
				RCSPACE(4)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))		// IC�ԍ�
					RCTOFIT(IDC_STATIC_HWYGUIDE_NUM)
					RCSPACE(5)
					RCTOFIT(IDC_EDIT_HWYGUIDE_NUM)
				ENDGROUP()
				RCSPACE(7)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))       // �����R�[�h�ƕ��򍇗��R�[�h
					RCTOFIT(IDC_STATIC_DIRECTION)
					RCSPACE(5)
					RCTOFIT(IDC_COMBO_DIRECTION)
					RCSPACE(10)
					RCTOFIT(IDC_STATIC_JUNCTION)
					RCSPACE(5)
					RCTOFIT(IDC_COMBO_JUNCTION)
				ENDGROUP()
				RCSPACE(5)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))       // ��������ʃR�[�h�ƈē��������t���O
					RCTOFIT(IDC_STATIC_TOLLCLASS)
					RCSPACE(5)
					RCTOFIT(IDC_COMBO_TOLLCLASS)
					RCSPACE(10)
					RCTOFIT(IDC_CHECK_GUIDE)
				ENDGROUP()
				RCSPACE(7)

				// ��ʎԗ��p�O���[�v
				BEGINROWS(WRCT_TOFIT, IDC_GROUP_NOETC, RCMARGINS(0, 0))
					BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(7,0))
						RCTOFIT(IDC_CHECK_NOETC_CAR)			// ��ʎԗ��Ή��t���O
					ENDGROUP()
					RCSPACE(5)
					BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(0,0))
						RCSPACE(15)
						RCTOFIT(IDC_STATIC_TOLLFUNC)
						RCSPACE(10)
						RCTOFIT(IDC_COMBO_TOLLFUNC)				// �������@�\�R�[�h
						RCSPACE(10)
						RCTOFIT(IDC_CHECK_NOGUIDETOLL)			// ��U���������t���O
					ENDGROUP()
					RCSPACE(5)
				ENDGROUP()
				RCSPACE(7)

				// ETC���ڎԗp�O���[�v
				BEGINROWS(WRCT_TOFIT, IDC_GROUP_ETC, RCMARGINS(0, 0))
					BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(7,0))
						RCTOFIT(IDC_STATIC_ETC_FACIL)			// ETC�{�݃R�[�h
						RCSPACE(5)
						RCTOFIT(IDC_COMBO_ETC_FACIL)
					ENDGROUP()
					RCSPACE(4)
					BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(0,0))
						RCSPACE(15)
						RCTOFIT(IDC_STATIC_TOLLFUNC_ETC)
						RCSPACE(10)
						RCTOFIT(IDC_COMBO_TOLLFUNC_ETC)			// �������@�\�R�[�h(ETC)
						RCSPACE(10)
						RCTOFIT(IDC_CHECK_NOGUIDETOLL_ETC)		// ��U���������t���O(ETC)
					ENDGROUP()
					RCSPACE(4)
				ENDGROUP()
				RCSPACE(7)

				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))       // �����m�[�hID�Ɨ��o�m�[�hID
					RCTOFIT(IDC_STATIC_FROMNODEID)
					RCTOFIT(IDC_COMBO_FROMNODE_ID)
					RCSPACE(32)
					RCTOFIT(IDC_STATIC_TONODEID)
					RCTOFIT(IDC_COMBO_TONODE_ID)
				ENDGROUP()
			ENDGROUP()
		ENDGROUP()
		RCSPACE(7)
		BEGINROWS(WRCT_REST, 0, 10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_SOURCE)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_SOURCE)
			ENDGROUP()
		ENDGROUP()
	ENDGROUP()
END_WINDOW_MAP()
// �E�B���h�E�}�l�[�W���z�u�}�b�v�i���������[�����^�u�p�j
BEGIN_WINDOW_MAP(HWYTOLLNODEMAP2)
	BEGINROWS(WRCT_REST,0,RCMARGINS(2,2))
		BEGINROWS(WRCT_PCT,0,90)
			BEGINROWS(WRCT_REST, IDC_TAB_HWYNODE, RCMARGINS(2,2))
				RCSPACE(20)
				BEGINROWS(WRCT_TOFIT, IDC_GROUP_TOLLGATELANE, RCMARGINS(10, 10))
					RCSPACE(5)
					RCFIXED(IDC_PICTURE_LANEIMG, 64)				// ���������[�����̃C���[�W
					RCSPACE(10)
					BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(0,0))
						RCTOFIT(IDC_EDIT_TOLLGATELANE)			// ���������[�����
					ENDGROUP()
				ENDGROUP()
			ENDGROUP()
		ENDGROUP()
		RCSPACE(7)
		BEGINROWS(WRCT_PCT, 0, 10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_SOURCE)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_SOURCE)
			ENDGROUP()
		ENDGROUP()
	ENDGROUP()
END_WINDOW_MAP()
// �E�B���h�E�}�l�[�W���z�u�}�b�v�i�������R�[�h�����^�u�p�j
BEGIN_WINDOW_MAP(HWYTOLLNODEMAP3)
	BEGINROWS(WRCT_REST,0,RCMARGINS(2,2))
		BEGINROWS(WRCT_PCT,0,90)
			BEGINROWS(WRCT_REST, IDC_TAB_HWYNODE, RCMARGINS(2,2))
				RCSPACE(22)
					BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))		// �y23-090�zETC�������R�[�h
						RCTOFIT(IDC_STATIC_ETC_TOLLGATE)
						RCSPACE(5)
						RCREST(IDC_EDIT_ETC_TOLLGATE)
					ENDGROUP()
			ENDGROUP()
		ENDGROUP()
	ENDGROUP()
	RCSPACE(7)
		BEGINROWS(WRCT_PCT, 0, 10)
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
 * @retval CAttrHwyNodeDlg��Ԃ��܂��B
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CAttrHwyNodeDlg();
}

// CAttrHwyNodeDlg�̐ÓI�����o�ϐ�
const long CAttrHwyNodeDlg::m_TollGateLaneStrMaxLength = 32;
const long CAttrHwyNodeDlg::m_TollGateLaneImageColumnMaxNum = 16;

/////////////////////////////////////////////////////////////////////////////
//
// CAttrHwyNodeDlg �����o�֐�
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrHwyNodeDlg::CAttrHwyNodeDlg()
 * @brief �R���X�g���N�^
 *
 * �u���V�̏����������Ă����܂��B
 */
CAttrHwyNodeDlg::CAttrHwyNodeDlg() 
	: ATTR_BASE_CLASS(HWYTOLLNODEMAP, IDD_ATTRHWYNODEDLG),
	m_wnd0 ( _T("CHECKBOX"),	this, IDC_CHECK_ISHWMODE ),
	m_wnd1 ( _T("COMBOBOX"),	this, IDC_COMBO_HWYNODE_ROADCLASS_REF ), 
	m_wnd2 ( _T("COMBOBOX"),	this, IDC_COMBO_HWYNODE_ROADNAME_REF ), 
	m_wnd3 ( _T("COMBOBOX"),	this, IDC_COMBO_SEQUENCE ), 
	m_wnd4 ( _T("COMBOBOX"),	this, IDC_COMBO_DIRECTION ), 
	m_wnd5 ( _T("COMBOBOX"),	this, IDC_COMBO_JUNCTION ), 
	m_wnd7 ( _T("CHECKBOX"),	this, IDC_CHECK_DUMMY ), 
	m_wnd8 ( _T("CHECKBOX"),	this, IDC_CHECK_GUIDE ), 
	m_wnd9 ( _T("COMBOBOX"),	this, IDC_COMBO_TOLLCLASS ),
	m_wnd10( _T("COMBOBOX"),	this, IDC_COMBO_TOLLFUNC ),
	m_wnd11( _T("COMBOBOX"),	this, IDC_COMBO_FROMNODE_ID ),
	m_wnd12( _T("COMBOBOX"),	this, IDC_COMBO_TONODE_ID ),
	m_wnd13( _T("EDIT"),		this, IDC_EDIT_SOURCE ),
	m_wnd16( _T("CHECKBOX"),	this, IDC_CHECK_NOETC_CAR ),
	m_wnd17( _T("COMBOBOX"),	this, IDC_COMBO_ETC_FACIL ),
	m_wnd18( _T("COMBOBOX"),	this, IDC_COMBO_TOLLFUNC_ETC ),
	m_wnd19( _T("CHECKBOX"),	this, IDC_CHECK_NOGUIDETOLL_ETC ),
	m_wnd20( _T("EDIT"),		this, IDC_EDIT_TOLLGATELANE ),
	m_wnd21( _T("EDIT"),		this, IDC_EDIT_HWYGUIDE_NUM ),
	m_wnd22( _T("EDIT"),		this, IDC_EDIT_ETC_TOLLGATE ),
	m_wndTab( _T("TAB"),		this, IDC_TAB_HWYNODE ),
	// �^�u�ʒu�̏�����
	m_CurrentTabIndex(0),
	m_ChangeType(0)
{
}

/**
 * @brief WM_NOTIFY �p�C�x���g�n���h��
 *		�^�u�̐؂�ւ��������������̏������s��
 */
LRESULT CAttrHwyNodeDlg::OnNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// �^�u�R���g���[���Ƀt�H�[�J�X������ꍇ
	if( wParam == IDC_TAB_HWYNODE ) {
		LPNMHDR pnmh = (LPNMHDR)lParam;
		if( pnmh->code == TCN_SELCHANGE ) {
			// �R���g���[���؂�ւ�
			m_ChangeType = TabCtrl_GetCurSel( pnmh->hwndFrom );
			
			m_cControlRel.Visible( m_ChangeType );
			ChangeWindowMgrMap( m_ChangeType );
			m_CurrentTabIndex = m_ChangeType; // �^�u�̈ʒu�ۑ�
			//[Bug 5510] AttrAccessPointDlg::OnNotify()�Ɠ��l�ɁA�^�u�؂�ւ����ɃR���{�{�b�N�X����ɂȂ�̂�h������
			// ������AttrHwyNodeDlg::SetCurrentFeatureDefIndex()���Ă�
			SetCurrentFeatureDefIndex( m_lFeatureIndex, m_lTableIndex, m_lRowIndex, m_bForce, m_bEditable );

			bHandled = TRUE;
			return 1;
		}
	}
	// �^�u�ȊO�̃R���g���[���Ƀt�H�[�J�X������ꍇ
	// ATTR_BASE_CLASS::OnKeyDown()�ŁAWM_NOTIFY��wParam==0��PostMessage()�����̂ł����ɗ���
	else if( wParam == 0 )
	{
		// �R���g���[���؂�ւ�
		switch( lParam )
		{
			case -2:	// Ctrl+Tab�������ꂽ�ꍇ
				if( m_ChangeType == hwynode_dialog_tab::tab_end - 1 )
					m_ChangeType = hwynode_dialog_tab::base_attr;
				else
					++m_ChangeType;
				break;
			case -1:	// Shift+Ctrl+Tab�������ꂽ�ꍇ
				if( m_ChangeType == hwynode_dialog_tab::base_attr )
					m_ChangeType = hwynode_dialog_tab::tab_end - 1;
				else
					--m_ChangeType;
				break;
			default:	// ��L�ȊO�̃L�[����ł����ɗ��邱�Ƃ͂Ȃ��͂�
				ATLASSERT(0);
		}

		::SendMessage(m_wndTab, TCM_SETCURSEL, m_ChangeType, 0);
		m_cControlRel.Visible( m_ChangeType );
		ChangeWindowMgrMap( m_ChangeType );
		m_CurrentTabIndex = m_ChangeType;	// �^�u�̈ʒu�ۑ�
		SetCurrentFeatureDefIndex( m_lFeatureIndex, m_lTableIndex, m_lRowIndex, m_bForce, m_bEditable );

		bHandled = TRUE;
		return 1;
	}

	return 0;
}

/**
 * @brief �I�[�i�[�h���[�p�֐�
 * @note	�I�[�i�[�h���[�C�x���g�������ɌĂ΂��COwnerDraw::DrawItem()�̃I�[�o�[���C�h
 */
void CAttrHwyNodeDlg::DrawItem(LPDRAWITEMSTRUCT lpdis)
{
	if (m_CurrentTabIndex == hwynode_dialog_tab::tollgate_lane)
	{
		DrawLaneImage();
	}
}

/** 
 * @fn HWND CAttrHwyNodeDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief �q�_�C�A���O���쐬���܂��B
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)�Ń_�C�A���O���쐬���܂��B
 *
 * @param hWndParent	[in]	�e�E�B���h�E�n���h��
 * @param dwInitParam	[in]	�_�C�A���O�������p�����[�^�i���ݓ��Ɏg�p���Ă��܂���j
 *
 * @retval �쐬���ꂽ�q�_�C�A���O�̃n���h����Ԃ��܂��B
 */
HWND CAttrHwyNodeDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	m_hChildWnd = ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
	return m_hChildWnd;
}

/**
 * @fn void CAttrHwyNodeDlg::Delete()
 * @brief �쐬�����q�_�C�A���O���폜���܂��B
 *
 * �_�C�A���O���폜����Ƃ��ɕK���Ă�ŉ������B�Ă΂Ȃ��ƃ��������[�N���Ă��܂��܂��B
 */
void CAttrHwyNodeDlg::Delete()
{
	ClearFeatureDefs();
	::DestroyWindow(m_hChildWnd);
    delete this; // �K��new�Ŋm�ۂ���O��
}

/**
 * @fn void CAttrHwyNodeDlg::SetArcHelper( IApplication* ipApp )
 * @brief ATTR_BASE_CLASS��ArcHelper��n���܂��A�܂��AHwyHelper�ɂ�ArcHelper���Z�b�g���܂��B
 */
void CAttrHwyNodeDlg::SetArcHelper( IApplication* ipApp )
{ 
    ATTR_BASE_CLASS::SetArcHelper( ipApp );
}

/**
 * @brief ETC�����ʗ����p�t�B�[���h���Ȃ��ꍇ�ɕK�v�ɂȂ�R���g���[����Enable/Disable��؂�ւ����s��
 * @note	[Bug6432]�Ή����ɒǉ�
 */
void CAttrHwyNodeDlg::SetAvailabilityOfEtcSpecificTollControls(void)
{
	// ETC�����ʗ����p�̃t�B�[���h�����݂��Ȃ��ꍇ
	if (! EtcSpecificTollFieldsExist())
	{
		// �Ή�����R���g���[�������Disable�ɂ���
		SetDlgStatus( IDC_CHECK_NOETC_CAR, FALSE );
		SetDlgStatus( IDC_COMBO_ETC_FACIL, FALSE );
		SetDlgStatus( IDC_COMBO_TOLLFUNC_ETC, FALSE );
		SetDlgStatus( IDC_CHECK_NOGUIDETOLL_ETC, FALSE );
		SetDlgStatus( IDC_STATIC_ETC_FACIL, FALSE );
		SetDlgStatus( IDC_STATIC_TOLLFUNC_ETC, FALSE );

		// �ҏW�J�n��ԂȂ�A��ʎԗ��Ή��t���O�ɏ]�����邽�߂�Disable�ɂȂ��Ă���R���g���[���������I��Enable�ɂ���
		// BUG 8550 ���[���I�ɕҏW�s��Ԃ̂Ƃ��ɁA�������@�\�R�[�h���ҏW�ł��� -> ���[���ҏW�̔��f��ǉ�����
		if (IsStartEditing() && m_bEditable)
		{
			SetDlgStatus( IDC_COMBO_TOLLFUNC, TRUE );
			SetDlgStatus( IDC_CHECK_NOGUIDETOLL, TRUE );
		}
	}
}

/**
 * @fn BOOL CAttrHwyNodeDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
 * @brief ���ݕ\�������ׂ��t�B�[�`����FeatureDef�C���f�b�N�X��n���܂��B
 * ����ɁA�t�B�[�`���̑����Ƃ͊֌W�Ȃ��R���g���[���̐�������Ă����܂��B
 * �_�C�A���O���J�����Ƃ��ɍŏ��Ƀt�H�[�J�X�̓������Ă��ė~�����R���g���[���̐ݒ�����Ă����܂��B
 * �܂��A�n�C�E�F�C�m�[�h�́A�����A���o�m�[�h�Ɍ����āA����\�������肵�Ȃ���΂Ȃ�Ȃ��̂ŁA���̐ݒ�����Ă����܂��B
 *
 * @param lFeatureIndex	[in]	�t�B�[�`����`�N���X�̃C���f�b�N�X�ԍ�
 * @param lTableIndex	[in]	�e�[�u����`�N���X�̃C���f�b�N�X�ԍ�
 * @param lRowIndex		[in]	���R�[�h��`�N���X�̃C���f�b�N�X�ԍ�
 * @param bForce		[in]	�����I�ɕҏW���邩�ǂ���
 * @param bEditable		[in]	�ҏW�\���ǂ���
 *
 * @retval �Ӗ����Ȃ��iTRUE�����Ԃ��Ȃ��j
 */
BOOL CAttrHwyNodeDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{
	BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, m_CurrentTabIndex, bForce, bEditable );
	
	m_lFeatureIndex = lFeatureIndex;
	m_lTableIndex = lTableIndex;
	m_lRowIndex = lRowIndex;
	m_bForce = bForce;
	m_bEditable = bEditable;

	// ��ԍŏ��Ɍ��ݑI�������n�C�E�F�C�m�[�h�̉��ɂ��铹�H�m�[�h���擾���Ă���
	// ���ݑI�𒆂̃t�B�[�`�����擾
	CFeatureDef* ipFeatureDef = GetCurrentFeatureDef();
	IFeaturePtr ipFeature = ipFeatureDef->GetFeature();
	// �W�I���g���擾
	IGeometryPtr ipGeometry;
	ipFeature->get_Shape( &ipGeometry );
	// �����n�C�E�F�C�m�[�h������ꍇ�ɁAm_ipNodeUnderHwyNode������������Ă��Ȃ���
	// ���Ƃŗ����邱�Ƃ�������
	m_ipNodeUnderHwyNode = NULL;
	if( ipGeometry )
	{
		// �m�[�h���C�����擾
		ILayerPtr ipLayer;
		CString strOwnerName = AheGetFeatureClassOwnerName( ipFeature );
		if( strOwnerName.IsEmpty() )
			strOwnerName = _T("ROAD_NODE");
		else
		{
			strOwnerName += _T(".");
			strOwnerName += _T("ROAD_NODE");
		}
		ipLayer = AheGetLayerByName( m_cArcHelper.GetMap(), strOwnerName, _FIND_TABLE, FALSE, TRUE );
		// ���C�����擾�ł��Ȃ������ꍇ�ɂ́A�p�[�\�i���W�I�f�[�^�x�[�X�̉\������
		if( !ipLayer )
			AheGetLayerByTableName( m_cArcHelper.GetMap(), _T("Road_Node"), &ipLayer );

		if ( ipLayer )
		{
			CTargetItems cItems;
			// �n�C�E�F�C�̉��̃m�[�h�t�B�[�`���擾
			AheSelectByShapeFromLayer( ipGeometry, cItems, ipLayer );

			LONG lCount = cItems.size();
			if( lCount == 1 )
				m_ipNodeUnderHwyNode = (cItems.begin())->GetFeature();
		}
	}
	
	// �H���R�[�h�A�H�����V�[�P���X�̏����l�m��
	CComVariant			vaRoadSequence;
	CFieldDef*			pFieldDef;
	CComVariant			vaValue;        
	pFieldDef = GetCurrentFeatureDef()->GetFieldDef( HIGHWAY_NODE_TABLE_NAME, _T("ROAD_SEQ") );
	vaValue= pFieldDef->m_vaValue;
	m_lOrgRoadSequence = vaValue.lVal;
	pFieldDef = GetCurrentFeatureDef()->GetFieldDef( HIGHWAY_NODE_TABLE_NAME, _T("ROAD_CODE") );
	vaValue = pFieldDef->m_vaValue;
	m_lOrgRoadCode = vaValue.lVal;
	
	// �ŏ�����H���R�[�h���o������
	SetLineClassList(m_lOrgRoadCode);
	ChangeRoadSeqCombobox();
	
	// �����A���o�m�[�h�̐ݒ�
	SetInOutNodeComboBox();
	
	CButton	cHwyCheckBox = CButton(GetDlgItem(IDC_CHECK_ISHWMODE));
	INT iCheckBox = cHwyCheckBox.GetCheck();
	
	// �n�C�E�F�C���[�h���I�t��������A���o�A�����m�[�h��`��
	if( iCheckBox == 0 )
		DrawInOutNode();

	// ���ڃt�B�[�`���Ɋ֌W�Ȃ��G�f�B�b�g�{�b�N�X(��F�Q�Ɨp�̘H�����V�[�P���X�⌴�e�ԍ�����)�Ȃǂ𐧌䂷��
	// �ҏW�J�n�O�ƕҏW�J�n��ł��ҏW�s�̂Ƃ���DISABLE�ɂ���
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		// �ҏW�J�n����Ă���Ƃ��ɂ��łɁASetFocus���Ă���
		switch( m_CurrentTabIndex )
		{
			case hwynode_dialog_tab::base_attr:
				::SetFocus( GetDlgItem(IDC_COMBO_HWYNODE_ROADCLASS_REF) );
				break;
			case hwynode_dialog_tab::tollgate_lane:
				::SetFocus( GetDlgItem(IDC_EDIT_TOLLGATELANE) );
				break;
			case hwynode_dialog_tab::etc_tollgate:
				::SetFocus( GetDlgItem(IDC_EDIT_ETC_TOLLGATE) );
				break;
			default:
				ATLASSERT(0);
		}
	}

	// [Bug6432] ETC�����ʗ����p�t�B�[���h���Ȃ��ꍇ�A�R���g���[����Enable/Disable��؂�ւ���
	SetAvailabilityOfEtcSpecificTollControls();
	// [Bug6570] ���������[�����t�B�[���h���Ȃ��ꍇ�́A���������[�����̃G�f�B�b�g�{�b�N�X��Disable�ɂ���
	if (! TollGateLaneFieldExist())
	{
		SetDlgStatus(IDC_EDIT_TOLLGATELANE, FALSE);
	}

	// ETC������ID�t�B�[���h���Ȃ����Disable
	if(0 > AheFindField(AheGetFields(ipFeature), highway_node::kTollgateID))
	{
		SetDlgStatus(IDC_EDIT_ETC_TOLLGATE, FALSE);
	}

	// ETC������ID��ETC��Ή��̂Ƃ���Disable�ɂ���
	auto fd = GetCurrentFeatureDef()->GetFieldDef( highway_node::kTableName, highway_node::kEtcFacil );
	SetDlgStatus( 
		IDC_EDIT_ETC_TOLLGATE,
		bool2BOOL( fd->m_vaValue.lVal != highway_node::etc_facil::kNoEtcFacil ));

	// �n�C�E�F�C���[�h�t���O�̃`�F�b�N�{�b�N�X�͏��Disable�i�\����p�j�ɂ���i[Bug 6217]�Ή����ɒǉ��j
	SetDlgStatus(IDC_CHECK_ISHWMODE, FALSE);

	// �����l�ɑ΂��鐮���`�F�b�N
	CheckItems();

	// �m�[�h�����ʒu�ɂȂ���΁A�S��Disable�ɂ���
	if( !m_ipNodeUnderHwyNode )
	{
		DisableAllControls();
	}

    return bRet;
}

/**
 * @brief ���݂̃^�u�ʒu��Ԃ�
 * @note	�������z�֐�CExportDlg::GetTabNum()�̎���
 * @note	���Ƀ_�C�A���O���J�������ɂ��̃^�u���A�N�e�B�u�ɂȂ�悤�ɂ��邽�߁ACExportDlg�Ɍ��݂̃^�u�ʒu��n���Ă���
 * @retval ���݂̃^�u�ʒu
 */
LONG CAttrHwyNodeDlg::GetTabNum()
{
	return m_CurrentTabIndex;
}

/**
 * @brief �^�u�ʒu���Z�b�g���܂�
 * @note	�������z�֐�CExportDlg::SetTabNum()�̎���
 * @param lTabNum	[in]	�^�u�ʒu
 */
void CAttrHwyNodeDlg::SetTabNum(LONG lTabNum)
{
	m_CurrentTabIndex = lTabNum;
	m_ChangeType = lTabNum;
}

/**
 * @brief ETC�����ʗ����Ή��̂��߂̃f�[�^���f���ύX[Bug6198]�Œǉ����ꂽ�t�B�[���h�����݂��邩�𔻒肷��
 * @note	�ŏ��ɌĂ΂ꂽ���ɔ��茋�ʂ�m_pEtcSpecificTollFieldsExist�Ɋi�[���āA�Ȍ�͂�����Q��
 * @return	�Ώۃt�B�[���h���S�đ��݂����true�A�P�ł����݂��Ȃ��t�B�[���h�������false
 */
bool CAttrHwyNodeDlg::EtcSpecificTollFieldsExist()
{
	bool aResult = true;
	if (m_pEtcSpecificTollFieldsExist.get() )
		aResult = *m_pEtcSpecificTollFieldsExist;
	else
	{
		using namespace sindy::schema;

		// �n�C�E�F�C�m�[�h��IFeatureClassPtr���擾
		// [Bug6432] HwyHelper���g��Ȃ��悤�ɏC��
		IFeaturePtr ipFeature(GetCurrentFeatureDef()->GetFeature());
		IFeatureClassPtr ipHwyNodeFeatureClass(AheGetFeatureClass(ipFeature));

		std::vector<CString> aFieldNames;
		aFieldNames.push_back(highway_node::kNoEtcCar);
		aFieldNames.push_back(highway_node::kEtcFacil);
		aFieldNames.push_back(highway_node::kEtcTollFunc);
		aFieldNames.push_back(highway_node::kEtcNoGuideToll);
		
		BOOST_FOREACH (CString aFieldName, aFieldNames)
		{
			long aIndex = ipHwyNodeFeatureClass->_FindField(aFieldName.GetString());
			if (aIndex == -1)
				aResult = false;
		}

		m_pEtcSpecificTollFieldsExist = std::auto_ptr<bool>(new bool(aResult));
	}

	return aResult;
}

/**
 * @brief ���������[���Ή��̂��߂̃f�[�^���f���ύX[Bug6557]�Œǉ����ꂽ�u���������[�����v�t�B�[���h�����݂��邩�𔻒肷��
 * @note	�ŏ��ɌĂ΂ꂽ���ɔ��茋�ʂ�m_pTollGateLaneFieldExist�Ɋi�[���āA�Ȍ�͂�����Q��
 * @return	�Ώۃt�B�[���h���S�đ��݂����true�A�P�ł����݂��Ȃ��t�B�[���h�������false
 */
bool CAttrHwyNodeDlg::TollGateLaneFieldExist()
{
	bool aResult = true;
	if (m_pTollGateLaneFieldExist.get() )
		aResult = *m_pTollGateLaneFieldExist;
	else
	{
		using namespace sindy::schema;

		// �n�C�E�F�C�m�[�h��IFeatureClassPtr���擾
		IFeaturePtr ipFeature(GetCurrentFeatureDef()->GetFeature());
		IFeatureClassPtr ipHwyNodeFeatureClass(AheGetFeatureClass(ipFeature));

		long aIndex = ipHwyNodeFeatureClass->_FindField(highway_node::kTollGateLane);
		if (aIndex == -1)
			aResult = false;
		m_pTollGateLaneFieldExist = std::auto_ptr<bool>(new bool(aResult));
	}

	return aResult;
}


/**
 * @fn void CAttrBaseMeshDlg::CreateControlRelation()
 * @brief �q�_�C�A���O�����ۂɍ쐬���܂��B
 *
 * ���ۂ̍쐬��ControlRel�ɔC���܂��B
 */
void CAttrHwyNodeDlg::CreateControlRelation()
{
	using namespace hwynode_dialog_tab;
    ATLASSERT( IsWindow() == TRUE );
	
    if( m_cControlRel.size() > 0 )
    {
        // �e�E�C���h�E�n���h����ݒ肵�Ȃ���
        m_cControlRel.ResetHWND( m_hWnd );
    } 
    else {
        m_cControlRel.SetHWND( m_hWnd );

		// �^�u
		m_cControlRel.SetControl( IDC_TAB_HWYNODE,		TYPE_OTHER, 	base_attr,	tab_end - 1 );

		using namespace sindy::schema;
        // �n�C�E�F�C���[�h�t���O
		m_cControlRel.SetControl( IDC_CHECK_ISHWMODE,		TYPE_CHECKBOX,	base_attr,	base_attr,	highway_node::kHwyMode,	NULL,	HIGHWAY_NODE_TABLE_NAME );
		// �H�����
		m_cControlRel.SetControl( IDC_STATIC_ROADCLASS_REF, TYPE_OTHER, 	base_attr,	base_attr );
        m_cControlRel.SetControl( IDC_COMBO_HWYNODE_ROADCLASS_REF,	TYPE_COMBO,		base_attr,	base_attr );
		// �H������
		m_cControlRel.SetControl( IDC_STATIC_ROADNAME_REF, TYPE_OTHER, 	base_attr,	base_attr );
		m_cControlRel.SetControl( IDC_COMBO_HWYNODE_ROADNAME_REF,		TYPE_COMBO,		base_attr,	base_attr,	highway_node::kRoadCode,	NULL,	HIGHWAY_NODE_TABLE_NAME);
		// �{�ݖ���
		m_cControlRel.SetControl( IDC_STATIC_SEQUENCE, TYPE_OTHER, 	base_attr,	base_attr );
		m_cControlRel.SetControl( IDC_COMBO_SEQUENCE,		TYPE_COMBO,		base_attr,	base_attr,	highway_node::kRoadSeq,		NULL,	HIGHWAY_NODE_TABLE_NAME );
		// IC�ԍ�(bug 10205)
		m_cControlRel.SetControl( IDC_STATIC_HWYGUIDE_NUM, TYPE_OTHER, 	0,	base_attr );
		m_cControlRel.SetControl( IDC_EDIT_HWYGUIDE_NUM,	TYPE_EDITSTRING,base_attr,	base_attr,	highway_node::kHWYGuideNumber,	NULL,	HIGHWAY_NODE_TABLE_NAME );
        // �����R�[�h
		m_cControlRel.SetControl( IDC_STATIC_DIRECTION, TYPE_OTHER, 	base_attr,	base_attr );
		m_cControlRel.SetControl( IDC_COMBO_DIRECTION,		TYPE_COMBO,		base_attr,	base_attr,	highway_node::kDirection,	NULL,	HIGHWAY_NODE_TABLE_NAME);
        // ����E�����R�[�h
		m_cControlRel.SetControl( IDC_STATIC_JUNCTION, TYPE_OTHER, 	base_attr,	base_attr );
		m_cControlRel.SetControl( IDC_COMBO_JUNCTION,		TYPE_COMBO,		base_attr,	base_attr,	highway_node::kInOut,		NULL,	HIGHWAY_NODE_TABLE_NAME );
        // ��������ʃR�[�h
		m_cControlRel.SetControl( IDC_STATIC_TOLLCLASS, TYPE_OTHER, 	base_attr,	base_attr );
		m_cControlRel.SetControl( IDC_COMBO_TOLLCLASS,		TYPE_COMBO,		base_attr,	base_attr,	highway_node::kTollClass,	NULL,	HIGHWAY_NODE_TABLE_NAME );
        // �ē��������t���O
		m_cControlRel.SetControl( IDC_CHECK_GUIDE,			TYPE_CHECKBOX,	base_attr,	base_attr,	highway_node::kGuide,		NULL,	HIGHWAY_NODE_TABLE_NAME );

		// [Bug6432] ETC�����ʗ����p�̃t�B�[���h�̗L���Ɋւ�炸SetControl()����悤�ɏC��
		// �������@�\�R�[�h
		m_cControlRel.SetControl( IDC_STATIC_TOLLFUNC, TYPE_OTHER, 	base_attr,	base_attr );
		m_cControlRel.SetControl( IDC_COMBO_TOLLFUNC,		TYPE_COMBO,		base_attr,	base_attr, highway_node::kTollFunc,	NULL,	HIGHWAY_NODE_TABLE_NAME,	NULL,	IDC_CHECK_NOETC_CAR, FOLLOW_VALUEIS1,	0 );
		// ��U���������t���O
		m_cControlRel.SetControl( IDC_CHECK_DUMMY,			TYPE_CHECKBOX,	base_attr,	base_attr,	highway_node::kDummyToll,	NULL,	HIGHWAY_NODE_TABLE_NAME,	NULL,	IDC_CHECK_NOETC_CAR, FOLLOW_VALUEIS1,	0 );
		// ��ʎԗ��Ή��t���O
		m_cControlRel.SetControl( IDC_GROUP_NOETC, TYPE_OTHER, 	base_attr,	base_attr );
		m_cControlRel.SetControl( IDC_CHECK_NOETC_CAR,		TYPE_CHECKBOX,	base_attr,	base_attr,	highway_node::kNoEtcCar,		NULL,	HIGHWAY_NODE_TABLE_NAME);
		// ETC�{�݃R�[�h
		m_cControlRel.SetControl( IDC_STATIC_ETC_FACIL, TYPE_OTHER, 	base_attr,	base_attr );
		m_cControlRel.SetControl( IDC_GROUP_ETC, TYPE_OTHER, 	base_attr,	base_attr );
		m_cControlRel.SetControl( IDC_COMBO_ETC_FACIL,		TYPE_COMBO,	base_attr,	base_attr,	highway_node::kEtcFacil,	NULL,	HIGHWAY_NODE_TABLE_NAME);
		// �������@�\�R�[�h(ETC)
		m_cControlRel.SetControl( IDC_STATIC_TOLLFUNC_ETC, TYPE_OTHER, 	base_attr,	base_attr );
		m_cControlRel.SetControl( IDC_COMBO_TOLLFUNC_ETC,	TYPE_COMBO,	base_attr,	base_attr,	highway_node::kEtcTollFunc,	NULL,	HIGHWAY_NODE_TABLE_NAME,	NULL,	IDC_COMBO_ETC_FACIL, FOLLOW_VALUE,	0 );
		// ��U���������t���O(ETC)
		m_cControlRel.SetControl( IDC_CHECK_NOGUIDETOLL_ETC,	TYPE_CHECKBOX,	0,	base_attr,	highway_node::kEtcNoGuideToll,	NULL,	HIGHWAY_NODE_TABLE_NAME,	NULL,	IDC_COMBO_ETC_FACIL, FOLLOW_VALUE,	0 );

		// �����m�[�hID
		m_cControlRel.SetControl( IDC_STATIC_FROMNODE_ID, TYPE_OTHER, 	base_attr,	base_attr );
		m_cControlRel.SetControl( IDC_COMBO_FROMNODE_ID,	TYPE_COMBO,		base_attr,	base_attr, highway_node::kApproachNodeID	,NULL,	HIGHWAY_NODE_TABLE_NAME,	NULL,	IDC_CHECK_ISHWMODE, FOLLOW_VALUEREVERSE,	0 );
        // ���o�m�[�hID
		m_cControlRel.SetControl( IDC_STATIC_TONODE_ID, TYPE_OTHER, 	base_attr,	base_attr );
		m_cControlRel.SetControl( IDC_COMBO_TONODE_ID,		TYPE_COMBO,		base_attr,	base_attr, highway_node::kFlowNodeID,	NULL,	HIGHWAY_NODE_TABLE_NAME,	NULL,	IDC_CHECK_ISHWMODE,	FOLLOW_VALUEREVERSE,	0 );

		// ���������[�����
		m_cControlRel.SetControl( IDC_GROUP_TOLLGATELANE, TYPE_OTHER, 	tollgate_lane,	tollgate_lane );
		m_cControlRel.SetControl( IDC_PICTURE_LANEIMG, TYPE_OTHER, 	tollgate_lane,	tollgate_lane );
		m_cControlRel.SetControl( IDC_EDIT_TOLLGATELANE,	TYPE_EDITSTRING,tollgate_lane,	tollgate_lane,	highway_node::kTollGateLane,	NULL,	HIGHWAY_NODE_TABLE_NAME );

		// ETC�������R�[�h�i�y23-090�z�j
		m_cControlRel.SetControl( IDC_STATIC_ETC_TOLLGATE, TYPE_OTHER, 	hwynode_dialog_tab::etc_tollgate,	hwynode_dialog_tab::etc_tollgate );
		m_cControlRel.SetControl( IDC_EDIT_ETC_TOLLGATE,	TYPE_EDITSTRING,hwynode_dialog_tab::etc_tollgate,	hwynode_dialog_tab::etc_tollgate,	highway_node::kTollgateID,	NULL,	highway_node::kTableName );

		// ���\�[�X
		m_cControlRel.SetControl( IDC_STATIC_SOURCE, TYPE_OTHER, 	base_attr,	tab_end - 1 );
		m_cControlRel.SetControl( IDC_EDIT_SOURCE,			TYPE_EDITSTRING,base_attr,	tab_end -1,	highway_node::kSource,		NULL,	HIGHWAY_NODE_TABLE_NAME );
	}
}

/**
 * @brief �E�B���h�E�}�l�[�W���̃}�b�v�̐؂�ւ����s��
 * @param[in]	cWindowMapIndex	�}�b�v�̃C���f�b�N�X�ԍ�
 */
void CAttrHwyNodeDlg::ChangeWindowMgrMap(int cWindowMapIndex)
{
	switch (cWindowMapIndex)
	{
		case 0:
				m_winMgr.ChangeMgrMap(HWYTOLLNODEMAP);
				break;
		case 1:
				m_winMgr.ChangeMgrMap(HWYTOLLNODEMAP2);
				break;
		case 2:
				m_winMgr.ChangeMgrMap(HWYTOLLNODEMAP3);
				break;
		default:
				break;
	}
	// �Ĕz�u
	m_winMgr.InitToFitSizeFromCurrent(this);
	m_winMgr.CalcLayout(this);
	m_winMgr.SetWindowPositions(this);
}

/**
 * @fn void CAttrHwyNodeDlg::ChangeRoadSeqCombobox()
 * @brief �Q�Ɨp�̘H�����V�[�P���X�̃R���{�{�b�N�X��ݒ肷��
 *
 * Facil_Info_Point���擾���A���̃e�[�u������H���ԍ��Ō������A�Ђ������������̂�H�����V�[�P���X�̃R���{�{�b�N�X�ɓo�^����
 */
void CAttrHwyNodeDlg::ChangeRoadSeqCombobox()
{
	// Facil_Info�e�[�u�����擾���Ă��Ȃ���΁A�擾����
	if( !m_ipFacilInfoTable )
	{
		IWorkspacePtr ipWorkspace;
		ITablePtr ipCurrentTable;
		IFeaturePtr ipCurrentFeature;
		ipCurrentFeature = GetCurrentFeatureDef()->GetFeature();
		if( !ipCurrentFeature )
			return;
		ipCurrentFeature->get_Table( &ipCurrentTable );
		IDatasetPtr ipDataset( ipCurrentTable );
		if( !ipDataset )
			return;
		ipDataset->get_Workspace( &ipWorkspace );
		if( !ipWorkspace )
			return;
		
		// Facil_Info_Point�e�[�u���擾
		CString strOwnerName = AheGetFeatureClassOwnerName( ipCurrentFeature );
		if( strOwnerName.IsEmpty() )
			strOwnerName = _T("Facil_Info_Point");
		else
			strOwnerName += _T(".Facil_Info_Point");
		m_ipFacilInfoTable = AheOpenTableByTableName( ipWorkspace, strOwnerName );
	}
	
	// �擾�����H���ԍ����e�[�u�����猟��
	_ICursorPtr		ipCursor;
	IQueryFilterPtr ipQueryFilter( CLSID_QueryFilter );
	CString strWhereClause, strSubFields;

	// [Bug6217] �H���R�[�h�̃G�f�B�b�g�{�b�N�X�̍폜�ɔ����A�G�f�B�b�g�{�b�N�X�ł͂Ȃ�m_nRoadCode����H���R�[�h���擾����悤�ɏC��
	// [Bug7173] ���P�[���Ɉˑ����Ȃ��������ɏC��
	strWhereClause.Format(_T("ROAD_CODE = %d"), m_nRoadCode );
	strSubFields.Format(_T("NAME_KANJI,ROAD_SEQ"));
	ipQueryFilter->put_WhereClause( CComBSTR(strWhereClause) );
	ipQueryFilter->put_SubFields( CComBSTR(_T("NAME_KANJI,ROAD_SEQ")) );

	if( m_ipFacilInfoTable )
		m_ipFacilInfoTable->Search( ipQueryFilter, VARIANT_FALSE, &ipCursor );
	
	// �H�����V�[�P���X�擾
	CComVariant                vaRoadSequence;
	CFieldDef*                pFieldDef;
	pFieldDef = GetCurrentFeatureDef()->GetFieldDef( HIGHWAY_NODE_TABLE_NAME, _T("ROAD_SEQ") );
	vaRoadSequence = pFieldDef->m_vaValue;
	m_cRoadSeq.Format(_T("%d"), vaRoadSequence.lVal);
	
	// �R���{�{�b�N�X�ɓ���邽�߂̒l���쐬����
	_IRowPtr ipRow;
	std::map<LONG, CString> mapDomain;
	if( ipCursor )
	{
		while( ipCursor->NextRow( &ipRow ) == S_OK )
		{
			if( !ipRow ) 
				break;
			
			CComVariant vaKanjiName, vaRoadSeq, vaKanaName, vaClass;
			LONG lIndex = -1;
			IFieldsPtr ipFields;
			ipRow->get_Fields( &ipFields );
			ipFields->FindField( CComBSTR(_T("NAME_KANJI")), &lIndex );
			ipRow->get_Value( lIndex, &vaKanjiName );
			ipFields->FindField( CComBSTR(_T("ROAD_SEQ")), &lIndex );
			ipRow->get_Value( lIndex, &vaRoadSeq );
			
			CString        strVal;
			strVal.Format(_T("%d"), vaRoadSeq.lVal );        
			
			USES_CONVERSION;
			CString strText;
			strText.Format(_T("%s : %s"), strVal, OLE2T(vaKanjiName.bstrVal) );	// [Bug6217] �\�����@��ύX
			
			mapDomain[vaRoadSeq.lVal] = strText;
		}
		
		// �R���{�{�b�N�X�ɒl������
		CComboBox cCombo( GetDlgItem( IDC_COMBO_SEQUENCE ) );
		cCombo.ResetContent();
		for( std::map<LONG, CString>::iterator it = mapDomain.begin(); it != mapDomain.end(); it++ )
		{
			INT iCurPos = cCombo.AddString( it->second );
			cCombo.SetItemData( iCurPos, it->first );
			if( vaRoadSequence.lVal == it->first )
				cCombo.SetCurSel( iCurPos );
		}
	}
	
	SetRoadSequence();
}

/**
 * @fn void CAttrHwyNodeDlg::SetRoadSequence()
 * @brief �H�����V�[�P���X�G�f�B�^�ɒl���o�͂���
 * @note	[Bug6217] �H�����V�[�P���X�̃G�f�B�b�g�{�b�N�X���폜�����̂ŁA���̊֐��̖����͒l���ύX����Ă��邩�̃`�F�b�N�݂̂ɂȂ���
 */
void CAttrHwyNodeDlg::SetRoadSequence()
{
	// �l���ύX����Ă��邩�m�F
	CFieldDef* pFieldDef;
	pFieldDef = GetCurrentFeatureDef()->GetFieldDef( HIGHWAY_NODE_TABLE_NAME, _T("ROAD_SEQ") );
	pFieldDef->m_vaValue = _ttol(m_cRoadSeq);
	LONG        lDummy = _ttol(m_cRoadSeq);
	if( lDummy != m_lOrgRoadSequence )
	{
		pFieldDef->m_bChanged = TRUE;
		SetButton( Changed() );
	}
}

/**
 * @fn bool CAttrHwyNodeDlg::SetLineClassList(LONG nRoadCode)
 * @brief �_�C�A���O���Ă΂ꂽ�Ƃ��ɘH����ʎQ�Ɨp�̃R���{�{�b�N�X�ɒl���Z�b�g����
 * @param nRoadCode	[in]	�H���R�[�h�̒l
 * @return	�H����ʂ��擾�ł�����TRUE�A�擾�ł��Ȃ�������FALSE
 * @note	[Bug6432] HwyHelper���g��Ȃ��悤�ɏC��
 */
bool CAttrHwyNodeDlg::SetLineClassList(LONG nRoadCode)
{
	// �H����ʃ��X�g�̃R���{�{�b�N�X
	CComboBox cbLineClass = CComboBox(GetDlgItem(IDC_COMBO_HWYNODE_ROADCLASS_REF));
	// �A�C�e���̃N���A
	cbLineClass.ResetContent();
	
	// �H���R�[�h�e�[�u�����擾���Ă��Ȃ���΁A�擾����
	if( !m_ipRoadCodeListTable )
	{
		IWorkspacePtr ipWorkspace;
		ITablePtr ipCurrentTable;
		IFeaturePtr ipCurrentFeature;
		ipCurrentFeature = GetCurrentFeatureDef()->GetFeature();
		if( !ipCurrentFeature )
			return FALSE;
		ipCurrentFeature->get_Table( &ipCurrentTable );
		IDatasetPtr ipDataset( ipCurrentTable );
		if( !ipDataset )
			return FALSE;
		ipDataset->get_Workspace( &ipWorkspace );
		if( !ipWorkspace )
			return FALSE;
		
		// REFERENCE.ROAD_CODE_LIST�e�[�u���擾
		CString strOwnerName = AheGetFeatureClassOwnerName( ipCurrentFeature );
		if( strOwnerName.IsEmpty() )
			strOwnerName = _T("ROAD_CODE_LIST");
		else
			strOwnerName += _T(".ROAD_CODE_LIST");
		m_ipRoadCodeListTable = AheOpenTableByTableName( ipWorkspace, strOwnerName );
	}

	// �^����ꂽ�H���R�[�h�̘H���̘H����ʃR�[�h���擾
	_ICursorPtr		ipCursor;
	IQueryFilterPtr	ipQueryFilter( CLSID_QueryFilter );
	CString strWhereClause, strSubFields;

	strWhereClause.Format(_T("ROAD_CODE = %ld"), nRoadCode );
	strSubFields.Format(_T("LINECLASS_C"));
	ipQueryFilter->put_WhereClause( CComBSTR(strWhereClause) );
	ipQueryFilter->put_SubFields( CComBSTR(strSubFields) );
	m_ipRoadCodeListTable->Search( ipQueryFilter, VARIANT_FALSE, &ipCursor );

	_IRowPtr ipRow;

	long aLineClass = -1;	// ipCursor==NULL�̏ꍇ�ɂ́A���������̂܂�ipRoadClass���Q�Ƃ���ă����^�C���G���[���o��̂ŁA���蓾�Ȃ��l�ŏ��������Ă���
	if( ipCursor )
	{
		while (ipCursor->NextRow(&ipRow) == S_OK)
		{
			// ipCursor�Ɋi�[���ꂽRoad_Code_List�̃��R�[�h�̂����A�ŏ��̂P����LineClass_c���擾
			aLineClass = ipRow->GetValue( ipRow->Fields->_FindField(sindy::schema::road_code_list::kLineClass) );
			break;
		}
	}

	// �H����ʃR�[�h�̃R�[�h�l�h���C�����擾
	ATLASSERT(m_ipRoadCodeListTable);
	IFieldsPtr ipFields(m_ipRoadCodeListTable->Fields);
	IDomainPtr ipDomain(ipFields->GetField(ipFields->_FindField(sindy::schema::road_code_list::kLineClass))->GetDomain());
	ICodedValueDomainPtr ipCodedValueDomain(ipDomain);

	// �R���{�{�b�N�X�ɃA�C�e����ǉ�
	int nCurIdx = 0;
	for (int i = 0; i < ipCodedValueDomain->GetCodeCount(); ++i)
	{
		long aValue	= ipCodedValueDomain->GetValue(i).lVal;
		CString aName = COLE2T(ipCodedValueDomain->GetName(i));

		int nIndex = cbLineClass.AddString(aName);
		if(nIndex != CB_ERR && nIndex != CB_ERRSPACE)
		{
			cbLineClass.SetItemData(nIndex, aValue);
			if(aValue == aLineClass)
				nCurIdx = nIndex;
		}
		else
		{
			AttrMessageBox( _T("�H����ʃR�[�h�̃R���{�{�b�N�X�ݒ莸�s"), _T("�H����ʐݒ莸�s"), MB_OK, NULL);
			return FALSE;
		}
	}
	// �J�[�\���̐ݒ�
	cbLineClass.SetCurSel(nCurIdx);
	// ���݂̘H����ʂ̎擾
	m_nLineClass = cbLineClass.GetItemData(nCurIdx);
	
	CFieldDef* pFieldDef;
	CComVariant vaRoadCode;
	pFieldDef = GetCurrentFeatureDef()->GetFieldDef( HIGHWAY_NODE_TABLE_NAME, _T("ROAD_CODE") );
	vaRoadCode = pFieldDef->m_vaValue;
	
	RedrawRoadNameList(vaRoadCode.lVal);

	// [Bug6217] �n�C�E�F�C�m�[�h��V�K�쐬�������ɁA�H���R���{�{�b�N�X��ύX�����ɂ��̂܂܊m�肷���
	// �H���R�[�h��0�ɂȂ��Ă��܂��̂�h�����߁A�����ŘH���R�[�h���X�V���ꂽ���̂Ƃ���CFieldDef�ɔ��f������
	SetRoadCode(m_nRoadCode);

	return TRUE;
}

/**
 * @fn bool CAttrHwyNodeDlg::GetLineClass(int* pClass)
 * @brief �ݒ肳��Ă���H����ʂ��擾����
 *
 * @param pClass	[out]	�H���R�[�h�̒l
 *
 * @return	�H����ʂ��擾�ł�����TRUE�A�擾�ł��Ȃ�������FALSE
 */
bool CAttrHwyNodeDlg::GetLineClass(int* pClass)
{
	if( !pClass )
		return false;
	// �R���{�{�b�N�X�̎擾
	CComboBox cbLineClass = CComboBox(GetDlgItem(IDC_COMBO_HWYNODE_ROADCLASS_REF));

	// �C���f�b�N�X�̎擾
	int nIndex = cbLineClass.GetCurSel();
	if(nIndex == CB_ERR)
		return false;

	// �l�̎擾
	*pClass = cbLineClass.GetItemData(nIndex);
	return true;
}

/**
 * @fn bool CAttrHwyNodeDlg::SetLineClass(int nClass)
 * @brief �H����ʂ̒l����R���{�{�b�N�X��ݒ�
 *
 * @param nClass	[in]	�H���R�[�h�̒l
 *
 * @return	�H����ʂ��擾�ł�����TRUE�A�擾�ł��Ȃ�������FALSE
 */
bool CAttrHwyNodeDlg::SetLineClass(int nClass)
{
	// �R���{�{�b�N�X�̎擾
	CComboBox cbLineClass = CComboBox(GetDlgItem(IDC_COMBO_HWYNODE_ROADCLASS_REF));
	// �C���f�b�N�X�̌v�Z
	int i = 0;
	int nMaxCount = cbLineClass.GetCount();
	for( ; i < nMaxCount; ++i)
	{
		int nLineClass = cbLineClass.GetItemData(i);
		if(nLineClass == nClass)
			break;
	}
	if(i == nMaxCount)
		return false;
	// �R���{�{�b�N�X�̐ݒ�
	cbLineClass.SetCurSel(i);
	
	return true;
}

/**
 * @fn bool CAttrHwyNodeDlg::RedrawRoadNameList(LONG nRoadCode)
 * @brief �H�����̃R���{�{�b�N�X�̍X�V
 *
 * @param nRoadCode	[in]	�H���R�[�h�̒l
 *
 * @return	�H�����̂��擾�ł�����TRUE�A�擾�ł��Ȃ�������FALSE
 */
bool CAttrHwyNodeDlg::RedrawRoadNameList(LONG nRoadCode)
{
	// [Bug6432] Road_Code_List���ǂݍ��܂�Ă��Ȃ���Γǂݍ���
	if (m_RoadCodeListArray.size() == 0)
	{
		ATLASSERT(m_ipRoadCodeListTable);
		_ICursorPtr ipCursor(m_ipRoadCodeListTable->_Search(IQueryFilterPtr(CLSID_QueryFilter), VARIANT_TRUE));

		// �擾�������t�B�[���h�̃t�B�[���h�C���f�b�N�X�����߂�
		using namespace sindy::schema::road_code_list;
		typedef CString FieldName;
		typedef long FieldIndex;
		typedef std::map<FieldName, FieldIndex> FieldIndexMap;
		FieldIndexMap aFieldIndexMap = boost::assign::map_list_of
			(CString(kRoadCode),	m_ipRoadCodeListTable->_FindField(kRoadCode))
			(CString(kNameKanji),	m_ipRoadCodeListTable->_FindField(kNameKanji))
			(CString(kNameYomi),	m_ipRoadCodeListTable->_FindField(kNameYomi))
			(CString(kLineClass),	m_ipRoadCodeListTable->_FindField(kLineClass));

		// �l���擾���ăR���e�i�Ɋi�[
		for (_IRowPtr ipRow; ipCursor->NextRow(&ipRow) == S_OK;)
		{
			RoadCodeListRecord aRecord;
			aRecord.m_ObjectID = ipRow->GetOID();
			aRecord.m_RoadCode	= ipRow->GetValue(aFieldIndexMap[kRoadCode]);
			VARIANT vaNameKanji = ipRow->GetValue(aFieldIndexMap[kNameKanji]);
			aRecord.m_NameKanji	= vaNameKanji.vt == VT_NULL ? "" : CT2A(vaNameKanji.bstrVal);
			VARIANT vaNameYomi = ipRow->GetValue(aFieldIndexMap[kNameYomi]);
			aRecord.m_NameYomi	= vaNameYomi.vt == VT_NULL ? "" : CT2A(vaNameYomi.bstrVal);
			aRecord.m_LineClass	= ipRow->GetValue(aFieldIndexMap[kLineClass]);
			m_RoadCodeListArray.insert(aRecord);
		}
	}
	
	// �H�����̃R���{�{�b�N�X
	CComboBox cbRoadName = CComboBox(GetDlgItem(IDC_COMBO_HWYNODE_ROADNAME_REF));
	// �R���{�{�b�N�X�̏��̃N���A
	cbRoadName.ResetContent();

	// �R���{�{�b�N�X�ɃA�C�e����ǉ�
	int nCurIdx = 0;
	BOOST_FOREACH (const RoadCodeListRecord& aRecord, m_RoadCodeListArray)
	{
		// �H����ʃR�[�h���A���ݑI�𒆂̘H����ʃR�[�h�ƈقȂ��Ă�����X�L�b�v
		if (aRecord.m_LineClass != m_nLineClass)
			continue;

		CString aRoadNameStr;
		aRoadNameStr.Format(_T("%d : %s"), aRecord.m_RoadCode, CA2T(aRecord.m_NameKanji.c_str()));	// [Bug6217] �H�����̂𕹋L����悤�ɕύX

		int nIndex = cbRoadName.AddString(aRoadNameStr);
		if(nIndex != CB_ERR && nIndex != CB_ERRSPACE)
		{
			cbRoadName.SetItemData(nIndex, aRecord.m_RoadCode);
			if(aRecord.m_RoadCode == nRoadCode)
				nCurIdx = nIndex;
		}
		else
		{
			AttrMessageBox( AheLoadString( IDS_FAILTOSETROUTECLASS ), AheLoadString( IDS_CAPTION ), MB_OK, NULL);
			return false;
		}
	}

	// ���݂̃J�[�\���̐ݒ�
	cbRoadName.SetCurSel(nCurIdx);
	// ���݂̘H���R�[�h�̎擾
	m_nRoadCode = cbRoadName.GetItemData(nCurIdx);
	
	return true;
}

/**
 * @fn void CAttrHwyNodeDlg::SetRoadCode(int nRoadCode)
 * @brief �H���R�[�h�̒l���w�肳�ꂽ�R�[�h�ɒu��������
 *
 * @param nRoadCode	[in]	�H���R�[�h�̒l
 *
 * @note	[Bug6217] �H���R�[�h�̃G�f�B�b�g�{�b�N�X���폜�����̂ŁA���̊֐��̖����͒l���ύX����Ă��邩�̃`�F�b�N�݂̂ɂȂ���
 *			false��Ԃ��\�����Ȃ��Ȃ����̂ŁA�߂�l��bool����void�ɕύX
 */
void CAttrHwyNodeDlg::SetRoadCode(int nRoadCode)
{
	CFieldDef* pFieldDef;
	pFieldDef = GetCurrentFeatureDef()->GetFieldDef( HIGHWAY_NODE_TABLE_NAME, _T("ROAD_CODE") );
	pFieldDef->m_vaValue = nRoadCode;
	if( nRoadCode != m_lOrgRoadCode)
	{
		pFieldDef->m_bChanged = TRUE;        
		SetButton( Changed() );
	}
}

/**
 * @fn BOOL CAttrHwyNodeDlg::ErrorCheck()
 * @brief �G���[���Ȃ����ǂ������ׂ�
 *
 * �_�C�A���O�����ۂɁAAttributeDlgObj����Ă΂��
 *  ��������ʁA�������@�\�̒l�ɃG���[���Ȃ����ǂ������ׂ�B
 *
 * @return	�G���[���Ȃ�������TRUE�A�G���[�������FALSE
 */
BOOL CAttrHwyNodeDlg::ErrorCheck()
{
	using namespace sindy::schema::highway_node;
	for( std::list<CFeatureDef>::iterator it = m_pFeatureDefList->begin(); it != m_pFeatureDefList->end(); it++ )
	{
		CString strTableName = it->GetTableName();
		
		if( strTableName == HIGHWAY_NODE_TABLE_NAME )
		{	
			// ���ݕҏW���̒l���K��
			CFieldDef* pFieldDef = it->GetFieldDef( HIGHWAY_NODE_TABLE_NAME, sindy::schema::category::highway_in_out_table::kInOut );
			long lInOut_c = pFieldDef->m_vaValue.lVal;
			pFieldDef = it->GetFieldDef( HIGHWAY_NODE_TABLE_NAME, kTollClass );
			long lTollClass_c = pFieldDef->m_vaValue.lVal;
			pFieldDef = it->GetFieldDef( HIGHWAY_NODE_TABLE_NAME, kTollFunc );
			long lTollFunc_c = pFieldDef->m_vaValue.lVal;
			
			pFieldDef = it->GetFieldDef( HIGHWAY_NODE_TABLE_NAME, kApproachNodeID );
			CComVariant vaFromNodeID = pFieldDef->m_vaValue;
			pFieldDef = it->GetFieldDef( HIGHWAY_NODE_TABLE_NAME, kFlowNodeID );
			CComVariant vaToNodeID = pFieldDef->m_vaValue;
			pFieldDef = it->GetFieldDef( HIGHWAY_NODE_TABLE_NAME, kHwyMode );
			long lHwyNode = pFieldDef->m_vaValue.lVal;
			
			// [bug7177]���������[�������擾
			CString strTollGateLane;
			if(TollGateLaneFieldExist()){	// �܂��͗��������[�����t�B�[���h�����邩�`�F�b�N
				pFieldDef = it->GetFieldDef( HIGHWAY_NODE_TABLE_NAME, kTollGateLane );
				if(pFieldDef->m_vaValue.vt == VT_BSTR)	// �t�B�[���h�������Ă�NULL�l�������Ă��邽��VT_EMPTY���`�F�b�N
					strTollGateLane = pFieldDef->m_vaValue.bstrVal;
			}

			BOOL bIsError = FALSE;
			
			LONG lOID = 0;
			it->GetFeature()->get_OID(&lOID);

			CString strMessage;
			// [bug7177]HWY�m�[�h�ɑ΂����������[�����𐮔�����ہA�K��l�O�Ōx�����o��
			if(TollGateLaneFieldExist() && !strTollGateLane.IsEmpty()){
				// �w��̕����Q�i0�`5�j�ȊO������ƕ����񒷈ȉ��̏o�͂ƂȂ�
				if(_tcsspn(strTollGateLane, _T("012345")) != strTollGateLane.GetLength()){
					strMessage.Format( AheLoadString( IDS_USABLENUMBER ),lOID);
					AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK, NULL);
					return FALSE;
				}
			}

			// �����A���o�m�[�h�̃`�F�b�N
			if( lHwyNode == 0 )
			{
				// [Bug 5507]�n�C�E�F�C�m�[�h�̗����E���o�m�[�hID�͋�ł͂����Ȃ�
				if( vaFromNodeID.vt == VT_EMPTY || vaToNodeID.vt == VT_EMPTY )
				{
					strMessage.Format(AheLoadString( IDS_INVALIDNODE ), lOID);
					AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK, NULL);
					return FALSE;
				}
				if( vaFromNodeID.lVal == vaToNodeID.lVal )
				{
					strMessage.Format( AheLoadString( IDS_SAMEINOUTNODE ),lOID);
					AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK, NULL);
					return FALSE;
				}
			}
			else if( lHwyNode == 1 )
			{
				if( !(vaFromNodeID.lVal == 0 && vaToNodeID.lVal == 0 ))
				{
					strMessage.Format( AheLoadString( IDS_INVALIDNODE ),lOID);
					AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK, NULL);
					return FALSE;
				}
			}

			// ETC�������R�[�h�̃`�F�b�N
			// �t�B�[���h���݊m�F�Ƃ��Ńl�X�g���[���Ȃ肻���Ȃ̂Ń����_�ɂ��Ă���
			auto isExistTollgateID = [&](){
				// �t�B�[���h���Ȃ���΃X���[
				auto fdTollgateID = it->GetFieldDef( kTableName, kTollgateID );
				if( !fdTollgateID )
					return true;

				// VT_NULL���l��
				CString tollGateID = uh::str_util::ToString(fdTollgateID->m_vaValue);

				// ETC��Ή��Ȃ�ETC�������R�[�h��NULL�łȂ���΃_��
				auto fdFacilC = it->GetFieldDef( kTableName, kEtcFacil );
				if( fdFacilC && fdFacilC->m_vaValue.lVal == etc_facil::kNoEtcFacil )
				{ 
					if( !tollGateID.IsEmpty() )
					{
						// ETC�{�݃R�[�h�uETC��Ή��v�Ȃ̂�ETC�������R�[�h�����͂���Ă��܂�
						AttrMessageBox( 
							uh::str_util::format( AheLoadString( IDS_INVALID_ETC_CODE ), lOID ),
							AheLoadString( IDS_CAPTION ), MB_OK, NULL);
						return false;
					}
				}

				// ETC��Ή��ȊO�ŁAETC�������R�[�h��null�Ȃ�X���[
				if( tollGateID.IsEmpty() )
					return true;

				// ETC_TOLLGATE�e�[�u���ɑ��݂���R�[�h��
				ITablePtr etcTollgateT = AheOpenSameOwnerTable( 
					ITablePtr(AheGetFeatureClass( it->GetFeature() )), etc_tollgate::kTableName );
				if( !etcTollgateT )
				{
					// ETC_TOLLGATE �e�[�u�����J���܂���ł����B
					AttrMessageBox( 
						uh::str_util::format( AheLoadString( IDS_FAILED_OPEN_TOLLGATE ), tollGateID ),
						AheLoadString( IDS_CAPTION ), MB_OK, NULL);
					return false;
				}

				auto query = AheInitQueryFilter( 
					nullptr,
					etc_tollgate::kTollgateID,
					uh::str_util::format( _T("%s = '%s'"), etc_tollgate::kTollgateID, tollGateID ) );

				long num(0);
				etcTollgateT->RowCount( query, &num );
				if(num == 0)
				{
					// ETC_TOLLGATE�ɑ��݂��Ȃ�ETC�������R�[�h�ł��B
					AttrMessageBox( 
						uh::str_util::format( AheLoadString( IDS_NOTEXISTTOLLGATEID ), lOID, tollGateID ),
						AheLoadString( IDS_CAPTION ), MB_OK, NULL);
					return false;
				}

				return true;
			};

			if( !isExistTollgateID() )
				return FALSE;
		}
	}
	return TRUE;
}

/**
 * @brief �^����ꂽ��������ʃR�[�h�̒l���u�{����ɂ�����́v������ȊO���𔻒肷��
 * @note	�{����ɂ���̂́u�{���㗿�����v�u�o���A�v
 * @note	���򍇗��R�[�h�Ɨ�������ʃR�[�h�̐����`�F�b�N�ɕK�v
 * @return	��������ʃR�[�h�̒l���u�{����ɂ�����́v�ł��邩�ۂ�
 */
bool IsMainLineTollClass(long cTollClass)
{
	bool aResult = false;
	using namespace sindy::schema::highway_node::toll_class;
	switch (cTollClass)
	{
		case kMainTollbooth:
		case kBarrier:
			aResult = true;
			break;
		case kNoTollbooth:
		case kRampTollbooth:
		case kGateway:
			break;
		default:
			ATLASSERT(0);
	}

	return aResult;
}

/**
 * @brief �^����ꂽ��������ʃR�[�h�̒l���u��������\�����́v������ȊO���𔻒肷��
 * @note	��������\���̂́u�{���㗿�����v�u�����v�㗿�����v�u�o���A�v
 * @note	��������ʃR�[�h�Ɨ������@�\�R�[�h�̐����`�F�b�N�ɕK�v
 * @return	��������ʃR�[�h�̒l���u��������\�����́v�ł��邩�ۂ�
 */
bool IsTollBoothTollClass(long cTollClass)
{
	bool aResult = false;
	using namespace sindy::schema::highway_node::toll_class;
	switch (cTollClass)
	{
		case kMainTollbooth:
		case kRampTollbooth:
		case kBarrier:
			aResult = true;
			break;
		case kNoTollbooth:
		case kGateway:
			break;
		default:
			ATLASSERT(0);
	}

	return aResult;
}


/**
 * @brief ��ʎԗ��Ή��t���O�ɏ]������R���g���[���Q�̐����`�F�b�N
 * @note	CAttrHwyNodeDlg::CheckItems()�Ŏg�p���邽�߂ɍ쐬
 * @note	�����̂����ACControlDef*�̓`�F�b�N�ΏۃR���g���[���i�G���[���ƐԂ��Ȃ�j�A����ȊO�͂��̔��f��ƂȂ�t�B�[���h�̒l
 * @param[in]	cCarFlag	��ʎԗ��Ή��t���O
 * @param[in]	cTollClass	��������ʃR�[�h
 * @param[in]	cTollFunc	�������@�\�R�[�h
 * @param[in]	cNoGuideToll	��U���������t���O
 * @param[in]	cpTollFuncDef	cTollFunc�ɑΉ�����R���g���[��
 * @param[in]	cpNoGuideTollDef	cNoGuideToll�ɑΉ�����R���g���[��
 */
void CheckDependentControlsOnNoEtcCarFlag(
	bool cNoEtcCarFlag,
	long cTollClass,
	long cTollFunc,
	bool cNoGuideToll,
	CControlDef* cpTollFuncDef,
	CControlDef* cpNoGuideTollDef)
{
	using namespace sindy::schema::highway_node;
	if (cNoEtcCarFlag)
	{
		if (IsTollBoothTollClass(cTollClass))
		{
			using namespace toll_func;
			if (cTollClass == toll_class::kBarrier)
			{
				switch (cTollFunc)
				{
					case kNoFunc:
					case kCheckTicket:
					case kAloneInvalidTicket:
						cpTollFuncDef->SetErrored(FALSE);
						break;
					default:
						cpTollFuncDef->SetErrored(TRUE);
				}
			}
			else
			{
				switch (cTollFunc)
				{
					case kTicket:
					case kAdjust:
					case kAlone:
					case kAloneTicket:
					case kAloneAdjust:
					case kCheckTicket:
					case kAloneInvalidTicket:
					case kAdjustTicket:
						cpTollFuncDef->SetErrored(FALSE);
						break;
					case kNoFunc:
						cpTollFuncDef->SetErrored(TRUE);
						break;
					default:
						ATLASSERT(0);
				}
			}
			cpNoGuideTollDef->SetErrored(FALSE);
		}
		else
		{
			cpTollFuncDef->SetErrored(cTollFunc != toll_func::kNoFunc ? TRUE : FALSE);
			cpNoGuideTollDef->SetErrored(cNoGuideToll ? TRUE : FALSE);
		}
	}
	else
	{
		cpTollFuncDef->SetErrored(cTollFunc != toll_func::kNoFunc ? TRUE : FALSE);
		cpNoGuideTollDef->SetErrored(cNoGuideToll ? TRUE : FALSE);
	}
}

/**
 * @brief ETC�{�݃R�[�h�ɏ]������R���g���[���Q�̐����`�F�b�N
 * @note	CAttrHwyNodeDlg::CheckItems()�Ŏg�p���邽�߂ɍ쐬
 * @note	�����̂����ACControlDef*�̓`�F�b�N�ΏۃR���g���[���i�G���[���ƐԂ��Ȃ�j�A����ȊO�͂��̔��f��ƂȂ�t�B�[���h�̒l
 * @param[in]	cEtcFacilCode	ETC�{�݃R�[�h
 * @param[in]	cTollClass	��������ʃR�[�h
 * @param[in]	cTollFunc	�������@�\�R�[�h(ETC)
 * @param[in]	cNoGuideToll	��U���������t���O(ETC)
 * @param[in]	cpTollFuncDef	cTollFunc�ɑΉ�����R���g���[��
 * @param[in]	cpNoGuideTollDef	cNoGuideToll�ɑΉ�����R���g���[��
 */
void CheckDependentControlsOnEtcFacilCode(
	long cEtcFacilCode,
	long cTollClass,
	long cTollFunc,
	bool cNoGuideToll,
	CControlDef* cpTollFuncDef,
	CControlDef* cpNoGuideTollDef)
{
	using namespace sindy::schema::highway_node;
	switch (cEtcFacilCode)
	{
		using namespace etc_facil;
		case kNoEtcFacil:
			cpTollFuncDef->SetErrored(cTollFunc != toll_func::kNoFunc ? TRUE : FALSE);
			cpNoGuideTollDef->SetErrored(cNoGuideToll ? TRUE : FALSE);
			break;
		case kGate:
			switch (cTollClass)
			{
				using namespace toll_class;
				case kMainTollbooth:
					cpTollFuncDef->SetErrored(FALSE);
					cpNoGuideTollDef->SetErrored(FALSE);
					break;
				case kRampTollbooth:
					cpTollFuncDef->SetErrored(cTollFunc == toll_func::kNoFunc ? TRUE : FALSE);
					cpNoGuideTollDef->SetErrored(FALSE);
					break;
				case kNoTollbooth:
				case kGateway:
					cpTollFuncDef->SetErrored(cTollFunc == toll_func::kNoFunc ? FALSE : TRUE);
					cpNoGuideTollDef->SetErrored(cNoGuideToll ? TRUE : FALSE);
					break;
				case kBarrier:
					switch (cTollFunc)
					{
						using namespace toll_func;
						case kNoFunc:
						case kCheckTicket:
						case kAloneInvalidTicket:
							cpTollFuncDef->SetErrored(FALSE);
							break;
						default:
							cpTollFuncDef->SetErrored(TRUE);
					}
					cpNoGuideTollDef->SetErrored(FALSE);
					break;
				default:
					ATLASSERT(0);
			}
			break;
		case kAntenna:
			switch (cTollClass)
			{
				using namespace toll_class;
				case kMainTollbooth:
				case kRampTollbooth:
					cpTollFuncDef->SetErrored(cTollFunc == toll_func::kNoFunc ? TRUE : FALSE);
					cpNoGuideTollDef->SetErrored(FALSE);
					break;
				case kNoTollbooth:
				case kGateway:
					cpTollFuncDef->SetErrored(cTollFunc == toll_func::kNoFunc ? FALSE : TRUE);
					cpNoGuideTollDef->SetErrored(cNoGuideToll ? TRUE : FALSE);
					break;
				case kBarrier:
					switch (cTollFunc)
					{
						using namespace toll_func;
						case kNoFunc:
						case kCheckTicket:
						case kAloneInvalidTicket:
							cpTollFuncDef->SetErrored(FALSE);
							break;
						default:
							cpTollFuncDef->SetErrored(TRUE);
					}
					cpNoGuideTollDef->SetErrored(FALSE);
					break;
				default:
					ATLASSERT(0);
			}
			break;
		default:
			ATLASSERT(0);
	}
}

/**
 * @fn void CAttrHwyNodeDlg::CheckItems()
 * @brief �����֌W�ŃG���[���Ȃ����ǂ������ׂ�
 *
 * ���򍇗��R�[�h�ƁA�����֌W�̒l�ŕs�������Ȃ����ǂ������ׂ�A�G���[�̍ۂɂ̓Z����ԐF�ɂ���
 * �ڂ�����[Bug1460][Bug6217]���Q�Ƃ̂���
 */
void CAttrHwyNodeDlg::CheckItems()
{
	using namespace sindy::schema::highway_node;

    LONG lInOut_c			= GetCurrentFeatureDef()->GetFieldDef( HIGHWAY_NODE_TABLE_NAME, kInOut )->m_vaValue.lVal;
    LONG lTollClass_c		= GetCurrentFeatureDef()->GetFieldDef( HIGHWAY_NODE_TABLE_NAME, kTollClass )->m_vaValue.lVal;
    LONG lTollFunc_c		= GetCurrentFeatureDef()->GetFieldDef( HIGHWAY_NODE_TABLE_NAME, kTollFunc )->m_vaValue.lVal;
	bool bNoGuideToll		= GetCurrentFeatureDef()->GetFieldDef( HIGHWAY_NODE_TABLE_NAME, kDummyToll )->m_vaValue.lVal != 0;

	// ��������͑Ή��\�ǂ���ɒ��Ƀ`�F�b�N�E�E�E[Bug1460] �� [Bug6217]�Ń`�F�b�N��ǉ��E�C��

	// ���򍇗��R�[�h�Ɨ�������ʃR�[�h
	CControlDef* pTollClassDef = m_cControlRel.GetControlDef(::GetDlgItem(m_hWnd, IDC_COMBO_TOLLCLASS));
	switch (lInOut_c)
	{
		case in_out::kInOut:
			pTollClassDef->SetErrored(IsMainLineTollClass(lTollClass_c) ? FALSE : TRUE);
			break;
		case in_out::kIn:
		case in_out::kOut:
			pTollClassDef->SetErrored(IsMainLineTollClass(lTollClass_c) ? TRUE : FALSE);
			break;
		default:
			ATLASSERT(0);
	}

	if (EtcSpecificTollFieldsExist())
	{
		bool bNoEtcCar			= GetCurrentFeatureDef()->GetFieldDef( HIGHWAY_NODE_TABLE_NAME, kNoEtcCar )->m_vaValue.lVal != 0;
		LONG lEtcFacil			= GetCurrentFeatureDef()->GetFieldDef( HIGHWAY_NODE_TABLE_NAME, kEtcFacil )->m_vaValue.lVal;
		LONG lEtcTollFunc_c		= GetCurrentFeatureDef()->GetFieldDef( HIGHWAY_NODE_TABLE_NAME, kEtcTollFunc )->m_vaValue.lVal;
		bool bEtcNoGuideToll	= GetCurrentFeatureDef()->GetFieldDef( HIGHWAY_NODE_TABLE_NAME, kEtcNoGuideToll )->m_vaValue.lVal != 0;

		// ��������ʃR�[�h�ƈ�ʎԗ��Ή��t���O��ETC�{�݃R�[�h
		CControlDef* pNoEtcCarDef = m_cControlRel.GetControlDef(::GetDlgItem(m_hWnd, IDC_CHECK_NOETC_CAR));
		CControlDef* pEtcFacilDef = m_cControlRel.GetControlDef(::GetDlgItem(m_hWnd, IDC_COMBO_ETC_FACIL));
		switch (lTollClass_c)
		{
			using namespace toll_class;
			using namespace etc_facil;
			case kNoTollbooth:
			case kGateway:
				pNoEtcCarDef->SetErrored(bNoEtcCar ? TRUE : FALSE );
				pEtcFacilDef->SetErrored(lEtcFacil != kNoEtcFacil ? TRUE : FALSE);
				break;
			case kMainTollbooth:
			case kRampTollbooth:
			case kBarrier:
				if (! bNoEtcCar && lEtcFacil == kNoEtcFacil)
				{
					pNoEtcCarDef->SetErrored(TRUE);
					pEtcFacilDef->SetErrored(TRUE);
				}
				else
				{
					pNoEtcCarDef->SetErrored(FALSE);
					pEtcFacilDef->SetErrored(FALSE);
				}
				break;
			default:
				ATLASSERT(0);
		}

		// ��ʎԗ��Ή��t���O������ɏ]������R���g���[���Ɨ�������ʃR�[�h
		CheckDependentControlsOnNoEtcCarFlag(
			bNoEtcCar,
			lTollClass_c,
			lTollFunc_c,
			bNoGuideToll,
			m_cControlRel.GetControlDef(::GetDlgItem(m_hWnd, IDC_COMBO_TOLLFUNC)),
			m_cControlRel.GetControlDef(::GetDlgItem(m_hWnd, IDC_CHECK_NOGUIDETOLL))
		);

		// ETC�{�݃R�[�h������ɏ]������R���g���[���Ɨ�������ʃR�[�h
		CheckDependentControlsOnEtcFacilCode(
			lEtcFacil,
			lTollClass_c,
			lEtcTollFunc_c,
			bEtcNoGuideToll,
			m_cControlRel.GetControlDef(::GetDlgItem(m_hWnd, IDC_COMBO_TOLLFUNC_ETC)),
			m_cControlRel.GetControlDef(::GetDlgItem(m_hWnd, IDC_CHECK_NOGUIDETOLL_ETC))
		);
	}
	else	// [Bug6198]�̃f�[�^���f���ύX���K�p����Ă��Ȃ��f�[�^�x�[�X�̏ꍇ�́A�]���̕��@�Ń`�F�b�N
	{
		CControlDef* pTollFuncDef = m_cControlRel.GetControlDef(::GetDlgItem(m_hWnd, IDC_COMBO_TOLLFUNC_ETC));

		// ��������ʃR�[�h�Ɨ������@�\�R�[�h
		CheckDependentControlsOnNoEtcCarFlag(
			true,			// ��ʎԗ��Ή��t���O�����ON�̏ꍇ�ƌ��Ȃ�
			lTollClass_c,
			lTollFunc_c,
			bNoGuideToll,
			m_cControlRel.GetControlDef(::GetDlgItem(m_hWnd, IDC_COMBO_TOLLFUNC)),
			m_cControlRel.GetControlDef(::GetDlgItem(m_hWnd, IDC_CHECK_NOGUIDETOLL))
		);
	}

	// [bug7177]���������[�����̃G�f�B�b�g�{�b�N�X�`�F�b�N
	// �w��l�ȊO�Ȃ�ԐF�ɂ���
	if(TollGateLaneFieldExist()){
		_TCHAR aTollgateLane[m_TollGateLaneStrMaxLength + 1] = {0};
		CEdit cTollGateLaneEdit = GetDlgItem(IDC_EDIT_TOLLGATELANE);
		cTollGateLaneEdit.GetWindowText(aTollgateLane, m_TollGateLaneStrMaxLength + 1);
		CString strTollGateLane(aTollgateLane);
		if(_tcsspn(strTollGateLane, _T("012345")) != strTollGateLane.GetLength())
			m_cControlRel.GetControlDef(::GetDlgItem(m_hWnd, IDC_EDIT_TOLLGATELANE))->SetErrored(TRUE);
		else
			m_cControlRel.GetControlDef(::GetDlgItem(m_hWnd, IDC_EDIT_TOLLGATELANE))->SetErrored(FALSE);
	}
}

/**
 * @brief �S�ẴR���g���[����ҏW�s��Ԃɂ���
 * @note	CAttrHwyNodeDlg::SetInOutNodeComboBox()���ɂR��o�Ă����̂Ŋ֐���
 * @noet	�����E���o�m�[�h�̃R���{�{�b�N�X���܂߂āA�S�Ă̕ҏW�ΏۃR���g���[����Disable�ɂ���
 */
void CAttrHwyNodeDlg::DisableAllControls()
{	
	SetDlgStatus( IDC_CHECK_ISHWMODE, FALSE );
	SetDlgStatus( IDC_COMBO_HWYNODE_ROADCLASS_REF, FALSE );
	SetDlgStatus( IDC_COMBO_HWYNODE_ROADNAME_REF, FALSE );
	SetDlgStatus( IDC_COMBO_SEQUENCE, FALSE );
	SetDlgStatus( IDC_EDIT_HWYGUIDE_NUM, FALSE );
	SetDlgStatus( IDC_COMBO_DIRECTION, FALSE );
	SetDlgStatus( IDC_COMBO_JUNCTION, FALSE );
	SetDlgStatus( IDC_CHECK_DUMMY, FALSE );
	SetDlgStatus( IDC_CHECK_GUIDE, FALSE );
	SetDlgStatus( IDC_COMBO_TOLLCLASS, FALSE );
	SetDlgStatus( IDC_COMBO_TOLLFUNC, FALSE );
	SetDlgStatus( IDC_EDIT_SOURCE, FALSE, TRUE );
	SetDlgStatus( IDC_CHECK_NOETC_CAR, FALSE );
	SetDlgStatus( IDC_COMBO_ETC_FACIL, FALSE );
	SetDlgStatus( IDC_COMBO_TOLLFUNC_ETC, FALSE );
	SetDlgStatus( IDC_CHECK_NOGUIDETOLL_ETC, FALSE );
	SetDlgStatus( IDC_COMBO_FROMNODE_ID, FALSE );
	SetDlgStatus( IDC_COMBO_TONODE_ID, FALSE );
	SetDlgStatus( IDC_EDIT_TOLLGATELANE, FALSE );
}

/**
 * @fn void CAttrHwyNodeDlg::SetInOutNodeComboBox()
 * @brief ���o�A�����m�[�h�̃R���{�{�b�N�X�ɁA�ڑ����Ă���m�[�h�̈ꗗ���Z�b�g����
 *
 * �n�C�E�F�C�m�[�h�̉��ɂ���A���H�m�[�h�Ɛڑ����Ă��铹�H�����N�Ɛڑ����Ă��铹�H�m�[�h���R���{�{�b�N�X�ɓ����
 */
void CAttrHwyNodeDlg::SetInOutNodeComboBox()
{
	m_ipFromRoadGeom = NULL;
	m_ipToRoadGeom = NULL;
	m_ipDummyFrom = NULL;
	m_ipDummyTo = NULL;
	
	// �`�F�b�N�{�b�N�X�̏�Ԏ擾
	CButton	cHwyCheckBox = CButton(GetDlgItem(IDC_CHECK_ISHWMODE));
	INT iCheckBox = cHwyCheckBox.GetCheck();
	
	// ���ݑI�𒆂̃t�B�[�`�����擾
	CFeatureDef* ipFeatureDef = GetCurrentFeatureDef();
	IFeaturePtr ipFeature = ipFeatureDef->GetFeature();
	// �W�I���g���擾
	IGeometryPtr ipGeometry;
	ipFeature->get_Shape( &ipGeometry );
	
	// ���݂̗����A���o�m�[�h���擾
	CFieldDef* pFieldDef;
	pFieldDef = ipFeatureDef->GetFieldDef( HIGHWAY_NODE_TABLE_NAME, _T("APPROACHNODE_ID"));
	long lFromNodeID = pFieldDef->m_vaValue.lVal;
	pFieldDef = ipFeatureDef->GetFieldDef( HIGHWAY_NODE_TABLE_NAME, _T("FLOWNODE_ID"));
	long lToNodeID = pFieldDef->m_vaValue.lVal;
	
	// �����m�[�h�R���{�{�b�N�X
	CComboBox cFromNodeID = CComboBox(GetDlgItem(IDC_COMBO_FROMNODE_ID));
	CComboBox cToNodeID = CComboBox(GetDlgItem(IDC_COMBO_TONODE_ID));
	// �R���{�{�b�N�X�̏��̃N���A
	cFromNodeID.ResetContent();
	cToNodeID.ResetContent();
	
	if( iCheckBox == 0 )
	{
		if( ipGeometry )
		{			
			// �m�[�h���C�����擾
			ILayerPtr ipLayer;
			CString strOwnerName = AheGetFeatureClassOwnerName( ipFeature );
			if( strOwnerName.IsEmpty() )
				strOwnerName = _T("ROAD_NODE");
			else
			{
				strOwnerName += _T(".");
				strOwnerName += _T("ROAD_NODE");
			}
			ipLayer = AheGetLayerByName( m_cArcHelper.GetMap(), strOwnerName, _FIND_TABLE, FALSE, TRUE );
			// ���C�����擾�ł��Ȃ������ꍇ�ɂ́A�p�[�\�i���W�I�f�[�^�x�[�X�̉\������
			if( ipLayer == NULL )
				AheGetLayerByTableName( m_cArcHelper.GetMap(), _T("Road_Node"), &ipLayer );
			
			if ( ipLayer )
			{
				CTargetItems cItems;
				// �n�C�E�F�C�̉��̃m�[�h�t�B�[�`���擾
				AheSelectByShapeFromLayer( ipGeometry, cItems, ipLayer );

				LONG lCount = cItems.size();
					
				if( lCount == 0 )
				{
					AttrMessageBox( AheLoadString( IDS_NOTONNODE ), AheLoadString( IDS_CAPTION ),MB_OK, NULL);
					
					// �Ƃ肠�����A�����݂̗����A���o�m�[�h��\�����Ă���
					CString strOID;
					strOID.Format(_T("%ld"),lToNodeID );
					int iComboNum = cToNodeID.AddString(strOID);
					cToNodeID.AddString(strOID);
					cToNodeID.SetItemData(iComboNum,lToNodeID);
					cToNodeID.SetCurSel(iComboNum);
					SetDlgStatus( IDC_COMBO_FROMNODE_ID, FALSE );
					
					strOID.Format(_T("%ld"),lFromNodeID );
					iComboNum = cFromNodeID.AddString(strOID);
					cFromNodeID.AddString(strOID);
					cFromNodeID.SetItemData(iComboNum,lFromNodeID);
					cFromNodeID.SetCurSel(iComboNum);
					SetDlgStatus( IDC_COMBO_TONODE_ID, FALSE );
					
					//���ׂĂ̑�����ҏW�s��Ԃɂ���
					DisableAllControls();
				}
				// �ʏ�n�C�E�F�C�m�[�h�̉��ɂ�1�̃m�[�h������
				else if( lCount == 1 )
				{
					// �t�B�[�`�������o��
					m_ipNodeUnderHwyNode = (cItems.begin())->GetFeature();
					LONG lNodeIDonHwyNode;
					m_ipNodeUnderHwyNode->get_OID( &lNodeIDonHwyNode );
					
					CTargetItems cTargetRoads;
					AheGetConnectedRoad( m_ipNodeUnderHwyNode, cTargetRoads );
					
					for(CTargetItems::iterator iRoad = cTargetRoads.begin(); iRoad != cTargetRoads.end(); iRoad++)
					{
						// �n�C�E�F�C�m�[�h�ɐڑ����Ă��铹�H���擾
						IFeaturePtr	ipRoadFeature = iRoad->GetFeature();
						
						CTargetItems cNodeItems;
						// ���H�����N�ɂ��Ă���m�[�h���擾
						AheGetConnectedNode( ipRoadFeature, cNodeItems );
						
						for(CTargetItems::iterator iNode = cNodeItems.begin(); iNode != cNodeItems.end(); iNode++ )
						{
							IFeaturePtr ipConnectedNode = iNode->GetFeature();
							LONG lOID = 0;
							ipConnectedNode->get_OID( &lOID );
							
							if( lOID != lNodeIDonHwyNode )
							{
								CString strOID;
								strOID.Format(_T("%ld"),lOID );
								
								int iComboNum = cFromNodeID.AddString(strOID);
								cToNodeID.AddString(strOID);
								cFromNodeID.SetItemData(iComboNum,lOID);
								cToNodeID.SetItemData(iComboNum,lOID);
								
								if( lFromNodeID == lOID )
								{
									cFromNodeID.SetCurSel(iComboNum);
									m_cControlRel.ControlChanged( IDC_COMBO_FROMNODE_ID );
								}
								if( lToNodeID == lOID )
								{
									cToNodeID.SetCurSel(iComboNum);
									m_cControlRel.ControlChanged( IDC_COMBO_TONODE_ID );
								}
							}
						}
					}
				}
			}
			else
			{
				AttrMessageBox( AheLoadString( IDS_CANTEDITATTRIBUTE ), AheLoadString( IDS_CAPTION ), MB_OK, NULL);
				
				// �Ƃ肠�����A�����݂̗����A���o�m�[�h��\�����Ă���
				CString strOID;
				strOID.Format(_T("%ld"),lToNodeID );
				int iComboNum = cToNodeID.AddString(strOID);
				cToNodeID.AddString(strOID);
				cToNodeID.SetItemData(iComboNum,lToNodeID);
				cToNodeID.SetCurSel(iComboNum);
				SetDlgStatus( IDC_COMBO_FROMNODE_ID, FALSE );
				
				strOID.Format(_T("%ld"),lFromNodeID );
				iComboNum = cFromNodeID.AddString(strOID);
				cFromNodeID.AddString(strOID);
				cFromNodeID.SetItemData(iComboNum,lFromNodeID);
				cFromNodeID.SetCurSel(iComboNum);
				SetDlgStatus( IDC_COMBO_TONODE_ID, FALSE );
				
				//���ׂĂ̑�����ҏW�s��Ԃɂ���
				DisableAllControls();
			}
		}
	}
	// �n�C�E�F�C���[�h�t���O���I���̂Ƃ��i�����A���o�t���Odisable�j
	else
	{
		CString strOID;
		strOID.Format(_T("%ld"),0 );
		
		int iComboNum = cFromNodeID.AddString(strOID);
		cToNodeID.AddString(strOID);
		cFromNodeID.SetItemData(iComboNum,0);
		cToNodeID.SetItemData(iComboNum,0);
		cFromNodeID.SetCurSel(iComboNum);
		cToNodeID.SetCurSel(iComboNum);
		m_cControlRel.ControlChanged( IDC_COMBO_FROMNODE_ID );
		m_cControlRel.ControlChanged( IDC_COMBO_TONODE_ID );
		// [Bug 5509]�֘A�@�����ŗ����E���o�m�[�h�R���{�{�b�N�X�𐧌䂵�Ȃ��Ƃ����Ȃ�
		// ���܂܂�OnCommand�ł���Ă������A��{��ControlRel�ł���Ă��炤���Ƃɂ����̂ŁA�����ł��Ȃ��Ƃ����Ȃ��Ȃ���
		SetDlgStatus( IDC_COMBO_FROMNODE_ID, FALSE );
		SetDlgStatus( IDC_COMBO_TONODE_ID, FALSE );
		
		if( lFromNodeID != 0 || lToNodeID != 0 )
			m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL );
		
		if( !m_ipNodeUnderHwyNode )
		{
			AttrMessageBox( AheLoadString( IDS_NOTONNODE ), AheLoadString( IDS_CAPTION ),MB_OK, NULL);
			//���ׂĂ̑�����ҏW�s��Ԃɂ���
			DisableAllControls();
		}
	}
}

/**
 * @fn void CAttrHwyNodeDlg::DrawInOutNode()
 * @brief ���o�A�����m�[�h�֖���`�悷��
 */
void CAttrHwyNodeDlg::DrawInOutNode()
{
	// ���H�ƃm�[�h�̃��C�����\������Ă��邩�ǂ����`�F�b�N
	ILayerPtr ipRoadLayer, ipNodeLayer;

	// �m�[�h���C�����擾
	CString strOwnerName = AheGetFeatureClassOwnerName( GetCurrentFeatureDef()->GetFeature() );
	if( strOwnerName.IsEmpty() )
		strOwnerName = _T("ROAD_NODE");
	else
	{
		strOwnerName += _T(".");
		strOwnerName += _T("ROAD_NODE");
	}
	ipNodeLayer = AheGetLayerByName( m_cArcHelper.GetMap(), strOwnerName, _FIND_TABLE, FALSE, TRUE );

	// ���H�����N���C�����擾
	strOwnerName = AheGetFeatureClassOwnerName( GetCurrentFeatureDef()->GetFeature() );
	if( strOwnerName.IsEmpty() )
		strOwnerName = _T("ROAD_LINK");
	else
	{
		strOwnerName += _T(".");
		strOwnerName += _T("ROAD_LINK");
	}
	ipRoadLayer = AheGetLayerByName( m_cArcHelper.GetMap(), strOwnerName, _FIND_TABLE, FALSE, TRUE );

	// ���ݑI������Ă��郌�C�����\������Ă��Ȃ���΃L�����Z��
	VARIANT_BOOL vaIsVisible = VARIANT_FALSE;
	
	if( !ipRoadLayer )
	{
		AttrMessageBox( AheLoadString( IDS_NOTEXISTROADLAYER ), AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_LOW );
		m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL );
		
		return;
	}
	ipRoadLayer->get_Visible( &vaIsVisible );
	if( !vaIsVisible )
	{
		AttrMessageBox( AheLoadString( IDS_NOTDISPLAYROADLAYER ) , AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_LOW );
		m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL );
		
		return;
	}

	if( !ipNodeLayer )
	{
		AttrMessageBox( AheLoadString( IDS_NOTEXISTNODELAYER ), AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_LOW );
		m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL );
		
		return;
	}
	ipNodeLayer->get_Visible( &vaIsVisible );
	if( !vaIsVisible )
	{
		AttrMessageBox( AheLoadString( IDS_NOTDISPLAYNODELAYER ), AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_LOW );
		m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL );
		
		return;
	}
	

	// �m�[�h�e�[�u�����擾���Ă��Ȃ���΁A�擾����
	if( !m_ipRoadNodeTable )
	{
		IWorkspacePtr ipWorkspace;
		ITablePtr ipCurrentTable;
		IFeaturePtr ipCurrentFeature;
		ipCurrentFeature = GetCurrentFeatureDef()->GetFeature();
		if( !ipCurrentFeature )
			return;
		ipCurrentFeature->get_Table( &ipCurrentTable );
		IDatasetPtr ipDataset( ipCurrentTable );
		if( !ipDataset )
			return;
		ipDataset->get_Workspace( &ipWorkspace );
		if( !ipWorkspace )
			return;

		// NODE_TABLE_NAME�擾
		CString strOwnerName = AheGetFeatureClassOwnerName( ipCurrentFeature );
		if( strOwnerName.IsEmpty() )
			strOwnerName = _T("ROAD_NODE");
		else
			strOwnerName += _T(".ROAD_NODE");

		m_ipRoadNodeTable = AheOpenTableByTableName( ipWorkspace, strOwnerName );
	}
	
	// ���ݑI�𒆂̃t�B�[�`�����擾
	CFeatureDef* ipFeatureDef = GetCurrentFeatureDef();
	
	// ���݂̗����A���o�m�[�h���擾
	CFieldDef* pFieldDef = ipFeatureDef->GetFieldDef( HIGHWAY_NODE_TABLE_NAME, _T("APPROACHNODE_ID"));
	long lFromNodeID = pFieldDef->m_vaValue.lVal;
	pFieldDef = ipFeatureDef->GetFieldDef( HIGHWAY_NODE_TABLE_NAME, _T("FLOWNODE_ID"));
	long lToNodeID = pFieldDef->m_vaValue.lVal;
	
	
	if( lFromNodeID > 0 && lFromNodeID == lToNodeID )
	{
		m_ipFromRoadGeom = NULL;
		m_ipToRoadGeom = NULL;
		
		m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL );
		
		AttrMessageBox( AheLoadString( IDS_SAMEINOUTNODEID ), AheLoadString( IDS_CAPTION ),MB_OK, NULL );
		return;
	}
	
	// �I�����ꂽ�m�[�h�̃W�I���g���擾
	// �����̓��H�W�I���g���擾
	m_ipFromRoadGeom = GetConnectedRoad( lFromNodeID, TRUE, ipFeatureDef );
	// ���o�̓��H�W�I���g���擾
	m_ipToRoadGeom = GetConnectedRoad( lToNodeID, FALSE, ipFeatureDef );

	// �����A���o���C���`��
	Refresh();
}

/**
 * @fn IGeometryPtr CAttrHwyNodeDlg::GetConnectedRoad( LONG lNodeID, CFeatureDef* ipFeatureDef )
 * @brief �����A���o�̓��H�����N�̃W�I���g�����擾����
 *
 * @param lNodeID			[in]	���o�A�܂��͗����m�[�hID
 * @param bIsIN				[in]	���o���������i�����̎�TRUE�j
 * @param ipFeatureDef		[in]	�n�C�E�F�C�m�[�h�̃t�B�[�`���f�t
 *
 * @return ipRoadGeometry	���o�A�܂��͗����̓��H�̃W�I���g��
 */
IGeometryPtr CAttrHwyNodeDlg::GetConnectedRoad( LONG lNodeID, BOOL bIsIN, CFeatureDef* ipFeatureDef)
{
    _ICursorPtr			ipCursor;
    IQueryFilterPtr		ipQueryFilter( CLSID_QueryFilter );
    CString			strWhereClause, strSubFields;
	// �I�u�W�F�N�gID���瓹�H������
	strWhereClause.Format(_T("OBJECTID = %ld"), lNodeID );
	strSubFields.Format(_T("OBJECTID,Shape"));
	
	ipQueryFilter->put_WhereClause( CComBSTR(strWhereClause) );
	ipQueryFilter->put_SubFields( CComBSTR(strSubFields) );
	m_ipRoadNodeTable->Search( ipQueryFilter, VARIANT_FALSE, &ipCursor );
	
    _IRowPtr ipRow = NULL;
    CComVariant vaRoad_Class(_T(""));

	IGeometryPtr	ipRoadGeometry;
    // �m�[�h���擾
    if( ipCursor )
    {
        while( ipCursor->NextRow( &ipRow ) == S_OK )
        {
            if( !ipRow ) 
				break;
			
			// �R���{�{�b�N�X�Őݒ肵���m�[�h���擾
			IFeaturePtr	ipSelectedNode;
			ipSelectedNode = ipRow;
			
			LONG lSelectedNodeOID = 0;
			ipSelectedNode->get_OID(&lSelectedNodeOID);
			ipRow->get_OID(&lSelectedNodeOID);
			IGeometryPtr ipGeometry;
			ipSelectedNode->get_Shape( &ipGeometry );
			
			// �n�C�E�F�C�m�[�h�̉��ɂ���m�[�h�̃I�u�W�F�N�gID�擾
			// ���ݑI�𒆂̃t�B�[�`�����擾
			IFeaturePtr ipFeature = ipFeatureDef->GetFeature();
			
			LONG lNodeUnderHwyNodeOID = 0;
			ipFeature->get_OID(&lNodeUnderHwyNodeOID);
			
			// �n�C�E�F�C�m�[�h���m�[�h�̏�ɂȂ����Ƃ�����̂ŁA���̏ꍇ�͈ȉ���if���ɂ��ꂽ�炾��
			if( m_ipNodeUnderHwyNode )
			{
				CTargetItems cTargetRoads;
				AheGetConnectedRoad( m_ipNodeUnderHwyNode, cTargetRoads);
				
				IFeaturePtr ipSelectedRoad;
				BOOL bIsFromNodeWithSelectedRoad = FALSE;
				for(CTargetItems::iterator iRoad = cTargetRoads.begin(); iRoad != cTargetRoads.end(); iRoad++)
				{
					// �n�C�E�F�C�m�[�h�ɐڑ����Ă��铹�H���擾
					IFeaturePtr	ipRoadFeature = iRoad->GetFeature();
					
					IFieldsPtr	ipFields;
					LONG		lIndex = 0;
					CComVariant	vaValue;
					
					// ���H��From�m�[�h���擾
					ipRoadFeature->get_Fields( &ipFields );
					ipFields->FindField(CComBSTR(_T("FROM_NODE_ID")),&lIndex);
					ipRoadFeature->get_Value( lIndex, &vaValue );
					long lFromNodeID = vaValue.lVal;
					
					// ���H��To�m�[�h���擾
					ipFields->FindField(CComBSTR(_T("TO_NODE_ID")),&lIndex);
					ipRoadFeature->get_Value( lIndex, &vaValue );
					long lToNodeID = vaValue.lVal;
					
					if( lSelectedNodeOID == lFromNodeID )
					{
						if( bIsIN )
							bIsFromNodeWithSelectedRoad = TRUE;
						else
							bIsFromNodeWithSelectedRoad = FALSE;

						ipSelectedRoad = ipRoadFeature;
						break;
					}
					else if( lSelectedNodeOID == lToNodeID )
					{
						if( bIsIN )
							bIsFromNodeWithSelectedRoad = FALSE;
						else
							bIsFromNodeWithSelectedRoad = TRUE;

						ipSelectedRoad = ipRoadFeature;
						break;
					}
				}
				IGeometryPtr ipRoadGeom;
				ipSelectedRoad->get_ShapeCopy( &ipRoadGeom );
				
				if( bIsFromNodeWithSelectedRoad )
					ipRoadGeometry = ipRoadGeom;
				else
				{					
					IPolylinePtr ipPolyline;
					ipPolyline = ipRoadGeom;
					
					ipPolyline->ReverseOrientation();
					ipRoadGeometry = ipPolyline;
				}
			}
		}
    }
	return ipRoadGeometry;
}

/**
 * @fn void CAttrHwyNodeDlg::Refresh()
 * @brief �`������t���V������
 */
void CAttrHwyNodeDlg::Refresh()
{
	if( m_ipFromRoadGeom )
	{
		AheDrawArrowline( m_cArcHelper.GetScreenDisplay(), m_ipFromRoadGeom, RGB( 0, 255, 0 ), TRUE, TRUE );
		if( m_ipFromRoadGeom != m_ipDummyFrom && m_ipFromRoadGeom != m_ipToRoadGeom )
		{
			m_ipDummyFrom = m_ipFromRoadGeom;
			m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL );
		}
	}
	if( m_ipToRoadGeom )
	{
		AheDrawArrowline( m_cArcHelper.GetScreenDisplay(), m_ipToRoadGeom, RGB( 0, 255, 255 ), TRUE, TRUE );
		if( m_ipToRoadGeom != m_ipDummyTo  && m_ipFromRoadGeom != m_ipToRoadGeom )
		{
			m_ipDummyTo = m_ipToRoadGeom;
			m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL );
		}
	}
}

/**
 * @brief ��ʎԗ��Ή��t���O�̏�Ԃɂ���āA�ˑ�����R���g���[����Enable/Disable��؂�ւ���
 * @note	[Bug 6217]�Œǉ�
 */
void CAttrHwyNodeDlg::ChangeDependentControlsOnNoEtcCarFlag()
{
	// ETC�����ʗ����֘A�̃t�B�[���h�����݂��Ȃ��ꍇ�͉������Ȃ�
	if (! EtcSpecificTollFieldsExist())
		return;

	// ��ʎԗ��Ή��t���O��OFF�Ȃ炻��Ɉˑ�����R���g���[����Disable�ɁAON�Ȃ�Enable�ɂ���
	CButton	aNoEtcCarCheckBox(GetDlgItem(IDC_CHECK_NOETC_CAR));
	if (aNoEtcCarCheckBox.GetCheck() == BST_UNCHECKED)
	{
		// ��ʎԗ��Ή��t���O��OFF�ɂȂ�����A�ˑ�����R���g���[���̒l�������l�i��OFF�̎��ɋ������l�j�Ƀ��Z�b�g����
		CButton(GetDlgItem(IDC_CHECK_DUMMY)).SetCheck(BST_UNCHECKED);
		CComboBox(GetDlgItem(IDC_COMBO_TOLLFUNC)).SetCurSel(0);

		m_cControlRel.ControlChanged( IDC_CHECK_DUMMY );
		m_cControlRel.ControlChanged( IDC_COMBO_TOLLFUNC );
	}
}

/**
 * @brief ETC�{�݃R�[�h�̏�Ԃɂ���āA�ˑ�����R���g���[����Enable/Disable��؂�ւ���
 * @note	[Bug 6217]�Œǉ�
 */
void CAttrHwyNodeDlg::ChangeDependentControlsOnEtcFacilCode()
{
	// ETC�����ʗ����֘A�̃t�B�[���h�����݂��Ȃ��ꍇ�͉������Ȃ�
	if (! EtcSpecificTollFieldsExist())
		return;

	// ETC�{�݃R�[�h���uETC��Ή��v�Ȃ炻��Ɉˑ�����R���g���[����Disable�ɁA����ȊO�Ȃ�Enable�ɂ���
	CComboBox aEtcFacilCodeComboBox(GetDlgItem(IDC_COMBO_ETC_FACIL));
	int aSelectedIndex = aEtcFacilCodeComboBox.GetCurSel();
	if (aSelectedIndex != CB_ERR)
	{
		long aSelectedValue = aEtcFacilCodeComboBox.GetItemData(aSelectedIndex);
		if(aSelectedValue == sindy::schema::highway_node::etc_facil::kNoEtcFacil)
		{
			// ETC�{�݃R�[�h���uETC��Ή��v�ɂȂ�����A�ˑ�����R���g���[���̒l�������l�i��ETC��Ή��̎��ɋ������l�j�Ƀ��Z�b�g����
			CButton(GetDlgItem(IDC_CHECK_NOGUIDETOLL_ETC)).SetCheck(BST_UNCHECKED);
			CComboBox(GetDlgItem(IDC_COMBO_TOLLFUNC_ETC)).SetCurSel(0);

			m_cControlRel.ControlChanged( IDC_CHECK_NOGUIDETOLL_ETC );
			m_cControlRel.ControlChanged( IDC_COMBO_TOLLFUNC_ETC );
		}
	}
}

/**
 * @brief �H���R�[�h�̒l�ɏ]���ăn�C�E�F�C���[�h�t���O��ύX
 * @note	[Bug 6217]�Ή����ɒǉ�
 * @param[in]	cRoadCode	�H���R�[�h
 */
void CAttrHwyNodeDlg::ChangeHwyModeCheckBox(long cRoadCode)
{
	// �H���R�[�h��200000�ȏ�Ȃ�n�C�E�F�C���[�h�t���O��ON�A200000�����Ȃ�OFF�ɂ���
	CButton(GetDlgItem(IDC_CHECK_ISHWMODE)).SetCheck(m_nRoadCode >= 200000 ? BST_CHECKED : BST_UNCHECKED);
	m_cControlRel.ControlChanged( IDC_CHECK_ISHWMODE );

	SetInOutNodeComboBox();
}

/**
 * @brief ���������[�����̃C���[�W�̕`��
 */
void CAttrHwyNodeDlg::DrawLaneImage(void)
{
	// �s�N�`���R���g���[���̃E�B���h�E�̃f�o�C�X�R���e�L�X�g�n���h�����擾
	CWindow aLaneImgWindow = GetDlgItem(IDC_PICTURE_LANEIMG);
	CDC aDC(aLaneImgWindow.GetDC());

	// �s�N�`���R���g���[���̈ʒu�ɔ�����`��\��
	CRect aLaneImageRect(0, 0, 256, 64);
	aDC.SelectStockPen(WHITE_PEN);
	aDC.Rectangle(aLaneImageRect);

	// �r����`��
	aDC.SelectStockPen(BLACK_PEN);
	aDC.SelectStockBrush(NULL_BRUSH);
	aDC.Rectangle(aLaneImageRect);
	aDC.MoveTo(aLaneImageRect.left, aLaneImageRect.top + 32);
	aDC.LineTo(aLaneImageRect.right, aLaneImageRect.top + 32);

	// �t�H���g�̐ݒ�
	HFONT aOrgFont = aDC.SelectFont(m_TollGateLaneImgFont);

	// ���[���ԍ���\��������\��
	for (long i=0; i<m_TollGateLaneStrMaxLength; ++i)
	{
		long aCurrentColumn	= i % m_TollGateLaneImageColumnMaxNum;
		long aCurrentRow	= i / m_TollGateLaneImageColumnMaxNum;

		CRect aLaneNumRect;
		aLaneNumRect.left	= aLaneImageRect.left + 16 * aCurrentColumn;
		aLaneNumRect.top	= aLaneImageRect.top + 32 * aCurrentRow;
		aLaneNumRect.right	= aLaneNumRect.left + 16;
		aLaneNumRect.bottom	= aLaneNumRect.top + 16;

		// [Bug7173] ���P�[���Ɉˑ����Ȃ��������ɏC��
		CString aLaneNumStr;
		aLaneNumStr.Format(_T("%d"), i+1 );

		aDC.DrawText(aLaneNumStr, aLaneNumStr.GetLength(), aLaneNumRect, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	}

	// �t�H���g�����ɖ߂�
	aDC.SelectFont(aOrgFont);

	// ���������[�����G�f�B�b�g�R���g���[�����當������擾
	std::string aTollGateLaneStr;
	{
		_TCHAR aTollGateLaneStrTmp[m_TollGateLaneStrMaxLength + 1];
		CEdit aTollGateLaneEdit = GetDlgItem(IDC_EDIT_TOLLGATELANE);
		aTollGateLaneEdit.GetWindowText(aTollGateLaneStrTmp, m_TollGateLaneStrMaxLength + 1);
		aTollGateLaneStr = CT2A(aTollGateLaneStrTmp);
	}

	// �e�r�b�g�}�b�v�����蓖�Ă邽�߂̃������f�o�C�X�R���e�L�X�g�Q���쐬
	typedef boost::array<CDC, tollgate_lane_type::tollgate_lane_type_num> MemoryDCArray;
	MemoryDCArray aMemoryDCArray;
	BOOST_FOREACH (CDC& rCDC, aMemoryDCArray)
	{
		rCDC = CreateCompatibleDC(aDC);
	}

	aMemoryDCArray[tollgate_lane_type::impassable	].SelectBitmap(m_ImpassableImg);
	aMemoryDCArray[tollgate_lane_type::public_lane	].SelectBitmap(m_PublicImg);
	aMemoryDCArray[tollgate_lane_type::etc_lane		].SelectBitmap(m_EtcImg);
	aMemoryDCArray[tollgate_lane_type::mixed_lane	].SelectBitmap(m_MixedImg);
	aMemoryDCArray[tollgate_lane_type::switch_lane	].SelectBitmap(m_SwitchImg);
	aMemoryDCArray[tollgate_lane_type::unknown_lane	].SelectBitmap(m_UnknownImg);
	aMemoryDCArray[tollgate_lane_type::invalid		].SelectBitmap(m_InvalidImg);

	// �C���[�W�̕\��
	for (unsigned long i = 0; i < aTollGateLaneStr.size(); ++i)
	{
		// ���ڒ��̕�������\�����ׂ��r�b�g�}�b�v������
		long aTollGateLaneType = boost::lexical_cast<long>(aTollGateLaneStr[i]);
		if (aTollGateLaneType >= tollgate_lane_type::invalid)
		{
			aTollGateLaneType = tollgate_lane_type::invalid;
		}

		long aCurrentColumn	= i % m_TollGateLaneImageColumnMaxNum;
		long aCurrentRow	= i / m_TollGateLaneImageColumnMaxNum;
		aDC.BitBlt(
			16 * aCurrentColumn,
			16 + 32 * aCurrentRow,
			16,
			16,
			aMemoryDCArray[aTollGateLaneType],
			0,
			0,
			SRCAND
		);
	}
}


