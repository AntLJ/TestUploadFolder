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


typedef CAttrBaseDlg ATTR_BASE_CLASS;
/////////////////////////////////////////////////////////////////////////////
// CAttrSJPolygonDlg
class CAttrSJPolygonDlg : 
	public ATTR_BASE_CLASS,
	public CExportDlg
{
public:
/////////////////////////////////////////////////////////////////////////////
//
// CAttrSJPolygonDlg ���b�Z�[�W�}�b�v
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MSG_MAP(CAttrSJPolygonDlg)
	MESSAGE_HANDLER(WM_WINMGR, OnWinMgr)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
	MESSAGE_HANDLER(WM_CTLCOLOREDIT, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORBTN, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_COMMAND, OnCommand)
	MESSAGE_HANDLER(WM_SIZE, OnSize)

ALT_MSG_MAP(IDC_COMBO_LANDCOVER)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_LANDUSE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_ROADFLAG)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_SOURCE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
END_MSG_MAP()

/////////////////////////////////////////////////////////////////////////////
//
// CAttrSJPolygonDlg ���b�Z�[�W�n���h��
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

			// ROAD_FLAG�̒l���ύX���ꂽ�ꍇ�ɓ���Ή�
			if( m_strClassName == sindy::schema::sj::kRoadTableName && msg == BN_CLICKED && (INT)LOWORD(wParam) == IDC_CHECK_ROADFLAG )
			{
				CButton cCheck( GetDlgItem( IDC_CHECK_ROADFLAG ) );
				// 0x0003�̓I���A�I�t��Ԃ𒲂ׂ�i�܂�A�I���Ȃ�true�j
				if( cCheck.GetState() & 0x0003 )
				{
					// LAND_COVER=0�łȂ��ꍇ�́A0�ɂ���i�ύX��������̂ŁAm_bChenged=true�ɂ��Ă����j
					if( GetCurrentFeatureDef()->GetFieldDef( m_strClassName, sindy::schema::sj::kLCCLS )->m_vaValue.lVal != 0 )
					{
						GetCurrentFeatureDef()->GetFieldDef( m_strClassName, sindy::schema::sj::kLCCLS )->m_vaValue = 0L;
						GetCurrentFeatureDef()->GetFieldDef( m_strClassName, sindy::schema::sj::kLCCLS )->m_bChanged = true;
					}
					// LAND_USE=0�łȂ��ꍇ�́A0�ɂ���i�ύX��������̂ŁAm_bChenged=true�ɂ��Ă����j
					if( GetCurrentFeatureDef()->GetFieldDef( m_strClassName, sindy::schema::sj::kLUCLS )->m_vaValue.lVal != 0 )
					{
						GetCurrentFeatureDef()->GetFieldDef( m_strClassName, sindy::schema::sj::kLUCLS )->m_vaValue = 0L;
						GetCurrentFeatureDef()->GetFieldDef( m_strClassName, sindy::schema::sj::kLUCLS )->m_bChanged = true;
					}
				}
				// ROAD_F�ɕύX�����������̂ŁALAND_COVER�ALAND_USE�̃R���{�{�b�N�X�̏�ԕύX
				SetRoadFlag( (bool)GetCurrentFeatureDef()->GetFieldDef( m_strClassName, sindy::schema::sj::kRoadF )->m_vaValue.lVal, true );
			}
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
// CAttrSJPolygonDlg �����o�֐���`
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
	virtual void SetFeatureClassName(LPCTSTR lpcszFeatureClassName){ m_strClassName = (CString)lpcszFeatureClassName;};

	CAttrSJPolygonDlg();
	BOOL SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable );
	BOOL CheckReturnKeyItem(INT nClassID){ return TRUE; }
	BOOL CheckEscKeyItem(INT nClassID){ return TRUE; }

private:
	void CreateControlRelation();
	/**
	 * @brief ROAD_F���ύX���ꂽ�Ƃ��ɁALCCLS�ALUCLS�̏�Ԃ�ݒ肷��֐�
	 *
	 * @note  ROAD_F�̒l��1�ɕύX����鎞�́ALCCLS�ALUCLS��0�ɏ����������
	 *        �܂��AROAD_F�̒l��0�ɕύX����鎞�́ALCCLS�ALUCLS�̕ύX�͂Ȃ����߁A
	 *        ROAD_F��ύX���悤�Ƃ���ꍇ�ɂ͂ǂ���ɂ���LCCLS�ALUCLS��0�ɂ��Ă����΂悢
	 *
	 * @param bEnalbe	[in]  ROAD_F�̒l
	 * @param bReset    [in]  LCCLS�ALUCLS�����������邩�ǂ���
	 */
	void SetRoadFlag( bool bCheck, bool bReset );

/////////////////////////////////////////////////////////////////////////////
//
// CAttrSJPolygonDlg �����o�ϐ���`
//
/////////////////////////////////////////////////////////////////////////////
private:
	std::map<int, CContainedWindow> m_mapSubClass; //!< �e�R���g���[���E�B���h�E
};
