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
#include "CheckString.h"

bool CCheckString::checkOutputString(const _IRowPtr& ipRow, const CConvTableInfo& cConvTableInfo, const CFieldDefInfo& cFieldDef, const CString& strValue)
{
	bool bReturn = true;

	// 有効値チェック
	if(!cFieldDef.m_setValidValue.empty()){
		if(!checkValue(cFieldDef.m_setValidValue, strValue, true)){
			CLog::GetInstance().PrintLog2(false, true, true, true, err_type::error, err_code::NgCheckValidValue, cConvTableInfo.m_oidTableName, getID(cConvTableInfo, ipRow), cFieldDef.m_srcTableName, cFieldDef.m_srcFieldName, strValue);
			bReturn = false;
		}
	}
	// エラー値チェック
	if(!cFieldDef.m_setErrValue.empty()){
		if(checkValue(cFieldDef.m_setErrValue, strValue, false)){
			CLog::GetInstance().PrintLog2(false, true, true, true, err_type::error, err_code::NgCheckErrValue, cConvTableInfo.m_oidTableName, getID(cConvTableInfo, ipRow), cFieldDef.m_srcTableName, cFieldDef.m_srcFieldName, strValue);
			bReturn = false;
		}
	}
	if(!strValue.IsEmpty() && !cFieldDef.m_setValidRange.empty()){
		// 有効値範囲チェック
		if(!checkRangeValue(cFieldDef, cFieldDef.m_setValidRange, strValue, true)){
			CLog::GetInstance().PrintLog2(false, true, true, true, err_type::error, err_code::NgCheckValidRange, cConvTableInfo.m_oidTableName, getID(cConvTableInfo, ipRow), cFieldDef.m_srcTableName, cFieldDef.m_srcFieldName, strValue);
			bReturn = false;
		}
	}
	if(!strValue.IsEmpty() && !cFieldDef.m_setErrRange.empty()){
		// エラー値範囲チェック
		if(checkRangeValue(cFieldDef, cFieldDef.m_setErrRange, strValue, false)){
			CLog::GetInstance().PrintLog2(false, true, true, true, err_type::error, err_code::NgCheckErrRange, cConvTableInfo.m_oidTableName, getID(cConvTableInfo, ipRow), cFieldDef.m_srcTableName, cFieldDef.m_srcFieldName, strValue);
			bReturn = false;
		}
	}

	// NULLチェック
	if(!checkNull(ipRow, cConvTableInfo, cFieldDef, cFieldDef.m_canNull, strValue)){
		bReturn = false;
	}
	// 列幅チェック
	if(!checkLength(ipRow, cConvTableInfo, cFieldDef, strValue)){
		bReturn = false;
	}
	return bReturn;
}

bool CCheckString::checkNull(const _IRowPtr& ipRow, const CConvTableInfo& cConvTableInfo, const CFieldDefInfo& cFieldDef, bool canNull, const CString& strValue)
{
	if(!canNull){
		if(strValue.IsEmpty()){
			CLog::GetInstance().PrintLog2(false, true, true, true, err_type::error, err_code::NgCheckNull, cConvTableInfo.m_oidTableName, getID(cConvTableInfo, ipRow), cFieldDef.m_srcTableName, cFieldDef.m_srcFieldName, strValue);
			return false;
		}
	}
	return true;
}

bool CCheckString::checkLength(const _IRowPtr& ipRow, const CConvTableInfo& cConvTableInfo, const CFieldDefInfo& cFieldDef, const CString& strValue)
{
	switch(cFieldDef.m_FieldType){
	case FIELDTYPE::INTEGER:
		{
			// マイナス値の場合は桁数+1する
			long number = cFieldDef.m_Number;
			if(0 == strValue.Left(1).CompareNoCase(NUMBER_MINUS)){
				++number;
			}
			if(strValue.GetLength() > number){
				CLog::GetInstance().PrintLog2(false, true, true, true, err_type::error, err_code::NgCheckIntegerDigitOver, cConvTableInfo.m_oidTableName, getID(cConvTableInfo, ipRow), cFieldDef.m_srcTableName, cFieldDef.m_srcFieldName, strValue);
				return false;
			}
		}
		break;
	case FIELDTYPE::STRING:
		if(strValue.GetLength() > cFieldDef.m_Length){
			CLog::GetInstance().PrintLog2(false, true, true, true, err_type::error, err_code::NgCheckStringLength, cConvTableInfo.m_srcTableName, getID(cConvTableInfo, ipRow), cFieldDef.m_srcTableName, cFieldDef.m_srcFieldName, strValue);
			return false;
		}
		break;
	case FIELDTYPE::DOUBLE:
		{
			// マイナス値の場合は桁数+1する
			long number = cFieldDef.m_Number;
			if(0 == strValue.Left(1).CompareNoCase(NUMBER_MINUS)){
				++number;
			}
			// 整数部のチェックのみ（小数部は文字列にする段階で切り捨てている）
			if(strValue.Find(_T(".")) > number){
				CLog::GetInstance().PrintLog2(false, true, true, true, err_type::error, err_code::NgCheckIntegerDigitOver, cConvTableInfo.m_oidTableName, getID(cConvTableInfo, ipRow), cFieldDef.m_srcTableName, cFieldDef.m_srcFieldName, strValue);
				return false;
			}
		}
		break;
	default:
		// ここにはこねえ
		CLog::GetInstance().PrintLog2(false, true, true, true, err_type::error, err_code::NgCheckFieldType, cConvTableInfo.m_oidTableName, getID(cConvTableInfo, ipRow), cFieldDef.m_srcTableName, cFieldDef.m_srcFieldName, strValue);
		return false;
		break;
	}
	return true;
}

bool CCheckString::checkValue(const std::set<CString>& setCheckValue, const CString& strValue, bool bValid)
{
	const auto& checkValue = setCheckValue.find(strValue);
	if(setCheckValue.cend() != checkValue){
		return true;
	}
	return false;
}

bool CCheckString::checkRangeValue(const CFieldDefInfo& cFieldDef, const std::set<CRange>& setRange, const CString& strValue, bool bValid)
{
	for(const auto& validRange : setRange){
		bool bCheck1 = false, bCheck2 = false;
		if(0 == validRange.m_strSymbol1.CompareNoCase(SY_GREATER_EQ)){
			if(cFieldDef.m_FieldType == FIELDTYPE::INTEGER){
				if(_ttol(strValue) >= _ttol(validRange.m_strValue1)){
					bCheck1 = true;
				}
			} else if(cFieldDef.m_FieldType == FIELDTYPE::DOUBLE){
				if(_ttof(strValue) >= _ttof(validRange.m_strValue1)){
					bCheck1 = true;
				}
			}
		} else if(0 == validRange.m_strSymbol1.CompareNoCase(SY_GREATER)){
			if(cFieldDef.m_FieldType == FIELDTYPE::INTEGER){
				if(_ttol(strValue) > _ttol(validRange.m_strValue1)){
					bCheck1 = true;
				}
			} else if(cFieldDef.m_FieldType == FIELDTYPE::DOUBLE){
				if(_ttof(strValue) > _ttof(validRange.m_strValue1)){
					bCheck1 = true;
				}
			}
		} else if(0 == validRange.m_strSymbol1.CompareNoCase(SY_LESS_EQ)){
			if(cFieldDef.m_FieldType == FIELDTYPE::INTEGER){
				if(_ttol(strValue) <= _ttol(validRange.m_strValue1)){
					bCheck1 = true;
				}
			} else if(cFieldDef.m_FieldType == FIELDTYPE::DOUBLE){
				if(_ttof(strValue) <= _ttof(validRange.m_strValue1)){
					bCheck1 = true;
				}
			}
		} else if(0 == validRange.m_strSymbol1.CompareNoCase(SY_LESS)){
			if(cFieldDef.m_FieldType == FIELDTYPE::INTEGER){
				if(_ttol(strValue) < _ttol(validRange.m_strValue1)){
					bCheck1 = true;
				}
			} else if(cFieldDef.m_FieldType == FIELDTYPE::DOUBLE){
				if(_ttof(strValue) < _ttof(validRange.m_strValue1)){
					bCheck1 = true;
				}
			}
		}

		if(!validRange.m_strSymbol2.IsEmpty()){
			if(0 == validRange.m_strSymbol2.CompareNoCase(SY_LESS_EQ)){
				if(cFieldDef.m_FieldType == FIELDTYPE::INTEGER){
					if(_ttol(strValue) <= _ttol(validRange.m_strValue2)){
						bCheck2 = true;
					}
				} else if(cFieldDef.m_FieldType == FIELDTYPE::DOUBLE){
					if(_ttof(strValue) <= _ttof(validRange.m_strValue2)){
						bCheck2 = true;
					}
				}
			} else if(0 == validRange.m_strSymbol2.CompareNoCase(SY_LESS)){
				if(cFieldDef.m_FieldType == FIELDTYPE::INTEGER){
					if(_ttol(strValue) < _ttol(validRange.m_strValue2)){
						bCheck2 = true;
					}
				} else if(cFieldDef.m_FieldType == FIELDTYPE::DOUBLE){
					if(_ttof(strValue) < _ttof(validRange.m_strValue2)){
						bCheck2 = true;
					}
				}
			}
			if(bCheck1 && bCheck2){
				return true;
			}
		} else {
			if(bCheck1){
				return true;
			}
		}
	}
	return false;
}
