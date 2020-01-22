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

// �v���O��������
static const char* g_strShapeDir = "shp_dir";
static const char* g_strMeshlist = "meshlist";
static const char* g_strSpatialDB = "spatial_db";
static const char* g_strSpatialDBSchema = "spatial_db_schema";
static const char* g_strRunLog = "run_log";
static const char* g_strErrLog = "err_log";

/**
 * @brief �R�}���h���C�������N���X
 */
class Arguments
{
public:
	/**
	 * @brief �R���X�g���N�^
	 */
	Arguments()
	{
	}

	/**
	 * @brief �R�}���h���C�������̉��
	 * @param	argc [in]	�R�}���h���C�������̐�
	 * @param	argv [in]	�R�}���h���C�������̓��e
	 * @retval	true	����
	 * @retval	false	���s
	 */
	bool parse(int argc, TCHAR* argv[]);

	CString GetOption();

private:
	CString GetOptionLine(const CString& strParam, const CString& strValue);

public:
	uh::tstring strShapeDir;        //!< Shape�̃p�X
	uh::tstring strMeshlist;        //!< ���b�V�����X�g�̃p�X
	uh::tstring strSpatialDB;       //!< ��ԎQ�Ǝ擾�pDB�p�X
	uh::tstring strSpatialDBSchema; //!< ��ԎQ�Ǝ擾�pDB�̃X�L�[�}��
	uh::tstring strRunLog;          //!< ���s���O�p�X
	uh::tstring strErrLog;          //!< �G���[���O�p�X
};
