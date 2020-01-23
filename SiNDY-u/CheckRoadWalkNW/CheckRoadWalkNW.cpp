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
 * @brief	�g�p�@
 */
void CCheckRoadWalkNW::printUsage()
{
	cerr
		<< "�ySJ�������H�E���s��NW�f�[�^ �`�F�b�N�c�[��" << "�z\n"
		<< "(Usage) CheckRoadWalkNW.exe [option]\n"
		<< "�E�I�v�V����\n"
		<< "-l [InLayerName]				���̓��C����\n"
		<< "-m [meshlist]				���b�V�����X�g�w��(���̓��C����roadcode�w�莞�͏ȗ���)\n"
		<< "-i [InShapeFilePath]			�`�F�b�N�Ώ�05Shape�̊�_�p�X\n"
		<< "-db [DBConnection]			��r�Ώۂ̓��HDB���\n"
		<< "-walkdb [DBConnection]			��r�Ώۂ̕��s��DB���(���HDB�Ɠ���̏ꍇ�͏ȗ���)\n"
		<< "-refdb [DBConnection]			�Q�ƃ��b�V��DB���(���HDB�Ɠ���̏ꍇ�͏ȗ��B���̓��C����roadcode�w�莞���ȗ���)\n"
		<< "-schema [Schema]			���HDB�X�L�[�}(SDE or ���v���J����̕ϊ����ɐڑ����[�U�����X�L�[�}���̏ꍇ�w��)\n"
		<< "-walkschema [Schema]			���s��DB�X�L�[�}(SDE or ���v���J����̕ϊ����ɐڑ����[�U�����X�L�[�}���̏ꍇ�w��)\n"
		<< "-refschema [Schema]			�Q�ƃ��b�V��DB�X�L�[�}(SDE or ���v���J����̕ϊ����ɐڑ����[�U�����X�L�[�}���̏ꍇ�w��)\n"
		<< "-x [logfile]			���O�o�̓t�@�C���w��\n"
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
	m_ErrorLog << "#�J�n���� : " << m_st.wYear << "/" << m_st.wMonth << "/" << m_st.wDay << " " << m_st.wHour << ":" << m_st.wMinute << ":" << m_st.wSecond << endl;

	return true;
}

bool CCheckRoadWalkNW::checkOptions(int argc, char* argv[])
{
	//TODO:boost_program_options���g�p����悤�ɕύX����
	int		OptionCount[OPTION_MAX];		// �I�v�V�����d���J�E���g�p
	int		i = 0;
	CString temp;

	for( i = 0 ; i < OPTION_MAX ; ++i )
	{
		OptionCount[i] = 0;
	}

	for( i = 0 ; i < argc ; ++i )
	{
		if( strcmp( argv[i], "-m" ) == 0 )			// ���b�V�����X�g
		{
			if( !checkOptionArgument( argc, argv, i ) )
				return false;
			temp  = argv[++i];
			if( !loadList( temp, m_meshList ) )			// ���b�V�����X�g�ǂݍ���
			{
				std::cerr << TAG_ERROR << "\t���b�V�����X�g�̓ǂݍ��݂Ɏ��s�I" << endl;
				return false;
			}
			++OptionCount[0];
		}
		else if( strcmp( argv[i], "-i" ) == 0 )			// ���̓t�@�C��
		{
			if( !checkOptionArgument( argc, argv, i ) )
				return false;
			m_inputDirPath = argv[++i];
			++OptionCount[1];
		}
		else if( strcmp( argv[i], "-x" ) == 0 )			// ���O�t�@�C��
		{
			if( !checkOptionArgument( argc, argv, i ) )
				return false;
			++OptionCount[2];
			m_ErrorLog.open( argv[++i], std::ios::out );
			if( m_ErrorLog == false )
			{
				std::cerr << TAG_ERROR << "\t���O�o�̓t�@�C���̃I�[�v���Ɏ��s�I" << endl;
				return false;
			}
		}
		else if (strcmp( argv[i], "-db" ) == 0)			// ���HDB
		{
			if( !checkOptionArgument( argc, argv, i ) )
				return false;
			++OptionCount[3];
			m_roadDBConnection = argv[++i];
		}
		else if (strcmp( argv[i], "-walkdb" ) == 0)			// ���s��DB
		{
			if( !checkOptionArgument( argc, argv, i ) )
				return false;
			++OptionCount[4];
			m_walkDBConnection = argv[++i];
		}
		else if (strcmp( argv[i], "-refdb" ) == 0)			// �Q�ƃ��b�V��DB
		{
			if( !checkOptionArgument( argc, argv, i ) )
				return false;
			++OptionCount[5];
			m_meshDBConnection = argv[++i];
		}
		else if (strcmp( argv[i], "-schema" ) == 0)			// ���HDB�X�L�[�}
		{
			if( !checkOptionArgument( argc, argv, i ) )
				return false;
			++OptionCount[6];
			m_roadSchema = argv[++i];
		}
		else if (strcmp( argv[i], "-walkschema" ) == 0)			// ���s��DB�X�L�[�}
		{
			if( !checkOptionArgument( argc, argv, i ) )
				return false;
			++OptionCount[7];
			m_walkSchema = argv[++i];
		}
		else if (strcmp( argv[i], "-refschema" ) == 0)			// �Q�ƃ��b�V��DB�X�L�[�}
		{
			if( !checkOptionArgument( argc, argv, i ) )
				return false;
			++OptionCount[8];
			m_meshSchema = argv[++i];
		}
		else if (strcmp( argv[i], "-l" ) == 0)		//�`�F�b�N�Ώۃ��C��
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
				std::cerr << TAG_ERROR << "\t�`�F�b�N�Ώۂ̃��C���ł͂���܂���\t"<< m_layer << endl;
				return false;
			}
		}
		else
		{
			if( i != 0 )
			{
				std::cerr << TAG_ERROR << "\t�s���Ȉ��� (" << argv[i] << ") ���m�F���Ă��������B" << endl;
				return false;
			}
		}
	}

	// �I�v�V�����d���̃`�F�b�N
	for(i = 0; i < OPTION_MAX; ++i)
	{
		if(OptionCount[i] > 1) {
			cerr << TAG_ERROR << "\t�I�v�V�����̎w�肪�d�����Ă��܂�!!" << endl;
			printUsage();
			return false;
		}
		if(OptionCount[i] == 0) {
			if (i == 0 && ! m_layer.compare(CT2CA(shp05::schema::road::road_code::kTableName)))
				continue;

			if (4 <= i && i <= 8)
				continue;

			cerr << TAG_ERROR << "\t�I�v�V�������s�����Ă��܂��A�m�F���Ă�������!!" << endl;
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
		std::cerr << TAG_ERROR << "\t�I�v�V���� (" << argv[pos] << ") �ɑ����������s�����Ă��܂��A�m�F���Ă�������!!" << endl;
		printUsage();
		return false;
	}
	return true;
}

bool CCheckRoadWalkNW::execute()
{
	// OLE2T���̈ꕔ�}�N�����g�p����ꍇ�ɕK�v
	USES_CONVERSION;

	std::string errDb;
	CDatabaseController dbCtrl;
	if(! dbCtrl.init(m_roadDBConnection, m_walkDBConnection, m_meshDBConnection, m_roadSchema, m_walkSchema, m_meshSchema, m_layer, errDb))
	{
		printLog(TAG_ERROR, "DB�̐ڑ��Ɏ��s���܂���", errDb);
		return false;
	}

	if(! m_layer.compare(CT2CA(shp05::schema::road::road_code::kTableName)))
	{
		// road_code�̏ꍇ���[�v���񂷂��߃��b�V�����X�g���N���A���ă_�~�[���P���ݒ�
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

		std::cout << TAG_INFO << "\t" <<itrMesh->c_str() << "\t�`�F�b�N��..." << endl;

		if( !checkShape( itrMesh->c_str(), strShapeDirPath , dbCtrl) )
		{
			printLog(TAG_ERROR, itrMesh->c_str(), " �`�F�b�N���s") ;
			m_bIsError = true;
			return false;
		}

		std::cout << TAG_INFO << "\t" << itrMesh->c_str() << "\t�`�F�b�N�I��" << endl;
	}

	::GetLocalTime( &m_st );
	m_ErrorLog << "#�I������ : " << m_st.wYear << "/" << m_st.wMonth << "/" << m_st.wDay << " " << m_st.wHour << ":" << m_st.wMinute << ":" << m_st.wSecond << endl; 

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
			printLog(TAG_ERROR, "�s���ȃ��b�V���R�[�h�ł�",strMesh);
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
			printLog(TAG_ERROR, "DB����̃��b�V���̎擾�Ɏ��s",strMesh);
			return false;
		}
		if( ! m_layer.compare(CT2CA(shp05::schema::road::road_walk_node::kSuffix)))
		{
			if( !dbCtrl.setAreaBuffer(strMesh, CDatabaseController::ROAD_NODE) )
			{
				printLog(TAG_ERROR, "DB����̃��b�V���{�o�b�t�@�`��Ɋ܂܂�郌�R�[�h�̎擾�Ɏ��s",strMesh);
				return false;
			}
		}
	}
	else if(! m_layer.compare(CT2CA(shp05::schema::road::road_walk_nopassage_condition::kSuffix)) ||
			! m_layer.compare(CT2CA(shp05::schema::road::road_walk_oneway_condition::kSuffix)))
	{
		if( !dbCtrl.setAreaRecords(strMesh) )
		{
			printLog(TAG_ERROR, "DB�̎擾�Ɏ��s",strMesh);
			return false;
		}
	}
	else if(! m_layer.compare(CT2CA(shp05::schema::road::road_code::kTableName)))
	{
		if( !dbCtrl.setRoadcode() )
		{
			printLog(TAG_ERROR, "DB�̎擾�Ɏ��s",strMesh);
			return false;
		}
	}

	IWorkspacePtr ipWorkspace;
	CString dbConnect(strShapeDirPath.c_str());
	ipWorkspace = sindy::create_workspace(dbConnect);
	if(! ipWorkspace)
	{
		printLog(TAG_ERROR, "SHAPE�f�[�^�ւ̐ڑ��Ɏ��s", strShapeDirPath);
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
			printLog(TAG_ERROR, "�t�H�[�}�b�g�`�F�b�N�Ɏ��s���܂���",m_layer);
		}
		messageList = checkRWL.getMeessageList();
		
		IFeatureClassPtr ipFetureclass_rwn;
		CString layerName_rwn;
		layerName_rwn.Format(_T("%s_%s"), meshcode, shp05::schema::road::road_walk_node::kSuffix);
		if(FAILED(((IFeatureWorkspacePtr)ipWorkspace)->OpenFeatureClass(CComBSTR(layerName_rwn),&ipFetureclass_rwn)))
			return false;
		
		if(! checkExist((IFeatureClassPtr)ipTable, ipFetureclass_rwn, messageList))
		{
			printLog(TAG_ERROR, "���݃`�F�b�N�Ɏ��s���܂���", m_layer);
		}
	}
	else if(! m_layer.compare(CT2CA(shp05::schema::road::road_walk_node::kSuffix)))
	{
		CCheckRoadWalkNode chkRWN;
		if(! chkRWN.checkFormat((IFeatureClassPtr)ipTable, shapeFiledIndex, dbCtrl))
		{
			printLog(TAG_ERROR, "�t�H�[�}�b�g�`�F�b�N�Ɏ��s���܂���", m_layer);
		}
		messageList = chkRWN.getMeessageList();

		// �ϊ��ΏۊO�`�F�b�N
		if(! checkNoTarget(strMesh, (IFeatureClassPtr)ipTable, shapeFiledIndex, dbCtrl, messageList))
		{
			printLog(TAG_ERROR, "�ϊ��ΏۊO�`�F�b�N�Ɏ��s���܂���", m_layer);
		}
	}
	else if(! m_layer.compare(CT2CA(shp05::schema::road::road_walk_nopassage_condition::kSuffix)))
	{
		CCheckRoadWalkPassageCondition chkNPW;
		if(! chkNPW.checkFormat(ipTable, shapeFiledIndex, dbCtrl))
		{
			printLog(TAG_ERROR, "�t�H�[�}�b�g�`�F�b�N�Ɏ��s���܂���", m_layer);
		}
		messageList = chkNPW.getMeessageList();
	}
	else if(! m_layer.compare(CT2CA(shp05::schema::road::road_walk_oneway_condition::kSuffix)))
	{
		CCheckRoadWalkOnewayCondition chkOWW;
		if(! chkOWW.checkFormat(ipTable, shapeFiledIndex, dbCtrl))
		{
			printLog(TAG_ERROR, "�t�H�[�}�b�g�`�F�b�N�Ɏ��s���܂���", m_layer);
		}
		messageList = chkOWW.getMeessageList();
	}
	else if(! m_layer.compare(CT2CA(shp05::schema::road::road_code::kTableName)))
	{
		CCheckRoadCode chkRoadcode;
		if(! chkRoadcode.checkFormat(ipTable, shapeFiledIndex, dbCtrl))
		{
			printLog(TAG_ERROR, "�t�H�[�}�b�g�`�F�b�N�Ɏ��s���܂���", m_layer);
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
				errorMessage.Format(_T("%ld\t%s\t%s\t%ld"), rwlItr->first, checkFieldName, _T("�Ή�����m�[�hID�����݂��܂���"), nodeIDValue.lVal);
				messageList.push_back(errorMessage);
			}
			
		}
	}
	return true;
}


bool CCheckRoadWalkNW::checkNoTarget(const string& strMesh, const IFeatureClassPtr ipRWNFeatureClass, const FIELD_INDEX& RWNFiledIndex, CDatabaseController dbCtrl, std::vector<CString>& messageList)
{
	// �ϊ���DB:���s�҃����N(walk_link)
	MAP_FEATURE Features_walklink_db;
	FIELD_INDEX FiledIndex_walklink_db;
	if(! dbCtrl.getFeatures(strMesh, CDatabaseController::WALK_LINK, Features_walklink_db, FiledIndex_walklink_db))
		return false;

	// �ϊ���DB:���s�҃m�[�h(walk_node)
	MAP_FEATURE Features_walknode_db;
	FIELD_INDEX FiledIndex_walknode_db;
	if(! dbCtrl.getFeatures(strMesh, CDatabaseController::WALK_NODE, Features_walknode_db, FiledIndex_walknode_db))
		return false;

	// SHAPE:RWN
	MAP_FEATURE rwnFeatures_shape;
	if(! CDBUtils::createFeatureMap(ipRWNFeatureClass, RWNFiledIndex, NULL, rwnFeatures_shape))
		return false;

	// �ϊ���DB:���s�҃����N�̕��s�Ҏ�ʁiWALKCLASS_C�j=�u100:�l�ԋ��p�����N�v�̃m�[�h��OBJECTID���擾
	multiset<long> humanCarLinkList;
	if(! getHumanCarLinkList(Features_walklink_db, FiledIndex_walklink_db, humanCarLinkList))
		return false;

	// �ϊ���DB:���s�҃m�[�h�̃��R�[�h����
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
			// �ϊ���DB���s�҃m�[�h��NODECLASS_C = �u3:���HNW�ڑ��m�[�h�v�́A
			// �ϊ���DB���H�m�[�h�Əd�Ȃ��Ă��邽�߁A���H�m�[�h�������o�͑ΏۂƂȂ�A���s�҃m�[�h�͏o�͑ΏۊO�B
			bTarget = false;
		}
		else if( dbNodeClassValue.lVal == sindy::schema::walk_node::node_class::kOutline)
		{
			// ���s�҃m�[�h��NODECLASS_C = �u99:�}�s��m�[�h�v �����̃m�[�h�ɐڑ�������s�҃����N�Ɂu�l�ԋ��p�����N�v��1�{�ȏ㑶�݂���ꍇ�A
			// ���̃m�[�h�͕ϊ���DB���H�m�[�h�Əd�Ȃ��Ă��邽�߁A���H�m�[�h�������o�͑ΏۂƂȂ�A���s�҃m�[�h�͏o�͑ΏۊO�B
			if(humanCarLinkList.count(objectidDB) != 0 )
			{
				bTarget = false;
			}
		}

		if(! bTarget && rwnFeatures_shape.count(objectidSHP) != 0)
		{
			// ���s�҃m�[�h�o�͑ΏۊO��SHAPE�ɂ���
			CString errorMessage;
			errorMessage.Format(_T("%ld\t%s\t%s\t%ld"), objectidSHP, sindy::schema::kObjectID, _T("�o�͑ΏۊO���s�҃m�[�h���o�͂���Ă��܂�"), objectidSHP);
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

		// ���s�Ҏ�ʁiWALKCLASS_C�j=�u100:�l�ԋ��p�����N�v�� FROM_NODE_ID �y�� TO_NODE_ID�̃��X�g�擾
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

