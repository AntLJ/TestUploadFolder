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
using namespace std;
/**
 * @class	TextAddressMaster
 * @brief	テキスト住所マスタ
 */
class TextAddressMaster
{
public:
	/**
	 * @brief  コンストラクタ
	 */
	TextAddressMaster(void){}

	 /**
	 * @brief  デストラクタ
	 */
	virtual ~TextAddressMaster(void){}

	/**
	 * @brief	テキスト住所読み込み
	 * @param	[in] path		テキスト住所パス
	 * @retval	true:成功  false:失敗
	 */
	bool load( const CString& path );

	/**
	 * @brief	住所マスタの取得
	 * @param	[in] key	    	住所コード
	 * @return	ステータス * 住所名
	 */
	AddressMaster getTextAddressMaster( const CString& key );

	/**
	 * @brief	住所マスタコードの取得
	 * @param	[in] key	    	住所名
	 * @return	ステータス * 住所コード
	 */
	const AddressMaster TextAddressMaster::getTextAddressMasterCode( const CString& key )const;

	/**
	 * @brief	テキスト住所マスタを検索する
	 * @param	[in] cityCode    市区町村コード
	 * @param	[in] status      ステータス
	 * @retval	true:成功  false:失敗
	 */
	bool searchMaster(const CString &cityCode, long status);

	/**
	 * @brief	テキスト住所マスタを検索する
	 * @param	[in] cityCode    市区町村コード
	 * @retval	true:成功  false:失敗
	 */
	bool searchMaster(const CString &cityCode);

	/**
	 * @brief	テキスト住所マスタフォルダを設定する
	 * @param	[in] textAddressMasterPath        テキスト住所マスタパス
	 */
	void setDirPath( const CString& textAddressMasterPath ){ m_textAddressMasterPath = textAddressMasterPath; }

private:
	/**
	 * @brief	テキスト住所ファイル探す
	 * @param	[in] filePath        テキスト住所パス
	 * @param	[in] folder          テキスト住所フォルダ
	 * @retval	true:成功  false:失敗
	 */
	bool findFile( const CString& filePath, const CString& folder);

	/**
	 * @brief	テキスト住所ファイル読み込み
	 * @param	[in] filePath        テキスト住所マスタパス
	 * @param	[in] folder          テキスト住所フォルダ
	 * @retval	true:成功  false:失敗
	 */
	bool readData( const CString& filePath, const CString& folder);

	map<CString, AddressMaster> m_textAddressMaster;         //!< テキスト住所マスタリスト
	map<CString, AddressMaster> m_textAddressMasterCode;     //!< テキスト住所マスタコードリスト
	map<CString, set<long>>  m_textAddressMasterCityMap;     //!< テキスト住所マスタ都市コードマップ
	set<CString>m_readedFolderSet;                           //!< 既に読んだフォルダ
	CString m_textAddressMasterPath;                         //!< テキスト住所マスタフォルダ
};
