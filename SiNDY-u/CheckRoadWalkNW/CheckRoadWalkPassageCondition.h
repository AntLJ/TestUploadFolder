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
#include "CheckRoadWalkNode.h"

class CCheckRoadWalkPassageCondition : public CCheckRoadWalkNode
{
public:
	CCheckRoadWalkPassageCondition(void);
	~CCheckRoadWalkPassageCondition(void);


	/**
	 * @brief	NPW�t�H�[�}�b�g�`�F�b�N
	 * @param	ipTable	[in]	�`�F�b�N�Ώۂ̃e�[�u��
	 * @param	fieldIndex	[in]	�`�F�b�N�Ώۂ̃t�B�[���h�C���f�b�N�X
	 * @param	dbCtrl	[in,out]	��r�Ώۂ�DB���
	 * @return	bool
	 */
	virtual bool checkFormat(const ITablePtr& ipTable, const FIELD_INDEX& fieldIndex, CDatabaseController& dbCtrl);

protected:
	/**
	 * @brief	�w�肳�ꂽ���R�[�h����l�̎擾
	 * @note	�擾�Ɏ��s�����ꍇ�A�G���[���b�Z�[�W��ǉ�����
	 * @param	linkID	[in]	�����NID
	 * @param	ipRow	[in]	�擾�Ώۂ�Row
	 * @param	fieldName	[in]	�擾�Ώۂ̃t�B�[���h��
	 * @param	fieldIndex	[in]	�擾�Ώۂ̃t�B�[���h�C���f�b�N�X
	 * @param	value	[out]	�擾�����l
	 * @return	bool
	 */
	virtual bool getValue(long linkID, const _IRowPtr& ipRow, const CString& fieldName, const FIELD_INDEX& fieldIndex, CComVariant& value);

	/**
	 * @brief	�l���}�b�v�ɐݒ�
	 * @param	dbFieldName	[in]	DB�t�B�[���h��
	 * @param	lValue		[in]	�ݒ肷��l
	 * @param	mapValue	[out]	�l�̃}�b�v(�L�[��DB�t�B�[���h��)
	 */
	virtual void setValue(const CString& dbFieldName, const long& lValue, MAP_VALUE& mapValue);

	/**
	 * @brief	�l���}�b�v�ɐݒ�
	 * @param	dbFieldName	[in]	DB�t�B�[���h��
	 * @param	varValue	[in]	�ݒ肷��l
	 * @param	mapValue	[out]	�擾�����l�̃}�b�v(�L�[��DB�t�B�[���h��)
	 */
	virtual bool setValue(const CString& dbFieldName, const CComVariant& varValue, MAP_VALUE& mapValue);


	/**
	 * @brief	���H�F�Œ�l��r�A���s�ҁF�Œ�l��r
	 * @param	linkID	[in]	�����NID
	 * @param	ipRow	[in]	�`�F�b�N�Ώۂ̃��R�[�h
	 * @param	fieldName	[in]	�`�F�b�N�Ώۂ̃t�B�[���h��
	 * @param	fieldIndex	[in]	�`�F�b�N�Ώۂ̃t�B�[���h�C���f�b�N�X
	 * @param	fixedTimeValue	[in]	��r����Œ�l(���H)
	 * @param	fixedWalkValue	[in]	��r����Œ�l(���s��)
	 * @return	bool
	 */
	virtual bool checkFixedValue(long linkID, const _IRowPtr& ipRow, const CString& fieldName, const FIELD_INDEX& fieldIndex, long fixedTimeValue, long fixedWalkValue);

	/**
	 * @brief	�G���[���b�Z�[�W�̍쐬�ƒǉ�
	 * @param	linkID	[in]	�����NID
	 * @param	fieldName	[in]	�t�B�[���h��
	 * @param	message	[in]	�G���[���b�Z�[�W
	 * @param	count	[in]	�G���[�Ώۂ̒l�̌���(����ȍ~�ɃG���[�Ώۂ̒l�𕡐��w��)
	 */
	using CCheckRoadWalkNode::pushColumnErrorMessage;
	virtual void pushColumnErrorMessage(long linkID, const CString& filedName, const CString& message, int count, ...);
};

