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

#include <TDC/sindylib_core/FieldMap.h>
#include <sindy/workspace/name.h>
#include <sindy/schema.h>

/**
 * @brief �f�[�^�x�[�X
 */
enum database
{
	road,
	walk,
	station,
	poi,
	underground_area,
	mesh,
};

/**
 * @brief �e�[�u��
 * @warinig �擪�̎��ʎq�̒l��0�ȊO�ɂ��邩�A������table_size�̈ʒu��ς����
 * OpenTables()�̏��������s����̂ŁA�ύX���Ȃ����ƁB
 */
enum table
{
	c_road_link = 0,
	c_road_node,
	c_segment_attr,
	c_walk_link,
	c_walk_node,
	c_height_node,
	c_link_relation,
	c_walk_code_list,
	c_walk_nopassage,
	c_walk_oneway,
	c_walk_type,
	c_station_gate,
	c_poi_point_org,
	c_underground_area,
	c_base_mesh,
	c_city_mesh,
	c_scramble_area,
	c_pedxing,
	table_size
};

/**
 * @class	CDatabaseControl
 * @brief	DB�ڑ��Ǘ��N���X
 */
class CDatabaseControl
{
public:

	CDatabaseControl(void);
	virtual ~CDatabaseControl(void){}

	/**
	 * @brief ������
	 * @param  roadDB    [in] ���HDB�ڑ�������
	 * @param  walkDB    [in] ���s��DB�ڑ�������
	 * @param  stationDB [in] �wDB�ڑ�������
	 * @param  poiDB     [in] POIDB�ڑ�������
	 * @param  underDB   [in] �n���XDB�ڑ�������
	 * @param  meshDB    [in] ���b�V��DB�ڑ�������
	 * @retval �󕶎� ����
	 * @retval �󕶎��ȊO �ڑ��Ɏ��s����DB��
	 */
	CString Init(
		const CString& roadDB,
		const CString& walkDB,
		const CString& stationDB,
		const CString& poiDB,
		const CString& underDB,
		const CString& meshDB);

	/**
	 * @brief	�e�[�u���Q�̃I�[�v��
	 * @retval	�󕶎�	����
	 * @retval	�󕶎��ȊO	�I�[�v���Ɏ��s�����e�[�u����
	 */
	CString OpenTables();

	/**
	 * @brief	���[�N�X�y�[�X�̎擾
	 * @param	value	[in]	�擾�Ώۂ�Enum�l
	 * @param	ipWorkspace	[out]	���[�N�X�y�[�X
	 * @retval	true	����
	 * @retval	false ���s
	 */
	bool GetWorkspace(const database value, IWorkspacePtr& ipWorkspace);

	/**
	 * @brief	�e�[�u���̎擾
	 * @param	value	[in]	�擾�Ώۂ�Enum�l
	 * @param	ipTable	[out]	�e�[�u��
	 * @retval	true	����
	 * @retval	false	���s
	 */
	bool GetTable(const table value, ITablePtr& ipTable);

	/**
	 * @brief	�t�B�[���h�}�b�v�̎擾
	 * @param	value	[in]	�擾�Ώۂ�Enum�l
	 * @param	fieldMap	[out]	�t�B�[���h�}�b�v
	 * @retval	true	����
	 * @retval	false	���s
	 */
	bool GetFieldMap(const table value, sindy::CFieldMap& fieldMap);

	/**
	 * @brief �f�[�^�^�C�v�̒ǉ�
	 * @param [in] dataType �f�[�^�^�C�v	 
	 */
	void setDataType(sindy::schema::walk_type::type_class::ECode dataType);

private:
	/**
	 * @brief	���[�N�X�y�[�X�I�[�v��
	 * @param	name	[in]	�ڑ���
	 * @param	ipWorkspace	[out]	���[�N�X�y�[�X
	 * @retval	true	����
	 * @retval	false	���s
	 */
	bool OpenWorkspace( const CString& name, IWorkspacePtr& ipWorkspace );

	/**
	 * @brief	�e�[�u�������ݒ�
	 * @param	workspaceValue	[in]	���[�N�X�y�[�X
	 * @param	tableValue	[in]	�I�[�v���Ώۂ̃e�[�u��
	 * @retval	true	����
	 * @retval	false	���s
	 */
	bool InitialaizeTable( table tableValue);

	/**
	 * @brief	�e�[�u���I�[�v��
	 * @param	name	[in]	�t�B�[�`���N���X��
	 * @param	ipWorkspace	[in]	�I�[�v�����邽�߂̃��[�N�X�y�[�X
	 * @param	ipTable	[out] �擾����e�[�u��
	 * @retval	true	����
	 * @retval	false	���s
	 */
	bool OpenTable( const CString& name, const IWorkspacePtr& ipWorkspace, ITablePtr& ipTable );

	/**
	 * @brief	�t�B�[���h�}�b�v�쐬
	 * @param	ipTable	[in]	�쐬�Ώۂ̃e�[�u��
	 * @param	fieldMap	[out]	�擾����t�B�[���h�}�b�v
	 * @retval	true	����
	 * @retval	false	���s
	 */
	bool CreateFiledMap( const ITablePtr& ipTable, sindy::CFieldMap& fieldMap );

	/**
	 * @brief	�������Ɏw�肳�ꂽEnum�l�̕�����擾
	 * @param	value	[in]	�擾�Ώۂ�Enum�l
	 * @return	������	�Ώۂ��Ȃ������ꍇ""(�󕶎�)
	 */
	CString GetEnumString(const table value);

private:
	/**
	 * @brief ���[�N�X�y�[�X��Map
	 */
	std::map<database, IWorkspacePtr> m_workspaceMap;
	/**
	 * @brief �t�B�[�`���[�N���X��Map
	 */
	std::map<table, IFeatureClassPtr> m_featureClassMap;
	/**
	 * @brief �e�[�u����Map
	 */
	std::map<table, ITablePtr> m_tableMap;
	/**
	 * @brief �t�B�[���h�}�b�v��Map
	 */
	std::map<table, sindy::CFieldMap> m_fieldMap;
	/**
	 * @brief	DB�ƃf�[�^�x�[�X��Map
	 */
	std::map<table, database> m_tableDatabasePairMap;

	sindy::schema::walk_type::type_class::ECode m_dataType;             //!< �f�[�^�^�C�v
};

