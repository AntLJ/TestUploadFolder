#include "stdafx.h"
#include "main.h"
#include "ShapeCodeChecker.h"
#include "..\..\LIB\WinLib\VersionInfo.h"

int _tmain(int argc, _TCHAR* argv[])
{
	try{
		// ���P�[���֘A�̕s��h�~
		uh::setJapaneseCharacterTypeFacet();

		CString strMsg;

		// �R�}���h���C����������
		Arguments args;
		if(!args.parse(argc, argv)){
			std::cerr << "�R�}���h���C�������ɖ�肪����܂�" << std::endl;
			return 1;
		}

		// �R���\�[����ʂɃI�v�V�����\��
		std::cout << CT2A(args.GetOption()) << std::endl;

		// �t�@�C���p�X�`�F�b�N�ŃG���[������ΏI��
		if(!args.CheckOptionParameter()){
			return 1;
		}

		// ���s���O�I�[�v��
		if(!CLog::GetInstance().SetRunLog(args.strRunLogFilePath.c_str())){
			CLog::GetInstance().PrintLog(true, false, false, false, _T("�G���[�I��"));
			return 1;
		}

		// �G���[���O�I�[�v��
		if(!CLog::GetInstance().SetErrLog(args.strErrLogFilePath.c_str())){
			CLog::GetInstance().PrintLog(true, false, false, false, _T("�G���[�I��"));
			return 1;
		}

		// ���s���O�Ƀc�[���o�[�W�������o��
		CVersion version;
		CString strFileName = version.GetOriginalFilename();
		CString strFileVersion = version.GetFileVersion();
		CString strProductVersion = version.GetProductVersion();
		strMsg.Format(_T("%s FILEVERSION %s PRODUCTVERSION %s\n"), strFileName, strFileVersion, strProductVersion);
		CLog::GetInstance().PrintLog(false, false, true, false, strMsg);

		// ���s���O�ɃI�v�V�����o��
		CLog::GetInstance().PrintLog(false, false, true, false, args.GetOption());

		// �G���[���O�Ƀw�b�_�o��
		strMsg.Format(_T("<LAYER>\t<OBJECTID>\t<FIELDNAME>\t<COMMENT>"));
		CLog::GetInstance().PrintLog(false, false, false, true, strMsg);

		// Text�Ή��Ŋg������Ƃ��́ACodeChecker����TextCodeChecker��h��������
		// CheckCharacterCode()���I�[�o�[���C�h����Shape��Text�Ő؂�ւ��邱��
		CodeChecker *objCC;
		if(args.strFileType.c_str() == g_strSHAPE){
			objCC = new ShapeCodeChecker();
		}

		// �`�F�b�N�Ώۃt�@�C�����X�g�ǂݍ���
		if(!objCC->SetTargetFileList(args.strFileListPath.c_str())){
			CLog::GetInstance().PrintLog(true, false, false, false, _T("�G���[�I��"));
			delete objCC;
			return 1;
		}

		// �����R�[�h�͈͎w��t�@�C���ǂݍ���
		if(!objCC->SetCharacterCodeList(args.strCharCodeListPath.c_str())){
			CLog::GetInstance().PrintLog(true, false, false, false, _T("�G���[�I��"));
			delete objCC;
			return 1;
		}

		// �����R�[�h�`�F�b�N���s
		bool isImmediateExit = false;
		if(args.strExitMode.c_str() == g_strIMMEDIATE){
			isImmediateExit = true;
		}
		if(!objCC->CheckCharacterCode(isImmediateExit)){

			// ����I���i�����R�[�h�`�F�b�N�FNG�j
			CLog::GetInstance().PrintLog(true, false, true, false, _T("\n����I���i�����R�[�h�`�F�b�N�FNG�j"));
			delete objCC;
			return 3;
		}

		// ����I���i�����R�[�h�`�F�b�N�FOK�j
		CLog::GetInstance().PrintLog(true, false, true, false, _T("\n����I���i�����R�[�h�`�F�b�N�FOK�j"));
		delete objCC;
		return 0;

	} catch(const std::exception& e) {
		std::cerr << e.what() << std::endl;
		CLog::GetInstance().PrintLog(false, true, true, false, _T("\n�ُ�I���i��O�����j"));
		return 2;
	}
}
