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
#include "FieldAttr.h"

bool CFieldAttr::Init(const CString& strFieldAttrPath, const std::map<CString,esriFieldType>& mapOrgFieldType)
{
	m_mapOrgFieldType = mapOrgFieldType;

	// 設定ファイル読み込み解析
	if(!AnalyzeFieldAttrFile(strFieldAttrPath))
		return false;

	// 新規作成フィールドかどうかで設定OK,NGのチェック
	if(!CheckInfoByNewField())
		return false;

	// フィールド設定情報のチェック
	if(!CheckFieldInfo())
		return false;

	// 初期値のチェック
	if(!CheckDefaultValue())
		return false;

	// 属性指定値のチェック
	if(!CheckFieldValue())
		return false;

	// 丸め込み指定のチェック
	if(!CheckTransValue())
		return false;

	// 属性切り出しのチェック
	if(!CheckAttrValue())
		return false;

	return true;
}

bool CFieldAttr::AnalyzeFieldAttrFile(const CString& strFileAttrPath)
{
	CString strMsg;

	//ファイルの読み込み
	std::ifstream ifs(static_cast<CT2CW>(strFileAttrPath));
	if(ifs.fail()){
		CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgOpenFile, strFileAttrPath);
		return false;
	}

	std::string aLine;
	while(std::getline(ifs, aLine)){
		CString strLine(aLine.c_str());
		if(strLine.IsEmpty())
			continue;

		CStringTokenizer cToken(strLine, _T("\t"), TOKEN_RET_EMPTY_ALL);
		size_t lTokenCount = cToken.CountTokens();
		if(2 >lTokenCount){
			CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgAttrFormatEssField, strLine);
			return false;
		}
		// 一旦vectorに確保
		std::vector<CString> vecAttrColumn;
		for(size_t l=0; l<lTokenCount; ++l)
			vecAttrColumn.push_back(cToken.GetNextToken());

		// 変換元フィールド名が空の場合はNG
		if(vecAttrColumn[0].IsEmpty()){
			CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgAttrFormatOrgFieldEmpty, strLine);
			return false;
		}
		// 変換先フィールド名が空の場合もNG
		if(vecAttrColumn[1].IsEmpty()){
			CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgAttrFormatDstFieldEmpty, strLine);
			return false;
		}
		// 変換元フィールド名に「@」オプション以外が指定されている場合はNG
		field_attr_mark::ECode eOrgMarkType = GetFieldMarkType(vecAttrColumn[0]);
		if(field_attr_mark::kNew != eOrgMarkType && field_attr_mark::kNull != eOrgMarkType){
			CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgAttrFormatOrgFieldExceptNewOp, vecAttrColumn[0]);
			return false;
		}
		// 新規作成フィールドでない場合、フィールドが存在するか確認
		if(field_attr_mark::kNew != eOrgMarkType){
			if(m_mapOrgFieldType.find(vecAttrColumn[0]) == m_mapOrgFieldType.end()){
				CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgAttrFormatOrgFieldName, vecAttrColumn[0]);
				return false;
			}
		}
		// 変換先フィールド名にオプションが指定されている場合はNG
		field_attr_mark::ECode eDstMarkType = GetFieldMarkType(vecAttrColumn[1]);
		if(field_attr_mark::kNull != eDstMarkType){
			CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgAttrFormatDstFieldSetOp, vecAttrColumn[0]);
			return false;
		}
		
		// 拡張機能の処理
		for(long l = 2; l < vecAttrColumn.size(); ++l){
			if(!SetOptionInfo(vecAttrColumn[0], eOrgMarkType, vecAttrColumn[1], vecAttrColumn[l]))
				return false;
		}

		// 変換元、先のフィールド設定が重複していないかチェック
		m_listFieldName.push_back(std::make_pair(vecAttrColumn[0], vecAttrColumn[1]));
		std::pair<std::map<CString,CString>::iterator,bool> result1 = m_mapOrgDstFieldName.insert(std::make_pair(vecAttrColumn[0], vecAttrColumn[1]));
		if(!result1.second){
			CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgAttrFormatOrgFieldDuplicate, vecAttrColumn[0]);
			return false;
		}
		std::pair<std::map<CString,CString>::iterator,bool> result2 = m_mapDstOrgFieldName.insert(std::make_pair(vecAttrColumn[1], vecAttrColumn[0]));
		if(!result2.second){
			CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgAttrFormatDstFieldDuplicate, vecAttrColumn[1]);
			return false;
		}
	}
	return true;
}

field_attr_mark::ECode CFieldAttr::GetFieldMarkType(const CString& strFieldColumn)
{
	CString strMark(strFieldColumn.Left(1));
	if(0 == strMark.Compare(FIELD_MARK_NEW)){
		return field_attr_mark::kNew;
	} else if(0 == strMark.Compare(FIELD_MARK_ROUND)){
		return field_attr_mark::kRound;
	} else if(0 == strMark.Compare(FIELD_MARK_DESIGNATION)){
		return field_attr_mark::kDesignation;
	} else if(0 == strMark.Compare(FIELD_MARK_DEFAULTVALUE)){
		return field_attr_mark::kDefaultValue;
	} else if(0 == strMark.Compare(FIELD_MARK_ATTR)){
		return field_attr_mark::kAttr;
	} else if(0 == strMark.Compare(FIELD_MARK_FIELDINFO)){
		return field_attr_mark::kFiledInfo;
	} else {
		return field_attr_mark::kNull;
	}
}

bool CFieldAttr::SetOptionInfo(const CString& strOrgFieldName, field_attr_mark::ECode eOrgFieldMark, const CString& strDstFieldName, const CString& strColumnInfo)
{
	CString strMsg;
	bool bReturn = false;

	field_attr_mark::ECode eMarkType = GetFieldMarkType(strColumnInfo);
	switch(eMarkType){
	case field_attr_mark::kNew:
		CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgAttrFormatNewOp, strColumnInfo);
		break;
	case field_attr_mark::kRound:
		bReturn = SetOptionRound(strOrgFieldName, eOrgFieldMark, strColumnInfo);
		break;
	case field_attr_mark::kDesignation:
		bReturn = SetOptionDesignation(strOrgFieldName, eOrgFieldMark, strColumnInfo);
		break;
	case field_attr_mark::kDefaultValue:
		bReturn = SetOptionDefaultValue(strDstFieldName, eOrgFieldMark, strColumnInfo);
		break;
	case field_attr_mark::kFiledInfo:
		bReturn = SetOptionFieldInfo(strOrgFieldName, strDstFieldName, eOrgFieldMark, strColumnInfo);
		break;
	case field_attr_mark::kAttr:
		bReturn = SetOptionAttr(strDstFieldName, eOrgFieldMark, strColumnInfo);
		break;
	case field_attr_mark::kNull:
		CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgAttrFormatNoOp, strColumnInfo);
		break;
	default:
		CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgAttrFormatNoOp, strColumnInfo);
		break;
	}

	return bReturn;
}

bool CFieldAttr::SetOptionRound(const CString& strOrgFieldName, field_attr_mark::ECode eOrgFieldMark, const CString& strColumnInfo)
{
	CString strMsg;

	CStringTokenizer cToken(strColumnInfo, _T("'"), TOKEN_RET_EMPTY_ALL);
	size_t lTokenCount = cToken.CountTokens();

	if(1 < lTokenCount){
		// 丸め込みmap取得
		if(!GetStrRoundValue(strOrgFieldName, strColumnInfo))
			return false;
	} else {
		// 丸め込みmap取得
		if(!GetNumRoundValue(strOrgFieldName, strColumnInfo))
			return false;
	}

	return true;
}
bool CFieldAttr::SetOptionDesignation(const CString& strOrgFieldName, field_attr_mark::ECode eOrgFieldMark, const CString& strColumnInfo)
{
	CString strDesignation;
	if(!GetSettingValue(strOrgFieldName, strColumnInfo, strDesignation, false, true))
		return false;

	// 重複OK
	m_mapFieldValue[strOrgFieldName].insert(strDesignation);

	return true;
}
bool CFieldAttr::SetOptionFieldInfo(const CString& strOrgFieldName, const CString& strDstFieldName, field_attr_mark::ECode eOrgFieldMark, const CString& strColumnInfo)
{
	CString strMsg;

	// 先頭のオプション文字を取る
	CString strColInfo = GetColumnValue(strColumnInfo);

	CStringTokenizer cToken(strColInfo, _T(","), TOKEN_RET_EMPTY_ALL);
	size_t lTokenCount = cToken.CountTokens();

	// 一旦mapへ格納
	std::map<CString,CString> mapFieldInfo;
	for(size_t l = 0; l < lTokenCount; ++l){
		CString strPair = cToken.GetNextToken();
		CStringTokenizer cTokenField(strPair, _T("="), TOKEN_RET_EMPTY_ALL);
		size_t lTokenFieldCount = cTokenField.CountTokens();
		if(2 != lTokenFieldCount){
			CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgAttrFormatNoOp, strColumnInfo);
			return false;
		}
		CString strInfo = cTokenField.GetNextToken();
		CString strValue = cTokenField.GetNextToken();

		if(0 != strInfo.CompareNoCase(FIELDINFO_TYPE) &&
			0 != strInfo.CompareNoCase(FIELDINFO_NAME) &&
			0 != strInfo.CompareNoCase(FIELDINFO_SCALE) &&
			0 != strInfo.CompareNoCase(FIELDINFO_PRECISION) &&
			0 != strInfo.CompareNoCase(FIELDINFO_LENGTH) &&
			0 != strInfo.CompareNoCase(FIELDINFO_NULLABLE) &&
			0 != strInfo.CompareNoCase(FIELDINFO_EDITABLE)){
			CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgAttrFormatUnknownFieldInfo, strColumnInfo);
			return false;
		}

		// 重複は認めない
		std::pair<std::map<CString,CString>::iterator,bool> result = mapFieldInfo.insert(std::make_pair(strInfo, strValue));
		if(!result.second){
			CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgAttrFormatFieldInfoDuplicate, strColumnInfo);
			return false;
		}
	}
	// フィールド名も追加しておく
	mapFieldInfo.insert(std::make_pair(FIELDINFO_NAME, strDstFieldName));

	m_mapFieldInfo[strDstFieldName] = mapFieldInfo;

	return true;
}
bool CFieldAttr::SetOptionDefaultValue(const CString& strDstFieldName, field_attr_mark::ECode eOrgFieldMark, const CString& strColumnInfo)
{
	CString strMsg;

	CString strDefaultValue;
	if(!GetSettingValue(strDstFieldName, strColumnInfo, strDefaultValue, true, true))
		return false;

	// 重複は認めない
	std::pair<std::map<DEST_COLNAME,CComVariant>::iterator,bool> result = m_mapDefaultValue.insert(std::make_pair(strDstFieldName,CComBSTR(strDefaultValue)));
	if(!result.second){
		CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgAttrFormatDefaultDupicate, strColumnInfo);
		return false;
	}

	return true;
}

bool CFieldAttr::SetOptionAttr(const CString& strDstFieldName, field_attr_mark::ECode eOrgFieldMark, const CString& strColumnInfo)
{
	CString strMsg;

	// 処理の都合上「[」を削除
	CString strColInfo = GetColumnValue(strColumnInfo);

	CStringTokenizer cToken(strColInfo, _T("'"), TOKEN_RET_EMPTY_ALL);
	size_t lTokenCount = cToken.CountTokens();

	// 一旦vectorに確保
	std::vector<CString> vecAttr;
	for(size_t l = 0; l < lTokenCount; ++l)
		vecAttr.push_back(cToken.GetNextToken());

	long lPos = 0, lSplitCount = -1;
	for(long l = 0; l < vecAttr.size(); ++l){
		lPos++;
		// 奇数番目で「]=」が存在する場合、その前までが設定値となる
		if(1 == lPos % 2 && 0 <= vecAttr.at(l).Find(_T("]="))){
			lSplitCount = l;
		}
	}
	if(0 > lSplitCount){
		CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgAttrFormatAttrAps, strColumnInfo);
		return false;
	}
	// 分割場所が分かったので変換先の値を取得
	long lFind = vecAttr[lSplitCount].Find(_T("]="));
	CString strAttrFilterAdd = vecAttr[lSplitCount].Left(lFind);
	CString strAttrFilter = GetStrValue(0, lSplitCount, vecAttr, false);
	if(!strAttrFilterAdd.IsEmpty())
		strAttrFilter.AppendFormat(_T("%s"), strAttrFilterAdd);

	// 設定値を取得
	CString strSetValueTmp(vecAttr[lSplitCount].Right(vecAttr[lSplitCount].GetLength() - lFind - 2));
	CString strSetValueTmpAdd = GetStrValue(lSplitCount+1, vecAttr.size()-1, vecAttr, false);
	if(!strSetValueTmpAdd.IsEmpty())
		strSetValueTmp = _T("'") + strSetValueTmpAdd;
	//	strSetValueTmp.AppendFormat(_T("'%s"), strSetValueTmpAdd);

	CString strSetValue;
	if(!GetSettingValue(strDstFieldName, strSetValueTmp, strSetValue, true, false))
		return false;

	// 重複NG
	std::pair<std::map<CString,CString>::iterator,bool> result = m_mapAttrValue[strDstFieldName].insert(std::make_pair(strAttrFilter, strSetValue));
	if(!result.second){
		CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgAttrFormatAttrDuplicate, strColumnInfo);
		return false;
	}

	return true;
}
bool CFieldAttr::GetSplitedValue(const CString& strOrgFieldName, const CString& strColumnInfoAll, const CString& strColumnInfo, const CString& str1stValue, const CString& strFindValue, bool bFind, bool bEscape, CString& strSplitedValue, CString& strLeftValue)
{
	CString strColumn = strColumnInfo;
	if(!str1stValue.IsEmpty()){
		if(0 != strColumn.Left(1).Compare(str1stValue)){
			CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgAttrFormatValueAps, strColumnInfoAll);
			return false;
		}
		strColumn.Delete(0,1);
	}

	CStringTokenizer cToken(strColumn, _T("'"), TOKEN_RET_EMPTY_ALL);
	size_t lTokenCount = cToken.CountTokens();

	// 一旦vectorに確保
	std::vector<CString> vecAttr;
	for(size_t l = 0; l < lTokenCount; ++l)
		vecAttr.push_back(cToken.GetNextToken());

	long lSplitCount = -1, lPos = 0;

	for(long l = 0; l < vecAttr.size(); ++l){
		lPos++;
		if(1 == lPos % 2 && 0 == vecAttr.at(l).Compare(strFindValue)){
			lSplitCount = l;
			break;
		} else if(1 == lPos % 2 && !vecAttr.at(l).IsEmpty()){
			// 指定文字or空文字じゃないとNG
			CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgAttrFormatRoundAps, strColumnInfoAll);
			return false;
		}
	}
	if(0 == lSplitCount){ // 一番目にあるのもダメだよね、、
		CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgAttrFormatRoundAps, strColumnInfoAll);
		return false;
	} else if(0 > lSplitCount){
		if(!bFind){
			CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgAttrFormatRoundAps, strColumnInfoAll);
			return false;
		}
		lSplitCount = vecAttr.size()-1;
	}

	// 分割場所が分かったので変換先の値を取得
	strSplitedValue = GetStrValue(1, lSplitCount, vecAttr, bEscape);
	if(bEscape)
		strSplitedValue = strSplitedValue.Left(strSplitedValue.GetLength()-1);
	else
		strSplitedValue = _T("'") + strSplitedValue;
	strLeftValue = GetStrValue(lSplitCount+1, vecAttr.size(), vecAttr, false);
	if(!strLeftValue.IsEmpty())
		strLeftValue = _T("'") + strLeftValue.Left(strLeftValue.GetLength()-1);

	// 空の場合は文字列に問題ありのはず
	if(strSplitedValue.IsEmpty()){
		CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgAttrFormatRoundAps, strColumnInfoAll);
		return false;
	}

	return true;
}

bool CFieldAttr::GetStrRoundValue(const CString& strOrgFieldName, const CString& strColumnInfo)
{
	CString strMsg;

	std::vector<CString> vecTmp;

	CString strDstValue, strLeftValue;
	if(!GetSplitedValue(strOrgFieldName, strColumnInfo, strColumnInfo, _T(":"), _T("("), false, true, strDstValue, strLeftValue)){
		return false;
	}
	strLeftValue = strLeftValue.Left(strLeftValue.GetLength()-1);
	while(1){
		CString strTransValue;
		if(!GetSplitedValue(strOrgFieldName, strColumnInfo, strLeftValue, _T(""), _T(" "), true, true, strTransValue, strLeftValue)){
			return false;
		}
		// 丸め込み用mapに格納
		std::pair<TRANS_VALUE::iterator,bool> result = m_mapTransValue[strOrgFieldName].insert(std::make_pair(CComBSTR(strTransValue), CComBSTR(strDstValue)));
		// 丸め込みの値に重複は認めない
		if(!result.second){
			CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgAttrFormatRoundDuplicate, strColumnInfo);
			return false;
		}
		if(strLeftValue.IsEmpty())
			break;
	}

	return true;
}
bool CFieldAttr::GetNumRoundValue(const CString& strOrgFieldName, const CString& strColumnInfo)
{
	CString strMsg;

	// 処理の都合上「:」を削除
	CString strColInfo = GetColumnValue(strColumnInfo);

	long lSplitCount = -1, lSplitCount2 = 0, lPos = 0;

	long lPos1 = strColInfo.Find(_T("("));
	long lPos2 = strColInfo.Find(_T(")"));
	if(0 >= lPos1 || 0 >= lPos2 || strColInfo.GetLength() != lPos2+1 || lPos1 == lPos2-1){
		CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgAttrFormatRound, strColumnInfo);
		return false;
	}

	// 分割場所が分かったので変換先の値を取得
	CString strDstValue = strColInfo.Mid(0, lPos1);
	if(strDstValue.IsEmpty() || !IsNumeric(strDstValue)){
		CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgAttrFormatRound, strColumnInfo);
		return false;
	}
	CComVariant vaDstValue;
	if(esriFieldTypeInteger == m_mapOrgFieldType[strOrgFieldName]){
		vaDstValue = _ttol(strDstValue);
	} else {
		vaDstValue = _ttof(strDstValue);
	}
	
	// 変換元の値を取得するために一旦加工
	CString strTrans = strColInfo.Mid(lPos1+1, strColInfo.GetLength()-lPos1-2);

	CStringTokenizer cToken(strTrans, _T(" "), TOKEN_RET_EMPTY_ALL);
	size_t lTokenCount = cToken.CountTokens();

	// 数値かどうかチェック
	std::set<CString> setTransValue;
	for(size_t l = 0; l < lTokenCount; ++l){
		bool bRet = true;
		CString strTransVal = cToken.GetNextToken();
		if(strTransVal.IsEmpty() || !IsNumeric(strTransVal)){
			CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgAttrFormatRound, strColumnInfo);
			return false;
		}
		setTransValue.insert(strTransVal);
	}

	// 空の場合は文字列に問題ありのはず
	if(setTransValue.empty()){
		CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgAttrFormatRound, strColumnInfo);
		return false;
	}

	// 丸め込み用mapに格納
	for(const auto trans_val : setTransValue){
		CComVariant vaTransValue;
		if(esriFieldTypeInteger == m_mapOrgFieldType[strOrgFieldName]){
			vaTransValue = _ttol(trans_val);
		} else {
			vaTransValue = _ttof(trans_val);
		}
		m_mapTransValue[strOrgFieldName].insert(std::make_pair(vaTransValue, vaDstValue));
	}

	return true;
}

CString CFieldAttr::GetStrValue(long lStart, long lEnd, const std::vector<CString>& vecRound, bool bEscSQuart)
{
	if(lStart == lEnd)
		return _T("");

	CString strRetValue;
	for(long l = lStart; l < lEnd; ++l){
		CString strTmp = vecRound.at(l);
		strTmp.Append(_T("'"));
		strRetValue.Append(strTmp);
	}
	//if(bAps)
	//strRetValue = strRetValue.Left(strRetValue.GetLength()-1);

	if(bEscSQuart){
		strRetValue.Replace(_T("''"), _T("'"));
	} else {
		//strRetValue = _T("'") + strRetValue;
	}

	return strRetValue;
}

CString CFieldAttr::GetColumnValue(const CString& strColumnInfo)
{
	CString strColInfo(strColumnInfo);
	strColInfo.Delete(0,1);
	return strColInfo;
}

bool CFieldAttr::GetSettingValue(const CString& strFieldName, const CString& strColumnInfo, CString& strRetValue, bool bEscSQuart, bool bOptionDel)
{
	CString strColInfo;
	if(bOptionDel)
		strColInfo = GetColumnValue(strColumnInfo);
	else
		strColInfo = strColumnInfo;

	if(0 <= strColumnInfo.Find(_T("'"))){
		CString strDstValue, strLeftValue;
		if(!GetSplitedValue(strFieldName, strColumnInfo, strColInfo, _T(""), _T(" "), true, bEscSQuart, strRetValue, strLeftValue)){
			return false;
		}
		if(!strLeftValue.IsEmpty()){
			return false;
		}
	} else {
		strRetValue = strColInfo;
		if(!IsNumeric(strColInfo)){
			CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgAttrFormatValue, strColumnInfo);
			return false;
		}
	}
	return true;
}

bool CFieldAttr::GetFieldType(const CString& strOrgFieldName, const CString& strDstFieldName, const std::map<CString,CString>& mapFieldInfo, esriFieldType& eFieldType)
{
	std::map<CString,CString>::const_iterator it = mapFieldInfo.find(FIELDINFO_TYPE);
	if(it == mapFieldInfo.end()){
		std::map<CString,esriFieldType>::const_iterator it2 = m_mapOrgFieldType.find(strOrgFieldName);
		if(it2 == m_mapOrgFieldType.end()){
			CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgGetFieldType, strOrgFieldName);
			return false;
		}
		eFieldType = it2->second;
	} else {
		if(0 == it->second.CompareNoCase(FIELDTYPE_LONG)){
			eFieldType = esriFieldTypeInteger;
		} else if(0 == it->second.CompareNoCase(FIELDTYPE_DOUBLE)){
			eFieldType = esriFieldTypeDouble;
		} else if(0 == it->second.CompareNoCase(FIELDTYPE_STRING)){
			eFieldType = esriFieldTypeString;
		} else {
			CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgGetFieldType, strDstFieldName);
			return false;
		}
	}
	return true;
}


bool CFieldAttr::CheckDefaultValue()
{
	CString strMsg;

	bool bReturn = true;

	// 文字列で格納されているが、CComVariantのフィールドタイプを変更する
	for(const auto default : m_mapDefaultValue){
		esriFieldType eFieldType;
		if(!GetFieldType(m_mapDstOrgFieldName[default.first], default.first, m_mapFieldInfo[default.first], eFieldType)){
			bReturn = false;
			continue;
		}
		
		// 文字列かどうか
		CString strDefaultValue = CString(default.second);
		if(!CheckValueOfFieldType(default.first, _T("初期値"), strDefaultValue, eFieldType)){
			bReturn = false;
			continue;
		}

		// 数値系はCComVariantのフィールドタイプを変えてあげる（ChangeTypeとどちらがよい？）
		switch(eFieldType){
		case esriFieldTypeInteger:
			m_mapDefaultValue[default.first] = _ttol(strDefaultValue);
			break;
		case esriFieldTypeDouble:
			m_mapDefaultValue[default.first] = _ttof(strDefaultValue);
			break;
		default:
			break;
		}
	}
	return bReturn;
}

bool CFieldAttr::CheckInfoByNewField()
{
	bool bReturn = true;

	// 新規作成フィールドで各コンテナの設定確認
	for(const auto pair_col : m_listFieldName){
		// 新規作成フィールドかどうかで処理を分ける
		if(0 == pair_col.first.Left(1).Compare(FIELD_MARK_NEW)){
			// 初期値設定がされていない場合はNG
			if(m_mapDefaultValue.end() == m_mapDefaultValue.find(pair_col.second)){
				CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgAttrFormatNewDefault, pair_col.second);
				bReturn = false;
			}

			// 属性値設定がされている場合はNG
			if(m_mapFieldValue.end() != m_mapFieldValue.find(pair_col.first)){
				CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgAttrFormatNewFieldValue, pair_col.first);
				bReturn = false;
			}

			// 丸め込み設定がされている場合はNG
			if(m_mapTransValue.end() != m_mapTransValue.find(pair_col.first)){
				CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgAttrFormatNewRound, pair_col.first);
				bReturn = false;
			}
		} else {
			// 初期値設定がされている場合はNG
			if(m_mapDefaultValue.end() != m_mapDefaultValue.find(pair_col.second)){
				CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgAttrFormatExistDefault, pair_col.second);
				bReturn = false;
			}

			// 属性切り出し設定がされている場合はNG
			if(m_mapAttrValue.end() != m_mapAttrValue.find(pair_col.second)){
				CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgAttrFormatExistAttr, pair_col.second);
				bReturn = false;
			}
		}
	}

	return bReturn;
}

bool CFieldAttr::CheckFieldInfo()
{
	if(!CheckFieldTypeByNewField())
		return false;

	if(!CheckFieldInfoByFieldType())
		return false;

	if(!CheckFieldInfoValue())
		return false;

	if(!CheckFieldInfoValue2())
		return false;

	return true;
}

bool CFieldAttr::CheckFieldTypeByNewField()
{
	bool bReturn = true;

	// 新規作成フィールドのとき、フィールドタイプが設定されているかどうかを確認
	for(const auto field_info : m_mapFieldInfo){
		bool bNewField = m_mapDstOrgFieldName[field_info.first].Left(1).CompareNoCase(FIELD_MARK_NEW) == 0 ? true : false;
		std::map<CString,CString>::const_iterator it = field_info.second.find(FIELDINFO_TYPE);
		
		if(bNewField && it == field_info.second.end()){
			// 新規作成フィールドでフィールドタイプの設定がされていない場合はNG
			CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgAttrFormatNewFieldInfoType, field_info.first);
			bReturn = false;
		} else if(!bNewField && it != field_info.second.end()){
			// 既存フィールドでフィールドタイプの設定がされている場合はNGにはしないように変更
		}
	}

	return bReturn;
}

bool CFieldAttr::CheckFieldInfoByFieldType()
{
	bool bReturn = true;

	for(const auto field_info : m_mapFieldInfo){
		esriFieldType eFieldType;
		if(!GetFieldType(m_mapDstOrgFieldName[field_info.first], field_info.first, field_info.second, eFieldType)){
			bReturn = false;
			continue;
		}

		std::map<CString,CString>::const_iterator itScale = field_info.second.find(FIELDINFO_SCALE);
		std::map<CString,CString>::const_iterator itPrecision = field_info.second.find(FIELDINFO_PRECISION);
		std::map<CString,CString>::const_iterator itLength = field_info.second.find(FIELDINFO_LENGTH);

		switch(eFieldType){
		case esriFieldTypeInteger:
			// スケール：×、精度：○、長さ：×
			if(itScale != field_info.second.end() ||
				itPrecision == field_info.second.end() ||
				itLength != field_info.second.end()){
					CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgAttrFormatFieldInfoTypeValue, field_info.first);
					bReturn = false;
			}
			break;
		case esriFieldTypeString:
			// スケール：×、精度：×、長さ：○
			if(itScale != field_info.second.end() ||
				itPrecision != field_info.second.end() ||
				itLength == field_info.second.end()){
					CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgAttrFormatFieldInfoTypeValue, field_info.first);
					bReturn = false;
			}
			break;
		case esriFieldTypeDouble:
			// スケール：○、精度：○、長さ：×
			if(itScale == field_info.second.end() ||
				itPrecision == field_info.second.end() ||
				itLength != field_info.second.end()){
					CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgAttrFormatFieldInfoTypeValue, field_info.first);
					bReturn = false;
			}
			break;
		default:
			break;
		}
	}

	return bReturn;
}

bool CFieldAttr::CheckFieldInfoValue()
{
	bool bReturn = true;

	// 格納値に対するチェック
	for(const auto field_info : m_mapFieldInfo){
		for(const auto info : field_info.second){
			CString strPair;
			strPair.Format(_T("%s=%s"), info.first, info.second);

			// 設定項目に対する設定値の確認
			if(0 == info.first.CompareNoCase(FIELDINFO_TYPE)){
				// フィールドタイプの設定値
				if(0 != info.second.CompareNoCase(FIELDTYPE_LONG) &&
					0 != info.second.CompareNoCase(FIELDTYPE_DOUBLE) &&
					0 != info.second.CompareNoCase(FIELDTYPE_STRING)){
						CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgAttrFormatFieldInfoTypeValue, strPair);
						bReturn = false;
				}
			} else if(
				0 == info.first.CompareNoCase(FIELDINFO_SCALE) ||
				0 == info.first.CompareNoCase(FIELDINFO_PRECISION) ||
				0 == info.first.CompareNoCase(FIELDINFO_LENGTH)){
					// スケール、精度、長さは整数値かどうかチェック
					if(!IsInteger(info.second)){
						CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgAttrFormatFieldInfoValue, strPair);
						bReturn = false;
					}
			} else if(
				0 == info.first.CompareNoCase(FIELDINFO_NULLABLE) ||
				0 == info.first.CompareNoCase(FIELDINFO_EDITABLE)){
					// NULL許可、編集許可は0、1のみ許可
					if(!IsMatch(_T("01"), info.second) && 1 == info.second.GetLength()){
						CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgAttrFormatFieldInfoValue, strPair);
						bReturn = false;
					}
			}
		}
	}
	return bReturn;
}
bool CFieldAttr::CheckFieldInfoValue2()
{
	CString strMsg;
	
	bool bReturn = true;
	
	// 格納値に対するチェック2
	for(const auto field_info : m_mapFieldInfo){
		esriFieldType eFieldType;
		if(!GetFieldType(m_mapDstOrgFieldName[field_info.first], field_info.first, field_info.second, eFieldType)){
			bReturn = false;
			continue;
		}

		for(const auto info : field_info.second){
			CString strPair;
			strPair.Format(_T("%s=%s"), info.first, info.second);

			// スケールの設定値確認
			if(0 == info.first.CompareNoCase(FIELDINFO_SCALE)){
				// 範囲内かチェック
				long lScale = _ttol(info.second);
				switch(eFieldType){
				case esriFieldTypeDouble:
					if(0 >= lScale || 18 < lScale){
						CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgAttrFormatFieldInfoScale, strPair);
						bReturn = false;
					}
					break;
				default:
					break;
				}
			} else if(0 == info.first.CompareNoCase(FIELDINFO_PRECISION)){
				long lPrecision = _ttol(info.second);
				switch(eFieldType){
				case esriFieldTypeInteger:
					if(0 >= lPrecision || 9 < lPrecision){
						CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgAttrFormatFieldInfoPrecision, strPair);
						bReturn = false;
					}
					break;
				case esriFieldTypeDouble:
					if(0 >= lPrecision || 18 < lPrecision){
						CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgAttrFormatFieldInfoPrecision, strPair);
						bReturn = false;
					}
					break;
				default:
					break;
				}
			} else if(0 == info.first.CompareNoCase(FIELDINFO_LENGTH)){
				long lLength = _ttol(info.second);
				switch(eFieldType){
				case esriFieldTypeString:
					if(0 >= lLength || 254 < lLength){
						CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgAttrFormatFieldInfoLength, strPair);
						bReturn = false;
					}
					break;
				default:
					break;
				}
			}
		}
	}

	return bReturn;
}

bool CFieldAttr::CheckFieldValue()
{
	bool bReturn = true;

	for(const auto field_value: m_mapFieldValue){
		esriFieldType eFieldType = m_mapOrgFieldType[field_value.first];
		for(const auto value : field_value.second){
			if(!CheckValueOfFieldType(field_value.first, _T("属性値指定"), value, eFieldType)){
				bReturn = false;
			}
		}
	}

	return bReturn;
}

bool CFieldAttr::CheckTransValue()
{
	bool bReturn = true;

	for(const auto& trans_value : m_mapTransValue){
		esriFieldType eFieldType = m_mapOrgFieldType[trans_value.first];
		for(const auto& value : trans_value.second){
			if(!CheckValueOfFieldType(trans_value.first, _T("丸め込み指定"), value.first, eFieldType)){
				bReturn = false;
			}
			if(!CheckValueOfFieldType(trans_value.first, _T("丸め込み指定"), CString(value.second), eFieldType)){
				bReturn = false;
			}
		}
	}

	return bReturn;
}

bool CFieldAttr::CheckAttrValue()
{
	CString strMsg;

	bool bReturn = true;

	for(const auto& attr_value : m_mapAttrValue){
		esriFieldType eFieldType;
		if(!GetFieldType(m_mapDstOrgFieldName[attr_value.first], attr_value.first, m_mapFieldInfo[attr_value.first], eFieldType)){
			bReturn = false;
			continue;
		}
		for(const auto& value : attr_value.second){
			if(!CheckValueOfFieldType(attr_value.first, _T("属性切り出し指定"), value.second, eFieldType)){
				bReturn = false;
			}
		}
	}

	return bReturn;
}

bool CFieldAttr::CheckValueOfFieldType(const CString& strFieldName, const CString& strOption, const CString& strValue, esriFieldType eFieldType)
{
	CString strMsg;

	bool bReturn = false;

	switch(eFieldType){
	case esriFieldTypeInteger:
		bReturn = IsInteger(strValue);
		break;
	case esriFieldTypeDouble:
		bReturn = IsNumeric(strValue);
		break;
	case esriFieldTypeString:
		// 文字列の場合は「'」の括りがOKなら問題なし
		bReturn = true;
		break;
	default:
		break;
	}
	if(!bReturn){
		CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgAttrFormatFieldInfoTypeValue, strValue);
	}
	return bReturn;
}
