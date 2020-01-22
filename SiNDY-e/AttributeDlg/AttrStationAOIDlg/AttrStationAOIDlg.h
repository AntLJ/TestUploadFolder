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

#pragma once

#include "TableManagerBase.h"
#include "AOIListViewCtrl.h"
#include "RelationTableManager.h"

const unsigned int WM_REFRESHFEATURE  = RegisterWindowMessage(_T("WM_REFRESHFEATURE"));       //!< �ĕ`��̈˗����󂯂邽�߂̃��b�Z�[�W

#include "AOISchema.h"
#include "sindy/schema.h"
#include "CommonData.h"

using namespace sindy::schema;

typedef CAttrBaseDlg ATTR_BASE_CLASS;
/////////////////////////////////////////////////////////////////////////////
// CAttrStationAOIDlg
class CAttrStationAOIDlg :
	public ATTR_BASE_CLASS,
	public CExportDlg
{
public:
	/**
	*  @brief �R���X�g���N�^
	*/
	CAttrStationAOIDlg();

/////////////////////////////////////////////////////////////////////////////
//
// CAttrStationAOIDlg ���b�Z�[�W�}�b�v
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MSG_MAP(CAttrStationAOIDlg)
	COMMAND_HANDLER(IDC_BUTTON_SEARCH_POI, BN_CLICKED, OnClickedSearchButton)
	COMMAND_HANDLER(IDC_BUTTON_APPLY, BN_CLICKED, OnClickedApplyButton)
	NOTIFY_HANDLER_EX(IDC_SEARCHLIST,LVN_ITEMCHANGED,OnClickedCheckBox)
	NOTIFY_HANDLER_EX(IDC_SEARCHLIST,LVN_ITEMCHANGING,OnClickedList)
	NOTIFY_HANDLER_EX(IDC_RELATIONLIST,LVN_ITEMCHANGING,OnClickedList)
	MESSAGE_HANDLER(WM_REFRESHFEATURE, OnRefresh)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_COMMAND, OnCommand)
	CHAIN_MSG_MAP(ATTR_BASE_CLASS);
// ���b�Z�[�W�}�b�v
ALT_MSG_MAP(IDC_BUTTON_SEARCH_POI)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
END_MSG_MAP()


/////////////////////////////////////////////////////////////////////////////
//
// CAttrStationAOIDlg ���b�Z�[�W�n���h��
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

		// FALSE�ɂ����ATTR_BASE_CLASS::OnInitDialog()���Ă΂��
		bHandled = FALSE;

		return TRUE;
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

		return 0;
	}

	/**
	 * @brief �u�����v�{�^���������ꂽ���̏���
	 * @note ���̃C�x���g�Ɋւ��Ă�CControlDef���ł܂��Ȃ��鏈���ł͂Ȃ��̂ŁA�����ŏ���
	 */
	LRESULT OnClickedSearchButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	/**
	 * @brief �u�K�p�v�{�^���������ꂽ���̏���
	 * @note ���̃C�x���g�Ɋւ��Ă�CControlDef���ł܂��Ȃ��鏈���ł͂Ȃ��̂ŁA�����ŏ���
	 */
	LRESULT OnClickedApplyButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	/**
	 * @brief ���X�g���N���b�N���ꂽ���̏���
	 */
	LRESULT OnClickedList(LPNMHDR pnmd);

	/**
	 * @brief �`�F�b�N�{�b�N�X���`�F�b�N���ꂽ���̏���
	 */
	LRESULT OnClickedCheckBox(LPNMHDR pnmd);

	/**
	 * @brief WM_REFRESHFEATURE �p�C�x���g�n���h��
	 * @note  �����_�C�A���O���瑗���Ă���ĕ`��C�x���g���󂯂āA�Ǝ��̍ĕ`����s��
	 */
	LRESULT OnRefresh( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );

private:

/////////////////////////////////////////////////////////////////////////////
//
// CAttrStationAOIDlg �����o�֐���`
//
/////////////////////////////////////////////////////////////////////////////
public:
	virtual HWND Create(HWND hWndParent, LPARAM dwInitParam = NULL) override;
	virtual void Delete() override;
	virtual void SetArcHelper( IApplication* ipApp ) override { ATTR_BASE_CLASS::SetArcHelper( ipApp ); };
	virtual void SetAliasOrField( BOOL bAliasOrField ) override { ATTR_BASE_CLASS::m_cControlRel.SetAliasOrField( bAliasOrField ); };
	virtual void SetFeatureDefList( std::list<CLQRowDef>* pFeatureDefList ) override {};
	virtual void SetFeatureDefList( std::list<CFeatureDef>* pFeatureDefList ) override { ATTR_BASE_CLASS::SetFeatureDefList( pFeatureDefList ); };
	virtual void ClearFeatureDefs() override { ATTR_BASE_CLASS::ClearFeatureDefs(); };
	virtual HWND GetDlg() override { return m_hWnd; };
	virtual LRESULT SendMessage( UINT message, WPARAM wParam, LPARAM lParam ) override { return ::SendMessage( m_hWnd, message, wParam, lParam ); }; 

	/**
	* @brief OK�������ꂽ��A�֘A�e�[�u���ɕҏW���e�𔽉f����
	* @note  �V�K�t�B�[�`���[�쐬�������Ă΂��
	* @retval TRUE �ҏW���e�̔��f�ɐ���
	* @retval FALSE �ҏW���e�̔��f�Ɏ��s
	*/
	virtual BOOL ErrorCheck() override;
	virtual LONG GetTabNum() override { return -1; };
	virtual void SetTabNum(LONG lTabNum) override {};
	virtual void SetFeatureClassName(LPCTSTR lpcszFeatureClassName) override {};

	BOOL SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable ) override ;
	BOOL CheckReturnKeyItem(INT nClassID) override { return TRUE; }
	BOOL CheckEscKeyItem(INT nClassID) override { return TRUE; }
private:
	void CreateControlRelation();

	/**
	* @brief ���X�g�r���[�̏���������
	* @retval true  ���X�g�r���[�̏������ɐ���
	* @retval false ���X�g�r���[�̏������Ɏ��s
	*/
	bool initializeListView();

	/**
	* @brief �w�肳�ꂽOBJECTID�̊֘A�����X�V����
	* @param[in] lOID  �֘A�����X�V����OBJECTID
	* @param[in] lList �X�V������Q(���X��CAOIListViewCtrl�œ����I�ɕێ�����Ă���f�[�^)
	* @retval true  �X�V�ɐ���
	* @retval false �X�V�Ɏ��s
	*/
	bool update( long lOID, const AOI_MATRIX& lList );

	/**
	 * @brief �w�肳�ꂽ�I�u�W�F�N�gID������STATION_POINT���n�C���C�g�\������
	 * @param[in] lOID �n�C���C�g�\������STATION_POINT��OBJECTID
	 * @retval true  ����
	 * @retval false DB����|�C���g���擾�ł��Ȃ������A�Ƃ������v���̂��߂Ɏ��s
	 */
	bool setHighLight( long lOID );

	/**
	* @brief �I�����ꂽ�t�B�[�`���[�Ɋ֘A�t�����Ă���POI���擾���A���X�g�r���[�ɕ\��
	* @param[in] nClassID POI�̏���\�����郊�X�g�r���[��ID
	*/
	void dispPOIInfo( UINT nClassID );

//�ȉ��A��L�֐����ŕK�v�ȏ�����ʊ֐��Ƃ��Ď�����������

	// update
	/**
	* @brief ���X�g���烆�j�[�N��ID�̃J���������𒊏o���A1�̃��X�g�Ƃ��Ď擾
	* @param[in] �J�����𒊏o���郊�X�g�Q(1�v�f=1�s 1�v�f���̗v�f���e�J�����̒l�ɂȂ��Ă���)
	* @return �擾�����f�[�^�̃��X�g
	*/
	AOI_COLUMN getUniqueData( const AOI_MATRIX& lList ) const;

	/**
	* @brief std::list<long>���m�̍����o��(����1�ɑ��݂��A����2�ɑ��݂��Ȃ����̂��Ԃ�l�Ƃ��ē�����)
	* @param[in] argList1 ���������std::list<long>����1
	* @param[in] argList2 ���������std::list<long>����2
	* @return argList1�ɑ��݂��AargList2�ɑ��݂��Ȃ��v�f�̃��X�g
	*/
	std::list<CString> getDiffList( const std::list<CString>& argList1, const std::list<CString>& argList2 ) const;

	/**
	* @brief �R�t�����Ă���POI�̃��X�g�r���[����A�`�F�b�N���O���ꂽPOI������ 
	*/
	bool removePOIFromRelationLV();

	/**
	* @brief �R�t�����Ă���POI�̃��X�g�r���[�ɁA�`�F�b�N������ꂽPOI�𑫂� 
	*/
	bool addPOIToRelationLV();

	/// ���X�g�r���[�ɕ\������f�[�^���擾
	AOI_MATRIX getViewList( long lOID );

	/// ���X�g�r���[�ɕ\������f�[�^���擾 ��̃��X�g�𒊏o���ĕ��בւ��Ă���
	AOI_MATRIX getViewList( AOI_MATRIX& railCorp, AOI_MATRIX& stationInfo ) const;

/////////////////////////////////////////////////////////////////////////////
//
// CAttrStationAOIDlg �����o�ϐ���`
//
/////////////////////////////////////////////////////////////////////////////
private:
	/*
	* @brief �e�R���g���[���E�B���h�E
	* @note  �R���g���[��ID���L�[�Ƃ��āA���̃R���g���[�����g���擾�ł���
	*/
	std::map<int, CContainedWindow> m_mapSubClass;

	CRelationTableManager m_RelTableM;       //!< �֘A�e�[�u���Ɋւ�鏈�����Ǘ�����C���X�^���X

	CTableManagerBase m_LineCorpManager;     //!< �Q�ƃe�[�u���̏���ǂݏo�����߂̃N���X
	CTableManagerBase m_StationNameManager;  //!< �֘A�e�[�u���̏���ǂݏo�����߂̃N���X

	CAOIListViewCtrl m_RelationView;         //!< �wAOI�Ɗ֘A�t�����Ă���JOIN_LINE_INFO��\�����郊�X�g�r���[
	CAOIListViewCtrl m_SearchView;           //!< �wAOI�̌`�󂩂���́�m�ȓ��ɑ��݂���JOIN_LINE_INFO��\�����郊�X�g�r���[

	IFeaturePtr      m_ipFeature;            //!< �n�C���C�g�\������STATION_POINT�̃t�B�[�`���[
	IFeatureClassPtr m_ipStationClass;       //!< STATION_POINT�̃t�B�[�`���[�N���X

	/**
	* @brief �`�F�b�N�{�b�N�X�̏�Ԃ̐���Ɏg�p
	* @warning �v���O������Ń`�F�b�N�{�b�N�X�̃`�F�b�N��Ԃ�ύX����ꍇ�ɂ́A�ύX����O��true�ɂ��A�ύX��Afalse�߂�
	*/
	bool m_IsCheckedChangeFromProg;
	std::set<CString> m_CheckedRemove; //!< �`�F�b�N���O���ꂽ���̂̃��X�g
};
