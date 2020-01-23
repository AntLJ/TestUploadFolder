#include "stdafx.h"
#include "CheckZipCodeArea.h"
#include "Logger.h"

using namespace std;
using namespace exec_opt;

// 初期化
bool CCheckZipCodeArea::init()
{
	return true;
}

// 実行
bool CCheckZipCodeArea::execute()
{
	// ログ準備
	if (!RUNLOGGER.Initialize(m_args.m_runLog))
	{
		cerr << "実行ログ作成に失敗" << endl;
		return false;
	}

	if (!ERRLOGGER.Initialize(m_args.m_errLog))
	{
		cerr << "エラーログ作成に失敗" << endl;
		return false;
	}

	RUNLOGGER.Info(m_args.GetAllOptionInfo().GetString());
	RUNLOGGER.Info(_T("処理開始\n"));

	// 初期化
	if (!init())
	{
		cerr << "初期化処理失敗 エラーログを確認してください" << endl;
		return false;
	}

	// チェック実行
	SetCheckObject(m_args.m_checkMode);
	bool result = DoCheck();

	RUNLOGGER.Info(_T("処理終了\n"));
	return result;
}

void CCheckZipCodeArea::SetCheckObject(const uh::tstring& checkMode)
{
	m_checkClassPtr = nullptr;

	if (0 == checkMode.compare(CHECK_ALL))
		m_checkClassPtr = std::make_shared<CheckAll>();
	else if (0 == checkMode.compare(CHECK_TABLE))
		m_checkClassPtr = std::make_shared<CheckTable>();
	else if (0 == checkMode.compare(CHECK_TABLEMASTER))
		m_checkClassPtr = std::make_shared<CheckTableAndMaster>();
	else if (0 == checkMode.compare(CHECK_TABLEADDR))
		m_checkClassPtr = std::make_shared<CheckTableAndAddr>();
	else if (0 == checkMode.compare(CHECK_POLYGON))
		m_checkClassPtr = std::make_shared<CheckPolygon>();
	else if (0 == checkMode.compare(CHECK_POLYGONTABLE))
		m_checkClassPtr = std::make_shared<CheckPolygonAndTable>();
	else if (0 == checkMode.compare(CHECK_POLYGONADDR))
		m_checkClassPtr = std::make_shared<CheckPolygonAndAddr>();

	if (m_checkClassPtr)
		m_checkClassPtr->SetArgument(m_args);
}

bool CCheckZipCodeArea::DoCheck()
{
	if (!m_checkClassPtr)
		return false;

	if (!m_checkClassPtr->Init())
		return false;

	return m_checkClassPtr->Check();
}
