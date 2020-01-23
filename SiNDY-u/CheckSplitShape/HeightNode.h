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

#include "LayerBase.h"

class CHeightNode : public CRecodeBase {
public:
	CHeightNode(){}
	~CHeightNode(){}
};

class CHeightNodeAll: public CTableBase {
public:
	CHeightNodeAll(){}
	~CHeightNodeAll(){}

public:
	bool Init(IFeatureClassPtr ipFeatureClass, IFeatureClassPtr ipMeshClass, const CString& strMeshCode, bool bJGD2000);

	virtual bool LoadData();
	
private:

public:
	bool m_bJGD2000;                            //!< JGD2000化フラグ（true:JGD2000化する, false;しない）
	CString m_strMeshCode;                      //!< メッシュコード
	IFeatureClassPtr m_ipMeshClass;             //!< メッシュクラス
	std::map<UNIQID,CHeightNode> m_mapData;     //!< OBJECTID単位のデータ情報
	std::map<LONLAT,UNIQID> m_mapLonLat2OID;    //!< 緯度経度でデータユニーク
	std::set<long> m_setLinkID;                 //!< 高さノードで使用したリンクID
};
