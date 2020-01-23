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

namespace curve_info
{
	namespace circle_fit_evaluator
	{
		typedef long DistanceOf3Points;		///< ３点の間隔（m）
		typedef long FitCount;				///< フィットした点の数

		/**
		 * @brief 評価結果
		 * @note	ある「３点の間隔」を選び、円を描いてフィッティングしてみた結果、何点フィットしたかという情報。
		 *			CircleFitEvaluator全体の結果はこれの集合になる。
		 */
		struct Result
		{
			IPointPtr		m_pTargetPoint;			///< 評価対象点
			long			m_DistanceOf3Points;	///< ３点の間隔（m）
			long			m_FitCount;				///< 円形状が対象形状とフィットした点数
			double			m_ROC;					///< 曲率半径（m）
			mutable double	m_Score;				///< スコア

			/// コンストラクタ
			Result(
				IPointPtr cpTargetPoint,
				long cDistanceOf3Points,
				double cROC,
				long cFitCount
			) :
				m_pTargetPoint( cpTargetPoint ),
				m_DistanceOf3Points( cDistanceOf3Points ),
				m_ROC( cROC ),
				m_FitCount( cFitCount ),
				m_Score( -1 )
			{}

			/**
			 * @brief スコア（この結果の評価の高さ）を返す
			 * @note	「フィット数を曲率半径で割った値」をスコアとする。
			 *			スコア同士を比較する事は、「フィットした領域の長さが、描いた円の円周のうちどれだけの割合を占めるか」を比較する事に相当する。
			 */
			double score() const
			{
				// 未算出なら算出する
				if (m_Score == -1)
				{
					m_Score = m_FitCount / m_ROC;
				}
				return m_Score;
			}

		};
		typedef std::tr1::shared_ptr< Result > ResultPtr;
		typedef std::vector< ResultPtr > Results;
		typedef std::tr1::shared_ptr< Results > ResultsPtr;

		/**
		 * @brief 評価対象点を通る円が、特定の形状とどれだけフィットするかを評価するクラス
		 * @note	評価対象点は対象形状の上にある点。
		 * @note	評価対象点は円周上の点である事を想定。円周上の点でない場合は、その点に最も近い円周上の点が使用される。
		 * @note	DAC = Distance Along Curve。曲線の始点からの道のり。http://edndoc.esri.com/arcobjects/9.1/componenthelp/esrigeometry/ICurve_QueryPointAndDistance.htm
		 * @note	DACT = DAC on Target geometry。対象形状上の点の、対象形状の始点からの道のり。
		 * @note	DACC = DAC on Circle。円周上の点の、円の始点からの道のり。
		 */
		class CircleFitEvaluator
		{
		public:
			typedef std::set< DistanceOf3Points > DistanceOf3PointsArray;	///< 「３点の間隔」の集合

		private:
		// 外部から与えられた情報
			const IPolycurve3Ptr m_pPolycurve;							///< 対象形状（円と比較する形状）
			IPointPtr m_pTargetPoint;									///< 評価対象点
			const double m_DACTofTargetPoint;							///< 対象形状の始点から評価対象点までの道のり
			const DistanceOf3PointsArray& m_rDistanceOf3PointsArray;	///< 評価する「３点の間隔」の集合
			const double m_FittingPointInterval;						///< 円弧が対象形状にフィットするかを評価する点の間隔（度）
			const double m_FittingThreshold;							///< 円弧が対象形状にフィットすると見なす閾値（度）
			double m_DegreeToMeterCoef;									///< 長さの単位を度からmに変換するために掛けるべき係数
			std::ostream& m_rLogStream;									///< ログ出力ストリーム
			bool m_LogFitEvaluatingProcess;								///< 処理過程をログファイルに出力するか否か
			ISpatialReferencePtr m_pSpRef;								///< 使用する空間参照（評価対象点から取得）

		// 結果
			typedef double ROC;										///< 曲率半径（m）
			double m_ROC;											///< 評価対象点における曲率半径（m）
			long m_DistanceOf3P;									///< 評価対象点における曲率半径を決定した際の３点の間隔（m）
			ResultsPtr m_pResults;									///< 評価結果群

		public:
			/// コンストラクタ
			CircleFitEvaluator(
				IPolycurve3Ptr cpPolycurve,
				IPointPtr cpTargetPoint,
				double cDACTofTargetPoint,
				const DistanceOf3PointsArray& cDistanceOf3PointsArray,
				double cFittingPointInterval,
				double cFittingThreshold,
				double cDegreeToMeterCoef,
				std::ostream& crStream,
				bool cLogFitEvaluatingProcess
			) :
				m_pPolycurve( cpPolycurve ),
				m_pTargetPoint( cpTargetPoint ),
				m_DACTofTargetPoint( cDACTofTargetPoint ),
				m_rDistanceOf3PointsArray( cDistanceOf3PointsArray ),
				m_FittingPointInterval( cFittingPointInterval ),
				m_FittingThreshold( cFittingThreshold ),
				m_DegreeToMeterCoef( cDegreeToMeterCoef ),
				m_rLogStream( crStream ),
				m_LogFitEvaluatingProcess( cLogFitEvaluatingProcess ),
				m_ROC( 0 ),
				m_DistanceOf3P( 0 )
			{
				m_pSpRef = m_pTargetPoint->GetSpatialReference();
			}

			/**
			 * @brief 評価して結果を返す
			 * @return	評価結果群（評価すべき点が1つもなければNULL）
			 */
			ResultsPtr evaluate();

			/// 評価対象点における曲率半径を返す
			double roc() const { return m_ROC; }

			/// 曲率半径を決定する際に採用された「３点の間隔」を返す
			long distanceOf3P() const { return m_DistanceOf3P; }

		private:
			/**
			 * @brief 円周上を一定間隔で進み、対象形状との間の距離をチェックして、それが閾値未満である回数（フィット回数）を返す
			 * @note	IProximityOperator.QueryNearestPointはesriExtendEmbeddedを指定すると正常に動作しないのでesriNoExtensionを使用している。
			 *			そのため、呼び出し元からは円弧ではなく円が与えられることが前提。
			 * @note	cIsForwardの指定に応じて片側ずつ実行するので、得られるフィット回数はその２回分を合わせて初めて完全となる。
			 * @param[in]	cpCA				円
			 * @param[in]	cDACCofTargetPoint	円の始点から評価対象点までの道のり（度）
			 * @param[in]	cDistanceOf3P		３点の間隔（m）
			 * @param[in]	cIsForward			進む方向は円の終点側か否（始点側）か
			 * @return	フィット回数
			 */
			long fitCount(
				const ICircularArcPtr cpCA,
				const double cDACCofTargetPoint,
				const long cDistanceOf3P,
				const bool cIsForward
			);

			/**
			 * @brief 評価結果をコンテナに追加する
			 * @note	評価結果群コンテナがまだ生成されていなければ生成する。
			 * @param[in]	cResult	評価結果
			 */
			void addResult( const ResultPtr cpResult );
		};

		/**
		 * @brief 目的の評価結果を選ぶための、評価結果の比較用プレディケート
		 * @note	スコアの高いものを選ぶ。
		 *			スコアが同じなら、曲率半径が小さいものを選ぶ。
		 *			曲率半径も同じなら、先に着目した方を選ぶ。
		 */
		struct lessResult : public std::binary_function< const ResultPtr, const ResultPtr, bool >
		{
			bool operator() ( const ResultPtr r1, const ResultPtr r2 ) const
			{
				return
					r1->score()    != r2->score()    ? r1->score()    > r2->score()    :	// スコアの降順
					r1->m_ROC      != r2->m_ROC      ? r1->m_ROC      < r2->m_ROC      :	// 曲率半径の昇順
					                                 true;
			}
		};
		typedef std::set< const ResultPtr, lessResult > ResultsRanking;		// 評価結果群を高評価な順にソートするためのコンテナ

		/**
		 * @brief 指定された評価結果群のうち、最も評価が高いものを返す
		 * @note	評価指標についてはlessResultのコメントを参照のこと。
		 * @param[in]	crResults		評価結果群
		 * @param[out]	cpLogStream		評価過程を出力するストリーム（NULLなら出力しない）
		 * @return	評価結果
		 */
		const ResultPtr bestFitResult( const Results& crResults, std::ostream* cpLogStream = NULL );

		/**
		 * @brief 評価結果群を指定されたストリームにダンプする（デバッグ用）
		 * @param[in]	crResults	対象評価結果群
		 * @param[in]	co			出力ストリーム
		 */
		void outputResults( const ResultsRanking& crResults, std::ostream& co );

	} // namespace circle_fit_evaluator
} // namespace curve_info
