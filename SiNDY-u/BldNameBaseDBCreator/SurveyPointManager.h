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

/**
 * @brief �����Ώۃ|�C���g���R�[�h
 */
class SurveyPointRec : public std::vector< CString >
{
public:
	/**
	 * @breif �R���X�g���N�^
	 */
	SurveyPointRec();

	/**
	 * @brief �f�X�g���N�^
	 */
	~SurveyPointRec();

	/**
	 * @brief ��r���Z�q
	 */
	bool operator < ( const SurveyPointRec& cRec ) const
	{
		if( e_objectID == cRec.e_objectID )
			return ( e_srcType < cRec.e_srcType );
		else
			return ( e_objectID < cRec.e_objectID );
	}

	bool setGeometry( const IFeaturePtr& ipFeature );

public:
	long	e_objectID;		//!< �I�u�W�F�N�gID
	long	e_srcType;		//!< �o�T�R�[�h
	long	e_bldOID;		//!< �����|���S��OID

	IPointPtr	e_ipPoint;		//!< ���Y�����Ώۃ|�C���g�W�I���g��
};

/**
 * @brief �����Ώۃ|�C���g���X�g�Ǘ��N���X
 */
class SurveyPointManager : std::set< SurveyPointRec, std::less< SurveyPointRec > >
{
public:
	/**
	 * @brief �����Ώۃ|�C���g���X�g�̃t�B�[���h�C���f�b�N�X
	 */
	enum EFieldIndex
	{
		kObjectID = 0,	//!< �I�u�W�F�N�gID
		kName,			//!< ����
		kOrgName1,		//!< ���H�O����1
		kOrgName2,		//!< ���H�O����2
		kKana,			//!< �J�i
		kAddr,			//!< �Z��
		kOrgAddr,		//!< ���`�O�Z��
		kAddrCode,		//!< �Z���R�[�h
		kAddrLon,		//!< �Z���o�x
		kAddrLat,		//!< �Z���ܓx
		kRepType,		//!< �n�ԑ�\�_�����R�[�h
		kAddrLv,		//!< �Z�����x���R�[�h
		kPinPoint,		//!< �s���|�C���g�t���O
		kMultiSameBldg,	//!< ����ƌ`�������Z�����݃t���O
		kSameAddr,		//!< ����iPC�Z���������݃t���O
		kSameAddrCount,	//!< ����iPC�Z����
		kAddrVer,		//!< �Z����͌�XYDB��
		kSrcType,		//!< �o�T�R�[�h
		kSrcName,		//!< �o�T��
		kSrcDate,		//!< �o�T�������莞��
		kSrcID,			//!< �o�T����ID
		kBldg1Div1,		//!< �p�r�敪1
		kBldg1Div2,		//!< �p�r�敪2
		kBldg2Div1,		//!< �������p�r�敪1
		kBldg2Div2,		//!< �������p�r�敪2
		kHeight,		//!< ��������
		kFloors,		//!< �n��K��
		kBaseFloors,	//!< �n���K��
		kBldgArea,		//!< �����ʐ�
		kAllArea,		//!< ���׏��ʐ�
		kSrcStartDate,	//!< �f�ގg�p�J�n�N����
		kSrcUpdateDate,	//!< �f�ލX�V�N����
		kUpdateDate,	//!< �X�V�N����
		kAdoption,		//!< �̗p�R�[�h
	};

	/**
	 * @brief �R���X�g���N�^
	 */
	SurveyPointManager(
		const IFeatureClassPtr& ipCityMesh,
		const IFeatureClassPtr& ipCityAdmin,
		const std::map<CString, long>& mapCityMeshIndexes,
		const std::map<CString, long>& mapCityAdminIndexes,
		const std::map<CString, long>& mapBldgSrcPtIndexes,
		const std::map<CString, long>& mapTownpageIndexes,
		const CString& sourceName,
		const CString& sourceDate,
		const ADODB::_ConnectionPtr& conAddrDB
		);

	/**
	 * @brief �f�X�g���N�^
	 */
	~SurveyPointManager();

	/**
	 * @brief �o�͐�t�@�C���I�[�v��
	 * @param fileName [in] �o�͐�t�@�C����
	 * @retval true  ����
	 * @retval false ���s
	 */
	bool open( const CString& fileName );

	/**
	 * @brief �o�͐�t�@�C���N���[�Y
	 */
	void close();

	/**
	 * @brief �����Ώۃ|�C���g���R�[�h�̒ǉ��i�f�ރ|�C���g�R���j
	 * @param srcOID [in] �f�ރ|�C���gOID
	 * @param bldOID [in] �����|���S��OID
	 * @param ipFeature [in] �Ώۑf�ރ|�C���g�t�B�[�`��
	 * @retval true  ����
	 * @retval false ���s
	 */
	bool addFromBldgNameSrc( long srcOID, long bldOID, const IFeaturePtr& ipFeature );

	/**
	 * @brief �����Ώۃ|�C���g���R�[�h�̒ǉ��i�^�E���y�[�W�R���j
	 * @param twnOID [in] �^�E���y�[�W�|�C���gOID
	 * @param bldOID [in] �����|���S��OID
	 * @param ipFeature [in] �Ώۃ^�E���y�[�W�|�C���g�t�B�[�`��
	 * @retval true  ����
	 * @retval false ���s
	 */
	bool addFromTownpage( long twnOID, long bldOID, const IFeaturePtr& ipFeature );

	/**
	 * @brief �t�@�C���ւ̏o��
	 * @retval true  ����
	 * @retval false ���s
	 */
	bool output();

private:
	/**
	 * @brief ���b�V���R�[�h�̎擾
	 * @param ipPoint [in] �Ώۃ|�C���g
	 * @return ���b�V���R�[�h
	 */
	CString getMeshCode( const IPointPtr& ipPoint );

	/**
	 * @brief �Z��������̎擾
	 * @param ipPoint [in] �Ώۃ|�C���g
	 * @return �Z��������i11���Z���R�[�h���x���j
	 */
	CString getAddrName( const IPointPtr& ipPoint );

	/**
	 * @brief �^�E���y�[�W�|�C���g����20���Z���R�[�h���擾
	 * @param ipFeature [in] �Ώۃ^�E���y�[�W�|�C���g�t�B�[�`��
	 * @param strAddrCode [out] 20���Z���R�[�h
	 * @retval true  ����
	 * @retval false ���s
	 */
	bool getAddrCodeFromTownpage( const IFeaturePtr& ipFeature, CString& strAddrCode );

private:
	static const long SRC_TYPE_TOWN = sindy::schema::sj::buildingname_common::source_type::kTownPage;	//!< �^�E���y�[�W�̏o�T�R�[�h�i�����Ώۃ|�C���g���X�g�����j

	std::ofstream m_ofsOutput;				//!< �o�͐�t�@�C���X�g���[��

	std::map< long, CString >		m_mapFields;			//!< �t�B�[���h���X�g

	const IFeatureClassPtr&			m_ipCityMesh;			//!< citymesh�t�B�[�`���N���X
	const IFeatureClassPtr&			m_ipCityAdmin;			//!< city_admin�t�B�[�`���N���X
	const std::map<CString, long>&	m_mapCityMeshIndexes;	//!< citymesh�e�[�u���t�B�[���h�C���f�b�N�X���
	const std::map<CString, long>&	m_mapCityAdminIndexes;	//!< city_admin�e�[�u���t�B�[���h�C���f�b�N�X���
	const std::map<CString, long>&	m_mapBldgSrcPtIndexes;	//!< buildingname_src_point�e�[�u���t�B�[���h�C���f�b�N�X���
	const std::map<CString, long>&	m_mapTownpageIndexes;	//!< townpage�e�[�u���t�B�[���h�C���f�b�N�X���

	CString	m_sourceName;		//!< TOWNPAGE�R�������Ώۃ|�C���g��SOURCENAME�t�B�[���h�ɏo�͂���l
	CString	m_sourceDate;		//!< TOWNPAGE�R�������Ώۃ|�C���g��SOURCEDATE�t�B�[���h�ɏo�͂���l

	const ADODB::_ConnectionPtr&	m_conAddrDB;			//!< �Z��DB�ڑ��I�u�W�F�N�g
};
