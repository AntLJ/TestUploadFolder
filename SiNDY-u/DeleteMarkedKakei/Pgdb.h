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

typedef std::set<long> ID_SET;	///< オブジェクトIDのセット

/**
 * @brief 「指定したキーワードを含む号ポイント」を含む家形を削除するクラス
 */
class CPgdb
{
public:
	/**
	 * @brief コンストラクタ。
	 */
	CPgdb(void);

	/**
	 * @brief デストラクタ。
	 */
	~CPgdb(void);

	/**
	 * @brief 初期化・事前準備。
	 *
	 * @param cTargetPath	[in]	ターゲットPGDBのベースパス
	 * @param cMarkPath		[in]	マーク用PGDBのベースパス
	 * @param cDelWord		[in]	削除用キーワード
	 * @param cDate1		[in]	日付条件１（無属性家形用）
	 * @param cDate2		[in]	日付条件２（属性付き家形用）
	 * @param pFile			[in]	ログファイル用ファイルポインタ
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool init(LPCTSTR cTargetPath, LPCTSTR cMarkPath, LPCTSTR cDelWord, LPCTSTR cDate1, LPCTSTR cDate2, FILE *pFile);

	/**
	 * @brief マーク家形削除のメイン部。
	 *
	 * @param cMeshList	[in]	メッシュリスト
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool execDeleteKakei(LPCTSTR cMeshList);

private:
	FILE* m_pFile;		///< ログファイル用ファイルポインタ
	CString m_TargetPath;	///< ターゲットPGDBのベースパス
	CString m_MarkPath;		///< マーク用PGDBのベースパス
	CComVariant m_Date[2];	///< 日付条件１，２
	CString m_Word;		///< 削除用キーワード
	long m_BuildIndex;	///< 家形属性用フィールドインデックス
	long m_ModDateIndex[3];	///< 最終更新日付用フィールドインデックス（ライン用は使わない）
	IFeatureClassPtr m_ipBuild[3];	///< 建物フィーチャクラス（Building,Building_Step,Building_Line）

	/**
	 * @brief メッシュ単位で対象家形を削除。
	 *
	 * @param cMeshCode	[in]	メッシュコード
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool fnDeleteKakeiMesh(long cMeshCode);


	/**
	 * @brief 指定メッシュコードのPGDBオープン。
	 *
	 * @param cBaseDir	[in]	ベースパス
	 * @param cMeshCode	[in]	メッシュコード
	 *
	 * @retval NULL以外		PGDBのワークスペース
	 * @retval NULL			失敗、エラー
	 */
	IWorkspacePtr fnOpenLocalDataBase(LPCTSTR cBaseDir, long cMeshCode);

	/**
	 * @brief 指定フィーチャクラスオープン。
	 *
	 * @param ipWorkspace	[in]	ワークスペース
	 * @param cLayerName	[in]	フィーチャクラス名
	 *
	 * @retval NULL以外		フィーチャクラス
	 * @retval NULL			失敗、エラー
	 */
	IFeatureClassPtr fnOpenFeatureClass(const IWorkspacePtr& ipWorkspace, LPCTSTR cLayerName);

	/**
	 * @brief 削除対象（条件を満たす）のID列抽出。
	 *
	 * @param ipGou	[in]	号ポイントフィーチャクラス
	 * @param cIds	[out]	削除対象のID列（建物・段差・ラインの３つ分）
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool fnGetDeleteTarget(const IFeatureClassPtr& ipGou, std::vector<ID_SET>& cIds);

	/**
	 * @brief １つの号ポイントから、削除対象となる建物・段差・ラインのID列の抽出。
	 *
	 * @param ipGou	[in]	号ポイントフィーチャクラス
	 * @param cIds	[out]	削除対象のID列（建物・段差・ラインの３つ分）
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool fnFindTarget(const IFeaturePtr& ipGouFeature, std::vector<ID_SET>& cIds);

	/**
	 * @brief 日付条件のチェック。
	 *
	 * @param ipFeature	[in]	調べるフィーチャ
	 * @param cIndex	[in]	フィーチャクラスを示すインデックス（0:建物,1:段差,2:ライン）
	 * @param cHaveKind	[in]	属性付き家形か（属性付きならtrue）
	 *
	 * @retval true		日付条件を満たしている（削除可）
	 * @retval false	日付条件を満たしていない（削除不可）
	 */
	bool fnDateCheck(const IFeaturePtr& ipFeature, int cIndex, bool cHaveKind);

	/**
	 * @brief 家形に含まれる段差・ラインのうち削除対象ID列を抽出。
	 *
	 * @param ipGeom	[in]	家形のジオメトリ
	 * @param cIds		[out]	削除対象のID列（建物・段差・ラインの３つ分）
	 * @param cIndex	[in]	フィーチャクラスを示すインデックス（0:建物,1:段差,2:ライン）
	 * @param cHaveKind	[in]	属性付き家形か（属性付きならtrue）
	 *
	 * @retval true		全て削除可
	 * @retval false	削除不可なものが含まれる
	 */
	bool fnFindInner(const IGeometryPtr& ipGeom, ID_SET& cIds, int cIndex, bool cHaveKind);

	/**
	 * @brief ID列指定でオブジェクトの削除。
	 *
	 * @param cIds		[in]	削除対象のID列
	 * @param cIndex	[in]	フィーチャクラスを示すインデックス（0:建物,1:段差,2:ライン）
	 *
	 * @retval true		成功
	 * @retval false	失敗・エラー
	 */
	bool fnDeleteObject(const ID_SET& cIds, int cIndex);

	/**
	 * @brief メッシュ・XY形式のログ出力。
	 *
	 * @param cLayerName	[in]	フィーチャクラス名
	 * @param ipFeature		[in]	フィーチャ（座標、ID取得用）
	 * @param cErrLv		[in]	エラーレベル（0:INFO,1:WARNING,2:ERROR）
	 * @param cMsg			[in]	同時に出力する（エラー）メッセージ
	 *
	 */
	void fnPrintLog(LPCTSTR cLayerName, const IFeaturePtr& ipFeature, int cErrLv, LPCTSTR cMsg);

	/**
	 * @brief フィーチャからログ出力用のメッシュ・XYを計算。
	 *
	 * @param ipFeature		[in]	フィーチャ
	 * @param cMesh			[out]	メッシュコード
	 * @param cX			[out]	X座標
	 * @param cY			[out]	Y座標
	 *
	 * @retval true		成功
	 * @retval false	失敗・エラー
	 */
	bool fnGetMXY(const IFeaturePtr& ipFeature, int& cMesh, int& cX, int& cY);
};
