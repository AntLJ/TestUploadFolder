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


class FeatureReader
{
public:
	/**
	 * @brief   �R���X�g���N�^
	 */	
	FeatureReader(void){}
	explicit FeatureReader(const IFeatureClassPtr& feature_class) : m_FeatureClass(feature_class){}
	FeatureReader(const IFeatureClassPtr& feature_class, const IFeatureClassPtr& mesh_class): 
		m_FeatureClass(feature_class),
		m_BaseMeshClass(mesh_class)
	{}

	/**
	 * @brief   �f�X�g���N�^
	 */	
	virtual ~FeatureReader(void){};

	/**
	 * @brief   �f�[�^�ǂݍ���
	 * @detail  �����Ŏw�肵��where��ɊY������t�B�[�`�����L���b�V������
	 * @param[in] where_clause  ��������
	 * @retval  true  ����
	 * @retval  false ���s
	 */	
	bool read(const CString& where_clause);

	/**
	 * @brief   �f�[�^�ǂݍ���
	 * @param[in] basemesh_code  �����Ώ۔͈͂ƂȂ�x�[�X���b�V���R�[�h
	 * @retval  true  ����
	 * @retval  false ���s
	 */	
	bool read(long basemesh_code);

	/**
	 * @brief   �f�[�^�ǂݍ���
	 * @param[in] geo           ��Ԍ����͈̔͂ƂȂ�W�I���g��
	 * @param[in] where_clause  ��������
	 * @retval  true  ����
	 * @retval  false ���s
	 */	
	bool read(const IGeometryPtr& geo, const CString& where_clause = _T(""));

	/**
	 * @brief   �f�[�^�ǂݍ���
	 * @param[in] geo           ��Ԍ����̌��ƂȂ�W�I���g��
	 * @param[in] meter         geo �̎��͉����[�g���ɑ΂��Č������s����
	 * @param[in] where_clause  ��������
	 * @retval  true  ����
	 * @retval  false ���s
	 */	
	bool readAround(const IGeometryPtr& geo, double meter, const CString& where_clause = _T(""));

	/**
	 * @brief   �f�[�^�ǂݍ���
	 * @param[in] basemesh_code  �����Ώ۔͈͂ƂȂ�x�[�X���b�V���R�[�h
	 * @param[in] where_clause   ��������
	 * @retval  true  ����
	 * @retval  false ���s
	 */	
	bool read(long basemesh_code, const CString& where_clause);

	/**
	 * @brief   �t�B�[�`���Q��Ԃ�
	 * @return  �ێ����Ă���t�B�[�`���Q
	 */	
	const std::vector<IFeaturePtr>& getFeatures() const { return m_Features; }

	/**
	 * @brief   �ێ����Ă���t�B�[�`������Ԃ�
	 * @return  �ێ����Ă���t�B�[�`����
	 */	
	unsigned long count() const{ return m_Features.size(); }

private:
	/**
	 * @brief   �W�I���g������w�苗�������o�b�t�@�����N�����`����쐬����
	 * @param[in] geo    �o�b�t�@�����O���̃W�I���g��
	 * @param[in] meter  �o�b�t�@�����O����(���[�g��)
	 * @return  �o�b�t�@�����N�����`��
	 */	
	IGeometryPtr getBuffer( const IGeometryPtr& geo, double meter ) const;

	/**
	 * @brief   �w�肵�����b�V���R�[�h�̃W�I���g�����擾����
	 * @param[in] basemesh_code  �x�[�X���b�V���R�[�h
	 * @retval null�ȊO  ���b�V���`��
	 * @retval nul�ȊO   ���b�V���`��̎擾���s
	 */	
	IGeometryPtr getBasemeshShape( long basemesh_code ) const;

private:
	IFeatureClassPtr m_FeatureClass;		//!< �ǂݍ��ݑΏۂ̃t�B�[�`���N���X
	IFeatureClassPtr m_BaseMeshClass;		//!< �����Ŏg�p���郁�b�V���t�B�[�`���N���X

	std::vector<IFeaturePtr> m_Features;	//!< �ǂݍ��񂾃t�B�[�`���Q
};

