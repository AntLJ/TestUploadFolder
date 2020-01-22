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

#include <atlstr.h>
#include <FieldMap.h>
#include <list>

#include "RelTableData.h"


/**
 * @brief �S�������N�R�t���e�[�u���ɑ΂��Ă̏������s�����߂̃w���p�[�N���X
 * @note  �S�������N�R���e�[�u������̊֘A��̓S���H���e�[�u���̒l�̎擾�����A<br>
 *        �y�сA�S���H���e�[�u����FK���瑼�e�[�u���̒l���擾���鏈�����{�N���X�Ŏ���
 */
class CRelationTableHelper
{
public:

	CRelationTableHelper() : m_Oid(-1) {}
	~CRelationTableHelper(){}

	/**
	 * @brief ����������
	 * @note  �ŏ��ɖ{�֐���K���Ăяo���K�v����
	 * @param[in] ipRow     ���̈����Ɠ����I�[�i�[�̃e�[�u�����J��
	 * @param[in] cApp      �A�v���P�[�V�����w���p�[
	 * @retval true  ����������
	 * @retval false ���������s
	 */
	bool init( _IRowPtr& ipRow, const CApplication& cApp );

	/**
	 * @brief �S�������N�R�t���e�[�u���̓ǂݍ���
	 * @param [out] cData �ǂݍ��񂾃f�[�^ 
	 */
	bool readRelRailwayLink( CRelTableData& cData );

	/**
	 * @brief �w�|�C���gID��OID����S���H���f�[�^��ǂݍ���
	 * @param [in]  lId    �ǂݍ��ݑΏۂ�STATION_ID
	 * @param [out] cData  �ǂݍ��񂾘H���f�[�^�Q
	 * @retval true  �擾����
	 * @retval false �擾���s
	 */
	bool readRailLineFromStaPtOid(long lStaPtOid, CRelTableData& cData);

	/**
	 * @brief �w������S���H���f�[�^��ǂݍ���
	 * @param [in]  strStaName    �ǂݍ��ݑΏۂ̉w��
	 * @param [out] cData         �ǂݍ��񂾓S���H���f�[�^
	 * @retval true  �擾����
	 * @retval false �擾���s
	 */
	bool readRailwayLineFromStaName(const CString& strStaName, CRelTableData& cData);

	/**
	 * @brief �S�������N�R�t���e�[�u���փf�[�^�ǉ�
	 * @param  [in]   cData   �ǉ����郌�R�[�h�Q
	 * @retval true   �ǉ�����
	 * @retval false  �ǉ����s
	 */
	bool addData( const CRelTableData& cData ) const;

	/**
	 * @brief �S�������N�R�t���e�[�u���̃f�[�^�폜
	 * @param [in]   IDs   �폜�Ώۂ̓S���H��OID�Q
	 * @retval true  �폜����
	 * @retval false �폜���s
	 */
	bool deleteData( const std::vector<long>& IDs ) const;

	/**
	 * @brief   �S���H����OID���w�肵�ĊY������S�������N�̃t�B�[�`���[�Q���擾����
	 * @param   [in]   lRwLineId  �S���H����OID 
	 * @return  �S�������N�̃t�B�[�`���Q
	 */
	std::vector<CAdapt<IFeaturePtr>> getRailwayLinkFeatures( long lRwLineId );


private:

	/**
	 * @brief   �S�������N�R�t���e�[�u���̍폜�������쐬
	 * @param   [in]     cData      �폜�Ώۂ̓S���H��OID�Q
	 * @return  �폜������
	 */
	CString getDeleteWhereClause( const std::vector<long>& IDs ) const;

	/**
	 * @brief �S���H���e�[�u����OID����f�[�^���擾����
	 * @param [in]   lOid    �S���H���e�[�u����OID
	 * @param [out]  record  �ǂݍ��񂾃��R�[�h 
	 * @retval true  �擾����
	 * @retval false �擾���s
	 */
	bool readRailwayLineRecord( long lOid, CRelTableData::RailwayLineRecord& record );

	/**
	 * @brief �S���H���e�[�u���̃f�[�^���w���ID����擾����
	 * @param [in]   lOid   �S���H���e�[�u���̓S�����ID
	 * @param [out]  cData  �ǂݍ��񂾃f�[�^ 
	 * @retval true  �擾����
	 * @retval false �擾���s
	 */
	bool readRailwayLineFromStaInfoId( long lStaInfoId, CRelTableData& cData );

	/**
	 * @brief  �S�����ID�����Ж����擾����
	 * @note   ��Ж����擾�ł��Ȃ���΁A�󕶎����Ԃ�
	 * @param  [in]  lCorpId  �S�����ID
	 * @return ��Ж� 
	 */
	CString getCorpName( long lCorpId );


private:

	long	m_Oid;								//!< �֘A�����R�[�h��OID

	ITablePtr         m_ipRelRailwayLink;		//!< �S�������N�R�t���e�[�u��
	ITablePtr         m_ipRailwayLink;			//!< �S�������N�e�[�u��
	ITablePtr         m_ipJoinLineInfo;			//!< ������H���e�[�u��
	ITablePtr         m_ipRailwayLine;			//!< �S���H���e�[�u��
	ITablePtr		  m_ipRailwayCorp;			//!< �S����Ѓe�[�u��
	ITablePtr		  m_ipStationInfo;			//!< �w���}�X�^�[
	IFeatureClassPtr  m_ipStationPoint;			//!< �w�|�C���g

	sindy::CFieldMap  m_RelRailwayLinkFM;		//!< �֘A�e�[�u���̃t�B�[���h�}�b�v
	sindy::CFieldMap  m_RailwayLinkFM;			//!< �S�������N�R�t���e�[�u���t�B�[���h�}�b�v
	sindy::CFieldMap  m_JoinLineInfoFM;			//!< ������H���e�[�u���̃t�B�[���h�}�b�v
	sindy::CFieldMap  m_RailwayLineFM;			//!< �S���H���e�[�u���̃t�B�[���h�}�b�v
	sindy::CFieldMap  m_RailwayCorpFM;			//!< �S����Ѓe�[�u���̃t�B�[���h�}�b�v
	sindy::CFieldMap  m_StationInfoFM;			//!< �w���}�X�^�[�̃t�B�[���h�}�b�v
	sindy::CFieldMap  m_StationPointFM;			//!< �w�|�C���g�e�[�u���̃t�B�[���h�}�b�v

};
