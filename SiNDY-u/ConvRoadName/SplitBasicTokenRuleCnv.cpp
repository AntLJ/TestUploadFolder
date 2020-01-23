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
#include "SplitBasicTokenRuleCnv.h"
#include <cwctype>

using namespace sindy::schema;

bool SplitBasicTokenRuleCnv::setRoadLinkFieldIndex()
{
	if (S_OK != m_ipRoadLink->FindField(CComBSTR(::road_link::kRoadCode), &m_roadLinkRoadCodeIndex)) {
		Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgGetIndex, ::road_link::kTableName, ::road_link::kRoadCode);
		return false;
	}
	return true;
}

bool SplitBasicTokenRuleCnv::setSubTblFieldIndex()
{
	
	if (S_OK != m_ipRoadCodeList->FindField(CComBSTR(::road_code_list::kRoadCode), &m_roadCodeListRoadCodeIndex)) {
		Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgGetIndex, ::road_code_list::kTableName, ::road_code_list::kRoadCode);
		return false;
	}
	if (S_OK != m_ipRoadCodeList->FindField(CComBSTR(::road_code_list::kLineClass), &m_roadCodeListLineClsIndex)) {
		Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgGetIndex, ::road_code_list::kTableName, ::road_code_list::kLineClass);
		return false;
	}
	return true;
}

bool SplitBasicTokenRuleCnv::loadSubTable()
{
	// 全てのレコードを取得するが、レプリカでも動くようIQueryPtrを定義(検索時にIQueryPtrを与えないとレプリカで動作しない)
	IQueryFilterPtr ipQFilter(CLSID_QueryFilter);
	// カーソル取得
	_ICursorPtr ipCodeListCursor;
	if (S_OK != m_ipRoadCodeList->Search(ipQFilter, VARIANT_FALSE, &ipCodeListCursor)) {
		Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgGetCursor, ::road_code_list::kTableName);

	}
	// 変換対象道路路線コードレコード取得
	_IRowPtr ipCode;
	while (S_OK == ipCodeListCursor->NextRow(&ipCode) && ipCode) {
		// OBJECTID取得
		long codeOID = -1;
		if (S_OK != ipCode->get_OID(&codeOID)) {
			Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgGetOID, ::road_code_list::kTableName);
			return false;
		}
		// 路線コード取得
		CComVariant codeVal;
		if (S_OK != ipCode->get_Value(m_roadCodeListRoadCodeIndex, &codeVal)) {
			Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgGetVal, ::road_code_list::kTableName, codeOID, ::road_code_list::kRoadCode);
			return false;
		}
		if (!m_tgtRdCodeListRecs.insert(std::make_pair(codeVal.lVal, ipCode)).second) {
			Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::DuplicateData, ::road_code_list::kTableName, codeOID);
			return false;
		}
	}
	return true;
}

bool SplitBasicTokenRuleCnv::cnvSplitName(const long& linkOID, const IFeaturePtr& ipLink, const long& subOID, const _IRowPtr& ipSubRow, const RoadNameRecord& roadNameRecord, std::set<SpltNameRecord>& spltRdNameRecs)
{
	// 辞書に登録されている名称の場合、辞書に従って登録して完了
	if (chkDictionary(roadNameRecord, spltRdNameRecs))
		return true;
	
	// 道路名称データの名称・読みを分割して登録
	// 名称分割用に対象道路リンクの路線種別取得
	long lineCls = -1;
	if (!getLineCls(linkOID, ipLink, lineCls))
		return false;
	// 文字列分割
	std::list<SplitElement> mainNameSpltElem, mainYomiSpltElem;
	basicTokenRuleCnv(roadNameRecord.m_nameID, ::sj::road_name_list::kTableName, lineCls, roadNameRecord.m_name, roadNameRecord.m_yomi, mainNameSpltElem, mainYomiSpltElem);
	// 出力用コンテナに登録
	long seq = 1;
	std::list<SplitElement>::const_iterator nameItr = mainNameSpltElem.begin(),
		yomiItr = mainYomiSpltElem.begin();
	while ((mainNameSpltElem.end() != nameItr) && (mainYomiSpltElem.end() != yomiItr)) {
		// 名称・読みがNULLの場合は飛ばす
		if (nameItr->m_str.IsEmpty() || yomiItr->m_str.IsEmpty()) {
			if (!(nameItr->m_str.IsEmpty() && yomiItr->m_str.IsEmpty())) {
				Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::UnsymmetricalWord, SPLIT_ROAD_NAME, roadNameRecord.m_nameID);
			}
			++nameItr;
			++yomiItr;
			continue;
		}
		SpltNameRecord spltNameRecord(roadNameRecord.m_nameID, nameItr->m_class, seq, nameItr->m_str, yomiItr->m_str);
		if (!spltRdNameRecs.insert(spltNameRecord).second) {
			Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::DuplicateData, SPLIT_ROAD_NAME, roadNameRecord.m_nameID);
			return false;
		}
		++seq;
		++nameItr;
		++yomiItr;
	} 
	// どちらかのイテレータがend()を指していない場合は、分割数が違うのでメッセージ出力
	if ((mainNameSpltElem.end() != nameItr) || (mainYomiSpltElem.end() != yomiItr)) {
		Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::DiffSplitCount, SPLIT_ROAD_NAME, roadNameRecord.m_nameID);
		m_isErr = true;
	}
	
	return true;
}

bool SplitBasicTokenRuleCnv::chkDictionary(const RoadNameRecord& roadNameRecord, std::set<SpltNameRecord>& spltRdNameRecs)
{
	bool existDic = false;
	std::map<CString, std::vector<CString>>::const_iterator nameItr = m_dictionary.find(roadNameRecord.m_name),
		yomiItr = m_dictionary.find(roadNameRecord.m_yomi);
	long seq = 1;
	if ((m_dictionary.end() != nameItr) && (m_dictionary.end() != yomiItr)) {
		std::vector<CString>::const_iterator splitNameItr = nameItr->second.begin(),
			splitYomiItr = yomiItr->second.begin();
		long seq = 1;
		while ((splitNameItr != nameItr->second.end()) || (splitYomiItr != yomiItr->second.end())) {
			split_class::ECode spltCls = getSplitClass(*splitNameItr);
			SpltNameRecord spltNameRecord(roadNameRecord.m_nameID, spltCls, seq, *splitNameItr, *splitYomiItr);
			spltRdNameRecs.insert(spltNameRecord);
			++seq;
			++splitNameItr;
			++splitYomiItr;
		}
		if (splitNameItr != nameItr->second.end() || splitYomiItr != yomiItr->second.end()) {
			Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::DiffSplitCount, SPLIT_ROAD_NAME, roadNameRecord.m_nameID);
			m_isErr = true;
		}
		return true;
	}
	return false;
}

bool SplitBasicTokenRuleCnv::getLineCls(const long& linkOID, const IFeaturePtr& ipLink, long& lineCls)
{
	long codeOID = -1;
	_IRowPtr ipCode;
	if (!getLinkRelRoadCode(linkOID, ipLink, codeOID, ipCode))
		return false;

	if (!ipCode) {
		// 関連付くroad_code_listが無い場合は、lineClsを-1にする。
		lineCls = -1;
		return true;
	}
	// 路線種別取得
	CComVariant lineClsVal;
	if (S_OK != ipCode->get_Value(m_roadCodeListLineClsIndex, &lineClsVal)) {
		Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgGetVal, ::road_code_list::kTableName, codeOID, ::road_code_list::kLineClass);
		return false;
	}
	lineCls = lineClsVal.lVal;
	return true;
}

void SplitBasicTokenRuleCnv::basicTokenRuleCnv(const long& tgtID,
	const CString& tgtTblName,
	const long& lineCls,
	const CString& nameStr,
	const CString& yomiStr,
	std::list<SplitElement>& mainNameSpltElem,
	std::list<SplitElement>& mainYomiSpltElem)
{
	// 分割処理のために名称・読みをmainNameSpltElem, mainYomiSpltElemに追加する。
	CStringW nameStrW = CT2CW(nameStr), yomiStrW = CT2CW(yomiStr);
	SplitElement nameElem(nameStrW, split_class::kBody, false), yomiElem(yomiStrW, split_class::kBody, false);
	mainNameSpltElem.push_back(nameElem);
	mainYomiSpltElem.push_back(yomiElem);
	// 名称・読み「（）」分割
	splitNameYomi(split_func::kParen, tgtID, tgtTblName, L"", L"", mainNameSpltElem, mainYomiSpltElem);

	// 名称・読み「～」分割
	splitNameYomi(split_func::kNormal, tgtID, tgtTblName, L"～", L"～", mainNameSpltElem, mainYomiSpltElem);

	// 路線種別が1,2,3,4,5,6,7,8,11の場合のみの分割
	if (((::road_code_list::line_class::kHighway <= lineCls) && (::road_code_list::line_class::kHiroshima >= lineCls)) ||
		(::road_code_list::line_class::kTF == lineCls)) {
		// 名称「自動車道」読み「ジドウシャドウ」で分割
		splitNameYomi(split_func::kNormal, tgtID, tgtTblName, L"自動車道", L"ジドウシャドウ", mainNameSpltElem, mainYomiSpltElem);
		// 名称「有料道路」読み「ユウリョウドウロ」で分割
		splitNameYomi(split_func::kNormal, tgtID, tgtTblName, L"有料道路", L"ユウリョウドウロ", mainNameSpltElem, mainYomiSpltElem);
		// 名称「道路」読み「ドウロ」で分割
		splitNameYomi(split_func::kNormal, tgtID, tgtTblName, L"道路", L"ドウロ", mainNameSpltElem, mainYomiSpltElem);
	}

	// 名称「××高速」読み「××コウソク」分割
	splitNameYomi(split_func::kHwy, tgtID, tgtTblName, L"高速", L"コウソク", mainNameSpltElem, mainYomiSpltElem);
	// 名称「××号線」読み「××ゴウセン」分割
	long preRoadNoSpltCount = mainNameSpltElem.size();
	splitNameYomiByRoadNo(tgtID, tgtTblName, L"号線", L"ゴウセン", mainNameSpltElem, mainYomiSpltElem);
	// 「号線」「ゴウセン」による分割が行われなかった場合の分割
	if (mainNameSpltElem.size() == preRoadNoSpltCount) {
		splitNameYomiByRoadNo(tgtID, tgtTblName, L"号", L"ゴウ", mainNameSpltElem, mainYomiSpltElem);
	}
	return;
}

bool SplitBasicTokenRuleCnv::getLinkRelRoadCode(const long& linkOID, const IFeaturePtr& ipLink, long& codeOID, _IRowPtr& ipCode)
{
	// リンクの路線コード取得し、road_code_list検索
	CComVariant linkRoadCodeVal;
	if (S_OK != ipLink->get_Value(m_roadLinkRoadCodeIndex, &linkRoadCodeVal)) {
		Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgGetVal, ::road_link::kTableName, linkOID, ::road_link::kRoadCode);
		return false;
	}
	long linkRoadCode = linkRoadCodeVal.lVal;
	// 40万番台以上の場合、渡り線 or 無料区間であり、リンク側のRoadCodeは値が追加されているので、それを考慮する。
	if (CROSS_OVER_CODE_LOW <= linkRoadCode) {
		// 40万番台 or 50万番台の場合は、渡り線であり、20万引く必要あり
		if (CROSS_OVER_CODE_HIG >= linkRoadCode) {
			linkRoadCode = linkRoadCode - CROSS_OVER_PLUS_VALUE;
		}
		else if ((FREE_HWY_CODE_LOW <= linkRoadCode) && (FREE_HWY_CODE_HIG >= linkRoadCode)) {
			linkRoadCode = linkRoadCode - FREE_HWY_PLUS_VALUE;
		}
	}
	std::map<long, _IRowPtr>::const_iterator itr = m_tgtRdCodeListRecs.find(linkRoadCode);
	if (itr == m_tgtRdCodeListRecs.end()) {
		// 取得できない場合もあるので、エラーにはしない
		ipCode = nullptr;
		codeOID = -1;
		return true;
	}
	ipCode = itr->second;
	if (S_OK != ipCode->get_OID(&codeOID)) {
		Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgGetVal, ::road_code_list::kTableName);
		return false;
	}
	return true;
}

split_class::ECode SplitBasicTokenRuleCnv::getSplitClass(const CString& splitStr)
{
	if (_T("～") == splitStr) return split_class::kConnector;
	if (_T("自動車道") == splitStr || _T("ジドウシャドウ") == splitStr ||
		_T("有料道路") == splitStr || _T("ユウリョウドウロ") == splitStr ||
		_T("道路") == splitStr || _T("ドウロ") == splitStr)
		return split_class::kClassifier;
	// 上記以外はBody
	return split_class::kBody;
}

void SplitBasicTokenRuleCnv::splitNameYomi(const split_func::ECode& funcCode,
	const long& tgtID,
	const CString& tgtTblName,
	const CStringW& spltNameWord,
	const CStringW& spltYomiWord,
	std::list<SplitElement>& mainNameSpltElem,
	std::list<SplitElement>& mainYomiSpltElem)
{
	std::list<SplitElement>::iterator nameItr = mainNameSpltElem.begin(),
		yomiItr = mainYomiSpltElem.begin();
	while ((mainNameSpltElem.end() != nameItr) && (mainYomiSpltElem.end() != yomiItr)) {
		// 名称・読みで()属性が統一されていることをチェック
		if ((nameItr->m_isParen && !yomiItr->m_isParen) ||
			(!nameItr->m_isParen && yomiItr->m_isParen)) {
			Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgParenConsis, tgtTblName, tgtID);
			m_isErr = true;
		}
		// ()付きの文字列は、分割対象外
		if (nameItr->m_isParen && yomiItr->m_isParen) {
			++nameItr;
			++yomiItr;
		}
		// 分割属性が統一されていることをチェック
		if (nameItr->m_class != yomiItr->m_class) {
			Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgSpClsConsis, tgtTblName, tgtID);
			m_isErr = true;
		}
		// 分割属性がBody以外は、分割対象外
		if ((nameItr->m_class != split_class::kBody) && (yomiItr->m_class != split_class::kBody)) {
			++nameItr;
			++yomiItr;
		}

		std::vector<SplitElement> nameSpltElem, yomiSpltElem;
		// 名称分割処理
		splitStr(funcCode, tgtID, tgtTblName, spltNameWord, *nameItr, nameSpltElem);
		if (1 < nameSpltElem.size()) {
			// 読み分割処理
			splitStr(funcCode, tgtID, tgtTblName, spltYomiWord, *yomiItr, yomiSpltElem);
		} else {
			// 名称側で分割がされない場合は、読みも分割しない
			yomiSpltElem.push_back(*yomiItr);
		}
		// 名称と読みで分割数が違う場合は、メッセージを出力する.
		if (nameSpltElem.size() != yomiSpltElem.size()) {
			Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::DiffSplitCount, tgtTblName, tgtID);
			m_isErr = true;
		}
		// 分割結果を全ての分割結果にマージ
		mergeElementsToMain(nameSpltElem, nameItr, mainNameSpltElem);
		mergeElementsToMain(yomiSpltElem, yomiItr, mainYomiSpltElem);
	}
	// 分割数がずれていないかのチェックは最後にやるので、ここではしない。
	return;
}

void SplitBasicTokenRuleCnv::splitNameYomiByRoadNo(const long& tgtID,
	const CString& tgtTblName,
	const CStringW& spltBaseNameWord,
	const CStringW& spltBaseYomiWord,
	std::list<SplitElement>& mainNameSpltElem,
	std::list<SplitElement>& mainYomiSpltElem)
{
	std::list<SplitElement>::iterator nameItr = mainNameSpltElem.begin(),
		yomiItr = mainYomiSpltElem.begin();
	while ((mainNameSpltElem.end() != nameItr) && (mainYomiSpltElem.end() != yomiItr)) {
		// 名称・読みで()属性が統一されていることをチェック
		if ((nameItr->m_isParen && !yomiItr->m_isParen) ||
			(!nameItr->m_isParen && yomiItr->m_isParen)) {
			Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgParenConsis, tgtTblName, tgtID);
			m_isErr = true;
		}
		// ()付きの文字列は、分割対象外
		if (nameItr->m_isParen && yomiItr->m_isParen) {
			++nameItr;
			++yomiItr;
		}
		// 分割属性が統一されていることをチェック
		if (nameItr->m_class != yomiItr->m_class) {
			Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgSpClsConsis, tgtTblName, tgtID);
			m_isErr = true;
		}
		// 分割属性がBody以外は、分割対象外
		if ((nameItr->m_class != split_class::kBody) && (yomiItr->m_class != split_class::kBody)) {
			++nameItr;
			++yomiItr;
		}
		std::vector<SplitElement> nameSpltElem, yomiSpltElem;
		// 名称分割処理
		std::vector<CStringW> spltNo;
		splitStrByRoadNo(spltBaseNameWord, *nameItr, spltNo, nameSpltElem);
		if (1 < nameSpltElem.size()) {
			// spltNoの各要素の数字を読みにしたもの ＋ spltBaseYomiWordで通常の分割を実施
			for (const auto strNo : spltNo) {
				long no = _wtol(strNo);
				CString spltYomiWord;
				spltYomiWord.Format(_T("%s%s"), num2YomiStr(no), spltBaseYomiWord);
				splitStrNormal(spltYomiWord, *yomiItr, yomiSpltElem);
			}
		} else {
			// 名称側で分割がされない場合は、読みも分割しない
			yomiSpltElem.push_back(*yomiItr);
		}
		// 分割結果を全ての分割結果にマージ
		mergeElementsToMain(nameSpltElem, nameItr, mainNameSpltElem);
		mergeElementsToMain(yomiSpltElem, yomiItr, mainYomiSpltElem);
	}
	return;
}

void SplitBasicTokenRuleCnv::splitStr(const split_func::ECode& funcCode, 
	const long& tgtID,
	const CString& tgtTblName,
	const CStringW& spltWord,
	const SplitElement& tgtElem,
	std::vector<SplitElement>& spltElem)
{
	switch (funcCode) {
	case split_func::kNormal:
		splitStrNormal(spltWord, tgtElem, spltElem);
		break;
	case split_func::kParen:
		splitStrByParen(tgtID, tgtTblName, tgtElem, spltElem);
		break;
	case split_func::kHwy:
		splitStrByHwy(spltWord, tgtElem, spltElem);
		break;
	default:
		Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgSplitFunc);
		m_isErr = true;
	}
	return;
}

void SplitBasicTokenRuleCnv::splitStrNormal(const CStringW& spltWord,
	const SplitElement& tgtElem,
	std::vector<SplitElement>& spltElem)
{
	long spltPos = tgtElem.m_str.Find(spltWord);
	if (-1 == spltPos) {
		// 分割文字列が存在しない場合は、vectorに挿入して完了
		spltElem.push_back(tgtElem);
		return;
	}
	// 文字列分割実施
	CStringW leftStr, midStr, rightStr;
	leftStr = tgtElem.m_str.Left(spltPos);
	midStr = tgtElem.m_str.Mid(spltPos, spltWord.GetLength());
	rightStr = tgtElem.m_str.Mid(spltPos + spltWord.GetLength());
	// vectorへの登録
	SplitElement leftElem(leftStr, split_class::kBody, false),
		midElem(midStr, getSplitClass(midStr), false), rightElem(rightStr, split_class::kBody, false);
	spltElem.push_back(leftElem);
	spltElem.push_back(midElem);
	// rightElemのみ、分割文字列が存在する可能性があるので、再帰処理
	splitStrNormal(spltWord, rightElem, spltElem);
	return;
}

void SplitBasicTokenRuleCnv::splitStrByParen(const long& tgtID,
	const CString& tgtTblName,
	const SplitElement& tgtElem,
	std::vector<SplitElement>& spltElem)
{
	long frontParenPos = tgtElem.m_str.Find(L"（");
	if (-1 == frontParenPos) {
		// "("が存在しない場合は、vectorに挿入して完了
		spltElem.push_back(tgtElem);
		return;
	}
	long frontParenNum = 0, behindParenNum = 0;
	long behindParenPos = 0;
	long tgtStrLen = tgtElem.m_str.GetLength();
	for (long i = 0; i < tgtStrLen; ++i) {
		CStringW temp = tgtElem.m_str[i];
		if (L"（" == temp)
			++frontParenNum;
		if (L"）" == temp)
			++behindParenNum;
		// ()が閉じるまでcontinue
		if ((0 == frontParenNum) || (frontParenNum != behindParenNum))
			continue;
		// 分割処理
		behindParenPos = i;
		CStringW leftStr, midStr, rightStr;
		leftStr = tgtElem.m_str.Left(frontParenPos);
		midStr = tgtElem.m_str.Mid(frontParenPos, behindParenPos - frontParenPos + 1);
		rightStr = tgtElem.m_str.Mid(behindParenPos + 1);
		// vectorへの登録
		SplitElement leftElem(leftStr, split_class::kBody, false),
			midElem(midStr, split_class::kBody, true), rightElem(rightStr, split_class::kBody, false);
		spltElem.push_back(leftElem);
		spltElem.push_back(midElem);
		// rightElemのみ()が存在する可能性があるので、再帰処理
		splitStrByParen(tgtID, tgtTblName, rightElem, spltElem);
		return;
	}
	// ここまで来た場合は、分割無しなので("（"のみ存在することなる)、vectorにそのまま挿入(errorは出す)
	spltElem.push_back(tgtElem);
	Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::UnsymmetricalParen, tgtTblName, tgtID);
	m_isErr = true;
	return;
}

void SplitBasicTokenRuleCnv::splitStrByHwy(const CStringW& spltWord, const SplitElement& tgtElem, std::vector<SplitElement>& spltElem)
{
	long spltPos = tgtElem.m_str.Find(spltWord);
	if (-1 == spltPos) {
		// 分割文字列が存在しない場合は、vectorに挿入して完了
		spltElem.push_back(tgtElem);
		return;
	}
	// 文字列分割を実施
	// →検索文字列より左と右に分割
	CStringW leftStr, rightStr;
	leftStr = tgtElem.m_str.Left(spltPos + spltWord.GetLength());
	rightStr = tgtElem.m_str.Mid(spltPos + spltWord.GetLength());
	// vectorへの登録
	SplitElement leftElem(leftStr, split_class::kBody, false),
		rightElem(rightStr, split_class::kBody, false);
	spltElem.push_back(leftElem);
	// rightelemはまだ分割できる可能性があるので、再帰処理
	splitStrByHwy(spltWord, rightElem, spltElem);
	return;
}

void SplitBasicTokenRuleCnv::splitStrByRoadNo(const CStringW& spltBaseWord, const SplitElement& tgtElem, std::vector<CString>& spltNo, std::vector<SplitElement>& spltElem)
{
	long spltPos = tgtElem.m_str.Find(spltBaseWord);
	if (-1 == spltPos) {
		// 分割文字列が存在しない場合は、vectorに挿入して完了
		spltElem.push_back(tgtElem);
		return;
	}
	// 検索文字の前の数字部分チェック
	long numberStartPos = -1;
	bool isPreNumber = false;
	for (long pos = 0; pos < spltPos; ++pos) {
		wchar_t oneWord = tgtElem.m_str[pos];
		if (std::iswdigit(oneWord)) {
			if (!isPreNumber)
				numberStartPos = pos;
			isPreNumber = true;
		}
		else {
			isPreNumber = false;
		}
	}
	// 最終的にisPreNumberがfalseの場合は、vectorに挿入して完了
	if (!isPreNumber) {
		spltElem.push_back(tgtElem);
		return;
	}
	// 文字列分割
	CStringW leftStr, midStr, rightStr;
	leftStr = tgtElem.m_str.Left(numberStartPos);
	midStr = tgtElem.m_str.Mid(numberStartPos, spltPos - numberStartPos + spltBaseWord.GetLength());
	rightStr = tgtElem.m_str.Mid(spltPos + spltBaseWord.GetLength());
	// midStrの数字部分を登録
	CStringW numStr = tgtElem.m_str.Mid(numberStartPos, spltPos - numberStartPos);
	spltNo.push_back(numStr);
	// vectorへの登録
	SplitElement leftElem(leftStr, split_class::kBody, false),
		midElem(midStr, split_class::kBody, false), rightElem(rightStr, split_class::kBody, false);
	spltElem.push_back(leftElem);
	spltElem.push_back(midElem);
	// rightElemのみ、分割文字列が存在する可能性があるので、再帰処理
	splitStrByRoadNo(spltBaseWord, rightElem, spltNo, spltElem);
	return;
}

void SplitBasicTokenRuleCnv::mergeElementsToMain(const std::vector<SplitElement>& spltElem, std::list<SplitElement>::iterator& itr, std::list<SplitElement>& mainSpltElem)
{
	// 分割文字列を追加
	for (const auto add : spltElem) {
		itr = mainSpltElem.insert(itr, add);
		++itr;
	}
	// 元文字列の削除
	itr = mainSpltElem.erase(itr);
	return;
}