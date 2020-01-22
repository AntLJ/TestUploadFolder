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
#include "GridField.h"


long GridField::getCol(const CString & field_name) const
{
	auto iter = find_if(begin(), end(),
		[&field_name](const FieldGridCol& rec)
	{
		return rec.m_field_name == field_name;
	}
	);

	if (iter == end())
		return -1;

	return iter->m_col;
}

const CString& GridField::getFieldName(int col) const
{
	auto iter = find_if(begin(), end(),
		[&col](const FieldGridCol& rec)
	{
		return rec.m_col == col;
	}
	);

	if (iter == end())
		return _T("");

	return iter->m_field_name;
}
