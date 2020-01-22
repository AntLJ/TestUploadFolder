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
#include "util.h"
#include <TDC/illustlib/HighwayInfo.h>
#include <TDC/sindylib_base/TableFinder.h>
#include <TDC/illustlib/IllustLinkTable.h>

#include <TDC/illustlib/IllustLinkRelCheck.h>

/**
 * @brief   メインクラス
 */
class CCheckIllustData
{
public:
	typedef std::map<CString, sindy::CSPIllustLinkRow> CHECKSAMENODEMAP;
	typedef std::multimap<CString, sindy::CSPIllustLinkRow> CHECKDIRECTIONCODEMAP;
	typedef CHECKDIRECTIONCODEMAP::const_iterator CHECKDIRECTIONCODEITER;

	enum EMode
	{
		kSchemaC = 1,        // スキーマチェック、オブジェクトチェック
		kNodeC = 2,          // ノードチェック
		kIllustC = 4,        // イラストチェック
		kNodeAndIllustC = 6, // ノード+イラストチェック
		kUnknown = -1,
	};

	/**
	 * @brief   コンストラクタ
	 * @param   args [in]  引数
	 */
	CCheckIllustData( const Arguments& args ){
		m_args = args;

		// 「システムが日本語環境」フラグを初期化する
		m_isJapaneseOs =  ( GetSystemDefaultLCID() == util::JAPANESE_LANGUAGE );

		// ASEAN向けフラグを初期化する
		m_isAseanSchema = AheIsWorldMeshMode();
	};

	/**
	 * @brief   デストラクタ
	 */
	~CCheckIllustData(){};

	/**
	 * @brief   実行
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool execute();

private:

	/**
	 * @brief   初期化
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool init();

	/**
	 * @brief   各ファイルを開く
	 * @retval true  成功
	 * @retval false 失敗
	 */
	bool OpenFile();

	/**
	 * @brief   モードを設定する
	 * @retval true  成功
	 * @retval false 失敗
	 */
	bool SetMode();

	/**
	 * @brief   イラストリンクに関連するフィーチャをキャッシュ
	 * @param  spIllustTable [in] イラストリンクテーブル
	 * @retval true  成功
	 * @retval false 失敗
	 */
	bool SetRelMap(sindy::CSPIllustLinkTable spIllustTable);

	/**
	 * @brief   メインチェック
	 * @param  cIllust [in] イラストテーブル
	 * @retval true  チェック成功
	 * @retval false チェック失敗
	 */
	bool check(sindy::CSPIllustLinkTable spIllustTable);

	/**
	 * @brief   同一種別、仕向先で重複しているレコードがないかチェック
	 * @param spRow [in] イラストリンク
	 * @param vecNodeQueue [in] ノード列
	 * @param checkSameNodeMap [in,out] キー：種別、仕向先、SINGLE_BRANCH_C、ノード列を文字列にしたもの　値：イラストリンク
	 * @param checkSameIllustNameSet [in,out] 種別、仕向先、イラスト名を文字列にした要素群
	 * @param errList [out] エラーリスト
	 */
	void CheckDuplicate( sindy::CSPIllustLinkRow& spRow, const sindy::CNodeQueueCheck::NODEQUEUE& vecNodeQueue,
		CHECKSAMENODEMAP& checkSameNodeMap, std::set<CString>& checkSameIllustNameSet, sindy::ERRLIST& errList );

	/**
	 * @brief   ラウンドアバウトのイラストリンクのチェック
	 * @param vecNodeQueue [in] ノード列
	 * @param errList [out] エラーリスト
	 */
	void checkRoundAbout( const sindy::CNodeQueueCheck::NODEQUEUE& vecNodeQueue, sindy::ERRLIST& errList);
	/**
	 * @brief   イラスト名を取得する
	 * @param spRow [in] イラストリンク
	 * @param[in] exceptFields 除外するフィールド群
	 * @return イラスト名
	 */
	CString GetIllustName(sindy::CSPIllustLinkRow& spRow, const std::set<CString>& exceptFields = std::set<CString>());

	/**
	 * @brief   起点から内容されているノード列チェック
	 * @param checkSameNodeMap [in] キー：種別、仕向先、ノード列を文字列にしたもの　値：イラストリンク
	 */
	void CheckComprehensionNode( const CHECKSAMENODEMAP& checkSameNodeMap );

	/**
	 * @brief   ノード列と方向コードの整合性チェック
	 * @param checkDirectionMap [in] [multimap]キー：イラストファイル名、種別　値：イラストリンク
	 * @retval true  チェック成功
	 * @retval false チェック失敗
	 */
	bool CheckDirectionCode( const CHECKDIRECTIONCODEMAP& checkDirectionMap );

	/**
	 * @brief   ノード列と方向コードの整合性チェック
	 * @param sameValsPair [in] equal_rangeでまとめた要素群
	 */
	void CheckDirectionCode( const std::pair<CHECKDIRECTIONCODEITER, CHECKDIRECTIONCODEITER>& sameValsPair );

	/**
	 * @brief   ノード列と方面案内の整合性チェック
	 * @param sameValsPair [in] equal_rangeでまとめた要素群
	 * @param setNoDirguideLink [in] 方面案内無しとして扱う道路リンク群
	 */
	void CheckRelDirguideExist( const std::pair<CHECKDIRECTIONCODEITER, CHECKDIRECTIONCODEITER>& sameValsPair, const std::set<long>& setNoDirguideLink );

	/**
	 * @brief   方面案内無しとして扱う道路リンク群を取得する
	 * @param setNoDirguideLink [out] 方面案内無しとして扱う道路リンク群
	 * @retval true  取得成功
	 * @retval false 取得失敗
	 */
	bool GetRoadLinkTreatedAsNoDirguide(std::set<long>& setNoDirguideLink);

	/**
	 * @brief   方面案内の有無からコード値取得
	 * @param lBeforeLinkID [in] 分岐前リンク
	 * @param lLeftLinkID   [in] 分岐左側のリンク
	 * @param lRightLinkID  [in] 分岐右側のリンク
	 * @param setNoDirguideLink [in] 方面案内無しとして扱う道路リンク群
	 * @retval 方面案内看板有無コード
	 */
	long GetDirGuideExist( long lBeforeLinkID, long lLeftLinkID, long lRightLinkID, const std::set<long>& setNoDirguideLink );

	/**
	 * @brief   道路ノード1→2→3の角度を求める。（∠123の角度）
	 * @note    正確には構成点→道路ノード2→構成点の角度を求める
	 * @param node1 [in] 流入側ノード
	 * @param node2 [in] 角度を求めるノード
	 * @param node3 [in] 流出側ノード
	 * @param spRow [in] イラストリンク
	 * @param angle [out] 角度(0～360度)
	 * @retval true  角度取得成功
	 * @retval false 対応するノード、リンクがない、形状がなくて角度を取得できなかった
	 */
	bool GetAngleOf3Nodes( long node1, long node2, long node3, const sindy::CSPIllustLinkRow& spRow, double& angle );

	/**
	 * @brief   道路リンクの対象ノード→構成点の角度を取得
	 * @param ipRoadLink [in] 道路リンク
	 * @param tgtNode [in] 角度を取得したい側の道路ノード
	 * @param angle [out] 取得した角度(ラジアン)
	 * @retval true  取得成功
	 * @retval false 取得失敗
	 */
	bool GetOutflowLinkAngle(IFeaturePtr ipRoadLink, long tgtNode, double& angle);

	/**
	 * @brief   ポイント1とポイント2の角度を求める
	 * @param ipPoint1 [in] ポイント1
	 * @param ipPoint2 [in] ポイント2
	 * @return ポイント1→ポイント2の角度(ラジアン)
	 */
	double GetAngle(IPointPtr ipPoint1, IPointPtr ipPoint2);

	/**
	 * @brief   ポイント1とポイント2の角度を求める
	 * @param lon1 [in] ポイント1の経度
	 * @param lat1 [in] ポイント1の緯度
	 * @param lon2 [in] ポイント2の経度
	 * @param lat2 [in] ポイント2の緯度
	 * @return ポイント1→ポイント2の角度(ラジアン)
	 */
	double GetAngle(double lon1, double lat1, double lon2, double lat2);

	/**
	 * @brief   HWY系の属性値とイラストリンクの属性値の整合性チェック
	 * @param spRow [in] イラストリンク
	 * @param vecNodeQueue [in] ノード列
	 */
	void CheckRelHighwayInfo( sindy::CSPIllustLinkRow& spRow, const sindy::CNodeQueueCheck::NODEQUEUE& vecNodeQueue );

	/**
	 * @brief   道路ノードの位置にあるHIGHWAY_NODEからHWY系の情報を関連付ける
	 * @param ipNode [in] 道路ノード
	 * @retval true  取得成功
	 * @retval false 取得失敗(HIGHWAY_NODEがない場合もここ)
	 */
	bool AssociateHighwayInfo( IFeaturePtr ipNode );

	/**
	 * @brief   チェック対象のHWYデータであるか
	 * @param lIllustC [in] イラスト種別
	 * @param cHighwayInfo [in] HWYデータ
	 * @retval true  対象である
	 * @retval false 対象でない
	 */
	bool IsTargetHWYInfo(long lIllustC, CHighwayInfo cHighwayInfo);

	/**
	 * @brief   HWY系の属性値とイラストリンクの属性値の整合性チェック
	 * @param spRow [in] イラストリンク
	 * @param cHighwayInfo [in] HWY系の情報
	 * @retval true  整合性が取れたデータ
	 * @retval false 不整合なデータ
	 */
	bool CheckRelHighwayInfo( sindy::CSPIllustLinkRow& spRow, CHighwayInfo& cHighwayInfo );

	/**
	 * @brief   2点の道路ノードから道路リンクを検索
	 * @note CNodeQueueCheck::FindLink相当(インスタンス作るの面倒なので)
	 * @param node1 [in] ノード1
	 * @param node2 [in] ノード2
	 * @return 検索された道路リンク
	 */
	sindy::ROADLINKMAP::const_iterator FindLink(long node1, long node2) const;

	sindy::ROADLINKMAP::const_iterator EndLink() const { return m_mapRoadLink.end(); }

	sindy::FEATUREMAP::const_iterator FindNode(long node) const;

	sindy::FEATUREMAP::const_iterator EndNode() const{ return m_mapRoadNode.end(); }
	
	/**
	 * @brief   実行ログを取得する
	 * @return  実行ログ
	 */
	std::wofstream& RunLog() const { return *m_ofsRunLog; }

	/**
	 * @brief   エラーログを取得する
	 * @return  エラーログ
	 */
	std::wofstream& ErrLog() const { return *m_ofsErrLog; }

	/**
	 * @brief   エラーログにコメントを書き込む
	 * @param message [in] メッセージ
	 */
	void WriteComment(const CString& message);

	/**
	 * @brief   全てのログにコメントを書き込む
	 * @param message [in] メッセージ
	 */
	void AllWriteComment(const CString& message);

	/**
	 * @brief   エラーログにエラーメッセージを書き込む
	 * @param spRow [in] イラストリンク
	 * @param errInfo [in] エラー情報
	 */
	void WriteError( const sindy::CSPIllustLinkRow& spRow, const IllustErrorInfo& errInfo);

	/**
	 * @brief   エラーログにエラーメッセージを書き込む
	 * @param field [in] フィールド名
	 * @param oid [in] OID
	 * @param errInfo [in] エラー情報
	 * @param lIllustC [in] イラスト種別
	 * @param lCustomer [in] 仕向先
	 * @param illustName [in] イラスト名
	 */
	void WriteError( const CString& field, long oid, const IllustErrorInfo& errInfo, long lIllustC, long lCustomer, const CString& illustName,
					const CString& strDate, const CString& strEditor);

	/**
	 * @brief   最新の更新日時、編集者を取得（ProgModifyかModifyか)
	 * @param spRow [in] イラストリンク
	 * @param strDate [out] 更新日時
	 * @param strEditor [out] 更新者
	 */
	void GetNewestEditInfo(const sindy::CSPIllustLinkRow& spRow, CString& strDate, CString& strEditor);

	/**
	 * @brief   フィールドが持つ日時を取得
	 * @param spRow [in] イラストリンク
	 * @param strField [in] フィールド名
	 */
	CString GetTime(const sindy::CSPIllustLinkRow& spRow, const CString& strField);

	/**
	 * @brief   終了ログを書きこむ
	 * @param   result [in] 実行結果
	 */
	void WriteEndLog(bool result);

	/**
	 * @brief   From Node　と To Node によってリンクを取得
	 * @param   [in]　firstNodeId From Node
	 * @param   [in]　secondNodeId To Node
	 * @return  リンクフィーチャ
	 */
	IFeaturePtr getRoadLinkFeature(long firstNodeId, long secondNodeId);

	/**
	 * @brief   リンクフィーチャクラース によってリンクの種類フラグを取得
	 * @param   [in]　ipLink リンクフィーチャクラース
	 * @param   [out]　isExistRoundAboutLink ラウンドアバウトリンクフラグ
	 * @param   [out]　isExistIntersectionLink 交差点リンクリンクフラグ
	 * @param   [out]　isExistNormalLink 普通リンクフラグ
	 */
	void getRoadLinkClassFlag(const IFeaturePtr& ipLink, bool& isExistRoundAboutLink, bool& isExistIntersectionLink, bool& isExistNormalLink );

private:

	Arguments m_args;                  //!< 引数
	IWorkspacePtr m_ipRoadWorkspace;   //!< 道路ワークスペース
	IWorkspacePtr m_ipIllustWorkspace; //!< イラストリンクワークスペース

	IFeatureClassPtr m_ipRoadLinkFC;    //!< 道路リンクフィーチャクラス
	IFeatureClassPtr m_ipRoadNodeFC;    //!< 道路ノードフィーチャクラス
	IFeatureClassPtr m_ipHighwayNodeFC; //!< HIGHWAY_NODEフィーチャクラス
	IFeatureClassPtr m_ipFacilInfoFC;   //!< FACIL_INFO_POINTフィーチャクラス
	ITablePtr m_ipRoadCodeT;            //!< ROAD_CODE_LISTテーブル
	ITablePtr m_ipLQDirguideT;          //!< LQ_DIRGUIDEテーブル
	ITablePtr m_ipLQTurnRegT;           //!< LQ_TURNREGテーブル
	ITablePtr m_ipINFTurnRegT;          //!< INF_TURNREGテーブル
	ITablePtr m_ipSubRoadLinkT;         //!< SUBROAD_LINKテーブル
	ITablePtr m_ipSubRoadPartT;         //!< SUBROAD_LINKテーブル
	EMode m_Mode;                       //!< 実行モード
	std::wofstream* m_ofsRunLog;         //!< 実行ログ
	std::wofstream* m_ofsErrLog;         //!< エラーログ

	CIllustDirInfo  m_DirInfo;          //!< イラストファイル格納先情報
	CIllustRuleInfo m_RuleInfo;         //!< イラストファイル名生成ルール

	sindy::CTableFinder m_finder;         //!< テーブルファインダ
	sindy::ROADLINKMAP m_mapRoadLink;     //!< イラストリンクに関連する道路リンク全て(FROM,TOノードIDがキー)
	sindy::FEATUREMAP m_mapRoadNode;      //!< イラストリンクに関連する道路ノード全て
	sindy::TURNREGMAP m_mapTurnReg;       //!< イラストリンクに関連する右左折禁止情報全て
	sindy::TURNREGINFMAP m_mapTurnRegInf; //!< イラストリンクに関連する道路リンクからINF_IDを逆引きするための情報
	std::set<long> m_unopenedRoadId;      //!< 未開通路線に関連する道路リンクID
	std::map<long, std::vector<long>> m_mapRelNodeHwyNode; //!< キー：ノードID、値：HIGHWAY_NODEのOID群(空間検索の回数を減らすため。)
	std::map<long, CHighwayInfo> m_mapHighwayInfo; //!< キー：HIGHWAY_NODE、値：HWY系情報

	bool m_isJapaneseOs;                   //!< システムが日本語環境フラグ(システム言語によって判定、値が0x0411の場合(日本語環境):true, 値が0x0411ではない場合(英語環境):false)
	bool m_isAseanSchema;                  //!< ASEAN向けフラグ(WorldMeshModeによって判定、値が1の場合(ASEAN向け):true, 値が0の場合(国内向け):false)
};

