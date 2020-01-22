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

	void SetDB(const CString& str) { m_strDB = str; };
	const CString& GetDB() const { return m_strDB; };
	void SetFeatureClass(const CString& str) { m_strFC = str; };
	const CString& GetFeatureClass() const { return m_strFC; };
	void SetClearance(bool bChk) { m_bClearance = bChk; };
	bool GetClearance() const { return m_bClearance; };
	void SetMultiP(bool bChk) { m_bMultiP = bChk; };
	bool GetMultiP() const { return m_bMultiP; };

private:
	CString m_strDB;
	CString m_strFC;
	bool m_bClearance;
	bool m_bMultiP;
};
