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
 * @class	SettingFile
 * @brief	設定ファイル
 */
class SettingFile
{
public:
	/**
	 * @brief  コンストラクタ
	 */
	SettingFile(void){}

	/**
	 * @brief  デストラクタ
	 */
	virtual ~SettingFile(void){}
	
	/**
	 * @brief	設定ファイル読み込み
	 * @param	[in] path     設定ファイルパス
	 * @param	[in] layer    レイヤリスト
	 * @retval	true:成功  false:失敗
	 */
	bool load(const CString& path, const std::vector<CString>& layer);

	/**
	 * @brief	ログファイル名の取得
	 * @param	[in] layer      現在のレイヤ
	 * @return	ログファイル名
	 */
	const CString& getLayerLogName( const CString& layer ) const;

	/**
	 * @brief	設定アイテムの取得
	 * @param	[in] layer        現在のレイヤ
	 * @param	[in] itemName     設定アイテム名
	 * @return	設定アイテム文字列
	 */
	CString getLayerSettingItem(const CString& layer, const CString& itemName)const;

private:
	std::map<CString,CString> m_layerLogName; //!< レイヤ * ログファイル名リスト
	CString m_settingFilePath;                //!< 設定ファイルパース
};
