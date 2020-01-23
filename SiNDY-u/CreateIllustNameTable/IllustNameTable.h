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

#include <boost/program_options.hpp>

/**
 * @brief �C���X�g�t�@�C�����Ή��\���R�[�h�N���X
 */
class IllustNameRec
{
public:
	/**
	 * @brief �R���X�g���N�^
	 */
	IllustNameRec() {}

	/**
	 * @brief �R���X�g���N�^
	 */
	IllustNameRec( int nIllustClass, int nCustomer, long lOID, const CString& strFileName ) :
		m_nIllustClass( nIllustClass ), m_nCustomer( nCustomer ), m_lOID( lOID ), m_strFileName( strFileName ) {}

	/**
	 * @brief �f�X�g���N�^
	 */
	~IllustNameRec() {}

	/**
	 * @brief ��r���Z�q
	 */
	bool operator < ( const IllustNameRec& rec ) const
	{
		if( m_nIllustClass != rec.m_nIllustClass )	return m_nIllustClass < rec.m_nIllustClass;
		if( m_nCustomer != rec.m_nCustomer )		return m_nCustomer < rec.m_nCustomer;
		return m_lOID < rec.m_lOID;
	}

public:
	int		m_nIllustClass;	//!< �C���X�g��ʃR�[�h
	int		m_nCustomer;	//!< �d����R�[�h
	long	m_lOID;			//!< �C���X�g�����NOID
	CString	m_strFileName;	//!< �C���X�g�t�@�C�����i�����p�j
};

/**
 * @brief �C���X�g�t�@�C�����Ή��\�N���X
 */
class IllustNameTable : public std::set< IllustNameRec, std::less< IllustNameRec > >
{
public:
	/**
	 * @brief �R���X�g���N�^
	 */
	IllustNameTable();

	/**
	 * @brief �f�X�g���N�^
	 */
	~IllustNameTable();

	/**
	 * @brief �o�̓t�@�C�����I�[�v��
	 * @param strFileName [in] �C���X�g�t�@�C�����Ή��\�̃t�@�C����
	 * @param varMap [in] �I�v�V�����i�[�}�b�v
	 * @retval true  ����
	 * @retval false ���s
	 */
	bool open( const CString& strFileName, const boost::program_options::variables_map& varMap );

	/**
	 * @brief �S���R�[�h���t�@�C���ɏ�������
	 */
	void write();

private:
	/**
	 * @brief �w�b�_�����o��
	 */
	void outputHeader( const boost::program_options::variables_map& varMap );

private:
	std::ofstream	m_ofsIllustNameTable;	//!< �C���X�g�t�@�C�����Ή��\�t�@�C���o�̓X�g���[��
};

/**
 * @brief �C���X�g�t�@�C�����ϊ��p���R�[�h�N���X
 */
class IllustNameConverterRec
{
public:
	/**
	 * @brief �R���X�g���N�^
	 */
	IllustNameConverterRec() {}

	/**
	 * @brief �R���X�g���N�^
	 * @param illustNameRec [in] �C���X�g�t�@�C�����Ή��\�̃��R�[�h
	 */
	IllustNameConverterRec( const IllustNameRec& illustNameRec ) :
		m_nIllustClass( illustNameRec.m_nIllustClass ), m_nCustomer( illustNameRec.m_nCustomer ), m_strFileName( illustNameRec.m_strFileName ) {}

	/**
	 * @brief �f�X�g���N�^
	 */
	~IllustNameConverterRec() {}

	/**
	 * @brief ��r���Z�q
	 */
	bool operator < ( const IllustNameConverterRec& rec ) const
	{
		if( m_nIllustClass != rec.m_nIllustClass )	return m_nIllustClass < rec.m_nIllustClass;
		if( m_nCustomer != rec.m_nCustomer )		return m_nCustomer < rec.m_nCustomer;
		return m_strFileName < rec.m_strFileName;
	}

public:
	int		m_nIllustClass;	//!< �C���X�g��ʃR�[�h
	int		m_nCustomer;	//!< �d����R�[�h
	CString	m_strFileName;	//!< �C���X�g�t�@�C�����i�����p�j
};

/**
 * @brief �C���X�g�t�@�C�����ϊ��N���X
 * @note �u�C���X�g��ʃR�[�h�v�u�d����R�[�h�v�u�C���X�g�t�@�C�����i�����p�j�v�ɑ΂��ă��j�[�N�ƂȂ�悤�ȃ����[�X�p�C���X�g�t�@�C������U��
 */
class IllustNameConverter : private std::map< IllustNameConverterRec, int >
{
private:
	/**
	 * @brief �R���X�g���N�^
	 * @note private�����Ă���̂Ŏg�p�s�\
	 */
	IllustNameConverter();

public:
	/**
	 * @brief �R���X�g���N�^
	 * @param illustNameTable [in] �C���X�g�t�@�C�����Ή��\�N���X
	 */
	IllustNameConverter( const IllustNameTable& illustNameTable );

	/**
	 * @brief �f�X�g���N�^
	 */
	~IllustNameConverter();

	/**
	 * @brief �Ή����郊���[�X�p�C���X�g�t�@�C�����̎擾
	 * @param illustNameRec [in] �C���X�g�t�@�C�����Ή��\�̃��R�[�h���
	 * @return �����[�X�p�C���X�g�t�@�C����
	 */
	CString getReleaseFileName( const IllustNameRec& illustNameRec );
};
