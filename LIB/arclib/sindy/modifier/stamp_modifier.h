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
 * @file stamp_modifier.h
 * @brief �X�V���X�V��񍏈�N���X��`
 *
 * @section lib_stamp_modifier �X�V���X�V��񍏈�N���X 
 * - sindy::stamp_modifier - �X�V���ɍX�V�������󂷂�B
 */
#ifndef SINDY_STAMP_MODIFIER_H_
#define SINDY_STAMP_MODIFIER_H_

#include <sindy/stamper/unary_modify_stamper.h>

namespace sindy {

/**
 * @brief �X�V���X�V��񍏈�N���X
 *
 * �X�V�֐���@ref sindy::unary_modify_stamper��g�����A�X�V���ɍX�V�������󂷂�N���X�B
 *
 * @par �e���v���[�g����:
 * @a TFunction : �X�V�p�P���֐��B�n���ꂽ�I�u�W�F�N�g�ɍX�V���������ꍇtrue���A�����Ȃ������ꍇfalse��Ԃ��B
 */
template <typename TFunction>
class stamp_modifier
{
public:
///@name �\�z/����
//@{
	/**
	 * @param cFunction [in] �X�V�֐��B
	 * @param pModifyStamper [in,ref,nullable] �X�V��񍏈�I�u�W�F�N�g�B�X�V���̍�����s��Ȃ��ꍇnull�B
	 */
	stamp_modifier(TFunction cFunction, const unary_modify_stamper* pModifyStamper) :
	m_Function(cFunction),
	m_pModifyStamper(pModifyStamper)
	{
	}

	/**
	 * @param cFunction [in] �X�V�֐��B
	 * @param rModifyStamper [in,ref] �X�V��񍏈�I�u�W�F�N�g�B
	 */
	stamp_modifier(TFunction cFunction, const unary_modify_stamper& rModifyStamper) :
	m_Function(cFunction),
	m_pModifyStamper(&rModifyStamper)
	{
	}
//@}

///@name ���Z
//@{
	/**
	 * @brief �X�V���ׂ��ꂽ�ꍇ�Ɍ���A�X�V�������󂷂�B
	 *
	 * @retval true �X�V���ׂ���A�X�V���̍���ɐ����i�����I�ɍ��󂵂Ȃ��̂��܂ށj�������A���󂵂Ȃ������B
	 * @retval false �X�V�Ɏ��s�����B���邢�͍X�V���̍���Ɏ��s�����B
	 */
	template <typename TRow>
	bool operator()(TRow const& rRow) const
	{
		return m_Function(rRow) && (m_pModifyStamper ? (*m_pModifyStamper)(rRow) : true);
	}
//@}

private:
// �ϐ�
	TFunction m_Function; ///< �X�V�֐��B
	const unary_modify_stamper* m_pModifyStamper; ///< �X�V��񍏈�I�u�W�F�N�g�B
};

/**
 * @defgroup make_stamp_modifier_group stamp_modifier �����֐�
 */
//@{
template <typename TFunction>
stamp_modifier<TFunction> make_stamp_modifier(TFunction cFunction, const unary_modify_stamper* pModifyStamper = 0)
{
	return stamp_modifier<TFunction>(cFunction, pModifyStamper);
}

template <typename TFunction>
stamp_modifier<TFunction> make_stamp_modifier(TFunction cFunction, const unary_modify_stamper& rModifyStamper)
{
	return stamp_modifier<TFunction>(cFunction, rModifyStamper);
}
//@}

} // namespace sindy

#endif // SINDY_STAMP_MODIFIER_H_
