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

// SINDY2ROADCODE.cpp: SINDY2ROADCODE �N���X�̃C���v�������e�[�V����
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
			if (aName1 == " " || aName1 == "�@") aName1 = "";
		}
		if (vaName2.vt != VT_NULL && vaName2.vt != VT_EMPTY)
		{
			vaName2.ChangeType(VT_BSTR);
			aName2 = COLE2T(vaName2.bstrVal);
			if (aName2 == " " || aName2 == "�@") aName2 = "";
		}
		if (vaName3.vt != VT_NULL && vaName3.vt != VT_EMPTY)
		{
			vaName3.ChangeType(VT_BSTR);
			aName3 = COLE2T(vaName3.bstrVal);
			if (aName3 == " " || aName3 == "�@") aName3 = "";
		}
		if (vaYomi1.vt != VT_NULL && vaYomi1.vt != VT_EMPTY)
		{
			vaYomi1.ChangeType(VT_BSTR);
			aYomi1 = COLE2T(vaYomi1.bstrVal);
			if (aYomi1 == " " || aYomi1 == "�@") aYomi1 = "";
		}
		if (vaYomi2.vt != VT_NULL && vaYomi2.vt != VT_EMPTY)
		{
			vaYomi2.ChangeType(VT_BSTR);
			aYomi2 = COLE2T(vaYomi2.bstrVal);
			if (aYomi2 == " " || aYomi2 == "�@") aYomi2 = "";
		}
		if (vaYomi3.vt != VT_NULL && vaYomi3.vt != VT_EMPTY)
		{
			vaYomi3.ChangeType(VT_BSTR);
			aYomi3 = COLE2T(vaYomi3.bstrVal);
			if (aYomi3 == " " || aYomi3 == "�@") aYomi3 = "";
		}

		RoadCodeDoc aRoadCodeDoc;
		// [Bug6456] �\�[�g���ς��邽�߁Asindy::road_code_list::lineclass_c�ł͂Ȃ��AINHwySchema�Œ�`�����l���i�[����悤�ɕύX
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
		// �t�F���[�q�H�Ƃ���ȊO�̔���
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

		// �Ǘ��c�̖��i���O�̃��R�[�h�̊Ǘ��c�̖��ƈقȂ鎞�����o�͂���j
		if (! aPrevClass || aPrevClass != aIter->eClass)
		{
			aFile << SINDY2INCODE::SJIS2EUC(LineClass2OrgName(aIter->eClass).c_str());
			aPrevClass = aIter->eClass;
		}

		// �H���R�[�h
		aFile << "\t";
		aFile.width(6); aFile.fill('0');
		aFile << std::dec << aIter->eRoadCode;

		// �H�����́E�ǂ�
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
		// �t�F���[�q�H�͏o�͂��Ȃ�
		if (aIter->eClassForSiRiUS == in::schema::road_code_doc::line_class::ferry) { continue; }

		// �X�����͏o�͂��Ȃ�
		if (aIter->eClassForSiRiUS == in::schema::road_code_doc::line_class::street) { continue; }

		if (cFormat == in::schema::road_code_doc::file_format::roadtype)
		{
			// �����Ǘ��c�̖����o�͂��Ȃ�
			if (aPrevClass != 0 && aPrevClass == aIter->eClassForSiRiUS) { continue; }
			aPrevClass = aIter->eClassForSiRiUS;
		}

		// �Ǘ��c�̃R�[�h
		aFile << aIter->eRoadCode/10000;

		std::pair<std::string, std::string> aNamePair = divideByCaseArc(aIter->eName);
		std::pair<std::string, std::string> aYomiPair = divideByCaseArc(aIter->eYomi);

		switch (cFormat)
		{
			using namespace in::schema::road_code_doc::file_format;
			case roadtype:
				// �Ǘ��c�̖�
				aFile << "\t";
				aFile << SINDY2INCODE::SJIS2EUC(LineClass2OrgName(aIter->eClassForSiRiUS).c_str());

				// �Ǘ��c�̖��ǂ�
				aFile << "\t";
				aFile << SINDY2INCODE::SJIS2EUC(LineClass2OrgYomi(aIter->eClassForSiRiUS).c_str());
				break;

			case roadline:
				// �H���V�[�P���X�ԍ�
				aFile << "\t";
				aFile << aIter->eRoadCode%10000;

				// �H������
				aFile << "\t";
				aFile << SINDY2INCODE::SJIS2EUC(aNamePair.first.c_str());

				// �H�����̓ǂ�
				aFile << "\t";
				aFile << SINDY2INCODE::SJIS2EUC(aYomiPair.first.c_str());

				// �H���⑫����
				aFile << "\t";
				if (aNamePair.second.size())
				{
					aFile << SINDY2INCODE::SJIS2EUC(aNamePair.second.c_str());
				}

				// �H���⑫���̓ǂ�
				aFile << "\t";
				if (aNamePair.second.size())
				{
					aFile << SINDY2INCODE::SJIS2EUC(aYomiPair.second.c_str());
				}

				// �H���Z�k����
				aFile << "\t";
				aFile << SINDY2INCODE::SJIS2EUC(shortenName(aNamePair.first, "�����ԓ�", "��").c_str());

				// �H���Z�k���̓ǂ�
				aFile << "\t";
				aFile << SINDY2INCODE::SJIS2EUC(shortenName(aYomiPair.first, "�W�h�E�V���h�E", "�h�E").c_str());
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
		case ferry:				aOrgName = "�t�F���[�q�H";			break;
		case street:			aOrgName = "�X����";				break;
		case ordinary_tollroad:	aOrgName = "��ʗL�����H";			break;
		case highway:			aOrgName = "�������H";				break;
		case tollroad:			aOrgName = "�L�����H";				break;
		case metropolis:		aOrgName = "��s�������H";			break;
		case hanshin:			aOrgName = "��_�������H";			break;
		case nagoya:			aOrgName = "���É��s�s�������H";	break;
		case fukuoka:			aOrgName = "�����s�s�������H";		break;
		case kitakyushu:		aOrgName = "�k��B�s�s�������H";	break;
		case hiroshima:			aOrgName = "�L���������H";			break;
		case kyouto:			aOrgName = "���s�������H";			break;
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
		case ferry:				aOrgYomi = "�t�F���[�R�E��";						break;
		case street:			aOrgYomi = "�J�C�h�E���C";							break;
		case ordinary_tollroad:	aOrgYomi = "�C�b�p�����E�����E�h�E��";				break;
		case highway:			aOrgYomi = "�R�E�\�N�h�E��";						break;
		case tollroad:			aOrgYomi = "���E�����E�h�E��";						break;
		case metropolis:		aOrgYomi = "�V���g�R�E�\�N�h�E��";					break;
		case hanshin:			aOrgYomi = "�n���V���R�E�\�N�h�E��";				break;
		case nagoya:			aOrgYomi = "�i�S���g�V�R�E�\�N�h�E��";				break;
		case fukuoka:			aOrgYomi = "�t�N�I�J�g�V�R�E�\�N�h�E��";			break;
		case kitakyushu:		aOrgYomi = "�L�^�L���E�V���E�g�V�R�E�\�N�h�E��";	break;
		case hiroshima:			aOrgYomi = "�q���V�}�R�E�\�N�h�E��";				break;
		case kyouto:			aOrgYomi = "�L���E�g�R�E�\�N�h�E��";				break;
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

	// ������'�j'�����邩�H
	std::wstring::size_type aCloseIndex = aSrc.find_last_of(L"�j");
	if (aCloseIndex != std::wstring::npos && aCloseIndex == aSrc.size()-1)
	{
		// '�i'�����邩�H
		std::wstring::size_type aOpenIndex = aSrc.find_last_of(L"�i");
		if (aOpenIndex != std::wstring::npos)
		{
			aExternal.erase(aOpenIndex, std::wstring::npos);
			aInternal.assign(aSrc, aOpenIndex+1, aCloseIndex-aOpenIndex-1);
		}
	}

	return std::make_pair(W2A(aExternal.c_str()), W2A(aInternal.c_str()));
}

