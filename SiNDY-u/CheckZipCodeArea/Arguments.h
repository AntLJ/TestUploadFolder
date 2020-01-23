#pragma once

namespace exec_opt
{
	// �`�F�b�N���s���[�h�I�v�V��������
	extern const uh::tstring CHECK_TABLE;
	extern const uh::tstring CHECK_TABLEMASTER;
	extern const uh::tstring CHECK_TABLEADDR;
	extern const uh::tstring CHECK_POLYGON;
	extern const uh::tstring CHECK_POLYGONTABLE;
	extern const uh::tstring CHECK_POLYGONADDR;
	extern const uh::tstring CHECK_ALL;
}

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
	bool parse(int argc, _TCHAR * argv[]);

	/**
	 * @brief  �R�}���h���C�������̊m�F
	 * @param  optName  [in]  �I�v�V������
	 * @param  optValue [in]  �I�v�V�����l
	 * @param  bFile    [in]  �t�@�C���ł��邩�i�f�t�H���gfalse�j
	 * @retval true:    ����OK
	 * @retval false:   ����NG
	 */
	bool CheckParam(const char* const optName, const uh::tstring& optValue, bool bFile = false);

	/**
	 * @brief �S�w��I�v�V�����𕶎���Ƃ��Ď擾����
	 * @retval �w��I�v�V�����̕�����
	 */
	CString GetAllOptionInfo();

public:
	uh::tstring m_checkMode; //!< �`�F�b�N���[�h
	uh::tstring m_zipDb; //!< �X�֔ԍ��|���S��DB�ڑ���
	uh::tstring m_addrDb; //!< �Z���f�[�^DB�ڑ���
	uh::tstring m_charConv; //!< �����t���Z���R�[�h�ϊ��e�[�u��
	uh::tstring m_charConvTmp; //!< �����t���Z���R�[�h�ϊ����e�[�u��
	uh::tstring m_jusyoMaster; //!< �Z���}�X�^�ڑ�������
	uh::tstring m_kajyoMaster; //!< �����}�X�^�ڑ�������
	uh::tstring m_dupliRate; //!< �|���S���d������臒l
	uh::tstring m_mesh; //!< ���b�V�����X�g
	uh::tstring m_runLog; //!< ���s���O�p�X
	uh::tstring m_errLog; //!< �G���[���O�p�X
};
