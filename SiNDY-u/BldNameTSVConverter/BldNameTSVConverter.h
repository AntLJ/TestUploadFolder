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

#include "LogMgr.h"
#include "common.h"
#include <boost/program_options.hpp>

/**
 * @brief �Ίp���Q�i�[�p�}���`�}�b�v
 * @note �������L�[�Ɋi�[���A�Œ��̑Ίp�����擾�ł���悤�ɂ���
 */
typedef std::multimap< double, ILinePtr, std::greater< double > > DiagonalLenMap;

/**
 * @brief ���s���[�h
 */
enum ExecuteMode
{
	ReleaseTsv,    // ���������[�X�f�[�^�쐬
	TranslateList  // ������|�󃊃X�g�쐬
};

/**
 * @brief �����r�����̌���TSV�ϊ��N���X
 */
class BldNameTSVConverter
{
public:
	/**
	 * @brief �R���X�g���N�^
	 */
	BldNameTSVConverter();

	/**
	 * @brief �f�X�g���N�^
	 */
	~BldNameTSVConverter();

	/**
	 * @brief ������
	 * @param argc [in] �����̐�
	 * @param argv [in] �������X�g
	 * @retval true  ����
	 * @retval false ���s
	 */
	bool init( int argc, _TCHAR* argv[] );

	/**
	 * @brief �����̎��s
	 * @retval true  ����
	 * @retval false ���s
	 */
	bool exec();

private:
	/**
	 * @brief �����̃`�F�b�N
	 * @param argc [in] �����̐�
	 * @param argv [in] �������X�g
	 * @retval true  ����
	 * @retval false ���s
	 */
	bool checkArg( int argc, _TCHAR* argv[] );

	/**
	 * @brief �����p�����[�^�̎擾
	 * @param varMap [in] �����}�b�v
	 * @param strName [in] ������
	 * @param strParam [out] �擾�����p�����[�^
	 * @retval true  ����
	 * @retval false ���s
	 */
	bool getArgParam( const boost::program_options::variables_map& varMap, const std::string& strName, CString& strParam );

	/**
	 * @brief �ݒ�t�@�C���̓ǂݍ���
	 * @retval true  ����
	 * @retval false ���s
	 */
	bool loadIniFile();

	/**
	 * @brief �Z�����ԃe�[�u���i�[DB�ւ̐ڑ�
	 * @retval true  ����
	 * @retval false ���s
	 */
	bool openAddrDB();

	/**
	 * @breif �G���[���b�Z�[�W�̏o��
	 * @param errLv [in] �G���[���x��
	 * @param strLayer [in] ���C����
	 * @param lOID [in] �I�u�W�F�N�gID
	 * @param strMsg [in] �G���[���b�Z�[�W
	 */
	void outputErrLog( LogMgr::EErrLevel errLv, const CString& strLayer, long lOID, const CString& strMsg );

	/**
	 * @brief ������ʃR�[�h�}�X�^�̐���
	 * @retval true  ����
	 * @retval false ���s
	 */
	bool createBldgClassMaster();

	/**
	 * @brief ���������[�X�f�[�^�iTSV�j�̐���
	 * @retval true  ����
	 * @retval false ���s
	 */
	bool createBldgNameTSV();

	/**
	 * @brief �Z���R�[�h�̎擾
	 * @param lOID [in] �����r�����̃|�C���gOID
	 * @param lBldgOID [in] �����|���S��OID
	 * @param ipFeature [in] �����r�����̃|�C���g�t�B�[�`��
	 * @param strAddrCodeRet [out] �Z���R�[�h
	 * @retval true  ����
	 * @retval false ���s
	 */
	bool getAddrCode( long lOID, long lBldgOID, const IFeaturePtr& ipFeature, CString& strAddrCodeRet );

	/**
	 * @brief �Z���R�[�h�̎擾�i�Z�����ԃe�[�u�����j
	 * @param lBldgOID [in] �����|���S��OID
	 * @param strAddrCode [out] �Z���R�[�h�i20���j
	 * @retval true  ����
	 * @retval false ���s
	 */
	bool getAddrCodeFromIntermediate( long lBldgOID, CString& strAddrCode );

	/**
	 * @breif �����|���S��OID���Z���R�[�h�Ή��\�̐���
	 * @note ���Y�����|���S��OID���܂� 500,000 ����OID�͈̔͂ŁA�Z�����ԃe�[�u������Ή��\���쐬����
	 * @param lBldgOID [in] �����|���S��OID
	 * @retval true  ����
	 * @retval false ���s
	 */
	bool createMapBldgOID2AddrCode( long lBldgOID );

	/**
	 * @brief �Z���R�[�h�̎擾�i�s�s�n�}�s���E�|���S�����j
	 * @param lOID [in] ���̃|�C���g��OID�i�G���[�o�͗p�j
	 * @param ipGeometry [in] ���̃|�C���g�̃W�I���g��
	 * @param strAddrCode [out] �Z���R�[�h�i16�� or 11�� or 5���j
	 * @retval true  ����
	 * @retval false ���s
	 */
	bool getAddrCodeFromAdmin( long lOID, const IGeometryPtr& ipGeometry, CString& strAddrCode );

	/**
	 * @brief �����|���S���̑�\�|�C���g���擾����
	 * @param lOID [in] ���̃|�C���g��OID�i�G���[�o�͗p�j
	 * @param lBldgOID [in] �����|���S����OID
	 * @param ipFeature [in] ���̃|�C���g�̃t�B�[�`��
	 * @param ipPoint [out] ��\�|�C���g
	 * @retval true  ����
	 * @retval false ���s
	 */
	bool getBldgRepPoint( long lOID, long lBldgOID, const IFeaturePtr& ipFeature, IPointPtr& ipPoint );

	/**
	 * @brief �Ώۃ|���S���̑�\�|�C���g���擾����
	 * @param ipPolygon [in] �Ώۃ|���S��
	 * @param ipPoint [out] ��\�|�C���g
	 * @retval true  ����
	 * @retval false ���s
	 */
	bool getRepPoint( const IPolygon4Ptr& ipPolygon, IPointPtr& ipPoint );

	/**
	 * @brief �Ίp���̐����ƃR���e�i�ւ̒ǉ�
	 * @param ipPolygon [in] �Ώۃ|���S��
	 * @param ipRing1 [in] �Ώۃ����O�@
	 * @param lIndex1 [in] �Ώۍ\���_�C���f�b�N�X�@
	 * @param ipRing2 [in] �Ώۃ����O�A
	 * @param lIndex2 [in] �Ώۍ\���_�C���f�b�N�X�A
	 * @param ipSpRef [in] ��ԎQ��
	 * @param mapDiagonals [out] �Ίp���Q�i�[�R���e�i
	 * @retval true  ����
	 * @retval false ���s
	 */
	bool createDiagonals( const IPolygon4Ptr& ipPolygon, const IRingPtr& ipRing1, long lIndex1, const IRingPtr& ipRing2, long lIndex2, const ISpatialReferencePtr& ipSpRef, DiagonalLenMap& mapDiagonals );

	/**
	 * @brief �����r�����̃|�C���g�t�B�[�`���J�[�\���擾
	 * @param ipFeatureCur [out] �t�B�[�`���J�[�\���i�����r�����̃|�C���g�j
	 * @param lCount       [out] �擾����
	 * @retval true  ����
	 * @retval false ���s
	 */
	bool searchBldNamePoint(IFeatureCursorPtr& ipFeatureCur, long& lCount);

	/**
	 * @brief �����r�����̃|�C���g�����l�i���s���[�h���ʑ����j�擾
	 * @param ipFeature [in]  �t�B�[�`���i�����r�����̃|�C���g�j
	 * @param rec       [out] �����r�����̃|�C���g�R���e�i
	 * @retval true  ����
	 * @retval false ���s
	 */
	bool getCommonAttrValue(const IFeaturePtr& ipFeature, BldNamePointRec& rec );

	/**
	 * @brief �����r�����̌��������[�X�f�[�^�쐬�������s
	 * @retval true  ����
	 * @retval false ���s
	 */
	bool execCreateReleaseTsv();

	/**
	 * @brief ������|�󃊃X�g�쐬�������s
	 * @retval true  ����
	 * @retval false ���s
	 */
	bool execCreateMultiTranslateList();

	/**
	 * @brief ������|�󃊃X�g�쐬
	 * @retval true  ����
	 * @retval false ���s
	 */
	bool createMultiTranslateList();


private:
	ExecuteMode m_mode;         //!< ���s���[�h
	CString		m_strRunLog;	//!< ���s���O�t�@�C����
	CString		m_strErrLog;	//!< �G���[���O�t�@�C����
	CString		m_strIniFile;	//!< �ݒ�t�@�C����
	CString     m_strAddColumn; //!< ������|�󃊃X�g�ǉ��J�����ݒ�t�@�C��
	CString     m_maxRecNum;    //!< �t�@�C�����ő僌�R�[�h����
	std::vector<CString> m_addcolumnList; //!< ������|�󃊃X�g�ǉ��J�������X�g

	CString		m_strDBBase;	//!< �x�[�XDB
	CString		m_strDBMap;		//!< �n�}DB
	CString		m_strDBAddr;	//!< �Z��DB
	CString		m_strDBAM;		//!< �Z�����ԃe�[�u���i�[DB
	CString		m_strGrpCode;	//!< �f�[�^���ރR�[�h
	CString		m_strOutDir;	//!< �o�̓f�B���N�g��

	IFeatureClassPtr	m_ipFCBldgNamePoint;	//!< �����r�����̃|�C���g�t�B�[�`���N���X
	IFeatureClassPtr	m_ipFCBuilding;			//!< �����|���S���t�B�[�`���N���X
	IFeatureClassPtr	m_ipFCCityAdmin;		//!< �s���E�|���S���t�B�[�`���N���X

	FieldIndexes		m_mapIndexesBldgNamePoint;	//!< �����r�����̃|�C���g�t�B�[���h�C���f�b�N�X�}�b�v
	FieldIndexes		m_mapIndexesCityAdmin;		//!< �s���E�|���S���t�B�[���h�C���f�b�N�X�}�b�v

	ADODB::_ConnectionPtr	m_conAddrDB;	//!< �Z�����ԃe�[�u���i�[DB�ւ̐ڑ��I�u�W�F�N�g

	std::map< long, CString >	m_mapBldgOID2AddrCode;	//!< �����|���S��OID���Z���R�[�h�Ή��\
};
