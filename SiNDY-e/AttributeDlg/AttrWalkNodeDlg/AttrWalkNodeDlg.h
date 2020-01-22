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

/* MIDL: ���̖��񐶐������t�@�C���ɂ̓C���^�[�t�F�C�X�̒�`���܂܂�Ă��܂� */
// AttrWalkNodeDlgDlg.h : CAttrWalkNodeDlgDlg �̐錾
#pragma once

#include "resource.h"
#include "globalfunc.h"
#include "ErrorCode.h"

typedef CAttrBaseDlg ATTR_BASE_CLASS;


/////////////////////////////////////////////////////////////////////////////
// CAttrWalkLinkDlg
class CAttrWalkNodeDlg :
	public ATTR_BASE_CLASS,
	public CExportDlg
{
	// �n���X�o�����|�C���g�̃R���e���c�R�[�h
	static const long UNDERGROUND_GATE_CONTENTS_CODE = 289000001L;

public:
	CAttrWalkNodeDlg();
	~CAttrWalkNodeDlg()
	{}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrWalkNodeDlgDlg ���b�Z�[�W�}�b�v
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MSG_MAP(CAttrWalkNodeDlg)
	COMMAND_HANDLER(IDC_BUTTON_SEARCHSTATIONGATE, BN_CLICKED, OnClkSearchBtn)
	COMMAND_HANDLER(IDC_BUTTON_SEARCHUNDERGROUNDGATE, BN_CLICKED, OnClkSearchBtn)
	COMMAND_HANDLER(IDC_BUTTON_SEARCHROADNODE, BN_CLICKED, OnClkSearchBtn)
	COMMAND_HANDLER(IDC_BUTTON_INSERT, BN_CLICKED, OnClkInsertBtn)
	COMMAND_HANDLER(IDC_DELETE_ROADNODEID, BN_CLICKED, OnClickedDeleteButton)
	COMMAND_HANDLER(IDC_DELETE_STATIONGATEID, BN_CLICKED, OnClickedDeleteButton)
	COMMAND_HANDLER(IDC_DELETE_UNDERGROUNDGATEID, BN_CLICKED, OnClickedDeleteButton)
	NOTIFY_HANDLER(IDC_LIST_RESULT, NM_CLICK, OnClkList)
	MESSAGE_HANDLER(WM_WINMGR, OnWinMgr)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
	MESSAGE_HANDLER(WM_CTLCOLOREDIT, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORBTN, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_COMMAND, OnCommand)
	MESSAGE_HANDLER(WM_SIZE, OnSize)
	
ALT_MSG_MAP(IDC_COMBO_WALKNODECLASS)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_ROADNODEID)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDownEditID) // ID���蓮���͂����ꍇ�̏���
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
	MESSAGE_HANDLER(WM_KILLFOCUS, OnEditFocusOut) // �t�H�[�J�X���O�ꂽ�Ƃ��̏���
ALT_MSG_MAP(IDC_BUTTON_SEARCHROADNODE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_DELETE_ROADNODEID)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_STATIONGATE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDownEditID)  // ID���蓮���͂����ꍇ�̏���
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
	MESSAGE_HANDLER(WM_KILLFOCUS, OnEditFocusOut) // �t�H�[�J�X���O�ꂽ�Ƃ��̏���
ALT_MSG_MAP(IDC_BUTTON_SEARCHSTATIONGATE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_DELETE_STATIONGATEID)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_UNDERGROUNDGATE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDownEditID)  // ID���蓮���͂����ꍇ�̏���
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
	MESSAGE_HANDLER(WM_KILLFOCUS, OnEditFocusOut) // �t�H�[�J�X���O�ꂽ�Ƃ��̏���
ALT_MSG_MAP(IDC_BUTTON_SEARCHUNDERGROUNDGATE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_DELETE_UNDERGROUNDGATEID)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_SEARCH)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_INSERT)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_LIST_RESULT)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_SOURCE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
END_MSG_MAP()

/////////////////////////////////////////////////////////////////////////////
//
// CAttrWalkNodeDlg ���b�Z�[�W�n���h��
//
/////////////////////////////////////////////////////////////////////////////

	/**
	 * �_�C�A���O�쐬����ۂɈ�ԍŏ��ɌĂ΂�܂��B�����Ń_�C�A���O�̏����������Ă�������
	 */
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// �R���g���[��
		CreateControlRelation();

		// �T�u�N���X��
		for( auto& subclass : m_mapSubClass)
		{
			subclass.second.SubclassWindow( GetDlgItem( subclass.first ) );
		}

		// �G�f�B�b�g�{�b�N�X�ɏ����l��ݒ�
		// �ύX������OnCommand���Ă΂�ė�����̂ŁAOnCommand�̋�������U�~�߂�
		m_bFinishInit = false;
		CString strMeter;
		strMeter.Format(_T("%ld"), 100);
		GetDlgItem( IDC_EDIT_SEARCH ).SetWindowText( strMeter );
		m_bFinishInit = true;

		return ATTR_BASE_CLASS::OnInitDialog( uMsg, wParam, lParam, bHandled );
	}

	/**
	 * WM_COMMAND �p�C�x���g�n���h��
	 *
	 * �e�R�����R���g���[���ŕύX���������ꍇ�A�����ŏ������܂�
	 * ���݂̓G�f�B�b�g�{�b�N�X�A�`�F�b�N�{�b�N�X�A�R���{�{�b�N�X�̏������s���܂�
	 */
	LRESULT OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// SetCurrentFeatureDefIndex() �̍Œ��̓L�����Z���i�^�C�~���O�ɂ���Ă͂��������Ȃ邽�߁j
		if( !m_bFinishInit ) return 0;

		INT msg = HIWORD(wParam);						// ���상�b�Z�[�W
		INT	nTargetControl = (INT)LOWORD(wParam);		// ����ΏۃR���g���[��

		// �G�f�B�b�g�{�b�N�X�̎��ɑS�I���ɂ���
		if( msg == EN_SETFOCUS )
			SelectDlgItem( nTargetControl );
		// �R���{�{�b�N�X�̃��X�g�{�b�N�X�̒����𒲐�
		if( msg == CBN_DROPDOWN )
			SetComboboxList( nTargetControl );

		// �R���{�{�b�N�X�A�G�f�B�b�g�{�b�N�X�A�`�F�b�N�{�b�N�X�̃��b�Z�[�W����
		if( ( msg == CBN_SELCHANGE ) || ( msg == BN_CLICKED ) || ( msg == EN_UPDATE ) ) {
			// �ύX���ꂽ���ǂ����`�F�b�N���A�ύX����Ă����瑼�̃R���g���[���ɂ����f������
			m_cControlRel.ControlChanged( (INT)LOWORD(wParam) );
			InvalidateRect( NULL, FALSE );
			// �ύX���ꂽ�ꍇ�̓{�^���� Enable ��
			SetButton( Changed() );

			// �m�[�h��ʂɂ��ҏW�ۂ��`�F�b�N���邽�߃R���{�{�b�N�X���Ď�
			if( IDC_COMBO_WALKNODECLASS == nTargetControl )
				// �����܂ŏ���������Ƃ������Ƃ͕ҏW�J�n����Ă����ԂƂ�����̂�
				// �����ɂ�true������
				SetControlState(true);
		}
		return 0;
	}

	/**
	 * @brief �֘AID�G�f�B�b�g�{�b�N�X�ŃL�[�������ꂽ�Ƃ��̏���
	 */
	LRESULT OnKeyDownEditID( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled )
	{
		using namespace sindy::schema;
		// Enter�L�[��������邩�A�e��TRUE���Ԃ��ꂽ=�t�H�[�J�X�ύX���ꂽ�Ƃ������ƂŖ��̂�����
		if( wParam == VK_RETURN || OnKeyDown(uMsg, wParam, lParam, bHandled))
		{
			SetIDName( road_node::kTableName, IDC_EDIT_ROADNODEID, IDC_STATIC_CROSSINGNAME2);
			SetIDName( station_gate_point::kTableName, IDC_EDIT_STATIONGATE, IDC_STATIC_STATIONGATE_NAME);
			SetIDName( sindyk::poi_point_org::kTableName, IDC_EDIT_UNDERGROUNDGATE, IDC_STATIC_UNDERGROUNDGATE_NAME);
			bHandled = TRUE;
			// �s���ȓ��̓f�[�^������΃G���[�t���O�𗧂Ă�
			CheckInvaliedValue();
			return TRUE;
		}
		return FALSE;
	}

	/**
	 * @brief �֘AID�G�f�B�b�g�{�b�N�X����t�H�[�J�X���O�ꂽ�Ƃ��̏���
	 */
	LRESULT OnEditFocusOut( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled )
	{
		using namespace sindy::schema;
		SetIDName( road_node::kTableName, IDC_EDIT_ROADNODEID, IDC_STATIC_CROSSINGNAME2);
		SetIDName( station_gate_point::kTableName, IDC_EDIT_STATIONGATE, IDC_STATIC_STATIONGATE_NAME);
		SetIDName( sindyk::poi_point_org::kTableName, IDC_EDIT_UNDERGROUNDGATE, IDC_STATIC_UNDERGROUNDGATE_NAME);
		// �s���ȓ��̓f�[�^������΃G���[�t���O�𗧂Ă�
		CheckInvaliedValue();
		bHandled = FALSE;
		return FALSE;
	}

	/**
	 * @brief �����{�^���������ꂽ�Ƃ��C�x���g�n���h��
	 * @note �����ꂽ�{�^���ɍ��킹�āA�w�肳�ꂽ�͈͂Ŏ��ӌ�������
	 */
	LRESULT OnClkSearchBtn( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled )
	{
		using namespace  sindy::schema;

		// ���̃J������̕\���Ǝ��ӌ����Ώۂ͉����ꂽ�{�^���ɂ���Đؑւ���
		CString whereClause;
		switch(wID)
		{
		case IDC_BUTTON_SEARCHSTATIONGATE:
			// Msg �w�o��������, �w�o�������ӌ�������
			ReplaceListViewNameColumn(IDS_STATIONGATECOLUMN, IDS_STATIONGATERESULT);
			SearchAroundPoint(station_gate_point::kTableName, station_gate_point::kName, station_gate_point::kYomi);
			break;
		case IDC_BUTTON_SEARCHUNDERGROUNDGATE:
			// Msg �n���X�o��������, �n���X�o�������ӌ�������
			ReplaceListViewNameColumn(IDS_UNDERGROUNDGATECOLUMN, IDS_UNDERGROUNDGATERESULT);
			// [bug 11559]�������Ă�����̂�������������
			whereClause.Format(_T("%s = %ld and %s = %d")
				, sindyk::poi_point_org::kContentsCode, UNDERGROUND_GATE_CONTENTS_CODE
				, sindyk::poi_point_org::kDelete, sindyk::poi_point_org::delete_code::kAlive );
			SearchAroundPoint(sindyk::poi_point_org::kTableName, sindyk::poi_point_org::kName, sindyk::poi_point_org::kYomi, whereClause );
			break;
		case IDC_BUTTON_SEARCHROADNODE:
			// Msg �����_����, �����_���ӌ�������
			ReplaceListViewNameColumn(IDS_CROSSNODECOLUMN, IDS_CROSSRESULT);
			SearchAroundPoint(road_node::kTableName, road_node::kNameKanji, road_node::kNameYomi);
			break;
		default:
			break;
		}
		return 0;
	}

	/**
	 * @brief ���X�g�r���[��ō��N���b�N�����ꍇ�̏���
	 * @note �I�����ꂽ���X�g�ɑΉ�����|�C���g�t�B�[�`����_�ł�����
	 */
	LRESULT OnClkList( int idCtrl, LPNMHDR pnmh, BOOL& bHandled )
	{
		// ���X�g�r���[�őI������Ă���t�B�[�`�������点��
		BlinkPointFeature();
		return 0;
	}

	/**
	 * @brief [ << ]�{�^���i�K�p�j�������ꂽ�ꍇ�̏���
	 */
	LRESULT OnClkInsertBtn( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled )
	{
		using namespace  sindy::schema;

		CListViewCtrl lvResult( GetDlgItem(IDC_LIST_RESULT) );
		int nCurIdx = lvResult.GetSelectedIndex();
		if( -1 == nCurIdx )
		{
			// Msg �������ʃ��X�g����Y�����R�[�h��I�����Ă��������B
			AttrMessageBox( AheLoadString( IDS_SELECTRECORD ), AheLoadString( IDS_CAPTION ), MB_OK, NULL );
			return 0;
		}
		InsertToEditBox(nCurIdx);
		return 0;
	}

	/**
	 * @brief �폜�{�^���������ꂽ�Ƃ��̏���
	 */
	LRESULT OnClickedDeleteButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		switch(wID)
		{
		case IDC_DELETE_ROADNODEID: // �����_�m�[�h�p�̍폜�{�^���������ꂽ
			SetDlgItemText( IDC_EDIT_ROADNODEID, _T("") );
			SetDlgItemText( IDC_STATIC_CROSSINGNAME2, _T("") );
			break;
		case IDC_DELETE_STATIONGATEID: // �w�o�����p�̍폜�{�^���������ꂽ
			SetDlgItemText( IDC_EDIT_STATIONGATE, _T("") );
			SetDlgItemText( IDC_STATIC_STATIONGATE_NAME, _T("") );
			break;
		case IDC_DELETE_UNDERGROUNDGATEID: // �n���X�o�����p�̍폜�{�^���������ꂽ
			SetDlgItemText( IDC_EDIT_UNDERGROUNDGATE, _T("") );
			SetDlgItemText( IDC_STATIC_UNDERGROUNDGATE_NAME, _T("") );
			break;
		default:
			break;
		}
		// �e�L�X�g�{�b�N�X���ύX���ꂽ���ʁA�s���ȓ��̓f�[�^�ɂȂ��Ă���Ȃ�G���[�t���O�𗧂Ă�
		CheckInvaliedValue();
		return 0;
	}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrWalkNodeDlg �����o�֐���`
//
/////////////////////////////////////////////////////////////////////////////
public:
	virtual HWND Create(HWND hWndParent, LPARAM dwInitParam = NULL);
	virtual void Delete();
	virtual void SetArcHelper( IApplication* ipApp ){ ATTR_BASE_CLASS::SetArcHelper( ipApp ); };
	virtual void SetAliasOrField( BOOL bAliasOrField ){ ATTR_BASE_CLASS::m_cControlRel.SetAliasOrField( bAliasOrField ); };
	virtual void SetFeatureDefList( std::list<CLQRowDef>* pFeatureDefList ){};
	virtual void SetFeatureDefList( std::list<CFeatureDef>* pFeatureDefList ){ ATTR_BASE_CLASS::SetFeatureDefList( pFeatureDefList ); };
	virtual void ClearFeatureDefs(){ ATTR_BASE_CLASS::ClearFeatureDefs(); };
	virtual HWND GetDlg(){ return m_hChildWnd; };
	virtual LRESULT SendMessage( UINT message, WPARAM wParam, LPARAM lParam ){ return ::SendMessage( m_hWnd, message, wParam, lParam ); }; 
	virtual BOOL ErrorCheck();
	virtual LONG GetTabNum(){ return -1; };
	virtual void SetTabNum(LONG lTabNum){};
	virtual void SetFeatureClassName(LPCTSTR lpcszFeatureClassName){};

	BOOL SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable );
	inline BOOL CheckReturnKeyItem(INT nClassID) { return ATTR_BASE_CLASS::CheckReturnKeyItem(nClassID); };
	inline BOOL CheckEscKeyItem(INT nClassID) { return ATTR_BASE_CLASS::CheckEscKeyItem(nClassID); };

private:
	void CreateControlRelation();
	/**
	 * @brief �R���{�{�b�N�X�̑I����Ԃɍ��킹�āA�e�R���g���[���̓��͉ۂ�ύX
	 */
	void SetControlState(bool bEditable);

	/**
	 * @brief �w��t�B�[�`���N���X���擾
	 * @note �擾���s���ɃG���[���b�Z�[�W���|�b�v�A�b�v����
	 * 
	 * @param fcName	[in]	�擾����t�B�[�`���N���X��
	 * @param popupMsgTOCHide	[in]	TOC��\�����̃G���[���b�Z�[�W�̎��s�L��(�f�t�H���g�Ftrue)
	 * @retval nullptr�ȊO	fcName�̎擾�ɐ���
	 * @retval nullptr	fcName�̎擾�Ɏ��s
	 */
	IFeatureClassPtr GetFeatureClass( LPCTSTR fcName , bool popupMsgTOCHide=true );
		/**
		 * @breif �w��t�B�[�`���Ɠ����O���[�v���C���ɏ����������̃t�B�[�`���N���X���擾
		 * @note GetFeatureClass���ł̂ݎg�p
		 * @note ipSameGroupFeature�Ɠ����O���[�v���C�����ɂ���targetName���擾����
		 * @note ipSameGroupFeature�Ɠ����O���[�v���C������targetName�����݂��Ȃ��ꍇ��TOC�ォ��targetName���擾����
		 * @note ��\�����C���̏ꍇ�͊�{�I�ɖ������邪�A��₪�S�Ĕ�\�����C���ł���Ȃ�
		 * �֘A�t���ėǂ����A�|�b�v�A�b�v�Ń��[�U�Ɋm�F����
		 * @note ��₪��������ꍇ�́A�ŏ��Ɍ����������̂��擾���A���b�Z�[�W���|�b�v�A�b�v����
		 *
		 * @param targetName         [in] �擾����t�B�[�`���N���X��
		 * @param ipSameGroupFeature [in] �w��t�B�[�`��
		 * @param bMsgDisp           [in] ���b�Z�[�W��\�����邩�ۂ�
		 * @param bCancel            [in] �|�b�v�A�b�v�ŃL�����Z���������ꂽ���ۂ�
		 * @param popupMsgTOCHide [in] TOC��\�����̃G���[���b�Z�[�W�̎��s�L��(�f�t�H���g�Ftrue)
		 * @retval nullptr�ȊO targetName�̎擾�ɐ���
		 * @retval nullpr targetName�̎擾�Ɏ��s
		 */
		IFeatureClassPtr GetSameGroupFeatureClass( LPCTSTR targetName, const IFeaturePtr& ipSameGroupFeature, bool bMsgDisp, bool& bCancel, bool popupMsgTOCHide=true );

	/**
	 * @brief fcName����Ή�ID�����t�B�[�`�����������A���̖��̂�\��
	 * @note  �����_���́E�w�o������|�C���g�E�n���X�o�����|�C���g����nCodeID�ɓ��͂���Ă���ID�����t�B�[�`������������
	 * ��3�����Ŏ������R���g���[���ɕ\������
	 * @param fcName				[in] �����Ώۂ̃t�B�[�`���N���X
	 * @param nCodeID				[in] �Ή�ID�������Ă���G�f�B�b�g�R���g���[����ID
	 * @param nLabelID				[in] ���̂�\������X�^�e�B�b�N�R���g���[����ID
	*/
	void SetIDName(LPCTSTR fcName, INT nCodeID, INT nLabelID);

	/**
	 * @brief �������X�g�̎w�背�R�[�h�̗v�f���G�f�B�b�g�{�b�N�X�E�X�^�e�B�b�N�e�L�X�g�ɓ���
	 * @warning m_searchedLayer�ŕێ����Ă���t�B�[�`���N���X�ɍ��킹�ē��͐�̃G�f�B�b�g�{�b�N�X��ύX���Ă���
	 * ���̂��߁Am_searchedLayer���X�V����O��InsertToEditBox�͌Ă�ł͂Ȃ�Ȃ�
	 * @param nCurIdx [in] �}�����郊�X�g���̃��R�[�h�ԍ�
	 */
	void InsertToEditBox(int nCurIdx);

	/**
	 * @brief ���X�g�r���[�̖��̃J��������m�F���A�w�蕶���񂪓����Ă��Ȃ��ꍇ�͎w�蕶����ɒu��������
	 * @param colNameID [in] ���̂��i�[����J�����p��StringTableID
	 * @param resultID  [in] �������ʃ��׃��ɕ\������镶����p��StringTableID
	 */
	void ReplaceListViewNameColumn(UINT colNameID, UINT resultID);

	/**
	 * @brief �����m�[�h�̎��ӂɑ��݂���fcName�t�B�[�`���N���X�̃t�B�[�`�����������ă��X�g�r���[�ɕ\��
	 * @note �G�f�B�b�g�{�b�N�X�ɓ��͂��ꂽ���[�g�����A�����m�[�h�̃o�b�t�@���Ƃ���
	 * ����Ɋ܂܂��t�B�[�`�����擾����
	 * @param fcName        [in] �����Ώۃt�B�[�`��
	 * @param nameFieldName [in] �����Ώۃt�B�[�`���ɂ����閼�̃t�B�[���h�̖��O
	 * @param yomiFieldName [in] �����Ώۃt�B�[�`���ɂ�����ǂ݃t�B�[���h�̖��O
	 * @param whereClause   [in] ���������i�f�t�H���g�FNULL�j
	 */
	void SearchAroundPoint(LPCTSTR fcName, LPCTSTR nameFieldName, LPCTSTR yomiFieldName, LPCTSTR whereClause=NULL );

	/**
	 * @brief ���X�g�r���[�őI������Ă���t�B�[�`����_�ł�����
	 */
	void BlinkPointFeature();

	/**
	 * @brief �������ʕ\���p���X�g�r���[�̒��g���쐬
	 * @param nameColName [in] ���̂��i�[����J�����̖��O
	 * @param resultStr   [in] �������ʃ��׃��ɕ\������镶����
	 * �Ⴆ�΁A"�w�o������̌�������"�A"�����_�m�[�h�̌�������"�Ƃ������������n��
	 */
	void CreateListView(LPCTSTR nameColName, LPCTSTR resultStr);

	/**
	 * @brief �G�f�B�b�g�{�b�N�X�̓��͂ɕs�����Ȃ����`�F�b�N
	 */
	void CheckInvaliedValue();
		/**
		 * @brief POI�Ή�ID�̓��͏�Ԃ��݂āA�G���[�t���O��ONOFF�𐧌�
		 * @note CheckInvaliedValue()���ł̂ݎg�p
		 * @note �G���[���������ꍇ�ɂ́A�G���[���e��m_errors��Push����
		 * @param nodeClass [in] �R���{�{�b�N�X�őI������Ă���m�[�h���
		 * @param nID [in] �Ή�ID�������Ă���G�f�B�b�g�R���g���[����ID
		 */
		void CheckCommonGateCtrl( DWORD_PTR nodeClass, INT nID);
		/**
		 * @brief �w�o�����Ή�ID�̓��͏�Ԃ��݂āA�G���[�t���O��ONOFF�𐧌�
		 * @note CheckInvaliedValue()���ł̂ݎg�p
		 * @note �G���[���������ꍇ�ɂ́A�G���[���e��m_errors��Push����
		 * @param nodeClass [in] �R���{�{�b�N�X�őI������Ă���m�[�h���
		 */
		void CheckStationGateCtrl( DWORD_PTR nodeClass );
		/**
		 * @brief �n���X�o�����Ή�ID�̓��͏�Ԃ��݂āA�G���[�t���O��ONOFF�𐧌�
		 * @note CheckInvaliedValue()���ł̂ݎg�p
		 * @note �G���[���������ꍇ�ɂ́A�G���[���e��m_errors��Push����
		 * @param nodeClass [in] �R���{�{�b�N�X�őI������Ă���m�[�h���
		 */
		void CheckUndergroundGateCtrl( DWORD_PTR nodeClass);
		/**
		 * @brief �����_�m�[�h�Ή�ID�̓��͏�Ԃ��݂āA�G���[�t���O��ONOFF�𐧌�
		 * @note CheckInvaliedValue()���ł̂ݎg�p
		 * @note �G���[���������ꍇ�ɂ́A�G���[���e��m_errors��Push����
		 * @param nodeClass [in] �R���{�{�b�N�X�őI������Ă���m�[�h���
		 */
		void CheckCrossNodeCtrl(DWORD_PTR nodeClass);

	/**
	 * @brief �t�B�[�`���Ƀt�B�[���h�����݂��邩�`�F�b�N
	 * @param	fcName		[in] �����Ώۃt�B�[�`��
	 * @param	fieldName	[in] �����Ώۃt�B�[���h
	 * @return	true		���݂���
	 * @return	false		���݂��Ȃ�
	 */
	bool CheckExistField( LPCTSTR fcName, LPCTSTR fieldName );

/////////////////////////////////////////////////////////////////////////////
//
// CAttrWalkNodeDlg �����o�ϐ���`
//
/////////////////////////////////////////////////////////////////////////////
private:
	std::map<int, CContainedWindow> m_mapSubClass; //!< �e�R���g���[���E�B���h�E

	HWND  m_hChildWnd;  //!< �q�_�C�A���O�n���h��
	HFONT m_hFont;
	static LVCOLUMN m_cResultColInfo[];		//!< �������ʃ��X�g�r���[�J�������
	static const int m_ListViewColumnSize = 2; //!< ���X�g�r���[�̃J������

	/**
	 * @brief �����m�[�h�t�B�[�`���Ƃ���Ɋ֘A�t������|�C���g�t�B�[�`���N���X�̑Ή��\
	 * @note �����_�C�A���O���J���Ď��ӌ���������Ԃ�TOC���猟���Ώۃt�B�[�`���������ꂽ�Ƃ��̔���Ɏg�p
	 * @note ���݁A���X�g�r���[�ɕ\������Ă���t�B�[�`���N���X����肷�邽�߂Ɏg�p
	 */
	std::map<IFeaturePtr,IFeatureClassPtr> m_searchedLayer;

	/**
	 * @brief �����m�[�h�t�B�[�`���ƃG���[�ێ���Ԃ̑Ή��\
	 * @note �����̃t�B�[�`����I�����Ă���Ƃ��A����t�B�[�`����s���ȓ��͂������Ԃɂ�
	 * �ʃt�B�[�`���ɐ؂�ւ��āA���̃t�B�[�`���̓��͂𐳏�Ȃ��̂ɂ��Ă���OK��������
	 * ��ڂ̃t�B�[�`���̑����Ƃ��ĕs���ȓ��͂��ۑ�����Ă��܂��̂�
	 * �t�B�[�`�����Ƃ̃G���[����ێ�����}�b�v��p�ӂ���ErroCheck�Ŏg�p���Ă���
	 * @note �G���[�����邩�ǂ��������ł͓K�؂ȃG���[�����o�͂ł��Ȃ����߁A
	 * �G���[�R�[�h��ێ�����ErrorCodeKeeper��v�f�Ƃ��Ă���
	 * TODO: Global��HNP���Q�ƁB
	 */
	std::map<IFeaturePtr,ErrorCodeKeeper> m_errors;
};
