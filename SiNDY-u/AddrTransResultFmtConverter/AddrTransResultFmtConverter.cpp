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

// 初期化
bool AddrTransResultFmtConverter::init( int argc, TCHAR** argv )
{
	// コマンドライン引数処理
	if( !m_argument.setParameter(argc, argv, m_runLogger, m_errLogger))
	{
		wcerr << (message::getMsg( eFailAnalyzeArgument));
		return false;
	}
	return true;
}

// 実行
bool AddrTransResultFmtConverter::run() 
{
	// 実行ログとエラーログの開始部分
	writeInformation();
	if (m_argument.m_convMode == CONV_MODE_CITY)
	{
		// ▼市区町村
		convertCity();
	}
	else if (m_argument.m_convMode == CONV_MODE_ADDR)
	{
		// ▼町大字・字丁目
		convertAddr();
	}
	else if (m_argument.m_convMode == CONV_MODE_ALL)
	{
		convertCity();
		convertAddr();
	}
	// 出力情報の統計
	outputFileStat();
	return true;
}

// city.txtファイルの処理
void AddrTransResultFmtConverter::convertCity()
{
	wcout << (message::getMsg( eCityFileStartConvert ));
	m_runLogger.log(message::getMsg( eCityFileStartConvert ));
	wstring fileFolder = m_argument.m_transResult + L"\\" + CITY_TXT;
	if (!m_sourceFile.initialize(fileFolder, UTF8_READ))
		return;
	wchar_t wstrLine[10240] = {0};
	// ヘッダの読み込むと分割
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
		if (strNameYomi2 == L"名称2" || strNameYomi2 == L"読み2")
		{
			// 「名称2」または「読み2」を含む列の集合
			m_vecNameYomi2.push_back(itrStr);
		}
	}
	wstring outputLine;
	int recordNum = 0;
	// 「名称2」または「読み2」を含む列のチェック
	for (const auto & iterVec : m_vecNameYomi2)
	{
		CombinePairs cPair;
		wstring strNameYomi1 = iterVec.substr(0,iterVec.length()-1) + L"1";
		wstring strNameYomi = iterVec.substr(0,iterVec.length()-1);
		auto result1 = m_mapFileHeadStr2Int.find(strNameYomi1);
		auto result = m_mapFileHeadStr2Int.find(strNameYomi);
		if (result1!= m_mapFileHeadStr2Int.end() && result!= m_mapFileHeadStr2Int.end())
		{
			//尚、「***名称」「***名称1」「***名称2」/または「***読み」「***読み1」「***読み2」のフィールドが存在する場合は、こちらの処理は行わず、エラーログに出力する。
			//また、その場合、該当ファイルにおいて、本結合処理は行わない。
			outputLine = mergeErrLogLine(CITY_TXT, -1, L"「" + strNameYomi.substr(strNameYomi.length()-2,2) + L"」「" + strNameYomi1.substr(strNameYomi1.length()-3,3) + L"」「" + iterVec.substr(iterVec.length()-3,3) + (message::getMsg( eFileColumnErr)), strNameYomi, strNameYomi1, iterVec); 
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
			// 実行ログの出力
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
	// ヘーダの整理と出力
	int columnNum = m_mapFileLine.size();
	for (const auto & itrMap2 : m_mapCombineColumnIdxs)
	{
		// 「***名称2」や「***読み2」の末尾の「2」を削除して、追加するヘッダ名を生成し、mapに追加する
		wstring wAddColumn = m_mapFileLine[itrMap2.second.secondColumn];
		m_mapFileLine[columnNum++] = wAddColumn.substr(0, wAddColumn.length()-1);
		m_mapFileLine.erase(itrMap2.second.firstColumn);
		m_mapFileLine.erase(itrMap2.second.secondColumn);
	}
	fileFolder = m_argument.m_out + L"\\" + CITY_TXT;
	if (!m_resultFile.initialize(fileFolder, UTF8_CREATE))
		return;
	// 元々、同名のファイルが存在している場合、上書き保存（≠追加書き込み）する。(w+)
	writeResultFile(m_mapFileLine);
	m_mapFileHeadStr2Int.clear();
	m_mapFileLine.clear();
	// 他の行の整理と出力
	while (fgetws(wstrLine, 10240, m_sourceFile.m_pFile))
	{
		++recordNum;
		split(wstrLine, L"\t");
		int originalSize = m_mapFileLine.size();
		for (const auto & itrMap2 : m_mapCombineColumnIdxs)
		{
			// 列の合併
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
	// 実行ログの出力
	outputLine = mergeRunLogLine(CITY_TXT, CITY_TXT,recordNum+1, recordNum+1, true);
	m_vecRunInfo.push_back(outputLine);
	m_mapCombineColumnIdxs.clear();
	wcout << (message::getMsg( eCityFileEndConvert ));
	m_runLogger.log(message::getMsg( eCityFileEndConvert ));
}

// addr[**].txtファイルの処理
void AddrTransResultFmtConverter::convertAddr()
{
	wcout << (message::getMsg( eAddrFileStartConvert ));
	m_runLogger.log(message::getMsg( eAddrFileStartConvert ));
	// addr[xx].txtのパスでファイルリストを取得
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

// aza[**].txtファイルの作成
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
	std::wregex regexStr(L"^大字・通称.*(名称|読み).*");
	for (const auto& itrMap : m_mapFileLine)
	{
		if (std::regex_match (itrMap.second, regexStr))
		{
			deleteColumIdxs.push_back(columIndex);
		}
		++columIndex;
	}
	// ヘーダの整理と出力
	for (const auto & itrVec : deleteColumIdxs)
	{
		m_mapFileLine.erase(itrVec);
	}
	wstring filePath = m_argument.m_out + L"\\aza" + (pFilePath.substr(pFilePath.length()-6, 2)).c_str() + L".txt";
	if (!m_resultFile.initialize(filePath, UTF8_CREATE))
		return false;
	// 元々、同名のファイルが存在している場合、上書き保存（≠追加書き込み）する。(w+)
	writeResultFile(m_mapFileLine);
	m_mapFileLine.clear();
	// 他の行の整理と出力
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
	
	// 実行ログの出力
	wstring outputLine = mergeRunLogLine(pFilePath.substr(pFilePath.length()-10, 10), L"aza" + pFilePath.substr(pFilePath.length()-6, 6), recordNum+1, recordNum+1, true);
	m_vecRunInfo.push_back(outputLine);
	return true;
}

// oaza[**].txtファイルの作成
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
	std::wregex regexStr1(L"^字・丁目.*");
	std::wregex regexStr2(L"^大字・通称.*(名称|読み).*");
	for (const auto& itrMap : m_mapFileLine)
	{
		// ヘッダ（1行目）に、「字・丁目」という文字列が先頭に付くフィールドが存在する場合、その列を削除。
		wstring compareColum = itrMap.second;
		if (std::regex_match (itrMap.second, regexStr1))
		{
			deleteColumIdxs.push_back(columIndex);
		}
		// ファイル内の「都道府県コード」「市区町村コード」「大字・通称コード」のフィールドの値が、全て同じ行の組み合わせごとに、
		if (compareColum == L"都道府県コード" || compareColum == L"市区町村コード" || compareColum == L"大字・通称コード")
		{
			keyColumIdxs.push_back(columIndex);
		}
		// 以下の2つの条件を両方満たすフィールドの列の値が、全て同じ値がどうかチェックする。
		// ・「大字・通称」という文字列が先頭に付く
		// ・「名称」または「読み」という文字列が含まれる
		if (std::regex_match (itrMap.second, regexStr2))
		{
			compareColumIdxs.push_back(columIndex);
		}
		++columIndex;
	}
	// ヘーダを整理して、出力する
	map<int, wstring> mapHeadLine = m_mapFileLine;
	for (const auto & itrVec : deleteColumIdxs)
	{
		mapHeadLine.erase(itrVec);
	}
	m_mapFileLine.clear();
	wstring filePath = m_argument.m_out + L"\\oaza" + (pFilePath.substr(pFilePath.length()-6, 2)).c_str() + L".txt";
	if (!m_resultFile.initialize(filePath, UTF8_CREATE))
		return false;
	// 元々、同名のファイルが存在している場合、上書き保存（≠追加書き込み）する。(w+)
	writeResultFile(mapHeadLine);
	// 他の行の出力
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
		// 基準行を結果容器への格納
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
				// 基準行との比較結果をエラーログへの出力
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
	// 実行ログの出力
	wstring outputLine = mergeRunLogLine(pFilePath.substr(pFilePath.length()-10, 10), L"oaza" + pFilePath.substr(pFilePath.length()-6, 6), recordNum+1, (m_mapOazaFile.size()+1), bNormal);
	m_vecRunInfo.push_back(outputLine);

	mapHeadLine.clear();
	m_mapErrCheck.clear();
	m_mapOazaFile.clear();
	return true;
}

// パスで、ファイルリストを取得
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
				// ファイル名を容器へ入力する
				m_vecAddressFileName.push_back(fileName);
			}
		}
	}
	FindClose(hFind);
}

// 結果ファイルの書き込む
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

// ファイルの行の分割
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

// 実行ログの整理
wstring AddrTransResultFmtConverter::mergeRunLogLine(const wstring& inFileName, const wstring& outFileName, int inRecordNum, int outRecordNum, bool bNormal)
{
	CStringW inputNum, outputNum;
	inputNum.Format(L"%d", inRecordNum);
	outputNum.Format(L"%d", outRecordNum);
	wstring wStatus = L"正常";
	if (!bNormal)
		wStatus = L"エラーあり";
	return (inFileName + L"\t" + outFileName + L"\t" + wStatus + L"\t" + inputNum.GetString() + L"\t" + outputNum.GetString() + L"\n");
}

// エラーログの整理
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

// 出力情報の統計
void AddrTransResultFmtConverter::outputFileStat()
{
	// 完了時間
	m_runLogger.log(timeMemo(L"完了時間"));
	m_runLogger.log(L"正常終了\n\n");
	for (const auto& itrVec : m_vecRunInfo)
	{
		m_runLogger.log(itrVec);
	}
}

// 実行ログとエラーログのメッセージ
void AddrTransResultFmtConverter::writeInformation()
{
	// errlogのヘッダ
	m_errLogger.log(L"ファイル名\t行数\tエラーメッセージ\t情報1\t情報2\t情報3\n");
	// runlogのヘッダ
	m_vecRunInfo.push_back(L"元ファイル\t出力ファイル\t処理結果\t元件数\t出力件数\n");
	CVersion version;
	CStringW productionVersion;
	productionVersion.Format(L"%s PRODUCTVERSION %s\n", version.GetInternalName(), version.GetProductVersion());
	m_runLogger.log(productionVersion.GetString());
	m_runLogger.log((L"--conv_mode    " + m_argument.m_convMode + L"\n").GetString());
	m_runLogger.log((L"--trans_result " + m_argument.m_transResult + L"\n").GetString());
	m_runLogger.log((L"--out          " + m_argument.m_out + L"\n").GetString());
	m_runLogger.log((L"--run_log      " + m_argument.m_runLog + L"\n").GetString());
	m_runLogger.log((L"--err_log      " + m_argument.m_errLog + L"\n\n").GetString());
	// 開始時間
	m_runLogger.log(timeMemo(L"開始時間"));
}

// 時間記録
wstring AddrTransResultFmtConverter::timeMemo(const wstring& phaseName)
{
	CStringW runTime;
	SYSTEMTIME st;
	::GetLocalTime( &st );
	runTime.Format( L" : %d/%02d/%02d  %02d:%02d:%02d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond );
	return (phaseName + runTime.GetString());
}