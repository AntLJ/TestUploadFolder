#pragma once

#include "CheckBase.h"
#include <TDC/sindylib_core/FieldMap.h>

class CheckTable
	: public CheckBase
{
public:
	CheckTable();
	virtual ~CheckTable();

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
	 * @brief �Z���R�[�h�̏d�����Ȃ����̃`�F�b�N
	 * @retval true �G���[�Ȃ�
	 * @retval false �G���[����
	 */
	bool CheckAddrCodeDuplication();

	/**
	 * @brief �s���Ȑ����󋵃R�[�h�������R�[�h�����Ȃ����̃`�F�b�N
	 * @retval true �G���[�Ȃ�
	 * @retval false �G���[����
	 */
	bool CheckModifyStatusCode();

private:
	ITablePtr m_ipRelAddrTable; //!< �Z���R�t���e�[�u��
	sindy::CFieldMap m_relAddrTableFieldMap; //!< �Z���R�t���e�[�u���t�B�[���h�}�b�v
};

