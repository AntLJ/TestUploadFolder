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

// sindylib_core,sindylib_base
#include <TDC/sindylib_core/Workspace.h>
#include <TDC/sindylib_base/TableFinder.h>
#include <TDC/sindylib_base/TableContainer.h>
#include <TDC/sindylib_base/RowContainer.h>

/**
 * @brief  �w�|���S���֘A�`�F�b�J�[�N���X
 * @detail ���ۂ̃`�F�b�N���W�b�N�����N���X
 */
class StationChecker
{
public:
	StationChecker(void){}
	~StationChecker(void){}

	/**
	 * @brief  ������
	 * @param  db_info   [in]  DB�ڑ����
	 * @retval true  ����
	 * @retval false ���s
	 */
	bool init( const sindy::CWorkspace& workspace );

	/**
	 * @brief  �w���R�t���`�F�b�N
	 * @param  row   [in]  �`�F�b�N�ΏۃI�u�W�F�N�g
	 * @retval true  ���Ȃ�
	 * @retval false ��肠��
	 */
	bool checkLinkedStation( const sindy::CRowContainer& row, const CString& table_name ) const;

	/**
	 * @brief  �|���S����ʃ`�F�b�N
	 * @param  row   [in]  �`�F�b�N�ΏۃI�u�W�F�N�g
	 * @retval true  ���Ȃ�
	 * @retval false ��肠��
	 */
	bool checkPolygonC( const sindy::CRowContainer& row ) const;

	/**
	 * @brief  �|���S����`�F�b�N
	 * @param  row            [in]  �`�F�b�N�ΏۃI�u�W�F�N�g
	 * @param  outer_FcName [in]  ����鑤(�O��)�̃t�B�[�`���N���X��
	 * @retval true  ���Ȃ�
	 * @retval false ��肠��
	 */
	bool checkContain( const sindy::CRowContainer& row, const CString& outer_fc_name ) const;

	/**
	 * @brief  �|���S����`�F�b�N(�wID�l��)
	 * @param  row            [in]  �`�F�b�N�ΏۃI�u�W�F�N�g
	 * @param  outer_FcName [in]  ����鑤(�O��)�̃t�B�[�`���N���X��
	 * @return �G���[�R�[�h
	 */
	bool checkContainedBySameStationPolygon( const sindy::CRowContainer& row, const CString& outer_fc_name ) const;

	/**
	 * @brief  �w�|���S���Ƃ̕R�t���������`�F�b�N
	 * @param  row            [in]  �`�F�b�N�ΏۃI�u�W�F�N�g
	 * @retval true  ���Ȃ�
	 * @retval false ��肠��
	 */
	bool checkLinkedNotExistPolygon( const sindy::CRowContainer& row );

	/**
	 * @brief  �w�Ƃ̕R�t���������`�F�b�N
	 * @param  row            [in]  �`�F�b�N�ΏۃI�u�W�F�N�g
	 * @retval true  ���Ȃ�
	 * @retval false ��肠��
	 */
	bool checkLinkedNotExistStation( const sindy::CRowContainer& row );

private:

	/**
	 * @brief  �e�[�u���t�@�C���_�̏�����
	 * @param  workspace  [in]  ���[�N�X�y�[�X
	 * @retval true  ����������
	 * @retval false ���������s
	 */
	bool initTableFinder( const sindy::CWorkspace& workspace );

	/**
	 * @brief  SJ�w�|���S����ʃR�[�h�̃R�[�h�l�h���C���ǂݍ���
	 * @retval true  �ǂݍ��ݐ���
	 * @retval false �ǂݍ��ݎ��s
	 */
	bool readCodeValueDomain();

	/**
	 * @brief  SJ�w�|���S����OID�Q��ǂݍ���
	 * @retval true  �ǂݍ��ݐ���
	 * @retval false �ǂݍ��ݎ��s
	 */
	void readStationPolygonOid();

	/**
	 * @brief  �wID�̓ǂݍ���
	 * @retval true  �ǂݍ��ݐ���
	 * @retval false �ǂݍ��ݎ��s
	 */
	void readStationIds();

	/**
	 * @brief  �w��̃t�B�[�`��(row)���w��̃t�B�[�`���N���X(outer_fc_name)�̃|���S���ɕ����Ă��邩���`�F�b�N����
	 * @detail OID���X�g(oids) ���w�肳��Ă���ꍇ�́A�w��OID�̃|���S���Ɏw��t�B�[�`��(row)�������̂����邩���`�F�b�N����B
	 * @detail OID���X�g(oids) �����w��(��)�ł���΁AOID�����肹���Ɏw��t�B�[�`��(row)�������̂����邩���`�F�b�N����B
	 * @param row   [in] �`�F�b�N�Ώۃt�B�[�`��
	 * @param table [in] ����鑤�̃t�B�[�`���N���X��
	 * @param oids  [in] ����鑤��OID
	 * @retval true  ���Ȃ�
	 * @retval false ��肠��
	 */
	bool checkContainedByFeatureClass( const sindy::CRowContainer& row, const CString& outer_fc_name, const std::set<long>& oids ) const;

private:

	sindy::CTableFinder m_TableFinder;	//!< �e�[�u���t�@�C���_

	std::set<long>  m_CVDomains;		//!< �|���S����ʂ̃R�[�h�l�h���C��

	std::unordered_multimap<long, long> m_OidList;	//!< �w�|���S����OID�̃��X�g ( Key: ���C���ԍ�, Value: OID )
	std::set<long>  m_StationIdList;				//!< �wID�̃��X�g
};

typedef std::shared_ptr<StationChecker> SPStationChecker;
