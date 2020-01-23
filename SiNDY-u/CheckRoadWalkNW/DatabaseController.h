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

#include "stdafx.h"

#include "Properties.h"
#include "DBUtils.h"
#include <coord_converter.h>

#define ROAD_LINK_WHERE _T("roadclass_c < 400 AND alleymainte_c <> 9")
#define ROAD_NODE_WHERE _T("")
#define WALK_LINK_WHERE _T("")
#define WALK_NODE_WHERE _T("")
#define TIME_NOPASSAGE_WHERE _T("meshcode = %s")
#define WALK_NOPASSAGE_WHERE _T("meshcode = %s")
#define TIME_ONEWAY_WHERE _T("meshcode = %s")
#define WALK_ONEWAY_WHERE _T("meshcode = %s")
#define ROAD_CODE_LIST_WHERE _T("")
#define WALK_CODE_LIST_WHERE _T("")

const double MESH_BUFFER_METER = 500.0;

typedef std::map<CString, CAdapt<CComVariant>> MAP_VALUE;
typedef std::map<CString, MAP_VALUE> MAP_ROWS;
typedef std::set<MAP_VALUE> SET_ROWS;

static crd_cnv g_crdCnv;

class CDatabaseController
{
public:
	typedef enum layer
	{
		BASE_MESH,
		ROAD_LINK,
		ROAD_NODE,
		WALK_LINK,
		WALK_NODE,
		TIME_NOPASSAGE,
		WALK_NOPASSAGE,
		TIME_ONEWAY,
		WALK_ONEWAY,
		ROAD_CODE_LIST,
		WALK_CODE_LIST,
		NONE,
	} LAYER;

//private:
public:
	IWorkspacePtr m_ipRoadWorkspace;
	IWorkspacePtr m_ipWalkWorkspace;
	IWorkspacePtr m_ipRefWorkspace;
	long m_meshcodeIdx;
	IFeatureClassPtr m_ipMeshFeatureClass;

	struct databaseInfo
	{
		// �i�[����Ă��郌�C��
		LAYER eLayer;
		// �t�B�[�`���[�N���X
		CAdapt<ITablePtr> ipTable;
		// WHERE���SQL
		CString sql;
		// ObjectID���L�[�Ƃ����t�B�[�`���[�̃}�b�v(ROAD_LINK,ROAD_NODE,WALK_LINK,WALK_NODE�p)
		MAP_FEATURE features;
		// ���b�V���{�o�b�t�@�`��Ɋ܂܂��AObjectID���L�[�Ƃ����t�B�[�`���[�̃}�b�v(ROAD_NODE�p(WALK_NODE.ROAD_NODE_ID�̃`�F�b�N�Ŏg�p))
		MAP_FEATURE buffer_features;
		// �e�[�u���̃��R�[�h�Z�b�g(TIME_NOPASSAGE,WALK_NOPASSAGE,TIME_ONEWAY,WALK_ONEWAY�p)
		SET_ROWS rows;
		// ROADCODE���L�[�Ƃ������R�[�h�̃}�b�v(ROAD_CODE_LIST,WALK_CODE_LIST�p)
		MAP_ROWS roadcode_rows;
		// �t�B�[���h�C���f�b�N�X
		FIELD_INDEX fieldIndex;
	};
	std::vector<databaseInfo> m_dbList;

	LAYER tmp_layer;
	LAYER tmp_buffer_layer;
	MAP_FEATURE tmp_features;
	MAP_FEATURE tmp_buffer_features;
	SET_ROWS tmp_rows;
	MAP_ROWS tmp_roadcode_rows;
	FIELD_INDEX tmp_fieldIndex;

	CString m_roadSchema;
	CString m_walkSchema;
	CString m_refSchema;

public:

	CDatabaseController(void);
	~CDatabaseController(void);

	/**
	 * @brief	DB������
	 * @param	roadDbConnection	[in]	���HDB�ڑ����
	 * @param	walkDbConnection	[in]	���s��DB�ڑ����
	 * @param	refDbConnection	[in]	�Q�ƃ��b�V��DB�ڑ����
	 * @param	roadSchema	[in]	���HDB�X�L�[�}
	 * @param	walkSchema	[in]	���s��DB�X�L�[�}
	 * @param	refSchema	[in]	�Q�ƃ��b�V��DB�X�L�[�}
	 * @param	inputLayer	[in]	�`�F�b�N�Ώۃ��C��
	 * @param	errDb	[out]	�G���[���̃G���[DB�ڑ����
	 * @return	bool
	 */
	bool init(std::string roadDbConnection, std::string walkDbConnection, std::string refDbConnection, std::string roadSchema, std::string walkSchema, std::string refSchema, std::string inputLayer, std::string& errDb);

	/**
	 * @brief	�擾�Ώۂ̃��b�V�����̃t�B�[�`���ݒ�(ROAD_LINK,ROAD_NODE,WALK_LINK,WALK_NODE�p)
	 * @param	cMeshcode	[in]	���b�V���R�[�h
	 * @return	bool
	 */
	bool setArea(std::string meshcode);

	/**
	 * @brief	�擾�Ώۂ̃��b�V���{�o�b�t�@���̃t�B�[�`���ݒ�(ROAD_NODE�p(WALK_NODE.ROAD_NODE_ID�̃`�F�b�N�Ŏg�p))
	 * @param	cMeshcode	[in]	���b�V���R�[�h
	 * @param	layer	[in]	�擾�Ώۂ̃��C��
	 * @return	bool
	 */
	bool setAreaBuffer(std::string meshcode, LAYER layer);

	/**
	 * @brief	�t�B�[�`���[�擾
	 * @param	layer	[in]	�擾�Ώۂ̃��C��
	 * @param	objectId	[in]	�擾�Ώۂ̃I�u�W�F�N�gID
	 * @param	ipFeature	[out]	�t�B�[�`���[
	 * @return	bool
	 */
	bool getFeature(LAYER layer, const long objectId, IFeaturePtr& ipFeature);

	/**
	 * @brief	���b�V���{�o�b�t�@���̃t�B�[�`���[�擾
	 * @param	layer	[in]	�擾�Ώۂ̃��C��
	 * @param	objectId	[in]	�擾�Ώۂ̃I�u�W�F�N�gID
	 * @param	ipFeature	[out]	�t�B�[�`���[
	 * @return	bool
	 */
	bool getBufferFeature(LAYER layer, const long objectId, IFeaturePtr& ipFeature);

	/**
	 * @brief	�擾�Ώۂ̃��b�V���R�[�h�̃��R�[�h�ݒ�(TIME_NOPASSAGE,WALK_NOPASSAGE,TIME_ONEWAY,WALK_ONEWAY�p)
	 * @param	cMeshcode	[in]	���b�V���R�[�h
	 * @return	bool
	 */
	bool setAreaRecords(std::string meshcode);

	/**
	 * @brief	�擾�Ώۂ̃��R�[�h�ݒ�(ROAD_CODE_LIST,WALK_CODE_LIST�p)
	 * @return	bool
	 */
	bool setRoadcode(void);

	/**
	 * @brief	�t�B�[���h�C���f�b�N�X�擾
	 * @param	layer	[in]	�擾�Ώۂ̃��C��
	 * @param	index	[out]	�t�B�[���h�C���f�b�N�X
	 * @return	bool
	 */
	bool getFieldIndex(LAYER layer, FIELD_INDEX& index);

	/**
	 * @brief	�����擾
	 * @param	layer	[in]	�擾�Ώۂ̃��C��
	 * @param	count	[out]	����
	 * @return	bool
	 */
	bool getRecordCount(LAYER layer,long& count);
	void clear(void);

	/**
	 * @brief	���R�[�h����(TIME_NOPASSAGE,WALK_NOPASSAGE,TIME_ONEWAY,WALK_ONEWAY�p)
	 * @param	layer	[in]	�Ώۂ̃��C��
	 * @param	mapValue	[in]	�����Ώۂ̃��R�[�h�̒lmap
	 * @return	bool
	 */
	bool findRow(LAYER layer, MAP_VALUE& mapValue);

	/**
	 * @brief	���R�[�h����(ROAD_CODE_LIST,WALK_CODE_LIST�p)
	 * @param	layer	[in]	�Ώۂ̃��C��
	 * @param	key	[in]	�����Ώۂ̃L�[(���H�̏ꍇ:�H���R�[�h,�H����ʃR�[�h�A���s�҂̏ꍇ:�H���R�[�h)
	 * @param	mapValue	[out]	�������ʂ̃��R�[�h�̒lmap
	 * @return	bool
	 */
	bool findRoadCode(LAYER layer, CString key, MAP_VALUE& mapValue);

	/**
	 * @brief	ObjectID���L�[�Ƃ����t�B�[�`���[�̃}�b�v�擾(setArea()�Ŏ擾�ς݂ł���Ύ擾�ς̓��e��Ԃ��B���擾�ł���ΐV�K�Ɏ擾����)
	 * @param	cMeshcode	[in]	���b�V���R�[�h
	 * @param	layer	[in]	�擾�Ώۂ̃��C��
	 * @param	MapFeatures	[out]	�t�B�[�`���[�̃}�b�v
	 * @param	FiledIndex	[out]	�t�B�[���h�C���f�b�N�X
	 * @return	bool
	 */
	bool getFeatures(std::string cMeshcode, const LAYER layer, MAP_FEATURE& MapFeatures, FIELD_INDEX& FiledIndex);

private :

	/**
	 * @brief	DB���擾
	 * @param	ipWorkspace	[in]	���[�N�X�y�[�X
	 * @param	tableName	[in]	�e�[�u����
	 * @param	schema	[in]	�X�L�[�}
	 * @param	dbInfo	[out]	DB���
	 * @return	bool
	 */
	bool init_db(const IWorkspacePtr ipWorkspace , CString tableName, CString schema, databaseInfo& dbInfo );

	/**
	 * @brief	ObjectID���L�[�Ƃ����e�[�u���}�b�v�̐����i�W�I���g���w��j
	 * @param	meshGeometry	[in]	�擾�͈̓W�I���g��
	 * @param	dbInfo	[in]	�擾�Ώۂ�DB���
	 * @param	features	[out]	�쐬�����}�b�v
	 * @return	bool
	 */
	bool createFeatureMap(const IGeometryPtr meshGeometry, databaseInfo dbInfo, MAP_FEATURE& features);

	/**
	 * @brief	�t�B�[���h�̒l���擾���ă}�b�v�ɐݒ�
	 * @param	dbInfo	[in]	DB���
	 * @param	ipRow	[in]	���R�[�h
	 * @param	field	[in]	�擾����t�B�[���h��
	 * @param	bSTR	[in]	�擾�����l�𕶎���ŕێ����邩�ǂ���(CComVariant.vt=VT_BSTR)
	 *							�g�p�P�[�X1: ������̏ꍇ�ACComVariant�ł�NULL�̈�����2��ނ��邽�ߓ��ꂵ�����ꍇ
	 *							�ECComVariant.vt == VT_NULL				�����R�[�h�ǉ���̏������
	 *							�ECComVariant.vt == VT_BSTR, �l = ""	����U�l��ݒ��ɍ폜�������
	 *							�g�p�P�[�X2: VT_ENPTY�AVT_NULL���ǂ����NULL�Ƃ��ċ�ʂ������Ȃ����ߕ�����^�Ƃ��Ĉ��������ꍇ
	 * @param	mapValue	[out]	�擾�����l��ݒ肷��}�b�v
	 * @return	bool
	 */
	bool setValue(const databaseInfo& dbInfo, const _IRowPtr& ipRow, const CString& field, const bool bSTR, MAP_VALUE& mapValue);

	/**
	 * @brief	ObjectID���L�[�Ƃ����t�B�[�`���[�̃}�b�v�擾(�V�K�Ɏ擾)
	 * @param	cMeshcode	[in]	���b�V���R�[�h
	 * @param	layer	[in]	�擾�Ώۂ̃��C��
	 * @param	ipFeature	[out]	�t�B�[�`���[�̃}�b�v
	 * @param	filedIndex	[out]	�t�B�[���h�C���f�b�N�X
	 * @return	bool
	 */
	bool getNewFeatures(std::string cMeshcode, const LAYER layer, MAP_FEATURE& features, FIELD_INDEX& filedIndex);

	/**
	 * @brief	���b�V���̃t�B�[�`���[�擾
	 * @param	cMeshcode	[in]	���b�V���R�[�h
	 * @param	feasures	[out]	�擾�����t�B�[�`���[
	 * @return	bool
	 */
	bool getMesh(const std::string& cMeshcode, MAP_FEATURE& feasures);

	/**
	 * @brief	���[�g�������W�ϊ�
	 * @param	meter	[in]	���[�g��
	 * @param	meshGeometry	[in]	���b�V���`��
	 * @param	coordinate	[out]	�ϊ���̍��W
	 * @return	bool
	 */
	bool meter2Coordinate(double meter, const IGeometryPtr& meshGeometry, double& coordinate);

	/**
	 * @brief	�o�b�t�@�擾
	 * @param	meshGeometry	[in]	���b�V���`��
	 * @param	ipBuffer	[out]	���b�V���{�o�b�t�@�`��
	 * @param	bufMeter	[in]	�o�b�t�@(���[�g��)
	 * @return	bool
	 */
	bool getBuffer(const IGeometryPtr& meshGeometry, IGeometryPtr& ipBuffer, double bufMeter = MESH_BUFFER_METER);

};

