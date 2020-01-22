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

#include "Utility.h"
#include <TDC/useful_headers/boost_program_options.h>

namespace po = boost::program_options;

/**
 * @brief �I�v�V������
*/
namespace arg {
	const char * const k_imp_type    = "imp_type";
	const char * const k_edt_addr_db = "edt_addr_db";
	const char * const k_old_addr_db = "old_addr_db";
	const char * const k_gs_addr_db   = "gs_addr_db";
	const char * const k_run_log = "run_log";
	const char * const k_err_log = "err_log";
	const char * const k_citylist = "citylist";
	const char * const k_extcode_list = "extcode_list";
};

/**
 * @brief imp_type�I�v�V�����l
*/
namespace arg_imp_type {
	const char * const k_placename_rep = "placename_rep";  // �n����\�_�|�C���g����
	const char * const k_addr_poly = "addr_poly";          // �Z���|���S������
	const char * const k_addr_point = "addr_point";        // �Z���|�C���g����
	const char * const k_addr_all = "addr_all";            // �Z���|���S���{�Z���|�C���g����
};

/**
 * @brief �R�}���h���C�������N���X
 */
class Arguments
{
public:
	/**
	 * @brief �R���X�g���N�^
	 */
	Arguments() : m_impType(Utility::ExecuteType::None) {}

	/**
	 * @brief �f�X�g���N�^
	 */
	virtual ~Arguments() {}

	/**
	 * @brief �R�}���h���C�������̉��
	 * @param	argc [in]	�R�}���h���C�������̐�
	 * @param	argv [in]	�R�}���h���C�������̓��e
	 * @retval	true	����
	 * @retval	false	���s
	 */
	bool parse(int argc, TCHAR* argv[]);

	/**
	 * @brief  �R�}���h���C�������iimp_type�I�v�V�����p�j�̊m�F
	 * @param  vm       [in]  �I�v�V������͌���
	 * @param  optName  [in]  �I�v�V������
	 * @param  optValue [in]  �I�v�V�����l
	 * @retval true:    ����OK
	 * @retval false:   ����NG
	 */
	bool CheckImpTypeParam
	(
		const po::variables_map& vm,
		const char* const optName,
		CString& optValue
	);

	/**
	 * @brief  �R�}���h���C�������̊m�F
	 * @param  vm       [in]  �I�v�V������͌���
	 * @param  optName  [in]  �I�v�V������
	 * @param  optValue [in]  �I�v�V�����l
	 * @param  bFile    [in]  �t�@�C���ł��邩�i�f�t�H���gfalse�j
	 * @retval true:    ����OK
	 * @retval false:   ����NG
	 */
	bool CheckParam
	( 
		const po::variables_map& vm,
		const char* const optName,
		CString& optValue,
		bool bFile = false
	);

public:
	CString  m_impTypeStr;             //!< �C���|�[�g�^�C�v�i������j
	Utility::ExecuteType  m_impType;   //!< �C���|�[�g�^�C�v�i�񋓌^�j
	CString  m_edtAddrDb;              //!< SiNDY�i�Z���E�ҏW�j�ڑ���
	CString  m_oldAddrDb;              //!< SiNDY�i���Z���E�ҏW�j�ڑ���
	CString  m_gsAddrDb;               //!< GS�[�i�f�[�^(�Z���j�ڑ���
	CString  m_runLog;                 //!< ���s���O
	CString  m_errLog;                 //!< �G���[���O
	CString  m_cityList;               //!< �s�撬���R�[�h���X�g
	CString  m_extcodeList;            //!< �g���R�[�h���X�g
};
