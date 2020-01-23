#pragma once

#include "CheckBase.h"
#include "CheckTable.h"
#include "CheckTableAndMaster.h"
#include "CheckTableAndAddr.h"
#include "CheckPolygon.h"
#include "CheckPolygonAndTable.h"
#include "CheckPolygonAndAddr.h"

class CheckAll :
	public CheckBase
{
public:
	CheckAll();
	virtual ~CheckAll();

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
	CheckTable m_checkTable; //!< �Z���R�t���e�[�u�������`�F�b�N�N���X
	CheckTableAndMaster m_checkTableAndMaster; //!< �Z���R�t���e�[�u���ƏZ���}�X�^�̐����`�F�b�N�N���X
	CheckTableAndAddr m_checkTableAndAddr; //!< �Z���R�t���e�[�u���ƏZ���f�[�^�̐����`�F�b�N�N���X
	CheckPolygon m_checkPolygon; //!< �X�֔ԍ��|���S�������`�F�b�N�N���X
	CheckPolygonAndTable m_checkPolygonAndTable; //!< �X�֔ԍ��|���S���ƏZ���R�t���e�[�u���̐����`�F�b�N�N���X
	CheckPolygonAndAddr m_checkPolygonAndAddr; //!< �X�֔ԍ��|���S���ƏZ���f�[�^�̐����`�F�b�N�N���X
};

