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


/**
 * @brief メッシュ境界のマージ（不要構成点削除も取り込んだ）クラス
 */
class CMeshMerge :
	public CSDEBase
{
public:
	/**
	 * @brief コンストラクタ
	 */
	CMeshMerge(CLayerManage* cLayerManage);

	/**
	 * @brief デストラクタ
	 */
	virtual~CMeshMerge(void);

	/**
	 * @brief 初期化関数。
	 */
	bool init(void);

private:
	typedef std::set<long> KIND_SET;	///< マージ対象属性管理用セット
	typedef std::list<CString> TGT_LAYER_LIST;	///< （処理対象）レイヤ管理用リスト

	ID_SET m_IDList;		///< 処理済みID管理用セット
	KIND_SET m_BGTarget;	///< マージ除外属性管理用セット
	TGT_LAYER_LIST m_TgtList;	///< 処理対象レイヤ管理用リスト

	/**
	 * @brief メッシュ単位での境界上の不要構成点削除と境界マージ関数。
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool fnExecMesh(void);

	/**
	 * @brief レイヤ単位のメッシュ境界上の不要構成点削除関数。
	 *
	 * @param cEnv	[in]	各種情報を保持した構造体
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool fnDeleteDisusedPoint(EnvTable& cEnv);

	/**
	 * @brief フィーチャ単位のメッシュ境界上の不要構成点削除関数。
	 *
	 * @param ipFeature	[in]	対象フィーチャ
	 * @param cEnv		[in]	各種情報を保持した構造体
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool fnCheckAndDelete(IFeaturePtr& ipFeature, EnvTable& cEnv);

	/**
	 * @brief 不要構成点を削除した後の形状と削除したポイント数を求める関数(必要に応じてポリゴンをリングに分解して再構成する)。
	 *
	 * @param ipSrcGeom	[in]	対象ジオメトリ
	 * @param cCount	[out]	削除したポイントの個数
	 *
	 * @retval NULL以外	演算後のジオメトリ
	 * @retval NULL		失敗、エラー（まずないはず・・）
	 */
	IGeometryPtr fnCroppingShape(const IGeometryPtr& ipSrcGeom, int& cCount);

	/**
	 * @brief 不要構成点を削除した後の形状と削除したポイント数を求める関数（リング単位）。
	 *
	 * @param ipSrcGeom	[in]	対象ジオメトリ
	 * @param cCount	[out]	削除したポイントの個数
	 *
	 * @retval NULL以外	演算後のジオメトリ
	 * @retval NULL		失敗、エラー（まずないはず・・）
	 */
	IGeometryPtr fnCroppingRing(const IGeometryPtr& ipSrcGeom, int& cCount);

	/**
	 * @brief レイヤ単位でメッシュ境界のマージをする関数。
	 *
	 * @param cIndex	[in]	テーブルの何番目のフィーチャクラスかを示すインデックス（0～2）
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool fnShapeMerge(EnvTable& cEnv);

	/**
	 * @brief フィーチャ毎にマージ可能なものをマージする関数。
	 *
	 * @param ipSrcFeature	[in]	マージ候補フィーチャ
	 * @param cEnv			[in]	各種情報を保持した構造体
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool fnSerchTargetsAndMerge(IFeaturePtr& ipSrcFeature, EnvTable& cEnv);

	/**
	 * @brief 形状的にマージ対象になるかを判定する関数（メッシュ境界上で線分を共有していること）。
	 *
	 * @param ipSrcFeature	[in]	マージ候補フィーチャ１（先に検索された方であって、扱い上区別はない）
	 * @param ipTgtFeature	[in]	マージ候補フィーチャ２（ソースから検索された方であって、扱い以上区別はない）
	 * @param cEnv			[in]	各種情報を保持した構造体
	 *
	 * @retval  0	形状的にマージ対象ではない
	 * @retval  1	形状的にマージ対象である
	 * @retval -1	失敗、エラー
	 */
	int fnIsNeedMerge(IFeaturePtr& ipSrcFeature, IFeaturePtr& ipTgtFeature, EnvTable& cEnv);

	/**
	 * @brief 属性的にマージ対象になるかを判定する関数（決められた属性が一致すること）。
	 *
	 * @param ipSrcFeature	[in]	マージ候補フィーチャ１（先に検索された方であって、扱い上区別はない）
	 * @param ipTgtFeature	[in]	マージ候補フィーチャ２（ソースから検索された方であって、扱い以上区別はない）
	 * @param cEnv			[in]	各種情報を保持した構造体
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool fnCompAttr(IFeaturePtr& ipSrcFeature, IFeaturePtr& ipTgtFeature, EnvTable& cEnv);

	/**
	 * @brief 片方をマージした形状に更新し、片方を削除して、マージ後の状態を作る関数。
	 *
	 * @param ipSrcFeature	[in]	マージ候補フィーチャ１（先に検索された方であって、扱い上区別はない）
	 * @param ipTgtFeature	[in]	マージ候補フィーチャ２（ソースから検索された方であって、扱い以上区別はない）
	 * @param cEnv			[in]	各種情報を保持した構造体
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool fnMerge(IFeaturePtr& ipSrcFeature, IFeaturePtr& ipTgtFeature, EnvTable& cEnv);


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
	 * @brief マージ対象属性かを判定する関数(中縮背景固定になってしまったので、対象が増えたら調整が必要)
	 *
	 * @param ipFeature		[in]	属性を調べるフィーチャ
	 * @param cEnv			[in]	各種情報を保持した構造体
	 *
	 * @retval true		マージ対象属性である
	 * @retval false	マージ対象属性ではない
	 */
	bool fnIsTargetKind(const IFeaturePtr& ipFeature, EnvTable& cEnv);

	/**
	 * @brief 指定フィーチャの指定属性が除外マージ属性かを判定する関数（中縮背景限定）
	 *
	 * @param ipFeature		[in]	属性を調べるフィーチャ
	 * @param cIndex		[in]	調べる属性のフィールドインデックス
	 *
	 * @retval true		マージ対象属性である
	 * @retval false	マージ対象属性ではない
	 */
	bool fnIsTargetBaseKind(const IFeaturePtr& ipFeature, long cIndex);

};
