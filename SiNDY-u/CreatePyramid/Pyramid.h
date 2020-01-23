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

class CPyramid
{
public:
	/**
     * @brief  コンストラクタ
     */	
	CPyramid(void);

	/**
     * @brief  デストラクタ
     */	
	virtual ~CPyramid(void){}

	/**
     * @brief   メッシュリスト指定で処理する
     * @param   bOption [in]  標準入力で指定するか
     */	
	void TargetMeshList( bool bOption );

	/**
     * @brief   ディレクトリ指定で処理する
     * @param   bOption [in]  標準入力で指定するか
     */	
	void TargetPathList( bool bOption );

	/**
     * @brief   ピラミッドファイルを作成する
	 * @note    ビラミッドファイルについては下記のページを参照。
	            http://desktop.arcgis.com/ja/arcmap/10.3/manage-data/raster-and-images/raster-pyramids.htm
				http://desktop.arcgis.com/ja/arcmap/10.3/manage-data/raster-and-images/ovr-pyramid-files.htm
     * @param   ipRasterWS [in]  ラスタワークスペース
     * @param   szFildFile [in]  処理対象ラスタファイル名
     * @param   szMesh     [in]  メッシュコード（表示用）
     * @param   i          [in]  処理番号（表示用）
     */	
	void CreatePyramidFile( IRasterWorkspacePtr ipRasterWS, LPCSTR szFildFile, LPCSTR szMesh, int i );

	/**
     * @brief   開始処理(処理モード受け付け)
     * @param   bOption [in]  標準入力で指定するか
	 * @return  処理モード
	 */	
	int StartFunction( bool bOption );

	/**
     * @brief   終了処理
	 * @return  次の処理フラグ 
     */	
	int EndFunction();

	/**
     * @brief   オプションを解析して設定する
     * @param   listOption [in]  コマンドライン引数リスト
	 * @retval  true   標準入力で指定する
	 * @retval  false  標準入力で指定しない（コマンドライン引数で指定）
	 */	
	bool InitOption( std::list<CString> listOption );

	CString m_strIniPath;		//!< iniファイルのフルパス
	CString	m_strCurrent;		//!< カレントディレクトリ（実行ファイルのディレクトリ）

private:
	bool		m_bBatch;		//!< 標準入力かどうか
	long		m_lDir;			//!< 読込みディレクトリ変更(0:./4桁/8桁or6桁.jpg 1:./8桁or6桁.jpg 2:./6桁/10桁.jpg)
	long		m_lExtension;	//!< 拡張子(0:jpg 1:png 2:bmp)
	long		m_lMode;		//!< 処理モード(1:メッシュリスト 2:ディレクトリ)
	CString		m_strMeshlist;	//!< メッシュリストファイルのフルパス
	CString		m_strRasterDir;	//!< ラスタファイルが格納されているディレクトリ

};
