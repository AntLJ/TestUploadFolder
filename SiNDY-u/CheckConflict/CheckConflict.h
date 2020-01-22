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
#include "Logger.h"
#include <TDC/sindylib_core/Workspace.h>
namespace sindy {
	class CTableFinder;
	class CTableContainer;
	typedef boost::shared_ptr<CTableContainer> CSPTableContainer;
}

/**
 * @brief   ���C���N���X
 */
class CheckConflict
{

public:

	/**
	 * @brief   �R���X�g���N�^
	 * @param   args [in]  ����
	 */
	CheckConflict( const Arguments& args ){
		m_args = args;
	}

	/**
	 * @brief   �f�X�g���N�^
	 */
	virtual ~CheckConflict(){}

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
	
	/**
	 * @brief   ���O�t�@�C���̏�����
	 * @retval true  ����
	 * @retval false ���s
	 */
	bool logInit();

	/**
	 * @brief   �R���t���N�g�`�F�b�N
	 */
	void checkConflict();

	/**
	 * @brief   �ݒ�t�@�C���ǂݍ���
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool readConfig();

	/**
	 * @brief   �e�[�u���t�@�C���_������
	 * @param   workspace [in] ���[�N�X�y�[�X
	 * @param   table [in] �e�[�u����
	 * @param   finder [out] �e�[�u���t�@�C���_
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool initTableFinder(const sindy::CWorkspace& workspace, const std::string& table, sindy::CTableFinder& finder);

	/**
	 * @brief   �����Ώۃ��R�[�h�Q���������A�L���b�V������
	 * @param   roopCount [in]     ���[�v�񐔁iEXECCOUNT���ƂɃ��[�v���񂷁j
	 * @param   table [in, out]    �e�[�u���R���e�i
	 * @param   listTgtOIDs [out]  �����Ώ�OBJECTID�Q
	 */
	void getCheckRecords(const long roopCount, sindy::CSPTableContainer& table, std::list<long>& listTgtOIDs);

	/**
	 * @brief   2�e�[�u���̃��R�[�h���m���r���A�l�ɍ��ق����邩���`�F�b�N����
	 * @param   listTgtOIDs [in] �����Ώ�OBJECTID�Q
	 * @param   tgtT  [in] �`�F�b�N�Ώۃe�[�u��
	 * @param   diffT [in] ����(��r��)�e�[�u��
	 */
	void checkConflict(const std::list<long>& listTgtOIDs, const sindy::CSPTableContainer& tgtT, const sindy::CSPTableContainer& diffT);

private:

	Arguments m_args;      //!< ����
	sindy::CWorkspace m_tgtWork;  //!< �`�F�b�N�Ώېڑ���
	sindy::CWorkspace m_diffWork; //!< ����DB�ڑ���
	std::list<std::string> m_tgtTables;  //!< �����Ώۃe�[�u���Q
};

