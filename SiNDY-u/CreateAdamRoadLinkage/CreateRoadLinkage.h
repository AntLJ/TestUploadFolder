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

#include <SiNDYLib/FieldMap.h>

#include "Arguments.h"
#include "FeatureReader.h"
#include "ChildVerEditWorkspace.h"

/**
 * @brief   メインクラス
 */
class CCreateRoadLinkage
{
public:
	/**
	 * @brief  道路リンク－車線リンク関連テーブルの情報
	 * @note   テーブルへのレコード追加用
	 */
	struct InsertRecord{
		long		 roadOid;	// 道路リンクのOID
		CString      laneGid;	// 車線リンクGID
		int			 roadDC;	// 道路リンク方向コード

		InsertRecord() : roadOid(0), roadDC(-1){}
		InsertRecord(long road_oid, CString lane_gid, int road_dc )
			: roadOid(road_oid), laneGid(lane_gid), roadDC(road_dc){ }
	};

	/**
	 * @brief  道路リンク－車線リンク関連テーブルから削除するフィーチャー情報
	 * @note   エラーログへ出力する際に必要になるOID,X,Y,補足メッセージを持つ
	 */
	struct Info4ErrLog{
		long		 laneOid;	//!< 車線リンクのOID
		double		 x;			//!< 経度
		double		 y;			//!< 緯度
		CString		 msg;		//!< 補足メッセージ

		Info4ErrLog() : laneOid(-1), x(0.0), y(0.0){}
		Info4ErrLog(long lOid, double dX, double dY, CString strMsg )
			: laneOid(lOid), x(dX), y(dY), msg(strMsg){}
	};

public:
	/**
	 * @brief   コンストラクタ
	 * @param   args [in]  引数
	 */
	CCreateRoadLinkage( const Arguments& args ) : m_Args( args ){}

	/**
	 * @brief   デストラクタ
	 */
	virtual ~CCreateRoadLinkage(){};

	/**
	 * @brief   実行
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool execute();

private:
	/**
	 * @brief   ログ初期化
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool initLog() const;

	/**
	 * @brief   テーブルのオープン
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool openTable();

	/**
	 * @brief   編集開始
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool startEdit();

	/**
	 * @brief   編集終了
	 */
	void stopEdit();

	/**
	 * @brief   編集中断
	 */
	void abortEdit();

	/**
	 * @brief   リンク紐付け作成処理
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool createLinkage();

	/**
	 * @brief   差分FGDBを使わないメイン処理
	 * @param[in]	mesh				メッシュ情報
	 * @param[out]	lanelinkFeatures	紐付け対象の車線リンク
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool run4NotUseDiff( const std::pair<long, IGeometryPtr>& mesh, std::vector<IFeaturePtr>& lanelinkFeatures ) const;

	/**
	 * @brief   差分FGDBを使ったメイン処理
	 * @note	以下を行う
				・再紐付け対象の選定
				・差分レコードから既存の関連を削除し、削除情報をエラーログ出力
	 * @param[in]	mesh				メッシュ情報
	 * @param[out]	lanelinkFeatures	紐付け対象の車線リンク
	 * @param[out]	lane2Road			削除した関連レコード
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool run4UseDiff(
		const std::pair<long, IGeometryPtr>& mesh,
		std::vector<IFeaturePtr>& lanelinkFeatures,
		std::multimap<CString, long>& lane2Road
	) const;

	/**
	 * @brief   差分FGDBから差分データを取得する
	 * @note	取得するデータは1メッシュ分のデータに限定する
	 * @param[out]	diffMap		差分データ（キー：差分タイプ、値：フィーチャ）
	 * @param[in]	mesh		メッシュ
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool getDiff( std::map<long, std::vector<IFeaturePtr>>& diffMap, const IGeometryPtr& mesh ) const;

	/**
	 * @brief   形状差分が道路リンクであるか
	 * @note	m_DiffFMがGLOBALIDのインデックスを持っているか否かで判定している
	 * @retval  true  道路リンク
	 * @retval  false 車線リンク
	 */
	bool isRoadDiff() const;

	/**
	 * @brief   車線リンクの形状差分を分析する
	 * @param[in]	diffMap				差分マップ
	 * @param[in]	baseLane			1メッシュ分の車線リンク
	 * @param[out]	lanelinkFeatures	紐付け対象の車線リンク
	 * @param[out]	lane4DeleteRel		車線リンクと情報（関連削除対象）
	 */
	void analyzeLaneDiff(
		const std::map<long, std::vector<IFeaturePtr>>& diffMap,
		const std::map<CString, IFeaturePtr>& baseLane,
		std::vector<IFeaturePtr>& lanelinkFeatures,
		std::map<CString, Info4ErrLog>& lane4DeleteRel
	) const;

	/**
	 * @brief   道路リンクの形状差分を分析する
	 * @note	分析と謳っているが、元OIDを取得するだけに成り下がっている
				一応差分の種類を見ているのと、車線リンク側と統一する意図も込めてanalyzeとしておく
	 * @param[in]	diffMap				差分マップ
	 * @param[out]	orgOids				元OID
	 */
	void analyzeRoadDiff(
		const std::map<long, std::vector<IFeaturePtr>>& diffMap,
		std::vector<long>& orgOids
	) const;

	/**
	 * @brief   関連テーブルから関連を削除する
	 * @param[in]	wheres			WHERE句（IN句1000要素区切り）
	 * @param[out]	lane2Road		削除した関連レコード
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool deleteRel( const std::vector<CString>& wheres, std::multimap<CString, long>& lane2Road ) const;

	/**
	 * @brief   紐付け対象の車線リンクと、削除された車線リンクの情報を取得する
	 * @note	道路差分の中間処理を強引に関数にまとめただけ
	 * @param[in]	lane2Road			削除した関連レコード
	 * @param[in]	meshCode			メッシュコード
	 * @param[out]	lanelinkFeatures	紐付け対象の車線リンク
	 * @param[out]	lane4DeleteRel		車線リンクと情報（関連削除対象）
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool getLinkInfo4CreateAndDeleted(
		const std::multimap<CString, long>& lane2Road,
		long meshCode,
		std::vector<IFeaturePtr>& lanelinkFeatures,
		std::map<CString, Info4ErrLog>& lane4DeleteRel
	) const;

	/**
	 * @brief	削除した関連についてエラーログに出力
	 * @param[in]	lane4DeleteRel	車線リンクと情報（関連削除対象）
	 * @param[in]	lane2Road		削除した関連レコード
	 * @param[in]	meshCode		メッシュコード
	 */
	void writeDeletedRelInfo( const std::map<CString, Info4ErrLog>& lane4DeleteRel, const std::multimap<CString, long>& lane2Road, long meshCode ) const;

	/**
	 * @brief   関連テーブルへの重複インポート防止
	 * @param[in,out]	lanelinkFeatures	紐付け対象の車線リンク
	 * @param[out]		existRel			関連テーブルに存在する関連（Key：車線リンク、Value：道路リンク）
	 */
	void preventDuplication( std::vector<IFeaturePtr>& lanelinkFeatures, std::multimap<CString, long>& existRel ) const;

	/**
	 * @brief   道路リンク－車線リンク関連テーブルに紐付けが存在しない車線リンクのみに絞り込む
	 * @param[in,out] lane_features  車線リンク群
	 */
	void extractNotLinkedFeature( std::vector<IFeaturePtr>& lane_features ) const;

	/**
	 * @brief   関連テーブルに紐付きを持つ車線リンクの関連を取得する
	 * @param[in]	lane_features	車線リンク群
	 * @param[in]	existGids		車線リンクのGID
	 * @param[in]	exsistRels		車線リンクと道路リンクの対応表
	 */
	void getExistRel( const std::vector<IFeaturePtr>& lane_features, std::set<CString>& existGids, std::multimap<CString, long>& exsistRel ) const;

	/**
	 * @brief  検索値群と検索フィールドを指定してIN検索のWhere句を取得する
	 * @param [in] vals        検索値群
	 * @param [in] field_name  検索フィールド
	 * @return Where句群 (IN句の中身が1000件を超える場合、複数のWhere句に分けて返す)
	 */
	std::vector<CString> getWhereClauseINSearch( const std::set<CString>& vals, const CString& field_name ) const;
	std::vector<CString> getWhereClauseINSearch( const std::vector<long>& vals, const CString& field_name ) const;
	std::vector<CString> getWhereClauseINSearch( const std::list<CString>& vals, const CString& field_name ) const;

	/**
	 * @brief	紐付け対象の車線リンクを使って、関連を作る
	 * @param [in] lanelinkFeatures	紐付け対象の車線リンク
	 * @param [in] lane2Road		削除した関連（Key：車線リンク、Value：道路リンク）
	 * @param [in] existRel			関連テーブルに存在する関連（Key：車線リンク、Value：道路リンク）
	 * @param [in] meshCode			メッシュコード
	 * @return Where句群 (IN句の中身が1000件を超える場合、複数のWhere句に分けて返す)
	 */
	bool createRel(
		const std::vector<IFeaturePtr>& lanelinkFeatures,
		const std::multimap<CString, long> lane2Road,
		const std::multimap<CString, long> existRel,
		long meshCode
	) const;

	/**
	 * @brief  車線リンク(lane_link)に紐づく道路リンクを道路リンク群(road_links)から取得する
	 * @param[in]	lane_link		紐付け元の車線リンク
	 * @param[in]	road_links		紐付け対象の道路リンク群
	 * @param[in]	lane2Road		削除した関連（Key：車線リンク、Value：道路リンク）
	 * @param[in]	existRel		関連テーブルに存在する関連（Key：車線リンク、Value：道路リンク）
	 * @param[in]	bash_mesh		ベースメッシュ番号
	 * @param[out]	insert_records	紐付け情報リスト
	 */
	void matchLaneLinkToRoadLinks(
		const IFeaturePtr& lane_link,
		const std::vector<IFeaturePtr> road_links,
		const std::multimap<CString, long>& lane2Road,
		const std::multimap<CString, long>& existRel,
		long base_mesh,
		std::vector<InsertRecord>& insert_records
	) const;

	/**
	 * @brief   道路リンク－車線リンク関連テーブルへの出力処理
	 * @param[in] insert_records  出力する情報
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool insertRecord(const std::vector<InsertRecord>& insert_records ) const;

	/**
	 * @brief	メッシュポリゴン取得
	 * @param[in]	meshList	 メッシュリスト
	 * @param[out]	meshGeoMap	 メッシュジオメトリマップ
	 * @retval	true  成功
	 * @retval	false 失敗
	 */
	bool getMeshPolygon( const std::vector<long>& meshList, std::map<long, IGeometryPtr>& meshGeoMap ) const;

	/**
	 * @brief   現在の時間を文字列で取得する
	 * @return  現在の時間を示す文字列 (YYYY/MM/DD HH:MM:SS)
	 */
	CString getCurrentTime() const;

	/**
	 * @brief   後処理 
	 * @param [in] isSucceeded 正常終了したか否か
	 */
	void finish(bool isSucceeded) const;

	/**
	 * @brief 子バージョン名を返す
	 * @return 子バージョン名
	 */
	CString getChildVersionName() const;

private:
	Arguments m_Args;      //!< 引数

	ChildVerEditWorkspace m_Workspace;		//!< 編集セッション制御

	ITablePtr m_LaneLinkTable;				//!< 車線リンクテーブル
	ITablePtr m_RoadLinkTable;				//!< 道路リンクテーブル
	ITablePtr m_BaseMeshTable;				//!< ベースメッシュテーブル
	ITablePtr m_RelRoadLinkLaneLinkTable;	//!< 道路リンク－車線リンク関連テーブル
	ITablePtr m_DiffTable;					//!< 道路リンクor車線リンクテーブル（形状差分）

	sindy::CFieldMap m_LaneLinkFM;				//!< 車線リンクテーブルのフィールドマップ
	sindy::CFieldMap m_RelRoadLinkLaneLinkFM;	//!< 道路リンク－車線リンク関連テーブルのフィールドマップ
	sindy::CFieldMap m_BaseMeshFM;				//!< ベースメッシュテーブルのフィールドマップ
	sindy::CFieldMap m_DiffFM;					//!< 道路リンクor車線リンクテーブル（形状差分）のフィールドマップ

	ISpatialReferencePtr m_LaneSpRef;			//!< 車線リンクテーブルの空間参照
	ISpatialReferencePtr m_DiffSpRef;			//!< 道路リンクor車線リンクテーブル（形状差分）の空間参照
};

