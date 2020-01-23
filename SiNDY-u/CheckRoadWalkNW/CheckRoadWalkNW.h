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
#include "DatabaseController.h"

#include "CheckRoadWalkLink.h"
#include "CheckRoadWalkNode.h"
#include "CheckRoadWalkPassageCondition.h"
#include "CheckRoadWalkOnewayCondition.h"
#include "CheckRoadCode.h"

#include <WinLib/VersionInfo.h>

#define OPTION_MAX 10

class	CCheckRoadWalkNW
{
private:
	
	std::ofstream			m_ErrorLog;				// ログ出力用
	SYSTEMTIME				m_st;					// 開始・終了時間取得用

	bool					m_bIsError;				// エラーが存在したかどうか
	std::string				m_inputDirPath;
	std::string				m_layer;				// 入力レイヤ名
	std::string				m_roadDBConnection;		// 道路DB接続情報
	std::string				m_walkDBConnection;		// 歩行者DB接続情報
	std::string				m_meshDBConnection;		// 参照メッシュDB接続情報
	std::string				m_roadSchema;			// 道路DBスキーマ
	std::string				m_walkSchema;			// 歩行者DBスキーマ
	std::string				m_meshSchema;			// 参照メッシュDBスキーマ

	std::vector<std::string>		m_meshList;

public:

	CCheckRoadWalkNW():m_bIsError(false){};
	~CCheckRoadWalkNW(){};

	/**
	 * @brief	使用方法
	 */
	void printUsage();

	/*
	 * @brief	初期化
	 * @param	argc	[in]	引数の数
	 * @param	argv	[in]	引数
	 * @return	bool
	 */
	bool init(int argc, char* argv[]);
	/*
	 * @brief	チェック実行
	 * @return	bool
	 */
	bool execute();

	/*
	 * @brief	エラーの存在有無
	 * @return	bool
	 */
	bool isError();

private:
	/*
	 * @brief	オプションチェック
	 * @param	argc	[in]	引数の数
	 * @param	argv	[in]	引数
	 * @return	bool
	 */
	bool	checkOptions(int argc, char* argv[]);

	/*
	 * @brief	オプションチェック
	 * @param	argc	[in]	引数の数
	 * @param	argv	[in]	引数
	 * @param	pos	[in]	チェックする引数が何番目の引数か
	 * @return	bool
	 */
	bool	checkOptionArgument(int argc, char* argv[], int pos);

	/**
	 * @brief	リストファイル読み込み
	 * @param	lpszFile	[in]	読み込みたいリスト名
	 * @param	VectorList	[out]	読込先Vector
	 * @return	bool
	 */
	bool	loadList( LPCTSTR lpszFile, std::vector<std::string> &VectorList );

	/**
	 * @brief	SHAPEが格納されているディレクトリパスの作成
	 * @param	strMesh		[in]	メッシュコード
	 * @param	strPath		[in]	ファイル基点パス
	 * @return	string		[out]	SHAPEが格納されているディレクトリパス
	 */
	std::string	makeShapeDirName( std::string strMesh, std::string strPath);

	/**
	 * @brief	シェープファイルチェック
	 * @param	strMesh			[in]	チェック対象メッシュ
	 * @param	strShapeDirPath	[in]	SHAPEデータ格納ディレクトリ
	 * @param	dbCtrl	[in]	参照DB管理クラス
	 * @return	bool
	 */	
	bool checkShape( std::string strMesh, std::string strShapeDirPath, CDatabaseController dbCtrl);

	/**
	 * @brief	RWLとRWNのノード存在チェック
	 * @param	ipRWLFeatureClass	[in]	チェックするRWLのフィーチャークラス
	 * @param	ipRWNFeatureClass	[in]	RWNのフィーチャークラス
	 * @param	messageList	[out]	エラーメッセ―ジリスト
	 * @return	bool
	 */	
	bool checkExist(const IFeatureClassPtr ipRWLFeatureClass,const IFeatureClassPtr ipRWNFeatureClass,std::vector<CString>& messageList);

	/**
	 * @brief	RWNの変換対象外レコードが存在しないことのチェック
	 * @param	strMesh		[in]	メッシュコード
	 * @param	ipRWNFeatureClass	[in]	チェックするRWNのフィーチャークラス
	 * @param	RWNFiledIndex	[in]	RWLのフィーチャークラス
	 * @param	dbCtrl	[in]	参照DB管理クラス
	 * @param	messageList	[out]	エラーメッセ―ジリスト
	 * @return	bool
	 */
	bool checkNoTarget(const string& strMesh, const IFeatureClassPtr ipRWNFeatureClass, const FIELD_INDEX& RWNFiledIndex, CDatabaseController dbCtrl, std::vector<CString>& messageList);

	/**
	 * @brief	歩行者リンクに「人車共用リンク」が1本以上存在するノードの取得
	 * @param	rwlFeatures	[in]	RWLのフィーチャー
	 * @param	rwlFieldIndex	[in]	RWLのフィールドインデックス
	 * @param	nodeList	[out]	ノードリスト
	 * @return	bool
	 */	
	bool getHumanCarLinkList(const MAP_FEATURE& rwlFeatures, const FIELD_INDEX& rwlFieldIndex, multiset<long>& nodeList);

	/**
	 * @brief	ログ出力
	 * @param	tag	[in]	ログレベルのタグ
	 * @param	message1	[in]	メッセージ1
	 * @param	message2	[in]	メッセージ2
	 */	
	void printLog(std::string tag, std::string message1, std::string message2);

	/**
	 * @brief	ログ出力(List)
	 * @note	メッセージのリスト１つ１つにタグをつけて出力する。
	 * @param	tag	[in]	ログレベルのタグ
	 * @param	messageList	[in]	メッセージのリスト
	 */	
	void printLog(std::string tag, std::vector<CString> messageList);

};
