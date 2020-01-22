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

#include <TDC/useful_headers/directory_util.h>
#include <TDC/useful_headers/boost_program_options.h>

/**
 * @brief �R�}���h���C�������N���X
 */
class Arguments
{
public:
	/**
	 * @brief �R���X�g���N�^
	 */
	Arguments() : m_output_header(false)
	{
	}

public:
	/**
	 * @brief �R�}���h���C�������̉��
	 * @param	argc [in]	�R�}���h���C�������̐�
	 * @param	argv [in]	�R�}���h���C�������̓��e
	 * @retval	true	����
	 * @retval	false	���s
	 */
	bool parse(int argc, TCHAR* argv[]);
	
private:
	/**
	 * @brief �R�}���h���C�������̉��
	 * @param strRunLog [in] ���s���O�p�X
	 * @param strErrLog [in] �G���[���O�p�X
	 * @retval true ����
	 * @retval false ���s
	 */
	bool initLog(const CString& strRunLog, const CString& strErrLog);

	/**
	 * @brief �R�}���h���C�������o�͗p������擾
	 * @return �R�}���h���C������������
	 */
	CString GetOption() const;

	/**
	 * @brief �I�v�V����������擾
	 * @param strParam [in] �I�v�V������
	 * @param strValue [in] �I�v�V�����l
	 * @return �I�v�V����������
	 */
	CString GetOptionLine(const CString& strParam, const CString& strValue) const;

public:
	boost::program_options::variables_map m_vm;
	uh::tstring	m_db_info;				//!< DB�ڑ����
	uh::tstring	m_conv_table_path;		//!< �ϊ��e�[�u���̃p�X
	uh::tstring	m_conv_field_path;		//!< �ϊ��t�B�[���h�Ή��\�̃p�X
	uh::tstring	m_cnv_param_path;		//!< ���W�ϊ��p�p�����[�^�t�@�C��(�������n�n��JGD2000�ϊ��p)
	uh::tstring	m_output_dir;			//!< �f�[�^�o�̓f�B���N�g��
	uh::tstring	m_output_delimiter;		//!< �o�̓f�[�^�̃f���~�^�icsv,tsv�j
	uh::tstring	m_output_encode;		//!< �o�̓f�[�^�̕����R�[�h�iutf-8,shift_jis�j
	uh::tstring	m_output_newline;		//!< �o�̓f�[�^�̉��s�R�[�h�iCR,LF,CRLF�j
	uh::tstring	m_output_georef;		//!< �o�̓f�[�^�̑��n�n�itky,jgd�j
	uh::tstring	m_output_shapetype;		//!< �o�̓f�[�^�̌`��^�C�v�inormal,wkt�j
	uh::tstring	m_output_polygontype;	//!< �o�̓f�[�^�̃|���S���̌����inormal,clockwise,anticlockwise�j
	bool m_output_header;

	uh::tstring	m_runlog_path;			//!< ���s���O
	uh::tstring	m_errlog_path;			//!< �G���[���O
};
