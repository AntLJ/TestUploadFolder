#pragma once

/**
 * @brief �I�v�V������
*/
namespace arg {
	const char * const k_ipc_addr_db      = "ipc_addr_db";     // SiNDY�i�Z���j�ڑ���
	const char * const k_ipc_bg_db        = "ipc_bg_db";       // SiNDY�i�w�i�j�ڑ���
	const char * const k_mid_addr_db      = "mid_addr_db";     // �Z�����ԃt�@�C��
	const char * const k_mid_addr_layer   = "mid_addr_layer";  // �Z�����ԃt�@�C�����C����
	const char * const k_citycode_list    = "citycode_list";   // �s�撬���R�[�h���X�g
	const char * const k_point_dist       = "point_dist";      // �|�C���g����臒l
	const char * const k_run_log          = "run_log";         // ���s���O�t�@�C���p�X
	const char * const k_err_log          = "err_log";         // �G���[���O�t�H���_�p�X
};

/**
 * @brief �R�}���h���C�������N���X
 */
class Arguments
{
private:

/**
 * @brief �I�v�V�����l���
 */
enum ArgValueType {
	File,    // �t�@�C��
	Folder,  // �t�H���_
	Other    // ���̑�
};

public:
	/**
	 * @brief �R���X�g���N�^
	 */
	Arguments() {}

	/**
	 * @brief �R�}���h���C�������̉��
	 * @param	argc [in]	�R�}���h���C�������̐�
	 * @param	argv [in]	�R�}���h���C�������̓��e
	 * @retval	true	����
	 * @retval	false	���s
	 */
	bool parse(int argc, TCHAR* argv[]);

	/**
	 * @brief  �R�}���h���C�������̊m�F
	 * @param  optName  [in]  �I�v�V������
	 * @param  optValue [in]  �I�v�V�����l
	 * @param  type     [in]  �I�v�V�����l�̎��
	 * @retval true:    ����OK
	 * @retval false:   ����NG
	 */
	bool CheckParam( const char* const optName, const uh::tstring& optValue, ArgValueType type = ArgValueType::Other );


	/**
	 * @brief  �I�v�V�����ipoint_dist�j�l�̌���
	 * @param  optName  [in]  �I�v�V������
	 * @param  optValue [in]  �I�v�V�����l
	 * @retval true:    OK
	 * @retval false:   �G���[
	 */
	bool CheckDistValue( const char* const optName, const uh::tstring& optValue );

public:
	uh::tstring m_ipc_addr_db;     //!< SiNDY�i�Z���j�ڑ���
	uh::tstring m_ipc_bg_db;       //!< SiNDY�i�w�i�j�ڑ���
	uh::tstring m_ipc_mesh_db;     //!< SiNDY�i���b�V���j�ڑ���
	uh::tstring m_mid_addr_db;     //!< �Z�����ԃt�@�C��
	uh::tstring m_mid_addr_layer;  //!< �Z�����ԃt�@�C�����C����
	uh::tstring m_citycode_list;   //!< �s�撬���R�[�h���X�g
	uh::tstring m_point_dist;      //!< �|�C���g����臒l
	uh::tstring m_run_log;         //!< ���s���O�t�@�C���p�X
	uh::tstring m_err_log;         //!< �G���[���O�t�H���_�p�X
};
