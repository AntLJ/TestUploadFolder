#pragma once

// コマンドライン引数
const LPCSTR INI = "ini";
const LPCSTR PROCLOG = "proc_log";
const LPCSTR ERRLOG = "err_log";


// 設定ファイル項目
const LPCSTR INPUTDIR = "INPUT_DIR";
const LPCSTR HEIGHTDB = "HEIGHT_DB";
const LPCSTR HEIGHTSCHEMA = "HEIGHT_SCHEMA";
const LPCSTR MESHDB = "MESH_DB";
const LPCSTR MESHSCHEMA = "MESH_SCHEMA";
const LPCSTR OUTPUTDIR = "OUTPUT_DIR";
const LPCSTR MESHLIST = "MESHLIST";
const LPCSTR BUFFER = "BUFFER";

const long HUNDRED_MILLION = 100000000;

// フィールド情報
struct FieldInfo
{
	CString m_fieldName;	// フィールド名
	esriFieldType m_fieldType;	// フィールドタイプ
	long m_length; // フィールド長

	FieldInfo(LPCTSTR fieldName, esriFieldType fieldType, long length)
	{
		m_fieldName = fieldName;
		m_fieldType = fieldType;
		m_length = length;
	}
};


namespace common_proc {

	/*
	 *	@brief	ワークスペース取得関数
	 *	@param	connectInfo [in] ワークスペースへのパス
	 *	@param	ipWorkspace [out] ワークスペース
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool GetWorkspace(LPCTSTR connectInfo, IWorkspacePtr& ipWorkspace); 


	/*
	 *	@brief	フィーチャクラス取得関数
	 *	@param  ipWorkspace [in] ワークスペース
	 *	@param  tableName [in] テーブル名
	 *	@param  schemaName [in] 対象テーブル格納スキーマ
	 *  @param  ipFeatureClass [out] フィーチャクラス
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool GetFeatureClass(const IWorkspacePtr& ipWorkspace, LPCTSTR tableName, const CString& schemaName, IFeatureClassPtr& ipFeatureClass);


	/*
	 *	@brief  フィーチャカーソル取得関数
	 *	@param  ipFeatureClass [in] フィーチャクラス
	 *	@param  LPCTSTR tableName [in] テーブル名
	 *	@param  ipGeometry [in] 空間検索用形状
	 *	@param	spRel [in] 空間検索用空間関係
	 *	@param  whereClasue [in] 属性条件
	 *	@param  ipFeatureCursor [out] フィーチャカーソル
	 *	@retval true 成功
	 *	@retval fasle 失敗
	 */
	bool GetFeatureCursor(const IFeatureClassPtr& ipFeatureClass, LPCTSTR tableName, const IGeometryPtr& ipGeometry, esriSpatialRelEnum spRel, const CString& whereClause, IFeatureCursorPtr& ipFeatureCursor);

	/*
	 *	@brief フィールド値取得関数
	 *	@param	ipRow	[in] IRowPtr
	 *	@param	layerName [in] レイヤ名(エラー出力用)
	 *	@param	fieldName [in] フィールド名(エラー出力用)
	 *	@param	index [in] フィールドindex
	 *	@param	val	[out]　取得した値
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool GetValue(_IRowPtr ipRow, LPCTSTR layerName, LPCTSTR fieldName, long index, CComVariant& val);

	/*
	 *	@brief	フィールド値設定関数
	 *	@param	ipRowBuffer	[in] IRowBufferPtr
	 *	@param	layerName [in] レイヤ名(エラー出力用)
	 *	@param	fieldName [in] フィールド名(エラー出力用)
	 *	@param	index [in] フィールドindex
	 *	@param	val [in] 設定する値
	 */
	bool PutValue(IRowBufferPtr ipRowBuffer, LPCTSTR layerName, LPCTSTR fieldName, long index, CComVariant& val);


};