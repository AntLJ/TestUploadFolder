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

// AttrRoadLinkDlg.cpp: CAttrRoadLinkDlg �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AttrRoadLinkDlg.h"
#include <ArcHelperEx/Map.h>
#include <TDC/sindylib_core/FieldMap.h>

#include "RoadMap.h"	// ���H�����N�_�C�A���O�̃E�B���h�E�z�u�}�b�v
#include <WinLib/PixTwip.h>
#include <WinLib/atlwfile.h>
#include <boost/assign.hpp> // for boost::assign

using namespace sindy::schema;
namespace
{
	// �s���{���R�[�h�i�[map
	const std::map<int, CString> prefCodeMap = boost::assign::map_list_of
		(0,  _T(""))
		(1,  _T("�k�C��"))
		(2,  _T("�X��"))
		(3,  _T("��茧"))
		(4,  _T("�{�錧"))
		(5,  _T("�H�c��"))
		(6,  _T("�R�`��"))
		(7,  _T("������"))
		(8,  _T("��錧"))
		(9,  _T("�Ȗ،�"))
		(10, _T("�Q�n��"))
		(11, _T("��ʌ�"))
		(12, _T("��t��"))
		(13, _T("�����s"))
		(14, _T("�_�ސ쌧"))
		(15, _T("�V����"))
		(16, _T("�x�R��"))
		(17, _T("�ΐ쌧"))
		(18, _T("���䌧"))
		(19, _T("�R����"))
		(20, _T("���쌧"))
		(21, _T("�򕌌�"))
		(22, _T("�É���"))
		(23, _T("���m��"))
		(24, _T("�O�d��"))
		(25, _T("���ꌧ"))
		(26, _T("���s�{"))
		(27, _T("���{"))
		(28, _T("���Ɍ�"))
		(29, _T("�ޗǌ�"))
		(30, _T("�a�̎R��"))
		(31, _T("���挧"))
		(32, _T("������"))
		(33, _T("���R��"))
		(34, _T("�L����"))
		(35, _T("�R����"))
		(36, _T("������"))
		(37, _T("���쌧"))
		(38, _T("���Q��"))
		(39, _T("���m��"))
		(40, _T("������"))
		(41, _T("���ꌧ"))
		(42, _T("���茧"))
		(43, _T("�F�{��"))
		(44, _T("�啪��"))
		(45, _T("�{�茧"))
		(46, _T("��������"))
		(47, _T("���ꌧ"))
		;
}

/**
 * �N���X�̖����I�����N���s�����߂ɁA���̃N���X��CExportDlg�N���X���p�����A�����_�C�A���O����CExportDlg���쐬���邱�Ƃɂ��
 * �����_�C�A���O������t���ɂ����邱�Ƃ��ł���N���X�{�̂��쐬����܂��B
 * �ڂ�����http://ptvi.hp.infoseek.co.jp/vc/p_0000010.html���Q�Ƃ̂���
 *
 * @retval CAttrRoadLinkDlg��Ԃ��܂��B
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CAttrRoadLinkDlg();
}

// �H�����̃R�[�h�ɑ΂���ucode�����v�{�^���������ꂽ���̏��� [bug 12036]
LRESULT CAttrRoadLinkDlg::OnClickedSearchRoadCodeButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if(CheckNumVal(IDC_EDIT_ROADCODE))
		SetRoadName(IDC_EDIT_ROADCODE, IDC_EDIT_NAMEKANJI, IDC_EDIT_DISPLAYKANJI, IDC_EDIT_SHIELDNUMBER);
	return 0;
}

// �s���{���R�[�h�ɑ΂���ucode�����v�{�^���������ꂽ���̏��� [bug 12036]
LRESULT CAttrRoadLinkDlg::OnClickedSearchPrefButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if(CheckNumVal(IDC_EDIT_PREFCODE)) {
		SetPrefCodeFromCityAdmin(IDC_EDIT_PREFCODE);
		SetPrefName(IDC_EDIT_PREFCODE, IDC_EDIT_PREFNAME);
	}
	return 0;
}

// ���H�ԍ��ɑ΂���uNo.�����v�{�^���������ꂽ���̏��� [bug 12036]
LRESULT CAttrRoadLinkDlg::OnClickedSearchRoadNumberButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if(CheckNumVal(IDC_EDIT_PREFCODE) && CheckNumVal(IDC_EDIT_ROADNUMBER))
		SetPrefRoadName(IDC_EDIT_PREFCODE, IDC_EDIT_ROADNUMBER, IDC_EDIT_PREFNAME, IDC_EDIT_PREFROADNAME);
	return 0;
}

// ��ԕʏ̃R�[�h�ɑ΂���ucode�����v�{�^���������ꂽ���̏��� [bug 12036]
LRESULT CAttrRoadLinkDlg::OnClickedSearchSectionCodeButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if(CheckNumVal(IDC_EDIT_SECTIONCODE))
		SetSectionName(IDC_EDIT_SECTIONCODE, IDC_EDIT_SECTIONNAME);
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrRoadLinkDlg �����o�֐�
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrBaseMeshDlg::CAttrBaseMeshDlg()
 * @brief �R���X�g���N�^
 *
 * �_�C�A���O�^�C�v�̐ݒ�ƁA�u���V�̏������A���̑����H�����N�֘A�t���O�����Ă����܂��B
 */
CAttrRoadLinkDlg::CAttrRoadLinkDlg() 
	: ATTR_BASE_CLASS(KIHONMAP, IDD_ATTRROADLINKDLG),
		m_wnd0( _T("COMBOBOX"), this, IDC_COMBO_DISPCLASS ),
		m_wnd1( _T("COMBOBOX"), this, IDC_COMBO_NAVICLASS ), 
		m_wnd2( _T("COMBOBOX"), this, IDC_COMBO_MANAGER ), 
		m_wnd3( _T("COMBOBOX"), this, IDC_COMBO_ROADWIDTH ), 
		m_wnd4( _T("COMBOBOX"), this, IDC_COMBO_LINKCLASS1 ), 
		m_wnd5( _T("COMBOBOX"), this, IDC_COMBO_BATCHCHANGE ),	// [bug 9397] 
		m_wnd6( _T("EDIT"), this, IDC_EDIT_SOURCE ),

		m_wndTuika0( _T("CHECKBOX"), this, IDC_CHECK_ISLANDF ),
		m_wndTuika1( _T("CHECKBOX"), this, IDC_CHECK_BYPASS ),
		m_wndTuika2( _T("CHECKBOX"), this, IDC_CHECK_CARONLYF ),
		m_wndTuika3( _T("COMBOBOX"), this, IDC_COMBO_WIDEAREA ),
		m_wndTuika4( _T("COMBOBOX"), this, IDC_COMBO_UPDOWNC ),
		m_wndTuika5( _T("CHECKBOX"), this, IDC_CHECK_LANE ),
		m_wndTuika6( _T("EDIT"), this, IDC_EDIT_LANE ),
		m_wndTuika7( _T("EDIT"), this, IDC_EDIT_RIGHTCHANGE ),
		m_wndTuika8( _T("EDIT"), this, IDC_EDIT_LEFTCHANGE ),
		m_wndTuika9( _T("COMBO"), this, IDC_COMBO_ALLEYMAINTE ),
		m_wndTuika10( _T("EDIT"), this, IDC_EDIT_WORKYEAR ),
		m_wndTuika11( _T("CHECKBOX"), this, IDC_CHECK_WIDEAREAF ),
		m_wndTuika12( _T("COMBO"), this, IDC_COMBO_LEGALSPEED ),
		m_wndTuika13( _T("COMBO"), this, IDC_COMBO_WALKABLE ),

		m_wndKisei0( _T("COMBOBOX"), this, IDC_COMBO_ONEWAY ),
		m_wndKisei1( _T("COMBOBOX"), this, IDC_COMBO_NOPASSAGE ),

		m_wndRosen0(_T("EDIT"),   this, IDC_EDIT_ROADCODE),			   // [bug 12036]
		m_wndRosen1(_T("BUTTON"), this, IDC_BUTTON_SEARCHROADCODE),	   // [bug 12036]
		m_wndRosen2(_T("EDIT"),   this, IDC_EDIT_PREFCODE),			   // [bug 12036]
		m_wndRosen3(_T("BUTTON"), this, IDC_BUTTON_SEARCHPREFCODE),	   // [bug 12036]
		m_wndRosen4(_T("EDIT"),   this, IDC_EDIT_ROADNUMBER),		   // [bug 12036]
		m_wndRosen5(_T("BUTTON"), this, IDC_BUTTON_SEARCHROADNUMBER),  // [bug 12036]
		m_wndRosen6(_T("EDIT"),   this, IDC_EDIT_HIGHWAYNUMBER),	   // [bug 12036]
		m_wndRosen7(_T("EDIT"),   this, IDC_EDIT_SECTIONCODE),		   // [bug 12036]
		m_wndRosen8(_T("BUTTON"), this, IDC_BUTTON_SEARCHSECTIONCODE), // [bug 12036]

		m_wndDRMA0( _T("COMBOBOX"), this, IDC_COMBO_KUBUNCODE ),
		m_wndDRMA1( _T("COMBOBOX"), this, IDC_COMBO_LIMITSPEED ),
		m_wndDRMA2( _T("EDIT"), this, IDC_EDIT_TRIPSPEED ),

		m_wndParking0( _T("COMBOBOX"), this, IDC_COMBO_PARKINGCLASS ),
		m_wndParking1( _T("COMBOBOX"), this, IDC_COMBO_PARKINGLINKCLASS ),
		m_wndParking2( _T("EDIT"), this, IDC_EDIT_FLOORLEVEL ),
		m_wndParking3( _T("EDIT"), this, IDC_EDIT_MIDDLEFLOOR_LEVEL ),

		m_wndTab( _T("TAB"), this, IDC_TAB_ROADATTR )
{
	// �^�u�ʒu�̏�����
	m_lTabNum = 0;
	m_iChangeType = 0;
	m_lCurrentNopassageIndex = 0;
	m_lCurrentOnewayIndex = 0;

	// �ꊇ�ύX�p�̒�`���C���f�b�N�X�̏����� [bug 9397]
	m_nIdx = -1;
}

/**
 * @fn void CAttrRoadLinkDlg::LoadIniFile()
 * @brief �ꊇ�ύX�p��ini�t�@�C����ǂݍ��݁A�����ɑ�����ێ����� [bug 9397] 
 *
 */
void CAttrRoadLinkDlg::LoadIniFile()
{
	// ���W�X�g������ini�t�@�C���p�X�擾
	GetRegCode();

	CFile	IniFile;
	if( IniFile.Open( m_strIniPath ) )
	{
		CString	strBuff;
		int key = 0;

		// �w�b�_�ǂݔ�΂�
		IniFile.GetLineText( strBuff );

		while( IniFile.GetLineText( strBuff ) ){

			int curPos = 0;

			// �^�u��؂�̑����l��ێ�
			BatchChange obj;
			obj.NAME          = strBuff.Tokenize( _T("\t"), curPos );
			obj.DISPCLASS     = strBuff.Tokenize( _T("\t"), curPos );
			obj.NAVICLASS     = strBuff.Tokenize( _T("\t"), curPos );
			obj.MANAGER       = strBuff.Tokenize( _T("\t"), curPos );
			obj.ROADWIDTH     = strBuff.Tokenize( _T("\t"), curPos );
			obj.LINKCLASS     = strBuff.Tokenize( _T("\t"), curPos );
			obj.LANE          = strBuff.Tokenize( _T("\t"), curPos );
			obj.LEGALSPEED    = strBuff.Tokenize( _T("\t"), curPos );
			obj.ALLEYMAINTE   = strBuff.Tokenize( _T("\t"), curPos );
			obj.ROADCODE      = strBuff.Tokenize( _T("\t"), curPos );
			obj.ROADNUMBER    = strBuff.Tokenize( _T("\t"), curPos );
			obj.KUBUNCODE     = strBuff.Tokenize( _T("\t"), curPos );
			obj.LIMITSPEED    = strBuff.Tokenize( _T("\t"), curPos );
			obj.TRIPSPEED     = strBuff.Tokenize( _T("\t"), curPos );

			m_mapBatchChange.insert( pair<int, BatchChange>(key++, obj) );
		}
	}

}

/**
 * @fn void CAttrRoadLinkDlg::GetRegCode()
 * @brief ���W�X�g������ini�t�@�C���̃A�h���X��ǂݍ��� [bug 9397] 
 *
 */
void CAttrRoadLinkDlg::GetRegCode()
{
	// ���W�X�g������ini�t�@�C���p�X���擾
	HKEY hOpenKey = EmxCreateRegKey( _T("Software\\INCREMENT P CORPORATION\\SiNDY-e\\Tools\\AttributeTool"), HKEY_CURRENT_USER );
	if ( hOpenKey ) {
		TCHAR strIniPath[255] = _T("");

		if( EmxGetRegString( _T("RoadLinkBatchIni"), strIniPath, 255, hOpenKey ))
			m_strIniPath = strIniPath;

		RegCloseKey( hOpenKey );
	}


}

/**
 * @fn void CAttrRoadLinkDlg::AddComboValue()
 * @brief ��`�����ꊇ�ύX�p�̃R���{�{�b�N�X�Ɋi�[���� [bug 9397] 
 *
 */
void CAttrRoadLinkDlg::AddComboValue()
{
	// �R���{�{�b�N�X�ɒl������
	CComboBox cCombo( GetDlgItem( IDC_COMBO_BATCHCHANGE ) );
	cCombo.ResetContent();

	INT iCurPos = 0;
	for( std::map<int, BatchChange>::const_iterator it = m_mapBatchChange.begin(); it != m_mapBatchChange.end(); ++it )
	{
		iCurPos = cCombo.AddString( it->second.NAME );
		cCombo.SetItemData( iCurPos, it->first );
	}
	cCombo.SetCurSel( m_nIdx );
}

/**
 * @fn void CAttrRoadLinkDlg::SetBatchValue(INT nID, INT nCpyID)
 * @brief �ꊇ�ύX�p��1�������Z�b�g���� [bug 9397] 
 *
 * @param	nID		[in]		�ݒ肷��R���g���[��ID		
 * @param	Value	[in]		�ݒ肷��l
 */
void CAttrRoadLinkDlg::SetBatchValue(INT nID, CString& Value)
{
	// �u-�v�͊����l���p�����邽�߁A�����Z�b�g���s��Ȃ�
	if( Value != _T("-") )
	{
		CControlDef* pControlDef = m_cControlRel.GetControlDef( ::GetDlgItem(m_hWnd, nID ) );
		CComVariant vaValue = _ttoi(Value);
		pControlDef->SetValue(vaValue);
		pControlDef->SetComboBox(vaValue.intVal);
	}
}

/** 
 * @fn HWND CAttrRoadLinkDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief �q�_�C�A���O���쐬���܂��B
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)�Ń_�C�A���O���쐬���܂��B
 *
 * @param hWndParent	[in]	�e�E�B���h�E�n���h��
 * @param dwInitParam	[in]	�_�C�A���O�������p�����[�^�i���ݓ��Ɏg�p���Ă��܂���j
 *
 * @retval �쐬���ꂽ�q�_�C�A���O�̃n���h����Ԃ��܂��B
 */
HWND CAttrRoadLinkDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	// �e�E�B���h���L�[�v���Ƃ�
	m_hAttributeDlgWnd = hWndParent;

	try{
	m_hChildWnd = ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
	} catch(...) {
	}
	return m_hChildWnd;
}

/**
 * @fn void CAttrRoadLinkDlg::Delete()
 * @brief �쐬�����q�_�C�A���O���폜���܂��B
 *
 * �_�C�A���O���폜����Ƃ��ɕK���Ă�ŉ������B�Ă΂Ȃ��ƃ��������[�N���Ă��܂��܂��B
 */
void CAttrRoadLinkDlg::Delete()
{
	ClearFeatureDefs();
	::DestroyWindow(m_hChildWnd);
    delete this; // �K��new�Ŋm�ۂ���O��
}

/**
 * @fn BOOL CAttrRoadLinkDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
 * @brief ���ݕ\�������ׂ��t�B�[�`����FeatureDef�C���f�b�N�X��n���܂��B
 * ����ɁA�t�B�[�`���̑����Ƃ͊֌W�Ȃ��R���g���[���̐�������Ă����܂��B
 * �_�C�A���O���J�����Ƃ��ɍŏ��Ƀt�H�[�J�X�̓������Ă��ė~�����R���g���[���̐ݒ�����Ă����܂��B
 * �܂��A�O��_�C�A���O���J���Ă����Ƃ��̃^�u�̏�Ԃ��H���������ꍇ�ɂ́ASetRoadName�ŘH�����������Ă���
 * 2�����b�V�����ׂ��ł��铹�H�����N���I������Ă���ꍇ�ɂ́A�֘A���铹�H�����N���m�ۂ���
 *
 * @param lFeatureIndex	[in]	�t�B�[�`����`�N���X�̃C���f�b�N�X�ԍ�
 * @param lTableIndex	[in]	�e�[�u����`�N���X�̃C���f�b�N�X�ԍ�
 * @param lRowIndex		[in]	���R�[�h��`�N���X�̃C���f�b�N�X�ԍ�
 * @param bForce		[in]	�����I�ɕҏW���邩�ǂ���
 * @param bEditable		[in]	�ҏW�\���ǂ���
 *
 * @retval �Ӗ����Ȃ��iTRUE�����Ԃ��Ȃ��j
 */
BOOL CAttrRoadLinkDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 

    BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, m_lTabNum, bForce, bEditable );

	m_lFeatureIndex = lFeatureIndex;
	m_lTableIndex = lTableIndex;
	m_lRowIndex = lRowIndex;
	m_bForce = bForce;
	m_bEditable = bEditable;

	if( m_lTabNum == 3) 
	{
		// ���H���̂��o�� [bug 12036]
		SetRoadName(IDC_EDIT_ROADCODE, IDC_EDIT_NAMEKANJI, IDC_EDIT_DISPLAYKANJI, IDC_EDIT_SHIELDNUMBER);
		m_bRoadCordFirstSearch = TRUE;

		// �s���{�������o�� [bug 12036]
		SetPrefName(IDC_EDIT_PREFCODE, IDC_EDIT_PREFNAME);

		// �������̂��o�� [bug 12036]
		SetPrefRoadName(IDC_EDIT_PREFCODE, IDC_EDIT_ROADNUMBER, IDC_EDIT_PREFNAME, IDC_EDIT_PREFROADNAME);

		// ��ԕʏ̂��o�� [bug 12036]
		SetSectionName(IDC_EDIT_SECTIONCODE, IDC_EDIT_SECTIONNAME);
	}

	// ���H�����N�́A2�����b�V���E���ׂ��ł��邩�ǂ����𒲂ׂ�K�v������A

	// 2�����b�V�����E�ׂ��ł��邩�ǂ����t���O�̏�����
	m_bIsRoadOnMesh = FALSE;
	m_pRoadOnMeshList.clear();

	// ���ݑI�𒆂̃t�B�[�`���擾
	IFeaturePtr		ipFeature = GetCurrentFeatureDef()->GetFeature();

	CTargetItems cRoadCrossMesh;
	cRoadCrossMesh.clear();

	// 2�����b�V�����ׂ��ł��邩�ǂ������`�F�b�N���A�ׂ��ł���ΐڑ����Ă��铹�H���擾
	if( AheIsRoadCrossMesh( m_cArcHelper.GetMap(), ipFeature, cRoadCrossMesh ) )
	{
		// 2�����b�V�����ׂ��ł���̂Ńt���O�ύX
		m_bIsRoadOnMesh = TRUE;

		// �I�����ꂽ�A2�����b�V���֘A�̓��H��o�^����
		for( CTargetItems::iterator it = cRoadCrossMesh.begin(); it != cRoadCrossMesh.end(); it++ )
		{
			IFeaturePtr ipFeatureOnMesh = it->GetFeature();
			CFeatureDef featDef;
			featDef.SetArcHelper( m_cArcHelper.GetApp() );
			featDef.SetFeature( ipFeatureOnMesh );
			m_pRoadOnMeshList.push_back(featDef);
		}
	}

	// ��ʂƒʋւ͕ҏW�ł��Ȃ��悤��DISABLE��
	SetDlgStatus(IDC_COMBO_ONEWAY, FALSE );
	SetDlgStatus(IDC_COMBO_NOPASSAGE, FALSE );

	// �_�C�A���O�J���Ƃ��ɂ̓G���[���b�Z�[�W�͏o�����Ƀ^�u�̃n�C���C�g�����ɂƂǂ߂�
	// ���H�����N�����[�V�������`�F�b�N���^�u�̃n�C���C�g������
	CString strMessage;
	CheckRoadLinkRelation( &strMessage, GetCurrentFeatureDef());

	// ���ڃt�B�[�`���Ɋ֌W�Ȃ��G�f�B�b�g�{�b�N�X(��F�Q�Ɨp�̘H�����V�[�P���X�⌴�e�ԍ�����)�Ȃǂ𐧌䂷��
	// �ҏW�J�n�O�ƕҏW�J�n��ł��ҏW�s�̂Ƃ���DISABLE�ɂ���
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		// �ҏW�J�n����Ă���Ƃ��ɂ��łɁASetFocus���Ă���
		SetForcusAfterTabChange();
	}

	// �ꊇ�ύX�pini�ǂݍ��� [bug 9397] 
	LoadIniFile();

	// �ǂݍ���ini����R���{�ɃL�[��ǉ� [bug 9397] 
	AddComboValue();

	// �ǂ����̒l���ύX����Ă���ꍇ�͈ꊇ�ύX�R���{���g�p�s�Ƃ��� [bug 9449]
	if( Changed() )
		SetDlgStatus(IDC_COMBO_BATCHCHANGE, FALSE );

	AdjustTuikaTabLayout();

    return bRet;
}

// [bug 6712] WIDEAREA_F�t�B�[���h�̗L���ɂ���ă��C�A�E�g��؂�ւ���
void CAttrRoadLinkDlg::AdjustTuikaTabLayout()
{
	WINRECT *map = NULL;		// WinMgr����̃R���g���[���擾�p
	bool bIsWideAreaF = false;
	if( GetCurrentFeatureDef()->GetFieldDef( _T("ROAD_LINK"), _T("WIDEAREA_F") ) )
	{
		// �t�B�[���h�����鎞�̓R���{�{�b�N�X�ƃX�^�e�B�b�N�R���g���[���͔�\��
		::ShowWindow( GetDlgItem( IDC_COMBO_WIDEAREA ), SW_HIDE );
		::ShowWindow( GetDlgItem( IDC_STATIC_WIDEAREA ), SW_HIDE );
		map = m_winMgr.FindRect( IDC_STATIC_WIDEAREA );
		bIsWideAreaF = true;
	}else{
		// �t�B�[���h�������ꍇ�̓`�F�b�N�{�b�N�X���\��
		::ShowWindow( GetDlgItem( IDC_CHECK_WIDEAREAF ), SW_HIDE );
		map = m_winMgr.FindRect( IDC_CHECK_WIDEAREAF );
	}

	// �@�葬�x(LEGALSPEED_C)�A���s�\���(WALKABLE_C)���Ȃ��ꍇ�ɂ́ADISABLE�ɂ��Ă���
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && m_bEditable )
	{
		if( !GetCurrentFeatureDef()->GetFieldDef( _T("ROAD_LINK"), _T("LEGALSPEED_C") ) )
			SetDlgStatus( IDC_COMBO_LEGALSPEED, FALSE );

		if( !GetCurrentFeatureDef()->GetFieldDef( _T("ROAD_LINK"), _T("WALKABLE_C") ) )
			SetDlgStatus( IDC_COMBO_WALKABLE, FALSE );
	}

	// �t�B�[���h������΃`�F�b�N�{�b�N�X���A������΃X�^�e�B�b�N�R���g���[����z�u
	if( map )
	{
		map->SetID( bIsWideAreaF ? IDC_CHECK_WIDEAREAF : IDC_STATIC_WIDEAREA );
		// �Ĕz�u
		m_winMgr.InitToFitSizeFromCurrent(this);
		m_winMgr.CalcLayout(this);
		m_winMgr.SetWindowPositions(this);	
	}
}


/**
 * @fn LONG CAttrRoadLinkDlg::GetTabNum()
 * @brief ���݂̃^�u�ʒu��n���܂�
 *
 * @retval ���݂̃^�u�ʒu
 */
LONG CAttrRoadLinkDlg::GetTabNum()
{
	return m_lTabNum;
}

/**
 * @fn void CAttrRoadLinkDlg::SetTabNum(LONG lTabNum)
 * @brief �^�u�ʒu���Z�b�g���܂�
 *
 * @param lTabNum	[in]	�^�u�ʒu
 */
void CAttrRoadLinkDlg::SetTabNum(LONG lTabNum)
{
	m_lTabNum = lTabNum;
	m_iChangeType = lTabNum;
}

/**
 * @fn void CAttrRoadLinkDlg::ChangeWindowMgrMap( INT iIndex )
 * �E�B���h�E�}�l�[�W���̃}�b�v�̐؂�ւ����s��
 *
 * @param iIndex	[in]	�}�b�v�̃C���f�b�N�X�ԍ�
 */
void CAttrRoadLinkDlg::ChangeWindowMgrMap( INT iIndex )
{
	switch( iIndex ) {
	case EDITVIEW_ROAD_KIHON:		// ��{�����^�u
		m_winMgr.ChangeMgrMap(KIHONMAP);
		break;
	case EDITVIEW_ROAD_TUIKA:		// �ǉ������^�u
		m_winMgr.ChangeMgrMap( TUIKAMAP );
		break;
	case EDITVIEW_ROAD_KOTUKISEI:	// ��ʋK���^�u
		m_winMgr.ChangeMgrMap(KOTUKISEIMAP);
		break;
	case EDITVIEW_ROAD_ROSEN:		// �H���ԍ��^�u 
		m_winMgr.ChangeMgrMap(ROSENMAP);
		break;
	case EDITVIEW_ROAD_DRMA:		// DRMA�^�u 
		m_winMgr.ChangeMgrMap(DRMAMAP);
		break;
	case EDITVIEW_ROAD_PARKING:		// ���ԏ�^�u 
		m_winMgr.ChangeMgrMap(PARKINGMAP);
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
 * @fn void CAttrRoadLinkDlg::CreateControlRelation()
 * @brief �q�_�C�A���O�����ۂɍ쐬���܂��B
 *
 * ���ۂ̍쐬��ControlRel�ɔC���܂��B
 */
void CAttrRoadLinkDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// �e�E�C���h�E�n���h����ݒ肵�Ȃ���
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );
		list<CString> array;
		
		// ���H�̊�{�����^�u
		m_cControlRel.SetControl( IDC_TAB_ROADATTR,		TYPE_OTHER,		EDITVIEW_ROAD_KIHON, EDITVIEW_ROAD_PARKING );
		m_cControlRel.SetControl( IDC_COMBO_BATCHCHANGE,TYPE_COMBO,		EDITVIEW_ROAD_KIHON, EDITVIEW_ROAD_KIHON);	// [bug 9397] 
		m_cControlRel.SetControl( IDC_STATIC_BATCHCHANGE,TYPE_OTHER,	EDITVIEW_ROAD_KIHON, EDITVIEW_ROAD_KIHON);
		m_cControlRel.SetControl( IDC_COMBO_DISPCLASS,	TYPE_COMBO,		EDITVIEW_ROAD_KIHON, EDITVIEW_ROAD_KIHON, _T("ROADCLASS_C"), NULL, _T("ROAD_LINK") );
		m_cControlRel.SetControl( IDC_STATIC_DISPCLASS,	TYPE_OTHER,		EDITVIEW_ROAD_KIHON, EDITVIEW_ROAD_KIHON);
		m_cControlRel.SetControl( IDC_COMBO_NAVICLASS,	TYPE_COMBO,		EDITVIEW_ROAD_KIHON, EDITVIEW_ROAD_KIHON, _T("NAVICLASS_C"), CHECK_DISPNAVI, _T("ROAD_LINK") );
		m_cControlRel.SetControl( IDC_STATIC_NAVICLASS,	TYPE_OTHER,		EDITVIEW_ROAD_KIHON, EDITVIEW_ROAD_KIHON);
		m_cControlRel.SetControl( IDC_COMBO_MANAGER,	TYPE_COMBO,		EDITVIEW_ROAD_KIHON, EDITVIEW_ROAD_KIHON, _T("ROADMANAGER_C"), CHECK_DISPOTHER, _T("ROAD_LINK") );
		m_cControlRel.SetControl( IDC_STATIC_MANAGER,	TYPE_OTHER,		EDITVIEW_ROAD_KIHON, EDITVIEW_ROAD_KIHON);
		m_cControlRel.SetControl( IDC_COMBO_ROADWIDTH,	TYPE_COMBO,		EDITVIEW_ROAD_KIHON, EDITVIEW_ROAD_KIHON, _T("ROADWIDTH_C"), CHECK_DISPOTHER, _T("ROAD_LINK") );
		m_cControlRel.SetControl( IDC_STATIC_ROADWIDTH,	TYPE_OTHER,		EDITVIEW_ROAD_KIHON, EDITVIEW_ROAD_KIHON);
		m_cControlRel.SetControl( IDC_COMBO_LINKCLASS1,	TYPE_COMBO,		EDITVIEW_ROAD_KIHON, EDITVIEW_ROAD_KIHON,  _T("MAIN_LINKCLASS_C"), CHECK_DISPOTHER, _T("ROAD_LINK") );
		m_cControlRel.SetControl( IDC_STATIC_LINKCLASS,	TYPE_OTHER,		EDITVIEW_ROAD_KIHON, EDITVIEW_ROAD_KIHON);
		m_cControlRel.SetControl( IDC_STATIC_SOURCE,	TYPE_OTHER,		EDITVIEW_ROAD_KIHON, EDITVIEW_ROAD_PARKING);
		m_cControlRel.SetControl( IDC_EDIT_SOURCE,		TYPE_EDITSTRING,EDITVIEW_ROAD_KIHON, EDITVIEW_ROAD_PARKING, _T("SOURCE"), NULL, _T("ROAD_LINK"));

		// ���H�̒ǉ������^�u
		m_cControlRel.SetControl( IDC_CHECK_ISLANDF,		TYPE_CHECKBOX,	EDITVIEW_ROAD_TUIKA, EDITVIEW_ROAD_TUIKA, _T("ISLAND_F"), CHECK_DISPOTHER, _T("ROAD_LINK") );
		m_cControlRel.SetControl( IDC_CHECK_BYPASS,			TYPE_CHECKBOX,	EDITVIEW_ROAD_TUIKA, EDITVIEW_ROAD_TUIKA, _T("BYPASS_F"), CHECK_DISPOTHER, _T("ROAD_LINK") );
		m_cControlRel.SetControl( IDC_CHECK_CARONLYF,		TYPE_CHECKBOX,	EDITVIEW_ROAD_TUIKA, EDITVIEW_ROAD_TUIKA, _T("CARONLY_F"), CHECK_DISPOTHER, _T("ROAD_LINK") );
		m_cControlRel.SetControl( IDC_CHECK_WIDEAREAF,		TYPE_CHECKBOX,	EDITVIEW_ROAD_TUIKA, EDITVIEW_ROAD_TUIKA, _T("WIDEAREA_F"), CHECK_DISPOTHER, _T("ROAD_LINK") );
		m_cControlRel.SetControl( IDC_COMBO_WIDEAREA,		TYPE_COMBO,		EDITVIEW_ROAD_TUIKA, EDITVIEW_ROAD_TUIKA, _T("WIDEAREA_C"), CHECK_DISPOTHER, _T("ROAD_LINK") );
		m_cControlRel.SetControl( IDC_STATIC_WIDEAREA,		TYPE_OTHER,		EDITVIEW_ROAD_TUIKA, EDITVIEW_ROAD_TUIKA);
		m_cControlRel.SetControl( IDC_STATIC_UPDOWNC,		TYPE_OTHER,		EDITVIEW_ROAD_TUIKA, EDITVIEW_ROAD_TUIKA);
		m_cControlRel.SetControl( IDC_COMBO_UPDOWNC,		TYPE_COMBO,		EDITVIEW_ROAD_TUIKA, EDITVIEW_ROAD_TUIKA, _T("UPDOWNCLASS_C"), CHECK_DISPOTHER, _T("ROAD_LINK") );
		m_cControlRel.SetControl( IDC_STATIC_LANE,			TYPE_OTHER,		EDITVIEW_ROAD_TUIKA, EDITVIEW_ROAD_TUIKA);
		m_cControlRel.SetControl( IDC_EDIT_LANE,			TYPE_EDIT,		EDITVIEW_ROAD_TUIKA, EDITVIEW_ROAD_TUIKA, _T("LANE_COUNT"), CHECK_DISPOTHER, _T("ROAD_LINK") );
		m_cControlRel.SetControl( IDC_CHECK_LANE,			TYPE_CHECKBOX,	EDITVIEW_ROAD_TUIKA, EDITVIEW_ROAD_TUIKA, _T("LANEVALID_F"), CHECK_DISPOTHER, _T("ROAD_LINK") );
		m_cControlRel.SetControl( IDC_STATIC_RIGHTCHANGE,	TYPE_OTHER,		EDITVIEW_ROAD_TUIKA, EDITVIEW_ROAD_TUIKA);
		m_cControlRel.SetControl( IDC_EDIT_RIGHTCHANGE,		TYPE_EDIT,		EDITVIEW_ROAD_TUIKA, EDITVIEW_ROAD_TUIKA, _T("RIGHT_CHANGE"), CHECK_DISPOTHER, _T("ROAD_LINK") );
		m_cControlRel.SetControl( IDC_STATIC_LEFTCHANGE,	TYPE_OTHER,		EDITVIEW_ROAD_TUIKA, EDITVIEW_ROAD_TUIKA);
		m_cControlRel.SetControl( IDC_EDIT_LEFTCHANGE,		TYPE_EDIT,		EDITVIEW_ROAD_TUIKA, EDITVIEW_ROAD_TUIKA, _T("LEFT_CHANGE"), CHECK_DISPOTHER, _T("ROAD_LINK") );
		m_cControlRel.SetControl( IDC_STATIC_ALLEYCODE,		TYPE_OTHER,		EDITVIEW_ROAD_TUIKA, EDITVIEW_ROAD_TUIKA);
		m_cControlRel.SetControl( IDC_COMBO_ALLEYMAINTE,	TYPE_COMBO,		EDITVIEW_ROAD_TUIKA, EDITVIEW_ROAD_TUIKA, _T("ALLEYMAINTE_C"), CHECK_DISPOTHER, _T("ROAD_LINK") );
		m_cControlRel.SetControl( IDC_STATIC_WORKYEAR,		TYPE_OTHER,		EDITVIEW_ROAD_TUIKA, EDITVIEW_ROAD_TUIKA);
		m_cControlRel.SetControl( IDC_EDIT_WORKYEAR,		TYPE_EDIT,		EDITVIEW_ROAD_TUIKA, EDITVIEW_ROAD_TUIKA, _T("WORK_YEAR"), NULL, _T("ROAD_LINK") );

		// ���H�̌�ʋK���^�u
		m_cControlRel.SetControl( IDC_STATIC_ONEWAYGROUP,		TYPE_OTHER,		EDITVIEW_ROAD_KOTUKISEI, EDITVIEW_ROAD_KOTUKISEI);
		m_cControlRel.SetControl( IDC_STATIC_REGULERONEWAY,		TYPE_OTHER,		EDITVIEW_ROAD_KOTUKISEI, EDITVIEW_ROAD_KOTUKISEI);
		m_cControlRel.SetControl( IDC_COMBO_ONEWAY,				TYPE_COMBO,		EDITVIEW_ROAD_KOTUKISEI, EDITVIEW_ROAD_KOTUKISEI, _T("ONEWAY_C"), CHECK_DISPOTHER, _T("ROAD_LINK") );
		array.clear();
		array.push_back( _T("LINKDIR_C") );
		array.push_back( _T("START_MONTH") );
		array.push_back( _T("START_DAY") );
		array.push_back( _T("END_MONTH") );
		array.push_back( _T("END_DAY") );
		array.push_back( _T("START_HOUR") );
		array.push_back( _T("START_MIN") );
		array.push_back( _T("END_HOUR") );
		array.push_back( _T("END_MIN") );
		array.push_back( _T("DAYOFWEEK_C") );
		m_cControlRel.SetControl( IDC_GRID_ONEWAY,				TYPE_OWTIMEREG,	EDITVIEW_ROAD_KOTUKISEI, EDITVIEW_ROAD_KOTUKISEI, array, NULL, TIME_ONEWAY_TABLE_NAME, _T("LINK_ID"), IDC_COMBO_ONEWAY, FOLLOW_VALUE );
		m_cControlRel.SetControl( IDC_STATIC_NOPASSAGEGROUP,	TYPE_OTHER,		EDITVIEW_ROAD_KOTUKISEI, EDITVIEW_ROAD_KOTUKISEI);
		m_cControlRel.SetControl( IDC_STATIC_REGULERNOPASSAGE,	TYPE_OTHER,		EDITVIEW_ROAD_KOTUKISEI, EDITVIEW_ROAD_KOTUKISEI);
		m_cControlRel.SetControl( IDC_COMBO_NOPASSAGE,			TYPE_COMBO,		EDITVIEW_ROAD_KOTUKISEI, EDITVIEW_ROAD_KOTUKISEI, _T("NOPASSAGE_C"), CHECK_DISPOTHER, _T("ROAD_LINK") );
		array.clear();
		array.push_back( _T("START_MONTH") );
		array.push_back( _T("START_DAY") );
		array.push_back( _T("END_MONTH") );
		array.push_back( _T("END_DAY") );
		array.push_back( _T("START_HOUR") );
		array.push_back( _T("START_MIN") );
		array.push_back( _T("END_HOUR") );
		array.push_back( _T("END_MIN") );
		array.push_back( _T("DAYOFWEEK_C") );
		array.push_back( _T("SEASONREG_F") );
		m_cControlRel.SetControl( IDC_GRID_NOPASSAGE,			TYPE_TIMEREG,	EDITVIEW_ROAD_KOTUKISEI, EDITVIEW_ROAD_KOTUKISEI, array, NULL, TIME_NOPASSAGE_TABLE_NAME, _T("LINK_ID"), IDC_COMBO_NOPASSAGE, FOLLOW_VALUE );

		// ���H�̘H���^�u [bug 12036]
		m_cControlRel.SetControl(IDC_STATIC_ROADCODE,			TYPE_OTHER, EDITVIEW_ROAD_ROSEN, EDITVIEW_ROAD_ROSEN);
		m_cControlRel.SetControl(IDC_EDIT_ROADCODE,				TYPE_EDIT,  EDITVIEW_ROAD_ROSEN, EDITVIEW_ROAD_ROSEN, road_link::kRoadCode, CHECK_DISPOTHER, road_link::kTableName);
		m_cControlRel.SetControl(IDC_BUTTON_SEARCHROADCODE,		TYPE_OTHER, EDITVIEW_ROAD_ROSEN, EDITVIEW_ROAD_ROSEN);
		m_cControlRel.SetControl(IDC_STATIC_NAMEKANJI,			TYPE_OTHER, EDITVIEW_ROAD_ROSEN, EDITVIEW_ROAD_ROSEN);
		m_cControlRel.SetControl(IDC_EDIT_NAMEKANJI,			TYPE_OTHER, EDITVIEW_ROAD_ROSEN, EDITVIEW_ROAD_ROSEN);
		m_cControlRel.SetControl(IDC_STATIC_DISPLAYKANJI,		TYPE_OTHER, EDITVIEW_ROAD_ROSEN, EDITVIEW_ROAD_ROSEN);
		m_cControlRel.SetControl(IDC_EDIT_DISPLAYKANJI,			TYPE_OTHER, EDITVIEW_ROAD_ROSEN, EDITVIEW_ROAD_ROSEN);
		m_cControlRel.SetControl(IDC_STATIC_SHIELDNUMBER,		TYPE_OTHER, EDITVIEW_ROAD_ROSEN, EDITVIEW_ROAD_ROSEN);
		m_cControlRel.SetControl(IDC_EDIT_SHIELDNUMBER,			TYPE_OTHER, EDITVIEW_ROAD_ROSEN, EDITVIEW_ROAD_ROSEN);
		m_cControlRel.SetControl(IDC_STATIC_PREFCODE,			TYPE_OTHER, EDITVIEW_ROAD_ROSEN, EDITVIEW_ROAD_ROSEN);
		m_cControlRel.SetControl(IDC_EDIT_PREFCODE,				TYPE_EDIT,  EDITVIEW_ROAD_ROSEN, EDITVIEW_ROAD_ROSEN, road_link::kPrefCode, CHECK_DISPOTHER, road_link::kTableName);
		m_cControlRel.SetControl(IDC_BUTTON_SEARCHPREFCODE,		TYPE_OTHER, EDITVIEW_ROAD_ROSEN, EDITVIEW_ROAD_ROSEN);
		m_cControlRel.SetControl(IDC_STATIC_PREFNAME,			TYPE_OTHER, EDITVIEW_ROAD_ROSEN, EDITVIEW_ROAD_ROSEN);
		m_cControlRel.SetControl(IDC_EDIT_PREFNAME,				TYPE_OTHER, EDITVIEW_ROAD_ROSEN, EDITVIEW_ROAD_ROSEN);
		m_cControlRel.SetControl(IDC_STATIC_ROADNUMBER,			TYPE_OTHER, EDITVIEW_ROAD_ROSEN, EDITVIEW_ROAD_ROSEN);
		m_cControlRel.SetControl(IDC_EDIT_ROADNUMBER,			TYPE_EDIT,  EDITVIEW_ROAD_ROSEN, EDITVIEW_ROAD_ROSEN, road_link::kRoadNo, CHECK_DISPOTHER, road_link::kTableName);
		m_cControlRel.SetControl(IDC_STATIC_ROADNUMBER2,		TYPE_OTHER, EDITVIEW_ROAD_ROSEN, EDITVIEW_ROAD_ROSEN);
		m_cControlRel.SetControl(IDC_BUTTON_SEARCHROADNUMBER,	TYPE_OTHER, EDITVIEW_ROAD_ROSEN, EDITVIEW_ROAD_ROSEN);
		m_cControlRel.SetControl(IDC_STATIC_PREFROADNAME,		TYPE_OTHER, EDITVIEW_ROAD_ROSEN, EDITVIEW_ROAD_ROSEN);
		m_cControlRel.SetControl(IDC_EDIT_PREFROADNAME,			TYPE_OTHER, EDITVIEW_ROAD_ROSEN, EDITVIEW_ROAD_ROSEN);
		m_cControlRel.SetControl(IDC_STATIC_HIGHWAYNUMBER,		TYPE_OTHER, EDITVIEW_ROAD_ROSEN, EDITVIEW_ROAD_ROSEN);
		m_cControlRel.SetControl(IDC_EDIT_HIGHWAYNUMBER,		TYPE_OTHER, EDITVIEW_ROAD_ROSEN, EDITVIEW_ROAD_ROSEN);
		m_cControlRel.SetControl(IDC_STATIC_SECTIONCODE,		TYPE_OTHER, EDITVIEW_ROAD_ROSEN, EDITVIEW_ROAD_ROSEN);
		m_cControlRel.SetControl(IDC_EDIT_SECTIONCODE,			TYPE_EDIT,  EDITVIEW_ROAD_ROSEN, EDITVIEW_ROAD_ROSEN, road_link::kSectionCode, CHECK_DISPOTHER, road_link::kTableName);
		m_cControlRel.SetControl(IDC_BUTTON_SEARCHSECTIONCODE,	TYPE_OTHER, EDITVIEW_ROAD_ROSEN, EDITVIEW_ROAD_ROSEN);
		m_cControlRel.SetControl(IDC_STATIC_SECTIONNAME,		TYPE_OTHER, EDITVIEW_ROAD_ROSEN, EDITVIEW_ROAD_ROSEN);
		m_cControlRel.SetControl(IDC_EDIT_SECTIONNAME,			TYPE_OTHER, EDITVIEW_ROAD_ROSEN, EDITVIEW_ROAD_ROSEN);
		m_cControlRel.SetControl( IDC_STATIC_BAR,				TYPE_OTHER,	EDITVIEW_ROAD_ROSEN, EDITVIEW_ROAD_ROSEN);
		m_cControlRel.SetControl( IDC_STATIC_BAR2,				TYPE_OTHER,	EDITVIEW_ROAD_ROSEN, EDITVIEW_ROAD_ROSEN);
		m_cControlRel.SetControl( IDC_STATIC_BAR3,				TYPE_OTHER,	EDITVIEW_ROAD_ROSEN, EDITVIEW_ROAD_ROSEN);

		// ���H��DRMA�^�u
		m_cControlRel.SetControl( IDC_STATIC_KUBUNCODE,		TYPE_OTHER,		EDITVIEW_ROAD_DRMA, EDITVIEW_ROAD_DRMA);
		m_cControlRel.SetControl( IDC_COMBO_KUBUNCODE,		TYPE_COMBO,		EDITVIEW_ROAD_DRMA, EDITVIEW_ROAD_DRMA, _T("KUBUN_C"), CHECK_DISPOTHER, _T("ROAD_LINK") );
		m_cControlRel.SetControl( IDC_STATIC_LIMITSPEED,	TYPE_OTHER,		EDITVIEW_ROAD_DRMA, EDITVIEW_ROAD_DRMA);
		m_cControlRel.SetControl( IDC_COMBO_LIMITSPEED,		TYPE_COMBO,		EDITVIEW_ROAD_DRMA, EDITVIEW_ROAD_DRMA, _T("LIMITSPEED_C"), CHECK_DISPOTHER, _T("ROAD_LINK") );
		m_cControlRel.SetControl( IDC_STATIC_TRIPSPEED,		TYPE_OTHER,		EDITVIEW_ROAD_DRMA, EDITVIEW_ROAD_DRMA);
		m_cControlRel.SetControl( IDC_EDIT_TRIPSPEED,		TYPE_EDIT,		EDITVIEW_ROAD_DRMA, EDITVIEW_ROAD_DRMA, _T("TRIP_SPEED"), CHECK_DISPOTHER, _T("ROAD_LINK") );

		//���ԏ�^�u
		m_cControlRel.SetControl( IDC_STATIC_PARKINGCLASS,		TYPE_OTHER,		EDITVIEW_ROAD_PARKING, EDITVIEW_ROAD_PARKING);
		m_cControlRel.SetControl( IDC_COMBO_PARKINGCLASS,		TYPE_COMBO,		EDITVIEW_ROAD_PARKING, EDITVIEW_ROAD_PARKING, _T("PARKINGCLASS_C"), CHECK_DISPOTHER, _T("ROAD_LINK"));
		m_cControlRel.SetControl( IDC_STATIC_PARKINGLINKCLASS,	TYPE_OTHER,		EDITVIEW_ROAD_PARKING, EDITVIEW_ROAD_PARKING);
		m_cControlRel.SetControl( IDC_COMBO_PARKINGLINKCLASS,	TYPE_COMBO,		EDITVIEW_ROAD_PARKING, EDITVIEW_ROAD_PARKING, _T("PARKINGLINKCLASS_C"), CHECK_DISPOTHER, _T("ROAD_LINK"));
		m_cControlRel.SetControl( IDC_STATIC_FLOORLEVEL,		TYPE_OTHER,		EDITVIEW_ROAD_PARKING, EDITVIEW_ROAD_PARKING);
		m_cControlRel.SetControl( IDC_EDIT_FLOORLEVEL,			TYPE_EDIT,		EDITVIEW_ROAD_PARKING, EDITVIEW_ROAD_PARKING, _T("FLOOR_LEVEL"), CHECK_DISPOTHER, _T("ROAD_LINK"));
		m_cControlRel.SetControl( IDC_STATIC_MIDDLEFLOOR_LEVEL,	TYPE_OTHER,		EDITVIEW_ROAD_PARKING, EDITVIEW_ROAD_PARKING);
		m_cControlRel.SetControl( IDC_EDIT_MIDDLEFLOOR_LEVEL,	TYPE_EDIT,		EDITVIEW_ROAD_PARKING, EDITVIEW_ROAD_PARKING, _T("MIDDLEFLOOR_LEVEL"), CHECK_DISPOTHER, _T("ROAD_LINK") );

		// [bug 8087] ���ۂɃt�B�[���h�����݂��Ȃ��ꍇ�A�Ō�ɒǉ����Ă����Ȃ���΃����[�V�����`�F�b�N�ŕs�������N����
		m_cControlRel.SetControl( IDC_STATIC_LEGALSPEED,		TYPE_OTHER,		EDITVIEW_ROAD_TUIKA, EDITVIEW_ROAD_TUIKA);
		m_cControlRel.SetControl( IDC_COMBO_LEGALSPEED,			TYPE_COMBO,		EDITVIEW_ROAD_TUIKA, EDITVIEW_ROAD_TUIKA, _T("LEGALSPEED_C"), CHECK_DISPOTHER, _T("ROAD_LINK") );
		m_cControlRel.SetControl( IDC_STATIC_WALKABLE,			TYPE_OTHER,		EDITVIEW_ROAD_TUIKA, EDITVIEW_ROAD_TUIKA);
		m_cControlRel.SetControl( IDC_COMBO_WALKABLE,			TYPE_COMBO,		EDITVIEW_ROAD_TUIKA, EDITVIEW_ROAD_TUIKA, _T("WALKABLE_C"), CHECK_DISPOTHER, _T("ROAD_LINK") );
	}
}

// �J�[�\�����擾���� [bug 12036]
bool CAttrRoadLinkDlg::GetCursor(const ITablePtr& ipTable, _ICursorPtr& ipCursor, const CString& strWhereClause, const CString& strSubFields)
{
	IQueryFilterPtr ipQueryFilter = AheInitQueryFilter(nullptr, strSubFields, _T("%s"), strWhereClause);
	ipTable->Search(ipQueryFilter, VARIANT_FALSE, &ipCursor);

	return (ipCursor);
}

// �G�f�B�b�g�{�b�N�X�ɓ��͂��ꂽ���l���擾���� [bug 12036]
long CAttrRoadLinkDlg::GetWindowNumberText(int nCodeID)
{
	CWindow edit(GetDlgItem(nCodeID));
	CString strCodeVal;
	edit.GetWindowText(strCodeVal);
	return _ttol(strCodeVal);
}

// �H���������s���A�H������\������ [bug 12036]
void CAttrRoadLinkDlg::SetRoadName(int nCodeID, int nNameLabelID, int nDispLabelID, int nShieldNumberLabelID)
{
	// ���ݑI�𒆂̃t�B�[�`���擾
	IFeaturePtr	ipCurrentFeature = GetCurrentFeatureDef()->GetFeature();

	// ROAD_CODE_LIST�e�[�u���擾
	ITablePtr ipTable = AheOpenSameOwnerTable(AheGetTable(ipCurrentFeature), road_code_list::kTableName);
	if (!ipTable) return;

	// �H���ԍ��R�[�h���G�f�B�b�g�{�b�N�X����擾
	long lRoadCode = GetWindowNumberText(nCodeID);

	// �N�G�����𐶐�
	CString strWhereClause, strSubFields;
	strWhereClause.Format(_T("%s = %ld"), road_link::kRoadCode, lRoadCode);
	strSubFields.Format(_T("%s,%s,%s,%s,%s"),
		road_code_list::kNameKanji, road_code_list::kNameYomi,
		road_code_list::kDisplayKanji, road_code_list::kDisplayYomi, road_code_list::kShieldNoStr);

	// �J�[�\���擾
	_ICursorPtr ipCursor;
	if (!(GetCursor(ipTable, ipCursor, strWhereClause, strSubFields))){
		AttrMessageBox(AheLoadString(IDS_FAILTOSEARCH), AheLoadString(IDS_CAPTION), MB_OK | MB_ICONSTOP, WARNLEVEL_HIGH);
		return;
	}

	_IRowPtr ipRow;
	CComVariant vaKanjiName, vaKanjiYomi,
				vaDispKanji, vaDispYomi, vaShieldNumber;
	if (ipCursor->NextRow(&ipRow) == S_OK && ipRow)
	{
		vaKanjiName = AheGetAttribute(ipRow, road_code_list::kNameKanji);
		vaKanjiYomi = AheGetAttribute(ipRow, road_code_list::kNameYomi);
		vaDispKanji = AheGetAttribute(ipRow, road_code_list::kDisplayKanji);
		vaDispYomi = AheGetAttribute(ipRow, road_code_list::kDisplayYomi);
		vaShieldNumber = AheGetAttribute(ipRow, road_code_list::kShieldNoStr);
	}

	// �������ʂ̊e��������o�͗p�R���g���[���ɏ�������
	CString strNameText, strDispText, strNumText;

	strNameText.Format(_T("%s(%s)"), uh::str_util::ToString(vaKanjiName), uh::str_util::ToString(vaKanjiYomi));
	::SetWindowText(GetDlgItem(nNameLabelID), strNameText);

	strDispText.Format(_T("%s(%s)"), uh::str_util::ToString(vaDispKanji), uh::str_util::ToString(vaDispYomi));
	::SetWindowText(GetDlgItem(nDispLabelID), strDispText);

	strNumText.Format(_T("%s"), uh::str_util::ToString(vaShieldNumber));
	::SetWindowText(GetDlgItem(nShieldNumberLabelID), strNumText);
}

// �s���{�����������s���A�s���{�����̂�\������ [bug 12036]
void CAttrRoadLinkDlg::SetPrefName(int nCodeID, int nLabelID)
{
	// �s���{���R�[�h���G�f�B�b�g�{�b�N�X����擾
	long lPrefCode = GetWindowNumberText(nCodeID);

	// �s���{���R�[�h����s���{�����擾
	CString strPrefName;
	const auto& itr = prefCodeMap.find(lPrefCode);
	if (itr != prefCodeMap.end()) {
		strPrefName = itr->second;
	}

	// �擾�����������s���{�����iIDC_EDIT_PREFNAME�j�ɏ�������
	::SetWindowText(GetDlgItem(nLabelID), strPrefName);
}

void CAttrRoadLinkDlg::SetPrefCodeFromCityAdmin(int nCodeID)
{
	// �s���{���R�[�h���G�f�B�b�g�{�b�N�X����擾
	long lPrefCode = GetWindowNumberText(nCodeID);
	if (lPrefCode == 0) lPrefCode = GetPrefCodeFromCityAdmin();

	// �s���{���R�[�h�iIDC_EDIT_PREFCODE�j�̏�������
	::SetWindowText(GetDlgItem(nCodeID), uh::str_util::ToString(lPrefCode));
}

// ���H�����N�����݂���CITY_ADMIN��CITYCODE����PREF_CODE���擾���� [bug 12036]
long CAttrRoadLinkDlg::GetPrefCodeFromCityAdmin()
{
	// ���H�����N�����݂���CITY_ADMIN�|���S����CITYCODE�i�[�pvector
	set<long> cityCodeSet;

	// �I�����Ă��铹�H�����N��IGeometryPtr���擾
	IFeaturePtr ipRoadLinkFeature = GetCurrentFeatureDef()->GetFeature();
	IGeometryPtr ipRoadLinkGeom = AheGetShape( ipRoadLinkFeature );

	// TOC�ɓǂݍ��܂�Ă���CITY_ADMIN���C���擾(��������ꍇ�́A��ԏ�ɒǉ�����Ă��郌�C��)
	CMap cMap(m_cArcHelper.GetFocusMap());
	ILayerPtr ipCityAdminLayer = cMap.FindLayer(nullptr, nullptr, city_admin::kTableName);
	if (!ipCityAdminLayer) {
		AttrMessageBox(AheLoadString(IDS_NOCITYADMIN), AheLoadString(IDS_CAPTION), MB_OK | MB_ICONSTOP, WARNLEVEL_HIGH);
		return 0;
	}
	IFeatureClassPtr ipCityAdminFC = AheGetFeatureClass(ipCityAdminLayer);

	// ���H�����N�����݂���CITY_ADMIN�|���S����CITYCODE�擾
	IQueryFilterPtr ipQuery = AheInitQueryFilter(AheInitSpatialFilter(nullptr, ipRoadLinkGeom), city_admin::kCityCode, nullptr);

	long lFeatureCount = 0;
	ipCityAdminFC->FeatureCount(ipQuery, &lFeatureCount);
	if (lFeatureCount == 0) return 0; // ���H�����N��CITY_ADMIN��ɂȂ�

	IFeatureCursorPtr ipCursor;
	ipCityAdminFC->Search(ipQuery, VARIANT_TRUE, &ipCursor);

	long lCityCodeIdx = 0;
	IFieldsPtr ipFields;
	ipCursor->get_Fields(&ipFields);
	ipFields->FindField(CComBSTR(city_admin::kCityCode), &lCityCodeIdx);

	IFeaturePtr ipCityAdminFeature;
	while (ipCursor->NextFeature(&ipCityAdminFeature) == S_OK && ipCityAdminFeature)
	{
		CComVariant vaCityCode;
		ipCityAdminFeature->get_Value(lCityCodeIdx, &vaCityCode);

		CString strCityCode;
		strCityCode.Format(_T("%s"), uh::str_util::ToString(vaCityCode));
		long lPrefCode = _ttol(strCityCode.Mid(0, 2)); // ���߂̓񌅂����擾

		cityCodeSet.insert(lPrefCode);
	}

	// ���H�����N�������ɂ���
	if (cityCodeSet.size() > 1) {
	AttrMessageBox(AheLoadString(IDS_BEYONDPREFBORDER), AheLoadString(IDS_CAPTION), MB_OK, WARNLEVEL_HIGH);
	return 0;
	}

	if (cityCodeSet.size() == 1) return *cityCodeSet.begin();

	return 0;
}

// �s���{�������̌������s���A�s���{�������̂�\������ [bug 12036]
void CAttrRoadLinkDlg::SetPrefRoadName(int nPrefCodeID, int nRoadNumberID, int nPrefNameLabelID, int nPrefRoadLabelID)
{
	// ���ݑI�𒆂̃t�B�[�`���擾
	IFeaturePtr	ipCurrentFeature = GetCurrentFeatureDef()->GetFeature();

	// PREF_ROAD_LIST�e�[�u���擾
	ITablePtr ipTable = AheOpenSameOwnerTable(AheGetTable(ipCurrentFeature), pref_road_list::kTableName);
	if (!ipTable) return;

	// ���H��ʂ��擾
	CFieldDef *pFieldDef = GetCurrentFeatureDef()->GetFieldDef(ROAD_TABLE_NAME, road_link::kRoadClass);
	long lRoadClass = pFieldDef->m_vaValue.lVal;

	// �s���{���R�[�h���G�f�B�b�g�{�b�N�X����擾
	long lPrefCode = GetWindowNumberText(nPrefCodeID);

	// ���H�ԍ����G�f�B�b�g�{�b�N�X����擾
	long lRoadNo = GetWindowNumberText(nRoadNumberID);

	// �N�G�����𐶐�
	CString strWhereClause, strSubFields;
	strWhereClause.Format(_T("%s = %ld AND %s = %ld AND %s = %ld"),
		road_link::kPrefCode, lPrefCode, road_link::kRoadNo, lRoadNo, road_link::kRoadClass, lRoadClass);
	strSubFields.Format(_T("%s,%s"), pref_road_list::kNameKanji, pref_road_list::kNameYomi);

	// �J�[�\���擾
	_ICursorPtr ipCursor;
	if (!(GetCursor(ipTable, ipCursor, strWhereClause, strSubFields))) {
		AttrMessageBox(AheLoadString(IDS_FAILTOSEARCH), AheLoadString(IDS_CAPTION), MB_OK | MB_ICONSTOP, WARNLEVEL_HIGH);
		return;
	}

	_IRowPtr ipRow;
	CComVariant vaKanjiName, vaKanjiYomi;
	if (ipCursor->NextRow(&ipRow) == S_OK && ipRow)
	{
		vaKanjiName = AheGetAttribute(ipRow, pref_road_list::kNameKanji);
		vaKanjiYomi = AheGetAttribute(ipRow, pref_road_list::kNameYomi);
	}

	// �s���{���R�[�h����s���{�����擾
	CString strPrefName;
	const auto& itr = prefCodeMap.find(lPrefCode);
	if (itr != prefCodeMap.end()) {
		strPrefName = itr->second;
	}

	// �s���{�����iIDC_EDIT_PREFNAME�j�̏�������
	::SetWindowText(GetDlgItem(nPrefNameLabelID), strPrefName);

	// ���H���́iIDC_EDIT_PREFROADNAME�j�̏�������
	CString strText;
	strText.Format(_T("%s(%s)"), uh::str_util::ToString(vaKanjiName), uh::str_util::ToString(vaKanjiYomi));
	::SetWindowText(GetDlgItem(nPrefRoadLabelID), strText);
}

// ��ԕʏ̃R�[�h�������s���A��ԕʏ̖���\������ [bug 12036]
void CAttrRoadLinkDlg::SetSectionName(int nCodeID, int nLabelID)
{
	// ���ݑI�𒆂̃t�B�[�`���擾
	IFeaturePtr	ipCurrentFeature = GetCurrentFeatureDef()->GetFeature();

	// SECTION_CODE_LIST�e�[�u���擾
	ITablePtr ipTable = AheOpenSameOwnerTable(AheGetTable(ipCurrentFeature), section_code_list::kTableName);
	if (!ipTable) return;

	// ��ԕʏ̃R�[�h���G�f�B�b�g�{�b�N�X����擾
	long lSectionCode = GetWindowNumberText(nCodeID);

	// �N�G�����𐶐�
	CString strWhereClause, strSubFields;
	strWhereClause.Format(_T("%s = %ld"), road_link::kSectionCode, lSectionCode);
	strSubFields.Format(_T("%s,%s"), section_code_list::kNameKanji, section_code_list::kNameYomi);

	// �J�[�\���擾
	_ICursorPtr ipCursor;
	if (!(GetCursor(ipTable, ipCursor, strWhereClause, strSubFields))) {
		AttrMessageBox(AheLoadString(IDS_FAILTOSEARCH), AheLoadString(IDS_CAPTION), MB_OK | MB_ICONSTOP, WARNLEVEL_HIGH);
		return;
	}

	_IRowPtr ipRow;
	CComVariant vaKanjiName, vaKanjiYomi;

	while (ipCursor->NextRow(&ipRow) == S_OK && ipRow)
	{
		vaKanjiName = AheGetAttribute(ipRow, section_code_list::kNameKanji);
		vaKanjiYomi = AheGetAttribute(ipRow, section_code_list::kNameYomi);
	}

	// �������ʂ̕��������ԕʏ́iIDC_EDIT_SECTIONNAME�j�ɏ�������
	CString strText;
	strText.Format(_T("%s(%s)"), uh::str_util::ToString(vaKanjiName), uh::str_util::ToString(vaKanjiYomi));
	::SetWindowText(GetDlgItem(nLabelID), strText);
}

// �G�f�B�b�g�{�b�N�X�ɓ��͂��ꂽ���l�̃`�F�b�N���s�� [bug 12036]
bool CAttrRoadLinkDlg::CheckNumVal(int nCodeID)
{
	// ���l���G�f�B�b�g�{�b�N�X����擾
	CWindow edit(GetDlgItem(nCodeID));
	CString strNumVal;
	edit.GetWindowText(strNumVal);
	long lNumVal = _ttol(strNumVal);

	switch (nCodeID)
	{
	case IDC_EDIT_ROADCODE:
		if (!CheckRoadCodeSectionCode(lNumVal)) {
			AttrMessageBox(AheLoadString(IDS_OUTOFROADCODERANGE), AheLoadString(IDS_CAPTION), MB_OK, WARNLEVEL_HIGH);
			return false;
		}
		break;
	case IDC_EDIT_PREFCODE:
		if (!CheckPrefCode(lNumVal)) {
			AttrMessageBox(AheLoadString(IDS_OUTOFPREFCODERANGE), AheLoadString(IDS_CAPTION), MB_OK, WARNLEVEL_HIGH);
			return false;
		}
		break;
	case IDC_EDIT_ROADNUMBER:
		if (!CheckRoadNo(lNumVal)) {
			AttrMessageBox(AheLoadString(IDS_OUTOFROADNORANGE), AheLoadString(IDS_CAPTION), MB_OK, WARNLEVEL_HIGH);
			return false;
		}
		break;
	case IDC_EDIT_SECTIONCODE:
		if (!CheckRoadCodeSectionCode(lNumVal)) {
			AttrMessageBox(AheLoadString(IDS_OUTOFSECTIONCODERANGE), AheLoadString(IDS_CAPTION), MB_OK, WARNLEVEL_HIGH);
			return false;
		}
		break;
	}
	return true;
}

// �H�����̃R�[�h�܂��͋�ԕʏ̃R�[�h�̒l���`�F�b�N [bug 12036]
bool CAttrRoadLinkDlg::CheckRoadCodeSectionCode(long lNumVal)
{
	// "0"��OK
	if (lNumVal == 0)
		return true;

	// ���͔͈͂̃`�F�b�N
	return (1000 <= lNumVal && lNumVal <= 999999);
}

// �s���{���R�[�h�̒l���`�F�b�N [bug 12036]
bool CAttrRoadLinkDlg::CheckPrefCode(long lPrefCode)
{
	// ���͔͈͂̃`�F�b�N
	return (0 <= lPrefCode && lPrefCode <= 47);
}

// ���H�ԍ��̒l���`�F�b�N [bug 12036]
bool CAttrRoadLinkDlg::CheckRoadNo(long lRoadNo)
{
	// ���H�ԍ��̃`�F�b�N
	return (0 <= lRoadNo && lRoadNo <= 9999);
}

/**
 * @fn void CAttrRoadLinkDlg::SetForcusAfterTabChange()
 * @brief ���݂̃^�u�ʒu�ɐݒ肵�܂�
 */
void CAttrRoadLinkDlg::SetForcusAfterTabChange()
{
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() )
	{
		if( m_lTabNum == 0 )
			::SetFocus( GetDlgItem(IDC_COMBO_DISPCLASS) );
		else if( m_lTabNum == 1) 
			::SetFocus( GetDlgItem(IDC_CHECK_ISLANDF) );
		else if( m_lTabNum == 2) 
		{
			SetDlgStatus(IDC_COMBO_ONEWAY, FALSE );
			SetDlgStatus(IDC_COMBO_NOPASSAGE, FALSE );
			::SetFocus( GetDlgItem(IDC_EDIT_SOURCE) );
		}
		else if( m_lTabNum == 3) 
			::SetFocus( GetDlgItem(IDC_EDIT_ROADCODE) );
		else if( m_lTabNum == 4) 
			::SetFocus( GetDlgItem(IDC_COMBO_KUBUNCODE) );
		else if( m_lTabNum == 5) 
			::SetFocus( GetDlgItem(IDC_COMBO_PARKINGCLASS) );
	}
	else
		::SetFocus( GetDlgItem(IDC_EDIT_SOURCE) );
}

/**
 * @fn BOOL CAttrRoadLinkDlg::ErrorCheck()
 * @brief �_�C�A���O��������Ƃ��ɓ��H�����N�̃G���[�`�F�b�N���s��
 *
 * @return �G���[�łȂ������� TRUE�A�G���[�������� FALSE
 */
BOOL CAttrRoadLinkDlg::ErrorCheck()
{
	for( list<CFeatureDef>::iterator it = m_pFeatureDefList->begin(); it != m_pFeatureDefList->end(); it++ )
	{
		if( !m_ipRule )
			m_ipRule = GetRule();

		CString strTableName = it->GetTableName();

		if( strTableName == ROAD_TABLE_NAME )
		{
			// �t�B�[�`���ƃI�u�W�F�N�gID�擾
			IFeaturePtr ipFeature(it->GetFeature());
			LONG lOID = 0;
			ipFeature->get_OID( &lOID );

			// ���ݕҏW���̒l���擾
			CFieldDef* pFieldDef;
			// ��ƔN�x�̃`�F�b�N
			pFieldDef = GetCurrentFeatureDef()->GetFieldDef( ROAD_TABLE_NAME, _T("WORK_YEAR") );
			CComVariant vaWorkYearValue = pFieldDef->m_vaValue;
			long lWorkYear = vaWorkYearValue.lVal;
			if( !(0 <= lWorkYear && lWorkYear < 100000) )
			{
				CString strMessage;
				strMessage.Format( AheLoadString( IDS_OUTOFWORKYEARRANGE ),lOID);
				AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK, NULL );
				return FALSE;					
			}

			// VICS,�C���^�[�i�rVICS�t���O��1�̏ꍇ�ɁA�o�H��ʂ��o�H�O���H�ł������ꍇ�ɂ́A�x�����o��
			pFieldDef = it->GetFieldDef( ROAD_TABLE_NAME, _T("VICS_LQRF") );
			long lVICSFlag = pFieldDef->m_vaValue.lVal;
			pFieldDef = it->GetFieldDef( ROAD_TABLE_NAME, _T("INTERNAVI_LQRF") );
			long lInterNaviFlag = pFieldDef->m_vaValue.lVal;
			pFieldDef = it->GetFieldDef( ROAD_TABLE_NAME, _T("NAVICLASS_C") );
			long lNaviClass = pFieldDef->m_vaValue.lVal;

			if( lNaviClass == 0 && (lInterNaviFlag == 1 || lVICSFlag == 1) )
			{
				CString strMessage;
				strMessage.Format( AheLoadString( IDS_VICSERROR ), lOID);
				INT iRet = AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_YESNO, NULL );

				if( iRet == IDNO )
					return FALSE;
			}

			// ���H��ʂɉ����ēs���{���R�[�h�A���H�ԍ��̓��͂��`�F�b�N
			CString strPrefRoadMsg;
			if (!(CheckPrefCodeRoadNo(strPrefRoadMsg, *it))){
				CString strError;
				strError.Format(AheLoadString(IDS_ERRORANDWORNING), lOID);
				strError += strPrefRoadMsg;
				AttrMessageBox(strError, AheLoadString(IDS_CAPTION), MB_OK | MB_ICONSTOP, NULL);
				return FALSE;
			}

			// ���H�����N�����[�V�����V�b�v�̃`�F�b�N
			CString strMessage;
			LONG lCheckRoadLinkRelation = CheckRoadLinkRelation( &strMessage, &(*it) );

			if( lCheckRoadLinkRelation == 2 )
			{
				strMessage += AheLoadString( IDS_RELATIONERROR );
				AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK, NULL );
				return FALSE;
			}
			else if( lCheckRoadLinkRelation == 1 )
			{
				strMessage += AheLoadString( IDS_RELATIONWORNING );
				INT iRet = AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_YESNO, NULL );
			
				if( iRet == IDNO )
					return FALSE;
			}

			// �ԍ��^�u�ɊY�����̂��Ȃ����ڂ͂Ȃ����`�F�b�N(�ۑ��͉\)
			CString strNonExistsNameMsg;
			strNonExistsNameMsg.Empty();
			if (!(CheckNameExists(strNonExistsNameMsg, *it))){
				CString strWarning;
				strWarning.Format(AheLoadString(IDS_NONAMEWARNING), lOID);
				strWarning += strNonExistsNameMsg;
				INT iRet = AttrMessageBox(strWarning, AheLoadString(IDS_CAPTION), MB_YESNO, NULL);

				if (iRet == IDNO)
					return FALSE;
			}
		}
	}

	return TRUE;
}

// ���H��ʂɉ����ēs���{���R�[�h�A���H�ԍ��̓��͂ɖ�肪�Ȃ������`�F�b�N���� [bug 12036]
bool CAttrRoadLinkDlg::CheckPrefCodeRoadNo(CString& strMsg, CFeatureDef& pFeatureDef)
{
	// �s���{���R�[�h�����͂ł��铹�H��ʂ̃R�[�h�lset
	static const std::set<long> okPrefCodeRoadClassSet = boost::assign::list_of
	(road_link::road_class::kPrefectureMain)
	(road_link::road_class::kPrefectureNormal)
	(road_link::road_class::kTollCountry)
	(road_link::road_class::kTollPrefectureMain)
	(road_link::road_class::kTollPrefectureNormal)
	;

	// ���H�ԍ������͂ł��铹�H��ʂ̃R�[�h�lset
	static const std::set<long> okRoadNoRoadClassSet = boost::assign::list_of
	(road_link::road_class::kCountry)
	(road_link::road_class::kPrefectureMain)
	(road_link::road_class::kPrefectureNormal)
	(road_link::road_class::kTollCountry)
	(road_link::road_class::kTollPrefectureMain)
	(road_link::road_class::kTollPrefectureNormal)
	;

	// ���H��ʂ��擾
	CFieldDef* pFieldDef = pFeatureDef.GetFieldDef(ROAD_TABLE_NAME, road_link::kRoadClass);
	long lRoadClass = pFieldDef->m_vaValue.lVal;

	// �R�[�h�l���擾
	pFieldDef = pFeatureDef.GetFieldDef(road_link::kTableName, road_link::kPrefCode);
	// ROAD_LINK�̃t�B�[���h��PREF_CODE����`����Ă��Ȃ��ꍇ�ɗ����Ȃ��悤��
	if(!pFieldDef) return true;
	long lPrefCode = pFieldDef->m_vaValue.lVal;
	pFieldDef = pFeatureDef.GetFieldDef(road_link::kTableName, road_link::kRoadNo);
	long lRoadNo = pFieldDef->m_vaValue.lVal;

	// ���H�ԍ��A�s���{���R�[�h������0�̏ꍇ��return
	if (lPrefCode == 0 && lRoadNo == 0)
		return true;

	// ���H��ʂ�(1,2,4,103,104,106)�ȊO�̂Ƃ����H�ԍ������͂���Ă��Ȃ���
	auto& itrRoadNo = okRoadNoRoadClassSet.find(lRoadClass);
	if (itrRoadNo == okRoadNoRoadClassSet.end() && lRoadNo != 0) {
		strMsg += AheLoadString(IDS_CANTSPECIFIEDROADNO);
	}

	// ���H��ʂ�(2,4,103,104,106)�ȊO�̂Ƃ��s���{���R�[�h�����͂���Ă��Ȃ���
	auto& itrPrefCode = okPrefCodeRoadClassSet.find(lRoadClass);
	if (itrPrefCode == okPrefCodeRoadClassSet.end() && lPrefCode != 0) {
		strMsg += AheLoadString(IDS_CANTSPECIFIEDPREFCODE);
	}

	return strMsg.IsEmpty();
}

/**
 * @fn LONG CAttrRoadLinkDlg::CheckRoadLinkRelation(CString *strMessage, CFeatureDef *pFeatureDef)
 * @brief ���H�����N�����[�V�����ŃG���[���Ȃ����ǂ����`�F�b�N����
 *
 * @param strMessage		[out]	�G���[���b�Z�[�W�̓��e
 *
 * @return	0:�G���[�Ȃ��A�`�F�b�N����K�v���Ȃ������Ƃ�
 * @return	1:���[�j���O������Ƃ�
 * @return	2:�G���[������Ƃ�
 */
LONG CAttrRoadLinkDlg::CheckRoadLinkRelation(CString *strMessage, CFeatureDef *pFeatureDef)
{
	LONG lCheckResult = 0;
	BOOL bWarning = FALSE;
	BOOL bError	  = FALSE;

	CFieldDef *pFieldDef;
	CComVariant vaRoadClassValue;
	pFieldDef = pFeatureDef->GetFieldDef( ROAD_TABLE_NAME, _T("ROADCLASS_C") );
	// �_�C�A���O������ɁA�I�����Ȃ����ăt�B�[�`����I�������Ƃ��ɁA�_�C�A���O�͕����Ă��Ȃ��̂ŁA
	// GetCurrentFeatureDef()�́A�܂��ݒ肳��Ă��Ȃ��H�Ƃ肠�����ApFieldDef��NULL�̂Ƃ��ɁA�`�F�b�N
	// ������K�v���Ȃ��̂ŁA���}���u�I�ɁAreturn �ŕԂ��ďI�����Ă����B[Bug2430]
	if( !pFieldDef )
		return 0;
	vaRoadClassValue = pFieldDef->m_vaValue;
	LONG lRoadClass = vaRoadClassValue.lVal;
	
	DOMAINMAP* mapDomain = pFieldDef->GetDomain();
	DOMAINMAP::iterator imap;
	CString strRoadClass;

	if( mapDomain )
	{
		for( imap = mapDomain->begin(); imap != mapDomain->end(); imap++ ) {
			if( imap->code.lVal == lRoadClass )
			{
				strRoadClass = imap->code_discription;
				break;
			}
		}

		LONG lOID = 0;
		IFeaturePtr ipFeature = pFeatureDef->GetFeature();
		ipFeature->get_OID( &lOID );

		(*strMessage).Format( AheLoadString( IDS_ERRORANDWORNING ), lOID);

		CString temp;
		temp.Format( AheLoadString( IDS_WHENROADCLASS ), strRoadClass );
		(*strMessage) += temp;


		CControlDef* def = NULL;
		for( CControlRel::iterator i = m_cControlRel.begin(); i != m_cControlRel.end(); i++ )
		{
			def = &(*i);

			if( def->m_uiCheckType == CHECK_DISPNAVI ||  def->m_uiCheckType == CHECK_DISPOTHER)
			{
				if( def->m_uiCheckType & CHECK_DISPNAVI )
				{
					sindyeDispNaviRelation nCheck;
					LONG lNaviClass_c;
					CComVariant vaNaviClassValue;

					pFieldDef = pFeatureDef->GetFieldDef( ROAD_TABLE_NAME, _T("NAVICLASS_C") );
					vaNaviClassValue = pFieldDef->m_vaValue;
					lNaviClass_c = vaNaviClassValue.lVal;

					mapDomain = pFieldDef->GetDomain();
					CString strNaviClass;
					for( imap = mapDomain->begin(); imap != mapDomain->end(); imap++ ) {
						if( imap->code.lVal == lNaviClass_c )
						{
							strNaviClass = imap->code_discription;
							break;
						}
					}

					m_ipRule->GetDispNaviRule( lRoadClass, lNaviClass_c, &nCheck );
					if( nCheck == sindyeDispNaviError )
					{
						def->m_bErrored = TRUE;
						CString temp;
						temp.Format( AheLoadString( IDS_NAVICLASSISERROR ), strNaviClass );
						(*strMessage) += temp;
						bError = TRUE;
					}
					else if ( nCheck == sindyeDispNaviWarn )
					{
						def->m_bErrored = TRUE;
						CString temp;
						temp.Format( AheLoadString( IDS_NAVICLASSISWORNING ), strNaviClass );
						(*strMessage) += temp;
						bWarning = TRUE;
					}
					else
						def->m_bErrored = FALSE;
				}
				else if( def->m_uiCheckType & CHECK_DISPOTHER ) 
				{
					sindyeDispOtherRelation nDispOtherRel;
					CString temp111 = def->GetFieldName();
					m_ipRule->GetDispOtherRule( lRoadClass, CComBSTR(def->GetFieldName()), &nDispOtherRel );

					// [Bug3078]�̃o�O�Ή��ASiNDYLib�Ή���SiNDYRule���g���ꍇ�ɁAsindyeDispOtherRelation�̏����l��
					// sindyeDispOtherNoDef�œo�^����Ă��܂��Ă���̂ŁA������WorkEditor�ŕۑ�����Ă��Ȃ��i��`����Ă��Ȃ��j
					// ���[���͂��ׂĖ���`�ƂȂ��Ă��܂��̂ŁA�����ɖ���`�ɂ��Ă����A����������ƒ����ɂ́AWorkEditor��
					// SiNDYRule�𒼂������Ȃ��B
					if( nDispOtherRel == sindyeDispOtherNoDef )
						nDispOtherRel = sindyeDispOtherNoRes;

					LONG lDispClass_c;
					CComVariant vaDispClassValue;

					pFieldDef = pFeatureDef->GetFieldDef( ROAD_TABLE_NAME, def->GetFieldName() );
					if( pFieldDef == NULL ) break;
					vaDispClassValue = pFieldDef->m_vaValue;
					lDispClass_c = vaDispClassValue.lVal;

					mapDomain = pFieldDef->GetDomain();
					CString strDispClass;
					BOOL bIsDomain = FALSE;
					if( mapDomain )
					{
						bIsDomain = TRUE;
						for( imap = mapDomain->begin(); imap != mapDomain->end(); imap++ ) {
							if( imap->code.lVal == lDispClass_c )
							{
								strDispClass = imap->code_discription;
								break;
							}
						}
					}
					
					// �������̎�
					if( nDispOtherRel == sindyeDispOtherNoRes ) {
						pFieldDef = pFeatureDef->GetFieldDef( ROAD_TABLE_NAME, def->GetFieldName() );
						vaDispClassValue = pFieldDef->m_vaValue;
						lDispClass_c = vaDispClassValue.lVal;
						if( lDispClass_c == 0 )
							def->m_bErrored = FALSE;
						else
						{
							def->m_bErrored = TRUE;
							CString temp;
							if( bIsDomain )
								temp.Format( AheLoadString( IDS_ERROR1 ), pFieldDef->m_strFieldAliasName ,strDispClass );
							else
								temp.Format( AheLoadString( IDS_ERROR2 ), pFieldDef->m_strFieldAliasName ,lDispClass_c );
							(*strMessage) += temp;
							bError = TRUE;

						}
					}
					// ���ʂ͖�����
					else if( nDispOtherRel == sindyeDispOtherUsuallyNoRes )
					{
						pFieldDef = pFeatureDef->GetFieldDef( ROAD_TABLE_NAME, def->GetFieldName() );
						vaDispClassValue = pFieldDef->m_vaValue;
						lDispClass_c = vaDispClassValue.lVal;
						if( lDispClass_c == 0 )
							def->m_bErrored = FALSE;
						else
						{
							def->m_bErrored = TRUE;
							CString temp;
							if( bIsDomain )
								temp.Format( AheLoadString( IDS_WORNING1 ), pFieldDef->m_strFieldAliasName ,strDispClass );
							else
								temp.Format( AheLoadString( IDS_WORNING2 ), pFieldDef->m_strFieldAliasName ,lDispClass_c );
							(*strMessage) += temp;
							bWarning = TRUE;
						}

					}
					// �������ȊO�̎�
					else if( nDispOtherRel == sindyeDispOtherExceptNoRes ) 
					{
						pFieldDef = pFeatureDef->GetFieldDef( ROAD_TABLE_NAME, def->GetFieldName() );
						vaDispClassValue = pFieldDef->m_vaValue;
						lDispClass_c = vaDispClassValue.lVal;
						if( lDispClass_c == 0 )
						{
							def->m_bErrored = TRUE;
							CString temp;
							if( bIsDomain )
								temp.Format( AheLoadString( IDS_ERROR1 ), pFieldDef->m_strFieldAliasName ,strDispClass );
							else
								temp.Format( AheLoadString( IDS_ERROR2 ), pFieldDef->m_strFieldAliasName ,lDispClass_c );
							(*strMessage) += temp;
							bError = TRUE;
						}else
							def->m_bErrored = FALSE;
					}
					else
						def->m_bErrored = FALSE;

				}	
			}
		}
	}
	if( bError )
		lCheckResult = 2;
	else
	{
		if( bWarning )
			lCheckResult = 1;
	}

	// �^�u�̃n�C���C�g����
	TabHighLight();

	return lCheckResult;
}

// �ԍ��^�u�ɓ��͂��ꂽ�e�R�[�h�l�ɊY�����̂����݂��邩�`�F�b�N���� [bug 12036]
bool CAttrRoadLinkDlg::CheckNameExists(CString& strMsg, CFeatureDef& pFeatureDef)
{
	// �H�����̃R�[�h�Ɋւ���`�F�b�N
	CheckRoadCodeNameExists(strMsg, pFeatureDef);

	// ���H���̂̃`�F�b�N
	CheckPrefRoadNameExists(strMsg, pFeatureDef);

	// ��ԕʏ̂̃`�F�b�N
	CheckSectionNameExists(strMsg, pFeatureDef);

	return strMsg.IsEmpty();
}

// ���͂��ꂽ�H�����̃R�[�h�ɊY�����̂����݂��邩�`�F�b�N���� [bug 12036]
void CAttrRoadLinkDlg::CheckRoadCodeNameExists(CString& strMsg, CFeatureDef& pFeatureDef)
{
	// ���ݑI�𒆂̃t�B�[�`���擾
	IFeaturePtr	ipCurrentFeature = GetCurrentFeatureDef()->GetFeature();

	// ROAD_CODE_LIST�e�[�u���擾
	ITablePtr ipTable = AheOpenSameOwnerTable(AheGetTable(ipCurrentFeature), road_code_list::kTableName);
	if (!ipTable) return;


	// �H���ԍ��R�[�h���擾
	CFieldDef* pFieldDef = pFeatureDef.GetFieldDef(road_link::kTableName, road_link::kRoadCode);
	if (!(pFieldDef->m_bChanged)) return; //!< �ҏW����Ă��Ȃ����return
	long lRoadCode = pFieldDef->m_vaValue.lVal;

	// 0�͋��l
	if (lRoadCode == 0) return;

	// �H���ԍ��R�[�h�̒l���`�F�b�N
	if (!CheckRoadCodeSectionCode(lRoadCode)) {
		AttrMessageBox(AheLoadString(IDS_OUTOFROADCODERANGE), AheLoadString(IDS_CAPTION), MB_OK, WARNLEVEL_HIGH);
		strMsg += uh::str_util::format(AheLoadString(IDS_NONAME), AheLoadString(IDS_NAMEKANJI));
		strMsg += uh::str_util::format(AheLoadString(IDS_NONAME), AheLoadString(IDS_DISPKANJI));
		strMsg += uh::str_util::format(AheLoadString(IDS_NONAME), AheLoadString(IDS_SHIELDNUMBER));
		return;
	}

	// �N�G�����𐶐�
	CString strWhereClause, strSubFields;
	strWhereClause.Format(_T("%s = %ld"), road_link::kRoadCode, lRoadCode);
	strSubFields.Format(_T("%s,%s,%s,%s,%s"),
		road_code_list::kNameKanji, road_code_list::kNameYomi,
		road_code_list::kDisplayKanji, road_code_list::kDisplayYomi, road_code_list::kShieldNoStr);

	// �J�[�\���擾
	_ICursorPtr ipCursor;
	if (!(GetCursor(ipTable, ipCursor, strWhereClause, strSubFields))) {
		strMsg += AheLoadString(IDS_FAILTOSEARCH);
		return;
	}

	_IRowPtr ipRow;
	CComVariant vaKanjiName, vaKanjiYomi,
		vaDispKanji, vaDispYomi, vaShieldNumber;
	if (ipCursor->NextRow(&ipRow) == S_OK && ipRow)
	{
		vaKanjiName = AheGetAttribute(ipRow, road_code_list::kNameKanji);
		vaKanjiYomi = AheGetAttribute(ipRow, road_code_list::kNameYomi);
		vaDispKanji = AheGetAttribute(ipRow, road_code_list::kDisplayKanji);
		vaDispYomi = AheGetAttribute(ipRow, road_code_list::kDisplayYomi);
		vaShieldNumber = AheGetAttribute(ipRow, road_code_list::kShieldNoStr);
	}
	
	// �e�Y�����̂������ꍇ�͌x������t�^
	if (uh::variant_util::isNullOrEmpty(vaKanjiName)) {
		strMsg += uh::str_util::format(AheLoadString(IDS_NONAME), AheLoadString(IDS_NAMEKANJI));
	}
	if (uh::variant_util::isNullOrEmpty(vaDispKanji)) {
		strMsg += uh::str_util::format(AheLoadString(IDS_NONAME), AheLoadString(IDS_DISPKANJI));
	}
	if (uh::variant_util::isNullOrEmpty(vaShieldNumber)) {
		strMsg += uh::str_util::format(AheLoadString(IDS_NONAME), AheLoadString(IDS_SHIELDNUMBER));
	}
}

// ���͂��ꂽ�s���{���R�[�h�A���H�ԍ��ɊY�����̂����݂��邩�`�F�b�N���� [bug 12036]
void CAttrRoadLinkDlg::CheckPrefRoadNameExists(CString& strMsg, CFeatureDef& pFeatureDef)
{
	// ���ݑI�𒆂̃t�B�[�`���擾
	IFeaturePtr	ipCurrentFeature = GetCurrentFeatureDef()->GetFeature();

	// PREF_ROAD_LIST�e�[�u���擾
	ITablePtr ipTable = AheOpenSameOwnerTable(AheGetTable(ipCurrentFeature), pref_road_list::kTableName);
	if (!ipTable) return;

	// ���H��ʂ��擾
	CFieldDef* pFieldDef = pFeatureDef.GetFieldDef(ROAD_TABLE_NAME, road_link::kRoadClass);
	long lRoadClass = pFieldDef->m_vaValue.lVal;

	// �s���{���R�[�h���擾
	pFieldDef = pFeatureDef.GetFieldDef(road_link::kTableName, road_link::kPrefCode);
	// ROAD_LINK�̃t�B�[���h��PREF_CODE����`����Ă��Ȃ��ꍇ�ɗ����Ȃ��悤��
	if (!pFieldDef) return;
	long lPrefCode = pFieldDef->m_vaValue.lVal;

	// �s���{���R�[�h���ҏW����Ă��邩
	if (pFieldDef->m_bChanged) {
		// �͈͊O�̓s���{���R�[�h�l�����͂���Ă����ꍇ�͌x���\��
		if (!CheckPrefCode(lPrefCode)) {
			AttrMessageBox(AheLoadString(IDS_OUTOFPREFCODERANGE), AheLoadString(IDS_CAPTION), MB_OK, WARNLEVEL_HIGH);
			strMsg += uh::str_util::format(AheLoadString(IDS_NONAME), AheLoadString(IDS_PREFNAME));
		}
		else {
			// �s���{�����̂������ꍇ�͌x������t�^
			const auto& itr = prefCodeMap.find(lPrefCode);
			if (itr == prefCodeMap.end()) {
				strMsg += uh::str_util::format(AheLoadString(IDS_NONAME), AheLoadString(IDS_PREFNAME));
			}
		}
	}

	// ���H�ԍ����擾
	pFieldDef = pFeatureDef.GetFieldDef(road_link::kTableName, road_link::kRoadNo);
	if (!(pFieldDef->m_bChanged)) return; //!< �ҏW����Ă��Ȃ����return
	long lRoadNo = pFieldDef->m_vaValue.lVal;

	// �͈͊O�̓��H�ԍ������͂���Ă����ꍇ�͌x���\��
	if (!CheckRoadNo(lRoadNo)) {
		AttrMessageBox(AheLoadString(IDS_OUTOFROADNORANGE), AheLoadString(IDS_CAPTION), MB_OK, WARNLEVEL_HIGH);
		strMsg += uh::str_util::format(AheLoadString(IDS_NONAME), AheLoadString(IDS_PREFROADNAME));
		return;
	}

	// �N�G�����𐶐�
	CString strWhereClause, strSubFields;
	strWhereClause.Format(_T("%s = %ld AND %s = %ld AND %s = %ld"),
		road_link::kPrefCode, lPrefCode, road_link::kRoadNo, lRoadNo, road_link::kRoadClass, lRoadClass);
	strSubFields.Format(_T("%s,%s"), pref_road_list::kNameKanji, pref_road_list::kNameYomi);

	// �J�[�\���擾
	_ICursorPtr ipCursor;
	if (!(GetCursor(ipTable, ipCursor, strWhereClause, strSubFields))) {
		strMsg += AheLoadString(IDS_FAILTOSEARCHROADNO);
		return;
	}

	_IRowPtr ipRow;
	CComVariant vaKanjiName, vaKanjiYomi;
	if (ipCursor->NextRow(&ipRow) == S_OK && ipRow)
	{
		vaKanjiName = AheGetAttribute(ipRow, pref_road_list::kNameKanji);
		vaKanjiYomi = AheGetAttribute(ipRow, pref_road_list::kNameYomi);
	}

	// ���H���̂������ꍇ�͌x������t�^
	if (uh::variant_util::isNullOrEmpty(vaKanjiName)) {
		strMsg += uh::str_util::format(AheLoadString(IDS_NONAME), AheLoadString(IDS_PREFROADNAME));
	}
}

// ���͂��ꂽ��ԕʏ̃R�[�h�ɊY�����̂����݂��邩�`�F�b�N���� [bug 12036]
void CAttrRoadLinkDlg::CheckSectionNameExists(CString& strMsg, CFeatureDef& pFeatureDef)
{
	// ���ݑI�𒆂̃t�B�[�`���擾
	IFeaturePtr	ipCurrentFeature = GetCurrentFeatureDef()->GetFeature();

	// SECTION_CODE_LIST�e�[�u���擾
	ITablePtr ipTable = AheOpenSameOwnerTable(AheGetTable(ipCurrentFeature), section_code_list::kTableName);
	if (!ipTable) return;


	// ��ԕʏ̃R�[�h���擾
	CFieldDef* pFieldDef = pFeatureDef.GetFieldDef(road_link::kTableName, road_link::kSectionCode);
	// ROAD_LINK�̃t�B�[���h��SECTION_CODE����`����Ă��Ȃ��ꍇ�ɗ����Ȃ��悤��
	if (!pFieldDef) return;
	if (!(pFieldDef->m_bChanged)) return; //!< �ҏW����Ă��Ȃ����return
	long lSectionCode = pFieldDef->m_vaValue.lVal;

	// 0�͋��l
	if (lSectionCode == 0) return;

	// �e�R�[�h�l�̃`�F�b�N
	if (!CheckRoadCodeSectionCode(lSectionCode)) {
		AttrMessageBox(AheLoadString(IDS_OUTOFSECTIONCODERANGE), AheLoadString(IDS_CAPTION), MB_OK, WARNLEVEL_HIGH);
		strMsg += uh::str_util::format(AheLoadString(IDS_NONAME), AheLoadString(IDS_SECTIONNAME));
		return;
	}

	// �N�G�����𐶐�
	CString strWhereClause, strSubFields;
	strWhereClause.Format(_T("%s = %ld"), road_link::kSectionCode, lSectionCode);
	strSubFields.Format(_T("%s,%s"), section_code_list::kNameKanji, section_code_list::kNameYomi);

	// �J�[�\���擾
	_ICursorPtr ipCursor;
	if (!(GetCursor(ipTable, ipCursor, strWhereClause, strSubFields))) {
		strMsg += AheLoadString(IDS_FAILTOSEARCHSECTIONCODE);
		return;
	}

	_IRowPtr ipRow;
	CComVariant vaKanjiName, vaKanjiYomi;
	if (ipCursor->NextRow(&ipRow) == S_OK && ipRow)
	{
		vaKanjiName = AheGetAttribute(ipRow, section_code_list::kNameKanji);
		vaKanjiYomi = AheGetAttribute(ipRow, section_code_list::kNameYomi);
	}

	// �Y�����̂������ꍇ�͌x������t�^
	if (uh::variant_util::isNullOrEmpty(vaKanjiName)) {
		strMsg += uh::str_util::format(AheLoadString(IDS_NONAME), AheLoadString(IDS_SECTIONNAME));
	}
}

void CAttrRoadLinkDlg::TabHighLight()
{
	// ���H�����N�����[�V�����̃G���[�܂��͌x���̎��Ƀ^�u���n�C���C�g�����邽��
	CControlDef* def = NULL;
	BOOL bKihon = FALSE;
	BOOL bTuika = FALSE;
	BOOL bKoutukisei = FALSE;
	BOOL bRosen =FALSE;
	BOOL bDRMA = FALSE;
	BOOL bParking = FALSE;
	for( CControlRel::iterator i = m_cControlRel.begin(); i != m_cControlRel.end(); i++ )
	{
		def = &(*i);
		if( def->m_uiCheckType == CHECK_DISPNAVI ||  def->m_uiCheckType == CHECK_DISPOTHER)
		{
			if( def->m_bErrored )
			{
				CString strFieldName = def->GetFieldName();
				if( strFieldName == _T("NAVICLASS_C") ||
					strFieldName == _T("ROADMANAGER_C") ||
					strFieldName == _T("ROADWIDTH_C") ||
					strFieldName == _T("MAIN_LINKCLASS_C") ||
					strFieldName == _T("SUB_LINKCLASS_C") )
				{
					bKihon = TRUE;
				}
				else if( strFieldName == _T("ISLAND_F") ||
					strFieldName == _T("BYPASS_F") ||
					strFieldName == _T("CARONLY_F") ||
					strFieldName == _T("WIDEAREA_C") ||
					strFieldName == _T("LANE_COUNT") ||
					strFieldName == _T("LANEVALID_F") ||
					strFieldName == _T("UPDOWNCLASS_C") ||
					strFieldName == _T("RIGHT_CHANGE") ||
					strFieldName == _T("LEFT_CHANGE") ||
					strFieldName == _T("ALLEYMAINTE_C"))
				{
					bTuika = TRUE;
				}
				else if( strFieldName == _T("ONEWAY_C") ||
					strFieldName == _T("NOPASSAGE_C") )
				{
					bKoutukisei = TRUE;
				}
				else if( strFieldName == road_link::kRoadCode ||
					strFieldName == road_link::kPrefCode ||
					strFieldName == road_link::kRoadNo ||
					strFieldName == road_link::kSectionCode)
				{
					bRosen = TRUE;
				}
				else if( strFieldName == _T("KUBUN_C") ||
					strFieldName == _T("LIMITSPEED_C") ||
					strFieldName == _T("TRIP_SPEED") ) 
				{
					bDRMA = TRUE;
				}
				else if( strFieldName == _T("PARKINGCLASS_C") ||
					strFieldName == _T("PARKINGLINKCLASS_C") ||
					strFieldName == _T("FLOOR_LEVEL") ||
					strFieldName == _T("MIDDLEFLOOR_LEVEL") )
				{
					bParking = TRUE;
				}
			}
		}
	}
	CTabCtrl cTab( GetDlgItem( IDC_TAB_ROADATTR ) );
	if( bKihon )
		cTab.HighlightItem( 0, TRUE );
	else
		cTab.HighlightItem( 0, FALSE );
	if( bTuika )
		cTab.HighlightItem( 1, TRUE );
	else
		cTab.HighlightItem( 1, FALSE );
	if( bKoutukisei )
		cTab.HighlightItem( 2, TRUE );
	else
		cTab.HighlightItem( 2, FALSE );
	if( bRosen )
		cTab.HighlightItem( 3, TRUE );
	else
		cTab.HighlightItem( 3, FALSE );
	if( bDRMA )
		cTab.HighlightItem( 4, TRUE );
	else
		cTab.HighlightItem( 4, FALSE );
	if( bParking )
		cTab.HighlightItem( 5, TRUE );
	else
		cTab.HighlightItem( 5, FALSE );

	InvalidateRect( NULL, FALSE );
}
/**
 * @fn BOOL CAttrRoadLinkDlg::CheckReturnKeyItem(INT nClassID)
 * @brief ���^�[���L�[�������ꂽ���̓���
 *
 * @param	INT		nClassID	[i]		���^�[���L�[�������ꂽ����ItemID
 *
 * @return	TRUE : ���^�[���L�[�Ƃ��Ă̓���(�K�p����)�� , FALSE : ���^�[���L�[�Ƃ��Ă̓���s�� 
 */
BOOL CAttrRoadLinkDlg::CheckReturnKeyItem(INT nClassID)
{
	// 2�����b�V�����E�����ׂ��ł��铹�H�̏ꍇ�͔��Α��̓��H�����������ɂ��Ȃ��Ă͂����Ȃ�����
	// ���̃R�s�[�����͂�邵���Ȃ��̂ł���������Ă͐�΂ɂ����Ȃ��I
	CopyMeshRoad();
	return TRUE;
}

void CAttrRoadLinkDlg::CopyMeshRoad()
{
	// 2�����b�V�����E�����ׂ��ł��铹�H�̏ꍇ�͔��Α��̓��H�����������ɂ��Ȃ��Ă͂����Ȃ�����
	// �ȉ��̏������s��
	if( m_bIsRoadOnMesh )
	{
		// �Ƃ肠�����A���ݕێ����Ă��郊�X�g���ׂĂɂ����ă��[�v���s��
		for( list<CFeatureDef>::iterator it = m_pFeatureDefList->begin(); it != m_pFeatureDefList->end(); it++ )
		{
			IFeaturePtr temp;
			LONG lOID = 0;
			temp = it->GetFeature();
			temp->get_OID( &lOID );

			// ���ݑI�𒆂�2�����b�V�����ׂ��ł��铹�H�����N�Ɋ֘A���Ă��郊�X�g�Ƃ̏ƍ�
			CFeatureDef *pFeatureDef = NULL;
			for( list<CFeatureDef>::iterator iRoad = m_pRoadOnMeshList.begin(); iRoad != m_pRoadOnMeshList.end(); iRoad++ )
			{
				LONG lMeshOID;
				iRoad->GetFeature()->get_OID( &lMeshOID );
				if( lOID == lMeshOID )
				{
					pFeatureDef = &(*it);
					break;
				}
			}

			if( pFeatureDef )
			{
				// ���ׂẴt�B�[���h�l���`�F�b�N
				for( list<CFieldDef>::iterator itField = GetCurrentFeatureDef()->begin(); itField != GetCurrentFeatureDef()->end(); itField++ )
				{
					CString strFieldName = itField->m_strFieldName;

					// �ȉ�if���̒��g�̓R�s�[���Ă��܂��Ă͂����Ȃ�����
					if( strFieldName != _T("OBJECTID") && strFieldName != _T("OPERATOR") &&
						strFieldName != _T("PURPOSE_C") && strFieldName != _T("MODIFYDATE") &&
						strFieldName != _T("UPDATETYPE_C") && strFieldName != _T("PROGMODIFYDATE") &&
						strFieldName != _T("MODIFYPROGNAME") && strFieldName != _T("TMP_MESHCODE") &&
						strFieldName != _T("TMP_PRIMID") && strFieldName != _T("TMPFROM_NODE") &&
						strFieldName != _T("TMPTO_NODE") && strFieldName != _T("FROM_NODE_ID") &&
						strFieldName != _T("ADMIN_CODE") && strFieldName != _T("FIELDSURVEY_F") &&
						strFieldName != _T("USERCLAIM_F") &&
						strFieldName != _T("TO_NODE_ID") && strFieldName != _T("TURNREG_LQRF") &&
						strFieldName != _T("DIRGUIDE_LQRF") && strFieldName != _T("GUIDE_LQRF") &&
						strFieldName != _T("LANE_LQRF") && strFieldName != _T("VICS_LQRF") &&
						strFieldName != _T("BYWAY_LQRF") && strFieldName != _T("ROUTE_LQRF") &&
						strFieldName != _T("UTURN_LQRF") && strFieldName != _T("INTERNAVI_LQRF") &&
						strFieldName != _T("SHAPE") && strFieldName != _T("SHAPE.LEN") )
					{
						// ��L��if���̏����ȊO�̓R�s�[����
						CFieldDef *pFieldDef;
						pFieldDef = pFeatureDef->GetFieldDef( ROAD_TABLE_NAME, itField->m_strFieldName );
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

/////////////////////////////////////////////////////////////////////////////
//
// �O���b�h�R���g���[���p
//
/////////////////////////////////////////////////////////////////////////////

/**
 * @fn void CAttrRoadLinkDlg::MouseDownTimeRegGrid( INT nID, IVSFlexGridPtr spGrid, LONG& lIndex, LPCTSTR lpcszTableName, SHORT Button, SHORT Shift, FLOAT X, FLOAT Y )
 * @brief �O���b�h�̃Z�����N���b�N�����Ƃ��ɌĂ΂��
 *
 * �����_�C�A���O�Ŏ��ԋK���̕ύX���ł����Ƃ��ɂ́A2�����b�V�����ׂ��ł��铹�H�̏ꍇ�ɂ́A�ׂ��ł��铹�H�����N�ɂ��R�s�[����
 * ���݂́A���ԋK���������A�~���ŕ��בւ��郁�j���[���o�������B 
 */
void CAttrRoadLinkDlg::MouseDownTimeRegGrid( INT nID, IVSFlexGridPtr spGrid, LONG& lIndex, LPCTSTR lpcszTableName, SHORT Button, SHORT Shift, FLOAT X, FLOAT Y )
{
	LONG lCol = 0, lRow = 0;
	CAxWindow wnd = GetDlgItem( nID );
		
	spGrid->get_MouseCol( &lCol );
	spGrid->get_MouseRow( &lRow );

		
	POINT pt = { XTwipsToPixels(X), YTwipsToPixels(Y) };
	wnd.ClientToScreen( &pt );
	
	if( lRow == 0 )
	{
		// �E�N���b�N�Ȃ烁�j���[���o��
		if( Button & 0x02 ) {

			AHE::CMenu menu;
			menu.CreateMenu( TRUE );

			menu.InsertMenu( 0, MF_BYPOSITION | MF_STRING, 1000, AheLoadString(IDS_ASCENDING) );
			menu.InsertMenu( 1, MF_BYPOSITION | MF_STRING, 1001, AheLoadString(IDS_DESCENDING) );

			INT intRet = menu.TrackPopupMenu( TPM_RETURNCMD | TPM_NONOTIFY | TPM_LEFTALIGN | TPM_TOPALIGN, pt.x, pt.y );
			
			LONG lBottomRow;
			spGrid->get_Rows( &lBottomRow );
			spGrid->Select( _variant_t(1L), _variant_t(lCol), _variant_t(LONG(lBottomRow-1)), _variant_t(lCol));
			if( intRet == 1000 )
				spGrid->put_Sort( flexSortGenericAscending );
			else if( intRet == 1001 )
				spGrid->put_Sort( flexSortGenericDescending );
		}
	}
}


HRESULT CAttrRoadLinkDlg::CreateActiveXControls(UINT nID)
{
	// Load dialog template and InitData
	HRSRC hDlgInit = ::FindResource(_AtlBaseModule.GetResourceInstance(), MAKEINTRESOURCE(nID), (LPTSTR)_ATL_RT_DLGINIT);
	BYTE* pInitData = NULL;
	HGLOBAL hData = NULL;
	HRESULT hr = S_OK;
	if (hDlgInit != NULL)
	{
		hData = ::LoadResource(_AtlBaseModule.GetResourceInstance(), hDlgInit);
		if (hData != NULL)
			pInitData = (BYTE*) ::LockResource(hData);
	}

	HRSRC hDlg = ::FindResource(_AtlBaseModule.GetResourceInstance(), MAKEINTRESOURCE(nID), (LPTSTR)RT_DIALOG);
	if (hDlg != NULL)
	{
		HGLOBAL hResource = ::LoadResource(_AtlBaseModule.GetResourceInstance(), hDlg);
		DLGTEMPLATE* pDlg = NULL;
		if (hResource != NULL)
		{
			pDlg = (DLGTEMPLATE*) ::LockResource(hResource);
			if (pDlg != NULL)
			{
				// Get first control on the template
				BOOL bDialogEx = _DialogSplitHelper::IsDialogEx(pDlg);
				WORD nItems = _DialogSplitHelper::DlgTemplateItemCount(pDlg);

				// Get first control on the dialog
				DLGITEMTEMPLATE* pItem = _DialogSplitHelper::FindFirstDlgItem(pDlg);
				HWND hWndPrev = GetWindow(GW_CHILD);

				// Create all ActiveX cotnrols in the dialog template and place them in the correct tab order (z-order)
				for (WORD nItem = 0; nItem < nItems; nItem++)
				{
					DWORD wID = bDialogEx ? ((_DialogSplitHelper::DLGITEMTEMPLATEEX*)pItem)->id : pItem->id;
					if (_DialogSplitHelper::IsActiveXControl(pItem, bDialogEx))
					{
						BYTE* pData = NULL;
						DWORD dwLen = _DialogSplitHelper::FindCreateData(wID, pInitData, &pData);
						CComPtr<IStream> spStream;
						if (dwLen != 0)
						{
							HGLOBAL h = GlobalAlloc(GHND, dwLen);
							if (h != NULL)
							{
								BYTE* pBytes = (BYTE*) GlobalLock(h);
								BYTE* pSource = pData; 
								Checked::memcpy_s(pBytes, dwLen, pSource, dwLen);
								GlobalUnlock(h);
								CreateStreamOnHGlobal(h, TRUE, &spStream);
							}
							else
							{
								hr = E_OUTOFMEMORY;
								break;
							}
						}

						CComBSTR bstrLicKey;
						hr = _DialogSplitHelper::ParseInitData(spStream, &bstrLicKey.m_str);
						if (SUCCEEDED(hr))
						{
							CAxWindow2 wnd;
							// Get control caption.
							LPWSTR pszClassName = 
								bDialogEx ? 
									(LPWSTR)(((_DialogSplitHelper::DLGITEMTEMPLATEEX*)pItem) + 1) :
									(LPWSTR)(pItem + 1);
							// Get control rect.
							RECT rect;
							rect.left = 
								bDialogEx ? 
									((_DialogSplitHelper::DLGITEMTEMPLATEEX*)pItem)->x : 
									pItem->x;
							rect.top = 
								bDialogEx ? 
									((_DialogSplitHelper::DLGITEMTEMPLATEEX*)pItem)->y : 
									pItem->y;
							rect.right = rect.left + 
								(bDialogEx ? 
									((_DialogSplitHelper::DLGITEMTEMPLATEEX*)pItem)->cx : 
									pItem->cx);
							rect.bottom = rect.top + 
								(bDialogEx ? 
									((_DialogSplitHelper::DLGITEMTEMPLATEEX*)pItem)->cy : 
									pItem->cy);

							// Convert from dialog units to screen units
							MapDialogRect(&rect);

							// Create AxWindow with a NULL caption.
							wnd.Create(m_hWnd, 
								&rect, 
								NULL, 
								(bDialogEx ? 
									((_DialogSplitHelper::DLGITEMTEMPLATEEX*)pItem)->style : 
									pItem->style) | WS_TABSTOP, 
								bDialogEx ? 
									((_DialogSplitHelper::DLGITEMTEMPLATEEX*)pItem)->exStyle : 
									0,
								bDialogEx ? 
									((_DialogSplitHelper::DLGITEMTEMPLATEEX*)pItem)->id : 
									pItem->id,
								NULL);

							if (wnd != NULL)
							{
								// Set the Help ID
								if (bDialogEx && ((_DialogSplitHelper::DLGITEMTEMPLATEEX*)pItem)->helpID != 0)
									wnd.SetWindowContextHelpId(((_DialogSplitHelper::DLGITEMTEMPLATEEX*)pItem)->helpID);
								// Try to create the ActiveX control.
								static WCHAR pwchLicenseKey[] =
								{
									0x004F,	0x0030,	0x0045,	0x0054,	0x0054,	0x0043,	
									0x0037,	0x0034,	0x0037
								};
								hr = wnd.CreateControlLic(pszClassName, spStream, NULL, /*bstrLicKey*/CComBSTR(pwchLicenseKey));
								if (FAILED(hr))
									break;
								// Set the correct tab position.
								if (nItem == 0)
									hWndPrev = HWND_TOP;
								wnd.SetWindowPos(hWndPrev, 0,0,0,0,SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
								hWndPrev = wnd;
							}
							else
							{
								hr = AtlHresultFromLastError();
							}
						}
					}
					else
					{
						if (nItem != 0)
							hWndPrev = ::GetWindow(hWndPrev, GW_HWNDNEXT);
					}
					pItem = _DialogSplitHelper::FindNextDlgItem(pItem, bDialogEx);
				}
			}
			else
				hr = AtlHresultFromLastError();
		}
		else
			hr = AtlHresultFromLastError();
	}
	return hr;
}

// [bug 6712] �L��t���O�Ɨ����t���O�����W�I�{�^�����ɂ���
LRESULT CAttrRoadLinkDlg::OnCheckWideAreaOrIsland(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	CButton cWideArea( GetDlgItem( IDC_CHECK_WIDEAREAF ) ), cIsland( GetDlgItem( IDC_CHECK_ISLANDF ) );

	CComboBox CCombo( GetDlgItem( IDC_COMBO_WIDEAREA ) );
	switch( cWideArea.GetCheck() )
	{
	case BST_CHECKED:
		// [bug 6712 #20] �L�摶�݃R�[�h��2�ɂ��ĕύX��ʒm
		if( CCombo.SetCurSel(2) != CB_ERR ) 
			OnCommand(0, MAKEWPARAM(IDC_COMBO_WIDEAREA, CBN_SELCHANGE), 0, bHandled);

		// �L��t���O�Ɨ����t���O�������Ƃ��`�F�b�N����Ă���A
		// �����̃N���b�N���ł��������ă`�F�b�N���͂���
		// BST_UNCHECKED���Z�b�g���邾�����ƕύX�ʒm�����Ȃ�
		if( cIsland.GetCheck() == BST_CHECKED )
		{
			switch( wID )
			{
			case IDC_CHECK_ISLANDF:
				cWideArea.Click();
				// [bug 6712 #20] �L�摶�݃R�[�h��0�ɂ��Ƃ�
				if( CCombo.SetCurSel(0) != CB_ERR )
					OnCommand(0, MAKEWPARAM(IDC_COMBO_WIDEAREA, CBN_SELCHANGE), 0, bHandled);
				break;
			case IDC_CHECK_WIDEAREAF:
				cIsland.Click();
				break;
			default:
				break;
			};
		}
		break;
	case BST_UNCHECKED:
		// [bug 6712 #20] �L�摶�݃R�[�h��0�ɂ��ĕύX��ʒm
		if( CCombo.SetCurSel(0) != CB_ERR )
			OnCommand(0, MAKEWPARAM(IDC_COMBO_WIDEAREA, CBN_SELCHANGE), 0, bHandled);
		break;
	default:
		break;
	}

	bHandled = FALSE;

	return 0;
}
