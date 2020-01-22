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

#include "SchemaWriter.h"

#include "AdamSchema.h"
#include "SurveyCSV.h"

namespace adam_v2
{

/**
 * @brief	ADAMスキーマ書き込みクラス
 */
class ADAMSchemaWriter : public SchemaWriter
{
public:
	ADAMSchemaWriter(void);
	~ADAMSchemaWriter(void);

	/**
	 * @brief	データ読み込み
	 * @note	未使用
	 * @param[in]	fileName	CSVファイル名
	 * @retval	true:	成功
	 * @retval	false:	失敗
	 */
	bool Read(const uh::tstring& fileName)
	{
		return false;
	}
	
	/**
	 * @brief	データ書き込み
	 * @param[in]	tableName	ADAMテーブル名
	 * @retval	true:	成功
	 * @retval	false:	失敗
	 */
	bool Write(const uh::tstring& tableName);

	/**
	 * @brief	データクリア
	 */
	void Clear();

	/**
	 * @brief	データセット
	 * @param[in]	csv		CSVデータ
	 */
	void SetCSV(const SurveyCSVPtr& csv);

	/**
	 * @brief	SRIDセット
	 * @retval	true:	成功
	 * @retval	false:	失敗
	 */
	bool SetSpatialReferenceId();

	/**
	 * @brief	編集開始/終了
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool StartEdit();
	bool StopEdit();
	bool AbortEdit();

	/**
	 * @brief	インポート対象スキーマと定義スキーマのバージョンチェックを行う
	 * @note	ADAM_VERSION テーブルがあればチェック実施
	 * @retval		true:成功
	 * @retval		false:失敗
	 */
	bool CheckAdamVersion();

	/**
	 * @brief	スキーマに接続
	 * @retval		true:成功
	 * @retval		false:失敗
	 */
	bool ConnectDB();

private:
	/**
	 * @brief	DBへ接続し、インポート保存する
	 * @param[in]	func	関数ポインタ
	 * @param[in]	tablename	テーブル名
	 * @retval		true:成功
	 * @retval		false:失敗
	 */
	bool Insert(
		bool (ADAMSchemaWriter::*func)(const uh::tstring&), 
		const uh::tstring& tablename
		);

	/**
	 * @brief	フィーチャごとのINSERT処理
	 * @note	ノード、リンク、ノード列の順に処理すること
	 * @TODO	フィーチャごとに別クラスにしたいけど...
	 * @param[in]	tableName	テーブル名
	 * @param[in]	ptr			ADAS-MAPデータ実体
	 * @param[in]	joinTxt		紐付けデータ実体
	 * @retval		true:成功
	 * @retval		false:失敗
	 */
	bool InsertLaneLink					(const uh::tstring& tablename);
	bool InsertLaneNode					(const uh::tstring& tablename);
	bool InsertNQLaneNode				(const uh::tstring& tablename);
	bool InsertRelRoadLinkLaneLink		(const uh::tstring& tablename);
	bool InsertLaneLinkGroup			(const uh::tstring& tablename);
	bool InsertCompartLink				(const uh::tstring& tablename);
	bool InsertCompartNode				(const uh::tstring& tablename);
	bool InsertNQCompartNode			(const uh::tstring& tablename);
	bool InsertRelCompartLinkLaneLink	(const uh::tstring& tablename);
	bool InsertBorderLink				(const uh::tstring& tablename);
	bool InsertBorderNode				(const uh::tstring& tablename);
	bool InsertNQBorderNode				(const uh::tstring& tablename);
	bool InsertRelBorderLinkLaneLink	(const uh::tstring& tablename);
	bool InsertSignalPoint				(const uh::tstring& tablename);
	bool InsertRelLaneNodeSignalPoint	(const uh::tstring& tablename);
	bool InsertCrossWalkLine			(const uh::tstring& tablename);
	bool InsertRelLaneNodeCrossWalkLine	(const uh::tstring& tablename);
	bool InsertRoadSignPoint			(const uh::tstring& tablename);
	bool InsertRelLaneNodeRoadSignPoint	(const uh::tstring& tablename);
	bool InsertVirtualCompartLine		(const uh::tstring& tablename);
	bool InsertCoveringArea				(const uh::tstring& tablename);

	/**
	 * @brief	3次元ジオメトリの作成
	 * @param[in]	loc			位置情報(入力データshape)
	 * @return	ip_polygon		3次元ジオメトリ(失敗時はnullptr)
	 * @note	空間参照取得元フィーチャは下記固定
	 *			ポイント	LANE_NODE
	 *			ポリライン	LANE_LINK
	 * @return	ジオメトリ
	 */
	IPointPtr CreatePointZ(const Location& loc);
	IPointPtr CreatePointZ(const Point& loc);
	IPolylinePtr CreatePolylineZ(const std::vector<Location>& loc);
	IPolylinePtr CreatePolylineZ(const Polyline& loc);
	IPolygonPtr CreatePolygonZ(const std::vector<Location>& loc);
	IPolygonPtr CreatePolygonZ(const Polygon& loc);

	/**
	 * @brief	編集履歴情報の設定
	 * @param[in]	adamSchema		AdamiPCFeature/TableClass
	 * @param[in]	ipRow			IRowBufferPtr
	 */
	void SetModifyData(const AdamSchema::AdamiPCFeature& adamSchema, IRowBufferPtr ipRow);
	void SetModifyData(const AdamSchema::TableClass& adamSchema, IRowBufferPtr ipRow);


	/**
	 * @brief	ロードオンリーモード使用プロセス
	 * @param[in]	ipT		テーブル
	 */
	void StartProcess(ITablePtr& ipT);
	static void FinishProcess(ITablePtr& ipT);

	/**
	 * @brief	スキーマのロック/アンロック制御
	 * @note	IFeatureClassLoad::LoadOnlyMode()を使う時は、スキーマロックする必要がある
	 * @param[in]	ipT		テーブル
	 * @param[in]	eStatus	ロック/アンロック（esriSharedSchemaLock:ロック, esriExclusiveSchemaLock:アンロック）
	 */
	static void ChangeSchemeLock(ITablePtr& ipT, esriSchemaLock eStatus);

	/**
	 * @brief	ロードオンリーモードのON/OFF制御
	 * @detail	IFeatureClassLoad::LoadOnlyMode()を使用し、
	 *			インポート時に発生する「空間インデックス グリッド サイズが正しくありません。」エラーを回避する
	 * @note	uh::scope_guardと組み合わせて、エラーハンドリング外でツールが落ちてもOFFにできるようにしている
	 *			テーブルのレコード追加にも対応しているが、LoadOnlyMode()はSDEではフィーチャークラスのみに使用可能なので、
	 *			ツールがSDEで使われることも想定して、テーブルの際には使っていない
	 * @param[in]	ipT		テーブル
	 * @param[in]	vbOnOff	ON/OFF（VARIANT_TRUE:ON, VARIANT_FALSE:OFF）
	 */
	static void ChangeLoadOnlyMode(ITablePtr& ipT, VARIANT_BOOL vbOnOff);

private:
	AdamSchemaPtr	_schema;				//!< スキーマ
	SurveyCSVPtr	_csv;					//!< 入力データ
};
typedef std::shared_ptr<ADAMSchemaWriter> ADAMSchemaWriterPtr;

};	//namespace adam_v2