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

// sindylib_core,sindylib_base
#include <TDC/sindylib_core/Workspace.h>
#include <TDC/sindylib_base/TableFinder.h>
#include <TDC/sindylib_base/TableContainer.h>
#include <TDC/sindylib_base/RowContainer.h>

// illustlib
#include "TDC/illustlib/IllustLinkRow.h"
#include "TDC/illustlib/IllustLinkTable.h"
#include "TDC/illustlib/IllustNameCreator.h"
#include "TDC/illustlib//IllustOutputInfo.h"

#include <sindy/schema/illust.h>


/**
 * @brief ノードリスト作成クラス
 */
class CNodelistCreator
{
private:
	/**
	 *	@brief ノードIDとメッシュ番号の対応レコード
	 *	@note  リンク列出力処理で使用
	 */
	struct NodeInfo
	{
		long          id;	//!< ノードID
		std::set<int> meshes; //!< 2次メッシュコード

		NodeInfo():id(-1){}

		// メッシュコード追加
		void addMesh( int nMesh ){ meshes.insert(nMesh); }	
		// 2次メッシュ上のノードか否か
		bool isOn2ndMesh() const { return meshes.size() >= 2; }	

		// 共通のメッシュコード取得
		int getCommonMesh( const NodeInfo& info ) const {
				std::set<int> common_mesh;
				std::set_intersection(std::begin(meshes),std::end(meshes),std::begin(info.meshes),std::end(info.meshes), std::inserter(common_mesh, std::end(common_mesh)));
				if( common_mesh.size() != 1 )
					return -1;
				return *common_mesh.begin();
		}
		// メッシュコード群の比較
		bool isEqualMesh( const NodeInfo& info ) const { return meshes == info.meshes; }
	};

public:
	/**
	 * @brief コンストラクタ
	 */
	CNodelistCreator(void){}
	/**
	 * @brief デストラクタ
	 */
	~CNodelistCreator(void){}

public:

	/**
	 * @brief 初期化
	 * @param[in]  strDBConnect  DB接続設定
	 * @param[in]  strOutputPath ノードリスト出力フォルダパス
	 * @param[in]  strLatLon     緯度・経度出力設定
	 * @param[out] strErrMsg     エラーメッセージ
	 * @retval     true          初期化成功
	 * @retval     false         初期化失敗
	 */
	bool init( const CString& strDBConnect, const CString& strOutputPath, const CString& strLatLon, CString& strErrMsg );
	/**
	 * @brief  初期化(道路ノードの接続先DBをイラストリンクとは別に指定)
	 * @detail ILLUST_LINKとROAD_NODEは同じデータセットに存在する想定だが、機能としては用意しておく
	 * @param[in]  strDBConnect		 DB接続設定(ILLUST_LINK)
	 * @param[in]  strRoadDBConnect  DB接続設定(ROAD_NODE)
	 * @param[in]  strOutputPath     ノードリスト出力フォルダパス
	 * @param[in]  strLatLon        緯度・経度出力設定
	 * @param[out] strErrMsg         エラーメッセージ
	 * @retval     true              初期化成功
	 * @retval     false             初期化失敗
	 */
	bool init( const CString& strDBConnect, const CString& strRoadDBConnect, const CString& strOutputPath, const CString& strLatLon, CString& strErrMsg );

	/**
	 * @brief ノードリスト作成
	 * @param[in]  info      ノードリスト出力情報
	 * @param[in]  rule      イラスト名の生成ルール
	 * @param[out] strErrMsg  エラーメッセージ
	 * @retval     true       ノードリスト作成成功
	 * @retval     false      ノードリスト作成失敗
	 */
	bool create( const CIllustOutputInfo::OutputInfo info, const std::vector<CString>& rule, CString& strErrMsg );

private:

	/**
	 * @brief  指定回数だけDBへの接続をトライする
	 * @param[in]   strDBConnect  DB接続情報
	 * @param[in]   nTryCount     接続をトライする回数 (ex. 3を指定すると接続に失敗した場合、2回までリトライする)
	 * @param[out]  ipWorkspace   接続後のワークスペース情報
	 * @retval     true           接続成功
	 * @retval     false          接続失敗
	 */
	bool connectSeveralTimes( const CString& strDBConnect, int nTryCount, sindy::CWorkspace& cWorkspace );

	/**
	 * @brief  設定ファイル記載の指定フィールドの値を取得する
	 * @detail 指定フィールドに"ILLUST_NAME"と指定されていた場合は、フィールド値ではなくイラスト名を出力する
	 * @param[in]  cNameCreator   イラスト名生成オブジェクト
	 * @param[in]  spRow          ILLUST_LINK 1レコード
	 * @param[in]  outputFields   値を出力する対象フィールド名群
	 * @param[out] OutString      出力値群
	 * @retval     true           値の取得成功
	 * @retval     false          値の取得失敗
	 */
	bool getFieldValue( sindy::CIllustNameCreator cNameCreator, sindy::CSPIllustLinkRow spRow , const std::vector<CString>& OutputFields, std::vector<CString>& OutString );

	/**
	 * @brief  フィールドの値を変換して返す
	 * @detail イラスト種別が側道分岐のみ対象
	 * @param[in]  spRow          ILLUST_LINK 1レコード
	 * @param[in] strFieldName   フィールド名
	 * @return    変換後の値
	 */
	CString convFieldValue( sindy::CSPIllustLinkRow spRow, const CString& strFieldName );

	/**
	 * @brief  緯度経度を出力する
	 * @detail init関数で緯度経度の出力指定がされていない場合は出力しない
	 * @param[in]  spRow          ILLUST_LINK 1レコード
	 * @param[out] OutString      出力値群
	 * @retval     true   出力成功
	 * @retval     false  出力失敗
	 */
	bool getLatLon( sindy::CSPIllustLinkRow spRow, std::vector<CString>& OutString );

	/**
	 * @brief  リンク列を出力する
	 * @detail 2次メッシュ境界上のノードの場合は両メッシュの2ノード分を出力する
	 * @detail 2ノードの出力順は1つ前のノードから判断する
	 * @param[in]  spRow          ILLUST_LINK 1レコード
	 * @param[in]  nStartNode     出力開始ノード
	 * @param[in]  nEndNode       出力終了ノード
	 * @param[out] OutString      出力値群
	 * @retval     true   出力成功
	 * @retval     false  出力失敗
	 */
	bool getLQ( sindy::CSPIllustLinkRow spRow, int nStartNode, int nEndNode, std::vector<CString>& OutString );

	/**
	 * @brief   ノードIDからメッシュコード群を取得する
	 * @param[in]  lNodeId  ノードID
	 * @return     メッシュコード
	 */
	std::vector<int> getMeshcodes( long lNodeId );

	/**
	 * @brief   ノードIDから実数緯度経度を取得する
	 * @param[in]  lNodeId  ノードID
	 * @return     実数緯度経度
	 */
	WKSPoint getLatLonByNodeId( long lNodeId );

	/**
	 * @brief   先頭ノードの1つ前のノードが属するメッシュコードを取得する
	 * @detail  先頭ノードが2次メッシュ上に存在する場合も考慮する
	 * @param[in]  infos  ILLUST_LINKのリンク列(ノードID群)
	 * @param[in]  iter   ILLUST_LINKのリンク列(ノードID群)の先頭イテレータ
	 * @return     メッシュコード
	 */
	int getPreRoadNodeMeshcode( std::vector<NodeInfo>& infos,  std::vector<NodeInfo>::iterator iter );

	/**
	 * @brief   ILLUST_LINKのリンク列(ノードID群)からIDが格納されている末尾フィールドのインデックスを取得する
	 * @param[in]  spRow  ILLUST_LINK 1レコード
	 * @return     格納されている末尾のインデックス(フィールド名"ROAD_NODE_XX"の"XX")
	 */
	int getLastEnableNodeField( sindy::CSPIllustLinkRow spRow ); 

	/**
	 * @brief  ノードが2次メッシュ上にあるかを判定する
	 * @param[in]  lNodeId  ノードID
	 * @retval     true   2次メッシュ上にある
	 * @retval     false  2次メッシュ上にない  
	 */
	bool isOn2ndMesh( long lNodeId );

	/**
	 * @brief  ノードリスト出力ストリームを取得する
	 * @detail 既にファイルが存在している場合は、追記モードで取得する
	 * @param[in]  strFolderPath  出力フォルダ
	 * @return  出力ストリーム
	 */
	std::ofstream getOutputFileStream( const CString& strFolderPath );

private:

	sindy::CTableFinder m_ipTableFinder;		//!< テーブルファインダ
	CString             m_strOutputPath;		//!< ノードリスト出力フォルダパス
	CString             m_LatLon;				//!< 緯度経度出力設定
};
