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

// AttrWarningSignDlg.cpp : DLL �A�v���P�[�V�����p�ɃG�N�X�|�[�g�����֐����`���܂��B
//

#include "stdafx.h"
#include "AttrWarningSignDlg.h"
#include <boost/assign/list_of.hpp>
#include <TDC/useful_headers/str_util.h>
#include <WinLib/winutil.h>
#include <ArcHelperEx/Map.h>
#include <WinLib/grid_util.h>

using namespace std;
using namespace sindy::schema;
using namespace uh::str_util;
using namespace search_road_link_grid;

namespace
{
	// �O���b�h�̃w�b�_
	static const _TCHAR* SEARCH_ROAD_LINK_GRID_HEADER = _T("ID|����(m)");

	// ���H�����N�����̏����l(���[�g��)
	static const long INIT_SEARCH_METER = 10;

	// ���H�����N�����̍ő勗��(���[�g��)
	static const long MAX_SEARCH_METER = 50;
}

// �E�B���h�E�}�l�[�W���z�u�}�b�v
BEGIN_WINDOW_MAP(WARNING_SIGN_MAP)
	BEGINROWS(WRCT_REST, 0, RCMARGINS(2, 2))
		BEGINROWS(WRCT_REST, 0, 0)
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10, 0))
				RCTOFIT(IDC_STATIC_WARNING_SIGN)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_WARNING_SIGN)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10, 0))
				RCTOFIT(IDC_STATIC_LINK_ID)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_LINK_ID)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10, 0))
				RCTOFIT(IDC_STATIC_PICTURE_PATH)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_PICTURE_PATH)
				RCTOFIT(IDC_BUTTON_COPY_PICTURE_PATH)
				RCTOFIT(-1)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10, 0))
				RCTOFIT(IDC_STATIC_DISTANCE)
				RCSPACE(6)
				RCTOFIT(IDC_EDIT_SEARCH_METER)
				RCSPACE(6)
				RCTOFIT(IDC_STATIC_METER)
				RCSPACE(10)
				RCTOFIT(IDC_BUTTON_SEARCH_ROAD_LINK)
				RCSPACE(10)
				RCTOFIT(IDC_BUTTON_UPDATE_LINK_ID)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_PCT, 0, 27)
				RCREST(IDC_GRID_ROAD_LINK)
			ENDGROUP()
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
 * @retval CAttrWarningSignDlg��Ԃ��܂��B
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
	return new CAttrWarningSignDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrWarningSignDlg ���b�Z�[�W�n���h��
//
/////////////////////////////////////////////////////////////////////////////

/**
 * �_�C�A���O�쐬����ۂɈ�ԍŏ��ɌĂ΂�܂��B�����Ń_�C�A���O�̏����������Ă�������
 */
LRESULT CAttrWarningSignDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	ATTR_BASE_CLASS::OnInitDialog(uMsg, wParam, lParam, bHandled);

	// �R���g���[��
	CreateControlRelation();
	// �T�u�N���X��
	for(auto& it : m_mapSubClass)
	{
		it.second.SubclassWindow(GetDlgItem(it.first));
	}

	// �V���N�C�x���g�̃A�h�o�C�Y
	AtlAdviseSinkMap(this, true);
	
	return 1;
}

/**
 * WM_COMMAND �p�C�x���g�n���h��
 *
 * �e�R�����R���g���[���ŕύX���������ꍇ�A�����ŏ������܂�
 * ���݂̓G�f�B�b�g�{�b�N�X�A�`�F�b�N�{�b�N�X�A�R���{�{�b�N�X�̏������s���܂�
 */
LRESULT CAttrWarningSignDlg::OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// SetCurrentFeatureDefIndex() �̍Œ��̓L�����Z���i�^�C�~���O�ɂ���Ă͂��������Ȃ邽�߁j
	if(!m_bFinishInit) return 0;

	int msg = HIWORD(wParam);						// ���상�b�Z�[�W
	int	nTargetControl = (int)LOWORD(wParam);		// ����ΏۃR���g���[��

	// �G�f�B�b�g�{�b�N�X�̎��ɑS�I���ɂ���
	if(msg == EN_SETFOCUS)
		SelectDlgItem(nTargetControl);
	// �R���{�{�b�N�X�̃��X�g�{�b�N�X�̒����𒲐�
	if(msg == CBN_DROPDOWN)
		SetComboboxList(nTargetControl);

	// �R���{�{�b�N�X�A�G�f�B�b�g�{�b�N�X�A�`�F�b�N�{�b�N�X�̃��b�Z�[�W����
	if((msg == CBN_SELCHANGE) || (msg == BN_CLICKED) || (msg == EN_UPDATE)) {
		// �ύX���ꂽ���ǂ����`�F�b�N���A�ύX����Ă����瑼�̃R���g���[���ɂ����f������
		m_cControlRel.ControlChanged((int)LOWORD(wParam));
		// �_�C�A���O���X�V�̈�ɒǉ����܂�
		InvalidateRect(NULL, FALSE);
		// �ύX���ꂽ�ꍇ�̓{�^���� Enable ��
		SetButton(Changed());
	}

	return 0;
}

/**
 * ���s�摜�t�@�C���p�X�R�s�[�{�^�����������Ƃ��̏���
 */
LRESULT CAttrWarningSignDlg::OnClickedButtonCopyPicturePath(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	// �N���b�v�{�[�h�ɑ��s�摜�t�@�C���p�X��n��
	CFieldDef* pPicPathDef = GetCurrentFeatureDef()->GetFieldDef(warning_sign::kTableName, warning_sign::kPicturePath);
	CString strPicPath = (pPicPathDef->m_vaValue.vt == VT_NULL) ? CString() : pPicPathDef->m_vaValue;
	winutil::SetToClipboard(strPicPath);

	return 0;
}

/**
* ���H�����N�����{�^�����������Ƃ��̏���
* �I�𒆂̌x���W������w�肵�������ȓ��ɑ��݂��铹�H�����N��T��
*/
LRESULT CAttrWarningSignDlg::OnClickedSearchRoadLink(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	// ���H�����N�̌��������{
	ExecuteSearchRoadLink();
	return 0;
}

/**
* �����NID�K�p�{�^�����������Ƃ��̏���
*/
LRESULT CAttrWarningSignDlg::OnClickedButtonUpdateLinkID(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	// �O���b�h�őI�������t�B�[�`�����擾
	// �t�B�[�`������A�܂��͓����t�B�[�`���ł������ꍇ�́A�ύX�Ȃ�
	// �ύX����Ȃ�LINK_ID�ɔ��f
	
	if(m_ipSelectedRoadLinkItem.GetFeature() && m_ipRoadLinkItem != m_ipSelectedRoadLinkItem)
	{
		m_ipRoadLinkItem = m_ipSelectedRoadLinkItem;
		SetDlgItemText(IDC_EDIT_LINK_ID, ToString(m_ipRoadLinkItem.GetOID()));
		CFieldDef* pLinkIdDef = GetCurrentFeatureDef()->GetFieldDef(warning_sign::kTableName, warning_sign::kLinkID);
		pLinkIdDef->SetNewValue(CComVariant(m_ipRoadLinkItem.GetOID()), m_hWnd);
		SetButton(Changed());
		m_cArcHelper.GetActiveView()->PartialRefresh(esriViewForeground, NULL, NULL);
	}

	return 0;
}


/**
 * �`�揈��
 */
LRESULT CAttrWarningSignDlg::OnRefresh(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// �R�t�����H�����N���n�C���C�g
	IGeometryPtr ipRoadLinkGeom = m_ipRoadLinkItem.GetShape();
	if(ipRoadLinkGeom)
	{
		AheDrawPolyline(m_cArcHelper.GetDisplay(), ipRoadLinkGeom, RGB(255, 0, 0), 5, esriSLSSolid, TRUE, TRUE);
	}
	// �O���b�h�őI�𒆂̓��H�����N���n�C���C�g
	IGeometryPtr ipSelectedRoadLinkGeom = m_ipSelectedRoadLinkItem.GetShape();
	if(ipSelectedRoadLinkGeom)
	{
		// �I�������t�B�[�`�����n�C���C�g
		AheDrawPolyline(m_cArcHelper.GetDisplay(), ipSelectedRoadLinkGeom, RGB(0, 0, 255), 5, esriSLSSolid, TRUE, TRUE);
	}

	return 0;
}

/**
 * �h���b�v�_�E�����X�g���J���Ƃ��̏���
 */
LRESULT CAttrWarningSignDlg::OnOpeningDropdownList(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	// �h���b�v�_�E�����X�g����"x : ����`"��I���ł��Ȃ��悤�A�J�����O�Ƀ��X�g����폜����
	CComboBox cb(GetDlgItem(IDC_COMBO_WARNING_SIGN));

	int nIdx = cb.FindStringExact(0, AheLoadString(IDS_COMBO_ITEM_FOR_UNDEFINED_VALUE).GetString());
	if(nIdx != CB_ERR)
		cb.DeleteString(nIdx);

	return 0;
}

/**
 * �h���b�v�_�E�����X�g������Ƃ��̏���
 */
LRESULT CAttrWarningSignDlg::OnClosedDropdownList(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	// ���X�g����I���������ڂ̃R�[�h�l����U�擾
	CComboBox cb(GetDlgItem(IDC_COMBO_WARNING_SIGN));
	int nCurSel = cb.GetCurSel();
	long lSelVal = cb.GetItemData(nCurSel);
	
	// �h���b�v�_�E�����X�g����蒼��
	SettingWarningSignDropdownList();
	
	// ���X�g����I�������R�[�h�l�̍��ڂ����߂ăZ�b�g������
	int nCount = cb.GetCount();
	int nIdxUndefinedItem = -1;
	for(int i = 0; i < nCount; ++i)
	{
		long lItemVal = cb.GetItemData(i);
		if(lItemVal == warning_sign::warning_sign_class::kNone)
		{
			nIdxUndefinedItem = i;
		}
		if(lItemVal == lSelVal)
		{
			cb.SetCurSel(i);
			return 0;
		}
	}

	// �h���C���Œ�`����Ă��Ȃ��R�[�h�l�̏ꍇ�́A
	// �R�[�h�l0�̍��ڂ��Z�b�g����
	cb.SetCurSel(nIdxUndefinedItem);

	return 0;
}

/**
* TAB�L�[�ł̃R���g���[���ړ��A�y�і��L�[�̏㉺�ŃR���{�{�b�N�X�̃A�C�e���̕ύX�̃G�~�����[�g
*/
LRESULT CAttrWarningSignDlg::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// �G���^�[�L�[�ȊO�̏ꍇ��ATTR_BASE_CLASS�̕��̏������Ăяo��
	if(wParam != VK_RETURN)
		return ATTR_BASE_CLASS::OnKeyDown(uMsg, wParam, lParam, bHandled);

	// ���������̃e�L�X�g�{�b�N�X�ŃG���^�[�L�[�������ꂽ�ꍇ�͓��H�����N������
	// ����ȊO�ł͉������Ȃ�
	CWindow cWnd = GetFocus();
	if(cWnd.GetDlgCtrlID() == IDC_EDIT_SEARCH_METER)
		ExecuteSearchRoadLink();

	return 1;
}

/**
* ���H�����N�������ʃO���b�h�Ń}�E�X�𗣂����Ƃ��̏���
*/
void __stdcall CAttrWarningSignDlg::OnRoadLinkMouseUp(SHORT Button, SHORT Shift, FLOAT X, FLOAT Y)
{
	long lRow = -1;
	m_spSearchRoadLinkGrid->get_MouseRow(&lRow);

	// �O���b�h�O�Ń}�E�X�𗣂����ꍇ�A�J�����g�Z����I����Ԃɂ���
	if(lRow <= 0)
		m_spSearchRoadLinkGrid->get_Row(&lRow);

	// �s�̑I����Ԃ��X�V
	for(int i = m_spSearchRoadLinkGrid->FixedRows; i <= m_spSearchRoadLinkGrid->Rows - m_spSearchRoadLinkGrid->FixedRows; ++i)
	{
		m_spSearchRoadLinkGrid->put_IsSelected(i, (i == lRow) ? VARIANT_TRUE : VARIANT_FALSE);
	}

	// �����s�̑I���͂ł��Ȃ��悤�ɂ���
	m_spSearchRoadLinkGrid->put_Row(lRow);

	// �I�������s�ɑΉ����铹�H�����N�̃t�B�[�`���������o�ϐ��ɕێ�
	_variant_t va;
	m_spSearchRoadLinkGrid->get_RowData(lRow, &va);
	IFeaturePtr ipFeature = (IFeature*)va.punkVal;
	m_ipSelectedRoadLinkItem = ipFeature;
	
	m_cArcHelper.GetActiveView()->PartialRefresh(esriViewForeground, NULL, NULL);
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrWarningSignDlg �����o�֐�
//
/////////////////////////////////////////////////////////////////////////////

/**
* @fn CAttrWarningSignDlg::CAttrWarningSignDlg()
* @brief �R���X�g���N�^
*
* �u���V�̏����������Ă����܂��B
*/
CAttrWarningSignDlg::CAttrWarningSignDlg()
	: ATTR_BASE_CLASS(WARNING_SIGN_MAP, IDD_WARNING_SIGN_DLG), m_lSearchDist(INIT_SEARCH_METER)
{
	m_mapSubClass = boost::assign::map_list_of
		(IDC_COMBO_WARNING_SIGN, CContainedWindow(_T("COMBO"), this, IDC_COMBO_WARNING_SIGN))
		(IDC_EDIT_LINK_ID, CContainedWindow(_T("EDIT"), this, IDC_EDIT_LINK_ID))
		(IDC_EDIT_PICTURE_PATH, CContainedWindow(_T("EDIT"), this, IDC_EDIT_PICTURE_PATH))
		(IDC_EDIT_SEARCH_METER, CContainedWindow(_T("EDIT"), this, IDC_EDIT_SEARCH_METER))
		(IDC_BUTTON_UPDATE_LINK_ID, CContainedWindow(_T("BUTTON"), this, IDC_BUTTON_UPDATE_LINK_ID))
		(IDC_EDIT_SOURCE, CContainedWindow(_T("EDIT"), this, IDC_EDIT_SOURCE));
}

/** 
 * @fn HWND CAttrWarningSignDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief �q�_�C�A���O���쐬���܂��B
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)�Ń_�C�A���O���쐬���܂��B
 *
 * @param hWndParent	[in]	�e�E�B���h�E�n���h��
 * @param dwInitParam	[in]	�_�C�A���O�������p�����[�^�i���ݓ��Ɏg�p���Ă��܂���j
 *
 * @retval �쐬���ꂽ�q�_�C�A���O�̃n���h����Ԃ��܂��B
 */
HWND CAttrWarningSignDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	return ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
}

/**
 * @fn void CAttrWarningSignDlg::Delete()
 * @brief �쐬�����q�_�C�A���O���폜���܂��B
 *
 * �_�C�A���O���폜����Ƃ��ɕK���Ă�ŉ������B�Ă΂Ȃ��ƃ��������[�N���Ă��܂��܂��B
 */
void CAttrWarningSignDlg::Delete()
{
	m_cArcHelper.GetActiveView()->PartialRefresh(esriViewForeground, nullptr, nullptr);
	ClearFeatureDefs();
	if( IsWindow() )
		DestroyWindow();
	delete this; // �K��new�Ŋm�ۂ���O��
}

/**
 * @brief �G���[�`�F�b�N���s��
 *
 * @retval  TRUE  �G���[�Ȃ�
 * @retval  FALSE �G���[����
 */
BOOL CAttrWarningSignDlg::ErrorCheck()
{
	BOOL bRet = TRUE;
	CString strMsg;

	for(auto& featureDef : *m_pFeatureDefList)
	{
		// �x���W�����C���łȂ���΃X�L�b�v
		CString strTableName(featureDef.GetTableName());
		if(strTableName.CompareNoCase(warning_sign::kTableName) != 0)
			continue;

		CString strErr; // �G���[���b�Z�[�W

		// �x���W����ʂ̃`�F�b�N
		CheckWarningSign(featureDef, strErr);

		// �G���[���b�Z�[�W���Ȃ���Ύ��̃t�B�[�`����
		if(strErr.IsEmpty())
			continue;

		// �G���[���b�Z�[�W������΁A�\���p�̃��b�Z�[�W�ɐ��`
		long lOID = -1;
		featureDef.GetFeature()->get_OID(&lOID);
		CString strTmpMsg;
		strTmpMsg.Format(AheLoadString(IDS_ERR_INVALID_ATTR), featureDef.GetTableName(), lOID);
		int nCurPos = 0;
		CString strToken(strErr.Tokenize(_T(","), nCurPos));
		while(-1 != nCurPos)
		{
			strTmpMsg += _T("\n") + strToken;
			strToken = strErr.Tokenize(_T(","), nCurPos);
		}
		strMsg += strTmpMsg + _T("\n");

		bRet = FALSE;
	}
	
	// �G���[������΃��b�Z�[�W���o��
	if(!bRet)
		AttrMessageBox(strMsg, AheLoadString(IDS_ERRCHK), MB_OK | MB_ICONWARNING, WARNLEVEL_HIGH);

	return bRet;
}

/**
 * @fn BOOL CAttrWarningSignDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
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
BOOL CAttrWarningSignDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
	BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	// TOC����\������Ă��铹�H�����N���C�����擾
	// ��������ꍇ��TOC�̒��ň�ԏ�ɂ�����̂��Ώ�
	m_ipRoadLinkFC = GetFeatureClass(road_link::kTableName);

	// ���C�����擾�ł�����Ή����铹�H�����N���擾
	// �ł��Ȃ�������G���[���b�Z�[�W���o��
	if(m_ipRoadLinkFC)
	{
		m_ipRoadLinkItem = GetRelatedLink(m_ipRoadLinkFC);
	}
	else
	{
		// �t�B�[�`���N���X�͋�ɂ��Ă���
		m_ipRoadLinkItem = CTargetItem();
		AttrMessageBox(AheLoadString(IDS_ERR_NO_EXIST_ROAD_LINK), AheLoadString(IDS_ERR_FAILED_READ_LAYER), MB_OK | MB_TASKMODAL, WARNLEVEL_NONE);
	}
	
	// �x���W����ʃR���{�{�b�N�X�̃��X�g��ݒ�
	SettingWarningSignDropdownList();
	
	// ���ڃt�B�[�`���Ɋ֌W�Ȃ��G�f�B�b�g�{�b�N�X(��F�Q�Ɨp�̘H�����V�[�P���X�⌴�e�ԍ�����)�Ȃǂ𐧌䂷��
	// �ҏW�J�n�O�ƕҏW�J�n��ł��ҏW�s�̂Ƃ���DISABLE�ɂ���
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		SetIMEMode(FALSE);
		::SetFocus( GetDlgItem(IDC_COMBO_WARNING_SIGN) );
	}
	
	// �Ή������NID�͎蓮�ŕҏW�ł��Ȃ��悤�ɂ��邽�߁ADISABLE��
	SetDlgStatus( IDC_EDIT_LINK_ID, FALSE, TRUE );

	// ���������͏�ɕҏW�\�Ƃ��邽�߁AENABLE��
	SetDlgStatus(IDC_EDIT_SEARCH_METER, TRUE, TRUE);

	// �������ʂ̃O���b�h�̏����ݒ�
	InitSearchRoadLinkGrid();

	// ���������̃G�f�B�b�g�{�b�N�X�ɏ����l���Z�b�g
	m_lSearchDist = INIT_SEARCH_METER;
	SetDlgItemInt(IDC_EDIT_SEARCH_METER, m_lSearchDist);

	// �����l(10m)�Ō������Ă���
	SearchRoadLink();

	m_cArcHelper.GetActiveView()->PartialRefresh(esriViewForeground, NULL, NULL);

	return bRet;
}

/**
 * @fn void CAttrWarningSignDlg::CreateControlRelation()
 * @brief �q�_�C�A���O�����ۂɍ쐬���܂��B
 *
 * ���ۂ̍쐬��ControlRel�ɔC���܂��B
 */
void CAttrWarningSignDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// �e�E�C���h�E�n���h����ݒ肵�Ȃ���
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );

		// ControlRel�Ƀt�B�[���h���ƃe�[�u������o�^

		// �x���W�����
		m_cControlRel.SetControl( IDC_COMBO_WARNING_SIGN, TYPE_COMBO, 0, 0, warning_sign::kWarningSign, 0, warning_sign::kTableName );

		// �Ή������NID
		m_cControlRel.SetControl( IDC_EDIT_LINK_ID, TYPE_EDITSTRING, 0, 0, warning_sign::kLinkID, 0, warning_sign::kTableName );

		// ���s�摜�t�@�C���p�X
		m_cControlRel.SetControl( IDC_EDIT_PICTURE_PATH, TYPE_EDITSTRING, 0, 0, warning_sign::kPicturePath, 0, warning_sign::kTableName );

		// ���\�[�X
		m_cControlRel.SetControl( IDC_EDIT_SOURCE, TYPE_EDITSTRING, 0, 0, ipc_table::kSource, 0, warning_sign::kTableName );

		// ��������
		m_cControlRel.SetControl( IDC_EDIT_SEARCH_METER, TYPE_EDITSTRING, 0, 0 );

		// �����NID�K�p�{�^��
		m_cControlRel.SetControl( IDC_BUTTON_UPDATE_LINK_ID, TYPE_BUTTON, 0, 0);
	}
}

/**
 * @brief �x���W����ʃR���{�{�b�N�X�̃h���b�v�_�E�����X�g���쐬����
 */
void CAttrWarningSignDlg::SettingWarningSignDropdownList()
{
	CComboBox cbWarningSign(GetDlgItem(IDC_COMBO_WARNING_SIGN));
	cbWarningSign.ResetContent();
	
	CFieldDef* pWarningSignDef = GetCurrentFeatureDef()->GetFieldDef( warning_sign::kTableName, warning_sign::kWarningSign );
	auto mapWarningSignDomain = pWarningSignDef->GetDomain();
	long lWarningSign = pWarningSignDef->m_vaValue.lVal;
	int nCounter = 0;
	int nUndefinedItemIdx = -1;
	bool bSetSel = false;
	for(const auto& domain : *mapWarningSignDomain)
	{
		// "�R�[�h�l : �h���C��"�̃t�H�[�}�b�g�ɉ����悤���`���A
		// �h���b�v�_�E�����X�g�ɒǉ�����
		// �Ȃ��A�R�[�h�l��0�̏ꍇ��"x �F ����`"�Ƃ���
		CString strItem;
		if(domain.code.lVal == warning_sign::warning_sign_class::kNone)
		{
			strItem = AheLoadString(IDS_COMBO_ITEM_FOR_UNDEFINED_VALUE);
			nUndefinedItemIdx = nCounter;
		}
		else
		{
			strItem = ToString(domain.code.lVal) + CString(_T(" : ")) + domain.code_discription;
		}

		cbWarningSign.InsertString( nCounter, strItem );
		cbWarningSign.SetItemData( nCounter, domain.code.lVal );
		if(lWarningSign == domain.code.lVal)
		{
			cbWarningSign.SetCurSel(nCounter);
			bSetSel = true;
		}
		++nCounter;
	}

	// �l����`����Ă��Ȃ��ꍇ�A"x �F ����`"���Z�b�g
	if(!bSetSel)
		cbWarningSign.SetCurSel(nUndefinedItemIdx);
}

/**
 * @brief �w�肵�����C���̃t�B�[�`���N���X���擾����
 * @note  TOC�̈�ԏ�ɂ���\����Ԃ̃��C�����Ώ�
 * @note  �O���[�v���C���̕\����Ԃ��m�F
 * @param strLayerName	[in]	���C����
 * @return TOC�̈�ԏ�ɂ���\����Ԃ̃t�B�[�`���N���X
 */
IFeatureClassPtr CAttrWarningSignDlg::GetFeatureClass(const CString& strLayerName)
{
	CMap cMap(m_cArcHelper.GetFocusMap());

	const auto& listLayer = cMap.FindLayers(nullptr, nullptr, strLayerName);
	if(listLayer.empty())
		return nullptr;

	for(auto& it = listLayer.begin(); it != listLayer.end(); ++it)
	{
		// �����Ă���O���[�v���C������\���̏ꍇ�͑ΏۊO
		ICompositeLayerPtr ipCompositeLayer = cMap.GetParentCompositeLayer(it->m_T);
		if(ipCompositeLayer)
		{
			VARIANT_BOOL bGroupVisible = VARIANT_FALSE;
			((ILayerPtr)ipCompositeLayer)->get_Visible(&bGroupVisible);
			if(!bGroupVisible)
				continue;
		}

		// �\����Ԃ̂��̂�Ԃ�
		VARIANT_BOOL bVisible = VARIANT_FALSE;
		it->m_T->get_Visible(&bVisible);
		if(bVisible)
		{
			return AheGetFeatureClass(it->m_T);
		}
	}

	return nullptr;
}

/**
 * @brief �t�B�[�`���N���X����w�肵��OID�̃t�B�[�`�����擾����
 * @param ipFeatureClass	[in]	�t�B�[�`���N���X
 * @param lOID	[in]	�擾�������t�B�[�`����OID
 * @return �w�肵��OID�̃t�B�[�`��
 */
CTargetItem CAttrWarningSignDlg::GetTargetItem(const IFeatureClassPtr& ipFeatureClass, long lOID)
{
	if(!ipFeatureClass)
		return CTargetItem();

	// �w�肳�ꂽ�l�Ɠ���OID�������R�[�h���擾
	IFeaturePtr ipFeature;
	return (ipFeatureClass->GetFeature(lOID, &ipFeature) == S_OK && ipFeature) ? CTargetItem(ipFeature) : CTargetItem();
}

/**
 * @brief �x���W���ɑΉ����铹�H�����N�̃t�B�[�`�����擾����
 * @param ipRoadLinkFC	[in]	���H�����N�̃t�B�[�`���N���X
 * @return �Ή����铹�H�����N�̃t�B�[�`��
 */
CTargetItem CAttrWarningSignDlg::GetRelatedLink(const IFeatureClassPtr& ipRoadLinkFC)
{
	if(!ipRoadLinkFC)
		return CTargetItem();

	CComVariant vaLinkID = GetCurrentFeatureDef()->GetFieldDef(warning_sign::kTableName, warning_sign::kLinkID)->m_vaValue;

	// LINK_ID�Ɠ���OID�̓��H�����N�t�B�[�`����Ԃ�
	// LINK_ID��NULL�Ȃ��̃t�B�[�`����Ԃ�
	return (vaLinkID.vt == VT_I4) ? GetTargetItem(ipRoadLinkFC, vaLinkID.lVal) : CTargetItem();
}

/**
* @brief �I�𒆂̌x���W������w�肵�������ȓ��ɑ��݂��铹�H�����N��T��
* @note  TOC�̓��H�����N���C���̏�Ԃ�A�w�苗���̃`�F�b�N���s���A��肪����Ό����͂��Ȃ�
*/
void CAttrWarningSignDlg::ExecuteSearchRoadLink()
{
	// TOC�ɓ��H�����N�����݂��Ȃ��A�܂��͂��ׂĔ�\���ɂȂ��Ă���ꍇ�A
	// �G���[���b�Z�[�W���o���Č��������Ȃ�
	if(!m_ipRoadLinkFC)
	{
		AttrMessageBox(AheLoadString(IDS_ERR_NO_EXIST_ROAD_LINK), AheLoadString(IDS_ERR_FAILED_READ_LAYER), MB_OK | MB_TASKMODAL, WARNLEVEL_NONE);
		return;
	}

	// �����O�Ƀe�L�X�g�{�b�N�X�̋����ɖ�肪�Ȃ����`�F�b�N
	// ��肪����ꍇ�A�G���[���b�Z�[�W���o���Č��������Ȃ�
	CString strErrMsg = CheckSearchMeter();
	if(!strErrMsg.IsEmpty())
	{
		AheMessageBox(nullptr, strErrMsg, AheLoadString(IDS_ERR_SEARCH_METER_INVALID), MB_OK | MB_ICONWARNING | MB_TASKMODAL, WARNLEVEL_NONE);
		return;
	}

	// ���H�����N���������A�O���b�h���X�V
	SearchRoadLink();
}

/**
* @brief ���H�����N�������ʃO���b�h�̏����ݒ�
*/
void CAttrWarningSignDlg::InitSearchRoadLinkGrid()
{
	// �O���b�h�R���g���[���쐬
	CAxWindow axWnd(GetDlgItem(IDC_GRID_ROAD_LINK));
	axWnd.QueryControl(&m_spSearchRoadLinkGrid);

	// �w�b�_�쐬
	grid_util::FormatString(m_spSearchRoadLinkGrid, SEARCH_ROAD_LINK_GRID_HEADER);
	// �񕝒���
	grid_util::AutoRowColSize(m_spSearchRoadLinkGrid);

	// �I�𒆂̃t�B�[�`���͂����ŋ�ɂ��Ă���
	m_ipSelectedRoadLinkItem = CTargetItem();
}

/**
* @brief ���H�����N�������ʃO���b�h�̍X�V
* @param resultMap [in] ���H�����N�������ʂ̃}�b�v
*/
void CAttrWarningSignDlg::UpdateSearchRoadLinkGrid(const ResultMap& resultMap)
{
	// �O���b�h�̃T�C�Y�ݒ�(+1�̓w�b�_��)
	m_spSearchRoadLinkGrid->PutRows(resultMap.size() + 1);

	long lRow = 1;
	for(const auto& record : resultMap)
	{
		// OID
		m_spSearchRoadLinkGrid->put_TextMatrix(lRow, grid_cols::kOID, CComBSTR(ToString(record.second.GetOID())));
		// ����(m)
		m_spSearchRoadLinkGrid->put_TextMatrix(lRow, grid_cols::kDist, CComBSTR(ToString(record.first, _T("%.8lf"))));

		// �O���b�h�ɑΉ�����t�B�[�`�����Z�b�g���Ă���
		IFeaturePtr ipFeature = record.second.GetFeature();
		_variant_t va = (IUnknown*)ipFeature;
		m_spSearchRoadLinkGrid->put_RowData(lRow, va);

		++lRow;
	}

	// �񕝒���
	grid_util::AutoRowColSize(m_spSearchRoadLinkGrid);
}

/**
* @brief �x���W������w�肵�������ȓ��ɑ��݂��铹�H�����N������
* @note �����̒P�ʂ̓��[�g��
*/
void CAttrWarningSignDlg::SearchRoadLink()
{
	IGeometryPtr ipWarningSignGeom = AheGetShape((IFeaturePtr)GetCurrentFeatureDef()->GetFeature());
	if(!ipWarningSignGeom)
		return;

	ITopologicalOperatorPtr ipTopo(ipWarningSignGeom);
	if(!ipTopo)
		return;

	// �����͈͂�x�ɕϊ�
	IUnitConverterPtr ipUnitConv(CLSID_UnitConverter);
	double dConvRange = 0.0;
	ipUnitConv->ConvertUnits(m_lSearchDist, esriMeters, esriDecimalDegrees, &dConvRange);

	// �o�b�t�@�쐬
	IGeometryPtr ipBuff;
	if(FAILED(ipTopo->Buffer(dConvRange, &ipBuff)) || !ipBuff)
	{
		AttrMessageBox(AheLoadString(IDS_FAILED_TO_CREATE_BUFFER), AheLoadString(IDS_ERROR), MB_OK | MB_TASKMODAL, WARNLEVEL_MIDDLE);
		return;
	}

	ipBuff->putref_SpatialReference(AheGetSpatialReference(m_ipRoadLinkFC));

	// ����
	CTargetItems cItems;
	AheSelectByShapeFromFeatureClass(ipBuff, cItems, m_ipRoadLinkFC);

	// �x���W�����瓹�H�����N�܂ł̍ŒZ���������߂�
	ResultMap resultMap;
	for(const auto& link : cItems)
	{
		// �x���W���ɍł��߂����H�����N��̃|�C���g���擾
		IProximityOperatorPtr ipProp(link.GetShape());
		IPointPtr ipWarningSignPoint(ipWarningSignGeom);
		IPointPtr ipNearestPoint;
		ipProp->ReturnNearestPoint(ipWarningSignPoint, esriNoExtension, &ipNearestPoint);
		if(!ipNearestPoint)
			continue;

		ipNearestPoint->putref_SpatialReference(AheGetSpatialReference(ipWarningSignPoint));
		double dDist = AheGetMeterLength(ipWarningSignPoint, ipNearestPoint);
		resultMap.emplace(dDist, link);
	}

	// �O���b�h���X�V
	UpdateSearchRoadLinkGrid(resultMap);
}

/**
* @brief ���H�����N�������̎w�苗�����`�F�b�N
* @note ��肪�Ȃ���΂��̂܂܌��������Ƃ��ĕێ�
*
* @return �G���[���b�Z�[�W
*/
CString CAttrWarningSignDlg::CheckSearchMeter()
{
	CString strMeter;
	GetDlgItemText(IDC_EDIT_SEARCH_METER, strMeter);

	// �󗓂łȂ���
	if(strMeter.IsEmpty())
		return AheLoadString(IDS_ERR_SEARCH_METER_EMPTY);

	// �����񁨐��l�ϊ�
	long lSearchDist = _tstol(strMeter);

	// �}�C�i�X�l�łȂ���
	if(lSearchDist < 0)
		return AheLoadString(IDS_ERR_SEARCH_METER_MINUS);

	// ����l�𒴂��Ă��Ȃ���
	if(lSearchDist > MAX_SEARCH_METER)
		return format(AheLoadString(IDS_ERR_SEARCH_METER_OVER_LIMIT), MAX_SEARCH_METER);

	// ���Ȃ���Ό��������Ƃ��ăZ�b�g���A���CString��Ԃ�
	m_lSearchDist = lSearchDist;
	return CString();
}

/**
 * @brief �x���W����ʂ̃`�F�b�N
 * @param	featureDef	[in]		�`�F�b�N�Ώۂ̃t�B�[�`��
 * @param	strErr		[out]		�G���[���b�Z�[�W
 *
 * @retval  TRUE  �t�B�[���h�T�C�Y�𒴂��Ă��Ȃ�
 * @retval  FALSE �t�B�[���h�T�C�Y�𒴂��Ă���
 */
BOOL CAttrWarningSignDlg::CheckWarningSign(CFeatureDef& cFeatureDef, CString& strErr)
{
	CFieldDef* pWarningSignDef = cFeatureDef.GetFieldDef(warning_sign::kTableName, warning_sign::kWarningSign);
	auto mapWarningSignDomain = pWarningSignDef->GetDomain();
	long lWarningSign = pWarningSignDef->m_vaValue.lVal;
	auto itr = find_if(mapWarningSignDomain->begin(), mapWarningSignDomain->end(),
		[&lWarningSign](const CODEVALUE_DOMAIN& domain){ return domain.code.lVal == lWarningSign; });
	if(lWarningSign == warning_sign::warning_sign_class::kNone || itr == mapWarningSignDomain->end())
	{
		strErr += format(AheLoadString(IDS_ERR_WARNING_SIGN_VALUE_NOT_EXIST), warning_sign::kWarningSign, lWarningSign);
		return FALSE;
	}
	
	return TRUE;
}