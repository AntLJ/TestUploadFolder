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

/**
 * @file RelAnnotationParameter.h
 * @brief <b>CRelAnnotationParameterクラス定義ヘッダファイル</b>
 * @author 地図ＤＢ制作部システム開発グループ 古川貴宏
 * @vertion $Id$
 */
#ifndef _RELANNOTATIONPARAMETER_H_
#define _RELANNOTATIONPARAMETER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RelAnnotation.h"

namespace sindy {

#ifdef SINDYLIB_CALC_TIME
extern SINDYLIB_API double g_dCRelAnnotationParameter_CopyTime;		//!< オブジェクトコピー時間
extern SINDYLIB_API double g_dCreateParameterCache_AllTime;			//!< CreateParameterCacheの総時間
extern SINDYLIB_API double g_dCreateParameterCache_RuleCacheTime;	//!< CreateParameterCacheのルールキャッシュ時間
extern SINDYLIB_API double g_dCreateParameterCache_AnnotationTime;	//!< CreateParameterCacheの注記真位置記号矩形作成時間
extern SINDYLIB_API double g_dCreateParameterCache_DispLineTime;	//!< CreateParameterCacheの注記表示位置バウンディングボックス作成時間
#endif // ifdef SINDYLIB_CALC_TIME

class CRelAnnotationParameter;
typedef boost::shared_ptr<CRelAnnotationParameter> CSPRelAnnotationParameter;

/**
 * @class CRelAnnotationParameter
 * @brief <b>パラメータキャッシュ機能付き注記クラス</b>\n
 * このクラスはCRelAnnotationの機能に加え、各種パラメータをキャッシュする
 * 機能を有します。チェッカーなどで高速かつ大量に処理する必要がある場合に
 * 使用してください。
 *
 * キャッシュするパラメータは以下のとおりです：
 * @li
 * @li
 * @li
 */
class SINDYLIB_API CRelAnnotationParameter : public CRelAnnotation
{
	ROWCLASS_CONSTRUCTOR( CRelAnnotationParameter, unknown );
	/**
	 * @brief <b>コピーする</b>\n
	 * destにキャッシュがある場合は、削除されます。
	 * キャッシュがある場合は、キャッシュも一緒にコピーされます。
	 * @see CRow::CopyTo
	 * @param dest				[in]	ペースト対象（CRelAnnotationParameter限定です）
	 * @param bIsCopyCache		[in]	キャッシュをコピーするかどうか
	 * @param bIsCopyRow		[in]	オブジェクトをコピーするかどうか
	 * @param bIsCopyFlag		[in]	作成削除フラグをコピーするかどうか
	 * @param bIsCopyContainer	[in]	コンテナをコピーするかどうか
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode CopyTo( CRow& dest, bool bIsCopyCache=true, bool bIsCopyRow=true, bool bIsCopyFlag=true, bool bIsCopyContainer=true ) const;

	/**
	 * @brief <b>パラメータキャッシュを削除する</b>\n
	 */
	void DeleteParameter();

	/**
	 * @brief <b>パラメータキャッシュを作成する</b>\n
	 * @param bForce	[in,optional]	強制的に作り直す場合はtrue（デフォルト：false）
	 */
	errorcode::sindyErrCode UpdateParameter( bool bForce = false );

	/**
	 * @brief <b>パラメータキャッシュが作成されているかどうかをチェックする</b>\n
	 */
	bool IsCreatedParameterCache() { return m_bIsCached; }

	//@{ @name チェックルーチン
	/**
	 * @brief <b>真位置と表示位置のセットでの単体・複合チェックを行う</b>\n
	 * 都市地図以外は、S1からS4までチェックされます。\n\n
	 * チェックされる項目は以下のとおり：\n
	 * @li 単体チェック（bDoUnitCheckがtrueの時のみ）
	 *  @li 真位置単体チェック（CAnnotaion::Check参照）
	 *  @li 表示位置単体チェック（CDispLine::Check参照）
	 * @li 複合チェック（bDoComplexCheckがtrueの時のみ）
	 *  @li 真位置から表示文字列ポリゴンまでの距離チェック
	 *  @li 真位置と表示文字列ポリゴンの包含関係が正しいかどうかのチェック
	 *  @li 真位置と表示位置のラインタイプの整合性チェック
	 * @param cErrObjects		[out]			エラーオブジェクトのリスト
	 * @param bDoUnitCheck		[in,optional]	単体チェックをするかどうか
	 * @param bDoComplexCheck	[in,optional]	複合チェックをするかどうか
	 * @param dThresholdMinPx	[in,optional]	最小閾値（単位：ピクセル、マイナス可、デフォルト：0.0）
	 * @param dThresholdMaxPx	[in,optional]	最大閾値（単位：ピクセル、デフォルト：16.0）
	 * @retval sindyErrCode 最後に見つかったエラー
	 */
	errorcode::sindyErrCode CheckLogic( CErrorObjects& cErrObjects, bool bDoUnitCheck = true, bool bDoComplexCheck = true, const double& dThresholdMinPx = 0.0, const double& dThresholdMaxPx = 16.0 ) const;
	//@}
	//@{ @name イテレータ
	typedef std::list<WKSEnvelope>::iterator annorect_iterator;
	typedef std::list<WKSEnvelope>::const_iterator annorect_const_iterator;
	annorect_iterator annorect_begin() { return m_listAnnoRect.begin(); }
	annorect_iterator annorect_end() { return m_listAnnoRect.end(); }
	annorect_const_iterator annorect_begin() const { return m_listAnnoRect.begin(); }
	annorect_const_iterator annorect_end() const { return m_listAnnoRect.end(); }
	//@}

	/**
	 * @brief <b>現在格納されているフィーチャの形状のコピーを返す</b>\n
	 * @param bLatest               	[in,optional] 最新の形状が欲しい場合はtrue（デフォルト：true）
	 * @param bOriginal             	[in,optional] 変更前の形状が欲しい場合はtrue（デフォルト：false）
	 * @param bFeatureChangedOriginal	[in,optional] IFeatureChanges::get_OriginalShapeの形状が欲しい場合にはtrue（デフォルト：false）
	 * @param bDiff				   		[in,optional] 複数状態を指定した時に、Unionした形状ではなく差分形状にするかどうか
	 * @param lParam					[in,optional] このクラスではスケール値として扱います。
	 * @note この関数は、変更前・変更後を含む全ての形状のコピーを取得することができます。
	 * 複数状態を指定した場合はUnionされた形状となります。
	 * @note IGeometry::get_ShapeCopy相当の機能を実現するには、引数に何も指定せずに使用してください。
	 * @note lParamはスケール値として扱います。この値がセットされている場合、そのスケールでの記号矩形
	 * 及び文字矩形のポリゴンが返ります。
	 * その場合、他の引数は無視されます。この仕様は、GetShapeCopyでこのような拡張形状を扱うための
	 * 苦肉の策です。
	 * @see CRow::GetShapeCopy
	 */
	IGeometryPtr GetShapeCopy( bool bLatest = true, bool bOriginal = false, bool bFeatureChangedOriginal = false, bool bDiff = false, long lParam = -1 ) const;

	/**
	 * @brief キャッシュを削除する
	 * @see CRow::DeleteCache()
	 */
	void DeleteCache( bool bRecursive = false );

	/**
	 * @brief <b>ルールキャッシュを返す</b>\n
	 * 地下鉄駅が地上と地下とで異なるルールを使用するため、ルールのキャッシュが
	 * 2つあることへの対応です。
	 * @retval NULL		エラー
	 * @retval NULL以外	ルールレコードオブジェクトのポインタ
	 */
	const annotation_class::CAnnotationClass* GetValidRule() const;
#ifdef SINDYLIB_CALC_TIME
	static void TraceTime();
	static void ClearTime()
	{
		g_dCRelAnnotationParameter_CopyTime = 0.0;
		g_dCreateParameterCache_AllTime = 0.0;
		g_dCreateParameterCache_RuleCacheTime = 0.0;
		g_dCreateParameterCache_AnnotationTime = 0.0;
		g_dCreateParameterCache_DispLineTime = 0.0;
	}
#endif // ifdef SINDYLIB_CALC_TIME
protected:
	/**
	 * @brief メンバ変数を初期化する
	 * see CRow::Init()
	 */
	void Init();
private:
	bool										m_bIsCached;			//!< パラメータキャッシュが作成されたかどうかの判定用
#pragma warning(push)
#pragma warning(disable: 4251)
	// 注記真位置キャッシュ
	std::list<WKSEnvelope>						m_listAnnoRect;			//!< 注記真位置記号矩形
#pragma warning(pop)
};

} // sindy

#endif // ifndef _RELANNOTATIONPARAMETER_H_
