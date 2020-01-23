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

// CreateEtcLinkList.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include <fstream>
#include <boost/program_options.hpp>
#include <boost/foreach.hpp>
#include <arctl/coinitializer.h>
#include <sindy/workspace.h>
#include <sindy/schema.h>
#include <sstream>
#include <rns/format.h>
#include "crd_cnv.h"
#include "EtcLinkList.h"

namespace po = boost::program_options;
int iRET; //�Ԃ�l�p�O���[�o���ϐ�

/**
 *	@brief	�w�肳�ꂽRoad_Link�I�u�W�F�N�g�����݂���Q�����b�V���R�[�h��Ԃ�
 *	@note	�����N��̒��_�����݂��郁�b�V�����A�����N�̑��݂��郁�b�V���ƌ��Ȃ�
 *	@param	cpFeature	[in]	�Ώ�Road_Link�I�u�W�F�N�g
 *	@return	�Ώ�Road_Link�I�u�W�F�N�g�����݂���Q�����b�V���R�[�h
 **/
long getMeshCode( IFeature* cpFeature )
{
	// �Q�_�����ԃ����N��̒��S�_�̃��b�V���R�[�h�𒲂ׂ�
	IPolylinePtr ipPolyline(cpFeature->GetShapeCopy());
	IPointPtr ipPoint(CLSID_Point);
	ipPolyline->QueryPoint( esriNoExtension, 0.5, VARIANT_TRUE, ipPoint );
	
	double aMidPointLon = ipPoint->GetX();
	double aMidPointLat = ipPoint->GetY();

	crd_cnv aCrd_cnv;
	int aMidX, aMidY;	// ���K�����W
	int aMeshCode;
	aCrd_cnv.LLtoMesh( aMidPointLon, aMidPointLat, 2, &aMeshCode, &aMidX, &aMidY, 0);

	return aMeshCode;
}

/**
 * @brief	�o��
 * @param	os	[in]	�o�̓X�g���[��
 * @param	crEtcLinkList	[in]	�o�͑Ώۃ��X�g
 **/
void write(std::ostream& os, EtcLinkList& crEtcLinkList)
{
	using namespace std;
	std::string fd = "\t";

	BOOST_FOREACH (EtcLink aEtcLink, crEtcLinkList)
	{
		os << aEtcLink.eType << fd;
		os << aEtcLink.eMeshCode << fd;

		BOOST_FOREACH (long aNodeID, aEtcLink.eNodeList) {
			os << aNodeID << fd;
		}

		os << aEtcLink.eFromNodeID << fd;
		os << aEtcLink.eToNodeID << endl;
	}
}

/**
 * @brief	ETC�����N���X�g�̍쐬
 * @param	cpFeatureWorkspace	[in]	�Ώۃ��[�N�X�y�[�X
 * @param	crEtcLinkList	[out]	ETC�����N���X�g�i�[�o�b�t�@�ւ̎Q��
 * @note	etc_link.txt�̃t�H�[�}�b�g�ł́A�u���_�m�[�h��v��񋓂��邱�Ƃɂ��A
 *			�A�����������N���P�̃��R�[�h�ɂ܂Ƃ߂邱�Ƃ��ł��邪�A�{�v���O�����ł͖��Ή��ł���B
 *			���_�m�[�h���i�[����悤�Ȏ������s���Ă��Ȃ����߁A�n�_�m�[�h�ƏI�_�m�[�h�݂̂̃��R�[�h�����쐬����Ȃ��B
 *			����́A�����N�Q�̎n�I�_��񋓂�����A�ǂ̂悤�ɘA�������`�F�b�N���Ē��_�m�[�h����쐬�����R�[�h�����炷���A
 *			�Ƃ����A���S���Y�������߂��Ȃ��������߂ł���B
 *			�t�H�[�}�b�g��͒��_�m�[�h�񂪑S���Ȃ��f�[�^�ł����͂Ȃ����߁A�ۗ��Ƃ��Ă���B
 **/
void makeLinkList(IFeatureWorkspace* cpFeatureWorkspace, EtcLinkList& crEtcLinkList)
{
	using namespace sindy::schema;
#ifdef	_DEBUG
	std::cout << "Road_Link�ǂݍ���" << std::endl;
#endif

	IFeatureClassPtr ipFeatureClass = cpFeatureWorkspace->_OpenFeatureClass(road_link::kTableName);
	IQueryFilterPtr ipQueryFilter(CLSID_QueryFilter);
	
	ipQueryFilter->WhereClause =
		(rns::tformat(_T("%1% IN(%2%,%3%)"))
			% road_link::kUpDownClass 
			% road_link::updown_class::kEtcRamp 
			% road_link::updown_class::kEtcAccess
		).str().c_str();

	ipQueryFilter->SubFields =
		(rns::tformat(_T("%1%,%2%,%3%,%4%"))
			% road_link::kUpDownClass
			% road_link::kFromNodeID
			% road_link::kToNodeID
			% ipFeatureClass->ShapeFieldName
		).str().c_str();

#ifdef	_DEBUG
	_bstr_t aWhereClause = ipQueryFilter->WhereClause;
	_bstr_t aSubFields = ipQueryFilter->SubFields;
#endif

	IFeatureCursorPtr ipCursor = ipFeatureClass->_Search(ipQueryFilter, FALSE);

#ifdef	_DEBUG
	std::cout << "�l�̎擾" << std::endl;
#endif

	IFeaturePtr ipFeature;
	while (ipCursor->NextFeature(&ipFeature) == S_OK)
	{
		EtcLink aEtcLink;

		long aUpDownClass		= ipFeature->GetValue( ipFeature->Fields->_FindField(road_link::kUpDownClass));
		aEtcLink.eFromNodeID	= ipFeature->GetValue( ipFeature->Fields->_FindField(road_link::kFromNodeID));
		aEtcLink.eToNodeID		= ipFeature->GetValue( ipFeature->Fields->_FindField(road_link::kToNodeID));
		aEtcLink.eMeshCode		= getMeshCode(ipFeature);

		switch (aUpDownClass)
		{
		case road_link::updown_class::kEtcRamp:
			aEtcLink.eType = EtcLink::type_ramp;
			break;
		case road_link::updown_class::kEtcAccess:
			aEtcLink.eType = EtcLink::type_access;
			break;
		default:
			ATLASSERT(0);
		}

		crEtcLinkList.insert(aEtcLink);
	}
}


/**
 * @param	vm	[in]	�R�}���h���C�������Ǘ��I�u�W�F�N�g
 **/
void process(po::variables_map& vm)
{
	// �o�̓t�@�C���X�g���[��������
	std::string aOutputFilePath = vm["output_dir"].as<std::string>() + "/" + "etc_link.txt";
	std::ofstream aFout(aOutputFilePath.c_str(), std::ios::binary);		// ���s�R�[�hLF�ŏo�͂��邽�߁A�o�C�i�����[�h�ŊJ��
	if (! aFout)
	{
		throw std::runtime_error("�o�̓t�@�C���I�[�v���G���[");
	}

	IFeatureWorkspacePtr pFeatureWorkspace = INamePtr(sindy::create_workspace_name(
		CA2T(vm["road_database"].as<std::string>().c_str())))->_Open();

	EtcLinkList aEtcLinkList;

	makeLinkList(pFeatureWorkspace, aEtcLinkList);

	write(aFout, aEtcLinkList);
}

/**
 * @brief	�G���g���|�C���g
 **/
int _tmain(int argc, _TCHAR* argv[])
{
	iRET = 0;
	bool aIsSuccess = true;

	try
	{
		const arctl::coinitializer aCoInitializer;	// Arc9.3.1�Ή�����try�X�R�[�v���Ɉړ�

		po::options_description desc("How to use");
		desc.add_options()
			("road_database",	po::value<std::string>(), "DB�w�蕶����")
			("output_dir",		po::value<std::string>(), "�o�̓f�B���N�g��");

		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);

		if (vm.count("road_database") == 0 ||
			vm.count("output_dir") == 0)
		{
			std::cerr << desc << std::endl;
			iRET=1;
			return iRET;
		}

		process(vm);
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		aIsSuccess = false;
		iRET=2;
	}
	catch (_com_error& e)
	{
		std::cerr << e << std::endl;
		aIsSuccess = false;
		iRET=2;
	}

	if (! aIsSuccess) 
	{ 
		std::cerr << "�ُ�I��" << std::endl;
	}

	
	return iRET;
}

