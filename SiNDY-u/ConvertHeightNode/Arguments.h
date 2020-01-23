#pragma once

#include <TDC/useful_headers/boost_program_options.h>
#include "common.h"


namespace po = boost::program_options;

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
		:m_buffer(0.0)
	{}

	/**
	 * @brief	�R�}���h���C�������̉��
	 * @param	argc [in]	�R�}���h���C�������̐�
	 * @param	argv [in]	�R�}���h���C�������̓��e
	 * @retval	true	����
	 * @retval	false	���s
	 */
	bool parse(int argc, TCHAR* argv[]);

	/**
	 *	@brief	�ݒ�t�@�C���w���v�o��
	 *	@note	�e��ݒ�Ɍ�肪�������ꍇ�Ɏd�l����֐��̂��߁A�߂�l�͕K��false
	 *	@retval false
	 */
	bool WriteUsage() const;

	/**
	 *	@brief	�����`�F�b�N
	 *	@param	vm [in] �擾�����R�}���h���C������
	 *	@retval	true ����
	 *	@retval false �s��
	 */
	bool CheckParam(const po::variables_map& vm) const;

	/**
	 *	@brief	�I�v�V�������݃`�F�b�N
	 *	@param	vm [in]		�I�v�V�����Q
	 *	@param	option [in]  �I�v�V������
	 *	@retval true ���݂���
	 *	@retval false ���݂��Ȃ�
	 */
	bool OptionExistChk(const po::variables_map& vm, LPCSTR option) const;

	/**
	 *	@brief	�t�@�C�����݃`�F�b�N
	 *	@param	vm [in]		�I�v�V�����Q
	 *	@param	option [in]	�I�v�V������
	 *	@retval true �t�@�C�������݂���B
	 *	@retval false �t�@�C�������݂��Ȃ��B
	 */
	bool FileExistChk(const po::variables_map& vm, LPCSTR option) const;

	/**
	 *	@brief	�f�B���N�g�����݃`�F�b�N
	 *	@param	vm [in]		�I�v�V�����Q
	 *	@param	option [in] �I�v�V������
	 *	@retval true �f�B���N�g�������݂���B
	 *	@retval false �f�B���N�g�������݂��Ȃ��B
	 */
	bool DirExistChk(const po::variables_map& vm, LPCSTR option) const;

	/**
	 *	@brief	�o�̓t�@�C���i�[�f�B���N�g�����݃`�F�b�N
	 *	@param	vm [in]		�I�v�V�����Q
	 *	@param	option [in] �I�v�V������
	 *	@retval true �f�B���N�g�������݂���.
	 *	@retval false �f�B���N�g�������݂��Ȃ��B
	 */
	bool FileDirExistChk(const po::variables_map& vm, LPCSTR option) const;

	/**
	 *	@brief	�p�����[�^�Z�b�g
	 *	@param	vm [in]
	 */
	void SetParam(const po::variables_map& vm);

	/**
	 *	@brief	�ݒ�t�@�C���`�F�b�N
	 *	@param	vm [in] �擾�����R�}���h���C������
	 *	@retval	true ����
	 *	@retval false �s��
	 */
	bool CheckIni(const po::variables_map& vm) const;

	/**
	 *	@brief	�ݒ�t�@�C���Z�b�g
	 *	@param	vm [in]
	 */
	void SetIni(const po::variables_map& vm);

	/**
	 *	@brief �c�[���o�[�W�����E�I�v�V�����̕\��
	 */
	void ProcInfoWrite(const po::variables_map& vm) const;

	/**
	 *	@brief	�I�v�V�����̕\��
	 *	@param	option [in] �I�v�V������
	 *	@param	val [in] �ݒ���e
	 */
	void WriteOptionVal(LPCSTR option, LPCTSTR val) const;

public:
	// ����
	uh::tstring m_iniFile;
	uh::tstring m_procLog;			//!< ���s���O
	uh::tstring m_errLog;			//!< �G���[���O

	// �ݒ�t�@�C��
	uh::tstring m_inputDir;			//!< �ϊ���Shape�i�[�f�B���N�g��
	uh::tstring m_heightNoodeDB;	//!< ���΍����m�[�h�i�[DB
	uh::tstring m_heightNodeSchema;	//!< ���΍����i�[�X�L�[�}
	uh::tstring m_baseMeshDB;		//!< BASEMESH�i�[DB
	uh::tstring m_baseMeshSchema;	//!< BASEMESH�i�[�X�L�[�}
	uh::tstring m_meshList;			//!< ���b�V�����X�g
	uh::tstring m_outputDir;		//!< �o�̓f�B���N�g��
	double m_buffer;				//!< ���H�E���s�҃����N�A���΍����m�[�h��Ԍ����p�o�b�t�@

};
