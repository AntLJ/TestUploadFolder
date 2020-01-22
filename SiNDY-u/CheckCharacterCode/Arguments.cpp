#include "stdafx.h"
#include "Arguments.h"
#include <TDC/useful_headers/directory_util.h>
#include <TDC/useful_headers/boost_program_options.h>

using namespace uh;
namespace po = boost::program_options;

// �R�}���h���C�������̉��
bool Arguments::parse(int argc, _TCHAR* argv[])
{
	po::options_description desc("�g����");

	try{
		desc.add_options()
			("help,h", "�w���v��\��")
			(g_strFileListPath,		uh::tvalue<uh::tstring>(&strFileListPath),		"[�K�{]���̓t�@�C�����X�g�̃p�X")
			(g_strFileType,			uh::tvalue<uh::tstring>(&strFileType),			"[�K�{]�`�F�b�N�Ώۃt�@�C���̃t�H�[�}�b�g [shape]")
			(g_strCharCodeListPath,	uh::tvalue<uh::tstring>(&strCharCodeListPath),	"[�K�{]���e�����R�[�h�͈͎w��t�@�C���̃p�X")
			(g_strExitMode,			uh::tvalue<uh::tstring>(&strExitMode),			"[�K�{]�s�������R�[�h���o���̏I�����[�h [immediate | all]")
			(g_strRunLogFilePath,	uh::tvalue<uh::tstring>(&strRunLogFilePath),	"[�K�{]���s���O�t�@�C���̃p�X")
			(g_strErrLogFilePath,	uh::tvalue<uh::tstring>(&strErrLogFilePath),	"[�K�{]�G���[���O�t�@�C���̃p�X")
			;

		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);

		if(argc == 1){
			std::cout << desc << std::endl;
			return false;
		}

		// �I�v�V�����`�F�b�N
		if(vm.count(g_strFileListPath) == 0 || vm.count(g_strFileType) == 0 || vm.count(g_strCharCodeListPath) == 0 ||
			vm.count(g_strExitMode) == 0 || vm.count(g_strRunLogFilePath) == 0 || vm.count(g_strErrLogFilePath) == 0){
				std::cout << "�����̐��������܂���" << std::endl;
				std::cout << desc << std::endl;
				return false;
		}
	} catch(const std::exception& e) {
		std::cerr << e.what() << std::endl;
		std::cerr << desc << std::endl;
		return false;
	}
	return true;
}


CString Arguments::GetOption()
{
	CString strOption = _T("[option]\n");
	if(!strFileListPath.empty())
		strOption += GetOptionLine(g_strFileListPath, strFileListPath.c_str());

	if(!strFileType.empty())
		strOption += GetOptionLine(g_strFileType, strFileType.c_str());

	if(!strCharCodeListPath.empty())
		strOption += GetOptionLine(g_strCharCodeListPath, strCharCodeListPath.c_str());

	if(!strExitMode.empty())
		strOption += GetOptionLine(g_strExitMode, strExitMode.c_str());

	if(!strRunLogFilePath.empty())
		strOption += GetOptionLine(g_strRunLogFilePath, strRunLogFilePath.c_str());

	if(!strErrLogFilePath.empty())
		strOption += GetOptionLine(g_strErrLogFilePath, strErrLogFilePath.c_str());

	return strOption;
}


CString Arguments::GetOptionLine(const CString& strParam, const CString& strValue)
{
	CString strOptionLine = _T("   --");
	strOptionLine.Format(_T("   --%s %s\n"), strParam, strValue);

	return strOptionLine;
}


bool Arguments::CheckOptionParameter()
{
	// ���̓t�@�C�����X�g���Ȃ���΃G���[
	if(!PathFileExists(strFileListPath.c_str())){
		std::cout << "���̓t�@�C�����X�g�F" << CT2A(strFileListPath.c_str()) << " ������܂���" << std::endl;
		return false;
	}

	// �`�F�b�N�Ώۃt�@�C���̃t�H�[�}�b�g�� [shape] �łȂ���΃G���[
	if(strFileType.c_str() != g_strSHAPE){
		std::cout << "�t�@�C���^�C�v�� shape �ł͂���܂���" << std::endl;
		return false;
	}

	// �����R�[�h�͈͎w��t�@�C�����Ȃ���΃G���[
	if(!PathFileExists(strCharCodeListPath.c_str())){
		std::cout << "�����R�[�h�͈͎w��t�@�C���F" << CT2A(strCharCodeListPath.c_str()) << " ������܂���" << std::endl;
		return false;
	}

	// �I�����[�h�� [immediate] �� [all] �łȂ���΃G���[
	if(strExitMode.c_str() != g_strIMMEDIATE && strExitMode.c_str() != g_strALL){
		std::cout << "�I�����[�h�� immediate | all �ł͂���܂���" << std::endl;
		return false;
	}

	// ���s���O�t�@�C�������łɂ���΃G���[
	if(PathFileExists(strRunLogFilePath.c_str())){
		std::cout << "���s���O�t�@�C���F" << CT2A(strRunLogFilePath.c_str()) << " �͂��łɂ���܂�" << std::endl;
		return false;
	}

	// �G���[���O�t�@�C�������łɂ���΃G���[
	if(PathFileExists(strErrLogFilePath.c_str())){
		std::cout << "�G���[���O�t�@�C���F" << CT2A(strErrLogFilePath.c_str()) << " �͂��łɂ���܂�" << std::endl;
		return false;
	}

	return true;
}