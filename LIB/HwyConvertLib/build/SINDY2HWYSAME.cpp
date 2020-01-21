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

// SINDY2HWYSAME.cpp: SINDY2HWYSAME クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "SINDY2HWYSAME.h"
#include "SINDY2INCODE.h"
#include "INHwyCommonUtility.h"


SINDY2HWYSAME::SINDY2HWYSAME(SINDY2INHWYINFO *cHwyInfo) :
SINDY2INHWY(cHwyInfo)
{

}

SINDY2HWYSAME::~SINDY2HWYSAME()
{

}

struct HwyFacil
{
	long		eID;
	_IRowPtr		ipFacil;

	HwyFacil() : eID(0), ipFacil(NULL) {}
	virtual ~HwyFacil() {}
};

struct lessFacil : public std::binary_function<HwyFacil,HwyFacil,bool>
{
	bool operator ()(const HwyFacil &cFacil1, const HwyFacil &cFacil2) const { return cFacil1.eID < cFacil2.eID; }
};

typedef std::vector<HwyFacil>					CHILDLIST;
typedef std::map<HwyFacil,CHILDLIST,lessFacil>	PARENTLIST;
typedef PARENTLIST::iterator					PARENTITER;
typedef std::pair<HwyFacil,CHILDLIST>			FACILSET;

HRESULT SINDY2HWYSAME::Convert()
{
	HRESULT hr;
	const char *func = "SINDY2HWYSAME::Convert";

	PARENTLIST aParentList;

#ifdef	_DEBUG
	debug = 0;
#endif
	for (GDBOBJITER aIter = mpHwyInfo->eHighwayText.begin(); aIter != mpHwyInfo->eHighwayText.end(); aIter++)
	{
#ifdef	_DEBUG
		std::cerr << "Creating hwysame.txt... : " << ++debug << "/" << mpHwyInfo->eHighwayText.size() << "\r";
#endif
		_IRowPtr ipHwyText = aIter->second;

		// 親施設を持つ（子施設である）施設のみ対象とする
		CComVariant vaParentID;
		if (FAILED(hr = GDBGetValue(ipHwyText, sindy::highway_text::PARENT_ID, &vaParentID))) return hr;
		vaParentID.ChangeType(VT_I4);
		if (! vaParentID.lVal) continue;

		// ハイウェイテキストから親施設（の行）へのポインタを取得（なければエラー）
		HwyFacil aParent;
		aParent.eID = vaParentID.lVal;
		if (FAILED(hr = mpHwyInfo->eHighwayText.SearchByObjectID(vaParentID.lVal, &aParent.ipFacil))) return hr;
		if (aParent.ipFacil == NULL) return GDBERROR2(E_UNEXPECTED, func);

		// 子施設の値を格納
		HwyFacil aChild;
		if (FAILED(hr = ipHwyText->get_OID(&aChild.eID))) return GDBERROR(hr, func);
		aChild.ipFacil = ipHwyText;

		// 親・子施設のリストに親が既存なら子を、なければ親を追加
		PARENTITER aFacil = aParentList.find(aParent);
		if (aFacil == aParentList.end())
		{
			CHILDLIST aChildList;
			aChildList.push_back(aChild);
			aParentList.insert(FACILSET(aParent, aChildList));
		}
		else
		{
			aFacil->second.push_back(aChild);
		}
	}

#ifdef	_DEBUG
	std::cerr << std::endl;
#endif

	for (PARENTITER aIter = aParentList.begin(); aIter != aParentList.end(); aIter++)
	{
		HwySame aHwySame;

		if (FAILED(hr = mpHwyInfo->GetFacilCode(aIter->first.ipFacil, &aHwySame.eRoadCode, &aHwySame.eSequence))) return hr;

		// 方向フラグを格納
		CComVariant vaUpDown;
		if (FAILED(hr = GDBGetValue(aIter->first.ipFacil, sindy::highway_text::UPDOWN_C, &vaUpDown))) return hr;
		vaUpDown.ChangeType(VT_I4);
		switch (vaUpDown.lVal)
		{
		case sindy::highway_text::updown_up: aHwySame.eDirection = HwySame::back; break;
		case sindy::highway_text::updown_down: aHwySame.eDirection = HwySame::fore; break;
		}

		// 施設名称を格納
		CComVariant vaName;
		if (FAILED(hr = GDBGetValue(aIter->first.ipFacil, sindy::highway_text::NAME_KANJI, &vaName))) return hr;
		if (vaName.vt != VT_NULL && vaName.vt != VT_EMPTY)
		{
			vaName.ChangeType(VT_BSTR);
			aHwySame.eName = COLE2T(vaName.bstrVal);
		}

		// 各子施設に路線内シーケンスを付与
		for (unsigned long i = 0; i < aIter->second.size(); i++)
		{
			CComVariant vaSeq;
			if (FAILED(hr = GDBGetValue(aIter->second[i].ipFacil, sindy::highway_text::ROAD_SEQ, &vaSeq))) return hr;
			vaSeq.ChangeType(VT_I4);
			aHwySame.eFacil.push_back(vaSeq.lVal);
		}

		mHwySameList.push_back(aHwySame);
	}

	return S_OK;
}

HRESULT SINDY2HWYSAME::Write(std::string cFileName, EFormat cFormat)
{
	const char *func = "SINDY2HWYSAME::Write";

	// 書き込み前に施設コードでソートする(施設コードが同一のものは存在してはいけない -- 2003/11/17)
	std::set<HwySame,lessHwySame> aSortHwySameList;

	switch(cFormat)
	{
	case hwysame_new:
		for (unsigned long i = 0; i < mHwySameList.size(); i++)
			aSortHwySameList.insert(mHwySameList[i]);
		break;
	case hwysame_old:
		for (unsigned long i = 0; i < mHwySameListWithoutSmartIC.size(); i++)
			aSortHwySameList.insert(mHwySameListWithoutSmartIC[i]);
		break;
	default:
		ATLASSERT(0);
	}

	std::ofstream aFile(cFileName.c_str(), ios::binary);
	if (! aFile.is_open()) return GDBERROR2(E_ACCESSDENIED, func);

	for (std::set<HwySame,lessHwySame>::iterator aIter = aSortHwySameList.begin(); aIter != aSortHwySameList.end(); aIter++)
	{
		aFile.width(6); aFile.fill('0');
		aFile << std::dec << aIter->eRoadCode;

		aFile << "\t";
		aFile << std::dec << aIter->eSequence;

		aFile << "\t";
		aFile << std::dec << aIter->eDirection;

		aFile << "\t";
		aFile << SINDY2INCODE::SJIS2EUC(aIter->eName.c_str());

		for (unsigned long j = 0; j < aIter->eFacil.size(); j++)
		{
			aFile << "\t";
			aFile << std::dec << aIter->eFacil[j];
		}

		aFile << std::endl;
	}

	return S_OK;
}

void SINDY2HWYSAME::changeFacilName( Customer cCustomer, FacilNameChangeTable* cpFacilNameChangeTable )
{
	if (! needFacilNameChange( cCustomer ) )
		{ return; }

	changeFacilName( cpFacilNameChangeTable );
}

void SINDY2HWYSAME::changeFacilName( FacilNameChangeTable* cpFacilNameChangeTable )
{
	for (Data::iterator aDataIt = mHwySameList.begin(); aDataIt != mHwySameList.end(); ++aDataIt)
	{
		std::auto_ptr<ChangedFacilName> pChangedFacilName = cpFacilNameChangeTable->getChangedFacilName(
			aDataIt->eRoadCode, aDataIt->eSequence, aDataIt->eName
		);
		if (pChangedFacilName.get() == NULL)
		{
			continue;	// 変換対象レコードでなければスキップ
		}
#ifdef	_DEBUG
		std::cout << "hwysame.txt" << "\t" << "施設名称変換" << "\t" << aDataIt->eRoadCode << "\t" << aDataIt->eSequence << "\t";
		std::cout << aDataIt->eName << "→" << pChangedFacilName->m_Kanji << std::endl;
#endif
		// 施設名称変換
		aDataIt->eName = pChangedFacilName->m_Kanji;
	}
}

/**
 * @brief スマートICを除いたデータの作成
 * @note	mHwySameListからスマートICを含まないレコードをmHwySameListWithoutSmartICにコピーする
**/
HRESULT SINDY2HWYSAME::createHwySameListWithoutSmartIC(void)
{
	HRESULT hr;

	for (Data::const_iterator aIt = mHwySameList.begin(); aIt != mHwySameList.end(); ++aIt)
	{
		HwySame aHwySame;
		aHwySame = *aIt;
		aHwySame.eFacil.clear();

		for (std::vector<long>::const_iterator aChildIt = aIt->eFacil.begin(); aChildIt != aIt->eFacil.end(); ++aChildIt)
		{
			GDBHWYFACILRANGE aRange;
			if (FAILED(hr = mpHwyInfo->eFacilInfoPoint.SearchByFacilCode(aIt->eRoadCode, *aChildIt, &aRange))) return hr;
			ATLASSERT(std::distance(aRange.first, aRange.second) == 1);
			IFeaturePtr ipFacilInfoPoint = aRange.first->second;

			CComVariant vaFacilClass;
			if (FAILED(hr = GDBGetValue(ipFacilInfoPoint, sindy::facil_info_point::FACILCLASS_C, &vaFacilClass))) return hr;
			vaFacilClass.ChangeType(VT_I4);

			if (vaFacilClass.lVal != sindy::facil_info_point::facilclass_smartic)
			{
				aHwySame.eFacil.push_back(*aChildIt);
			}
		}
		
		if (aHwySame.eFacil.size() > 0)
		{
			mHwySameListWithoutSmartIC.push_back(aHwySame);
		}
	}

	return S_OK;
}
