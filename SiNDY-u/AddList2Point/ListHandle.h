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

#include <map>
#include <string>


/**
 * @class	CIDPoint
 * @brief	IDリストのレコード
 */
struct CIDPoint
{
	LONG	m_lOID;							/// オブジェクトＩＤ
	std::map<CString, CString>	m_mapVal;	/// デフォルト値
};

/**
 * @class	CMXYPoint
 * @brief	MXYリストのレコード
 */
struct CMXYPoint
{
	int		m_nMeshCode;					/// メッシュコード
	int		m_nMeshX;						/// メッシュＸ
	int		m_nMeshY;						/// メッシュＹ
	std::map<CString, CString>	m_mapVal;	/// デフォルト値
};

/**
 * @class	CListHandle
 * @brief	各種リストハンドリングクラス
 */
class CListHandle
{
public:
	CListHandle(void);
	~CListHandle(void);

	/**
	 * @brief	ＩＤリスト読み込み
	 * @param	lpszFile	[in]	リストファイル
	 * @param	rVecList	[out]	ＩＤリスト格納コンテナ
	 * @return	bool
	 */
	bool loadIDList( LPCTSTR lpszFile, std::vector<CIDPoint>& rVecList );

	/**
	 * @brief	MeshXYリスト読み込み
	 * @param	lpszFile	[in]	リストファイル
	 * @param	rVecList	[out]	MeshXYリスト格納コンテナ
	 * @return	bool
	 */
	bool loadMXYList( LPCTSTR lpszFile, std::vector<CMXYPoint>& rVecList );

private:

	/**
	 * @brief	属性地セット
	 * @param	strData	[in]	データ
	 * @param	rMapVal	[out]	属性値格納コンテナ
	 * @retval	true : 成功
	 * @retval	false : 失敗
	 */
	bool setMapVal( const std::string& strData, std::map<CString, CString>& rMapVal );

};
