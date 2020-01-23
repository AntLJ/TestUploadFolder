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
 * @file MoveFeedback.h
 * @brief CMoveFeedback クラス定義ファイル
 * @author 地図DB制作部システム開発グループ 古川 貴宏
 * @version $Id$
 */
#ifndef _MOVEFEEDBACK_H_
#define _MOVEFEEDBACK_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "exportimport.h"
#include "Row.h"
#include "EditDef.h"

namespace sindy {

class SINDYLIB_API CMoveFeedback  
{
	struct NEWLINEFEEDBACK
	{
		INewLineFeedbackPtr Feedback;
		double X;
		double Y;
	};
	struct MOVEPOINTFEEDBACK
	{
		IMovePointFeedbackPtr Feedback;
		double X;
		double Y;
	};
public:
	CMoveFeedback();
	virtual ~CMoveFeedback();

public:
	/**
	 * @brief <b>伸縮禁止なテーブルタイプを設定する</b>\n
	 * Stopでクリアされます。
	 * @param emCode	[in]	伸縮を禁止したいテーブルタイプ
	 */
	void SetCantElasticType( sindyTableType::ECode emCode );
	std::pair<double,double> Stop( double* pdScale = NULL, double* pdAngle = NULL );
	void SetDisplay( IScreenDisplay* ipDisp ){ m_ipDisp = ipDisp; }

	/**
	 * @brief <b>フィードバックをスタートする</b>\n
	 * 移動候補フィーチャの中から、クエリ形状に引っかかるものについて
	 * フィードバックを発生させます。どのようにフィードバックを発生させるかに
	 * ついてはemModeによって異なります。詳しくは、CFeatures::Moveを参照して
	 * ください。\n
	 * エラーにならなくても、移動対象が見つからない場合フィードバックが開始されません。
	 * そのときはbFeedbackStartedがfalseになるので、チェックしてください。\n
	 * この関数を呼ぶ前に、SegDisplayでスクリーンディスプレイをセットしておく必要があります。
	 * @param cGeom				[in]	クエリ形状
	 * @param ipStartPoint		[in]	フィードバック開始点（MouseUpの位置）
	 * @param listFeatures		[in]	移動候補フィーチャ
	 * @param emMove			[in]	移動モード
	 * @param bFeedbackStarted	[out]	フィードバックがスタートしたらtrue
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode Start( const CGeometry& cGeom, IPoint* ipStartPoint, const CContainer& listFeatures, sindyeMoveMode emMode, bool& bFeedbackStarted );

	void MoveTo( IPoint* Point );
	void Refresh( OLE_HANDLE hDC );
	void Rotate( double dAngle );
	void Scale( double dScale );

	/**
	 * @brief <b>回転・拡大・縮小ができるかどうか</b>\n
	 * @return true : 回転・拡大・縮小することができる
	 */
	bool CanTransform();
	void GetMovePoint(double* pX, double* pY);

	inline sindyeMoveMode GetMoveMode() const { return m_emMoveMode; }
private:
	typedef std::list<NEWLINEFEEDBACK>::iterator segment_feedback_iterator;
	typedef std::list<MOVEPOINTFEEDBACK>::iterator point_feedback_iterator;
	segment_feedback_iterator segment_feedback_begin(){ return m_listSegmentFeedback.begin(); }
	segment_feedback_iterator segment_feedback_end(){ return m_listSegmentFeedback.end(); }
	point_feedback_iterator point_feedback_begin(){ return m_listPointFeedback.begin(); }
	point_feedback_iterator point_feedback_end(){ return m_listPointFeedback.end(); }
	void segment_feedback_push_back( IPoint* StartPoint );
	void point_feedback_push_back( IPoint* StartPoint );
	void SetBasePoint( IPoint* Point ){	Point->QueryCoords( &m_dBaseX, &m_dBaseY );	}

	IPointPtr GetCenterPoint( const CGeometry& cGeom );

private:
#pragma warning(push)
#pragma warning(disable:4251)
	IScreenDisplayPtr					m_ipDisp;				//!< フィードバック表示に必要
	IMoveLineFeedbackPtr				m_ipMoveLineFeedback;	//!< 平行移動部分フィードバック用
	std::list<MOVEPOINTFEEDBACK>		m_listPointFeedback;	//!< ポイントオブジェクトフィードバック用
	std::list<NEWLINEFEEDBACK>			m_listSegmentFeedback;	//!< 移動部分と非移動部分の境目フィードバック用
	double								m_dBaseX, m_dBaseY;		//!< 移動開始ベースポイント
	double								m_dMoveX, m_dMoveY;		//!< 移動分保持用
	std::list<sindyTableType::ECode>	m_listCantElastic;		//!< 伸縮禁止タイプ保持用
	sindyeMoveMode						m_emMoveMode;			//!< 移動モード
	double								m_dAngle;				//!< 回転角度
	double								m_dScale;				//!< 拡大率
#pragma warning(pop)
};

} // sindy

#endif // _MOVEFEEDBACK_H_
