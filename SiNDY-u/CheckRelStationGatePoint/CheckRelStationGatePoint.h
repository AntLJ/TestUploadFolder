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

#include <WinLib/VersionInfo.h>
#include <TDC\useful_headers\boost_program_options.h>
#include <shapelib/DBFHandle.h>

#include "Properties.h"
#include "StationGateFinder.h"

#define OPT_INPUT_DIR "input_dir,i"
#define OPT_MESH_LIST "mesh, m"
#define OPT_SGP_FILE "sgp_file, s"
#define OPT_JGD2000_PR_FILE "jgd2000_parameter_file, p"
#define OPT_LOG "log, x"

#define OPT_MAX 11

typedef std::multimap<long, long> REL_SGP;

class CCheckRelStationGatePoint
{
public:
	CCheckRelStationGatePoint();
	~CCheckRelStationGatePoint();

private:
	std::string m_shapeDir;
	std::string m_stationGatePoint;
	std::string m_jgd2000_pr;
	std::vector<std::string>	m_meshList;

	std::ofstream m_log;

	std::set<long> m_checkId;

	crd_cnv m_crd_cnv;

	bool m_isError;

public:
	/**
	 *@brief	�������`�F�b�N
	 *@param	argc	[in]	������(����)
	 *@param	argv	[in]	������(�l)
	 *@return	bool
	 */
	bool arg_check(int argc, char* argv[]);

	/**
	 *@brief	�`�F�b�N���s
	 *@return	bool
	 */
	bool execute();

	/**
	 *@brief	�G���[�̑��ݗL��
	 *@return	bool
	 */
	bool isError();

protected:

	/**
	 *@brief	���b�V�����X�g�̓ǂݍ���
	 *@param	file	[in]	���b�V�����X�g�t�@�C���p�X
	 *@param	argv	[out]	���b�V���R�[�h�̃��X�g
	 *@return	bool
	 */
	bool loadList(std::string file, std::vector<std::string> &VectorList);

	/**
	 *@brief	SHAPE�f�[�^�̃p�X�쐬
	 *@param	strMesh	[in]	���b�V���R�[�h
	 *@param	strPath	[in]	SHAPE�f�[�^�̊�p�X
	 *@return	SHAPE�f�[�^�ւ̃p�X
	 */
	std::string makeShapeDirName(std::string strMesh, std::string strPath);

	/**
	 *@brief	���H�E���s�҃m�[�h-�w�o����POI�֘A�e�[�u���̓ǂݍ���
	 *@param	shapeDir	[in]	SHAPE�f�[�^�̊�p�X
	 *@param	relSgp	[out]	���H�E���s�҃m�[�h-�w�o����POI�֘A�e�[�u��
	 *@return	bool
	 */
	bool readRelSgp(std::string shapeDir, REL_SGP &relSgp);

	/**
	 *@brief	�֌W�`�F�b�N
	 *@param	relSgp	[in]	���H�E���s�҃m�[�h-�w�o����POI�֘A�e�[�u��
	 *@param	shapeDir	[in]	SHAPE�f�[�^�̊�p�X
	 *@param	meshcode	[in]	�`�F�b�N�Ώۂ̃��b�V���R�[�h
	 *@param	sgpFinder	[in]	�w�o����POI�f�[�^�����N���X
	 *@return	bool
	 */	
	bool check_relation(REL_SGP relSgpMap,const std::string shapeDir, const std::string meshcode, StationGateFinder sgpFinder);

	/**
	 *@breif	�t�B�[���h�C���f�b�N�X�̎擾
	 *@param	ipTable	[in]	�擾���e�[�u��
	 *@param	fieldName	[in]	�t�B�[���h��
	 *@param	index	[out]	�C���f�b�N�X
	 *@return	bool
	 */
	bool getFieldIndex(const ITablePtr ipTable, CString fieldName, long &index);

	/**
	 *@brief	�|�C���g�擾
	 *@param	ipGeometry	[in]	�擾�Ώۂ�Geometry
	 *@param	rWKSPoint	[out]	���W���Z�b�g���ꂽWKSPoint
	 *@return	bool
	 */	
	bool getPoint( const IGeometryPtr ipGeometry, WKSPoint& rWKSPoint );
	
	void printLog(std::string tag, std::string message1, std::string message2);
	void printLog(std::string tag, std::string message1, CString message2);
};
