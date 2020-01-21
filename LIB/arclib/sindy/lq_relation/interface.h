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

#ifndef SINDY_LQ_RELATION_INTERFACE_H_
#define SINDY_LQ_RELATION_INTERFACE_H_

#include <arctl/field.h>
#include <sindy/schema/road.h>

namespace sindy {

struct lq_relation_base
{
	typedef arctl::field::value_t<schema::link_queue::kLinkID, long> link_id_type;
	typedef arctl::field::value_t<schema::link_queue::kLinkDir, long> link_dir_type;

	ARCTL_FIELD_VALUE_CLASS(
		field_value_type,
		((link_id_type))
		((link_dir_type))
	);
};

/**
 * @brief �����N��֌W�m�F�N���X�B
 */
class lq_relation_exist : public lq_relation_base
{
public:
// �R���X�g���N�^�ƃf�X�g���N�^
	virtual ~lq_relation_exist() {}

///@name �������z�֐�
//@{
	/**
	 * @brief �w�肵�������N�Ƀ����N��͊֘A�t���Ă��邩�B
	 *
	 * @param id [in] �����NID�B
	 * @retval true �����N�񂪊֘A�t�����Ă���B
	 * @retval false �����N��͊֘A�t�����Ă��Ȃ��B
	 */
	virtual bool exist_lq(const link_id_type& id) const = 0;
//@}
};

/**
 * @brief �����N��֌W�ҏW�N���X�B
 */
class lq_relation_edit : public lq_relation_base
{
public:
// ��`
	typedef arctl::field::value_t<schema::link_queue::kInfID, long> inf_id_type;

	ARCTL_FIELD_VALUE_CLASS(
		field_value_type,
		((lq_relation_base::field_value_type))
		((inf_id_type))
	);

// �R���X�g���N�^�ƃf�X�g���N�^
	virtual ~lq_relation_edit() {}

///@name �������z�֐�
//@{
	/**
	 * @brief �����N�Ƀ����N����֘A�t����B
	 *
	 * @param link_id [in] �����NID�B
	 * @param link_dir [in] �����N�����B
	 * @param inf_id [in] �����N��ID�B
	 * @retval true �֘A�t���ɐ����B
	 * @retval false �֘A�t���Ɏ��s�B���Ɋ֘A�t�����Ă���ꍇ���܂ށB
	 */
	virtual bool attach_lq(const link_id_type& link_id, const link_dir_type& link_dir, const inf_id_type& inf_id) = 0;

	/**
	 * @brief �����N����A�����N��ւ̊֘A�t������������B
	 *
	 * @param link_id [in] �����NID�B
	 * @param inf_id [in] �����N��ID�B
	 * @retval true �֘A�t���̉����ɐ����B
	 * @retval false �֘A�t���̉����Ɏ��s�B�֘A�t�����Ă��Ȃ��ꍇ���܂ށB
	 */
	virtual bool detach_lq(const link_id_type& link_id, const inf_id_type& inf_id) = 0;
//@}
};

/**
 * @brief �����N��֌W�m�F�E�ҏW�N���X�B
 */
class lq_relation_mutable : virtual public lq_relation_exist, virtual public lq_relation_edit
{
};

} // namespace sindy

#endif // SINDY_LQ_RELATION_INTERFACE_H_
