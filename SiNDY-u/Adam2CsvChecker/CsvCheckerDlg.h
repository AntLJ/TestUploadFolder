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

#include "ProgressCallback.h"

struct Feature;
struct Row;

class CCsvCheckerDlg : public CAxDialogImpl<CCsvCheckerDlg>, public IProgressCallback {
public:
	enum {IDD = IDD_CSVCHECKER_DIALOG };

BEGIN_MSG_MAP(CCsvCheckerDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_SIZE,       OnSize)
	COMMAND_HANDLER(IDCANCEL,       BN_CLICKED, OnCancel)
	COMMAND_HANDLER(IDC_BUTTON_RUN, BN_CLICKED, OnRunCheck)

	CHAIN_MSG_MAP(CAxDialogImpl<CCsvCheckerDlg>)
END_MSG_MAP()

public:
	~CCsvCheckerDlg();
	CCsvCheckerDlg ();

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnSize      (UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCancel    (WORD wNotifyCode, WORD wID, HWND hWndCtrl, BOOL& bHandled);
	LRESULT OnRunCheck  (WORD wNotifyCode, WORD wID, HWND hWndCtrl, BOOL& bHandled);

	DWORD   run();
	void    callback(const wchar_t* format, ...);

private:
	/**
	* @brief  CSVファイル読み込み
	* @@param[in]  csv_path     CSVファイル格納フォルダパス
	* @@param[out] all_features 形状を持つデータ読み込み先のコンテナ
	* @@param[out] all_row      形状を持たないデータ読み込み先のコンテナ
	* @retval true   読み込み成功
	* @retval false  読み込み失敗
	*/
	bool readCsv(const CString& csv_path, std::vector<std::vector<Feature>>& all_features, std::vector<std::vector<Row>>& all_rows);

	/**
	* @brief メッセージ更新
	* @detail ダイアログ上のエディットボックスのメッセージ更新、及びログ出力
	* @@param[in] format メッセージ
	*/
	void updateMsg(const TCHAR* format, ...);

	/**
	* @brief コントロール有効/無効切り替え
	*/
	void chageControlEnabled(bool enable);

private:
	CEdit   m_xcDataPathEdit;
	CEdit   m_xcSdeConnEdit;
	CEdit   m_xcStatusMsg;

	HANDLE  m_hThread;
	DWORD   m_dwThread;
};
