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

#include <TDC/useful_headers/boost_program_options.h>
namespace po = boost::program_options;

/**
* @brief	�����i�[�p�R���e�i
*/
struct Arguments
{
	uh::tstring edtAddrDB;     //!< �Z���i�ҏW�jDB
	uh::tstring meshDB;        //!< ���b�V��DB
	uh::tstring mapDB;         //!< �w�i�|���S��DB
	uh::tstring cityMeshList;  //!< �s�s�n�}���b�V���R�[�h���X�g
	uh::tstring baseMeshList;  //!< �x�[�X���b�V���R�[�h���X�g
	uh::tstring layer;         //!< �`�F�b�N�Ώۃ��X�g
	uh::tstring logDir;        //!< ���O�t�H���_
	uh::tstring iniFile;       //!< �ݒ�t�@�C��
	uh::tstring master;        //!< �e�L�X�g�Z���}�X�^
	uh::tstring runLog;        //!< ���s���O�t�@�C��
};

/**
 * @enum	argumentType
 * @brief	�����^�C�v
 */
enum argumentType
{
	kAddressDBPath     = 1,   //!< �Z���i�ҏW�jDB
	kMeshDBPath        = 2,   //!< ���b�V��DB
	kMapDBPath         = 3,   //!< �w�i�|���S��DB
	kMasterPath        = 4,   //!< �e�L�X�g�Z���}�X�^
	kIniFilePath       = 5,   //!< �ݒ�t�@�C��
	kCityMeshListPath  = 6,   //!< �s�s�n�}���b�V���R�[�h���X�g
	kBaseMeshListPath  = 7,   //!< �x�[�X���b�V���R�[�h���X�g
	kLayer             = 8,   //!< �`�F�b�N�Ώۃ��X�g
	kLogDir            = 9,   //!< ���O�t�H���_
	kRunLog            = 10,  //!< ���s���O�t�@�C��
};

/**
 * @class	Argument
 * @brief	�R�}���h���C�������̏���
 * @note	
 */
class Argument
{
public:
	/**
	 * @brief  �R���X�g���N�^
	 */
	Argument(void){}

	/**
	 * @brief  �f�X�g���N�^
	 */
	virtual ~Argument(void){}

	/**
	* @brief �K�v�ȃI�v�V������񂪂��邩���`�F�b�N����
	* @param [in] vm           ������map
	* @param [in] option       �I�v�V��������
	* @param [in] desctiption  �I�v�V�����̋L�q
	* @retval true �K�v�ȃI�v�V��������
	* @retval false �K�v�ȃI�v�V�����Ȃ�
	*/
	bool checkParameter(const po::variables_map& vm, LPCSTR option, LPCSTR desctiption) const;

	/**
	* @brief �R�}���h���C�������̉��
	* @param	[in] argc 	�R�}���h���C�������̐�
	* @param	[in] argv 	�R�}���h���C�������̓��e
	* @retval	true	����
	* @retval	false	���s
	*/
	bool setParameter(int argc, TCHAR** argv);

	/**
	* @brief   �����̎擾
	* @param	[in] type 	�����^�C�v
	* @return	�����̓��e
	*/
	const CString& getValue(argumentType type);	

private:

	CString           m_addressDBPath; //!< �Z���i�ҏW�jDB
	CString           m_meshDBPath;    //!< �s�s�n�}���b�V��DB�p�X
	CString           m_mapDBPath;     //!< �w�i�|���S��DB�p�X
	CString           m_masterPath;    //!< �e�L�X�g�Z���t�@�C���p�X
	CString           m_iniFilePath;   //!< �ݒ�t�@�C���p�X
	CString           m_cityMeshListPath;  //!< �s�s�n�}���b�V�����X�g�p�X
	CString           m_baseMeshListPath;  //!< �x�[�X���b�V�����X�g�p�X
	CString           m_layer;         //!< �`�F�b�N�Ώۃ��C��
	CString           m_logDir;        //!< ���O�̏o�̓t�H���_�p�X
	CString           m_runLog;        //!< ���s���O�t�@�C���p�X
};
