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

class CRWNDiv : public CRecodeBase {
public:
	CRWNDiv(){}
	~CRWNDiv(){}
public:
	virtual bool SetData(const CString& strTableName, IFeaturePtr ipFeature, const std::map<long,CString>& mapFieldIndex2Name, const std::map<CString,long>& mapFieldName2Index, const std::map<CString,long>& mapAttrName2Index);
};

class CRWNAll: public CTableBase {
public:
	CRWNAll(){}
	~CRWNAll(){}

public:
	virtual bool LoadData();

private:

public:
	std::map<UNIQID,CRWNDiv> m_mapData;             //!< DIVID単位のデータ情報
	std::map<long,std::set<long>> m_mapDivOID;      //!< オブジェクトIDに所属するDIVIDのグループ
	std::multimap<LONLAT,UNIQID> m_mapLonLat2OID;   //!< 緯度経度でデータユニーク（リンク内属性分割で新規作成された道路ノードと既存歩行者ノードが重複するケースがあるためmultimap）
};
