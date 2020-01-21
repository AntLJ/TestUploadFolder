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

// SiNDYMapTableFinder.h: SiNDYMapTableFinder クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SINDYMAPTABLEFINDER_H__E4F17499_4C90_4BBB_BCD9_0FD942D01140__INCLUDED_)
#define AFX_SINDYMAPTABLEFINDER_H__E4F17499_4C90_4BBB_BCD9_0FD942D01140__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SiNDYTableFinder.h"

class SiNDYMapTableFinder : public SiNDYTableFinder
{
public:
	typedef std::map<CString, ILayerPtr> AutoComplete;

// 構築/消滅
	SiNDYMapTableFinder();
	SiNDYMapTableFinder(IMapPtr ipMap);

// 設定/取得
	void setMap(IMapPtr ipMap);
	void setPair(CString strTableName, ILayer* ipLayer);
	ILayerPtr getPairLayer(CString strTableName) const;

	IMapPtr getMap() const { return m_ipMap; }

	virtual ITablePtr getTable(LPCTSTR lpszTableName);

	IFeatureClassPtr findFeatureClass(CString strTableName) const;

	void queryLayerSet(const CString& rLayerName, _ISet* ipLayerSet) const;
	void queryLayerArray(const CString& rLayerName, IArray* ipLayerArray) const;

	static void queryLayerSet(IMxDocument* ipMxDocument, const CString& rLayerName, _ISet* ipLayerSet);
	static void queryLayerSet(IMap* ipMap, const CString& rLayerName, _ISet* ipLayerSet);
	static void queryLayerSet(ILayer* ipLayer, const CString& rLayerName, _ISet* ipLayerSet);

	static void queryLayerArray(IMxDocument* ipMxDocument, const CString& rLayerName, IArray* ipLayerArray);
	static void queryLayerArray(IMap* ipMap, const CString& rLayerName, IArray* ipLayerArray);
	static void queryLayerArray(ILayer* ipLayer, const CString& rLayerName, IArray* ipLayerArray);

private:
	ITablePtr findTable(LPCTSTR lpszTableName) const;
	void getTableList(LPCTSTR lpszTableName, _ISet* ipLayerSet) const;
	void getTableList(LPCTSTR lpszTableName, IArray* ipLayerArray) const;

	IMapPtr m_ipMap;
	AutoComplete m_cAutoComplete;
};

#endif // !defined(AFX_SINDYMAPTABLEFINDER_H__E4F17499_4C90_4BBB_BCD9_0FD942D01140__INCLUDED_)
