#include "stdafx.h"
#include "CheckZipCodeArea.h"
#include "Logger.h"

using namespace std;
using namespace exec_opt;

// ������
bool CCheckZipCodeArea::init()
{
	return true;
}

// ���s
bool CCheckZipCodeArea::execute()
{
	// ���O����
	if (!RUNLOGGER.Initialize(m_args.m_runLog))
	{
		cerr << "���s���O�쐬�Ɏ��s" << endl;
		return false;
	}

	if (!ERRLOGGER.Initialize(m_args.m_errLog))
	{
		cerr << "�G���[���O�쐬�Ɏ��s" << endl;
		return false;
	}

	RUNLOGGER.Info(m_args.GetAllOptionInfo().GetString());
	RUNLOGGER.Info(_T("�����J�n\n"));

	// ������
	if (!init())
	{
		cerr << "�������������s �G���[���O���m�F���Ă�������" << endl;
		return false;
	}

	// �`�F�b�N���s
	SetCheckObject(m_args.m_checkMode);
	bool result = DoCheck();

	RUNLOGGER.Info(_T("�����I��\n"));
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
