#pragma once
#include "Output.h"

/**
* @brief   �W���o�͊Ǘ��N���X [bug 12017]
*/
class StdOutput :
	public Output
{
public:
	/**
	* @brief �R���X�g���N�^
	*/
	StdOutput() {}

	/**
	* @brief �f�X�g���N�^
	*/
	virtual ~StdOutput() {}

	/**
	* @brief �H�������o�͂���
	* @note �H�����X�g�Ɏw�肳�ꂽ�H�������o�͂���
	* @param msg[in] �H�����
	*/
	void writeRoadInfoMsg(const CString& msg);

	/**
	* @brief �G���[���b�Z�[�W���o�͂���
	* @note �������ɔ��������G���[�̓��e���o�͂���
	* @param msg[in] �G���[���b�Z�[�W
	*/
	void writeErrMsg(const CString& msg);

	/**
	* @brief �R���\�[���Ƀ��b�Z�[�W���o�͂���
	* @param msg[in] ���b�Z�[�W
	*/
	void writeConsole(const CString& msg);
};
