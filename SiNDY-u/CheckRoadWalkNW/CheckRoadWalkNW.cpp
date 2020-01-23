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

#include "stdafx.h"
#include "CheckRoadWalkNW.h"
#include <sindy/workspace.h>

/**
 * @brief	使用法
 */
void CCheckRoadWalkNW::printUsage()
{
	cerr
		<< "【SJ向け道路・歩行者NWデータ チェックツール" << "】\n"
		<< "(Usage) CheckRoadWalkNW.exe [option]\n"
		<< "・オプション\n"
		<< "-l [InLayerName]				入力レイヤ名\n"
		<< "-m [meshlist]				メッシュリスト指定(入力レイヤにroadcode指定時は省略可)\n"
		<< "-i [InShapeFilePath]			チェック対象05Shapeの基点パス\n"
		<< "-db [DBConnection]			比較対象の道路DB情報\n"
		<< "-walkdb [DBConnection]			比較対象の歩行者DB情報(道路DBと同一の場合は省略可)\n"
		<< "-refdb [DBConnection]			参照メッシュDB情報(道路DBと同一の場合は省略可。入力レイヤにroadcode指定時も省略可)\n"
		<< "-schema [Schema]			道路DBスキーマ(SDE or レプリカからの変換時に接続ユーザ名≠スキーマ名の場合指定)\n"
		<< "-walkschema [Schema]			歩行者DBスキーマ(SDE or レプリカからの変換時に接続ユーザ名≠スキーマ名の場合指定)\n"
		<< "-refschema [Schema]			参照メッシュDBスキーマ(SDE or レプリカからの変換時に接続ユーザ名≠スキーマ名の場合指定)\n"
		<< "-x [logfile]			ログ出力ファイル指定\n"
		<< endl;
}


bool CCheckRoadWalkNW::init(int argc, char* argv[])
{
	
	if(!checkOptions(argc, argv)){
		return false;
	}

	CVersion cVer;
	::GetLocalTime( &m_st );

	m_ErrorLog << "#" << CT2CA(cVer.GetOriginalFilename()) << " FILEVERSION " <<CT2CA(cVer.GetFileVersion()) << " PRODUCTVERSION " << CT2CA(cVer.GetProductVersion()) << endl;
	m_ErrorLog << "#開始時間 : " << m_st.wYear << "/" << m_st.wMonth << "/" << m_st.wDay << " " << m_st.wHour << ":" << m_st.wMinute << ":" << m_st.wSecond << endl;

	return true;
}

bool CCheckRoadWalkNW::checkOptions(int argc, char* argv[])
{
	//TODO:boost_program_optionsを使用するように変更する
	int		OptionCount[OPTION_MAX];		// オプション重複カウント用
	int		i = 0;
	CString temp;

	for( i = 0 ; i < OPTION_MAX ; ++i )
	{
		OptionCount[i] = 0;
	}

	for( i = 0 ; i < argc ; ++i )
	{
		if( strcmp( argv[i], "-m" ) == 0 )			// メッシュリスト
		{
			if( !checkOptionArgument( argc, argv, i ) )
				return false;
			temp  = argv[++i];
			if( !loadList( temp, m_meshList ) )			// メッシュリスト読み込み
			{
				std::cerr << TAG_ERROR << "\tメッシュリストの読み込みに失敗！" << endl;
				return false;
			}
			++OptionCount[0];
		}
		else if( strcmp( argv[i], "-i" ) == 0 )			// 入力ファイル
		{
			if( !checkOptionArgument( argc, argv, i ) )
				return false;
			m_inputDirPath = argv[++i];
			++OptionCount[1];
		}
		else if( strcmp( argv[i], "-x" ) == 0 )			// ログファイル
		{
			if( !checkOptionArgument( argc, argv, i ) )
				return false;
			++OptionCount[2];
			m_ErrorLog.open( argv[++i], std::ios::out );
			if( m_ErrorLog == false )
			{
				std::cerr << TAG_ERROR << "\tログ出力ファイルのオープンに失敗！" << endl;
				return false;
			}
		}
		else if (strcmp( argv[i], "-db" ) == 0)			// 道路DB
		{
			if( !checkOptionArgument( argc, argv, i ) )
				return false;
			++OptionCount[3];
			m_roadDBConnection = argv[++i];
		}
		else if (strcmp( argv[i], "-walkdb" ) == 0)			// 歩行者DB
		{
			if( !checkOptionArgument( argc, argv, i ) )
				return false;
			++OptionCount[4];
			m_walkDBConnection = argv[++i];
		}
		else if (strcmp( argv[i], "-refdb" ) == 0)			// 参照メッシュDB
		{
			if( !checkOptionArgument( argc, argv, i ) )
				return false;
			++OptionCount[5];
			m_meshDBConnection = argv[++i];
		}
		else if (strcmp( argv[i], "-schema" ) == 0)			// 道路DBスキーマ
		{
			if( !checkOptionArgument( argc, argv, i ) )
				return false;
			++OptionCount[6];
			m_roadSchema = argv[++i];
		}
		else if (strcmp( argv[i], "-walkschema" ) == 0)			// 歩行者DBスキーマ
		{
			if( !checkOptionArgument( argc, argv, i ) )
				return false;
			++OptionCount[7];
			m_walkSchema = argv[++i];
		}
		else if (strcmp( argv[i], "-refschema" ) == 0)			// 参照メッシュDBスキーマ
		{
			if( !checkOptionArgument( argc, argv, i ) )
				return false;
			++OptionCount[8];
			m_meshSchema = argv[++i];
		}
		else if (strcmp( argv[i], "-l" ) == 0)		//チェック対象レイヤ
		{
			if( !checkOptionArgument( argc, argv, i ) )
				return false;
			++OptionCount[9];
			m_layer = argv[++i];
			if(m_layer.compare(CT2CA(shp05::schema::road::road_walk_link::kSuffix)) &&
				m_layer.compare(CT2CA(shp05::schema::road::road_walk_node::kSuffix)) &&
				m_layer.compare(CT2CA(shp05::schema::road::road_walk_nopassage_condition::kSuffix)) &&
				m_layer.compare(CT2CA(shp05::schema::road::road_walk_oneway_condition::kSuffix)) &&
				m_layer.compare(CT2CA(shp05::schema::road::road_code::kTableName)))
			{
				std::cerr << TAG_ERROR << "\tチェック対象のレイヤではありません\t"<< m_layer << endl;
				return false;
			}
		}
		else
		{
			if( i != 0 )
			{
				std::cerr << TAG_ERROR << "\t不正な引数 (" << argv[i] << ") を確認してください。" << endl;
				return false;
			}
		}
	}

	// オプション重複のチェック
	for(i = 0; i < OPTION_MAX; ++i)
	{
		if(OptionCount[i] > 1) {
			cerr << TAG_ERROR << "\tオプションの指定が重複しています!!" << endl;
			printUsage();
			return false;
		}
		if(OptionCount[i] == 0) {
			if (i == 0 && ! m_layer.compare(CT2CA(shp05::schema::road::road_code::kTableName)))
				continue;

			if (4 <= i && i <= 8)
				continue;

			cerr << TAG_ERROR << "\tオプションが不足しています、確認してください!!" << endl;
			printUsage();
			return false;
		}
	}

	if (m_walkDBConnection.empty())
		m_walkDBConnection = m_roadDBConnection;

	if (m_meshDBConnection.empty())
		m_meshDBConnection = m_roadDBConnection;

	return true;

}

bool CCheckRoadWalkNW::checkOptionArgument(int argc, char* argv[], int pos)
{
	if( argc <= pos+1 || argv[pos+1][0] == '-' )
	{
		std::cerr << TAG_ERROR << "\tオプション (" << argv[pos] << ") に続く引数が不足しています、確認してください!!" << endl;
		printUsage();
		return false;
	}
	return true;
}

bool CCheckRoadWalkNW::execute()
{
	// OLE2T等の一部マクロを使用する場合に必要
	USES_CONVERSION;

	std::string errDb;
	CDatabaseController dbCtrl;
	if(! dbCtrl.init(m_roadDBConnection, m_walkDBConnection, m_meshDBConnection, m_roadSchema, m_walkSchema, m_meshSchema, m_layer, errDb))
	{
		printLog(TAG_ERROR, "DBの接続に失敗しました", errDb);
		return false;
	}

	if(! m_layer.compare(CT2CA(shp05::schema::road::road_code::kTableName)))
	{
		// road_codeの場合ループを回すためメッシュリストをクリアしてダミーを１件設定
		m_meshList.clear();
		m_meshList.push_back("");
	}

	std::string	strShapeDirPath;
	for(std::vector<std::string>::iterator itrMesh = m_meshList.begin() ; itrMesh != m_meshList.end() ; itrMesh++ )
	{
		strShapeDirPath	= makeShapeDirName( itrMesh->c_str(), m_inputDirPath);
		if( strShapeDirPath.empty() )
		{
			m_bIsError = true;
			continue;
		}

		std::cout << TAG_INFO << "\t" <<itrMesh->c_str() << "\tチェック中..." << endl;

		if( !checkShape( itrMesh->c_str(), strShapeDirPath , dbCtrl) )
		{
			printLog(TAG_ERROR, itrMesh->c_str(), " チェック失敗") ;
			m_bIsError = true;
			return false;
		}

		std::cout << TAG_INFO << "\t" << itrMesh->c_str() << "\tチェック終了" << endl;
	}

	::GetLocalTime( &m_st );
	m_ErrorLog << "#終了時間 : " << m_st.wYear << "/" << m_st.wMonth << "/" << m_st.wDay << " " << m_st.wHour << ":" << m_st.wMinute << ":" << m_st.wSecond << endl; 

	m_ErrorLog.close();

	return true;

}
bool CCheckRoadWalkNW::isError(){
	return m_bIsError;
}

bool	CCheckRoadWalkNW::loadList(LPCTSTR lpszFile, std::vector<std::string> &VectorList)
{
	ifstream ifs( static_cast<CT2CW>(lpszFile) );
	if ( !ifs )
		return false;

	copy(
		istream_iterator<string>(ifs),
		istream_iterator<string>(),
		back_inserter( VectorList )
	);

	return ifs.eof();
}

string	CCheckRoadWalkNW::makeShapeDirName ( string strMesh,	string strPath)
{
	const int		iMeshCode = atoi( strMesh.c_str() );
	stringstream	ssSubStr;
	stringstream	ssDirPath;
	const string	sDataSet = "Road";

	if( !strMesh.empty() )
	{
		if(  100000 <= iMeshCode && iMeshCode < 1000000)
		{
			ssSubStr << iMeshCode/100 << "\\" << iMeshCode;
		}
		else
		{
			printLog(TAG_ERROR, "不正なメッシュコードです",strMesh);
			return "";
		}

		ssDirPath << strPath.c_str() << "\\" << sDataSet.c_str() << "\\" << ssSubStr.str();
	}
	else
		ssDirPath << strPath.c_str() << "\\" << sDataSet.c_str();

	return ssDirPath.str();
}

bool CCheckRoadWalkNW::checkShape( string strMesh, string strShapeDirPath , CDatabaseController dbCtrl)
{

	if( ! m_layer.compare(CT2CA(shp05::schema::road::road_walk_link::kSuffix)) ||
		! m_layer.compare(CT2CA(shp05::schema::road::road_walk_node::kSuffix)))
	{
		if( !dbCtrl.setArea(strMesh) )
		{
			printLog(TAG_ERROR, "DBからのメッシュの取得に失敗",strMesh);
			return false;
		}
		if( ! m_layer.compare(CT2CA(shp05::schema::road::road_walk_node::kSuffix)))
		{
			if( !dbCtrl.setAreaBuffer(strMesh, CDatabaseController::ROAD_NODE) )
			{
				printLog(TAG_ERROR, "DBからのメッシュ＋バッファ形状に含まれるレコードの取得に失敗",strMesh);
				return false;
			}
		}
	}
	else if(! m_layer.compare(CT2CA(shp05::schema::road::road_walk_nopassage_condition::kSuffix)) ||
			! m_layer.compare(CT2CA(shp05::schema::road::road_walk_oneway_condition::kSuffix)))
	{
		if( !dbCtrl.setAreaRecords(strMesh) )
		{
			printLog(TAG_ERROR, "DBの取得に失敗",strMesh);
			return false;
		}
	}
	else if(! m_layer.compare(CT2CA(shp05::schema::road::road_code::kTableName)))
	{
		if( !dbCtrl.setRoadcode() )
		{
			printLog(TAG_ERROR, "DBの取得に失敗",strMesh);
			return false;
		}
	}

	IWorkspacePtr ipWorkspace;
	CString dbConnect(strShapeDirPath.c_str());
	ipWorkspace = sindy::create_workspace(dbConnect);
	if(! ipWorkspace)
	{
		printLog(TAG_ERROR, "SHAPEデータへの接続に失敗", strShapeDirPath);
		return false;
	}

	CString layerName, meshcode(strMesh.c_str()), inputLayer(m_layer.c_str());
	if(m_layer.compare(CT2CA(shp05::schema::road::road_code::kTableName)))
		layerName.Format(_T("%s_%s"), meshcode, inputLayer);
	else
		layerName.Format(_T("%s"), inputLayer);

	ITablePtr ipTable;
	if(FAILED(((IFeatureWorkspacePtr)ipWorkspace)->OpenTable(CComBSTR(layerName),&ipTable)))
		return false;

	FIELD_INDEX shapeFiledIndex;
	if(! CDBUtils::createFieldIndex(ipTable, shapeFiledIndex))
		return false;

	std::vector<CString> messageList;
	if(! m_layer.compare(CT2CA(shp05::schema::road::road_walk_link::kSuffix)))
	{
		CCheckRoadWalkLink checkRWL;
		if(! checkRWL.checkFormat((IFeatureClassPtr)ipTable, shapeFiledIndex, dbCtrl))
		{
			printLog(TAG_ERROR, "フォーマットチェックに失敗しました",m_layer);
		}
		messageList = checkRWL.getMeessageList();
		
		IFeatureClassPtr ipFetureclass_rwn;
		CString layerName_rwn;
		layerName_rwn.Format(_T("%s_%s"), meshcode, shp05::schema::road::road_walk_node::kSuffix);
		if(FAILED(((IFeatureWorkspacePtr)ipWorkspace)->OpenFeatureClass(CComBSTR(layerName_rwn),&ipFetureclass_rwn)))
			return false;
		
		if(! checkExist((IFeatureClassPtr)ipTable, ipFetureclass_rwn, messageList))
		{
			printLog(TAG_ERROR, "存在チェックに失敗しました", m_layer);
		}
	}
	else if(! m_layer.compare(CT2CA(shp05::schema::road::road_walk_node::kSuffix)))
	{
		CCheckRoadWalkNode chkRWN;
		if(! chkRWN.checkFormat((IFeatureClassPtr)ipTable, shapeFiledIndex, dbCtrl))
		{
			printLog(TAG_ERROR, "フォーマットチェックに失敗しました", m_layer);
		}
		messageList = chkRWN.getMeessageList();

		// 変換対象外チェック
		if(! checkNoTarget(strMesh, (IFeatureClassPtr)ipTable, shapeFiledIndex, dbCtrl, messageList))
		{
			printLog(TAG_ERROR, "変換対象外チェックに失敗しました", m_layer);
		}
	}
	else if(! m_layer.compare(CT2CA(shp05::schema::road::road_walk_nopassage_condition::kSuffix)))
	{
		CCheckRoadWalkPassageCondition chkNPW;
		if(! chkNPW.checkFormat(ipTable, shapeFiledIndex, dbCtrl))
		{
			printLog(TAG_ERROR, "フォーマットチェックに失敗しました", m_layer);
		}
		messageList = chkNPW.getMeessageList();
	}
	else if(! m_layer.compare(CT2CA(shp05::schema::road::road_walk_oneway_condition::kSuffix)))
	{
		CCheckRoadWalkOnewayCondition chkOWW;
		if(! chkOWW.checkFormat(ipTable, shapeFiledIndex, dbCtrl))
		{
			printLog(TAG_ERROR, "フォーマットチェックに失敗しました", m_layer);
		}
		messageList = chkOWW.getMeessageList();
	}
	else if(! m_layer.compare(CT2CA(shp05::schema::road::road_code::kTableName)))
	{
		CCheckRoadCode chkRoadcode;
		if(! chkRoadcode.checkFormat(ipTable, shapeFiledIndex, dbCtrl))
		{
			printLog(TAG_ERROR, "フォーマットチェックに失敗しました", m_layer);
		}
		messageList = chkRoadcode.getMeessageList();
	}

	if( messageList.size() > 0)
	{
		m_bIsError = true;
		printLog(TAG_ERROR, messageList);
	}
	return true;
}


bool CCheckRoadWalkNW::checkExist(const IFeatureClassPtr ipRWLFeatureClass,const IFeatureClassPtr ipRWNFeatureClass, std::vector<CString>& messageList)
{
	FIELD_INDEX rwlFieldIndex, rwnFieldIndex;
	if(! CDBUtils::createFieldIndex(ipRWLFeatureClass, rwlFieldIndex))
		return false;
	if(! CDBUtils::createFieldIndex(ipRWNFeatureClass, rwnFieldIndex))
		return false;

	MAP_FEATURE rwlFeatures, rwnFeatures;
	if(! CDBUtils::createFeatureMap(ipRWLFeatureClass, rwlFieldIndex, NULL, rwlFeatures))
		return false;
	if(! CDBUtils::createFeatureMap(ipRWNFeatureClass, rwnFieldIndex, NULL, rwnFeatures))
		return false;

	vector<CString> checkFields;
	checkFields.push_back(shp05::schema::road::road_walk_link::kFromNodeId);
	checkFields.push_back(shp05::schema::road::road_walk_link::kToNodeId);

	for(MAP_FEATURE::iterator rwlItr = rwlFeatures.begin(); rwlItr != rwlFeatures.end(); ++rwlItr)
	{
		IFeaturePtr ipFeature = rwlItr->second;
		for(CString checkFieldName: checkFields)
		{
			CComVariant nodeIDValue;
			if(FAILED(ipFeature->get_Value(rwlFieldIndex.find(checkFieldName)->second, &nodeIDValue)))
				return false;

			nodeIDValue.ChangeType(VT_I4);
			if(rwnFeatures.count(nodeIDValue.lVal) == 0 )
			{
				CString errorMessage;
				errorMessage.Format(_T("%ld\t%s\t%s\t%ld"), rwlItr->first, checkFieldName, _T("対応するノードIDが存在しません"), nodeIDValue.lVal);
				messageList.push_back(errorMessage);
			}
			
		}
	}
	return true;
}


bool CCheckRoadWalkNW::checkNoTarget(const string& strMesh, const IFeatureClassPtr ipRWNFeatureClass, const FIELD_INDEX& RWNFiledIndex, CDatabaseController dbCtrl, std::vector<CString>& messageList)
{
	// 変換元DB:歩行者リンク(walk_link)
	MAP_FEATURE Features_walklink_db;
	FIELD_INDEX FiledIndex_walklink_db;
	if(! dbCtrl.getFeatures(strMesh, CDatabaseController::WALK_LINK, Features_walklink_db, FiledIndex_walklink_db))
		return false;

	// 変換元DB:歩行者ノード(walk_node)
	MAP_FEATURE Features_walknode_db;
	FIELD_INDEX FiledIndex_walknode_db;
	if(! dbCtrl.getFeatures(strMesh, CDatabaseController::WALK_NODE, Features_walknode_db, FiledIndex_walknode_db))
		return false;

	// SHAPE:RWN
	MAP_FEATURE rwnFeatures_shape;
	if(! CDBUtils::createFeatureMap(ipRWNFeatureClass, RWNFiledIndex, NULL, rwnFeatures_shape))
		return false;

	// 変換元DB:歩行者リンクの歩行者種別（WALKCLASS_C）=「100:人車共用リンク」のノードのOBJECTIDを取得
	multiset<long> humanCarLinkList;
	if(! getHumanCarLinkList(Features_walklink_db, FiledIndex_walklink_db, humanCarLinkList))
		return false;

	// 変換元DB:歩行者ノードのレコード検索
	for(MAP_FEATURE::iterator rwnItr = Features_walknode_db.begin(); rwnItr != Features_walknode_db.end(); ++rwnItr)
	{
		IFeaturePtr ipFeature = rwnItr->second;

		CComVariant objectIDValue;
		if( FAILED(ipFeature->get_Value(FiledIndex_walknode_db.find(sindy::schema::kObjectID)->second, &objectIDValue)))
			return false;

		objectIDValue.ChangeType(VT_I4);
		long objectidDB = objectIDValue.lVal;
		long objectidSHP = objectIDValue.lVal + WALK_OBJECTID;

		CComVariant dbNodeClassValue;
		if( FAILED(ipFeature->get_Value(FiledIndex_walknode_db.find(sindy::schema::walk_node::kNodeClass)->second, &dbNodeClassValue)))
			return false;

		dbNodeClassValue.ChangeType(VT_I4);

		bool bTarget = true;
		if( dbNodeClassValue.lVal == sindy::schema::walk_node::node_class::kRoadNW)
		{
			// 変換元DB歩行者ノードのNODECLASS_C = 「3:道路NW接続ノード」は、
			// 変換元DB道路ノードと重なっているため、道路ノードだけが出力対象となり、歩行者ノードは出力対象外。
			bTarget = false;
		}
		else if( dbNodeClassValue.lVal == sindy::schema::walk_node::node_class::kOutline)
		{
			// 歩行者ノードのNODECLASS_C = 「99:図郭上ノード」 且つこのノードに接続する歩行者リンクに「人車共用リンク」が1本以上存在する場合、
			// このノードは変換元DB道路ノードと重なっているため、道路ノードだけが出力対象となり、歩行者ノードは出力対象外。
			if(humanCarLinkList.count(objectidDB) != 0 )
			{
				bTarget = false;
			}
		}

		if(! bTarget && rwnFeatures_shape.count(objectidSHP) != 0)
		{
			// 歩行者ノード出力対象外がSHAPEにある
			CString errorMessage;
			errorMessage.Format(_T("%ld\t%s\t%s\t%ld"), objectidSHP, sindy::schema::kObjectID, _T("出力対象外歩行者ノードが出力されています"), objectidSHP);
			messageList.push_back(errorMessage);
		}
	}
	return true;
}


bool CCheckRoadWalkNW::getHumanCarLinkList(const MAP_FEATURE& rwlFeatures, const FIELD_INDEX& rwlFieldIndex, multiset<long>& nodeList)
{
	vector<CString> checkFields;
	checkFields.push_back(sindy::schema::walk_link::kFromNodeID);
	checkFields.push_back(sindy::schema::walk_link::kToNodeID);

	for(MAP_FEATURE::const_iterator rwlItr = rwlFeatures.begin(); rwlItr != rwlFeatures.end(); ++rwlItr)
	{
		IFeaturePtr ipFeature = rwlItr->second;

		// 歩行者種別（WALKCLASS_C）=「100:人車共用リンク」の FROM_NODE_ID 及び TO_NODE_IDのリスト取得
		CComVariant walkClassValue;
		if(FAILED(ipFeature->get_Value(rwlFieldIndex.find(sindy::schema::walk_link::kWalkClass)->second, &walkClassValue)))
			return false;
		walkClassValue.ChangeType(VT_I4);

		if(walkClassValue.lVal != sindy::schema::walk_link::walk_class::kWalkableRoad)
			continue;

		for(CString checkFieldName: checkFields)
		{
			CComVariant nodeIDValue;
			if(FAILED(ipFeature->get_Value(rwlFieldIndex.find(checkFieldName)->second, &nodeIDValue)))
				return false;

			nodeIDValue.ChangeType(VT_I4);
			nodeList.insert(nodeIDValue.lVal);
		}
	}
	return true;
}


void CCheckRoadWalkNW::printLog(std::string tag, std::vector<CString> messageList){
	
	for(CString message: messageList)
	{
		m_ErrorLog << tag << "\t" << CT2CA(message) << endl;
	}
}

void CCheckRoadWalkNW::printLog(std::string tag, std::string message1, std::string message2)
{
	m_ErrorLog << tag << "\t" << message1 << "\t" << message2 << endl;;
}

