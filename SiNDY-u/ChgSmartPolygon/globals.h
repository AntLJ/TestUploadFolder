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

class CSmartPath
{
public:
	CSmartPath() : m_bUseReverse( false ), m_bGeneralize( true ), m_lPairOID( 0 ) {};
	CSmartPath(long lOID, IPathPtr ipPath) : m_bUseReverse( false ), m_bGeneralize( true ) { m_lPairOID = lOID; m_ipPath = ipPath; };

	void SetOID(long lOID) { m_lPairOID = lOID; };
	long GetOID() { return m_lPairOID; };
	void SetPath(IPathPtr ipPath) { m_ipPath = ipPath; };
	IPathPtr GetPath() const { return m_ipPath; };
	void SetGeneralize(bool bGeneralize) { m_bGeneralize = bGeneralize; };
	bool GetGeneralize() { return m_bGeneralize; };
	void SetUseReverse(bool bReverse) { m_bUseReverse = bReverse; };
	bool GetUseReverse() { return m_bUseReverse; };

private:
	bool m_bUseReverse;			// 間引き後、リバースする必要あるかどうか
	bool m_bGeneralize;			// 間引きを行うかどうか
	long m_lPairOID;			// 相手先のOID
	IPathPtr m_ipPath;
};

IFeatureClassPtr GetFeatureClass(const CString& strDir, const CString& strFileName, IWorkspacePtr& ipWorkspace);
void OutPutTime(std::string str);
bool CheckCrossMyself(const IGeometryPtr& ipGeom);
