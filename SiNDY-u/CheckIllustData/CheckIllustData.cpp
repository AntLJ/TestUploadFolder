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

// ���s
bool CCheckIllustData::execute()
{
	// �u�V�X�e�������{����v�t���O���擾����
	m_isJapaneseOs =  ( GetSystemDefaultLCID() == util::JAPANESE_LANGUAGE );

	// ������
	if( !init() )
		return false;

	// ASEAN�����t���O���擾����
	m_isAseanSchema = AheIsWorldMeshMode();

	IFeatureClassPtr ipIllustLinkFC;
	// �C���X�g�����N�t�B�[�`���N���X���擾
	if( !util::OpenFeatureClass( m_isAseanSchema?global::illust_link::kTableName:illust_link::kTableName, m_ipIllustWorkspace, ipIllustLinkFC ) )
	{
		CString err = uh::str_util::format( getMsg(message::iCannotOpen), m_isAseanSchema?global::illust_link::kTableName:illust_link::kTableName );
		WriteComment(err);
		WriteEndLog(false);
		return false;
	}

	// ���H�����N�t�B�[�`���N���X���擾
	if( !util::OpenFeatureClass( m_isAseanSchema?global::road_link::kTableName:road_link::kTableName, m_ipRoadWorkspace, m_ipRoadLinkFC ) )
	{
		CString err = uh::str_util::format( getMsg(message::iCannotOpen), m_isAseanSchema?global::road_link::kTableName:road_link::kTableName );
		WriteComment(err);
		WriteEndLog(false);
		return false;
	}

	// ���H�m�[�h�t�B�[�`���N���X���擾
	if( !util::OpenFeatureClass( m_isAseanSchema?global::road_node::kTableName:road_node::kTableName, m_ipRoadWorkspace, m_ipRoadNodeFC ) )
	{
		CString err = uh::str_util::format( getMsg(message::iCannotOpen), m_isAseanSchema?global::road_node::kTableName:road_node::kTableName );
		WriteComment(err);
		WriteEndLog(false);
		return false;
	}

	// HIGHWAY_NODE�t�B�[�`���N���X���擾
	if( !m_isAseanSchema )
	{
		if( !util::OpenFeatureClass( highway_node::kTableName, m_ipRoadWorkspace, m_ipHighwayNodeFC ) )
		{
			CString err = uh::str_util::format( getMsg(message::iCannotOpen), highway_node::kTableName );
			WriteComment(err);
			WriteEndLog(false);
			return false;
		}

		// FACIL_INFO_POINT�t�B�[�`���N���X���擾
		if( !util::OpenFeatureClass( facil_info_point::kTableName, m_ipRoadWorkspace, m_ipFacilInfoFC ) )
		{
			CString err = uh::str_util::format( getMsg(message::iCannotOpen), facil_info_point::kTableName );
			WriteComment(err);
			WriteEndLog(false);
			return false;
		}

		// ROAD_CODE_LIST�e�[�u�����擾
		if( !util::OpenTable( road_code_list::kTableName, m_ipRoadWorkspace, m_ipRoadCodeT ) )
		{
			CString err = uh::str_util::format( getMsg(message::iCannotOpen), road_code_list::kTableName );
			WriteComment(err);
			WriteEndLog(false);
			return false;
		}
	}
	// LQ_DIRGUIDE�e�[�u�����擾
	if( !util::OpenTable( m_isAseanSchema?global::lq_dirguide::kTableName:lq_dirguide::kTableName, m_ipRoadWorkspace, m_ipLQDirguideT ) )
	{
		CString err = uh::str_util::format( getMsg(message::iCannotOpen), m_isAseanSchema?global::lq_dirguide::kTableName:lq_dirguide::kTableName );
		AllWriteComment(err);
		return false;
	}

	// LQ_TURNREG�e�[�u�����擾
	if( !util::OpenTable( m_isAseanSchema?global::lq_turnreg::kTableName:lq_turnreg::kTableName, m_ipRoadWorkspace, m_ipLQTurnRegT ) )
	{
		CString err = uh::str_util::format( getMsg(message::iCannotOpen), m_isAseanSchema?global::lq_turnreg::kTableName:lq_turnreg::kTableName );
		AllWriteComment(err);
		return false;
	}

	// INF_TURNREG�e�[�u�����擾
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

	// �e�[�u���t�@�C���_�̏�����
	CModel model;
	m_finder.InitCollection(ITablePtr(ipIllustLinkFC), model);

	CSPIllustLinkTable spIllustTable = CAST_ILLUSTLINKTABLE(m_finder.FindTable(m_isAseanSchema?global::illust_link::kTableName:illust_link::kTableName));
	spIllustTable->_Select(AheInitQueryFilter(), true);

	// �֘A���铹�H�m�[�h�A���H�����N�A�E���܋֎~����map�֊i�[
	if(!SetRelMap(spIllustTable))
	{
		WriteComment(getMsg(message::iCannotSearchRelIllustLink));
		WriteEndLog(false);
		return false;
	}

	// �`�F�b�N���s
	if( !check(spIllustTable) )
	{
		WriteEndLog(false);
		return false;
	}
	WriteEndLog(true);
	return true;
}

// ������
bool CCheckIllustData::init()
{
	if(!OpenFile())		return false;

	std::cout << CStringA(m_args.m_roaddb.c_str()) << CStringA(getMsg(message::iConnectMsg)) << std::endl;
	// ���HDB�ɐڑ�
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
	// �C���X�gDB�ɐڑ�
	m_ipIllustWorkspace = sindy::create_workspace(m_args.m_illustdb.c_str());
	if(!m_ipIllustWorkspace)
	{
		CString err = uh::str_util::format(getMsg(message::iNotConnectDB), m_args.m_illustdb.c_str());
		WriteComment(err);
		WriteEndLog(false);
		return false;
	}
	std::cout << CStringA(getMsg(message::iSuccessConnect)) << std::endl;

	// ���s���O�ɏ�����������
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

	// �G���[���O�ɏ�����������
	ErrLog() << L"# SINDYSTDLOG" << std::endl;

	if(!SetMode())
	{
		RunLog() << CStringW(getMsg(message::iInvalidMode)).GetString() << std::endl;

		WriteEndLog(false);
		return false;
	}
	return true;
}



// �e�t�@�C�����J��
bool CCheckIllustData::OpenFile()
{
	bool ret = true;

	// [config]�Őݒ肳�ꂽ�C���X�g�t�@�C���i�[�����ǂݎ��
	if( !m_DirInfo.read( m_args.m_config.c_str()) )
	{
		std::cerr << CStringA(getMsg(message::iFailedToReadConfig)) << std::endl;
		ret = false;
	}

	// [name_rule]�Őݒ肳�ꂽ�C���X�g�t�@�C�����������[����ǂݎ��
	if( !m_RuleInfo.read(m_args.m_namerule.c_str()) )
	{
		std::cerr << CStringA(getMsg(message::iFailedToReadIllustNameRule)) << std::endl;
		ret = false;
	}

	/*
	// ���s���O�A�G���[���O�̃t�@�C�����ɓ��t��ǉ�����
	SYSTEMTIME aTime;
	GetLocalTime(&aTime);
	CString runLog = util::AddDateToFileName(aTime, m_args.m_runlog.c_str());
	CString errLog = util::AddDateToFileName(aTime, m_args.m_errlog.c_str());
	*/
	m_ofsRunLog = new std::wofstream(CStringA(m_args.m_runlog.c_str()), std::ios::out);
	m_ofsErrLog = new std::wofstream(CStringA(m_args.m_errlog.c_str()), std::ios::out);

	// ���s���O�J���ĂȂ�������G���[
	if(!RunLog().is_open())
	{
		CString err = uh::str_util::format(getMsg(message::iCannotOpen), m_args.m_runlog.c_str());
		std::cerr << CStringA(err) << std::endl;
		ret = false;
	}
	// �G���[���O�J���ĂȂ�������G���[
	if(!ErrLog().is_open())
	{
		CString err = uh::str_util::format(getMsg(message::iCannotOpen), m_args.m_errlog.c_str());
		std::cerr << CStringA(err) << std::endl;
		ret = false;
	}

	// ���{����ł͂Ȃ��ꍇ
	if( !m_isJapaneseOs )
	{
		// BOM�Ȃ�UTF-8�ŏo�͂���
		std::locale utf8_locale(std::locale(), new std::codecvt_utf8<wchar_t>);
		m_ofsRunLog->imbue( utf8_locale );
		m_ofsErrLog->imbue( utf8_locale );
	}
	return ret;
}

// ���[�h��ݒ肷��
bool CCheckIllustData::SetMode()
{
	int mode = _ttol(m_args.m_mode.c_str());
	switch(mode)
	{
	case 1:
		m_Mode = EMode::kSchemaC;
		break;
	case 2:
		m_Mode = EMode::kNodeAndIllustC; // �m�[�h���X�g+�C���X�g�`�F�b�N
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

// �C���X�g�����N�Ɋ֘A����t�B�[�`�����L���b�V��
bool CCheckIllustData::SetRelMap(CSPIllustLinkTable spIllustTable)
{
	std::cout << CStringA(getMsg(message::iCacheDataMsg)) << std::endl;
	// ILLUST_LINK�Ɋւ��m�[�hID��S�Ē��o
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
	// �����p��1000���P�ʂŕ���
	std::vector<std::string> vecNodes = uh::enumerate_elements(vecNodeID);

	// FROM_NODE_ID��TO_NODE_ID��TURNREG_LQRF�̃C���f�b�N�X�����
	// ���C�u�����ɈڐA����ꍇ�̓��[�v�̒��Ŏ��Ȃ��Ƃ����Ȃ�����
	long fromIndex = m_ipRoadLinkFC->GetFields()->_FindField(road_link::kFromNodeID);
	long toIndex = m_ipRoadLinkFC->GetFields()->_FindField(road_link::kToNodeID);
	long turnRegIndex = m_ipRoadLinkFC->GetFields()->_FindField(road_link::kTurnRegLQRF);
	long size = vecNodes.size();
	long count = 0;

	std::cout << CStringA(getMsg(message::iCachingRoadLink)) << std::endl;

	// �E���܋֎~�����������H�����N�Q
	std::vector<long> hasTurnRegLinks;

	// �֘A����t�B�[�`�����i�[
	for(const auto& nodes : vecNodes)
	{
		long aProgress = ++count * 100/size;
		std::cout << aProgress << "%" << "\r";
		// ROAD_NODE���i�[
		IFeatureCursorPtr ipNodeCursor;

		// SubFields��K�X�ݒ肷�邱��
		CString nodeSubFields = uh::str_util::format(_T("%s,%s,%s"), kObjectID, road_node::kNodeClass, feature::kShape);
		if( FAILED(m_ipRoadNodeFC->Search(AheInitQueryFilter(NULL, nodeSubFields, _T("%s IN(%s)"), kObjectID, CString(nodes.c_str())), VARIANT_FALSE, &ipNodeCursor)) )
			return false;

		IFeaturePtr ipNodeFeature;
		while(S_OK == ipNodeCursor->NextFeature(&ipNodeFeature) && ipNodeFeature)
		{
			long oid = ipNodeFeature->GetOID();
			m_mapRoadNode.insert(std::make_pair(oid, ipNodeFeature));
		}

		// ROAD_LINK���i�[
		IFeatureCursorPtr ipLinkCursor;

		// SubFields��K�X�ݒ肷�邱��
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
		// ���Ӄ����N����肽���̂ŁAOR�ŏ����w��
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
				hasTurnRegLinks.emplace_back(ipLinkFeature->GetOID()); // �E���܋֎~���擾�p�ɏW�߂Ă���
		}
	}

	std::cout << CStringA(getMsg(message::iFinishedToCacheRoadLink)) << std::endl;

	// �C���X�g�����N�Ɋ֘A����E���܋֎~���S�Ă��擾
	if( !CNodeQueueCheck::CacheTurnReg(hasTurnRegLinks, m_ipINFTurnRegT, m_ipLQTurnRegT, m_mapTurnReg, m_mapTurnRegInf) )
		return false;

	std::cout << CStringA(getMsg(message::iFinishedToCacheTurnReg)) << std::endl;

	if(m_ipSubRoadLinkT && m_ipSubRoadPartT)
	{
		// �C���X�g�����N�Ɋ֘A����J�ʓ��J�ʏ����擾
		CNodeQueueCheck::CacheSubRoad(m_ipSubRoadLinkT, m_ipSubRoadPartT, m_unopenedRoadId);
		std::cout << CStringA(getMsg(message::iFinishedToCacheSubRoad)) << std::endl;
	}

	std::cout << CStringA(getMsg(message::iSuccessfulCache)) << std::endl;

	return true;
}

// ���C���`�F�b�N
bool CCheckIllustData::check(CSPIllustLinkTable spIllustTable)
{
	using namespace illust_link;

	size_t containerSize = spIllustTable->size();
	std::set<CString> checkSameIllustNameSet; // �����ʁA�d����̃C���X�g���`�F�b�N�p
	CHECKSAMENODEMAP checkSameNodeMap; // �����ʁA�d����̃m�[�h��`�F�b�N�p
	CHECKDIRECTIONCODEMAP checkDirectionMap; // �����R�[�h�̐������`�F�b�N�p
	std::set<CString> setExceptField ; // �C���X�g���������ɁA�Ӑ}�I�ɍl�����Ȃ��t�B�[���h�ꗗ
	// �X�L�[�}���u���������v�̏ꍇ
	if(!m_isAseanSchema)
	{
		setExceptField = boost::assign::list_of(kDirectionCode)(kDirguideExistCode)(kSingleBranchCode);
	}else
	// �X�L�[�}���uASEAN�����v�̏ꍇ
	{
		setExceptField = boost::assign::list_of(global::illust_link::kDirectionCode)
			(global::illust_link::kUTurnFlag)
			(global::illust_link::kRoundaboutFlag);
	}

	int aCount = 0;
	// ILLUST_LINK�P�̂̃`�F�b�N
	for(const auto& col : *spIllustTable)
	{
		std::cout << ++aCount << " / " << containerSize << "\r";
		CSPIllustLinkRow row = CAST_ILLUSTLINKROW(col);
		CNodeQueueCheck cNodeCheck(row, m_mapRoadLink, m_mapRoadNode, m_mapTurnReg, m_mapTurnRegInf, m_ipRoadLinkFC, m_ipRoadNodeFC, m_unopenedRoadId);
		// �X�L�[�}�`�F�b�N
		if(m_Mode & EMode::kSchemaC)
		{
			ERRLIST errList;
			// �X�L�[�}�`�F�b�N
			errList = CIllustLinkRelCheck::SchemaCheckAll(row, getMsg(message::eIllustLinkHasInvalidValue));
			if(!errList.empty())
			{
				for(const auto& err : errList)
					WriteError(row, err);
				// �X�L�[�}�`�F�b�N�ɒʂ��ĂȂ��̂ɁA�l�`�F�b�N���Ă��Ӗ��Ȃ��̂ŏI��
				continue;
			}

			// �X�L�[�}���u���������v�̏ꍇ,�l�`�F�b�N
			if(!m_isAseanSchema)
			{
				// �l�`�F�b�N
				errList = CIllustLinkRelCheck::RelCheckAll(row, getMsg(message::eIllustLinkHasInvalidValue));
			}

			// �I�u�W�F�N�g�`�F�b�N
			cNodeCheck.CheckIllustShape(errList);
			for(const auto& err : errList)
				WriteError(row, err);
		}

		// �m�[�h���X�g�`�F�b�N
		if(m_Mode & EMode::kNodeC)
		{
			CNodeQueueCheck::NODEQUEUE vecNodeQueue;
			// �m�[�h���X�g�`�F�b�N
			ERRLIST errList = cNodeCheck.CheckNodeQueue(vecNodeQueue);

			// �m�[�h��A�C���X�g���d���`�F�b�N
			CheckDuplicate(row, vecNodeQueue, checkSameNodeMap, checkSameIllustNameSet, errList);

			for(const auto& err : errList)
				WriteError(row, err);

			// �X�L�[�}���u���������v�̏ꍇ,HWY�n�̐������`�F�b�N
			if(!m_isAseanSchema)
			{
				// HWY�n�̐������`�F�b�N
				CheckRelHighwayInfo(row, vecNodeQueue);
			}

			long lIllustC = row->GetValue(m_isAseanSchema?global::illust_link::kIllustClass:kIllustClass, -1L);
			long lCustomer = row->GetValue(m_isAseanSchema?global::illust_link::kCustomerCode:kCustomerCode, -1L);
			long lNode1 = row->GetValue(m_isAseanSchema?global::illust_link::kNode1:kNode1, -1L);
			// �X�L�[�}���uASEAN�����v�̏ꍇ
			if(m_isAseanSchema)
			{
				// �C���X�g�� + �C���X�g��� + �d���� + �m�[�h1���L�[�ɃO���[�v��
				CString checkDirectionKey = GetIllustName(row, setExceptField);
				checkDirectionKey.AppendFormat(_T(",%ld,%ld,%ld"), lIllustC, lCustomer, lNode1); 
				checkDirectionMap.insert(std::make_pair(checkDirectionKey, row)); // �����R�[�h�̐������`�F�b�N�p�Ɋi�[
			}else
			// �X�L�[�}���u���������v�̏ꍇ
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
						// �C���X�g�� + �C���X�g��� + �d���� + �m�[�h1���L�[�ɃO���[�v��
						CString checkDirectionKey = GetIllustName(row, setExceptField);
						checkDirectionKey.AppendFormat(_T(",%ld,%ld,%ld"), lIllustC, lCustomer, lNode1); 
						checkDirectionMap.insert(std::make_pair(checkDirectionKey, row)); // �����R�[�h�̐������`�F�b�N�p�Ɋi�[
					}
				default:
					break;
				}
			}
		}

		// �C���X�g�`�F�b�N
		if(m_Mode & EMode::kIllustC)
		{
			ERRLIST errList = cNodeCheck.CheckIllustFile(m_DirInfo, m_RuleInfo);
			for(const auto& err : errList)
				WriteError(row, err);
		}
		
	}

	std::cout << CStringA(getMsg(message::iFinishedToCheckSingleIllustLink)) << std::endl;
	// ILLUST_LINK�S�̂̃`�F�b�N
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

// ���E���h�A�o�E�g�̃`�F�b�N
void CCheckIllustData::checkRoundAbout( const CNodeQueueCheck::NODEQUEUE& vecNodeQueue, ERRLIST& errList)
{
	for(unsigned long nodeIdx1 = 0; nodeIdx1 < vecNodeQueue.size()-2; ++nodeIdx1)
	{
		unsigned long firstNode = vecNodeQueue[nodeIdx1];
		unsigned long secondNode = vecNodeQueue[nodeIdx1+1];

		// �m�[�h�ɂ���ă����N���擾
		IFeaturePtr ipCurrentLink = getRoadLinkFeature( firstNode, secondNode );
		IFeaturePtr ipLink = getRoadLinkFeature( vecNodeQueue[nodeIdx1+1], vecNodeQueue[nodeIdx1+2] );

		// �����N���擾���Ȃ��ꍇ
		if( !ipCurrentLink || !ipLink )
		{
			CString err = uh::str_util::format(getMsg(message::eNotExistRoadLink), uh::str_util::ToString(firstNode), uh::str_util::ToString(secondNode));
			IllustErrorInfo errInfo(1001, EErrorLevel::kError, err);
			errList.push_back(errInfo);	
			return;
		}

		// ���E���h�A�o�E�g�̃C���X�g�����N���쐬����B�C���X�g�����N���쐬����ꍇ�ɁA���ꃊ���N����I��,�A���w��͂m�f.
		if( firstNode == vecNodeQueue[nodeIdx1+2] )
		{
			CString err = uh::str_util::format(getMsg(message::eRounAboutLinkIsContinuous));
			IllustErrorInfo errInfo(3004, EErrorLevel::kError, err);
			errList.push_back(errInfo);
			return;
		}

		// �Y�������N�̃I�u�W�F�N�gID
		long currentLinkId = ipCurrentLink->GetOID();

		// �o�R�����N����1�Ԗڃ����N�̎�ʂ̎擾�B
		bool isExistRoundAboutLink(false), isExistIntersectionLink(false), isExistNormalLink(false);
		getRoadLinkClassFlag( ipLink, isExistRoundAboutLink, isExistIntersectionLink, isExistNormalLink );

		// �c���Ă��郊���N�Ɣ�r����
		for(unsigned long nodeIdx2 = nodeIdx1+2; nodeIdx2 < vecNodeQueue.size()-1; ++nodeIdx2)
		{
			// �m�[�h�ɂ���ă����N���擾
			IFeaturePtr ipNextLink = getRoadLinkFeature( vecNodeQueue[nodeIdx2], vecNodeQueue[nodeIdx2+1] );

			// �����N���擾���Ȃ��ꍇ
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
				// �o�R�����N����2�Ԗڃ����N���炷�ׂĂ̌o�R�����N�̎�ʂ̎擾�B
				getRoadLinkClassFlag( ipNextLink, isExistRoundAboutLink, isExistIntersectionLink, isExistNormalLink );
				continue;
			}

			// �ŏ��̃����N�ƍŌ�̃����N�ȊO�ŁA���ꃊ���N�����I������Ă����ꍇ
			if( nodeIdx1 != 0 )
			{
				CString err = uh::str_util::format(getMsg(message::eNotRoundaboutLink));
				IllustErrorInfo errInfo(3006, EErrorLevel::kError, err);
				errList.push_back(errInfo);
				return ;			
			}

			// �C���X�g�����N���쐬����ꍇ�ɁA���ꃊ���N����I��,���������͂m�f.
			if( firstNode == vecNodeQueue[nodeIdx2] && secondNode == vecNodeQueue[nodeIdx2+1] )
			{
				CString err = uh::str_util::format( getMsg( message::eRounAboutLinkIsSameDirection ) );
				IllustErrorInfo errInfo( 3003, EErrorLevel::kError, err );
				errList.push_back(errInfo);
				return;
			}

			// ���E���h�A�o�E�g�̃C���X�g�����N���쐬����B�C���X�g�����N���쐬����ꍇ�ɁA���ꃊ���N����I��,���ꃊ���N�ɖ߂�����͒ǉ������N�I��s��
			if( firstNode == vecNodeQueue[nodeIdx2+1] && secondNode == vecNodeQueue[nodeIdx2] && nodeIdx2+1 != vecNodeQueue.size()-1 )
			{
				CString err = uh::str_util::format(getMsg(message::eRounAboutLinkIsAdditional));
				IllustErrorInfo errInfo(3005, EErrorLevel::kError, err);
				errList.push_back(errInfo);
				return;
			}

			// ���E���h�A�o�E�g�̃C���X�g�����N���쐬����B�C���X�g�����N���쐬����ꍇ�ɁA���ꃊ���N����I��, �������N�o�R�͂m�f
			// ���������N�������_�����N�����o�����N�i�����������N)�I��s��
			if(isExistIntersectionLink && !isExistRoundAboutLink && !isExistNormalLink)
			{
				CString err = uh::str_util::format(getMsg(message::eNotRoundaboutLink));
				IllustErrorInfo errInfo(3006, EErrorLevel::kError, err);
				errList.push_back(errInfo);	
				return ;			
			}

			// ���������N�����ʃ����N�����o�����N�i�����������N)�I��s��
			if(!isExistIntersectionLink && !isExistRoundAboutLink && isExistNormalLink)
			{
				CString err = uh::str_util::format(getMsg(message::eNotRoundaboutLink));
				IllustErrorInfo errInfo(3006, EErrorLevel::kError, err);
				errList.push_back(errInfo);	
				return ;			
			}

			// ���������N�����ʃ����N�ƌ����_�����N���݁����o�����N�i�����������N)�I��s��
			if(isExistIntersectionLink && !isExistRoundAboutLink && isExistNormalLink)
			{
				CString err = uh::str_util::format(getMsg(message::eNotRoundaboutLink));
				IllustErrorInfo errInfo(3006, EErrorLevel::kError, err);
				errList.push_back(errInfo);	
				return ;			
			}

			// ���������N�����E���h�A�o�E�g�ƕ��ʃ����N���݁����o�����N�i�����������N)�I��s��
			if(!isExistIntersectionLink && isExistRoundAboutLink && isExistNormalLink)
			{
				CString err = uh::str_util::format(getMsg(message::eNotRoundaboutLink));
				IllustErrorInfo errInfo(3006, EErrorLevel::kError, err);
				errList.push_back(errInfo);	
				return ;			
			}

			// ���������N�����E���h�A�o�E�g�ƕ��ʃ����N�ƌ����_�����N���݁����o�����N�i�����������N)�I��s��
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

// �����ʁA�d����ŏd�����Ă��郌�R�[�h���Ȃ����`�F�b�N
void CCheckIllustData::CheckDuplicate( sindy::CSPIllustLinkRow& spRow, const CNodeQueueCheck::NODEQUEUE& vecNodeQueue, CHECKSAMENODEMAP& checkSameNodeMap, std::set<CString>& checkSameIllustNameSet, ERRLIST& errList )
{
	long lIllustC = spRow->GetValue(m_isAseanSchema?global::illust_link::kIllustClass:illust_link::kIllustClass,-1L);
	long lCustomer = spRow->GetValue(m_isAseanSchema?global::illust_link::kCustomerCode:illust_link::kCustomerCode,-1L);
	// ASEAN�����̃X�L�[�}�ɂ̓t�B�[���h�uSINGLE_BRANCH_C�v���Ȃ��̂ŁA-1�Ɛݒ肷��
	long lSingleBranch = m_isAseanSchema?-1L:spRow->GetValue(illust_link::kSingleBranchCode, -1L);
	// �C���X�g��ʁA�d����ASINGLE_BRANCH_C
	CString strCheck = uh::str_util::format(_T("%ld,%ld,%ld-"), lIllustC, lCustomer, lSingleBranch);

	// �C���X�g��ʁA�d����A�m�[�h��̕�����
	for(const auto& node : vecNodeQueue)
	{
		strCheck.AppendFormat(_T("%ld,"), node);
	}

	// �Ō�́u�C�v������Ă������ƂŁA�m�[�h�񂪁u1,11,2,�v�Ɓu1,11,21,�v�̏ꍇ�Ɍ���ăG���[�o�͂���Ȃ��Ȃ�i�G���[�R�[�h:3002�j
	auto nodeRet = checkSameNodeMap.insert(std::make_pair(strCheck, spRow));

	// ���������񂪂���΃m�[�h�񂪏d�����Ă���
	if(!nodeRet.second)
	{
		CString err = uh::str_util::format(getMsg(message::eSameLinkQueue));
		IllustErrorInfo errInfo(3001, EErrorLevel::kFatal, err);
		errList.push_back(errInfo);
	}

	// �X�L�[�}���u���������v�̏ꍇ
	if(!m_isAseanSchema)
	{
		// �����ȍ~�̓C���X�g���d���`�F�b�N
		// �`�F�b�N���Ȃ���ʁA�d����͏I��
		switch(lIllustC)
		{
		case illust_link::illust_class::kEntrance:
		case illust_link::illust_class::kExit:
		case illust_link::illust_class::kGrade:
			return;
		case illust_link::illust_class::kBranch:
		case illust_link::illust_class::kBranch2:
			// TF�ȊO�͏I��
			if(lCustomer != illust_link::customer::kTF)
				return;
			break;
		default:
			break;
		}
	}
	CString checkNameStr;
	// �X�L�[�}���u���������v�̏ꍇ
	if(!m_isAseanSchema)
	{
		// SAPA�̓C���X�g��ʁA�C���X�g���̕�����ł܂Ƃ߂�
		if(lIllustC == illust_link::illust_class::kSAPA)
			checkNameStr.Format(_T("%ld,%s"), lIllustC, GetIllustName(spRow));
		// SAPA�ȊO�̓C���X�g��ʁA�d����A�C���X�g���̕�����ł܂Ƃ߂�
		else
			checkNameStr.Format(_T("%ld,%ld,%s"), lIllustC, lCustomer, GetIllustName(spRow));
	}else
	// �X�L�[�}���uASEAN�����v�̏ꍇ
	{
		long lDireCode = spRow->GetValue(global::illust_link::kDirectionCode, -1L);
		checkNameStr.Format(_T("%ld,%ld,%s,%ld"), lIllustC, lCustomer, GetIllustName(spRow), lDireCode);
	}
	auto nameRet = checkSameIllustNameSet.insert(checkNameStr);

	// ���������񂪂���΃C���X�g�����d�����Ă���
	if(!nameRet.second)
	{
		CString err = uh::str_util::format(getMsg(message::eSameFileName), GetIllustName(spRow));
		IllustErrorInfo errInfo(6001, EErrorLevel::kError, err);
		errList.push_back(errInfo);
	}
	
	// ���E���h�A�o�E�g�̃C���X�g�����N�̃`�F�b�N
	checkRoundAbout(vecNodeQueue, errList);

}

// �C���X�g�����擾����
CString CCheckIllustData::GetIllustName(sindy::CSPIllustLinkRow& spRow, const std::set<CString>& exceptFields)
{
	long lIllustC = spRow->GetValue(m_isAseanSchema?global::illust_link::kIllustClass:illust_link::kIllustClass, -1L);
	long lCustomer = spRow->GetValue(m_isAseanSchema?global::illust_link::kCustomerCode:illust_link::kCustomerCode, -1L);
	const auto& itIllustRule = m_RuleInfo.find(lIllustC, lCustomer);
	if(itIllustRule == m_RuleInfo.end())
		return CString();
	return spRow->GetIllustName(itIllustRule->mIllustNameRule, exceptFields);
}

// �N�_��������Ă���m�[�h��`�F�b�N
void CCheckIllustData::CheckComprehensionNode( const CHECKSAMENODEMAP& checkSameNodeMap )
{
	for(CHECKSAMENODEMAP::const_iterator it = checkSameNodeMap.begin(); it != checkSameNodeMap.end(); ++it)
	{
		CHECKSAMENODEMAP::const_iterator nextIt = std::next(it); // ���̗v�f

		// �Ō�܂ŉ��Ȃ����ǂ��傤���Ȃ�
		if(nextIt == checkSameNodeMap.end())
			break;
		// �N�_��������Ă���ꍇ��Find��0���Ԃ�
		if( 0 == nextIt->first.Find(it->first) )
		{
			CString err = uh::str_util::format(getMsg(message::eFirstNodeIsSameAndIncludedLink));
			IllustErrorInfo errInfo(3002, EErrorLevel::kFatal, err);
			WriteError(it->second, errInfo);
		}
	}
}

// �m�[�h��ƕ����R�[�h�̐������`�F�b�N
bool CCheckIllustData::CheckDirectionCode( const CHECKDIRECTIONCODEMAP& checkDirectionMap )
{
	std::set<CString> directionKeySet;
	// �L�[�l���i�[
	for(const auto& illust : checkDirectionMap)
	{
		directionKeySet.insert(illust.first);
	}

	// ���ʈē������Ƃ��Ĉ������H�����N���擾����
	std::set<long> setNoDirguideLink;
	if(!GetRoadLinkTreatedAsNoDirguide(setNoDirguideLink))
	{
		return false;
	}

	// �L�[�P�ʂŏ������s��
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
	// �C���X�g�����N�̃m�[�h�̊p�x�Q���i�[����i�m�[�h�̊p�x = �\���_���Ώۃm�[�h���\���_���Ȃ��p�x�j
	// �p�x�̕��я��ɂ���āA�����R�[�h�̐��������`�F�b�N����
	std::map<std::vector<double>, CSPIllustLinkRow> linkAngleMap;
	for( auto iter = sameValsPair.first; iter != sameValsPair.second; ++iter )
	{
		std::vector<double> vecAngle;
		// �C���X�g�����N�Ɋւ��m�[�h�̊p�x��S�ċ��߂Ċi�[
		int nMaxNode =  m_isAseanSchema?global::illust_link::kMaxNodeNumber:illust_link::kMaxNodeNumber;
		for(int i = 1; i < ( nMaxNode - 1); ++i)
		{
			long node1 = iter->second->GetValue(m_isAseanSchema?global::illust_link::kNode[i]:illust_link::kNode[i], -1L);
			long node2 = iter->second->GetValue(m_isAseanSchema?global::illust_link::kNode[i+1]:illust_link::kNode[i+1], -1L);
			long node3 = iter->second->GetValue(m_isAseanSchema?global::illust_link::kNode[i+2]:illust_link::kNode[i+2], -1L);
			double angle = -1.0;
			// 3�_�̊p�x�����Ȃ��Ȃ�܂Ń��[�v
			if(!GetAngleOf3Nodes(node1, node2, node3, iter->second, angle))
				break;
			vecAngle.push_back(angle);
		}
		linkAngleMap.insert(std::make_pair(vecAngle, iter->second));
	}

	long directionC = m_isAseanSchema?global::illust_link::direction_code::kA:illust_link::direction_code::kA; // �����R�[�h�����Ɍ��Ă���
	for(auto& check : linkAngleMap)
	{
		// �����R�[�h�̐������������Ă��Ȃ�������G���[
		if(check.second->GetValue(m_isAseanSchema?global::illust_link::kDirectionCode:illust_link::kDirectionCode, -1L) != directionC)
		{
			CString err = uh::str_util::format(getMsg(message::eNotMatchedNodeQueueAndIllustName));
			IllustErrorInfo errInfo(5012, EErrorLevel::kWarning, err);
			WriteError(check.second, errInfo);
		}
		
		// �C���X�g�P����R�[�h���u�P����f�U�C���v�̏ꍇ�́A�����R�[�h���d�����āA�i�[����Ă���̂ŁA�����R�[�h�͂��̂܂�
		// �u�P����f�U�C���v�ȊO�̏ꍇ�͕����R�[�h�����̃R�[�h�l�ɂ���
		if(m_isAseanSchema?true:(check.second->GetValue(illust_link::kSingleBranchCode, -1L) != illust_link::single_branch_code::kSingleBranchDesign))
		{
			++directionC;
		}
	}
}

void CCheckIllustData::CheckRelDirguideExist( const std::pair<CHECKDIRECTIONCODEITER, CHECKDIRECTIONCODEITER>& sameValsPair, const std::set<long>& setNoDirguideLink )
{
	// �C���X�g��ʂ̓��[�v�̒��ł��ׂē����Ȃ̂ŁA�ŏ��̗v�f�̒l�����擾
	long lIllustC = sameValsPair.first->second->GetValue(m_isAseanSchema?global::illust_link::kIllustClass:illust_link::kIllustClass, -1L);

	// �m�[�h�P�ʂŏ������s��
	int nMaxNode =  m_isAseanSchema?global::illust_link::kMaxNodeNumber:illust_link::kMaxNodeNumber;
	for(int i = 1; i < ( nMaxNode - 1); ++i)
	{
		std::set<std::tuple<long, long, long>> checkNodeSet; // ����_�T���p
		std::set<std::pair<long, long>> checkBeforeBranchLink; // ����O�����N�`�F�b�N�p
		std::multimap<std::tuple<long, long, long>, CSPIllustLinkRow> checkIllustMap; // ���ʈē��`�F�b�N�p
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
		// ����O�ł����continue
		if(checkNodeSet.size() == 1)
			continue;

		// ����O�̃����N����v���Ȃ�
		// ROAD_NODE_01�ŃO���[�v������悤�ɂ������߁A�قڗ��Ȃ��͂��B
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

		// �X�L�[�}���uASEAN�����v�̏ꍇ�A�G���[���ځu5013�v�̔p�~
		if( m_isAseanSchema )	break;

		// �����ȍ~�͕��ʈē��������`�F�b�N
		// �Ώێ�ʈȊO�͏I��
		switch(lIllustC)
		{
		case illust_link::illust_class::kEntrance:
		case illust_link::illust_class::kExit:
			break;
		default:
			return;
		}

		// ����O�����N��UPDOWNCLASS_C���s���ȏꍇ�̓G���[
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

		std::map<double, long> checkLinkMap; // �L�[�F�p�x�A�l�F���o�����N��OID
		for(auto& illust : checkIllustMap)
		{
			double angle = -1.0;
			// �����N�����݂��Ȃ��P�[�X
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

		// 2����ȊO�̓`�F�b�N�ł��Ȃ��̂ŏI���i�R�[�h�l�̊֌W��j
		if(checkLinkMap.size() != 2)
		{
			CString err = uh::str_util::format(getMsg(message::iCannotDirguideCheck));
			AllWriteComment(err);
			return;
		}

		// ���ʈē��ŔL���R�[�h�̐������ł��낤�l���擾
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
		break; // ����n�_����������I��
	}
}

bool CCheckIllustData::GetRoadLinkTreatedAsNoDirguide(std::set<long>& setNoDirguideLink)
{
	using namespace inf_dirguide;
	using namespace guide_class;
	ITablePtr ipInfDirguideT;
	// INF_DIRGUIDE�e�[�u�����擾
	if( !util::OpenTable( m_isAseanSchema?global::inf_dirguide::kTableName:inf_dirguide::kTableName, m_ipRoadWorkspace, ipInfDirguideT ) )
	{
		CString err = uh::str_util::format( getMsg(message::iCannotOpen), m_isAseanSchema?global::inf_dirguide::kTableName:inf_dirguide::kTableName );
		AllWriteComment(err);
		return false;
	}

	std::vector<long> vecInfDirIDs;
	// �n���ʂ��u�L���i�����ԍ��j�v�u�L���i�����ԍ��j�v�݂̂̏ꍇ�͕��ʈē������Ƃ��Ĉ���
	// IN�ϊ����ɊԈ�����邽��
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

	std::vector<std::string> vecInfDir = uh::enumerate_elements(vecInfDirIDs); // IN��p��1000���P�ʂɕ�������

	for(const auto& infdir : vecInfDir)
	{
		// LQ_DIRGUIDE������
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
	// ����O�����N����n�܂�A����O�����N������ナ���N�ƂȂ�INF_DIRGUIDE�������Ă��邩���f����
	// ����O�����N����n�܂�INF_DIRGUIDE��ID������
	if( FAILED(m_ipLQDirguideT->Search(AheInitQueryFilter(NULL, CString(m_isAseanSchema?global::lq_dirguide::kInfID:kInfID), _T("%s = %ld AND %s = 1"),
		kLinkID, lBeforeLinkID, m_isAseanSchema?global::lq_dirguide::kSequence:kSequence), VARIANT_FALSE, &ipBeforeCur)) )
	{
		CString err = uh::str_util::format( getMsg(message::iFailedToSearch) );
		AllWriteComment(err);
		return -1;
	}
	// ������ナ���N���񃊃��N�Ɏ���INF_DIRGUIDE��ID������
	if( FAILED(m_ipLQDirguideT->Search(AheInitQueryFilter(NULL, CString(m_isAseanSchema?global::lq_dirguide::kInfID:kInfID), _T("%s = %ld AND %s = 2"),
		kLinkID, lLeftLinkID, kSequence), VARIANT_FALSE, &ipLeftCur)) )
	{
		CString err = uh::str_util::format( getMsg(message::iFailedToSearch) );
		AllWriteComment(err);
		return -1;
	}
	// �E����ナ���N���񃊃��N�Ɏ���INF_DIRGUIDE��ID������
	if( FAILED(m_ipLQDirguideT->Search(AheInitQueryFilter(NULL, CString(m_isAseanSchema?global::lq_dirguide::kInfID:kInfID), _T("%s = %ld AND %s = 2"),
		kLinkID, lRightLinkID, kSequence), VARIANT_FALSE, &ipRightCur)) )
	{
		CString err = uh::str_util::format( getMsg(message::iFailedToSearch) );
		AllWriteComment(err);
		return -1;
	}

	std::set<long> setInfIDOfBefore; // ����O�����N�Ɋւ��INF_DIRGUIDE��ID�Q
	_IRowPtr ipRow;
	while(S_OK == ipBeforeCur->NextRow(&ipRow) && ipRow)
	{
		setInfIDOfBefore.insert(AheGetAttribute(ipRow, m_isAseanSchema?global::lq_dirguide::kInfID:kInfID).lVal);
	}

	bool bLeft = false, bRight = false;
	// ���򍶕����̃C���X�g�����N�����ʈē����������f
	while(S_OK == ipLeftCur->NextRow(&ipRow) && ipRow)
	{
		if(setInfIDOfBefore.find(AheGetAttribute(ipRow, m_isAseanSchema?global::lq_dirguide::kInfID:kInfID).lVal) != setInfIDOfBefore.end())
		{
			bLeft = true;
			break;
		}
	}
	// ����E�����̃C���X�g�����N�����ʈē����������f
	while(S_OK == ipRightCur->NextRow(&ipRow) && ipRow)
	{
		if(setInfIDOfBefore.find(AheGetAttribute(ipRow, m_isAseanSchema?global::lq_dirguide::kInfID:kInfID).lVal) != setInfIDOfBefore.end())
		{
			bRight = true;
			break;
		}
	}

	// �n����13�A14�݂̂������ʈē��͕��ʖ��������Ƃ���
	// ����O�����N�����ʈē����������̃����N�̏ꍇ
	if(setNoDirguideLink.find(lBeforeLinkID) != setNoDirguideLink.end())
	{
		// ���򍶃����N�����ʈē����������̃����N
		if(setNoDirguideLink.find(lLeftLinkID) != setNoDirguideLink.end())
			bLeft = false;
		// ����E�����N�����ʈē����������̃����N
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

// ���H�m�[�h1��2��3�̊p�x�����߂�B�i��123�̊p�x�j
// ���m�ɂ͍\���_�����H�m�[�h2���\���_�̊p�x�����߂�
bool CCheckIllustData::GetAngleOf3Nodes( long node1, long node2, long node3, const CSPIllustLinkRow& spRow, double& angle )
{
	// �m�[�h1���m�[�h2���Ȃ������N�擾
	auto& firstLink = FindLink(node1, node2);
	if(firstLink == EndLink())
		return false;

	double firstAngle = -1.0;
	// �m�[�h2���\���_�̊p�x���~����
	if(!GetOutflowLinkAngle(firstLink->second, node2, firstAngle))
	{
		return false;
	}

	// �m�[�h2���m�[�h3���Ȃ������N�擾
	auto& secondLink = FindLink(node2, node3);
	if(secondLink == EndLink())
		return false;

	double secondAngle = -1.0;
	// �m�[�h2���\���_�̊p�x���~����
	if(!GetOutflowLinkAngle(secondLink->second, node2, secondAngle))
	{
		return false;
	}

	// �ǂ�������o���̊p�x���擾���Ă���̂ŁA��123�̊p�x�����܂�
	double diffAngle = fmod(secondAngle - firstAngle, 2*M_PI);

	// �}�C�i�X�̊p�x�����Ȃ��̂Ŕ��]
	while(diffAngle < 0)
		diffAngle += 2*M_PI;

	angle = 360 - (diffAngle * 180 / M_PI);
	return true;
}

// ���H�����N�̑Ώۃm�[�h���\���_�̊p�x���擾
bool CCheckIllustData::GetOutflowLinkAngle(IFeaturePtr ipRoadLink, long tgtNode, double& angle)
{
	CComVariant vaFrom = AheGetAttribute(ipRoadLink, m_isAseanSchema?global::road_link::kFromNodeID:road_link::kFromNodeID);
	_ASSERT(vaFrom.vt == VT_I4);
	IGeometryPtr ipGeo;

	// �`��擾�ł��Ȃ������I��
	if(FAILED(ipRoadLink->get_Shape(&ipGeo)))
	{
		AllWriteComment(getMsg(message::iFailedToGetShape));
		return false;
	}
	IPointCollectionPtr ipPointCol(ipGeo);

	long count = ipPointCol->GetPointCount();
	// ������
	if (vaFrom.lVal == tgtNode)
	{
		angle = GetAngle(ipPointCol->GetPoint(0), ipPointCol->GetPoint(1));
	}
	// �t����
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
	// �`�F�b�N���Ȃ���ʂ͏I��
	switch(lIllustC)
	{
	case illust_class::kAirport:
	case illust_class::kGrade:
	case illust_class::kRealCross:
		return;
	default:
		break;
	}

	bool existHighway = false; // HWY�m�[�h�����邩

	// �C���X�g�����N�����m�[�hID�Ń��[�v
	for(const auto& nodeID : vecNodeQueue)
	{
		// ���H�m�[�h�ɑ΂��Ă�HIGHWAY_NODE�̏�񂪊֘A�t������Ă��Ȃ������ꍇ
		if(m_mapRelNodeHwyNode.find(nodeID) == m_mapRelNodeHwyNode.end())
		{
			auto& itNode = FindNode(nodeID);
			// ���H�m�[�h�����݂��Ȃ��ꍇ
			if(itNode == EndNode())
				return;

			// �֘A�t�����s��
			if(!AssociateHighwayInfo(itNode->second))
				return;
		}

		// �m�[�hID�̈ʒu�ɂ���HIGHWAY_NODE�Q�Ń��[�v
		for(const auto& hwyID : m_mapRelNodeHwyNode[nodeID])
		{
			existHighway = true;
			if(!IsTargetHWYInfo(lIllustC, m_mapHighwayInfo[hwyID]))
				continue;
			if(CheckRelHighwayInfo(spRow, m_mapHighwayInfo[hwyID]))
			{
				// �G���[�ł͂Ȃ��̂ŏI��
				return;
			}
		}
	}
	switch(lIllustC)
	{
	case illust_class::kETC:
	case illust_class::kETC2:
	case illust_class::kSAPA:
		// HWY�m�[�h���Ȃ��ꍇ�̓G���[
		if(!existHighway)
		{
			// ��\�Ƃ���ROAD_NODE_2���G���[���b�Z�[�W�ɏo��
			CString err = uh::str_util::format(getMsg(message::eThereIsDifferenceInSecondNodeInfoAndHWYInfo), uh::str_util::ToString(spRow->GetValue(kNode2)));
			IllustErrorInfo errInfo(5010, EErrorLevel::kError, err);
			WriteError(spRow, errInfo);
		}
		break;
	default:
		break;
	}

	// �����܂ŗ��Ă�����A�����̂ƂꂽHWY�m�[�h��������Ȃ��Ƃ������ƂȂ̂ŃG���[
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
	// �m�[�h�̈ʒu�ɂ���HIGHWAY_NODE����Ԍ���
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

	// �擾�ł���HIGHWAY_NODE�̏��ƃm�[�hID���֘A�t����
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
		// �����i�[
		m_mapHighwayInfo.insert(std::make_pair(ipHighwayFeature->GetOID(), cHighwayInfo));
		vecHighwayID.push_back(ipHighwayFeature->GetOID());
	}

	// �܂��������Ȃ��悤�ɃL���b�V�����Ă���
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

// �G���[���O�ɃR�����g����������
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

// �G���[���O�ɃG���[���b�Z�[�W����������
void CCheckIllustData::WriteError( const sindy::CSPIllustLinkRow& spRow, const IllustErrorInfo& errInfo )
{
	long lIllustC = spRow->GetValue(m_isAseanSchema?global::illust_link::kIllustClass:illust_link::kIllustClass, -1L);
	long lCustomer = spRow->GetValue(m_isAseanSchema?global::illust_link::kCustomerCode:illust_link::kCustomerCode, -1L);
	auto& rule = m_RuleInfo.find(lIllustC, lCustomer);
	bool bExistRule = rule != m_RuleInfo.end(); // ���[�������邩
	CString strDate, strEditor;
	GetNewestEditInfo(spRow, strDate, strEditor);

	WriteError(spRow->GetTableName(), spRow->GetOID(), errInfo, lIllustC, lCustomer, (bExistRule ? spRow->GetIllustName(rule->mIllustNameRule) : _T("")), strDate, strEditor);
}

// �G���[���O�ɃG���[���b�Z�[�W����������
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


// �I�����O����������
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
