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

/**
 * @file	Addr2TsvBase.h
 */

// 本クラスを使用するには以下のincludeが必要
// 可能な限り.hでのincludeは避けるべきだが、ほぼ全ての.cppに影響があるため本ヘッダでincludeしてしまう
#include <crd_cnv/coord_converter.h>

/**
 * @class	CAddr2TsvBase
 * @brief	TSV出力ベースクラス
 */
class CAddr2TsvBase abstract
{
public:
	CAddr2TsvBase(void);
	~CAddr2TsvBase(void);

	/**
	* @brief	初期化
	* @retval	true	成功
	* @retval	false	失敗
	*/
	virtual bool init(void);

	/**
	* @brief	処理実行
	* @param	bDispProgress	[in]	進捗率出力有無
	* @param	strWhere		[in]	検索条件
	* @retval	true	成功
	* @retval	false	失敗
	*/
	bool execute(const bool bDispProgress = true, const CString& strWhere = NULL);

protected:
	/**
	* @brief	レコード変換
	* @note		純粋仮想関数（継承先で実装が必要）
	* @param	ipRow	[in]	レコード
	* @retval	true	成功
	* @retval	false	失敗
	*/
	virtual bool convRecord(const _IRowPtr& ipRow) = 0;

	/**
	* @brief	フィールドインデックス取得
	* @param	fieldIndex		[in]	フィールドINDEX
	* @param	strFieldName	[in]	フィールド名
	* @param	index			[out]	取得したインデックス
	* @retval	true	成功
	* @retval	false	失敗
	*/
	bool getFieldIndex(const FIELD_INDEX& fieldIndex, const CString& strFieldName, long& index) const;

	/**
	* @brief	値取得
	* @param	ipRow			[in]	レコード
	* @param	strFieldName	[in]	フィールド名
	* @param	index			[in]	インデックス
	* @param	strVal			[out]	取得した値
	* @retval	true	成功
	* @retval	false	失敗
	*/
	bool getValue(const _IRowPtr& ipRow, const CString& strFieldName, const long& index, CString& strVal) const;

	/**
	* @brief	2次メッシュXY取得
	* @param	ipFeature	[in]	フィーチャ
	* @param	mesh		[in]	2次メッシュコード
	* @param	x			[in]	X座標
	* @param	y			[out]	Y座標
	* @retval	true	成功
	* @retval	false	失敗
	*/
	bool getMeshXY(const IFeaturePtr& ipFeature, int& mesh, int &x, int& y) const;

	/**
	* @brief	各種設定
	*/
	void setWorkspace(const IWorkspacePtr& ipWS) { m_ipWorkspace = ipWS; };
	void setOwner(const CString& strOwner) { m_strOwner = strOwner; };
	void setTableName(const CString& strTableName) { m_strTableName = strTableName; };
	void setSortField(const CString& strSortField) { m_strSortField = strSortField; };
	void setProgressLogPrefix(const CString& strProgressLogPrefix) { m_strProgressLogPrefix = strProgressLogPrefix; };

protected:
	IWorkspacePtr		m_ipWorkspace;
	ITablePtr			m_ipTable;
	FIELD_INDEX			m_fieldIndex;

	CString				m_strOwner;
	CString				m_strTableName;
	CString				m_strSortField;
	CString				m_strProgressLogPrefix;

	static crd_cnv		m_cCrdCnv;
};
