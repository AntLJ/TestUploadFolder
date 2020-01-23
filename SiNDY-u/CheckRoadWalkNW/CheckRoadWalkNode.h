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
#include "DatabaseController.h"
#include "GeomProc.h"

#include "Properties.h"
#include "Log.h"

using namespace std;

class CCheckRoadWalkNode
{
protected:
	std::vector<CString> m_errorMessageList;
	CGeomProc m_geomProcess;
 
public:
	CCheckRoadWalkNode(void);
	~CCheckRoadWalkNode(void);

	/**
	 * @brief	RWNフォーマットチェック
	 * @param	ipFeatureClass	[in]	チェック対象のフィーチャークラス
	 * @param	fieldIndex	[in]	チェック対象のフィールドインデックス
	 * @param	dbCtrl	[in]	比較対象のDB情報
	 * @return	bool
	 */
	virtual bool checkFormat(const IFeatureClassPtr ipFeatureClass, FIELD_INDEX fieldIndex, CDatabaseController dbCtrl);

	/**
	 * @brief	エラーメッセージリストの取得
	 * @return	std::vector<CString>	エラーメッセージリスト
	 */
	virtual std::vector<CString> getMeessageList();
	
	virtual void clear(void);

protected:
	/**
	 * @brief	指定されたフィーチャーから値の取得
	 * @note	取得に失敗した場合、エラーメッセージを追加する
	 * @param	objectID	[in]	オブジェクトID
	 * @param	ipFeature	[in]	取得対象のフィーチャー
	 * @param	fieldName	[in]	取得対象のフィールド名
	 * @param	fieldIndex	[in]	取得対象のフィールドインデックス
	 * @param	value	[out]	比較した値
	 * @return	bool
	 */
	virtual bool getFeatureValue(long objectID, const IFeaturePtr ipFeature,CString fieldName, FIELD_INDEX fieldIndex, CComVariant& value); 
	
	/**
	 * @brief	道路：DB比較、歩行者：固定値比較、またはその逆のチェック
	 * @param	objectID	[in]	オブジェクトID
	 * @param	ipFeature	[in]	チェック対象のフィーチャー
	 * @param	fieldName	[in]	チェック対象のフィールド名
	 * @param	fieldIndex	[in]	チェック対象のフィールドインデックス
	 * @param	ipDbFeature	[in]	比較先のフィーチャー
	 * @param	dbFieldName	[in]	比較先のフィールド名
	 * @param	dbFieldIndex	[in]	比較先のフィールドインデックス
	 * @param	walkIsFixedValue	[in]	固定値比較の対象が歩行者か否か
	 * @param	fixedValue	[in]	比較する固定値
	 * @return	bool
	 */
	virtual bool checkDBValueFixedValue(long objectID, 
										const IFeaturePtr ipFeature, CString fieldName, FIELD_INDEX fieldIndex,
										const IFeaturePtr ipDbFeature, CString dbFieldName, FIELD_INDEX dbFieldIndex,
										bool walkIsFixedValue, long fixedValue);

	/**
	 * @brief	SHAPEの値とDBの値の同一判定
	 * @note	DBの値がNULLであった場合、SHAPEの型から" "または-1と同じであるか比較する
	 * @param	shapeValue	[in]	SHAPEの値
	 * @param	dbValue	[in]	DBの値
	 * @return	bool
	 */
	virtual bool compShapeDbValue(CComVariant shapeValue, CComVariant dbValue);

	/**
	 * @brief	形状一致チェック（ポイント）
	 * @param	objectID	[in]	オブジェクトID
	 * @param	ipShapeFeature	[in]	SHAPEのフィーチャー
	 * @param	ipDBFeature	[in]	DBのフィーチャー
	 * @return	bool
	 */
	virtual bool checkGeomPoint(long objectID, const IFeaturePtr ipShapeFeature, const IFeaturePtr ipDBFeature);

	/**
	 * @brief	形状一致チェック（複数ポイント）
	 * @param	objectID	[in]	オブジェクトID
	 * @param	ipShapeFeature	[in]	SHAPEのフィーチャー
	 * @param	ipDBFeature	[in]	DBのフィーチャー
	 * @return	bool
	 */
	virtual bool checkGeomPoints(long objectID, const IFeaturePtr ipShapeFeature, const IFeaturePtr ipDBFeature);


	/**
	 * @brief	エラーメッセージの作成と追加
	 * @param	objectID	[in]	オブジェクトID
	 * @param	fieldName	[in]	フィールド名
	 * @param	message	[in]	エラーメッセージ
	 * @param	value	[in]	エラー対象の値
	 * @param	compValue	[in]	比較対象の値
	 */
	virtual void pushColumnErrorMessage(long objectID,CString filedName, CString message, CComVariant value=NULL, CComVariant compValue=NULL);

	/**
	 * @brief	エラーメッセージの追加
	 * @param	message	[in]	エラーメッセージ
	 */
	virtual void pushErrorMessage(CString message);
};
