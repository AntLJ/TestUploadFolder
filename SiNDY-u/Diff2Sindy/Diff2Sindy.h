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

#include "Arguments.h"
#include "Workspace.h"
#include <WinLib/VersionInfo.h>

using namespace sindy;


/**
 * @brief   ���C���N���X
 */
class CDiff2Sindy
{

public:

	/**
	 * @brief   �R���X�g���N�^
	 * @param   args [in]  ����
	 */
	CDiff2Sindy( Arguments args )
	:m_args(args)
	{
		// CVersion���{�c�[���Ŏg�p��������擾
		CVersion version;
		m_InternalName = version.GetInternalName();		//!< �c�[������
		m_FileVersion = version.GetFileVersion();		//!< �t�@�C���o�[�W����
		m_ProductVersion = version.GetProductVersion();	//!< �v���_�N�g�o�[�W����
	};

	/**
	 * @brief   ���s
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool execute();

private:

	/**
	 * @brief   ������
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool init();

private:

	/**
	 * @brief  �������O��\��
	 */
	void outputExecInfo() const;

	/**
	 * @brief  ������DB�����DB�ɂ���
	 * @param meshes [in] �����Ώۃ��b�V�����X�g
	 * @retval true ��������
	 */
	bool convertDB( const std::vector<uh::tstring>& meshes );

	/**
	 * @brief  �אڃ}�[�W�����{����
	 * @param meshes [in] �����Ώۃ��b�V�����X�g
	 * @retval true:��������
	 */
	bool mergeDB( const std::vector<uh::tstring>& meshes );

	/**
 	 * @brief �N���b�s���O���������{����
	 * @param meshes [in] �����Ώۃ��b�V�����X�g
	 * @retval true ��������
	*/
	bool clipDB( const std::vector<uh::tstring>& meshes );

	mutable Arguments m_args;				//!< ����
	CString m_InternalName;					//!< �c�[������
	CString m_FileVersion;					//!< �t�@�C���o�[�W����
	CString m_ProductVersion;				//!< �v���_�N�g�o�[�W����
};

