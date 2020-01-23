#pragma once

/**
 * @brief データ変換用の基底クラス
 */
class CConvBase
{
public:
	CConvBase() : m_bFeatureClass(false){}
	virtual ~CConvBase(){
	}

public:
	/** 
	 * @brief 初期化用
	 * @retval true 処理成功
	 * @retval false 処理失敗
	 */
	bool init();

	/** 
	 * @brief データ変換用
	 * @retval true 処理成功
	 * @retval false 処理失敗
	 */
	bool convert();

protected:
	/**
	 * @brief 変換元テーブルからデータ読み込み（純粋仮想関数）
	 */
	virtual bool loadData() = 0;

	/**
	 * @brief 特殊変換（仮想関数）
	 * @retval true 処理成功
	 * @retval false 処理失敗
	 */
	virtual bool convSpecial();

	/**
	 * @brief Shapefileへデータ出力（純粋仮想関数）
	 */
	virtual bool outputData() = 0;

	/** 
	 * @brief フィールドインデックス作成（純粋仮想関数）
	 */
	virtual bool createFieldIndex() = 0;

	/** 
	 * @brief 出力先Shapefileのフィールド設定（純粋仮想関数）
	 */
	virtual bool createFields(IFieldsPtr& ipFields) = 0;

protected:
	/** 
	 * @brief 出力先のShapefile作成
	 * @retval true 処理成功
	 * @retval false 処理失敗
	 */
	bool createShapefile();

	/**
	 * @brief 属性フィールド追加
	 * @note 対応しているのは文字列型、整数型のみ
	 * @param strTableName [in] テーブル名（ログ出力用）
	 * @param strFieldName [in] フィールド名
	 * @param eFieldType [in] フィールドタイプ
	 * @param vaIsEditable [in] 編集許可
	 * @param vaIsNullable [in] NULL許可
	 * @param lLength [in] 長さ
	 * @param lPrecision [in] 精度
	 * @param ipFields [out] フィールド群
	 * @retval true 処理成功
	 * @retval false 処理失敗
	 */
	bool addAttrField(const CString&strTableName, const CString& strFieldName, esriFieldType eFieldType, VARIANT_BOOL vaIsEditable, VARIANT_BOOL vaIsNullable, long lLength, long lPrecision, IFieldsPtr& ipFields);

	/**
	 * @brief 形状フィールド追加
	 * @param strTableName [in] テーブル名（ログ出力用）
	 * @param eGeoType [in] 形状タイプ
	 * @param ipFields [out] フィールド群
	 * @retval true 処理成功
	 * @retval false 処理失敗
	 */
	bool addShapeField(const CString&strTableName, esriGeometryType eGeoType, IFieldsPtr& ipFields);

	/**
	 * @brief フィールドインデックス取得
	 * @param strTableName [in] テーブル名（ログ出力用）
	 * @param ipTable [in] テーブル
	 * @param mapFieldIndex [out] フィールドインデックス
	 * @retval true 処理成功
	 * @retval false 処理失敗
	 */
	bool setFieldIndex(const CString&strTableName, const ITablePtr& ipTable, std::map<CString,long>& mapFieldIndex);

	/**
	 * @brief 既存のShapefile削除用
	 * @retval true 処理成功
	 * @retval false 処理失敗
	 */
	bool deleteShapefile();

	/**
	 * @brief テーブル削除
	 * @note フィーチャクラスでも削除可能
	 * @param ipOutdirWork [in] 削除対象のテーブルを持つワークスペース
	 * @param strTableName [in] テーブル名
	 * @retval true 処理成功
	 * @retval false 処理失敗
	 */
	bool deleteTable(const IWorkspacePtr ipOutdirWork, const CString& strTableName);

protected:
	CString m_strOutDir;			//!< 出力先ディレクトリ
	IWorkspacePtr m_ipOutWork;		//!< 出力先ディレクトリのワークスペース
	ITablePtr m_ipOutShapefile;		//!< 出力Shapefileテーブル（フィーチャクラス）

	bool m_bFeatureClass;			//!< 出力Shapefileがフィーチャクラスかどうか
	CString m_strShapefileName;		//!< 出力Shapefile名
	std::map<CString,long> m_mapFieldIndexOutShapefile;	//!< 変換先（Shapefile）テーブルのフィールドインデックス
};
