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
 * @file modify_stamper.h
 * @brief �X�V��񍏈�N���X�e���v���[�g�B
 *
 * sindy::modify_stamper���`����w�b�_�B
 */
#ifndef SINDY_MODIFY_STAMPER_H_
#define SINDY_MODIFY_STAMPER_H_

#include <functional>
#include <sindy/schema/common.h>

namespace sindy {

/**
 * @brief �X�V��񍏈�N���X�C���^�t�F�[�X
 *
 * �I�u�W�F�N�g�ƍX�V���e�����l��n���A�I�u�W�F�N�g�ɍX�V�������󂷂�N���X�B<br>
 */
class modify_stamper : public std::binary_function<IRowBuffer*, schema::ipc_table::update_type::ECode, bool>
{
public:
//@{ @name �\�z/����
	modify_stamper() {}
	virtual ~modify_stamper() {}
//@}

//@{ @name ���Z
	/**
	 * @brief �X�V���̍�����s��
	 *
	 * @param ipRowBuffer [in] �X�V��񍏈�ΏۃI�u�W�F�N�g�B
	 * @param eUpdateType [in] �X�V���e�B
	 * @retval true ����ɐ��������B
	 * @retval false ����Ɏ��s�����B
	 *
	 * @attention
	 * ���󎸔s��ʍ�����̂͗�O�̖�ڂł���A�^���l��Ԃ��Ă���̂͌Â������̖��c�ł���B
	 */
	bool operator()(IRowBuffer* ipRowBuffer, schema::ipc_table::update_type::ECode eUpdateType) const
	{
		return stamp(ipRowBuffer, eUpdateType);
	}
//@}

protected:
//@{ @name �������z�֐�
	/**
	 * @brief �X�V���̍�����s���B
	 *
	 * @param ipRowBuffer [in] �X�V��񍏈�ΏہB
	 * @param eUpdateType [in] �X�V���e�B
	 * @retval true ����ɐ��������B
	 * @retval false ����Ɏ��s�����B
	 */
	virtual bool stamp(IRowBuffer* ipRowBuffer, schema::ipc_table::update_type::ECode eUpdateType) const = 0;
//@}
};

} // namespace sindy

#endif // SINDY_MODIFY_STAMPER_H_
