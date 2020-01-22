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
#include "Arguments.h"
#include "RunLogger.h"
#include "ErrLogger.h"
#include "TDC/sindylib_core/Workspace.h"

/**
 * @brief 実行クラス
 */
class CCheckFollowingRoad
{
public:

	// 道路ノードの道なりリンク列を管理
	// (道路リンクのOIDその1, 道路リンクのOIDその2, 2つのリンクが成す角度)
	typedef std::map<std::pair<long, long>, double> FollowingRoad;

	CCheckFollowingRoad( const Arguments& args, CRunLogger& runLog, CErrLogger& errLog, const std::vector<long>& meshes )
		: m_Args( args ), m_RunLogger( runLog ), m_ErrLogger( errLog ), m_Meshes( meshes ) {};

	/**
	* @brief 処理を実行
	* @note メッシュ単位でループを回し、チェック処理を行う
	* @retval true	すべてのチェック処理で正常終了した
	* @retval false	1回でもチェック処理で異常終了した
	*/
	bool execute();

private:

	/**
	* @brief チェック処理前の準備
	* @note DB接続、テーブルの有無の確認、テーブルファインダーのモデル設定を行う
	* @retval true	DBへの接続成功＆処理対象テーブルがすべて存在している
	* @retval false	DBへの接続に失敗、または処理対象テーブルが存在しない
	*/
	bool preProcessing();

	/**
	* @brief チェックを実行
	* @note 指定されたメッシュ内のフィーチャに対してチェックする
	* @param mesh[in] チェック範囲のメッシュコード
	* @retval true	チェック処理に成功
	* @retval false	チェック処理に失敗
	*/
	bool doProcessing( const long mesh );

	/**
	* @brief 指定されたDBに接続する
	* @note 接続に失敗した際には、実行ログに出力する
	* @param dbName[in] DB名
	* @param ws[out] 指定したDBのワークスペース
	* @retval true	接続に成功
	* @retval false	接続に失敗
	*/
	bool connectDB( const uh::tstring& dbName, sindy::CWorkspace& ws );

	/**
	* @brief 指定したテーブルを取得する
	* @note 取得に失敗した際には、実行ログに出力する
	* @param ws[in] DBのワークスペース
	* @param tableName[in] テーブル名
	* @param table[out] 指定したテーブルへのポインタ
	* @retval true	取得に成功
	* @retval false	取得に失敗
	*/
	bool openTargetTable( const sindy::CWorkspace& ws, const uh::tstring& tableName, ITablePtr& table );
	
	/**
	* @brief 指定した道路ノードの道なりリンク列を取得
	* @param nodeID[in] 道なりリンク列を取得したい道路ノードのOID
	* @param linkT[in] 道路リンクのテーブルコンテナ
	* @param followingRoad[out] 道なりリンク列
	* @retval true	取得に成功
	* @retval false	取得に失敗
	*/
	bool getFollowingRoad( const long nodeOID, const sindy::CTableContainer& linkT, FollowingRoad& followingRoad );

	/**
	* @brief fromPointからtoPointまでの角度[°]を求める
	* @param fromPoint[in] 始点
	* @param toPoint[in] 終点
	* @return fromPointからtoPointまでの角度[°]
	*/
	double getAngle( const IPointPtr& fromPoint, const IPointPtr& toPoint );

	/**
	* @brief バージョン間で道路ノードに接続する道路リンクに変化がないかチェック
	* @param baseLinkT[in] 比較元バージョンの道路リンクのテーブルコンテナ
	* @param compLinkT[in] 比較先バージョンの道路リンクのテーブルコンテナ
	* @retval true	変化あり
	* @retval false	変化なし
	*/
	bool hasChangedConnectedLink( const sindy::CTableContainer& baseLinkT, const sindy::CTableContainer& compLinkT );

	/**
	* @brief 道路ノードまたはそれに接続する道路リンクが、指定日以降に更新されたかチェック
	* @note 最終更新日と最終プログラム更新日時がチェック対象
	* @note 最終更新日が指定されているかどうかもここで判定する
	* @param nodeRow[in] 道路ノードのRowコンテナ(道路リンクが紐付いている)
	* @retval true	更新されている or 最終更新日が指定されていない
	* @retval false	更新されていない
	*/
	bool isModifiedSinceDesignedDate( const sindy::CRowContainer& nodeRow );

	/**
	* @brief 比較元バージョンの道路ノードと道なりリンク列の対応マップを取得
	* @param nodeT[in] 道路ノードのテーブルコンテナ
	* @param mapFollowingRoad[out] 道路ノードと道なりリンク列の対応マップ
	* @retval true	取得に成功
	* @retval false	取得に失敗
	*/
	bool getMapFollowingRoad( const sindy::CTableContainer& nodeT, std::map<long, FollowingRoad>& mapFollowingRoad );

	/**
	* @brief 比較元バージョンと比較先バージョンの道なりリンク列を比較
	* @param mesh[in] チェック範囲のメッシュコード
	* @param baseNodeT[in] 比較元バージョンの道路ノードのテーブルコンテナ
	* @param compNodeT[in] 比較先バージョンの道路ノードのテーブルコンテナ
	* @retval true	比較処理に成功
	* @retval false	比較処理に失敗
	*/
	bool chkDiffFollowingRoad( const long mesh, sindy::CTableContainer& baseNodeT, sindy::CTableContainer& compNodeT );
	
private:
	const Arguments& m_Args; //!< コマンドライン引数管理クラス
	CRunLogger& m_RunLogger; //!< 実行ログ管理クラス
	CErrLogger& m_ErrLogger; //!< エラーログ管理クラス
	const std::vector<long>& m_Meshes;	//!< 処理対象となるメッシュコードの一覧

	sindy::CTableFinder m_BaseFinder; //!< 比較元バージョンのテーブルファインダー
	sindy::CTableFinder m_CompFinder; //!< 比較先バージョンのテーブルファインダー
};

