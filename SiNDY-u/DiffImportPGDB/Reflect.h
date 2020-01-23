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

// メッシュリストコマンド
enum COMMAND
{
	NONE = -1,   //!< 不定
	DIFF_UPDATE, //!< 差分更新
	ONLY_DELETE, //!< 削除のみ
	ONLY_ADD,    //!< 追加のみ
	DELETE_ADD,  //!< 削除＋追加(差分を見ずに-Nオプションで指定したPGDBの内容を反映)
};
typedef std::map<long, COMMAND> MESH_MAP;	///< メッシュ毎のコマンド保持用マップ
typedef std::map<long, IGeometryPtr> MESH_POLY_MAP;	///< メッシュコードと対応するメッシュポリゴンの保持用マップ

// 扱うフィーチャクラスの最大数
static const int fc_max_size = 6;
typedef std::array<CAdapt<IFeatureClassPtr>, fc_max_size> FCArray;

/**
 * @brief 変種前・編集後・SiNDYのフィールドインデックスとマージ時の比較有無を保持するための構造体
 */
struct FieldTable {
	LPCTSTR m_fieldName; //!< フィールド名
	long m_OldPgdbIndex; //!< 編集前PGDB用フィールドインデックス（通常は編集後と同じはず）
	long m_NewPgdbIndex; //!< 編集後PGDB用フィールドインデックス（通常は編集前と同じはず）
	long m_SindyIndex;   //!< SiNDY上のフィールドインデックス
	bool m_Compare;      //!< メッシュ境界マージ時に属性を比較するかのフラグ
};

/**
 * @brief 拡張子毎にフィーチャクラス名やフィールド情報等を保持するための構造体
 */
struct EnvTable {
	LPCTSTR	m_Suffix;		///< VDB時代の拡張子
	LPCTSTR	*m_OrgTableNames;	///< 対象フィーチャクラス名（群）固定
	CString	*m_TgtTableNames;	///< 反映先フィーチャクラス名（群）環境変数で変更可能
	FieldTable **m_Fields;	///< フィールドテーブル
	long m_Type;			///< 反映のタイプ（メッシュ境界の特別処理が必要かに使う）
};

enum {
	CHANGE_TYPE_NORMAL = 0,	///< 通常インポートタイプ（メッシュ境界の特殊処理あり）
	CHANGE_TYPE_POINTS = 1,	///< ポイントインポートタイプ（メッシュ境界を気にしない）
	CHANGE_TYPE_POI = 2,	///< POIタイプ（インポート条件が特殊なのでポイントタイプと分けている）
};

namespace Reflect
{
	// TODO: メンテしやすくなるようにする
	extern esriGeometryType g_ShapeType[fc_max_size];	///< フィーチャクラスのシェープ対応
	extern CString g_LayerName[fc_max_size];		///< 反映先のフィーチャクラス名（ログ出力用）
	extern long g_OperatorIndex[fc_max_size];		///< 最終更新者のフィールドインデックス
	extern long g_UpdateIndex[fc_max_size];			///< 最終更新内容のフィールドインデックス
	extern long g_PurposeIndex[fc_max_size];		///< 作業目的のフィールドインデックス
	extern long g_ClaimIndex[fc_max_size];			///< クレームフラグのフィールドインデックス
	extern long g_ProgNameIndex[fc_max_size];		///< 最終更新プログラム名のフィールドインデックス
	extern long g_ModifyDateIndex[fc_max_size];		///< 最終更新日付のフィールドインデックス
	extern long g_ProgModifyDateIndex[fc_max_size];	///< 最終プログラム更新日付のフィールドインデックス
	extern long g_SourceIndex[fc_max_size];			///< 情報ソースのフィールドインデックス
	extern long g_OrgOperatorIndex[fc_max_size];	///< ORG_OPERATOR(sindy-c)のフィールドインデックス
	extern long g_OrgModifyIndex[fc_max_size];		///< ORG_MODIFYDATE(sindy-c)のフィールドインデックス
	extern long g_OrgObjIdIndex[fc_max_size];		///< ORG_OBJID(sindy-c)のフィールドインデックス
	extern long g_UpdateCIndex[fc_max_size];		///< UPDATE_C(sindy-c)のフィールドインデックス
	extern long g_CheckCIndex[fc_max_size];			///< CHECK_C(sindy-c)のフィールドインデックス
	extern long g_MeshcodeIndex[fc_max_size];		///< MESHCODE(sindy-c)のフィールドインデックス
	extern long g_GouTypeIndex;		///< 号ポイント時限定号タイプのフィールドインデックス
	extern long g_CurrentMesh;	///< 現在処理中メッシュコード
	extern bool g_IsContinue;	///< 処理の続行フラグ（中断制御で使用）
	extern CString g_Suffix;	///< 作業中の（VDB時代の）拡張子
	extern MESH_MAP g_MeshMap;	///< メッシュリスト兼メッシュ毎のコマンドマップ
	extern MESH_POLY_MAP g_MeshPolyMap;	///< メッシュ毎のメッシュポリゴンマップ
	extern long g_NowProccess;		///< 処理中の工程管理用
	extern int g_DataIndex;		///< 処理対象を保持するテーブルのインデックス
	extern CString g_Operator;	///< セット用の最終更新者名
	
	extern const EnvTable gEnvTable[];	///< 拡張子毎にフィーチャクラス名やフィールド情報等を保持するテーブル
	extern LPCTSTR GOUTYPE_NAME;		///< 号タイプのフィールド名
	extern LPCTSTR PROG_NAME;			///< プログラム名
}

/**
 * @brief 編集差分反映を総合的に管理・実行するクラス
 */
class CReflect
{
public:
	/**
	 * @brief コンストラクタ。
	 */
	CReflect(void);

	/**
	 * @brief デストラクタ。
	 */
	~CReflect(void);

	/**
	 * @brief 初期化・事前準備。初回と2回目以降の不要部分は内部で制御。
	 *
	 * @param cConnectStr [in] サーバ接続文字列
	 * @param arg         [in] 実行時オプション
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool init(LPCTSTR cConnectStr, const CArguments& args);


	/**
	 * @brief 編集差分反映作業メイン関数。次の（以前は個別だった）機能が組み込まれている
	 *
	 * @param arg [in] 実行時オプション
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool execReflect( const CArguments& args );

	/**
	 * @brief 初期化・事前準備。初回と2回目以降の不要部分は内部で制御。
	 *
	 * @param lpszConnectStr	[in]	サーバ接続文字列
	 *
	 * @retval NULL以外	接続したワークスペース
	 * @retval NULL	失敗、エラー
	 */
	static IWorkspacePtr connectSDE(LPCTSTR lpszConnectStr);

private:
	/**
	 * @brief 環境変数から反映先のフィーチャクラス名を取得。
	 */
	void fnGetEnv(void);

	/**
	 * @brief 初回のみ必要な初期化・事前準備。フォルダの有無等一部は外で実行。
	 *
	 * @param arg       [in] 実行時オプション
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool fnFirstOnlyInit(const CArguments& args);

	/**
	 * @brief 共通で使用するフィールドインデックス取得。
	 *
	 * @param cIndex	[in]	テーブルの何番目のフィーチャクラスかを示すインデックス
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool fnGetCommonFieldIndex(int cIndex);

	/**
	 * @brief 指定フィールドのフィールドインデックスを求める
	 *
	 * @param cIndex     [in]  テーブルの何番目のフィーチャクラスかを示すインデックス
	 * @param fieldName  [in]  フィールド名
	 * @param fieldIndex [out] フィールドインデックス
	 *
	 * @retval true  フィールドインデックス取得に成功
	 * @retval false 失敗、エラー
	 */
	bool fnFindFieldIndex(int cIndex, const CString& fieldName, long& fieldIndex);

	/**
	 * @briefコマンド付きメッシュリストの読み込みとメッシュポリゴン取得。
	 *
	 * @param args	[in]	実行時オプション
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool fnMeshLoad(const CArguments& args);


	/**
	 * @brief 後処理関数（全部破棄してSDEから切断する）
	 */
	void fnUnInit(void);

private:
	CString m_OldPath;  //!< 編集前PGDBのベースパス
	CString m_NewPath;  //!< 編集後PGDBのベースパス

	IWorkspacePtr m_ipWorkspace;    //!< インポート先ワークスペース
	FCArray m_ipFeatureClasses;     //!< インポート対象フィーチャクラスの配列
	ISpatialReferencePtr m_ipSpRef; //!< 空間参照

	bool m_sindyCImport; //!< インポート先がSiNDY-cスキーマかどうか
};
