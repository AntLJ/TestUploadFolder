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
using namespace std;
/**
 * @class	TextAddressMaster
 * @brief	�e�L�X�g�Z���}�X�^
 */
class TextAddressMaster
{
public:
	/**
	 * @brief  �R���X�g���N�^
	 */
	TextAddressMaster(void){}

	 /**
	 * @brief  �f�X�g���N�^
	 */
	virtual ~TextAddressMaster(void){}

	/**
	 * @brief	�e�L�X�g�Z���ǂݍ���
	 * @param	[in] path		�e�L�X�g�Z���p�X
	 * @retval	true:����  false:���s
	 */
	bool load( const CString& path );

	/**
	 * @brief	�Z���}�X�^�̎擾
	 * @param	[in] key	    	�Z���R�[�h
	 * @return	�X�e�[�^�X * �Z����
	 */
	AddressMaster getTextAddressMaster( const CString& key );

	/**
	 * @brief	�Z���}�X�^�R�[�h�̎擾
	 * @param	[in] key	    	�Z����
	 * @return	�X�e�[�^�X * �Z���R�[�h
	 */
	const AddressMaster TextAddressMaster::getTextAddressMasterCode( const CString& key )const;

	/**
	 * @brief	�e�L�X�g�Z���}�X�^����������
	 * @param	[in] cityCode    �s�撬���R�[�h
	 * @param	[in] status      �X�e�[�^�X
	 * @retval	true:����  false:���s
	 */
	bool searchMaster(const CString &cityCode, long status);

	/**
	 * @brief	�e�L�X�g�Z���}�X�^����������
	 * @param	[in] cityCode    �s�撬���R�[�h
	 * @retval	true:����  false:���s
	 */
	bool searchMaster(const CString &cityCode);

	/**
	 * @brief	�e�L�X�g�Z���}�X�^�t�H���_��ݒ肷��
	 * @param	[in] textAddressMasterPath        �e�L�X�g�Z���}�X�^�p�X
	 */
	void setDirPath( const CString& textAddressMasterPath ){ m_textAddressMasterPath = textAddressMasterPath; }

private:
	/**
	 * @brief	�e�L�X�g�Z���t�@�C���T��
	 * @param	[in] filePath        �e�L�X�g�Z���p�X
	 * @param	[in] folder          �e�L�X�g�Z���t�H���_
	 * @retval	true:����  false:���s
	 */
	bool findFile( const CString& filePath, const CString& folder);

	/**
	 * @brief	�e�L�X�g�Z���t�@�C���ǂݍ���
	 * @param	[in] filePath        �e�L�X�g�Z���}�X�^�p�X
	 * @param	[in] folder          �e�L�X�g�Z���t�H���_
	 * @retval	true:����  false:���s
	 */
	bool readData( const CString& filePath, const CString& folder);

	map<CString, AddressMaster> m_textAddressMaster;         //!< �e�L�X�g�Z���}�X�^���X�g
	map<CString, AddressMaster> m_textAddressMasterCode;     //!< �e�L�X�g�Z���}�X�^�R�[�h���X�g
	map<CString, set<long>>  m_textAddressMasterCityMap;     //!< �e�L�X�g�Z���}�X�^�s�s�R�[�h�}�b�v
	set<CString>m_readedFolderSet;                           //!< ���ɓǂ񂾃t�H���_
	CString m_textAddressMasterPath;                         //!< �e�L�X�g�Z���}�X�^�t�H���_
};
