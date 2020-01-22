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
	* @brief  CSV�t�@�C���ǂݍ���
	* @@param[in]  csv_path     CSV�t�@�C���i�[�t�H���_�p�X
	* @@param[out] all_features �`������f�[�^�ǂݍ��ݐ�̃R���e�i
	* @@param[out] all_row      �`��������Ȃ��f�[�^�ǂݍ��ݐ�̃R���e�i
	* @retval true   �ǂݍ��ݐ���
	* @retval false  �ǂݍ��ݎ��s
	*/
	bool readCsv(const CString& csv_path, std::vector<std::vector<Feature>>& all_features, std::vector<std::vector<Row>>& all_rows);

	/**
	* @brief ���b�Z�[�W�X�V
	* @detail �_�C�A���O��̃G�f�B�b�g�{�b�N�X�̃��b�Z�[�W�X�V�A�y�у��O�o��
	* @@param[in] format ���b�Z�[�W
	*/
	void updateMsg(const TCHAR* format, ...);

	/**
	* @brief �R���g���[���L��/�����؂�ւ�
	*/
	void chageControlEnabled(bool enable);

private:
	CEdit   m_xcDataPathEdit;
	CEdit   m_xcSdeConnEdit;
	CEdit   m_xcStatusMsg;

	HANDLE  m_hThread;
	DWORD   m_dwThread;
};
