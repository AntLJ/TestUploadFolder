#include "StdAfx.h"
#include "Arguments.h"
#include "Log.h"
#include "Common.h"
#include <iostream>
#include <WinLib/VersionInfo.h>

using namespace std;
using namespace uh;
namespace po = boost::program_options;

const char * const k_zip_db = "zip_db";
const char * const k_zip_schema = "zip_schema";
const char * const k_addr_db = "addr_db";
const char * const k_addr11_dir = "addr11_dir";
const char * const k_out_dir = "out_dir";
const char * const k_run_log = "run_log";
const char * const k_err_log = "err_log";

// �R�}���h���C�������̉��
bool Arguments::parse(int argc, _TCHAR* argv[])
{
	po::options_description desc("How to use");

	try
	{
		desc.add_options()
			(k_zip_db,		tvalue<tstring>(&m_zip_db),		"[�K�{]�X�֔ԍ��|���S���̐ڑ���")
			(k_zip_schema,	tvalue<tstring>(&m_zip_schema),	"[�C��]�X�֔ԍ��|���S���̃X�L�[�}��")
			(k_addr_db,		tvalue<tstring>(&m_addr_db),	"[�K�{]�Z��DB�ڑ���")
			(k_addr11_dir,	tvalue<tstring>(&m_addr11_dir),	"[�C��]11���Z���f�[�^�i�[��")
			(k_out_dir,		tvalue<tstring>(&m_out_dir),	"[�K�{]�f�[�^�o�͐�")
			(k_run_log,		tvalue<tstring>(&m_run_log),	"[�K�{]���s���O")
			(k_err_log,		tvalue<tstring>(&m_err_log),	"[�K�{]�G���[���O")
			;

		po::store(po::parse_command_line(argc, argv, desc), m_vm);
		po::notify(m_vm);

		// �K�{�p�����[�^�̃`�F�b�N
		bool good = true;
		if(m_zip_db.empty() ||
			m_addr_db.empty() ||
			m_out_dir.empty() ||
			m_run_log.empty() ||
			m_err_log.empty()){
				std::cerr << "�K�{�I�v�V�������w�肳��Ă��܂���" << std::endl;
				good = false;
		} else {
			// �o�͐�f�B���N�g���̑��݊m�F
			if(!PathIsDirectory(m_out_dir.c_str())){
				std::cerr << k_out_dir << "�I�v�V���� : �u" << toStr(m_out_dir) << "�v�̃f�B���N�g���͑��݂��܂���" << std::endl;
				good = false;
			}

			// �Z���f�[�^�i�[��f�B���N�g�������݂��邩
			if(!m_addr11_dir.empty()){
				if(!PathIsDirectory(m_addr11_dir.c_str())){
					std::cerr << k_addr11_dir << "�I�v�V���� : �u" << toStr(m_addr11_dir) << "�v�̃f�B���N�g���͑��݂��܂���" << std::endl;
					good = false;
				}
			}

			// ���O������
			if(good){
				if(!initLog(m_run_log.c_str(), m_err_log.c_str())){
					good = false;
				}
			}
		}
		if(!good){
			cerr << desc << endl;
			return false;
		}
	}
	catch(const std::exception& e)
	{
		cerr << e.what() << endl;
		cerr << desc << endl;
		return false;
	}
	return true;
}

CString Arguments::GetOption() const
{
	CString strOption = _T("[option]\n");
	for(const auto& op : m_vm){
		CString strOptionLine;
		strOptionLine.Format(_T("   --%s %s\n"), CString(op.first.c_str()), CString(op.second.as<uh::tstring>().c_str()));
		strOption += strOptionLine;
	}

	return strOption;
}

bool Arguments::initLog(const CString& strRunLog, const CString& strErrLog)
{
	// ���O�t�@�C���쐬
	if(!CLog::GetInstance().SetRunLog(strRunLog) ||
		!CLog::GetInstance().SetErrLog(strErrLog)){
		return false;
	}
	CLog::GetInstance().PrintLog(false, false, false, true,_T("#FREESTYLELOG"));
	CLog::GetInstance().PrintLog(false, false, false, true,_T("ERROR_TYPE\tTABLE_NAME\tOBJECTID\tERROR_CODE\tERROR_MSG\t"));

	CVersion cVer;
	CString strMsg;
	strMsg.Format(_T("%s FILEVERSION:%s  PRODUCTVERSION:%s"), cVer.GetOriginalFilename(), cVer.GetFileVersion(), cVer.GetProductVersion());
	CLog::GetInstance().PrintLog(true, false, true, false, strMsg);
	CLog::GetInstance().PrintLog(true, false, true, false, GetOption());

	strMsg.Format(_T("Start : %s\n"), getNowTime());
	CLog::GetInstance().PrintLog(true, false, true, false, strMsg);

	return true;
}
