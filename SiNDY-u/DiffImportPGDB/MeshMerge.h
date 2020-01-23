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
#include "sdebase.h"


typedef std::set<long> KIND_SET;  //!< マージ対象属性管理用セット

/**
 * @brief メッシュ境界のマージ（不要構成点削除も取り込んだ）クラス
 */
class CMeshMerge :
	public CSDEBase
{
public:
	/**
	 * @brief コンストラクタ。
	 */
	CMeshMerge();

	/**
	 * @brief デストラクタ。
	 */
	~CMeshMerge(void);

	/**
	 * @brief 初期化関数。
	 */
	bool init(const CArguments& args, const IWorkspacePtr& ipWorkspace,
		const ISpatialReferencePtr& ipSpRef, const FCArray& ipFeatureClasses, bool isSindyCImport );

private:
	// 扱うフィーチャクラスの最大数が変わったら、以下のメンバ変数も合わせて変更すること
	// TODO: どこかで一括管理できるようにするなり、メンテしやすくする
	long m_FloorsIndex[fc_max_size];  //!< 建物階数フィールドインデックス
	long m_BGClsIndex[fc_max_size];   //!< 背景ポリゴン属性フィールドインデックス
	long m_AreaClsIndex[fc_max_size]; //!< 都市エリア属性フィールドインデックス
	long m_AdmClsIndex[fc_max_size];  //!< 行政界エリアクラスフィールドインデックス
	long m_CLClsIndex[fc_max_size];  //!< 背景ライン属性フィールドインデックス
	KIND_SET m_CSTarget;    //!< マージ対象属性管理用セット(背景ポリゴン)
	KIND_SET m_CLTarget;    //!< マージ対象属性管理用セット(背景ライン)
	KIND_SET m_APTarget;    //!< マージ対象属性管理用セット(CITY_AREA)
	KIND_SET m_ADMTarget;   //!< マージ対象属性管理用セット(行政界)
	ID_SET m_mergedIDList;  //!< マージ処理済みオブジェクトID
	ID_SET m_deleteIDList;  //!< 削除対象オブジェクトID

	/**
	 * @brief メッシュ単位での境界上の不要構成点削除と境界マージ関数。
	 *
	 * @param cCommand		[in]	反映用コマンド（未使用）
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool fnExecMesh(COMMAND cCommand);

	/**
	 * @brief レイヤ単位のメッシュ境界上の不要構成点削除関数。
	 *
	 * @param cIndex	[in]	テーブルの何番目のフィーチャクラスかを示すインデックス
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool fnDeleteDisusedPoint(long cIndex);

	/**
	 * @brief フィーチャ単位のメッシュ境界上の不要構成点削除関数。
	 *
	 * @param ipFeature	[in]	対象フィーチャ
	 * @param cIndex	[in]	テーブルの何番目のフィーチャクラスかを示すインデックス
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool fnCheckAndDelete(IFeaturePtr ipFeature, long cIndex);

	/**
	 * @brief 不要構成点を削除した後の形状と削除したポイント数を求める関数(必要に応じてポリゴンをリングに分解して再構成する)。
	 *
	 * @param ipSrcGeom	[in]	対象ジオメトリ
	 * @param cCount	[out]	削除したポイントの個数
	 *
	 * @retval NULL以外	演算後のジオメトリ
	 * @retval NULL		失敗、エラー（まずないはず・・）
	 */
	IGeometryPtr fnCroppingShape(IGeometryPtr ipSrcGeom, int& cCount);

	/**
	 * @brief 不要構成点を削除した後の形状と削除したポイント数を求める関数（リング単位）。
	 *
	 * @param ipSrcGeom	[in]	対象ジオメトリ
	 * @param cCount	[out]	削除したポイントの個数
	 *
	 * @retval NULL以外	演算後のジオメトリ
	 * @retval NULL		失敗、エラー（まずないはず・・）
	 */
	IGeometryPtr fnCroppingRing(IGeometryPtr ipSrcGeom, int& cCount);

	/**
	 * @brief レイヤ単位でメッシュ境界のマージをする関数。
	 *
	 * @param cIndex	[in]	テーブルの何番目のフィーチャクラスかを示すインデックス
	 * @param ipCursor	[in]	メッシュ内のフィーチャを空間検索した結果のフィーチャカーソル
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool fnShapeMerge(long cIndex/*, const IFeatureCursorPtr& ipCursor*/);

	/**
	 * @brief フィーチャ毎にマージ可能なものをマージする関数。
	 *
	 * @param ipSrcFeature	[in]	マージ候補フィーチャ
	 * @param cIndex		[in]	テーブルの何番目のフィーチャクラスかを示すインデックス
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool fnSearchTargetsAndMerge(IFeaturePtr ipSrcFeature, long cIndex);

	/**
	 * @brief 形状的にマージ対象になるかを判定する関数（メッシュ境界上で線分を共有していること）。
	 *
	 * @param ipSrcFeature	[in]	マージ候補フィーチャ１（先に検索された方であって、扱い上区別はない）
	 * @param ipTgtFeature	[in]	マージ候補フィーチャ２（ソースから検索された方であって、扱い以上区別はない）
	 * @param cIndex		[in]	テーブルの何番目のフィーチャクラスかを示すインデックス
	 *
	 * @retval  0	形状的にマージ対象ではない
	 * @retval  1	形状的にマージ対象である
	 * @retval -1	失敗、エラー
	 */
	int fnIsNeedMerge(IFeaturePtr ipSrcFeature, IFeaturePtr ipTgtFeature, long cIndex);

	/**
	 * @brief 属性的にマージ対象になるかを判定する関数（決められた属性が一致すること）。
	 *
	 * @param ipSrcFeature	[in]	マージ候補フィーチャ１（先に検索された方であって、扱い上区別はない）
	 * @param ipTgtFeature	[in]	マージ候補フィーチャ２（ソースから検索された方であって、扱い以上区別はない）
	 * @param cIndex		[in]	テーブルの何番目のフィーチャクラスかを示すインデックス
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool fnCompAttr(IFeaturePtr ipSrcFeature, IFeaturePtr ipTgtFeature, long cIndex);

	/**
	 * @brief 片方をマージした形状に更新し、片方を削除して、マージ後の状態を作る関数。
	 *
	 * @param ipSrcFeature	[in]	マージ候補フィーチャ１（先に検索された方であって、扱い上区別はない）
	 * @param ipTgtFeature	[in]	マージ候補フィーチャ２（ソースから検索された方であって、扱い以上区別はない）
	 * @param cIndex		[in]	テーブルの何番目のフィーチャクラスかを示すインデックス
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool fnMerge(IFeaturePtr ipSrcFeature, IFeaturePtr ipTgtFeature, long cIndex);


	/**
	 * @brief 属性的にマージ対象になるかを判定する関数（建物高さを特別扱いしたもの）。
	 *
	 * @param ipSrcFeature	[in]	マージ候補フィーチャ１（先に検索された方であって、扱い上区別はない）
	 * @param ipTgtFeature	[in]	マージ候補フィーチャ２（ソースから検索された方であって、扱い以上区別はない）
	 * @param cIndex		[in]	テーブルの何番目のフィーチャクラスかを示すインデックス
	 *
	 * @retval -1以外	高い方の建物高さ（階数以外の対象属性が一致している）
	 * @retval -1		階数以外も属性に違うものがある
	 */
	int fnCompAttrSp(IFeaturePtr ipSrcFeature, IFeaturePtr ipTgtFeature, long cIndex);

	/**
	 * @brief 許容範囲を持たせた座標（値）の一致確認関数。
	 *
	 * @param cPoint1		[in]	比較する数字１（座標）
	 * @param cPoint2		[in]	比較する数字２（座標）
	 * @param cAcceptable	[in]	同一とみなす際の許容できる最大値
	 *
	 * @retval true		一致
	 * @retval false	不一致
	 */
	bool fnCompXY(double cPoint1, double cPoint2, double cAcceptable);

	/**
	 * @brief マージ対象属性かを判定する関数。
	 *
	 * @param ipFeature [in] 属性を調べるフィーチャ
	 * @param cIndex    [in] テーブルの何番目のフィーチャクラスかを示すインデックス
	 *
	 * @retval true		マージ対象属性である
	 * @retval false	マージ対象属性ではない
	 */
	bool fnIsTargetKind(const IFeaturePtr& ipFeature, const long cIndex);
};
