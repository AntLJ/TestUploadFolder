#include "stdafx.h"
#include "CheckIllustData.h"

#include <sindy/schema/illust.h>
#include <TDC/illustlib/illust_util.h>
#include <TDC/sindylib_base/RuleWrapper.h>
#include <TDC/illustlib/IllustLinkTable.h>
#include <TDC/illustlib/IllustLinkRow.h>
#include <TDC/illustlib/IllustNameCreator.h>
#include <TDC/illustlib/Message.h>
#include <TDC/illustlib/IllustErrorInfo.h>
#include <TDC/illustlib/IllustLinkCheck.h>

#include <sindy/workspace.h>
#include <VersionInfo.h>
#include <TDC/useful_headers/str_util.h>
#include <TDC/useful_headers/enumerate_elements.h>
#include <TDC/useful_headers/tformat.h>

#include <ArcHelperEx/GlobalFunctions.h>
#include <ArcHelperEx/AheGlobalsMisc.h>
#include <ArcHelperEx/define.h>
#include <ArcHelperEx/AheGlobals.h>
#include <ArcHelperEx/AheMeshUtil.h>
#include <boost/assign.hpp>

#include <locale>
#include <codecvt>
namespace sindy
{
	class CIllustLinkTable;
	class CIllustLinkRow;
	class CIllustLinkCheck;
	class CTableFinder;
	typedef boost::shared_ptr<CIllustLinkTable> CSPIllustLinkTable;
}

using namespace sindy;
using namespace schema;
using namespace message;

// 実行
bool CCheckIllustData::execute()
{
	// 「システムが日本語環境」フラグを取得する
	m_isJapaneseOs =  ( GetSystemDefaultLCID() == util::JAPANESE_LANGUAGE );

	// 初期化
	if( !init() )
		return false;

	// ASEAN向けフラグを取得する
	m_isAseanSchema = AheIsWorldMeshMode();

	IFeatureClassPtr ipIllustLinkFC;
	// イラストリンクフィーチャクラスを取得
	if( !util::OpenFeatureClass( m_isAseanSchema?global::illust_link::kTableName:illust_link::kTableName, m_ipIllustWorkspace, ipIllustLinkFC ) )
	{
		CString err = uh::str_util::format( getMsg(message::iCannotOpen), m_isAseanSchema?global::illust_link::kTableName:illust_link::kTableName );
		WriteComment(err);
		WriteEndLog(false);
		return false;
	}

	// 道路リンクフィーチャクラスを取得
	if( !util::OpenFeatureClass( m_isAseanSchema?global::road_link::kTableName:road_link::kTableName, m_ipRoadWorkspace, m_ipRoadLinkFC ) )
	{
		CString err = uh::str_util::format( getMsg(message::iCannotOpen), m_isAseanSchema?global::road_link::kTableName:road_link::kTableName );
		WriteComment(err);
		WriteEndLog(false);
		return false;
	}

	// 道路ノードフィーチャクラスを取得
	if( !util::OpenFeatureClass( m_isAseanSchema?global::road_node::kTableName:road_node::kTableName, m_ipRoadWorkspace, m_ipRoadNodeFC ) )
	{
		CString err = uh::str_util::format( getMsg(message::iCannotOpen), m_isAseanSchema?global::road_node::kTableName:road_node::kTableName );
		WriteComment(err);
		WriteEndLog(false);
		return false;
	}

	// HIGHWAY_NODEフィーチャクラスを取得
	if( !m_isAseanSchema )
	{
		if( !util::OpenFeatureClass( highway_node::kTableName, m_ipRoadWorkspace, m_ipHighwayNodeFC ) )
		{
			CString err = uh::str_util::format( getMsg(message::iCannotOpen), highway_node::kTableName );
			WriteComment(err);
			WriteEndLog(false);
			return false;
		}

		// FACIL_INFO_POINTフィーチャクラスを取得
		if( !util::OpenFeatureClass( facil_info_point::kTableName, m_ipRoadWorkspace, m_ipFacilInfoFC ) )
		{
			CString err = uh::str_util::format( getMsg(message::iCannotOpen), facil_info_point::kTableName );
			WriteComment(err);
			WriteEndLog(false);
			return false;
		}

		// ROAD_CODE_LISTテーブルを取得
		if( !util::OpenTable( road_code_list::kTableName, m_ipRoadWorkspace, m_ipRoadCodeT ) )
		{
			CString err = uh::str_util::format( getMsg(message::iCannotOpen), road_code_list::kTableName );
			WriteComment(err);
			WriteEndLog(false);
			return false;
		}
	}
	// LQ_DIRGUIDEテーブルを取得
	if( !util::OpenTable( m_isAseanSchema?global::lq_dirguide::kTableName:lq_dirguide::kTableName, m_ipRoadWorkspace, m_ipLQDirguideT ) )
	{
		CString err = uh::str_util::format( getMsg(message::iCannotOpen), m_isAseanSchema?global::lq_dirguide::kTableName:lq_dirguide::kTableName );
		AllWriteComment(err);
		return false;
	}

	// LQ_TURNREGテーブルを取得
	if( !util::OpenTable( m_isAseanSchema?global::lq_turnreg::kTableName:lq_turnreg::kTableName, m_ipRoadWorkspace, m_ipLQTurnRegT ) )
	{
		CString err = uh::str_util::format( getMsg(message::iCannotOpen), m_isAseanSchema?global::lq_turnreg::kTableName:lq_turnreg::kTableName );
		AllWriteComment(err);
		return false;
	}

	// INF_TURNREGテーブルを取得
	if( !util::OpenTable( m_isAseanSchema?global::inf_turnreg::kTableName:inf_turnreg::kTableName, m_ipRoadWorkspace, m_ipINFTurnRegT ) )
	{
		CString err = uh::str_util::format( getMsg(message::iCannotOpen), m_isAseanSchema?global::inf_turnreg::kTableName:inf_turnreg::kTableName );
		AllWriteComment(err);
		return false;
	}

	if( (!util::OpenTable(subroad_link::kTableName, m_ipRoadWorkspace, m_ipSubRoadLinkT)) ||
		(!util::OpenTable(subroad_part::kTableName, m_ipRoadWorkspace, m_ipSubRoadPartT)) )
	{
		CString err = uh::str_util::format( getMsg(message::iCannotOpenSubroad) );
		AllWriteComment(err);
	}

	// テーブルファインダの初期化
	CModel model;
	m_finder.InitCollection(ITablePtr(ipIllustLinkFC), model);

	CSPIllustLinkTable spIllustTable = CAST_ILLUSTLINKTABLE(m_finder.FindTable(m_isAseanSchema?global::illust_link::kTableName:illust_link::kTableName));
	spIllustTable->_Select(AheInitQueryFilter(), true);

	// 関連する道路ノード、道路リンク、右左折禁止情報をmapへ格納
	if(!SetRelMap(spIllustTable))
	{
		WriteComment(getMsg(message::iCannotSearchRelIllustLink));
		WriteEndLog(false);
		return false;
	}

	// チェック実行
	if( !check(spIllustTable) )
	{
		WriteEndLog(false);
		return false;
	}
	WriteEndLog(true);
	return true;
}

// 初期化
bool CCheckIllustData::init()
{
	if(!OpenFile())		return false;

	std::cout << CStringA(m_args.m_roaddb.c_str()) << CStringA(getMsg(message::iConnectMsg)) << std::endl;
	// 道路DBに接続
	m_ipRoadWorkspace = sindy::create_workspace(m_args.m_roaddb.c_str());
	if(!m_ipRoadWorkspace)
	{
		CString err = uh::str_util::format(getMsg(message::iNotConnectDB), m_args.m_roaddb.c_str());
		WriteComment(err);
		WriteEndLog(false);
		return false;
	}
	std::cout << CStringA(getMsg(message::iSuccessConnect)) << std::endl;

	std::cout << CStringA(m_args.m_illustdb.c_str()) << CStringA(getMsg(message::iConnectMsg)) << std::endl;
	// イラストDBに接続
	m_ipIllustWorkspace = sindy::create_workspace(m_args.m_illustdb.c_str());
	if(!m_ipIllustWorkspace)
	{
		CString err = uh::str_util::format(getMsg(message::iNotConnectDB), m_args.m_illustdb.c_str());
		WriteComment(err);
		WriteEndLog(false);
		return false;
	}
	std::cout << CStringA(getMsg(message::iSuccessConnect)) << std::endl;

	// 実行ログに情報を書き込む
	SYSTEMTIME aTime;
	GetLocalTime(&aTime);
	CVersion cVersion;

	RunLog() << CStringW(cVersion.GetInternalName()).GetString();
	RunLog() << CStringW(getMsg(message::iWriteVersion)).GetString() << CStringW(cVersion.GetFileVersion()).GetString() << std::endl;
	RunLog() << CStringW(getMsg(message::iWriteMode)).GetString() << CStringW(m_args.m_mode.c_str()).GetString() << std::endl;
	RunLog() << CStringW(getMsg(message::iWriteConfigFile)).GetString() << CStringW(m_args.m_config.c_str()).GetString() << std::endl;
	RunLog() << CStringW(getMsg(message::iWriteIllustNameRuleFile)).GetString() << CStringW(m_args.m_namerule.c_str()).GetString() << std::endl;
	RunLog() << CStringW(getMsg(message::iWriteRoadDB)).GetString() << CStringW(m_args.m_roaddb.c_str()).GetString() << std::endl;
	RunLog() << CStringW(getMsg(message::iWriteIllustDB)).GetString() << CStringW(m_args.m_illustdb.c_str()).GetString() << std::endl;
	RunLog() << CStringW(getMsg(message::iWriteStartTime)).GetString() << CStringW(uh::str_util::ToString(&aTime)).GetString() << std::endl;

	// エラーログに情報を書き込む
	ErrLog() << L"# SINDYSTDLOG" << std::endl;

	if(!SetMode())
	{
		RunLog() << CStringW(getMsg(message::iInvalidMode)).GetString() << std::endl;

		WriteEndLog(false);
		return false;
	}
	return true;
}



// 各ファイルを開く
bool CCheckIllustData::OpenFile()
{
	bool ret = true;

	// [config]で設定されたイラストファイル格納先情報を読み取る
	if( !m_DirInfo.read( m_args.m_config.c_str()) )
	{
		std::cerr << CStringA(getMsg(message::iFailedToReadConfig)) << std::endl;
		ret = false;
	}

	// [name_rule]で設定されたイラストファイル名生成ルールを読み取る
	if( !m_RuleInfo.read(m_args.m_namerule.c_str()) )
	{
		std::cerr << CStringA(getMsg(message::iFailedToReadIllustNameRule)) << std::endl;
		ret = false;
	}

	/*
	// 実行ログ、エラーログのファイル名に日付を追加する
	SYSTEMTIME aTime;
	GetLocalTime(&aTime);
	CString runLog = util::AddDateToFileName(aTime, m_args.m_runlog.c_str());
	CString errLog = util::AddDateToFileName(aTime, m_args.m_errlog.c_str());
	*/
	m_ofsRunLog = new std::wofstream(CStringA(m_args.m_runlog.c_str()), std::ios::out);
	m_ofsErrLog = new std::wofstream(CStringA(m_args.m_errlog.c_str()), std::ios::out);

	// 実行ログ開いてなかったらエラー
	if(!RunLog().is_open())
	{
		CString err = uh::str_util::format(getMsg(message::iCannotOpen), m_args.m_runlog.c_str());
		std::cerr << CStringA(err) << std::endl;
		ret = false;
	}
	// エラーログ開いてなかったらエラー
	if(!ErrLog().is_open())
	{
		CString err = uh::str_util::format(getMsg(message::iCannotOpen), m_args.m_errlog.c_str());
		std::cerr << CStringA(err) << std::endl;
		ret = false;
	}

	// 日本語環境ではない場合
	if( !m_isJapaneseOs )
	{
		// BOMなしUTF-8で出力する
		std::locale utf8_locale(std::locale(), new std::codecvt_utf8<wchar_t>);
		m_ofsRunLog->imbue( utf8_locale );
		m_ofsErrLog->imbue( utf8_locale );
	}
	return ret;
}

// モードを設定する
bool CCheckIllustData::SetMode()
{
	int mode = _ttol(m_args.m_mode.c_str());
	switch(mode)
	{
	case 1:
		m_Mode = EMode::kSchemaC;
		break;
	case 2:
		m_Mode = EMode::kNodeAndIllustC; // ノードリスト+イラストチェック
		break;
	case 3:
		m_Mode = EMode::kNodeC;
		break;
	case 4:
		m_Mode = EMode::kIllustC;
		break;
	default:
		break;
	}
	if(m_Mode == EMode::kUnknown)
		return false;

	return true;
}

// イラストリンクに関連するフィーチャをキャッシュ
bool CCheckIllustData::SetRelMap(CSPIllustLinkTable spIllustTable)
{
	std::cout << CStringA(getMsg(message::iCacheDataMsg)) << std::endl;
	// ILLUST_LINKに関わるノードIDを全て抽出
	std::vector<long> vecNodeID;
	for(const auto& row : *spIllustTable)
	{
		CIllustLinkRow& illust = *CAST_ILLUSTLINKROW(row);
		for(int i = 1; i <= illust_link::kMaxNodeNumber; ++i)
		{
			long nodeID = illust.GetValue(illust_link::kNode[i], illust_util::DEFAULT_LONG_VAL);
			if(nodeID == illust_util::DEFAULT_LONG_VAL)
				continue;
			vecNodeID.push_back(nodeID);
		}
	}
	std::sort(vecNodeID.begin(), vecNodeID.end());
	vecNodeID.erase(std::unique(vecNodeID.begin(), vecNodeID.end()), vecNodeID.end());

	std::cout << CStringA(getMsg(message::iFinishedToCacheRoadNode)) << std::endl;
	// 検索用に1000件単位で分割
	std::vector<std::string> vecNodes = uh::enumerate_elements(vecNodeID);

	// FROM_NODE_IDとTO_NODE_IDとTURNREG_LQRFのインデックスを取る
	// ライブラリに移植する場合はループの中で取らないといけないかも
	long fromIndex = m_ipRoadLinkFC->GetFields()->_FindField(road_link::kFromNodeID);
	long toIndex = m_ipRoadLinkFC->GetFields()->_FindField(road_link::kToNodeID);
	long turnRegIndex = m_ipRoadLinkFC->GetFields()->_FindField(road_link::kTurnRegLQRF);
	long size = vecNodes.size();
	long count = 0;

	std::cout << CStringA(getMsg(message::iCachingRoadLink)) << std::endl;

	// 右左折禁止属性を持つ道路リンク群
	std::vector<long> hasTurnRegLinks;

	// 関連するフィーチャを格納
	for(const auto& nodes : vecNodes)
	{
		long aProgress = ++count * 100/size;
		std::cout << aProgress << "%" << "\r";
		// ROAD_NODEを格納
		IFeatureCursorPtr ipNodeCursor;

		// SubFieldsを適宜設定すること
		CString nodeSubFields = uh::str_util::format(_T("%s,%s,%s"), kObjectID, road_node::kNodeClass, feature::kShape);
		if( FAILED(m_ipRoadNodeFC->Search(AheInitQueryFilter(NULL, nodeSubFields, _T("%s IN(%s)"), kObjectID, CString(nodes.c_str())), VARIANT_FALSE, &ipNodeCursor)) )
			return false;

		IFeaturePtr ipNodeFeature;
		while(S_OK == ipNodeCursor->NextFeature(&ipNodeFeature) && ipNodeFeature)
		{
			long oid = ipNodeFeature->GetOID();
			m_mapRoadNode.insert(std::make_pair(oid, ipNodeFeature));
		}

		// ROAD_LINKを格納
		IFeatureCursorPtr ipLinkCursor;

		// SubFieldsを適宜設定すること
		CString linkSubFields = uh::str_util::format(_T("%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s"),
			kObjectID,
			road_link::kFromNodeID,
			road_link::kToNodeID,
			road_link::kRoadClass,
			road_link::kMainLinkClass,
			road_link::kOneway,
			road_link::kNoPassage,
			road_link::kRoadCode,
			road_link::kLaneLQRF,
			road_link::kUpDownClass,
			road_link::kTurnRegLQRF,
			feature::kShape);
		// 周辺リンクも取りたいので、ORで条件指定
		if( FAILED(m_ipRoadLinkFC->Search(
			AheInitQueryFilter(NULL, linkSubFields,
				_T("%s IN(%s) OR %s IN(%s)"),
					road_link::kFromNodeID, CString(nodes.c_str()),
					road_link::kToNodeID, CString(nodes.c_str())),
				VARIANT_FALSE, &ipLinkCursor)) )
			return false;

		IFeaturePtr ipLinkFeature;
		while(S_OK == ipLinkCursor->NextFeature(&ipLinkFeature) && ipLinkFeature)
		{
			long fromID = ipLinkFeature->GetValue(fromIndex);
			long toID = ipLinkFeature->GetValue(toIndex);
			FromToNode fromTo(fromID, toID);
			m_mapRoadLink.insert(std::make_pair(fromTo, ipLinkFeature));

			long turnRegLQRF = ipLinkFeature->GetValue(turnRegIndex);
			if( turnRegLQRF > 0 )
				hasTurnRegLinks.emplace_back(ipLinkFeature->GetOID()); // 右左折禁止情報取得用に集めておく
		}
	}

	std::cout << CStringA(getMsg(message::iFinishedToCacheRoadLink)) << std::endl;

	// イラストリンクに関連する右左折禁止情報全てを取得
	if( !CNodeQueueCheck::CacheTurnReg(hasTurnRegLinks, m_ipINFTurnRegT, m_ipLQTurnRegT, m_mapTurnReg, m_mapTurnRegInf) )
		return false;

	std::cout << CStringA(getMsg(message::iFinishedToCacheTurnReg)) << std::endl;

	if(m_ipSubRoadLinkT && m_ipSubRoadPartT)
	{
		// イラストリンクに関連する開通日開通情報を取得
		CNodeQueueCheck::CacheSubRoad(m_ipSubRoadLinkT, m_ipSubRoadPartT, m_unopenedRoadId);
		std::cout << CStringA(getMsg(message::iFinishedToCacheSubRoad)) << std::endl;
	}

	std::cout << CStringA(getMsg(message::iSuccessfulCache)) << std::endl;

	return true;
}

// メインチェック
bool CCheckIllustData::check(CSPIllustLinkTable spIllustTable)
{
	using namespace illust_link;

	size_t containerSize = spIllustTable->size();
	std::set<CString> checkSameIllustNameSet; // 同一種別、仕向先のイラスト名チェック用
	CHECKSAMENODEMAP checkSameNodeMap; // 同一種別、仕向先のノード列チェック用
	CHECKDIRECTIONCODEMAP checkDirectionMap; // 方向コードの整合性チェック用
	std::set<CString> setExceptField ; // イラスト名生成時に、意図的に考慮しないフィールド一覧
	// スキーマが「国内向け」の場合
	if(!m_isAseanSchema)
	{
		setExceptField = boost::assign::list_of(kDirectionCode)(kDirguideExistCode)(kSingleBranchCode);
	}else
	// スキーマが「ASEAN向け」の場合
	{
		setExceptField = boost::assign::list_of(global::illust_link::kDirectionCode)
			(global::illust_link::kUTurnFlag)
			(global::illust_link::kRoundaboutFlag);
	}

	int aCount = 0;
	// ILLUST_LINK単体のチェック
	for(const auto& col : *spIllustTable)
	{
		std::cout << ++aCount << " / " << containerSize << "\r";
		CSPIllustLinkRow row = CAST_ILLUSTLINKROW(col);
		CNodeQueueCheck cNodeCheck(row, m_mapRoadLink, m_mapRoadNode, m_mapTurnReg, m_mapTurnRegInf, m_ipRoadLinkFC, m_ipRoadNodeFC, m_unopenedRoadId);
		// スキーマチェック
		if(m_Mode & EMode::kSchemaC)
		{
			ERRLIST errList;
			// スキーマチェック
			errList = CIllustLinkRelCheck::SchemaCheckAll(row, getMsg(message::eIllustLinkHasInvalidValue));
			if(!errList.empty())
			{
				for(const auto& err : errList)
					WriteError(row, err);
				// スキーマチェックに通ってないのに、値チェックしても意味ないので終了
				continue;
			}

			// スキーマが「国内向け」の場合,値チェック
			if(!m_isAseanSchema)
			{
				// 値チェック
				errList = CIllustLinkRelCheck::RelCheckAll(row, getMsg(message::eIllustLinkHasInvalidValue));
			}

			// オブジェクトチェック
			cNodeCheck.CheckIllustShape(errList);
			for(const auto& err : errList)
				WriteError(row, err);
		}

		// ノードリストチェック
		if(m_Mode & EMode::kNodeC)
		{
			CNodeQueueCheck::NODEQUEUE vecNodeQueue;
			// ノードリストチェック
			ERRLIST errList = cNodeCheck.CheckNodeQueue(vecNodeQueue);

			// ノード列、イラスト名重複チェック
			CheckDuplicate(row, vecNodeQueue, checkSameNodeMap, checkSameIllustNameSet, errList);

			for(const auto& err : errList)
				WriteError(row, err);

			// スキーマが「国内向け」の場合,HWY系の整合性チェック
			if(!m_isAseanSchema)
			{
				// HWY系の整合性チェック
				CheckRelHighwayInfo(row, vecNodeQueue);
			}

			long lIllustC = row->GetValue(m_isAseanSchema?global::illust_link::kIllustClass:kIllustClass, -1L);
			long lCustomer = row->GetValue(m_isAseanSchema?global::illust_link::kCustomerCode:kCustomerCode, -1L);
			long lNode1 = row->GetValue(m_isAseanSchema?global::illust_link::kNode1:kNode1, -1L);
			// スキーマが「ASEAN向け」の場合
			if(m_isAseanSchema)
			{
				// イラスト名 + イラスト種別 + 仕向先 + ノード1をキーにグループ化
				CString checkDirectionKey = GetIllustName(row, setExceptField);
				checkDirectionKey.AppendFormat(_T(",%ld,%ld,%ld"), lIllustC, lCustomer, lNode1); 
				checkDirectionMap.insert(std::make_pair(checkDirectionKey, row)); // 方向コードの整合性チェック用に格納
			}else
			// スキーマが「国内向け」の場合
			{
				switch(lIllustC)
				{
				case illust_class::kAirport:
				case illust_class::kBranch:
				case illust_class::kBranch2:
				case illust_class::kEntrance:
				case illust_class::kExit:
				case illust_class::kProperJCT:
				case illust_class::kRealCross:
					{
						// イラスト名 + イラスト種別 + 仕向先 + ノード1をキーにグループ化
						CString checkDirectionKey = GetIllustName(row, setExceptField);
						checkDirectionKey.AppendFormat(_T(",%ld,%ld,%ld"), lIllustC, lCustomer, lNode1); 
						checkDirectionMap.insert(std::make_pair(checkDirectionKey, row)); // 方向コードの整合性チェック用に格納
					}
				default:
					break;
				}
			}
		}

		// イラストチェック
		if(m_Mode & EMode::kIllustC)
		{
			ERRLIST errList = cNodeCheck.CheckIllustFile(m_DirInfo, m_RuleInfo);
			for(const auto& err : errList)
				WriteError(row, err);
		}
		
	}

	std::cout << CStringA(getMsg(message::iFinishedToCheckSingleIllustLink)) << std::endl;
	// ILLUST_LINK全体のチェック
	if(m_Mode & EMode::kNodeC)
	{
		CheckComprehensionNode(checkSameNodeMap);
		if(!CheckDirectionCode(checkDirectionMap))
			return false;
	}

	std::cout << CStringA(getMsg(message::iFinishedToCheckCompositeIllustLink)) << std::endl;
	return true;
}

// Lookup the link based on the nodes.
IFeaturePtr CCheckIllustData::getRoadLinkFeature(long firstNodeId, long secondNodeId)
{
	auto link = m_mapRoadLink.find( FromToNode( firstNodeId, secondNodeId ) );		
	if( link != m_mapRoadLink.end() )	return link->second;

	link = m_mapRoadLink.find( FromToNode( secondNodeId, firstNodeId ) );
	if( link != m_mapRoadLink.end() )	return link->second;

	return nullptr; 
}

// Get the link's MainLinkClass flag.
void CCheckIllustData::getRoadLinkClassFlag(const IFeaturePtr& ipLink, bool& isExistRoundAboutLink, bool& isExistIntersectionLink, bool& isExistNormalLink )
{
	long oid = ipLink->GetOID();
	long mainLinkClass = AheGetAttribute( ipLink, road_link::kMainLinkClass).lVal;
	if( mainLinkClass == road_link::main_link_class::kRoundAbout )
	{
		isExistRoundAboutLink = true;
		return;
	}

	if( mainLinkClass == road_link::main_link_class::kCross )
	{
		isExistIntersectionLink = true;
		return;
	}

	isExistNormalLink = true;
}

// ラウンドアバウトのチェック
void CCheckIllustData::checkRoundAbout( const CNodeQueueCheck::NODEQUEUE& vecNodeQueue, ERRLIST& errList)
{
	for(unsigned long nodeIdx1 = 0; nodeIdx1 < vecNodeQueue.size()-2; ++nodeIdx1)
	{
		unsigned long firstNode = vecNodeQueue[nodeIdx1];
		unsigned long secondNode = vecNodeQueue[nodeIdx1+1];

		// ノードによってリンクを取得
		IFeaturePtr ipCurrentLink = getRoadLinkFeature( firstNode, secondNode );
		IFeaturePtr ipLink = getRoadLinkFeature( vecNodeQueue[nodeIdx1+1], vecNodeQueue[nodeIdx1+2] );

		// リンクを取得しない場合
		if( !ipCurrentLink || !ipLink )
		{
			CString err = uh::str_util::format(getMsg(message::eNotExistRoadLink), uh::str_util::ToString(firstNode), uh::str_util::ToString(secondNode));
			IllustErrorInfo errInfo(1001, EErrorLevel::kError, err);
			errList.push_back(errInfo);	
			return;
		}

		// ラウンドアバウトのイラストリンクを作成する。イラストリンクを作成する場合に、同一リンクを二回選択,連続指定はＮＧ.
		if( firstNode == vecNodeQueue[nodeIdx1+2] )
		{
			CString err = uh::str_util::format(getMsg(message::eRounAboutLinkIsContinuous));
			IllustErrorInfo errInfo(3004, EErrorLevel::kError, err);
			errList.push_back(errInfo);
			return;
		}

		// 該当リンクのオブジェクトID
		long currentLinkId = ipCurrentLink->GetOID();

		// 経由リンク中の1番目リンクの種別の取得。
		bool isExistRoundAboutLink(false), isExistIntersectionLink(false), isExistNormalLink(false);
		getRoadLinkClassFlag( ipLink, isExistRoundAboutLink, isExistIntersectionLink, isExistNormalLink );

		// 残っているリンクと比較する
		for(unsigned long nodeIdx2 = nodeIdx1+2; nodeIdx2 < vecNodeQueue.size()-1; ++nodeIdx2)
		{
			// ノードによってリンクを取得
			IFeaturePtr ipNextLink = getRoadLinkFeature( vecNodeQueue[nodeIdx2], vecNodeQueue[nodeIdx2+1] );

			// リンクを取得しない場合
			if( !ipNextLink )
			{
				CString err = uh::str_util::format(getMsg(message::eNotExistRoadLink), uh::str_util::ToString(vecNodeQueue[nodeIdx2]), uh::str_util::ToString(vecNodeQueue[nodeIdx2+1]));
				IllustErrorInfo errInfo( 1001, EErrorLevel::kError, err );
				errList.push_back(errInfo);	
				return;
			}

			long nextLinkId = ipNextLink->GetOID();

			if( currentLinkId != nextLinkId )
			{
				// 経由リンク中の2番目リンクからすべての経由リンクの種別の取得。
				getRoadLinkClassFlag( ipNextLink, isExistRoundAboutLink, isExistIntersectionLink, isExistNormalLink );
				continue;
			}

			// 最初のリンクと最後のリンク以外で、同一リンクが二回選択されていた場合
			if( nodeIdx1 != 0 )
			{
				CString err = uh::str_util::format(getMsg(message::eNotRoundaboutLink));
				IllustErrorInfo errInfo(3006, EErrorLevel::kError, err);
				errList.push_back(errInfo);
				return ;			
			}

			// イラストリンクを作成する場合に、同一リンクを二回選択,同じ向きはＮＧ.
			if( firstNode == vecNodeQueue[nodeIdx2] && secondNode == vecNodeQueue[nodeIdx2+1] )
			{
				CString err = uh::str_util::format( getMsg( message::eRounAboutLinkIsSameDirection ) );
				IllustErrorInfo errInfo( 3003, EErrorLevel::kError, err );
				errList.push_back(errInfo);
				return;
			}

			// ラウンドアバウトのイラストリンクを作成する。イラストリンクを作成する場合に、同一リンクを二回選択,同一リンクに戻った後は追加リンク選択不可
			if( firstNode == vecNodeQueue[nodeIdx2+1] && secondNode == vecNodeQueue[nodeIdx2] && nodeIdx2+1 != vecNodeQueue.size()-1 )
			{
				CString err = uh::str_util::format(getMsg(message::eRounAboutLinkIsAdditional));
				IllustErrorInfo errInfo(3005, EErrorLevel::kError, err);
				errList.push_back(errInfo);
				return;
			}

			// ラウンドアバウトのイラストリンクを作成する。イラストリンクを作成する場合に、同一リンクを二回選択, 他リンク経由はＮＧ
			// 流入リンク→交差点リンク→流出リンク（＝流入リンク)選択不可
			if(isExistIntersectionLink && !isExistRoundAboutLink && !isExistNormalLink)
			{
				CString err = uh::str_util::format(getMsg(message::eNotRoundaboutLink));
				IllustErrorInfo errInfo(3006, EErrorLevel::kError, err);
				errList.push_back(errInfo);	
				return ;			
			}

			// 流入リンク→普通リンク→流出リンク（＝流入リンク)選択不可
			if(!isExistIntersectionLink && !isExistRoundAboutLink && isExistNormalLink)
			{
				CString err = uh::str_util::format(getMsg(message::eNotRoundaboutLink));
				IllustErrorInfo errInfo(3006, EErrorLevel::kError, err);
				errList.push_back(errInfo);	
				return ;			
			}

			// 流入リンク→普通リンクと交差点リンク混在→流出リンク（＝流入リンク)選択不可
			if(isExistIntersectionLink && !isExistRoundAboutLink && isExistNormalLink)
			{
				CString err = uh::str_util::format(getMsg(message::eNotRoundaboutLink));
				IllustErrorInfo errInfo(3006, EErrorLevel::kError, err);
				errList.push_back(errInfo);	
				return ;			
			}

			// 流入リンク→ラウンドアバウトと普通リンク混在→流出リンク（＝流入リンク)選択不可
			if(!isExistIntersectionLink && isExistRoundAboutLink && isExistNormalLink)
			{
				CString err = uh::str_util::format(getMsg(message::eNotRoundaboutLink));
				IllustErrorInfo errInfo(3006, EErrorLevel::kError, err);
				errList.push_back(errInfo);	
				return ;			
			}

			// 流入リンク→ラウンドアバウトと普通リンクと交差点リンク混在→流出リンク（＝流入リンク)選択不可
			if(isExistIntersectionLink && isExistRoundAboutLink && isExistNormalLink)
			{
				CString err = uh::str_util::format(getMsg(message::eNotRoundaboutLink));
				IllustErrorInfo errInfo(3006, EErrorLevel::kError, err);
				errList.push_back(errInfo);	
				return ;			
			}
		}
	}
}

// 同一種別、仕向先で重複しているレコードがないかチェック
void CCheckIllustData::CheckDuplicate( sindy::CSPIllustLinkRow& spRow, const CNodeQueueCheck::NODEQUEUE& vecNodeQueue, CHECKSAMENODEMAP& checkSameNodeMap, std::set<CString>& checkSameIllustNameSet, ERRLIST& errList )
{
	long lIllustC = spRow->GetValue(m_isAseanSchema?global::illust_link::kIllustClass:illust_link::kIllustClass,-1L);
	long lCustomer = spRow->GetValue(m_isAseanSchema?global::illust_link::kCustomerCode:illust_link::kCustomerCode,-1L);
	// ASEAN向けのスキーマにはフィールド「SINGLE_BRANCH_C」がないので、-1と設定する
	long lSingleBranch = m_isAseanSchema?-1L:spRow->GetValue(illust_link::kSingleBranchCode, -1L);
	// イラスト種別、仕向先、SINGLE_BRANCH_C
	CString strCheck = uh::str_util::format(_T("%ld,%ld,%ld-"), lIllustC, lCustomer, lSingleBranch);

	// イラスト種別、仕向先、ノード列の文字列
	for(const auto& node : vecNodeQueue)
	{
		strCheck.AppendFormat(_T("%ld,"), node);
	}

	// 最後の「，」も入れておくことで、ノード列が「1,11,2,」と「1,11,21,」の場合に誤ってエラー出力されなくなる（エラーコード:3002）
	auto nodeRet = checkSameNodeMap.insert(std::make_pair(strCheck, spRow));

	// 同じ文字列があればノード列が重複している
	if(!nodeRet.second)
	{
		CString err = uh::str_util::format(getMsg(message::eSameLinkQueue));
		IllustErrorInfo errInfo(3001, EErrorLevel::kFatal, err);
		errList.push_back(errInfo);
	}

	// スキーマが「国内向け」の場合
	if(!m_isAseanSchema)
	{
		// ここ以降はイラスト名重複チェック
		// チェックしない種別、仕向先は終了
		switch(lIllustC)
		{
		case illust_link::illust_class::kEntrance:
		case illust_link::illust_class::kExit:
		case illust_link::illust_class::kGrade:
			return;
		case illust_link::illust_class::kBranch:
		case illust_link::illust_class::kBranch2:
			// TF以外は終了
			if(lCustomer != illust_link::customer::kTF)
				return;
			break;
		default:
			break;
		}
	}
	CString checkNameStr;
	// スキーマが「国内向け」の場合
	if(!m_isAseanSchema)
	{
		// SAPAはイラスト種別、イラスト名の文字列でまとめる
		if(lIllustC == illust_link::illust_class::kSAPA)
			checkNameStr.Format(_T("%ld,%s"), lIllustC, GetIllustName(spRow));
		// SAPA以外はイラスト種別、仕向先、イラスト名の文字列でまとめる
		else
			checkNameStr.Format(_T("%ld,%ld,%s"), lIllustC, lCustomer, GetIllustName(spRow));
	}else
	// スキーマが「ASEAN向け」の場合
	{
		long lDireCode = spRow->GetValue(global::illust_link::kDirectionCode, -1L);
		checkNameStr.Format(_T("%ld,%ld,%s,%ld"), lIllustC, lCustomer, GetIllustName(spRow), lDireCode);
	}
	auto nameRet = checkSameIllustNameSet.insert(checkNameStr);

	// 同じ文字列があればイラスト名が重複している
	if(!nameRet.second)
	{
		CString err = uh::str_util::format(getMsg(message::eSameFileName), GetIllustName(spRow));
		IllustErrorInfo errInfo(6001, EErrorLevel::kError, err);
		errList.push_back(errInfo);
	}
	
	// ラウンドアバウトのイラストリンクのチェック
	checkRoundAbout(vecNodeQueue, errList);

}

// イラスト名を取得する
CString CCheckIllustData::GetIllustName(sindy::CSPIllustLinkRow& spRow, const std::set<CString>& exceptFields)
{
	long lIllustC = spRow->GetValue(m_isAseanSchema?global::illust_link::kIllustClass:illust_link::kIllustClass, -1L);
	long lCustomer = spRow->GetValue(m_isAseanSchema?global::illust_link::kCustomerCode:illust_link::kCustomerCode, -1L);
	const auto& itIllustRule = m_RuleInfo.find(lIllustC, lCustomer);
	if(itIllustRule == m_RuleInfo.end())
		return CString();
	return spRow->GetIllustName(itIllustRule->mIllustNameRule, exceptFields);
}

// 起点から内包されているノード列チェック
void CCheckIllustData::CheckComprehensionNode( const CHECKSAMENODEMAP& checkSameNodeMap )
{
	for(CHECKSAMENODEMAP::const_iterator it = checkSameNodeMap.begin(); it != checkSameNodeMap.end(); ++it)
	{
		CHECKSAMENODEMAP::const_iterator nextIt = std::next(it); // 次の要素

		// 最後まで回らないけどしょうがない
		if(nextIt == checkSameNodeMap.end())
			break;
		// 起点から内包されている場合はFindで0が返る
		if( 0 == nextIt->first.Find(it->first) )
		{
			CString err = uh::str_util::format(getMsg(message::eFirstNodeIsSameAndIncludedLink));
			IllustErrorInfo errInfo(3002, EErrorLevel::kFatal, err);
			WriteError(it->second, errInfo);
		}
	}
}

// ノード列と方向コードの整合性チェック
bool CCheckIllustData::CheckDirectionCode( const CHECKDIRECTIONCODEMAP& checkDirectionMap )
{
	std::set<CString> directionKeySet;
	// キー値を格納
	for(const auto& illust : checkDirectionMap)
	{
		directionKeySet.insert(illust.first);
	}

	// 方面案内無しとして扱う道路リンクを取得する
	std::set<long> setNoDirguideLink;
	if(!GetRoadLinkTreatedAsNoDirguide(setNoDirguideLink))
	{
		return false;
	}

	// キー単位で処理を行う
	for(const auto& key :directionKeySet)
	{
		auto& pair = checkDirectionMap.equal_range(key);
		CheckDirectionCode(pair);
		CheckRelDirguideExist(pair, setNoDirguideLink);
	}
	return true;
}

void CCheckIllustData::CheckDirectionCode( const std::pair<CHECKDIRECTIONCODEITER, CHECKDIRECTIONCODEITER>& sameValsPair )
{
	// イラストリンクのノードの角度群を格納する（ノードの角度 = 構成点→対象ノード→構成点がなす角度）
	// 角度の並び順によって、方向コードの整合性をチェックする
	std::map<std::vector<double>, CSPIllustLinkRow> linkAngleMap;
	for( auto iter = sameValsPair.first; iter != sameValsPair.second; ++iter )
	{
		std::vector<double> vecAngle;
		// イラストリンクに関わるノードの角度を全て求めて格納
		int nMaxNode =  m_isAseanSchema?global::illust_link::kMaxNodeNumber:illust_link::kMaxNodeNumber;
		for(int i = 1; i < ( nMaxNode - 1); ++i)
		{
			long node1 = iter->second->GetValue(m_isAseanSchema?global::illust_link::kNode[i]:illust_link::kNode[i], -1L);
			long node2 = iter->second->GetValue(m_isAseanSchema?global::illust_link::kNode[i+1]:illust_link::kNode[i+1], -1L);
			long node3 = iter->second->GetValue(m_isAseanSchema?global::illust_link::kNode[i+2]:illust_link::kNode[i+2], -1L);
			double angle = -1.0;
			// 3点の角度が取れなくなるまでループ
			if(!GetAngleOf3Nodes(node1, node2, node3, iter->second, angle))
				break;
			vecAngle.push_back(angle);
		}
		linkAngleMap.insert(std::make_pair(vecAngle, iter->second));
	}

	long directionC = m_isAseanSchema?global::illust_link::direction_code::kA:illust_link::direction_code::kA; // 方向コードを順に見ていく
	for(auto& check : linkAngleMap)
	{
		// 方向コードの整合性があっていなかったらエラー
		if(check.second->GetValue(m_isAseanSchema?global::illust_link::kDirectionCode:illust_link::kDirectionCode, -1L) != directionC)
		{
			CString err = uh::str_util::format(getMsg(message::eNotMatchedNodeQueueAndIllustName));
			IllustErrorInfo errInfo(5012, EErrorLevel::kWarning, err);
			WriteError(check.second, errInfo);
		}
		
		// イラスト単分岐コードが「単分岐デザイン」の場合は、方向コードが重複して、格納されているので、方向コードはそのまま
		// 「単分岐デザイン」以外の場合は方向コードを次のコード値にする
		if(m_isAseanSchema?true:(check.second->GetValue(illust_link::kSingleBranchCode, -1L) != illust_link::single_branch_code::kSingleBranchDesign))
		{
			++directionC;
		}
	}
}

void CCheckIllustData::CheckRelDirguideExist( const std::pair<CHECKDIRECTIONCODEITER, CHECKDIRECTIONCODEITER>& sameValsPair, const std::set<long>& setNoDirguideLink )
{
	// イラスト種別はループの中ですべて同じなので、最初の要素の値だけ取得
	long lIllustC = sameValsPair.first->second->GetValue(m_isAseanSchema?global::illust_link::kIllustClass:illust_link::kIllustClass, -1L);

	// ノード単位で処理を行う
	int nMaxNode =  m_isAseanSchema?global::illust_link::kMaxNodeNumber:illust_link::kMaxNodeNumber;
	for(int i = 1; i < ( nMaxNode - 1); ++i)
	{
		std::set<std::tuple<long, long, long>> checkNodeSet; // 分岐点探索用
		std::set<std::pair<long, long>> checkBeforeBranchLink; // 分岐前リンクチェック用
		std::multimap<std::tuple<long, long, long>, CSPIllustLinkRow> checkIllustMap; // 方面案内チェック用
		for(auto iter = sameValsPair.first; iter!= sameValsPair.second; ++iter)
		{
			long node1 = iter->second->GetValue(m_isAseanSchema?global::illust_link::kNode[i]:illust_link::kNode[i], -1L);
			long node2 = iter->second->GetValue(m_isAseanSchema?global::illust_link::kNode[i+1]:illust_link::kNode[i+1], -1L);
			long node3 = iter->second->GetValue(m_isAseanSchema?global::illust_link::kNode[i+2]:illust_link::kNode[i+2], -1L);
			std::tuple<long, long, long> tupleNode = std::make_tuple(node1, node2, node3);
			checkNodeSet.insert(tupleNode);
			checkBeforeBranchLink.insert(std::make_pair(node1, node2));
			checkIllustMap.insert(std::make_pair(tupleNode, iter->second));
		}
		// 分岐前であればcontinue
		if(checkNodeSet.size() == 1)
			continue;

		// 分岐前のリンクが一致しない
		// ROAD_NODE_01でグループ化するようにしたため、ほぼ来ないはず。
		if(checkBeforeBranchLink.size() != 1)
		{
			CString err = uh::str_util::format(getMsg(message::eNotMatchInflowNode));
			IllustErrorInfo errInfo(4003, EErrorLevel::kError, err);
			for(auto iter = sameValsPair.first; iter!= sameValsPair.second; ++iter)
			{
				WriteError(iter->second, errInfo);
			}
			return;
		}

		// スキーマが「ASEAN向け」の場合、エラー項目「5013」の廃止
		if( m_isAseanSchema )	break;

		// ここ以降は方面案内整合性チェック
		// 対象種別以外は終了
		switch(lIllustC)
		{
		case illust_link::illust_class::kEntrance:
		case illust_link::illust_class::kExit:
			break;
		default:
			return;
		}

		// 分岐前リンクのUPDOWNCLASS_Cが不正な場合はエラー
		auto beforeLink = FindLink(checkBeforeBranchLink.begin()->first, checkBeforeBranchLink.begin()->second);
		if(beforeLink == EndLink())
		{
			CString err = uh::str_util::format(getMsg(message::iCannotCheckDueToFatalError), 5013);
			AllWriteComment(err);
			continue;
		}

		long updownClassC = AheGetAttribute(beforeLink->second, road_link::kUpDownClass).lVal;
		switch(updownClassC)
		{
		case road_link::up_down_class::kNoHighway:
		case road_link::up_down_class::kEtcAccess:
			{
				CString err = uh::str_util::format(getMsg(message::eNotExistDirguideOfConfiguredNodeQueue));
				IllustErrorInfo errInfo(5013, EErrorLevel::kError, err);
				for(auto iter = sameValsPair.first; iter!= sameValsPair.second; ++iter)
				{
					WriteError(iter->second, errInfo);
				}
			}
			return;
		default:
			break;
		}

		std::map<double, long> checkLinkMap; // キー：角度、値：流出リンクのOID
		for(auto& illust : checkIllustMap)
		{
			double angle = -1.0;
			// リンクが存在しないケース
			long lFirst = std::get<0>(illust.first), lSecond = std::get<1>(illust.first), lThird = std::get<2>(illust.first);
			if(!GetAngleOf3Nodes( lFirst, lSecond, lThird, illust.second, angle ))
			{
				CString err = uh::str_util::format(getMsg(message::iCannotCheckDueToFatalError), 5013);
				AllWriteComment(err);
				return;
			}
			auto& itLink = FindLink(lSecond, lThird);

			checkLinkMap.insert( std::make_pair(angle, itLink->second->GetOID()) );
		}

		// 2分岐以外はチェックできないので終了（コード値の関係上）
		if(checkLinkMap.size() != 2)
		{
			CString err = uh::str_util::format(getMsg(message::iCannotDirguideCheck));
			AllWriteComment(err);
			return;
		}

		// 方面案内看板有無コードの正しいであろう値を取得
		long lLeftLinkID = -1, lRrightLinkID = -1;
		auto& leftlink = checkLinkMap.begin();
		lLeftLinkID = leftlink->second;
		auto& rightlink = std::next(leftlink);
		lRrightLinkID = rightlink->second;

		long correctDirGuideExist = GetDirGuideExist(beforeLink->second->GetOID(), lLeftLinkID, lRrightLinkID, setNoDirguideLink);
		
		for(auto iter = sameValsPair.first; iter!= sameValsPair.second; ++iter)
		{
			long lDirExistC = iter->second->GetValue(illust_link::kDirguideExistCode, -1L);
			if( correctDirGuideExist != lDirExistC )
			{
				CString err = uh::str_util::format(getMsg(message::eNotExistDirguideOfConfiguredNodeQueue));
				IllustErrorInfo errInfo(5013, EErrorLevel::kError, err);
				WriteError(iter->second, errInfo);
			}

		}
		break; // 分岐地点見つかったら終了
	}
}

bool CCheckIllustData::GetRoadLinkTreatedAsNoDirguide(std::set<long>& setNoDirguideLink)
{
	using namespace inf_dirguide;
	using namespace guide_class;
	ITablePtr ipInfDirguideT;
	// INF_DIRGUIDEテーブルを取得
	if( !util::OpenTable( m_isAseanSchema?global::inf_dirguide::kTableName:inf_dirguide::kTableName, m_ipRoadWorkspace, ipInfDirguideT ) )
	{
		CString err = uh::str_util::format( getMsg(message::iCannotOpen), m_isAseanSchema?global::inf_dirguide::kTableName:inf_dirguide::kTableName );
		AllWriteComment(err);
		return false;
	}

	std::vector<long> vecInfDirIDs;
	// 地先種別が「有料（国道番号）」「有料（県道番号）」のみの場合は方面案内無しとして扱う
	// IN変換時に間引かれるため
	CString strWhere = uh::tstring((uh::tformat(_T("(%1% IN (%4%,%5%) AND (%2% IN (%4%,%5%) OR %2% IS NULL) AND %3% IS NULL)")) % kGuideClass[1] % kGuideClass[2] % kGuideClass[3] % kTollNationalWayNo % kTollPrefWayNo).str()).c_str();
	
	_ICursorPtr ipInfDirCur;
	if( FAILED(ipInfDirguideT->Search(AheInitQueryFilter(NULL, m_isAseanSchema?global::inf_dirguide::kObjectID:inf_dirguide::kObjectID, strWhere), VARIANT_FALSE, &ipInfDirCur) ) )
	{
		CString err = uh::str_util::format( getMsg(message::iFailedToSearchTable), m_isAseanSchema?global::inf_dirguide::kTableName:inf_dirguide::kTableName );
		AllWriteComment(err);
		return false;
	}

	_IRowPtr ipInfRow;
	while( S_OK == ipInfDirCur->NextRow(&ipInfRow) && ipInfRow )
	{
		vecInfDirIDs.push_back(ipInfRow->GetOID());
	}

	std::vector<std::string> vecInfDir = uh::enumerate_elements(vecInfDirIDs); // IN句用に1000件単位に分割する

	for(const auto& infdir : vecInfDir)
	{
		// LQ_DIRGUIDEを検索
		_ICursorPtr ipLQDirCursor;

		CString strLQSubFields = uh::str_util::format(_T("%s,%s"), m_isAseanSchema?global::lq_dirguide::kObjectID:lq_dirguide::kObjectID, lq_dirguide::kLinkID);
		if( FAILED(m_ipLQDirguideT->Search(AheInitQueryFilter(NULL, strLQSubFields, _T("%s IN (%s)"), lq_dirguide::kInfID, CString(infdir.c_str())), VARIANT_FALSE, &ipLQDirCursor)) )
		{
			CString err = uh::str_util::format( getMsg(message::iFailedToSearchTable), m_isAseanSchema?global::lq_dirguide::kTableName:lq_dirguide::kTableName );
			AllWriteComment(err);
			return false;
		}
		_IRowPtr ipLQRow;
		while(S_OK == ipLQDirCursor->NextRow(&ipLQRow) && ipLQRow)
		{
			long linkID = AheGetAttribute(ipLQRow, m_isAseanSchema?global::lq_dirguide::kLinkID:lq_dirguide::kLinkID).lVal;
			setNoDirguideLink.insert(linkID);
		}
	}
	return true;
}

long CCheckIllustData::GetDirGuideExist( long lBeforeLinkID, long lLeftLinkID, long lRightLinkID, const std::set<long>& setNoDirguideLink )
{
	using namespace lq_dirguide;
	using namespace illust_link::dir_guide_exist_code;

	_ICursorPtr ipBeforeCur, ipLeftCur, ipRightCur;
	// 分岐前リンクから始まり、分岐前リンク→分岐後リンクとなるINF_DIRGUIDEをもっているか判断する
	// 分岐前リンクから始まるINF_DIRGUIDEのIDを検索
	if( FAILED(m_ipLQDirguideT->Search(AheInitQueryFilter(NULL, CString(m_isAseanSchema?global::lq_dirguide::kInfID:kInfID), _T("%s = %ld AND %s = 1"),
		kLinkID, lBeforeLinkID, m_isAseanSchema?global::lq_dirguide::kSequence:kSequence), VARIANT_FALSE, &ipBeforeCur)) )
	{
		CString err = uh::str_util::format( getMsg(message::iFailedToSearch) );
		AllWriteComment(err);
		return -1;
	}
	// 左分岐後リンクを第二リンクに持つINF_DIRGUIDEのIDを検索
	if( FAILED(m_ipLQDirguideT->Search(AheInitQueryFilter(NULL, CString(m_isAseanSchema?global::lq_dirguide::kInfID:kInfID), _T("%s = %ld AND %s = 2"),
		kLinkID, lLeftLinkID, kSequence), VARIANT_FALSE, &ipLeftCur)) )
	{
		CString err = uh::str_util::format( getMsg(message::iFailedToSearch) );
		AllWriteComment(err);
		return -1;
	}
	// 右分岐後リンクを第二リンクに持つINF_DIRGUIDEのIDを検索
	if( FAILED(m_ipLQDirguideT->Search(AheInitQueryFilter(NULL, CString(m_isAseanSchema?global::lq_dirguide::kInfID:kInfID), _T("%s = %ld AND %s = 2"),
		kLinkID, lRightLinkID, kSequence), VARIANT_FALSE, &ipRightCur)) )
	{
		CString err = uh::str_util::format( getMsg(message::iFailedToSearch) );
		AllWriteComment(err);
		return -1;
	}

	std::set<long> setInfIDOfBefore; // 分岐前リンクに関わるINF_DIRGUIDEのID群
	_IRowPtr ipRow;
	while(S_OK == ipBeforeCur->NextRow(&ipRow) && ipRow)
	{
		setInfIDOfBefore.insert(AheGetAttribute(ipRow, m_isAseanSchema?global::lq_dirguide::kInfID:kInfID).lVal);
	}

	bool bLeft = false, bRight = false;
	// 分岐左方向のイラストリンクが方面案内を持つか判断
	while(S_OK == ipLeftCur->NextRow(&ipRow) && ipRow)
	{
		if(setInfIDOfBefore.find(AheGetAttribute(ipRow, m_isAseanSchema?global::lq_dirguide::kInfID:kInfID).lVal) != setInfIDOfBefore.end())
		{
			bLeft = true;
			break;
		}
	}
	// 分岐右方向のイラストリンクが方面案内を持つか判断
	while(S_OK == ipRightCur->NextRow(&ipRow) && ipRow)
	{
		if(setInfIDOfBefore.find(AheGetAttribute(ipRow, m_isAseanSchema?global::lq_dirguide::kInfID:kInfID).lVal) != setInfIDOfBefore.end())
		{
			bRight = true;
			break;
		}
	}

	// 地先種別13、14のみを持つ方面案内は方面無し扱いとする
	// 分岐前リンクが方面案内無し扱いのリンクの場合
	if(setNoDirguideLink.find(lBeforeLinkID) != setNoDirguideLink.end())
	{
		// 分岐左リンクが方面案内無し扱いのリンク
		if(setNoDirguideLink.find(lLeftLinkID) != setNoDirguideLink.end())
			bLeft = false;
		// 分岐右リンクが方面案内無し扱いのリンク
		if(setNoDirguideLink.find(lRightLinkID) != setNoDirguideLink.end())
			bRight = false;
	}

	if(bLeft)
	{
		if(bRight)
			return kLR;

		return kLX;
	}
	if(bRight)
		return kXR;

	return kXX;
}

// 道路ノード1→2→3の角度を求める。（∠123の角度）
// 正確には構成点→道路ノード2→構成点の角度を求める
bool CCheckIllustData::GetAngleOf3Nodes( long node1, long node2, long node3, const CSPIllustLinkRow& spRow, double& angle )
{
	// ノード1→ノード2がなすリンク取得
	auto& firstLink = FindLink(node1, node2);
	if(firstLink == EndLink())
		return false;

	double firstAngle = -1.0;
	// ノード2→構成点の角度が欲しい
	if(!GetOutflowLinkAngle(firstLink->second, node2, firstAngle))
	{
		return false;
	}

	// ノード2→ノード3がなすリンク取得
	auto& secondLink = FindLink(node2, node3);
	if(secondLink == EndLink())
		return false;

	double secondAngle = -1.0;
	// ノード2→構成点の角度が欲しい
	if(!GetOutflowLinkAngle(secondLink->second, node2, secondAngle))
	{
		return false;
	}

	// どちらも流出側の角度を取得しているので、∠123の角度が求まる
	double diffAngle = fmod(secondAngle - firstAngle, 2*M_PI);

	// マイナスの角度を入れないので反転
	while(diffAngle < 0)
		diffAngle += 2*M_PI;

	angle = 360 - (diffAngle * 180 / M_PI);
	return true;
}

// 道路リンクの対象ノード→構成点の角度を取得
bool CCheckIllustData::GetOutflowLinkAngle(IFeaturePtr ipRoadLink, long tgtNode, double& angle)
{
	CComVariant vaFrom = AheGetAttribute(ipRoadLink, m_isAseanSchema?global::road_link::kFromNodeID:road_link::kFromNodeID);
	_ASSERT(vaFrom.vt == VT_I4);
	IGeometryPtr ipGeo;

	// 形状取得できなかった終了
	if(FAILED(ipRoadLink->get_Shape(&ipGeo)))
	{
		AllWriteComment(getMsg(message::iFailedToGetShape));
		return false;
	}
	IPointCollectionPtr ipPointCol(ipGeo);

	long count = ipPointCol->GetPointCount();
	// 順方向
	if (vaFrom.lVal == tgtNode)
	{
		angle = GetAngle(ipPointCol->GetPoint(0), ipPointCol->GetPoint(1));
	}
	// 逆方向
	else
	{
		angle = GetAngle(ipPointCol->GetPoint(count-1), ipPointCol->GetPoint(count-2));
	}
	return true;
}

double CCheckIllustData::GetAngle(IPointPtr ipPoint1, IPointPtr ipPoint2)
{
	double lon1=-1.0, lat1=-1.0, lon2=-1.0, lat2=-1.0;
	ipPoint1->QueryCoords(&lon1, &lat1);
	ipPoint2->QueryCoords(&lon2, &lat2);
	return GetAngle(lon1, lat1, lon2, lat2);
}

double CCheckIllustData::GetAngle(double lon1, double lat1, double lon2, double lat2)
{
	double distX = -1.0, distY = -1.0;
	g_cnv.GetDistXY(lon1, lat1, lon2, lat2, &distX, &distY);
	return atan2(distY, distX);
}

void CCheckIllustData::CheckRelHighwayInfo( sindy::CSPIllustLinkRow& spRow, const sindy::CNodeQueueCheck::NODEQUEUE& vecNodeQueue )
{
	using namespace illust_link;
	long lIllustC = spRow->GetValue(kIllustClass, -1L);
	// チェックしない種別は終了
	switch(lIllustC)
	{
	case illust_class::kAirport:
	case illust_class::kGrade:
	case illust_class::kRealCross:
		return;
	default:
		break;
	}

	bool existHighway = false; // HWYノードがあるか

	// イラストリンクが持つノードIDでループ
	for(const auto& nodeID : vecNodeQueue)
	{
		// 道路ノードに対してのHIGHWAY_NODEの情報が関連付けされていなかった場合
		if(m_mapRelNodeHwyNode.find(nodeID) == m_mapRelNodeHwyNode.end())
		{
			auto& itNode = FindNode(nodeID);
			// 道路ノードが存在しない場合
			if(itNode == EndNode())
				return;

			// 関連付けを行う
			if(!AssociateHighwayInfo(itNode->second))
				return;
		}

		// ノードIDの位置にあるHIGHWAY_NODE群でループ
		for(const auto& hwyID : m_mapRelNodeHwyNode[nodeID])
		{
			existHighway = true;
			if(!IsTargetHWYInfo(lIllustC, m_mapHighwayInfo[hwyID]))
				continue;
			if(CheckRelHighwayInfo(spRow, m_mapHighwayInfo[hwyID]))
			{
				// エラーではないので終了
				return;
			}
		}
	}
	switch(lIllustC)
	{
	case illust_class::kETC:
	case illust_class::kETC2:
	case illust_class::kSAPA:
		// HWYノードがない場合はエラー
		if(!existHighway)
		{
			// 代表としてROAD_NODE_2をエラーメッセージに出す
			CString err = uh::str_util::format(getMsg(message::eThereIsDifferenceInSecondNodeInfoAndHWYInfo), uh::str_util::ToString(spRow->GetValue(kNode2)));
			IllustErrorInfo errInfo(5010, EErrorLevel::kError, err);
			WriteError(spRow, errInfo);
		}
		break;
	default:
		break;
	}

	// ここまで来ていたら、整合のとれたHWYノードが見つからないということなのでエラー
	CString err2 = uh::str_util::format(getMsg(message::eThereIsDifferenceInSecondNodeInfoAndRoadCoadInfo), 
		uh::str_util::ToString(spRow->GetValue(kNode2)),
		uh::str_util::ToString(spRow->GetValue(kRoadName)),
		uh::str_util::ToString(spRow->GetValue(kRoadCode)),
		spRow->GetDomainName(kUpDownC));
	IllustErrorInfo errInfo2(5009, EErrorLevel::kWarning, err2);
	WriteError(spRow, errInfo2);

}

bool CCheckIllustData::AssociateHighwayInfo( IFeaturePtr ipNode )
{
	// ノードの位置にあるHIGHWAY_NODEを空間検索
	IGeometryPtr ipGeo;
	ipNode->get_ShapeCopy(&ipGeo);

	ISpatialReferencePtr ipSpRef;
	IGeoDatasetPtr(m_ipHighwayNodeFC)->get_SpatialReference(&ipSpRef);
	ipGeo->putref_SpatialReference(ipSpRef);

	CString subFields;
	subFields.Format(_T("%s,%s,%s,%s,%s,%s"), kObjectID, highway_node::kRoadCode, highway_node::kRoadSeq, highway_node::kDirection, highway_node::kTollClass, highway_node::kInOut);

	IFeatureCursorPtr ipHighwayCur;
	if( FAILED(m_ipHighwayNodeFC->Search(AheInitQueryFilter(AheInitSpatialFilter(NULL, ipGeo, _T("SHAPE"), esriSearchOrderSpatial, esriSpatialRelIntersects), subFields), VARIANT_FALSE, &ipHighwayCur)) )
	{
		AllWriteComment(getMsg(message::iFailedToSearch));
		return false;
	}

	// 取得できたHIGHWAY_NODEの情報とノードIDを関連付ける
	IFeaturePtr ipHighwayFeature;
	std::vector<long> vecHighwayID;

	while(S_OK == ipHighwayCur->NextFeature(&ipHighwayFeature) && ipHighwayFeature)
	{
		long roadCode = AheGetAttribute(ipHighwayFeature, highway_node::kRoadCode).lVal;
		long roadSeq = AheGetAttribute(ipHighwayFeature, highway_node::kRoadSeq).lVal;
		long direction = AheGetAttribute(ipHighwayFeature, highway_node::kDirection).lVal;
		long tollC = AheGetAttribute(ipHighwayFeature, highway_node::kTollClass).lVal;
		long inOutC = AheGetAttribute(ipHighwayFeature, highway_node::kInOut).lVal;
		CHighwayInfo cHighwayInfo(roadCode, roadSeq, direction, tollC, inOutC);
		cHighwayInfo.SetInfo(m_ipRoadCodeT, m_ipFacilInfoFC);
		// 情報を格納
		m_mapHighwayInfo.insert(std::make_pair(ipHighwayFeature->GetOID(), cHighwayInfo));
		vecHighwayID.push_back(ipHighwayFeature->GetOID());
	}

	// また検索しないようにキャッシュしておく
	m_mapRelNodeHwyNode.insert(std::make_pair(ipNode->GetOID(), vecHighwayID));
	return true;
}

bool CCheckIllustData::IsTargetHWYInfo(long lIllustC, CHighwayInfo cHighwayInfo)
{
	using namespace illust_link;
	switch(lIllustC)
	{
	case illust_class::kBranch:
	case illust_class::kBranch2:
		{
			long facilC = cHighwayInfo.GetFacilClass();
			switch(facilC)
			{
			case facil_info_point::facil_class::kJCT:
			case facil_info_point::facil_class::kRamp:
			case facil_info_point::facil_class::kSmartIC:
				return true;
			default: break;
			}
		}
		break;
	case illust_class::kETC:
		if(cHighwayInfo.GetTollClass() == highway_node::toll_class::kMainTollbooth)
			return true;
		break;
	case illust_class::kETC2:
		if(cHighwayInfo.GetTollClass() == highway_node::toll_class::kRampTollbooth)
			return true;
		break;
	case illust_class::kEntrance:
		if(cHighwayInfo.GetInOutClass() == highway_node::in_out::kIn)
			return true;
		break;
	case illust_class::kExit:
		if(cHighwayInfo.GetInOutClass() == highway_node::in_out::kOut)
			return true;
		break;
	case illust_class::kInIC:
		if(cHighwayInfo.GetTollClass() == highway_node::toll_class::kGateway)
			return true;
		break;
	case illust_class::kProperJCT:
		{
			long facilC = cHighwayInfo.GetFacilClass();
			switch(facilC)
			{
			case facil_info_point::facil_class::kJCT:
			case facil_info_point::facil_class::kRamp:
			case facil_info_point::facil_class::kSmartIC:
				return true;
			default: break;
			}
		}
		break;
	case illust_class::kSAPA:
		{
			long facilC = cHighwayInfo.GetFacilClass();
			switch(facilC)
			{
			case facil_info_point::facil_class::kSA:
			case facil_info_point::facil_class::kPA:
				return true;
			default: break;
			}
		}
		break;
	default:
		break;
	}
	return false;
}

bool CCheckIllustData::CheckRelHighwayInfo( sindy::CSPIllustLinkRow& spRow, CHighwayInfo& cHighwayInfo )
{
	if( spRow->GetValue(illust_link::kRoadCode, -1L) == cHighwayInfo.GetRoadCode() &&
		spRow->GetValue(illust_link::kRoadName, _T("")) == cHighwayInfo.GetRoadKanji() &&
		spRow->GetValue(illust_link::kRoadSeq, -1L) == cHighwayInfo.GetRoadSeq() &&
		spRow->GetValue(illust_link::kFacilName, _T("")) == cHighwayInfo.GetFacilKanji() &&
		spRow->GetValue(illust_link::kUpDownC, -1L) == cHighwayInfo.GetDirectionCode() )
	{
		CString strGuideRoute = spRow->GetValue(illust_link::kGuideRoute, _T(""));
		if( illust_util::ConvGuideRoute(CStringA(strGuideRoute).GetString()) == cHighwayInfo.GetDirectionCode() )
			return true;
	}
	return false;
}

ROADLINKMAP::const_iterator CCheckIllustData::FindLink(long node1, long node2) const
{
	const auto& itFromTo =  m_mapRoadLink.find(FromToNode(node1, node2));
	return (itFromTo == EndLink()) ? m_mapRoadLink.find(FromToNode(node2, node1)) : itFromTo;
}

FEATUREMAP::const_iterator CCheckIllustData::FindNode(long node) const
{
	return m_mapRoadNode.find(node);
}

// エラーログにコメントを書き込む
void CCheckIllustData::WriteComment(const CString& message)
{
	ErrLog() << L"# " << CStringW(message).GetString() << std::endl;

}

void CCheckIllustData::AllWriteComment(const CString& message)
{
	std::cout << CStringA(message) << std::endl;
	RunLog() << CStringW(message).GetString() << std::endl;

	WriteComment(message);
}

// エラーログにエラーメッセージを書き込む
void CCheckIllustData::WriteError( const sindy::CSPIllustLinkRow& spRow, const IllustErrorInfo& errInfo )
{
	long lIllustC = spRow->GetValue(m_isAseanSchema?global::illust_link::kIllustClass:illust_link::kIllustClass, -1L);
	long lCustomer = spRow->GetValue(m_isAseanSchema?global::illust_link::kCustomerCode:illust_link::kCustomerCode, -1L);
	auto& rule = m_RuleInfo.find(lIllustC, lCustomer);
	bool bExistRule = rule != m_RuleInfo.end(); // ルールがあるか
	CString strDate, strEditor;
	GetNewestEditInfo(spRow, strDate, strEditor);

	WriteError(spRow->GetTableName(), spRow->GetOID(), errInfo, lIllustC, lCustomer, (bExistRule ? spRow->GetIllustName(rule->mIllustNameRule) : _T("")), strDate, strEditor);
}

// エラーログにエラーメッセージを書き込む
void CCheckIllustData::WriteError( const CString& field,
								  long oid,
								  const IllustErrorInfo& errInfo,
								  long lIllustC,
								  long lCustomer,
								  const CString& illustName,
								  const CString& strDate,
								  const CString& strEditor)
{
	CString message = uh::str_util::format(_T("0\t%s\t%ld\t\t\t%s\t%d\t%s\t%ld\t%ld\t%s\t%s\t%s\t%s"),
		field, oid, errInfo.GetErrorLevelStr(), errInfo.GetErrorCode(), errInfo.GetErrorMessage(), lIllustC, lCustomer, illustName, errInfo.GetIllustDir(), strDate, strEditor);

	ErrLog() << CStringW(message).GetString() << std::endl;

}

void CCheckIllustData::GetNewestEditInfo(const sindy::CSPIllustLinkRow& spRow, CString& strDate, CString& strEditor)
{
	CString strModDate = GetTime(spRow, m_isAseanSchema?global::illust_link::kModifyDate:illust_link::kModifyDate);
	CString strProgModDate = GetTime(spRow, m_isAseanSchema?global::illust_link::kProgModifyDate:illust_link::kProgModifyDate);
	if(strModDate >= strProgModDate)
	{
		strDate = strModDate;
		strEditor = spRow->GetValue(m_isAseanSchema?global::illust_link::kOperator:illust_link::kOperator, _T(""));
	}
	else
	{
		strDate = strProgModDate;
		strEditor = spRow->GetValue(m_isAseanSchema?global::illust_link::kModifyProgName:illust_link::kModifyProgName, _T(""));
	}
}

CString CCheckIllustData::GetTime(const sindy::CSPIllustLinkRow& spRow, const CString& strField)
{
	_variant_t vaDate = spRow->GetValue(strField);

	if(vaDate.vt == VT_DATE)
	{
		SYSTEMTIME stDate;
		VariantTimeToSystemTime(vaDate.date, &stDate);
		return uh::str_util::ToString(&stDate);
	}
	return _T("");
}


// 終了ログを書きこむ
void CCheckIllustData::WriteEndLog(bool result)
{
	SYSTEMTIME aTime;
	GetLocalTime(&aTime);
	RunLog() << CStringW(getMsg(message::iWriteFinishTime)).GetString() << CStringW(uh::str_util::ToString(&aTime)).GetString() << std::endl;
		if(result)
			RunLog() << CStringW(getMsg(message::iSuccessfulTermination)).GetString() << std::endl;
		else
			RunLog() << CStringW(getMsg(message::iAbnormalTermination)).GetString() << std::endl;
}
