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
	bool m_bJGD2000;                            //!< JGD2000���t���O�itrue:JGD2000������, false;���Ȃ��j
	CString m_strMeshCode;                      //!< ���b�V���R�[�h
	IFeatureClassPtr m_ipMeshClass;             //!< ���b�V���N���X
	std::map<UNIQID,CHeightNode> m_mapData;     //!< OBJECTID�P�ʂ̃f�[�^���
	std::map<LONLAT,UNIQID> m_mapLonLat2OID;    //!< �ܓx�o�x�Ńf�[�^���j�[�N
	std::set<long> m_setLinkID;                 //!< �����m�[�h�Ŏg�p���������NID
};
