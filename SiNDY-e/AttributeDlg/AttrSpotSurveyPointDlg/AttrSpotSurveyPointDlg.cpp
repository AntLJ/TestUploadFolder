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

// AttrSpotSurveyPointDlg.cpp : DLL �A�v���P�[�V�����p�ɃG�N�X�|�[�g�����֐����`���܂��B
//

#include "stdafx.h"
#include "shellapi.h" // ShellExecute
#include "AttrSpotSurveyPointDlg.h"
#include "AheLayerFunctions.h"
#include "AheGlobals.h"
#include "AheDraw.h"
#include "DrawFunctions.h"
#include "genericFunction.h"
#include <boost/assign/list_of.hpp>
#include <GdiPlus.h>
#pragma comment (lib, "Gdiplus.lib")
#undef min
#undef max

using namespace sindy::schema;
using namespace sindy::schema::sindym;

static const long FileNameMaxSize = 255;
static const long DirectoryMaxSize = 511;
static const long FullPathMaxSize = 2047;

// �E�B���h�E�}�l�[�W���z�u�}�b�v
BEGIN_WINDOW_MAP(SPOT_SURVEY_POINT_MAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(2,2))
		BEGINROWS(WRCT_TOFIT,0,0)
			// �����|�C���g��񕔕�
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCPERCENT(IDC_STATIC_MANAGEMENT_ID, 15)
				RCTOFIT(-1)
				RCPERCENT(IDC_EDIT_MANAGEMENT_ID, 30)
				RCSPACE(20)
				RCPERCENT(IDC_STATIC_TARGETTYPE, 20)
				RCTOFIT(-1)
				RCPERCENT(IDC_EDIT_TARGETTYPE, 35)
			ENDGROUP()
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCPERCENT(IDC_STATIC_SEASON, 15)
				RCTOFIT(-1)
				RCPERCENT(IDC_EDIT_SEASON, 30)
				RCSPACE(20)
				RCPERCENT(IDC_STATIC_SURVEYTYPE, 20)
				RCTOFIT(-1)
				RCPERCENT(IDC_EDIT_SURVEYTYPE, 35)
			ENDGROUP()
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCPERCENT(IDC_STATIC_COMPANYNAME, 15)
				RCTOFIT(-1)
				RCPERCENT(IDC_EDIT_COMPANYNAME, 30)
				RCSPACE(20)
				RCPERCENT(IDC_STATIC_SURVEYATTR, 20)
				RCTOFIT(-1)
				RCPERCENT(IDC_EDIT_SURVEYATTR, 35)
			ENDGROUP()
		ENDGROUP()
		BEGINROWS(WRCT_TOFIT,0,0)
			// �摜�\������
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(0,5))
				RCREST(IDC_STATIC_VAR1)
			ENDGROUP()
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_CHANGECODE)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_CHANGECODE)
			ENDGROUP()
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_NAME)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_NAME)
			ENDGROUP()
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_NAMEORG)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_NAMEORG)
			ENDGROUP()
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_SURVEYCOMMENT)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_SURVEYCOMMENT)
			ENDGROUP()
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_SURVEYDATE)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_SURVEYDATE)
				RCSPACE(20)
				RCTOFIT(IDC_CHK_PIC_IMP_F)
				RCSPACE(20)
				RCTOFIT(IDC_CHK_IPC_F)
			ENDGROUP()
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				BEGINCOLS(WRCT_FIXED, 0, 0)
					BEGINROWS(WRCT_FIXED, 0, RCMARGINS(0,0))
						BEGINROWS(WRCT_FIXED, 0, RCMARGINS(0,0))
							RCTOFIT(IDC_BUTTON_ILLUST)
						ENDGROUP()
						BEGINROWS(WRCT_FIXED, 0, RCMARGINS(0,0))
							RCTOFIT(IDC_BUTTON_POINT)
						ENDGROUP()
					ENDGROUP()
				ENDGROUP()
				BEGINCOLS(WRCT_REST, 0, 0)
					BEGINROWS(WRCT_REST, 0, RCMARGINS(0,0))
						RCREST(IDC_STATIC_PIC)
						BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(0,0))
							RCREST(IDC_SLIDER_PIC)
							RCTOFIT(-1)
							RCTOFIT(IDC_STATIC_PICNUM)
						ENDGROUP()
					ENDGROUP()
				ENDGROUP()
			ENDGROUP()
		ENDGROUP()
		BEGINROWS(WRCT_TOFIT,0,0)
			// ������ЃR�����g����
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(0,5))
				RCREST(IDC_STATIC_VAR2)
			ENDGROUP()
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				BEGINCOLS(WRCT_REST, 0, 0)
					RCTOFIT(IDC_STATIC_REQUEST)
					RCTOFIT(-1)
					RCREST(IDC_EDIT_REQUEST)
				ENDGROUP()
			ENDGROUP()
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_COMPANYCOMMENT)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_COMPANYCOMMENT)
			ENDGROUP()
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_REMARKS1)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_REMARKS1)
			ENDGROUP()
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_REMARKS2)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_REMARKS2)
			ENDGROUP()
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_NG_ATTR)
				RCTOFIT(-1)
				RCTOFIT(IDC_EDIT_NG_ATTR)
			ENDGROUP()
			BEGINCOLS(WRCT_REST, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_NG_COMMENT)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_NG_COMMENT)
			ENDGROUP()
		ENDGROUP()
	ENDGROUP()
END_WINDOW_MAP()

/**
 * �N���X�̖����I�����N���s�����߂ɁA���̃N���X��CExportDlg�N���X���p�����A�����_�C�A���O����CExportDlg���쐬���邱�Ƃɂ��
 * �����_�C�A���O������t���ɂ����邱�Ƃ��ł���N���X�{�̂��쐬����܂��B
 * �ڂ�����http://ptvi.hp.infoseek.co.jp/vc/p_0000010.html���Q�Ƃ̂���
 *
 * @retval CAttrSpotSurveyPointDlg��Ԃ��܂��B
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CAttrSpotSurveyPointDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrSpotSurveyPointDlg �����o�֐�
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrSpotSurveyPointDlg::CAttrSpotSurveyPointDlg()
 * @brief �R���X�g���N�^
 *
 * �u���V�̏����������Ă����܂��B
 */
CAttrSpotSurveyPointDlg::CAttrSpotSurveyPointDlg() 
	: ATTR_BASE_CLASS(SPOT_SURVEY_POINT_MAP, IDD_SPOTSURVEYPOINTDLG)
{
	// �e�R���g���[�����T�u�N���X�p�ɏ���
	m_mapSubClass = boost::assign::map_list_of
		( IDC_EDIT_MANAGEMENT_ID,  CContainedWindow( _T("EDIT"),     this, IDC_EDIT_MANAGEMENT_ID ) )
		( IDC_EDIT_TARGETTYPE,     CContainedWindow( _T("EDIT"),     this, IDC_EDIT_TARGETTYPE ) )
		( IDC_EDIT_SEASON,         CContainedWindow( _T("EDIT"),     this, IDC_EDIT_SEASON ) )
		( IDC_EDIT_SURVEYTYPE,     CContainedWindow( _T("EDIT"),     this, IDC_EDIT_SURVEYTYPE ) )
		( IDC_EDIT_COMPANYNAME,    CContainedWindow( _T("EDIT"),     this, IDC_EDIT_COMPANYNAME ) )
		( IDC_EDIT_SURVEYATTR,     CContainedWindow( _T("EDIT"),     this, IDC_EDIT_SURVEYATTR ) )
		( IDC_EDIT_REQUEST,        CContainedWindow( _T("EDIT"),     this, IDC_EDIT_REQUEST ) )
		( IDC_COMBO_CHANGECODE,    CContainedWindow( _T("COMBO"),    this, IDC_COMBO_CHANGECODE ) )
		( IDC_EDIT_NAME,           CContainedWindow( _T("EDIT"),     this, IDC_EDIT_NAME ) )
		( IDC_EDIT_NAMEORG,        CContainedWindow( _T("EDIT"),     this, IDC_EDIT_NAMEORG ) )
		( IDC_EDIT_SURVEYCOMMENT,  CContainedWindow( _T("EDIT"),     this, IDC_EDIT_SURVEYCOMMENT ) )
		( IDC_EDIT_SURVEYDATE,     CContainedWindow( _T("EDIT"),     this, IDC_EDIT_SURVEYDATE ) )
		( IDC_CHK_PIC_IMP_F,       CContainedWindow( _T("CHECKBOX"), this, IDC_CHK_PIC_IMP_F ) )
		( IDC_CHK_IPC_F,           CContainedWindow( _T("CHECKBOX"), this, IDC_CHK_IPC_F ) )
		( IDC_BUTTON_ILLUST,       CContainedWindow( _T("BUTTON"),   this, IDC_BUTTON_ILLUST ) )
		( IDC_BUTTON_POINT,        CContainedWindow( _T("BUTTON"),   this, IDC_BUTTON_POINT ) )
		( IDC_EDIT_COMPANYCOMMENT, CContainedWindow( _T("EDIT"),     this, IDC_EDIT_COMPANYCOMMENT ) )
		( IDC_EDIT_REMARKS1,       CContainedWindow( _T("EDIT"),     this, IDC_EDIT_REMARKS1 ) )
		( IDC_EDIT_REMARKS2,       CContainedWindow( _T("EDIT"),     this, IDC_EDIT_REMARKS2 ) )
		( IDC_EDIT_NG_ATTR,        CContainedWindow( _T("EDIT"),     this, IDC_EDIT_NG_ATTR ) )
		( IDC_EDIT_NG_COMMENT,     CContainedWindow( _T("EDIT"),     this, IDC_EDIT_NG_COMMENT ) );

	m_objectID = 0;
	m_slider = 0;
	m_directionIdx = -1;
	m_selectedPicSeq = -1;
}

/** 
 * @fn HWND CAttrSpotSurveyPointDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief �q�_�C�A���O���쐬���܂��B
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)�Ń_�C�A���O���쐬���܂��B
 *
 * @param hWndParent	[in]	�e�E�B���h�E�n���h��
 * @param dwInitParam	[in]	�_�C�A���O�������p�����[�^�i���ݓ��Ɏg�p���Ă��܂���j
 *
 * @retval �쐬���ꂽ�q�_�C�A���O�̃n���h����Ԃ��܂��B
 */
HWND CAttrSpotSurveyPointDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	m_hChildWnd = ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
	return m_hChildWnd;
}

/**
 * @fn void CAttrSpotSurveyPointDlg::Delete()
 * @brief �쐬�����q�_�C�A���O���폜���܂��B
 *
 * �_�C�A���O���폜����Ƃ��ɕK���Ă�ŉ������B�Ă΂Ȃ��ƃ��������[�N���Ă��܂��܂��B
 */
void CAttrSpotSurveyPointDlg::Delete()
{
	ClearFeatureDefs();
	SetNull2DrawGeometry();
	m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL );

	if( IsWindow() )
		DestroyWindow();
    delete this; // �K��new�Ŋm�ۂ���O��
}

/**
 * @fn BOOL CAttrSpotSurveyPointDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
 * @brief ���ݕ\�������ׂ��t�B�[�`����FeatureDef�C���f�b�N�X��n���܂��B
 * ����ɁA�t�B�[�`���̑����Ƃ͊֌W�Ȃ��R���g���[���̐�������Ă����܂��B
 * �_�C�A���O���J�����Ƃ��ɍŏ��Ƀt�H�[�J�X�̓������Ă��ė~�����R���g���[���̐ݒ�����Ă����܂��B
 * �܂��A���e�ԍ��̒l�����������āA���[������擾���Ă���
 *
 * @param lFeatureIndex	[in]	�t�B�[�`����`�N���X�̃C���f�b�N�X�ԍ�
 * @param lTableIndex	[in]	�e�[�u����`�N���X�̃C���f�b�N�X�ԍ�
 * @param lRowIndex		[in]	���R�[�h��`�N���X�̃C���f�b�N�X�ԍ�
 * @param bForce		[in]	�����I�ɕҏW���邩�ǂ���
 * @param bEditable		[in]	�ҏW�\���ǂ���
 *
 * @retval �Ӗ����Ȃ��iTRUE�����Ԃ��Ȃ��j
 */
BOOL CAttrSpotSurveyPointDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{
	// �`��p�W�I���g���ϐ�������
	SetNull2DrawGeometry();
	BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );
	m_objectID = AheGetOID( GetCurrentFeatureDef()->GetFeature() );
	((IFeaturePtr)GetCurrentFeatureDef()->GetFeature())->get_Shape( &m_surveyPointGeom );

	// ���ڃt�B�[�`���Ɋ֌W�Ȃ��G�f�B�b�g�{�b�N�X(��F�Q�Ɨp�̘H�����V�[�P���X�⌴�e�ԍ�����)�Ȃǂ𐧌䂷��
	// �ҏW�J�n�O�ƕҏW�J�n��ł��ҏW�s�̂Ƃ���DISABLE�ɂ���
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		// �ŏ��Ƀt�H�[�J�X�����R���g���[��
		::SetFocus( GetDlgItem(IDC_EDIT_SOURCE) );
	}

	// �ړ��O�|�C���g�T�����łɈړ��O��Ń|�C���g�����������ǂ����`�F�b�N
	// ��������΃{�^������
	m_orgPointIsDrawed = false;
	CButton cPointButton( GetDlgItem( IDC_BUTTON_POINT ) );
	cPointButton.EnableWindow( LoadSurveyPointOrg() ? FALSE : TRUE );

	// �摜�T��
	m_slider = 0;
	CTrackBarCtrl( GetDlgItem( IDC_SLIDER_PIC ) ).SetPos(0);
	m_directionIdx = -1;
	m_selectedPicSeq = -1;
	m_picPointVec.clear();
	FindPicture();
	DrawPicturePoint();

	// ���Ƀr���[�A���J���Ă���Ƃ��͕\���摜��؂�ւ���
	auto dock = generic::getDockableWindow( CLSID_SpotAttributeWindow, m_cArcHelper.GetApp() );
	auto user = generic::castUserDataToIUnknownPtr(dock);
	auto attr = static_cast<ISpotAttributeWindowPtr>(user);
	attr->updatePictureViewer( 0, GetCurrentFeatureDef()->GetFeature(), VARIANT_FALSE );
	RefreshPicturePointAllow();

	// �C���X�g�`��
	m_illustIsDrawed = false;
	DrawIllustGeometry();
	CButton cIllustButton( GetDlgItem( IDC_BUTTON_ILLUST ) );
	if( !m_illustPointGeom && !m_illustLineGeom && !m_illustPolygonGeom )
	{
		m_illustIsDrawed = false;
		cIllustButton.EnableWindow( FALSE );
	}
	else
	{
		cIllustButton.SetWindowText( _T("�C���X�gOFF") );
	}

	return bRet;
}

LRESULT CAttrSpotSurveyPointDlg::OnInitDialog( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	// �R���g���[��
	CreateControlRelation();
	// �T�u�N���X��
	for( auto& it : m_mapSubClass)
	{
		it.second.SubclassWindow( GetDlgItem( it.first ) );
	}
	// ���W�X�g���̐ݒ�擾
	GetRegCode();
	// �E�B���h�E�}�l�[�W���ɂ��Ĕz�u
	// ����Ȃ��ƃG�f�B�b�g�{�b�N�X��x�B����(�c������)�ׂ�Ă��܂�
	m_winMgr.InitToFitSizeFromCurrent(this);
	m_winMgr.CalcLayout(this);
	m_winMgr.SetWindowPositions(this);

	return ATTR_BASE_CLASS::OnInitDialog( uMsg, wParam, lParam, bHandled );
}

/**
 * @fn void CAttrSpotSurveyPointDlg::CreateControlRelation()
 * @brief �q�_�C�A���O�����ۂɍ쐬���܂��B
 *
 * ���ۂ̍쐬��ControlRel�ɔC���܂��B
 */
void CAttrSpotSurveyPointDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// �e�E�C���h�E�n���h����ݒ肵�Ȃ���
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else
	{
		using namespace spot_survey_point;
		m_cControlRel.SetHWND( m_hWnd );
		// �Ǘ�ID
		m_cControlRel.SetControl( IDC_EDIT_MANAGEMENT_ID, TYPE_EDITSTRING, 0, 0, kManagementId, NULL, kTableName );
		// �����f�[�^
		m_cControlRel.SetControl( IDC_EDIT_SEASON, TYPE_EDITSTRING, 0, 0, kSeason, NULL, kTableName );
		// ������Ж�
		m_cControlRel.SetControl( IDC_EDIT_COMPANYNAME, TYPE_EDITSTRING, 0, 0, kCompanyName, NULL, kTableName );
		// �Ώێ��
		m_cControlRel.SetControl( IDC_EDIT_TARGETTYPE, TYPE_EDITSTRING, 0, 0, kTargetType, NULL, kTableName );
		// �������
		m_cControlRel.SetControl( IDC_EDIT_SURVEYTYPE, TYPE_EDITSTRING, 0, 0, kSurveyType, NULL, kTableName );
		// �����Ώۑ���
		m_cControlRel.SetControl( IDC_EDIT_SURVEYATTR, TYPE_EDITSTRING, 0, 0, kSurveyAttribute, NULL, kTableName );
		// �˗����e
		m_cControlRel.SetControl( IDC_EDIT_REQUEST, TYPE_EDITSTRING, 0, 0, kSurveyRequestType, NULL, kTableName );
		// �ύX���e
		m_cControlRel.SetControl( IDC_COMBO_CHANGECODE, TYPE_COMBO, 0, 0, kChangedCode, NULL, kTableName );
		// ����
		m_cControlRel.SetControl( IDC_EDIT_NAME, TYPE_EDITSTRING, 0, 0, kName, NULL, kTableName );
		// �C���O����
		m_cControlRel.SetControl( IDC_EDIT_NAMEORG, TYPE_EDITSTRING, 0, 0, kNameOrg, NULL, kTableName );
		// �����R�����g
		m_cControlRel.SetControl( IDC_EDIT_SURVEYCOMMENT, TYPE_EDITSTRING, 0, 0, kSurveyComment, NULL, kTableName );
		// ������
		m_cControlRel.SetControl( IDC_EDIT_SURVEYDATE, TYPE_EDITSTRING, 0, 0, kSurveyDate, NULL, kTableName );
		// �B�e�s��F
		m_cControlRel.SetControl( IDC_CHK_PIC_IMP_F, TYPE_CHECKBOX, 0, 0, kPictureImpossibleF, NULL, kTableName );
		// iPC�쐬F
		m_cControlRel.SetControl( IDC_CHK_IPC_F, TYPE_CHECKBOX, 0, 0, kCreatedByIPCF, NULL, kTableName );
		// ������ЃR�����g
		m_cControlRel.SetControl( IDC_EDIT_COMPANYCOMMENT, TYPE_EDITSTRING, 0, 0, kCompanyComment, NULL, kTableName );
		// ���l1
		m_cControlRel.SetControl( IDC_EDIT_REMARKS1, TYPE_EDITSTRING, 0, 0, kRemarks1, NULL, kTableName );
		// ���l2
		m_cControlRel.SetControl( IDC_EDIT_REMARKS2, TYPE_EDITSTRING, 0, 0, kRemarks2, NULL, kTableName );
		// NG���
		m_cControlRel.SetControl( IDC_EDIT_NG_ATTR, TYPE_EDITSTRING, 0, 0, kNgAttribute, NULL, kTableName );
		// NG�R�����g
		m_cControlRel.SetControl( IDC_EDIT_NG_COMMENT, TYPE_EDITSTRING, 0, 0, kNgComment, NULL, kTableName );
	}
}

LRESULT CAttrSpotSurveyPointDlg::OnCommand( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	// SetCurrentFeatureDefIndex() �̍Œ��̓L�����Z���i�^�C�~���O�ɂ���Ă͂��������Ȃ邽�߁j
	if( !m_bFinishInit )
		return 0;

	int msg = HIWORD(wParam); // ���상�b�Z�[�W
	int nTargetControl = (int)LOWORD(wParam); // ����ΏۃR���g���[��

	// �G�f�B�b�g�{�b�N�X�̎��ɑS�I���ɂ���
	if( msg == EN_SETFOCUS )
		SelectDlgItem( nTargetControl );

	// �R���{�{�b�N�X�̃��X�g�{�b�N�X�̒����𒲐�
	if( msg == CBN_DROPDOWN )
		SetComboboxList( nTargetControl );

	// �R���{�{�b�N�X�A�G�f�B�b�g�{�b�N�X�A�`�F�b�N�{�b�N�X�̃��b�Z�[�W����
	if( ( msg == CBN_SELCHANGE ) || ( msg == BN_CLICKED ) || ( msg == EN_UPDATE ) ) {
		// �ύX���ꂽ���ǂ����`�F�b�N���A�ύX����Ă����瑼�̃R���g���[���ɂ����f������
		m_cControlRel.ControlChanged( (int)LOWORD(wParam) );
		// �_�C�A���O���X�V�̈�ɒǉ����܂�
		InvalidateRect( NULL, FALSE );
		// �ύX���ꂽ�ꍇ�̓{�^���� Enable ��
		SetButton( Changed() );
	}
	return 0;
}

// �����摜�T���l�C���`��
LRESULT CAttrSpotSurveyPointDlg::OnPaint( UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	// �`��J�n
	PAINTSTRUCT ps;
	HDC hDC = BeginPaint(&ps);

	if( m_baseDir.IsEmpty() )
	{
		EndPaint( &ps );
		return 0;
	}

	// �`��͈͂̎擾
	HWND hPicWnd = GetDlgItem( IDC_STATIC_PIC );
	CWindow cPicWindow( hPicWnd );
	long lWidth = 0, lHeight = 0, lPicWidth = 0, lPicHeight = 0;
	RECT rect;
	cPicWindow.GetClientRect( &rect );
	lWidth = rect.right - rect.left;
	lHeight = rect.bottom - rect.top;

	// �`�文�������߂�
	long maxPaintNum = 0;
	maxPaintNum = (long)m_picPath.size();

	// 0�Ȃ炱���ŏI���
	if( maxPaintNum == 0 )
	{
		EndPaint( &ps );
		return 0;
	}

	// �ő�4���܂�
	if( maxPaintNum > 4 )
		maxPaintNum = 4;

	// �`��͈͂𓙕�����
	lWidth = ( lWidth / maxPaintNum ) - maxPaintNum + 1;
	RECT PicRect;
	long lPicNo = m_slider;

	// �T���l�C���̉��ɖ�����\�����邽�߂ɕ�����t�H�[�}�b�g��ݒ�
    Gdiplus::StringFormat stringFormat;
	stringFormat.SetAlignment( Gdiplus::StringAlignmentCenter );
    stringFormat.SetLineAlignment( Gdiplus::StringAlignmentCenter );
	// �`��p�Ƀt�H���g�ƃu���V���쐬
	Gdiplus::Font strFont(_T("MS UI Gothic"), 15);
    Gdiplus::SolidBrush strBrush( Gdiplus::Color::Black );

	// 1��������̏c���̔䗦�ŕ`��T�C�Y�ς���
	CRect pictRect(0,0,0,0);
	cPicWindow.GetClientRect(pictRect);

	HDC picDC = cPicWindow.GetDC();
	Gdiplus::Graphics graphic(picDC);

	// �w�i���N���A
	// ��������Ȃ��ƃX���C�_�[�ړ������Ƃ��ɖ����\�����d�Ȃ�
	graphic.FillRectangle(
		&Gdiplus::SolidBrush( Gdiplus::Color(240, 240, 240) ),
		Gdiplus::Rect(pictRect.left, pictRect.top, pictRect.Width(), pictRect.Height()) );

	auto thumbRects = getThumbSize();
	for(auto& elem : thumbRects)
	{
		// first�ɂ͉摜�i�[��������
		// �K�v�Ȃ̂�vector�̃C���f�b�N�X�ԍ��Ȃ̂ŁA�����ŕϊ�
		const int index = elem.first - 1;
		if( index >= m_pictureVec.size() )
			continue;
		m_pictureVec[index].draw(hPicWnd, elem.second);

		// �����`��p�̋�`��p��
		Gdiplus::RectF strRect(
			static_cast<Gdiplus::REAL>(elem.second.left)
			, static_cast<Gdiplus::REAL>(elem.second.bottom)
			, static_cast<Gdiplus::REAL>(elem.second.Width())
			, static_cast<Gdiplus::REAL>(pictRect.Height() - elem.second.Height())
			);

		graphic.DrawString(
			CString(std::to_wstring(elem.first).c_str()) + _T("����")
			, -1
            , &strFont
            , strRect
            , &stringFormat
            , &strBrush
        );
	}
	ReleaseDC(picDC);
	// �`��I��
	EndPaint( &ps );
	return 0;
}

LRESULT CAttrSpotSurveyPointDlg::OnRefresh( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	// �B�e�摜�|�C���g���̍ĕ`��
	RefreshPicturePointAllow();

	// �ړ��O�����|�C���g
	if( m_orgPointIsDrawed )
	{
		if( m_orgSurveyPointGeom )
		{
			AheDrawPoint( m_cArcHelper.GetScreenDisplay(), m_orgSurveyPointGeom, FALSE, 5, 0, 0, 0, esriSMSCircle );
		}
	}

	// �C���X�g
	if( m_illustIsDrawed )
	{
		if( m_illustPointGeom )
			AheDrawPoint( m_cArcHelper.GetScreenDisplay(), m_illustPointGeom, FALSE, 5, 0, 200, 50, esriSMSCircle );
		if( m_illustLineGeom )
			AheDrawPolyline( m_cArcHelper.GetScreenDisplay(), m_illustLineGeom, RGB( 0, 200, 50 ), 2, esriSLSSolid, FALSE );
		if( m_illustPolygonGeom )
			AheDrawPolygon( m_cArcHelper.GetScreenDisplay(), m_illustPolygonGeom, RGB( 0, 200, 50 ), 2, esriSLSSolid, FALSE );
	}

	// TODO: ������̓n�C���C�g�̎d�l���m�肵���璼������
	// �b��Ή��ł���B
	// AfterDraw���Ă΂�鏇�Ԃ��A
	// �摜�r���[�A�̃n�C���C�g���������̏����ƂȂ�A�n�C���C�g�������㏑������邽�߁A
	// ���߂āA�摜�r���[�A���̃n�C���C�g�������Ăяo���Ă���
	// �i�܂�A�摜�r���[�A�̃n�C���C�g���������̏������摜�r���[�A�̃n�C���C�g�����A�Ƃ����悤�ɁA�������������Ăяo����Ă���j
	auto dock = generic::getDockableWindow( CLSID_SpotAttributeWindow, m_cArcHelper.GetApp() );
	auto user = generic::castUserDataToIUnknownPtr(dock);
	auto attr = static_cast<ISpotAttributeWindowPtr>(user);
	IActiveViewEventsPtr(user)->AfterDraw(m_cArcHelper.GetScreenDisplay(), esriViewDrawPhase::esriViewForeground);

	return 0;
}

// ���N���b�N�ŎB�e�|�C���g�n�C���C�g
LRESULT CAttrSpotSurveyPointDlg::OnLButtonDown( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	if( m_picPath.empty() ) return 0;

	// �q�_�C�A���O���̃N���b�N�ʒu
	int pictSeq = getClickedPictureSeq( LOWORD(lParam), HIWORD(lParam) );
	if( pictSeq <= 0 ) return 0;

	m_selectedPicSeq = pictSeq;
	auto dock = generic::getDockableWindow( CLSID_SpotAttributeWindow, m_cArcHelper.GetApp() );
	auto user = generic::castUserDataToIUnknownPtr(dock);
	auto attr = static_cast<ISpotAttributeWindowPtr>(user);
	attr->updatePictureViewer( pictSeq, GetCurrentFeatureDef()->GetFeature(), VARIANT_FALSE );
	m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL );
	return 0;
}

// �_�u���N���b�N�ŉ摜�J��
LRESULT CAttrSpotSurveyPointDlg::OnLButtonDblClk( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	if( m_picPath.empty() ) return 0;

	// �q�_�C�A���O���̃N���b�N�ʒu
	int pictSeq = getClickedPictureSeq( LOWORD(lParam), HIWORD(lParam) );
	if( pictSeq <= 0 ) return 0;

	m_selectedPicSeq = pictSeq;

	auto dock = generic::getDockableWindow( CLSID_SpotAttributeWindow, m_cArcHelper.GetApp() );
	auto user = generic::castUserDataToIUnknownPtr(dock);
	auto attr = static_cast<ISpotAttributeWindowPtr>(user);
	// Shift������Ă��狭�����č��
	if( wParam&MK_SHIFT)
		attr->createPictureViewer( pictSeq, GetCurrentFeatureDef()->GetFeature() );
	else
		attr->updatePictureViewer( pictSeq, GetCurrentFeatureDef()->GetFeature(), VARIANT_TRUE );

	m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL );
	return 0;
}

// �X���C�_�[�����������̃C�x���g
void CAttrSpotSurveyPointDlg::OnHScroll( int nCode, short nPos, HWND hWnd )
{
	CTrackBarCtrl trackbar = hWnd;
	m_slider = trackbar.GetPos();
	InvalidateRect( NULL, FALSE );
}

IGeometryPtr CAttrSpotSurveyPointDlg::GetIllustGeometry( const IWorkspacePtr& ipWorkspace, const CString& owner, const CString& table, const long id )
{
	CString tableName;
	if( owner.IsEmpty() )
		tableName = table;
	else
		tableName = owner + _T(".") + table;

	ITablePtr ipTable;
	ipTable = AheOpenTableByTableName( ipWorkspace, tableName );
	IFeatureClassPtr ipFeatureClass( ipTable );

	IFeatureCursorPtr ipCursor;
	ipFeatureClass->Search( AheInitQueryFilter( NULL, NULL, _T("%s = %d"), spot_survey_point_relational_table::kSurveyPointId, id ), VARIANT_FALSE, &ipCursor );

	IGeometryPtr ipRetGeom;
	IFeaturePtr ipFeature;
	while( S_OK == ipCursor->NextFeature( &ipFeature ) && ipFeature )
	{
		IGeometryPtr ipGeom;
		ipFeature->get_Shape( &ipGeom );
		if( !ipRetGeom )
		{
			ipRetGeom = ipGeom;
		}
		else
		{
			// ���������肤��
			IGeometryPtr ipTmpGeom = ipRetGeom;
			((ITopologicalOperatorPtr)ipTmpGeom)->Union( ipGeom, &ipRetGeom );
		}
	}

	return ipRetGeom;
}

IWorkspacePtr CAttrSpotSurveyPointDlg::GetWorkspace()
{
	IFeaturePtr ipCurrentFeature;
	ipCurrentFeature = GetCurrentFeatureDef()->GetFeature();
	ITablePtr ipCurrentTable;
	ipCurrentFeature->get_Table( &ipCurrentTable );
	IDatasetPtr ipDataset( ipCurrentTable );

	IWorkspacePtr ipWorkspace;
	ipDataset->get_Workspace( &ipWorkspace );

	return ipWorkspace;
}

// �C���X�g�\���{�^��
LRESULT CAttrSpotSurveyPointDlg::OnClickedIllustButton( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled )
{
	CString str;
	CButton cButton( GetDlgItem( IDC_BUTTON_ILLUST ) );
	if( m_illustIsDrawed )
		str = _T("�C���X�gON");
	else
		str = _T("�C���X�gOFF");

	cButton.SetWindowText( str );
	DrawIllustGeometry();
	return 0;
}

void CAttrSpotSurveyPointDlg::DrawIllustGeometry()
{
	if( m_illustIsDrawed )
	{
		m_illustIsDrawed = false;
		m_illustPointGeom = nullptr;
		m_illustLineGeom = nullptr;
		m_illustPolygonGeom = nullptr;
	}
	else
	{
		m_illustIsDrawed = true;

		// �C���X�g����Ă��ăn�C���C�g
		if( !m_illustPointGeom && !m_illustLineGeom && !m_illustPolygonGeom )
		{
			IWorkspacePtr ipWorkspace = GetWorkspace();
			CString owner = AheGetFeatureClassOwnerName( GetCurrentFeatureDef()->GetFeature() );
			m_illustPointGeom = GetIllustGeometry( ipWorkspace, owner, illustration_point::kTableName, m_objectID );
			m_illustLineGeom = GetIllustGeometry( ipWorkspace, owner, illustration_line::kTableName, m_objectID );
			m_illustPolygonGeom = GetIllustGeometry( ipWorkspace, owner, illustration_polygon::kTableName, m_objectID );
		}
	}
	m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL );
}

// �ړ��O��m�F�{�^��
LRESULT CAttrSpotSurveyPointDlg::OnClickedPointButton( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled )
{
	CButton cButton( GetDlgItem( IDC_BUTTON_POINT ) );
	CString str;
	if( m_orgPointIsDrawed )
		str = _T("�ړ��O�m�F");
	else
		str = _T("�ړ���m�F");

	cButton.SetWindowText( str );
	DrawOrgPointGeometry();
	return 0;
}

bool CAttrSpotSurveyPointDlg::LoadSurveyPointOrg()
{
	if( !m_orgSurveyPointGeom )
	{
		IWorkspacePtr ipWorkspace = GetWorkspace();
		CString owner = AheGetFeatureClassOwnerName( GetCurrentFeatureDef()->GetFeature() );
		m_orgSurveyPointGeom = GetIllustGeometry( ipWorkspace, owner, spot_survey_point_org::kTableName, m_objectID );
	}

	if( m_orgSurveyPointGeom )
	{
		double x = 0, y = 0, orgx = 0, orgy = 0;
		((IPointPtr)m_surveyPointGeom)->QueryCoords( &x, &y );
		((IPointPtr)m_orgSurveyPointGeom)->QueryCoords( &orgx, &orgy );
		// �ړ��O�オ�������W�̏ꍇ�A�ړ��O�͂Ȃ����̂Ƃ݂Ȃ�
		return ( x == orgx && y == orgy );
	}
	return true;
}

void CAttrSpotSurveyPointDlg::DrawOrgPointGeometry()
{
	IEnvelopePtr ipExtent;
	m_cArcHelper.GetActiveView()->get_Extent( &ipExtent );
	if( m_orgPointIsDrawed )
	{
		m_orgPointIsDrawed = false;
		ipExtent->CenterAt( (IPointPtr)m_surveyPointGeom );
	}
	else
	{
		m_orgPointIsDrawed = true;
		ipExtent->CenterAt( (IPointPtr)m_orgSurveyPointGeom );
	}
	m_cArcHelper.GetActiveView()->put_Extent( ipExtent );
	m_cArcHelper.GetActiveView()->Refresh();
}

void CAttrSpotSurveyPointDlg::DrawPicturePoint()
{
	IWorkspacePtr ipWorkspace = GetWorkspace();
	CString owner = AheGetFeatureClassOwnerName( GetCurrentFeatureDef()->GetFeature() );

	CString tableName;
	if( owner.IsEmpty() )
		tableName = picture_point::kTableName;
	else
		tableName = owner + _T(".") + picture_point::kTableName;

	ITablePtr ipTable;
	ipTable = AheOpenTableByTableName( ipWorkspace, tableName );
	IFeatureClassPtr ipFeatureClass( ipTable );
	if( !ipFeatureClass )
		return;

	IFeatureCursorPtr ipCursor;
	ipFeatureClass->Search( AheInitQueryFilter( NULL, NULL, _T("%s = %d"), picture_point::kSurveyPointId, m_objectID ), VARIANT_FALSE, &ipCursor );

	IFeaturePtr ipFeature;
	while( S_OK == ipCursor->NextFeature( &ipFeature ) && ipFeature )
	{
		// TODO:���̂ւ�PicPoint�����Ŋ�������悤�ɂ�����
		if( m_pictureSeqIdx < 0 || m_directionIdx < 0 )
		{
			IFieldsPtr ipFields;
			ipFeature->get_Fields( &ipFields );
			ipFields->FindField( CComBSTR(picture_point::kPictureSeq), &m_pictureSeqIdx );
			ipFields->FindField( CComBSTR(picture_point::kDirection), &m_directionIdx );
		}
		m_picPointVec.push_back( PicPoint( ipFeature, m_pictureSeqIdx, m_directionIdx ) );
	}
	// �摜�i�[���Ń\�[�g
	std::sort( m_picPointVec.begin(), m_picPointVec.end(), []( PicPoint& left, PicPoint& right ){return left.m_pictureSeq < right.m_pictureSeq;});

	m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL );
}

void CAttrSpotSurveyPointDlg::FindPicture()
{
	m_picPath.clear();
	m_pictureVec.clear();
	m_slider = 0;

	// �������
	CEdit cEditType(GetDlgItem( IDC_EDIT_SURVEYTYPE ) );
	LPTSTR lpszType = (LPTSTR)_alloca( ( cEditType.GetWindowTextLength() + 1 ) * sizeof(TCHAR) );
	cEditType.GetWindowText( lpszType, cEditType.GetWindowTextLength() + 1 );
	CString surveyType = lpszType;

	// �����f�[�^
	CEdit cEditSeason(GetDlgItem( IDC_EDIT_SEASON ) );
	LPTSTR lpszSeason = (LPTSTR)_alloca( ( cEditSeason.GetWindowTextLength() + 1 ) * sizeof(TCHAR) );
	cEditSeason.GetWindowText( lpszSeason, cEditSeason.GetWindowTextLength() + 1 );
	CString seasonData = lpszSeason;

	// OID
	CString strOid;
	strOid.Format( _T("%d"), m_objectID );

	// �t�H���_���̃t�@�C����������
	// TODO:uh::getFileNames�g��
	HANDLE hFile; // �����p�n���h��
	WIN32_FIND_DATA findData; // �����p�\����
	CString dirPath; // ��������f�B���N�g��
	long itemCount = 0;
	dirPath = m_baseDir + _T("\\") + surveyType + _T("\\") + seasonData + _T("\\") + strOid + _T("\\") + _T("*.JPG");

	// �����p�n���h���ƍŏ��̃t�@�C���擾
	hFile = FindFirstFile( dirPath, &findData );
	if( hFile == INVALID_HANDLE_VALUE )
	{
		// TODO:�u�摜�Ȃ��v�Ɓu�摜������Ȃ��v�̋�ʂ�t������Ȃ�A�u�摜������Ȃ��v�ꍇ�̓G���[���b�Z�[�W���o���悤�ɂ���B
		SetDlgItemText( IDC_STATIC_PICNUM, _T("0") );
		return;
	}

	// �摜�p�X���i�[
	// �ŏ��̃t�@�C��(findData)�擾�ς݂Ȃ̂�do�`while�ŏ���
	do
	{
		// *.jpg�Ō������Ă�̂Ŗ��Ȃ��Ǝv�����A�ꉞ�t�@�C�������̃`�F�b�N
		// �f�B���N�g���u.�v�u..�v�͌��Ȃ��ėǂ��̂Ŕ�΂�
		if( findData.cFileName[0] == '.' )
			continue;

		// �T�u�f�B���N�g���͖���
		if ( findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
			continue;

		// ������t�@�C�����̓t���p�X�Ȃ̂ŕ���
		TCHAR strDrive[FileNameMaxSize], strDir[DirectoryMaxSize], strFileName[FileNameMaxSize], strExt[FileNameMaxSize];
		_tsplitpath( findData.cFileName, strDrive, strDir, strFileName, strExt );
		// jpg�ȊO�͖���
		if( ((CString)strExt).CompareNoCase( _T(".JPG") ) != 0 )
			continue;

		// �i�[
		CString strParameters;
		strParameters += m_baseDir + _T("\\") + surveyType + _T("\\") + seasonData + _T("\\") + strOid + _T("\\") + strFileName + _T(".JPG");
		m_picPath.push_back( strParameters );
	} while( FindNextFile( hFile, &findData ) );

	// �����I������猟���p�n���h������Ȃ��Ă͂����Ȃ�
	FindClose( hFile );

	// �t�@�C�����̓��̓V�[�P���X�ԍ��Ȃ̂�sort���Ă���DB�i�[���ɂȂ�͂�
	std::sort( m_picPath.begin(), m_picPath.end() );

	// �摜�����[�h���Ċi�[
	for( auto& path : m_picPath )
	{
		CJpeg2 cJpeg;
		if( !cJpeg.load( path ) )
		{
#ifdef _DEBUG
			CString msg;
			msg.Format( _T("���[�h���s�F%s"), path );
			MessageBox( msg, _T("FindPicture()"), MB_OK );
#endif
		}
		m_pictureVec.push_back( cJpeg );
	}

	// �擾�����摜�̖����ɍ��킹�ăX���C�_�[����ݒ肷��
	long picNum = 0;
	picNum = (long)m_picPath.size();
	CString strPicNum;
	strPicNum.Format( _T("%d"), picNum );
	SetDlgItemText( IDC_STATIC_PICNUM, strPicNum );

	// �f�t�H���g��4���܂ŕ\��������̂�4�������Ȃ�X���C�_�[����Ȃ�
	CTrackBarCtrl cTrackBar( GetDlgItem( IDC_SLIDER_PIC ) );
	if( picNum > 4 )
	{
		::ShowWindow( cTrackBar, true );
		cTrackBar.SetRange( 0, picNum-4 );
	}
	else
	{
		::ShowWindow( cTrackBar, false );
	}
}

// �N���b�N���ꂽ�T���l�C���̌��摜�p�X�擾
int CAttrSpotSurveyPointDlg::getClickedPictureSeq(long clickedX, long clickedY)
{
	CWindow parentWnd( m_hWnd );
	CWindow staticPict( GetDlgItem( IDC_STATIC_PIC ) );

	CRect parentRect(0,0,0,0), picRect(0,0,0,0);
	parentWnd.GetWindowRect( parentRect );
	staticPict.GetWindowRect( picRect );

	// �q�_�C�A���O�̃E�B���h�E�ʒu����N���b�N���|�C���g�����߂�
	// �܂肱��X�AY��IDC_STATIC_PIC�̃E�B���h�E�͈͂ɓ����Ă���΂悢
	const int nStaticPicX = parentRect.left + clickedX;
	const int nStaticPixY = parentRect.top + clickedY;
	if( nStaticPicX < picRect.left || nStaticPicX > picRect.right ||
		nStaticPixY < picRect.top || nStaticPixY > picRect.bottom )
		return -1;

	// �̈�̂ǂ����N���b�N�������H
	auto thumbSizes = getThumbSize();
	for(auto& thumbnail : thumbSizes)
	{
		// ���̋�`�̈ʒu��IDC_STATIC_PIC����̑��Έʒu�ɂȂ��Ă���̂ŁA
		// IDC_STATIC_PIC�̐�΍��W�Ɉړ�����
		CRect& collisionRect = thumbnail.second;
		collisionRect.OffsetRect(picRect.left, picRect.top);
		if( collisionRect.left <= nStaticPicX
			&& nStaticPicX <= collisionRect.right
			&& collisionRect.top <= nStaticPixY
			&& nStaticPixY <= collisionRect.bottom)
		{
			return thumbnail.first;
		}
	}
	return -1;
}

std::map<long,CRect> CAttrSpotSurveyPointDlg::getThumbSize()
{
	// �`��͈͂Ƃ���IDC_STATIC_PIC�R���g���[���̕`��̈���擾
	CRect staticArea(0,0,0,0);
	CWindow(GetDlgItem( IDC_STATIC_PIC )).GetClientRect( staticArea );

	// �T���l�C���Ƃ��ĕ`��\�Ȗ����͍ő��4���Ƃ���
	const long maxThumbNum = std::min(m_picPath.size(), 4U );
	// �T���l�C���摜�̍ő卂���́AIDC_STATIC_PIC�̈��7���̑傫���Ƃ���
	const long thumbMaxHeight = static_cast<long>(staticArea.Height() * 0.7);
	// �T���l�C���摜�̍ő啝�́AIDC_STATIC_PIC�̈���A�`�文���œ��������傫���Ƃ���
	const long thumbMaxWidth  = staticArea.Width() / maxThumbNum;

	// �T���l�C���摜�̍ő卂���E���Ɏ��܂�A���A�A�X�y�N�g�䂪4:3���ۂ����
	// �摜���E�������v�Z����
	// double�v�Z�̉ߒ��ő����덷���o��ꍇ�����邪�A�����ɋ��߂�K�v�͂Ȃ�
	double thumbHeight = thumbMaxWidth * 3.0/4.0;
	if( thumbHeight > thumbMaxHeight )
		thumbHeight = thumbMaxHeight;
	double thumbWidth = thumbHeight * 4.0 / 3.0;

	// �T���l�C�����̗]��
	const long leftMargin = static_cast<long>((thumbMaxWidth - thumbWidth)/2.0);
	// �T���l�C���E�̗]��
	const long rightMargin = leftMargin;
	// �T���l�C����̗]��(���C�A�E�g�̓s���ŉ��̗]���͂Ƃ�Ȃ�)
	const long topMargin = static_cast<long>((thumbMaxHeight - thumbHeight)/2.0);

	std::map<long,CRect> thumbRects;
	// dispIndex�́A�`�悵���Ƃ��ɍ����牽�Ԗڂ̉摜�ɂȂ邩��\��
	// �T���l�C���̈�̋�`���`
	CRect thumbArea(
		0,          // ����X���΍��W
		topMargin,  // ���Y���΍��W
		thumbWidth, // ��
		thumbHeight // ����
		);
	for(long dispIndex = 0; dispIndex < maxThumbNum; ++dispIndex)
	{
		thumbArea.OffsetRect( leftMargin, 0 );
		thumbRects.emplace( dispIndex+m_slider+1, thumbArea );
		thumbArea.OffsetRect( thumbWidth+rightMargin, 0 );
	}
	return thumbRects;
}

void CAttrSpotSurveyPointDlg::GetRegCode()
{
 	CString strRegOpenKey( _T("SOFTWARE\\INCREMENT P CORPORATION\\SiNDY-e\\TOOLS\\SpotViewer" ) );

	HKEY hOpenKey = EmxCreateRegKey( strRegOpenKey, HKEY_CURRENT_USER );
	if( hOpenKey )
	{
		TCHAR strBaseDir[DirectoryMaxSize];

		if( EmxGetRegString( _T("SurveyDir"), strBaseDir, DirectoryMaxSize, hOpenKey ) )
			m_baseDir = strBaseDir;

		RegCloseKey( hOpenKey );
	}
}

void CAttrSpotSurveyPointDlg::DrawPicturePointAllow( IDisplayPtr piScreenDisplay, IPointPtr ipPoint, BOOL bXOR, long size, long red, long green, long blue, long angle, bool bDoStartStop )
{
	// �V���{���쐬
	IArrowMarkerSymbolPtr ipArrowMarkerSymbol( CLSID_ArrowMarkerSymbol );
	ipArrowMarkerSymbol->put_Style( esriAMSPlain );

	IColorPtr ipColor( AheGetColor( RGB( red, green, blue ) ) );
	ipArrowMarkerSymbol->put_Color( ipColor );
	ipArrowMarkerSymbol->put_Size( size );
	ipArrowMarkerSymbol->put_Angle( angle );

	ISymbolPtr ipArrowSymbol( ipArrowMarkerSymbol );
	if( bXOR )
	{
		ipArrowSymbol->put_ROP2( esriROPXOrPen );
	}

	// �`��
	double dScale = 0;
	IDisplayTransformationPtr ipTrans;
	piScreenDisplay->get_DisplayTransformation( &ipTrans );
	ipTrans->get_ReferenceScale( &dScale );
	ipTrans->put_ReferenceScale( 0.0 );
	if( bDoStartStop )
	{
		UINT hDC = 0;
		piScreenDisplay->get_hDC( &hDC );
		piScreenDisplay->StartDrawing( hDC, esriNoScreenCache );
	}
	piScreenDisplay->SetSymbol( ipArrowSymbol );
	piScreenDisplay->DrawPoint( ipPoint );
	if( bDoStartStop )
	{
		piScreenDisplay->FinishDrawing();
	}
	ipTrans->put_ReferenceScale( dScale );
}

void CAttrSpotSurveyPointDlg::SetNull2DrawGeometry()
{
	m_surveyPointGeom = nullptr;
	m_orgSurveyPointGeom = nullptr;
	m_illustPointGeom = nullptr;
	m_illustLineGeom = nullptr;
	m_illustPolygonGeom = nullptr;
}

// �B�e�摜�|�C���g���̍ĕ`��
void CAttrSpotSurveyPointDlg::RefreshPicturePointAllow()
{
	// �B�e�|�C���g(�S��)
	for( auto& picPoint : m_picPointVec )
	{
		// NULL�����蓾��̂ŁA���̎��͔�΂�
		if( picPoint.m_x == 0 && picPoint.m_y == 0 )
			continue;

		// ���ݒ�(�B�e������-1)�̂Ƃ�����΂�
		if( picPoint.m_angle < 0 )
			continue;

		IPointPtr ipPoint( CLSID_Point );
		ipPoint->PutCoords( picPoint.m_x, picPoint.m_y );
		DrawPicturePointAllow( m_cArcHelper.GetScreenDisplay(), ipPoint, FALSE, 12, 255, 0, 0, picPoint.m_angle );
		}

	// �B�e�|�C���g(�I��)
	if( m_selectedPicSeq > 0 )
	{
		IPointPtr ipPoint( CLSID_Point );
		ipPoint->PutCoords( m_picPointVec[m_selectedPicSeq-1].m_x, m_picPointVec[m_selectedPicSeq-1].m_y );
		DrawPicturePointAllow( m_cArcHelper.GetScreenDisplay(), ipPoint, FALSE, 12, 0, 0, 255, m_picPointVec[m_selectedPicSeq-1].m_angle );
		m_selectedPicSeq = -1;
	}
}
