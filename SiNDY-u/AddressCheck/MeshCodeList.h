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
 * @class	MeshCodeList
 * @brief	メッシュコードリスト
 */
class MeshCodeList
{
public:
	/**
	 * @brief  コンストラクタ
	 */
	MeshCodeList(void){}

	 /**
	 * @brief  デストラクタ
	 */
	virtual ~MeshCodeList(void){}
	
	/**
	 * @brief	メッシュリスト読み込み
	 * @param	[in] path     メッシュリストパス
	 * @retval	true:成功  false:失敗
	 */
	bool load( const CString& path);

	/**
	 * @brief	メッシュリスト取得
	 * @return	メッシュリスト
	 */
	 const std::list<unsigned long>& getMeshCodeList() const;

private:
	std::list<unsigned long> m_listMeshCode; //!< メッシュリスト
};
