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

/**
 * @brief フィールドインデックスとエイリアスを保持する構造体
 *
 */
struct FieldInfo{
	long m_Index;	///< フィールドインデックス
	CString m_Alias;	///< エイリアス（フィールドの日本語表示用）
	/**
	 * @brief コンストラクタ
	 */
	FieldInfo(void)
	{
	}
	/**
	 * @brief コンストラクタ
	 */
	FieldInfo(long cIndex, LPCTSTR cAlias)
	{
		m_Index = cIndex;
		m_Alias = cAlias;
	}
};

typedef std::set<long> MESH_SET;	///< メッシュリスト保持用セット
typedef std::map<CString, FieldInfo> INDEX_MAP;	///< フィールド名とフィールドインデックス対応用マップ

/**
 * @brief フィーチャクラス単位で、出力に必要な情報を保持する構造体（一般フィーチャクラス用）
 *        IDの付け替えがある（かもしれない）
 *        TODO: PGDBは2つ存在するのにインデックスのmapは1個しかないので、新旧PGDBのフィールドのつくりが違うと危ないため、要修正
 */
struct EnvTable {
	CString	m_Tag;	///<　データ識別：主に進捗表示用（固定値で初期化）
	CString	m_OrgTable;	///< PGDBのフィーチャクラス名兼レイヤ名判別のタグ（固定値で初期化：ユーザ名も含まず、不変）
	CString m_SdeTable;		///< SDEのフィーチャクラス名（1実行当たり1回初期化:ユーザ名が付いたりすることもある）
	INDEX_MAP m_SDEIndexMap;	///< SDEのフィールド名とフィールドインデックスの関係（1実行当たり1回初期化）
	INDEX_MAP m_PGDBIndexMap;	///< PGDBのフィールド名とフィールドインデックスの関係（念のため各メッシュごとに初期化）
	bool m_IsImport;	///< 処理対象かのフラグ(未使用：不要になるかも)
	//NAME_SET m_ExcludeFields;	///< フィールド除外リスト
	EnvTable(LPCTSTR cTag, LPCTSTR cTableName){
		m_Tag = cTag;
		m_OrgTable = m_SdeTable = cTableName;	//初期値は一緒
		m_IsImport = true;	//デフォルトで「処理する」でいいのかは後で検討
	}
};

/**
 * @brief リンク列の単位（LQとINF）で、出力に必要な情報を保持する構造体）
 */
struct LqEnvTable {
	CString	m_Tag;	///<　データ識別：主に進捗表示用（固定値で初期化）
	CString	m_OrgTables[2];	///< PGDBのテーブル名兼レイヤ名判別のタグ（固定値で初期化：ユーザ名も含まず、不変）
	CString m_SdeTables[2];	///< SDEのテーブル名（1実行当たり1回初期化:ユーザ名が付いたりすることもある）
	INDEX_MAP m_SDEIndexMap[2];	///< SDEのフィールド名とフィールドインデックスの関係（1実行当たり1回初期化）
	INDEX_MAP m_PGDBIndexMap[2];	///< PGDBのフィールド名とフィールドインデックスの関係（念のため各メッシュごとに初期化）
	LqEnvTable(LPCTSTR cTag, LPCTSTR cInfName, LPCTSTR cLqName){
		m_Tag = cTag;
		m_OrgTables[0] = m_SdeTables[0] = cInfName;	//初期値は一緒
		m_OrgTables[1] = m_SdeTables[1] = cLqName;	//初期値は一緒
	}
};

/**
 * @brief ユーザ名付きテーブル名とテーブル（又はフィーチャクラス）の関連保持用構造体
 */
struct LayerInfo{
	CString m_LayerName;	///< （ユーザ名付き）テーブル名
	ITablePtr m_ipTable;	///< テーブル・フィーチャクラス
};

typedef std::map<CString, LayerInfo> TABLE_INFO;	///< レイヤ名とユーザ名付きテーブル名（ログ出力用）・テーブルのマップ

/**
 * @brief テーブル・フィーチャクラスと処理順管理用クラス
 */
class CLayerManage
{
public:
	enum CONNECT_DB {	///< ワークスペースの識別用
		DB_ROAD = 0,	///< 一時停止以外の一般
		DB_STOP=1		///< 一時停止用
	};
	enum TABLE_TYPE {	///< 管理上のテーブルの分類識別用
		FC_TABLE = 0,	///< （一時停止以外の）フィーチャクラス
		EXT_FC_TABLE = 1,	///< 現状、一時停止のみ
		OTHER_TABLE = 2		///< その他テーブル（リンク列関連は別管理）
	};

	/**
	 * @brief コンストラクタ
	 */
	CLayerManage(void);

	/**
	 * @brief デストラクタ
	 */
	~CLayerManage(void);

	/**
	 * @brief 初期化
	 *
	 * @param cConnectStr		[in]	サーバ接続用文字列
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool init(const CString cConnectStr[2]);

	/**
	 * @brief SDEへの接続
	 *
	 * @param lpszConnectStr	[in]	サーバ接続文字列
	 *
	 * @retval NULL以外	接続したワークスペース
	 * @retval NULL	失敗、エラー
	 */
	static IWorkspacePtr connectSDE(LPCTSTR lpszConnectStr);

	/**
	 * @brief 属性コピー対象のフィールド名とSDEのフィールドインデックスを取得
	 *
	 * @param ipTable		[in]	情報を取得するテーブル
	 * @param cIndexMap		[out]	取得したフィールドの情報
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	static bool getFieldInfo(const ITablePtr& ipTable, INDEX_MAP& cIndexMap);

	/**
	 * @brief 識別用テーブル名称から対応するテーブルを取得
	 *
	 * @param cLayerName		[in]	取得するテーブルの名称
	 *
	 * @retval NULL以外	対応するテーブル
	 * @retval NULL	該当なし（バグのはず）
	 */
	ITablePtr findTable(LPCTSTR cLayerName) const;

	/**
	 * @brief 識別用テーブル名称から対応する実際の（ユーザ名付きとかの）テーブル名称を取得
	 *
	 * @param cLayerName		[in]	取得するテーブルの名称
	 *
	 * @retval NULL以外			対応するテーブル名称
	 * @retval "不明（バグ）"	該当なし
	 */
	LPCTSTR findTableName(LPCTSTR cLayerName) const;

	/**
	 * @brief 編集（反映）対象のワークスペースを取得
	 *
	 * @retval 編集対象のワークスペース
	 */
	IWorkspacePtr getEditWorkspace(void) const;

	/**
	 * @brief テーブルの種類毎に最初の処理対象の情報を取得
	 *
	 * @param cType		[in]	取得するテーブルの種類
	 *
	 * @retval NULL以外	該当する必要な情報を持つ構造体へのポインタ
	 * @retval NULL		該当なし（バグのはず）
	 */
	EnvTable* getFitstEnv(TABLE_TYPE cType);

	/**
	 * @brief テーブルの種類毎に次の処理対象の情報を取得
	 *
	 * @param cType		[in]	取得するテーブルの種類
	 *
	 * @retval NULL以外	該当する必要な情報を持つ構造体へのポインタ
	 * @retval NULL		該当なし（バグのはず）
	 */
	EnvTable* getNextEnv(TABLE_TYPE cType);

	/**
	 * @brief 識別用テーブル名称から対応する情報を取得
	 *
	 * @param cLayerName	[in]	識別用テーブル名称
	 *
	 * @retval NULL以外	該当する必要な情報を持つ構造体へのポインタ
	 * @retval NULL		該当なし（バグのはず）
	 */
	EnvTable* findEnvTable(LPCTSTR cLayerName) const;

	/**
	 * @brief リンク列で最初の処理対象の情報を取得
	 *
	 * @retval NULL以外	該当する必要な情報を持つ構造体へのポインタ
	 * @retval NULL		該当なし（バグのはず）
	 */
	LqEnvTable* getFitstLQEnv(void);

	/**
	 * @brief リンク列で次の処理対象の情報を取得
	 *
	 * @retval NULL以外	該当する必要な情報を持つ構造体へのポインタ
	 * @retval NULL		該当なし（バグのはず）
	 */
	LqEnvTable* getNextLQEnv(void);

	/**
	 * @brief 後処理関数（全部破棄してSDEから切断する）
	 */
	void unInit(void);

	/**
	 * @brief プログラム内共通の空間参照を取得
	 *
	 * @retval NULL以外	空間参照
	 * @retval NULL		バグのはず
	 */
	ISpatialReferencePtr get_SpRef(void) const
	{
		return m_ipSpRef;
	}

private:
	IWorkspacePtr m_ipWorkspace[2];		///< 反映先ワークスペース
	ISpatialReferencePtr m_ipSpRef;	///< 共通の空間参照（演算しないうちは未使用）
	bool m_IsInitial;		///< 初期化済みかのフラグ
	long m_FcIndex;		///< 「FC_TABLE」内の現在処理中を指すインデックス
	long m_ExtFcIndex;		///< 「EXT_FC_TABLE」内の現在処理中を指すインデックス
	long m_OtherIndex;		///< 「OTHER_TABLE」内の現在処理中を指すインデックス
	long m_LqIndex;		///< リンク列管理用テーブル内の現在処理中を指すインデックス
	CString m_MeshLayer;	///< メッシュポリゴンのフィーチャクラス名（オープン用）

	static EnvTable m_FcEnvTable[];	///< フィーチャクラス名やフィールド情報等を保持するテーブル
	static EnvTable m_ExtFcEnvTable[];	///< フィーチャクラス名やフィールド情報等を保持するテーブル(現状は一時停止専用)
	static LqEnvTable m_LqEnvTable[];	///< テーブル名やフィールド情報等を保持するテーブル(リンク列用)
	static EnvTable m_OtherTable[];	///< テーブル名やフィールド情報等を保持するテーブル(その他テーブル系)

	TABLE_INFO m_TableInfo;	///< レイヤ名とユーザ名付きテーブル名（ログ出力用）・テーブルのマップ（グローバルやstaticでCOMを使うと後処理が面倒なのでここで別管理）

	/**
	 * @brief SDEのテーブル・フィーチャクラス（リンク列関連以外の全部）のオープンと各種準備
	 *
	 * @param cIsEdit	[in]	編集対象かのフラグ（バージョン対応である必要があるかのチェックをするか）
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool fnOpenSDELayers(bool cIsEdit);

	/**
	 * @brief 環境変数から設定を反映
	 */
	void fnGetEnv(void);

	/**
	 * @brief SDEのフィーチャクラスのオープンと各種準備（1個分）
	 *
	 * @param ipFeatureWorkspace	[in]		ワークスペース
	 * @param cEnv					[in/out]	各種情報を保持した構造体
	 * @param cIsEdit				[in]		編集対象かのフラグ（falseならバージョン対応かを見ない：2回目の時も使用）
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool fnOpenSDEFeatureClass(const IFeatureWorkspacePtr& ipFeatureWorkspace, EnvTable& cEnv, bool cIsEdit);

	/**
	 * @brief SDEのリンク列関連テーブル全部のオープンと各種準備
	 *
	 * @param ipFeatureWorkspace	[in]		ワークスペース
	 * @param cEnv					[in/out]	各種情報を保持した構造体
	 * @param cIsEdit				[in]		編集対象かのフラグ（falseならバージョン対応かを見ない：2回目の時も使用）
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool fnOpenSDETables(const IFeatureWorkspacePtr& ipFeatureWorkspace, LqEnvTable& cEnv, bool cIsEdit);

	/**
	 * @brief SDEのリンク列関連テーブルのオープンと各種準備（1個分）
	 *
	 * @param ipFeatureWorkspace	[in]	ワークスペース
	 * @param cTableName			[in]	識別用テーブル名称
	 * @param cIndexMap				[out]	フィールドインデックスの情報
	 * @param cIsEdit				[in]	編集対象かのフラグ（falseならバージョン対応かを見ない：2回目の時も使用）
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool fnOpenSDETable(const IFeatureWorkspacePtr& ipFeatureWorkspace, LPCTSTR cTableName, INDEX_MAP& cIndexMap, bool cIsEdit);

	/**
	 * @brief メッシュポリゴンオープン
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool fnOpenMeshPoly(void);

	/**
	 * @brief 識別用テーブル名称とテーブルを関連づけて管理（登録）
	 *
	 * @param ipTable		[in]	対応するテーブル
	 * @param cTableName	[in]	識別用テーブル名称
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool fnRegistTable(const ITablePtr& ipTable, LPCTSTR cTableName);
};
