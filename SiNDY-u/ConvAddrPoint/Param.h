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
 * @file	Param.h
 */

// �{�N���X���g�p����ɂ͈ȉ���include���K�v
// �\�Ȍ���.h�ł�include�͔�����ׂ������A�قڑS�Ă�.cpp�ɉe�������邽�ߖ{�w�b�_��include���Ă��܂�
#include <TDC/useful_headers/tstring.h>
#include <TDC/useful_headers/boost_program_options.h>

#define D_TYPE				"type"
#define D_ADDR_DB			"addr_db"
#define D_ADDR_OWNER		"addr_owner"
#define D_MLANG_DB			"mlang_db"
#define D_MLANG_OWNER		"mlang_owner"
#define D_OUT_DIR			"out_dir"
#define D_RUN_LOG			"run_log"
#define D_ERR_LOG			"err_log"

#define D_TYPE_ADR_AUTHDB		"adr_authdb"	// �Z���E�ҏW�pDB����
#define D_TYPE_ADR_ESPODB		"adr_espoirdb"	// �Z���Eespoir-DB�����@��������
#define D_TYPE_POS_AUTHDB		"pos_authdb"	// �X�֔ԍ��E�ҏW�pDB����
#define D_TYPE_POS_ESPODB		"pos_espoirdb"	// �X�֔ԍ��Eespoir-DB�����@��������
#define D_TYPE_ADRMST_AUTHDB	"adrmst_authdb"	// �Z���n�}�X�^�E�ҏW�p-DB����

namespace outtype {
	enum EType {
		unknown       = 0,
		adr_authdb    = 1,	// �Z���E�ҏW�pDB����
		adr_espoirdb,		// �Z���Eespoir-DB�����@��������
		pos_authdb,			// �X�֔ԍ��E�ҏW�pDB����
		pos_espoirdb,		// �X�֔ԍ��Eespoir-DB�����@��������
		adrmst_authdb		// �Z���n�}�X�^�E�ҏW�p-DB����
	};
}

/**
* @class	CParam
* @brief	�p�����[�^�N���X
*/
class CParam
{
public:
	CParam(void);
	~CParam(void);

	/**
	* @brief	�I�v�V�������
	* @param	argc	[in]	�����̐�
	* @param	argv	[in]	����
	* @retval	true	����
	* @retval	false	���s
	*/
	bool parse(int argc, _TCHAR* argv[]);

	/**
	* @brief	�I�v�V�����擾
	* @return	�l
	*/
	std::string	getType(void) const { return uh::toStr(m_tstrType); };
	std::string	getAddrDB(void) const { return uh::toStr(m_tstrAddrDB); };
	std::string	getAddrOwner(void) const { return uh::toStr(m_tstrAddrOwner); };
	std::string	getMlangDB(void) const { return uh::toStr(m_tstrMlangDB); };
	std::string	getMlangOwner(void) const { return uh::toStr(m_tstrMlangOwner); };
	std::string	getOutDir(void) const { return uh::toStr(m_tstrOutDir); };
	std::string	getRunLog(void) const { return uh::toStr(m_tstrRunLog); };
	std::string	getErrLog(void) const { return uh::toStr(m_tstrErrLog); };

	/**
	* @brief	�I�v�V�����擾
	* @return	�I�v�V����������
	*/
	std::string	getOption(void) const;

	/**
	* @brief	�o�̓^�C�v�擾
	* @return	�l
	*/
	outtype::EType	getEType(void) const { return m_eType; };

private:
	/**
	* @brief	�K�{�`�F�b�N
	* @param	tstrValue	[in]	�l
	* @param	lpszDesc	[in]	�\����
	* @retval	true	����
	* @retval	false	���s
	*/
	bool check_required(const uh::tstring& tstrValue, LPCSTR lpszDesc);

	/**
	* @brief	�I�v�V�����P�s�擾
	* @param	cParam	[in]	�p�����[�^
	* @param	strValue	[in]	�l
	* @return	�I�v�V�����P�s������
	*/
	std::string	getOptionLine(const char* const cParam, const std::string& strValue) const;

private:
	// option
	uh::tstring	m_tstrType;			// �o�̓^�C�v
	uh::tstring	m_tstrAddrDB;		// �Z��DB�ڑ����
	uh::tstring	m_tstrAddrOwner;	// �Z��DB�̃I�[�i�[
	uh::tstring	m_tstrMlangDB;		// ������DB�ڑ����
	uh::tstring	m_tstrMlangOwner;	// ������DB�̃I�[�i�[
	uh::tstring	m_tstrOutDir;		// �o�̓f�B���N�g��
	uh::tstring	m_tstrRunLog;		// ���s���O
	uh::tstring	m_tstrErrLog;		// �G���[���O

	outtype::EType m_eType;			// �o�̓^�C�v
};
