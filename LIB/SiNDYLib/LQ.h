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

// LQ.h: CLQ �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LQ_H__45408FF8_9E86_422A_A325_D808888844F2__INCLUDED_)
#define AFX_LQ_H__45408FF8_9E86_422A_A325_D808888844F2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Row.h"

namespace sindy {
namespace link_queue {

class SINDYLIB_API CLQ : public CRow
{
	ROWCLASS_CONSTRUCTOR( CLQ, unknown )

	bool operator<( const CLQ& obj )
	{
		return (GetSequence() < obj.GetSequence());
	}
	bool operator>( const CLQ& obj )
	{
		return (GetSequence() > obj.GetSequence());
	}

	//@{ @name �����擾�֐�
	/**
	 * @brief Road_Link�̃����N��ID��Ԃ�
	 * @retval -1 �G���[
	 * @retval -1�ȊO Road_Link�̃����N��ID
	 */
	inline long GetLinkId() const
	{
		return GetLongValueByFieldName( schema::link_queue::kLinkID, -1 );
	}
	
	/**
	 * @brief �����R�[�h��Ԃ�
	 * @retval -1 �G���[
	 * @retval -1�ȊO �����R�[�h
	 */
	inline long GetLinkDir() const
	{
		return GetLongValueByFieldName( schema::link_queue::kLinkDir, -1 );
	}

	/**
	 * @brief ���Ԃ�Ԃ�
	 * @retval -1 �G���[
	 * @retval -1�ȊO ����
	 */
	inline long GetSequence() const
	{
		return GetLongValueByFieldName( schema::link_queue::kSequence, -1 );
	}

	/**
	 * @brief �I�[�t���O��Ԃ�
	 * @retval -1 �G���[
	 * @retval -1�ȊO �I�[�t���O
	 */
	inline long GetLastLink() const
	{
		return GetLongValueByFieldName( schema::link_queue::kLastLink, -1 );
	}

	/**
	 * @brief InfID��Ԃ�
	 * @retval -1 �G���[
	 * @retval -1�ȊO InfID
	 */
	inline long GetInfId() const
	{
		return GetLongValueByFieldName( schema::link_queue::kInfID, -1 );
	}
	//@}

	//@{ @name �����ݒ�֐�
	/**
	 * @brief Road_Link�̃����N��ID��ݒ肷��
	 * @retval -1 �G���[
	 * @retval -1�ȊO Road_Link�̃����N��ID
	 */
	errorcode::sindyErrCode SetLinkId(long lValue)
	{
		return SetLongValueByFieldName( schema::link_queue::kLinkID, lValue );
	}
	
	/**
	 * @brief �����R�[�h��ݒ肷��
	 * @retval -1 �G���[
	 * @retval -1�ȊO �����R�[�h
	 */
	errorcode::sindyErrCode SetLinkDir(long lValue)
	{
		return SetLongValueByFieldName( schema::link_queue::kLinkDir, lValue );
	}

	/**
	 * @brief ���Ԃ�ݒ肷��
	 * @retval -1 �G���[
	 * @retval -1�ȊO ����
	 */
	errorcode::sindyErrCode SetSequence(long lValue)
	{
		return SetLongValueByFieldName( schema::link_queue::kSequence, lValue );
	}

	/**
	 * @brief �I�[�t���O��ݒ肷��
	 * @retval -1 �G���[
	 * @retval -1�ȊO �I�[�t���O
	 */
	errorcode::sindyErrCode SetLastLink(long lValue)
	{
		return SetLongValueByFieldName( schema::link_queue::kLastLink, lValue );
	}

	/**
	 * @brief InfID��ݒ肷��
	 * @retval -1 �G���[
	 * @retval -1�ȊO InfID
	 */
	errorcode::sindyErrCode SetInfId(long lValue)
	{
		return SetLongValueByFieldName( schema::link_queue::kInfID, lValue );
	}
	//@}
};

} // link_queue

} // sindy

#endif // !defined(AFX_LQ_H__45408FF8_9E86_422A_A325_D808888844F2__INCLUDED_)
