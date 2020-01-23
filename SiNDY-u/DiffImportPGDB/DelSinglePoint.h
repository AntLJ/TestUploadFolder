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
 * @brief 浮いた構成点を調べ易くするための構造体
 *        連続する3点を保持
 */
struct CheckPoint
{
	WKSPoint m_Center;
	WKSPoint m_Left;
	WKSPoint m_Right;
	bool operator<(const CheckPoint &cCmp) const{
		if(m_Center.X != cCmp.m_Center.X){
			return m_Center.X < cCmp.m_Center.X;
		}else{
			return m_Center.Y < cCmp.m_Center.Y;
		}
	}
};

typedef std::set<CheckPoint, std::less<CheckPoint>> POINT_SET;	///< 順序付けて点列を比較するためのセット

/**
 * @brief 差分反映を一通り終えた後に対応が取れなくなった構成点を削除するクラス
 *        道路と街区等、隣接するポリゴンに対し、片方しか変更が行われなかったことから、
 *        メッシュ境界上の構成点に不整合を生じる（変更が行われた方には新たに構成点ができる）ことがある。
 *        これを解消するため、連続する3点の真ん中がメッシュ境界上の点で、その両端が別のメッシュにある場合、
 *        他のポリゴンの構成点で、同じものがあるかを調べ、なければ削除する。
 */
class CDelSinglePoint :
	public CSDEBase
{
public:
	/**
	 * @brief コンストラクタ。
	 */
	CDelSinglePoint(void);

	/**
	 * @brief デストラクタ。
	 */
	~CDelSinglePoint(void);

	/**
	 * @brief 初期化関数。
	 */
	bool init(const CArguments& args, const IWorkspacePtr& ipWorkspace,
		const ISpatialReferencePtr& ipSpRef, const FCArray& ipFeatureClasses, bool isSindyCImport );

private:
	ISpatialReferencePtr m_ipSpRef;


	/**
	 * @brief メッシュ単位での不要構成点削除（その２）関数。
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
	 * @brief ポリゴンの削除候補点とその前後の点の情報を取得する関数。
	 *
	 * @param ipGeom		[in]	対象ジオメトリ
	 * @param cCheckPoints	[out]	削除候補ポイント群
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool fnGetCheckPoints(IGeometryPtr ipGeom, POINT_SET& cCheckPoints);

	/**
	 * @brief 削除候補ポイントの前後からなるラインを持っているかチェックして、削除対象ポイントを求める関数。
	 *
	 * @param ipFeature		[in]	対象ジオメトリ
	 * @param cCheckPoints	[in]	削除候補ポイント群
	 * @param cDeletePoints	[out]	削除対象ポイント群
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool fnFindPairLine(IFeaturePtr ipFeature, const POINT_SET& cCheckPoints, POINT_SET& cDeletePoints);

	/**
	 * @brief 不要構成点を削除した後の形状を求める関数(必要に応じてポリゴンをリングに分解して再構成する)。
	 *
	 * @param ipSrcGeom		[in]	対象ジオメトリ
	 * @param cDeletePoints	[in]	削除対象ポイント群
	 * @param cObjID		[in]	対象オブジェクトID
	 * @param cIndex		[in]	テーブルの何番目のフィーチャクラスかを示すインデックス
	 *
	 * @retval NULL以外	演算後のジオメトリ
	 * @retval NULL		失敗、エラー（まずないはず・・）
	 */
	IGeometryPtr fnCroppingShape(IGeometryPtr ipSrcGeom, const POINT_SET& cDeletePoints, long cObjID, long cIndex);

	/**
	 * @brief （ポリゴンを分解した）リングの削除候補点とその前後の点の情報を取得する関数。
	 *
	 * @param ipGeom		[in]	対象ジオメトリ
	 * @param cCheckPoints	[out]	削除候補ポイント群
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool fnCheckRing(IGeometryPtr ipGeom,  POINT_SET& cCheckPoints);

	/**
	 * @brief 2点のWKSPoint一致確認関数。
	 *
	 * @param cPoint1		[in]	比較するポイント１
	 * @param cPoint2		[in]	比較するポイント２
	 *
	 * @retval true		一致
	 * @retval false	不一致
	 */
	bool fnIsEqual(const WKSPoint& aPoint1, const WKSPoint& aPoint2);

	/**
	 * @brief 不要構成点を削除した後の形状を求める関数(リング単位)。
	 *
	 * @param ipSrcGeom		[in]	対象ジオメトリ
	 * @param cDeletePoints	[in]	削除対象ポイント群
	 * @param cObjID		[in]	対象オブジェクトID
	 * @param cIndex		[in]	テーブルの何番目のフィーチャクラスかを示すインデックス
	 *
	 * @retval NULL以外	演算後のジオメトリ
	 * @retval NULL		失敗、エラー（まずないはず・・）
	 */
	IGeometryPtr fnCroppingRing(IGeometryPtr ipSrcGeom, const POINT_SET& cDeletePoints, long cObjID, long cIndex);

	/**
	 * @brief ポイント（座標）から都市地図のメッシュコードを計算する関数
	 *
	 * @param cPoint		[in]	メッシュコードを求めるポイント（座標）
	 *
	 * @retval 都市地図のメッシュコード
	 */
	long fnCalcMeshCode(const WKSPoint& cPoint);
};
