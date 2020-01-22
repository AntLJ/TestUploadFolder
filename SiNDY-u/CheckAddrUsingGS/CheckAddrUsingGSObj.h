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

#include <SiNDYLib/Workspace.h>
#include <SiNDYLib/FeatureClass.h>
#include "ToolBase.h"
#include <oo4o/include/oracl.h>
#pragma comment( lib, "../../LIB/oo4o/lib/ORACLM32.LIB" )

/**
 * @brief  ���O�o�͏��N���X
 */
class CLogInfo
{
public:

	/**
	 * @brief  �R���X�g���N�^
	 */
	CLogInfo() : e_lOID(0) { }

	/**
	 * @brief  �R���X�g���N�^
	 * @param  lOID [in] �I�u�W�F�N�gID
	 */
	CLogInfo( long lOID ) : e_lOID( lOID ) { }

	/**
	 * @brief  �f�X�g���N�^
	 */
	~CLogInfo() { }

	long	e_lOID;				//!< �I�u�W�F�N�gID
	CString	e_strAddrName;		//!< �Z�����́iiPC�j
	CString	e_strAddrNameGeo;	//!< �Z�����́iGEOSPACE�j
};

/**
 * @brief  GS�f�[�^�ɂ��Z���Ó����`�F�b�N�N���X
 */
class CCheckAddrUsingGSObj : public CToolBase
{
public:

	/**
	 * @brief  �R���X�g���N�^
	 */
	CCheckAddrUsingGSObj(void);

	/**
	 * @brief  �f�X�g���N�^
	 */
	~CCheckAddrUsingGSObj(void);

	/**
	 * @brief  ������
	 * @param  argc [in]  �R�}���h���C��������
	 * @param  argv [in]  �R�}���h���C������������
	 * @return bool ����ɏ������ł������ǂ���
	 */
	bool Init( int argc, _TCHAR* argv[] );

	/**
	 * @brief  ���s
	 * @return bool ����ɏ��������s�ł������ǂ���
	 */
	bool Execute();

private:

	/**
	 * @brief  ���O�w�b�_�̏o��
	 * @note   �e�N���X�̊֐��ɃI�[�o���C�h
	 */
	void WriteLogHeader();

	/**
	 * @brief  �����`�F�b�N
	 * @param  argc [in]  �R�}���h���C��������
	 * @param  argv [in]  �R�}���h���C������������
	 * @return bool �����𐳏�ɉ�͂ł������ǂ���
	 */
	bool CheckArg( int argc, _TCHAR* argv[] );

	/**
	 * @brief  ���̓t�@�C���I�[�v��
	 * @return bool ���̓t�@�C���𐳏�ɓǂݍ��߂����ǂ���
	 */
	bool OpenInputFile();

	/**
	 * @brief  �^�u��؂�̃��R�[�h���t�B�[���h�ɕ���
	 * @param  strBuf    [in]  ���R�[�h�o�b�t�@
	 * @param  vecFields [out] �t�B�[���h�Q
	 */
	void Split(const CString& strBuf, std::vector< CString >& vecFields );

	/**
	 * @brief  ���O�̏o��
	 * @param  cLogInfo [in] ���O���
	 * @param  strMsg   [in] ���b�Z�[�W
	 */
	void WriteLogInfo( const CLogInfo& cLogInfo, const CString& strMsg );

	/**
	 * @brief  �Z��DB�ւ̐ڑ�
	 * @param  strConnect [in]  DB�ڑ�������
	 * @param  cDB        [out] �ڑ���DB�\�[�X
	 * @return ����ɐڑ��ł������ǂ���
	 */
	bool ConnectJDB( const CString& strConnect, ODatabase& cDB );

	/**
	 * @brief  �|�C���g���܂ރ|���S��������
	 * @param  ipPoint        [in]  �|�C���g
	 * @param  ipFeatureClass [in]  �����Ώۂ̃t�B�[�`���N���X
	 * @return �|���S���t�B�[�`��
	 */
	IFeaturePtr SearchPolygon( IPointPtr ipPoint, sindy::CFeatureClass ipFeatureClass );

	/**
	 * @brief  �|�C���g�t�B�[�`�����܂ޓs�s�s���E�t�B�[�`��������
	 * @param  ipF_Point [in]  �|�C���g�t�B�[�`��
	 * @return �s�s�s���E�t�B�[�`��
	 */
	IFeaturePtr SearchAdmin( IFeaturePtr ipF_Point );

	/**
	 * @brief  �|�C���g�t�B�[�`�����܂މƌ`�t�B�[�`��������
	 * @param  ipF_Point [in]  �|�C���g�t�B�[�`��
	 * @return �ƌ`�t�B�[�`��
	 */
	IFeaturePtr SearchBuilding( IFeaturePtr ipF_Point );

	/**
	 * @brief  ���YGeospaceID������GEOSPACE�ƌ`�t�B�[�`��������
	 * @param  strGeospaceID [in]  GeospaceID
	 * @return GEOSPACE�ƌ`�t�B�[�`��
	 */
	IFeaturePtr SearchBuildingGeo( const CString& strGeospaceID );

	/**
	 * @brief  11���Z���R�[�h�����n���R�[�h�ɕϊ�
	 * @param  strAddrCode [in] 11���Z���R�[�h�i�����x�[�X�j
	 * @return 11���Z���R�[�h�i���n���j
	 */
	CString ConvAddrCode2JGDC( const CString& strAddrCode );

	/**
	 * @brief  11���Z���R�[�h����Z����������擾
	 * @param  strAddrCode [in] 11���Z���R�[�h�i�����x�[�X�j
	 * @return �Z��������
	 */
	CString GetAddrName( const CString& strAddrCode );

private:

	CString			m_strConnectAdr;	//!< SDE�ڑ�������i�Z���j
	CString			m_strConnectBld;	//!< SDE�ڑ�������i�ƌ`�j
	CString			m_strConnectGeo;	//!< SDE�ڑ�������iGEOSPACE�j
	CString			m_strConnectJDB;	//!< �Z��DB�ڑ�������i�Z��������擾�j
	CString			m_strConnectJGDC;	//!< �Z��DB�ڑ�������i���������n���j
	CString			m_strLogInput;		//!< ���̓��O�t�@�C����
	CString			m_strLogOutput;		//!< �o�̓��O�t�@�C����

	ODatabase		m_cDBSourceJDB;		//!< DB�\�[�X�i�Z��������擾�j
	ODatabase		m_cDBSourceJGDC;	//!< DB�\�[�X�i���������n���j

	std::list<int>	m_listOID_GouPoint;	//!< �`�F�b�N�Ώ�OID���X�g�i���|�C���g�j

	std::map<CString, CString>	m_mapAddrName;		//!< �Z��������擾�p�L���b�V��
	std::map<CString, CString>	m_mapConvAddr;		//!< 11���Z���R�[�h�ϊ��p�L���b�V���i���������n���j

	sindy::CFeatureClass	m_cFCGouPoint;			//!< ���|�C���g�t�B�[�`���N���X
	sindy::CFeatureClass	m_cFCCityAdmin;			//!< �s�s�s���E�t�B�[�`���N���X
	sindy::CFeatureClass	m_cFCBuilding;			//!< �ƌ`�|���S���t�B�[�`���N���X
	sindy::CFeatureClass	m_cFCBuildingGeo;		//!< GEOSPACE�ƌ`�|���S���t�B�[�`���N���X

	sindy::CSPFieldMap		m_cFMGouPoint;			//!< ���|�C���g�t�B�[���h�}�b�v
	sindy::CSPFieldMap		m_cFMCityAdmin;			//!< �s�s�s���E�t�B�[���h�}�b�v
	sindy::CSPFieldMap		m_cFMBuilding;			//!< �ƌ`�|���S���t�B�[���h�}�b�v
	sindy::CSPFieldMap		m_cFMBuildingGeo;		//!< GEOSPACE�ƌ`�|���S���t�B�[���h�}�b�v

	sindy::CSPTableNameString	m_cTNGouPoint;		//!< ���|�C���g�e�[�u���l�[��
	sindy::CSPTableNameString	m_cTNCityAdmin;		//!< �s�s�s���E�e�[�u���l�[��
	sindy::CSPTableNameString	m_cTNBuilding;		//!< �ƌ`�|���S���e�[�u���l�[��
	sindy::CSPTableNameString	m_cTNBuildingGeo;	//!< GEOSPACE�ƌ`�|���S���e�[�u���l�[��
};
