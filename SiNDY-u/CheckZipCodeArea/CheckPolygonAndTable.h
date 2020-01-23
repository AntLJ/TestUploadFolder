#pragma once

#include "CheckBase.h"
#include <TDC/sindylib_core/FieldMap.h>

class CheckPolygonAndTable
	: public CheckBase
{
public:
	CheckPolygonAndTable();
	virtual ~CheckPolygonAndTable();

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
	 * @brief �X�֔ԍ��|���S���̎��X�֔ԍ��ƏZ���R�t���e�[�u���̎��X�֔ԍ��̐����`�F�b�N
	 * @retval true �G���[�Ȃ�
	 * @retval false �G���[����
	 */
	bool CheckZipCode();

private:
	ITablePtr m_ipRelAddrTable; //!< �Z���R�t���e�[�u��
	IFeatureClassPtr m_ipZipCodeArea; //!< �X�֔ԍ��|���S���t�B�[�`���N���X
	sindy::CFieldMap m_relAddrTableFieldMap; //!< �Z���R�t���e�[�u���t�B�[���h�}�b�v
	sindy::CFieldMap m_zipCodeAreaFieldMap; //!< �Z���R�t���e�[�u���t�B�[���h�}�b�v
};

