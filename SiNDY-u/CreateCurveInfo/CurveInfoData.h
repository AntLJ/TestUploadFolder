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
 * @brief 本ツール独自のデータ群の定義
 */
#pragma once

#include "adlib.h"
#include "CircleFitEvaluator.h"

namespace curve_info
{
	using namespace adlib;

	/**
	 * @brief セグメント（線分）
	 * @note	Segmentコンテナ内での向きを揃えるため、取得元（ISegmentPtr）の向きとは無関係に始終点を所持できるものとする。
	 *			「仮想セグメント」（SDEには存在しないが、始終点だけを設定して独自に考えたもの）も扱う。
	 */
	struct Segment
	{
		long m_LinkID;			///< 所属するリンクのオブジェクトID（仮想セグメントの場合は0）
		long m_Index;			///< 所属するリンク内でのインデックス（仮想セグメントの場合は0）
		IPointPtr m_pFromPoint;	///< セグメントの始点（取得元のFromPointまたはToPoint）
		IPointPtr m_pToPoint;	///< セグメントの終点（取得元のFromPointまたはToPoint）
		bool m_IsHighSpeedArea;		///< 所属するリンクが高速域であるか否か（仮想セグメントの場合はfalse）

		/**
		 * @brief コンストラクタ
		 * @param[in]	cLinkID				所属するリンクのオブジェクトID
		 * @param[in]	cIndex				所属するリンク内でのインデックス
		 * @param[in]	cpFromPoint			セグメントの始点（取得元のFromPointまたはToPoint）
		 * @param[in]	cpToPoint			セグメントの終点（取得元のFromPointまたはToPoint）
		 * @param[in]	cIsHighSpeedArea	所属するリンクが高速域であるか否か
		 */
		Segment( long cLinkID, long cIndex, IPointPtr cpFromPoint, IPointPtr cpToPoint, bool cIsHighSpeedArea ) :
		m_LinkID( cLinkID ), m_Index( cIndex ), m_pFromPoint( cpFromPoint ), m_pToPoint( cpToPoint ), m_IsHighSpeedArea( cIsHighSpeedArea )
		{}

		/**
		 * @brief コンストラクタ（仮想セグメント用）
		 * @param[in]	cpFromPoint	セグメントの始点（取得元のFromPointまたはToPoint）
		 * @param[in]	cpToPoint	セグメントの終点（取得元のFromPointまたはToPoint）
		 * @note	仮想セグメントのm_IsHighSpeedAreaの値は意味を持たないので、ここではfalseとしている。
		 *			TODO: ↑この設計は間違いの元になりそうなので、要修正。
		 *			始終点以外の情報を全てポインタにするなどして、仮想セグメントの場合は無効値である事を明示すべき。
		 *			案：始終点以外の情報を別の構造体のメンバにして、それへのポインタをSegmentのメンバとする。
		 */
		Segment( IPointPtr cpFromPoint, IPointPtr cpToPoint ) :
		m_LinkID( 0 ), m_Index( 0 ), m_pFromPoint( cpFromPoint ), m_pToPoint( cpToPoint ), m_IsHighSpeedArea( false )
		{}

		/**
		 * @brief セグメントの長さを返す
		 * @return	セグメントの長さ（度）
		 */
		double length() const;

		/**
		 * @brief セグメントが有効であるか否かを返す
		 * @note	始終点が同一（長さが0）のセグメントは無効とする。
		 * @return	判定結果
		 */
		bool valid() const;
	};
	typedef std::tr1::shared_ptr<Segment> SegmentPtr;
	typedef std::deque<SegmentPtr> Segments;	// std::listのiteratorはboost::priorが使えないので、dequeに変更。
	typedef Segments::const_iterator SegmentIt;

	typedef std::vector<LinkID> LinkIDArray;	///< リンクID列（順序付きリンクID群）

	/// 一方通行規制の種類（一本道の構成リンク群に一方通行規制が存在するか否かと、その方向）
	namespace oneway_type
	{
		enum ECode
		{
			none,	///< 規制なし
			fore,	///< 一本道に対して順方向
			back,	///< 一本道に対して逆方向
		};
	}

	/**
	 * @brief 一本道（分岐のない連続したセグメントの列）
	 * @note	一本道の方向は、最初に追加したリンクのFromノードからToノードへ向かう方向とする。
	 * @note	構成リンク群は全て「高速域か否か」が同じでなければならないものとする。（Bug9619）
	 * @note	構成リンク群は全て「一方通行規制の有無と方向」が同じでなければならないものとする。（Bug9642）
	 */
	struct SingleRoad : public Segments
	{
		typedef double Ratio;							///< 始点を0、終点を1とした時の、一本道上の位置
		typedef std::pair< Ratio, Ratio > Range;		///< 一本道上の範囲

		bool m_IsHighSpeedArea;							///< 高速域か否か
		oneway_type::ECode m_Oneway;					///< 構成リンク群に一方通行規制が存在するか否かと、その方向
		ISpatialReferencePtr m_pSpRef;					///< 使用する空間参照

	// 内部キャッシュ
		mutable double m_Length;					///< 一本道の長さ
		typedef std::map< Ratio, SegmentIt > RatioMap;
		mutable RatioMap m_RatioMap;				///< セグメント開始位置とセグメントの対応表
		mutable IGeometryPtr m_Geometry;			///< 形状

		/// コンストラクタ
		SingleRoad(
			bool cIsHighSpeedArea,
			oneway_type::ECode cOneway,
			ISpatialReferencePtr cpSpatialReference
		) :
			m_IsHighSpeedArea( cIsHighSpeedArea ),
			m_Oneway( cOneway ),
			m_Length( 0 ),
			m_pSpRef( cpSpatialReference )
		{}

		/**
		 * @brief 対象リンクのセグメント群を自身のコンテナに追加する
		 * @param[in]	cpLink			対象リンク
		 * @param[in]	cStartIsFrom	セグメント群を対象リンクのFromノード側から追加するか否（Toノード側から追加する）か
		 * @param[in]	cIsBack			セグメント群をコンテナのBack側に追加するか否（Front側に追加する）か
		 */
		void addLink( IFeaturePtr cpLink, bool cStartIsFrom, bool cIsBack );

		/**
		 * @brief 一本道を構成するリンクID列を返す
		 * @return	リンクID列
		 */
		LinkIDArray linkIDArray() const;

		 /**
		  * @brief 一本道の長さを返す
		  * @note	初回呼び出し時のみ、算出のコストが発生。
		  * @return	長さ（度）
		  */
		double length() const;

		/**
		 * @brief セグメント開始位置とセグメントの対応表を返す
		 * @note	初回呼び出し時のみ、算出のコストが発生。
		 * @return	対応表
		 */
		const RatioMap& ratioMap() const;

		/**
		 * @brief 指定した位置を含むセグメントを返す
		 * @param[in]	cRatio	位置
		 * @return	セグメント
		 */
		SegmentIt segment( Ratio cRatio ) const;

		/**
		 * @brief 指定した範囲を包含するようなセグメント群を返す
		 * @param[in]	cRange	範囲
		 * @return	セグメント群
		 */
		Segments segments( Range cRange ) const;

		/**
		 * @brief 一本道全体の形状を返す
		 * @note	初回呼び出し時のみ、算出のコストが発生。
		 * @return	形状
		 */
		IGeometryPtr geometry() const;

		/**
		 * @brief セグメントを追加する
		 * @note	push_back()との違いは、「無効なセグメントなら追加しない」事。
		 * @param[in]	cpSegment	対象セグメント
		 */
		void add( SegmentPtr cpSegment );
	};
	typedef std::tr1::shared_ptr< SingleRoad > SingleRoadPtr;

	/**
	 * @brief 方向付き一本道（Directional Single Road = DSR）
	 * @note	変曲点において一本道が分割される事により作成される。
	 *			「方向付き」というのは、「左」「右」のいずれかに分類でき、真っ直ぐであったり、左右両方に曲がっていたりする事はないという意味。
	 */
	struct DirectionalSR : public SingleRoad
	{
		bool m_IsLeftCurve;			///< 左カーブか否か
		SingleRoad::Range m_Range;	///< 元の一本道のうち、このデータに該当する範囲

		/// コンストラクタ
		DirectionalSR(
			bool cIsLeftCurve,
			SingleRoad::Range cRange,
			bool cIsHighSpeedArea,
			oneway_type::ECode cOneway,
			ISpatialReferencePtr cpSpRef
		) :
			m_IsLeftCurve( cIsLeftCurve ),
			m_Range( cRange ),
			SingleRoad( cIsHighSpeedArea, cOneway, cpSpRef )
		{}
	};
	typedef std::tr1::shared_ptr< DirectionalSR > DirectionalSRPtr;
	typedef std::vector< DirectionalSRPtr > DirectionalSRArray;

	/**
	 * @brief 区間
	 * @note	１つのカーブを表す情報。
	 */
	struct Section	// TODO: クラス化
	{
	public:
	// 公開定義
		typedef std::vector<IPointPtr> Points;

//	private:
	// 非公開変数
		//----- 出力のための変数 -----//
		double	m_ROC;						///< 曲率半径（m）
		long	m_DistanceOf3P;				///< 曲率半径を決定する際に採用された「３点の間隔」（m）
		long	m_RepLinkID;				///< 代表点存在リンクID
		double	m_WayToRepPoint;			///< 代表点位置
		Points	m_Points;					///< 点列（始終点以外はリンクの構成点）	TODO: 始終点だけ別に持ってもよいかも？
		bool	m_IsHighSpeedArea;			///< 高速域か否か

		//----- デバッグ用の参考情報 -----//
		IPointPtr m_pRepPoint;				///< 代表点

		//----- 内部処理用変数 -----//
		bool	m_IsLeftCurve;			///< 左カーブか否か（フォーマット上は点列の順序で表すが、プログラム上では保持し、点列の順序はSRと同じにしておく）
		double	m_FitRegionAngle;		///< CircleFitEvaluatorによる評価で、フィット領域が円周のうちどれだけの角度を占めていたか（Bug9638で追加）

	// 公開関数
		/// コンストラクタ
		Section(
			bool cIsLeftCurve,
			bool cIsHighSpeedArea
		) :
			m_ROC( 0 ),
			m_DistanceOf3P( 0 ),
			m_RepLinkID( 0 ),
			m_WayToRepPoint( 0 ),
			m_IsLeftCurve( cIsLeftCurve ),
			m_IsHighSpeedArea( cIsHighSpeedArea ),
			m_FitRegionAngle( 0 )
		{}

		/**
		 * @brief 左カーブか否かを返す
		 * @return	左カーブか否か
		 */
		bool isLeftCurve() const { return m_IsLeftCurve; }

		/**
		 * @brief 曲率半径を変更する
		 * @param[in]	cROC	曲率半径
		 */
		void setROC( double cROC ) { m_ROC = cROC; }

		/**
		 * @brief 曲率半径を返す
		 * @return	曲率半径
		 */
		double ROC() const { return m_ROC; }
	};
	typedef std::tr1::shared_ptr<Section> SectionPtr;

	typedef tstring FieldName;
	typedef long FieldIndex;
	typedef std::map<FieldName, FieldIndex> FieldIndexMap;	///< フィールド名とインデックスの対応表

	/**
	 * @brief 「曲率半径を算出する点」の情報
	 */
	struct ROCPointInfo
	{
		IPointPtr							m_pPoint;		///< 点
		SingleRoad::Ratio					m_Ratio;		///< 元のSingleRoad上の位置
		circle_fit_evaluator::ResultsPtr	m_pCFEResults;	///< CircleFitEvaluatorによる評価結果

		 /// コンストラクタ
		ROCPointInfo(
			IPointPtr cpPoint,
			circle_fit_evaluator::ResultsPtr cpCFEResults,
			SingleRoad::Ratio cRatio
		) : 
			m_pPoint( cpPoint ),
			m_pCFEResults( cpCFEResults ),
			m_Ratio( cRatio )
		{}
	};
	typedef std::tr1::shared_ptr< ROCPointInfo > ROCPointInfoPtr;

	/**
	 * @brief ROCPointInfoの集合
	 * @note	点による検索を可能にするため、typedefではなく構造体として定義。
	 */
	struct ROCPointInfos : public std::vector< ROCPointInfoPtr >
	{
		typedef std::pair< double, double > LonLat;	///< 経緯度1
		typedef std::map< LonLat, ROCPointInfoPtr > LonLatMap;
		mutable LonLatMap m_pLonLatMap;	///< 点による検索のためのテーブル

		/**
		 * @brief 指定された点に対応する要素を返す
		 * @param[in]	cpPoint	対象点
		 * @return	要素（見つからなければNULL）
		 */
		const ROCPointInfoPtr searchByPoint( IPointPtr cpPoint ) const
		{
			if (m_pLonLatMap.empty())
			{
				// 検索用テーブルが作成されていなければ作成する
				const ROCPointInfos& rROCPointInfos = *this;
				BOOST_FOREACH (const ROCPointInfoPtr pROCPointInfo, rROCPointInfos)
				{
					ATLASSERT( pROCPointInfo );
					m_pLonLatMap.insert( std::make_pair(
						std::make_pair( pROCPointInfo->m_pPoint->GetX(), pROCPointInfo->m_pPoint->GetY() ),
						pROCPointInfo
					));
				}
			}

			LonLatMap::const_iterator it = m_pLonLatMap.find( std::make_pair( cpPoint->GetX(), cpPoint->GetY() ) );
			return it != m_pLonLatMap.end() ? it->second : ROCPointInfoPtr();
		}
	};
	typedef std::tr1::shared_ptr< ROCPointInfos > ROCPointInfosPtr;

	/**
	 * @brief DSR情報（方向付き一本道とそれに関連する情報）
	 */
	// TODO: できればメンバを全て参照にする
	struct DirectionalSRInfo
	{
		SingleRoadPtr		m_pSingleRoad;		///< 元の一本道
		DirectionalSRPtr	m_pDirectionalSR;	///< 方向付き一本道
		ROCPointInfosPtr	m_pROCPointInfos;	///< 一本道に対応するROCPointInfo列

		/// コンストラクタ
		DirectionalSRInfo(
			SingleRoadPtr cpSingleRoad,
			DirectionalSRPtr cpDirectionalSR,
			ROCPointInfosPtr m_pROCPointInfos
		) :
			m_pSingleRoad( cpSingleRoad ),
			m_pDirectionalSR( cpDirectionalSR ),
			m_pROCPointInfos( m_pROCPointInfos )
		{}
	};

	/**
	 * @brief 点の座標
	 */
	struct Point
	{
		Point() : x(0), y(0) {}
		Point( double cX, double cY ) : x(cX), y(cY) {}

		double x;	///< X座標（度）
		double y;	///< Y座標（度）
	};
	typedef std::vector<Point> Points;

	/// 区間候補
	struct SectionCandidate
	{
		SingleRoad::Range m_Range;					///< 範囲
		ROCPointInfos m_ROCPointInfos;				///< 範囲内の点情報列
		DirectionalSRInfo m_rDirectionalSRInfo;		///< この区間候補が含まれるDSR

		/// コンストラクタ
		SectionCandidate(
			DirectionalSRInfo crDirectionalSRInfo
		) :
			m_Range( std::make_pair( 0, 0 ) ),
			m_rDirectionalSRInfo( crDirectionalSRInfo )
		{}

		/// コピーコンストラクタ
		SectionCandidate(
			const SectionCandidate& rSectionCandidate
		) :
			m_Range( rSectionCandidate.m_Range ),
			m_ROCPointInfos( rSectionCandidate.m_ROCPointInfos ),
			m_rDirectionalSRInfo( rSectionCandidate.m_rDirectionalSRInfo )
		{}

	};
	typedef std::vector< SectionCandidate > SectionCandidates;
	typedef std::tr1::shared_ptr< SectionCandidate > SectionCandidatePtr;

} // namespace curve_info

