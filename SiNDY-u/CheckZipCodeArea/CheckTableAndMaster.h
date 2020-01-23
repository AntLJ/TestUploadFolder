#pragma once

#include "CheckBase.h"
#include <AddrLib/JusyoHelper.h>
#include <TDC/sindylib_core/FieldMap.h>

class CheckTableAndMaster
	: public CheckBase
{
public:
	CheckTableAndMaster();
	virtual ~CheckTableAndMaster();

	/**
	 * @brief ������
	 * @retval true ����
	 * @retval false ���s
	 */
	bool Init();

	/**
	 * @brief �`�F�b�N���s
	 * @retval true �G���[�Ȃ�
	 * @retval false �G���[����
	 */
	bool Check();

private:
	/**
	 * @brief �Z���R�t���e�[�u���̏Z���R�[�h���Z���}�X�^�ɑ��݂��邩�ǂ������`�F�b�N
	 * @retval true �G���[�Ȃ�
	 * @retval false �G���[����
	 */
	bool CheckExistAddrCode();

	/**
	 * @brief �Z���R�t���e�[�u���̏Z���R�[�h�i5���j���Z���}�X�^�ɑ��݂��邩�ǂ������`�F�b�N
	 * @retval true �G���[�Ȃ�
	 * @retval false �G���[����
	 */
	bool CheckExistCityCode();

private:
	addr::CJusyoHelper m_jusyoMasterHelper; //!< �Z���}�X�^�擾�p�N���X
	addr::CJusyoHelper m_kajyoMasterHelper; //!< �����}�X�^�ڑ��p�N���X
	ITablePtr m_ipRelAddrTable; //!< �Z���R�t���e�[�u��
	sindy::CFieldMap m_relAddrTableFieldMap; //!< �Z���R�t���e�[�u���t�B�[���h�}�b�v
};

