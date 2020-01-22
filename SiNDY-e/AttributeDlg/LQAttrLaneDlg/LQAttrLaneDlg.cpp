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

// LQAttrLaneDlg.cpp : CLQAttrLaneDlg �̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LQAttrLaneDlg.h"
#include <boost/assign.hpp>		// [Bug9400]�Œǉ�

#include "../../../LIB/WinLib/WKSHelper.h"
/// �E�B���h�E�}�l�[�W���z�u�}�b�v�i���[�����p�j
BEGIN_WINDOW_MAP(LANE_MAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(0,0))
		RCSPACE(3)
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,10))
			RCTOFIT(IDC_STATIC_INLINKID)
			RCTOFIT(IDC_COMBO_INLINKID)
			RCREST(-1)
			RCSPACE(4)
			RCTOFIT(IDC_STATIC_INNODEID)
			RCTOFIT(IDC_COMBO_INNODEID)
		ENDGROUP()
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
			RCREST(IDC_STATIC_SEP)
		ENDGROUP()
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,4))
			BEGINROWS(WRCT_REST, 0, RCMARGINS(0,0))
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(0,2))	// ���[�������R�[�h
					RCTOFIT(IDC_STATIC_LANECLASS)
					RCSPACE(6)
					// �������̓{�^����i[Bug9403]�Œǉ��j
					RCTOFIT(IDC_BUTTON_DIR_NULL)
					RCTOFIT(IDC_BUTTON_DIR_225)
					RCTOFIT(IDC_BUTTON_DIR_270)
					RCTOFIT(IDC_BUTTON_DIR_315)
					RCTOFIT(IDC_BUTTON_DIR_0)
					RCTOFIT(IDC_BUTTON_DIR_45)
					RCTOFIT(IDC_BUTTON_DIR_90)
					RCTOFIT(IDC_BUTTON_DIR_135)
					RCTOFIT(IDC_BUTTON_DIR_180)
					RCTOFIT(IDC_BUTTON_DIR_WORKING)
					RCSPACE(6)
					RCTOFIT(IDC_BUTTON_AUTODIR)
				ENDGROUP()
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(0,0))	// ����UI�ƃX�e�[�^�X�pUI�̋�؂�
					BEGINROWS(WRCT_TOFIT, 0, RCMARGINS(0,2))	// ���o�A���߁A�������r�b�g�}�b�v�̍s
						BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(0,0))	// ���o�A���߂ƕ������r�b�g�}�b�v�̋�؂�
							BEGINROWS(WRCT_TOFIT, 0, RCMARGINS(0,0))
								RCSPACE(25)
								BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(0,2))	// ���ߎԐ���
									RCTOFIT(IDC_STATIC_LANEAPP)
									RCTOFIT(IDC_EDIT_LANEAPP)
									RCTOFIT(IDC_SPIN_LANEAPP)
								ENDGROUP()
							ENDGROUP()
							//RCREST(-1)
							RCSPACE(8)
							RCTOFIT(IDC_STATIC_DIRECTION)
						ENDGROUP()
						RCSPACE(6)
						BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(0,0))	// ���A�E�A��1�E0�̍s
							RCTOFIT(IDC_STATIC_LANELCHANGE)
							RCTOFIT(IDC_STATIC_LANERCHANGE)
							RCREST(-1)
							RCTOFIT(IDC_BUTTON_L1R0)
						ENDGROUP()
						BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(0,0))	// �����A�G�f�B�b�g�{�b�N�X�A��0�E1�̍s
							RCTOFIT(IDC_STATIC_LANECHANGE)
							RCTOFIT(IDC_EDIT_LANELCHANGE)
							RCTOFIT(IDC_SPIN_LANELCHANGE)
							RCTOFIT(IDC_EDIT_LANERCHANGE)
							RCTOFIT(IDC_SPIN_LANERCHANGE)
							RCREST(-1)
							RCTOFIT(IDC_BUTTON_L0R1)
						ENDGROUP()
						BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(0,0))	// ��0�E0�̍s
							RCREST(-1)
							RCTOFIT(IDC_BUTTON_L0R0)
						ENDGROUP()
						BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(0,0))	// �����Ԑ����̍s
							RCTOFIT(IDC_STATIC_LANEFLOW2)
							RCTOFIT(IDC_EDIT_LANEFLOW2)
						ENDGROUP()
					ENDGROUP()
					RCSPACE(14)
					RCTOFIT(IDC_STATIC_LANESTATE)	// �X�e�[�^�X�pUI
				ENDGROUP()
				RCSPACE(4)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(0,0))	// �Ԑ��ʍs��Ԃ̍s
					RCTOFIT(IDC_STATIC_LANEPASS)
					RCREST(IDC_EDIT_LANEPASS)
				ENDGROUP()
				RCSPACE(4)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(0,0))	// �p�^�[�����̓{�^����i[Bug9400]�Œǉ��j
					RCTOFIT(IDC_STATIC_PATTERN)
					RCTOFIT(IDC_BUTTON_PATTERN_A)
					RCTOFIT(IDC_BUTTON_PATTERN_B)
					RCTOFIT(IDC_BUTTON_PATTERN_C)
					RCTOFIT(IDC_BUTTON_PATTERN_D)
					RCTOFIT(IDC_BUTTON_PATTERN_E)
					RCTOFIT(IDC_BUTTON_PATTERN_F)
				ENDGROUP()
			ENDGROUP()
		ENDGROUP()
		RCREST(-1)
		RCTOFIT(IDC_STATIC_SEP2)
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,4))
			RCTOFIT(IDC_STATIC_SOURCE)
			RCREST(IDC_EDIT_SOURCE)
		ENDGROUP()
	ENDGROUP()
END_WINDOW_MAP()

CString	CLQAttrLaneDlg::m_strInfoTableName;
CString	CLQAttrLaneDlg::m_strAppLaneFieldName;
CString	CLQAttrLaneDlg::m_strLanePassFieldName;
CString	CLQAttrLaneDlg::m_strRChangeFieldName;
CString	CLQAttrLaneDlg::m_strLChangeFieldName;

/**
 * �N���X�̖����I�����N���s�����߂ɁA���̃N���X��CExportDlg�N���X���p�����A�����_�C�A���O����CExportDlg���쐬���邱�Ƃɂ��
 * �����_�C�A���O������t���ɂ����邱�Ƃ��ł���N���X�{�̂��쐬����܂��B
 * �ڂ�����http://ptvi.hp.infoseek.co.jp/vc/p_0000010.html���Q�Ƃ̂���
 *
 * @retval CLQAttrLaneDlg��Ԃ��܂��B
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CLQAttrLaneDlg;
}
/////////////////////////////////////////////////////////////////////////////
//
// CLQAttrLaneDlg �����o�֐�
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CLQAttrLaneDlg::CLQAttrLaneDlg()
 * @brief �R���X�g���N�^
 *
 * �����t�H���g�ݒ�Abitmap�摜�̏����������܂�
 */
CLQAttrLaneDlg::CLQAttrLaneDlg() 
	: LQATTR_COMMON_BASE_CLASS(LANE_MAP, IDD_LQATTRLANEDLG),
	m_wndLaneInLink( _T("COMBOBOX"), this, IDC_COMBO_INLINKID ),
	m_wndLaneInNode( _T("COMBOBOX"), this, IDC_COMBO_INNODEID ),
	m_wndLaneApp( _T("EDIT"), this, IDC_EDIT_LANEAPP ),
	m_wndLaneRChange( _T("EDIT"), this, IDC_EDIT_LANERCHANGE ),
	m_wndLaneLChange( _T("EDIT"), this, IDC_EDIT_LANELCHANGE ),
	m_wndLanePass( _T("EDIT"), this, IDC_EDIT_LANEPASS ),
	m_wndLaneBtn1( _T("BUTTON"), this, IDC_BUTTON_L1R0 ),
	m_wndLaneBtn2( _T("BUTTON"), this, IDC_BUTTON_L0R0 ),
	m_wndLaneBtn3( _T("BUTTON"), this, IDC_BUTTON_L0R1 ),
	m_wndLaneGroup( _T("STATIC"), this, IDC_STATIC_LANESTATE ),
	m_wndSource( _T("EDIT"), this, IDC_EDIT_SOURCE ),
	m_wndPatternA( _T("BUTTON"), this, IDC_BUTTON_PATTERN_A ),	// [Bug9400]�Œǉ�
	m_wndPatternB( _T("BUTTON"), this, IDC_BUTTON_PATTERN_B ),	// [Bug9400]�Œǉ�
	m_wndPatternC( _T("BUTTON"), this, IDC_BUTTON_PATTERN_C ),	// [Bug9400]�Œǉ�
	m_wndPatternD( _T("BUTTON"), this, IDC_BUTTON_PATTERN_D ),	// [Bug9400]�Œǉ�
	m_wndPatternE( _T("BUTTON"), this, IDC_BUTTON_PATTERN_E ),	// [Bug9400]�Œǉ�
	m_wndPatternF( _T("BUTTON"), this, IDC_BUTTON_PATTERN_F ),	// [Bug9400]�Œǉ�
	m_wndDir_Working( _T("BUTTON"), this, IDC_BUTTON_DIR_WORKING ),	// [Bug9403]�Œǉ�
	m_wndDir_0( _T("BUTTON"), this, IDC_BUTTON_DIR_0 ),	// [Bug9403]�Œǉ�
	m_wndDir_45( _T("BUTTON"), this, IDC_BUTTON_DIR_45 ),	// [Bug9403]�Œǉ�
	m_wndDir_90( _T("BUTTON"), this, IDC_BUTTON_DIR_90 ),	// [Bug9403]�Œǉ�
	m_wndDir_135( _T("BUTTON"), this, IDC_BUTTON_DIR_135 ),	// [Bug9403]�Œǉ�
	m_wndDir_180( _T("BUTTON"), this, IDC_BUTTON_DIR_180 ),	// [Bug9403]�Œǉ�
	m_wndDir_225( _T("BUTTON"), this, IDC_BUTTON_DIR_225 ),	// [Bug9403]�Œǉ�
	m_wndDir_270( _T("BUTTON"), this, IDC_BUTTON_DIR_270 ),	// [Bug9403]�Œǉ�
	m_wndDir_315( _T("BUTTON"), this, IDC_BUTTON_DIR_315 ),	// [Bug9403]�Œǉ�
	m_wndDir_Null( _T("BUTTON"), this, IDC_BUTTON_DIR_NULL )	// [Bug9403]�Œǉ�
{
	// �t�H���g�쐬
	m_hFont = CreateFont(
		14, 0, 0, 0, 100,                           // �����C���C�w�p�x�C�x�p�x�C����
		FALSE, FALSE, FALSE,                        // �ΆC�����C�ŏ���
		SHIFTJIS_CHARSET,                           // �L�����N�^�ݒ�
		OUT_DEFAULT_PRECIS,                         // �o�͐��x
		CLIP_DEFAULT_PRECIS,                        // �N���b�v���x
		DEFAULT_QUALITY,                            // �i��
		DEFAULT_PITCH | FF_DONTCARE,                // �s�b�`
		AheLoadString(IDS_FONT));                       // �t�H���g���

	// �{�^���̃r�b�g�}�b�v�ǂݍ���
	m_hStraightBmp   = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_STRAIGHT));
	m_hStraightGBmp  = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_STRAIGHTG));
	m_hRightTurnBmp  = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_RIGHTTURN));
	m_hRightTurnGBmp = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_RIGHTTURNG));
	m_hLeftTurnBmp   = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_LEFTTURN));
	m_hLeftTurnGBmp  = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_LEFTTURNG));
	m_hAllLaneBitmap = NULL;
	m_hDirection     = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_DIRECTION));
	m_hDirectionN    = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_DIRECTIONN));
	m_hDirectionNE   = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_DIRECTIONNE));
	m_hDirectionE    = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_DIRECTIONE));
	m_hDirectionSE   = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_DIRECTIONSE));
	m_hDirectionS    = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_DIRECTIONS));
	m_hDirectionSW   = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_DIRECTIONSW));
	m_hDirectionW    = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_DIRECTIONW));
	m_hDirectionNW   = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_DIRECTIONNW));
	m_hDirectionNull = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_DIRECTIONNULL));	// [Bug9403]�Œǉ�
	m_hPatternA		 = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_PATTERN_A));				// [Bug9400]�Œǉ�
	m_hPatternB		 = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_PATTERN_B));				// [Bug9400]�Œǉ�
	m_hPatternC		 = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_PATTERN_C));				// [Bug9400]�Œǉ�
	m_hPatternD		 = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_PATTERN_D));				// [Bug9400]�Œǉ�
	m_hPatternE		 = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_PATTERN_E));				// [Bug9400]�Œǉ�
	m_hPatternF		 = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_PATTERN_F));				// [Bug9400]�Œǉ�
	m_hPatternA2	 = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_PATTERN_A_DISABLED));	// [Bug9400]�Œǉ�
	m_hPatternB2	 = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_PATTERN_B_DISABLED));	// [Bug9400]�Œǉ�
	m_hPatternC2	 = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_PATTERN_C_DISABLED));	// [Bug9400]�Œǉ�
	m_hPatternD2	 = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_PATTERN_D_DISABLED));	// [Bug9400]�Œǉ�
	m_hPatternE2	 = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_PATTERN_E_DISABLED));	// [Bug9400]�Œǉ�
	m_hPatternF2	 = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_PATTERN_F_DISABLED));	// [Bug9400]�Œǉ�

	m_hDir_Working	 = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_DIR_WORKING));// [bug 9403]�Œǉ�
	m_hDir_0		 = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_DIR_0));		// [bug 9403]�Œǉ�  
	m_hDir_45		 = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_DIR_45));	// [bug 9403]�Œǉ�  
	m_hDir_90		 = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_DIR_90));	// [bug 9403]�Œǉ�  
	m_hDir_135		 = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_DIR_135));	// [bug 9403]�Œǉ�  
	m_hDir_180		 = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_DIR_180));	// [bug 9403]�Œǉ�  
	m_hDir_225		 = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_DIR_225));	// [bug 9403]�Œǉ�  
	m_hDir_270		 = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_DIR_270));	// [bug 9403]�Œǉ�  
	m_hDir_315		 = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_DIR_315));	// [bug 9403]�Œǉ�  
	m_hDir_Null		 = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_DIR_NULL));	// [bug 9403]�Œǉ�  

	// ���̑�
	m_strInfoTableName = m_cLQInfo.GetLoadQueueInfo( sindyeRoadQueueLane, sindyeLoadQueueInfoInfoTableName );
	m_strAppLaneFieldName = _T("APPROACH_LANE");
	m_strLanePassFieldName = _T("LANE_PASSABLE");
	m_strRChangeFieldName = _T("RIGHT_CHANGE");
	m_strLChangeFieldName = _T("LEFT_CHANGE");

	m_bCheckLaneConsistency = TRUE;
	_m_bCheckLaneConsistency = TRUE;
	m_bLaneBitEditable = FALSE;
	m_bCheckLaneConsistencyw = FALSE;

	m_bInitializing = FALSE;
	m_bOnInitDlg = FALSE;
}


/** 
 * @fn HWND CLQAttrLaneDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief �q�_�C�A���O���쐬���܂��B
 *
 * LQATTR_COMMON_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)�Ń_�C�A���O���쐬���܂��B
 *
 * @param hWndParent	[in]	�e�E�B���h�E�n���h��
 * @param dwInitParam	[in]	�_�C�A���O�������p�����[�^�i���ݓ��Ɏg�p���Ă��܂���j
 *
 * @retval �쐬���ꂽ�q�_�C�A���O�̃n���h����Ԃ��܂��B
 */
HWND CLQAttrLaneDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{
	// �_�C�A���O�̍쐬��LQATTR_COMMON_BASE_CLASS�ɔC���܂�
	m_hChildWnd = LQATTR_COMMON_BASE_CLASS::Create(hWndParent,dwInitParam);
	return m_hChildWnd;
}

/**
 * @fn void CLQAttrLaneDlg::Delete()
 * @brief �쐬�����q�_�C�A���O���폜���܂��B
 *
 * �_�C�A���O���폜����Ƃ��ɕK���Ă�ŉ������B�Ă΂Ȃ��ƃ��������[�N���Ă��܂��܂��B
 */
void CLQAttrLaneDlg::Delete()
{
	// bug 9598 - [e]�����N�񑮐��ҏW�c�[���Ń��[���̕\�����������ŁA
	// ������R���g���[�����폜���Ă���
	CWindow wndParent = GetDlgItem( IDC_STATIC_LANESTATE );
	CStatic stc( wndParent.GetDlgItem( 5000 ) );
	if( !wndParent.m_hWnd || stc.IsWindow() )
		stc.DestroyWindow();

	::DestroyWindow(m_hChildWnd);
    delete this;
}

bool CLQAttrLaneDlg::AvailabilityOfPatternButton( const long cControlID ) const
{
	// ���O���[�v�ɑ����郊���N��̐����`�F�b�N
	switch (cControlID)
	{
		case IDC_BUTTON_PATTERN_A:
		case IDC_BUTTON_PATTERN_B:
			if (m_listSameINFeature.size() != 3)
				{ return false; }
			break;
		case IDC_BUTTON_PATTERN_C:
		case IDC_BUTTON_PATTERN_D:
		case IDC_BUTTON_PATTERN_E:
		case IDC_BUTTON_PATTERN_F:
			if (m_listSameINFeature.size() != 2)
				{ return false; }
			break;
		default:
			ATLASSERT(0);
			return false;
	}

	// �����N��̕����̑g�ݍ��킹���`�F�b�N
	typedef std::multiset<long> LinkDirCodes;
	LinkDirCodes aLinkDirCodes;	///< LinkDir_C�̏W��
	BOOST_FOREACH (const CLevelOID& rCLevelOID, m_listSameINFeature)
	{
		aLinkDirCodes.insert( LaneDirFromAngle( rCLevelOID.m_lAngle ) );
	}

	switch (cControlID)
	{
		using namespace lane_dir_alias;
		case IDC_BUTTON_PATTERN_A:
		case IDC_BUTTON_PATTERN_B:
			if (aLinkDirCodes != LinkDirCodes( boost::assign::list_of( kLeft )( kStraight )( kRight ) ))
				{ return false; }
			break;
		case IDC_BUTTON_PATTERN_C:
			if (aLinkDirCodes != LinkDirCodes( boost::assign::list_of( kLeft )( kStraight ) ))
				{ return false; }
			break;
		case IDC_BUTTON_PATTERN_D:
		case IDC_BUTTON_PATTERN_E:
			if (aLinkDirCodes != LinkDirCodes( boost::assign::list_of( kStraight )( kRight ) ))
				{ return false; }
			break;
		case IDC_BUTTON_PATTERN_F:
			if (aLinkDirCodes != LinkDirCodes( boost::assign::list_of( kLeft )( kRight ) ))
				{ return false; }
			break;
		default:
			ATLASSERT(0);
			return false;
	}
	
	return true;
}

void CLQAttrLaneDlg::SetStateOfPatternButtons( bool cIsEditable )
{
	// �ҏW�\�ł���A���A�e�R���g���[���ɑΉ���������𖞂����Ă����Enabled�ɂ���
	if( cIsEditable )
	{
		::EnableWindow( GetDlgItem( IDC_BUTTON_PATTERN_A ), AvailabilityOfPatternButton( IDC_BUTTON_PATTERN_A ) ? TRUE : FALSE );
		::EnableWindow( GetDlgItem( IDC_BUTTON_PATTERN_B ), AvailabilityOfPatternButton( IDC_BUTTON_PATTERN_B ) ? TRUE : FALSE );
		::EnableWindow( GetDlgItem( IDC_BUTTON_PATTERN_C ), AvailabilityOfPatternButton( IDC_BUTTON_PATTERN_C ) ? TRUE : FALSE );
		::EnableWindow( GetDlgItem( IDC_BUTTON_PATTERN_D ), AvailabilityOfPatternButton( IDC_BUTTON_PATTERN_D ) ? TRUE : FALSE );
		::EnableWindow( GetDlgItem( IDC_BUTTON_PATTERN_E ), AvailabilityOfPatternButton( IDC_BUTTON_PATTERN_E ) ? TRUE : FALSE );
		::EnableWindow( GetDlgItem( IDC_BUTTON_PATTERN_F ), AvailabilityOfPatternButton( IDC_BUTTON_PATTERN_F ) ? TRUE : FALSE );
	}
	// �ҏW�s�\�ł����Disabled�ɂ���
	else
	{
		::EnableWindow( GetDlgItem( IDC_BUTTON_PATTERN_A ), FALSE);
		::EnableWindow( GetDlgItem( IDC_BUTTON_PATTERN_B ), FALSE);
		::EnableWindow( GetDlgItem( IDC_BUTTON_PATTERN_C ), FALSE);
		::EnableWindow( GetDlgItem( IDC_BUTTON_PATTERN_D ), FALSE);
		::EnableWindow( GetDlgItem( IDC_BUTTON_PATTERN_E ), FALSE);
		::EnableWindow( GetDlgItem( IDC_BUTTON_PATTERN_F ), FALSE);
	}

	// �{�^����Enabled/Disabled�ɉ����ĉ摜�����蓖�Ă�
	CButton aPatternAButton = GetDlgItem( IDC_BUTTON_PATTERN_A );
	aPatternAButton.SetBitmap( aPatternAButton.IsWindowEnabled() ? m_hPatternA : m_hPatternA2 );
	CButton aPatternBButton = GetDlgItem( IDC_BUTTON_PATTERN_B );
	aPatternBButton.SetBitmap( aPatternBButton.IsWindowEnabled() ? m_hPatternB : m_hPatternB2 );
	CButton aPatternCButton = GetDlgItem( IDC_BUTTON_PATTERN_C );
	aPatternCButton.SetBitmap( aPatternCButton.IsWindowEnabled() ? m_hPatternC : m_hPatternC2 );
	CButton aPatternDButton = GetDlgItem( IDC_BUTTON_PATTERN_D );
	aPatternDButton.SetBitmap( aPatternDButton.IsWindowEnabled() ? m_hPatternD : m_hPatternD2 );
	CButton aPatternEButton = GetDlgItem( IDC_BUTTON_PATTERN_E );
	aPatternEButton.SetBitmap( aPatternEButton.IsWindowEnabled() ? m_hPatternE : m_hPatternE2 );
	CButton aPatternFButton = GetDlgItem( IDC_BUTTON_PATTERN_F );
	aPatternFButton.SetBitmap( aPatternFButton.IsWindowEnabled() ? m_hPatternF : m_hPatternF2 );
}

/**
 * @fn BOOL CLQAttrLaneDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
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
BOOL CLQAttrLaneDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
    BOOL bRet = LQATTR_COMMON_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	const bool aIsEditable = ( IsStartEditing() == TRUE && IsEditableFeature() == TRUE && bEditable == TRUE );	///< �ҏW�\����

	if( aIsEditable )
	{
		::SetFocus( GetDlgItem(IDC_COMBO_INLINKID) );

		if( m_lLastLink == 1 )
		{
			SetDlgStatus( IDC_SPIN_LANEAPP, FALSE );
			SetDlgStatus( IDC_SPIN_LANERCHANGE, FALSE );
			SetDlgStatus( IDC_SPIN_LANELCHANGE, FALSE );
			SetDlgStatus( IDC_EDIT_LANEAPP, FALSE, TRUE );
			SetDlgStatus( IDC_EDIT_LANELCHANGE, FALSE, TRUE );
			SetDlgStatus( IDC_EDIT_LANERCHANGE, FALSE, TRUE );
			SetDlgStatus( IDC_EDIT_LANEPASS, FALSE, TRUE );
		}
	}
	else
	{
		::EnableWindow( GetDlgItem( IDC_SPIN_LANEAPP ), FALSE);
		::EnableWindow( GetDlgItem( IDC_SPIN_LANERCHANGE ), FALSE);
		::EnableWindow( GetDlgItem( IDC_SPIN_LANELCHANGE ), FALSE);
	}
	
	// ���[���̃{�^���쐬
	if( !CreateLaneButton() )
		SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, bForce, bEditable );

	// �p�^�[�����̓{�^���Q��Enable/Disable�i[Bug9400]�Œǉ��j
	// m_listSameINFeature���g�p���邽�߁A���ꂪ��łȂ��Ȃ�CreateLaneButton()�̎��s��ɂ��K�v������̂ł����Ŏ��s�i�����Ƃ����ꏊ������H�j�B
	SetStateOfPatternButtons( aIsEditable );

    return bRet;
}

/**
 * @fn void CLQAttrLaneDlg::CreateControlRelation()
 * @brief �q�_�C�A���O�����ۂɍ쐬���܂��B
 *
 * ���ۂ̍쐬��ControlRel�ɔC���܂��B
 */
void CLQAttrLaneDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// �e�E�C���h�E�n���h����ݒ肵�Ȃ���
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );
		// ���[�����p
		m_cControlRel.SetControl( IDC_BUTTON_AUTODIR,		TYPE_BUTTON,	0, 0 );
		m_cControlRel.SetControl( IDC_BUTTON_L1R0,			TYPE_BUTTON,	0, 0 );
		m_cControlRel.SetControl( IDC_BUTTON_L0R1,			TYPE_BUTTON,	0, 0 );
		m_cControlRel.SetControl( IDC_BUTTON_L0R0,			TYPE_BUTTON,	0, 0 );
		m_cControlRel.SetControl( IDC_EDIT_LANEAPP,			TYPE_EDIT,		0, 0,	m_strAppLaneFieldName,	NULL, m_strInfoTableName     );
		m_cControlRel.SetControl( IDC_SPIN_LANEAPP,			TYPE_OTHER,		0, 0 );	// [bug 6705] [e]�ҏW�x���̂��郌�[�����̃X�s���{�^�����g���Ȃ�
		m_cControlRel.SetControl( IDC_EDIT_LANERCHANGE,		TYPE_EDIT,		0, 0,	m_strRChangeFieldName,	NULL, m_strInfoTableName     );
		m_cControlRel.SetControl( IDC_SPIN_LANERCHANGE,		TYPE_OTHER,		0, 0 );	// [bug 6705] [e]�ҏW�x���̂��郌�[�����̃X�s���{�^�����g���Ȃ�
		m_cControlRel.SetControl( IDC_EDIT_LANELCHANGE,		TYPE_EDIT,		0, 0,	m_strLChangeFieldName,	NULL, m_strInfoTableName     );
		m_cControlRel.SetControl( IDC_SPIN_LANELCHANGE,		TYPE_OTHER,		0, 0 );	// [bug 6705] [e]�ҏW�x���̂��郌�[�����̃X�s���{�^�����g���Ȃ�
		if( m_bLaneBitEditable == FALSE )
			m_cControlRel.SetControl( IDC_EDIT_LANEPASS,	TYPE_EDITSTRING,	0, 0,	m_strLanePassFieldName,	NULL, m_strInfoTableName, NULL, -1, FOLLOW_READONLY );
		else
			m_cControlRel.SetControl( IDC_EDIT_LANEPASS,	TYPE_EDITSTRING,	0, 0,	m_strLanePassFieldName,	NULL, m_strInfoTableName );
		m_cControlRel.SetControl( IDC_BUTTON_PATTERN_A,		TYPE_BUTTON,	0, 0 );		// [Bug9400]�Œǉ�
		m_cControlRel.SetControl( IDC_BUTTON_PATTERN_B,		TYPE_BUTTON,	0, 0 );		// [Bug9400]�Œǉ�
		m_cControlRel.SetControl( IDC_BUTTON_PATTERN_C,		TYPE_BUTTON,	0, 0 );		// [Bug9400]�Œǉ�
		m_cControlRel.SetControl( IDC_BUTTON_PATTERN_D,		TYPE_BUTTON,	0, 0 );		// [Bug9400]�Œǉ�
		m_cControlRel.SetControl( IDC_BUTTON_PATTERN_E,		TYPE_BUTTON,	0, 0 );		// [Bug9400]�Œǉ�
		m_cControlRel.SetControl( IDC_BUTTON_PATTERN_F,		TYPE_BUTTON,	0, 0 );		// [Bug9400]�Œǉ�
		m_cControlRel.SetControl( IDC_EDIT_SOURCE,			TYPE_EDITSTRING,	0, 0,	_T("SOURCE"),		NULL, m_strInfoTableName     );

		m_cControlRel.SetControl( IDC_BUTTON_DIR_WORKING,		TYPE_BUTTON,	0, 0 );	// [Bug9403]�Œǉ�
		m_cControlRel.SetControl( IDC_BUTTON_DIR_0,		TYPE_BUTTON,	0, 0 );			// [Bug9403]�Œǉ�
		m_cControlRel.SetControl( IDC_BUTTON_DIR_45,		TYPE_BUTTON,	0, 0 );		// [Bug9403]�Œǉ�
		m_cControlRel.SetControl( IDC_BUTTON_DIR_90,		TYPE_BUTTON,	0, 0 );		// [Bug9403]�Œǉ�
		m_cControlRel.SetControl( IDC_BUTTON_DIR_135,		TYPE_BUTTON,	0, 0 );		// [Bug9403]�Œǉ�
		m_cControlRel.SetControl( IDC_BUTTON_DIR_180,		TYPE_BUTTON,	0, 0 );		// [Bug9403]�Œǉ�
		m_cControlRel.SetControl( IDC_BUTTON_DIR_225,		TYPE_BUTTON,	0, 0 );		// [Bug9403]�Œǉ�
		m_cControlRel.SetControl( IDC_BUTTON_DIR_270,		TYPE_BUTTON,	0, 0 );		// [Bug9403]�Œǉ�
		m_cControlRel.SetControl( IDC_BUTTON_DIR_315,		TYPE_BUTTON,	0, 0 );		// [Bug9403]�Œǉ�
		m_cControlRel.SetControl( IDC_BUTTON_DIR_NULL,		TYPE_BUTTON,	0, 0 );		// [Bug9403]�Œǉ�
	}
	// [Bug9403]�Œǉ�
	// ���[�������̃{�^���̃r�b�g�}�b�v��\��
	CButton aButton	= GetDlgItem( IDC_BUTTON_DIR_WORKING );
	aButton.SetBitmap( m_hDir_Working );
	aButton = GetDlgItem( IDC_BUTTON_DIR_0 );
	aButton.SetBitmap( m_hDir_0 );
	aButton	= GetDlgItem( IDC_BUTTON_DIR_45 );
	aButton.SetBitmap( m_hDir_45 );
	aButton	= GetDlgItem( IDC_BUTTON_DIR_90 );
	aButton.SetBitmap( m_hDir_90 );
	aButton	= GetDlgItem( IDC_BUTTON_DIR_135 );
	aButton.SetBitmap( m_hDir_135 );
	aButton	= GetDlgItem( IDC_BUTTON_DIR_180 );
	aButton.SetBitmap( m_hDir_180 );
	aButton	= GetDlgItem( IDC_BUTTON_DIR_225 );
	aButton.SetBitmap( m_hDir_225 );
	aButton	= GetDlgItem( IDC_BUTTON_DIR_270 );
	aButton.SetBitmap( m_hDir_270 );
	aButton	= GetDlgItem( IDC_BUTTON_DIR_315 );
	aButton.SetBitmap( m_hDir_315 );
	aButton	= GetDlgItem( IDC_BUTTON_DIR_NULL );
	aButton.SetBitmap( m_hDir_Null );
}

/// ���݃Z�b�g����Ă��郌�[����OID���擾����
LONG CLQAttrLaneDlg::GetLaneOID( FEATUREDEF* pFeatDef )
{
	LONG lOID = -1;

	if( pFeatDef )
	{
		if( pFeatDef->GetFeature() != NULL )
			pFeatDef->GetFeature()->get_OID( &lOID );
	}
	return lOID;
}
/// �w�肳�ꂽ���[���̗��������NID���擾����
LONG CLQAttrLaneDlg::GetLaneInLinkID( FEATUREDEF* pFeatDef )
{
	CRelationalTableDef* pRelTableDef = pFeatDef->GetRelationalTableDef( m_cLQInfo.GetLoadQueueInfo( sindyeRoadQueueLane, sindyeLoadQueueInfoLQTableName ) );
	for( CRelationalTableDef::iterator itLQRow = pRelTableDef->begin(); itLQRow != pRelTableDef->end(); itLQRow++ )
	{
		if( GetLongValue( &(*itLQRow), SEQUENCE_FIELD ) == 1 )
			return GetLongValue( &(*itLQRow), LINK_ID_FIELD );
	}

	return -1;
}
/// �w�肳�ꂽ���[���̗��o�����NID���擾����
LONG CLQAttrLaneDlg::GetLaneOutLinkID( FEATUREDEF* pFeatDef )
{
	CRelationalTableDef* pRelTableDef = pFeatDef->GetRelationalTableDef( m_cLQInfo.GetLoadQueueInfo( sindyeRoadQueueLane, sindyeLoadQueueInfoLQTableName ) );
	for( CRelationalTableDef::iterator itLQRow = pRelTableDef->begin(); itLQRow != pRelTableDef->end(); itLQRow++ )
	{
		if( GetLongValue( &(*itLQRow), LASTLINK_F_FIELD ) == 1 )
			return GetLongValue( &(*itLQRow), LINK_ID_FIELD );
	}

	return -1;
}
///
LONG CLQAttrLaneDlg::GetLongValue( CRowDef* pRowDef, LPCTSTR lpcszFieldName )
{
	LONG lVal = -1;

	if( pRowDef )
	{
		const CFieldDef* pFieldDef = pRowDef->GetFieldDef( lpcszFieldName );
		if( pFieldDef )
		{
			if( pFieldDef->m_vaValue.vt == VT_I4 )
				lVal = pFieldDef->m_vaValue.lVal;
		}
	}
	return lVal;
}
IFeatureClassPtr CLQAttrLaneDlg::GetRoadLinkWithLQ( CLQRowDef* pRowDef )
{
	IFeatureClassPtr ipRoadFeatureClass;
	if( !pRowDef )
		return NULL;

	_IRowPtr ipRow = pRowDef->GetFeature();
	ITablePtr ipTestTable;
	ipRow->get_Table( &ipTestTable );
	IDatasetPtr ipDataset = ipTestTable;
	CString strTestOwnerName = AheGetFeatureClassOwnerName( ipDataset );

	// TOC�ɂ��邷�ׂẴW�I�t�B�[�`�����C�����擾���ă��[�v�񂵂��ړ��Ă̓��H�����N�擾����
	IMapPtr ipMap( m_cArcHelper.GetMap() );
	IUIDPtr ipUID( CLSID_UID );
	ipUID->put_Value( CComVariant(L"{E156D7E5-22AF-11D3-9F99-00C04F6BC78E}") ); // IGeoFeatureLayer
	IEnumLayerPtr ipEnumLayer;
	if( SUCCEEDED( ipMap->get_Layers( ipUID, VARIANT_TRUE, &ipEnumLayer ) ) )
	{
		ipEnumLayer->Reset();
		ILayerPtr ipLayer;
		while( S_OK == ipEnumLayer->Next( &ipLayer ) )
		{
			if( !ipLayer )
				break;

			IFeatureClassPtr ipFeatClass;
			if( SUCCEEDED( ((IFeatureLayerPtr)ipLayer)->get_FeatureClass( &ipFeatClass ) ) )
			{
				// ���H�����N���A�I�[�i���������łȂ��Ƃ����Ȃ�
				if( AheGetFeatureClassName( ipLayer ).CompareNoCase( ROAD_TABLE_NAME ) == 0 && strTestOwnerName == AheGetFeatureClassOwnerName( ipFeatClass ))
				{
					ipRoadFeatureClass = ipFeatClass;
					break;
				}
			}
		}
	}
	return ipRoadFeatureClass;
}
IFeaturePtr CLQAttrLaneDlg::GetRoadFeature( CLQRowDef* pRowDef, LONG lOID )
{
	IFeaturePtr ipFeature;
	if( lOID > 0 )
	{
		// LQ�Ɠ������[�N�X�y�[�X�̓��H�����N�t�B�[�`���N���X���擾
		IFeatureClassPtr ipRoadClass = GetRoadLinkWithLQ( pRowDef );
		// ���������N�t�B�[�`���擾
		// �ҏW�J�n�O�͕ҏW�L���b�V�����Ƃ�Ȃ��̂ŁA�S���������ق����͂₢�̂ŕҏW�J�n�O�J�n��ŏꍇ�킯
		if( IsStartEditing() )
		{
			if( ipRoadClass )
				ipRoadClass->GetFeature( lOID, &ipFeature );
		}
		else{
			// ���H�����N�e�[�u������N�G���������ĒT��
			CString		strWhereClause, strSubFields;
			strWhereClause.Format(_T("OBJECTID = %ld"), lOID );
			strSubFields.Format(_T("TO_NODE_ID,FROM_NODE_ID,SHAPE"));
			IQueryFilterPtr		ipQueryFilter( CLSID_QueryFilter );
			ipQueryFilter->put_WhereClause( CComBSTR(strWhereClause) );
			ipQueryFilter->put_SubFields( CComBSTR(strSubFields) );

			IFeatureCursorPtr ipCursor;
			ipRoadClass->Search( ipQueryFilter, VARIANT_FALSE, &ipCursor );
			ipCursor->NextFeature( &ipFeature );
		}
	}

	return ipFeature;	
}

/**
 * @brief ���[���p�̉�ʂ��쐬����
 *
 * @retval FALSE SetCurrentFeatureDefIndex() ����K�v�A��
 * @retval TRUE
 */
BOOL CLQAttrLaneDlg::CreateLaneButton()
{
	if( ! GetCurrentFeatureDef() )
		return FALSE;

	FEATUREDEF* pFeatDef = GetCurrentFeatureDef();	
	LONG lCurrentInLinkID = GetLaneInLinkID( pFeatDef );	// ���݃Z�b�g����Ă��郌�[���̗��������NID���擾����
	LONG lCurrentLaneOID  = GetLaneOID( pFeatDef );			// ���݃Z�b�g����Ă��郌�[����OID���擾����

	if( lCurrentInLinkID < 0 )
	{
		// ���������NID��������Ȃ������ꍇ
		ATLASSERT( FALSE );
		// ���[�j���O���b�Z�[�W���o���Ă�������
		return TRUE;
	}

	// ���݃Z�b�g����Ă��郌�[���̍ŏI�|�C���g���擾����
	IPointPtr ipCurrentFirstPoint;
	IPolylinePtr ipCurrentLaneGeom = pFeatDef->GetShape();
	if( ipCurrentLaneGeom != NULL )	ipCurrentLaneGeom->get_FromPoint( &ipCurrentFirstPoint );

	// ���݃Z�b�g����Ă��郌�[���̗��������N��FROM�ATO�̂ǂ��炪 ipCurrentFirstPoint �Ȃ̂��𒲂ׂ�
	IFeaturePtr ipRoadFeature = GetRoadFeature( pFeatDef, lCurrentInLinkID ); // ���݃Z�b�g����Ă��郌�[���̗��������N���擾����
	BOOL bIsEqualFrom = FALSE;
	if( ipRoadFeature )
	{
		IGeometryPtr ipGeom;
		IPointPtr ipFromPoint, ipToPoint;

		ipRoadFeature->get_Shape( &ipGeom );
		if( ipGeom == NULL )
		{
			ATLASSERT( ipGeom != NULL );
			return TRUE;
		}
		((IPolylinePtr)ipGeom)->get_FromPoint( &ipFromPoint );
		((IPolylinePtr)ipGeom)->get_ToPoint( &ipToPoint );
		
		if( AheIsEqual( ipFromPoint, ipCurrentFirstPoint ) )
			bIsEqualFrom = TRUE;
		else if( AheIsEqual( ipToPoint, ipCurrentFirstPoint ) )
			bIsEqualFrom = FALSE;
		else
		{
			ATLASSERT(FALSE);
			return TRUE;
		}
	}
	
	// �����m�[�hID���R���{�{�b�N�X�ɓ����p
	USES_CONVERSION;
	CComboBox comboNode( GetDlgItem(IDC_COMBO_INNODEID) );			// �����m�[�hID�p�R���{�{�b�N�X

	comboNode.ResetContent();
	CComVariant vaFromID = AheGetAttribute( ipRoadFeature, FROM_FIELD );
	CComVariant vaToID   = AheGetAttribute( ipRoadFeature, TO_FIELD );
	vaFromID.ChangeType( VT_BSTR );
	vaToID.ChangeType( VT_BSTR );

	// vaLinkID �Ɠ��� ID �̓��H�����N�Ɋ֘A���Ă��āA�����̓��H�����N�����������N�ł��郌�[����T��
	list<FEATUREDEF*> listSameInFeature;
	CComboBox combo(     GetDlgItem(IDC_COMBO_INLINKID) );			// ���������NID�p�R���{�{�b�N�X
	combo.ResetContent();
	m_listSameINFeature.clear();

	CString strLQTableName   = m_cLQInfo.GetLoadQueueInfo( sindyeRoadQueueLane, sindyeLoadQueueInfoLQTableName );

	// ���ݕ\������Ă��郊���N��̃I�[�i�����擾
	CString strCurrentOwnerName = AheGetFeatureClassOwnerName( GetCurrentFeatureDef()->GetFeature() );

	INT nLaneNum = -1;	// ���ۂ̎Ԑ���
	std::list<FEATUREDEF>* pFeatDefList = m_cControlRel.GetFeatureDefList();
	for( std::list<FEATUREDEF>::iterator itFeatureDef = pFeatDefList->begin(); itFeatureDef != pFeatDefList->end(); itFeatureDef++ )
	{
		// ���ݕ\������Ă��郊���N��̃f�[�^�Z�b�g�łȂ�������A���������NID�ɃZ�b�g���Ȃ�
		CString strOwnerName = AheGetFeatureClassOwnerName( itFeatureDef->GetFeature() );
		if( strOwnerName != strCurrentOwnerName ) 
			continue;

		if( lstrcmpi( itFeatureDef->GetTableName(), m_strInfoTableName ) != 0 )
			continue;	// ���[���ȊO�͔�΂�

		CRelationalTableDef* pRelTableDef = itFeatureDef->GetRelationalTableDef( strLQTableName );
		if( !pRelTableDef ) 
			continue;

		for( CRelationalTableDef::iterator itLQRow = pRelTableDef->begin(); itLQRow != pRelTableDef->end(); itLQRow++ )
		{
			if( GetLongValue( &(*itLQRow), SEQUENCE_FIELD ) == 1 )
			{
				LONG lLinkID = GetLongValue( &(*itLQRow), LINK_ID_FIELD );
				CString strLinkID;

				// �R���{�{�b�N�X�ɗ��������NID��o�^
				strLinkID.Format(_T("%d"), lLinkID );
				INT nComboIndex = combo.FindString( 0, strLinkID );
				if( nComboIndex == CB_ERR )
					nComboIndex = combo.AddString( strLinkID );

				if( lLinkID == lCurrentInLinkID )
				{
					IPointPtr ipFirstPoint;
					IPolylinePtr ipLaneGeom = itFeatureDef->GetShape();
					if( ipLaneGeom != NULL ) ipLaneGeom->get_FromPoint( &ipFirstPoint );

					// �����m�[�hID���Z�b�g����
					LPCTSTR lpcszID = NULL;
					if( ( bIsEqualFrom && AheIsEqual( ipCurrentFirstPoint, ipFirstPoint ) ) ||
						( !bIsEqualFrom && !AheIsEqual( ipCurrentFirstPoint, ipFirstPoint ) ) )
						lpcszID = OLE2T(vaFromID.bstrVal);
					else
						lpcszID = OLE2T(vaToID.bstrVal);

					INT nPos = comboNode.FindString( 0, lpcszID );
					if( nPos == CB_ERR )
						nPos = comboNode.AddString( lpcszID );
					// �����n�_�������݂̂̂��O���[�s���O����
					if( AheIsEqual( ipCurrentFirstPoint, ipFirstPoint ) )
					{
						// ���݂̑I�����ڂƂ���
						combo.SetCurSel( nComboIndex );
						//
						comboNode.SetCurSel( nPos );
						// �Ԑ����̃J�E���g
						CFieldDef* pFieldDef = itFeatureDef->GetFieldDef( m_strInfoTableName, m_strLanePassFieldName );
						ATLASSERT( pFieldDef != NULL );
						if( pFieldDef != NULL )
						{
							if( pFieldDef->m_vaValue.vt == VT_BSTR )
							{
								CString strLanePass = pFieldDef->m_vaValue.bstrVal;
								if( nLaneNum > 0 && nLaneNum != strLanePass.GetLength() )
								{
									ATLASSERT( nLaneNum != strLanePass.GetLength() );
									// ���傤���Ȃ��̂ő傫���ق��ɂ��킹��
									if( nLaneNum < strLanePass.GetLength() )
										nLaneNum = strLanePass.GetLength();
								}
								else 
									nLaneNum = strLanePass.GetLength();
							}
						}
						// ���X�g�ɂԂ�����ł���
						listSameInFeature.push_back( &(*itFeatureDef) );
						break;
					}
				}
			}
		}
	}

	// �p�x�Ń\�[�g
	SortByAngle( GetCurrentFeatureDef(), lCurrentInLinkID, listSameInFeature, m_listSameINFeature );

	if( m_listSameINFeature.size() <= 0 ) 
	{
		ATLASSERT( m_listSameINFeature.size() > 0 );
		return TRUE;
	}
	// �V�����t�B�[�`��������΁A�������Z�b�g���Ȃ���
	if( AheSetNewLaneAttribute( listSameInFeature, m_listSameINFeature ) == FALSE )
		return FALSE;

	if( m_lLastLink != 1 )
	// [bug 2048] �_�C�A���O���J�����Ƃ��ɕs���������邩�ǂ������`�F�b�N����
	if( m_bCheckLaneConsistency && !m_bInitialCheckedLaneConsistency )
	{
		m_bInitialCheckedLaneConsistency = TRUE;
		if( !CheckLaneBit( m_listSameINFeature ) )
			AttrMessageBox( AheLoadString( IDS_INCONSISTENTDATA ), AheLoadString( IDS_CAPTION ), MB_OK|MB_ICONWARNING, WARNLEVEL_HIGH );
	}

	INT nWidth = 30, nHeight = 30;
	INT nBaseWidth = nWidth * nLaneNum, nBaseHeight = 147/* �Ƃ肠����*/;

	CWindow wndParent = GetDlgItem( IDC_STATIC_LANESTATE );

	if( m_hAllLaneBitmap != NULL ) ::DeleteObject( m_hAllLaneBitmap );
	m_hAllLaneBitmap = NULL;
	m_hAllLaneBitmap = CreateAllLaneBitmap( lCurrentLaneOID, nWidth, nLaneNum );	// �{�^���S�̂̃r�b�g�}�b�v�쐬
	INT nBitmapLeft = 1;

	// �E���������擾
	CString strPassable( pFeatDef->GetFieldDef( m_strInfoTableName, m_strLanePassFieldName )->m_vaValue.bstrVal );
	CComVariant vaRightChange = pFeatDef->GetFieldDef( m_strInfoTableName, m_strRChangeFieldName )->m_vaValue;
	CComVariant vaLeftChange = pFeatDef->GetFieldDef( m_strInfoTableName, m_strLChangeFieldName )->m_vaValue;
	LONG lRightChange = 0, lLeftChange = 0;

	if( vaRightChange.vt == VT_I4 )
		lRightChange = vaRightChange.lVal;
	else
		lRightChange = 0;
	if( vaLeftChange.vt == VT_I4 )
		lLeftChange = vaLeftChange.lVal;
	else
		lLeftChange = 0;

	if( m_lLastLink != 1 )
	{
		// ���[���؂�ւ��{�^���쐬
		CreateChangeLaneButton( lCurrentLaneOID, 0, (nWidth * nLaneNum)/m_listSameINFeature.size(), 30, lLeftChange, lRightChange );

		// ���[���̏�ԕ\���p�{�^���쐬
		CreateLaneStateButton( strPassable, nLaneNum, 10/*�؂�ւ��{�^��*/ + nHeight/*���̍���*/ + 1, nWidth, nHeight, lLeftChange, lRightChange );

		// �E���ܑ����{�^����Enable/Disable��؂�ւ���
		if( m_cArcHelper.IsStartEditing() )		// TODO: SetCurrentFeatureDefIndex()�ł���Ă���悤�ɁA�uIsEditableFeature() == TRUE && bEditable == TRUE�v�ł��鎖�̓`�F�b�N���Ȃ��Ă悢�̂��H
		{
			for( LONG i = 0; i < 3; i++ )
			{
				LONG lLChange = 0, lRChange = 0;
				UINT nID = -1;
				switch( i ) {
					case 0: lLChange = 1; lRChange = 0; nID = IDC_BUTTON_L1R0; break;
					case 1: lLChange = 0; lRChange = 1; nID = IDC_BUTTON_L0R1; break;
					case 2: lLChange = 0; lRChange = 0; nID = IDC_BUTTON_L0R0; break;
					default: break;
				}

				// ���ߎԐ��� - �E������ - �������� > 0 �̃`�F�b�N
				if( pFeatDef ) 
				{
					CFieldDef* pAppLaneFieldDef = pFeatDef->GetFieldDef( m_strInfoTableName, m_strAppLaneFieldName );
					if( pAppLaneFieldDef != NULL && pAppLaneFieldDef->m_vaValue.vt == VT_I4 )
					{
						LONG lLanePassNum = CString_GetLength2( GetLanePassable( pFeatDef ) );
						_ASSERTE( pAppLaneFieldDef->m_vaValue.lVal == lLanePassNum );	// ���ߎԐ����ƃ��[���ʍs��Ԃ̃r�b�g�����ꏏ�ł��邱�Ƃ̊m�F
						::EnableWindow( GetDlgItem( nID ), ( CString_GetLength2( GetLanePassable( pFeatDef ) ) - lLChange - lRChange <= 0 ? FALSE : TRUE ) );
					}
				}
			}
		}
	}
	// ���ߎԐ����A�E�������̍ő�ŏ���ݒ肷��
	LONG lLaneApp = 0, lLChange = 0, lRChange = 0;
	CFieldDef* pLaneAppFieldDef = pFeatDef->GetFieldDef( m_strInfoTableName, m_strAppLaneFieldName );
	CFieldDef* pLChangeFieldDef = pFeatDef->GetFieldDef( m_strInfoTableName, m_strLChangeFieldName );
	CFieldDef* pRChangeFieldDef = pFeatDef->GetFieldDef( m_strInfoTableName, m_strRChangeFieldName );
	if( pLaneAppFieldDef != NULL && pLaneAppFieldDef->m_vaValue.vt == VT_I4 ) lLaneApp = pLaneAppFieldDef->m_vaValue.lVal; else ATLASSERT( FALSE );
	if( pLChangeFieldDef != NULL && pLChangeFieldDef->m_vaValue.vt == VT_I4 ) lLChange = pLChangeFieldDef->m_vaValue.lVal; else ATLASSERT( FALSE );
	if( pRChangeFieldDef != NULL && pRChangeFieldDef->m_vaValue.vt == VT_I4 ) lRChange = pRChangeFieldDef->m_vaValue.lVal; else ATLASSERT( FALSE );
	CUpDownCtrl cSpin( GetDlgItem( IDC_SPIN_LANERCHANGE ) );
	cSpin.SetRange( -5, lLaneApp - lLChange - 1 );
	cSpin = GetDlgItem( IDC_SPIN_LANELCHANGE );
	cSpin.SetRange( -5, lLaneApp - lRChange - 1 );
	cSpin = GetDlgItem( IDC_SPIN_LANEAPP );
	cSpin.SetRange( ( lLChange + lRChange + 1 > 0 ? lLChange + lRChange + 1 : 1 ), 15 );

	// �\���G���A�����킹��
	INT nParentWidth = nBaseWidth;	// �E���܌��̎��́A���̕��]�v�Ɏ��K�v����
	if( lLeftChange < 0 ) nParentWidth -= nWidth * lLeftChange;
	if( lRightChange < 0 ) nParentWidth -= nWidth * lRightChange;

	// �����p�r�b�g�}�b�v���Z�b�g
	SetLaneDirectionBitmap( pFeatDef );

	// �����Ԑ���
	TCHAR strInLane[3];
	_ltot( lLaneApp - lLChange - lRChange, strInLane, 10 );
	::SetWindowText( GetDlgItem(IDC_EDIT_LANEFLOW2), strInLane );

	wndParent.ResizeClient( nParentWidth, nBaseHeight, FALSE );
	CRect dlgRect;
	GetWindowRect( dlgRect );
	m_winMgr.InitToFitSizeFromCurrent(this);
	m_winMgr.CalcLayout(this);
	m_winMgr.SetWindowPositions(this);

	return TRUE;
}
void CLQAttrLaneDlg::SetLaneDirectionBitmap( FEATUREDEF* pFeatDef )
{
	HBITMAP hBmp = m_hDirection;
	if( pFeatDef )
	{
		CFieldDef* pFieldDef = pFeatDef->GetFieldDef( m_strInfoTableName, _T("LANEDIR_C") );
		if( pFieldDef )
		{
			if( pFieldDef->m_vaValue.vt == VT_I4 )
			{
				switch( pFieldDef->m_vaValue.lVal )
				{
					case 1: hBmp = m_hDirectionN;  break;
					case 2: hBmp = m_hDirectionNE; break;
					case 3: hBmp = m_hDirectionE;  break;
					case 4: hBmp = m_hDirectionSE; break;
					case 5: hBmp = m_hDirectionS;  break;
					case 6: hBmp = m_hDirectionSW; break;
					case 7: hBmp = m_hDirectionW;  break;
					case 8: hBmp = m_hDirectionNW; break;
					default: break;
				}
			}else if( pFieldDef->m_vaValue.vt == VT_NULL ) { // [Bug9403]�Œǉ�
				hBmp = m_hDirectionNull; // �������E��ƒ���NULL�̕\���𕪂�����
			}
		}
	}
	CStatic cBmp( GetDlgItem(IDC_STATIC_DIRECTION) );
	cBmp.SetBitmap( hBmp );

	// ������������{�^��Enable/Disable
	if( IsStartEditing() )
	{
		BOOL bEnable = TRUE;
		for( list<CLevelOID>::iterator it = m_listSameINFeature.begin(); it != m_listSameINFeature.end(); it++ )
		{
			FEATUREDEF* pFeatDefTmp = it->m_pLQRowDef;
			if( pFeatDefTmp )
			{
				CFieldDef* pFieldDef = pFeatDefTmp->GetFieldDef( m_strInfoTableName, _T("LANEDIR_C") );
				if( pFieldDef )
				{
					// �L����LaneDir_C���������N�񂪂P�ł����݂����Disabled�ɂ���
					if( pFieldDef->m_vaValue.vt == VT_I4 && pFieldDef->m_vaValue.lVal > 0 )
					{
						bEnable = FALSE;
						break;
					}
				}
			}
		}
		::EnableWindow( GetDlgItem(IDC_BUTTON_AUTODIR), bEnable );
	}
}

CString CLQAttrLaneDlg::GetLanePassable( FEATUREDEF* pFeatDef )
{
	if( pFeatDef )
	{
		CFieldDef* pFieldDef = pFeatDef->GetFieldDef( m_strInfoTableName, m_strLanePassFieldName );
		if( pFieldDef )
		{
			if( pFieldDef->m_vaValue.vt == VT_BSTR )
				return CString( pFieldDef->m_vaValue.bstrVal );
		}
	}
	return _T("");
}
BOOL CLQAttrLaneDlg::SetLanePassable( FEATUREDEF* pFeatDef, LPCTSTR lpcszLanePassable )
{
	if( pFeatDef && lpcszLanePassable )
	{
		CFieldDef* pFieldDef = pFeatDef->GetFieldDef( m_strInfoTableName, m_strLanePassFieldName );
		if( pFieldDef )
		{
			CComVariant vaNewPass;
			vaNewPass.vt = VT_BSTR;
			vaNewPass.bstrVal = T2BSTR(lpcszLanePassable);
			return pFieldDef->SetNewValue( vaNewPass, m_hWnd );
		}
	}
	return FALSE;
}
BOOL CLQAttrLaneDlg::HasReservedLane( LONG lExeptOID, list<INT>& listBits )
{
	BOOL bFind = FALSE;
	listBits.clear();

	for( list<CLevelOID>::iterator it = m_listSameINFeature.begin(); it != m_listSameINFeature.end(); it++ )
	{
		if( lExeptOID == it->m_lOID )
			continue;
		CString strLanePassable = GetLanePassable( it->m_pLQRowDef );
		for( INT i = 0; i < strLanePassable.GetLength(); i++ )
		{
			if( strLanePassable[i] == _RPAS_ )
			{
				bFind = TRUE;
				listBits.push_back( i );
			}
		}
	}
	listBits.sort();
	listBits.unique();

	return bFind;
}
void CLQAttrLaneDlg::OnChangeLanePassable( LPCTSTR lpcszLanePassable/* = NULL*/ )
{
	// ���ݕ\�����Ă���t�B�[�`�����擾����
	FEATUREDEF* pFeatDef = GetCurrentFeatureDef();
	LONG lOID = GetLaneOID( pFeatDef );

	if( !pFeatDef || lOID < 0 )
	{
		ATLASSERT( pFeatDef != NULL && lOID > 0 );
		AttrMessageBox( AheLoadString( IDS_CANTGETCURRENTFEATURE ), AheLoadString( IDS_CAPTION ), MB_OK|MB_ICONWARNING, WARNLEVEL_HIGH );
		return;
	}

	CString strLanePass;
	if( !lpcszLanePassable )
	{
		// ���ݕ\�����Ă���t�B�[�`���̒l���擾����
		CEdit edit( GetDlgItem(IDC_EDIT_LANEPASS) );
		INT nTextLen = edit.GetWindowTextLength() + 1;
		LPTSTR lpszText = (LPTSTR)_alloca( nTextLen * sizeof(TCHAR) );
		edit.GetWindowText( lpszText, nTextLen );
		strLanePass = lpszText;
	}
	else
		strLanePass = lpcszLanePassable;

	ATLASSERT( ! strLanePass.IsEmpty() );

	// �ݒ肵�Ȃ���
	if( CheckLaneBit( lOID, strLanePass, TRUE ) && SetLanePassable( pFeatDef, strLanePass ) && ResetLanePassable( strLanePass ) )
	{
		// ���ߎԐ����C��
		for( list<CLevelOID>::iterator it = m_listSameINFeature.begin(); it != m_listSameINFeature.end(); it++ )
			it->m_pLQRowDef->GetFieldDef( m_strInfoTableName, m_strAppLaneFieldName )->SetNewValue( CComVariant( (LONG)(CString_GetLength2( strLanePass ) ) ), m_hWnd );
		// �e�L�X�g�\�����Ȃ���
		SetDlgItemInt( IDC_EDIT_LANEAPP, CString_GetLength2(strLanePass) );

		// ���[���{�^���쐬���Ȃ���
		CreateLaneButton();
		// �e�L�X�g�\�����Ȃ���
		::SetWindowText( GetDlgItem(IDC_EDIT_LANEPASS), strLanePass );
	}
	else
	{
		// �e�L�X�g�\�����Ȃ���
		::SetWindowText( GetDlgItem(IDC_EDIT_LANEPASS), GetLanePassable( pFeatDef ) );
		AttrMessageBox( AheLoadString( IDS_CANTSETLANEPASS ), AheLoadString( IDS_CAPTION ), MB_OK|MB_ICONWARNING, WARNLEVEL_HIGH );
	}
	// �ĕ`��
	SetButton( Changed() );
}
/**
 * @brief ���[���ʍs��Ԃ̃r�b�g�p�^�[�������������ǂ����`�F�b�N����
 *
 * @retval FALSE �s���ȃr�b�g�p�^�[���inStart�AnEnd�ɂ͐������l�͓���Ȃ��j
 */
BOOL CLQAttrLaneDlg::CheckLaneBit( LONG lTargetOID, CString& strLaneState, INT& nStart, INT& nEnd, INT& nRStart, INT& nREnd, BOOL bRecover/* = FALSE*/, BOOL bAssert/* = TRUE*/  )
{
	if( strLaneState.IsEmpty() ) return FALSE;

	// ���[���̐����������Ȃ��Ȃ�{���ɂ������邩�ǂ����̃_�C�A���O���o��
	if( !m_bCheckLaneConsistency )
	{
		if( _m_bCheckLaneConsistency )
		{
			if( !m_bCheckLaneConsistencyw )
			{
				if( AttrMessageBox( AheLoadString( IDS_NOCHECK ), AheLoadString(IDS_CAPTION), MB_YESNO|MB_ICONWARNING, WARNLEVEL_HIGH ) == IDYES )
				{
					_m_bCheckLaneConsistency = FALSE;	// ���̃_�C�A���O�������Ă������2�x�ƕ�����Ȃ��悤��
					return TRUE;
				}
				else
					m_bCheckLaneConsistency = TRUE;	// �`�F�b�N������悤�ɂ���
			}
			else {
				_m_bCheckLaneConsistency = FALSE;
				return TRUE;
			}
		}
		else
			return TRUE;
	}

	BOOL bIsFirst = FALSE, bIsLast = FALSE;
	BOOL bFindStart = FALSE, bFindEnd = FALSE, bFindRStart = FALSE, bFindREnd = FALSE;
	BOOL bStartIsReserve = FALSE, bEndIsReserve = FALSE;

	// ������
	nStart = nEnd = nRStart = nREnd = -1;

	// �ŏ����Ōォ�̃`�F�b�N
	// [Bug 5387]�Ή��ŁA��̃C�e���[�^�̎Q�Ƃ�����Ɨ�����̂ŋ󂩂ǂ������ׂ�B
	// bIsFirst��bIsLast�͂ǂ��ł������悤���B
	if( !m_listSameINFeature.empty() )
	{
		LONG lOIDFirst = -1, lOIDLast = -1;
		list<CLevelOID>::iterator it2 = m_listSameINFeature.begin();
		list<CLevelOID>::reverse_iterator it = m_listSameINFeature.rbegin();
		it2->m_pLQRowDef->GetFeature()->get_OID( &lOIDFirst );
		it->m_pLQRowDef->GetFeature()->get_OID( &lOIDLast );
		if( lTargetOID == lOIDFirst ) bIsFirst = TRUE;
		if( lTargetOID == lOIDLast  ) bIsLast  = TRUE;
	}

	// ��̃��[�������Ȃ��ꍇ�A0�͑��݂��Ă͂����Ȃ����A���̃A���S���Y���ɂ�
	// ���Ă͂܂�Ȃ��̂œ��ʏ���
	if( bRecover && bIsFirst && bIsLast )
	{
		for( INT i = 0; i < strLaneState.GetLength(); i++ )
		{
			if( strLaneState[i] == _NPAS_ ) strLaneState.SetAt( i, _PASS_ );
		}
	}
	// �w��Ԃ����݂��邩�ǂ����`�F�b�N
	list<INT> listResBits;
	HasReservedLane( lTargetOID, listResBits );

	// ��Ԑ������Ȃ��ꍇ�� Recover ���[�h�Ɍ��� 1 ���� 2 ����
	if( bRecover )
	{
		if( strLaneState.GetLength() == 1 && strLaneState[0] == _NPAS_ )
			strLaneState.SetAt( 0, ( *(listResBits.begin()) == _RPAS_ ? _RPAS_ : _PASS_ ) );	// TODO: ������ArcMap���ُ�I�����鎖������B[Bug9441]�Ƃ��ē��e�ς݁B
	}

	for( INT i = 0; i < strLaneState.GetLength(); i++ )
	{
		if( strLaneState[i] != _NPAS_ )
		{
			if( !bFindStart )
			{
				if( strLaneState[i] == _RPAS_ ) 
					bStartIsReserve = TRUE;
				bFindStart = TRUE;
				nStart = i;
			}
			// 2�̊J�n�ʒu
			if( strLaneState[i] == _RPAS_ )
			{
				if( !bFindRStart )
				{
					bFindRStart = TRUE;
					nRStart = i;
				}
				else if( bFindREnd )
				{
					// 2���A�����Ă��Ȃ�
					if( bRecover == TRUE )
					{
						for( INT j = nREnd + 1; j <= i; j++ )
							strLaneState.SetAt( j, _RPAS_ );

						bFindREnd = FALSE;	// �܂� End �������Ă��Ȃ����Ƃɂ���
					}
					else {
						if( bAssert == TRUE ) ATLASSERT( FALSE );
						return FALSE;
					}
				}
			}
			// 2�̏I���ʒu
			else if( strLaneState[i] == _PASS_ )
			{
				if( bFindRStart == TRUE && bFindREnd == FALSE )
				{
					bFindREnd = TRUE;
					nREnd = i - 1;
				}
			}
			if( bFindEnd && strLaneState[i] == _PASS_ )	// 1���A�����Ă��Ȃ�
			{
				// 201 �Ȃǂ̏ꍇ�͂����ɓ����Ă���B���̂Ƃ��� nStart == nRStart, nEnd == nREnd �Ȃ̂ŁA������`�F�b�N
				if( nStart == nRStart && nEnd == nREnd )
				{
					nStart = i;
					nEnd = -1;
					bFindEnd = FALSE;
				}
				// �C�����[�h�̏ꍇ�͏C��
				else if( bRecover )
				{
					for( INT j = nEnd + 1; j <= i; j++ )
						strLaneState.SetAt( j, _PASS_ );

					bFindEnd = FALSE;	// �܂� End �������ĂȂ����Ƃɂ���
				}
				else
				{
					if( bAssert == TRUE ) ATLASSERT( FALSE );
					return FALSE;
				}
			}
			else if( bFindStart == TRUE && bStartIsReserve == TRUE && strLaneState[i] == _PASS_ )
				bStartIsReserve = FALSE;	// �ŏ���2�ł��̌��1��������A�ŏ���2�͓��ʈ������Ȃ�*/
		}
		else 
		{
			// listResBits����ł������Ă��܂��Ă��܂�Ȃ��悤��
			if( i == 0 && bIsFirst == TRUE && (listResBits.empty() || *(listResBits.begin()) != 0) )
			{
				// �C�����[�h�̏ꍇ�͏C��
				if( bRecover == TRUE )
				{
					strLaneState.SetAt( 0, _PASS_ );
					bFindStart = TRUE;
				} else {
					// ERROR: ���̃��[����1�r�b�g�ڂ�2���Ȃ��āA�������ŏ��̃��[���Ȃ̂�1�r�b�g�ڂ�0
					if( bAssert == TRUE ) ATLASSERT( FALSE );
					ATLTRACE(_T("CLQAttrLaneDlg::CheckLaneBit() : ERROR: ���̃��[����1�r�b�g�ڂ�2���Ȃ��āA�������ŏ��̃��[���Ȃ̂�1�r�b�g�ڂ�0\n") );
					return FALSE;	
				}
			}
			else if( i == strLaneState.GetLength() - 1 && bIsLast == TRUE && ( listResBits.empty() || *listResBits.rbegin() != strLaneState.GetLength() - 1) )
			{
				// �C�����[�h�̏ꍇ�͏C��
				if( bRecover )
				{
					strLaneState.SetAt( strLaneState.GetLength() - 1, _PASS_ );
					bFindEnd = TRUE;
					nEnd = strLaneState.GetLength() - 1;
				}
				else {
					// ERROR: ���̃��[���̍ŏI�r�b�g��2���Ȃ��āA�������Ō�̃��[���Ȃ̂ɍŏI�r�b�g��0
					if( bAssert == TRUE ) ATLASSERT( FALSE );
					ATLTRACE(_T("CLQAttrLaneDlg::CheckLaneBit() : ERROR: ���̃��[���̍ŏI�r�b�g��2���Ȃ��āA�������Ō�̃��[���Ȃ̂ɍŏI�r�b�g��0\n") );
					return FALSE;	
				}
			}
			if( bFindStart )
			{
				if( !bFindEnd )
				{
					bFindEnd = TRUE;
					nEnd = i - 1;
				}
			}
			if( bFindRStart && !bFindREnd )
			{
				bFindREnd = TRUE;
				nREnd = i - 1;
			}
		}
	}
	// ��ԍŌ�̃r�b�g��1�̏ꍇ�� bFindEnd == FALSE �Ȃ̂ŁA���ʏ���
	if( strLaneState[ strLaneState.GetLength() - 1 ] != _NPAS_ )
	{
		if( strLaneState[ strLaneState.GetLength() - 1 ] == _RPAS_ )
		{
			if( !bFindREnd )
			{
				bFindREnd = TRUE;
				nREnd = strLaneState.GetLength() - 1;
			}
			else
			{
				if( bAssert == TRUE ) ATLASSERT( FALSE );	// �A�����Ă��Ȃ� e.x. 00202
				return FALSE;
			}
		}
		if( !bFindEnd )
		{
			bFindEnd = TRUE;
			nEnd = strLaneState.GetLength() - 1;
		}
		else if( nEnd != strLaneState.GetLength() - 1 )
		{
			if( bAssert == TRUE ) ATLASSERT(FALSE);
			return FALSE;	// �A�����Ă��Ȃ� e.x. 11001 
		}
	}
	if( bFindStart == FALSE || bFindEnd == FALSE )
	{
		// ����r�b�g�������Ă��Ȃ��A���̓A���S���Y���G���[
		if( bAssert == TRUE ) ATLASSERT(FALSE);
		return FALSE;
	}

	return TRUE;
}

/**
 * @brief ���[���ʍs��Ԃ̃r�b�g�p�^�[�������������ǂ����`�F�b�N����
 *
 * @note �S�Ẵ��[���Ńr�b�g�p�^�[�������������Ă��邩�ǂ������`�F�b�N���܂�
 *
 * @retval FALSE �s���ȃr�b�g�p�^�[��
 */
BOOL CLQAttrLaneDlg::CheckLaneBit( const list<CLevelOID>& listLane )
{
	CString strAllLanePassable;
	for( list<CLevelOID>::iterator it = m_listSameINFeature.begin(); it != m_listSameINFeature.end(); it++ )
	{
		CString strLanePassable = GetLanePassable( it->m_pLQRowDef );
		// �X�̃r�b�g�����������ǂ����`�F�b�N
		if( CheckLaneBit( it->m_lOID, strLanePassable ) == FALSE )
			return FALSE;	// �X�̃��[���̃r�b�g���������Ȃ��i��ѐ΂Ȃǁj
		// �}�X�^�[�Ɣ�r
		if( strAllLanePassable.IsEmpty() )
			strAllLanePassable = strLanePassable;
		else {
			// �������r
			if( strAllLanePassable.GetLength() != strLanePassable.GetLength() ) 
				return FALSE;	// ���[���̕����Ⴄ

			BOOL bOverlap = FALSE;
			for( INT i = 0; i < strAllLanePassable.GetLength(); i++ )
			{
				BOOL bPassStart = FALSE, bPassEnd = FALSE;
				switch( strLanePassable[i] )
				{
					case _RPAS_:
						// �r�b�g��2�̏ꍇ�̓}�X�^��1�ȊO�łȂ���΂Ȃ�Ȃ�
						if( strAllLanePassable[i] == _PASS_ )
							return FALSE;	// 1��2�������ʒu�ɂ���
						// ���ɃX�^�[�g�t���O�����Ă����ꍇ�̓G���h�t���O������
						if( bPassStart == TRUE ) bPassEnd = TRUE;
						// �}�X�^�[�ɓo�^
						strAllLanePassable.SetAt( i, _RPAS_ );
						break;
					case _PASS_:
						if( bPassStart == FALSE )
							bPassStart = TRUE;	// 1�̊J�n�ʒu���L��
						if( bPassEnd == TRUE )
							return FALSE;		// 1����ђn�ɂȂ��Ă���
						if( i == strAllLanePassable.GetLength() - 1 )
							bPassEnd = TRUE;	// �Ōオ1�̏ꍇ�͂����ŏI���ʒu���L��
						// �}�X�^�[�ɓo�^
						if( strAllLanePassable[i] == _PASS_ )
						{
							if( bOverlap == TRUE )
								return FALSE;	// 1�����̃��[���ƕ����d�����Ă���
							else
								bOverlap = TRUE;	// 1���d��
						}
						else if( strAllLanePassable[i] == _NPAS_ )
							strAllLanePassable.SetAt( i, _PASS_ );
						break;
					case _NPAS_:
						if( bPassStart == TRUE ) bPassEnd = TRUE;
				}
			}
		}
	}
	// �S�̂Ƃ��Ă݂��Ƃ��Ɏ��������Ȃ����`�F�b�N
	for( INT i = 0; i < strAllLanePassable.GetLength(); i++ )
	{
		if( strAllLanePassable[i] == _NPAS_ )
			return FALSE;	// ������
	}

	return TRUE;
}
BOOL CLQAttrLaneDlg::AddOrDeleteLaneBit( LONG lExeptOID, INT nBitNum )
{
	for( list<CLevelOID>::iterator it = m_listSameINFeature.begin(); it != m_listSameINFeature.end(); it++ )
	{
		if( lExeptOID == it->m_lOID )
			continue;

		CString strLanePassable = GetLanePassable( it->m_pLQRowDef );
		INT nLanePassNum = strLanePassable.GetLength();
		// �폜����ꍇ
		if( nBitNum < nLanePassNum )
		{
			INT nDelBits = nLanePassNum - nBitNum, nDeleteCount = 0;
			for( INT i = nLanePassNum - 1; i >= 0 && nDeleteCount < nDelBits; i-- )
			{
				// 2����Ȃ��āA�����0�ȊO�� Find �̌��ʂ����ݍ폜���悤�Ƃ��Ă�����̂łȂ��ꍇ�Ɍ���A�폜
				// e.x. 001 �� i �� 2 �̎� --> Find( _PASS_ ) �̌��ʂ� i �Ȃ̂ŁA��΂�
				if( strLanePassable[i] != _RPAS_ && strLanePassable.Find( _PASS_ ) != i && strLanePassable.Find( _RPAS_ ) != i )
				{
					nDeleteCount++;
					strLanePassable.Delete(i);
				}
			}
			// �폜�ł��Ȃ������ꍇ�́A�ⓚ���p�Ō�납��폜
			if( nDeleteCount != nDelBits )
			{
				strLanePassable.Delete( strLanePassable.GetLength() - nBitNum, nBitNum );
				return TRUE;
			}
		}
		// �ǉ�����ꍇ
		else if( nBitNum > nLanePassNum )
		{
			for( INT i = nLanePassNum - 1; i >= 0; i-- )
			{
				if( strLanePassable[i] == _NPAS_ || strLanePassable[i] == _PASS_ )
				{
					for( INT j = 0; j < nBitNum - nLanePassNum; j++ )
						strLanePassable.Insert( i + 1, strLanePassable[i] );

					break;
				}
				// �Ōオ2�̏ꍇ��1��ǉ����Ă���
				else if( strLanePassable[i] == _RPAS_ )
				{
					for( INT j = 0; j < nBitNum - nLanePassNum; j++ )
						strLanePassable.Insert( i + 1, _PASS_ );

					break;
				}
			}
		}
		SetLanePassable( it->m_pLQRowDef, strLanePassable );
	}

	return TRUE;
}
BOOL CLQAttrLaneDlg::ResetLanePassable( CString& strLanePassOrg, FEATUREDEF* pFeatDef/* = NULL*/ )
{
	// ���ݕ\�����Ă���t�B�[�`���̒l���擾����
	if( !pFeatDef )
		pFeatDef = GetCurrentFeatureDef();

	LONG lOriginalOID = GetLaneOID( pFeatDef );
	INT nStart = -1, nEnd = -1, nRStart = -1, nREnd = -1;

	if( !pFeatDef || lOriginalOID < 1 )
	{
		ATLASSERT( pFeatDef != NULL && lOriginalOID > 0 );
		return FALSE;
	}

	// ���͂��ꂽ������̃`�F�b�N���āA�����C������
	if( CheckLaneBit( lOriginalOID, strLanePassOrg, nStart, nEnd, nRStart, nREnd, TRUE ) == FALSE )
	{
		// �C���s�\������
		ATLASSERT( FALSE );
		return FALSE;
	}

	// �����񒷂̕ω����������瑼�̒ʍs��ԃr�b�g�����������ɂ���
	AddOrDeleteLaneBit( lOriginalOID, strLanePassOrg.GetLength() );

	// �s����OK�Ȃ炱���ŏI���
	if( !m_bCheckLaneConsistency )
		return TRUE;

	USES_CONVERSION;

	CString strOriginalLanePassable = strLanePassOrg;
	// �ҏW�Ώۂ��O���C��
	BOOL bFind = FALSE;
	INT nStartPrev = nStart, nEndPrev = nEnd, nRStartPrev = nRStart, nREndPrev = nREnd;
	for( list<CLevelOID>::reverse_iterator it = m_listSameINFeature.rbegin(); it != m_listSameINFeature.rend(); it++ )
	{
		LONG lOID = GetLaneOID( it->m_pLQRowDef );

		if( lOriginalOID == lOID ) 
		{
			bFind = TRUE;
			continue;
		}

		if( bFind )
		{
			CString strLanePass = GetLanePassable( it->m_pLQRowDef );
			if( strLanePass.IsEmpty() ) strLanePass = strOriginalLanePassable;	// �ʓ|�Ȃ̂ŃR�s�[

			INT nStartTmp = -1, nEndTmp = -1, nRStartTmp = -1, nREndTmp = -1;
			if( CheckLaneBit( lOID, strLanePass, nStartTmp, nEndTmp, nRStartTmp, nREndTmp ) == TRUE )
			{
				// �w��Ԃ��������ꍇ�̍��킹����
				if( nStart >= 0 && nEnd >= 0 )
				{
					for( INT i = nStart; i <= nEnd; i++ )
					{
						if( strLanePass[i] == _RPAS_ ) strLanePass.SetAt( i, _PASS_ );
					}
				}
				// �w��Ԃ��ǉ����ꂽ�ꍇ�̍��킹����
				// e.x. 21000
				//      01110 -> 21110
				if( nRStart >= 0 && nREnd >= 0 )
				{
					for( INT i = nRStart; i <= nREnd; i++ )
					{
						if( strLanePass[i] == _PASS_ ) strLanePass.SetAt( i, _RPAS_ );
					}
				}
				// ��ђn�ɂȂ�ꍇ������̂ŁA�C��
				// e.x. 10001 -> 11111
				CheckLaneBit( lOID, strLanePass, nStartTmp, nEndTmp, nRStartTmp, nREndTmp, TRUE, FALSE );

				// 100,011 -> 100,001 �ɂ��ꂽ�Ƃ��� 110,001 �Ƃ��鏈��
				for( INT i = nStartTmp + 1; i < nStartPrev; i++ )
				{
					if( strLanePass[i] == _NPAS_ ) strLanePass.SetAt( i, _PASS_ );
				}
				// strLanePass �� nStart + 1 �ȍ~�� 0 �ɂ���
				INT nStartPrev1 = -1;
				if( nRStartPrev < 0 )
					nStartPrev1 = nStartPrev + 1;
				else
					nStartPrev1 = nStartPrev + 1 + nRStartPrev + 1;
				for( int i = nStartPrev1; i < strLanePass.GetLength(); i++ )
				{
					if( strLanePass[i] == _PASS_ ) strLanePass.SetAt( i, _NPAS_ );
				}

				if( nStartPrev - 1 < 0 && strLanePass[0] == _NPAS_ )
					strLanePass.SetAt( 0, _PASS_ );
			}
			else
				ATLASSERT( FALSE );	// �ǂ�����H

			SetLanePassable( it->m_pLQRowDef, strLanePass );

			strOriginalLanePassable = strLanePass;
			if( CheckLaneBit( lOID, strOriginalLanePassable, nStartPrev, nEndPrev, nRStartPrev, nREndPrev ) == FALSE )
			{
				// �ύX�����������I�I
				ATLASSERT( FALSE );
			}
		}
	}
	// �ҏW�Ώۂ������C��
	strOriginalLanePassable = strLanePassOrg;
	nStartPrev = nStart, nEndPrev = nEnd, nRStartPrev = nRStart, nREndPrev = nREnd;
	CheckLaneBit( lOriginalOID, strOriginalLanePassable, nStart, nEnd, nRStart, nREnd );
	bFind = FALSE;
	for( list<CLevelOID>::iterator it2 = m_listSameINFeature.begin(); it2 != m_listSameINFeature.end(); it2++ )
	{
		LONG lOID = GetLaneOID( it2->m_pLQRowDef );
	
		if( lOriginalOID == lOID ) 
		{
			bFind = TRUE;
			continue;
		}

		if( bFind )
		{
			CString strLanePass = GetLanePassable( it2->m_pLQRowDef );
			if( strLanePass.IsEmpty() ) strLanePass = strLanePass;	// �ʓ|�Ȃ̂ŃR�s�[

			INT nStartTmp = -1, nEndTmp = -1, nRStartTmp = -1, nREndTmp = -1;
			if( CheckLaneBit( lOID, strLanePass, nStartTmp, nEndTmp, nRStartTmp, nREndTmp ) == TRUE )
			{
				// �w��Ԃ��������ꍇ�̍��킹����
				// e.x. 21000 -> 11000
				//      21110    11110
				//      20001    10001
				if( nStart >= 0 && nEnd >= 0 )
				{
					for( INT i = nStart; i <= nEnd; i++ )
					{
						if( strLanePass[i] == _RPAS_ ) 
							strLanePass.SetAt( i, _PASS_ );
					}
				}
				// �w��Ԃ��ǉ����ꂽ�ꍇ�̍��킹����
				// e.x. 21000
				//      01110 -> 21110
				if( nRStart >= 0 && nREnd >= 0 )
				{
					for( INT i = nRStart; i <= nREnd; i++ )
					{
						if( strLanePass[i] == _PASS_ )
							strLanePass.SetAt( i, _RPAS_ );
					}
				}
				// ��ђn�ɂȂ�ꍇ������̂ŁA�C��
				// e.x. 10001 -> 11111
				CheckLaneBit( lOID, strLanePass, nStartTmp, nEndTmp, nRStartTmp, nREndTmp, TRUE, FALSE );

				// 1 ���� 2 �̕��
				// �ύX���ꂽ���[���A���͑O�̃��[���� i �Ԗڂ� 2 �Ȃ� 2 ����
				// �����łȂ���� 1 ����
				for( INT i = nEndPrev + 1; i < nStartTmp; i++ )
				{
					// �ύX���ꂽ���[���A���͑O�̃��[���� i �Ԗڂ� 2 �Ȃ� 2 ����
					if( strLanePassOrg[i] == _RPAS_ || strOriginalLanePassable[i] == _RPAS_ )
						strLanePass.SetAt( i, _RPAS_ );
					// �����łȂ���� 1 ����
					else
						strLanePass.SetAt( i, _PASS_ );
				}
				// nEnd - 1 >= nStartTmp �Ȃ�AstrLanePass �� nEnd - 1 �ȉ��� 0 �ɂ���
				if( nEndPrev - 1 >= nStartTmp )
				{
					for( INT i = 0; i < nEndPrev; i++ )
						strLanePass.SetAt( i, _NPAS_ );
				}
				// 1 ���� 2 �̕�ԁi�S�� 0 �ɂȂ��Ă��܂����ꍇ�̉����j
				BOOL bFind1Or2 = FALSE;
				for(int  i = 0; i < strLanePass.GetLength(); i++ )
				{
					if( strLanePass[i] != _NPAS_ )
					{
						bFind1Or2 = TRUE;
						break;
					}
				}
				if( !bFind1Or2 )
				{
					// �ύX���ꂽ���[���A���͑O�̃��[���� i �Ԗڂ� 2 �Ȃ� 2 ����
					if( strLanePassOrg[strLanePass.GetLength() - 1] == _RPAS_ || strOriginalLanePassable[strLanePass.GetLength() - 1] == _RPAS_ )
						strLanePass.SetAt( strLanePass.GetLength() - 1, _RPAS_ );
					// �����łȂ���� 1 ����
					else
						strLanePass.SetAt( strLanePass.GetLength() - 1, _PASS_ );
				}
			}
			else
				ATLASSERT( FALSE );	// �ǂ�����H

			SetLanePassable( it2->m_pLQRowDef, strLanePass );
			strOriginalLanePassable = strLanePass;

			if( !CheckLaneBit( lOID, strOriginalLanePassable, nStartPrev, nEndPrev, nRStartPrev, nREndPrev ) )
			{
				// �ύX�����������I�I
				ATLASSERT( FALSE );
			}
		}
	}

	return TRUE;
}
void CLQAttrLaneDlg::CreateChangeLaneButton( LONG lCurrentLaneOID, INT nTop, INT nOneLaneWidth, INT nButtonHeight, LONG lLeftChange, LONG lRightChange )
{
	CWindow wndParent = GetDlgItem( IDC_STATIC_LANESTATE );
	INT nHeight = 10;

	// ���ɕ\�����Ă��镪������Ȃ����
	for( UINT i = 0;; i++ )
	{
		CButton btn( wndParent.GetDlgItem( 1000 + i ) );
		if( btn.IsWindow() )
		{
			if( btn.GetBitmap() != NULL )
				DeleteObject( btn.GetBitmap() );
			btn.DestroyWindow();
		} 
		else
			break;
	}

	// �I��p�r�b�g�}�b�v�쐬
	HBITMAP hBitmapSrc = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_SELECTOR));

	BITMAP	bitmap;
	GetObject( hBitmapSrc, sizeof(BITMAP), &bitmap );

	HDC hDC = GetDC();
	HDC hdcDst = CreateCompatibleDC (hDC);
	HBITMAP hBitmapDst = CreateCompatibleBitmap( hDC, nOneLaneWidth * m_listSameINFeature.size(), bitmap.bmHeight );
	SelectObject (hdcDst, hBitmapDst);	// �쐬�����r�b�g�}�b�v��HDC �Ɋ��蓖��

	HDC hdcSrc = CreateCompatibleDC( hDC );
	SelectObject( hdcSrc, hBitmapSrc );

 	StretchBlt( hdcDst, 0, 0, nOneLaneWidth * m_listSameINFeature.size(), nHeight, hdcSrc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY );

	DeleteObject( hBitmapSrc );
	DeleteObject( hdcSrc );
	DeleteObject( hdcDst );

	INT nCount = 0;
	for( list<CLevelOID>::iterator it = m_listSameINFeature.begin(); it != m_listSameINFeature.end(); it++, nCount++ )
	{
		if( (LONG)(it->m_lOID) == lCurrentLaneOID ) break;
	}

	// �Ԑ����{�^�����쐬
	for( int i = 0; i < (int)(m_listSameINFeature.size()); i++ )
	{
		INT nButtonLeft = i * nOneLaneWidth + ( lLeftChange < 0 ? -lLeftChange * /*nOneLaneWidth*/nButtonHeight/*���r�b�g�}�b�v�̕�*/ : 0 );
		CRect rect( nButtonLeft, nTop, nButtonLeft + nOneLaneWidth, nHeight + nTop );
		CButton btn;
		btn.Create( wndParent.m_hWnd, rect, _T("BUTTON"), BS_BITMAP|BS_FLAT|WS_CHILD|WS_VISIBLE, NULL, 1000 + i );
		// �I��F
		if( i == (ULONG)nCount ) btn.SetBitmap( hBitmapDst );
	}

	// �c��̕��������x���ŏ���
	// [bug 9598] - [e]�����N�񑮐��ҏW�c�[���Ń��[���̕\���������
	// �Ăт��̊֐������Ă�����A�E�B���h�E�����ԂȂ̂ŁA��������Destroy���邱�Ƃɂ���
	// bug9400�̂Ƃ��̑Ή����ƁA���ō쐬����rect�𓖂Ă͂߂Ȃ�������ŁA���[���̕\�������ꂽ�B
	CStatic stc( wndParent.GetDlgItem( 5000 ) );
	if( !wndParent.m_hWnd || stc.IsWindow() )
		stc.DestroyWindow();

	INT nParentWidth = m_listSameINFeature.size() * nOneLaneWidth;
	CRect rect( ( lLeftChange < 0 ? -lLeftChange * /*nOneLaneWidth*/nButtonHeight/*���r�b�g�}�b�v�̕�*/ : 0 ), nTop + nHeight, nParentWidth, nTop + nHeight + 30 );
	stc.Create( wndParent.m_hWnd, rect, _T("STATIC"), SS_BITMAP|WS_CHILD|WS_VISIBLE, NULL, 5000 );
	stc.SetBitmap( m_hAllLaneBitmap );
}
void CLQAttrLaneDlg::CreateLaneStateButton( LPCTSTR lpcszLaneState, INT nLaneNum, INT nTop, INT nButtonWidth, INT nButtonHeight, INT nLeftChange, INT nRightChange )
{
	LONG i = 0;
	CWindow wndParent = GetDlgItem( IDC_STATIC_LANESTATE );
	CString strLaneState( lpcszLaneState );

	// ���[���ʍs��Ԃ̃X�^�e�B�b�N�R���g���[���쐬
	CStatic cLaneState( wndParent.GetDlgItem( 1999 ) );
	if( cLaneState.IsWindow() )
	{
		DeleteObject( cLaneState.GetBitmap() );
		cLaneState.DestroyWindow();
	}

	INT nParentWidth = strLaneState.GetLength() * nButtonWidth;
	if( nLeftChange  < 0 ) 
		nParentWidth += nButtonWidth * (-nLeftChange );
	if( nRightChange < 0 )
		nParentWidth += nButtonWidth * (-nRightChange );
	CRect rect( ( nLeftChange < 0 ? nButtonWidth * (-nLeftChange ) : 0 ), nTop/* + nHeight*/, nParentWidth, nTop + /*nHeight + */100 );
	cLaneState.Create( wndParent.m_hWnd, rect, _T("STATIC"), SS_BITMAP|WS_CHILD|WS_VISIBLE, NULL, 1999 );

	INT nTimes = nLaneNum;
	if( nLeftChange  < 0 ) nTimes -= nLeftChange;
	if( nRightChange < 0 ) nTimes -= nRightChange;

	// �܂��x�[�X�̃r�b�g�}�b�v�����
	INT nBmpWidth = 30, nBmpHeight = 48; // ���\�[�X�̃r�b�g�}�b�v�� 30x48
	INT nMiniButtonLen = nBmpWidth / 3;
	HDC hDC = GetDC();
	HDC hdcDst = CreateCompatibleDC (hDC);
	HBITMAP hBitmapDst = CreateCompatibleBitmap( hDC, nBmpWidth * nTimes, nBmpHeight );	
	SelectObject( hdcDst, hBitmapDst );	// �쐬�����r�b�g�}�b�v��HDC �Ɋ��蓖��

	for( i = 0;; i++ )
	{
		CButton btn( wndParent.GetDlgItem( 2000 + i ) );
		if( btn.IsWindow() )
		{
			DeleteObject( btn.GetBitmap() );
			btn.DestroyWindow();
		}
		else
			break;
	}

	// �����[�����H
	for( i = 0; i < strLaneState.GetLength(); i++ )
	{
		INT nButtonLeft = i * nButtonWidth;
		if( nLeftChange < 0 )
			nButtonLeft += nButtonWidth * (-nLeftChange );

		TCHAR charLaneState = strLaneState.GetAt(i);
		for( INT j = 0; j < 3; j++ )
		{
			CRect rect( nButtonLeft + j * nMiniButtonLen, nTop + nBmpHeight, nButtonLeft + j * nMiniButtonLen + nMiniButtonLen, nTop + nBmpHeight + nMiniButtonLen );
			CButton btn;
			INT nMiniBmpID;

			btn.Create( wndParent.m_hWnd, rect, _T("BUTTON"), BS_BITMAP|BS_FLAT|WS_CHILD|WS_VISIBLE, NULL, 2000 + i * 3 + j );
			switch( j )
			{
			case 0: nMiniBmpID = IDB_BITMAP_MINISTRAIGHT; break;
			case 1: nMiniBmpID = IDB_BITMAP_MINISHITEIGO; break;
			case 2: nMiniBmpID = IDB_BITMAP_MINIDONTGO; break;
			default: break;
			}
			// ���݂̏�ԂƓ����{�^���Ȃ� Disable �ɂ���
			btn.SetBitmap( ::LoadBitmap( _AtlBaseModule.m_hInst, MAKEINTRESOURCE( nMiniBmpID ) ) );
			if( ( charLaneState == _NPAS_ && nMiniBmpID == IDB_BITMAP_MINIDONTGO   ) ||
				( charLaneState == _PASS_ && nMiniBmpID == IDB_BITMAP_MINISTRAIGHT ) ||
				( charLaneState == _RPAS_ && nMiniBmpID == IDB_BITMAP_MINISHITEIGO ) )
			{
				btn.ShowWindow( SW_HIDE );
			}
		}
		INT nBitmapID = -1;
		switch( charLaneState )
		{
		// Bug11268�Ή���CMNGO��SHITEIGO�̉p������\�[�X��p�ӂ����B�t�H���g�ݒ�͉��L�̂Ƃ���
		// �t�H���g��:Franklin Gothic �X�^�C��:���� �T�C�Y:14
		case _NPAS_: nBitmapID = IDB_BITMAP_DONTGO; break;
		case _PASS_: nBitmapID = IDB_BITMAP_CMNGO; break;
		case _RPAS_: nBitmapID = IDB_BITMAP_SHITEIGO; break;
		}
		// �Ή�����r�b�g�}�b�v�����[�h
		HBITMAP hBitmapSrc = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(nBitmapID));
		HDC hdcSrc = CreateCompatibleDC( hDC );
		SelectObject( hdcSrc, hBitmapSrc );
		BitBlt( hdcDst, i * nBmpWidth, 0, nBmpWidth, nBmpHeight, hdcSrc, 0, 0, SRCCOPY );
		DeleteObject( hBitmapSrc );
		DeleteDC( hdcSrc );
	}
	DeleteDC( hdcDst );
	ReleaseDC( hDC );
	cLaneState.SetBitmap( hBitmapDst );

	// �c��̕��������x���ŏ���
	CStatic stc( wndParent.GetDlgItem( 3000 ) );
	if( stc.m_hWnd != NULL )
	{
		DeleteObject( stc.GetBitmap() );
		stc.DestroyWindow();
	}

	if( nLeftChange  < 0 )
		nParentWidth += nButtonWidth * (-nLeftChange );
	if( nRightChange < 0 )
		nParentWidth += nButtonWidth * (-nRightChange );
	CRect rectStaticLane( 0, nTop + nBmpHeight + nMiniButtonLen, nParentWidth, nTop + nBmpHeight + nMiniButtonLen + 100 );

	stc.Create( wndParent.m_hWnd, rectStaticLane, _T("STATIC"), SS_BITMAP|WS_CHILD|WS_VISIBLE, NULL, 3000 );
	stc.SetBitmap( CreateStaticLaneBitmap( nLaneNum, nLeftChange, nRightChange ) );
}

LONG CLQAttrLaneDlg::ChangeINLinkAndNodeID( LONG lInLinkID, LONG lInNodeID )
{
	if( ! GetCurrentFeatureDef() )
		return -1;

	list<FEATUREDEF*> listSameInFeature;
	list<CLevelOID>  listLevelOID;
	CString strInfoTableName = m_cLQInfo.GetLoadQueueInfo( sindyeRoadQueueLane, sindyeLoadQueueInfoInfoTableName );
	CString strLQTableName   = m_cLQInfo.GetLoadQueueInfo( sindyeRoadQueueLane, sindyeLoadQueueInfoLQTableName );

	// lInLinkID �Ɠ��� ID �̓��H�����N�Ɋ֘A���Ă��āA�����̓��H�����N�����������N�ł��郌�[����T��
	list<FEATUREDEF>* pFeatDefList = m_cControlRel.GetFeatureDefList();
	for( list<FEATUREDEF>::iterator itFeatureDef = pFeatDefList->begin(); itFeatureDef != pFeatDefList->end(); itFeatureDef++ )
	{
		if( lstrcmpi( itFeatureDef->GetTableName(), strInfoTableName ) != 0 )
			continue;	// ���[���ȊO�͔�΂�

		CRelationalTableDef* pRelTableDef = itFeatureDef->GetRelationalTableDef( strLQTableName );
		if( pRelTableDef == NULL )
			continue;

		for( CRelationalTableDef::iterator itLQRow = pRelTableDef->begin(); itLQRow != pRelTableDef->end(); itLQRow++ )
		{
			if( itLQRow->GetFieldDef( SEQUENCE_FIELD )->m_vaValue == CComVariant(1L) )
			{
				if( itLQRow->GetFieldDef( LINK_ID_FIELD )->m_vaValue == CComVariant(lInLinkID) )
				{
					if( lInNodeID > 0 )
					{
						// ���H�����N���擾
						IFeaturePtr ipRoadFeature = GetRoadFeature( &(*itFeatureDef), lInLinkID );
						CComVariant vaFromID = AheGetAttribute( ipRoadFeature, FROM_FIELD );
						CComVariant vaToID   = AheGetAttribute( ipRoadFeature, TO_FIELD   );
						// �����m�[�hID��lInNodeID���������ǂ���
						if( ( ( itLQRow->GetFieldDef( LINKDIR_C_FIELD )->m_vaValue == CComVariant(1L)/* ������ */ ) && ( vaFromID.vt == VT_I4 && vaFromID.lVal == lInNodeID ) ) || 
							( ( itLQRow->GetFieldDef( LINKDIR_C_FIELD )->m_vaValue == CComVariant(2L)/* �t���� */ ) && ( vaToID.vt   == VT_I4 && vaToID.lVal   == lInNodeID ) ) )
						{
							// ���X�g�ɂԂ�����ł���
							listSameInFeature.push_back( &(*itFeatureDef) );
							break;
						}
					}
					else
					{
						listSameInFeature.push_back( &(*itFeatureDef) );
						break;
					}
				}
			}
		}
	}

	// �p�x�Ń\�[�g
	SortByAngle( GetCurrentFeatureDef(), lInLinkID, listSameInFeature, listLevelOID );
	
	LONG lOID = -1;
	if( listLevelOID.size() > 0 )
		listLevelOID.begin()->m_pLQRowDef->GetFeature()->get_OID( &lOID );
	else
		ATLASSERT( listLevelOID.size() > 0 );

	return lOID;
}

HBITMAP CLQAttrLaneDlg::CreateStaticLaneBitmap( INT nLaneNum, INT nLeftChange, INT nRightChange )
{
	HBITMAP hBitmapSrc = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_LANE));

	BITMAP	bitmap;
	GetObject( hBitmapSrc, sizeof(BITMAP), &bitmap );
	INT nTimes = nLaneNum;
	if( nLeftChange  < 0 )
		nTimes -= nLeftChange;
	if( nRightChange < 0 )
		nTimes -= nRightChange;

	// �܂��x�[�X�̃r�b�g�}�b�v�����
	HDC hDC = GetDC();
	HDC hdcDst = CreateCompatibleDC (hDC);
	HBITMAP hBitmapDst = CreateCompatibleBitmap( hDC, bitmap.bmWidth * nTimes, bitmap.bmHeight );
	SelectObject (hdcDst, hBitmapDst);	// �쐬�����r�b�g�}�b�v��HDC �Ɋ��蓖��

	HDC hdcSrc = CreateCompatibleDC( hDC );
	SelectObject( hdcSrc, hBitmapSrc );

	for( INT i = 0; i < nTimes; i++ )
		BitBlt( hdcDst, i * bitmap.bmWidth, 0, bitmap.bmWidth, bitmap.bmHeight, hdcSrc, 0, 0, SRCCOPY );

	DeleteObject( hBitmapSrc );

	// ���ɑ��������}�X�N����
	INT nID = IDB_BITMAP_LANERMASK, nWidthDest, nHeightDest, nXOriginDest, nYOriginDest;
	if( nRightChange != 0 )
	{
		// �E
		if( nRightChange < 0 )
		{
			nWidthDest = -bitmap.bmWidth * nRightChange;
			nHeightDest = -bitmap.bmHeight;
			nXOriginDest = nLaneNum * bitmap.bmWidth;
			if( nLeftChange < 0 )
				nXOriginDest += (-nLeftChange) * bitmap.bmWidth;
			nYOriginDest = bitmap.bmHeight;
		} else {
			nWidthDest = bitmap.bmWidth * nRightChange;
			nHeightDest = bitmap.bmHeight;
			nXOriginDest = ( nLaneNum - nRightChange ) * bitmap.bmWidth;
			nYOriginDest = 0;
			if( nLeftChange < 0 )
				nXOriginDest += (-nLeftChange) * bitmap.bmWidth;
		}
		hBitmapSrc = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(nID));
		SelectObject( hdcSrc, hBitmapSrc );
		StretchBlt( hdcDst, nXOriginDest, nYOriginDest, nWidthDest, nHeightDest, hdcSrc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCAND );
		DeleteObject( hBitmapSrc );
	}
	if( nLeftChange != 0 )
	{
		// ��
		nXOriginDest = 0;
		if( nLeftChange < 0 )
		{
			nWidthDest = bitmap.bmWidth * nLeftChange;
			nHeightDest = -bitmap.bmHeight;
			nXOriginDest = -bitmap.bmWidth * nLeftChange;
			nYOriginDest = bitmap.bmHeight;
		} else {
			nWidthDest = -bitmap.bmWidth * nLeftChange;
			nHeightDest = bitmap.bmHeight;
			nXOriginDest = bitmap.bmWidth * nLeftChange;
			nYOriginDest = 0;
		}
		hBitmapSrc = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(nID));
		SelectObject( hdcSrc, hBitmapSrc );
		StretchBlt( hdcDst, nXOriginDest, nYOriginDest, nWidthDest, nHeightDest, hdcSrc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCAND );
		DeleteObject( hBitmapSrc );
	}
	DeleteDC( hdcSrc );
	DeleteDC( hdcDst );
	ReleaseDC( hDC );

	return hBitmapDst;
}
HBITMAP CLQAttrLaneDlg::CreateAllLaneBitmap( const LONG lCurrentOID, const INT nButtonLen, const INT nLaneNum )
{
	INT nButtonNum = m_listSameINFeature.size();	// �쐬����{�^���̐�
	INT nDiffButtonAndBitmap = 2;
	INT nBitmapLen = nButtonLen - nDiffButtonAndBitmap;

	if( nButtonNum <= 0 )
		return NULL;

	INT nAllBitmapWidth = nLaneNum * nButtonLen;
	INT nAllBitmapHeight = nButtonLen;

	// �r�b�g�}�b�v�쐬
	HDC hDC = GetDC();
	HDC hdcDst = CreateCompatibleDC( hDC );
	HBITMAP hBitmapDst = CreateCompatibleBitmap( hDC, nAllBitmapWidth, nAllBitmapHeight );
	SelectObject (hdcDst, hBitmapDst);	// �쐬�����r�b�g�}�b�v��HDC�Ɋ��蓖��

	// ���ŏ�����
	BitBlt( hdcDst, 0, 0, nAllBitmapWidth, nAllBitmapHeight, NULL, 0, 0, WHITENESS );

	// �܂�Disable�̕����̂ݍ쐬����
	for( list<CLevelOID>::iterator it = m_listSameINFeature.begin(); it != m_listSameINFeature.end(); it++ )
	{
		// �R�s�[���̃r�b�g�}�b�v��HDC�Ɋ��蓖��
		HDC hdcSrc = CreateCompatibleDC( hDC );
		SelectObject ( hdcSrc, GetLaneBitmapByAngle( it->m_lAngle, FALSE ) );

		// �r�b�g�������Ă��镪�����R�s�[����
		CComVariant vaLaneState = it->m_pLQRowDef->GetFieldDef( m_strInfoTableName, m_strLanePassFieldName )->m_vaValue;
		if( vaLaneState.vt == VT_BSTR )
		{
			CString strLaneState( vaLaneState.bstrVal );
			for( INT i = 0; i < strLaneState.GetLength(); i++ )
			{
				INT nXSrc = 0, nYSrc = 0;
				if( strLaneState[i] != _NPAS_ )
				{
					BitBlt( hdcDst, i * nButtonLen, 0, nButtonLen, nButtonLen, hdcSrc, nXSrc, nYSrc, SRCERASE );
					BitBlt( hdcDst, i * nButtonLen, 0, nButtonLen, nButtonLen, hdcSrc, nXSrc, nYSrc, SRCERASE );
				}
			}
		}
		else
			ATLASSERT( vaLaneState.vt == VT_BSTR );

		DeleteDC( hdcSrc );
		hdcSrc = NULL;
	}
	// ����Enable�̕������쐬����
	for( list<CLevelOID>::iterator it = m_listSameINFeature.begin(); it != m_listSameINFeature.end(); it++ )
	{
		if( it->m_lOID != lCurrentOID ) 
			continue;

		// �R�s�[���̃r�b�g�}�b�v��HDC�Ɋ��蓖��
		HDC hdcSrc = CreateCompatibleDC( hDC );
		SelectObject (hdcSrc, GetLaneBitmapByAngle( it->m_lAngle, TRUE ) );

		// �r�b�g�������Ă��镪�����R�s�[����
		CComVariant vaLaneState = it->m_pLQRowDef->GetFieldDef( m_strInfoTableName, m_strLanePassFieldName )->m_vaValue;
		if( vaLaneState.vt == VT_BSTR )
		{
			CString strLaneState( vaLaneState.bstrVal );
			for( INT i = 0; i < strLaneState.GetLength(); i++ )
			{
				INT nXSrc = 0, nYSrc = 0;
				if( strLaneState[i] != _NPAS_ )
				{
					BitBlt( hdcDst, i * nButtonLen, 0, nButtonLen, nButtonLen, hdcSrc, nXSrc, nYSrc, MERGEPAINT );
					BitBlt( hdcDst, i * nButtonLen, 0, nButtonLen, nButtonLen, hdcSrc, nXSrc, nYSrc, SRCERASE );
					BitBlt( hdcDst, i * nButtonLen, 0, nButtonLen, nButtonLen, hdcSrc, nXSrc, nYSrc, SRCERASE );
				}
			}
		}
		else
			ATLASSERT( vaLaneState.vt == VT_BSTR );

		DeleteDC( hdcSrc );
		hdcSrc = NULL;

		break;
	}

	DeleteDC( hdcDst );
	ReleaseDC( hDC );

	return hBitmapDst;
}

HBITMAP CLQAttrLaneDlg::GetLaneBitmapByAngle( LONG lAngle, BOOL bCurrent )
{
	HBITMAP hRetBitmap = NULL;

	if( 159 < lAngle && lAngle < 201 )
	{
		if( bCurrent )
			hRetBitmap = m_hStraightBmp;
		else
			hRetBitmap = m_hStraightGBmp;
	}
	// �܂������ȊO�͈�ڂŏI���
	else if( lAngle < 160 )
	{
		if( bCurrent )
			hRetBitmap = m_hRightTurnBmp;
		else
			hRetBitmap = m_hRightTurnGBmp;
	}
	else if( lAngle > 200 )
	{
		if( bCurrent )
			hRetBitmap = m_hLeftTurnBmp;
		else
			hRetBitmap = m_hLeftTurnGBmp;
	}

	return hRetBitmap;
}

HBITMAP CLQAttrLaneDlg::CreateLaneBitmap( HBITMAP hBitmapSrc, INT nLeft, INT nWidth )
{
	if( hBitmapSrc == NULL || nLeft < 0 || nWidth <= 0 ) 
		return NULL;

	BITMAP	bitmap;
	GetObject( hBitmapSrc, sizeof(BITMAP), &bitmap );

	// �r�b�g�}�b�v�����
	HDC hDC = GetDC();
	HDC hdcDst = CreateCompatibleDC (hDC);
	HBITMAP hBitmapDst = CreateCompatibleBitmap( hDC, nWidth, bitmap.bmHeight );
	SelectObject (hdcDst, hBitmapDst);	// �쐬�����r�b�g�}�b�v��HDC �Ɋ��蓖��

	HDC hdcSrc = CreateCompatibleDC( hDC );
	SelectObject( hdcSrc, hBitmapSrc );

	BitBlt( hdcDst, 0, 0, nWidth, bitmap.bmHeight, hdcSrc, nLeft, 0, SRCCOPY );	// ��̃��[���ŉE���ܒ��i�������ꍇ

	DeleteDC( hdcSrc );
	DeleteDC( hdcDst );
	ReleaseDC( hDC );

	return hBitmapDst;
}
void CLQAttrLaneDlg::SortByAngle( CLQRowDef* pRowDef, LONG lStartLinkID, list<FEATUREDEF*>& listOrg, list<CLevelOID>& listNew )
{
	if( lStartLinkID < 0 || listOrg.size() <= 0 ) 
		return;

	// ���������N�t�B�[�`���擾
	IFeaturePtr ipInRoadLink = GetRoadFeature( pRowDef, lStartLinkID );
	// [Bug2848]�Ή��̂��߂ɁAipInRoadLink��NULL���ǂ������ׂȂ��Ă͂Ȃ�Ȃ�
	// ���������N�ɂ́A�ʂ̃f�[�^�Z�b�g�̃����N�������Ă��Ă��܂����߁A
	if( !ipInRoadLink )
		return;

	IGeometryPtr ipInRoadLinkGeom;
	ipInRoadLink->get_Shape( &ipInRoadLinkGeom );
	
	// ���Ԗڂ̍\���_����͂��߂�΂悢�̂����ׂ�
	LONG lStart = -1;
	((IPointCollectionPtr)ipInRoadLinkGeom)->get_PointCount( &lStart );
	lStart -= 2;	// e.x. ���H�����N�̍\���_����3�Ȃ�1����i0�X�^�[�g�j
	_ASSERTE( lStart >= 0 );

	for( list<FEATUREDEF*>::iterator it = listOrg.begin(); it != listOrg.end(); it++ )
	{
		// �����N��`��ƍ\���_�����擾
		LONG lTotalPointCount = -1;
		IPointCollectionPtr ipLQGeometry = (*it)->GetShape();
		ipLQGeometry->get_PointCount( &lTotalPointCount );

		// �ŏI�����N�擾
		LONG lEndLinkID = GetLaneOutLinkID( *it );
		IFeaturePtr ipOutRoadLink = GetRoadFeature( pRowDef, lEndLinkID );
		if( ipOutRoadLink == NULL )
			return;

		IGeometryPtr ipOutRoadLinkGeom;
		ipOutRoadLink->get_Shape( &ipOutRoadLinkGeom );

		// ���Ԗڂ̍\���_�����擾
		LONG lEnd = -1;
		((IPointCollectionPtr)ipOutRoadLinkGeom)->get_PointCount( &lEnd );
		lEnd = lTotalPointCount - lEnd;

		LONG lOID = -1;
		LONG lAngle = (LONG)GetAngle( ipLQGeometry, lStart );
		LONG lDrawAngle = (LONG)GetAngle2( ipLQGeometry, lStart, lEnd );
		(*it)->GetFeature()->get_OID( &lOID );
		
		CLevelOID cOID;
		cOID.m_lOID = lOID;
		cOID.m_mapLevelAngle[lStart] = lAngle;
		cOID.m_pLQRowDef = *it;
		cOID.m_lAngle = lDrawAngle;
		listNew.push_back( cOID );
	}
	// ������������Ȃ�A����ɂ��̐�𒲂ׂ�
	for( list<CLevelOID>::iterator itLevelOID1 = listNew.begin(); itLevelOID1 != listNew.end(); itLevelOID1++ )
	{
		IGeometryPtr ipLevelOID1Geom = itLevelOID1->m_pLQRowDef->GetShape();
		for( list<CLevelOID>::iterator itLevelOID2 = listNew.begin(); itLevelOID2 != listNew.end(); itLevelOID2++ )
		{
			if( itLevelOID1->m_lOID == itLevelOID2->m_lOID )
				continue;

			if( *itLevelOID1 == *itLevelOID2 )
			{
				IGeometryPtr ipLevelOID2Geom = itLevelOID2->m_pLQRowDef->GetShape();
				IGeometryPtr ipIntersectGeom = AheIntersect( ipLevelOID1Geom, ipLevelOID2Geom );
				if( AheIsEqual( ipInRoadLinkGeom, ipIntersectGeom ) == FALSE )
				{
					IPointCollectionPtr ipPointCol( ipIntersectGeom );
					if( ipPointCol != NULL )
					{
						LONG lStart = -1;
						ipPointCol->get_PointCount( &lStart );
						lStart -= 2;

						itLevelOID1->m_mapLevelAngle[lStart] = (LONG)GetAngle( ipLevelOID1Geom, lStart );
						itLevelOID2->m_mapLevelAngle[lStart] = (LONG)GetAngle( ipLevelOID2Geom, lStart );
					}
				}
			}
		}
	}
	listNew.sort(greater<CLevelOID>());
}

DOUBLE CLQAttrLaneDlg::GetAngle( IPolylinePtr ipPolyline, LONG lStart/* = 0*/, BOOL bIsDrawAngle/* = FALSE*/ )
{
	IPointCollectionPtr ipPolyCopy( AheGetClone( ipPolyline ) );

	if( !ipPolyCopy )
		return -1;

	IPointPtr ipPointS, ipPointC, ipPointE;
	LONG lPointCount = -1;

	ipPolyCopy->get_PointCount( &lPointCount );

	if( lPointCount > lStart )
		ipPolyCopy->get_Point( lStart, &ipPointS );
	if( lPointCount > lStart+1 )
		ipPolyCopy->get_Point( lStart+1, &ipPointC );
	if( !bIsDrawAngle )
	{
		if( lPointCount > lStart+2 ) ipPolyCopy->get_Point( lStart+2, &ipPointE );
	}
	else 
		ipPolyCopy->get_Point( lPointCount - 1, &ipPointE );
	
	if( !ipPointS || !ipPointC || !ipPointE )
		return -1;

	DOUBLE dAngle = AheGetAngle( ipPointS, ipPointC, ipPointE );

	if( dAngle < 0.0 )
		dAngle = 360 + dAngle;

	return dAngle;
}

DOUBLE CLQAttrLaneDlg::GetAngle2( IPolylinePtr ipPolyline, LONG lStart, LONG lEnd )
{
	// [lStart] - [lStart+1] �̃Z�O�����g��
	// [lEnd]   - [lEnd+1]   �̃Z�O�����g�̒[�_���������悤�ɕ��s�ړ����A�p�x�����߂�
	LONG lPointCount = -1;
	IPointCollectionPtr ipPoly( ipPolyline );
	if( !ipPoly )
		return -1;
	ipPoly->get_PointCount( &lPointCount );
	if( ! ( lPointCount > lStart + 1 && lPointCount > lEnd + 1 ) ) 
		return -1;

	// �x�N�g��AB�ƃx�N�g��CD�Ƃ���
	IPointPtr A, B, C, D;
	ipPoly->get_Point( lStart, &A );
	ipPoly->get_Point( lStart+1, &B );
	ipPoly->get_Point( lEnd, &C);
	ipPoly->get_Point( lEnd+1, &D );
	DOUBLE Ax, Ay, Bx, By, Cx, Cy, Dx, Dy;
	A->QueryCoords( &Ax, &Ay );
	B->QueryCoords( &Bx, &By );
	C->QueryCoords( &Cx, &Cy );
	D->QueryCoords( &Dx, &Dy );
	// ���s�ړ�����
	DOUBLE Px = Cx - ( Cx - Bx );
	DOUBLE Py = Cy - ( Cy - By );
	DOUBLE D1x = Dx - ( Cx - Bx );
	DOUBLE D1y = Dy - ( Cy - By );

	IPointPtr D1( CLSID_Point );
	D1->PutCoords( D1x, D1y );

	DOUBLE dAngle = AheGetAngle( A, B, D1 );
	if( dAngle < 0.0 )
		dAngle = 360 + dAngle;

	return dAngle;
}

CString CLQAttrLaneDlg::ConvertToLanePassageStr( LPCTSTR lpcszText )
{
	INT nTextLen = lstrlen( lpcszText ) + 1;
	LPTSTR lpszText;

	lpszText = (LPTSTR)_alloca( nTextLen * sizeof(TCHAR) );	// �����̈���m��
	// �S�Ĕ��p�ɂ��Ă��܂�
	LCMapString( GetUserDefaultLCID(), LCMAP_HALFWIDTH, lpcszText, nTextLen, lpszText, nTextLen );

	CString strText( lpszText );
	CString strRet;
	LONG lLen = CString_GetLength2( strText );

	USES_CONVERSION;

	for( LONG i = 0; i < lLen; i++ )
	{
		CString str = CString_GetAt2(strText, i);
		LPSTR lpstr = T2A( (LPTSTR)((LPCTSTR)str) );

		if( lpstr[0] == _NPAS_ || lpstr[0] == _PASS_ || lpstr[0] == _RPAS_ )
		{
			strRet += A2CT( lpstr );
		}
	}

	return strRet;
}

CString CLQAttrLaneDlg::ConvertToLongStr( LPCTSTR lpcszText )
{
	if( lstrcmp( _T(""), lpcszText ) == 0 ||
		lstrcmp( _T("-"), lpcszText ) == 0 ) return CString( lpcszText );

	INT nTextLen = lstrlen( lpcszText ) + 1;
	LPTSTR lpszText;

	lpszText = (LPTSTR)_alloca( nTextLen * sizeof(TCHAR) );	// �����̈���m��
	// �S�Ĕ��p�ɂ��Ă��܂�
	LCMapString( GetUserDefaultLCID(), LCMAP_HALFWIDTH, lpcszText, nTextLen, lpszText, nTextLen );

	CString strText( lpszText );

	USES_CONVERSION;

	// �擪�̃}�C�i�X���l��
	BOOL bMinus = FALSE;
	if( CString_GetLength2( strText ) > 0 )
	{
		CString strFirst = CString_GetAt2(strText, 0);
		LPSTR lpstrFirst = T2A( (LPTSTR)((LPCTSTR)strFirst) );
		if( isasciiminus( lpstrFirst[0] ) ) bMinus = TRUE;
	}

	CString strRetTmp;
	strRetTmp.Format(_T("%d"), _ttol( lpszText ) );

	CString strRet;
	if( bMinus )
	{
		if( strRetTmp.Compare(_T("-0")) == 0 )
			strRet = _T("-");
		else
			strRet = strRetTmp;
	}
	else 
		strRet = strRetTmp;

	return strRet;
}

void CLQAttrLaneDlg::ConvertToLRChange( WORD wID, HWND hWndCtl )
{
	if( ::IsWindow( hWndCtl ) )
	{
		CEdit edit( hWndCtl );
		INT nTextLen = edit.GetWindowTextLength() + 1;
		LPTSTR lpszText = (LPTSTR)_alloca( nTextLen * sizeof(TCHAR) );
		edit.GetWindowText( lpszText, nTextLen );

		// <null> �̏ꍇ���l��
		if( lstrcmp( lpszText, NULL_VALUE ) != 0 )
		{
			CString strOldLRChange;
			CString strLRChange = ConvertToLongStr( lpszText );
			if( strLRChange.Compare(_T("-")) != 0 )
			{
				// ���ߎԐ��� - �E������ - �������� > 0 �̃`�F�b�N
				FEATUREDEF* pFeatDef = GetCurrentFeatureDef();
				if( pFeatDef ) 
				{
					LONG lOriginalOID = -1;
					pFeatDef->GetFeature()->get_OID( &lOriginalOID );

					CString strOldLRChange;
					switch( wID )
					{
						case IDC_EDIT_LANERCHANGE: strOldLRChange = m_strOldRChange; break;
						case IDC_EDIT_LANELCHANGE: strOldLRChange = m_strOldLChange; break;
						case IDC_EDIT_LANEAPP:     strOldLRChange = m_strOldAppLane; break;
						default: break;
					}
	
					LONG lLaneApp = 0, lLChange = 0, lRChange = 0;
					// ���ߎԐ����̒l���擾
					if( wID == IDC_EDIT_LANEAPP )
						lLaneApp = _ttol( strLRChange );
					else
					{
						CFieldDef* pLaneAppFieldDef = pFeatDef->GetFieldDef( m_strInfoTableName, m_strAppLaneFieldName );
						if( pLaneAppFieldDef != NULL && pLaneAppFieldDef->m_vaValue.vt == VT_I4 )
							lLaneApp = pLaneAppFieldDef->m_vaValue.lVal;
						else 
						{
							ATLASSERT( FALSE );
							AttrMessageBox( AheLoadString( IDS_EXCEPTNUMBEROFLANEAPP ), AheLoadString(IDS_CAPTION), MB_OK|MB_ICONWARNING, WARNLEVEL_HIGH  );
							return;
						}
					}
					// �E�������̒l���擾
					if( wID == IDC_EDIT_LANERCHANGE )
						lRChange = _ttol( strLRChange );
					else
					{
						CFieldDef* pRChangeFieldDef = pFeatDef->GetFieldDef( m_strInfoTableName, m_strRChangeFieldName );
						if( pRChangeFieldDef != NULL && pRChangeFieldDef->m_vaValue.vt == VT_I4 )
							lRChange = pRChangeFieldDef->m_vaValue.lVal;
						else 
						{
							ATLASSERT( FALSE );
							AttrMessageBox(  AheLoadString( IDS_EXCEPTNUMBEROFRIGHTLANE ), AheLoadString(IDS_CAPTION), MB_OK|MB_ICONWARNING, WARNLEVEL_HIGH  );
							return;
						}
					}
					// ���������̒l���擾
					if( wID == IDC_EDIT_LANELCHANGE )
						lLChange = _ttol( strLRChange );
					else
					{
						CFieldDef* pLChangeFieldDef = pFeatDef->GetFieldDef( m_strInfoTableName, m_strLChangeFieldName );
						if( pLChangeFieldDef != NULL && pLChangeFieldDef->m_vaValue.vt == VT_I4 )
							lLChange = pLChangeFieldDef->m_vaValue.lVal;
						else 
						{
							ATLASSERT( FALSE );
							AttrMessageBox(  AheLoadString( IDS_EXCEPTNUMBEROFLEFTLANE ), AheLoadString(IDS_CAPTION), MB_OK|MB_ICONWARNING, WARNLEVEL_HIGH  );
							return;
						}
					}

					// �`�F�b�N
					if( lLaneApp - lRChange - lLChange <= 0 )
					{
						// ���ɖ߂��đ��v�Ȃ猳�ɖ߂�
						switch( wID )
						{
							case IDC_EDIT_LANERCHANGE: lRChange = _ttol( m_strOldRChange ); break;
							case IDC_EDIT_LANELCHANGE: lLChange = _ttol( m_strOldLChange ); break;
							case IDC_EDIT_LANEAPP:     lLaneApp = _ttol( m_strOldAppLane ); break;
							default: break;
						}
						if( lLaneApp - lRChange - lLChange > 0 )
						{
							// ���ɖ߂�
							edit.SetWindowText( strOldLRChange );
							strLRChange = strOldLRChange;
							AttrMessageBox( AheLoadString( IDS_RETURNVALUE ), AheLoadString(IDS_CAPTION), MB_OK|MB_ICONWARNING, WARNLEVEL_HIGH );
						}
						else
						{
							// �߂��Ȃ��̂ŕs���̎|�\��
							AttrMessageBox( AheLoadString( IDS_CORRECTVALUE ), AheLoadString(IDS_CAPTION), MB_OK|MB_ICONWARNING, WARNLEVEL_HIGH  );
							return;
						}
					}
					else {
						// �����̃o�b�N�A�b�v�ێ�
						switch( wID )
						{
						case IDC_EDIT_LANERCHANGE: m_strOldRChange = strLRChange; break;
						case IDC_EDIT_LANELCHANGE: m_strOldLChange = strLRChange; break;
						case IDC_EDIT_LANEAPP:     m_strOldAppLane = strLRChange; break;
						default: break;
						}
					}
				}
			}
					
			if( strLRChange.Compare( lpszText ) != 0 )
			{
				INT dwSelStart = 0, dwSelEnd = 0;
				edit.GetSel( dwSelStart, dwSelEnd );
				edit.SetWindowText( strLRChange );
				// ���͈ʒu�̏C��
				INT nDiff = CString_GetLength2(CString( lpszText )) - CString_GetLength2(strLRChange);
				edit.SetSel( dwSelStart - nDiff, dwSelEnd - nDiff );
			}
		}
		m_cControlRel.ControlChanged( wID );
		SetButton( Changed() );
	}
}

BOOL CLQAttrLaneDlg::AheSetNewLaneAttribute( list<FEATUREDEF*>& listFeatureDef, list<CLevelOID>& listLevelOID )
{
	FEATUREDEF* pNewRowDef = NULL;

	// ���ʍ��ڂ��Z�b�g����
	CComVariant vaLanePass, vaAppLane, vaRChange, vaLChange;
	list<CString> listLanePass;

	USES_CONVERSION;

	// �V�K�ɍ쐬���ꂽ���̂�T��
	FEATUREDEF* pOtherFeatDef = NULL;
	for( list<FEATUREDEF*>::iterator it = listFeatureDef.begin(); it != listFeatureDef.end(); it++ )
	{
		// ���[���ȊO�͔�΂�
		if( m_cLQInfo.GetLoadQueueType( AheGetFeatureClassName( (*it)->GetFeature() ), sindyeLoadQueueInfoInfoTableName ) != sindyeRoadQueueLane ) continue;

		if( (*it)->GetDefault() )	
		{
			(*it)->SetDefault( FALSE );	// ��x���������ʂ��Ȃ�����
			pNewRowDef = *it;
			continue;
		}
		else {
			if( !pOtherFeatDef )
				pOtherFeatDef = *it;
		}
	}
	if( !pNewRowDef )
		return TRUE;	// ���ꂽ���̂��Ȃ���΋A��

	for( list<FEATUREDEF*>::iterator it = listFeatureDef.begin(); it != listFeatureDef.end(); it++ )
	{
		// ���[���ȊO�͔�΂�
		if( m_cLQInfo.GetLoadQueueType( AheGetFeatureClassName( (*it)->GetFeature() ), sindyeLoadQueueInfoInfoTableName ) != sindyeRoadQueueLane ) continue;
		if( *it == pNewRowDef ) continue;

		// ���[���ʍs��ԁE���ߎԐ���
		CComVariant vaAppLaneTmp = (*it)->GetFieldDef( m_strInfoTableName, m_strAppLaneFieldName )->m_vaValue;
		if( vaAppLane.vt == VT_EMPTY )
			vaAppLane = vaAppLaneTmp;
		else if( vaAppLane != vaAppLaneTmp )
		{
			ATLASSERT( vaAppLane == vaAppLaneTmp );
			// �����I�ɒ������ق����ǂ��H
			// �����Ă�
		}
		CComVariant vaLanePassTmp = (*it)->GetFieldDef( m_strInfoTableName, m_strLanePassFieldName )->m_vaValue;
		if( vaLanePassTmp.vt != VT_BSTR )
		{
			ATLASSERT( vaLanePassTmp.vt == VT_BSTR );
			// �����I�ɒ������ق����悢�H
			// �����Ă�
		}
		else {
			CString strLanePassTmp = OLE2T( vaLanePassTmp.bstrVal );

			if( vaAppLane.lVal == strLanePassTmp.GetLength() )
				listLanePass.push_back( strLanePassTmp );
			else {
				ATLASSERT( vaAppLane.lVal == strLanePassTmp.GetLength() );
				// �����I�ɒ������ق����ǂ��H
				// �����Ă�
			}
		}

		// �E���ܑ���
		CComVariant vaRChangeTmp = (*it)->GetFieldDef( m_strInfoTableName, m_strRChangeFieldName )->m_vaValue;
		if( vaRChange.vt == VT_EMPTY )
			vaRChange = vaRChangeTmp;
		else if( vaRChange != vaRChangeTmp )
		{
			ATLASSERT( vaRChange == vaRChangeTmp );
			// �����I�ɒ������ق����ǂ��H
			// �����Ă�
		}
		CComVariant vaLChangeTmp = (*it)->GetFieldDef( m_strInfoTableName, m_strLChangeFieldName )->m_vaValue;
		if( vaLChange.vt == VT_EMPTY )
			vaLChange = vaLChangeTmp;
		else if( vaLChange != vaLChangeTmp )
		{
			ATLASSERT( vaLChange == vaLChangeTmp );
			// �����I�ɒ������ق����ǂ��H
			// �����Ă�
		}
	}

	LONG lNewOID = -1;
	pNewRowDef->GetFeature()->get_OID( &lNewOID );
	// �����̃��[���ʍs��Ԃ���V�������[���̒ʍs��Ԃ�ݒ肷��
	CString strPrevLanePassable(_T("1"));
	if( vaAppLane.vt == VT_I4 )
	{
		for( INT i = 1; i < vaAppLane.lVal; i++ ) 
			strPrevLanePassable += _T("0");	// ��ԍ��̃��[����������ꍇ�A�����ƃZ�b�g����Ȃ��̂Ńr�b�g����AppLane�̒l�����킹�Ă���
	}
	for( list<CLevelOID>::iterator it2 = listLevelOID.begin(); it2 != listLevelOID.end(); it2++ )
	{
		if( lNewOID == it2->m_lOID ) break;

		CComVariant vaPrevLanePassable = it2->m_pLQRowDef->GetFieldDef( m_strInfoTableName, m_strLanePassFieldName )->m_vaValue;
		if( vaPrevLanePassable.vt == VT_BSTR )
			strPrevLanePassable = OLE2T( vaPrevLanePassable.bstrVal );
		else {
			ATLASSERT( vaPrevLanePassable.vt == VT_BSTR );
			// ���ɂȂ����Ă���͂��Ȃ̂ŁA�����ɂ͂��Ȃ��Ƃ�����
		}
	}
	vaLanePass.vt = VT_BSTR;
	vaLanePass.bstrVal = strPrevLanePassable.AllocSysString();	// CComBSTR �ł͊֐��𔲂���ƊJ������Ă��܂��̂ŐV�����������m��
	if( vaAppLane.vt != VT_I4 )	
		vaAppLane = (LONG)strPrevLanePassable.GetLength();
	if( vaRChange.vt != VT_I4 )	
		vaRChange = 0L;
	if( vaLChange.vt != VT_I4 ) 
		vaLChange = 0L;

	// �l���Z�b�g
	BOOL bSetted = FALSE;
	if( pNewRowDef->GetFieldDef( m_strInfoTableName, m_strAppLaneFieldName  )->SetNewValue( vaAppLane,  m_hWnd ) ) 
		bSetted = TRUE;
	if( pNewRowDef->GetFieldDef( m_strInfoTableName, m_strRChangeFieldName  )->SetNewValue( vaRChange,  m_hWnd ) ) 
		bSetted = TRUE;
	if( pNewRowDef->GetFieldDef( m_strInfoTableName, m_strLChangeFieldName  )->SetNewValue( vaLChange,  m_hWnd ) ) 
		bSetted = TRUE;
	if( pNewRowDef->GetFieldDef( m_strInfoTableName, m_strLanePassFieldName )->SetNewValue( vaLanePass, m_hWnd ) ) 
		bSetted = TRUE;
	
	// �V�K�ɓ��͂��ꂽ���̂ɑ΂��ċ����I�ɐ����������킹��
	CString strFirstLanePassable = GetLanePassable( pOtherFeatDef );
	ResetLanePassable( strFirstLanePassable, pOtherFeatDef );

	return !bSetted;
}

BOOL CLQAttrLaneDlg::LRChanged( INT wID, BOOL bUseArg/* = FALSE*/, INT nLRChange/* = -100*/ )
{
	CString strFieldName;
	switch( wID )
	{
	case IDC_EDIT_LANERCHANGE: strFieldName = m_strRChangeFieldName; break;
	case IDC_EDIT_LANELCHANGE: strFieldName = m_strLChangeFieldName; break;
	default: break;
	}

	if( ! strFieldName.IsEmpty() )
	{
		FEATUREDEF* pFeatDef = GetCurrentFeatureDef();

		if( pFeatDef ) 
		{
			LONG lOriginalOID = -1;
			pFeatDef->GetFeature()->get_OID( &lOriginalOID );
			CFieldDef* pFieldDef = pFeatDef->GetFieldDef( m_strInfoTableName, strFieldName );
		
			if( pFieldDef )
			{
				// �����Ŏw�肳��Ă���ꍇ�͂�����Z�b�g���Ă���
				if( bUseArg )
					pFieldDef->SetNewValue( CComVariant( (LONG)nLRChange ), m_hWnd );

				for( list<CLevelOID>::iterator it = m_listSameINFeature.begin(); it != m_listSameINFeature.end(); it++ )
				{
					LONG lOID = -1;

					it->m_pLQRowDef->GetFeature()->get_OID( &lOID );
					if( lOriginalOID != lOID )
						it->m_pLQRowDef->GetFieldDef( m_strInfoTableName, strFieldName )->SetNewValue( pFieldDef->m_vaValue, m_hWnd );
				}
			}
			// ���[���̃{�^����蒼��
			CreateLaneButton();
		}
	}
	// �ĕ`��
	SetButton( Changed() );

	return TRUE;
}

BOOL CLQAttrLaneDlg::LaneAppChanged( BOOL bUseArg/* = FALSE*/, INT nNewLaneApp/* = 0*/ )
{
	// ��ԍ������ύX���ꂽ�ꍇ�͍��ɁA��ԉE���ύX���ꂽ�ꍇ�͈�ԉE�ɁA����ȊO�͓K����
	// �폜�̏ꍇ�͒P���ɂ���؂�
	FEATUREDEF* pFeatDef = GetCurrentFeatureDef();

	if( pFeatDef )
	{
		// �ŏ����Ōォ�̃`�F�b�N
		LONG lTargetOID = GetLaneOID( pFeatDef );
		LONG lOIDFirst = -1, lOIDLast = -1;
		BOOL bIsFirst = FALSE, bIsLast = FALSE;

		// [Bug 5387]�Ή��ŁA��̃C�e���[�^�̎Q�Ƃ�����Ɨ�����̂ŋ󂩂ǂ������ׂ�B
		// bIsFirst��bIsLast�͂ǂ��ł������悤���B
		if( !(m_listSameINFeature.empty()) )
		{
			list<CLevelOID>::iterator it2 = m_listSameINFeature.begin();
			list<CLevelOID>::reverse_iterator it = m_listSameINFeature.rbegin();
			it2->m_pLQRowDef->GetFeature()->get_OID( &lOIDFirst );
			it->m_pLQRowDef->GetFeature()->get_OID( &lOIDLast );
			if( lTargetOID == lOIDFirst ) bIsFirst = TRUE;
			if( lTargetOID == lOIDLast  ) bIsLast  = TRUE;
		}

		CString strLanePass = GetLanePassable( pFeatDef );
		INT nLaneApp = ( bUseArg == TRUE ? nNewLaneApp : GetDlgItemInt( IDC_EDIT_LANEAPP ) );
		INT nDiff = nLaneApp - strLanePass.GetLength();

		if( nDiff > 0 )
		{
			if( bIsFirst )
			{
				for( INT i = 0; i < nDiff; i++ )
					strLanePass.Insert( 0, strLanePass[0] );
			}
			else if( bIsLast )
			{
				for( INT i = 0; i < nDiff; i++ )
					strLanePass.Insert( strLanePass.GetLength(), strLanePass[strLanePass.GetLength() - 1] );
			} else {
				for( INT i = 0; i < strLanePass.GetLength(); i++ )
				{
					if( strLanePass[i] == _PASS_ || strLanePass[i] == _RPAS_ )
					{
						for( INT j = 0; j < nDiff; j++ )
							strLanePass.Insert( i, strLanePass[i] );
						break;
					}
				}
			}
		}
		else if( nDiff < 0 )
			strLanePass.Delete( strLanePass.GetLength() + nDiff - 1, -nDiff );	// �폜�̏ꍇ�͂���؂�
		else
			return TRUE;

		OnChangeLanePassable( strLanePass );
	}
	
	return TRUE;
}

