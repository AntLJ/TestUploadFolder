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

#include "stdafx.h"

#include <shapelib/DBFHandle.h>
#include "Moji_Chk.h"

using namespace std;

struct	ShpRecord
{
	// チェックで使うのはこっち
	wstring	wsNameKanji;		// 漢字名称(UTF16)
	wstring	wsNameYomi;			// ヨミ名称(UTF16)
	wstring	wsNameEng;			// 英語名称(UTF16)

	ShpRecord(){ }
	~ShpRecord(){ }

	bool	operator=(const ShpRecord temp_ShpRecord ) const
	{
		if( wsNameKanji == temp_ShpRecord.wsNameKanji && wsNameYomi == temp_ShpRecord.wsNameYomi )
			return true;
		else
			return false;
	}
};


class	CCheckMultiLanguageRoad
{
public:
	// コンストラクタ
	CCheckMultiLanguageRoad() : m_bIsError( false )
	{
	}
	// デクストラクタ
	~CCheckMultiLanguageRoad()
	{
	}

	/**
	 * @brief	使用法
	 */
	static void	printUsage();

	/**
	 * @brief	初期化
	 * @param	argc	[in]	引数の数
	 * @param	argv	[in]	引数
	 * @return	bool
	 */
	bool	init( int argc, char* argv[] );

	/**
	 * @brief	実行
	 * @return	bool
	 */
	bool	execute();

	/**
	 * @brief	エラーが存在したかどうか
	 * @return	bool
	 */
	bool	IsError() { return m_bIsError; };


private:

	/**
	 * @brief	引数チェック
	 * @param	argc	[in]	引数の数
	 * @param	argv	[in]	引数
	 * @return	bool
	 */
	bool	arg_check( int argc, char* argv[] );

	/**
	 * @brief	リストファイル読み込み
	 * @param	lpszFile	[in]	読み込みたいリスト名
	 * @param	VectorList	[out]	読込先Vector
	 * @return	bool
	 */
	bool	loadlist( LPCTSTR lpszFile, vector<string> &VectorList );

	/**
	 * @brief	メッシュ単位ファイル名の作成
	 * @param	strMesh		[in]	メッシュコード
	 * @param	strPath		[in]	ファイル基点パス
	 * @param	strLayer	[in]	レイヤ名
	 * @return	string		[out]	ファイルのフルパス
	 */
	string	make_mesh_fname( string strMesh, string strPath, string strLayer );

	/**
	 * @brief	dbfファイルを読み込み、チェックを行う
	 * @param	strMesh			[in]	変換メッシュ
	 * @param	strInFullPath	[in]	入力ファイルのフルパス
	 * @return	bool
	 */	
	bool	CheckDBF( string strMesh, string strInFullPath );

	/**
	 * @brief	dbfファイルの1レコードを読み込む
	 * @param	cDBFHdl			[in]	該当dbfのdbfHandleクラス
	 * @param	iRecNum			[in]	読み込みたいレコード番号
	 * @param	stShpRecord		[out]	読み込み先の構造体
	 * @return	bool
	 */	
	bool	LoadRecord( CDBFHandle cDBFHdl, int iRecNum, ShpRecord &stShpRecord );

	/**
	 * @brief	文字列ルールチェック
	 * @param	stShpRecord		[in]	shapeの1レコード(漢字、読み、翻訳のみ)	
	 */	
	void	CCheckMultiLanguageRoad::MojiStrChk( ShpRecord &stShpRecord );

	/**
	 * @brief	文字列関連性チェック
	 * @param	stShpRecord		[in]	shapeの1レコード(漢字、読み、翻訳のみ)	
	 */	
	void	CCheckMultiLanguageRoad::MojiRelationChk( ShpRecord &stShpRecord );

	/**
	 * @brief	文字列関連性チェック
	 * @param	stShpRecord		[in]	shapeの1レコード(漢字、読み、翻訳のみ)	
	 */	
	void	CCheckMultiLanguageRoad::OutputError( ShpRecord stShpRecord, string sErrorMessage );

private:

	vector<string>		m_vsMesh;				// メッシュリスト1行(メッシュコード+エリアコード)ずつ格納
	string				m_Meshlistname;			// メッシュリスト名
	string				m_InLayer;				// 入力レイヤ名

	string				m_InFilePath;			// データ入力パス名

	ofstream			m_ErrorLog;				// ログ出力用
	SYSTEMTIME			m_st;					// 開始・終了時間取得用

	Moji_Chk			e_sChk;					// チェック用クラス(本体はMultiNoteShpChk内)

	bool				m_bIsError;				// エラーが存在したかどうか
};
