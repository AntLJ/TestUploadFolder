#include "stdafx.h"
#include "ShapeCodeChecker.h"

bool ShapeCodeChecker::CheckCharacterCode(bool isImmediateExit)
{
	CString strMsg;
	bool isCheckOK = true;

	// �t�@�C�����X�g�ɏ����ꂽ�Ώۃt�@�C���Ń��[�v
	for(const auto& itlTargetFile : m_vecTargetFileList){
		CDBFHandle dbfHandle;

		// DBF�I�[�v��
		if(!dbfHandle.open(CT2A(itlTargetFile), "rb")){
			strMsg.Format(_T("%s\tNG�F�t�@�C���I�[�v�����s"), itlTargetFile);
			CLog::GetInstance().PrintLog(true, false, true, true, strMsg);
			isCheckOK = false;
			continue;
		}

		// ������^�t�B�[���h�𒊏o
		std::map<int, std::string> mapFieldName;

		for(int i=0; i<dbfHandle.get_FieldCount(); ++i){
			char* fldName = nullptr;
			int iWidth = 0, iDecimals = 0;

			DBFFieldType iDbfFieldType;
			iDbfFieldType = dbfHandle.get_FieldInfo(i, &fldName, &iWidth, &iDecimals);
			if(iDbfFieldType == FTString){
				std::string strFieldName = fldName;
				mapFieldName[i] = strFieldName;
			}
		}

		// ������^�t�B�[���h���Ȃ���Ύ��̃t�@�C����ǂݍ���
		if(mapFieldName.empty()){
			strMsg.Format(_T("%s\t������t�B�[���h�Ȃ�"), itlTargetFile);
			CLog::GetInstance().PrintLog(true, false, true, false, strMsg);
			continue;
		}

		bool isParmittedFile = true;

		// ���R�[�h�Ń��[�v
		for(int i=0; i<dbfHandle.get_RecordCount(); ++i){

			// ������^�t�B�[���h�Ń��[�v
			for(const auto& itlFieldID : mapFieldName){

				// ������f�[�^���擾
				const char *strTemp;
				dbfHandle.read_Attribute(i, itlFieldID.first, &strTemp);
				bool isParmittedString = true;
				std::string chkStr = strTemp;

				// �����񂪋�Ȃ玟�̃t�B�[���h�ɃX�L�b�v
				if(chkStr.empty()){
					continue;
				}

				// �擾�����������IsParmitted�ɓ�����
				if(!IsParmitted(chkStr)){
					isParmittedString = false;
					isParmittedFile = false;
					isCheckOK = false;
				}

				// �s����������G���[���O�ɏo��
				if(!isParmittedString){
					CString strFileName = itlTargetFile.Mid(itlTargetFile.ReverseFind(_T('\\'))+1);

					CString strFieldName = itlFieldID.second.c_str();
					strMsg.Format(_T("%s\t%d\t%s\tNG"), strFileName, i, strFieldName);
					CLog::GetInstance().PrintLog(false, false, false, true, strMsg);
				}

				// �����I�����[�h�̏ꍇ���f���Ď��̃t�@�C���Ɉڂ�
				if(isImmediateExit && !isParmittedFile){
					break;
				}
			}

			if(isImmediateExit && !isParmittedFile){
				break;
			}
		}

		// �t�@�C�����ɕs�������R�[�h����������
		if(isParmittedFile){
			strMsg.Format(_T("%s\tOK"), itlTargetFile);
			CLog::GetInstance().PrintLog(true, false, true, false, strMsg);
		} else {
			strMsg.Format(_T("%s\tNG"), itlTargetFile);
			CLog::GetInstance().PrintLog(true, false, true, false, strMsg);
		}
	}

	return isCheckOK;
}
