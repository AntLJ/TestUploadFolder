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

// AttrSegmentAttrDlg.h : CAttrSegmentAttrDlg �̐錾

#ifndef _ATTRSEGMENTATTRDLG_
#define _ATTRSEGMENTATTRDLG_

#include "resource.h"

typedef CAttrBaseDlg ATTR_BASE_CLASS;

/////////////////////////////////////////////////////////////////////////////
// CAttrSegmentAttrDlg
class CAttrSegmentAttrDlg :
	public ATTR_BASE_CLASS,
	public CExportDlg
{
public:

/////////////////////////////////////////////////////////////////////////////
//
// CAttrSegmentAttrDlg ���b�Z�[�W�}�b�v
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MSG_MAP(CAttrSegmentAttrDlg)
	MESSAGE_HANDLER(WM_WINMGR, OnWinMgr)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
	MESSAGE_HANDLER(WM_CTLCOLOREDIT, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORBTN, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_COMMAND, OnCommand)
	MESSAGE_HANDLER(WM_SIZE, OnSize)

ALT_MSG_MAP(IDC_SEGMENT_ATTR_CHECK1)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_SEGMENT_ATTR_EDIT2)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_SEGMENT_ATTR_EDIT3)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_SEGMENT_ATTR_COMBO1)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_SEGMENT_ATTR_EDIT4)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_SEGMENT_ATTR_EDIT5)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_SOURCE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
END_MSG_MAP()

/////////////////////////////////////////////////////////////////////////////
//
// CAttrSegmentAttrDlg ���b�Z�[�W�n���h��
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

		// �R���{�{�b�N�X�̒��g�ύX
		// TODO:�e�R���g���[���ŃC�x���g�߂܂���悤�ɂ���B
		if( nTargetControl == IDC_SEGMENT_ATTR_COMBO1 )
		{
			if( msg == CBN_SETFOCUS )
			{
				SetLinkAttrComboBox();
			}
			else if( msg == CBN_SELCHANGE )
			{
				SetNameFieldEditable();
			}
		}

		// �R���{�{�b�N�X�A�G�f�B�b�g�{�b�N�X�A�`�F�b�N�{�b�N�X�̃��b�Z�[�W����
		if( ( msg == CBN_SELCHANGE ) || ( msg == BN_CLICKED ) || ( msg == EN_UPDATE )  ) 
		{
			// �ύX���ꂽ���ǂ����`�F�b�N���A�ύX����Ă����瑼�̃R���g���[���ɂ����f������
			m_cControlRel.ControlChanged( (INT)LOWORD(wParam) );
			// 2�����b�V�����E�����ׂ��ł��铹�H�̏ꍇ�͔��Α��̓��H�����������ɂ��Ȃ��Ă͂����Ȃ�����
			// �ȉ��̏������s��
			if( m_bIsRoadOnMesh )
			{
				// �Ƃ肠�����A���ݕێ����Ă��郊�X�g���ׂĂɂ����ă��[�v���s��
				for( std::list<CFeatureDef>::iterator it = m_pFeatureDefList->begin(); it != m_pFeatureDefList->end(); it++ )
				{
					// ���ݑI�𒆂�2�����b�V�����ׂ��ł��铹�H�����N�Ɋ֘A���Ă��郊�X�g�Ƃ̏ƍ�
					CFeatureDef *pFeatureDef = NULL;
					for( std::list<CFeatureDef>::iterator iAttr = m_pAttrLinkOnMeshList.begin(); iAttr != m_pAttrLinkOnMeshList.end(); iAttr++ )
					{
						if( iAttr->GetFeature() == it->GetFeature() )
						{
							pFeatureDef = &(*it);
							break;
						}
					}

					if( pFeatureDef )
					{
						// ���ׂẴt�B�[���h�l���`�F�b�N
						for( std::list<CFieldDef>::iterator itField = GetCurrentFeatureDef()->begin(); itField != GetCurrentFeatureDef()->end(); itField++ )
						{
							CString strFieldName = itField->m_strFieldName;

							// �ȉ����e���R�s�[����
							if( 0 == strFieldName.CompareNoCase( sindy::schema::segment_attr::kLinkAttr ) ||
								0 == strFieldName.CompareNoCase( sindy::schema::segment_attr::kUnderOver ) ||
								0 == strFieldName.CompareNoCase( sindy::schema::segment_attr::kName ) ||
								0 == strFieldName.CompareNoCase( sindy::schema::segment_attr::kSource ) )
							{
								// ��L��if���̏����ȊO�̓R�s�[����
								CFieldDef *pFieldDef;
								pFieldDef = pFeatureDef->GetFieldDef( SEGMENTATTR_TABLE_NAME, itField->m_strFieldName );
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

			InvalidateRect( NULL, FALSE );
			// �ύX���ꂽ�ꍇ�̓{�^���� Enable ��
			SetButton( Changed() );
		}

		return 0;
	}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrSegmentAttrDlg �����o�֐���`
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

	CAttrSegmentAttrDlg();
	BOOL SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable );
	inline BOOL CheckReturnKeyItem(INT nClassID);
	inline BOOL CheckEscKeyItem(INT nClassID) { return ATTR_BASE_CLASS::CheckEscKeyItem(nClassID); };

private:
	void CreateControlRelation();
	void SetLinkAttrComboBox();

	/**
	 * @brief ���̃G�f�B�b�g�{�b�N�X�̗L�������؂�ւ�
	 * @note  ��ʃR���{�{�b�N�X�̒l�ɂ���Đ؂�ւ���
	 */
	void SetNameFieldEditable();

	/**
	 * @brief ��ʃR���{�{�b�N�X��NAME�t�B�[���h�̕ҏW���\�Ȏ�ʂ��I������Ă��邩�ǂ���
	 * @retval true  NAME�t�B�[���h�ҏW�̎�ʂ��I������Ă���
	 * @retval false NAME�t�B�[���h�ҏW�s�̎�ʂ��I������Ă���
	 */
	bool IsNameFieldEditableAttr();

	/**
	 * @brief ���̃t�B�[���h���Ȃ���΃R���g���[�����\���ɂ���(ASEAN���̑Ή�)
	 */
	void AdjustLayout();

////////////////////////////////////////////////////////////////////////////
//
// CAttrSegmentAttrDlg �����o�ϐ���`
//
/////////////////////////////////////////////////////////////////////////////
private:
	std::map<int, CContainedWindow> m_mapSubClass; //!< �e�R���g���[���E�B���h�E
	std::list<CFeatureDef>	m_pAttrLinkOnMeshList;		//!< ���b�V�����ׂ��ł��郊���N�������̃t�B�[�`���f�t
	BOOL				m_bIsRoadOnMesh;			//!< ���b�V�����ׂ��ł��邩�ǂ���
	HWND				m_hChildWnd;				//!< �q�_�C�A���O�n���h��
};

#endif // _ATTRSEGMENTATTRDLG_

