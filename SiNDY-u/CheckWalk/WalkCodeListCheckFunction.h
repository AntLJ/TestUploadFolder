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
#include "util.h"
#include "OutputLog.h"

class WalkCodeListCheckFunction
{
public:
	/**
	 * @brief �R���X�g���N�^
	 * @param out           [in] ���O�o�͊Ǘ��N���X�̎Q��
	 * @param modufyFilter  [in] �ҏW�����t�B���^
	 */
	WalkCodeListCheckFunction(
		COutPut& out
		, CComVariant modifyFilter
		):
		m_output(out)
		, m_modifyFilter(modifyFilter)
	{}

	/**
	 * @brief ���s�ҘH���R�[�h�e�[�u���̑S�`�F�b�N
	 * @param walkCodeSet [in] �`�F�b�N�Ώۂł���
	 * WALK_CODE_LIST�e�[�u���̃��R�[�h��OID
	 * @param walkCode    [in] ���s�ҘH���R�[�h�̏��
	 */
	void checkWalkCodeList(
		const std::set<long>& walkCodeSet,
		const RowsPack& walkCode
		);

	// TODO: checkWalkCodeList�Ɠ����\�ȉӏ��𓝍�
	/**
	 * @brief release���[�h�p�̃`�F�b�N
	 * @param walkCode    [in] ���s�ҘH���̏��
	 */
	void checkRelease(
		const RowsPack& walkCode
	);

protected:
	COutPut& m_output; //!< ���O�o�͗p�C���X�^���X�ւ̎Q��
	CComVariant m_modifyFilter; //!< �ҏW�����t�B���^
};
