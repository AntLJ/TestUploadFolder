#include "stdafx.h"
#include "ConvertZipCodeData.h"
#include "ConvRelZipAddr.h"
#include "ConvZipPol.h"
#include "ConvCentroid.h"
#include "ConvRep.h"
#include "Log.h"
#include "Common.h"

bool CConvertZipCodeData::init(const Arguments& args)
{
	// �f�[�^�o�͐�ݒ�
	m_strOutDir = args.m_out_dir.c_str();
	m_strOutDir = m_strOutDir.TrimRight(_T("\\"));

	// �X�֔ԍ��|���S���̃��[�N�X�y�[�X�擾
	IWorkspacePtr ipWork = sindy::create_workspace(args.m_zip_db.c_str());
	if(!ipWork){
		CLog::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::NgDBConnect, CString(args.m_zip_db.c_str()));
		return false;
	}

	// �X�֔ԍ��|���S���̃e�[�u�����ݒ�
	CString strZipPol;
	if(args.m_zip_schema.empty())
		strZipPol = sindy::schema::zipcode_area::kTableName;
	else
		strZipPol = CString(args.m_zip_schema.c_str()) + _T(".") + sindy::schema::zipcode_area::kTableName;

	// �X�֔ԍ��|���S���̃t�B�[�`���N���X�擾
	if(S_OK != IFeatureWorkspacePtr(ipWork)->OpenFeatureClass(CComBSTR(strZipPol), &m_ipZipAreaClass)){
		CLog::GetInstance().PrintLog2(false, true, false, true, err_type::fatal, err_code::NgOpenTable, strZipPol, _T(""));
		return false;
	}

	// 11���Z���f�[�^�i�[��ݒ�
	m_strAddr11Dir = args.m_addr11_dir.c_str();

	// �Z���R�t���e�[�u���i20���R�[�h�j�̃e�[�u�����ݒ�
	CString strRelAddr;
	if(args.m_zip_schema.empty())
		strRelAddr = sindy::schema::rel_addr::kTableName20;
	else
		strRelAddr = CString(args.m_zip_schema.c_str()) + _T(".") + sindy::schema::rel_addr::kTableName20;

	// �Z���R�t���e�[�u���i20���R�[�h�j�̃e�[�u���擾
	if(S_OK != IFeatureWorkspacePtr(ipWork)->OpenTable(CComBSTR(strRelAddr), &m_ipRelAddr)){
		CLog::GetInstance().PrintLog2(false, true, false, true, err_type::fatal, err_code::NgOpenTable, strRelAddr, _T(""));
		return false;
	}

	// �s�撬����\�_�ڑ����[�h�̏Z��DB�ڑ���
	m_strAddrDB = args.m_addr_db.c_str();
	if(!m_strAddrDB.IsEmpty()){
		std::vector<CString> vecAddr = uh::str_util::split(m_strAddrDB, _T("@"));
		if(2 != vecAddr.size()){
			CLog::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::NgAddrDBConnectString, m_strAddrDB);
			return false;
		}
		// MS��ODBC���g�p
		CString strConnect;
		strConnect.Format(_T("Driver={Microsoft ODBC for Oracle};Server=%s;UID=%s;PWD=%s;"), vecAddr[1], vecAddr[0], vecAddr[0]);
		if(!m_cADOBase.connect_by_string(strConnect)){
			CLog::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::NgDBConnect, strConnect);
			return false;
		}
	}

	return true;
}

bool CConvertZipCodeData::run()
{
	// �Z���R�t���e�[�u���ϊ�
	CLog::GetInstance().PrintLog(true, false, true, false, _T("���Z���R�t���e�[�u���ϊ�"), true);
	CConvRelZipAddr cConvRelZipAddr(m_strOutDir, m_ipRelAddr, m_strAddr11Dir);
	if(!cConvRelZipAddr.init())
		return false;
	if(!cConvRelZipAddr.convert())
		return false;

	// �s�撬����\�|�C���g�ϊ�
	CLog::GetInstance().PrintLog(true, false, true, false, _T("���s�撬����\�|�C���g�ϊ�"), true);
	CConvRep cConvRep(m_strOutDir, m_ipRelAddr, m_strAddrDB, m_cADOBase);
	if(!cConvRep.init())
		return false;
	if(!cConvRep.convert())
		return false;

	// ��������H���̂ŕs�v�f�[�^���
	cConvRelZipAddr.clearData();
	cConvRep.clearData();

	// �X�֔ԍ��|���S���ϊ�
	CLog::GetInstance().PrintLog(true, false, true, false, _T("���X�֔ԍ��|���S���ϊ�"), true);
	CConvZipPol cConvZipPol(m_strOutDir, m_ipZipAreaClass);
	if(!cConvZipPol.init())
		return false;
	if(!cConvZipPol.convert())
		return false;

	// �d�S�|�C���g�ϊ�
	CLog::GetInstance().PrintLog(true, false, true, false, _T("���d�S�|�C���g�ϊ�"), true);
	CConvCentroid cConvCentroid(m_strOutDir);
	if(!cConvCentroid.init())
		return false;
	cConvCentroid.setMergedZipGeom(cConvZipPol.getMergedZipGeom()); //!< �}�[�W��̗X�֔ԍ��|���S����ݒ�
	cConvZipPol.clearData();
	if(!cConvCentroid.convert())
		return false;

	return true;
}
