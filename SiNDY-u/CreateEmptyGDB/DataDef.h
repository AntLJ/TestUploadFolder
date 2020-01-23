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

const esriFeatureType UNKOWN_FEATURE_TYPE = static_cast<esriFeatureType>(-1);
const esriFieldType   UNKOWN_FIELD_TYPE   = static_cast<esriFieldType>(-1);

// �o�͐�̃^�C�v���`
enum OUTPUT_TYPE
{
	UNKOWN, //!< ����`
	SDE,    //!< SDE
	FOLDER, //!< �t�H���_
	LOCAL,  //!< PDGB/FGDB
};

namespace create_info
{

/**
 * @brief ���C�������ɕK�v�ȏ����i�[
 */
class CreateLayerInfo
{
public:
	uh::tstring m_name;	                      //!< �������郌�C������
	boost::optional<uh::tstring> m_aliasName; //!< �������郌�C������
	IFieldsEditPtr m_addFields;               //!< ���C���ɒǉ��Ő�������t�B�[���h��`

};

/**
 * @brief �h���C�������ɕK�v�ȏ����i�[
 */
class CreateDomainInfo
{
public:
	/// �R���X�g���N�^
	CreateDomainInfo(){}
	/**
	 * @brief �R���X�g���N�^
	 */
	CreateDomainInfo(const uh::tstring& name):
		m_name(name)
	{}

	bool operator <(const CreateDomainInfo& c) const
	{
		return m_name < c.m_name;
	}

	uh::tstring m_name;	//!< ��������h���C������
};

/**
 * @brief �t�B�[�`���f�[�^�Z�b�g�����ɕK�v�ȏ����i�[
 */
class CreateDatasetInfo
{
public:
	/// �R���X�g���N�^
	CreateDatasetInfo(){}
	/**
	 * @brief �R���X�g���N�^
	 */
	CreateDatasetInfo(const uh::tstring& name):
		m_name(name)
	{}

	bool operator <(const CreateDatasetInfo& c) const
	{
		return m_name < c.m_name;
	}

	uh::tstring m_name;	//!< ��������t�B�[�`���f�[�^�Z�b�g����
};

/**
 * @brief ���C���E�h���C���E�t�B�[�`���f�[�^�Z�b�g�����̂��߂ɕK�v�ȏ����i�[
 */
class CreateInfo
{
public:
	std::vector<CreateLayerInfo> m_layerInfo;   //!< ���C���̏��
	std::set<CreateDomainInfo>   m_domainInfo;  //!< �h���C���̏��
	std::set<CreateDatasetInfo>  m_datasetInfo; //!< �t�B�[�`���f�[�^�Z�b�g�̏��
};

/**
 * @brief ����DB���̃��C���E�h���C���E�t�B�[�`���f�[�^�Z�b�g�����̂��߂ɕK�v�ȏ����i�[
 * < ����DB, �������>
 * @detail ���������`���ɂ��邽�߁A���̂�map�ł͂Ȃ�vector<pair>�Œ�`���Ă���
 */
typedef std::vector<std::pair<uh::tstring, CreateInfo>> CREATEINFOMAP;

} // create_info


namespace product
{

/**
 * @brief �������郌�C����`�N���X
 */
class ProductLayerDef
{
public:
	ProductLayerDef():
		m_srcFeatureType(UNKOWN_FEATURE_TYPE)
	{}
	virtual ~ProductLayerDef(){}

	uh::tstring m_name;	     //!< �쐬���郌�C������
	uh::tstring m_aliasName; //!< �쐬���郌�C���̃G�C���A�X����
	IFieldsEditPtr m_fields; //!< �쐬����t�B�[���h�Q
	IUIDPtr m_srcCLSID;      //!< �����C����CLSID
	IUIDPtr m_srcEXTCLSID;   //!< �����C����EXTCLSID

	// �ȉ��̓t�B�[�`���N���X�쐬�Ŏg�p
	uh::tstring m_shapeFieldName;     //!< ���t�B�[�`���N���X�̌`��t�B�[���h����
	esriFeatureType m_srcFeatureType; //!< ���t�B�[�`���N���X�̃t�B�[�`���N���X�^�C�v
	uh::tstring m_datasetName;        //!< ��������f�[�^�Z�b�g
};

/**
 * @brief �h���C�������̂��߂ɕK�v�ȏ����`����N���X
 */
class ProductDomainDef
{
public:
	uh::tstring m_name;      //!< ����
	IDomainPtr  m_domainPtr; //!< �h���C��
};

/**
 * @brief �t�B�[�`���f�[�^�Z�b�g�����̂��߂ɕK�v�ȏ����`����N���X
 */
class ProductDatasetDef
{
public:
	uh::tstring m_name; //!< ����
	ISpatialReferencePtr m_spRef; //!< ��ԎQ��
};

/**
 * @brief ���C�������̂��߂ɕK�v�ȏ����i�[����N���X
 */
class ProductDef
{
public:
	std::vector<ProductLayerDef>   m_layerList;   //!< �e�[�u����`
	std::vector<ProductDomainDef>  m_domainList;  //!< �h���C����`
	std::vector<ProductDatasetDef> m_datasetList; //!< �t�B�[�`���f�[�^�Z�b�g��`
};

} // product
