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

// SINDY2ROADCODE.cpp: SINDY2ROADCODE クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "SINDY2ROADCODE.h"
#include "SINDY2INCODE.h"
#include "WKHString.h"

SINDY2ROADCODE::SINDY2ROADCODE(SINDY2INHWYINFO *cHwyInfo) :
SINDY2INHWY(cHwyInfo)
{

}

SINDY2ROADCODE::~SINDY2ROADCODE()
{

}

HRESULT SINDY2ROADCODE::Convert()
{
	HRESULT hr;
	const char *func = "SINDY2ROADCODE::Convert";

#ifdef	_DEBUG
	debug = 0;
#endif
	for (GDBOBJITER aIter = mpHwyInfo->eRoadCodeList.begin(); aIter != mpHwyInfo->eRoadCodeList.end(); aIter++)
	{
#ifdef	_DEBUG
		std::cerr << "Creating road.code.doc... : " << ++debug << "/" << mpHwyInfo->eRoadCodeList.size() << "\r";
#endif
		_IRowPtr ipRow = aIter->second;

		static long aLineClassFieldIndex = ipRow->Fields->_FindField(_bstr_t(sindy::road_code_list::LINECLASS_C.c_str()));
		long aLineClass = ipRow->GetValue(aLineClassFieldIndex);

		static long aRoadCodeFieldIndex = ipRow->Fields->_FindField(_bstr_t(sindy::road_code_list::ROAD_CODE.c_str()));
		long aRoadCode = ipRow->GetValue(aRoadCodeFieldIndex);

		CComVariant vaName1, vaName2, vaName3, vaYomi1, vaYomi2, vaYomi3;
		if (FAILED(hr = GDBGetValue(ipRow, sindy::road_code_list::NAME_KANJI, &vaName1))) return hr;
		if (FAILED(hr = GDBGetValue(ipRow, sindy::road_code_list::NAME_YOMI, &vaYomi1))) return hr;
		if (FAILED(hr = GDBGetValue(ipRow, sindy::road_code_list::WORK_KANJI, &vaName2))) return hr;
		if (FAILED(hr = GDBGetValue(ipRow, sindy::road_code_list::WORK_YOMI, &vaYomi2))) return hr;
		if (FAILED(hr = GDBGetValue(ipRow, sindy::road_code_list::DISPLAY_KANJI, &vaName3))) return hr;
		if (FAILED(hr = GDBGetValue(ipRow, sindy::road_code_list::DISPLAY_YOMI, &vaYomi3))) return hr;

		std::string aName1(""), aName2(""), aName3(""), aYomi1(""), aYomi2(""), aYomi3("");
		if (vaName1.vt != VT_NULL && vaName1.vt != VT_EMPTY)
		{
			vaName1.ChangeType(VT_BSTR);
			aName1 = COLE2T(vaName1.bstrVal);
			if (aName1 == " " || aName1 == "　") aName1 = "";
		}
		if (vaName2.vt != VT_NULL && vaName2.vt != VT_EMPTY)
		{
			vaName2.ChangeType(VT_BSTR);
			aName2 = COLE2T(vaName2.bstrVal);
			if (aName2 == " " || aName2 == "　") aName2 = "";
		}
		if (vaName3.vt != VT_NULL && vaName3.vt != VT_EMPTY)
		{
			vaName3.ChangeType(VT_BSTR);
			aName3 = COLE2T(vaName3.bstrVal);
			if (aName3 == " " || aName3 == "　") aName3 = "";
		}
		if (vaYomi1.vt != VT_NULL && vaYomi1.vt != VT_EMPTY)
		{
			vaYomi1.ChangeType(VT_BSTR);
			aYomi1 = COLE2T(vaYomi1.bstrVal);
			if (aYomi1 == " " || aYomi1 == "　") aYomi1 = "";
		}
		if (vaYomi2.vt != VT_NULL && vaYomi2.vt != VT_EMPTY)
		{
			vaYomi2.ChangeType(VT_BSTR);
			aYomi2 = COLE2T(vaYomi2.bstrVal);
			if (aYomi2 == " " || aYomi2 == "　") aYomi2 = "";
		}
		if (vaYomi3.vt != VT_NULL && vaYomi3.vt != VT_EMPTY)
		{
			vaYomi3.ChangeType(VT_BSTR);
			aYomi3 = COLE2T(vaYomi3.bstrVal);
			if (aYomi3 == " " || aYomi3 == "　") aYomi3 = "";
		}

		RoadCodeDoc aRoadCodeDoc;
		// [Bug6456] ソート基準を変えるため、sindy::road_code_list::lineclass_cではなく、INHwySchemaで定義した値を格納するように変更
		aRoadCodeDoc.eClass				= Sindy2INCode(in::schema::road_code_doc::line_class::tLineClass, aLineClass);
		aRoadCodeDoc.eClassForSiRiUS	= Sindy2INCode(in::schema::road_code_doc::line_class::tLineClassForSiRiUS, aLineClass);
		aRoadCodeDoc.eRoadCode  = aRoadCode;
		aRoadCodeDoc.eName  = aName1;
		aRoadCodeDoc.eYomi  = aYomi1;
		aRoadCodeDoc.eName2 = aName2;
		aRoadCodeDoc.eYomi2 = aYomi2;
		aRoadCodeDoc.eName3 = aName3;
		aRoadCodeDoc.eYomi3 = aYomi3;

		mRoadCodeDocArray.insert(aRoadCodeDoc);
	}

#ifdef	_DEBUG
	std::cerr << std::endl;
#endif

	return S_OK;
}

HRESULT SINDY2ROADCODE::Write(std::string cFileName, long cType)
{
	const char *func = "SINDY2ROADCODE::Write";

	ofstream aFile(cFileName.c_str(), ios::binary);
	if (! aFile.is_open()) GDBERROR2(E_ACCESSDENIED, func);

	long aPrevClass = 0;
	for (std::multiset<RoadCodeDoc,lessRoadCodeDoc>::iterator aIter = mRoadCodeDocArray.begin(); aIter != mRoadCodeDocArray.end(); aIter++)
	{
		// フェリー航路とそれ以外の判別
		switch (cType)
		{
			using namespace in::schema::road_code_doc::file_type;
			case road_code:
			case road_code_highway:
			case road_code_display:
				if (aIter->eClass == in::schema::road_code_doc::line_class::ferry) continue;
				break;
			case ferry_code:
				if (aIter->eClass != in::schema::road_code_doc::line_class::ferry) continue;
				break;
			default:
				ATLASSERT(0);
		}

		// 管理団体名（直前のレコードの管理団体名と異なる時だけ出力する）
		if (! aPrevClass || aPrevClass != aIter->eClass)
		{
			aFile << SINDY2INCODE::SJIS2EUC(LineClass2OrgName(aIter->eClass).c_str());
			aPrevClass = aIter->eClass;
		}

		// 路線コード
		aFile << "\t";
		aFile.width(6); aFile.fill('0');
		aFile << std::dec << aIter->eRoadCode;

		// 路線名称・読み
		switch (cType)
		{
			using namespace in::schema::road_code_doc::file_type;
			case road_code:
			case ferry_code:
				aFile << "\t";
				aFile << SINDY2INCODE::SJIS2EUC(aIter->eName.c_str());
				if (aIter->eYomi.size())
				{
					aFile << "\t";
					aFile << SINDY2INCODE::SJIS2EUC(aIter->eYomi.c_str());
				}
				break;
			case road_code_highway:
				aFile << "\t";
				aFile << SINDY2INCODE::SJIS2EUC(aIter->eName2.c_str());
				aFile << "\t";
				aFile << SINDY2INCODE::SJIS2EUC(aIter->eYomi2.c_str());
				break;
			case road_code_display:
				aFile << "\t";
				aFile << SINDY2INCODE::SJIS2EUC(aIter->eName3.c_str());
				aFile << "\t";
				aFile << SINDY2INCODE::SJIS2EUC(aIter->eYomi3.c_str());
				break;
		}

		aFile << std::endl;
	}

	return S_OK;
}

HRESULT SINDY2ROADCODE::WriteForSirius(std::string cFileName, long cFormat)
{
	const char *func = "SINDY2ROADCODE::WriteForSirius";

	ofstream aFile(cFileName.c_str(), ios::binary);
	if (! aFile.is_open()) GDBERROR2(E_ACCESSDENIED, func);

	long aPrevClass = 0;
	for (std::multiset<RoadCodeDoc,lessRoadCodeDoc>::const_iterator aIter = mRoadCodeDocArray.begin(); aIter != mRoadCodeDocArray.end(); aIter++)
	{
		// フェリー航路は出力しない
		if (aIter->eClassForSiRiUS == in::schema::road_code_doc::line_class::ferry) { continue; }

		// 街道名は出力しない
		if (aIter->eClassForSiRiUS == in::schema::road_code_doc::line_class::street) { continue; }

		if (cFormat == in::schema::road_code_doc::file_format::roadtype)
		{
			// 同じ管理団体名を出力しない
			if (aPrevClass != 0 && aPrevClass == aIter->eClassForSiRiUS) { continue; }
			aPrevClass = aIter->eClassForSiRiUS;
		}

		// 管理団体コード
		aFile << aIter->eRoadCode/10000;

		std::pair<std::string, std::string> aNamePair = divideByCaseArc(aIter->eName);
		std::pair<std::string, std::string> aYomiPair = divideByCaseArc(aIter->eYomi);

		switch (cFormat)
		{
			using namespace in::schema::road_code_doc::file_format;
			case roadtype:
				// 管理団体名
				aFile << "\t";
				aFile << SINDY2INCODE::SJIS2EUC(LineClass2OrgName(aIter->eClassForSiRiUS).c_str());

				// 管理団体名読み
				aFile << "\t";
				aFile << SINDY2INCODE::SJIS2EUC(LineClass2OrgYomi(aIter->eClassForSiRiUS).c_str());
				break;

			case roadline:
				// 路線シーケンス番号
				aFile << "\t";
				aFile << aIter->eRoadCode%10000;

				// 路線名称
				aFile << "\t";
				aFile << SINDY2INCODE::SJIS2EUC(aNamePair.first.c_str());

				// 路線名称読み
				aFile << "\t";
				aFile << SINDY2INCODE::SJIS2EUC(aYomiPair.first.c_str());

				// 路線補足名称
				aFile << "\t";
				if (aNamePair.second.size())
				{
					aFile << SINDY2INCODE::SJIS2EUC(aNamePair.second.c_str());
				}

				// 路線補足名称読み
				aFile << "\t";
				if (aNamePair.second.size())
				{
					aFile << SINDY2INCODE::SJIS2EUC(aYomiPair.second.c_str());
				}

				// 路線短縮名称
				aFile << "\t";
				aFile << SINDY2INCODE::SJIS2EUC(shortenName(aNamePair.first, "自動車道", "道").c_str());

				// 路線短縮名称読み
				aFile << "\t";
				aFile << SINDY2INCODE::SJIS2EUC(shortenName(aYomiPair.first, "ジドウシャドウ", "ドウ").c_str());
				break;

			default:
				ATLASSERT(0);
		}
		aFile << std::endl;
	}
			
	return S_OK;
}


std::string SINDY2ROADCODE::LineClass2OrgName(long cLineClass)
{
	const char *func = "SINDY2ROADCODE::LineClass2OrgName";
	std::string aOrgName;

	switch (cLineClass)
	{
		using namespace in::schema::road_code_doc::line_class;
		case ferry:				aOrgName = "フェリー航路";			break;
		case street:			aOrgName = "街道名";				break;
		case ordinary_tollroad:	aOrgName = "一般有料道路";			break;
		case highway:			aOrgName = "高速道路";				break;
		case tollroad:			aOrgName = "有料道路";				break;
		case metropolis:		aOrgName = "首都高速道路";			break;
		case hanshin:			aOrgName = "阪神高速道路";			break;
		case nagoya:			aOrgName = "名古屋都市高速道路";	break;
		case fukuoka:			aOrgName = "福岡都市高速道路";		break;
		case kitakyushu:		aOrgName = "北九州都市高速道路";	break;
		case hiroshima:			aOrgName = "広島高速道路";			break;
		case kyouto:			aOrgName = "京都高速道路";			break;
		default: GDBERROR2(E_NOINTERFACE, func);
	}

	return aOrgName;
}

std::string SINDY2ROADCODE::LineClass2OrgYomi(long cLineClass)
{
	const char *func = "SINDY2ROADCODE::LineClass2OrgYomi";
	std::string aOrgYomi;

	switch (cLineClass)
	{
		using namespace in::schema::road_code_doc::line_class;
		case ferry:				aOrgYomi = "フェリーコウロ";						break;
		case street:			aOrgYomi = "カイドウメイ";							break;
		case ordinary_tollroad:	aOrgYomi = "イッパンユウリョウドウロ";				break;
		case highway:			aOrgYomi = "コウソクドウロ";						break;
		case tollroad:			aOrgYomi = "ユウリョウドウロ";						break;
		case metropolis:		aOrgYomi = "シュトコウソクドウロ";					break;
		case hanshin:			aOrgYomi = "ハンシンコウソクドウロ";				break;
		case nagoya:			aOrgYomi = "ナゴヤトシコウソクドウロ";				break;
		case fukuoka:			aOrgYomi = "フクオカトシコウソクドウロ";			break;
		case kitakyushu:		aOrgYomi = "キタキュウシュウトシコウソクドウロ";	break;
		case hiroshima:			aOrgYomi = "ヒロシマコウソクドウロ";				break;
		case kyouto:			aOrgYomi = "キョウトコウソクドウロ";				break;
		default: GDBERROR2(E_NOINTERFACE, func);
	}

	return aOrgYomi;
}

std::string SINDY2ROADCODE::shortenName(std::string cName, std::string cBefore, std::string cAfter)
{
	std::string aName = cName;

	while(1)
	{
		std::string::size_type aIndex = aName.find(cBefore);
		if (aIndex == std::string::npos) { break; }
		aName.replace(aIndex, cBefore.size(), cAfter);
	}

	return aName;
}

std::pair<std::string, std::string> SINDY2ROADCODE::divideByCaseArc(std::string cStr)
{
	USES_CONVERSION;

	std::wstring aSrc = A2W(cStr.c_str());
	std::wstring aExternal = aSrc;
	std::wstring aInternal;

	// 末尾に'）'があるか？
	std::wstring::size_type aCloseIndex = aSrc.find_last_of(L"）");
	if (aCloseIndex != std::wstring::npos && aCloseIndex == aSrc.size()-1)
	{
		// '（'があるか？
		std::wstring::size_type aOpenIndex = aSrc.find_last_of(L"（");
		if (aOpenIndex != std::wstring::npos)
		{
			aExternal.erase(aOpenIndex, std::wstring::npos);
			aInternal.assign(aSrc, aOpenIndex+1, aCloseIndex-aOpenIndex-1);
		}
	}

	return std::make_pair(W2A(aExternal.c_str()), W2A(aInternal.c_str()));
}

