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

// AttrCSDBDlg.h : CAttrCSDBDlg �̐錾

#pragma once

#include "resource.h"

typedef CAttrBaseDlg ATTR_BASE_CLASS;

/////////////////////////////////////////////////////////////////////////////
// CAttrCSDBDlg
class CAttrCSDBDlg : 
	public ATTR_BASE_CLASS,
	public IDispEventImpl<IDC_GRIDCTRL, CAttrCSDBDlg>, // �f�t�H���g�̃O���b�h�R���g���[��
	public CExportDlg
{
public:
/////////////////////////////////////////////////////////////////////////////
//
// CAttrCSDBDlg ���b�Z�[�W�}�b�v
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MSG_MAP(CAttrCSDBDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	CHAIN_MSG_MAP(ATTR_BASE_CLASS)
	MESSAGE_HANDLER(WM_SIZE, OnSize)
END_MSG_MAP()

	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

/////////////////////////////////////////////////////////////////////////////
//
// CAttrCSDBDlg ���b�Z�[�W�n���h��
//
/////////////////////////////////////////////////////////////////////////////

	/**
	 * �_�C�A���O�쐬����ۂɈ�ԍŏ��ɌĂ΂�܂��B�����Ń_�C�A���O�̏����������Ă�������
	 */
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// �R���g���[���쐬
		CreateControlRelation();

		// �f�t�H���g�p�O���b�h�R���g���[��
		CAxWindow wndGrid = GetDlgItem(IDC_GRIDCTRL);
		wndGrid.QueryControl(&m_spGrid);
		wndGrid.SetFocus();		

		// �O���b�h�R���g���[������̃C�x���g���E���̂ɕK�v
		AtlAdviseSinkMap(this, TRUE);

		ATTR_BASE_CLASS::OnInitDialog( uMsg, wParam, lParam, bHandled );

		return 1;  // �V�X�e���Ƀt�H�[�J�X��ݒ肳���܂�
	}

	/**
	 * �_�C�A���O���j�����ꂽ�Ƃ��ɌĂ΂�܂��B
	 */
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		AtlAdviseSinkMap(this, FALSE);

		return 0;  // �V�X�e���Ƀt�H�[�J�X��ݒ肳���܂�
	}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrCSDBDlg �C�x���g�V���N�}�b�v
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_SINK_MAP(CAttrCSDBDlg)
	///Make sure the Event Handlers have __stdcall calling convention
	SINK_ENTRY(IDC_GRIDCTRL, 0x5,	OnBeforeMouseDown)
	SINK_ENTRY(IDC_GRIDCTRL, 48,	OnSetupEditStyle)
	SINK_ENTRY(IDC_GRIDCTRL, 0x12,	OnAfterMoveColumn)
END_SINK_MAP()

/////////////////////////////////////////////////////////////////////////////
//
// VS-FlexGrid �C�x���g�n���h��
//
/////////////////////////////////////////////////////////////////////////////
public:
	void __stdcall OnSetupEditStyle( long Row, long Col, VARIANT_BOOL IsCombo, long * Style, long * StyleEx )
	{
		*Style |= ES_READONLY;
	}

	void __stdcall OnBeforeMouseDown(SHORT Button, SHORT Shift, FLOAT X, FLOAT Y, VARIANT_BOOL * Cancel)
	{
		LONG lCol = 0, lRow = 0;
		m_spGrid->get_MouseCol( &lCol );
		m_spGrid->get_MouseRow( &lRow );
	}
	/**
	 * �W���O���b�h�ŗ񂪈ړ����ꂽ��ɔ�������C�x���g�n���h��
	 */
	VOID __stdcall OnAfterMoveColumn  ( long Row, long Col )
	{
		RefreshGrid();
	}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrCSDBDlg �����o�֐���`
//
/////////////////////////////////////////////////////////////////////////////
public:
	virtual HWND Create(HWND hWndParent, LPARAM dwInitParam = NULL);
	virtual void Delete();
	virtual void SetArcHelper( IApplication* ipApp ){ ATTR_BASE_CLASS::SetArcHelper( ipApp ); };
	virtual void SetAliasOrField( BOOL bAliasOrField );
	virtual void SetFeatureDefList( std::list<CLQRowDef>* pFeatureDefList ){};
	virtual void SetFeatureDefList( std::list<CFeatureDef>* pFeatureDefList );
	virtual void ClearFeatureDefs(){ ATTR_BASE_CLASS::ClearFeatureDefs(); };
	virtual HWND GetDlg(){ return m_hChildWnd; };
	virtual LRESULT SendMessage( UINT message, WPARAM wParam, LPARAM lParam ){ return ::SendMessage( m_hWnd, message, wParam, lParam ); }; 
	virtual BOOL ErrorCheck(){ return TRUE; };
	virtual LONG GetTabNum(){ return -1; };
	virtual void SetTabNum(LONG lTabNum){};
	virtual void SetFeatureClassName(LPCTSTR lpcszFeatureClassName){};

	CAttrCSDBDlg();
	BOOL SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable );
	inline BOOL CheckReturnKeyItem(INT nClassID) { return ATTR_BASE_CLASS::CheckReturnKeyItem(nClassID); };
	inline BOOL CheckEscKeyItem(INT nClassID) { return ATTR_BASE_CLASS::CheckEscKeyItem(nClassID); };


private:
	void CreateControlRelation();
	/** 
	* @fn		HRESULT CreateActiveXControls(UINT nID);
	*
	* @brief	FlexGrid�̃��C�Z���X�E�B���h�E���o���Ȃ��悤�ɂ��邽�߂̊֐�
	*
	* @note		CAxDialogImpl::CreateActiveXControls���I�[�o�[���C�h��ActiveXControl�쐬�̍ۂ�
	*			FlexGrid�̃L�[�𖳗����蓖�ĂĂ���
	*/
	HRESULT CreateActiveXControls(UINT nID);

	void CreateAliasMap( _IRowPtr ipRow );

	void RefreshGrid()
	{
		long lCols = -1;
		m_spGrid->get_Cols( &lCols );
		m_spGrid->put_AutoSizeMode( flexAutoSizeColWidth );
		if( lCols > 1 )		// �u�������́v�����͎������T�C�Y���Ȃ�
			m_spGrid->AutoSize( 1, lCols-1 );
		m_spGrid->put_AutoSizeMode( flexAutoSizeRowHeight );
		if( lCols > 1 )		// �u�������́v�����͎������T�C�Y���Ȃ�
			m_spGrid->AutoSize( 1, lCols-1 );
	};

	/** 
	* @fn		void SetHiddenRow();
	*
	* @brief	<Null>�l�̍s�͔�\����Ԃɂ���
	*
	* @note		���܂����O���[�v�P�ʂŔ�\���̃R���g���[�����s��
	*			
	*/
	void SetHiddenRow();
	void _SetHiddenRow(_IRowPtr ipRow, std::list<CString>& strList);

/////////////////////////////////////////////////////////////////////////////
//
// CAttrCSDBDlg �����o�ϐ���`
//
/////////////////////////////////////////////////////////////////////////////
private:
	IVSFlexGridPtr		m_spGrid;			//!< �O���b�h�\���p
	ISiNDYRulePtr		m_ipRule;			//!< ��ƖړIDB�Ή�
	HWND				m_hChildWnd;		//!< �q�_�C�A���O�n���h��
	LONG				m_lPosition;
	std::list<CFeatureDef> m_lstDmyFeatureDef;
	std::map< CString, CString > m_mapDB2Alias;
	std::map< CString, CString > m_mapAlias2DB;
};

