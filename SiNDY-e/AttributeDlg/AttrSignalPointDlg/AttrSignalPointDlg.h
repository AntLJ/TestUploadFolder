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

const unsigned int WM_REFRESHFEATURE = RegisterWindowMessage(_T("WM_REFRESHFEATURE")); //!< �ĕ`��̈˗����󂯂邽�߂̃��b�Z�[�W

class CAttrSignalPointDlg
	: public CAttrBaseDlg
	, public CExportDlg2
	, public IDispEventImpl<IDC_VSFLEXGRID_NODE, CAttrSignalPointDlg>
{
	static const LONG kDRAW_SIZE =   10;
	static const LONG kDRAW_RED  = ~0xE0;
	static const LONG kDRAW_GRN  = ~0x80;
	static const LONG kDRAW_BLU  = ~0x80;

	static const long LAYER_COL = 0;
	static const long OID_COL = 1;

public:
	CAttrSignalPointDlg(void);
	~CAttrSignalPointDlg(void);

BEGIN_MSG_MAP(CAttrSignalPointDlg)
	MESSAGE_HANDLER(WM_WINMGR,          OnWinMgr)
	MESSAGE_HANDLER(WM_INITDIALOG,      OnInitDialog)
	MESSAGE_HANDLER(WM_GETMINMAXINFO,   OnGetMinMaxInfo)
	MESSAGE_HANDLER(WM_CTLCOLOREDIT,    OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORBTN,     OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORSTATIC,  OnCtlColorEdit)
	COMMAND_HANDLER(IDC_BUTTON_TRANSFER_ALTITUDE, BN_CLICKED, OnClickedTransferAltitudeButton)
	COMMAND_HANDLER(IDC_BUTTON_TRANSFER_GR_HEIGHT, BN_CLICKED, OnClickedTransferGroundHeightButton)

	MESSAGE_HANDLER(WM_COMMAND,         OnCommand)
	MESSAGE_HANDLER(WM_SIZE,            OnSize)
	MESSAGE_HANDLER(WM_DESTROY,         OnDestroy)
	MESSAGE_HANDLER(WM_REFRESHFEATURE,  OnRefresh)

	ALT_MSG_MAP(IDC_EDIT_HORIZONTAL_ANGLE)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
		MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
	ALT_MSG_MAP(IDC_COMBO_SIGNAL_MOLD_C)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
		MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
	ALT_MSG_MAP(IDC_EDIT_LAMP_COUNT)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
		MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
	ALT_MSG_MAP(IDC_CHECK_ARROW_F)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
		MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
	ALT_MSG_MAP(IDC_CHECK_WARNING_F)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
		MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
	ALT_MSG_MAP(IDC_CHECK_INDEPENDENT_F)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
		MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
	ALT_MSG_MAP(IDC_EDIT_ALTITUDE)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
		MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
	ALT_MSG_MAP(IDC_BUTTON_TRANSFER_ALTITUDE)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
		MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
	ALT_MSG_MAP(IDC_EDIT_GROUND_HEIGHT)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
		MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
	ALT_MSG_MAP(IDC_BUTTON_TRANSFER_GR_HEIGHT)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
		MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
	ALT_MSG_MAP(IDC_EDIT_HEIGHT)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
		MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
	ALT_MSG_MAP(IDC_EDIT_WIDTH)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
		MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
	ALT_MSG_MAP(IDC_EDIT_SOURCE_ID)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
		MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
	ALT_MSG_MAP(IDC_COMBO_SPECULATION_C)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
		MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
	ALT_MSG_MAP(IDC_EDIT_SOURCE)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
		MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
END_MSG_MAP()

BEGIN_SINK_MAP(CAttrSignalPointDlg)
	SINK_ENTRY(IDC_VSFLEXGRID_NODE, DISPID_KEYDOWN, OnKeyDownGrid)
	SINK_ENTRY(IDC_VSFLEXGRID_NODE, DISPID_MOUSEUP, OnMouseUpGrid)
	SINK_ENTRY(IDC_VSFLEXGRID_NODE, 0x01,           OnCellChangedGrid)          // �Z���ړ�
END_SINK_MAP()

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
		CreateControlRelation();

		// �O���b�h�R���g���[��������ꍇ
		CAxWindow grid = GetDlgItem(IDC_VSFLEXGRID_NODE);
		grid.QueryControl(&m_ipFlexGrid);
		//grid_util::FormatString(m_ipFlexGrid, _T(""));
		if (m_ipFlexGrid)
		{
			long lWidth = 0;
			m_ipFlexGrid->get_ClientWidth(&lWidth);

			m_ipFlexGrid->put_ColWidth(LAYER_COL, 3500);
			m_ipFlexGrid->put_ColWidth(OID_COL, 1500);

			m_ipFlexGrid->put_ExplorerBar  (flexExSortShowAndMove);
			m_ipFlexGrid->put_ExtendLastCol(VARIANT_TRUE);
			m_ipFlexGrid->put_SelectionMode(flexSelectionByRow);

			CString combo;
			combo.Format(_T("%s|%s")
				, sindy::schema::adam_v2::lane_node::kTableName
				, sindy::schema::adam_v2::updown_node::kTableName
			);

			m_ipFlexGrid->PutColComboList(0, _bstr_t(combo.GetString()));
		}

		// GridCtrl����̃C�x���g���擾
		AtlAdviseSinkMap(this, TRUE);

		// �T�u�N���X��
		for( auto& it : m_mapSubClass)
		{
			it.second.SubclassWindow( GetDlgItem( it.first ) );
		}

		return CAttrBaseDlg::OnInitDialog(uMsg, wParam, lParam, bHandled);
	}

	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
		//if (m_ipPoint)
		//{
		//	AheDrawPoint(m_cArcHelper.GetDisplay(), m_ipPoint, TRUE, kDRAW_SIZE, kDRAW_RED, kDRAW_GRN, kDRAW_BLU);
		//	m_ipPoint = NULL;
		//}
		
		ClearNodePoint();
		
		ClearRelationLine();

		AtlAdviseSinkMap(this, FALSE);

		//�n�}�ĕ`��
		m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL );

		return 0;
	}

	LRESULT OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
		if (! m_bFinishInit) { return 0; }

		INT iMsg  = HIWORD(wParam);
		INT nCtrl = LOWORD(wParam);

		if      (iMsg == EN_SETFOCUS)  { SelectDlgItem  (nCtrl); }
		else if (iMsg == CBN_DROPDOWN) { SetComboboxList(nCtrl); }
		else if (iMsg == CBN_SELCHANGE || iMsg == BN_CLICKED || iMsg == EN_UPDATE)
		{
			m_cControlRel.ControlChanged(nCtrl);

			InvalidateRect(NULL, FALSE);
			SetButton(Changed());
		}

		// �ҏW�ۂ̎擾�Ɗ֘A����
		return 0;
	}

	/**
	 * �ĕ`��̃C�x���g�n���h��
	 * 
	 * @see	SiNDY-e\AttributeDlg\AttrRailwayLinkDlg\AttrRailwayLinkDlg.h	OnRefresh
	 */
	LRESULT OnRefresh( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
	{
		UpdateNodePoint();
		UpdateRelationLine();

		return 0;
	}

	/**
	 * @brief �u�֘A�m�[�h��Z�l��]�ʁv�{�^�����������Ƃ��̏���
	 * @note �֘A�m�[�h�ЂƂƒn�㍂�̐ݒ肪�擾�ł���΁A�m�[�h��Z�l+�n�㍂��Z�l�ɐݒ肷��B�_���Ȃ烁�b�Z�[�W�{�b�N�X�o���ďI���
	 */
	LRESULT OnClickedTransferAltitudeButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	/**
	 * @brief �uZ�l����n�㍂��]�ʁv�{�^�����������Ƃ��̏���
	 * @note �֘A�m�[�h�ЂƂƃt�B�[�`����Z�l(0�ȊO)���擾�ł���΁A�t�B�[�`����Z-�m�[�h��Z�l��n�㍂�ɐݒ肷��B�_���Ȃ烁�b�Z�[�W�{�b�N�X�o���ďI���
	 */
	LRESULT OnClickedTransferGroundHeightButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled);

public:
	virtual HWND Create(HWND hWndParent, LPARAM dwInitParam = 0L);
	virtual void Delete();
	virtual bool Update();
	virtual HWND GetDlg() { return m_hChildWnd; }
	virtual void SetArcHelper   (IApplication* ipApp) { CAttrBaseDlg::SetArcHelper(ipApp); }
	virtual void SetAliasOrField(BOOL bAliasOrField)  { m_cControlRel.SetAliasOrField(bAliasOrField); }
	virtual void SetFeatureDefList(std::list<CLQRowDef>*   pFeatureDefList) {}
	virtual void SetFeatureDefList(std::list<CFeatureDef>* pFeatureDefList) { CAttrBaseDlg::SetFeatureDefList(pFeatureDefList); }
	virtual void ClearFeatureDefs () { CAttrBaseDlg::ClearFeatureDefs(); }
	virtual BOOL SetCurrentFeatureDefIndex(LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable);
	virtual BOOL ErrorCheck();
	virtual LONG GetTabNum () { return -1; }
	virtual void SetTabNum (LONG lTabNum) {}
	virtual void SetFeatureClassName(LPCTSTR lpcszFeatureClassName) {}

	virtual LRESULT SendMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) { return ::SendMessage(m_hWnd, uMsg, wParam, lParam); }


	inline BOOL CheckReturnKeyItem(INT nClassID) { return CAttrBaseDlg::CheckReturnKeyItem(nClassID); }
	inline BOOL CheckEscKeyItem   (INT nClassID) { return CAttrBaseDlg::CheckEscKeyItem   (nClassID); }

private:
	void CreateControlRelation();
	BOOL SetRelationValue(CString gID = _T(""));
	/**
	 * �|�C���g�\��/��\��/����
	 */
	void DrawNodePoint(const IPointPtr& ipPoint);		// �`��(�`���ĂȂ���Ε`��)
	void UpdateNodePoint();								// �ĕ`��(�`���Ă��͕̂`��)
	void EraseNodePoint(const IPointPtr& ipPoint);		// �폜(�ĕ`�悠��)
	void ClearNodePoint();								// �폜(�ĕ`��Ȃ�)
	/**
	 * �ĕ`��
	 *
	 * @param[in]	drawObj	true:�|�C���g�A�|�����C���ĕ`�悠��
	 */
	void RefreshScreen(bool drawObj = false);

	/**
	 * �|�C���g�\��
	 *
	 * @parma[in]	ipPoint	�|�C���g�W�I���g��
	 */
	void DrawPoint(const IPointPtr& ipPoint);

	// VS-FlexGrid�C�x���g�n���h��
	void __stdcall OnKeyDownGrid();
	void __stdcall OnMouseUpGrid(SHORT Button, SHORT Shift, FLOAT X, FLOAT Y);
	void __stdcall OnCellChangedGrid(/*LONG Row, LONG Col*/);

	/**
	 * �|�����C���\��/��\��/����
	 */
	void DrawRelationLine();	// �`��(�`���ĂȂ���Ε`��)
	void UpdateRelationLine();	// �ĕ`��(�`���Ă��͕̂`��)
	void EraseRelationLine();	// �폜(�ĕ`�悠��)
	void ClearRelationLine();	// �폜(�ĕ`��Ȃ�)

	/**
	 * �|�����C���\��
	 *
	 * @param[in]	ipLine	�W�I���g��
	 * @param[in]	color	�\���F
	 */
	void DrawPolyLine(const IPolylinePtr& ipLine, const DWORD& color);

	/**
	 * �|�����C���W�I���g���̍쐬
	 *
	 * @param[in]	point1	�n�_���|�C���g
	 * @param[in]	point1	�I�_���|�C���g
	 * @return		�|�����C���W�I���g��
	 */
	IPolylinePtr makeLine(const IPointPtr& point1, const IPointPtr& point2);

	/**
	 * �\���p�|�����C���̍쐬
	 *
	 * @param[in]	relTableName	�֘A�e�[�u����
	 * @param[in]	relOid			�֘A�e�[�u��OID
	 * @param[in]	nodeTableName	�m�[�h�e�[�u����
	 * @param[in]	nodeGid			�m�[�hGID
	 * @param[in]	signalTableName	�M���@�e�[�u����
	 * @param[in]	signalGid		�M���@GID
	 */
	void setLine(
		const CString& relTableName , long relOid, 
		const CString& nodeTableName , CString nodeGid, 
		const CString& signalTableName , CString signaGid
		);

	/**
	 * GLOBALID ���擾����
	 *
	 * @param[in]	table   �擾���̃e�[�u��
	 * @param[in]	oid     GLOBALID �擾�Ώۂ� OBJECTID
	 * @return		GLOBALID
	 */
	CString getGID(const ITablePtr& table, long oid);

	/**
	 * OBJECTID ���擾����
	 *
	 * @param[in]	table   �擾���̃e�[�u��
	 * @param[in]	gid     OBJECTID �擾�Ώۂ� GLOBALID
	 * @return		OBJECTID
	 */
	long getOID(const ITablePtr& table, const CString& gid);

	/**
	 * @brief �G�f�B�b�g�R���g���[���̕����񂩂�double�l�𓾂�
	 * @param [in] controlId �R���g���[���̃��\�[�XID
	 * @param [out] value ����ꂽdouble�l
	 * @retval true double�l�̎擾�ɐ���
	 * @retval false double�l�̎擾�Ɏ��s(���l�Ƃ��ēǂݎ��Ȃ�������B�ΐ��\�����_��)
	 */
	bool GetDoubleFromEditControl(long controlId, double& value);

	/**
	 * @brief ���H�W�����֘A����Ԑ��m�[�hID�𓾂�
	 * @note �Ԑ��m�[�h��1�Ȃ�ΑI���Ɋւ�炸����ID��Ԃ��B�����Ȃ�I������Ă���ID��Ԃ�
	 * @return �Ԑ��m�[�hID
	 * @retval -1 �Ԑ��m�[�h�������Ȃ�����
	 */
	bool GetTargetNodeInfo(CString& tableName, long& nodeId);

	/**
	 * @brief �֘A�m�[�h��Z�l���擾����
	 * @note �֘A�m�[�h����ӂɓ���ł��Ȃ�������t�B�[�`�����擾�ł��Ȃ������肷��΃G���[(�x�����o��)
	 * @param nodeAlt [out] �m�[�h��Z�l
	 * @retval true Z�l�̎擾����
	 * @retval false Z�l�̎擾���s
	 */
	bool GetNodeAlt(double& nodeAlt);

private:
	IVSFlexGridPtr      m_ipFlexGrid;
	std::map<int, CContainedWindow> m_mapSubClass;	//!< �e�R���g���[���E�B���h�E

	HWND    m_hChildWnd;        // �q�_�C�A���O�n���h��


	/**
	 *	@brief  �R�t���m�[�h���
	 */
	struct LinkageNode
	{
		CString layerName;
		long    oID;
		CString	gID;

		LinkageNode() : oID(0) {}
		LinkageNode(CString name, long oid, CString gid) 
			: layerName(name), oID(oid), gID(gid)  {}
		~LinkageNode() {}

		bool operator<(const LinkageNode& node) const {
			if( layerName.Compare(node.layerName) != 0 )
				return layerName < node.layerName;
			if( oID != node.oID )
				return oID < node.oID;
			return gID < node.gID;
		}

		bool operator=(const LinkageNode& node) const {
			return
				layerName.Compare(node.layerName) == 0 &&
				oID == node.oID &&
				gID.Compare(node.gID);
		}
	};

	std::set<LinkageNode> m_LinkagePoints;	//!<  �R�t���m�[�h���
	IPointPtr       m_ipPoint;
	bool            m_isDraw;

	class NameID
	{
	public:
		long    oid;			// OID
		CString gid;			// GID
		CString className;		// �e�[�u����

		NameID() : oid(0) {}
		~NameID(){}
		void clear()
		{
			oid = 0;
			gid.Empty();
			className.Empty();
		}
	};

	class PolyLineInfo
	{
	public:
		class Key
		{
		public:
			NameID signalNameID;	// �M���@
			NameID nodeNameID;		// �m�[�h
			NameID relNameID;		// �֘A�e�[�u��

			Key(){}
			~Key(){}
			void clear()
			{
				signalNameID.clear();
				nodeNameID.clear();
				relNameID.clear();
			}

			bool operator<(const Key &right) const
			{
				//relNameID�Ŕ�r
				return this->relNameID.oid < right.relNameID.oid;
			}
		};

		class Value
		{
		public:
			IPolylinePtr ipLine;	// �W�I���g��
			COLORREF color;			// �\���F
			bool isDraw;			// �\�����(true:�\����)
			long row;				//�s��
			
			Value() : 
				ipLine(nullptr),
				color(RGB(0xC0, 0x80, 0xC0)),
				isDraw(false),
				row(0)
			{}
			~Value(){}
			void clear()
			{
				ipLine = nullptr;
				color = RGB(0xC0, 0x80, 0xC0);
				isDraw = false;
				row = 0;
			}
		};
	public:
		Key key;
		Value value;

		PolyLineInfo(){}
		~PolyLineInfo(){}

		void clear()
		{
			key.clear();
			value.clear();
		}
	};

	std::map<PolyLineInfo::Key, PolyLineInfo::Value> m_mDrawLine;
	static const COLORREF LINE_COLOR;
};

