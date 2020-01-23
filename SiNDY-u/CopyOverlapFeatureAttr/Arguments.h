#pragma once


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
	* @brief �f�X�g���N�^
	*/
	virtual ~Arguments(){}

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
	 * @param  bFile    [in]  �t�@�C���ł��邩�i�f�t�H���gfalse�j
	 * @retval true:    ����OK
	 * @retval false:   ����NG
	 */
	bool CheckParam( const char* const optName, const uh::tstring& optValue, bool bFile = false ) const;
public:
	uh::tstring  m_setting;    //!< �ݒ�t�@�C��
	uh::tstring  m_runlog;     //!< ���s���O
	uh::tstring  m_errlog;     //!< �G���[���O
};
