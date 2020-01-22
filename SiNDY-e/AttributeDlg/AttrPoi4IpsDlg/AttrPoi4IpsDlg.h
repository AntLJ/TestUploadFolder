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

#include "TDC/sindylib_core/FieldMap.h"
#include "libschema.h"
#include "AccTable.h"

typedef CAttrBaseDlg ATTR_BASE_CLASS;
/////////////////////////////////////////////////////////////////////////////
// CAttrPoi4IpsDlg
class CAttrPoi4IpsDlg : 
	public ATTR_BASE_CLASS,
	public CExportDlg
{
public:
/////////////////////////////////////////////////////////////////////////////
//
// CAttrPoi4IpsDlg ���b�Z�[�W�}�b�v
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MSG_MAP(CAttrPoi4IpsDlg)
	COMMAND_HANDLER(IDC_BUTTON_PP_FOUND, BN_CLICKED, onPressPpFound)
	COMMAND_HANDLER(IDC_BUTTON_DELETE,BN_CLICKED, onPressDelete)
	MESSAGE_HANDLER(WM_WINMGR, OnWinMgr)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
	MESSAGE_HANDLER(WM_COMMAND, OnCommand)
	MESSAGE_HANDLER(WM_CTLCOLOREDIT, OnCtlColorEditEx)
	MESSAGE_HANDLER(WM_CTLCOLORBTN, OnCtlColorEditEx)
	MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorEditEx)
	MESSAGE_HANDLER(WM_SIZE, OnSize)

// ���b�Z�[�W�}�b�v
ALT_MSG_MAP(IDC_EDIT_SOURCE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_CONTENTS_CODE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_CONTENTS_NAME)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_NAME)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_PP_FOUND)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
	COMMAND_CODE_HANDLER(BN_CLICKED, onPressPpFound)
ALT_MSG_MAP(IDC_COMBO_PST_IP_C)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_AAC_C)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_DELETE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
	COMMAND_CODE_HANDLER(BN_CLICKED, onPressDelete)
ALT_MSG_MAP(IDC_EDIT_CLOSE_C)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_DELETE_C)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_ANNO_DISP_C)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)

END_MSG_MAP()

/////////////////////////////////////////////////////////////////////////////
//
// CAttrPoi4IpsDlg ���b�Z�[�W�n���h��
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

		// �R���e���c�}�X�^�ǂݍ���
		readContentsMaster();
		// ���x�}�X�^�擾
		m_AccCodeMaster = AccTable::codes;
		
		return ATTR_BASE_CLASS::OnInitDialog( uMsg, wParam, lParam, bHandled );
	}

	/**
	 * �R�����R���g���[���̐F�ύX�p�C�x���g�n���h�����ǔ�
	 *
	 * �t�B�[���h�l�̕ύX���������ꍇ�A�����ŃR�����R���g���[���̐F���ς��܂�
	 */
	LRESULT OnCtlColorEditEx(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{

		HDC hdc = (HDC) wParam;    // handle to display context 
		HWND hWnd = (HWND) lParam; // handle to static control 
		INT	nTargetControl = (INT)LOWORD(wParam);		// ����ΏۃR���g���[��

		// �e�N���X��OnCtlColorEdit���Ăяo��
		LRESULT ret = OnCtlColorEdit(uMsg, wParam, lParam, bHandled);

		using namespace sindy::schema;

		// ���񋟌��R�[�h�A���x�̃R���{�{�b�N�X�A�����R�A�폜�R�[�h�A���L�\���R�[�h�̃G�f�B�b�g�{�b�N�X�͎��O�ŕύX�Ǘ�
		CFieldDef* pFieldDef = nullptr;
		switch (::GetDlgCtrlID((HWND)lParam))
		{
			case IDC_COMBO_PST_IP_C:
				pFieldDef = GetCurrentFeatureDef()->GetFieldDef(sindyk::poi_point::kTableName, sindyk::poi_point::kPSTIPCode );
				break;
			case IDC_COMBO_AAC_C:
				pFieldDef = GetCurrentFeatureDef()->GetFieldDef(sindyk::poi_point::kTableName, sindyk::poi_point::kACCCode );
				break;
			case IDC_EDIT_CLOSE_C:
				pFieldDef = GetCurrentFeatureDef()->GetFieldDef(sindyk::poi_point::kTableName, sindyk::poi_point::kCloseCode );
				break;
			case IDC_EDIT_DELETE_C:
				pFieldDef = GetCurrentFeatureDef()->GetFieldDef(sindyk::poi_point::kTableName, sindyk::poi_point::kDelete );
				break;
			case IDC_EDIT_ANNO_DISP_C:
				pFieldDef = GetCurrentFeatureDef()->GetFieldDef(sindyk::poi_point::kTableName, sindyk::poi_point::kAnnoDispCode );
				break;
			default:
				break;
		}

		if ( pFieldDef == nullptr ||
			 ( pFieldDef != nullptr && !pFieldDef->m_bChanged )  )
			return ret;

		if( !m_hChangedBrush )
			m_hChangedBrush = CreateSolidBrush( RGB(255,168,126) );

		SetBkMode(hdc, TRANSPARENT );
		SetBkColor(hdc, RGB(255,168,126));

		// Return a handle to the brush
		HBRUSH far* hbr = (HBRUSH far*) m_hChangedBrush;
		
		return ((DWORD) hbr);
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

			// �R���{�{�b�N�X�̒l���Z���`�F���W�����Ƃ��ɒl��ύX����B
			if( ( nTargetControl == IDC_COMBO_PST_IP_C ) && ( msg == CBN_SELCHANGE) )
				setPstIPCode(uMsg, wParam, lParam, bHandled);
			else if( ( nTargetControl == IDC_COMBO_AAC_C ) && ( msg == CBN_SELCHANGE) )
				setAccCode(uMsg, wParam, lParam, bHandled);

			// �ύX���ꂽ���ǂ����`�F�b�N���A�ύX����Ă����瑼�̃R���g���[���ɂ����f������
			m_cControlRel.ControlChanged( (INT)LOWORD(wParam) );
			// �_�C�A���O���X�V�̈�ɒǉ����܂�
			InvalidateRect( NULL, FALSE );
			// �ύX���ꂽ�ꍇ�̓{�^���� Enable ��
			SetButton( Changed() );
		}
		return 0;
	}

private:

/////////////////////////////////////////////////////////////////////////////
//
// CAttrPoi4IpsDlg �����o�֐���`
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
	virtual BOOL ErrorCheck(){ return TRUE; };
	virtual LONG GetTabNum(){ return -1; };
	virtual void SetTabNum(LONG lTabNum){};
	virtual void SetFeatureClassName(LPCTSTR lpcszFeatureClassName){};

	CAttrPoi4IpsDlg();
	BOOL SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable );
	BOOL CheckReturnKeyItem(INT nClassID){ return TRUE; }
	BOOL CheckEscKeyItem(INT nClassID){ return TRUE; }

private:
	void CreateControlRelation();

	/**
	 * @brief ���ʒu�����{�^�����������ꂽ�ꍇ�̏���
	 */
	LRESULT onPressPpFound(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	/**
	 * @brief  �폜�{�^�����������ꂽ�ꍇ�̏���
	 */
	LRESULT onPressDelete(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	/**
	 * @brief ���񋟌��R�[�h�Œl���I�����ꂽ�ꍇ�̏���
	 */
	LRESULT  setPstIPCode(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	/**
	 * @brief ���x�Œl���I�����ꂽ�ꍇ�̏���
	 */
	LRESULT  setAccCode(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	/**
	 * @brief  ���W�X�g���l���擾
	 * @param  path  [in]  ���W�X�g���̃p�X
	 * @param  key  [in]  ���W�X�g���L�[
	 * @param  out  [out]  �l
	 * @return true:����
	 */
	bool getRegKey(const CString& path , const CString& key, CString& out);

	/**
	 * @brief  �I������Ă���t�B�[�`���̏����擾�������o�ϐ��ɃZ�b�g
	 * @return�@�Ȃ�
	 */
	void getCurrentFeatureInfo();
	
	/**
	 * @brief  �R���{�{�b�N�X�쐬(�ėp)
	 * @param  id  [in]  �R���g���[��ID
	 * @param  tbl  [in]  �ݒ茳�}�X�^�[�e�[�u��
	 * @param  lpszTableName  [in]  �e�[�u����
	 * @param  lpszFieldName  [in]  �J������
	 * @return
	 */
	void createComboBox(UINT id, const StringMasterTbl& tbl, LPCTSTR lpszTableName, LPCTSTR lpszFieldName);

	/**
	 * @brief  ���񋟌��R�[�h�R���{�{�b�N�X�쐬
	 * @return�@�Ȃ�
	 */
	void createPstIpCombo();

	/**
	 * @brief  ���x�R���{�{�b�N�X�쐬
	 * @return�@�Ȃ�
	 */
	void createAcc();

	/**
	 * @brief  �R���{�{�b�N�X�̃A�C�e����FieldDef�ɃZ�b�g
	 * @param  tbl  [in]  �h���C���̃e�[�u��
	 * @param  id  [in]  �h���C���̃e�[�u���̃C���f�b�N�X
	 * @param  lpszFieldName  [in]  �ݒ肷��t�B�[���h����
	 * @return �Ȃ�
	 */
	void setFieldDef4Combo(const StringMasterTbl& tbl, UINT id,  const LPCTSTR lpszFieldName);
	
	/**
	 * @brief  TSV�t�@�C����2�����z��ŕԋp����
	 * @param  file  [in]  �t�@�C���p�X
	 * @return 2�����z��
	 */
	std::vector<std::vector<CString>> parseTsvFile(const CString& file);
	
	/**
	 * @brief  �h���C������V�[�P���X���擾����
	 * @param  ipCode  [in]  IP�R�[�h
	 * @param  tbl  [in]  �h���C���̃e�[�u��
	 * @return �V�[�P���X
	 */
	INT getComboIdx(const CString& ipCode, const StringMasterTbl& tbl);

	/**
	 * @brief  �X�V����t�^����
	 * @param  featureDef  [in]  �X�V����t�^����CFeatureDef
	 * @return true:����
	 */
	bool UpdateHistory(CFeatureDef* featureDef);

	/**
	 * @brief  �폜�{�^���t���O�����݂̏󋵂��画�f����
	 * @return �폜�t���O�ɐݒ肷��l
	 */
	bool judgeDeleteFlg();

	/**
	 * @brief  �폜�{�^���t���O�����݂̏󋵂��画�f����
	 * @param  pFieldDef  [in]  �X�V�t���O��ύX����CFieldDef
	 * @return �Ȃ�
	 */
	void updateChange(CFieldDef* pFieldDef);

	/**
	 * @brief  �L�[��������̃h���C�����쐬����
	 * @return �Ȃ�
	 */
	void createStrKeyDomain();

	/**
	* @brief  �L�[��������̃h���C���l���擾����
	 * @return�@�h���C���̃��R�[�h
	 */
	std::map< CString, CString> getStrKeyDomein(const LPCTSTR lpszFieldName);

	/**
	 * @brief  �h���C���̒l���G�f�B�b�g�{�b�N�X�ɓ����
	 * @param  id  [in]  �R���g���[��ID
	 * @param  lpszFieldName  [in]  �t�B�[���h����
	 * @return
	 */
	void setEditBoxByDomainVal(int id, LPCTSTR lpszFieldName);

	/**
	 * @brief  �R���e���c�}�X�^�ǂݍ���
	 */
	bool readContentsMaster();

////////////////////////////////////////////////////////////////////////////
//
// CAttrPoi4IpsDlg �����o�ϐ���`
//
/////////////////////////////////////////////////////////////////////////////
private:
	/// �e�R���g���[���E�B���h�E
	std::map<int, CContainedWindow> m_mapSubClass; 
	/// �{�^�����폜��
	bool m_isDelete;
	/// GRPCODE
	CString m_grpcode;
	/// CHAINCODE
	CString m_chainCode;
	/// ContentsSeq
	CString m_contentsSeq;
	/// �R���e���c����
	CString m_contentsName;
	/// �R���e���c�^�C�v
	LONG m_contentsType;
	///  �V�KPOI�� 
	bool m_isNewlyPoi;
	/// �t�B�[���h�}�b�v
	sindy::CFieldMap m_fieldMap;
	/// ���񋟌��R�[�h�}�X�^�l
	StringMasterTbl m_pstIpCodeMaster;
	/// ���x�}�X�^�l
	StringMasterTbl m_AccCodeMaster;
	/// �L�[��������^�ł���h���C���̃}�b�v�@std::map< �t�B�[���h�� , map<�L�[, �l> >
	std::map< CString, std::map<CString, CString> > m_strKeyDomainMap;
	/// �R���e���c�}�X�^
	std::vector<std::vector<CString>> m_contentsMaster;
	/// ���񋟌��R�[�h�}�X�^�ǂݍ���
	std::vector<std::vector<CString>> m_infoProvideMaster;
};
