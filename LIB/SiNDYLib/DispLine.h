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
 * @file DispLine.h
 * @brief CDispLineクラス定義ファイル
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#ifndef _DISPLINE_H_
#define _DISPLINE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Feature.h"
#include "AnnotationCommunity.h"
#include "AheWKSFunctions.h"

namespace sindy {

#ifdef SINDYLIB_CALC_TIME
extern SINDYLIB_API double g_dCalcFlagStringBoundsTime;	//!< CalcStringBounds総時間
extern SINDYLIB_API double g_dCalcPositionTime;			//!< CalcPosition総時間
extern SINDYLIB_API double g_dCalcParameterTime;		//!< CalcParameter総時間
extern SINDYLIB_API double g_dQueryPointsTime;			//!< CalcParameterの点列取得総時間
extern SINDYLIB_API double g_dCalcStringBoundsTime;		//!< CalcParameterのAheCalcStringBounds総時間
#endif // ifdef SINDYLIB_CALC_TIME

class CDispLine;
typedef boost::shared_ptr<CDispLine> CSPDispLine;

/**
 * @class CDispLine
 * @brief <b>注記表示位置フィーチャ抽象クラス</b>\n
 * このクラスは注記表示位置フィーチャの抽象クラスです。
 * 注記表示位置フィーチャの共通部分は全てこのクラスに記述
 * されています。
 */
class SINDYLIB_API CDispLine : public CFeature
{
	ROWCLASS_CONSTRUCTOR( CDispLine, unknown )

	friend class CRelAnnotationParameter; // パラメータキャッシュを直接参照する関係でfriend設定 TODO: いつか直そうね
	friend class CAnnotations; // 空間インデックスを作成する関係でfriend設定 TODO: いつか直そうね

	//@{ @name 属性取得関数
	/**
	 * @brief <b>フォントサイズを取得する</b>\n
	 * @return エラー又はNULLの場合は-1
	 */
	schema::disp_line::font_size::ECode GetFontSize() const
	{
		return (schema::disp_line::font_size::ECode)GetLongValueByFieldName( schema::disp_line::kFontSize, -1 );
	}

	/**
	 * @brief <b>関連する都市地図注記IDを取得する</b>\n
	 * @return エラー又はNULLの場合は-1、それ以外は都市地図注記OBJECTID
	 */
	long GetAnnoID() const
	{
		return GetLongValueByFieldName( schema::disp_line::kAnnoID, -1 );
	}

	/**
	 * @brief <b>参照文字列コードを返す</b>\n
	 * @return エラー又はNULLの場合は-1、それ以外はtargetstr_code::ECode
	 */
	schema::disp_line::target_str::ECode GetTargetStr() const
	{
		return (schema::disp_line::target_str::ECode)GetLongValueByFieldName( schema::disp_line::kTargetStr, -1 );
	}
	
	/**
	 * @brief <b>二行注記の二行目開始インデックス番号を取得する</b>\n
	 * @return 二行注記の二行目開始インデックス番号
	 */
	long GetSecondLineStartIndex() const
	{
		return m_lSecondLineStartIndex;
	}

	/**
	 * @brief <b>注記文字列ラインタイプを取得する</b>\n
	 * @return sindyAnnotationLineType
	 */
	sindyAnnotationLineType GetAnnotationLineType() const
	{
		return m_emLineType;
	}
	//@}

	//@{ @name チェック関数
	/**
	 * @brief <b>フォントサイズ値をチェックする</b>\n
	 * @note フォントサイズが10の時は、漢字が使用されていればエラー、使用されていなければ
	 * 警告となります。
	 * @param lpcszNameString	[in]	対応する表示文字列（フォントサイズが10の場合のみ使用）
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode CheckFontSize( LPCTSTR lpcszNameString ) const;

	/**
	 * @brief <b>関連注記真位置IDをチェックする</b>\n
	 * 0以上を適正とします。その場合、関連IDを持つ真位置が
	 * 本当に存在するかどうかのチェックはしません。
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode CheckAnnoID() const;

	/**
	 * @brief <b>参照文字列コードをチェックする</b>\n
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode CheckTargetStr() const;

	/**
	 * @brief <b>表示ラインの角度をチェックする</b>\n
	 * @param emLineType	[in]			ラインタイプ
	 * @param dErrAngle		[out,optional]	エラー時の角度が返る（デフォルト：NULL）
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode CheckAngle( sindyAnnotationLineType emLineType, double* dErrAngle/* = NULL*/ ) const;

	/** 表示ライン形状の整合性をチェックする
	 * @brief emDispTypeRule [in] ライン表示ルール
	 * @retval sindyErr_AnnoDispLineTypeInvalid ルールが「線状」なのにラインタイプが「線状」ではない、又はルールが「線状」じゃないのにラインタイプが「線状」
	 * @retval sindyErr_AnnoDispLineIllegalDouble 一文字しか存在しない行がある、又は2行目折り返し差分が正値
	 * @retval sindyErr_NoErr 正しい
	 */
	errorcode::sindyErrCode CheckLine( schema::annotation_class::disptype_rule::ECode emDispTypeRule ) const;

	//@}

	/**
	 * @brief <b>表示位置パラメータを計算する</b>\n
	 * 縦横の判定は、一点目と二点目のXY座標をみて判定します。\n
	 * 一行二行の判定は一点目とその他の点を比較して、変化があるごとに
	 * カウンタを増加させて判定します。カウンタが2以上になると、線状
	 * 注記とみなされます。\n\n
	 * @param bIsHalfString			[in]			文字幅を規定の半分として計算する\n山岳標高値の場合はこれをtrueにする必要があります。\n
	 *												山岳標高値かどうかはCDispLine::IsAltitudeValueで調べることができます。
	 * @param pSecondLineStartIndex	[out,optional]	二行注記の場合に二行目開始インデックス番号が入る（デフォルト：NULL）
	 * @param pStringRects			[out,optional]	個々の表示文字列矩形が入る\nそれぞれは閉じたリングになる（デフォルト：NULL）
	 * @param pRect					[out,optional]	表示位置ライン矩形（最終点は含まない）（デフォルト：NULL）\n
	 *                                              一行注記の時は矩形にはなりませんので注意！！
	 * @param ipStringBounds		[out,optional]	文字列のバウンディングボックスが入る（デフォルト：NULL）
	 * @retval -1 形状が存在しない
	 * @retval sindyAnnotationLineType
	 */
	sindyAnnotationLineType CalcParameter( bool bIsHalfString, long* pSecondLineStartIndex = NULL, WKSPoint* pStringRects = NULL, WKSEnvelope* pRect = NULL, IGeometry** ipStringBounds = NULL ) const;

	/**
	 * @brief <b>渡されたポイントに対して表示位置が縦横右左のどの位置にあるかを判定する</b>\n
	 * @param cPoint		[in]	判定対象のポイント
	 * @param emLineType	[in]	縦横・一行二行（CalcParameterで取得可能）
	 * @param cRect			[in]	表示位置ライン矩形（最終点は含まない）（CalcParameterで取得可能）
	 * @return sindyAnnotationPosition
	 */
	sindyAnnotationPosition CalcPosition( const CPoint& cPoint, sindyAnnotationLineType emLineType, const WKSEnvelope& cRect ) const;

	/**
	 * @brief <b>表示文字外形を計算する</b>\n
	 * 渡された表示文字外形に旗揚げの場合の形状を付加します。
	 * @param ipBound	[in,out]	表示文字列外形
	 * return sindyErrCode
	 */
	errorcode::sindyErrCode CalcStringBounds( IPoint* pAnnoPoint, IGeometry* ipBound ) const;

	IGeometryPtr GetStringBounds() const { return m_ipGeom; }

	// 
	/**
	 * @brief <b>1pt当たりの長さを取得する</b>\n
	 * @param bCalcX	[in]	X座標の長さを取得する場合はtrue、Y座標の長さを取得する場合はfalse
	 * @return 1pt当たりの長さ（単位：度（緯度経度））
	 */
	double Calc1PTLen( bool bCalcX ) const;

	/**
	 * @brief <b>テーブルタイプとスケールを比較する</b>\n
	 * @param emScale	[in]	スケール
	 * @retval true テーブルタイプにふさわしいスケール
	 * @retval false テーブルタイプに合わないスケール
	 */
	bool IsSameScale( sindyAnnotationScale emScale ) const { return ( emScale == GetMaxScale() ) ? true : false; }

	/**
	 * @brief <b>山岳標高値かどうか</b>\n
	 * @param emCode	[in]	種別コード
	 * @retval true		山岳標高値である
	 * @retval false	山岳標高値ではない
	 */
	bool IsAltitudeValue( schema::annotation::annotation_code::ECode emCode ) const { 
		return ( emCode == schema::annotation::annotation_code::kMountain && GetTargetStr() == schema::disp_line::target_str::kStr3 ) ? true : false;
	}

	/**
	 * @brief <b>表示できるスケールの最大値を取得する</b>
	 * @retval -1		エラー
	 * @retval -1以外	表示最大スケール
	 */
	sindyAnnotationScale CDispLine::GetMaxScale() const;


#ifdef SINDYLIB_CALC_TIME
	static void TraceTime();
	static void ClearTime()
	{
		g_dCalcStringBoundsTime = 0.0;
		g_dCalcPositionTime = 0.0;
		g_dCalcParameterTime = 0.0;
		g_dQueryPointsTime = 0.0;
	}
#endif // ifdef SINDYLIB_CALC_TIME

	/**
	 * @brief <b>コピーする</b>\n
	 * destにキャッシュがある場合は、削除されます。
	 * キャッシュがある場合は、キャッシュも一緒にコピーされます。
	 * @see CRow::CopyTo
	 * @param dest				[in]	ペースト対象（CDispLine限定です）
	 * @param bIsCopyCache		[in]	キャッシュをコピーするかどうか
	 * @param bIsCopyRow		[in]	オブジェクトをコピーするかどうか
	 * @param bIsCopyFlag		[in]	作成削除フラグをコピーするかどうか
	 * @param bIsCopyContainer	[in]	コンテナをコピーするかどうか
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode CopyTo( CRow& dest, bool bIsCopyCache=true, bool bIsCopyRow=true, bool bIsCopyFlag=true, bool bIsCopyContainer=true ) const;

	/**
	 * @brief キャッシュを削除する
	 * @see CRow::DeleteCache()
	 */
	void DeleteCache( bool bDeleteContainerCache = false );

	/**
	 * @brief パラメータキャッシュを削除する
	 */
	void ClearParameterCache();
protected:
	/**
	 * @brief メンバ変数を初期化する
	 * see CRow::Init()
	 */
	void Init();

	/**
	 * @brief パラメータキャッシュをセットする
	 * 
	 * この関数は、CRelAnnotationParameter::CreateParameterCacheのみから呼ばれます。
	 * TODO: 本来パラメータキャッシュはCDispLineで作成されるべきですが、実装を移すまでの
	 * 間、暫定的にこの関数でセットします。
	 */
	void SetParameterCache( int nStringRectSize, long lSecondLineStartIndex, WKSPoint* pStringRects, const WKSEnvelope& cRect, IGeometry* ipGeom, sindyAnnotationLineType emLineType, bool bIsFlag, bool bCanCopy );
private:
#pragma warning(push)
#pragma warning(disable: 4251)
	int							m_nStringRectsSize;			//!< m_pStringRectsのサイズ
	long						m_lSecondLineStartIndex;	//!< 二行注記の二行目開始インデックス番号
	WKSPoint*					m_pStringRects;				//!< 個々の注記文字矩形
	WKSEnvelope					m_cRect;					//!< 注記文字列表示矩形
	IGeometryPtr				m_ipGeom;					//!< 注記文字列バウンディングボックス（旗揚げ形状含む）
	sindyAnnotationLineType		m_emLineType;				//!< 注記文字列ラインタイプ
	bool						m_bIsFlag;					//!< 旗揚げかどうか
	bool						m_bCanCopy;					//!< コピー可能かどうか
#pragma warning(pop)
};

} // sindy

#endif // _DISPLINE_H_
