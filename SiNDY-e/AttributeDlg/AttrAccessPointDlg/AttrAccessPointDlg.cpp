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

// AttrAccessPointDlg.cpp: CAttrAccessPointDlg �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WinLib/libwinlib.h"
#include "crd_cnv/coord_converter.h"
#include "AttrDlgLib.h"
#include "AttrAccessPointDlg.h"
#include <sstream>					// [Bug5556]�Œǉ�
#include <boost/lexical_cast.hpp>	// [Bug5556]�Œǉ�

const long CAttrAccessPointDlg::m_MasterCodeMaxLength = 8;	// [Bug5556]�Œǉ�
const long CAttrAccessPointDlg::m_TimeStrMaxLength = 2;		// [Bug5556]�Œǉ�
const long CAttrAccessPointDlg::m_ASEditBufferLength= 32;	// [Bug5556]�Œǉ��B�u9�v�ŏ\���Ȃ͂������A�O�̂��ߑ傫�߂Ɋm�ہB

/// �E�B���h�E�}�l�[�W���z�u�}�b�v�i�A�N�Z�X�|�C���g�A��{���j
BEGIN_WINDOW_MAP(ACCESS_POINT_MAP)
	BEGINROWS( WRCT_REST, 0,RCMARGINS(2,2)) 
		BEGINROWS(WRCT_PCT,0,90)
			BEGINROWS(WRCT_REST, IDC_TAB_ACCESSPOINT, RCMARGINS(2,2))
				RCSPACE(20)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
						RCTOFIT(IDC_CHECK_HWYMODEFLAG)
				ENDGROUP()
				RCSPACE(5)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
						RCTOFIT(IDC_STATIC_ROADCLASS_REF)
						RCTOFIT(-1)
						RCREST(IDC_COMBO_ROADCLASS_REF)
				ENDGROUP()
				RCSPACE(5)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
						RCTOFIT(IDC_STATIC_ROADNAME_REF)
						RCTOFIT(-1)
						RCREST(IDC_COMBO_ROADNAME_REF)
				ENDGROUP()
				RCSPACE(5)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
						RCTOFIT(IDC_STATIC_ROSENCODE)
						RCTOFIT(-1)
						RCREST(IDC_EDIT_ROSENCODE)
				ENDGROUP()
				RCSPACE(5)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
						RCTOFIT(IDC_STATIC_SEQUENCE_REF)
						RCTOFIT(-1)
						RCREST(IDC_COMBO_SEQUENCE)
				ENDGROUP()
				RCSPACE(5)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
						RCTOFIT(IDC_STATIC_SEQUENCE)
						RCTOFIT(-1)
						RCREST(IDC_EDIT_SEQUENCE)
				ENDGROUP()
				RCSPACE(5)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
						RCTOFIT(IDC_STATIC_DIRECTION)
						RCTOFIT(-1)
						RCREST(IDC_COMBO_DIRECTION)
				ENDGROUP()
				RCSPACE(5)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
						RCTOFIT(IDC_STATIC_JUNCTIONCODE)
						RCTOFIT(-1)
						RCREST(IDC_COMBO_JUNCTIONCODE)
				ENDGROUP()
				RCSPACE(5)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
						RCTOFIT(IDC_STATIC_HOUMENKANJI)
						RCTOFIT(-1)
						RCREST(IDC_EDIT_HOUMENKANJI)
				ENDGROUP()
				RCSPACE(5)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
						RCTOFIT(IDC_STATIC_HOUMENKANA)
						RCTOFIT(-1)
						RCREST(IDC_EDIT_HOUMENKANA)
				ENDGROUP()
			ENDGROUP()
		ENDGROUP()
		RCSPACE(10)
		BEGINROWS(WRCT_PCT, 0, 10)
			BEGINCOLS(WRCT_TOFIT, 20, 20)
				RCTOFIT(IDC_STATIC_SOURCE)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_SOURCE)
			ENDGROUP()
		ENDGROUP()
	ENDGROUP()
END_WINDOW_MAP()
/// �E�B���h�E�}�l�[�W���z�u�}�b�v�i�A�N�Z�X�|�C���g�A�{�ݏ��j
BEGIN_WINDOW_MAP(ACCESS_POINT_MAP2)
	BEGINROWS( WRCT_REST, 0,RCMARGINS(2,2)) 
		BEGINROWS(WRCT_PCT,0,90)
			BEGINROWS(WRCT_REST, IDC_TAB_ACCESSPOINT, RCMARGINS(2,2))
				RCSPACE(30)
				BEGINROWS(WRCT_REST, IDC_STATIC_SHISETUINFO, RCMARGINS(10,10))        // �{�ݏ��
					RCSPACE(10)
					BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
						BEGINROWS(WRCT_TOFIT, 0, RCMARGINS(0,0))
							RCTOFIT(IDC_CHECK_RESTAURANT)
							RCTOFIT(IDC_CHECK_RESTING)
							RCTOFIT(IDC_CHECK_INFOMATION)
							RCTOFIT(IDC_CHECK_SPA)
							RCTOFIT(IDC_CHECK_CASHSERVICE)
							RCTOFIT(IDC_CHECK_TOILET)
							RCTOFIT(IDC_CHECK_GS)
						ENDGROUP()
						BEGINROWS(WRCT_TOFIT, 0, RCMARGINS(0,0))
							RCTOFIT(IDC_CHECK_CAFETERIA)
							RCTOFIT(IDC_CHECK_NAP)
							RCTOFIT(IDC_CHECK_SHOWER)
							RCTOFIT(IDC_CHECK_FAX)
							RCTOFIT(IDC_CHECK_HWYOASIS)
							RCTOFIT(IDC_CHECK_HANDYCAP)
							RCTOFIT(IDC_CHECK_MULTIPLESTORE)
						ENDGROUP()
						BEGINROWS(WRCT_TOFIT, 0, RCMARGINS(0,0))
							RCTOFIT(IDC_CHECK_SOUBENIR)
							RCTOFIT(IDC_CHECK_INFODESK)
							RCTOFIT(IDC_CHECK_LAUNDRY)
							RCTOFIT(IDC_CHECK_MAILBOX)
							RCTOFIT(IDC_CHECK_DOGRUN)
						ENDGROUP()
					ENDGROUP()
				ENDGROUP()
 			ENDGROUP()
		ENDGROUP()
		RCSPACE(10)
		BEGINROWS(WRCT_PCT, 0, 10)
			BEGINCOLS(WRCT_TOFIT, 20, 20)
				RCTOFIT(IDC_STATIC_SOURCE)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_SOURCE)
			ENDGROUP()
		ENDGROUP()
	ENDGROUP()
END_WINDOW_MAP()
/// �E�B���h�E�}�l�[�W���z�u�}�b�v�i�A�N�Z�X�|�C���g�AAccess_Store�j�i[Bug5556]�Œǉ��j
BEGIN_WINDOW_MAP(ACCESS_POINT_MAP3)
	BEGINROWS(WRCT_REST, 0,RCMARGINS(2,2)) 
		BEGINROWS(WRCT_PCT,0,90)											// �^�u�y�C��
			BEGINROWS(WRCT_REST, IDC_TAB_ACCESSPOINT, RCMARGINS(2,2))
				RCSPACE(30)
				BEGINROWS(WRCT_REST, 0, RCMARGINS(0,0))
					RCPERCENT(IDC_GRID_ACCESS_STORE,50)							// �O���b�h�y�C��
					BEGINROWS(WRCT_REST, 0, RCMARGINS(10,10))					// �ҏW�\���y�C���i�}�[�W���w��p�j
						BEGINROWS(WRCT_PCT, 0, 60)									// �ҏW�\���y�C��1
							BEGINCOLS(WRCT_TOFIT, 0, 0)
								BEGINROWS(WRCT_PCT, 0, 35)								// �X�^�e�B�b�N�e�L�X�g��
									RCFIXED(IDC_STATIC_MASTERCODE,20)
									RCSPACE(5)
									RCFIXED(IDC_STATIC_START_TIME,20)
									RCSPACE(5)
									RCFIXED(IDC_STATIC_END_TIME,20)
								ENDGROUP()
								BEGINROWS(WRCT_PCT, 0, 20)								// �G�f�B�b�g�{�b�N�X��
									BEGINCOLS(WRCT_TOFIT, 0, 0)
										RCFIXED(IDC_EDIT_MASTERCODE,70)
									ENDGROUP()
									RCSPACE(5)
									BEGINCOLS(WRCT_TOFIT, 0, 0)
										RCFIXED(IDC_EDIT_START_HOUR,30)
										RCFIXED(IDC_STATIC_COLON1,10)
										RCFIXED(IDC_EDIT_START_MIN,30)
									ENDGROUP()
									RCSPACE(5)
									BEGINCOLS(WRCT_TOFIT, 0, 0)
										RCFIXED(IDC_EDIT_END_HOUR,30)
										RCFIXED(IDC_STATIC_COLON2,10)
										RCFIXED(IDC_EDIT_END_MIN,30)
									ENDGROUP()
								ENDGROUP()
								BEGINROWS(WRCT_PCT, 0, 25)							// �X�܎�ʃ��W�I�{�^����
									BEGINROWS(WRCT_REST, IDC_GROUP_STORETYPE, RCMARGINS(12,20))
										RCTOFIT(IDC_RADIO_GS)
										RCSPACE(5)
										RCTOFIT(IDC_RADIO_CHAIN)
									ENDGROUP()
								ENDGROUP()
								RCREST(-1)
								BEGINROWS(WRCT_TOFIT, 0, 0)							// �ҏW�{�^����
									BEGINROWS(WRCT_TOFIT, 0, 0)
										RCTOFIT(IDC_BUTTON_ADD_ACCESS_STORE)
										RCSPACE(5)
										RCTOFIT(IDC_BUTTON_UPDATE_ACCESS_STORE)
										RCSPACE(5)
										RCTOFIT(IDC_BUTTON_DELETE_ACCESS_STORE)
									ENDGROUP()
								ENDGROUP()
							ENDGROUP()
						ENDGROUP()
						RCREST(-1)
						BEGINROWS(WRCT_TOFIT, 0, RCMARGINS(2,2))									// �ҏW�\���y�C��2
							BEGINCOLS(WRCT_TOFIT, 0, 0)									// �`�F�b�N�{�b�N�X�s
								BEGINROWS(WRCT_TOFIT, 0, 0)
									RCTOFIT(IDC_STATIC_MONDAY)
									RCTOFIT(IDC_CHECK_MONDAY)
								ENDGROUP()
								RCSPACE(2)
								BEGINROWS(WRCT_TOFIT, 0, 0)
									RCTOFIT(IDC_STATIC_TUESDAY)
									RCTOFIT(IDC_CHECK_TUESDAY)
								ENDGROUP()
								RCSPACE(2)
								BEGINROWS(WRCT_TOFIT, 0, 0)
									RCTOFIT(IDC_STATIC_WEDNESDAY)
									RCTOFIT(IDC_CHECK_WEDNESDAY)
								ENDGROUP()
								RCSPACE(2)
								BEGINROWS(WRCT_TOFIT, 0, 0)
									RCTOFIT(IDC_STATIC_THURSDAY)
									RCTOFIT(IDC_CHECK_THURSDAY)
								ENDGROUP()
								RCSPACE(2)
								BEGINROWS(WRCT_TOFIT, 0, 0)
									RCTOFIT(IDC_STATIC_FRIDAY)
									RCTOFIT(IDC_CHECK_FRIDAY)
								ENDGROUP()
								RCSPACE(2)
								BEGINROWS(WRCT_TOFIT, 0, 0)
									RCTOFIT(IDC_STATIC_SATURDAY)
									RCTOFIT(IDC_CHECK_SATURDAY)
								ENDGROUP()
								RCSPACE(2)
								BEGINROWS(WRCT_TOFIT, 0, 0)
									RCTOFIT(IDC_STATIC_SUNDAY)
									RCTOFIT(IDC_CHECK_SUNDAY)
								ENDGROUP()
								RCSPACE(7)
								BEGINROWS(WRCT_TOFIT, 0, 0)
									RCTOFIT(IDC_STATIC_HOLIDAY)
									RCTOFIT(IDC_CHECK_HOLIDAY)
								ENDGROUP()
								RCSPACE(7)
								BEGINROWS(WRCT_FIXED, 20, 0)
									RCTOFIT(IDC_STATIC_GOLDENWEEK)
									RCTOFIT(IDC_CHECK_GOLDENWEEK)
								ENDGROUP()
								RCSPACE(7)
								BEGINROWS(WRCT_TOFIT, 0, 0)
									RCTOFIT(IDC_STATIC_BON)
									RCTOFIT(IDC_CHECK_BON)
								ENDGROUP()
								RCSPACE(7)
								BEGINROWS(WRCT_TOFIT, 0, 0)
									RCTOFIT(IDC_STATIC_YEAREND)
									RCTOFIT(IDC_CHECK_YEAREND)
								ENDGROUP()
								RCSPACE(7)
								BEGINROWS(WRCT_TOFIT, 0, 0)
									RCTOFIT(IDC_STATIC_NEWYEAR)
									RCTOFIT(IDC_CHECK_NEWYEAR)
								ENDGROUP()
								RCSPACE(10)
								RCTOFIT(IDC_BUTTON_CHECK_ALL)
								RCSPACE(5)
								RCTOFIT(IDC_BUTTON_UNCHECK_ALL)
								RCREST(-1)
								RCTOFIT(IDC_BUTTON_SAVE_ACCESS_STORE)
							ENDGROUP()
						ENDGROUP()
					ENDGROUP()
				ENDGROUP()
 			ENDGROUP()
		ENDGROUP()
		RCSPACE(10)
		BEGINROWS(WRCT_REST, 0, 0)											// ���\�[�X�y�C��
			BEGINCOLS(WRCT_TOFIT, 20, 20)
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
 * @retval CAttrAccessPointDlg��Ԃ��܂��B
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CAttrAccessPointDlg();
}


/////////////////////////////////////////////////////////////////////////////
//
// CAttrAccessPointDlg �����o�֐�
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrAccessPointDlg::CAttrAccessPointDlg()
 * @brief �R���X�g���N�^
 *
 * �u���V�̏������A����Ƀ^�u�ʒu�A�t�H���g�^�C�v�̏����������Ă����܂��B
 */
CAttrAccessPointDlg::CAttrAccessPointDlg()
	: ATTR_BASE_CLASS(ACCESS_POINT_MAP, IDD_ATTRACCESSPOINTDLG),
		m_wnd0( _T("CHECKBOX"), this, IDC_CHECK_HWYMODEFLAG ),
		m_wnd1( _T("COMBOBOX"), this, IDC_COMBO_ROADCLASS_REF ),
		m_wnd2( _T("COMBOBOX"), this, IDC_COMBO_ROADNAME_REF ), 
		m_wnd3( _T("EDIT"), this, IDC_EDIT_ROSENCODE ), 
		m_wnd4( _T("COMBOBOX"), this, IDC_COMBO_SEQUENCE ), 
		m_wnd5( _T("EDIT"), this, IDC_EDIT_SEQUENCE ), 
		m_wnd6( _T("COMBOBOX"), this, IDC_COMBO_DIRECTION ), 
		m_wnd7( _T("COMBOBOX"), this, IDC_COMBO_JUNCTIONCODE ), 
		m_wnd8( _T("EDIT"), this, IDC_EDIT_HOUMENKANJI ), 
		m_wnd9( _T("EDIT"), this, IDC_EDIT_HOUMENKANA ),
		m_wnd10( _T("EDIT"), this, IDC_EDIT_SOURCE ),
		m_wndInfo1( _T("CHECKBOX"), this, IDC_CHECK_RESTAURANT ),
		m_wndInfo2( _T("CHECKBOX"), this, IDC_CHECK_RESTING ),
		m_wndInfo3( _T("CHECKBOX"), this, IDC_CHECK_INFOMATION ),
		m_wndInfo4( _T("CHECKBOX"), this, IDC_CHECK_SPA ),
		m_wndInfo5( _T("CHECKBOX"), this, IDC_CHECK_CASHSERVICE ),
		m_wndInfo6( _T("CHECKBOX"), this, IDC_CHECK_TOILET ),
		m_wndInfo7( _T("CHECKBOX"), this, IDC_CHECK_CAFETERIA ),
		m_wndInfo8( _T("CHECKBOX"), this, IDC_CHECK_NAP ),
		m_wndInfo9( _T("CHECKBOX"), this, IDC_CHECK_SHOWER ),
		m_wndInfo10( _T("CHECKBOX"), this, IDC_CHECK_FAX ),
		m_wndInfo11( _T("CHECKBOX"), this, IDC_CHECK_HWYOASIS ),
		m_wndInfo12( _T("CHECKBOX"), this, IDC_CHECK_HANDYCAP ),
		m_wndInfo13( _T("CHECKBOX"), this, IDC_CHECK_SOUBENIR ),
		m_wndInfo14( _T("CHECKBOX"), this, IDC_CHECK_INFODESK ),
		m_wndInfo15( _T("CHECKBOX"), this, IDC_CHECK_LAUNDRY ),
		m_wndInfo16( _T("CHECKBOX"), this, IDC_CHECK_MAILBOX ),
		m_wndInfo19( _T("CHECKBOX"), this, IDC_CHECK_GS ),
		m_wndInfo20( _T("CHECKBOX"), this, IDC_CHECK_MULTIPLESTORE ),
		m_wndInfo21( _T("CHECKBOX"), this, IDC_CHECK_DOGRUN ),
		m_wndAS1( _T("EDIT"), this, IDC_EDIT_MASTERCODE ),					// [Bug5556]�Œǉ�
		m_wndAS2( _T("EDIT"), this, IDC_EDIT_START_HOUR ),					// [Bug5556]�Œǉ�
		m_wndAS3( _T("EDIT"), this, IDC_EDIT_START_MIN ),					// [Bug5556]�Œǉ�
		m_wndAS4( _T("EDIT"), this, IDC_EDIT_END_HOUR ),					// [Bug5556]�Œǉ�
		m_wndAS5( _T("EDIT"), this, IDC_EDIT_END_MIN ),						// [Bug5556]�Œǉ�
		m_wndAS6( _T("CHECKBOX"), this, IDC_CHECK_MONDAY ),					// [Bug5556]�Œǉ�
		m_wndAS7( _T("CHECKBOX"), this, IDC_CHECK_TUESDAY ),				// [Bug5556]�Œǉ�
		m_wndAS8( _T("CHECKBOX"), this, IDC_CHECK_WEDNESDAY ),				// [Bug5556]�Œǉ�
		m_wndAS9( _T("CHECKBOX"), this, IDC_CHECK_THURSDAY ),				// [Bug5556]�Œǉ�
		m_wndAS10( _T("CHECKBOX"), this, IDC_CHECK_FRIDAY ),				// [Bug5556]�Œǉ�
		m_wndAS11( _T("CHECKBOX"), this, IDC_CHECK_SATURDAY ),				// [Bug5556]�Œǉ�
		m_wndAS12( _T("CHECKBOX"), this, IDC_CHECK_SUNDAY ),				// [Bug5556]�Œǉ�
		m_wndAS13( _T("CHECKBOX"), this, IDC_CHECK_HOLIDAY ),				// [Bug5556]�Œǉ�
		m_wndAS14( _T("CHECKBOX"), this, IDC_CHECK_GOLDENWEEK ),			// [Bug5556]�Œǉ�
		m_wndAS15( _T("CHECKBOX"), this, IDC_CHECK_BON ),					// [Bug5556]�Œǉ�
		m_wndAS16( _T("CHECKBOX"), this, IDC_CHECK_YEAREND ),				// [Bug5556]�Œǉ�
		m_wndAS17( _T("CHECKBOX"), this, IDC_CHECK_NEWYEAR ),				// [Bug5556]�Œǉ�
		m_wndAS18( _T("BUTTON"), this, IDC_BUTTON_CHECK_ALL ),				// [Bug5556]�Œǉ�
		m_wndAS19( _T("BUTTON"), this, IDC_BUTTON_UNCHECK_ALL ),			// [Bug5556]�Œǉ�
		m_wndAS20( _T("BUTTON"), this, IDC_BUTTON_ADD_ACCESS_STORE ),		// [Bug5556]�Œǉ�
		m_wndAS21( _T("BUTTON"), this, IDC_BUTTON_UPDATE_ACCESS_STORE ),	// [Bug5556]�Œǉ�
		m_wndAS22( _T("BUTTON"), this, IDC_BUTTON_DELETE_ACCESS_STORE ),	// [Bug5556]�Œǉ�
		m_wndAS23( _T("BUTTON"), this, IDC_BUTTON_SAVE_ACCESS_STORE ),		// [Bug5556]�Œǉ�
		m_wndTab( _T("TAB"), this, IDC_TAB_ACCESSPOINT )
{
	// �^�u�ʒu�̏�����
	m_iChangeType = 0;
	m_lAccessTabNum = 0;
	// �t�H���g�쐬
	m_hFont = CreateFont(
		18, 0, 0, 0, 100,		// �����C���C�w�p�x�C�x�p�x�C����
		FALSE, FALSE, FALSE,    // �ΆC�����C�ŏ���
		SHIFTJIS_CHARSET,       // �L�����N�^�ݒ�
		OUT_DEFAULT_PRECIS,     // �o�͐��x
		CLIP_DEFAULT_PRECIS,    // �N���b�v���x
		DEFAULT_QUALITY,        // �i��
		FIXED_PITCH | FF_DONTCARE,// �s�b�`
		AheLoadString(IDS_FONT));   // �t�H���g���
}

/** 
 * @fn HWND CAttrAccessPointDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief �q�_�C�A���O���쐬���܂��B
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)�Ń_�C�A���O���쐬���܂��B
 *
 * @param hWndParent	[in]	�e�E�B���h�E�n���h��
 * @param dwInitParam	[in]	�_�C�A���O�������p�����[�^�i���ݓ��Ɏg�p���Ă��܂���j
 *
 * @retval �쐬���ꂽ�q�_�C�A���O�̃n���h����Ԃ��܂��B
 */
HWND CAttrAccessPointDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	m_hChildWnd = ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
	return m_hChildWnd;
}

/**
 * @fn void CAttrAccessPointDlg::Delete()
 * @brief �쐬�����q�_�C�A���O���폜���܂��B
 *
 * �_�C�A���O���폜����Ƃ��ɕK���Ă�ŉ������B�Ă΂Ȃ��ƃ��������[�N���Ă��܂��܂��B
 */
void CAttrAccessPointDlg::Delete()
{
	ClearFeatureDefs();
	::DestroyWindow(m_hChildWnd);
    delete this; // �K��new�Ŋm�ۂ���O��
}

/**
 * @fn void CAttrAccessPointDlg::SetArcHelper( IApplication* ipApp )
 * @brief ATTR_BASE_CLASS��ArcHelper��n���܂��B
 */
void CAttrAccessPointDlg::SetArcHelper( IApplication* ipApp )
{ 
    ATTR_BASE_CLASS::SetArcHelper( ipApp );
}

/**
 * @fn BOOL CAttrAccessPointDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
 * @brief ���ݕ\�������ׂ��t�B�[�`����FeatureDef�C���f�b�N�X��n���܂��B
 * ����ɁA�t�B�[�`���̑����Ƃ͊֌W�Ȃ��R���g���[���̐�������Ă����܂��B
 * �H���R�[�h�A�H�����V�[�P���X�̐ݒ�����Ă����܂��B
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
BOOL CAttrAccessPointDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
    BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, m_lAccessTabNum, bForce, bEditable );

	m_lFeatureIndex = lFeatureIndex;
	m_lTableIndex = lTableIndex;
	m_lRowIndex = lRowIndex;
	m_bForce = bForce;
	m_bEditable = bEditable;

    // �H���R�[�h�A�H�����V�[�P���X�̏����l�m��
    CComVariant		vaRoadSequence;
    CFieldDef*		pFieldDef;
    CComVariant		vaValue;
	// ���ݑI�𒆂̃t�B�[�`������H�����V�[�P���X���擾
    pFieldDef = GetCurrentFeatureDef()->GetFieldDef( ACCESS_POINT_TABLE_NAME, _T("ROAD_SEQ") );
    vaValue= pFieldDef->m_vaValue;
    m_lOrgRoadSequence = vaValue.lVal;
	// ���ݑI�𒆂̃t�B�[�`������H���R�[�h�擾
    pFieldDef = GetCurrentFeatureDef()->GetFieldDef( ACCESS_POINT_TABLE_NAME, _T("ROAD_CODE") );
    vaValue = pFieldDef->m_vaValue;
    m_lOrgRoadCode = vaValue.lVal;

    // �ŏ�����H���R�[�h���o������
    SetLineClassList(m_lOrgRoadCode);
	// �Q�Ɨp�̘H�����V�[�P���X�R���{�{�b�N�X�̃Z�b�g
    ChangeRoadSeqCombobox();

	// ���ڃt�B�[�`���Ɋ֌W�Ȃ��G�f�B�b�g�{�b�N�X(��F�Q�Ɨp�̘H�����V�[�P���X�⌴�e�ԍ�����)�Ȃǂ𐧌䂷��
	// �ҏW�J�n�O�ƕҏW�J�n��ł��ҏW�s�̂Ƃ���DISABLE�ɂ���
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		// �ҏW�J�n����Ă���Ƃ��ɂ��łɁASetFocus���Ă���
		if( m_lAccessTabNum == 0 )
			::SetFocus( GetDlgItem(IDC_CHECK_HWYMODEFLAG) );
		else 
			::SetFocus( GetDlgItem(IDC_CHECK_RESTAURANT) );
	}

	// �n�C�E�F�C���[�h�t���O�̃`�F�b�N�{�b�N�X�͏��Disable�i�\����p�j�ɂ���i[Bug 6217]�Ή����ɒǉ��j
	SetDlgStatus(IDC_CHECK_HWYMODEFLAG, FALSE);

	// Access_Store�^�u���A�N�e�B�u�Ȃ�i[Bug5556]�Œǉ��j
	if (m_lAccessTabNum == 2)
	{
		// Access_Store�e�[�u����ǂݍ���
		LoadAccessStore();
		// Access_Store���O���b�h�X�V
		UpdateASGrid();
	}

    return bRet;
}

/**
 * @brief �t�B�[���h���ɑΉ�����O���b�h��̃t�B�[���h�C���f�b�N�X��Ԃ�
 * @note	�����ȃt�B�[���h�����w�肳�ꂽ��std::runtime_error��O�𑗏o����B
 * @param[in]	cFieldName	�Ώۃt�B�[���h��
 * @return	�t�B�[���h�C���f�b�N�X
 */
long ASGridFieldIndex( tstring cFieldName )
{
	using namespace sindy::schema;
	typedef tstring FieldName;
	typedef long FieldIndex;
	typedef std::map<FieldName, FieldIndex> FieldIndexMap;
	static FieldIndexMap aFieldIndexMap = boost::assign::map_list_of
		( tstring( _T("OBJECTID") ),					0 )
		( tstring( access_store::kMultipleStore ),		1 )
		( tstring( access_store::kStartHour ),			2 )
		( tstring( access_store::kStartMin ),			3 )
		( tstring( access_store::kEndHour ),			4 )
		( tstring( access_store::kEndMin ),				5 )
		( tstring( access_store::kMonday ),				6 )
		( tstring( access_store::kTuesday ),			7 )
		( tstring( access_store::kWednesday ),			8 )
		( tstring( access_store::kThursday ),			9 )
		( tstring( access_store::kFriday ),				10 )
		( tstring( access_store::kSaturday ),			11 )
		( tstring( access_store::kSunday ),				12 )
		( tstring( access_store::kHoliday ),			13 )
		( tstring( access_store::kGoldenWeek ),			14 )
		( tstring( access_store::kBon ),				15 )
		( tstring( access_store::kYearEnd ),			16 )
		( tstring( access_store::kNewYear ),			17 )
		( tstring( access_store::kStoreType ),			18 )
	;

	FieldIndexMap::const_iterator it = aFieldIndexMap.find( cFieldName );
	if (it == aFieldIndexMap.end())
	{
		ATLASSERT(0);
		throw std::runtime_error( "ASGridFieldIndex()���s" );
	}

	return it->second;
}

void CAttrAccessPointDlg::UpdateASGrid()
{
	// �Œ�s�ȊO�̍s���폜
	m_ipAccessStoreGrid->PutRows( 1 );

	std::locale loc( std::locale( "japanese" ), "C", std::locale::numeric );

	// �J�����g�ȃA�N�Z�X�|�C���g��ID���擾
	CFeatureDef* pFeatureDef = GetCurrentFeatureDef();
	ATLASSERT( pFeatureDef );
	if (! pFeatureDef) { return; }
	long aCurrentAccessPointID = pFeatureDef->GetFeature()->OID;

	bool aSucceeded = true;
	using namespace access_store_cache;
	const CSortedASRows* pSortedRows = m_ASCache.sortRows();
	if (! pSortedRows) { aSucceeded = false; }

	if (aSucceeded)
	{
		try
		{
			long aRowIndex = 0;
			BOOST_FOREACH (const CSortedASRows::value_type& rElement, *pSortedRows)
			{
				const CASRow& rRow = rElement.second;

				// �J�����g�ȃA�N�Z�X�|�C���g�Ɋ֘A�t�����Ă��Ȃ��s�Ȃ�X�L�b�v
				using namespace sindy::schema;
				if (aCurrentAccessPointID != rRow.validField( access_store::kAccessPointID ).m_NewValue)
					{ continue;	}

				++aRowIndex;

				std::stringstream ss;
				ss.imbue( loc );
				std::string fd = "\t";

				ss << rRow.validField( _T("OBJECTID") ).m_NewValue << fd;
				ss << rRow.validField( access_store::kMultipleStore ).m_NewValue << fd;
				ss << rRow.validField( access_store::kStartHour ).m_NewValue << fd;
				ss << rRow.validField( access_store::kStartMin ).m_NewValue << fd;
				ss << rRow.validField( access_store::kEndHour ).m_NewValue << fd;
				ss << rRow.validField( access_store::kEndMin ).m_NewValue << fd;
				ss << rRow.validField( access_store::kMonday ).m_NewValue << fd;
				ss << rRow.validField( access_store::kTuesday ).m_NewValue << fd;
				ss << rRow.validField( access_store::kWednesday ).m_NewValue << fd;
				ss << rRow.validField( access_store::kThursday ).m_NewValue << fd;
				ss << rRow.validField( access_store::kFriday ).m_NewValue << fd;
				ss << rRow.validField( access_store::kSaturday ).m_NewValue << fd;
				ss << rRow.validField( access_store::kSunday ).m_NewValue << fd;
				ss << rRow.validField( access_store::kHoliday ).m_NewValue << fd;
				ss << rRow.validField( access_store::kGoldenWeek ).m_NewValue << fd;
				ss << rRow.validField( access_store::kBon ).m_NewValue << fd;
				ss << rRow.validField( access_store::kYearEnd ).m_NewValue << fd;
				ss << rRow.validField( access_store::kNewYear ).m_NewValue << fd;
				ss << rRow.validField( access_store::kStoreType ).m_NewValue;

				m_ipAccessStoreGrid->AddItem( _bstr_t( ss.str().c_str() ), aRowIndex );
			}
		}
		// CASRow::validField()���s���ɑ��o������O��catch�����珈���𒆒f����B
		catch (...)
		{
			ATLASSERT(0);
			aSucceeded = false;
		}
	}

	// ���s���Ă����珈���𒆒f����
	if (! aSucceeded)
	{
		AttrMessageBox( AheLoadString( IDS_FAILTOUPDATEACCESSSTORE ), AheLoadString( IDS_CAPTION ), MB_OK, NULL );
		return;
	}

	// �O���b�h�ɌŒ�s�ȊO�̍s�����݂���ꍇ
	long aRows = m_ipAccessStoreGrid->GetRows();
	if (m_ipAccessStoreGrid->GetRows() > 1)
	{
		// �P�s�ڂ�I��
		m_ipAccessStoreGrid->PutRow( 1 );

		// �Z���̐F��ݒ�
		SetASGridColor();
	}

	// �O���b�h�ȊO�̃R���g���[�����X�V
	UpdateASControls();
}

void CAttrAccessPointDlg::UpdateASControls()
{
	// �Œ�s�ȊO�̍s�����݂��Ȃ��ꍇ
	if (m_ipAccessStoreGrid->GetRows() <= 1)
	{
		BOOST_FOREACH (long aControlID, GetASEditableControlIDs())
		{
			// �u�ǉ��v�{�^����Enabled/Disabled��ҏW�ۂɉ����Đݒ肷��
			if (aControlID == IDC_BUTTON_ADD_ACCESS_STORE)
			{
				SetDlgStatus( aControlID, IsEditableFeature() );
			}
			// �u�ǉ��v�{�^���ȊO�̕ҏW�\�R���g���[����Disabled�ɂ���
			else
			{
				SetDlgStatus( aControlID, FALSE );
			}
		}
		return;		// �ȉ��̏����͍s��Ȃ�
	}
	// �Œ�s�ȊO�̍s�����݂���ꍇ
	// �ҏW�\��ԂȂ�S�Ă̕ҏW�\�R���g���[����Enabled�ɁA�s�\��ԂȂ�Disabled�ɂ���
	BOOST_FOREACH (long aControlID, GetASEditableControlIDs())
	{
		SetDlgStatus( aControlID, IsEditableFeature() );
	}

	long aRow = m_ipAccessStoreGrid->GetRow();

	try
	{
		using namespace sindy::schema;
		UpdateASEditControl( aRow, ASGridFieldIndex( access_store::kMultipleStore ),	IDC_EDIT_MASTERCODE );
		UpdateASEditControl( aRow, ASGridFieldIndex( access_store::kStartHour ),		IDC_EDIT_START_HOUR );
		UpdateASEditControl( aRow, ASGridFieldIndex( access_store::kStartMin ),			IDC_EDIT_START_MIN );
		UpdateASEditControl( aRow, ASGridFieldIndex( access_store::kEndHour ),			IDC_EDIT_END_HOUR );
		UpdateASEditControl( aRow, ASGridFieldIndex( access_store::kEndMin ),			IDC_EDIT_END_MIN );
		UpdateASCheckBox( aRow, ASGridFieldIndex( access_store::kMonday ),				IDC_CHECK_MONDAY );
		UpdateASCheckBox( aRow, ASGridFieldIndex( access_store::kTuesday ),				IDC_CHECK_TUESDAY );
		UpdateASCheckBox( aRow, ASGridFieldIndex( access_store::kWednesday ),			IDC_CHECK_WEDNESDAY );
		UpdateASCheckBox( aRow, ASGridFieldIndex( access_store::kThursday ),			IDC_CHECK_THURSDAY );
		UpdateASCheckBox( aRow, ASGridFieldIndex( access_store::kFriday ),				IDC_CHECK_FRIDAY );
		UpdateASCheckBox( aRow, ASGridFieldIndex( access_store::kSaturday ),			IDC_CHECK_SATURDAY );
		UpdateASCheckBox( aRow, ASGridFieldIndex( access_store::kSunday ),				IDC_CHECK_SUNDAY );
		UpdateASCheckBox( aRow, ASGridFieldIndex( access_store::kHoliday ),				IDC_CHECK_HOLIDAY );
		UpdateASCheckBox( aRow, ASGridFieldIndex( access_store::kGoldenWeek ),			IDC_CHECK_GOLDENWEEK );
		UpdateASCheckBox( aRow, ASGridFieldIndex( access_store::kBon ),					IDC_CHECK_BON );
		UpdateASCheckBox( aRow, ASGridFieldIndex( access_store::kYearEnd ),				IDC_CHECK_YEAREND );
		UpdateASCheckBox( aRow, ASGridFieldIndex( access_store::kNewYear ),				IDC_CHECK_NEWYEAR );

		long aStoreType = static_cast<long>( m_ipAccessStoreGrid->GetValueMatrix( aRow, ASGridFieldIndex( access_store::kStoreType ) ) );
		switch (aStoreType)
		{
			using namespace sindy::schema::access_store::store_type;
			case kGS:
				CButton( GetDlgItem( IDC_RADIO_GS ) ).SetCheck( BST_CHECKED );
				CButton( GetDlgItem( IDC_RADIO_CHAIN ) ).SetCheck( BST_UNCHECKED );
				break;
			case kChainStore:
				CButton( GetDlgItem( IDC_RADIO_CHAIN ) ).SetCheck( BST_CHECKED );
				CButton( GetDlgItem( IDC_RADIO_GS ) ).SetCheck( BST_UNCHECKED );
				break;
		}
	}
	// ASGridFieldIndex���s���ɑ��o������O��catch�����珈���𒆒f����B
	catch (...)
	{
		AttrMessageBox( AheLoadString( IDS_FAILTOUPDATEACCESSSTORE ), AheLoadString( IDS_CAPTION ), MB_OK, NULL );
		return;
	}
}

void CAttrAccessPointDlg::SetASGridColor()
{
	// �Œ�s�ȊO�̍s�����݂��Ȃ���΃X�L�b�v
	if (m_ipAccessStoreGrid->GetRows() <= 1)
		{ return; }

	// �O���b�h�p�Ƀ\�[�g���ꂽ�s�Q���擾
	long aCurrentRow = m_ipAccessStoreGrid->GetRow();
	long aRowIndex = 0;
	using namespace access_store_cache;
	const CSortedASRows& rSortedRows = m_ASCache.sortedRows();

	// �J�����g�ȃA�N�Z�X�|�C���g��ID���擾
	CFeatureDef* pFeatureDef = GetCurrentFeatureDef();
	ATLASSERT( pFeatureDef );
	if (! pFeatureDef) { return; }
	long aCurrentAccessPointID = pFeatureDef->GetFeature()->OID;

	try
	{
		BOOST_FOREACH (const CSortedASRows::value_type& rElement, rSortedRows)
		{
			const CASRow& rRow = rElement.second;
			
			// �J�����g�ȃA�N�Z�X�|�C���g�Ɋ֘A�t�����Ă��Ȃ��s�Ȃ�X�L�b�v
			using namespace sindy::schema;
			if (aCurrentAccessPointID != rRow.validField( access_store::kAccessPointID ).m_NewValue)
				{ continue;	}

			++aRowIndex;
			bool aIsCurrentRow = aCurrentRow == aRowIndex;

			// �Z���̐F��ς���
			// �D�揇�ʂ́u�폜�s�Ȃ�O���[�v���u�ǉ��s�Ȃ�I�����W�v���u�X�V�Z���Ȃ�I�����W�v
			// �폜or�ǉ�or�X�V����Ă���J�����g�Z���Ȃ�F��Z�����A�X�V����Ă��Ȃ��J�����g�Z���͗ΐF�ɂ���
			if (rRow.m_IsDeleted)
			{
				m_ipAccessStoreGrid->PutCell( flexcpBackColor, aRowIndex, 0, aRowIndex, m_ipAccessStoreGrid->GetCols() - 1, 
					aIsCurrentRow ? RGB(105,105,105) : RGB(211,211,211) );
			}
			else if (rRow.m_IsAdded)
			{
				m_ipAccessStoreGrid->PutCell( flexcpBackColor, aRowIndex, 0, aRowIndex, m_ipAccessStoreGrid->GetCols() - 1, 
					aIsCurrentRow ? RGB(255,140,0) : RGB(255,218,185) );
			}
			else
			{
				// �ύX����Ă���t�B�[���h�����F��ς���
				BOOST_FOREACH (const CASFields::value_type& rElement, rRow.m_Fields)
				{
					tstring aFieldName = rElement.first;
					const CASField& rField = rElement.second;

					// �A�N�Z�X�|�C���gID�Ȃ�X�L�b�v�i�Ή�����t�B�[���h���O���b�h��ɂȂ����߁j
					if (aFieldName == sindy::schema::access_store::kAccessPointID)
						{ continue; }

					long aFieldIndex = ASGridFieldIndex( aFieldName );

					if (rField.m_OrgValue != rField.m_NewValue)
					{
						m_ipAccessStoreGrid->PutCell( flexcpBackColor, aRowIndex, aFieldIndex, aRowIndex, aFieldIndex, 
							aIsCurrentRow ? RGB(255,140,0) : RGB(255,218,185) );
					}
					else
					{
						m_ipAccessStoreGrid->PutCell( flexcpBackColor, aRowIndex, aFieldIndex, aRowIndex, aFieldIndex,
							aIsCurrentRow ? RGB(0,255,127) : RGB(255,255,255));
					}
				}
			}
		}
	}
	// CASRow::validField()�܂���ASGridFieldIndex���s���ɑ��o������O��catch�����珈���𒆒f����B
	catch (...)
	{
		AttrMessageBox( AheLoadString( IDS_FAILTOUPDATEACCESSSTORE ), AheLoadString( IDS_CAPTION ), MB_OK, NULL );
		return;
	}


}

void CAttrAccessPointDlg::UpdateASEditControl( long cRow, long cCol, long cControlID )
{
	ATLASSERT( m_ipAccessStoreGrid );
	if (! m_ipAccessStoreGrid) { return; }

	long aGridValue = static_cast<long>( m_ipAccessStoreGrid->GetValueMatrix( cRow, cCol ) );
	CString aStr;
	aStr.Format( _T("%d"), aGridValue );
	CEdit( GetDlgItem( cControlID ) ).SetWindowText( aStr );
}

void CAttrAccessPointDlg::UpdateASCheckBox( long cRow, long cCol, long cControlID )
{
	ATLASSERT( m_ipAccessStoreGrid );
	if (! m_ipAccessStoreGrid) { return; }

	long aGridValue = static_cast<long>( m_ipAccessStoreGrid->GetValueMatrix( cRow, cCol ) );
	CButton( GetDlgItem( cControlID ) ).SetCheck( aGridValue ==0 ? BST_UNCHECKED : BST_CHECKED );
}

void CAttrAccessPointDlg::LoadAccessStore()
{
	m_ASCache.clear();

	typedef std::set<long> OIDs;
	OIDs aSelectedAccessPointIDs;

	ATLASSERT( m_pFeatureDefList->size() );
	if (! m_pFeatureDefList->size())
		{ return; }
	IFeaturePtr ipAccessPoint( m_pFeatureDefList->begin()->GetFeature() );

	// �I������Ă���A�N�Z�X�|�C���g�Q�̂P�ڂƓ���DB��Access_Store�e�[�u�����擾
	ITablePtr ipAccessStoreTable = AheOpenSameOwnerTable( (ITablePtr)AheGetFeatureClass( ipAccessPoint ), sindy::schema::access_store::kTableName );
	if (! ipAccessStoreTable)
	{
		AttrMessageBox( AheLoadString( IDS_FAILTOUPDATEACCESSSTORE ), AheLoadString( IDS_CAPTION ), MB_OK, NULL );
		return;
	}

	// �I������Ă���A�N�Z�X�|�C���g�Q�Ɋ֘A�t���Ă���Access_Store�I�u�W�F�N�g�Q���擾
	BOOST_FOREACH (CFeatureDef& rFeatureDef, *m_pFeatureDefList)
	{
		aSelectedAccessPointIDs.insert( rFeatureDef.GetFeature()->OID );
	}

	std::stringstream ss;
	ss.imbue( std::locale( std::locale( "japanese" ), "C", std::locale::numeric ) );
	ss << CT2A( sindy::schema::access_store::kAccessPointID ) << " IN(";
	for (OIDs::iterator it = aSelectedAccessPointIDs.begin(); it != aSelectedAccessPointIDs.end(); ++it)
	{
		if (it != aSelectedAccessPointIDs.begin())
		{
			ss << ",";
		}
		ss << *it;
	}
	ss << ")";

	_ICursorPtr ipCursor( ipAccessStoreTable->_Search( AheInitQueryFilter( NULL, NULL, CA2T( ss.str().c_str() ) ), VARIANT_FALSE ) );

	// �擾�����I�u�W�F�N�g�Q���L���b�V���Ɋi�[
	_IRowPtr ipRow;
	while (ipCursor->NextRow( &ipRow ) == S_OK && ipRow)
	{
		m_ASCache.addRow( ipRow );
	}
}

void CAttrAccessPointDlg::EditAccessStore( long cControlID )
{
	try
	{
		// �u�ǉ��v�{�^���������ꂽ���̏���
		if (cControlID == IDC_BUTTON_ADD_ACCESS_STORE)
		{
			// �I������Ă���A�N�Z�X�|�C���g��ID���擾
			CFeatureDef* pFeatureDef = GetCurrentFeatureDef();
			ATLASSERT( pFeatureDef );
			if (! pFeatureDef) { return; }
			long aCurrentAccessPointID = pFeatureDef->GetFeature()->OID;

			// �L���b�V���ɍs��ǉ�
			m_ASCache.addRow( aCurrentAccessPointID );

			// �O���b�h���X�V
			UpdateASGrid();
		}

		// �u�X�V�v�{�^���������ꂽ���̏���
		if (cControlID == IDC_BUTTON_UPDATE_ACCESS_STORE)
		{
			long aCurrentRowIndex = m_ipAccessStoreGrid->GetRow();
			long aAccessStoreID = static_cast<long>( m_ipAccessStoreGrid->GetValueMatrix( aCurrentRowIndex, ASGridFieldIndex( _T("OBJECTID") ) ) );

			using namespace sindy::schema;
			// �G�f�B�b�g�R���g���[���Q����擾�����l���L���b�V���ɔ��f
			if (! UpdateASCacheByEditControls( IDC_EDIT_MASTERCODE,	aAccessStoreID ))
				{ return; }		// ���s�����璆�f

			// �`�F�b�N�{�b�N�X�Q����擾�����l���L���b�V���ɔ��f
			UpdateASCacheByCheckBox( IDC_CHECK_MONDAY,		aAccessStoreID,	access_store::kMonday );
			UpdateASCacheByCheckBox( IDC_CHECK_TUESDAY,		aAccessStoreID,	access_store::kTuesday );
			UpdateASCacheByCheckBox( IDC_CHECK_WEDNESDAY,	aAccessStoreID,	access_store::kWednesday );
			UpdateASCacheByCheckBox( IDC_CHECK_THURSDAY,	aAccessStoreID,	access_store::kThursday );
			UpdateASCacheByCheckBox( IDC_CHECK_FRIDAY,		aAccessStoreID,	access_store::kFriday );
			UpdateASCacheByCheckBox( IDC_CHECK_SATURDAY,	aAccessStoreID,	access_store::kSaturday );
			UpdateASCacheByCheckBox( IDC_CHECK_SUNDAY,		aAccessStoreID,	access_store::kSunday );
			UpdateASCacheByCheckBox( IDC_CHECK_HOLIDAY,		aAccessStoreID,	access_store::kHoliday );
			UpdateASCacheByCheckBox( IDC_CHECK_GOLDENWEEK,	aAccessStoreID,	access_store::kGoldenWeek );
			UpdateASCacheByCheckBox( IDC_CHECK_BON,			aAccessStoreID,	access_store::kBon );
			UpdateASCacheByCheckBox( IDC_CHECK_YEAREND,		aAccessStoreID,	access_store::kYearEnd );
			UpdateASCacheByCheckBox( IDC_CHECK_NEWYEAR,		aAccessStoreID,	access_store::kNewYear );

			// ���W�I�{�^���Q����擾�����l���L���b�V���ɔ��f
			if (CButton( GetDlgItem( IDC_RADIO_GS ) ).GetCheck() == BST_CHECKED)
			{
				m_ASCache.updateField( aAccessStoreID, access_store::kStoreType, access_store::store_type::kGS );
			}
			else if (CButton( GetDlgItem( IDC_RADIO_CHAIN ) ).GetCheck() == BST_CHECKED)
			{
				m_ASCache.updateField( aAccessStoreID, access_store::kStoreType, access_store::store_type::kChainStore );
			}

			// �O���b�h���X�V
			UpdateASGrid();

			// �O���b�h�̍s�̒�����X�V���ꂽ�s��T���Ĕ�I����Ԃɂ���
			for (long i = 0; i < m_ipAccessStoreGrid->GetRows(); ++i)
			{
				long aAccessStoreID2 = static_cast<long>( m_ipAccessStoreGrid->GetValueMatrix( i, ASGridFieldIndex( _T("OBJECTID") ) ) );
				if (aAccessStoreID == aAccessStoreID2)
				{
					m_ipAccessStoreGrid->PutRow( i );
					break;
				}
			}

			// �J�����g�Z���̈ʒu�܂ŃX�N���[��
			m_ipAccessStoreGrid->ShowCell( m_ipAccessStoreGrid->GetRow(), m_ipAccessStoreGrid->GetCol() );
		}

		// �u�폜�v�{�^���������ꂽ���̏���
		if (cControlID == IDC_BUTTON_DELETE_ACCESS_STORE)
		{
			long aCurrentRowIndex = m_ipAccessStoreGrid->GetRow();
			long aAccessStoreID = static_cast<long>( m_ipAccessStoreGrid->GetValueMatrix( aCurrentRowIndex, ASGridFieldIndex( _T("OBJECTID") ) ) );

			m_ASCache.changeDeleteFlag( aAccessStoreID );

			// �O���b�h���X�V
			UpdateASGrid();

			// ���X�I������Ă����s��I��
			m_ipAccessStoreGrid->PutRow( aCurrentRowIndex );
		}
	}
	// ASGridFieldIndex���s���ɑ��o������O��catch�����珈���𒆒f����B
	catch (...)
	{
		AttrMessageBox( AheLoadString( IDS_FAILTOUPDATEACCESSSTORE ), AheLoadString( IDS_CAPTION ), MB_OK, NULL );
		return;
	}
}

void CAttrAccessPointDlg::UpdateASCacheByEditControl( long cControlID, long cObjectID, tstring cFieldName )
{
	long aNum = GetNumFromEditControl( cControlID );
	m_ASCache.updateField( cObjectID, cFieldName, aNum );
}

long CAttrAccessPointDlg::GetNumFromEditControl( long cControlID )
{
	CString lpszStr;
	CEdit( GetDlgItem( cControlID ) ).GetWindowText( lpszStr );
	return boost::lexical_cast<long>( lpszStr.GetString() );
}

bool CAttrAccessPointDlg::UpdateASCacheByEditControls( long cControlID, long cObjectID )
{
	CString aControlName;
	try
	{
		// �G�f�B�b�g�R���g���[���Q���琔�l�𐳏�Ɏ擾�ł��邩�ǂ�������
		aControlName = AheLoadString( IDS_CHAINMASTERCODE );
		GetNumFromEditControl( IDC_EDIT_MASTERCODE );
		aControlName = AheLoadString( IDS_OPENINGTIME );
		GetNumFromEditControl( IDC_EDIT_START_HOUR );
		GetNumFromEditControl( IDC_EDIT_START_MIN );
		aControlName = AheLoadString( IDS_CLOSINGTIME );
		GetNumFromEditControl( IDC_EDIT_END_HOUR );
		GetNumFromEditControl( IDC_EDIT_END_MIN );
	}
	// �G�f�B�b�g�R���g���[���ւ̃y�[�X�g�ɂ�蔼�p�����ȊO�̕��������͂���Ă����UpdateASCacheByEditControl()����boost::lexical_cast�����s����
	// ��O�����o�����̂ł�����catch���A���̏ꍇ�͏����𒆒f����B
	catch (...)
	{
		CString aStr;
		aStr.Format( AheLoadString( IDS_INVALID ), aControlName );
		AttrMessageBox( aStr, AheLoadString( IDS_CAPTION ), MB_OK, NULL );
		return false;
	}

	// ���Ȃ���΃L���b�V�����X�V
	using namespace sindy::schema;
	UpdateASCacheByEditControl( IDC_EDIT_MASTERCODE,	cObjectID, access_store::kMultipleStore );
	UpdateASCacheByEditControl( IDC_EDIT_START_HOUR,	cObjectID, access_store::kStartHour );
	UpdateASCacheByEditControl( IDC_EDIT_START_MIN,		cObjectID, access_store::kStartMin );
	UpdateASCacheByEditControl( IDC_EDIT_END_HOUR,		cObjectID, access_store::kEndHour );
	UpdateASCacheByEditControl( IDC_EDIT_END_MIN,		cObjectID, access_store::kEndMin );

	return true;
}

void CAttrAccessPointDlg::UpdateASCacheByCheckBox( long cControlID, long cObjectID, tstring cFieldName )
{
	m_ASCache.updateField( cObjectID, cFieldName, CButton( GetDlgItem( cControlID ) ).GetCheck() == BST_CHECKED ? 1 : 0 );
}

namespace access_store_row_edit_type
{
	/**
	 * @brief �s�ɑ΂���X�V���e�̎��
	 */
	enum ECode
	{
		none,				///< ����
		added,				///< �ǉ�
		deleted,			///< �폜
		may_be_updated,		///< �X�V�܂��͕ύX�Ȃ�
	};

	/**
	 * @brief �ǉ��t���O�ƍ폜�t���O�ɑΉ�����u�X�V���e�̎�ށv�iaccess_store_row_edit_type::ECode�j��Ԃ�
	 * @param[in]	cIsAdded	�ǉ��t���O
	 * @param[in]	cIsDeleted	�폜�t���O
	 * @return	�X�V���e�̎��
	 */
	ECode code( bool cIsAdded, bool cIsDeleted )
	{
		ECode aEditType = none;
		if (cIsAdded)
		{
			if (cIsDeleted)
			{
				aEditType = none;
			}
			else
			{
				aEditType = added;
			}
		}
		else
		{
			if (cIsDeleted)
			{
				aEditType = deleted;
			}
			else
			{
				aEditType = may_be_updated;
			}
		}
		return aEditType;
	}
}

void CAttrAccessPointDlg::SaveAccessStore()
{
	using namespace access_store_cache;

	// �I������Ă���A�N�Z�X�|�C���g�Q�̂P�ڂƓ���DB��Access_Store�e�[�u�����擾
	IFeaturePtr ipAccessPoint( m_pFeatureDefList->begin()->GetFeature() );
	ITablePtr ipAccessStoreTable = AheOpenSameOwnerTable( (ITablePtr)AheGetFeatureClass( ipAccessPoint ), sindy::schema::access_store::kTableName );
	if (! ipAccessStoreTable)
	{
		AttrMessageBox( AheLoadString( IDS_FAILTOUPDATEACCESSSTORE ), AheLoadString( IDS_CAPTION ), MB_OK, NULL );
		return;
	}

	long aSucceededRowCount = 0;	// DB�ւ̔��f�����������s�̐�
	long aFailedRowCount = 0;		// DB�ւ̔��f�����s�����s�̐�

	m_cArcHelper.StartEditOperation();

	// �L���b�V���̑S�Ă̍s�ɂ���
	BOOST_FOREACH (const CASRows::value_type& rElement, m_ASCache.rows())
	{
		long aAccessStoreID = rElement.first;
		const CASRow& rRow = rElement.second;
		_IRowPtr ipRow = rRow.m_pRow;

		using namespace access_store_row_edit_type;
		ECode aEditType = code( rRow.m_IsAdded, rRow.m_IsDeleted );

		// �u�����v�Ȃ�X�L�b�v
		if (aEditType == none)
			{ continue; }
		// �u�폜�v���ꂽ�s�Ȃ�폜�𔽉f
		else if (aEditType == deleted)
		{
			ATLASSERT( ipRow );
			if (ipRow)
			{
				ipRow->Delete();
				++aSucceededRowCount;
			}
		}
		// �u�ǉ��v�܂��́u�X�V�܂��͕ύX�Ȃ��v�̏ꍇ
		else
		{
			ATLASSERT( aEditType == added || aEditType == may_be_updated );

			// �l�̑Ó����`�F�b�N
			if (! CheckASValuesValidity( rRow ))
			{
				++aFailedRowCount;
				break;
			}

			_IRowPtr ipNewRow;
			// �ǉ��Ȃ�Row��V�K�쐬
			if (aEditType == added)
			{
				ipAccessStoreTable->CreateRow( &ipNewRow );
			}

			long aUpdatedFieldCount = 0;
			// �S�Ẵt�B�[���h�ɂ���
			BOOST_FOREACH (const CASFields::value_type& rElement, rRow.m_Fields)
			{
				tstring aFieldName = rElement.first;
				const CASField& rField = rElement.second;

				// ObjectID�Ȃ�X�L�b�v
				if (aFieldName == _T("OBJECTID"))
					{ continue; }

				// �u�ǉ��v���ꂽ�s�Ȃ�A�S�t�B�[���h�̒l�𔽉f
				if (aEditType == added)
				{
					ipNewRow->PutValue( ipNewRow->Fields->_FindField( aFieldName.c_str() ), rField.m_NewValue );
				}
				// �u�X�V�܂��͕ύX�Ȃ��v�̍s�Ȃ�A�X�V���ꂽ�t�B�[���h�̂ݒl�𔽉f
				else
				{
					if (rField.m_OrgValue != rField.m_NewValue)
					{
						ipRow->PutValue( ipRow->Fields->_FindField( aFieldName.c_str() ), rField.m_NewValue );
						++aUpdatedFieldCount;
					}
				}
			}

			if (aEditType == added)
			{
				
				AheSetModifyData( ipNewRow, sindyUpdateTypeCreate );		// �V�K�쐬�������i�[
				ipNewRow->Store();
				++aSucceededRowCount;
			}
			else
			{
				// �X�V���ꂽ�t�B�[���h���P�ȏ゠���
				if (aUpdatedFieldCount > 0)
				{
					AheSetModifyData( ipRow, sindyUpdateTypeProperty );		// �X�V�������i�[
					ipRow->Store();
					++aSucceededRowCount;
				}
			}
		}
	}

	// �������s���Ă�����ҏW���~
	if (aFailedRowCount > 0)
	{
		m_cArcHelper.AbortEditOperation();
	}
	// �����I�ɉ����ύX����Ă��Ȃ���ΕҏW���~
	else if (aSucceededRowCount == 0)
	{
		AttrMessageBox( AheLoadString( IDS_NOTSAVE ), AheLoadString( IDS_CAPTION ), MB_OK, NULL );
		m_cArcHelper.AbortEditOperation();
	}
	// ����ȊO�Ȃ�ҏW�m��
	else
	{
		// �ǉ����ۑ����Undo����Ė����ɂȂ���Row���X�V���悤�Ƃ���Ƃ����Ŏ��s����B
		// ArcHelper�ɂ��G���[�_�C�A���O���\������邪�AArcMap���I�����Ă��܂��킯�ł͂Ȃ��̂ŁA
		// �����P�󋵐����̃_�C�A���O��\�����A�L���b�V���ƃO���b�h�̏�Ԃ��ŐV�ɂ��ď������I����B
		if (! m_cArcHelper.StopEditOperation( AheLoadString( IDS_NAME ) ))
		{
			AttrMessageBox( AheLoadString( IDS_FAILTOSAVEFORUNDO ),  AheLoadString( IDS_CAPTION ), MB_OK, NULL );
		}
		// �L���b�V���č쐬
		LoadAccessStore();
		// �O���b�h�X�V
		UpdateASGrid();
	}
}

bool CAttrAccessPointDlg::IsValidASValue( const access_store_cache::CASRow& crRow, tstring cFieldName, const CString& cFieldAlias, long cMin, long cMax )
{
	bool aResult = true;
	long aValue = crRow.validField( cFieldName.c_str() ).m_NewValue;
	if (aValue < cMin || cMax < aValue)
	{
		CString aStr;
		aStr.Format( AheLoadString( IDS_INVALIDASVALUE ), cFieldAlias, aValue );
		AttrMessageBox( aStr, AheLoadString( IDS_CAPTION ), MB_OK, NULL );
		aResult = false;
	}
	return aResult;
}

bool CAttrAccessPointDlg::CheckASValuesValidity( const access_store_cache::CASRow& crRow )
{
	bool aResult = true;
	try
	{
		using namespace sindy::schema;
		if (
			! IsValidASValue( crRow, access_store::kMultipleStore, AheLoadString( IDS_CHAINMASTERCODE ), 1, 99999999 ) ||	// �`�F�[���X�}�X�^�[�R�[�h��0�Ȃ�G���[
			! IsValidASValue( crRow, access_store::kStartHour, AheLoadString( IDS_OPENINGTIME ), 0, 23 ) ||		// �u�J�n���ԁv�u�I�����ԁv��0�ȏ�23�ȉ��łȂ���΃G���[
			! IsValidASValue( crRow, access_store::kStartMin, AheLoadString( IDS_OPENINGTIMEMIN ), 0, 59 ) ||		// �u�J�n���v�u�I�����v��0�ȏ�59�ȉ��łȂ���΃G���[
			! IsValidASValue( crRow, access_store::kEndHour, AheLoadString( IDS_CLOSINGTIME ), 0, 23 ) ||
			! IsValidASValue( crRow, access_store::kEndMin, AheLoadString( IDS_CLOSINGTIMEMIN ), 0, 59 )
		)
		{
			aResult = false;
		}
	}
	catch (...)
	{
		ATLASSERT(0);
		aResult = false;
	}
	return aResult;
}

/**
 * @fn LONG CAttrAccessPointDlg::GetTabNum()
 * @brief ���݂̃^�u�ʒu��n���܂�
 *
 * @retval ���݂̃^�u�ʒu
 */
LONG CAttrAccessPointDlg::GetTabNum()
{
	return m_lAccessTabNum;
}

/**
 * @fn void CAttrAccessPointDlg::SetTabNum(LONG lTabNum)
 * @brief �^�u�ʒu���Z�b�g���܂�
 *
 * @param lTabNum	[in]	�^�u�ʒu
 */
void CAttrAccessPointDlg::SetTabNum(LONG lTabNum)
{
	m_lAccessTabNum = lTabNum;
	m_iChangeType = lTabNum;
}

/**
 * @fn void CAttrAccessPointDlg::CreateControlRelation()
 * @brief �q�_�C�A���O�����ۂɍ쐬���܂��B
 *
 * ���ۂ̍쐬��ControlRel�ɔC���܂��B
 */
void CAttrAccessPointDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );
	
	if( m_cControlRel.size() > 0 )
	{
		// �e�E�C���h�E�n���h����ݒ肵�Ȃ���
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );
		// �^�u
		m_cControlRel.SetControl( IDC_TAB_ACCESSPOINT,		TYPE_OTHER, 	0,	2);
		// ���\�[�X
		m_cControlRel.SetControl( IDC_STATIC_SOURCE,	TYPE_OTHER,			0,	2 );
		m_cControlRel.SetControl( IDC_EDIT_SOURCE,		TYPE_EDITSTRING,	0,	2, _T("SOURCE"),		NULL,	ACCESS_POINT_TABLE_NAME );

		// �n�C�E�F�C���[�h�t���O
		m_cControlRel.SetControl( IDC_CHECK_HWYMODEFLAG,	TYPE_CHECKBOX,	0,	0, _T("HWYMODE_F"),		NULL,	ACCESS_POINT_TABLE_NAME);
		// �H����ʎQ�Ɨp
		m_cControlRel.SetControl( IDC_STATIC_ROADCLASS_REF, TYPE_OTHER, 	0,	0 );
		m_cControlRel.SetControl( IDC_COMBO_ROADCLASS_REF,	TYPE_COMBO,		0,	0 );
		// �H�����̎Q�Ɨp
		m_cControlRel.SetControl( IDC_STATIC_ROADNAME_REF,	TYPE_OTHER, 	0,	0 );
		m_cControlRel.SetControl( IDC_COMBO_ROADNAME_REF,	TYPE_COMBO,		0,	0 );
		// �H���R�[�h
		m_cControlRel.SetControl( IDC_STATIC_ROSENCODE, 	TYPE_OTHER, 	0,	0 );
		m_cControlRel.SetControl( IDC_EDIT_ROSENCODE,		TYPE_EDIT,		0,	0, _T("ROAD_CODE"),		NULL,	ACCESS_POINT_TABLE_NAME ,	NULL,	-1, FOLLOW_READONLY );
		// �H�����V�[�P���X�Q�Ɨp 
		m_cControlRel.SetControl( IDC_STATIC_SEQUENCE_REF,	TYPE_OTHER, 	0,	0 );
		m_cControlRel.SetControl( IDC_COMBO_SEQUENCE,		TYPE_COMBO,		0,	0 );
		// �H�����V�[�P���X
		m_cControlRel.SetControl( IDC_STATIC_SEQUENCE,		TYPE_OTHER, 	0,	0 );
		m_cControlRel.SetControl( IDC_EDIT_SEQUENCE,		TYPE_EDIT,		0,	0, _T("ROAD_SEQ"), 		NULL,	ACCESS_POINT_TABLE_NAME,	NULL,	-1, FOLLOW_READONLY );
		// �����R�[�h
		m_cControlRel.SetControl( IDC_STATIC_DIRECTION, 	TYPE_OTHER, 	0,	0 );
		m_cControlRel.SetControl( IDC_COMBO_DIRECTION,		TYPE_COMBO, 	0,	0, _T("DIRECTION_C"),	NULL,	ACCESS_POINT_TABLE_NAME );
		// ����E�����R�[�h
		m_cControlRel.SetControl( IDC_STATIC_JUNCTIONCODE,	TYPE_OTHER, 	0,	0 );
		m_cControlRel.SetControl( IDC_COMBO_JUNCTIONCODE,	TYPE_COMBO, 	0,	0, _T("INOUT_C"),		NULL,	ACCESS_POINT_TABLE_NAME );
		// ���ʊ�������
		m_cControlRel.SetControl( IDC_STATIC_HOUMENKANJI,	TYPE_OTHER, 	0,	0 );
		m_cControlRel.SetControl( IDC_EDIT_HOUMENKANJI, 	TYPE_EDITSTRING,0,	0, _T("DIRECTION_KANJI"),NULL,	ACCESS_POINT_TABLE_NAME );
		// ���ʃJ�i����
		m_cControlRel.SetControl( IDC_STATIC_HOUMENKANA,	TYPE_OTHER, 	0,	0 );
		m_cControlRel.SetControl( IDC_EDIT_HOUMENKANA,		TYPE_EDITSTRING,0,	0, _T("DIRECTION_YOMI"),NULL,	ACCESS_POINT_TABLE_NAME );
		
		// �{�ݏ��p�̃O���[�v�{�b�N�X
		m_cControlRel.SetControl( IDC_STATIC_SHISETUINFO,	TYPE_OTHER, 	1,	1 );
		// ���X�g����
		m_cControlRel.SetControl( IDC_CHECK_RESTAURANT, 	TYPE_CHECKBOX,	1,	1, _T("RESTAURANT_F"), 	NULL,	ACCESS_POINT_TABLE_NAME );
		// �x�e��
		m_cControlRel.SetControl( IDC_CHECK_RESTING,		TYPE_CHECKBOX,	1,	1, _T("RESTING_F"),		NULL,	ACCESS_POINT_TABLE_NAME );
		// HWY���^�[�~�i���i���u�C���t�H���[�V�����v�j
		m_cControlRel.SetControl( IDC_CHECK_INFOMATION, 	TYPE_CHECKBOX,	1,	1, _T("INFOMATION_F"), 	NULL,	ACCESS_POINT_TABLE_NAME );
		// ���C
		m_cControlRel.SetControl( IDC_CHECK_SPA,			TYPE_CHECKBOX,	1,	1, _T("SPA_F"),			NULL,	ACCESS_POINT_TABLE_NAME );
		// �L���b�V���T�[�r�X
		m_cControlRel.SetControl( IDC_CHECK_CASHSERVICE,	TYPE_CHECKBOX,	1,	1, _T("CASHSERVICE_F"),	NULL,	ACCESS_POINT_TABLE_NAME );
		// �g�C��
		m_cControlRel.SetControl( IDC_CHECK_TOILET, 		TYPE_CHECKBOX,	1,	1, _T("TOILET_F"), 		NULL,	ACCESS_POINT_TABLE_NAME );
		// �K�\�����X�^���h
		m_cControlRel.SetControl( IDC_CHECK_GS, 			TYPE_CHECKBOX,	1,	1, _T("GS_F"), 		NULL,	ACCESS_POINT_TABLE_NAME );
		// �y�H
		m_cControlRel.SetControl( IDC_CHECK_CAFETERIA,		TYPE_CHECKBOX,	1,	1, _T("CAFETERIA_F"),	NULL,	ACCESS_POINT_TABLE_NAME );
		// �����x�e�{��
		m_cControlRel.SetControl( IDC_CHECK_NAP,			TYPE_CHECKBOX,	1,	1, _T("NAP_F"),			NULL,	ACCESS_POINT_TABLE_NAME );
		// �V�����[���[��
		m_cControlRel.SetControl( IDC_CHECK_SHOWER, 		TYPE_CHECKBOX,	1,	1, _T("SHOWER_F"), 		NULL,	ACCESS_POINT_TABLE_NAME );
		// FAX
		m_cControlRel.SetControl( IDC_CHECK_FAX,			TYPE_CHECKBOX,	1,	1, _T("FAX_F"),			NULL,	ACCESS_POINT_TABLE_NAME );
		// �n�C�E�F�C�I�A�V�X
		m_cControlRel.SetControl( IDC_CHECK_HWYOASIS,		TYPE_CHECKBOX,	1,	1, _T("HWYOASIS_F"),	NULL,	ACCESS_POINT_TABLE_NAME );
		// �g��җp�g�C��
		m_cControlRel.SetControl( IDC_CHECK_HANDYCAP,		TYPE_CHECKBOX,	1,	1, _T("HANDICAP_F"),	NULL,	ACCESS_POINT_TABLE_NAME );
		// �`�F�[���X
		m_cControlRel.SetControl( IDC_CHECK_MULTIPLESTORE, 	TYPE_CHECKBOX,	1,	1, _T("MULTIPLESTORE_F"), 		NULL,	ACCESS_POINT_TABLE_NAME );
		// ���X
		m_cControlRel.SetControl( IDC_CHECK_SOUBENIR,		TYPE_CHECKBOX,	1,	1, _T("SOUVENIR_F"),	NULL,	ACCESS_POINT_TABLE_NAME );
		// �C���t�H���[�V�����i���u�ē����i�ΐl�j�v�j
		m_cControlRel.SetControl( IDC_CHECK_INFODESK,		TYPE_CHECKBOX,	1,	1, _T("INFODESK_F"),	NULL,	ACCESS_POINT_TABLE_NAME );
		// �R�C�������h���[
		m_cControlRel.SetControl( IDC_CHECK_LAUNDRY,		TYPE_CHECKBOX,	1,	1, _T("LAUNDRY_F"),		NULL,	ACCESS_POINT_TABLE_NAME );
		// �X�փ|�X�g
		m_cControlRel.SetControl( IDC_CHECK_MAILBOX,		TYPE_CHECKBOX,	1,	1, _T("MAILBOX_F"),		NULL,	ACCESS_POINT_TABLE_NAME );
		// �h�b�O����
		m_cControlRel.SetControl( IDC_CHECK_DOGRUN,			TYPE_CHECKBOX,	1,	1, _T("DOGRUN_F"),		NULL,	ACCESS_POINT_TABLE_NAME );

		// Access_Store�^�u�̃R���g���[���Q�i[Bug5556]�Œǉ��j
		// Access_Store���O���b�h
		m_cControlRel.SetControl( IDC_GRID_ACCESS_STORE,	TYPE_OTHER, 	2,	2 );
		// �`�F�[���X�}�X�^�[�R�[�h
		m_cControlRel.SetControl( IDC_STATIC_MASTERCODE,	TYPE_OTHER, 	2,	2 );
		m_cControlRel.SetControl( IDC_EDIT_MASTERCODE,		TYPE_EDIT,		2,	2 );
		// �c�ƊJ�n����
		m_cControlRel.SetControl( IDC_STATIC_START_TIME,	TYPE_OTHER, 	2,	2 );
		m_cControlRel.SetControl( IDC_EDIT_START_HOUR,		TYPE_EDIT,		2,	2 );
		m_cControlRel.SetControl( IDC_STATIC_COLON1,		TYPE_OTHER, 	2,	2 );
		m_cControlRel.SetControl( IDC_EDIT_START_MIN,		TYPE_EDIT,		2,	2 );
		// �c�ƏI������
		m_cControlRel.SetControl( IDC_STATIC_END_TIME,		TYPE_OTHER, 	2,	2 );
		m_cControlRel.SetControl( IDC_EDIT_END_HOUR,		TYPE_EDIT,		2,	2 );
		m_cControlRel.SetControl( IDC_STATIC_COLON2,		TYPE_OTHER, 	2,	2 );
		m_cControlRel.SetControl( IDC_EDIT_END_MIN,			TYPE_EDIT,		2,	2 );
		// �X�܎��
		m_cControlRel.SetControl( IDC_GROUP_STORETYPE,		TYPE_OTHER, 	2,	2 );
		m_cControlRel.SetControl( IDC_RADIO_GS,				TYPE_BUTTON, 	2,	2 );
		m_cControlRel.SetControl( IDC_RADIO_CHAIN,			TYPE_BUTTON, 	2,	2 );
		// �j���E���Ԍn�`�F�b�N�{�b�N�X�Q
		m_cControlRel.SetControl( IDC_STATIC_MONDAY,		TYPE_OTHER, 	2,	2 );
		m_cControlRel.SetControl( IDC_CHECK_MONDAY,			TYPE_CHECKBOX, 	2,	2 );
		m_cControlRel.SetControl( IDC_STATIC_TUESDAY,		TYPE_OTHER, 	2,	2 );
		m_cControlRel.SetControl( IDC_CHECK_TUESDAY,		TYPE_CHECKBOX, 	2,	2 );
		m_cControlRel.SetControl( IDC_STATIC_WEDNESDAY,		TYPE_OTHER, 	2,	2 );
		m_cControlRel.SetControl( IDC_CHECK_WEDNESDAY,		TYPE_CHECKBOX, 	2,	2 );
		m_cControlRel.SetControl( IDC_STATIC_THURSDAY,		TYPE_OTHER, 	2,	2 );
		m_cControlRel.SetControl( IDC_CHECK_THURSDAY,		TYPE_CHECKBOX, 	2,	2 );
		m_cControlRel.SetControl( IDC_STATIC_FRIDAY,		TYPE_OTHER, 	2,	2 );
		m_cControlRel.SetControl( IDC_CHECK_FRIDAY,			TYPE_CHECKBOX, 	2,	2 );
		m_cControlRel.SetControl( IDC_STATIC_SATURDAY,		TYPE_OTHER, 	2,	2 );
		m_cControlRel.SetControl( IDC_CHECK_SATURDAY,		TYPE_CHECKBOX, 	2,	2 );
		m_cControlRel.SetControl( IDC_STATIC_SUNDAY,		TYPE_OTHER, 	2,	2 );
		m_cControlRel.SetControl( IDC_CHECK_SUNDAY,			TYPE_CHECKBOX, 	2,	2 );
		m_cControlRel.SetControl( IDC_STATIC_HOLIDAY,		TYPE_OTHER, 	2,	2 );
		m_cControlRel.SetControl( IDC_CHECK_HOLIDAY,		TYPE_CHECKBOX, 	2,	2 );
		m_cControlRel.SetControl( IDC_STATIC_GOLDENWEEK,	TYPE_OTHER, 	2,	2 );
		m_cControlRel.SetControl( IDC_CHECK_GOLDENWEEK,		TYPE_CHECKBOX, 	2,	2 );
		m_cControlRel.SetControl( IDC_STATIC_BON,			TYPE_OTHER, 	2,	2 );
		m_cControlRel.SetControl( IDC_CHECK_BON,			TYPE_CHECKBOX, 	2,	2 );
		m_cControlRel.SetControl( IDC_STATIC_YEAREND,		TYPE_OTHER, 	2,	2 );
		m_cControlRel.SetControl( IDC_CHECK_YEAREND,		TYPE_CHECKBOX, 	2,	2 );
		m_cControlRel.SetControl( IDC_STATIC_NEWYEAR,		TYPE_OTHER, 	2,	2 );
		m_cControlRel.SetControl( IDC_CHECK_NEWYEAR,		TYPE_CHECKBOX, 	2,	2 );
		// �S�I���E�S�����{�^��
		m_cControlRel.SetControl( IDC_BUTTON_CHECK_ALL,		TYPE_BUTTON, 	2,	2 );
		m_cControlRel.SetControl( IDC_BUTTON_UNCHECK_ALL,	TYPE_BUTTON, 	2,	2 );
		// �ҏW�{�^��
		m_cControlRel.SetControl( IDC_BUTTON_ADD_ACCESS_STORE,		TYPE_BUTTON, 	2,	2 );
		m_cControlRel.SetControl( IDC_BUTTON_UPDATE_ACCESS_STORE,	TYPE_BUTTON, 	2,	2 );
		m_cControlRel.SetControl( IDC_BUTTON_DELETE_ACCESS_STORE,	TYPE_BUTTON, 	2,	2 );
		m_cControlRel.SetControl( IDC_BUTTON_SAVE_ACCESS_STORE,		TYPE_BUTTON, 	2,	2 );
	}
}

/**
* @fn void CAttrAccessPointDlg::ChangeWindowMgrMap( INT iIndex )
* @brief �E�B���h�E�}�l�[�W���̃}�b�v�̐؂�ւ����s��
*
* @param	iIndex	[in]	�}�b�v�̃C���f�b�N�X�ԍ�
**/
void CAttrAccessPointDlg::ChangeWindowMgrMap( INT iIndex )
{
	switch( iIndex ) {
	case 0:
			m_winMgr.ChangeMgrMap(ACCESS_POINT_MAP);
			break;
	case 1:
			m_winMgr.ChangeMgrMap(ACCESS_POINT_MAP2);
			break;
	case 2:												// [Bug5556]�Œǉ�
			m_winMgr.ChangeMgrMap(ACCESS_POINT_MAP3);
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
* @fn void CAttrAccessPointDlg::ChangeRoadSeqCombobox()
* @brief �H���R�[�h����H�����V�[�P���X�̒l���擾���A�Q�Ɨp�H�����V�[�P���X�̃R���{�{�b�N�X�ɐݒ肷��
**/
void CAttrAccessPointDlg::ChangeRoadSeqCombobox()
{
    // �H���R�[�h�̃_�C�A���O���擾���A�H���R�[�h�擾
    CWindow edit( GetDlgItem( IDC_EDIT_ROSENCODE ) );
    LPTSTR lpszRoadCode = (LPTSTR)_alloca((edit.GetWindowTextLength() + 1) * sizeof(TCHAR));        //!< ���������������m��
    edit.GetWindowText( lpszRoadCode, edit.GetWindowTextLength() + 1 );
    m_cRoadCode = lpszRoadCode;

    IMapPtr				ipMap;
    ITablePtr			ipTable;
    _ICursorPtr			ipCursor;
    IQueryFilterPtr		ipQueryFilter( CLSID_QueryFilter );
    CString		strWhereClause, strSubFields;

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

	// �}�b�v���擾
    ipMap = m_cArcHelper.GetMap();
    
    // Facil_Info_Point�e�[�u���擾
	CString strOwnerName = AheGetFeatureClassOwnerName( ipCurrentFeature );
	if( strOwnerName.IsEmpty() )
		strOwnerName = _T("Facil_Info_Point");
	else
		strOwnerName += _T(".Facil_Info_Point");

	ipTable = AheOpenTableByTableName( ipWorkspace, strOwnerName );

    // �擾�����H���ԍ����e�[�u�����猟��
    strWhereClause.Format(_T("ROAD_CODE = %s"), m_cRoadCode );
    strSubFields.Format(_T("NAME_KANJI,ROAD_SEQ"));
    ipQueryFilter->put_WhereClause( CComBSTR(strWhereClause) );
    ipQueryFilter->put_SubFields( CComBSTR(_T("NAME_KANJI,ROAD_SEQ")) );
    ipTable->Search( ipQueryFilter, VARIANT_FALSE, &ipCursor );

    // �H�����V�[�P���X�擾
    CComVariant			vaRoadSequence;
    CFieldDef*			pFieldDef;

	// �I�𒆂̃t�B�[�`���̘H�����V�[�P���X���擾
    pFieldDef = GetCurrentFeatureDef()->GetFieldDef( ACCESS_POINT_TABLE_NAME, _T("ROAD_SEQ") );
    vaRoadSequence = pFieldDef->m_vaValue;
    m_cRoadSeq.Format(_T("%d"), vaRoadSequence.lVal);

    // �R���{�{�b�N�X�ɓ���邽�߂̒l���쐬����
    _IRowPtr ipRow;
    std::map<LONG, CString> mapDomain;
    if( ipCursor != NULL)
    {
		// �������ďo�Ă����H�����V�[�P���X���ʑS�Ă𒲂ׂ�
		while( ipCursor->NextRow( &ipRow ) == S_OK )
		{
			if( ipRow == NULL ) 
					break;
			CComVariant vaKanjiName, vaRoadSeq, vaKanaName, vaClass;

			LONG lIndex = -1;
			IFieldsPtr ipFields;
			ipRow->get_Fields( &ipFields );
			// �{�ݖ��̂��擾
			ipFields->FindField( CComBSTR(_T("NAME_KANJI")), &lIndex );
			ipRow->get_Value( lIndex, &vaKanjiName );
			// �H�����V�[�P���X���擾
			ipFields->FindField( CComBSTR(_T("ROAD_SEQ")), &lIndex );
			ipRow->get_Value( lIndex, &vaRoadSeq );

			CString        strVal;
			strVal.Format(_T("%d"), vaRoadSeq.lVal );        

			// �{�ݖ��̂ƘH�����V�[�P���X��map�ɃZ�b�g����
			USES_CONVERSION;
			CString strText;
			strText.Format(_T("%s(%s)"), OLE2T(vaKanjiName.bstrVal), strVal );

			mapDomain[vaRoadSeq.lVal] = strText;
		}

		// �R���{�{�b�N�X�ɒl������
		CComboBox cCombo( GetDlgItem( IDC_COMBO_SEQUENCE ) );
		cCombo.ResetContent();
		for( std::map<LONG, CString>::iterator it = mapDomain.begin(); it != mapDomain.end(); it++ )
		{
			INT iCurPos = cCombo.AddString( it->second );
			cCombo.SetItemData( iCurPos, it->first );
			// ���ݑI�𒆂̃t�B�[�`���̘H�����V�[�P���X�Ȃ�΁A������o��
			if( vaRoadSequence.lVal == it->first )
				cCombo.SetCurSel( iCurPos );
		}
    }

	// �H�����V�[�P���X�G�f�B�^�ɒl���o�͂���
	SetRoadSequence();
}

/**
* @fn void CAttrAccessPointDlg::SetRoadSequence()
* @brief �H�����V�[�P���X�G�f�B�^�ɒl���o�͂���
**/
void CAttrAccessPointDlg::SetRoadSequence()
{

	// �H�����V�[�P���X�G�f�B�^���擾
	CEdit        cRoadSeq(GetDlgItem( IDC_EDIT_SEQUENCE ));
	//�@�H�����V�[�P���X�����ۂɐݒ�
	cRoadSeq.SetSel(0,-1);
	cRoadSeq.ReplaceSel(m_cRoadSeq);
	// �l���ύX���ꂢ�邩�m�F
	CFieldDef* pFieldDef;
	pFieldDef = GetCurrentFeatureDef()->GetFieldDef( ACCESS_POINT_TABLE_NAME, _T("ROAD_SEQ") );
	pFieldDef->m_vaValue = _ttol(m_cRoadSeq);
	LONG        lDummy = _ttol(m_cRoadSeq);
	if(  lDummy != m_lOrgRoadSequence)
	{
		pFieldDef->m_bChanged = TRUE;
		SetButton( Changed() );
	}
}

/** 
 * @fn bool CAttrAccessPointDlg::SetLineClassList(LONG nLineClass)
 * @brief        �H����ʃ��X�g�̃R���{�{�b�N�X��ݒ肷��
 *
 * @param	nLineClass	[in]	�H����ʃR�[�h
 *
 * @return	�R���{�{�b�N�X�̐ݒ�ł�����TRUE�A���s������FALSE
 */
bool CAttrAccessPointDlg::SetLineClassList(LONG nLineClass)
{
    // �H����ʃ��X�g�̃R���{�{�b�N�X
    CComboBox cbLineClass = CComboBox(GetDlgItem(IDC_COMBO_ROADCLASS_REF));
    // �A�C�e���̃N���A
    cbLineClass.ResetContent();

	// �H���R�[�h�e�[�u�����擾���Ă��Ȃ���΁A�擾����
	ITablePtr ipRoadCodeListTable = GetRoadCodeTable();
	if(!ipRoadCodeListTable)
		return false;

	// �^����ꂽ�H���R�[�h�̘H���̘H����ʃR�[�h���擾
	_ICursorPtr		ipCursor;
	IQueryFilterPtr	ipQueryFilter( CLSID_QueryFilter );
	CString strWhereClause, strSubFields;

	strWhereClause.Format(_T("ROAD_CODE = %ld"), nLineClass );
	strSubFields.Format(_T("LINECLASS_C"));
	ipQueryFilter->put_WhereClause( CComBSTR(strWhereClause) );
	ipQueryFilter->put_SubFields( CComBSTR(strSubFields) );
	ipRoadCodeListTable->Search( ipQueryFilter, VARIANT_FALSE, &ipCursor );

	_IRowPtr ipRow;

	long aLineClass = -1;
	if (ipCursor )
	{
		while (ipCursor->NextRow(&ipRow) == S_OK)
		{
			// ipCursor�Ɋi�[���ꂽRoad_Code_List�̃��R�[�h�̂����A�ŏ��̂P����LineClass_c���擾
			aLineClass = ipRow->GetValue( ipRow->Fields->_FindField(sindy::schema::road_code_list::kLineClass) );
			break;
		}
	}

	// �H����ʃR�[�h�̃R�[�h�l�h���C�����擾
	ATLASSERT(ipRoadCodeListTable);
	IFieldsPtr ipFields(ipRoadCodeListTable->Fields);
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
			AttrMessageBox( AheLoadString( IDS_FAILTOSETROUTECLASS ), AheLoadString( IDS_CAPTION ), MB_OK, NULL);
			return false;
		}
	}

    // �J�[�\���̐ݒ�
    cbLineClass.SetCurSel(nCurIdx);
    // ���݂̘H����ʂ̎擾
    m_lLineClass = cbLineClass.GetItemData(nCurIdx);

    CFieldDef* pFieldDef;
    CComVariant                vaRoadCode;        
    pFieldDef = GetCurrentFeatureDef()->GetFieldDef( ACCESS_POINT_TABLE_NAME, _T("ROAD_CODE") );
    vaRoadCode = pFieldDef->m_vaValue;

    RedrawRoadNameList(vaRoadCode.lVal);
    return true;
}

/**
 * @fn bool CAttrAccessPointDlg::GetLineClass(int* pClass)
 * @brief �ݒ肳��Ă���H����ʂ��擾����
 *
 * @param pClass	[out]	�H���R�[�h�̒l
 *
 * @return	�H����ʂ��擾�ł�����TRUE�A�擾�ł��Ȃ�������FALSE
 */
bool CAttrAccessPointDlg::GetLineClass(int* pClass)
{
	if(pClass == NULL)
		return false;
	// �R���{�{�b�N�X�̎擾
	CComboBox cbLineClass = CComboBox(GetDlgItem(IDC_COMBO_ROADCLASS_REF));
	// �C���f�b�N�X�̎擾
	int nIndex = cbLineClass.GetCurSel();
	if(nIndex == CB_ERR)
		return false;
	// �l�̎擾
	*pClass = cbLineClass.GetItemData(nIndex);

	return true;
}

/**
 * @fn bool CAttrAccessPointDlg::SetLineClass(int nClass)
 * @brief �H����ʂ̒l����R���{�{�b�N�X��ݒ�
 *
 * @param nClass	[in]	�H���R�[�h�̒l
 *
 * @return	�H����ʂ��擾�ł�����TRUE�A�擾�ł��Ȃ�������FALSE
 */
bool CAttrAccessPointDlg::SetLineClass(int nClass)
{
	// �R���{�{�b�N�X�̎擾
	CComboBox cbLineClass = CComboBox(GetDlgItem(IDC_COMBO_ROADCLASS_REF));
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
 * @fn bool CAttrAccessPointDlg::RedrawRoadNameList(LONG nRoadCode)
 * @brief �H�����̃R���{�{�b�N�X�̍X�V
 *
 * @param nRoadCode	[in]	�H���R�[�h�̒l
 *
 * @return	�H�����̂��擾�ł�����TRUE�A�擾�ł��Ȃ�������FALSE
 */
bool CAttrAccessPointDlg::RedrawRoadNameList(LONG nRoadCode)
{
	// �H�����̃R���{�{�b�N�X
	CComboBox cbRoadName = CComboBox(GetDlgItem(IDC_COMBO_ROADNAME_REF));
	// �R���{�{�b�N�X�̏��̃N���A
	cbRoadName.ResetContent();
	// �J�i���̂Ń}�b�v�̍쐬
	std::map<CString, int> cYomiMap;

	// �H���R�[�h���̍ēǂݍ���
	if(! GetRoadCodeSet(&m_cRoadCodeSet))
	{
		AttrMessageBox( AheLoadString( IDS_FAILTOGETROUTECODE ), AheLoadString( IDS_CAPTION ), MB_OK, NULL );
		return 0;
	}
	for(CHetRoadCodeSet::const_iterator aIter = m_cRoadCodeSet.begin(); aIter != m_cRoadCodeSet.end(); ++aIter)
	{
		if(m_lLineClass == 0 || aIter->second.m_nLineClass == m_lLineClass)
			cYomiMap.insert(std::map<CString, int>::value_type(aIter->second.m_strNameYomi, aIter->second.m_nRoadCode));
	}

	// ���X�g�{�b�N�X�ɃA�C�e���̒ǉ�
	int nCurIdx = 0;
	for(std::map<CString, int>::const_iterator aIter2 = cYomiMap.begin(); aIter2 != cYomiMap.end(); ++aIter2)
	{
		const CHetRoadCode* pRoadCode = m_cRoadCodeSet.FindRoadCode(aIter2->second);
		if(pRoadCode)
		{
			int nIndex = cbRoadName.AddString(pRoadCode->m_strNameKanji);
			if(nIndex != CB_ERR && nIndex != CB_ERRSPACE)
			{
				cbRoadName.SetItemData(nIndex, pRoadCode->m_nRoadCode);
				if(pRoadCode->m_nRoadCode == nRoadCode)
					nCurIdx = nIndex;
			}
			else
			{
				AttrMessageBox( AheLoadString( IDS_FAILTOSETROUTENAME ), AheLoadString( IDS_CAPTION ), MB_OK, NULL );
				return false;
			}
		}
	}
    // ���݂̃J�[�\���̐ݒ�
    cbRoadName.SetCurSel(nCurIdx);
    // ���݂̘H���R�[�h�̎擾
    m_lRoadCode = cbRoadName.GetItemData(nCurIdx);

    return true;
}

/**
 * @fn bool CAttrAccessPointDlg::GetRoadCode(int* pRoadCode)
 * @brief �H���R�[�h�G�f�B�b�g�{�b�N�X����H���R�[�h�̎擾
 *
 * @param nRoadCode	[out]	�H���R�[�h�̒l
 *
 * @return	�H���R�[�h���擾�ł�����TRUE�A�擾�ł��Ȃ�������FALSE
 */
bool CAttrAccessPointDlg::GetRoadCode(int* pRoadCode)
{
	// �G�f�B�b�g�{�b�N�X�̎擾
	CEdit edRoadCode = CEditT<CWindow>(GetDlgItem(IDC_EDIT_ROSENCODE));
	// �l�̎擾
	TCHAR lpszRoadCode[32];
	if(edRoadCode.GetLine(0, lpszRoadCode, 32) == 0)
		return false;
	// ���l�ɕϊ�
	if(! CHwyHelper::StrToRoadCode(pRoadCode, lpszRoadCode))
		return false;

	return true;
}


/**
 * @fn bool CAttrHwyNodeDlg::SetRoadCode(int nRoadCode)
 * @brief �H���R�[�h�̒l���w�肳�ꂽ�R�[�h�ɒu��������
 *
 * @param nRoadCode	[in]	�H���R�[�h�̒l
 *
 * @return	�H���R�[�h���ݒ�ł�����TRUE�A�ݒ�ł��Ȃ�������FALSE
 */
bool CAttrAccessPointDlg::SetRoadCode(int nRoadCode)
{
	// ������ϊ�
	TCHAR lpszRoadCode[8];
	if(! CHwyHelper::RoadCodeToStr(nRoadCode, lpszRoadCode))
		return true;
	// �G�f�B�b�g�{�b�N�X�̎擾
	CEdit edRoadCode = CEditT<CWindow>(GetDlgItem(IDC_EDIT_ROSENCODE));
	// �l�̐ݒ�
	edRoadCode.SetSel(0, -1);
	edRoadCode.ReplaceSel(lpszRoadCode);

	CFieldDef* pFieldDef;
	pFieldDef = GetCurrentFeatureDef()->GetFieldDef( ACCESS_POINT_TABLE_NAME, _T("ROAD_CODE") );
	pFieldDef->m_vaValue = nRoadCode;
	if( nRoadCode != m_lOrgRoadCode)
	{
		pFieldDef->m_bChanged = TRUE;        
		SetButton( Changed() );
	}
	return true;
}

/**
 * @fn BOOL CAttrAccessPointDlg::ErrorCheck()
 * @brief �G���[���Ȃ����ǂ������ׂ�
 *
 * �_�C�A���O�����ۂɁAAttributeDlgObj����Ă΂��
 * ���ʖ��̂ƕ��ʃJ�i���̂������Ƒ΂ɂȂ��ė��������Ă��邩�A�܂��͓����Ă��Ȃ����������߂�
 *
 * @return	�G���[���Ȃ�������TRUE�A�G���[�������FALSE
 */
BOOL CAttrAccessPointDlg::ErrorCheck()
{
	for( std::list<CFeatureDef>::iterator it = m_pFeatureDefList->begin(); it != m_pFeatureDefList->end(); it++ )
	{
		CString strTableName = it->GetTableName();

		if( strTableName == ACCESS_POINT_TABLE_NAME )
		{
			// ���ݕҏW���̒l���K��
			CFieldDef* pFieldDef;
			CComVariant vaName_Kanji, vaName_Yomi;
			pFieldDef = it->GetFieldDef( ACCESS_POINT_TABLE_NAME, _T("DIRECTION_KANJI") );
			vaName_Kanji = pFieldDef->m_vaValue;
			pFieldDef = it->GetFieldDef( ACCESS_POINT_TABLE_NAME, _T("DIRECTION_YOMI") );
			vaName_Yomi = pFieldDef->m_vaValue;

			CString strMessage;
			LONG lOID;
			it->GetFeature()->get_OID( &lOID );

			if( vaName_Kanji.vt != VT_NULL && vaName_Yomi.vt == VT_NULL )
			{
				strMessage.Format(AheLoadString( IDS_INVALIDDIRSIGNKANA ),lOID);
				AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK, NULL );
				return FALSE;
			}
			else if( vaName_Kanji.vt == VT_NULL && vaName_Yomi.vt != VT_NULL )
			{
				strMessage.Format(AheLoadString( IDS_INVALIDDIRSIGNKANJI ),lOID);
				AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK, NULL );
				return FALSE;
			}
		}
	}

	return TRUE;
}

/**
* @brief	�H���R�[�h�e�[�u���擾
*/
ITablePtr CAttrAccessPointDlg::GetRoadCodeTable()
{
	ITablePtr ipRoadCodeListTable;
	IWorkspacePtr ipWorkspace;
	ITablePtr ipCurrentTable;
	IFeaturePtr ipCurrentFeature;
	ipCurrentFeature = GetCurrentFeatureDef()->GetFeature();
	if( ipCurrentFeature == NULL )
		return NULL;
	ipCurrentFeature->get_Table( &ipCurrentTable );
	IDatasetPtr ipDataset( ipCurrentTable );
	if( ipDataset == NULL )
		return NULL;
	ipDataset->get_Workspace( &ipWorkspace );
	if( ipWorkspace == NULL )
		return NULL;
	
	// REFERENCE.ROAD_CODE_LIST�e�[�u���擾
	CString strOwnerName = AheGetFeatureClassOwnerName( ipCurrentFeature );
	if( strOwnerName.IsEmpty() == TRUE )
		strOwnerName = _T("ROAD_CODE_LIST");
	else
		strOwnerName += _T(".ROAD_CODE_LIST");

	return AheOpenTableByTableName( ipWorkspace, strOwnerName );
}

/**
* @brief	�H���R�[�h�擾
*/
bool CAttrAccessPointDlg::GetRoadCodeSet(CHetRoadCodeSet* pCodeSet)
{
	HRESULT hr;
	// �e�[�u���̎擾
	ITablePtr ipRoadCodeListTable = GetRoadCodeTable();
	if(!ipRoadCodeListTable)
		return false;
	// �s�擾�̃p�����[�^�̐ݒ�
	IQueryFilterPtr ipQFilter(CLSID_QueryFilter);
	// �s�̎擾
	_ICursorPtr ipCursor;
	hr = ipRoadCodeListTable->Search(ipQFilter, VARIANT_FALSE, &ipCursor);
	if(FAILED(ipRoadCodeListTable->Search(ipQFilter, VARIANT_FALSE, &ipCursor))){
		::MessageBox(m_hWnd, AheLoadString( IDS_FAILTOGETROW ), AheLoadString( IDS_CAPTION ), MB_OK);
		return false;
	}
	// �e�s������̎擾
	_IRowPtr ipRow;
	while(ipCursor->NextRow(&ipRow) == S_OK){
		CHetRoadCode cRoadCode(ipRow);
		pCodeSet->Append(cRoadCode);
	}
	return true;
}

/**
 * @brief �H���R�[�h�̒l�ɏ]���ăn�C�E�F�C���[�h�t���O��ύX
 * @note	[Bug7342]�Ή����ɒǉ�
 * @param[in]	cRoadCode	�H���R�[�h
 */
void CAttrAccessPointDlg::ChangeHwyModeCheckBox(long cRoadCode)
{
	// �H���R�[�h��200000�ȏ�Ȃ�n�C�E�F�C���[�h�t���O��ON�A200000�����Ȃ�OFF�ɂ���
	CButton(GetDlgItem(IDC_CHECK_HWYMODEFLAG)).SetCheck(cRoadCode >= 200000 ? BST_CHECKED : BST_UNCHECKED);
	m_cControlRel.ControlChanged( IDC_CHECK_HWYMODEFLAG );
}

void __stdcall CAttrAccessPointDlg::SelChangeGridAccessStore()
{
	// �J�����g�s�������\��
	SetASGridColor();

	// �O���b�h�̒l�𑼂̃R���g���[���ɔ��f
	UpdateASControls();
}

std::vector<long> CAttrAccessPointDlg::GetASEditableControlIDs()
{
	return boost::assign::list_of
		(IDC_EDIT_MASTERCODE)
		(IDC_EDIT_START_HOUR)
		(IDC_EDIT_START_MIN)
		(IDC_EDIT_END_HOUR)
		(IDC_EDIT_END_MIN)
		(IDC_RADIO_GS)
		(IDC_RADIO_CHAIN)
		(IDC_CHECK_MONDAY)
		(IDC_CHECK_TUESDAY)
		(IDC_CHECK_WEDNESDAY)
		(IDC_CHECK_THURSDAY)
		(IDC_CHECK_FRIDAY)
		(IDC_CHECK_SATURDAY)
		(IDC_CHECK_SUNDAY)
		(IDC_CHECK_HOLIDAY)
		(IDC_CHECK_GOLDENWEEK)
		(IDC_CHECK_BON)
		(IDC_CHECK_YEAREND)
		(IDC_CHECK_NEWYEAR)
		(IDC_BUTTON_CHECK_ALL)
		(IDC_BUTTON_UNCHECK_ALL)
		(IDC_BUTTON_ADD_ACCESS_STORE)
		(IDC_BUTTON_UPDATE_ACCESS_STORE)
		(IDC_BUTTON_DELETE_ACCESS_STORE)
		(IDC_BUTTON_SAVE_ACCESS_STORE)
	;
}

std::vector<long> CAttrAccessPointDlg::GetASCheckBoxIDs()
{
	return boost::assign::list_of
		(IDC_CHECK_MONDAY)
		(IDC_CHECK_TUESDAY)
		(IDC_CHECK_WEDNESDAY)
		(IDC_CHECK_THURSDAY)
		(IDC_CHECK_FRIDAY)
		(IDC_CHECK_SATURDAY)
		(IDC_CHECK_SUNDAY)
		(IDC_CHECK_HOLIDAY)
		(IDC_CHECK_GOLDENWEEK)
		(IDC_CHECK_BON)
		(IDC_CHECK_YEAREND)
		(IDC_CHECK_NEWYEAR)
	;
}


