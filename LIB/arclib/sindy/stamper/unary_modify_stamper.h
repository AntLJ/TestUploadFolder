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

/**
 * @file unary_modify_stamper.h
 * @brief �P���ōX�V��񍏈�N���X�e���v���[�g�B
 */
#ifndef SINDY_UNARY_MODIFY_STAMPER_H_
#define SINDY_UNARY_MODIFY_STAMPER_H_

#include <functional>
#include <sindy/stamper/modify_stamper.h>
//#include <SiNDYAttrCommunity.h>

namespace sindy {

/**
 * @brief �X�V��񍏈�N���X
 *
 * �n���ꂽ�I�u�W�F�N�g��@ref modify_stamper�p���N���X�ɓn���A�X�V�������󂷂�N���X�B<br>
 * ���󂷂�@ref term_update_type�̓R���X�g���N�^�Œ�߂�B
 */
class unary_modify_stamper : public std::unary_function<IRowBuffer*, bool>
{
public:
//@{ @name �\�z/����
	/**
	 * @param pModifyStamper [in,ref,nullable] ���ۂɍX�V���̍�����s���֐��I�u�W�F�N�g�Bnull��n�����ꍇ�A����͍s��Ȃ��B
	 * @param eUpdateType [in] �X�V���e�B
	 *
	 * @note
	 * @a pModifyStamper��null�������Ȃ������A����āu�X�V�������󂵂Ȃ��I�u�W�F�N�g�v�𐶐����Ă��܂�Ȃ����A���S�ł͂���B<br>
	 * �M�҂́u�}�[�W�������ɂ͍X�V�������󂷂ׂ��ł͂Ȃ��v�ƍl���Ă��邽�߁Anull�ɂ��񍏈�ݒ���c���Ă���B
	 */
	unary_modify_stamper(const modify_stamper* pModifyStamper, schema::ipc_table::update_type::ECode eUpdateType) :
	m_pModifyStamper(pModifyStamper),
	m_eUpdateType(eUpdateType)
	{
	}

	/**
	 * @param rModifyStamper [in,ref] ���ۂɍX�V���̍�����s���֐��I�u�W�F�N�g�B
	 * @param eUpdateType [in] �X�V���e�B
	 */
	unary_modify_stamper(const modify_stamper& rModifyStamper, schema::ipc_table::update_type::ECode eUpdateType) :
	m_pModifyStamper(&rModifyStamper),
	m_eUpdateType(eUpdateType)
	{
	}

	~unary_modify_stamper()
	{
	}
//@}

//@{ @name ���Z
	/**
	 * @brief �X�V���̍�����s��
	 *
	 * @param ipRowBuffer [in] ����ΏۃI�u�W�F�N�g�B
	 * @retval true ����ɐ����������A���󂵂Ȃ������B
	 * @retval false ����Ɏ��s�����B
	 */
	bool operator()(IRowBuffer* ipRowBuffer) const
	{
		return ! m_pModifyStamper || (*m_pModifyStamper)(ipRowBuffer, m_eUpdateType);
	}
//@}

private:
// �ϐ�
	const modify_stamper* m_pModifyStamper; ///< �X�V��񍏈�I�u�W�F�N�g�B
	const schema::ipc_table::update_type::ECode m_eUpdateType; ///< �X�V���e�B
};

} // namespace sindy

#endif // SINDY_UNARY_MODIFY_STAMPER_H_
