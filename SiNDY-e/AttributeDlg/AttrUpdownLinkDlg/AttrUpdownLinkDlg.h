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

#include "WinLib/Str2.h"
#include "InvolvedRoadLink.h"
#include "TableDataGrid.h"

// �ĕ`��̈˗����󂯂邽�߂̃��b�Z�[�W
const unsigned int WM_REFRESHFEATURE = RegisterWindowMessage(_T("WM_REFRESHFEATURE")); //!< �ĕ`�施�߂��󂯂邽�߂̃��b�Z�[�W

/// �I������Ă���㉺�������N�ɕR�t���Ă��铹�H�����N�|�㉺�������N�֘A���R�[�h
/// �L�[1:�I������Ă���㉺�������N��OID, �L�[2:���H�����N��OID, �l:���H�����N�|�㉺�������N�֘A���R�[�h
typedef std::map<long, std::map<long, _IRowPtr>> RelRoadLink_UpdownLink;
/// �R�t���Ă��铹�H�����N���Ǘ�����}�b�v
/// �L�[1:�I������Ă���㉺�������N��OID, �L�[2:���H�����N��OID, �l:TODO
typedef std::map<long, std::map<long, InvolvedRoadLink>> RoadLinkList;
/// InvolvedRoadLink�̃}�b�v
/// �L�[1:OID, �L�[2:���H�����N��OID, �l:TODO
typedef std::map<long, InvolvedRoadLink> InvolvedRoadLinkMap;

typedef CAttrBaseDlg ATTR_BASE_CLASS;
/////////////////////////////////////////////////////////////////////////////
// CAttrUpdownLinkDlg
class CAttrUpdownLinkDlg : 
	public ATTR_BASE_CLASS,
	public CExportDlg2,
	public IDispEventImpl<IDC_GRID_NODELIST, CAttrUpdownLinkDlg>
{
	
private:
	/**
	* �ҏW�r���[�̃��[�h
	*
	* �ҏW����ۂɂǂ̃R���g���[����\��������悢���̎w�j�Ƃ��Ďg�p���܂��B
	* CRowsControlRelation::SetControl() �̑�R�A��S�����Ƃ��Ďg�p���܂��B
	* @see CreateControlRelation()
	*/
	enum EditView {
		EDITVIEW_KIHON,		//!< ��{�����i��P�^�u�j
		EDITVIEW_NODE,		//!< �m�[�h�񑮐��i��Q�^�u�j
	};

public:
/////////////////////////////////////////////////////////////////////////////
//
// CAttrUpdownLinkDlg ���b�Z�[�W�}�b�v
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MSG_MAP(CAttrUpdownLinkDlg)
	MESSAGE_HANDLER(WM_WINMGR, OnWinMgr)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
	MESSAGE_HANDLER(WM_CTLCOLOREDIT, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORBTN, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_COMMAND, OnCommand)
	MESSAGE_HANDLER(WM_SIZE, OnSize)
	MESSAGE_HANDLER(WM_REFRESHFEATURE, OnRefresh)
	MESSAGE_HANDLER(WM_NOTIFY, OnNotify)
	MESSAGE_HANDLER(WM_DESTROY, OnDestroy)

// [SiNDYChildDlg] ���b�Z�[�W�}�b�v

// �^�u�L�[���͂��T�u�N���X�������R���g���[���ɓn�����߂̃}�N��
#define ALT_MSG_MAP_(id) \
	ALT_MSG_MAP(id)\
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	\
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)

ALT_MSG_MAP_(IDC_EDIT_UPDOWN_GROUP_ID)
ALT_MSG_MAP_(IDC_EDIT_GROUP_C)
ALT_MSG_MAP_(IDC_EDIT_LANE_COUNT)
ALT_MSG_MAP_(IDC_EDIT_RIGHT_CHANGE)
ALT_MSG_MAP_(IDC_EDIT_LEFT_CHANGE)
ALT_MSG_MAP_(IDC_COMBO_MAX_LEGAL_SPEED_C)
ALT_MSG_MAP_(IDC_COMBO_MIN_LEGAL_SPEED_C)
ALT_MSG_MAP_(IDC_COMBO_TRAVEL_DIRECTION_C)
ALT_MSG_MAP_(IDC_COMBO_ROAD_STRUCTURE_C)
ALT_MSG_MAP_(IDC_COMBO_ONEWAY_C)
ALT_MSG_MAP_(IDC_CHECK_RUBBING_F)
ALT_MSG_MAP_(IDC_CHECK_TUNNEL_F)
ALT_MSG_MAP_(IDC_COMBO_SPECULATION_C)
ALT_MSG_MAP_(IDC_EDIT_SOURCE)

ALT_MSG_MAP(IDC_TAB_UPDOWNATTR)


END_MSG_MAP()

BEGIN_SINK_MAP(CAttrUpdownLinkDlg)
	SINK_ENTRY(IDC_GRID_NODELIST, DISPID_MOUSEUP, OnGridMouseUp)
	SINK_ENTRY(IDC_GRID_NODELIST, grid_util::disp_id::BeforeEdit, OnBeforeEdit)
	SINK_ENTRY(IDC_GRID_NODELIST, grid_util::disp_id::AfterEdit, OnAfterEdit)

END_SINK_MAP()

/////////////////////////////////////////////////////////////////////////////
//
// VS-FlexGrid �C�x���g�n���h��
//
/////////////////////////////////////////////////////////////////////////////
	/**
	* @brief   Before Edit
	* @param   Row     [in]  Row Num
	* @param   Col     [in]  Col Num
	* @param   Cancel  [in]  Cancel Code
	*/
	void __stdcall OnBeforeEdit(LONG Row, LONG Col, VARIANT_BOOL * Cancel);

	/**
	* @brief   MouseUp Event on Grid
	* @param   Button    [in]  Button Code
	* @param   Shift     [in]  Shift Code
	* @param   X         [in]  X Coordinate
	* @param   Y         [in]  Y Coordinate
	* @param   Cancel    [in]  Cancel Code
	*/
	void __stdcall OnGridMouseUp(SHORT Button, SHORT Shift, FLOAT X, FLOAT Y, VARIANT_BOOL * Cancel);

	/**
	* @brief   After Edit
	* @param   Row     [in]  Row Num
	* @param   Col     [in]  Col Num
	*/
	void __stdcall OnAfterEdit(LONG Row, LONG Col);

/////////////////////////////////////////////////////////////////////////////
//
// CAttrUpdownLinkDlg ���b�Z�[�W�n���h��
//
/////////////////////////////////////////////////////////////////////////////

	/**
	 * �_�C�A���O�쐬����ۂɈ�ԍŏ��ɌĂ΂�܂��B�����Ń_�C�A���O�̏����������Ă�������
	 */
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// �R���g���[��
		CreateControlRelation();

		// �m�[�h��O���b�h�R���g���[��
		CAxWindow wndGrid = GetDlgItem(IDC_GRID_NODELIST);
		wndGrid.QueryControl(&m_nqUpdownNodeGrid);

		// �O���b�h�R���g���[������̃C�x���g���E���̂ɕK�v
		AtlAdviseSinkMap(this, true);

		CString strTemp;  //!< pszText�ɓn����������ꎞ�I�Ɋi�[����ϐ�

		// ���H�����ҏW�p�^�u�R���g���[���ɃA�C�e����ǉ����܂�
		HWND hwndTab = GetDlgItem( IDC_TAB_UPDOWNATTR );
		ATLASSERT( ::IsWindow( hwndTab ) );
		TC_ITEM pTabCtrlItem;
		pTabCtrlItem.mask = TCIF_TEXT;
		strTemp = AheLoadString(IDS_BASE);
		pTabCtrlItem.pszText = const_cast<LPTSTR>(strTemp.GetString());
		::SendMessage(hwndTab, TCM_INSERTITEM, EDITVIEW_KIHON, (LPARAM)(&pTabCtrlItem));
		strTemp = AheLoadString(IDS_NODE);
		pTabCtrlItem.pszText = const_cast<LPTSTR>(strTemp.GetString());
		::SendMessage(hwndTab, TCM_INSERTITEM, EDITVIEW_NODE, (LPARAM)(&pTabCtrlItem));

		// �T�u�N���X��
		for( auto& it : m_mapSubClass)
		{
			it.second.SubclassWindow( GetDlgItem( it.first ) );
		}

		::SendMessage(hwndTab, TCM_SETCURSEL, m_iChangeType, 0L);

		ChangeWindowMgrMap( m_iChangeType );

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

		return 0;
	}

	/**
	 * IDC_EDIT_NAME �p�C�x���g�n���h��
	 *
	 * �����ŁA�������͂̃`�F�b�N�������Ȃ��A�s�K�؂ȕ��������͂��ꂽ�ꍇ�ɂ�
	 * ���͂ł��Ȃ��悤�ɂ��܂��B
	 *
	 * @see	SiNDY-e\AttributeDlg\AttrRoadNodeDlg ���Q�l
	 */
	LRESULT OnUpdateCrossName(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		if( ::IsWindow( hWndCtl ) )
		{
			CEdit edit( hWndCtl );
			INT nTextLen = edit.GetWindowTextLength() + 1;
			LPTSTR lpszText = (LPTSTR)_alloca( nTextLen * sizeof(TCHAR) );
			edit.GetWindowText( lpszText, nTextLen );

			// <null> �̏ꍇ���l��
			if( lstrcmp( lpszText, NULL_VALUE ) != 0 )
			{
				CString strName = AheConvertToIntersectionName( lpszText, TRUE );

				if( strName.Compare( lpszText ) != 0)
				{
					INT dwSelStart = 0, dwSelEnd = 0;
					edit.GetSel( dwSelStart, dwSelEnd );
					edit.SetWindowText( strName );
					// ���͈ʒu�̏C��
					INT nDiff = CString_GetLength2( CString( lpszText ) ) - CString_GetLength2( strName );
					edit.SetSel( dwSelStart - nDiff, dwSelEnd - nDiff );
				}
			}
			m_cControlRel.ControlChanged( wID );
			SetButton( Changed() );
			InvalidateRect( NULL, FALSE );
		}

		return 0;
	}

	/**
	 * IDC_EDIT_YOMI �p�C�x���g�n���h��
	 *
	 * �����ŁA�������͂̃`�F�b�N�������Ȃ��A�s�K�؂ȕ��������͂��ꂽ�ꍇ�ɂ�
	 * ���͂ł��Ȃ��悤�ɂ��܂��B
	 *
	 * @see	SiNDY-e\AttributeDlg\AttrRoadNodeDlg ���Q�l
	 */
	LRESULT OnUpdateCrossYomi(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		if( ::IsWindow( hWndCtl ) )
		{
			CEdit edit( hWndCtl );
			INT nTextLen = edit.GetWindowTextLength() + 1;
			LPTSTR lpszText = (LPTSTR)_alloca( nTextLen * sizeof(TCHAR) );
			edit.GetWindowText( lpszText, nTextLen );


			// <null> �̏ꍇ���l��
			if( lstrcmp( lpszText, NULL_VALUE ) != 0 )
			{
				CString strYomi = AheConvertToIntersectionName( lpszText, FALSE );
				if( strYomi.Compare( lpszText ) !=0 )
				{
					INT nSelStart = 0, nSelEnd = 0;
					edit.GetSel( nSelStart, nSelEnd );
					edit.SetWindowText( strYomi );
					// ���͈ʒu�̏C��
					INT nDiff = CString_GetLength2( CString( lpszText ) ) - CString_GetLength2( strYomi );
					edit.SetSel( nSelStart - nDiff, nSelEnd - nDiff );
				}
			}
			m_cControlRel.ControlChanged( wID );
			SetButton( Changed() );
			InvalidateRect( NULL, FALSE );
		}

		return 0;
	}

	/**
	* @brief   WM_NOTIFY �p�C�x���g�n���h��
	* @note �ȉ��̏������s���܂��E�^�u�̐؂�ւ�������������
	*
	* @param   uMsg      [in]   Msg ID
	* @param   wParam    [in]   Control ID
	* @param   lParam    [in]   Control Event
	* @param   bHandled  [out]  The return of processing
	* @retval  1:����  0:���s
	*/	
	LRESULT OnNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		if( wParam == IDC_TAB_UPDOWNATTR ) {
			
			LPNMHDR pnmh = (LPNMHDR)lParam;
			if( pnmh->code == TCN_SELCHANGE ) {
				// �R���g���[���؂�ւ�
				m_iChangeType = TabCtrl_GetCurSel( pnmh->hwndFrom );				
				
				m_cControlRel.Visible( m_iChangeType );
				ChangeWindowMgrMap( m_iChangeType );
			
				ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( m_lFeatureIndex, m_lTableIndex, m_lRowIndex, m_iChangeType, m_bForce, m_bEditable );

				bHandled = TRUE;
								
				return 1;
			}
		}
		
		return 0;
	}

private:

/////////////////////////////////////////////////////////////////////////////
//
// CAttrUpdownLinkDlg �����o�֐���`
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
	virtual bool Update() override;
	virtual LONG GetTabNum(){ return -1; };
	virtual void SetTabNum(LONG lTabNum){};
	virtual void SetFeatureClassName(LPCTSTR lpcszFeatureClassName){};

	CAttrUpdownLinkDlg();
	BOOL SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable );
	BOOL CheckReturnKeyItem(INT nClassID){ return TRUE; }
	BOOL CheckEscKeyItem(INT nClassID){ return TRUE; }

	LRESULT OnRefresh(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	/**
	* @brief   WM_DESTROY �p�C�x���g�n���h��
	* @note    Tear down connection points
	*
	* @param   uMsg      [in]   Msg ID
	* @param   wParam    [in]   Control ID
	* @param   lParam    [in]   Control Event
	* @param   bHandled  [out]  The return of processing
	*/	
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	/**
	* @brief   Check Data Changes
	*
	* @retval  true:changed  false:no changed
	*/
	bool Changed();

private:

	/**
	 * @brief  �q�_�C�A���O�����ۂɍ쐬���܂�
	 */
	void CreateControlRelation();

	/**
	 * @brief  �c�[���`�b�v���쐬���܂�
	 * @param  nTargetItem  [in]  �c�[���`�b�v���쐬����R���g���[���A�C�e����ID
	 */
	void SetToolTip(INT nTargetItem);

	/**
	 * �Z�O�����g�����l�̍X�V
	 */
	void UpdateGroupC();

	/**
	 * �R�[�h�l�h���C�����̐ݒ�
	 *
	 * @param	update	�ݒ�ς݂̏ꍇ�ɍĎ擾���邩
	 *
	 * @retval	true	����
	 * @retval	false	���s
	 */
	bool SetDomain(bool update = false);
	
	/**
	 * ���ݒ��OID�̃R���g���[���X�V
	 *
	 * ���H�����NOID�A�Z�O�����gOID�A�y�A�㉺�������NOID
	 */
	void UpdateUnDefOID();

	/**
	 * @brief  ����������
	 */
	void init();

	/**
	 * @brief  TOC�ɂ���w�肵���e�[�u������ITable���擾
	 * @param  lpcszName   [in]  �e�[�u������
	 * @param  lpcszOwner  [in]  �I�[�i�[����
	 * @return �e�[�u��
	 */
	ITablePtr getSameTable(LPCTSTR lpcszName, LPCTSTR lpcszOwner);

	/**
	 * @brief  ���H�����N���擾
	 * @return ���H�����N
	 */
	IFeatureClassPtr getRoadLinkFC();

	/**
	 * @brief  ���ݑI������Ă���t�B�[�`���ɕR�t���֘A���R�[�h(���l)��ԋp����
	 * @return �֘A���R�[�h(���l)
	 */
	std::map<long, _IRowPtr>& getCurrentRelRoadLinkOrg();

	/**
	 * @brief  ���ݑI������Ă���t�B�[�`���ɕR�t���Ă��铹�H�����N�̊֘A���R�[�h��ԋp����
	 * @return �֘A���R�[�h(����)
	 */
	InvolvedRoadLinkMap& getCurrentRelRoadLinkList();

	/**
	 * @brief  ���ӌ������s�������H�����N�̊֘A���R�[�h��ԋp����
	 * @return �֘A���R�[�h(�E��)
	 */
	InvolvedRoadLinkMap& getCurrentSearchedList();

	/**
	 * @brief  ���H�����N�|�㉺�������N�֘A���R�[�h�ɕύX����������
	 * @retval	true	�ύX����
	 * @retval	false	�ύX�Ȃ�
	 */
	bool isChangeRefTbl();

	/**
	 * @brief  ���H�����N�����R�[�h�ɕύX�����邩
	 * @param  oid  [in]  ���ׂ�֘A���R�[�h��OID
	 * @retval	true	�ύX����
	 * @retval	false	�ύX�Ȃ�
	 */
	bool isChangeDirectionC(long oid);

	/**
	 * @brief  �R�[�h�l�h���C�����擾����
	 * @param  ipMap           [in]  �R�[�h�l�h���C�����擾����IMap
	 * @param  lpcszTableName  [in]  �R�[�h�l�h���C�����擾����e�[�u��
	 * @param  lpcszFieldName  [in]  �R�[�h�l�h���C�����擾����t�B�[���h
	 * @return
	 */
	std::map<CString,long> getDomainCodeValue( IMap* ipMap ,LPCTSTR lpcszTableName, LPCTSTR lpcszFieldName );


	/**
	* @brief   Change Window Manage Map
	*
	* @param   iIndex      [in]   Window Index
	*/	
	void ChangeWindowMgrMap( int iIndex );

	/**
	* @brief   �O���b�h�𐶐�����
	*
	*/
	void createGrid();

	/**
	*  @brief  �{�^����Enable/Disable�̍X�V�A�֘A�e�[�u���̍X�V���̍X�V
	*  @note   �O���b�h�ŊǗ����Ă���֘A�e�[�u���̏��́AControlRel�ōX�V�ł��Ȃ����߁A
	*          �ύX���̃t���O�����X�V����B�t���O���X�V���Ȃ���DB���X�V����Ȃ�
	*
	*/
	void updateChanged();

	/**
	*  @brief  �E�N���b�N����� <�폜> �|�b�v�A�b�v
	*
	* @param   nID      [in]  Control ID
	* @param   ipGrid   [in]  Grid control
	* @param   x        [in]  X Coordinate
	* @param   y        [in]  Y Coordinate
	*/
	void gridMenu( int nID, const IVSFlexGridPtr& ipGrid, float x, float y);

	/**
	* @brief  �O���b�h�őI�����Ă���m�[�h��̃��C�����n�C���C�g����
	*
	*/
	void highlightLine();

	/**
	* @brief  get Point Geometry
	*
	* @param [out]  ipFromPoint       From Point Geometry
	* @param [out]  ipToPoint         To Point Geometry
	*/
	void getSequenceGeometry(IGeometryPtr& ipFromPoint, IGeometryPtr& ipToPoint);

	/**
	*  @brief  �m�[�h��̃W�����v,���S�ֈړ�
	*  @param [in] ipFromPoint        �n�_
	*  @param [in] ipToPoint          �I�_
	*/
	void moveToCenter(const IGeometryPtr& ipFromPoint, const IGeometryPtr& ipToPoint);

	/**
	* @brief  ���W�X�g������f�[�^��ǂݍ���
	*
	*/
	void readReg();

/////////////////////////////////////////////////////////////////////////////
//
// CAttrUpdownLinkDlg �����o�ϐ���`
//
/////////////////////////////////////////////////////////////////////////////
private:

	IFeatureClassPtr m_roadLinkFeatureClass;		//!< ���H�����N�̃t�B�[�`���[�N���X

	std::map<int, CContainedWindow> m_mapSubClass;  //!< �e�R���g���[���E�B���h�E
	HFONT							m_hFont;		//!< �����t�H���g
	std::map<long, CString> m_segAttrDomain;		//!< �Z�O�����g�����R�[�h�l�h���C��(KEY: DomainCode, ELEM: DomainName)
	static const CString UNDEF_NAME;				//!< ���ݒ莞�̕\����
	static const long UNDEF_OID;					//!< ���ݒ��OID�l

	IGeometryPtr m_ipSelectedLine;					//!< �R�t���Ă��铹�H�����N���X�g�őI�����ꂽ���H�����N
	IGeometryPtr m_ipSearchedSelectedLine;			//!< ���ӌ������ꂽ���H�����N���X�g�őI�����ꂽ���H�����N

	RelRoadLink_UpdownLink m_relRoadLinkOrgs;		//!< �I������Ă���㉺�������N�ɕR�t���Ă��铹�H�����N�|�㉺�������N�֘A���R�[�h
													//   �L�[:OID �l:���H�����N�|�㉺�������N�֘A���R�[�h
	RoadLinkList m_relRoadLinkLists;				//!< �ҏW�ΏۃI�u�W�F�N�g���Ƃ̏㉺�������N�ɕR�t���Ă��铹�H�����N
	RoadLinkList m_searchedRoadLinkLists;			//!< �ҏW�ΏۃI�u�W�F�N�g���Ƃ̎��ӌ������s�������H�����N

	std::map<long, CString> m_GIDMap;				//!< �㉺�������N��GID�ϊ��}�b�v �L�[:OID �l:GID

	std::map<CString,long> m_directionCDomain;		//!< ���H�����N�����R�[�h�̃h���C���l

	IVSFlexGridPtr		            m_nqUpdownNodeGrid;	//!< �㉺�������N�p�̃O���b�h		
	std::map<long, TableDataGrid>   m_nqDataGridMap;	//!< �m�[�h��O���b�h�R���g���[�� (key: �㉺���m�[�h��OID, value: �m�[�h��O���b�h)
	long                            m_currentOid;       //!< ���݃_�C�A���O�ɕ\�����Ă���㉺���m�[�h��OID

	long	                        m_lFeatureIndex;	//!< �㉺�������N�_�C�A���O���ł̃t�B�[�`���C���f�b�N�X�ێ��p
	long	                        m_lTableIndex;		//!< �㉺�������N�_�C�A���O���ł̃e�[�u���C���f�b�N�X�ێ��p
	long	                        m_lRowIndex;		//!< �㉺�������N�_�C�A���O���ł�Row�C���f�b�N�X�ێ��p
	bool	                        m_bForce;			//!< �㉺�������N�_�C�A���O���ł̋����ҏW���[�h�ێ��p
	bool	                        m_bEditable;		//!< �㉺�������N�_�C�A���O���ł̕ҏW�ł��邩�ǂ����ێ��p
	long	                        m_lCurrentIndex;	//!< �㉺���m�[�h���0�J�����ڐF�ύX�߂��p�C���f�b�N�X�ԍ��ێ��p
public:
	int					            m_iChangeType;		//!< �R���g���[���؂�ւ��p
	bool                            m_gridSelect;       //!< �O���b�h�̑I���t���O
};
