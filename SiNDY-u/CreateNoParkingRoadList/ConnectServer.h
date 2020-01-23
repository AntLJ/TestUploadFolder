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
 *  @brief サーバ接続設定
 */
struct ConfData
{
	CString eSDEServer;			//!< 接続するSDEサーバ情報
	CString eNoParkingPath;		//!< 駐禁取締路線情報のパス
	CString eOutput;			//!< 出力リストパス
	CString eOutputShape;		//!< 出力shapeパス
	CString eOutputLink;		//!< 駐車禁止取締区間のラインの出力shapeパス
};

class CConnectServer
{
public:
	CConnectServer(void);
	~CConnectServer(void);

	/**
	 * @brief 指定サーバへの接続を行う。
	 * @note  指定したサーバへの接続を行う。
	 *
	 * @param	rConf		[in]		設定ファイルパス	
	 *
	 * @return true		正常終了
	 * @return false	異常終了
	 */
	bool run(CString& rConf);

private:

	/**
	 * @brief 設定ファイルを読み込む。
	 * @note  指定した設定ファイルを読み込む。
	 *
	 * @param	rConf		[in]		設定ファイルパス
	 * @param	rConfData	[out]		接続設定	
	 *
	 * @return なし
	 */
	void ReadConf(CString& rConf, ConfData& rConfData);

	/**
	 * @brief	shapeの都道府県ディレクトリを取得する
	 * @param	lpcszNoParkingPath		[in]		設定ファイルのNOPARKINGDATA
	 * @param	setPerfecturePath		[out]		都道府県ディレクトリリスト
	 * @retval	true	取得成功
	 * @retval	false	取得失敗
	 */
	bool GetPerfecturePath( LPCTSTR lpcszNoParkingPath, std::set<CString>& setPerfecturePath );

	/**
	 * @brief	IWorkspace取得
	 * @param	lpcszDBPath				[in]		取得するDBへのパス
	 * @param	lpcszMessage			[in]		標準エラー出力へのメッセージ（XXを取得中...のXX部分）
	 * @return	IWorkspacePtr
	 */
	IWorkspacePtr GetWorkspace( LPCTSTR lpcszDBPath, LPCTSTR lpcszMessage );

	/**
	 * @brief	IFeatureClass取得
	 * @param	ipWorkspace				[in]		取得したいフィーチャクラスのワークスペース
	 * @param	lpcszFeatureClassName	[in]		取得したいフィーチャクラスの名前
	 * @return	IFeatureClassPtr
	 */
	IFeatureClassPtr GetFeatureClass( IWorkspace* ipWorkspace, LPCTSTR lpcszFeatureClassName );
};
