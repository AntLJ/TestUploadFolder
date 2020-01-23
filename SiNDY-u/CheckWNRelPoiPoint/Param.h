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

#include "stdafx.h"
#include "Common.h"


// �v���O��������
char* const g_strIniFile	= "ini";
char* const g_strRunLog		= "run_log";
char* const g_strErrLog		= "err_log";
#define OPT_INI_FILE	"ini,i"
#define OPT_RUN_LOG		"run_log,r"
#define OPT_ERR_LOG		"err_log,e"

#define INI_REL_TBL		"REL_TBL"
#define INI_WN_DIR		"WN_DIR"
#define INI_POI_LIST	"POI_FILE_LIST"
#define INI_MESHLIST	"MESHLIST"
#define INI_CODE_LIST	"CONTENTS_CODE_LIST"
#define INI_NODECLASS	"NODECLASS"
#define INI_DISTANCE	"DISTANCE"
#define INI_PARAM		"PARAM"

#define DISTANCE_DEF    "50.0"

const CString CHECK_INTEGER_STR	= _T("1234567890");
const CString CHECK_DOUBLE_STR	= _T("1234567890.");

namespace po = boost::program_options;

class CParam
{
public:
	CParam(void);
	~CParam(void);

	/**
	 * @brief	�p�����[�^�`�F�b�N
	 * @param	argc	[in]	���̓p�����[�^��
	 * @param	argv[]	[in]	���̓p�����[�^
	 * @return					��������
	 */
	bool checkParam(int argc, _TCHAR* argv[]);

	/**
	 * @brief	�w��I�v�V�����\���p������擾
	 */
	std::string getOption();

	/**
	 * @brief	�ݒ�t�@�C���p�����[�^�擾
	 */
	std::string getIniFile() const { return uh::toStr(m_tstrIniFile); };

	/**
	 * @brief	���s���O�p�����[�^�擾
	 */
	std::string getRunLog() const { return uh::toStr(m_tstrRunLog); };

	/**
	 * @brief	�G���[���O�p�����[�^�擾
	 */
	std::string getErrLog() const { return uh::toStr(m_tstrErrLog); };

	/**
	 * @brief	�֘A�e�[�u���ݒ�l�擾
	 */
	std::string getRelTbl() const { return m_strRelTbl; };

	/**
	 * @brief	���H�E���s�҃m�[�h�i�[Dir�ݒ�l�擾
	 */
	std::string getWnDir() const { return m_strWnDir; };

	/**
	 * @brief	POI���X�g�t�@�C���ݒ�l�擾
	 */
	std::string getPoiList() const { return m_strPoiList; };

	/**
	 * @brief	���b�V�����X�g�t�@�C���ݒ�l�擾
	 */
	std::string getMeshList() const { return m_strMeshList; };

	/**
	 * @brief	�R���e���c�R�[�h���X�g�t�@�C���ݒ�l�擾
	 */
	std::string getCodeList() const { return m_strCodeList; };

	/**
	 * @brief	�m�[�h��ʐݒ�l�擾
	 */
	std::string getNodeClass() const { return m_strNodeClass; };

	/**
	 * @brief	����臒l�ݒ�l�擾
	 */
	std::string getDistance() const { return m_strDistance; };

	/**
	 * @brief	���E���n�n�p�����[�^�t�@�C���ݒ�l�擾
	 */
	std::string getParam() const { return m_strParam; };

private:
	/**
	 * @brief	�ݒ�t�@�C���`�F�b�N
	 * @return		��������
	 */
	bool checkIni();

	/**
	 * @brief	�w��I�v�V�����\���p������擾�i1Line�p�j
	 */
	std::string getOptionLine(const char* const cParam, std::string strValue);

	/**
	 * @brief	�ݒ�t�@�C�����ڕ\���p�����͎擾
	 */
	std::string getIniItem();

	/**
	 * @brief	�ݒ�t�@�C�����ڕ\���p������擾�i1Line�p�j
	 */
	std::string getIniItemLine(const char* const cParam, std::string strValue);

	/**
	 * @brief	�G���[�o��
	 */
	void outputCerrItem(std::string errKind, std::string itemName, std::string msg);

private:
	uh::tstring	m_tstrIniFile;		// �ݒ�t�@�C��						�i�t���p�X�j
	uh::tstring	m_tstrRunLog;		// ���s���O�t�@�C��					�i�t���p�X�j
	uh::tstring	m_tstrErrLog;		// �G���[�t�@�C��					�i�t���p�X�j

	std::string	m_strRelTbl;		// �֘A�e�[�u���t�@�C��				�i�t���p�X�j
	std::string	m_strWnDir;			// ���H�E���s�҃m�[�h�i�[�f�B���N�g��	�i�t���p�X�j
	std::string	m_strPoiList;		// POI�t�@�C�����X�g					�i�t���p�X�j
	std::string	m_strMeshList;		// ���b�V�����X�g					�i�t���p�X�j
	std::string	m_strCodeList;		// �R���e���c�R�[�h���X�g				�i�t���p�X�j
	std::string	m_strNodeClass;		// ���s�҃m�[�h���
	std::string	m_strDistance;		// �����`�F�b�N�p臒l
	std::string	m_strParam;			// ���E���n�n�p�����[�^�t�@�C��		�i�t���p�X�j
};
