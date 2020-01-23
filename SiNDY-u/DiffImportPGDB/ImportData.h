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

class CImportData
{
public:
	CImportData(void);
	~CImportData(void);

	bool IsSameAttribute( CImportData& data )
	{
		return this->m_mapFieldValue == data.m_mapFieldValue;
	}

	bool UnionGeometry ( const IFeaturePtr& ipFeatuer );

public:
	std::map<CString, CComVariant> m_mapFieldValue; //!< 比較用属性値格納変数
	IFeaturePtr m_feature; //!< インポート対象のフィーチャ
	long m_updateStatus; //!< インポート対象のUPDATE_Cの値
};

