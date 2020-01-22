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

#include "LogSys.h"
#include "common.h"
#include "SourceReader.h"
#include "BDNCooker.h"
#include "ShapeMaker.h"

#include <TDC/useful_headers/boost_program_options.h>
#include <TDC/useful_headers/tstring.h>


namespace po = boost::program_options;

/**
 *	@brief ���b�V���`��擾�֐�	
 *	@param mesh [in] �Ώۃ��b�V��
 *	@param ipCityMes [in] CityMesh�t�B�[�`���N���X
 *	@param ipMeshGeometry [out] �`��
 *	@return true ����
 *	@return false ���s
 */
bool get_mesh_geometry(long mesh, const IFeatureClassPtr& ipCityMesh, IGeometryPtr& ipMeshGeometry)
{
	CString errMsg;
	// where��쐬
	CString whereClause;
	whereClause.Format(_T("%s = %d"), sindy::schema::citymesh::kCityMeshCode, mesh);
	// filter�쐬	
	IQueryFilterPtr ipQueryFilter(CLSID_QueryFilter);
	if (FAILED(ipQueryFilter->put_WhereClause(CComBSTR(whereClause)))) {
		errMsg.Format(_T("�t�B���^�쐬�Ɏ��s���܂���"), whereClause);
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, sindy::schema::citymesh::kTableName);
		return false;
	}
	// cursor�擾
	IFeatureCursorPtr ipFeatureCursor;
	if (FAILED(ipCityMesh->Search(ipQueryFilter, VARIANT_FALSE, &ipFeatureCursor))) {
		errMsg.Format(_T("�J�[�\���擾�Ɏ��s���܂����F%s"), whereClause);
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, sindy::schema::citymesh::kTableName);
		return false;
	}
	// �t�B�[�`���擾
	IFeaturePtr ipFeature;
	if (ipFeatureCursor->NextFeature(&ipFeature) != S_OK || ipFeature == NULL) {
		errMsg.Format(_T("�Ώۃ��b�V���`�󂪑��݂��܂���:%d"), mesh);
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, sindy::schema::citymesh::kTableName);
		return false;
	}
	// �`��擾
	if (FAILED(ipFeature->get_ShapeCopy(&ipMeshGeometry))) {
		long oid;
		ipFeature->get_OID(&oid);
		errMsg.Format(_T("���b�V���`��̎擾�Ɏ��s���܂����F%d"), mesh);
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, sindy::schema::citymesh::kTableName, oid);
		return false;
	}
	return true;
}

/**
 *	@biref ���X�g�擾�֐�
 *	@param filePath [in] ���X�g�t�@�C���̃p�X	
 *	@parma list [out] �ǂݍ��񂾃��X�g
 *	@return true ����
 *	@return false ���s	
 */
bool get_list(LPCTSTR filePath, std::set<long>* list)
{
	// �t�@�C���I�[�v��
	std::ifstream ifs(filePath);
	if (! ifs.is_open()) {
		CString errMsg;
		errMsg.Format(_T("%s�̓ǂݍ��݂Ɏ��s���܂���"), filePath);
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg);
		return false;
	}
	// �ǂݍ���
	std::string temp;
	while (std::getline(ifs, temp)) {
		CString record = CA2CT(temp.c_str());
		record.Trim();
		if (! record.IsEmpty())
			list->insert(_ttol(record));
	}
	return true;
}

/**
 *	@brief ���X�g�擾�֐�	
 *	@param filePath [in] ���X�g�t�@�C���̃p�X
 *	@param list [out] �ǂݍ��񂾃��X�g
 *	@return true ����	
 *	@return false ���s
 */
bool get_list(LPCTSTR filePath, std::vector<long>* list)
{
	// �t�@�C���I�[�v��
	std::ifstream ifs(filePath);
	if (! ifs.is_open()) {
		CString errMsg;
		errMsg.Format(_T("%s�̓ǂݍ��݂Ɏ��s���܂���"), filePath);
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg);
		return false;
	}
	// �ǂݍ���
	std::string temp;
	while (std::getline(ifs, temp)) {
		CString record = CA2CT(temp.c_str());
		record.Trim();
		if (! record.IsEmpty())
			list->push_back(_ttol(record));
	}
	return true;

}

bool exec(const po::variables_map &vmContent, bool isTest)
{
	// �e��DB�ւ̐ڑ�
	IWorkspacePtr ipBaseWorkspace, ipMapWorkspace, ipPoiWorkspace, ipMeshWorkspace;
	if (! common_proc::GetWorkspace(vmContent[BASE_DB].as<uh::tstring>().c_str(), ipBaseWorkspace) ||
		! common_proc::GetWorkspace(vmContent[MAP_DB].as<uh::tstring>().c_str(), ipMapWorkspace) ||
		! common_proc::GetWorkspace(vmContent[MESH_DB].as<uh::tstring>().c_str(), ipMeshWorkspace))
		return false;
	// �e��e�[�u���I�[�v��
	IFeatureClassPtr ipBldNamePt, ipBld, ipCityMesh;
	if (! common_proc::GetTable(sindy::schema::sj::buildingname_point::kTableName, ipBaseWorkspace, ipBldNamePt) ||
		! common_proc::GetTable(sindy::schema::building::kTableName, ipMapWorkspace, ipBld) ||
		! common_proc::GetTable(sindy::schema::citymesh::kTableName, ipMeshWorkspace, ipCityMesh))
		return false;
	// �e��e�[�u���C���f�b�N�X�擾
	FieldIndexMap bldNamePtIndeces, bldIndeces;
	if (! common_proc::GetFieldIndeces(sindy::schema::sj::buildingname_point::kTableName, (ITablePtr)ipBldNamePt, &bldNamePtIndeces) ||
		! common_proc::GetFieldIndeces(sindy::schema::building::kTableName, (ITablePtr)ipBld, &bldIndeces))
		return false;
	// ���b�V�����X�g�Ǎ�
	std::set<long> meshList;
	if (! get_list(vmContent[MESH_LIST].as<uh::tstring>().c_str(), &meshList))
		return false;
	// �����ΏۊO������ʃ��X�g�ǂݍ���
	std::set<long> bldExcList;
	if (! get_list(vmContent[BLD_EXC_LIST].as<uh::tstring>().c_str(), &bldExcList))
		return false;
	// �o�̓f�B���N�g��+Citymap�擾 �ˁ@���݂��Ȃ��ꍇ�͍쐬
	CString outRootDir;
	outRootDir.Format(_T("%s\\Citymap\\"), vmContent[OUTPUT_DIR].as<uh::tstring>().c_str());
	if (! MakeSureDirectoryPathExists(CT2CA(outRootDir))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�o�̓f�B���N�g���쐬�Ɏ��s���܂���"));
		return false;
	}
	//�@���،`��o�͗p GeometryDef�擾(�o�͂̓|�C���g�Ȃ̂ŁA�����r�����̃|�C���g�̂��̂��擾)
	IGeometryDefPtr ipGeometryDef;
	if (! common_proc::GetGeometryDef(sindy::schema::sj::buildingname_point::kTableName, ipBldNamePt, ipGeometryDef))
		return false;
	// �����N���X��`
	SourceReader sourceReader(ipBldNamePt, ipBld, &bldNamePtIndeces, &bldIndeces, &bldExcList);
	BDNCooker bdnCooker(&bldNamePtIndeces);
	ShapeMaker shpMaker(outRootDir, isTest, ipGeometryDef);
	// �s�s�n�}���b�V���P�ʂł̏���	
	bool retval = true;
	CString procMsg;
	for (const auto& cityMesh : meshList)
	{
		procMsg.Format(_T("%d...\t"), cityMesh);
		LogSys::GetInstance().WriteProcLog(true, false, procMsg);
		// ���b�V���`��擾
		IGeometryPtr ipMeshGeometry;
		if (! get_mesh_geometry(cityMesh, ipCityMesh, ipMeshGeometry)) {
			retval = false;
			LogSys::GetInstance().WriteProcLog(true, false, _T("NG\n"));
			continue;
		}
		// �s���ȃf�[�^�����݂������`�F�b�N����ϐ�
		bool existWrongDataRead = true;
		// �f�[�^�擾
		IDFeatureMap bldFeatures;
		IDFeatureMap bldNamePtFeatures;
		IDFeatureMap bldNamePtIDs;
 		if (! sourceReader.readSource(ipMeshGeometry, &bldFeatures, &bldNamePtFeatures, &bldNamePtIDs, &existWrongDataRead)) {
			retval = false;
			LogSys::GetInstance().WriteProcLog(true, false, _T("NG\n"));
			continue;
		}
		// �s���ȃf�[�^�����݂������`�F�b�N����ϐ�
		bool existWrongDataCnv = true;
		// �f�[�^���H
		IDRecordMap cnvInfo;
		if (! bdnCooker.cookBDN(ipMeshGeometry, bldFeatures, bldNamePtFeatures, &bldNamePtIDs, &cnvInfo, &existWrongDataCnv)) {
			retval = false;
			LogSys::GetInstance().WriteProcLog(true, false, _T("NG\n"));
			continue;
		}
		// Shape�o��
		if (! shpMaker.makeShape(cnvInfo, cityMesh)) {
			retval = false;
			LogSys::GetInstance().WriteProcLog(true, false, _T("NG\n"));
			continue;
		}
		if (existWrongDataRead && existWrongDataCnv) {
			LogSys::GetInstance().WriteProcLog(true, false, _T("OK\n"));
		} else {
			retval = false;
			LogSys::GetInstance().WriteProcLog(true, false, _T("NG(�s���f�[�^���ϊ�)\n"));
		}
	}
	return retval;
}
