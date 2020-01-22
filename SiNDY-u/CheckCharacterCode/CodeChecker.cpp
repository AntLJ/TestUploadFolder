#include "stdafx.h"
#include "CodeChecker.h"

bool CodeChecker::SetTargetFileList(const CString& strFileListPath)
{
	CString strMsg;
	std::ifstream ifsFileList;
	ifsFileList.open(strFileListPath);
	if(!ifsFileList.is_open()){
		strMsg.Format(_T("�t�@�C���I�[�v�����s�F%s"), strFileListPath);
		CLog::GetInstance().PrintLog(true, false, true, false, strMsg);
		return false;
	}

	// �Ώۃt�@�C�����X�g��ǂݍ����vector�ɕێ�
	std::string strFilePath;
	while(getline(ifsFileList, strFilePath)){
		m_vecTargetFileList.push_back(strFilePath.c_str());
	}

	ifsFileList.close();

	return true;
}


bool CodeChecker::SetCharacterCodeList(const CString& strCharCodeListPath)
{
	CString strMsg;
	std::ifstream ifsCodeList;
	ifsCodeList.open(strCharCodeListPath);
	if(!ifsCodeList.is_open()){
		strMsg.Format(_T("�t�@�C���I�[�v�����s�F%s"), strCharCodeListPath);
		CLog::GetInstance().PrintLog(true, false, true, false, strMsg);
		return false;
	}

	bool bReturn = true;
	int iRow = 0;
	
	// �����R�[�h���X�g��ǂݍ����vector�ɕێ�
	std::string strCharCodeRange;
	while(getline(ifsCodeList, strCharCodeRange)){
		std::vector<std::string> vecStrCodeRange = GetTSVParameter(strCharCodeRange);
		++iRow;

		// �^�u2��łȂ���΃G���[
		if(vecStrCodeRange.size() != 2){
			strMsg.Format(_T("%d�s�ځF�����R�[�h�͈͂̓^�u2��Ŏw�肵�Ă�������"), iRow);
			CLog::GetInstance().PrintLog(true, false, true, false, strMsg);
			bReturn = false;
			continue;
		}

		std::string strMin = vecStrCodeRange[m_intCharCodeMin];
		std::string strMax = vecStrCodeRange[m_intCharCodeMax];

		// �R�[�h�̕\����0x********�ł��邱��
		if(strMin.substr(0, 2) != "0x" || strMax.substr(0, 2) != "0x"){
			strMsg.Format(_T("%d�s�ځF�����R�[�h�̎w���0x****�̌`�ɂ��Ă�������"), iRow);
			CLog::GetInstance().PrintLog(true, false, true, false, strMsg);
			bReturn = false;
			continue;
		}

		// �R�[�h��0x�ȍ~�̕�����16�i���ł��邱��
		CString chkHexMin = strMin.substr(2).c_str();
		CString chkHexMax = strMax.substr(2).c_str();
		CString resHexMin = chkHexMin.SpanIncluding(_T("0123456789ABCDEF"));
		CString resHexMax = chkHexMax.SpanIncluding(_T("0123456789ABCDEF"));
		if(chkHexMin != resHexMin || chkHexMax != resHexMax){
			strMsg.Format(_T("%d�s�ځF�����R�[�h�̒l��\"0123456789ABCDEF\"�Ŏw�肵�Ă�������"), iRow);
			CLog::GetInstance().PrintLog(true, false, true, false, strMsg);
			bReturn = false;
			continue;
		}

		// �o�C�g�ׂ��͕s���Ƃ���
		if(strMin.length() != strMax.length()){
			strMsg.Format(_T("%d�s�ځF�����R�[�h�͈͎w��͌����𑵂��Ă�������"), iRow);
			CLog::GetInstance().PrintLog(true, false, true, false, strMsg);
			bReturn = false;
			continue;
		}

		// 5�o�C�g�ȏ�̕����R�[�h�͕s���Ƃ���
		if(strMin.length() > 10 || strMax.length() > 10){
			strMsg.Format(_T("%d�s�ځF�����R�[�h�̎w���4�o�C�g�ȉ��ɂ��Ă�������"), iRow);
			CLog::GetInstance().PrintLog(true, false, true, false, strMsg);
			bReturn = false;
			continue;
		}

		CharacterCode objCharCode;
		objCharCode.CodeRangeMin = std::stoul(strMin, nullptr, 16);
		objCharCode.CodeRangeMax = std::stoul(strMax, nullptr, 16);

		// �͈͎w��̑召���t��������s��
		if(objCharCode.CodeRangeMax < objCharCode.CodeRangeMin){
			strMsg.Format(_T("%d�s�ځF�͈͎w��̒l�̑召���t�ł�"), iRow);
			CLog::GetInstance().PrintLog(true, false, true, false, strMsg);
			bReturn = false;
			continue;
		}

		m_vecCharCodeList.push_back(objCharCode);
	}

	ifsCodeList.close();

	if(!bReturn){
		strMsg = "�����R�[�h�͈͎w��t�@�C�����s���ł�";
		CLog::GetInstance().PrintLog(true, false, true, false, strMsg);
	}

	return bReturn;
}


std::vector<std::string> CodeChecker::GetTSVParameter(std::string strTSVRecord)
{
	std::string strTemp;
	std::istringstream issInput(strTSVRecord);
	std::vector<std::string> vecItems;

	while(getline(issInput, strTemp, '\t')){
		vecItems.push_back(strTemp);
	}

	return vecItems;
}


bool CodeChecker::IsParmitted(const std::string& chkStr)
{
	for(int i=0; i<chkStr.size();){
		int term = 1;
		bool chk = false;
		while(true){
			std::string temp = chkStr.substr(i, term);
			for (const auto& itlCharacterCode : m_vecCharCodeList){
				unsigned long lTgt = std::stoul(ToHex(temp), nullptr, 16);
				if (itlCharacterCode.CodeRangeMin <= lTgt && lTgt <= itlCharacterCode.CodeRangeMax){
					chk = true;
					break;
				}
			}

			// �����R�[�h�`�F�b�N��OK�������ꍇ
			if(chk){
				i+=term;
				break;
			}

			// 1�o�C�g�Ń`�F�b�NNG�Ȃ�2�o�C�g�A2�o�C�g��NG�Ȃ�3�o�C�g�c�c��term�̒����𑝂₷
			if(i+term <= chkStr.size()){
				++term;
			} else {
				break;
			}

			// ������term��4�o�C�g�𒴂�����Y�������Ȃ��Ƃ���break
			if(term > 4){
				break;
			}
		}
		if(!chk)
			return false;
	}

	return true;
}


std::string CodeChecker::ToHex(const std::string& org_str)
{
	std::string hex_str;
	char chOutchar[] = {"0123456789ABCDEF"};
	for (int i=0; i<org_str.size(); ++i){
		hex_str += chOutchar[(unsigned char)org_str[i]>>4];
		hex_str += chOutchar[(unsigned char)org_str[i]&0x0F];
	}

	return hex_str;
}
