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

#include "stdafx.h"
#include "CheckRoadWalkNode.h"

class CCheckRoadWalkLink : public CCheckRoadWalkNode
{
public:
	CCheckRoadWalkLink(void);
	~CCheckRoadWalkLink(void);


	/**
	 * @brief	RWLフォーマットチェック
	 * @param	ipFeatureClass	[in]	チェック対象のフィーチャークラス
	 * @param	fieldIndex	[in]	チェック対象のフィールドインデックス
	 * @param	dbCtrl	[in]	比較対象のDB情報
	 * @return	bool
	 */
	virtual bool checkFormat(const IFeatureClassPtr ipFeatureClass, FIELD_INDEX fieldIndex, CDatabaseController dbCtrl);	

protected:
	enum walkAttrType 
	{
		eAUTOWALK=1, 
		eSTAIR=2, 
		eSLOPE=4, 
		eESCALATOR=8, 
		eARCADE=16, 
		eTUNNEL=32,
		eOPENAREA=64
	};

	/**
	 * @brief	NodeID用比較チェック
	 * @param	objectID	[in]	オブジェクトID
	 * @param	ipFeature	[in]	チェック対象のフィーチャー
	 * @param	fieldName	[in]	チェック対象のフィールド名
	 * @param	fieldIndex	[in]	チェック対象のフィールドインデックス
	 * @param	ipDbFeature	[in]	比較対象のフィーチャー
	 * @param	dbWalkfieldName	[in]	比較対象の歩行者フィールド名
	 * @param	dbRoadfieldName	[in]	比較対象の道路フィールド名
	 * @param	dbFieldIndex	[in]	比較対象のフィールドインデックス
	 * @return	bool
	 */
	virtual bool checkNodeID(long objectID, const IFeaturePtr ipFeature,CString fieldName, FIELD_INDEX fieldIndex,const IFeaturePtr ipDbFeature, CString dbWalkFieldName, CString dbRoadFieldName, FIELD_INDEX dbFieldIndex);

	/**
	 * @brief	DB比較チェック
	 * @param	objectID	[in]	オブジェクトID
	 * @param	ipFeature	[in]	チェック対象のフィーチャー
	 * @param	fieldName	[in]	チェック対象のフィールド名
	 * @param	fieldIndex	[in]	チェック対象のフィールドインデックス
	 * @param	ipDbFeature	[in]	比較対象のフィーチャー
	 * @param	dbWalkfieldName	[in]	比較対象の歩行者フィールド名
	 * @param	dbRoadfieldName	[in]	比較対象の道路フィールド名
	 * @param	dbFieldIndex	[in]	比較対象のフィールドインデックス
	 * @return	bool
	 */
	virtual bool checkDBValue(long objectID, const IFeaturePtr ipFeature,CString fieldName, FIELD_INDEX fieldIndex,const IFeaturePtr ipDbFeature, CString dbWalkFieldName, CString dbRoadFieldName,  FIELD_INDEX dbFieldIndex);
	
	/**
	 * @brief	歩行者属性作成
	 * @param	objectID	[in]	オブジェクトID
	 * @param	ipDbFeature	[in]	作成元のDBのフィーチャー
	 * @param	dbFieldName	[in]	作成元のDBのフィールド名
	 * @param	dbFieldIndex	[in]	作成元のDBのフィールドインデックス
	 * @param	walkAttrType	[in]	作成する歩行者属性の種類
	 * @param	walkAttrValue	[out]	作成した歩行者属性の値
	 * @return	bool
	 */
	virtual bool getWalkAttr(long objectID, const IFeaturePtr ipDbFeature, CString dbFieldName, FIELD_INDEX dbFieldIndex, walkAttrType walkAttrType, int& walkAttrValue);
}
;
