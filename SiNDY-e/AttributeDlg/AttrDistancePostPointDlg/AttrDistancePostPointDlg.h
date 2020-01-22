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

// AttrDistancePostPointDlg.h : CAttrDistancePostPointDlg �̐錾

#pragma once

#include "resource.h"
#include "../HwyStartTool/HwyHelper.h"
#include "../../LIB/WinLib/str2.h"

typedef CHetCVDMap CCodeValueDomain;	//!< �R�[�h�h���C�����i�[����
typedef CAttrBaseDlg ATTR_BASE_CLASS;

/////////////////////////////////////////////////////////////////////////////
// CAttrDistancePostPointDlg
class CAttrDistancePostPointDlg :
	public ATTR_BASE_CLASS,
	public CExportDlg
{
public:
	CAttrDistancePostPointDlg();
	~CAttrDistancePostPointDlg()
	{
	}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrDistancePostPointDlg ���b�Z�[�W�}�b�v
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MSG_MAP(CAttrDistancePostPointDlg)
	MESSAGE_HANDLER(WM_WINMGR, OnWinMgr)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
	MESSAGE_HANDLER(WM_CTLCOLOREDIT, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORBTN, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorEdit)
	COMMAND_HANDLER(IDC_COMBO_ROADCLASS_REF, CBN_SELCHANGE, OnSelchangeRsel_class)
	COMMAND_HANDLER(IDC_COMBO_ROADNAME_REF, CBN_SELCHANGE, OnSelchangeRsel_Name)
	COMMAND_HANDLER(IDC_COMBO_DIRECTIONCODE, CBN_SELCHANGE, OnSelchangeDirection)
	MESSAGE_HANDLER(WM_COMMAND, OnCommand)
	MESSAGE_HANDLER(WM_SIZE, OnSize)
ALT_MSG_MAP(IDC_COMBO_ROADCLASS_REF)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_ROADNAME_REF)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_DIRECTIONCODE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_KILOPOST)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_KILOPOST2)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_SOURCE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
END_MSG_MAP()

/////////////////////////////////////////////////////////////////////////////
//
// CAttrDistancePostPointDlg ���b�Z�[�W�n���h��
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
		m_wnd0.SubclassWindow(GetDlgItem(IDC_COMBO_ROADCLASS_REF));
		m_wnd1.SubclassWindow(GetDlgItem(IDC_COMBO_ROADNAME_REF));
		m_wnd2.SubclassWindow(GetDlgItem(IDC_COMBO_DIRECTIONCODE));
		m_wnd3.SubclassWindow(GetDlgItem(IDC_EDIT_KILOPOST));
		m_wnd4.SubclassWindow(GetDlgItem(IDC_EDIT_SOURCE));
		m_wnd5.SubclassWindow(GetDlgItem(IDC_EDIT_KILOPOST2));

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
		if( ( msg == CBN_SELCHANGE ) || ( msg == BN_CLICKED ) || ( msg == EN_UPDATE )  ) 
		{
			// �ύX���ꂽ���ǂ����`�F�b�N���A�ύX����Ă����瑼�̃R���g���[���ɂ����f������
			m_cControlRel.ControlChanged( (INT)LOWORD(wParam) );
			InvalidateRect( NULL, FALSE );
			// �ύX���ꂽ�ꍇ�̓{�^���� Enable ��
			SetButton( Changed() );
		}

		return 0;
	}

	/**
	 * �H����ʎQ�Ɨp���ύX���ꂽ�Ƃ��̃C�x���g�n���h��
	 *
	 * �H�����̎Q�Ɨp�ƁA�H���R�[�h�̒l��ύX���܂�
	 */
	LRESULT OnSelchangeRsel_class(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled)
	{
		// �I�����ꂽ�H����ʂ̎擾
		CComboBox cbLineClass = CComboBoxT<CWindow>(GetDlgItem(IDC_COMBO_ROADCLASS_REF));
		int nIdx = cbLineClass.GetCurSel();
		if(nIdx == CB_ERR)
			return 0;
		m_nLineClass = cbLineClass.GetItemData(nIdx);
		// �H���R�[�h�̍ĕ`��
		if(! RedrawRoadNameList(0))
			return 0;
		// �H���R�[�h�̐ݒ�
		if(! SetRoadCode(m_nRoadCode))
			return 0;

		return 0;
	}

	/**
	 * �H�����̂��ύX���ꂽ�Ƃ��̃C�x���g�n���h��
	 *
	 * �H���R�[�h�̒l��ύX���܂�
	 */
	LRESULT OnSelchangeRsel_Name(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		// �I�����ꂽ�H���R�[�h�̎擾
		CComboBox cbRoadName = CComboBoxT<CWindow>(GetDlgItem(IDC_COMBO_ROADNAME_REF));
		int nIdx = cbRoadName.GetCurSel();
		if(nIdx == CB_ERR)
			return 0;
		m_nRoadCode = cbRoadName.GetItemData(nIdx);
		// �H���R�[�h�̐ݒ�
		if(! SetRoadCode(m_nRoadCode))
			return 0;

		return 0;
	}

	/**
	 * �����R�[�h���ύX���ꂽ�Ƃ��̃C�x���g�n���h��
	 *
	 * �����R�[�h�̒l��ύX���܂�
	 */
	LRESULT OnSelchangeDirection(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		// �I�����ꂽ�H���R�[�h�̎擾
		CComboBox cbDirection = CComboBoxT<CWindow>(GetDlgItem(IDC_COMBO_DIRECTIONCODE));
		int nIdx = cbDirection.GetCurSel();
		if(nIdx == CB_ERR)
			return 0;
		m_nDirCode = cbDirection.GetItemData(nIdx);

		CFieldDef* pFieldDef = GetCurrentFeatureDef()->GetFieldDef( sindy::schema::distance_post_point::kTableName, sindy::schema::category::highway_direction_table::kDirection );
		pFieldDef->m_vaValue = m_nDirCode;
		if(m_nDirCode != m_lOrgDirCode)
		{
			pFieldDef->m_bChanged = TRUE;	
			SetButton( Changed() );
		}

		return 0;
	}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrDistancePostPointDlg �����o�֐���`
//
/////////////////////////////////////////////////////////////////////////////
public:
	/** 
	 * @fn HWND CAttrDistancePostPointDlg::Create(HWND hWndParent, LPARAM dwInitParam)
	 * @brief �q�_�C�A���O���쐬���܂��B
	 *
	 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)�Ń_�C�A���O���쐬���܂��B
	 *
	 * @param hWndParent [in] �e�E�B���h�E�n���h��
	 * @param dwInitParam [in] �_�C�A���O�������p�����[�^�i���ݓ��Ɏg�p���Ă��܂���j
	 *
	 * @retval �쐬���ꂽ�q�_�C�A���O�̃n���h����Ԃ��܂��B
	 */
	virtual HWND Create(HWND hWndParent, LPARAM dwInitParam = NULL);

	/**
	 * @fn void CAttrDistancePostPointDlg::Delete()
	 * @brief �쐬�����q�_�C�A���O���폜���܂��B
	 *
	 * �_�C�A���O���폜����Ƃ��ɕK���Ă�ŉ������B�Ă΂Ȃ��ƃ��������[�N���Ă��܂��܂��B
	 */
	virtual void Delete();

	/**
	 * @fn void CAttrDistancePostPointDlg::SetArcHelper( IApplication* ipApp )
	 * @brief ATTR_BASE_CLASS��ArcHelper��n���܂��A�܂��AHwyHelper�ɂ�ArcHelper���Z�b�g���܂��B
	 */
	virtual void SetArcHelper( IApplication* ipApp );
	virtual void SetAliasOrField( BOOL bAliasOrField ){ ATTR_BASE_CLASS::m_cControlRel.SetAliasOrField( bAliasOrField ); };
	virtual void SetFeatureDefList( std::list<CLQRowDef>* pFeatureDefList ){};
	virtual void SetFeatureDefList( std::list<CFeatureDef>* pFeatureDefList ){ ATTR_BASE_CLASS::SetFeatureDefList( pFeatureDefList ); };
	virtual void ClearFeatureDefs(){ ATTR_BASE_CLASS::ClearFeatureDefs(); };
	virtual HWND GetDlg(){ return m_hChildWnd; };
	virtual LRESULT SendMessage( UINT message, WPARAM wParam, LPARAM lParam ){ return ::SendMessage( m_hWnd, message, wParam, lParam ); }; 

	/**
	 * @fn BOOL CAttrDistancePostPointDlg::ErrorCheck()
	 * @brief �G���[���Ȃ����ǂ������ׂ�
	 *
	 * �_�C�A���O�����ۂɁAAttributeDlgObj����Ă΂��
	 * �{�ݏ�񖼏̂Ǝ{�ݏ��J�i���̂������Ƒ΂ɂȂ��ė��������Ă��邩�A�܂��͓����Ă��Ȃ����������߂�
	 *
	 * @return	�G���[���Ȃ�������TRUE�A�G���[�������FALSE
	 */
	virtual BOOL ErrorCheck();
	virtual LONG GetTabNum(){ return -1; };
	virtual void SetTabNum(LONG lTabNum){};
	virtual void SetFeatureClassName(LPCTSTR lpcszFeatureClassName){};

	/**
	 * @fn BOOL CAttrDistancePostPointDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
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
	BOOL SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable );
	inline BOOL CheckReturnKeyItem(INT nClassID) { return ATTR_BASE_CLASS::CheckReturnKeyItem(nClassID); };
	inline BOOL CheckEscKeyItem(INT nClassID) { return ATTR_BASE_CLASS::CheckEscKeyItem(nClassID); };

private:
	/**
	 * @fn void CAttrDistancePostPointDlg::CreateControlRelation()
	 * @brief �q�_�C�A���O�����ۂɍ쐬���܂��B
	 *
	 * ���ۂ̍쐬��ControlRel�ɔC���܂��B
	 */
	void CreateControlRelation();

	/**
	 * @fn bool CAttrDistancePostPointDlg::SetLineClassList(LONG nLineClass)
	 * @brief �_�C�A���O���Ă΂ꂽ�Ƃ��ɘH����ʎQ�Ɨp�̃R���{�{�b�N�X�ɒl���Z�b�g����
	 *
	 * @param nLineClass [in] �H���R�[�h�̒l
	 *
	 * @return �H����ʂ��擾�ł�����TRUE�A�擾�ł��Ȃ�������FALSE
	 */
	bool SetLineClassList(int nLineClass);

	/**
	 * @fn bool CAttrDistancePostPointDlg::GetLineClass(int* pClass)
	 * @brief �ݒ肳��Ă���H����ʂ��擾����
	 *
	 * @param pClass [out] �H���R�[�h�̒l
	 *
	 * @return �H����ʂ��擾�ł�����TRUE�A�擾�ł��Ȃ�������FALSE
	 */
	bool GetLineClass(int* pClass);

	/**
	 * @fn bool CAttrDistancePostPointDlg::SetLineClass(int nClass)
	 * @brief �H����ʂ̒l����R���{�{�b�N�X��ݒ�
	 *
	 * @param nClass [in] �H���R�[�h�̒l
	 *
	 * @return �H����ʂ��擾�ł�����TRUE�A�擾�ł��Ȃ�������FALSE
	 */
	bool SetLineClass(int nClass);

	/**
	 * @fn bool CAttrDistancePostPointDlg::SetLineClassList(LONG nLineClass)
	 * @brief �_�C�A���O���Ă΂ꂽ�Ƃ��ɕ����R�[�h�p�̃R���{�{�b�N�X�ɒl���Z�b�g����
	 *
	 * @param nDir [in] �����R�[�h�̒l
	 */
	void SetDirList(int nDir);

	/**
	 * @fn bool CAttrDistancePostPointDlg::RedrawRoadNameList(LONG nRoadCode)
	 * @brief �H�����̃R���{�{�b�N�X�̍X�V
	 *
	 * @param nRoadCode [in] �H���R�[�h�̒l
	 *
	 * @return �H�����̂��擾�ł�����TRUE�A�擾�ł��Ȃ�������FALSE
	 */
	bool RedrawRoadNameList(int nRoadCode);

	/**
	 * @fn bool CAttrDistancePostPointDlg::GetRoadCode(int* pRoadCode)
	 * @brief �H���R�[�h�G�f�B�b�g�{�b�N�X����H���R�[�h�̎擾
	 *
	 * @param nRoadCode [out] �H���R�[�h�̒l
	 *
	 * @return �H���R�[�h���擾�ł�����TRUE�A�擾�ł��Ȃ�������FALSE
	 */
	bool GetRoadCode(int* pRoadCode);

	/**
	 * @fn bool CAttrDistancePostPointDlg::GetRoadCode(int* pRoadCode)
	 * @brief �H���R�[�h�G�f�B�b�g�{�b�N�X����H���R�[�h�̎擾
	 *
	 * @param nRoadCode [in] �H���R�[�h�̒l
	 *
	 * @return �H���R�[�h���擾�ł�����TRUE�A�擾�ł��Ȃ�������FALSE
	 */
	bool SetRoadCode(int nRoadCode);

	/**
	* @brief �H���R�[�h�e�[�u���擾
	* @return �H���R�[�h�e�[�u��
	*/
	ITablePtr GetRoadCodeTable();

	/**
	* @brief �H���R�[�h�擾
	* @return �H���R�[�h���擾�ł������ǂ���
	*/
	bool GetRoadCodeSet(CHetRoadCodeSet* pCodeSet);

////////////////////////////////////////////////////////////////////////////
//
// CAttrDistancePostPointDlg �����o�ϐ���`
//
/////////////////////////////////////////////////////////////////////////////
private:
	CContainedWindow m_wnd0,m_wnd1,m_wnd2,m_wnd3,m_wnd4,m_wnd5;

	CString m_cRoadCode;                    //!< ������ł̘H���R�[�h

	// ���̒l
	long m_lOrgRoadCode;                    //!< �ŏ��̘H���R�[�h
	long m_lOrgDirCode;                     //!< �ŏ��̕����R�[�h

	// �_�C�A���O�̒l�ۑ��p�f�[�^
	int m_nLineClass;                       //!< ���ݑI�𒆂̘H����ʃR�[�h
	int m_nRoadCode;                        //!< ���ݑI�𒆂̘H���R�[�h
	int m_nDirCode;                         //!< ���ݑI�𒆂̕����R�[�h

	// �n�C�E�F�C�ҏW�p�e��f�[�^�̃R���e�i
	CHetRoadCodeSet m_cRoadCodeSet;         //!< �H���R�[�h���R���e�i
	HWND m_hChildWnd;                       //!< �q�_�C�A���O�n���h��
};
