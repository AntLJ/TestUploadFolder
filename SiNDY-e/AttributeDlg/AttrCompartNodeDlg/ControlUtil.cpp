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
#include "ControlUtil.h"

namespace control_util
{
	void ResetComboBox(HWND wnd)
	{
		CComboBox cCombo(wnd);
		cCombo.ResetContent();
	}

	void AddtoComboBox(HWND wnd, const CString & str)
	{
		CComboBox cCombo(wnd);
		INT curPos = cCombo.AddString(str);
		cCombo.SetItemData(curPos, curPos);
	}

	CString GetComboBoxText(HWND wnd)
	{
		CComboBox cCombo(wnd);
		int idx = cCombo.GetCurSel();
		TCHAR* str = _T("");
		cCombo.GetLBText(idx, str);

		return CString(str);
	}

	long GetCellVal(IVSFlexGridPtr grid, int rowNum, int colNum)
	{
		CComBSTR vaStr;
		if (FAILED(grid->get_TextMatrix(rowNum, colNum, &vaStr)))
			return -1;

		return CString(vaStr).IsEmpty() ? -1 : _ttol(vaStr);
	}

	CString GetCellString(IVSFlexGridPtr grid, int rowNum, int colNum)
	{
		CComBSTR vaStr;
		if (FAILED(grid->get_TextMatrix(rowNum, colNum, &vaStr)))
			return CString();

		return CString(vaStr);
	}

	bool GetCellChecked(IVSFlexGridPtr grid, int rowNum, int colNum)
	{
		if (FAILED(grid->Select(rowNum, colNum)))
			return false;

		auto checked = grid->GetCellChecked();
		return (checked == flexChecked || checked == flexTSChecked);
	}

	DataTypeSettings GetGridDataType(esriFieldType field_type)
	{
		switch (field_type)
		{
		case esriFieldTypeSmallInteger:
			return flexDTShort;
			break;
		case esriFieldTypeInteger:
		case esriFieldTypeOID:
			return flexDTLong;
			break;
		case esriFieldTypeSingle:
			return flexDTSingle;
			break;
		case esriFieldTypeDouble:
			return flexDTDouble;
			break;
		case esriFieldTypeString:
		case esriFieldTypeGUID:
		case esriFieldTypeGlobalID:
			return flexDTString;
			break;
		case esriFieldTypeDate:
			return flexDTDate;
			break;
		case esriFieldTypeGeometry:
		case esriFieldTypeBlob:
		case esriFieldTypeRaster:
		case esriFieldTypeXML:
			break;
		default:
			break;
		}

		return flexDTEmpty;
	}
}
