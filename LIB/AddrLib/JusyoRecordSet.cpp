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

#include "StdAfx.h"
#include "JusyoRecordSet.h"
#include <SiNDYLib/util.h>

namespace addr
{
	using namespace sindy;

	// ���R�[�h�Z�b�g
	void CJusyoRecordSet::SetRecordSet(const _RecordsetPtr& recordset, const _TCHAR* lpcszSQL)
	{
		if (recordset && recordset->State == adStateOpen)
		{
			m_recordset = recordset;
			m_strSQL = lpcszSQL;
		}
	}

	// �f�[�^�擾(_variant_t)
	bool CJusyoRecordSet::GetValue(long lIndex, _variant_t& pVal) const
	{
		if (!m_recordset || m_recordset->State != adStateOpen)
			return false;

		auto fields = m_recordset->ADOFields;
		if (!fields)
			return false;

		auto item = fields->Item[lIndex];
		if (!item)
			return false;

		pVal = item->Value;
		return true;
	}

	// �f�[�^�擾(_variant_t)
	bool CJusyoRecordSet::GetValue(const _TCHAR* lpcszField, _variant_t& pVal) const
	{
		if (!m_recordset || m_recordset->State != adStateOpen)
			return false;

		auto fields = m_recordset->ADOFields;
		if (!fields)
			return false;

		auto item = fields->Item[lpcszField];
		if (!item)
			return false;

		pVal = item->Value;
		return true;
	}

	// �l�擾(long)
	long CJusyoRecordSet::GetLongValue(long lIndex)
	{
		_variant_t val;
		if (GetValue(lIndex, val))
		{
			return (val.vt == VT_EMPTY || val.vt == VT_NULL) ? -1 : (long)val;
		}
		else
		{
			SASSERT(false);	// �l�擾���s
			return -1;
		}
	}

	// �l�擾(long)
	long CJusyoRecordSet::GetLongValue(const _TCHAR* lpcszField)
	{
		_variant_t val;
		if (GetValue(lpcszField, val))
		{
			// _variant_t�͂���ŏ�肭�ϊ����Ă����
			return (val.vt == VT_EMPTY || val.vt == VT_NULL) ? -1 : (long)val;
		}
		else
		{
			SASSERT(false);	// �l�擾���s
			return -1;
		}
	}

	// �l�擾(double)
	double CJusyoRecordSet::GetDoubleValue(long lIndex)
	{
		_variant_t val;
		if (GetValue(lIndex, val))
		{
			return (val.vt == VT_EMPTY || val.vt == VT_NULL) ? -1 : (double)val;
		}
		else
		{
			SASSERT(false);	// �l�擾���s
			return -1;
		}
	}

	// �l�擾(double)
	double CJusyoRecordSet::GetDoubleValue(const _TCHAR* lpcszField)
	{
		_variant_t val;
		if (GetValue(lpcszField, val))
		{
			return (val.vt == VT_EMPTY || val.vt == VT_NULL) ? -1 : (double)val;
		}
		else
		{
			SASSERT(false);	// �l�擾���s
			return -1;
		}
	}


	// �l�擾(char)
	CString CJusyoRecordSet::GetStringValue(long lIndex)
	{
		_variant_t val;
		if (GetValue(lIndex, val))
		{
			return (val.vt == VT_EMPTY || val.vt == VT_NULL) ? CString() : CString(val);
		}
		else
		{
			SASSERT(false);	// �l�擾���s
			return CString();
		}
	}

	// �l�擾(char)
	CString CJusyoRecordSet::GetStringValue(const _TCHAR* lpcszField)
	{
		_variant_t val;
		if (GetValue(lpcszField, val))
		{
			return (val.vt == VT_EMPTY || val.vt == VT_NULL) ? CString() : CString(val);
		}
		else
		{
			return CString();
		}
	}

}	// namespace addr
