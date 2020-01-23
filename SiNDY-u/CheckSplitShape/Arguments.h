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
static const char* g_strBeforeShpPath = "before_shp";
static const char* g_strBeforeDBPath = "before_db";
static const char* g_strAfterShpPath = "after_shp";
static const char* g_strMeshDBPath = "mesh_db";
static const char* g_strMeshSchema = "mesh_schema";
static const char* g_strMeshlistPath = "meshlist_path";
static const char* g_strCheckMode = "check_mode";
static const char* g_strTky2JgdPath = "tky2jgd_path";
static const char* g_strMinSegment = "min_segment";
static const char* g_strRunlog = "runlog_path";
static const char* g_strErrlog = "errlog_path";

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

	CString Arguments::GetOption();

private:
	CString GetOptionLine(const CString& strParam, const CString& strValue);

public:
	uh::tstring BeforeShpPath;       //!< �ϊ��OShape�̃p�X
	uh::tstring BeforeDBPath;        //!< �ϊ��OShape�̃p�X
	uh::tstring AfterShpPath;        //!< �ϊ���Shape�̃p�X
	uh::tstring MeshDBPath;          //!< ���b�V��DB
	uh::tstring MeshSchema;          //!< ���b�V��DB�̃X�L�[�}
	uh::tstring MeshlistPath;        //!< ���b�V�����X�g�̃p�X
	uh::tstring CheckMode;           //!< �`�F�b�N���[�h�i1=�����N�����������N������, 2=���s�҃m�[�h�����E���s�҃����N�}�[�W����, 3=�����m�[�h���������j
	uh::tstring Tky2JgdPath;         //!< �n��p�����[�^�̃p�X
	uh::tstring MinSegment;          //!< �����Z�O�����g��[�P��m]
	uh::tstring ErrLogPath;          //!< �G���[���O�̃p�X
	uh::tstring RunLogPath;          //!< ���s���O�̃p�X

};
