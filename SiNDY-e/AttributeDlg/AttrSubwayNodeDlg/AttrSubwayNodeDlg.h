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

#include "SearchListCtrls.h"


typedef CAttrBaseDlg ATTR_BASE_CLASS;
/////////////////////////////////////////////////////////////////////////////
// CAttrSubwayNodeDlg
class CAttrSubwayNodeDlg : 
	public ATTR_BASE_CLASS,
	public CExportDlg
{
public:
/////////////////////////////////////////////////////////////////////////////
//
// CAttrSubwayNodeDlg ���b�Z�[�W�}�b�v
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MSG_MAP(CAttrSubwayNodeDlg)
	COMMAND_HANDLER( IDC_LIST_STATION, LBN_SELCHANGE, OnSelChange )
	COMMAND_HANDLER( IDC_LIST_LINE, LBN_SELCHANGE, OnSelChange )
	COMMAND_ID_HANDLER( IDC_BUTTON_APPLY, OnClickedApply )
	MESSAGE_HANDLER(WM_WINMGR, OnWinMgr)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
	MESSAGE_HANDLER(WM_CTLCOLOREDIT, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORBTN, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_COMMAND, OnCommand)
	MESSAGE_HANDLER(WM_SIZE, OnSize)

// TODO[SiNDYChildDlg] ���b�Z�[�W�}�b�v
ALT_MSG_MAP(IDC_COMBO_CODECLASS)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_SOURCE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
END_MSG_MAP()

/////////////////////////////////////////////////////////////////////////////
//
// CAttrSubwayNodeDlg ���b�Z�[�W�n���h��
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
		for( auto& it : m_mapSubClass)
		{
			it.second.SubclassWindow( GetDlgItem( it.first ) );
		}


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

		INT msg = HIWORD(wParam);                   // ���상�b�Z�[�W
		INT nTargetControl = (INT)LOWORD(wParam);   // ����ΏۃR���g���[��

		// �G�f�B�b�g�{�b�N�X�̎��ɑS�I���ɂ���
		if( msg == EN_SETFOCUS )
			SelectDlgItem( nTargetControl );
		// �R���{�{�b�N�X�̃��X�g�{�b�N�X�̒����𒲐�
		if( msg == CBN_DROPDOWN )
			SetComboboxList( nTargetControl );

		// �R���{�{�b�N�X�A�G�f�B�b�g�{�b�N�X�A�`�F�b�N�{�b�N�X�̃��b�Z�[�W����
		if( ( msg == CBN_SELCHANGE ) || ( msg == BN_CLICKED ) || ( msg == EN_UPDATE )  ) {
			// �ύX���ꂽ���ǂ����`�F�b�N���A�ύX����Ă����瑼�̃R���g���[���ɂ����f������
			m_cControlRel.ControlChanged( (INT)LOWORD(wParam) );
			// �_�C�A���O���X�V�̈�ɒǉ����܂�
			InvalidateRect( NULL, FALSE );
			// �ύX���ꂽ�ꍇ�̓{�^���� Enable ��
			SetButton( Changed() );
		}

		return 0;
	}

	LRESULT OnSelChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedApply(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

private:

/////////////////////////////////////////////////////////////////////////////
//
// CAttrSubwayNodeDlg �����o�֐���`
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
	virtual HWND GetDlg(){ return m_hWnd; };
	virtual LRESULT SendMessage( UINT message, WPARAM wParam, LPARAM lParam ){ return ::SendMessage( m_hWnd, message, wParam, lParam ); }; 
	virtual BOOL ErrorCheck() override;
	virtual LONG GetTabNum(){ return -1; };
	virtual void SetTabNum(LONG lTabNum){};
	virtual void SetFeatureClassName(LPCTSTR lpcszFeatureClassName) override;

	CAttrSubwayNodeDlg();
	BOOL SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable );
	BOOL CheckReturnKeyItem(INT nClassID){ return TRUE; }
	BOOL CheckEscKeyItem(INT nClassID){ return TRUE; }

private:
	void CreateControlRelation();
	CTargetItems& GetConnectedLink(IFeaturePtr nodePtr);
	/**
	* @brief �E�B���h�E�}�l�[�W���̃}�b�v��؂�ւ���
	* @note �����N�p�r�m�[�h�p�̃_�C�A���O��؂�ւ��܂��B
	* @param bLink [in] �����N�p���ۂ�
	*/
	void ChangeWindowMap( bool bLink );

	/**
	* @brief �m�[�h�ɑΉ�����H��ID���擾����
	* @param subway_node [in] �m�[�h
	* @return �Ή�����H��ID
	*/
	long SearchLineID( const IFeaturePtr& subway_node );
	
	/**
	* @brief �t�����郊���N�̔��Α��̃m�[�h���擾����
	* @param node_id [in] ������OBJECTID
	* @param linkPtr [in] �t�����郊���N
	* @return ���Α��̃m�[�h
	*/
	IFeaturePtr GetCorrespNode( long node_id, const IFeaturePtr& linkPtr );
	
	bool CheckNode( const IFeaturePtr& node, long railway_id, CTargetItems& allLinks );

/////////////////////////////////////////////////////////////////////////////
//
// CAttrSubwayNodeDlg �����o�ϐ���`
//
/////////////////////////////////////////////////////////////////////////////
private:
	std::map<int, CContainedWindow> m_mapSubClass; //!< �e�R���g���[���E�B���h�E
	CSearchListCtrls m_serchLists;                 //!< �����p���X�g�{�b�N�X�Ǘ�
	CTargetItems m_connectedLink;                  //!< �ڑ����Ă��郊���N
	CString m_layerName;                           //!< �\���Ώۂ̃��C�����i�����Nor�m�[�h�j
	ITablePtr m_joinLineT;                         //!< ������H���e�[�u���ێ��p
};
