#include "stdafx.h"
#include "CodeChecker.h"

bool CodeChecker::SetTargetFileList(const CString& strFileListPath)
{
	CString strMsg;
	std::ifstream ifsFileList;
	ifsFileList.open(strFileListPath);
	if(!ifsFileList.is_open()){
		strMsg.Format(_T("ファイルオープン失敗：%s"), strFileListPath);
		CLog::GetInstance().PrintLog(true, false, true, false, strMsg);
		return false;
	}

	// 対象ファイルリストを読み込んでvectorに保持
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
		strMsg.Format(_T("ファイルオープン失敗：%s"), strCharCodeListPath);
		CLog::GetInstance().PrintLog(true, false, true, false, strMsg);
		return false;
	}

	bool bReturn = true;
	int iRow = 0;
	
	// 文字コードリストを読み込んでvectorに保持
	std::string strCharCodeRange;
	while(getline(ifsCodeList, strCharCodeRange)){
		std::vector<std::string> vecStrCodeRange = GetTSVParameter(strCharCodeRange);
		++iRow;

		// タブ2列でなければエラー
		if(vecStrCodeRange.size() != 2){
			strMsg.Format(_T("%d行目：文字コード範囲はタブ2列で指定してください"), iRow);
			CLog::GetInstance().PrintLog(true, false, true, false, strMsg);
			bReturn = false;
			continue;
		}

		std::string strMin = vecStrCodeRange[m_intCharCodeMin];
		std::string strMax = vecStrCodeRange[m_intCharCodeMax];

		// コードの表現は0x********であること
		if(strMin.substr(0, 2) != "0x" || strMax.substr(0, 2) != "0x"){
			strMsg.Format(_T("%d行目：文字コードの指定は0x****の形にしてください"), iRow);
			CLog::GetInstance().PrintLog(true, false, true, false, strMsg);
			bReturn = false;
			continue;
		}

		// コードの0x以降の部分が16進数であること
		CString chkHexMin = strMin.substr(2).c_str();
		CString chkHexMax = strMax.substr(2).c_str();
		CString resHexMin = chkHexMin.SpanIncluding(_T("0123456789ABCDEF"));
		CString resHexMax = chkHexMax.SpanIncluding(_T("0123456789ABCDEF"));
		if(chkHexMin != resHexMin || chkHexMax != resHexMax){
			strMsg.Format(_T("%d行目：文字コードの値は\"0123456789ABCDEF\"で指定してください"), iRow);
			CLog::GetInstance().PrintLog(true, false, true, false, strMsg);
			bReturn = false;
			continue;
		}

		// バイト跨ぎは不正とする
		if(strMin.length() != strMax.length()){
			strMsg.Format(_T("%d行目：文字コード範囲指定は桁数を揃えてください"), iRow);
			CLog::GetInstance().PrintLog(true, false, true, false, strMsg);
			bReturn = false;
			continue;
		}

		// 5バイト以上の文字コードは不正とする
		if(strMin.length() > 10 || strMax.length() > 10){
			strMsg.Format(_T("%d行目：文字コードの指定は4バイト以下にしてください"), iRow);
			CLog::GetInstance().PrintLog(true, false, true, false, strMsg);
			bReturn = false;
			continue;
		}

		CharacterCode objCharCode;
		objCharCode.CodeRangeMin = std::stoul(strMin, nullptr, 16);
		objCharCode.CodeRangeMax = std::stoul(strMax, nullptr, 16);

		// 範囲指定の大小が逆だったら不正
		if(objCharCode.CodeRangeMax < objCharCode.CodeRangeMin){
			strMsg.Format(_T("%d行目：範囲指定の値の大小が逆です"), iRow);
			CLog::GetInstance().PrintLog(true, false, true, false, strMsg);
			bReturn = false;
			continue;
		}

		m_vecCharCodeList.push_back(objCharCode);
	}

	ifsCodeList.close();

	if(!bReturn){
		strMsg = "文字コード範囲指定ファイルが不正です";
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

			// 文字コードチェックがOKだった場合
			if(chk){
				i+=term;
				break;
			}

			// 1バイトでチェックNGなら2バイト、2バイトでNGなら3バイト……とtermの長さを増やす
			if(i+term <= chkStr.size()){
				++term;
			} else {
				break;
			}

			// ただしtermが4バイトを超えたら該当文字なしとしてbreak
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
