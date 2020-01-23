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

/**
 *	@file	shputil.cpp
 *	@brief	
 *	@author	F.Abe
 *	@date	2004/10/03
 */


#include "stdafx.h"
#include "shputil.h"

long DBFGetFieldIndex(CDBFHandle *cHandle, const char *cFieldName)
{
	if (! cHandle)
		return -1;
	long nFieldCount = cHandle->get_FieldCount();
	for (long i = 0; i < nFieldCount; i++) {
		char *fname = 0;
		int width = 0, decimals = 0;
		cHandle->get_FieldInfo(i, &fname, &width, &decimals);
		if (! strcmp(cFieldName, fname))
			return i;
	}
	return -1;
}

bool DBFGetAttributes(CDBFHandle *cHandle, DBFAttributes *cAttributes, const char *cFieldName)
{
	// チェック
	if (! cHandle)
		return false;

	if (! cAttributes)
		return false;

	if (! cFieldName)
		return false;

	// レコード数の取得
	long nRecordCount = cHandle->get_RecordCount();

	// フィールド数のカウント
	long nFieldCount = cHandle->get_FieldCount();

	// レコードの取得
	for (long i = 0; i < nRecordCount; i++) {

		std::vector<VARIANT> vecAttr;
		long nOID = 0;

		for (long j = 0; j < nFieldCount; j++) {
			char *name = 0;
			int width = 0, decimals = 0;
			_variant_t value;
			switch (cHandle->get_FieldInfo(j, &name, &width, &decimals)) {
			case FTInteger:
				{
					int attr;
					if (! cHandle->read_Attribute(i, j, &attr))
						return false;
					value.vt = VT_I4;
					value.lVal = attr;
				}
				break;
			case FTString:
				{
					const char *attr = 0;
					if (! cHandle->read_Attribute(i, j, &attr))
						return false;
					value.vt = VT_BSTR;
					value.bstrVal = ::SysAllocString(_bstr_t(_T(attr)));
				}
				break;
			case FTDouble:
				{
					double attr = 0.0;
					if (! cHandle->read_Attribute(i, j, &attr))
						return false;
					value.vt = VT_R8;
					value.dblVal = attr;
				}
				break;
			default:
				break;
			}
			if (! strcmp(name, cFieldName)) {
				value.ChangeType(VT_I4);
				nOID = value.lVal;
			}
			vecAttr.push_back(value);
		}
		cAttributes->insert(std::pair<long, std::vector<VARIANT> >(nOID, vecAttr));
	}

	return true;
}

template <class T>
bool __SHPGetEntities(CSHPHandle *cShpHandle, CDBFHandle *cDbfHandle, T *cEntities, const char *cFieldName)
{
	// チェック
	if (! cShpHandle || ! cDbfHandle)
		return false;

	if (! cEntities)
		return false;

	if (! cFieldName)
		return false;

	// ファイル情報の取得
	int nRecordCount = 0, nShpType = 0;
	double dMinbound[4], dMaxbound[4];
	if (! cShpHandle->get_FileInfo(&nRecordCount, &nShpType, dMinbound, dMaxbound))
		return false;

	long nFieldCount = cDbfHandle->get_FieldCount();

	// 実体の取得
	for (long i = 0; i < nRecordCount; i++) {
		CSHPObject aObject;
		long nShpId = cShpHandle->read_Object(i, &aObject);
		if (nShpId < 0)
			return false;

		std::vector<VARIANT> vecAttr;
		long nOID = 0;

		for (long j = 0; j < nFieldCount; j++) {
			char *name = 0;
			int width = 0, decimals = 0;
			_variant_t value;
			switch (cDbfHandle->get_FieldInfo(j, &name, &width, &decimals)) {
			case FTInteger:
				{
					int attr;
					if (! cDbfHandle->read_Attribute(i, j, &attr))
						return false;
					value.vt = VT_I4;
					value.lVal = attr;
				}
				break;
			case FTString:
				{
					const char *attr = 0;
					if (! cDbfHandle->read_Attribute(i, j, &attr))
						return false;
					value.vt = VT_BSTR;
					value.bstrVal = ::SysAllocString(_bstr_t(_T(attr)));
				}
				break;
			case FTDouble:
				{
					double attr = 0.0;
					if (! cDbfHandle->read_Attribute(i, j, &attr))
						return false;
					value.vt = VT_R8;
					value.dblVal = attr;
				}
				break;
			default:
				break;
			}
			if (! strcmp(name, cFieldName)) {
				value.ChangeType(VT_I4);
				nOID = value.lVal;
			}
			vecAttr.push_back(value);
		}

		cEntities->insert(std::pair<long, ShpEntity>(nOID, ShpEntity(aObject, vecAttr)));
	}

	return true;
}

bool SHPGetEntities(CSHPHandle *cShpHandle, CDBFHandle *cDbfHandle, SHPEntities *cEntities, const char *cFieldName)
{
	return __SHPGetEntities<SHPEntities>(cShpHandle, cDbfHandle, cEntities, cFieldName);
}

bool SHPGetEntities2(CSHPHandle *cShpHandle, CDBFHandle *cDbfHandle, SHPEntities2 *cEntities, const char *cFieldName)
{
	return __SHPGetEntities<SHPEntities2>(cShpHandle, cDbfHandle, cEntities, cFieldName);
}
