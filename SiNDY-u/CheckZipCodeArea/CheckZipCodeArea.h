#pragma once

#include "Arguments.h"
#include "CheckBase.h"
#include "CheckAll.h"
#include "CheckTable.h"
#include "CheckTableAndMaster.h"
#include "CheckTableAndAddr.h"
#include "CheckPolygon.h"
#include "CheckPolygonAndTable.h"
#include "CheckPolygonAndAddr.h"

/**
 * @brief   ���C���N���X
 */
class CCheckZipCodeArea
{
public:

	/**
	 * @brief   �R���X�g���N�^
	 * @param   args [in]  ����
	 */
	CCheckZipCodeArea(const Arguments& args) : m_args(args) {}

	/**
	 * @brief   �f�X�g���N�^
	 */
	virtual ~CCheckZipCodeArea() {};

	/**
	 * @brief   ���s
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool execute();

private:

	/**
	 * @brief   ������
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool init();

	/**
	 * @brief �`�F�b�N�N���X�̃C���X�^���X��p�ӂ���
	 * @param checkMode [in] �`�F�b�N���[�h
	 */
	void SetCheckObject(const uh::tstring& checkMode);

	/**
	 * @brief �`�F�b�N���s
	 */
	bool DoCheck();

private:

	Arguments m_args; //!< ����
	std::shared_ptr<CheckBase> m_checkClassPtr; //!< �`�F�b�N�p�N���X�C���X�^���X
};

