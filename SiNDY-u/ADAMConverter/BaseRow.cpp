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
#include "BaseRow.h"

CString CBaseRow::getID(const CConvTableInfo& cConvTableInfo, const _IRowPtr& ipRow)
{
	CString strOID;
	if(cConvTableInfo.m_oidField.IsEmpty()){
		long lOID = 0;
		ipRow->get_OID(&lOID);
		strOID.Format(_T("%ld"), lOID);
	} else {
		IFieldsPtr ipFields;
		ipRow->get_Fields(&ipFields);
		long index = -1;
		if(cConvTableInfo.m_subTableNames.IsEmpty()){
			CString strTmp = cConvTableInfo.m_oidField;
			strTmp = strTmp.Mid(strTmp.Find(_T(".")) + 1);
			ipFields->FindField(CComBSTR(strTmp), &index);
		} else {
			ipFields->FindField(CComBSTR(cConvTableInfo.m_oidField), &index);
		}
		CComVariant vaValue;
		ipRow->get_Value(index, &vaValue);
		vaValue.ChangeType(VT_BSTR);
		strOID.Format(_T("%s"), vaValue.bstrVal);
	}
	return strOID;
}
