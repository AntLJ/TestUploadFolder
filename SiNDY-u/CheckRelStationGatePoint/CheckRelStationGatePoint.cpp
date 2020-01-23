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

// CheckRelStationGatePoint.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include <sindy/workspace.h>
#include <crd_cnv/crd_cnv.h>
#include "CheckRelStationGatePoint.h"
#include "StationGateFinder.h"

using boost::program_options::options_description;
using boost::program_options::value;

namespace po = boost::program_options;


CCheckRelStationGatePoint::CCheckRelStationGatePoint()
{
	m_isError = false;
}

CCheckRelStationGatePoint::~CCheckRelStationGatePoint()
{
}

bool CCheckRelStationGatePoint::arg_check(int argc, char* argv[])
{

	// �����ǂݍ���
	std::string meshList, log;

	po::options_description descPath("How to user");
	descPath.add_options()
		("input,i", po::value<std::string>(&m_shapeDir), "�`�F�b�N�Ώۃf�B���N�g��(�K�{)")
		("mesh,m", po::value<std::string>(&meshList), "���b�V�����X�g(�K�{)")
		("station_gate_point,s", po::value<std::string>(&m_stationGatePoint), "�S���w�o����POI�f�[�^(�K�{)")
		("parameter,p", po::value<std::string>(&m_jgd2000_pr), "���E���n�n�ϊ��p�����[�^�t�@�C��(�K�{)")
		("log,x", po::value<std::string>(&log), "���O(�K�{");

	if (argc != OPT_MAX )
	{
		std::cout << descPath << std::endl;
		return false;
	}

	po::variables_map vmPath;
	po::store(po::parse_command_line(argc, argv, descPath), vmPath);
	po::notify(vmPath);

	if( !loadList( meshList, m_meshList ) )			// ���b�V�����X�g�ǂݍ���
	{
		std::cerr << LOG_ERROR <<"\t" << meshList <<"\t���b�V�����X�g�̓ǂݍ��݂Ɏ��s" << std::endl;
		return false;
	}

	// ���O�̃`�F�b�N
	m_log.open(log, std::ios::out);
	if(m_log == false)
	{
		std::cerr << LOG_ERROR <<"\t" << OPT_LOG << "\t�I�v�V�������s���ł�" << std::endl;
		return false;
	}

	return true;
}

bool CCheckRelStationGatePoint::execute()
{

	REL_SGP relSgpMap;
	if( ! readRelSgp(m_shapeDir, relSgpMap))
	{
		printLog(LOG_ERROR, "���H�E���s�҃m�[�h-�w�o����POI�֘A�f�[�^�̎擾�Ɏ��s", m_shapeDir);
		return false;
	}

	CString sgpFile(m_stationGatePoint.c_str());
	StationGateFinder sgpFinder(sgpFile);
	if( ! sgpFinder.is_open())
	{
		printLog(LOG_ERROR, "�w�o����POI�f�[�^�̎擾�Ɏ��s", m_stationGatePoint);
		return false;
	}

	if( ! m_crd_cnv.JGD2000_RP_Init((char*)m_jgd2000_pr.c_str()))
	{
		printLog(LOG_ERROR, "���E���n�n�ϊ��p�p�����[�^�̎擾�Ɏ��s", m_jgd2000_pr);
		return false;
	}

	std::string	shapeDir;
	for(std::vector<std::string>::iterator itrMesh = m_meshList.begin() ; itrMesh != m_meshList.end() ; itrMesh++ )
	{
		shapeDir	= makeShapeDirName( itrMesh->c_str(), m_shapeDir);
		if( shapeDir.empty() )
			continue;

		std::cout << LOG_INFO << "\t" <<itrMesh->c_str() << "\t�`�F�b�N��..." << std::endl;

		if(! check_relation(relSgpMap, shapeDir, itrMesh->c_str(),sgpFinder))
			continue;

		std::cout << LOG_INFO << "\t" << itrMesh->c_str() << "\t�`�F�b�N�I��" << std::endl;
	}
	
	std::cout << LOG_INFO << "\t���H�E���s�҃m�[�h���C�� ID���݃`�F�b�N��..." << std::endl;

	if(m_checkId.size() == 0)
	{
		printLog(LOG_ERROR, "�w�o������m�[�h�̑����������H�E���s�҃m�[�h��0���ł�", m_shapeDir);
	}
	else
	{
		long temp=0;
		for(std::pair<long, long> relSgpRec: relSgpMap)
		{
			if(temp == 0 || temp != relSgpRec.first)
			{
				temp = relSgpRec.first;
				std::set<long>::iterator itr = m_checkId.find(temp);
				if( itr == m_checkId.end())
				{
					printLog(LOG_ERROR, "���H�E���s�҃m�[�h�ɑ��݂��Ȃ��m�[�hID�ł�", std::to_string(temp));
					continue;
				}
			}
		}
	}
	std::cout << LOG_INFO << "\t���H�E���s�҃m�[�h���C�� ID���݃`�F�b�N�I��" << std::endl;
	
	return true;
}
	
bool CCheckRelStationGatePoint::loadList(std::string file, std::vector<std::string> &VectorList)
{
	std::ifstream ifs(file);
	if ( !ifs )
		return false;

	copy(
		std::istream_iterator<std::string>(ifs),
		std::istream_iterator<std::string>(),
		back_inserter( VectorList )
	);

	return ifs.eof();
}

std::string CCheckRelStationGatePoint::makeShapeDirName ( std::string strMesh, std::string strPath)
{
	const int		iMeshCode = atoi( strMesh.c_str() );
	std::stringstream	ssSubStr;
	std::stringstream	ssDirPath;

	if(  100000 <= iMeshCode && iMeshCode < 1000000)
	{
		ssSubStr << iMeshCode/100 << "\\" << iMeshCode;
	}
	else
	{
		printLog(LOG_ERROR, "�s���ȃ��b�V���R�[�h�ł�",strMesh);
		return "";
	}

	ssDirPath << strPath.c_str() << "\\" << CT2CA(DATA_SET) << "\\" << ssSubStr.str();

	return ssDirPath.str();
}

bool CCheckRelStationGatePoint::readRelSgp(const std::string shapeDir,REL_SGP &relSgp)
{
	CString shapeConnect, shapeBaseDir(shapeDir.c_str());
	shapeConnect.Format(_T("%s\\%s"),shapeBaseDir, DATA_SET);

	IWorkspacePtr ipWorkspace = sindy::create_workspace(shapeConnect);
	if(! ipWorkspace)
	{
		printLog(LOG_ERROR, "�ڑ��Ɏ��s",shapeConnect);
		return false;
	}

	ITablePtr ipTable;
	if(FAILED(((IFeatureWorkspacePtr)ipWorkspace)->OpenTable(CComBSTR(shp05::schema::road::rel_sgp::kSuffix), &ipTable)))
	{
		printLog(LOG_ERROR, "�e�[�u���̃I�[�v���Ɏ��s", shp05::schema::road::rel_sgp::kSuffix);
		return false;
	}
	
	_ICursorPtr ipCursor;
	if(FAILED(ipTable->Search(NULL,VARIANT_FALSE, &ipCursor)))
	{
		printLog(LOG_ERROR, "�J�[�\���̎擾�Ɏ��s", shp05::schema::road::rel_sgp::kSuffix);
		return false;
	}

	long nid_field = -1 ,sid_field = -1;
	if(! getFieldIndex(ipTable, shp05::schema::road::rel_sgp::kNodeId, nid_field) || 
		! getFieldIndex(ipTable, shp05::schema::road::rel_sgp::kPoiId, sid_field))
		return false;

	_IRowPtr ipRow;
	while(ipCursor->NextRow(&ipRow) == S_OK)
	{
		CComVariant nodeId, serialId;
		if(FAILED(ipRow->get_Value(nid_field, &nodeId)) ||
			FAILED(ipRow->get_Value(sid_field, &serialId)))
		{
			printLog(LOG_ERROR, "�l�̎擾�Ɏ��s", shp05::schema::road::rel_sgp::kSuffix);
			break;
		}
		nodeId.ChangeType(VT_I4);
		serialId.ChangeType(VT_I4);

		relSgp.insert(REL_SGP::value_type(nodeId.lVal, serialId.lVal));
	}

	return true;
}


bool CCheckRelStationGatePoint::check_relation(REL_SGP relSgpMap, const std::string shapeDir, const std::string meshcode, StationGateFinder sgpFinder)
{

	CString dbConnect(shapeDir.c_str());
	IWorkspacePtr ipWorkspace = sindy::create_workspace(dbConnect);
	if(! ipWorkspace)
	{
		printLog(LOG_ERROR, "SHAPE�f�[�^�ւ̐ڑ��Ɏ��s", shapeDir);
		return false;
	}

	CString layerName, cs_meshcode(meshcode.c_str());
	layerName.Format(_T("%s_%s"), cs_meshcode, MWN_LAYER);

	IFeatureClassPtr ipFetureclass;
	if(FAILED(((IFeatureWorkspacePtr)ipWorkspace)->OpenFeatureClass(CComBSTR(layerName),&ipFetureclass)))
	{
		printLog(LOG_ERROR, "�ڑ��Ɏ��s", layerName);
		return false;
	}

	long oid_field = 0;
	if(! getFieldIndex(ipFetureclass, shp05::schema::kObjectId, oid_field))
		return false;

	IQueryFilterPtr ipQueryFilter(CLSID_QueryFilter);
	if(FAILED(ipQueryFilter->put_WhereClause(CComBSTR(SQL_RWN))))
	{
		printLog(LOG_ERROR, "SQL�̐ݒ�Ɏ��s", MWN_LAYER);
		return false;
	}

	IFeatureCursorPtr ipFeatureCursor;
	if(FAILED(ipFetureclass->Search(ipQueryFilter, VARIANT_FALSE, &ipFeatureCursor)))
	{
		printLog(LOG_ERROR, "�J�[�\���擾�Ɏ��s", MWN_LAYER);
		return false;
	}

	IFeaturePtr ipFeature;
	while(ipFeatureCursor->NextFeature(&ipFeature) == S_OK)
	{
		CComVariant objectId;
		if(FAILED(ipFeature->get_Value(oid_field, &objectId)))
		{
			printLog(LOG_ERROR, "�l�̎擾�Ɏ��s", shp05::schema::kObjectId);
			continue;
		}

		objectId.ChangeType(VT_I4);

		std::pair<REL_SGP::iterator, REL_SGP::iterator> relSgpItr = relSgpMap.equal_range(objectId.lVal);
		if(relSgpItr.first == relSgpItr.second)
		{
			printLog(LOG_ERROR, "�֘ATB�Ƀm�[�hID�����݂��܂���", std::to_string(objectId.lVal));
			continue;
		}

		m_checkId.insert(objectId.lVal);

		IGeometryPtr ipGeometry;
		if(FAILED(ipFeature->get_ShapeCopy(&ipGeometry)))
		{
			printLog(LOG_ERROR, "�`��̎擾�Ɏ��s", std::to_string(objectId.lVal));
			continue;
		}
		
		WKSPoint nodePoint;
		if(! getPoint(ipGeometry, nodePoint))
		{
			printLog(LOG_ERROR, "���W�̃Z�b�g�Ɏ��s", std::to_string(objectId.lVal));
			continue;
		}

		for(REL_SGP::iterator itr = relSgpItr.first ; itr != relSgpItr.second; ++itr)
		{
			long sid = itr->second;
			long dist = sgpFinder.find(sid, nodePoint.X, nodePoint.Y, &m_crd_cnv);
			if(dist < 0)
			{
				printLog(LOG_ERROR, "�w�o����POI�f�[�^��POIID�����݂��܂���", std::to_string(sid));
				continue;
			}
			else if(dist != 1)
			{
				printLog(LOG_INFO, "�m�[�h��POI�̋�����臒l���z���Ă��܂�",std::to_string(objectId.lVal)+ ", "+ std::to_string(sid)+ ", " + std::to_string(dist)+"m");
				continue;
			}
		}
	}

	return true;
}

bool CCheckRelStationGatePoint::getFieldIndex(const ITablePtr ipTable, CString fieldName, long &index)
{
	IFieldsPtr ipFields;
	if(FAILED(ipTable->get_Fields(&ipFields)))
	{
		printLog(LOG_ERROR, "�t�B�[���h�̎擾�Ɏ��s", fieldName);
		return false;
	}
	if(FAILED(ipFields->FindField(CComBSTR(fieldName), &index)))
	{
		printLog(LOG_ERROR, "�t�B�[���h�C���f�b�N�X�̎擾�Ɏ��s", fieldName);
		return false;
	}

	return true;
}


bool CCheckRelStationGatePoint::getPoint( const IGeometryPtr ipGeometry, WKSPoint& rWKSPoint )
{
	esriGeometryType aGeomType;
	if( FAILED(ipGeometry->get_GeometryType( &aGeomType ) ) ){
		return false;
	}

	if( esriGeometryPoint != aGeomType ){
		return false;
	}

	IPointPtr ipPoint( ipGeometry );
	if( FAILED( ipPoint->get_X( &(rWKSPoint.X) ) ) || FAILED( ipPoint->get_Y( &(rWKSPoint.Y) ) ) )
		return false;

	return true;
}

bool CCheckRelStationGatePoint::isError()
{
	return m_isError;
}

void CCheckRelStationGatePoint::printLog(std::string tag, std::string message1, std::string message2)
{
	if(tag == LOG_ERROR)
		m_isError = true;

	m_log << tag << "\t" << message1 << "\t" << message2 << std::endl;
}

void CCheckRelStationGatePoint::printLog(std::string tag, std::string message1, CString message2)
{
	if(tag == LOG_ERROR)
		m_isError = true;

	m_log << tag << "\t" << message1 << "\t" << CT2CA(message2) << std::endl;
}
