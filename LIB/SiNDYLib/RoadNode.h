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
 * @file CRoadNode.h
 * @brief <b>���H�m�[�h�N���X��`�w�b�_�t�@�C��</b>\n
 * @version $Id$
 */

#ifndef _ROADNODE_H_
#define _ROADNODE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ErrorCode.h"
#include "Feature.h"

namespace sindy {
namespace road_node {

/**
 * @class CRoadNode
 * @brief ���H�m�[�h�t�B�[�`���N���X
 */

class SINDYLIB_API CRoadNode : public CFeature  
{
public:
	ROWCLASS_CONSTRUCTOR( CRoadNode, road_node )
public:
	//@{ @name �����擾�֐�
	/**
	 * @brief �m�[�h��ʂ�Ԃ�
	 * @retval -1 �G���[����NULL
	 */
	schema::road_node::node_class::ECode GetNodeClassCode() const
	{
		return (schema::road_node::node_class::ECode)GetLongValueByFieldName( schema::road_node::kNodeClass, -1 );
	}
		
	/**
	 * @brief �����_�������̂�Ԃ�
	 * @param nLen		[in]	lpszName�̎��[�\������
	 * @param lpszName	[out]	�̈���m�ۂ���Ă���o�b�t�@
	 * @return �G���[����NULL�̏ꍇ�͋󕶎���
	 */
	LPCTSTR GetNameKanji( int nLen, LPTSTR lpszName ) const
	{
		return GetStringValueByFieldName( schema::road_node::kNameKanji, nLen, lpszName );
	}	

	/**
	 * @brief �����_�J�i���̂�Ԃ�
	 * @param nLen		[in]	lpszName�̎��[�\������
	 * @param lpszName	[out]	�̈���m�ۂ���Ă���o�b�t�@
	 * @return �G���[����NULL�̏ꍇ�͋󕶎���
	 */
	LPCTSTR GetNameYomi( int nLen, LPTSTR lpszName ) const
	{
		return GetStringValueByFieldName( schema::road_node::kNameYomi, nLen, lpszName );
	}	
	
	/**
	 * @brief ���΍����R�[�h��Ԃ�
	 * @retval -1 �G���[����NULL
	 */
	schema::road_node::height_code::ECode GetHeightCode() const
	{
		return (schema::road_node::height_code::ECode)GetLongValueByFieldName( schema::road_node::kHeightCode, -1 );
	}
	
	/**
	 * @brief ���΍�����Ԃ�
	 * @retval -1 �G���[
	 * @retval -1�ȊO ���΍���
	 */
	long GetHeight() const
	{
		return GetLongValueByFieldName( schema::road_node::kHeight, -1 );
	}
	
	/**
	 * @brief �m�[�h�M���@�R�[�h��Ԃ�
	 * @retval -1 �G���[����NULL
	 */
	schema::road_node::signal::ECode GetSignalCode() const
	{
		return (schema::road_node::signal::ECode)GetLongValueByFieldName( schema::road_node::kSignal, -1 );
	}
	
	/**
	 * @brief ���ԏ�ID��Ԃ�
	 * @retval -1 �G���[
	 * @retval -1�ȊO ���ԏ�ID
	 */
	long GetParkingID() const
	{
		return GetLongValueByFieldName( schema::road_node::kParkingID, -1 );
	}	
	
	//@}

	//@{ @name �����ݒ�֐�

	/**
	 * @brief �m�[�h��ʂ�ݒ肷��
	 * @brief NULL �s����
	 * @retval �����@sindyErr_NoErr
	 * @retval ���s�@����ȊO�� sindyErrCode
	 */
	errorcode::sindyErrCode SetNodeClassCode(schema::road_node::node_class::ECode ecode)
	{
		return SetLongValueByFieldName( schema::road_node::kNodeClass, ecode );
	}
		
	/**
	 * @brief �����_�������̂�ݒ肷��
	 * @param lpszName	[out]	�̈���m�ۂ���Ă���o�b�t�@
	 * @brief NULL ����
	 * @retval �����@sindyErr_NoErr
	 * @retval ���s�@����ȊO�� sindyErrCode
	 */
	errorcode::sindyErrCode SetNameKanji( LPTSTR lpszName )
	{
		return SetStringValueByFieldName( schema::road_node::kNameKanji, lpszName );
	}	

	/**
	 * @brief �����_�J�i���̂�ݒ肷��
	 * @param lpszName	[out]	�̈���m�ۂ���Ă���o�b�t�@
	 * @brief NULL ����
	 * @retval �����@sindyErr_NoErr
	 * @retval ���s�@����ȊO�� sindyErrCode
	 */
	errorcode::sindyErrCode SetNameYomi( LPTSTR lpszName )
	{
		return SetStringValueByFieldName( schema::road_node::kNameYomi, lpszName );
	}	
	
	/**
	 * @brief ���΍����R�[�h��ݒ肷��
	 * @brief NULL �s����
	 * @retval �����@sindyErr_NoErr
	 * @retval ���s�@����ȊO�� sindyErrCode
	 */
	errorcode::sindyErrCode SetHeightCode(schema::road_node::height_code::ECode ecode)
	{
		return SetLongValueByFieldName( schema::road_node::kHeightCode, ecode );
	}
	
	/**
	 * @brief ���΍�����ݒ肷��
	 * @brief NULL ����
	 * @retval �����@sindyErr_NoErr
	 * @retval ���s�@����ȊO�� sindyErrCode
	 */
	errorcode::sindyErrCode SetHeight(long lValue)
	{
		return SetLongValueByFieldName( schema::road_node::kHeight, lValue );
	}
	
	/**
	 * @brief �m�[�h�M���@�R�[�h��ݒ肷��
	 * @brief NULL �s����
	 * @retval �����@sindyErr_NoErr
	 * @retval ���s�@����ȊO�� sindyErrCode
	 */
	errorcode::sindyErrCode SetSignalCode(schema::road_node::signal::ECode ecode)
	{
		return SetLongValueByFieldName( schema::road_node::kSignal, ecode );
	}
	
	/**
	 * @brief ���ԏ�ID��ݒ肷��
	 * @brief NULL �s����
	 * @retval �����@sindyErr_NoErr
	 * @retval ���s�@����ȊO�� sindyErrCode
	 */
	errorcode::sindyErrCode SetParkingID(long lValue)
	{
		return SetLongValueByFieldName( schema::road_node::kParkingID, lValue );
	}	
	
	//@}


	//@{ @name �`�F�b�N�֐�
	/**
	 * @brief <b>���������`�F�b�N����</b>\n
	 * �m�[�h��ҏW������ɕK���`�F�b�N���s���Ă��������B
	 * ���̃��\�b�h�Ń`�F�b�N����鍀�ڂ͈ȉ��̂Ƃ���ł��B
	 * @li �񎟃��b�V��������m�[�h�E�񎟃��b�V��������PEC�m�[�h�͓񎟃��b�V��������ɂȂ��ꍇ�̓G���[
	 * @li �񎟃��b�V��������ɏ�L�ȊO�̎�ʂ̃m�[�h������ꍇ�̓G���[
	 * @li �񎟃��b�V���l���Ƀm�[�h������ꍇ�̓G���[�i��ޖ�킸�j
	 * @param cRule			[in]	�������`�F�b�N�p���[��
	 * @param cErrInfos		[out]	�G���[���i�[�p�R���e�i
	 * @return sindyErrCode
	 */
	virtual errorcode::sindyErrCode CheckLogic( const CLogicRule& cRule, CErrorObjects& cErrs ) const;

	/**
	 * @brief ��������ʂ��ݒ肳��Ă��邩
	 * @retval	sindyErr_NoErr		�����F���������
	 * @retval	����ȊO�̃G���[	���s�F�K��Ɉᔽ�������
	*/
	errorcode::sindyErrCode CheckNodeClass() const;
	//@}

	bool IsExistAttrNode( bool bCheckAll = true ) const;
	/**
	 * @brief 2�����b�V����m�[�h���ǂ���
	 * @retval	�����F2�����b�V����m�[�h
	 * @retval	���s�F��L�ȊO
	*/
	bool IsBorderNode() const;
};

} // namespace road_node
} // namespace sindy

#endif // _ROADNODE_H_
