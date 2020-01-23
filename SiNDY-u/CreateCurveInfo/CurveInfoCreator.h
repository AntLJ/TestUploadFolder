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
#include <boost/logic/tribool.hpp>
#include <shapelib/SHPHandle.h>
#include <shapelib/DBFHandle.h>
#include "OutputOptions.h"
#include "CurveInfoData.h"
#include "AngleAccumulating.h"
#include "CacheManager.h"

namespace curve_info
{
	using namespace boost::filesystem;
	using namespace adlib;

	/**
	 * @brief カーブ情報作成クラス
	 * @note	出力済みメッシュの区間群は解放してもよいが、データ量がさほど大きくないので今のところは解放していない。
	 *			
	 */
	class CurveInfoCreator
	{
	public:
	// 公開定義
		typedef std::vector< SectionPtr > Sections;				///< 区間群
		typedef std::map< MeshCode, Sections > SectionsByMesh;	///< メッシュ毎の区間群

	private:
	// 非公開変数
		// 外部から与えられた情報
		MeshCodes				m_BaseMeshList;					///< 出力対象２次メッシュリスト
		path					m_OutputDirPath;				///< Shapeファイル出力ディレクトリパス
		mutable std::ofstream	m_LogStream;					///< ログ出力ストリーム
		mutable std::ofstream	m_VerificationLogStream;		///< 検証用ログ出力ストリーム
		OutputOptions			m_OutputOptions;				///< 出力オプション

		// 成果物
		SectionsByMesh m_SectionsByMesh;						///< メッシュ毎の区間群

		// 制御用変数
		long m_SuccessfulMeshCount;								///< 正常終了メッシュ数カウンタ
		CacheManagerPtr m_pCacheManager;						///< キャッシュマネージャ
		long m_MeshCode;										///< 処理中のメッシュコード
		LinkIDs m_ProcessedLinkIDs;								///< 処理済みリンクID集合
		ISpatialReferencePtr m_pSpRef;						///< 空間参照（最初に着目したリンクから取得する）

	public:
	// 公開関数
		/**
		 * @brief コンストラクタ
		 * @param[in]	cOutputOptions	出力オプション
		 */
		CurveInfoCreator( OutputOptions cOutputOptions ) :
			m_OutputOptions( cOutputOptions ),
			m_SuccessfulMeshCount( 0 ),
			m_MeshCode( 0 )
		{}

		/// デストラクタ
		~CurveInfoCreator(void) {}

		/**
		 * @brief 初期化
		 * @note	コンストラクタの直後に必ず呼ばれることが前提
		 * @param[in]	cRoadDB						道路データベース
		 * @param[in]	cMeshDB						２次メッシュデータベース
		 * @param[in]	cBaseMeshListFilePath		出力対象２次メッシュコードのリストのファイルパス
		 * @param[in]	cOutputDirPath				Shapeファイル出力ディレクトリのパス
		 * @param[in]	cLogFilePath				ログファイルのパス
		 * @param[in]	cVerificationLogFilePath	検証用ログファイルのパス
		 */
		void init(
			tstring cRoadDB,
			tstring cMeshDB,
			path cBaseMeshListFilePath,
			path cOutputDirPath,
			path cLogFilePath,
			path cVerificationLogFilePath
		);

		/**
		 * @brief データ変換を実行する
		 * @return	全てのメッシュの処理が正常に終了したか否か
		 */
		bool convert();

	private:
	// 非公開関数
		/// 処理中のメッシュのキャッシュを返す
		const CSPTableFinder currentMeshCache() { return m_pCacheManager->cache( m_MeshCode, m_OutputOptions.m_QueryWhereClause ); }

		/**
		 * @brief ２次メッシュ単位の変換
		 * @param[in]	cMeshCode	対象メッシュコード
		 */
		void convertMesh( long cMeshCode );

		/**
		 * @brief 実行時の情報を出力
		 * @param[in]	co			出力ストリーム
		 * @param[in]	cRoadDB		道路データベース
		 * @param[in]	cMeshDB		２次メッシュデータベース
		 */
		void outputExecInfo( std::ostream& co, tstring cRoadDB, tstring cMeshDB ) const;

		/**
		 * @brief 経過時間をstd::cerrとm_LogStreamに出力する
		 * @param[in]	cTask		タスク名
		 * @param[in]	cIsStart	タスクの開始か終了か（true：開始、false：終了）
		 */
		void outputElapsedTime( std::string cTask, bool cIsStart );

		/**
		 * @brief 対象リンクが処理対象リンクであるか否かを判定する
		 * @note	[Bug9694] 「細街路を対象外にする」という仕様を廃止。
		 * @param[in]	cpLink	対象リンク
		 * @return	判定結果
		 */
		bool isTargetLink( IFeaturePtr cpLink );

		/**
		 * @brief 対象リンクを含む一本道を取得する
		 * @note	まずcpLinkをcrSingleRoadに追加し、その両端にあるリンク群の追加処理を呼び出す
		 * @param[in]	cpLink			対象リンク（isTargetLink()でtrueと判定されている事が前提）
		 * @return	一本道
		 */
		SingleRoadPtr getSingleRoad( IFeaturePtr cpLink );

			/**
			 * @brief 対象リンクのcNodeIDのノード側にある一本道のリンク群を構成するセグメント群をcrSingleRoadのcIsFront側に再帰的に追加する
			 * @param[in]		cpLink			対象リンク
			 * @param[in]		cNodeID			対象ノードID
			 * @param[in]		cIsBack			取得した一本道リンク群を追加するのは、crSingleRoadのBack側か否（Front側）か
			 * @param[in]		cMeshCode		対象リンクが存在する２次メッシュのメッシュコード
			 * @param[in,out]	crSingleRoad	取得した一本道を返す変数
			 */
			void getOneSideSingleRoad( IFeaturePtr cpLink, long cNodeID, bool cIsBack, long cMeshCode, SingleRoad& crSingleRoad );

			/**
			 * @brief 長さが「カーブ内直線部分の最大値」以上であるか否かを判定する
			 * @param[in]	対象長さ
			 * @return	判定結果
			 */
			bool isLongStraight( double cLength ) const;

		/**
		 * @brief 角度累積の方向が左であるか否（右である）かを判定する
		 * @note	角度累積の方向が左右どちらかのみである事を前提とし、そうでない場合は例外を送出する。
		 * @param[in]	cDirection	角度累積の方向
		 * @return	判定結果
		 */
		bool CurveInfoCreator::isLeft( angle_accumulating::curve_direction::ECode cDirection ) const;

		/**
		 * @brief 一本道を変曲点で分割し、DSR列にして返す
		 * @note	ここで言う変曲点とは、曲線ではなくポリライン形状から、そう見なせる箇所のこと。
		 * @note	変曲点があると考えられるセグメントをその中点で分割するため、仮想セグメントができる。
		 * @param[in]	cpSingleRoad	一本道
		 * @return	DSR列
		 */
		DirectionalSRArray divideSingleRoad( SingleRoadPtr cpSingleRoad ) const;

		/**
		 * @brief 一本道上を等間隔な点群で区切り、各点の曲率半径の情報を作成する
		 * @return	「点と曲率半径」の列
		 */
		ROCPointInfosPtr createROCPointInfos( const DirectionalSR& crDirectionalSR );

			/**
			 * @brief 与えられたポリラインに含まれる長いセグメントに構成点を追加する
			 * @param[in,out]	cpPolyline	対象ポリライン
			 */
			void addPointsOnLongSegment( IPolyline4Ptr cpPolyline ) const;

		/**
		 * @brief 区間を作成する
		 * @param[in]	crDirectionalSRInfo	DSR情報
		 */
		void createSections( const DirectionalSRInfo& crDirectionalSRInfo );

		/**
		 * @brief ポリライン化
		 * @note	リンク形状のうち、ベジエ曲線上の「曲率半径を求める点」列に対応する部分を求め、それを区間とする。
		 * @param[in]	crDirectionalSRInfo	DSR情報
		 */
		void polylinize( const DirectionalSRInfo& crDirectionalSRInfo );

			/**
			 * @brief 対象区間情報のうち、代表点に関する情報を設定する
			 * @param[in]		crSingleRoad		着目中の一本道
			 * @param[in]		crSectionCandidate	着目中の区間候補
			 * @param[in,out]	cpSection			対象区間情報
			 */
			void setRepPointInfo(
				const SingleRoad& crSingleRoad,
				const SectionCandidate& crSectionCandidate,
				SectionPtr cpSection ) const;

		/**
		 * @brief 指定されたメッシュが出力対象メッシュコードであるか否かを判定する
		 * @param[in]	cMeshCode	対象２次メッシュコード
		 * @return	判定結果
		 */
		bool isTargetMesh( long cMeshCode ) const;

		/**
		 * @brief 全メッシュのデータを１組のShapeファイルとして出力する
		 */
		void createShape() const;

		/**
		 * @brief 指定されたメッシュのShapeファイルを出力する
		 * @param[in]	cMeshCode	対象２次メッシュコード
		 */
		void createShape( long cMeshCode ) const;

			/**
			 * @brief 指定された区間の情報をShapeファイルに出力する
			 * @param[in]		cpSection		対象区間
			 * @param[in,out]	crSHPHandle		Shapeハンドルクラスのインスタンス
			 * @param[in,out]	crDBFHandle		DBFハンドルクラスのインスタンス
			 * @param[in]		crFieldIndexMap	フィールド名とインデックスの対応表
			 * @param[in]		cMeshCode		対象２次メッシュコード
			 */
			void createShapeSection(
				const SectionPtr cpSection,
				CSHPHandle& crSHPHandle,
				CDBFHandle& crDBFHandle, 
				FieldIndexMap& crFieldIndexMap,
				long cMeshCode) const;

			/**
			 * @brief 点列と属性をShape/dbfハンドルクラスのインスタンスに書き込む
			 * @note	createShape()の順・逆共通の処理
			 * @param[in,out]	crCSHPHandle	Shapeハンドルクラスのインスタンス
			 * @param[in,out]	crCDBFHandle	DBFハンドルクラスのインスタンス
			 * @param[in]		crFieldIndexMap	フィールド名とインデックスの対応表
			 * @param[in]		crPointArray	点列
			 * @param[in]		crSection		対象Section
			 * @param[in]		cMeshCode		対象２次メッシュコード（検証用ログへの出力用）
			 */
			void writeShape(
				CSHPHandle& crCSHPHandle,
				CDBFHandle& crCDBFHandle,
				FieldIndexMap& crFieldIndexMap,
				const CSHPPointArray& crPointArray,
				const Section& crSection,
				long cMeshCode) const;

			/**
			 * @brief 区間群コンテナに区間を追加する
			 * @note	区間が所属するメッシュの区間群に追加する。
			 * @param[in]	cpSection
			 */
			void addSection( SectionPtr cpSection );

		/**
		 * @brief 空間参照をまだ記録していなければ、指定されたフィーチャから取得して記録する
		 * @param[in]	cpLink	取得元フィーチャ
		 */
		void setSpatialReferenceBy( const IFeaturePtr cpLink )
		{
			if (! m_pSpRef)
			{
				m_pSpRef = cpLink->GetShape()->GetSpatialReference();
			}
		}
	};

} // namespace curve_info
