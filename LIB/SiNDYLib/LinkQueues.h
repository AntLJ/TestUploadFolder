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

// LinkQueues.h: CLinkQueues �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(_LINKQUEUES_H_)
#define _LINKQUEUES_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LinkQueue.h"

namespace sindy {

class SINDYLIB_API CLinkQueues
{
public:
	CLinkQueues();
	virtual ~CLinkQueues();

	/**
	 * @brief ���H�ɑ����Ă��郊���N����擾����
	 * @param	lValue	[in]		�����N����������������H�� OBJECTID
	 * @param	eCode	[in]		���������������N��̎��
	 * @retval	sindyErr_NoErr		�����F�����o m_mapData �Ɏ擾���������N��̏����i�[
	 * @retval	����ȊO�̃G���[	���s�F�K��Ɉᔽ�������
	*/
	errorcode::sindyErrCode SelectByRoadID(long lValue, sindyTableType::ECode emLinkQueue );

private:
	std::map<long, CLinkQueue*>		m_mapLQs;
};

}

#endif // !defined(_LINKQUEUES_H_)
