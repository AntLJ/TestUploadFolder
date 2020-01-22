/*
 * Copyright (C) INCREMENT P CORP. All Rights Reserved.
 *
 * THIS SOFTWARE IS PROVIDED BY INCREMENT P CORP., WITHOUT WARRANTY OF
 * ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT.
 *
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDER BE LIABLE FOR ANY
 * CLAIM, DAMAGES SUFFERED BY LICENSEE AS A RESULT OF USING, MODIFYING
 * OR DISTRIBUTING THIS SOFTWARE OR ITS DERIVATIVES.
*/

#pragma once

namespace sindy_error
{
namespace walk_node
{

/**
 * @brief �G���[�R�[�h�̒�`
 */
enum class ErrorCode
{
	ROADNODEID_INPUT_ERROR = 0,       //!< ���H�m�[�hID���s�v�Ȏ�ʂȂ̂ɓ��͂���Ă���
	STATIONGATE_INPUT_ERROR,          //!< �w�o����ID���s�v�Ȏ�ʂȂ̂ɓ��͂���Ă���
	STATIONGATE_NONE_INPUT_ERROR,     //!< �w�o����ID���K�v�Ȏ�ʂȂ̂ɓ��͂���Ă��Ȃ�
	UNDERGROUNDGATE_INPUT_ERROR,      //!< �n���X�o����ID���s�v�Ȏ�ʂȂ̂ɓ��͂���Ă���
	UNDERGROUNDGATE_NONE_INPUT_ERROR, //!< �n���X�o����ID���K�v�Ȏ�ʂȂ̂ɓ��͂���Ă��Ȃ�
};

} // walk_node

} // sindy_error

/**
 * @brief �G���[�R�[�h�Ǘ��N���X
 * @note �G���[�R�[�h�ɑ΂���G���[���b�Z�[�W�������ŊǗ�����
 */
class ErrorCodeKeeper
{
public:
	/**
	 * @brief �R���X�g���N�^
	 */
	ErrorCodeKeeper(){}

	/**
	 * @brief �ێ����Ă���G���[���폜����
	 */
	void Reset();

	/**
	 * @brief �G���[�Ƃ��ĕێ�������̂�ǉ�����
	 * @param code [in] �ǉ�����G���[
	 */
	void Push(sindy_error::walk_node::ErrorCode code);

	/**
	 * @brief �G���[����ł������Ă��邩�ۂ�
	 * @retval true  �G���[����ȏ㎝���Ă���
	 * @retval false �G���[����������Ă��Ȃ�
	 */
	bool IsError();

	/**
	 * @brief �ێ����Ă���G���[�𕶎��񉻂��āAlist�ɓ���ĕԂ�
	 * @note for���̒��ŉ��x���Ăяo���A�Ƃ����悤�Ȏg�p�͑z�肵�Ă��Ȃ��̂ŁA
	 * �����ł������������񃊃X�g����蒼��
	 * @return �ێ����Ă���G���[�𕶎��񉻂��A���X�g����������
	 */
	std::vector<CString> GetStringList();
private:
	std::vector<sindy_error::walk_node::ErrorCode> m_errorCodes; //!< �G���[�R�[�h��ێ�
};
