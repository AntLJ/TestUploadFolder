/*
 * Copyright (C) INCREMENT P CORP. All Rights Reserved.
 *
 * THIS SOFTWARE IS PROVIDED BY INCREMENT P CORP., WITHOUT WARRANTY OF
 * ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT.
 *
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDER BE LIABLE FOR ANY
 * CLAIM, DAMAGES SUFFERED BY LICENSEE AS A RESULT OF USING, MODIFYING
 * OR DISTRIBUTING THIS SOFTWARE OR ITS DERIVATIVES.
*/

#include "stdafx.h"
#include "AddrTransResultFmtConverter.h"
#include <WinLib/VersionInfo.h>
#include <regex>

// ������
bool AddrTransResultFmtConverter::init( int argc, TCHAR** argv )
{
	// �R�}���h���C����������
	if( !m_argument.setParameter(argc, argv, m_runLogger, m_errLogger))
	{
		wcerr << (message::getMsg( eFailAnalyzeArgument));
		return false;
	}
	return true;
}

// ���s
bool AddrTransResultFmtConverter::run() 
{
	// ���s���O�ƃG���[���O�̊J�n����
	writeInformation();
	if (m_argument.m_convMode == CONV_MODE_CITY)
	{
		// ���s�撬��
		convertCity();
	}
	else if (m_argument.m_convMode == CONV_MODE_ADDR)
	{
		// �����厚�E������
		convertAddr();
	}
	else if (m_argument.m_convMode == CONV_MODE_ALL)
	{
		convertCity();
		convertAddr();
	}
	// �o�͏��̓��v
	outputFileStat();
	return true;
}

// city.txt�t�@�C���̏���
void AddrTransResultFmtConverter::convertCity()
{
	wcout << (message::getMsg( eCityFileStartConvert ));
	m_runLogger.log(message::getMsg( eCityFileStartConvert ));
	wstring fileFolder = m_argument.m_transResult + L"\\" + CITY_TXT;
	if (!m_sourceFile.initialize(fileFolder, UTF8_READ))
		return;
	wchar_t wstrLine[10240] = {0};
	// �w�b�_�̓ǂݍ��ނƕ���
	fgetws(wstrLine, 10240, m_sourceFile.m_pFile);
	split(wstrLine, L"\t");
	for (const auto & iterMap : m_mapFileHeadStr2Int)
	{
		wstring itrStr = iterMap.first;
		if (itrStr.length() <= 2)
		{
			continue;
		}
		wstring strNameYomi2 = itrStr.substr(itrStr.length()-3, 3);
		if (strNameYomi2 == L"����2" || strNameYomi2 == L"�ǂ�2")
		{
			// �u����2�v�܂��́u�ǂ�2�v���܂ޗ�̏W��
			m_vecNameYomi2.push_back(itrStr);
		}
	}
	wstring outputLine;
	int recordNum = 0;
	// �u����2�v�܂��́u�ǂ�2�v���܂ޗ�̃`�F�b�N
	for (const auto & iterVec : m_vecNameYomi2)
	{
		CombinePairs cPair;
		wstring strNameYomi1 = iterVec.substr(0,iterVec.length()-1) + L"1";
		wstring strNameYomi = iterVec.substr(0,iterVec.length()-1);
		auto result1 = m_mapFileHeadStr2Int.find(strNameYomi1);
		auto result = m_mapFileHeadStr2Int.find(strNameYomi);
		if (result1!= m_mapFileHeadStr2Int.end() && result!= m_mapFileHeadStr2Int.end())
		{
			//���A�u***���́v�u***����1�v�u***����2�v/�܂��́u***�ǂ݁v�u***�ǂ�1�v�u***�ǂ�2�v�̃t�B�[���h�����݂���ꍇ�́A������̏����͍s�킸�A�G���[���O�ɏo�͂���B
			//�܂��A���̏ꍇ�A�Y���t�@�C���ɂ����āA�{���������͍s��Ȃ��B
			outputLine = mergeErrLogLine(CITY_TXT, -1, L"�u" + strNameYomi.substr(strNameYomi.length()-2,2) + L"�v�u" + strNameYomi1.substr(strNameYomi1.length()-3,3) + L"�v�u" + iterVec.substr(iterVec.length()-3,3) + (message::getMsg( eFileColumnErr)), strNameYomi, strNameYomi1, iterVec); 
			m_errLogger.log(outputLine);
			wcerr<<outputLine;
			wstring fileFolder = m_argument.m_out + L"\\" + CITY_TXT;
			if (!m_resultFile.initialize(fileFolder, UTF8_CREATE))
				return;
			writeResultFile(m_mapFileLine);
			m_mapFileHeadStr2Int.clear();
			m_mapFileLine.clear();
			while(fgetws(wstrLine, 10240, m_sourceFile.m_pFile))
			{
				split(wstrLine, L"\t");
				writeResultFile(m_mapFileLine);
			}
			m_resultFile.finalize();
			m_sourceFile.finalize();
			// ���s���O�̏o��
			outputLine = mergeRunLogLine(CITY_TXT, CITY_TXT,recordNum, recordNum, false);
			m_vecRunInfo.push_back(outputLine);
			return;
		}
		if(result1 != m_mapFileHeadStr2Int.end())
		{
			cPair.firstColumn = m_mapFileHeadStr2Int[strNameYomi1];
		}
		else if(result != m_mapFileHeadStr2Int.end())
		{
			cPair.firstColumn = m_mapFileHeadStr2Int[strNameYomi];
		}
		cPair.secondColumn = m_mapFileHeadStr2Int[iterVec];
		m_mapCombineColumnIdxs[cPair.firstColumn] = cPair;
	}
	m_vecNameYomi2.clear();
	// �w�[�_�̐����Əo��
	int columnNum = m_mapFileLine.size();
	for (const auto & itrMap2 : m_mapCombineColumnIdxs)
	{
		// �u***����2�v��u***�ǂ�2�v�̖����́u2�v���폜���āA�ǉ�����w�b�_���𐶐����Amap�ɒǉ�����
		wstring wAddColumn = m_mapFileLine[itrMap2.second.secondColumn];
		m_mapFileLine[columnNum++] = wAddColumn.substr(0, wAddColumn.length()-1);
		m_mapFileLine.erase(itrMap2.second.firstColumn);
		m_mapFileLine.erase(itrMap2.second.secondColumn);
	}
	fileFolder = m_argument.m_out + L"\\" + CITY_TXT;
	if (!m_resultFile.initialize(fileFolder, UTF8_CREATE))
		return;
	// ���X�A�����̃t�@�C�������݂��Ă���ꍇ�A�㏑���ۑ��i���ǉ��������݁j����B(w+)
	writeResultFile(m_mapFileLine);
	m_mapFileHeadStr2Int.clear();
	m_mapFileLine.clear();
	// ���̍s�̐����Əo��
	while (fgetws(wstrLine, 10240, m_sourceFile.m_pFile))
	{
		++recordNum;
		split(wstrLine, L"\t");
		int originalSize = m_mapFileLine.size();
		for (const auto & itrMap2 : m_mapCombineColumnIdxs)
		{
			// ��̍���
			wstring wFirstColumn = m_mapFileLine[itrMap2.second.firstColumn];
			wstring wSecondColumn = m_mapFileLine[itrMap2.second.secondColumn];
			if (!wFirstColumn.empty() && !wSecondColumn.empty())
			{
				wFirstColumn = wFirstColumn + L" " + wSecondColumn;
			}
			else
			{
				wFirstColumn += wSecondColumn;
			}
			m_mapFileLine[originalSize++] = wFirstColumn;
			m_mapFileLine.erase(itrMap2.second.firstColumn);
			m_mapFileLine.erase(itrMap2.second.secondColumn);
		}
		writeResultFile(m_mapFileLine);
		m_mapFileLine.clear();
	}
	m_resultFile.finalize();
	m_sourceFile.finalize();
	// ���s���O�̏o��
	outputLine = mergeRunLogLine(CITY_TXT, CITY_TXT,recordNum+1, recordNum+1, true);
	m_vecRunInfo.push_back(outputLine);
	m_mapCombineColumnIdxs.clear();
	wcout << (message::getMsg( eCityFileEndConvert ));
	m_runLogger.log(message::getMsg( eCityFileEndConvert ));
}

// addr[**].txt�t�@�C���̏���
void AddrTransResultFmtConverter::convertAddr()
{
	wcout << (message::getMsg( eAddrFileStartConvert ));
	m_runLogger.log(message::getMsg( eAddrFileStartConvert ));
	// addr[xx].txt�̃p�X�Ńt�@�C�����X�g���擾
	wstring filePath = m_argument.m_transResult + L"\\*.*";
	getFileNames(filePath.c_str());
	for ( auto & itrVec : m_vecAddressFileName )
	{
		wcout << L"\t" << (itrVec) << (message::getMsg( eAddrFileStartConvert ));
		m_runLogger.log(L"\t" + (itrVec) + message::getMsg( eAddrFileStartConvert ));
		filePath = m_argument.m_transResult + L"\\" + itrVec.c_str();
		if (!createAzaFile(filePath))
			continue;
		createOazaFile(filePath);
		wcout << L"\t" << (itrVec) << (message::getMsg( eAddrFileEndConvert ));
		m_runLogger.log(L"\t" + (itrVec) + message::getMsg( eAddrFileEndConvert ));
	}
	wcout << (message::getMsg( eAddrFileEndConvert ));
	m_runLogger.log(message::getMsg( eAddrFileEndConvert ));
}

// aza[**].txt�t�@�C���̍쐬
bool AddrTransResultFmtConverter::createAzaFile(const wstring& pFilePath)
{
	int recordNum = 0;
	if (!m_sourceFile.initialize(pFilePath, UTF8_READ))
		return false;
	wchar_t wstrLine[10240] = {0};
	fgetws(wstrLine, 10240, m_sourceFile.m_pFile);
	split(wstrLine, L"\t");
	vector<int> deleteColumIdxs;
	int columIndex = 0;
	std::wregex regexStr(L"^�厚�E�ʏ�.*(����|�ǂ�).*");
	for (const auto& itrMap : m_mapFileLine)
	{
		if (std::regex_match (itrMap.second, regexStr))
		{
			deleteColumIdxs.push_back(columIndex);
		}
		++columIndex;
	}
	// �w�[�_�̐����Əo��
	for (const auto & itrVec : deleteColumIdxs)
	{
		m_mapFileLine.erase(itrVec);
	}
	wstring filePath = m_argument.m_out + L"\\aza" + (pFilePath.substr(pFilePath.length()-6, 2)).c_str() + L".txt";
	if (!m_resultFile.initialize(filePath, UTF8_CREATE))
		return false;
	// ���X�A�����̃t�@�C�������݂��Ă���ꍇ�A�㏑���ۑ��i���ǉ��������݁j����B(w+)
	writeResultFile(m_mapFileLine);
	m_mapFileLine.clear();
	// ���̍s�̐����Əo��
	while (fgetws(wstrLine, 10240, m_sourceFile.m_pFile))
	{
		++recordNum;
		split(wstrLine, L"\t");
		for (const auto & itrVec : deleteColumIdxs)
		{
			m_mapFileLine.erase(itrVec);
		}
		writeResultFile(m_mapFileLine);
		m_mapFileLine.clear();
	}
	m_resultFile.finalize();
	m_sourceFile.finalize();
	
	// ���s���O�̏o��
	wstring outputLine = mergeRunLogLine(pFilePath.substr(pFilePath.length()-10, 10), L"aza" + pFilePath.substr(pFilePath.length()-6, 6), recordNum+1, recordNum+1, true);
	m_vecRunInfo.push_back(outputLine);
	return true;
}

// oaza[**].txt�t�@�C���̍쐬
bool AddrTransResultFmtConverter::createOazaFile(const wstring& pFilePath)
{
	if (!m_sourceFile.initialize(pFilePath, UTF8_READ))
		return false;
	wchar_t wstrLine[10240] = {0};
	fgetws(wstrLine, 10240, m_sourceFile.m_pFile);
	split(wstrLine, L"\t");
	vector<int>			deleteColumIdxs;
	vector<int>			keyColumIdxs;
	vector<int>			compareColumIdxs;
	int columIndex = 0;
	bool bNormal = true;
	std::wregex regexStr1(L"^���E����.*");
	std::wregex regexStr2(L"^�厚�E�ʏ�.*(����|�ǂ�).*");
	for (const auto& itrMap : m_mapFileLine)
	{
		// �w�b�_�i1�s�ځj�ɁA�u���E���ځv�Ƃ��������񂪐擪�ɕt���t�B�[���h�����݂���ꍇ�A���̗���폜�B
		wstring compareColum = itrMap.second;
		if (std::regex_match (itrMap.second, regexStr1))
		{
			deleteColumIdxs.push_back(columIndex);
		}
		// �t�@�C�����́u�s���{���R�[�h�v�u�s�撬���R�[�h�v�u�厚�E�ʏ̃R�[�h�v�̃t�B�[���h�̒l���A�S�ē����s�̑g�ݍ��킹���ƂɁA
		if (compareColum == L"�s���{���R�[�h" || compareColum == L"�s�撬���R�[�h" || compareColum == L"�厚�E�ʏ̃R�[�h")
		{
			keyColumIdxs.push_back(columIndex);
		}
		// �ȉ���2�̏����𗼕��������t�B�[���h�̗�̒l���A�S�ē����l���ǂ����`�F�b�N����B
		// �E�u�厚�E�ʏ́v�Ƃ��������񂪐擪�ɕt��
		// �E�u���́v�܂��́u�ǂ݁v�Ƃ��������񂪊܂܂��
		if (std::regex_match (itrMap.second, regexStr2))
		{
			compareColumIdxs.push_back(columIndex);
		}
		++columIndex;
	}
	// �w�[�_�𐮗����āA�o�͂���
	map<int, wstring> mapHeadLine = m_mapFileLine;
	for (const auto & itrVec : deleteColumIdxs)
	{
		mapHeadLine.erase(itrVec);
	}
	m_mapFileLine.clear();
	wstring filePath = m_argument.m_out + L"\\oaza" + (pFilePath.substr(pFilePath.length()-6, 2)).c_str() + L".txt";
	if (!m_resultFile.initialize(filePath, UTF8_CREATE))
		return false;
	// ���X�A�����̃t�@�C�������݂��Ă���ꍇ�A�㏑���ۑ��i���ǉ��������݁j����B(w+)
	writeResultFile(mapHeadLine);
	// ���̍s�̏o��
	int recordNum = 0;
	while (fgetws(wstrLine, 10240, m_sourceFile.m_pFile))
	{
		++recordNum;
		split(wstrLine, L"\t");
		for (const auto & itrVec : deleteColumIdxs)
		{
			m_mapFileLine.erase(itrVec);
		}
		wstring compareKey;
		for ( const auto& itrVecKey : keyColumIdxs)
		{
			compareKey += m_mapFileLine[itrVecKey];
		}
		// ��s�����ʗe��ւ̊i�[
		if (m_mapOazaFile.find(compareKey) == m_mapOazaFile.end())
		{
			m_mapOazaFile[compareKey] = m_mapFileLine;
		}
		else
		{
			GroupLines gls;
			gls.lineNum = recordNum;
			gls.mapGroupLines = m_mapFileLine;
			m_mapErrCheck[compareKey].push_back(gls);
		}
		m_mapFileLine.clear();
	}
	for (auto& itrMap : m_mapOazaFile)
	{
		writeResultFile(itrMap.second);
	}
	for (const auto& itrMap : m_mapErrCheck)
	{
		for (auto itrVec : itrMap.second)
		{
			for (const auto& itrVecCompare : compareColumIdxs)
			{
				if (itrVec.mapGroupLines[itrVecCompare] == m_mapOazaFile[itrMap.first][itrVecCompare])
					continue;
				// ��s�Ƃ̔�r���ʂ��G���[���O�ւ̏o��
				CStringW strLineNum;
				strLineNum.Format(L"%d", (itrVec.lineNum+1));
				wstring outputLine = mergeErrLogLine( pFilePath.substr(pFilePath.length()-10, 10), (itrVec.lineNum+1), (message::getMsg( eFindDifference)), mapHeadLine[itrVecCompare].c_str(), itrVec.mapGroupLines[itrVecCompare]); 
				m_errLogger.log(outputLine);
				bNormal = false;
			}
		}
	}
	m_resultFile.finalize();
	m_sourceFile.finalize();
	// ���s���O�̏o��
	wstring outputLine = mergeRunLogLine(pFilePath.substr(pFilePath.length()-10, 10), L"oaza" + pFilePath.substr(pFilePath.length()-6, 6), recordNum+1, (m_mapOazaFile.size()+1), bNormal);
	m_vecRunInfo.push_back(outputLine);

	mapHeadLine.clear();
	m_mapErrCheck.clear();
	m_mapOazaFile.clear();
	return true;
}

// �p�X�ŁA�t�@�C�����X�g���擾
void AddrTransResultFmtConverter::getFileNames(const wchar_t* path)
{
	WIN32_FIND_DATA findFileData;
	HANDLE hFind = FindFirstFile(path, &findFileData);
	if (hFind == INVALID_HANDLE_VALUE) 
	{
		wcerr<< (message::getMsg( eFailGetFile));
		return;
	}
	else 
	{
		std::wregex regexStr(L"^addr[0-9][0-9].txt");
		while (FindNextFile(hFind, &findFileData))
		{
			wstring fileName = findFileData.cFileName;
			if (std::regex_match(fileName, regexStr))
			{
				// �t�@�C������e��֓��͂���
				m_vecAddressFileName.push_back(fileName);
			}
		}
	}
	FindClose(hFind);
}

// ���ʃt�@�C���̏�������
void AddrTransResultFmtConverter::writeResultFile(map<int, wstring>& ret)
{
	size_t mapIdx = 0;
	for ( const auto& itrMap : ret )
	{
		fwprintf(m_resultFile.m_pFile, (itrMap.second).c_str());
		if (mapIdx < ret.size() - 1)
		{
			fwprintf(m_resultFile.m_pFile, L"\t");
		}
		else
		{
			fwprintf(m_resultFile.m_pFile, L"\n");
			break;
		}
		++mapIdx;
	}
}

// �t�@�C���̍s�̕���
void AddrTransResultFmtConverter::split(const std::wstring& str, const std::wstring& delim)
{
	wstring str2 = str.substr(0, str.length()-1);
	size_t last = 0;
	size_t index=str2.find_first_of(delim,last);
	int keyIndex = 0;
	while (index!=std::wstring::npos)
	{
		wstring insertStr = str2.substr(last,index-last);
		m_mapFileHeadStr2Int[insertStr] = keyIndex;
		m_mapFileLine[keyIndex] = insertStr;
		++keyIndex;
		last=index+1;
		index=str2.find_first_of(delim,last);
	}
	wstring lastStr = str2.substr(last,index-last);
	m_mapFileHeadStr2Int[lastStr] = keyIndex;
	m_mapFileLine[keyIndex] = lastStr;
}

// ���s���O�̐���
wstring AddrTransResultFmtConverter::mergeRunLogLine(const wstring& inFileName, const wstring& outFileName, int inRecordNum, int outRecordNum, bool bNormal)
{
	CStringW inputNum, outputNum;
	inputNum.Format(L"%d", inRecordNum);
	outputNum.Format(L"%d", outRecordNum);
	wstring wStatus = L"����";
	if (!bNormal)
		wStatus = L"�G���[����";
	return (inFileName + L"\t" + outFileName + L"\t" + wStatus + L"\t" + inputNum.GetString() + L"\t" + outputNum.GetString() + L"\n");
}

// �G���[���O�̐���
wstring AddrTransResultFmtConverter::mergeErrLogLine(const wstring& sFileName, int lineNum, const wstring& errMsg, const wstring& info1, const wstring& info2, const wstring& info3)
{
	CStringW strLineNum;
	strLineNum.Format(L"%d", lineNum);
	wstring outputLine;
	if (lineNum == -1)
		return (sFileName + L"\t-" + L"\t" + errMsg + L"\t" + info1 + L"\t" + info2 + L"\t" + info3 + L"\n");
	else
		return (sFileName + L"\t" + strLineNum.GetString() + L"\t" + errMsg + L"\t" + info1 + L"\t" + info2 + L"\n");
}

// �o�͏��̓��v
void AddrTransResultFmtConverter::outputFileStat()
{
	// ��������
	m_runLogger.log(timeMemo(L"��������"));
	m_runLogger.log(L"����I��\n\n");
	for (const auto& itrVec : m_vecRunInfo)
	{
		m_runLogger.log(itrVec);
	}
}

// ���s���O�ƃG���[���O�̃��b�Z�[�W
void AddrTransResultFmtConverter::writeInformation()
{
	// errlog�̃w�b�_
	m_errLogger.log(L"�t�@�C����\t�s��\t�G���[���b�Z�[�W\t���1\t���2\t���3\n");
	// runlog�̃w�b�_
	m_vecRunInfo.push_back(L"���t�@�C��\t�o�̓t�@�C��\t��������\t������\t�o�͌���\n");
	CVersion version;
	CStringW productionVersion;
	productionVersion.Format(L"%s PRODUCTVERSION %s\n", version.GetInternalName(), version.GetProductVersion());
	m_runLogger.log(productionVersion.GetString());
	m_runLogger.log((L"--conv_mode    " + m_argument.m_convMode + L"\n").GetString());
	m_runLogger.log((L"--trans_result " + m_argument.m_transResult + L"\n").GetString());
	m_runLogger.log((L"--out          " + m_argument.m_out + L"\n").GetString());
	m_runLogger.log((L"--run_log      " + m_argument.m_runLog + L"\n").GetString());
	m_runLogger.log((L"--err_log      " + m_argument.m_errLog + L"\n\n").GetString());
	// �J�n����
	m_runLogger.log(timeMemo(L"�J�n����"));
}

// ���ԋL�^
wstring AddrTransResultFmtConverter::timeMemo(const wstring& phaseName)
{
	CStringW runTime;
	SYSTEMTIME st;
	::GetLocalTime( &st );
	runTime.Format( L" : %d/%02d/%02d  %02d:%02d:%02d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond );
	return (phaseName + runTime.GetString());
}