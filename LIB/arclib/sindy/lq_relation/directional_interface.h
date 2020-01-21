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

#ifndef SINDY_DIRECTIONAL_LQ_RELATION_INTERFACE_H_
#define SINDY_DIRECTIONAL_LQ_RELATION_INTERFACE_H_

#include <sindy/lq_relation/interface.h>

namespace sindy {

/**
 * @brief �L�������N��֌W�m�F�N���X�B
 */
class directional_lq_relation_exist : virtual public lq_relation_exist
{
public:
///@name �������z�֐�
//@{
	/**
	 * @brief �����N�ɁA�w�肵�������������������N�񂪊֘A�t�����Ă��邩�B
	 *
	 * @param link_id [in] �����NID�B
	 * @param link_dir [in] �����N�����B
	 * @retval true �����N�񂪊֘A�t�����Ă���B
	 * @retval false �����N��͊֘A�t�����Ă��Ȃ��B
	 */
	virtual bool exist_directional_lq(const link_id_type& link_id, const link_dir_type& link_dir) const = 0;
//@}
};

/**
 * @brief �L�������N��֌W�m�F�E�ҏW�N���X�B
 */
class directional_lq_relation_mutable : virtual public directional_lq_relation_exist, virtual public lq_relation_mutable
{
};

} // namespace sindy

#endif // SINDY_DIRECTIONAL_LQ_RELATION_INTERFACE_H_
