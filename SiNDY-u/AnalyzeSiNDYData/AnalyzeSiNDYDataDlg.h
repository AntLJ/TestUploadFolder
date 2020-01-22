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
#include "resource.h"
#include "../../Lib/SizingFramework/include/WinMgr.h"
#include "../../SiNDY-e/SearchTool/PieChart.h"
#include "../../SiNDY-e/SearchTool/Legend.h"

using namespace sindy;

class CAnalyzeSiNDYDataDlg :
	public CAxDialogImpl<CAnalyzeSiNDYDataDlg>,
	public IDispEventImpl<IDC_GRID_ANALYZE,CAnalyzeSiNDYDataDlg>
{
public:
	CAnalyzeSiNDYDataDlg(void);
	~CAnalyzeSiNDYDataDlg(void);

	enum { IDD = IDD_DIALOG };

	enum AnalyzeType{
		eArea = 0,
		eLength = 1,
		eNum = 2
	};
/////////////////////////////////////////////////////////////////////////////
//
// CAnalyzeSiNDYDataDlg MSG�}�b�v��`
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MSG_MAP(CAnalyzeSiNDYDataDlg)
	MESSAGE_HANDLER(WM_WINMGR, OnWinMgr)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
	MESSAGE_HANDLER(WM_SIZE, OnSize)
	MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
	MESSAGE_HANDLER(WM_CLOSE, OnClose)

	COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnClickedCancel)
	COMMAND_HANDLER(IDOK, BN_CLICKED, OnOK)
	COMMAND_HANDLER(IDC_BUTTON_ANALYZECONNECT, BN_CLICKED, OnBnClickedButtonServerConnect)
	COMMAND_HANDLER(IDC_BUTTON_AREACONNECT, BN_CLICKED, OnBnClickedButtonServerConnect)
	COMMAND_HANDLER(IDC_COMBO_FEATURECLASS, CBN_SELCHANGE, OnCbnSelchangeComboFeatureclass)
	COMMAND_HANDLER(IDC_COMBO_AREAFEATURECLASS, CBN_SELCHANGE, OnCbnSelchangeComboFeatureclass)
	COMMAND_HANDLER(IDC_COMBO_FIELD, CBN_SELCHANGE, OnCbnSelchangeComboField)
	COMMAND_HANDLER(IDC_BUTTON_EXPORT, BN_CLICKED, OnClickedExport)
	COMMAND_HANDLER(IDC_RADIO_SERVER, BN_CLICKED, OnClickedServer)
	COMMAND_HANDLER(IDC_RADIO_FOLDER, BN_CLICKED, OnClickedServer)
END_MSG_MAP()

public:
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnWinMgr(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClickedCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnBnClickedButtonServerConnect(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCbnSelchangeComboFeatureclass(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnClickedExport(WORD/* wNotifyCode*/, WORD/* wID*/, HWND/* hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCbnSelchangeComboField(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnClickedServer(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

/////////////////////////////////////////////////////////////////////////////
//
// CAnalyzeSiNDYDataDlg �V���N�}�b�v
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_SINK_MAP(CAnalyzeSiNDYDataDlg)
	SINK_ENTRY(IDC_GRID_ANALYZE, 5,	OnGridMouseDown)
END_SINK_MAP()
void __stdcall OnGridMouseDown(SHORT Button, SHORT Shift, FLOAT X, FLOAT Y, VARIANT_BOOL * Cancel);

public:
	/**
	 * @brief	���X�g����A�T���G���A���X�g�𒊏o
	 *
	 * @note	�T�����X�g�����b�V�����X�g�ł��A������Ŋi�[����B
	 *
	 * @param	strList	[in]  ���X�g�̃p�X��
	 * @param	listSearchArea	[out]  �T�����X�g
	 *
	 * @return	true:���X�g�쐬���ł���
	 *			false:���X�g�쐬�ł��Ȃ�����
	 */
	bool CreateSearchArea( CString strList, list< CString >& listSearchArea );

	/**
	 * @brief	�O���b�h�̐擪�s�ɕ��ނ킯����������
	 *
	 * @note	���݃R�[�h�l�̃t�B�[���h�݂̂����Ή����Ă��Ȃ�
	 *
	 * @param	mapDomain	[out]  �h���C���̃}�b�v<�R�[�h�l�A�R�[�h��>
	 *
	 * @return	true:�擪�s�ɕ��ނ킯���������߂�
	 *			false:�擪�s�ɕ��ނ킯���������߂Ȃ�����
	 */
	bool CreateGridCol( map<CString, CString >& mapDomain );

	/**
	 * @brief	�O���b�h�̐擪�s���當����ƈ�v������̂������Ă���
	 *
	 * @param	strColName	[in]  �T��������������
	 *
	 * @return	long	����������ԍ�
	 *					������Ȃ�������-1��Ԃ�
	 */
	long FindCol( CString strColName );

	/**
	 * @brief	���ރ^�C�v��Ԃ�
	 *
	 * @return	long	AnalyzeType��Ԃ�
	 */
	long GetAnalyzeType();

	/**
	 * @brief	�`���[�g�����֐��i�����c�[������q�؁j
	 *
	 * @param	lRow	[in]  �O���b�h�̍s�ԍ�
	 */
	void CreateChart( long lRow );

	/**
	 * @brief	���͂��邽�߂Ƀf�[�^�������Ă��邩�ǂ������ׂ�֐�
	 *
	 * @return	true:���͂ł���B
	 *			false:�Ȃɂ������肪����
	 */
	bool CheckAnalyzeInfo();

	/**
	 * @brief	�t�H���_�̒��ɂ���mdb�t�@�C���������Ă���
	 *
	 * @note	mdb�����Ȃ̂ŁAfdb�Ƃ������K�v������
	 *
	 * @param	hFind	[in]  �擪�̃t�@�C���n���h��
	 * @param	fd		[in]  �擪�̃f�[�^�o�b�t�@
	 * @param	lpPath	[in]  �T������t�H���_��
	 * @param	listFileName	[out]  mdb�t�@�C���̃p�X�̃��X�g
	 */
	void FindFile(HANDLE hFind, WIN32_FIND_DATA& fd, LPCTSTR lpPath, std::list<CString> &listFileName);

	/**
	 * @brief	�t�B�[�`���J�[�\���𕪐͂���
	 *
	 * @note	���ۂɂ́AAnalyzeData�̂ق��ōׂ������A���̊֐��͕��͏���
	 *
	 * @param	ipFeatureCursor	[in]  �������邽�߂̃t�B�[�`���J�[�\���i�Ⴆ�΃��b�V����`�Ƃ��A�s���{���̃t�B�[�`���J�[�\���Ƃ��j
	 * @param	lRow		[in]  ���͌��ʂ��ڂ���s��
	 * @param	strRow		[in]  ���͌��ʂ̍��ږ��i���Ƃ��΁A���b�V���ԍ��Ƃ��A�s���{�����Ƃ��j
	 * @param	strQuery	[in]  ���͂��i��N�G��
	 * @param	mapDomain	[in]  �h���C���}�b�v
	 * @param	strRowName	[in]  �i�������ŕ\�����邽�߂́A���ޖ��i�Ⴆ�΁A�u�h���b�V���R�[�h�F123456�������h���̃��b�V���R�[�h�̕����j
	 */
	void AnalyzeFeatureCursor(IFeatureCursorPtr ipFeatureCursor, long lRow, CString strRow, CString strQuery, map< CString, CString > mapDomain, CString strRowName );

	/**
	 * @brief	�f�[�^�𕪐͂���
	 *
	 * @param	ipAreaGeom	[in]  ��������G���A�`��
	 * @param	strQuery	[in]  ���͂��i��N�G��
	 * @param	mapAnalyze		[out]  ���͌���<���ރR�[�h,���͌���>
	 * @param	strRowName	[in]  �i�������ŕ\�����邽�߂́A���ޖ��i�Ⴆ�΁A�u�h���b�V���R�[�h�F123456�������h���̃��b�V���R�[�h�̕����j
	 * @param	bIntersect	[in]  �G���A�`��ŁA�f�[�^�𕪊����邩�ǂ����i��{�͂���ׂ��j
	 */
	void AnalyzeData( IGeometryPtr ipAreaGeom, CString strQuery, map< CString, double >& mapAnalyze, CString strRowName, bool bIntersect );

	/**
	 * @brief	���͌��ʂ��O���b�h�ɕ\������
	 *
	 * @param	mapAnalyze	[in]  ���͌���<���ރR�[�h,���͌���>
	 * @param	mapDomain	[in]  �h���C���}�b�v
	 * @param	lRow		[in]  ���s�ڂɏ������ނ�
	 */
	void OutputGrid( const map< CString, double > &mapAnalyze, const map< CString, CString >& mapDomain, long lRow );

private:
	CWinMgr				m_winMgr;				//!< �E�B���h�E�}�l�[�W���[
	IVSFlexGridPtr		m_spAnalyzeGrid;		//!< �O���b�h�n���h��
	IWorkspacePtr		m_ipAnalyzeWorkspace, m_ipAreaWorkspace; //!< ���͂��郏�[�N�X�y�[�X�ƁA���̓G���A���[�N�X�y�[�X
	IFeatureClassPtr	m_ipAnalyzeFeatureClass, m_ipAreaFeatureClass;	//!< ���͂���t�B�[�`���N���X�ƁA���̓G���A�t�B�[�`���N���X
	long				m_lFieldIndex;		//!< ���ނ���t�B�[���h�C���f�b�N�X
	CString				m_strSearchFieldName;	//!< ���̓G���A�����߂邽�߂̃t�B�[���h��
	esriFieldType		m_eSearchFieldType;		//!< ���̓G���A�����߂邽�߂̃t�B�[���h�̂���t�B�[���h�^�C�v
	list< CString >		m_listPGDBName;		//!< ���͕��@���t�H���_�^�C�v�������ꍇ��PGDB�̖��O
	CPieChart			m_wndChart;			//!< �`���[�g�n���h��
	CLegend				m_wndLegend;		//!< ���W�F���h�n���h��

};
