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
#include "RelStationSiteData.h"

// STL
#include <list>

// SiNDYLib
#include <FieldMap.h>



/**
 * @brief �w�|���S���R�t���e�[�u���ɑ΂��Ă̏������s�����߂̃w���p�[�N���X
 */
class CRelationTableHelper
{
public:

	CRelationTableHelper() : m_Oid(-1), m_LayerNo(-1) {}
	~CRelationTableHelper(){}

	/**
	 * @brief ����������
	 * @note  �ŏ��ɖ{�֐���K���Ăяo���K�v����
	 * @param[in] ipRow     ���̈����Ɠ����I�[�i�[�̃e�[�u�����J��
	 * @retval true  ����������
	 * @retval false ���������s
	 */
	bool init( const _IRowPtr& ipRow );

	/**
	 * @brief �w�|���S���R�t���e�[�u���̓ǂݍ���
	 * @param [out] cData �ǂݍ��񂾃f�[�^ 
	 * @retval true  �ǂݍ��ݐ���
	 * @retval false �ǂݍ��ݎ��s
	 */
	bool readRelStationSite( CRelStationSiteData& cData );

	/**
	 * @brief �w������w�|���S���R�t���e�[�u����ǂݍ���
	 * @param [in]  strStaName    �ǂݍ��ݑΏۂ̉w��
	 * @param [out] cData         �ǂݍ��񂾓S���H���f�[�^
	 * @retval true  �ǂݍ��ݐ���
	 * @retval false �ǂݍ��ݎ��s
	 */
	bool readRelStationSiteByStationName(const CString& strStationName, CRelStationSiteData& cData);

	/**
	 * @brief �w�|���S���R�t���e�[�u���փf�[�^�ǉ�
	 * @param  [in]   cData   �ǉ����郌�R�[�h�Q
	 * @retval true   �ǉ�����
	 * @retval false  �ǉ����s
	 */
	bool insertData( const CRelStationSiteData& cData ) const;

	/**
	 * @brief �w�|���S���R�t���e�[�u���̃f�[�^�폜
	 * @param [in]   cData  �폜���郌�R�[�h�Q
	 * @retval true  �폜����
	 * @retval false �폜���s
	 */
	bool deleteData( const CRelStationSiteData& cData ) const;

	/**
	 * @brief   �O���[�vID�}�X�^����w�����擾����
	 * @param   [in]   lGroupId  �O���[�vID
	 * @return  �w��
	 */
	CString getStationName( long lGroupId ) const;


private:

	/**
	 * @brief   �w�|���S���R�t���e�[�u���̍폜�������쐬
	 * @param   [in]     cData      �폜�Ώۂ̓S���H��OID�Q
	 * @return  �폜������
	 */
	CString getDeleteWhereClause( const CRelStationSiteData& cData ) const;

	/**
	 * @brief �w�|���S���R�t���e�[�u���̃f�[�^���wID����擾����
	 * @param [in]   lStationId   �wID
	 * @param [out]  cData        �ǂݍ��񂾃f�[�^ 
	 * @retval true  �擾����
	 * @retval false �擾���s
	 */
	bool readRelStationSiteByStationId( long lStationId, CRelStationSiteData& cData );


public:

	/**
	 * @brief ���C���ԍ����擾����
	 * @param [in]   ipRow
	 * @retval -1�ȊO �擾����
	 * @retval -1     �擾���s
	 */
	static int getLayerNo( const _IRowPtr& ipRow );

	/**
	 * @brief ���C���ԍ����擾����
	 * @param [in]   ���C����
	 * @retval -1�ȊO �擾����
	 * @retval -1     �擾���s
	 */
	static int getLayerNo( const CString& strLayerName );


private:

	long	m_Oid;								//!< �֘A�����R�[�h��OID
	int     m_LayerNo;							//!< ���C���[�ԍ�

	ITablePtr         m_ipRelStationSite;		//!< �w�|���S���R�t���e�[�u��
	ITablePtr         m_ipGroupIdMaster;		//!< �O���[�vID�}�X�^

	sindy::CFieldMap  m_ipRelStationSiteFM;		//!< �w�|���S���R�t���e�[�u���̃t�B�[���h�}�b�v
	sindy::CFieldMap  m_ipGroupIdMasterFM;		//!< �O���[�vID�}�X�^�̃t�B�[���h�}�b�v

};
