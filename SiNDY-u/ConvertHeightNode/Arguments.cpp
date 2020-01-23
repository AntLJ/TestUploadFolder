#include "StdAfx.h"
#include "Arguments.h"
#include "LogSys.h"
#include <TDC/useful_headers/directory_util.h>
#include <WinLib/VersionInfo.h>

// �R�}���h���C�������̉��
bool Arguments::parse(int argc, _TCHAR* argv[])
{
	po::options_description desc("How to use");

	try{
		// �p�����[�^�ǂݍ���
		desc.add_options()
			(INI, uh::tvalue<uh::tstring>(), "�ݒ�t�@�C��(�K�{)")
			(PROCLOG, uh::tvalue<uh::tstring>(), "���s���O(�C��)")
			(ERRLOG, uh::tvalue<uh::tstring>(), "�G���[���O(�C��)");
		if (1 == argc) {
			std::cout << desc << std::endl;
			return WriteUsage();
		}

		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);

		// �p�����[�^�`�F�b�N
		if (! CheckParam(vm)) {
			std::cout << desc << std::endl;
			return WriteUsage();
		}

		// �p�����[�^�ǂݍ���
		SetParam(vm);

		// ���O�I�[�v��
		if (m_procLog.size() != 0) {
			if (! LogSys::GetInstance().SetProcLog(m_procLog.c_str())) {
				std::cerr << "EEROR\t" << PROCLOG << "�I�v�V�������s���ł�" << std::endl;
				return false;
			}
		}
		if (m_errLog.size() != 0) {
			if (! LogSys::GetInstance().SetErrLog(m_errLog.c_str())) {
				std::cerr << "ERROR\t" << ERRLOG << "�I�v�V�������s���ł�" << std::endl;
				return false;
			}
			LogSys::GetInstance().WriteErrLog(false, false, _T("# FREESTYLELOG\n"));
			LogSys::GetInstance().WriteErrLog(false, false, _T("# �t���O\t���C����\t�I�u�W�F�N�gID\t�G���[���x��\t�G���[���b�Z�[�W\n"));
		}
		// �ݒ�t�@�C���ǂݍ���
		po::variables_map vmContent;
		po::options_description descContent("How to set a ini file");
		descContent.add_options()
			(INPUTDIR, uh::tvalue<uh::tstring>(), "�ϊ���Shape�i�[�f�B���N�g��(Road�f�B���N�g���܂ł̃p�X)(�K�{)")
			(HEIGHTDB, uh::tvalue<uh::tstring>(), "HEIGHT_NODE�i�[DB(�K�{)")
			(HEIGHTSCHEMA, uh::tvalue<uh::tstring>(), "HEIGHT_NODE�i�[�X�L�[�}(�C��)")
			(MESHDB, uh::tvalue<uh::tstring>(), "BASEMESH�i�[DB(�K�{)")
			(MESHSCHEMA, uh::tvalue<uh::tstring>(), "BASEMESH�i�[�X�L�[�}(�C��)")
			(MESHLIST, uh::tvalue<uh::tstring>(), "�ϊ��Ώۃ��b�V�����X�g(�K�{)")
			(BUFFER, uh::tvalue<double>(), "�����m�[�h�o�b�t�@(�C��:�����l=0.0)")
			(OUTPUTDIR, uh::tvalue<uh::tstring>(), "�o�̓f�B���N�g��(�K�{)");
		std::ifstream ifsIniFile(CT2CA(CString(m_iniFile.c_str())));
		if (! ifsIniFile.is_open()) {
			std::cerr << "ERROR\t�ݒ�t�@�C���̃I�[�v���Ɏ��s���܂���" << std::endl;
			std::cout << desc << std::endl;
			return false;
		}
		po::store(po::parse_config_file(ifsIniFile, descContent), vmContent);
		po::notify(vmContent);

		// �ݒ�t�@�C���`�F�b�N
		if (! CheckIni(vmContent)){
			std::cout << desc << std::endl;
			return WriteUsage();
		}
		
		// �ݒ�t�@�C���ǂݍ���
		SetIni(vmContent);

		// �o�[�W�����E�I�v�V�������\��
		ProcInfoWrite(vmContent);
	}
	catch(const std::exception& e){
		std::cerr << e.what() << std::endl;
		std::cerr << desc << std::endl;
		return false;
	}
	return true;
}

bool Arguments::WriteUsage() const
{
	std::cout << "How to set ini_file" << std::endl;
	std::cout << "  " << INPUTDIR << " = <�ϊ���Shape�i�[�f�B���N�g��(Road�f�B���N�g���܂ł̃p�X)(�K�{)>" << std::endl;
	std::cout << "  " << HEIGHTDB << " = <HEIGHT_NODE�i�[DB>(�K�{)" << std::endl;
	std::cout << "  " << HEIGHTSCHEMA << " = <HEIGHT_NODE�i�[�X�L�[�}(�C��)>" << std::endl;
	std::cout << "  " << MESHDB << " = <BASEMESH�i�[DB>(�K�{)" << std::endl;
	std::cout << "  " << MESHSCHEMA << " = <BASEMESH�i�[�X�L�[�}(�C��)>" << std::endl;
	std::cout << "  " << MESHLIST << " = <�ϊ��Ώۃ��b�V�����X�g>(�K�{)" << std::endl;
	std::cout << "  " << BUFFER  << " = <�����m�[�h�o�b�t�@>(�C��:�����l=0.0)" << std::endl;
	std::cout << "  " << OUTPUTDIR << " = <�o�̓f�B���N�g��>(�K�{)" << std::endl;
	return false;
}

bool Arguments::CheckParam(const po::variables_map& vm) const
{
	bool ret = true;
	ret &= FileExistChk(vm, INI);
	if (vm.count(PROCLOG) != 0) {
		ret &= FileDirExistChk(vm, PROCLOG);
	}
	if (vm.count(ERRLOG) != 0) {
		ret &= FileDirExistChk(vm, ERRLOG);
	}
	return ret;
}

bool Arguments::OptionExistChk(const po::variables_map& vm, LPCSTR option) const
{
	if (vm.count(option) == 0) {
		std::cerr << "ERROR\t" << option << "���ݒ肳��Ă��܂���" << std::endl;
		return false;
	}
	return true;
}

bool Arguments::FileExistChk(const po::variables_map& vm, LPCSTR option) const
{
	if (! OptionExistChk(vm, option))
		return false;
	if (! uh::isFile(vm[option].as<uh::tstring>().c_str())) {
		std::cerr << "ERROR\t" << option << "�Ŏw�肵���t�@�C�������݂��܂���" << std::endl;
		return false;
	}
	return true;
}

bool Arguments::DirExistChk(const po::variables_map& vm, LPCSTR option) const
{
	if (! OptionExistChk(vm, option))
		return false;
	if (! uh::isDirectory(vm[option].as<uh::tstring>().c_str())) {
		std::cerr << "ERROR\t" << option << "�Ŏw�肵���f�B���N�g�������݂��܂���" << std::endl;
		return false;
	}
	return true;
}

bool Arguments::FileDirExistChk(const po::variables_map& vm, LPCSTR option) const
{
	CString filePath = vm[option].as<uh::tstring>().c_str();
	long dirPos = filePath.ReverseFind(_T('\\'));
	CString dirPath = filePath.Left(dirPos);
	if (! uh::isDirectory(dirPath)) {
		std::cerr << "ERROR\t" << option << "�Ŏw�肵���t�@�C�����i�[����f�B���N�g�������݂��܂���" << std::endl;
		return false;
	}
	return true;
}

void Arguments::SetParam(const po::variables_map& vm)
{
	m_iniFile = vm[INI].as<uh::tstring>();
	if (vm.count(PROCLOG) != 0) {
		m_procLog = vm[PROCLOG].as<uh::tstring>();
	}
	if (vm.count(ERRLOG) != 0) {
		m_errLog = vm[ERRLOG].as<uh::tstring>();
	}
}

bool Arguments::CheckIni(const po::variables_map& vm) const
{
	bool ret = true;
	ret &= DirExistChk(vm, INPUTDIR);
	ret &= OptionExistChk(vm, HEIGHTDB);
	ret &= OptionExistChk(vm, MESHDB);
	ret &= DirExistChk(vm, OUTPUTDIR);
	ret &= FileExistChk(vm, MESHLIST);
	return ret;
}

void Arguments::SetIni(const po::variables_map& vm)
{
	m_inputDir = vm[INPUTDIR].as<uh::tstring>();
	m_heightNoodeDB = vm[HEIGHTDB].as<uh::tstring>();
	if (vm.count(HEIGHTSCHEMA) != 0)
		m_heightNodeSchema = vm[HEIGHTSCHEMA].as<uh::tstring>();
	m_baseMeshDB = vm[MESHDB].as<uh::tstring>();
	if (vm.count(MESHSCHEMA) != 0)
		m_baseMeshSchema = vm[MESHSCHEMA].as<uh::tstring>();
	m_meshList = vm[MESHLIST].as<uh::tstring>();
	m_outputDir = vm[OUTPUTDIR].as<uh::tstring>();
	if (vm.count(BUFFER) != 0)
		m_buffer = vm[BUFFER].as<double>();
}

void Arguments::ProcInfoWrite(const po::variables_map& vm) const
{
	// �c�[�����̕\��
	CVersion toolVersion;
	CString toolInfo;
	toolInfo.Format(_T("%s VERSION:%s\n"), toolVersion.GetOriginalFilename(), toolVersion.GetFileVersion());
	LogSys::GetInstance().WriteProcLog(true, false, toolInfo);
	// �ݒ���e�̕\��
	LogSys::GetInstance().WriteProcLog(true, false, _T("option\n"));
	WriteOptionVal(INPUTDIR, m_inputDir.c_str());
	WriteOptionVal(HEIGHTDB, m_heightNoodeDB.c_str());
	if (vm.count(HEIGHTSCHEMA) > 0)
		WriteOptionVal(HEIGHTSCHEMA, m_heightNodeSchema.c_str());
	WriteOptionVal(MESHDB, m_baseMeshDB.c_str());
	if (vm.count(MESHSCHEMA) > 0)
		WriteOptionVal(MESHSCHEMA, m_baseMeshSchema.c_str());
	WriteOptionVal(MESHLIST, m_meshList.c_str());
	if (vm.count(BUFFER) > 0)
		WriteOptionVal(BUFFER, CA2CT(std::to_string(m_buffer).c_str()));
	else
		WriteOptionVal(BUFFER, _T("0.0"));
	WriteOptionVal(OUTPUTDIR, m_outputDir.c_str());
}

void Arguments::WriteOptionVal(LPCSTR option, LPCTSTR val) const
{
	CString opAndVal;
	opAndVal.Format(_T("  %s\t%s\n"), CString(option), val);
	LogSys::GetInstance().WriteProcLog(true, false, opAndVal);
}