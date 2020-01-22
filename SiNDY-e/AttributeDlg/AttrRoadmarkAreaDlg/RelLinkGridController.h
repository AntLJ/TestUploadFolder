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


class RelLinkGridController
{
public:
	RelLinkGridController(void) {};
	virtual ~RelLinkGridController(void) {};

	/**
 	 * @brief 初期化
	 * @param[in]  hwnd  関連リンクグリッドのハンドル
	 */
	void init( HWND hwnd);

	/**
	 * @brief OID群をグリッドへセット
	 * @param[in]  oids  OID群
	 */
	void setData( const std::vector<long>& oids);

	/**
	 * @brief OID群を返す
	 * @return OID群
	 */
	std::vector<long> getData() const;

	/**
	 * @brief グリッド上でのクリック操作時の処理
	 */
	void OnMouseUpGrid(SHORT Button, SHORT Shift, FLOAT X, FLOAT Y, HWND hwnd, UINT menuId);

	/**
	 * @brief 行を追加する
	 */
	void addRow();

	/**
	 * @brief 指定されたOIDに一致する行を削除する
	 * @param[in]  oids  削除対象のOID群
	 */
	void deleteRow(const std::vector<long>& oids);

	/**
	 * @brief 選択中の行を削除する
	 */
	void deleteSelectedRow();

	/**
	 * @brief 選択中のレコードのOIDを取得する
	 */
	std::vector<long> getSelectedData() const;


private:
	IVSFlexGridPtr m_relLinkGrid;	//!< 関連リンクのグリッドコントロール
	HWND m_gridWnd;					//!< グリッドのウィンドウハンドル
};

