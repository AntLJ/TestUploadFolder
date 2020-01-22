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


#include "WinLib/str2.h"
#include "WinLib/charutil.h"

typedef CAttrBaseDlg ATTR_BASE_CLASS;
/////////////////////////////////////////////////////////////////////////////
// CAttrPlacenameRepPointDlg
class CAttrPlacenameRepPointDlg : 
	public ATTR_BASE_CLASS,
	public CExportDlg
{
public:
/////////////////////////////////////////////////////////////////////////////
//
// CAttrPlacenameRepPointDlg ���b�Z�[�W�}�b�v
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MSG_MAP(CAttrPlacenameRepPointDlg)
	MESSAGE_HANDLER(WM_WINMGR, OnWinMgr)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
	MESSAGE_HANDLER(WM_CTLCOLOREDIT, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORBTN, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorEdit)
	COMMAND_HANDLER(IDC_EDIT_ELEVENCODE, EN_UPDATE, OnUpdateElevenCode)
	COMMAND_HANDLER(IDC_EDIT_NAME, EN_UPDATE, OnUpdateName)
	MESSAGE_HANDLER(WM_COMMAND, OnCommand)
	MESSAGE_HANDLER(WM_SIZE, OnSize)

// ���b�Z�[�W�}�b�v
ALT_MSG_MAP(IDC_EDIT_ELEVENCODE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_NAME)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_ZIPCODE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_SOURCE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
END_MSG_MAP()

/////////////////////////////////////////////////////////////////////////////
//
// CAttrPlacenameRepPointDlg ���b�Z�[�W�n���h��
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

		INT msg = HIWORD(wParam);						// ���상�b�Z�[�W
		INT	nTargetControl = (INT)LOWORD(wParam);		// ����ΏۃR���g���[��

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

		// 11���Z���R�[�h�������l�Ɠ����ꍇ��OK�{�^����Disable�ɂ���(Ctrl+Z�΍�)
		// TODO:���̃R�[�h���Ȃ���Ctrl+Z�ŃG�f�B�b�g�̕�����߂��Ă��{�^����Disable�ɂȂ�Ȃ��B�i�����Ă����Q�͂Ȃ��j
		// TODO:���̃R�[�h������Ɓu����t�B�[�`����ҏW�v���u�c���[�ŕʂ̃t�B�[�`����I���v���uOK�������v��OnOK�ɍs���Ȃ��̂ŃR�����g�A�E�g�B
		/*
		if( nTargetControl == IDC_EDIT_ELEVENCODE){
			using namespace sindy::schema::placename_rep_point;
			CString strEdit;
			GetDlgItemText(nTargetControl, strEdit);
			CString strCityCode,strAddrCode;
			int codeLength = strEdit.GetLength();
			if(codeLength <= 5){
				strCityCode = strEdit;
				strAddrCode = _T("");
			}
			else{
				strCityCode = strEdit.Left(5);
				strAddrCode = strEdit.Right(codeLength - 5);
			}

			CFeatureDef* pFeatureDef = GetCurrentFeatureDef();
			CString strInitCityCode,strInitAddrCode;
			strInitCityCode = getInitStr(pFeatureDef->GetFieldDef(kTableName, kCityCode));
			strInitAddrCode = getInitStr(pFeatureDef->GetFieldDef(kTableName, kAddrCode));
			if( strCityCode == strInitCityCode && strAddrCode == strInitAddrCode){
				InvalidateRect( NULL, FALSE );
				SetButton(FALSE);
			}
		}
		*/

		return 0;
	}

	/**
	 * 11���Z���R�[�h�p�C�x���g�n���h��
	 *
	 * 11���Z���R�[�h�̃G�f�B�b�g�{�b�N�X�ŕύX���������ꍇ�A�����ŏ������܂�
	 */
	LRESULT OnUpdateElevenCode(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled){
		// ���͐���
		if( ::IsWindow( hWndCtl ) ){
			CEdit edit( hWndCtl );
			// 11���R�[�h�p�e�L�X�g�o�b�t�@
			INT nTextLen = edit.GetWindowTextLength() + 1;
			LPTSTR lpszText = (LPTSTR)_alloca( nTextLen * sizeof(TCHAR) );
			edit.GetWindowText( lpszText, nTextLen );

			// <null> �̏ꍇ���l��
			if( lstrcmp( lpszText, NULL_VALUE ) != 0 ){
				CString strText( lpszText );
				CString strRet;
				LONG lLen = CString_GetLength2( strText );

				// "0123456789ABCDEF"(���p)�̂ݓ��͂�������
				for( LONG i = 0; i < lLen; i++ ){
					CString str = CString_GetAt2( strText, i );
					LPSTR lpstr = CT2A( (LPTSTR)((LPCTSTR)str) );

					if( ( isasciinumber( lpstr[0] ) ||
						( 0x41 <= (unsigned char)((lpstr[0])&0xff) &&
						(unsigned char)(lpstr[0]&0xff) <= 0x46 ) ) )
						strRet += CA2CT( lpstr );
				}

				LONG lCheckLen = CString_GetLength2( strRet );
				if( lLen != lCheckLen ){
					INT dwSelStart = 0, dwSelEnd = 0;
					edit.GetSel( dwSelStart, dwSelEnd );
					edit.SetWindowText( strRet );
					// ���͈ʒu�̏C��
					INT nDiff = CString( lpszText ).GetLength() - strRet.GetLength();
					edit.SetSel( dwSelStart - nDiff, dwSelEnd - nDiff );
				}
			}
			m_cControlRel.ControlChanged( wID );
			InvalidateRect( NULL, FALSE );
		}

		using namespace sindy::schema::placename_rep_point;

		CString strElevenCode;
		GetDlgItemText(IDC_EDIT_ELEVENCODE, strElevenCode);

		// �uGrid�\��->Common�\���v�ŃG�f�B�b�g�{�b�N�X����u��ɂȂ�A
		// ���̎��ɂ��̃C�x���g�n���h�����Ăяo�����B
		// �t���O�ŏ�Ԃ��Ǘ����āA�l�����������̂�h���B
		if(strElevenCode.IsEmpty() && m_isFirst){
			m_isFirst=false;
			return 0;
		}
		m_isFirst=false;

		CString strCityCode, strAddrCode;
		int codeLength = strElevenCode.GetLength();
		if(codeLength <= 5){
			strCityCode = strElevenCode;
			strAddrCode = _T("");
		}
		else{
			strCityCode = strElevenCode.Left(5);
			strAddrCode = strElevenCode.Right(codeLength - 5);
		}

		CFeatureDef* pFeatureDef = GetCurrentFeatureDef();
		CFieldDef* cityFieldDef = pFeatureDef->GetFieldDef( kTableName, kCityCode );
		CFieldDef* addrFieldDef = pFeatureDef->GetFieldDef( kTableName, kAddrCode );
		cityFieldDef->SetNewValue(CComVariant(strCityCode), hWndCtl);
		addrFieldDef->SetNewValue(CComVariant(strAddrCode), hWndCtl);

		if(cityFieldDef->m_bChanged || addrFieldDef->m_bChanged){
			InvalidateRect( NULL, FALSE );
			SetButton(TRUE);
		}

		return 0;
	}

	/**
	 * ��\�_���̗p�C�x���g�n���h��
	 *
	 * ��\�_���̂̃G�f�B�b�g�{�b�N�X�ŕύX���������ꍇ�A�����ŏ������܂�
	 */
	LRESULT OnUpdateName(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled){
		CString strName;
		CEdit edit( hWndCtl );
		INT nTextLen = edit.GetWindowTextLength() + 1;
		edit.GetWindowText( strName.GetBuffer(nTextLen), nTextLen );

		// ���p�X�y�[�X�݂̂̏ꍇ�͋󕶎�����i�[����
		if(strName == _T(" "))
			SetDlgItemText(IDC_EDIT_NAME, _T(""));

		m_cControlRel.ControlChanged( wID );
		SetButton( Changed() );
		InvalidateRect( NULL, FALSE);
		return 0;
	}

private:
	/**
	 * @brief  fieldDef����ҏW�O�̏����l�𕶎���Ŏ擾����
	 * @param  fieldDef [in] �ҏW�O�̏����l���擾�������t�B�[���h��fieldDef
	 * @return �ҏW�O�̏����l�i������j
	 */
	CString getInitStr(CFieldDef* fieldDef);

/////////////////////////////////////////////////////////////////////////////
//
// CAttrPlacenameRepPointDlg �����o�֐���`
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
	virtual BOOL ErrorCheck();
	virtual LONG GetTabNum(){ return -1; };
	virtual void SetTabNum(LONG lTabNum){};
	virtual void SetFeatureClassName(LPCTSTR lpcszFeatureClassName){};

	CAttrPlacenameRepPointDlg();
	BOOL SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable );
	BOOL CheckReturnKeyItem(INT nClassID){ return TRUE; }
	BOOL CheckEscKeyItem(INT nClassID){ return TRUE; }

private:
	void CreateControlRelation();

/////////////////////////////////////////////////////////////////////////////
//
// CAttrPlacenameRepPointDlg �����o�ϐ���`
//
/////////////////////////////////////////////////////////////////////////////
private:
	std::map<int, CContainedWindow> m_mapSubClass; //!< �e�R���g���[���E�B���h�E
	bool m_isFirst; //!< ����ł��邩(true:����, false:����ł͂Ȃ�)
};
