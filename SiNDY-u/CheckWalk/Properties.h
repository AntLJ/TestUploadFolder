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
#include "Arguments.h"
#include <TDC/useful_headers/directory_util.h>
#include <memory>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/optional.hpp>
#include "util.h"

namespace properties {
	/**
	 * @brief 設定項目
	 */
	enum value {
		stationDB,
		roadDB,
		walkDB,
		meshDB,
		poiDB,
		underDB,
		meshList,
		heightNode_ini,
		target,
		day,
		time,
		dist_threshold,
		neighbor_threshold,
		refer_id_threshold,
		angle_threshold,
		buddy_dist_threshold,
		buddy_angle_threshold,
		sindyTolerance_threshold,
		microLogicalConnection
	};
	
	/**
	 * @brief チェック対象
	 */
	enum check_target
	{
		all,
		humancar,
		walkable,
		height_node,
		link_relation,
		under,
		other,
		release,
		pedxing,
		error,
	};

} // properties_value

/**
 * @class	CProperties
 * @brief	設定値管理クラス
 */
class CProperties
{
private:
	boost::property_tree::ptree m_proper_tree; //!< 設定ファイルの内容の格納先

public:
	CProperties(){}
	virtual ~CProperties(){}

	/**
	 * @brief	設定ファイル読み込み
	 * @param	fileName	[in]	設定ファイルパス
	 * @param	dataType	[in]	データタイプ
	 * @retval	true	成功
	 * @retval	false	失敗
	 */
	bool Read(const CString& fileName, const CString& dataType);

	/**
	 * @brief	設定値取得
	 * @param	propValue	[in]	取得したい設定値のタグのEnum型
	 * @retval	設定値（CString）
	 */
	CString GetValue(const properties::value propValue) const ;

	/**
	 * @brief	設定値取得
	 * @param	propValue	[in]	取得したい設定値のタグのEnum型
	 * @retval	設定値（double）
	 */
	double GetValueDouble(const properties::value propValue) const;

	/**
	 * @brief	設定値取得
	 * @param	propValue	[in]	取得したい設定値のタグのEnum型
	 * @retval	設定値（チェックタイプのEnum型）
	 */
	properties::check_target GetValueTarget(const properties::value propValue) const;

	/**
	 * @brief 設定値から生成した編集履歴フィルタを取得
	 * @return 編集履歴フィルタ
	 */
	CComVariant getModifyFilter() const;

private:

	/**
	 * @brief	タグ文字列取得
	 * @param	propValue	[in]	取得したいタグのEnum型
	 * @retval	タグ文字列
	 */
	std::string GetTag(const properties::value propValue) const ;

	/**
	 * @brief	設定値チェック
	 * @param	dataType	[in]	データタイプ
	 * @retval	true	成功
	 * @retval	false	失敗
	 */
	bool CheckValue(const CString& dataType);

	/**
	 * @brief	ファイルパス設定値チェック
	 * @param	propValue	[in]	チェックするタグのEnum型
	 * @retval	true	成功
	 * @retval	false	失敗
	 */
	bool CheckFileValue(const properties::value propValue);

	/**
	 * @brief	ディレクトリパス設定値チェック
	 * @param	propValue	[in]	チェックするタグのEnum型
	 * @retval	true	成功
	 * @retval	false	失敗
	 */
	bool CheckDirValue(const properties::value propValue);

	/**
	 * @brief	文字列設定値チェック
	 * @param	propValue	[in]	チェックするタグのEnum型
	 * @retval	true	成功
	 * @retval	false	失敗
	 */
	bool CheckStringValue(const properties::value propValue);

	/**
	 * @brief	チェックタイプ設定値チェック
	 * @param	propValue	[in]	チェックするタグのEnum型
	 * @retval	true	成功
	 * @retval	false	失敗
	 */
	bool CheckTargetValue(const properties::value propValue);

	/**
	 * @brief	標準エラーへのメッセージ出力
	 * @param	propValue	[in]	タグ
	 * @param	message	[in]	メッセージ
	 */
	void OutputStdErr(const properties::value propValue, const CString message);

};
