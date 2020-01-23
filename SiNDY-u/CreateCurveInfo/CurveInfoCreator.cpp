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

#include "StdAfx.h"
#include <VersionInfo.h>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/timer.hpp>
#include <boost/bind.hpp>
#include <boost/format.hpp>
#include <sindy/workspace.h>
#include <crd_cnv/crd_cnv.h>
#include "CurveInfoCreator.h"
#include "Notifier.h"
#include "CurveInfoSchema.h"
#include "CurveInfoUtility.h"
#include "CircleFitEvaluator.h"
#include <RowContainer.h>


namespace curve_info
{
	using namespace utility;
	using namespace spatial_reference;

	void CurveInfoCreator::init(
		tstring cRoadDB,
		tstring cMeshDB,
		path cBaseMeshListFilePath,
		path cOutputDirPath,
		path cLogFilePath,
		path cVerificationLogFilePath
	)
	{
		// 出力ディレクトリがなければ作成
		createDirectory( cOutputDirPath );

		m_OutputDirPath = cOutputDirPath;

		using namespace sindy::schema;

		// 道路データベースに接続し、リンク、ノード、右左折禁止リンク列のフィーチャクラスを取得
		const IWorkspacePtr ipRoadWorkspace( getWorkspace( cRoadDB, notifier::critical::asCannotGetWorkspace ) );
		IFeatureClassPtr ipRoadLinkClass = getFeatureClass( ipRoadWorkspace, road_link::kTableName, notifier::critical::asCannotGetTable );
		IFeatureClassPtr ipRoadNodeClass = getFeatureClass( ipRoadWorkspace, road_node::kTableName, notifier::critical::asCannotGetTable );

		// ２次メッシュデータベースに接続し、２次メッシュフィーチャクラスを取得
		const IWorkspacePtr ipMeshWorkspace( getWorkspace( cMeshDB, notifier::critical::asCannotGetWorkspace ) );
		IFeatureClassPtr ipBaseMeshClass = getFeatureClass( ipMeshWorkspace, sindy::schema::basemesh::kTableName, notifier::critical::asCannotGetTable );

		// キャッシュマネージャを生成
		m_pCacheManager = CacheManagerPtr( new CacheManager( ipRoadLinkClass, ipRoadNodeClass, ipBaseMeshClass ) );

		// ２次メッシュリストを読み込む
		m_BaseMeshList = loadMeshList( cBaseMeshListFilePath, notifier::critical::asCannotOpenMeshListFile, notifier::critical::asMeshListFileIsInvalid );

		// ログファイル名の変更
		identifiedFileName( cLogFilePath );
		identifiedFileName( cVerificationLogFilePath );

		// ログ出力ストリームを開く
		m_LogStream.open( CString( cLogFilePath.string().c_str() ) );
		if (! m_LogStream)
		{
			notifier::critical::asCannotOpenLogFile( cLogFilePath.string() );
		}
		if (m_OutputOptions.m_OutputVerificationLog)
		{
			m_VerificationLogStream.open( CString( cVerificationLogFilePath.string().c_str() ) );
			if (! m_VerificationLogStream)
			{
				notifier::critical::asCannotOpenLogFile( cVerificationLogFilePath.string() );
			}
		}
		
		// ログに実行時の情報を出力
		outputExecInfo( m_LogStream, cRoadDB, cMeshDB );

		// 検証用ログのヘッダを出力
		if (m_OutputOptions.m_OutputVerificationLog)
		{
			adlib::outputSindyFreeStyleLogHeader( m_VerificationLogStream, boost::assign::list_of
				("FLAG")("LAYER")("OBJECTID")("LON")("LAT")("ROC")("REPLINKID")("WAYTORP")("３点の間隔")("角度")("COMMENT")
			);
		}
	}

	void CurveInfoCreator::outputExecInfo( std::ostream& co, tstring cRoadDB, tstring cMeshDB ) const
	{
		CVersion aCVersion;
		CString aFileVersion = aCVersion.GetFileVersion();
		CString aProductVersion = aCVersion.GetProductVersion();

		TCHAR aModuleFileName[MAX_PATH+1]={0};
		::GetModuleFileName( NULL, aModuleFileName, MAX_PATH );
		path aModuleFilePath = aModuleFileName;

		// ログのヘッダ出力
		using namespace std;
		outputSindyFreeStyleLogHeader( co, utility::log_format::fieldNames() );

		co << "# 開始時刻 : " << boost::lexical_cast<string>( boost::posix_time::second_clock::local_time() ) << endl;
		co << "# " << aModuleFilePath.leaf() << " F:" << CT2A( aFileVersion.GetString() ) << " P:" << CT2A( aProductVersion.GetString() ) << endl;
		co << "# 道路データベース : " << cRoadDB << endl;
		co << "# ２次メッシュデータベース : " << cMeshDB << endl;
		co << "# 対象メッシュ数 : " << m_BaseMeshList.size() << endl;
		co << "# 領域削除のための最大曲率半径（低速域用）（m） : " << m_OutputOptions.maxROCForRegionDeletion( false ) << endl;
		co << "# 領域削除のための最大曲率半径（高速域用）（m） : " << m_OutputOptions.maxROCForRegionDeletion( true ) << endl;
		co << "# 区間の最大曲率半径（低速域用）（m） : " << m_OutputOptions.maxROCOfSection( false ) << endl;
		co << "# 区間の最大曲率半径（高速域用）（m） : " << m_OutputOptions.maxROCOfSection( true ) << endl;
		co << "# 直線と見なす角度（度） : " << m_OutputOptions.m_StraightAngle << endl;
		co << "# 曲率半径を求める点の間隔(m) : " << m_OutputOptions.m_ROCPointInterval << endl;
		co << "# カーブ認定角度（度） : " << m_OutputOptions.m_CurveCertifyAngle << endl;
		co << "# 円弧がベジエ曲線にフィットするかを評価する点の間隔(m) : " << m_OutputOptions.m_FittingPointInterval << endl;
		co << "# 円弧がベジエ曲線にフィットすると見なす閾値(m) : " << m_OutputOptions.m_FittingThreshold << endl;
		co << "# 区間に対応する円弧の最小角度（度） : " << m_OutputOptions.m_SectionMinAngle << endl;
		co << "# 対象リンク指定クエリ : " << (m_OutputOptions.m_QueryWhereClause.IsEmpty() ? "なし" : CStringA(m_OutputOptions.m_QueryWhereClause)) << endl;
		co << "# 出力単位 : " << (m_OutputOptions.m_CreateEachMesh ? "メッシュごとに出力" : "１組のShapeファイル群にまとめて出力") << endl;
		co << "# SHP05のディレクトリ構成で出力 : " << (m_OutputOptions.m_SHP05DirConfig ? "する" : "しない") << endl;
		co << "# 検証用ログ作成 : " << (m_OutputOptions.m_OutputVerificationLog ? "する" : "しない") << endl;
		co << "# 処理過程をログファイルに出力 : " << (m_OutputOptions.m_LogHandlingProcess ? "する" : "しない") << endl;
		co << "# フィット評価過程をログファイルに出力 : " << (m_OutputOptions.m_LogFitEvaluatingProcess ? "する" : "しない") << endl;
	}

	void CurveInfoCreator::outputElapsedTime( std::string cTask, bool cIsStart )
	{
		static boost::timer t;
		if (cIsStart)
		{
			t.restart();
			std::cerr << cTask;
			m_LogStream << "# " << cTask << " 開始" << std::endl;
		}
		else
		{
			std::cerr << "(" << t.elapsed() << "sec)" << std::endl;
			m_LogStream << "# " << cTask << " 終了" << "(" << t.elapsed() << "sec)"<< std::endl;
		}
	}

	bool CurveInfoCreator::convert()
	{
		// ２次メッシュごとに処理
		BOOST_FOREACH (long aMeshCode, m_BaseMeshList)
		{
			// 周囲9メッシュ以外のキャッシュを解放
			m_pCacheManager->clearExcept( surroundingMeshCodes( aMeshCode ) );

			try
			{
				convertMesh( aMeshCode );
					
				// メッシュごとに出力
				if (m_OutputOptions.m_CreateEachMesh)
				{
					createShape( aMeshCode );
				}

				++m_SuccessfulMeshCount;
			}
			catch (std::runtime_error& e)
			{
				// 例外が発生したら、エラーメッセージを出力してこのメッシュをスキップ
				notifier::asAbortedByUnexpectedData( std::cerr, aMeshCode );
				notifier::asAbortedByUnexpectedData( m_LogStream, aMeshCode );
				std::cerr << std::endl;		// 途中でやめるとプロンプトの表示が中途半端になるので改行しておく
			}
		}

		try
		{
			// １組のShapeファイル群にまとめて出力
			if (! m_OutputOptions.m_CreateEachMesh)
			{
				createShape();
			}
		}
		catch (std::runtime_error& e)
		{
			notifier::asAbortedByUnexpectedData( m_LogStream );
			std::cerr << std::endl;
		}

		std::cerr << "--------------------------------------------------" << std::endl;
		m_LogStream << "#--------------------------------------------------" << std::endl;

		// 終了時刻をログに出力
		m_LogStream << "# 終了時刻 : " << boost::lexical_cast<std::string>( boost::posix_time::second_clock::local_time() ) << std::endl;

		return m_SuccessfulMeshCount == m_BaseMeshList.size();
	}

	bool CurveInfoCreator::isTargetLink( IFeaturePtr cpLink )
	{
		using namespace sindy::schema;

		// 表示種別による判定
		long aRoadClass = cpLink->GetValue( fieldIndex( cpLink, road_link::kTableName, road_link::kRoadClass ) );
		switch (aRoadClass)
		{
			using namespace road_link::road_class;
			// フェリー航路は対象外
			case kFerryNoDisp:
			case kFerryS2:
			case kFerryS3:
			case kFerryS4:
				return false;
			default:
				break;
		}

		// 条件無し通行禁止と通行不可のリンクは対象外とする（Bug9642）
		if (! isPassable( cpLink ))
			{ return false; }

		return true;
	}

	/**
	 * @brief 指定されたリンクの一方通行規制の種類を返す
	 * @param[in]	cpLink	対象リンク
	 * @return	一方通行規制
	 */
	oneway_type::ECode onewayType( IFeaturePtr cpLink )
	{
		using namespace sindy::schema;
		long aOneway = cpLink->GetValue( fieldIndex( cpLink, road_link::kTableName, road_link::kOneway ) );
		switch (aOneway)
		{
			using namespace road_link::oneway;
			case kNoOneway:
			case kForeWithReg:			// 条件付き規制は規制無しと同じ扱い
			case kReverseWithReg:
			case kForeReverseWithReg:
				return oneway_type::none;
			case kFore:
				return oneway_type::fore;
			case kReverse:
				return oneway_type::back;
			default:
				ATLASSERT(0);
				return oneway_type::none;
		}
	}

	namespace get_oneside_single_road
	{
		/**
		 * @brief 対象フィーチャが存在する２次メッシュのメッシュコード群を返す
		 * @note	「メッシュ境界上ノードが接触しているメッシュ群」を求めるために作成。
		 * @note	方法は幾つか考えられるが、ここでは「ノード→メッシュ」の空間検索のコストがそれほど高くなく、
		 *			機会もメッシュ境界のみなので問題ないと考えて採用。
		 * @param[in]	cpFeature			対象フィーチャ
		 * @param[in]	cpBaseMeshClass		２次メッシュフィーチャクラス
		 * @return	メッシュコード群
		 */
		MeshCodes existingMesh( IFeaturePtr cpFeature, IFeatureClassPtr cpBaseMeshClass )
		{
			using namespace sindy::schema;

			MeshCodes aMeshCodes;
			IFeatureCursorPtr ipFeatureCursor = cpBaseMeshClass->_Search( AheGetSpatialFilter( cpFeature->GetShape(), cpBaseMeshClass, esriSpatialRelIntersects ), VARIANT_TRUE );
			IFeaturePtr ipFeature;
			while (ipFeatureCursor->NextFeature( &ipFeature ) == S_OK && ipFeature)
			{
				long aMeshCode = ipFeature->GetValue( fieldIndex( ipFeature, basemesh::kTableName, basemesh::kMeshCode ) );
				aMeshCodes.insert( aMeshCode );
			}

			return aMeshCodes;
		}

		typedef CIndexedContainer::IteratorContainer IteratorContainer;

		/**
		 * @brief 指定されたキャッシュ内で、対象ノードに接続されているリンク群を返す
		 * @param[in]	cNodeID		対象ノード
		 * @param[in]	cpLinkTable	検索対象リンクキャッシュ
		 * @return	リンク群
		 */
		IteratorContainer connectedLinks( long cNodeID, const CSPTableContainer cpLinkTable )
		{
			using namespace sindy::schema;
			const IteratorContainer& itrsFrom = cpLinkTable->finds_by_key( road_link::kFromNodeID, cNodeID );
			const IteratorContainer& itrsTo = cpLinkTable->finds_by_key( road_link::kToNodeID, cNodeID );
			IteratorContainer itrs;
			std::copy( itrsFrom.begin(), itrsFrom.end(), std::inserter( itrs, itrs.begin() ) );
			std::copy( itrsTo.begin(), itrsTo.end(), std::inserter( itrs, itrs.begin() ) );
			return itrs;
		}

	} // namespace get_oneside_single_road

	void CurveInfoCreator::getOneSideSingleRoad( IFeaturePtr cpLink, long cNodeID, bool cIsBack, long cMeshCode, SingleRoad& crSingleRoad )
	{
		using namespace get_oneside_single_road;
		using namespace sindy::schema;
		using namespace utility::sindylib_base;

		const long aCurrentLinkID = cpLink->OID;
		const long aCurrentMeshCode = cMeshCode;

		// 対象リンクに通行禁止規制があったら終了
		if (! isPassable( cpLink ))
			{ return; }

		long aNextMeshCode = cMeshCode;		///< 次のリンクが存在する２次メッシュのメッシュコード

		const CSPTableFinder& pTableFinder = m_pCacheManager->cache( cMeshCode, m_OutputOptions.m_QueryWhereClause );	///< 着目メッシュのキャッシュ

		// 対象ノードが２次メッシュ境界上ノードなら、次のメッシュはその向こう側のメッシュとする
		const CSPTableContainer pNodeTable = pTableFinder->FindTable( road_node::kTableName );
		_IRowPtr ipNode = irow( *pNodeTable->find_by_key( kObjectID, cNodeID ) );
		long aNodeClass = ipNode->GetValue( fieldIndex( ipNode, road_node::kTableName, road_node::kNodeClass ) );
		if (aNodeClass == road_node::node_class::kEdge)
		{
			// 次に扱うべきメッシュを特定する
			MeshCodes aMeshCodes = existingMesh( ipNode, m_pCacheManager->meshClass() );		///< メッシュ境界上ノードが接触しているメッシュ群
			ATLASSERT( aMeshCodes.size() == 2);	// メッシュの4隅にノードが存在している事はない（RC1001）ので、接触しているメッシュの数は必ず2であるはず
			BOOST_FOREACH (long aMeshCode, aMeshCodes)
			{
				// 着目メッシュと同一でない唯一のメッシュが次に扱うべきメッシュである
				if (aMeshCode != cMeshCode)
				{
					aNextMeshCode = aMeshCode;
				}
			}
		}
		// TODO: メッシュ境界上ノードをSRに含めると、それが存在する事によってベジエ曲線の形状が変化し、
		// 存在しない場合とは結果が微妙に変わってしまう。厳密にはメッシュ境界上ノードは除外した方がよい。
		// （交差点ノードや属性変化点ノードにも同じ事が言えるが…。）

		const CSPTableFinder pNextMeshTableFinder = m_pCacheManager->cache( aNextMeshCode, m_OutputOptions.m_QueryWhereClause );	///< 次に扱うべきメッシュのキャッシュ
		const CSPRoadLinkTable pNextLinkTable = road_link_table( pNextMeshTableFinder->FindTable( road_link::kTableName ) );	///< 次のリンクを含むリンクキャッシュ

		// 着目ノードに接続するリンク群のそれぞれについて
		IteratorContainer aConnectedLinks = connectedLinks( cNodeID, pNextLinkTable );
		long aTargetLinkCount = 0;													// 接続リンク群のうち、処理対象リンクの条件を満たすものの数
		IFeaturePtr ipNextLink;													// 接続リンク群のうち、最後に見つかった（２差路の場合は唯一の）処理対象リンク
		BOOST_FOREACH (CContainerBase::iterator itLink, aConnectedLinks)
		{
			IFeaturePtr ipLink = irow( *itLink );
			ATLASSERT( ipLink );
			if (! ipLink) { continue; }
			long aLinkID = ipLink->OID;

			// 着目中のリンクならスキップ
			if (aCurrentLinkID == aLinkID)
				{ continue; }

			// 「高速域か否か」が作成中の一本道と一致しなければスキップ（Bug9619）
			if (isHighSpeedArea( ipLink ) != crSingleRoad.m_IsHighSpeedArea)
				{ continue; }

			// 処理対象リンクの条件を満たさなければスキップ
			if (! isTargetLink( ipLink ))
				{ continue; }
			++aTargetLinkCount;
			ipNextLink = ipLink;
		}

		// 処理対象リンクのみを考えた時、その数が１本（２差路）でなければ終了
		if (aTargetLinkCount != 1)
			{ return; }
		// ここまで来れば、次のリンクの候補がipNextLinkただ１つに絞られる。

		// 次のリンクが処理済みリンクなら終了（Bug9701）
		if (m_ProcessedLinkIDs.find( ipNextLink->OID ) != m_ProcessedLinkIDs.end())
			{ return; }

		// 次のリンクに通行禁止規制があったら終了
		if (! isPassable( ipNextLink ))
			{ return; }

		// メッシュ境界でなければ右左折禁止規制をチェック
		if (cMeshCode == aNextMeshCode)
		{
			// 「着目リンク→次のリンク」、「次のリンク→着目リンク」の両方が通行できなければ終了（Bug9642）
			if (! isPassableByTurnreg( cpLink, ipNextLink, *pNextLinkTable ) &&
				! isPassableByTurnreg( ipNextLink, cpLink, *pNextLinkTable ))
				{ return; }
		}

		// 状況確認
		using namespace sindy::schema;
		long aFromNodeID = ipNextLink->GetValue( fieldIndex( ipNextLink, road_link::kTableName, road_link::kFromNodeID ) );
		long aToNodeID = ipNextLink->GetValue( fieldIndex( ipNextLink, road_link::kTableName, road_link::kToNodeID ) );
		bool aConnectionNodeIsFrom = (aFromNodeID == cNodeID);				///< 作成中の一本道の端と接続されているのはリンクのFromノードであるか否（Toノードである）か
		long aEndNodeID = aConnectionNodeIsFrom ? aToNodeID : aFromNodeID;

		// 一方通行規制の種類が作成中の一本道と合わないものであれば終了（Bug9642）
		long aLinkOnewayType = onewayType( ipNextLink );
		if (aLinkOnewayType == oneway_type::none)
		{
			if (crSingleRoad.m_Oneway != oneway_type::none)
				{ return; }
		}
		else
		{
			bool aLinkOnewayIsFore =  ( aLinkOnewayType == oneway_type::fore );							///< リンクの一方通行が順方向か否か
			bool aSROnewayIsFore = ( crSingleRoad.m_Oneway == oneway_type::fore );						///< 一本道の一方通行が順方向か否か
			bool aLinkOnewayIsFromCurrentNode = ( aConnectionNodeIsFrom == aLinkOnewayIsFore );			///< リンクの一方通行の方向が着目ノードをFromとする方向か否か
			bool aCurrentNodeIsSROnewayToSide = ( aSROnewayIsFore == cIsBack );							///< 着目ノードは一本道の一方通行の向かう側であるか否か
			bool aLinkOnewayIsOK = ( aCurrentNodeIsSROnewayToSide == aLinkOnewayIsFromCurrentNode );	///< 一本道にリンクを追加しても良いか否か
			if (! aLinkOnewayIsOK)
				{ return; }
		}

		// ２差路の行き先にある唯一のリンクをcrSingleRoadに追加
		crSingleRoad.addLink( ipNextLink, aConnectionNodeIsFrom, cIsBack );
		m_ProcessedLinkIDs.insert( ipNextLink->OID );

		// 再帰
		getOneSideSingleRoad( ipNextLink, aEndNodeID, cIsBack, aNextMeshCode, crSingleRoad );
	}

	SingleRoadPtr CurveInfoCreator::getSingleRoad( IFeaturePtr cpLink )
	{
		SingleRoadPtr pSingleRoad( new SingleRoad(
			isHighSpeedArea( cpLink ),
			onewayType( cpLink ),
			AheGetSpatialReference( cpLink ) ) );
		SingleRoad& rSingleRoad = *pSingleRoad;		// TODO: shared_ptrが指すオブジェクトを直接参照するのはよくない？

		using namespace sindy::schema;
		long aFromNodeID = cpLink->GetValue( fieldIndex( cpLink, road_link::kTableName, road_link::kFromNodeID ) );
		long aToNodeID = cpLink->GetValue( fieldIndex( cpLink, road_link::kTableName, road_link::kToNodeID ) );

		// SingleRoadの最初のリンクとしてcpLinkを追加
		rSingleRoad.addLink( cpLink, true, true );

		getOneSideSingleRoad( cpLink, aFromNodeID,	false, m_MeshCode,  rSingleRoad );		// Fromノード側の一本道リンク群をSingleRoadのFront側に追加
		getOneSideSingleRoad( cpLink, aToNodeID,	true, m_MeshCode,	rSingleRoad );		// Toノード側の一本道リンク群をSingleRoadのBack側に追加

		return pSingleRoad;
	}

	bool CurveInfoCreator::isLeft( angle_accumulating::curve_direction::ECode cDirection ) const
	{
		switch (cDirection)
		{
			using namespace angle_accumulating::curve_direction;
			case left:
				return true;
			case right:
				return false;
			case straight:
			case both:
			default:
				notifier::safeAssert( false, m_LogStream );
				throw std::runtime_error("一本道の方向決定に失敗");
				break;
		}
	}

	/// CurveInfoCreator::divideSingleRoadの一部
	namespace divide_single_road
	{
		/**
		 * @brief 指定された範囲にあるセグメント列の長さを求める
		 * @param[in]	cBeginSegmentIt	範囲の最初のセグメント
		 * @param[in]	cEndSegmentIt	範囲の最後のセグメント
		 * @return	長さ（度）
		 */
		double length( SegmentIt citBeginSegment, SegmentIt citEndSegment )
		{
			double aLength = 0;
			for (SegmentIt it = citBeginSegment; it != boost::next( citEndSegment ); ++it)
			{
				aLength += (*it)->length();
			}
			return aLength;
		}

		/// 変曲点に関する情報
		struct InflectionInfo
		{
			bool		m_CurrentDirectionIsLeft;			///< 最後に見つかった変曲点の後、左右どちらに曲がっているか
			SegmentIt	m_itInflectionSegment;				///< 変曲点セグメント
			SegmentPtr	m_pSegmentNextToInflectionPoint;	///< 変曲点の直後の仮想セグメント

			/// コンストラクタ
			InflectionInfo(
				bool cCurrentDirectionIsLeft,
				SegmentIt citInflectionSegment,
				SegmentPtr cpSegmentNextToInflectionPoint
			) :
				m_CurrentDirectionIsLeft( cCurrentDirectionIsLeft ),
				m_itInflectionSegment( citInflectionSegment ),
				m_pSegmentNextToInflectionPoint( cpSegmentNextToInflectionPoint )
			{}
		};
		typedef std::tr1::shared_ptr< InflectionInfo > InflectionInfoPtr;

	} // namespace divide_single_road

	DirectionalSRArray CurveInfoCreator::divideSingleRoad( SingleRoadPtr cpSingleRoad ) const
	{
		ATLASSERT( cpSingleRoad );
		const SingleRoad& rSingleRoad = *cpSingleRoad;	// TODO: shared_ptrが指すものを参照するのはよくない？

		using namespace angle_accumulating;
		using namespace divide_single_road;

		LRAccumulating aLRAccumulating(						///< 左右の角度累積状態
			rSingleRoad,
			m_OutputOptions.m_StraightAngle,
			m_OutputOptions.m_CurveCertifyAngle,
			m_pSpRef );

		DirectionalSRArray aDSRArray;								///< 一本道を分割する事によって作成されるDSR列
		SegmentIt itCurrentDSRBeginSegment = rSingleRoad.begin();	///< 作成中のDSRの始端セグメント（仮想セグメントは除く）
		double aWayFromOrigin1 = 0;									///< 元の一本道の始点から、作成中のDSRの始点までの道のり（度）
		InflectionInfoPtr pInflectionInfo;							///< 最後に見つかった変曲点に関する情報

		// 一本道を構成する各セグメントについて
		for (SegmentIt itCurrentSegment = rSingleRoad.begin(); itCurrentSegment != rSingleRoad.end(); ++itCurrentSegment )
		{
#ifdef _DEBUG
			long aLinkID = (*itCurrentSegment)->m_LinkID;
#endif
			// 最初のセグメントならスキップ
			if (itCurrentSegment == rSingleRoad.begin())
				{ continue; }

			SegmentIt itPrevSegment = boost::prior( itCurrentSegment );
			double aAngle = angle( **itPrevSegment, **itCurrentSegment );	///< 前のセグメントとのなす角

			// 角度累積状態を更新
			aLRAccumulating.update( aAngle, itCurrentSegment );
			
			// 変曲点が見つかっていなければスキップ
			IPointPtr pInflectionPoint = aLRAccumulating.m_pInflectionPoint;	///< 変曲点
			if (! pInflectionPoint)
				{ continue; }
			SegmentIt itInflectionSegment = aLRAccumulating.m_itInflectionSegment;	///< 変曲点セグメント
			ATLASSERT( itInflectionSegment != rSingleRoad.end() );
			// 変曲点が見つかったら、そこで一本道を分割する

			// 左右の決定
			// 変曲点が見つかった時点の２セグメントでどちらに曲がっているかを調べ、変曲点の前はその逆、変曲点の後は順方向に曲がっているとする
			bool aIsLeft = isLeft( aLRAccumulating.m_CurrentDirection );

			// 変曲点セグメントの始点から変曲点までに当たる仮想セグメントを作成する
			IPointPtr& ipFromPoint = (*itInflectionSegment)->m_pFromPoint;
			IPointPtr& ipToPoint = (*itInflectionSegment)->m_pToPoint;
			SegmentPtr ipBeforeInflectionSegment( new Segment( ipFromPoint, pInflectionPoint ) );	///< 変曲点の直前の仮想セグメント
			if (m_OutputOptions.m_LogHandlingProcess)
			{
				m_LogStream << utility::log_format::pointRecord( pInflectionPoint->GetX(), pInflectionPoint->GetY(), "変曲点" );
			}

			double aWayFromOrigin2 = length( rSingleRoad.begin(), boost::prior( itInflectionSegment ) ) + ipBeforeInflectionSegment->length();	///< 元の一本道の始点から変曲点までの道のり（度）
			SingleRoad::Range aRange = std::make_pair(
				aWayFromOrigin1 / rSingleRoad.length(), 
				aWayFromOrigin2 / rSingleRoad.length() );			///< 元の一本道上における、作成中のDSRの範囲

			// 新しいDSRを作成
			DirectionalSRPtr pCurrentDSR = DirectionalSRPtr( new DirectionalSR(
				! aIsLeft,
				aRange,
				rSingleRoad.m_IsHighSpeedArea,
				rSingleRoad.m_Oneway,
				rSingleRoad.m_pSpRef ) );
			// １つ前の変曲点が存在する場合、その直後の仮想セグメントを追加
			if (pInflectionInfo)
			{
				pCurrentDSR->add( pInflectionInfo->m_pSegmentNextToInflectionPoint );
			}
			// 始端セグメントから変曲点セグメントの１つ前までのセグメント列を追加
			std::copy( itCurrentDSRBeginSegment, itInflectionSegment, std::back_inserter( *pCurrentDSR ) );
			// 変曲点の直前の仮想セグメントを追加
			pCurrentDSR->add( ipBeforeInflectionSegment );

			// 作成したDSRを登録
			aDSRArray.push_back( pCurrentDSR );

			itCurrentDSRBeginSegment = boost::next( itInflectionSegment );
			aWayFromOrigin1 = aWayFromOrigin2;

			// 変曲点から変曲点セグメントの終点までに当たる仮想セグメントを作成
			SegmentPtr pSegmentNextToInflectionPoint = SegmentPtr( new Segment( pInflectionPoint, ipToPoint ) );

			// 変曲点に関する情報を記録
			pInflectionInfo = InflectionInfoPtr( new InflectionInfo( aIsLeft, itInflectionSegment, pSegmentNextToInflectionPoint ) );

			// 累積状態を初期化して、「変曲点セグメントとその次のセグメント」から再開
			itCurrentSegment = itInflectionSegment;
			aLRAccumulating.reset();
		}

		// 変曲点が１つ以上見つかっている場合
		if (pInflectionInfo)
		{
			// 最後に見つかった変曲点から、元の一本道の終点までのセグメント列に当たるDSRを作成
			SingleRoad::Range aRange = std::make_pair(
				aWayFromOrigin1 / rSingleRoad.length(), 
				1
			);
			DirectionalSRPtr pCurrentDSR = DirectionalSRPtr( new DirectionalSR(
				pInflectionInfo->m_CurrentDirectionIsLeft,
				aRange,
				rSingleRoad.m_IsHighSpeedArea,
				rSingleRoad.m_Oneway,
				rSingleRoad.m_pSpRef ) );
			// 変曲点セグメントの次のセグメントからcpSingleRoadの終点までのセグメントを、作成中のDSRに追加
			ATLASSERT( pInflectionInfo->m_itInflectionSegment != rSingleRoad.end() );	// 見つかっているはず
			// 最後の変曲点の直後の仮想セグメントを追加
			pCurrentDSR->add( pInflectionInfo->m_pSegmentNextToInflectionPoint );

			// 変曲点セグメントの次から元の一本道の最後までのセグメント列を追加
			std::copy( boost::next( pInflectionInfo->m_itInflectionSegment ), rSingleRoad.end(), std::back_inserter( *pCurrentDSR ) );
			// 作成したDSRを登録
			aDSRArray.push_back( pCurrentDSR );
		}
		// 変曲点が１つも見つかっていない場合
		else
		{
			// 元の一本道は「右に曲がっている」「左に曲がっている」「真っ直ぐ」のいずれかであるはず。
			// 「真っ直ぐ」の場合はその領域にカーブは存在しないものと見なし、その情報は破棄する。
			if (aLRAccumulating.direction() == angle_accumulating::curve_direction::straight)
			{
				return aDSRArray;		// 空のまま返す
			}

			// 元の一本道全体に当たるDSRを作成
			SingleRoad::Range aRange = std::make_pair( 0, 1 );
			DirectionalSRPtr pCurrentDSR = DirectionalSRPtr( new DirectionalSR(
				isLeft( aLRAccumulating.direction() ),
				aRange,
				rSingleRoad.m_IsHighSpeedArea,
				rSingleRoad.m_Oneway,
				rSingleRoad.m_pSpRef ) );
			std::copy( rSingleRoad.begin(), rSingleRoad.end(), std::back_inserter( *pCurrentDSR ) );
			aDSRArray.push_back( pCurrentDSR );
		}

		return aDSRArray;
	}

	/**
	 * @brief CurveInfoCreator::createROCPointInfosの一部
	 */
	namespace create_roc_point_infos
	{
		/**
		 * @brief 与えられたポリラインのセグメント長の平均値を求める
		 * @param[in]	cpPolyline	対象ポリライン
		 * @return	セグメント長の平均値
		 */
		double segmentLengthAverage( const IPolyline4Ptr cpPolyline )
		{
			double aSegmentLengthSum = 0;		///< セグメント長の和
			ISegmentCollectionPtr ipSegmentCol( cpPolyline );
			for (long i = 0; i < ipSegmentCol->GetSegmentCount(); ++i)
			{
				_ISegmentPtr ipSegment = ipSegmentCol->GetSegment( i );
				aSegmentLengthSum += ipSegment->GetLength();
			}
			return aSegmentLengthSum / ipSegmentCol->GetSegmentCount();
		}
	} // namespace create_roc_point_infos

	ROCPointInfosPtr CurveInfoCreator::createROCPointInfos( const DirectionalSR& crDirectionalSR )
	{
		using namespace create_roc_point_infos;
		using namespace circle_fit_evaluator;

		// DSR全体のジオメトリを作成
		IPolycurve3Ptr ipPolycurve( crDirectionalSR.geometry() );
		ATLASSERT( ipPolycurve );

		// 長いセグメント上に構成点を追加
		addPointsOnLongSegment( ipPolycurve );

		// ベジエ曲線群化
		HRESULT hr = ipPolycurve->Smooth( 0 );
		ATLASSERT( hr == S_OK );
		IPolycurve3Ptr& ipBezierCurves = ipPolycurve;
		setSpatialReference( ipBezierCurves, m_pSpRef );

		static const CircleFitEvaluator::DistanceOf3PointsArray aDistanceOf3PointsArray = boost::assign::list_of
			( 10 )( 20 )( 30 )( 40 )( 60 )( 80 )( 100 )( 200 )( 500 )( 1000 );		///< 「３点の間隔」の候補の集合	TODO: コマンドライン引数で指定可能にすべき？

		// 着目DSRの第１セグメントの始点と終点により、度→mの変換係数を決定
		SegmentPtr pFirstSegment = NONZERO( *crDirectionalSR.begin() );
		ATLASSERT( pFirstSegment->valid() );
		double aDegreeToMeterCoef = degreeToMeterCoef(													///< 長さの単位を度からmに変換するために掛けるべき係数
			pFirstSegment->m_pFromPoint->GetX(), pFirstSegment->m_pFromPoint->GetY(),
			pFirstSegment->m_pToPoint->GetX(), pFirstSegment->m_pToPoint->GetY()
		);
		if (m_OutputOptions.m_LogHandlingProcess)
		{
			m_LogStream << utility::log_format::freeComment( std::string("度→mの変換係数=") + utility::toStr( aDegreeToMeterCoef, 20 ) );
		}

		const double aROCPointInterval = m_OutputOptions.m_ROCPointInterval / aDegreeToMeterCoef;					///< 曲率半径を求める点（分割点）の間隔（度）
		const double aFittingPointInterval = m_OutputOptions.m_FittingPointInterval / aDegreeToMeterCoef;			///< 円弧がベジエ曲線にフィットするかを評価する点の間隔（度）
		const double aFittingThreshold = m_OutputOptions.m_FittingThreshold / aDegreeToMeterCoef;					///< 円弧がベジエ曲線にフィットすると見なす閾値（度）
#ifdef _DEBUG
		double aBezierCurvesLength = ipBezierCurves->Length;
#endif
		const double aLengthByInterval = ipBezierCurves->Length / aROCPointInterval;					///< aROCPointIntervalを1とした時のベジエ曲線群の長さ
		const long aPointIndexMax =	static_cast<long>( aLengthByInterval );								///< 分割点のインデックスの最大値

		const SingleRoad::Range rRange = crDirectionalSR.m_Range;
		const double aROCPointIntervalRatio = ( rRange.second - rRange.first ) / aLengthByInterval;		///< １点進むごとに、元の一本道のどれだけの割合を進むか

		ROCPointInfosPtr pROCPointInfos( new ROCPointInfos );

		// TODO: DSRの始点ではなく中点を基準にすると、より対称性が高まる（結果が一意であればよい、とするなら必要ないが…）

		// 各分割点について
		for (long aPointIndex = 0; aPointIndex <= aPointIndexMax; ++aPointIndex)
		{
			double aCurrentPointRatio = rRange.first + aROCPointIntervalRatio * aPointIndex;	///< 元の一本道上の着目点の位置

			// 着目する点の位置を決定
			IPointPtr ipPoint2 = newGeometry< IPointPtr >( CLSID_Point, m_pSpRef );
			const double aDAC2 = aROCPointInterval * aPointIndex;		///< ３点の内の第２点の、ペジエ曲線群の始点からの道のり（DistanceAlongCurve）
			hr = ipBezierCurves->QueryPoint( esriNoExtension, aDAC2, VARIANT_FALSE, ipPoint2 );
			ATLASSERT( hr == S_OK );
			if (! ipPoint2)
			{
				notifier::safeAssert(0, m_LogStream);
				continue;
			}
			setSpatialReference( ipPoint2, m_pSpRef );

			if (m_OutputOptions.m_LogHandlingProcess)
			{
				m_LogStream << utility::log_format::pointRecord( ipPoint2->GetX(), ipPoint2->GetY(), "曲率半径を求める点" );
			}

			// 評価
			CircleFitEvaluator aCircleFitEvaluator(
				ipBezierCurves,
				ipPoint2,
				aDAC2,
				aDistanceOf3PointsArray,
				aFittingPointInterval,
				aFittingThreshold,
				aDegreeToMeterCoef,
				m_LogStream,
				m_OutputOptions.m_LogFitEvaluatingProcess);
			// 評価すべき位置でなければスキップ
			ResultsPtr pResults = aCircleFitEvaluator.evaluate();
			if (! pResults)
				{ continue; }

			notifier::safeAssert( 0 <= aCurrentPointRatio && aCurrentPointRatio <= 1, m_LogStream);	// TODO: doubleの演算誤差によって1を超える事があるかも…。

			// 点と曲率半径を記録
			pROCPointInfos->push_back( ROCPointInfoPtr( new ROCPointInfo( ipPoint2, pResults, aCurrentPointRatio ) ) );
		}

		return pROCPointInfos;
	}

	void CurveInfoCreator::addPointsOnLongSegment( IPolyline4Ptr cpPolyline ) const
	{
		using namespace create_roc_point_infos;

		// セグメント長の平均値から、点の間隔の許容値を求める
		double aLengthThreshold = segmentLengthAverage( cpPolyline ) * 2;	///< セグメントが「長い」と判断する閾値
	
		// ポリラインを構成する各セグメントについて
		ISegmentCollectionPtr ipSegmentCol( cpPolyline );
		long aSegmentCount = ipSegmentCol->GetSegmentCount();
		for (long i = 0; i < aSegmentCount; ++i)
		{
			_ISegmentPtr ipSegment = ipSegmentCol->GetSegment( i );
			double aSegmentLength = ipSegment->GetLength();

			// 長いセグメントがあれば構成点を追加する
			if (aLengthThreshold < aSegmentLength)
			{
				IPointPtr ipNewPoint = newGeometry< IPointPtr >( CLSID_Point, m_pSpRef );
				ipSegment->QueryPoint( esriNoExtension, 0.5, VARIANT_TRUE, ipNewPoint );
				ATLASSERT( ipNewPoint );
				setSpatialReference( ipNewPoint, m_pSpRef );

				if (m_OutputOptions.m_LogHandlingProcess)
				{
					m_LogStream << utility::log_format::pointRecord( ipNewPoint->GetX(), ipNewPoint->GetY(), std::string( "長いセグメントに追加された点 SegmentIndex=" ) + boost::lexical_cast< std::string >( i ) );
				}

				IPointCollectionPtr ipPointCol2 = newGeometry< IPointCollectionPtr >( CLSID_Polyline, m_pSpRef );
				ipPointCol2->AddPoint( ipNewPoint );
				IPointCollectionPtr( cpPolyline )->InsertPointCollection( i + 1, ipPointCol2 );
				setSpatialReference( ipPointCol2, m_pSpRef );

				// 着目中のセグメントが追加された点によって分割されてできた２セグメントのうち、前のものから処理が再開されるようにする
				// （それがまだ長ければさらに点が追加されるように）
				--i;
				aSegmentCount = ipSegmentCol->GetSegmentCount();	// セグメント数を更新
			}
		}
	}

	void CurveInfoCreator::writeShape(
		CSHPHandle& crCSHPHandle,
		CDBFHandle& crCDBFHandle,
		FieldIndexMap& crFieldIndexMap,
		const CSHPPointArray& crPointArray,
		const Section& crSection,
		long cMeshCode) const
	{
		long aOID = crCSHPHandle.write_Object( &CSHPObject( SHPT_ARC, crPointArray ) );
		long aROC = roundOff( crSection.ROC() );
		long aRepLinkID = crSection.m_RepLinkID;
		double aWayToRepPoint = crSection.m_WayToRepPoint;

		using namespace schema::curve_line;
		crCDBFHandle.write_Attribute( aOID, crFieldIndexMap[kROC],				aROC );
		crCDBFHandle.write_Attribute( aOID, crFieldIndexMap[kRepLinkID],		aRepLinkID );
		crCDBFHandle.write_Attribute( aOID, crFieldIndexMap[kWayToRepPoint],	aWayToRepPoint );

		// 検証用ログへの出力
		if (m_OutputOptions.m_OutputVerificationLog)
		{
			std::stringstream aLayerName;
			if (m_OutputOptions.m_CreateEachMesh)
			{
				aLayerName << cMeshCode << "_CRV";
			}
			else
			{
				aLayerName << "CRV";
			}

			// 区間
			m_VerificationLogStream 
				<< "0" << fd
				<< aLayerName.str() << fd
				<< aOID << fd
				<< fd
				<< fd
				<< aROC << fd
				<< aRepLinkID << fd
				<< std::setprecision( 20 ) << aWayToRepPoint << fd
				<< crSection.m_DistanceOf3P << fd
				<< crSection.m_FitRegionAngle << fd
				<< std::endl;

			// 代表点
			m_VerificationLogStream 
				<< "0" << fd
				<< "代表点" << fd
				<< aOID << fd
				<< crSection.m_pRepPoint->GetX() << fd
				<< crSection.m_pRepPoint->GetY() << fd
				<< std::endl;
		}
	}

	void CurveInfoCreator::addSection( SectionPtr cpSection )
	{
		ATLASSERT( cpSection );

		// 対象区間の代表点存在リンクが存在するメッシュの区間群に対象区間を追加
		long aMeshCode = m_pCacheManager->existingMeshCode( cpSection->m_RepLinkID );	///< 代表点存在リンクが存在するメッシュコード
		m_SectionsByMesh[ aMeshCode ].push_back( cpSection );
	}

	void CurveInfoCreator::createSections( const DirectionalSRInfo& crDirectionalSRInfo )
	{
		polylinize( crDirectionalSRInfo );
	}

	/**
	 * @brief CurveInfoCreator::polylinize()の一部
	 */
	namespace polylinize
	{
		/**
		 * @brief DSR情報から区間候補群を作成する
		 * @param[in]	crDirectionalSRInfo		DSR情報
		 * @param[in]	crOutputOptions			出力オプション
		 * @param[out]	crSectionCandidates		区間候補群の格納先
		 */
		void createSectionCandidates(
			const DirectionalSRInfo& crDirectionalSRInfo,
			const OutputOptions& crOutputOptions,
			SectionCandidates& crSectionCandidates)
		{
			ATLASSERT( crDirectionalSRInfo.m_pDirectionalSR );
			const ROCPointInfos& rROCPointInfos = *crDirectionalSRInfo.m_pROCPointInfos;

			crSectionCandidates.clear();
			SectionCandidatePtr pSectionCandidate;	///< 作成中の区間候補

			// 点列の各点について
			for (ROCPointInfos::const_iterator itROCPointInfo = rROCPointInfos.begin(); itROCPointInfo != rROCPointInfos.end(); ++itROCPointInfo)
			{
				const ROCPointInfoPtr pROCPointInfo = *itROCPointInfo;
				ATLASSERT( pROCPointInfo );
				const ROCPointInfo& rROCPointInfo = *pROCPointInfo;
				// 着目点における曲率半径を決定
				// 点における曲率半径は、（複数の点の評価結果群を総合的に判断する「区間の曲率半径」と異なり）
				// その点のみにおける評価結果群のうち、最も高評価のものを採用する。
				double aROC = circle_fit_evaluator::bestFitResult( *rROCPointInfo.m_pCFEResults )->m_ROC;

				long aMaxROC = crOutputOptions.maxROCForRegionDeletion( crDirectionalSRInfo.m_pSingleRoad->m_IsHighSpeedArea );	///< 最大曲率半径

				// 曲率半径が最大曲率半径以下の点があったら
				if (aROC <= aMaxROC)
				{
					// 区間候補が未作成なら作成する
					if (! pSectionCandidate)
					{
						pSectionCandidate = SectionCandidatePtr( new SectionCandidate( crDirectionalSRInfo ) );
						pSectionCandidate->m_Range.first = rROCPointInfo.m_Ratio;
					}

					// 区間候補に点情報を追加
					pSectionCandidate->m_ROCPointInfos.push_back( pROCPointInfo );
				}
				// 曲率半径が最大曲率半径を超える点があったら
				else
				{
					// 区間候補が作成済みなら
					if (pSectionCandidate)
					{
						pSectionCandidate->m_Range.second = (*boost::prior( itROCPointInfo ))->m_Ratio;	// １つ前で終了
						crSectionCandidates.push_back( SectionCandidate( *pSectionCandidate ) );		// コピーを追加
						pSectionCandidate = SectionCandidatePtr();	// NULLにする
					}
				}

				// 最後の点なら
				if (itROCPointInfo == boost::prior( rROCPointInfos.end() ))
				{
					// 区間候補が作成済みなら
					if (pSectionCandidate)
					{
						pSectionCandidate->m_Range.second = (*itROCPointInfo)->m_Ratio;	// ここで終了
//						ATLASSERT( pSectionCandidate->m_Range.first != pSectionCandidate->m_Range.second);	// TODO: 必要ならここでチェック
						crSectionCandidates.push_back( SectionCandidate( *pSectionCandidate ) );		// コピーを追加
						pSectionCandidate = SectionCandidatePtr();	// NULLにする
					}
				}
			}
		}
	} // namespace polylinize

	void CurveInfoCreator::polylinize( const DirectionalSRInfo& crDirectionalSRInfo )
	{
		using namespace polylinize;

		ATLASSERT( crDirectionalSRInfo.m_pSingleRoad );
		ATLASSERT( crDirectionalSRInfo.m_pDirectionalSR );
		ATLASSERT( crDirectionalSRInfo.m_pROCPointInfos );

		const SingleRoad& rSingleRoad = *crDirectionalSRInfo.m_pSingleRoad;
		const ROCPointInfos& rROCPointInfos = *crDirectionalSRInfo.m_pROCPointInfos;
		const DirectionalSR& rDirectionalSR = *crDirectionalSRInfo.m_pDirectionalSR;

		// 区間候補群を作成
		SectionCandidates aSectionCandidates;	///< 区間候補群
		createSectionCandidates( crDirectionalSRInfo, m_OutputOptions, aSectionCandidates );

		// 各区間候補について
		BOOST_FOREACH (const SectionCandidate& rSectionCandidate, aSectionCandidates)
		{
			HRESULT hr;
			const SingleRoad& rSingleRoad = *rSectionCandidate.m_rDirectionalSRInfo.m_pSingleRoad;	///< 区間候補を含む一本道

			// 区間作成
			SectionPtr pSection( new Section( rSectionCandidate.m_rDirectionalSRInfo.m_pDirectionalSR->m_IsLeftCurve, rSingleRoad.m_IsHighSpeedArea ) );

			// 代表点に関する情報を設定
			setRepPointInfo( rSingleRoad, rSectionCandidate, pSection );

			// 区間の点列を作成
			{
				IPolylinePtr ipSRPolyline = rSingleRoad.geometry();	///< 一本道全体の形状

				// 区間候補の始終点を求める
				IPointPtr ipBeginPoint = newGeometry< IPointPtr >( CLSID_Point, m_pSpRef );
				IPointPtr ipEndPoint = newGeometry< IPointPtr >( CLSID_Point, m_pSpRef );
				{
					hr = ipSRPolyline->QueryPoint( esriNoExtension, rSectionCandidate.m_Range.first, VARIANT_TRUE, ipBeginPoint );
					ATLASSERT( hr == S_OK );
					hr = ipSRPolyline->QueryPoint( esriNoExtension, rSectionCandidate.m_Range.second, VARIANT_TRUE, ipEndPoint );
					ATLASSERT( hr == S_OK );
				}
				setSpatialReference( ipBeginPoint, m_pSpRef );
				setSpatialReference( ipEndPoint, m_pSpRef );

				Segments aSegments = rSingleRoad.segments( rSectionCandidate.m_Range );		///< 区間候補を包含するセグメント列
				for (SegmentIt itSegment = aSegments.begin(); itSegment != aSegments.end(); ++itSegment)
				{
					// 最初のセグメントなら、区間候補の始点を区間に追加
					if (itSegment == aSegments.begin())
					{
						pSection->m_Points.push_back( ipBeginPoint );
					}

					// 最後のセグメントなら、区間候補の終点を区間に追加
					if (itSegment == boost::prior( aSegments.end() ))
					{
						pSection->m_Points.push_back( ipEndPoint );
					}
					// 最後のセグメント以外なら、セグメント終点を区間に追加
					else
					{
						pSection->m_Points.push_back( (*itSegment)->m_pToPoint );
					}
				}
			}

			// 登録
			if (3 <= pSection->m_Points.size())		// 構成点が3点未満の区間は採用しない	TODO: 他にも登録すべきでない場合がある？
			{
				addSection( pSection );
			}
		}
	}

	void CurveInfoCreator::setRepPointInfo(
		const SingleRoad& crSingleRoad,
		const SectionCandidate& crSectionCandidate,
		SectionPtr cpSection ) const
	{
		using namespace polylinize;
		using namespace circle_fit_evaluator;
		using namespace sindy::schema;
		using namespace utility::sindylib_base;

		// 区間候補の範囲に含まれる各点の評価結果群を、１つのコンテナに集約
		Results aResults;		///< 評価結果コンテナ
		BOOST_FOREACH (const ROCPointInfoPtr cpROCPointInfo, crSectionCandidate.m_ROCPointInfos)
		{
			ResultsPtr pResults = cpROCPointInfo->m_pCFEResults;
			ATLASSERT( pResults );
			std::copy( pResults->begin(), pResults->end(), std::inserter( aResults, aResults.begin() ) );
		}
		// どの評価結果を採用するかを決定する
		// 区間候補の範囲に含まれる点群における評価結果群のうち、最も高評価のものを採用する。
		const ResultPtr pBestFitResult = bestFitResult( aResults, m_OutputOptions.m_LogHandlingProcess ? &m_LogStream : NULL );
		ATLASSERT( pBestFitResult );
		const ROCPointInfoPtr pRepPointInfo = crSectionCandidate.m_ROCPointInfos.searchByPoint( pBestFitResult->m_pTargetPoint );
		ATLASSERT( pRepPointInfo );
		const ROCPointInfo& rRepPointInfo = *pRepPointInfo;	///< 代表点情報

		long aRepLinkID = (*crSingleRoad.segment( rRepPointInfo.m_Ratio ))->m_LinkID;
		const CSPTableContainer pLinkTable = m_pCacheManager->cache( m_pCacheManager->existingMeshCode( aRepLinkID ), m_OutputOptions.m_QueryWhereClause )->FindTable( road_link::kTableName );
		IPolylinePtr ipRepLinkGeom = IFeaturePtr( irow( *pLinkTable->find_by_key( kObjectID, aRepLinkID ) ) )->GetShape();	///< 代表点存在リンクの形状
		double aDistanceAlongCurve = 0;
		double aDistanceFromCurve = 0;		// 使用しない
		IPointPtr ipRepPoint = newGeometry< IPointPtr >( CLSID_Point, m_pSpRef );
		ipRepLinkGeom->QueryPointAndDistance( esriNoExtension, rRepPointInfo.m_pPoint, VARIANT_FALSE, ipRepPoint, &aDistanceAlongCurve, &aDistanceFromCurve, VARIANT_BOOL() );
		setSpatialReference( ipRepPoint, m_pSpRef );

		// 代表リンクの始点と終点により、度→mの変換係数を決定
		double aDegreeToMeterCoef = degreeToMeterCoef(
			ipRepLinkGeom->GetFromPoint()->GetX(),	ipRepLinkGeom->GetFromPoint()->GetY(),
			ipRepLinkGeom->GetToPoint()->GetX(),	ipRepLinkGeom->GetToPoint()->GetY()
		);
		if (m_OutputOptions.m_LogHandlingProcess)
		{
			m_LogStream << utility::log_format::freeComment( std::string("度→mの変換係数=") + utility::toStr( aDegreeToMeterCoef, 20 ) );
		}

		cpSection->m_RepLinkID = aRepLinkID;
		cpSection->m_WayToRepPoint = aDistanceAlongCurve * aDegreeToMeterCoef;
		cpSection->m_pRepPoint = ipRepPoint;
		cpSection->m_ROC = pBestFitResult->m_ROC;
		cpSection->m_DistanceOf3P = pBestFitResult->m_DistanceOf3Points;

		double aFitRegionLength = (pBestFitResult->m_FitCount - 1) * m_OutputOptions.m_FittingPointInterval;	///< フィット領域の長さ
		cpSection->m_FitRegionAngle = circularArcAngle( aFitRegionLength, pBestFitResult->m_ROC );
		ATLASSERT( cpSection->m_FitRegionAngle < 360 );
	}

	/// CurveInfoCreator::createShape()の一部
	namespace create_shape
	{
		/**
		 * @brief Shapeファイルのスキーマを定義し、FieldIndexMapを返す
		 * @param[in,out]	crCDBFHandle	DBFハンドルクラスのインスタンス
		 * @return	FieldIndexMap
		 */
		FieldIndexMap fieldIndexMap( CDBFHandle& crDBFHandle )
		{
			// TODO: フィールドの桁数を検討
			// TODO: フィールドの並び順がBoost.Assignの仕様に依存するのは危険なので、要修正。
			return boost::assign::map_list_of	// map_list_ofは下から解釈される（らしい）ので、下に書いたフィールドほど先
				( tstring( schema::curve_line::kWayToRepPoint ),	crDBFHandle.add_Field( CT2A( schema::curve_line::kWayToRepPoint ), FTDouble, 9, 2 ) )
				( tstring( schema::curve_line::kRepLinkID ),		crDBFHandle.add_Field( CT2A( schema::curve_line::kRepLinkID ), FTInteger, 9, 0 ) )
				( tstring( schema::curve_line::kROC ),				crDBFHandle.add_Field( CT2A( schema::curve_line::kROC ), FTInteger, 9, 0 ) )
			;
		}
	} // namespace create_shape

	bool CurveInfoCreator::isTargetMesh( long cMeshCode ) const
	{
		return m_BaseMeshList.find( cMeshCode ) != m_BaseMeshList.end();
	}

	void CurveInfoCreator::createShape() const
	{
		using namespace create_shape;

		// Shapeファイルのパスを設定
		path aFilePath = m_OutputDirPath / ( std::string("CRV") );
		CSHPHandle aSHPHandle;
		CDBFHandle aDBFHandle;
		aSHPHandle.create( aFilePath.string().c_str(), SHPT_ARC );
		aDBFHandle.create( aFilePath.string().c_str() );
		
		// Shapeファイルのスキーマを設定
		FieldIndexMap aFieldIndexMap = fieldIndexMap( aDBFHandle );

		// 全メッシュの全区間について
		BOOST_FOREACH (const SectionsByMesh::value_type& rElement, m_SectionsByMesh)
		{
			long aMeshCode = rElement.first;
			const Sections& rSections = rElement.second;

			// 出力対象メッシュでなければスキップ
			if (! isTargetMesh( aMeshCode ))
				{ continue; }

			// 区間ごとに出力
			BOOST_FOREACH (const SectionPtr pSection, rSections)
			{
				createShapeSection( pSection, aSHPHandle, aDBFHandle, aFieldIndexMap, aMeshCode );
			}
		}
	}

	void CurveInfoCreator::createShape( long cMeshCode ) const
	{
		using namespace create_shape;

		// 出力対象メッシュでなければ終了
		if (! isTargetMesh( cMeshCode ))
			{ return; }

		// 区間が１つもない場合はShapeファイルを作成しない（Bug9691）
		SectionsByMesh::const_iterator it = m_SectionsByMesh.find( cMeshCode );
		if (it == m_SectionsByMesh.end())
		{
			notifier::asMeshWithNoSection( m_LogStream, cMeshCode );
			notifier::asMeshWithNoSection( std::cerr, cMeshCode );
			return;
		}

		// Shapeファイルのパスを設定
		path aMeshOutputDirPath = m_OutputDirPath;	///< 当該メッシュの出力ディレクトリパス
		std::string aMeshCode1 = boost::lexical_cast<std::string>( cMeshCode / 100 );	///< １次メッシュコード
		std::string aMeshCode2 = boost::lexical_cast<std::string>( cMeshCode );			///< ２次メッシュコード
		if (m_OutputOptions.m_SHP05DirConfig)
		{
			aMeshOutputDirPath /= path( aMeshCode1 ) / aMeshCode2;
			// 出力ディレクトリがなければ作成
			createDirectory( aMeshOutputDirPath );
		}
		path aFilePath = aMeshOutputDirPath / ( aMeshCode2 + "_CRV" );
		CSHPHandle aSHPHandle;
		CDBFHandle aDBFHandle;
		aSHPHandle.create( aFilePath.string().c_str(), SHPT_ARC );
		aDBFHandle.create( aFilePath.string().c_str() );

		// Shapeファイルのスキーマを設定
		FieldIndexMap aFieldIndexMap = fieldIndexMap( aDBFHandle );

		// 対象メッシュの全区間を出力
		const Sections& rSections = it->second;
		BOOST_FOREACH (const SectionPtr pSection, rSections)
		{
			createShapeSection( pSection, aSHPHandle, aDBFHandle, aFieldIndexMap, cMeshCode );
		}
	}

	void CurveInfoCreator::createShapeSection(
		const SectionPtr cpSection,
		CSHPHandle& crSHPHandle,
		CDBFHandle& crDBFHandle, 
		FieldIndexMap& crFieldIndexMap,
		long cMeshCode) const
	{
		// TODO: 左右のループを１つにしたい…。

		ATLASSERT( cpSection->m_Points.size() != 0 );
		ATLASSERT( ! cpSection->m_Points.empty() );

		// 曲率半径が「区間の最大曲率半径」を超える区間は作成しない（Bug9628）
		if (m_OutputOptions.maxROCOfSection( cpSection->m_IsHighSpeedArea ) < cpSection->m_ROC)
			{ return; }

		// 曲率半径に対して短すぎる区間は作成しない（Bug9638）
		if (cpSection->m_FitRegionAngle < m_OutputOptions.m_SectionMinAngle)
			{ return; }

		// 左カーブの場合は、格納されている順に点列を作成
		if (cpSection->isLeftCurve())
		{
			CSHPPointArray aPointArray;
			BOOST_FOREACH (const IPointPtr cpPoint, cpSection->m_Points)
			{
				aPointArray.push_back( CSHPPoint(
					cpPoint->GetX(),
					cpPoint->GetY()
				));
			}

			writeShape( crSHPHandle, crDBFHandle, crFieldIndexMap, aPointArray, *cpSection, cMeshCode );
		}
		// 右カーブの場合は、格納されている順とは逆に点列を作成
		else
		{
			CSHPPointArray aPointArray;
			for (Section::Points::const_reverse_iterator itPoint = cpSection->m_Points.rbegin(); itPoint != cpSection->m_Points.rend(); ++itPoint)
			{
				aPointArray.push_back( CSHPPoint(
					(*itPoint)->GetX(),
					(*itPoint)->GetY()
				));
			}

			writeShape( crSHPHandle, crDBFHandle, crFieldIndexMap, aPointArray, *cpSection, cMeshCode );
		}
	}

	void CurveInfoCreator::convertMesh( long cMeshCode )
	{
		m_MeshCode = cMeshCode;

		std::cerr << "--------------------------------------------------" << std::endl;
		m_LogStream << "#--------------------------------------------------" << std::endl;
		std::cerr << "メッシュコード : " << cMeshCode << std::endl;
		m_LogStream << "# メッシュコード : " << cMeshCode << std::endl;
		m_LogStream << "# メッシュ処理開始時刻 : " << boost::lexical_cast<std::string>( boost::posix_time::second_clock::local_time() ) << std::endl;

		std::string aProgressMsg = "読み込み";
		outputElapsedTime( aProgressMsg, true );

		using namespace sindy::schema;
		using namespace utility::sindylib_base;

		// 対象メッシュのデータを読み込む
		CSPTableFinder pTableFinder = m_pCacheManager->cache( cMeshCode, m_OutputOptions.m_QueryWhereClause );
		CSPTableContainer pLinkTable = pTableFinder->FindTable( road_link::kTableName );
		outputElapsedTime( aProgressMsg, false );
		// リンク数が0件なら通知（Bug9697）
		if (pLinkTable->empty())
		{
			notifier::asLinkNotFound( std::cerr, cMeshCode );
			notifier::asLinkNotFound( m_LogStream, cMeshCode );
		}

		long aLinkCount = 0;

		// 読み込んだリンクのそれぞれについて
		BOOST_FOREACH (CSPContainerBase pLink, *pLinkTable)
		{
			ATLASSERT( pLink );
			IFeaturePtr ipLink = irow( pLink );
			long aLinkID = ipLink->OID;

			// 空間参照を記録（最初に着目した１リンクのみ）
			setSpatialReferenceBy( ipLink );

			// 進捗表示
			std::cerr << "Road_Link " << ++aLinkCount << " / " << pLinkTable->size() << "\r";

			// 処理済みリンクならスキップ
			if (m_ProcessedLinkIDs.find( aLinkID ) != m_ProcessedLinkIDs.end())
				{ continue; }
			m_ProcessedLinkIDs.insert( aLinkID );

			// リンクが対象リンクの条件を満たさなければスキップ
			if (! isTargetLink( ipLink ))
				{ continue; }

			if (m_OutputOptions.m_LogHandlingProcess)
			{
				m_LogStream << CStringA( sindy::schema::road_link::kTableName ) << fd << aLinkID << fd << fd << fd << "このリンクを含む一本道の処理開始" << std::endl;
			}

			// 対象リンクを含む一本道を取得
			SingleRoadPtr pSingleRoad = getSingleRoad( ipLink );		// TODO: できるならconst参照に修正。
			// 一本道を構成するセグメント数が1以下ならスキップ
			if (pSingleRoad->size() <= 1)
				{ continue; }

			// 一本道を変曲点で分割し、DSRの列にする
			DirectionalSRArray aDirectionalSRArray = divideSingleRoad( pSingleRoad );

			// DSRごとに処理
			BOOST_FOREACH (const DirectionalSRPtr pDirectionalSR, aDirectionalSRArray)
			{
				// 「点と曲率半径」の列の情報を作成
				DirectionalSRInfo aDirectionalSRInfo( pSingleRoad, pDirectionalSR, createROCPointInfos( *pDirectionalSR ) );

				// 区間を作成
				createSections( aDirectionalSRInfo );
			}
		}
		std::cerr << std::endl;

		// 必要ならログにキャッシュの状態を出力
		if (m_OutputOptions.m_LogHandlingProcess)
		{
			m_LogStream << "# キャッシュ存在メッシュ :";
			BOOST_FOREACH (long aMeshCode, m_pCacheManager->cachedMeshCodes())
			{
				m_LogStream << " " << aMeshCode;
			}
			m_LogStream << std::endl;
		}

		std::cerr << "メッシュコード : " << cMeshCode << " 終了" << std::endl;
		m_LogStream << "# メッシュコード : " << cMeshCode << " 終了"  << std::endl;
		m_LogStream << "# メッシュ処理終了時刻 : " << boost::lexical_cast<std::string>( boost::posix_time::second_clock::local_time() ) << std::endl;
	}
} // namespace curve_info

