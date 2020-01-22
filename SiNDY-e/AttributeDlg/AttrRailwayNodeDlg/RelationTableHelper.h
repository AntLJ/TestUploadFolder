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
 * @brief �S���m�[�h�R�t���e�[�u���ɑ΂��Ă̏������s�����߂̃w���p�[�N���X
 * @note  �S���m�[�h�R���e�[�u������̊֘A��̏�����H���e�[�u���̒l�̎擾�����A<br>
 *        �y�сA������H���e�[�u����FK���瑼�e�[�u���̒l���擾���鏈�����{�N���X�Ŏ���
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
	 * @brief �S���m�[�h�R�t���e�[�u���̓ǂݍ���
	 * @param [out] cData �ǂݍ��񂾃f�[�^ 
	 */
	bool read( CRelTableData& cData );

	/**
	 * @brief ������H���e�[�u���̓ǂݍ���
	 * @param [in]  lId    �ǂݍ��ݑΏۂ�STATION_ID
	 * @param [out] cData  �ǂݍ��񂾉w���f�[�^
	 * @retval true  �擾����
	 * @retval false �擾���s
	 */
	bool readJoinLineInfo(long lStaPtOid, CRelTableData& cData);

	/**
	 * @brief �S���m�[�h�R�t���e�[�u���փf�[�^�ǉ�
	 * @param  [in]   cData   �ǉ����郌�R�[�h�Q
	 * @retval true   �ǉ�����
	 * @retval false  �ǉ����s
	 */
	bool addData( const CRelTableData& cData ) const;

	/**
	 * @brief �S���m�[�h�R�t���e�[�u���̃f�[�^�폜
	 * @param [in]   IDs   �폜�Ώۂ�JOIN_LINE_INFO_ID�Q
	 * @retval true  �폜����
	 * @retval false �폜���s
	 */
	bool deleteData( const std::vector<long>& IDs ) const;

	/**
	 * @brief �S���m�[�h�R�t���e�[�u���̉w�ԍ����X�V
	 * @param [in]   cData  �w�ԍ��ύX�Ώۃ��R�[�h�Q
	 * @retval true  �ύX����
	 * @retval false �ύX���s
	 */
	bool changeStationNumber( const CRelTableData& cData ) const;

	/**
	 * @brief �S���m�[�h�R�t���e�[�u���̍폜�������쐬
	 * @param   [in]     cData      �폜�Ώۂ�JOIN_LINE_INFO_ID�Q
	 * @retval  �폜������
	 */
	CString getDeleteWhereClause( const std::vector<long>& IDs ) const;

	/**
	 * @brief  �w�|�C���g�̃t�B�[�`�����擾
	 * @param  [in]    lId     �t�B�[�`���擾�Ώۂ�JOIN_LINE_INFO_ID
	 * @retval return  �w�|�C���g�̃t�B�[�`��
	 */
	IFeaturePtr getStationPointFeature( long lId );


private:

	/**
	 * @brief  �H��ID����H�������擾����
	 * @param  �w�����擾�ł��Ȃ���΁A�󕶎����Ԃ�
	 * @param  [in]  lLineId  �H��ID
	 * @return �H���� 
	 */
	CString getRailwayLineName( long lLineId );

	/**
	 * @brief  �w���ID����w�����擾����
	 * @param  �w���}�X�^�[����w�����擾����
	 * @param  [in]  lInfoId �w���ID
	 * @return �w�� 
	 */
	CString getStaionName( long lInfoOId );	


private:

	long	m_Oid;								//!< �֘A�����R�[�h��OID

	ITablePtr         m_ipRelRailwayNode;		//!< �֘A�e�[�u��
	ITablePtr         m_ipJoinLineInfo;			//!< ������H���e�[�u��
	ITablePtr         m_ipRailwayLine;			//!< �S���H���e�[�u��
	ITablePtr		  m_ipStationInfo;			//!< �w���}�X�^�[
	IFeatureClassPtr  m_ipStationPoint;			//!< �w�|�C���g

	sindy::CFieldMap  m_RelRailwayNodeFM;		//!< �֘A�e�[�u���̃t�B�[���h�}�b�v
	sindy::CFieldMap  m_JoinLineInfoFM;			//!< ������H���e�[�u���̃t�B�[���h�}�b�v
	sindy::CFieldMap  m_RailwayLineFM;			//!< ������H���e�[�u���̃t�B�[���h�}�b�v
	sindy::CFieldMap  m_StationInfoFM;			//!< ������H���e�[�u���̃t�B�[���h�}�b�v
	sindy::CFieldMap  m_StationPointFM;			//!< �w�|�C���g�e�[�u���̃t�B�[���h�}�b�v

};
