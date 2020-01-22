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
// CAnalyzeSiNDYDataDlg MSGマップ定義
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
// CAnalyzeSiNDYDataDlg シンクマップ
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_SINK_MAP(CAnalyzeSiNDYDataDlg)
	SINK_ENTRY(IDC_GRID_ANALYZE, 5,	OnGridMouseDown)
END_SINK_MAP()
void __stdcall OnGridMouseDown(SHORT Button, SHORT Shift, FLOAT X, FLOAT Y, VARIANT_BOOL * Cancel);

public:
	/**
	 * @brief	リストから、探索エリアリストを抽出
	 *
	 * @note	探索リストがメッシュリストでも、文字列で格納する。
	 *
	 * @param	strList	[in]  リストのパス名
	 * @param	listSearchArea	[out]  探索リスト
	 *
	 * @return	true:リスト作成ができた
	 *			false:リスト作成できなかった
	 */
	bool CreateSearchArea( CString strList, list< CString >& listSearchArea );

	/**
	 * @brief	グリッドの先頭行に分類わけを書き込む
	 *
	 * @note	現在コード値のフィールドのみしか対応していない
	 *
	 * @param	mapDomain	[out]  ドメインのマップ<コード値、コード名>
	 *
	 * @return	true:先頭行に分類わけを書き込めた
	 *			false:先頭行に分類わけを書き込めなかった
	 */
	bool CreateGridCol( map<CString, CString >& mapDomain );

	/**
	 * @brief	グリッドの先頭行から文字列と一致するものを見つけてくる
	 *
	 * @param	strColName	[in]  探索したい文字列
	 *
	 * @return	long	見つかった列番号
	 *					見つからなかったら-1を返す
	 */
	long FindCol( CString strColName );

	/**
	 * @brief	分類タイプを返す
	 *
	 * @return	long	AnalyzeTypeを返す
	 */
	long GetAnalyzeType();

	/**
	 * @brief	チャートを作る関数（検索ツールから拝借）
	 *
	 * @param	lRow	[in]  グリッドの行番号
	 */
	void CreateChart( long lRow );

	/**
	 * @brief	分析するためにデータが揃っているかどうか調べる関数
	 *
	 * @return	true:分析できる。
	 *			false:なにかしら問題がある
	 */
	bool CheckAnalyzeInfo();

	/**
	 * @brief	フォルダの中にあるmdbファイルを見つけてくる
	 *
	 * @note	mdbだけなので、fdbとかもやる必要あるよね
	 *
	 * @param	hFind	[in]  先頭のファイルハンドル
	 * @param	fd		[in]  先頭のデータバッファ
	 * @param	lpPath	[in]  探索するフォルダ名
	 * @param	listFileName	[out]  mdbファイルのパスのリスト
	 */
	void FindFile(HANDLE hFind, WIN32_FIND_DATA& fd, LPCTSTR lpPath, std::list<CString> &listFileName);

	/**
	 * @brief	フィーチャカーソルを分析する
	 *
	 * @note	実際には、AnalyzeDataのほうで細かくやる、この関数は分析準備
	 *
	 * @param	ipFeatureCursor	[in]  分割するためのフィーチャカーソル（例えばメッシュ矩形とか、都道府県のフィーチャカーソルとか）
	 * @param	lRow		[in]  分析結果を載せる行数
	 * @param	strRow		[in]  分析結果の項目名（たとえば、メッシュ番号とか、都道府県名とか）
	 * @param	strQuery	[in]  分析を絞るクエリ
	 * @param	mapDomain	[in]  ドメインマップ
	 * @param	strRowName	[in]  進捗処理で表示するための、分類名（例えば、「”メッシュコード：123456を処理”中のメッシュコードの部分）
	 */
	void AnalyzeFeatureCursor(IFeatureCursorPtr ipFeatureCursor, long lRow, CString strRow, CString strQuery, map< CString, CString > mapDomain, CString strRowName );

	/**
	 * @brief	データを分析する
	 *
	 * @param	ipAreaGeom	[in]  分割するエリア形状
	 * @param	strQuery	[in]  分析を絞るクエリ
	 * @param	mapAnalyze		[out]  分析結果<分類コード,分析結果>
	 * @param	strRowName	[in]  進捗処理で表示するための、分類名（例えば、「”メッシュコード：123456を処理”中のメッシュコードの部分）
	 * @param	bIntersect	[in]  エリア形状で、データを分割するかどうか（基本はきるべき）
	 */
	void AnalyzeData( IGeometryPtr ipAreaGeom, CString strQuery, map< CString, double >& mapAnalyze, CString strRowName, bool bIntersect );

	/**
	 * @brief	分析結果をグリッドに表示する
	 *
	 * @param	mapAnalyze	[in]  分析結果<分類コード,分析結果>
	 * @param	mapDomain	[in]  ドメインマップ
	 * @param	lRow		[in]  何行目に書き込むか
	 */
	void OutputGrid( const map< CString, double > &mapAnalyze, const map< CString, CString >& mapDomain, long lRow );

private:
	CWinMgr				m_winMgr;				//!< ウィンドウマネージャー
	IVSFlexGridPtr		m_spAnalyzeGrid;		//!< グリッドハンドル
	IWorkspacePtr		m_ipAnalyzeWorkspace, m_ipAreaWorkspace; //!< 分析するワークスペースと、分析エリアワークスペース
	IFeatureClassPtr	m_ipAnalyzeFeatureClass, m_ipAreaFeatureClass;	//!< 分析するフィーチャクラスと、分析エリアフィーチャクラス
	long				m_lFieldIndex;		//!< 分類するフィールドインデックス
	CString				m_strSearchFieldName;	//!< 分析エリアを決めるためのフィールド名
	esriFieldType		m_eSearchFieldType;		//!< 分析エリアを決めるためのフィールドのするフィールドタイプ
	list< CString >		m_listPGDBName;		//!< 分析方法がフォルダタイプだった場合のPGDBの名前
	CPieChart			m_wndChart;			//!< チャートハンドル
	CLegend				m_wndLegend;		//!< レジェンドハンドル

};
