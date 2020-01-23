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
	Arguments(){}

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

public:
	boost::program_options::variables_map m_vm;
	uh::tstring	m_zip_db;		//!< �X�֔ԍ��|���S���̐ڑ���
	uh::tstring	m_zip_schema;	//!< �X�֔ԍ��|���S���ڑ���̃X�L�[�}���iRONLY�ڑ��⃌�v���J�Ŏg�p�j
	uh::tstring	m_addr_db;		//!< �s�撬����\�|�C���g�o�͎��̏Z��DB�ڑ���
	uh::tstring	m_addr11_dir;	//!< 11���Z���f�[�^�i�[��f�B���N�g��
	uh::tstring	m_out_dir;		//!< �f�[�^�o�͐�f�B���N�g��
	uh::tstring	m_run_log;		//!< ���s���O
	uh::tstring	m_err_log;		//!< �G���[���O
};
