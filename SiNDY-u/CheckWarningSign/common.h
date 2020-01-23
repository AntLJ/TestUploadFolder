#pragma once

// option用変数
const LPCSTR ROADDB = "road_conn_db";
const LPCSTR ROADOWNER = "road_owner";
const LPCSTR ROADWHERE = "road_where";
const LPCSTR WARINIGSIGNDB = "warning_sign_conn_db";
const LPCSTR WARINGSIGNOWNER = "warning_sign_owner";
const LPCSTR BUFFER = "buffer";
const LPCSTR OUTPUTLOG ="log";
const LPCSTR RUNLOG = "runlog";
const LPCSTR ERRLOG = "errlog";


namespace common_proc {

	/*
	 *	@brief	ワークスペース取得関数
	 *	@param	connectInfo [in] ワークスペースへのパス
	 *	@param	ipWorkspace [out] ワークスペース
	 *	@retval	true 成功
	*	@retval	false 失敗
	 */
	bool getWorkspace(LPCTSTR connectInfo, IWorkspacePtr& ipWrokspace);

	/*
	 *	@brief	フィーチャクラス取得関数
	 *	@param  ipWorkspace [in] ワークスペース
	 *	@param  tableName [in] テーブル名
	 *	@param  schemaName [in] 対象テーブル格納スキーマ
	 *  @param  ipFeatureClass [out] フィーチャクラス
	 *	@retval	true 成功
	 *	@retval	false 失敗
	 */
	bool getFeatureClass(const IWorkspacePtr& ipWorkspace, LPCTSTR tableName, const CString& schemaName, IFeatureClassPtr& ipFeatureClass);

	/*
	 *	@brief  フィーチャカーソル取得関数
	 *	@param  ipFeatureClass [in] フィーチャクラス
	 *	@param  LPCTSTR tableName [in] テーブル名
	 *	@param  ipGeometry [in] 空間検索用形状
	 *	@param	spRel [in] 空間検索用空間関係
	 *	@param  whereClasue [in] 属性条件
	 *	@param  ipFeatureCursor [out] フィーチャカーソル
	 *	@retval	true 成功
	 *	@retval	false 失敗
	 */
	bool getFeatureCursor(const IFeatureClassPtr& ipFeatureClass, LPCTSTR tableName, const IGeometryPtr& ipGeometry, esriSpatialRelEnum spRel, const CString& whereClause, IFeatureCursorPtr& ipFeatureCursor);

	/*
	 * @brief フィールド群取得関数
	 * @param tableName      [in] レイヤ名(エラー出力用)
	 * @param ipFeatureClsas [in] フィーチャクラス
	 * @param ipFields       [out] 取得したフィールド群
	 *	@retval	true 成功
	 *	@retval	false 失敗
	 */
	bool getFields(LPCTSTR tableName, IFeatureClassPtr& ipFeatureClass, IFieldsPtr& ipFields);

	/*
	 * @brief フィールド取得関数
	 * @param tableName      [in] レイヤ名(エラー出力用)
	 * @param ipFields       [in] フィールド群
	 * @param lIndex         [in] フィールドインデックス
	 * @param ipField        [out] 取得したフィールド
	 *	@retval	true 成功
	 *	@retval	false 失敗
	 */
	bool getField(LPCTSTR tableName, IFieldsPtr& ipFields, long lIndex, IFieldPtr& ipField);
}

