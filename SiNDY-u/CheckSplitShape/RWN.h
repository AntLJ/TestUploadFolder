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
	std::map<UNIQID,CRWNDiv> m_mapData;             //!< DIVID�P�ʂ̃f�[�^���
	std::map<long,std::set<long>> m_mapDivOID;      //!< �I�u�W�F�N�gID�ɏ�������DIVID�̃O���[�v
	std::multimap<LONLAT,UNIQID> m_mapLonLat2OID;   //!< �ܓx�o�x�Ńf�[�^���j�[�N�i�����N�����������ŐV�K�쐬���ꂽ���H�m�[�h�Ɗ������s�҃m�[�h���d������P�[�X�����邽��multimap�j
};
