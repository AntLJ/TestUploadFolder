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

#include "stdafx.h"

#include "CsvCheckerDlg.h"
#include "stlaux.h"
#include "LogSystem.h"
#include "data_aux.h"
#include "adam2_filelist.h"
#include "AdamSimpleChecker.h"
#include "AdamLinkPointChecker.h"
#include "AdamLinkShapeChecker.h"
#include "AdamNwChecker.h"
#include "AdamNearNodeChecker.h"
#include "AdamRelationChecker.h"

using namespace std;

std::vector<std::vector<Feature> >* sindy::adam2::format::CAdam2LoglcalChecker::sALL_FEATURE = nullptr;
std::vector<std::vector<Row    > >* sindy::adam2::format::CAdam2LoglcalChecker::sALL_ROW     = nullptr;
IProgressCallback*                  sindy::adam2::format::CAdam2LoglcalChecker::sCALLBACK    = nullptr;

CCsvCheckerDlg::~CCsvCheckerDlg()
{
}

CCsvCheckerDlg::CCsvCheckerDlg()
: m_hThread(nullptr)
, m_dwThread(-1)
{
}

LRESULT CCsvCheckerDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtrl, BOOL& bHandled)
{
	EndDialog(IDOK);
	return 0;
}

LRESULT CCsvCheckerDlg::OnInitDialog(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CAxDialogImpl<CCsvCheckerDlg>::OnInitDialog(msg, wParam, lParam, bHandled);
	bHandled = TRUE;

	ModifyStyle(0, WS_THICKFRAME);

	CenterWindow();

	HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
	SetIcon(hIcon, TRUE);
	SetIcon(hIcon, FALSE);

	m_xcDataPathEdit = GetDlgItem(IDC_EDIT_DATA_PATH);
	m_xcStatusMsg    = GetDlgItem(IDC_EDIT_STATUS);

	m_xcStatusMsg.SetLimitText(0x7FFFFFFE);
	m_xcDataPathEdit.SetCueBannerText(_T("データ格納フォルダを指定してください"), TRUE);

	return 0;
}

LRESULT CCsvCheckerDlg::OnSize(UINT umsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = TRUE;

	if (m_xcStatusMsg.IsWindowVisible())
	{
		int cx = LOWORD(lParam);
		int cy = HIWORD(lParam);
		m_xcStatusMsg.SetWindowPos(NULL, 5, 55, cx - 15, cy - 90, SWP_NOZORDER | SWP_NOMOVE);
	}

	return 0;
}

void CCsvCheckerDlg::callback(const wchar_t* format, ...)
{
	USES_CONVERSION;

	CString str;

	va_list ap;
	va_start(ap, format);
	str.FormatV(format, ap);
	va_end(ap);

	CString msg;
	msg.Format(_T("[%s]%s\r\n"), GetNowWString().c_str(), str);
	LOG_I((LPCSTR)CT2CA((LPCTSTR)str));

	auto pos = m_xcStatusMsg.GetWindowTextLength();
	m_xcStatusMsg.SetSel(pos, pos);
	m_xcStatusMsg.ReplaceSel(msg);
}


DWORD WINAPI fRun(LPVOID pVoid)
{
	auto pDlg = reinterpret_cast<CCsvCheckerDlg*>(pVoid);
	return pDlg->run();
}

LRESULT CCsvCheckerDlg::OnRunCheck(WORD wNotifyCode, WORD wID, HWND hWndCtrl, BOOL& bHandled)
{
	bHandled = TRUE;
#if defined(_DEBUG)
	::fRun(this);
#else
	m_hThread = ::CreateThread(NULL, 0, ::fRun, this, 0, &m_dwThread);
#endif

	return 0;
}

using namespace sindy::adam2::format;


DWORD CCsvCheckerDlg::run()
{
	chageControlEnabled(false);

	sindy::adam2::format::CAdam2LoglcalChecker::sCALLBACK = this;

	CString strDataPath;
	m_xcDataPathEdit.GetWindowText(strDataPath);

	USES_CONVERSION;

	updateMsg(_T("---------- 処理開始 ----------"));
	updateMsg(_T("データフォルダ[%s]"), strDataPath);


	// CSVデータ読み込み
	std::vector<std::vector<Feature> > ALL_FEATURE;
	std::vector<std::vector<Row> >     ALL_ROW;
	if (!readCsv(strDataPath, ALL_FEATURE, ALL_ROW)) {
		chageControlEnabled(true);
		return 0;
	}

	sindy::adam2::format::CAdam2LoglcalChecker::sALL_FEATURE = &ALL_FEATURE;
	sindy::adam2::format::CAdam2LoglcalChecker::sALL_ROW     = &ALL_ROW;

	const struct{
		SPCAdam2LoglcalChecker checker;
		std::initializer_list<long> indexes;
	} checkers[] = {
		// 単一フィーチャクラスのチェック
		std::make_shared<CSimpleLaneLinkChecker>() ,    { index_lane_link         },
		std::make_shared<CSimpleLanePointChecker>(),    { index_lane_point        },
		std::make_shared<CSimpleCompartLinkChecker>() , { index_compartment_link  },
		std::make_shared<CSimpleCompartPointChecker>(), { index_compartment_point },
		std::make_shared<CSimpleBorderLinkChecker>() ,  { index_border_link       },
		std::make_shared<CSimpleBorderPointChecker>(),  { index_border_point      },
		std::make_shared<CSimpleSignalPointChecker>(),  { index_signal_point      },
		std::make_shared<CSimpleRoadsignPointChecker>(),{ index_roadsign_point    },
		// 車線専用チェック
		std::make_shared<CSimpleLinkPointChecker>(),  { index_lane_link, index_lane_point},
		// リンク形状チェック
		std::make_shared<CLinkShapeChecker>(), { index_lane_link        },
		std::make_shared<CLinkShapeChecker>(), { index_compartment_link },
		std::make_shared<CLinkShapeChecker>(), { index_border_link      },
		// NWチェック
		std::make_shared<CNwChecker>(std::vector<long>({9, 18})), { index_lane_link,        index_lane_point        },
		std::make_shared<CNwChecker>(),                           { index_compartment_link, index_compartment_point },
		std::make_shared<CNwChecker>(),                           { index_border_link,      index_border_point      },
		// ノード近傍チェック
		std::make_shared<CNearNodeChecker>(2.5), { index_lane_point        },
		std::make_shared<CNearNodeChecker>(7.5), { index_compartment_point },
		std::make_shared<CNearNodeChecker>(7.5), { index_border_point      },
		// リンク関連チェック
		std::make_shared<CLaneCompartLinkRelationChecker>(), { index_lane_link, index_compartment_link, index_rel_lane_link_compartment_link },
		std::make_shared<CLinkRelationChecker>(),            { index_lane_link, index_border_link,      index_rel_lane_link_border_link      },
		// ノードと地物の関連チェック
		std::make_shared<CLaneNodeSignalRelationChecker>(), { index_lane_point, index_signal_point,   index_rel_lane_point_signal_point   },
		std::make_shared<CNodeRelationChecker>(),           { index_lane_point, index_roadsign_point, index_rel_lane_point_roadsign_point },
	};
	
	for (const auto& c : checkers) {
		c.checker->initialize(c.indexes);
		c.checker->run();
	}

	updateMsg(_T("---------- 処理完了 ----------"));

	chageControlEnabled(true);

	sindy::adam2::format::CAdam2LoglcalChecker::sALL_FEATURE = nullptr;
	sindy::adam2::format::CAdam2LoglcalChecker::sALL_ROW = nullptr;
	sindy::adam2::format::CAdam2LoglcalChecker::sCALLBACK = nullptr;

	return 0;
}


bool CCsvCheckerDlg::readCsv(const CString & csv_path, std::vector<std::vector<Feature>>& all_features, std::vector<std::vector<Row>>& all_rows)
{
	for (const auto& itr : kFeatureFileList)
	{
		updateMsg(_T("[INFO][%s]ファイル読み込み開始"), itr.alias_name);

		vector<Feature> vf;

		CString fname;
		fname.Format(_T("%s\\%s"), csv_path, itr.file_name);
		if (!fLoad(fname, itr.checker.get(), vf))
		{
			updateMsg(_T("[ERROR][%s]ファイル読み込みに失敗しました."), itr.alias_name);
			continue;
		}
		map<long, const Feature*> dic;
		if (!fBuildDic(vf, kFEATURE_OID, dic))
		{
			updateMsg(_T("[ERROR][%s]OIDの重複があります."), itr.alias_name);
		}

		all_features.push_back(vf);
		updateMsg(_T("[INFO][%s]ファイル読み込み完了"), itr.alias_name);
		updateMsg(_T("[INFO][%s]レコード %d件."), itr.alias_name, vf.size());
	}
	for (const auto& itr : kRelFileList)
	{
		updateMsg(_T("[INFO][%s]ファイル読み込み開始"), itr.alias_name);

		vector<Row> vr;

		CString fname;
		fname.Format(_T("%s\\%s"), csv_path, itr.file_name);
		if (!fLoad(fname, itr.checker.get(), vr))
		{
			updateMsg(_T("[ERROR][%s]ファイル読み込みに失敗しました."), itr.alias_name);
			continue;
		}
		map<long, const Row*> dic;
		if (!fBuildDic(vr, kTABLE_OID, dic))
		{
			updateMsg(_T("[ERROR][%s]OIDの重複があります."), itr.alias_name);
		}

		all_rows.push_back(vr);
		updateMsg(_T("[INFO][%s]ファイル読み込み完了"), itr.alias_name);
		updateMsg(_T("[INFO][%s]レコード %d件."), itr.alias_name, vr.size());
	}

	if (all_features.size() != _countof(kFeatureFileList))
	{
		updateMsg(_T("[ERROR]フィーチャデータの読み込みに失敗しましたので終了します."));
		return false;
	}
	if (all_rows.size() != _countof(kRelFileList))
	{
		updateMsg(_T("[ERROR]関連データの読み込みに失敗しましたので終了します."));
		return false;
	}

	return true;
}

void CCsvCheckerDlg::updateMsg(const TCHAR * format, ...)
{
	CString str;

	va_list ap;
	va_start(ap, format);
	str.FormatV(format, ap);
	va_end(ap);

	CString msg;
	msg.Format(_T("[%s]%s\r\n"), GetNowWString().c_str(), str);
	LOG_I((LPCSTR)CT2CA((LPCTSTR)str));

	auto pos = m_xcStatusMsg.GetWindowTextLength();
	m_xcStatusMsg.SetSel(pos, pos);
	m_xcStatusMsg.ReplaceSel(msg);
}

void CCsvCheckerDlg::chageControlEnabled(bool enable)
{
	BOOL b = enable ? TRUE : FALSE;
	GetDlgItem(IDC_BUTTON_RUN).EnableWindow(b);
	GetDlgItem(IDC_EDIT_DATA_PATH).EnableWindow(b);
}
