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

/**
 * @file	Parameter.h
 * @brief	�ݒ���i�[�\���̒�`
 */

#include <string>

/**
 * @enum	emConvMode
 * @brief	�ϊ����[�h
 */
enum emConvMode
{
	kNormalConv		= 1,	//!< �i�r����
	kOutsideConv	= 2		//!< �O���񋟌���(Bug7599)
};

/**
 * @struct	CParameter
 * @brief	�ݒ���
 */
struct CParameter
{
	emConvMode		m_emConvMode;			//!< �ϊ����[�h
	std::string		m_strAddrDBProp;		//!< �Z���nSDE�ڑ��v���p�e�B
	std::string		m_strMapDBProp;			//!< �Z���nSDE�ڑ��v���p�e�B

	std::string		m_strCityAdminFCName;	//!< �s�s�n�}�s���E�t�B�[�`���N���X��
	std::string		m_strGouPointFCName;	//!< ���|�C���g�t�B�[�`���N���X��
	std::string		m_strCitySiteFCName;	//!< �s�s�n�}�w�i�t�B�[�`���N���X��
	std::string		m_strBuildingFCName;	//!< �s�s�����t�B�[�`���N���X��
	std::string		m_strBuildingRoofFCName;	//!< �s�s��������t�B�[�`���N���X��
	std::string		m_strCityMeshFCName;	//!< �s�s�n�}���b�V���t�B�[�`���N���X��

	std::string		m_strCodeConvDB;		//!< �Z���R�[�h�ϊ�DB
	std::string		m_strYomiConvDB;		//!< ���~�����ϊ�DB

	std::string		m_strJusyoDB;			//!< �Z��DB�ڑ��v���p�e�B

	std::string		m_strPntDir;			//!< pntex�f�[�^�o�͐�
	std::string		m_strGouDir;			//!< gouex�f�[�^�o�͐�

	CParameter() : m_emConvMode(kNormalConv){}
};
