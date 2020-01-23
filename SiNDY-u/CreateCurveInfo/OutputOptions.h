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
	/**
	 * @brief 出力に関するオプション
	 */
	// TODO: 値域のチェック
	//	・m_StraightAngleは0または正の実数でなければならない
	//　・m_ROCPointInterval、m_FittingPointIntervalは正の値でなければならない
	//	など
	struct OutputOptions
	{
		long m_MaxROCForRegionDeletion;				///< 領域削除のための最大曲率半径（低速域用）(m)
		long m_MaxROCForRegionDeletionHigh;			///< 領域削除のための最大曲率半径（高速域用）(m)
		long m_MaxROCOfSection;						///< 区間の最大曲率半径（低速域用）(m)
		long m_MaxROCOfSectionHigh;					///< 区間の最大曲率半径（高速域用）(m)
		double m_StraightAngle;						///< 直線と見なす角度（度）　TODO: 不要かも
		double m_CurveCertifyAngle;					///< カーブ認定角度（度）
		long m_ROCPointInterval;					///< 曲率半径を求める点の間隔(m)
		long m_FittingPointInterval;				///< 円弧がベジエ曲線にフィットするかを評価する点の間隔(m)
		long m_FittingThreshold;					///< 円弧がベジエ曲線にフィットすると見なす閾値(m)
		double m_SectionMinAngle;					///< 区間に対応する円弧の最小角度（度）
		bool m_CreateEachMesh;						///< メッシュごとにShapeを作成するか否か
		bool m_SHP05DirConfig;						///< SHP05のディレクトリ構成で出力するか否か
		bool m_OutputVerificationLog;				///< 検証用ログを作成するか否か
		bool m_LogHandlingProcess;					///< 処理過程をログファイルに出力するか否か
		bool m_LogFitEvaluatingProcess;				///< フィット評価過程をログファイルに出力する
		CString m_QueryWhereClause;					///< 対象の道路リンクを決定するためのクエリWhere句

		/// コンストラクタ
		OutputOptions::OutputOptions(
			long cMaxROCForRegionDeletion,
			long cMaxROCForRegionDeletionHigh,
			long cMaxROCOfSection,
			long cMaxROCOfSectionHigh,
			double cCurveCertifyAngle,
			double cStraightAngle,
			long cROCPointInterval,
			long cFittingPointInterval,
			long cFittingThreshold,
			double cSectionMinAngle
		) :
			m_MaxROCForRegionDeletion( cMaxROCForRegionDeletion ),
			m_MaxROCForRegionDeletionHigh( cMaxROCForRegionDeletionHigh ),
			m_MaxROCOfSection( cMaxROCOfSection ),
			m_MaxROCOfSectionHigh( cMaxROCOfSectionHigh ),
			m_CurveCertifyAngle( cCurveCertifyAngle ),
			m_StraightAngle( cStraightAngle ),
			m_ROCPointInterval( cROCPointInterval ),
			m_FittingPointInterval( cFittingPointInterval ),
			m_FittingThreshold( cFittingThreshold ),
			m_SectionMinAngle( cSectionMinAngle ),
			m_QueryWhereClause( CString() ),
			m_CreateEachMesh( false ),					// デフォルトでは指定メッシュ全部まとめてShapeを作成する
			m_SHP05DirConfig( false ),
			m_OutputVerificationLog( false ),			// デフォルトでは作成しない
			m_LogHandlingProcess( false ),
			m_LogFitEvaluatingProcess( false )
		{
		}

		/**
		 * @brief 高速域か否かに応じて適切な「領域削除のための最大曲率半径」を返す
		 * @param[in]	cIsHighSpeedArea	高速域か否か
		 * @return	最大曲率半径
		 */
		long maxROCForRegionDeletion( bool cIsHighSpeedArea ) const
		{
			return cIsHighSpeedArea ? m_MaxROCForRegionDeletionHigh : m_MaxROCForRegionDeletion;
		}

		/**
		 * @brief 高速域か否かに応じて適切な「区間の最大曲率半径」を返す
		 * @param[in]	cIsHighSpeedArea	高速域か否か
		 * @return	最大曲率半径
		 */
		long maxROCOfSection( bool cIsHighSpeedArea ) const
		{
			return cIsHighSpeedArea ? m_MaxROCOfSectionHigh : m_MaxROCOfSection;
		}
	};
} // namespace curve_info
