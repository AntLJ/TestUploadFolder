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

// �c�[����
const LPCTSTR TOOLNAME = _T("BldNameConverter");

// ����
const LPCSTR INI		= "ini";
const LPCSTR PROCLOG	= "proc_log";
const LPCSTR ERRLOG		= "err_log";
const LPCSTR TEST		= "test_mode";

// �ݒ�t�@�C������
const LPCSTR BASE_DB		= "BASE_DB";
const LPCSTR MAP_DB			= "MAP_DB";
const LPCSTR MESH_DB		= "MESH_DB";
const LPCSTR BLD_EXC_LIST	= "BLD_EXC_LIST";
const LPCSTR MESH_LIST		= "MESH_LIST";
const LPCSTR OUTPUT_DIR		= "OUTPUT_DIR";

/**
 * @brief �ܓx�o�x���
 */
struct LONLAT
{
	double e_lon;	//!< �o�x
	double e_lat;	//!< �ܓx

	bool operator < (const LONLAT& lonlat) const {
		bool retVal = false;
		if (e_lon != lonlat.e_lon) {
			if (e_lon < lonlat.e_lon)
				retVal = true;
		} else if (e_lat != lonlat.e_lat) {
			if (e_lat < lonlat.e_lat)
				retVal = true;
		}
		return retVal;
	}

	LONLAT() : e_lon(0.0), e_lat(0.0)
	{
	}
};

/**
 * @brief �����r�����̃����[�X�f�[�^���R�[�h���
 */
struct RECORDINFO
{
	long			e_bldID;		//!< �����|���S��ID
	CString			e_bldName;		//!< �����r������
	IGeometryPtr	e_ipGeometry;	//!< �����r�����̃|�C���g�E�W�I���g���i���ؗpShape�o�͗p�j

	RECORDINFO() : e_bldID(-1)
	{
	}

	RECORDINFO(long bldID, CString bldName, IGeometryPtr ipGeometry) :
		e_bldID(bldID), e_bldName(bldName), e_ipGeometry(ipGeometry)
	{
	}
};

/**
 * @brief �����|���S��ID���L�[�Ƃ��������[�X�f�[�^���R�[�h���̃}�b�v
 */
typedef std::map<long, RECORDINFO> IDRecordMap;

/**
 * @brief ID���L�[�Ƃ����t�B�[�`���̃}�b�v
 */
typedef std::map<long, IFeaturePtr> IDFeatureMap;

/**
 * @brief �t�B�[���h�����L�[�Ƃ����t�B�[���h�C���f�b�N�X�̃}�b�v
 */
typedef std::map<CString, long> FieldIndexMap;

// ���ʊ֐�
namespace common_proc
{
	/**
	 * @brief  ���[�N�X�y�[�X�擾�֐�
	 * @param  connectInfo [in] ���[�N�X�y�[�X�ւ̃p�X(DB��)
	 * @param  ipWorkspace [out] ���[�N�X�y�[�X	
	 * @retval true ����
	 * @retval false ���s	
	 */
	bool GetWorkspace(LPCTSTR connectInfo, IWorkspacePtr& ipWorkspace);

	/**
	 * @brief  �e�[�u���I�[�v���֐�
	 * @param  tableName [in] �e�[�u����
	 * @param  ipWorkspace [in] ���[�N�X�y�[�X
	 * @param  ipTable [out] �e�[�u��
	 * @retval ture ����
	 * @retval false ���s
	 */
	bool GetTable(LPCTSTR tableName, const IWorkspacePtr& ipWorkspace, IFeatureClassPtr& ipTable);

	/**
	 * @brief  GeometryDef�擾�֐�
	 * @param  featureClassName [in] �t�B�[�`���N���X��
	 * @param  ipFeatureClass [in] �t�B�[�`���N���X
	 * @param  ipGeoDef [out] GeometryDef
	 * @retval true ����
	 * @retval false ���s
	 */
	bool GetGeometryDef(LPCTSTR featureClassName, const IFeatureClassPtr& ipFeatureClass, IGeometryDefPtr &ipGeoDef);

	/**
	 * @brief  �t�B�[���h�C���f�b�N�X�擾�֐�
	 * @param  tableName [in] �e�[�u����
	 * @param  ipTable [in] �e�[�u��
	 * @param  indeces [out] �C���f�b�N�X���
	 * @retval true ����
	 * @retval false ���s
	 */
	bool GetFieldIndeces(LPCTSTR tableName, const ITablePtr& ipTable, FieldIndexMap* indeces);

	/**
	 * @brief  �t�B�[�`���J�[�\���擾�֐��i��Ԍ����j
	 * @param  tableName [in] �e�[�u����
	 * @param  ipFeatureClass [in] �t�B�[�`���N���X	
	 * @param  ipGeometry [in] ��ƂȂ�`��
	 * @param  spRel [in] �����[�V����
	 * @param  ipFeatureCursor [out] �t�B�[�`���J�[�\��
	 * @param  whereClause [in] Where��
	 * @retval true ����
	 * @retval false ���s
	 */
	bool GetRelFeatureCursor(LPCTSTR tableName, const IFeatureClassPtr& ipFeatureClass, const IGeometryPtr& ipGeometry, const esriSpatialRelEnum spRel, IFeatureCursorPtr& ipFeatureCursor, CString whereClause = _T(""));

	/**
	 * @brief  �t�B�[���h�l�擾�֐�
	 * @param  ipRow [in] IRowPtr
	 * @param  talbleName [in] �e�[�u����(�G���[�o�͗p)
	 * @param  fieldName [in] �t�B�[���h��
	 * @param  indeces [in] �t�B�[���h�C���f�b�N�X���
	 * @param  val [out]�@�擾�����l
	 * @retval true ����
	 * @retval false ���s
	 */
	bool GetValue(const _IRowPtr& ipRow, LPCTSTR talbleName, LPCTSTR fieldName, const FieldIndexMap& indeces, CComVariant* val);

	/**
	 * @brief  �I�u�W�F�N�g�̃G�N�X�e���g�̍ő�E�ŏ����W�擾�֐�
	 * @param  ipGeometry [in] �W�I���g��
	 * @param  minLonLat [out] �G�N�X�e���g�̍ŏ����W
	 * @param  maxLonLat [out] �G�N�X�e���g�̍ő���W
	 * @retval true ����	
	 * @retval false ���s
	 */
	bool GetMinMax(const IGeometryPtr ipGeometry, LONLAT* minLonLat, LONLAT* maxLonLat);

};
