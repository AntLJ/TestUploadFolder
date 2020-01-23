#pragma once

#include "CheckBase.h"

class CheckTableAndAddr
	: public CheckBase
{
public:
	CheckTableAndAddr();
	virtual ~CheckTableAndAddr();

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
	 * @brief �R�t���e�[�u���̏Z���R�[�h�ɊY������f�[�^��SiNDY��ɑ��݂��Ă��邩�ǂ������`�F�b�N
	 * @retval true �G���[�Ȃ�
	 * @retval false �G���[����
	 */
	bool CheckTableToAddr();

	/**
	 * @brief �s���E��11���R�[�h���R�t���e�[�u���ɑ��݂��Ă��邩�ǂ������`�F�b�N
	 * @retval true �G���[�Ȃ�
	 * @retval false �G���[����
	 */
	bool CheckCityAdminToTable();

private:
	addr::AddrRecHelper m_addrRecHelper; //!< �Z�����R�[�h�����⏕�N���X
	sindy::CTableFinder m_addrFinder; //!< �Z���f�[�^(�s���E�A���ACS)�擾�p
	sindy::CTableFinder m_relAddrFinder; //!< �R�t���e�[�u���擾�p
};
