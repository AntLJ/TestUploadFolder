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

#pragma once

class CParamCtrl
{
public:
	CParamCtrl(void);
	~CParamCtrl(void);

	void SetAllowOffset(double dOffset) { m_dAllowOffset = dOffset; };
	double GetAllowOffset() const { return m_dAllowOffset; };
	void SetDB(const CString& str) { m_strDB = str; };
	const CString& GetDB() const { return m_strDB; };
	void SetFeatureClassName(const CString& str) { m_strFeatureClass = str; };
	const CString& GetFeatureClassName() const { return m_strFeatureClass; };

private:
	double m_dAllowOffset;
	CString m_strDB;
	CString m_strFeatureClass;
};
