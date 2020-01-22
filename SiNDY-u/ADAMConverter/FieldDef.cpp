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
#include "FieldDef.h"

bool CFieldDefInfo::init(const CString& groupName, const CString& line)
{
	bool bReturn = true;

	CStringTokenizer cToken(line, CTF_SPLIT_CHAR, TOKEN_RET_EMPTY_ALL);
	long size = cToken.CountTokens();

	if(CF_TABCOUNT > size){
		CString strTmp = line;
		strTmp.Replace(CTF_SPLIT_CHAR, CTF_SPACE_CHAR);
		CLog::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::NgConvFieldTab, groupName, CF_TABCOUNT, strTmp);
		return false;
	}
	
	CString column1 = cToken.GetNextToken();
	CString column2 = cToken.GetNextToken();
	CString column3 = cToken.GetNextToken();
	CString column4 = cToken.GetNextToken();
	CString column5 = cToken.GetNextToken();

	// 変換元情報設定
	if(!setSrcInfo(groupName, column1)){
		bReturn = false;
	}

	// 変換左記情報設定
	if(!setDstInfo(groupName, column2)){
		bReturn = false;
	}

	// フィールド情報設定
	if(0 != m_srcFieldName.CompareNoCase(CF_SHAPE_FIELD)){
		if(!setFieldInfo(groupName, column1, column3, column4, column5)){
			bReturn = false;
		}
	} else {
		m_FieldType = FIELDTYPE::SHAPE;
	}

	// オプションがなければここで終了
	if(CF_TABCOUNT == size)
		return bReturn;

	while(1){
		CString optionName = cToken.GetNextToken();
		if(0 == optionName.CompareNoCase(CF_OPTION_ROUND)){
			// 丸め込み値取得
			if(!setRoundValue(groupName, column1, cToken)){
				bReturn = false;
			}
		} else if(0 == optionName.CompareNoCase(CF_OPTION_ERRVALUE)){
			// エラー値取得
			if(!setConditionalValue(groupName, column1, CF_OPTION_ERRVALUE, cToken, m_setErrValue)){
				bReturn = false;
			}
		} else if(0 == optionName.CompareNoCase(CF_OPTION_ERRRANGE)){
			// エラー範囲取得
			if(!setConditionalRange(groupName, column1, CF_OPTION_ERRRANGE, cToken, m_setErrRange)){
				bReturn = false;
			}
		} else if(0 == optionName.CompareNoCase(CF_OPTION_VALIDVALUE)){
			// 有効値取得
			if(!setConditionalValue(groupName, column1, CF_OPTION_VALIDVALUE, cToken, m_setValidValue)){
				bReturn = false;
			}
		} else if(0 == optionName.CompareNoCase(CF_OPTION_VALIDRANGE)){
			// 有効範囲取得
			if(!setConditionalRange(groupName, column1, CF_OPTION_VALIDRANGE, cToken, m_setValidRange)){
				bReturn = false;
			}
		} else if(0 == optionName.CompareNoCase(CF_OPTION_NULLVALUE)){
			// NULL値取得
			if(!setNullValue(groupName, column1, CF_OPTION_NULLVALUE, cToken, m_NullValue)){
				bReturn = false;
			}
			m_bNullValue = true;
		} else if(0 == optionName.CompareNoCase(CF_OPTION_FIXEDVALUE)){
			// 固定値取得
			if(!setFixedValue(groupName, column1, CF_OPTION_FIXEDVALUE, cToken, m_FixedValue)){
				bReturn = false;
			}
			m_bFixedValue = true;
		} else {
			// その他はNG
			CLog::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::UnknownConvFieldAttribute, groupName, optionName);
			bReturn = false;
		}
		if(!cToken.HasMoreTokens())
			break;
	}

	// 有効値範囲の内容チェック
	if(!checkConditionalRange(groupName, column1, CF_OPTION_VALIDRANGE, m_setValidRange)){
		bReturn = false;
	}
	// エラー値範囲の内容チェック
	if(!checkConditionalRange(groupName, column1, CF_OPTION_ERRRANGE, m_setErrRange)){
		bReturn = false;
	}

	return bReturn;
}

bool CFieldDefInfo::setSrcInfo(const CString& groupName, const CString& column1)
{
	CString strMsg;

	long find = column1.Find(_T("."));
	if(0 > find){
		CLog::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::NoneConvFieldSrcFieldComma, groupName, column1);
		return false;
	}
	m_srcTableName = column1.Left(find);
	m_srcFieldName = column1.Mid(find + 1);

	return true;
}

bool CFieldDefInfo::setDstInfo(const CString& groupName, const CString& column2)
{
	m_dstFieldName = column2;

	return true;
}

bool CFieldDefInfo::setFieldInfo(const CString& groupName, const CString& column1, const CString& column3, const CString& column4, const CString& column5)
{
	bool bReturn = true;

	if(0 == column3.CompareNoCase(CF_FIELDINFO_INTEGER)){
		m_FieldType = FIELDTYPE::INTEGER;
	} else if(0 == column3.CompareNoCase(CF_FIELDINFO_DOUBLE)){
		m_FieldType = FIELDTYPE::DOUBLE;
	} else if(0 == column3.CompareNoCase(CF_FIELDINFO_STRING)){
		m_FieldType = FIELDTYPE::STRING;
	} else if(0 == column3.CompareNoCase(CF_FIELDINFO_SHAPE)){
		m_FieldType = FIELDTYPE::SHAPE;
	} else {
		CLog::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::NgConvFieldFieldType, groupName, column1, column3);
		bReturn = false;
	}
	
	long find = 0;
	switch(m_FieldType){
	case FIELDTYPE::INTEGER:
		if(0 != column4.CompareNoCase(CString(std::to_string(_ttol(column4)).c_str()))){
			CLog::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::NgConvFieldLength, groupName, column1, column4);
			bReturn = false;
		} else {
			m_Number = _ttol(column4);
		}
		break;
	case FIELDTYPE::DOUBLE:
		{
			CStringTokenizer cToken(column4, _T("."), TOKEN_RET_EMPTY_ALL);
			if(2 != cToken.CountTokens()){
				CLog::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::NgConvFieldLength, groupName, column1, column4);
				bReturn = false;
			}
			CString number = cToken.GetNextToken();
			CString decimal = cToken.GetNextToken();
			// 整数部、小数部に数字だけが設定されているかチェック
			if(0 != number.CompareNoCase(CString(std::to_string(_ttol(number)).c_str())) ||
				0 != decimal.CompareNoCase(CString(std::to_string(_ttol(decimal)).c_str()))){
				CLog::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::NgConvFieldLength, groupName, column1, column4);
				bReturn = false;
			} else {
				m_Number = _ttol(number);
				m_Decimal = _ttol(decimal);
			}
			break;
		}
	case FIELDTYPE::STRING:
		if(0 != column4.CompareNoCase(CString(std::to_string(_ttol(column4)).c_str()))){
			CLog::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::NgConvFieldLength, groupName, column1, column4);
			bReturn = false;
		} else {
			m_Length = _ttol(column4);
		}
		break;
	case FIELDTYPE::SHAPE:
		break;
	default:
		// ここにはこない
		CLog::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::NgConvFieldFieldType, groupName, column1, column3);
		bReturn = false;
		break;
	}

	if(0 == column5.CompareNoCase(CF_CANLNULL_NG)){
		m_canNull = false;
	} else if(0 == column5.CompareNoCase(CF_CANLNULL_OK)){
		m_canNull = true;
	} else {
		CLog::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::NgConvFieldNull, groupName, column1, column5);
		bReturn = false;
	}

	return bReturn;
}


bool CFieldDefInfo::setRoundValue(const CString& groupName, const CString& column1, CStringTokenizer& cToken)
{
	// 丸め込み値取得
	CString srcVal, roundVal;
	if(cToken.HasMoreTokens()){
		srcVal = cToken.GetNextToken();
	} else {
		// タブ数不足ならエラー
		CLog::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::NgConvFieldOptionValue, groupName, column1, CF_OPTION_ROUND);
		return false;
	}
	if(cToken.HasMoreTokens()){
		roundVal = cToken.GetNextToken();
	} else {
		// タブ数不足ならエラー
		CLog::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::NgConvFieldOptionValue, groupName, column1, CF_OPTION_ROUND);
		return false;
	}
	// 1対多になるケースはNG
	if(!m_mapRoundValue.insert(std::make_pair(srcVal, roundVal)).second){
		CLog::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::DuplicateConvFieldRoundValue, groupName, column1, srcVal, roundVal);
		return false;
	}
	return true;
}

bool CFieldDefInfo::setConditionalValue(const CString& groupName, const CString& column1, const CString& type, CStringTokenizer& cToken, std::set<CString>& setConditionalValue)
{
	// 条件値取得
	CString ConditionalValue;
	if(cToken.HasMoreTokens()){
		ConditionalValue = cToken.GetNextToken();
	} else {
		CLog::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::NgConvFieldOptionValue, groupName, column1, type);
		return false;
	}
	setConditionalValue.insert(ConditionalValue);

	return true;
}

bool CFieldDefInfo::setConditionalRange(const CString& groupName, const CString& column1, const CString& type, CStringTokenizer& cToken, std::set<CRange>& setConditionalRange)
{
	CString strMsg;

	bool bToken = true;
	CString strSymbol1, strValue1, strSymbol2, strValue2;
	if(cToken.HasMoreTokens()){
		strSymbol1 = cToken.GetNextToken();
	} else {
		bToken = false;
	}
	if(cToken.HasMoreTokens()){
		strValue1 = cToken.GetNextToken();
	} else {
		bToken = false;
	}
	if(cToken.HasMoreTokens()){
		strSymbol2 = cToken.GetNextToken();
	} else {
		bToken = false;
	}
	if(cToken.HasMoreTokens()){
		strValue2 = cToken.GetNextToken();
	} else {
		bToken = false;
	}
	if(!bToken){
		CLog::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::NgConvFieldOptionValue, groupName, column1, type);
		return false;
	}

	// strValue1,2は数値のみ許容
	CRange cConditionalRange;
	if(!cConditionalRange.init(groupName, column1, type, strSymbol1, strValue1, strSymbol2, strValue2)){
		return false;
	}
	setConditionalRange.insert(cConditionalRange);

	return true;
}

bool CFieldDefInfo::setNullValue(const CString& groupName, const CString& column1, const CString& type, CStringTokenizer& cToken, CString& strNullValue)
{
	if(cToken.HasMoreTokens()){
		strNullValue = cToken.GetNextToken();
	} else {
		// タブ数不足ならエラー
		CLog::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::NgConvFieldOptionValue, groupName, column1, CF_OPTION_NULLVALUE);
		return false;
	}
	// NULL値設定が空文字の場合はエラー
	if(strNullValue.IsEmpty()){
		CLog::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgNullValue, groupName, column1, CF_OPTION_NULLVALUE);
		return false;
	}

	return true;
}

bool CFieldDefInfo::setFixedValue(const CString& groupName, const CString& column1, const CString& type, CStringTokenizer& cToken, CString& strFixedValue)
{
	if(cToken.HasMoreTokens()){
		strFixedValue = cToken.GetNextToken();
	} else {
		// タブ数不足ならエラー
		CLog::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::NgConvFieldOptionValue, groupName, column1, CF_OPTION_FIXEDVALUE);
		return false;
	}

	return true;
}
bool CFieldDefInfo::checkConditionalRange(const CString& groupName, const CString& column1, const CString& type, const std::set<CRange>& setConditionalRange)
{
	bool bReturn = true;

	CRange cPreRange;

	for(const auto& validRange : setConditionalRange){
		if(!cPreRange.m_strValue1.IsEmpty()){
			// 0 < a < 10, 8 < a < 15 のように重複している場合のチェック
			if(_ttof(cPreRange.m_strValue2) > _ttof(validRange.m_strValue1)){
				CLog::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::DuplicateRange, groupName, column1, type, cPreRange.m_strSymbol1, cPreRange.m_strValue1, cPreRange.m_strSymbol2, cPreRange.m_strValue2, validRange.m_strSymbol1, validRange.m_strValue1, validRange.m_strSymbol2, validRange.m_strValue2);
				bReturn = false;
			} else if(_ttof(cPreRange.m_strValue2) == _ttof(validRange.m_strValue1)){
				// 0 < a <= 7, 7 <= a < 15 のように重複している場合のチェック
				if(0 == cPreRange.m_strSymbol2.CompareNoCase(SY_LESS_EQ) && 0 == validRange.m_strSymbol1.CompareNoCase(SY_GREATER_EQ)){
					CLog::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::DuplicateRange, groupName, column1, type, cPreRange.m_strSymbol1, cPreRange.m_strValue1, cPreRange.m_strSymbol2, cPreRange.m_strValue2, validRange.m_strSymbol1, validRange.m_strValue1, validRange.m_strSymbol2, validRange.m_strValue2);
					bReturn = false;
				}
			}
		}
		cPreRange = validRange;
	}

	return bReturn;
}

bool CRange::init(const CString& groupName, const CString& column1, const CString& type, const CString& strSymbol1, const CString& strValue1, const CString& strSymbol2, const CString& strValue2)
{
	CString strMsg;

	bool bReturn = true;

	if(strValue1.IsEmpty() || strSymbol1.IsEmpty()){
		CLog::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::NoneRequiredRange, groupName, column1, type);
		bReturn = false;
	}
	if((strValue2.IsEmpty() && !strSymbol2.IsEmpty()) || (!strValue2.IsEmpty() && strSymbol2.IsEmpty())){
		CLog::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::NgSelectRange, groupName, column1, type);
		bReturn = false;
	}

	// 数値が設定されているか確認
	if(!strValue1.IsEmpty() && !checkNumber(strValue1)){
		CLog::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::NgRangeValue, groupName, column1, type, strValue1);
		bReturn = false;
	}
	if(!strValue2.IsEmpty() && !checkNumber(strValue2)){
		CLog::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::NgRangeValue, groupName, column1, type, strValue2);
		bReturn = false;
	}

	// strValue1には小さい方の値を格納させる
	if(strValue2.IsEmpty()){
		if(0 == strSymbol1.CompareNoCase(SY_GREATER_EQ) || 0 == strSymbol1.CompareNoCase(SY_GREATER)){
			m_strValue1 = strValue1;
			m_strSymbol1 = strSymbol1;
			m_strValue2 = RANGE_MAX;
			m_strSymbol2 = SY_LESS_EQ;
		} else if(0 == strSymbol1.CompareNoCase(SY_LESS_EQ) || 0 == strSymbol1.CompareNoCase(SY_LESS)){
			m_strValue1 = RANGE_MIN;
			m_strSymbol1 = SY_GREATER_EQ;
			m_strValue2 = strValue1;
			m_strSymbol2 = strSymbol1;
		}
	} else {
		if(_ttof(strValue1) < _ttof(strValue2)){
			m_strValue1 = strValue1;
			m_strSymbol1 = strSymbol1;
			m_strValue2 = strValue2;
			m_strSymbol2 = strSymbol2;
		} else {
			m_strValue1 = strValue2;
			m_strSymbol1 = strSymbol2;
			m_strValue2 = strValue1;
			m_strSymbol2 = strSymbol1;
		}
		if(0 != m_strSymbol1.CompareNoCase(SY_GREATER_EQ) && 0 != m_strSymbol1.CompareNoCase(SY_GREATER)){
			CLog::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::NgConfigRange, groupName, column1, type, m_strSymbol1, m_strValue1);
			bReturn = false;
		}
		if(0 != m_strSymbol2.CompareNoCase(SY_LESS_EQ) && 0 != m_strSymbol2.CompareNoCase(SY_LESS)){
			CLog::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::NgConfigRange, groupName, column1, type, m_strSymbol2, m_strValue2);
			bReturn = false;
		}
	}

	return bReturn;
}
