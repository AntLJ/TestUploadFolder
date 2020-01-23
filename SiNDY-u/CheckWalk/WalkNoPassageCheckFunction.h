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

class WalkNoPassageCheckFunction
{
public:
	/**
	 * @brief �R���X�g���N�^
	 * @param out           [in] ���O�o�͊Ǘ��N���X�̎Q��
	 * @param modufyFilter  [in] �ҏW�����t�B���^
	 */
	WalkNoPassageCheckFunction(
		COutPut& out
		, CComVariant modifyFilter
		, sindy::schema::walk_type::type_class::ECode dataType
	) :
		m_output(out)
		, m_modifyFilter(modifyFilter)
		, m_dataType(dataType)
	{}

	/**
	 * @brief ���s�Ғʍs�֎~�K���e�[�u���̑S�`�F�b�N
	 * @param ipMeshGeo        [in] �`�F�b�N�Ώۃ��b�V���̃W�I���g��
	 * @param walkLink         [in] ���s�҃����N�Q�N���X
	 * @param walkNoPassage    [in] ���s�Ғʍs�֎~�K�����
	 */
	void checkWalkNoPassage(
		const IGeometryPtr& ipMeshGeo,
		const FeaturesPack& walkLink,
		const MultiRowsPack& walkNoPassage
	);

	// TODO: checkWalkNoPassage�Ɠ����\�ȉӏ��𓝍�
	/**
	 * @brief release���[�h�p�̃`�F�b�N
	 * @param ipMeshGeo        [in] �`�F�b�N�Ώۃ��b�V���̃W�I���g��
	 * @param walkLink         [in] ���s�҃����N�Q�N���X
	 * @param walkNoPassage    [in] ���s�Ғʍs�֎~�K�����
	 */
	void checkRelease(
		const IGeometryPtr& ipMeshGeo,
		const FeaturesPack& walkLink,
		const MultiRowsPack& walkNoPassage
	);

	/**
	 * @brief ��O���[�h�p�̃`�F�b�N
	 * @param walkNoPassage       [in] ���s�Ғʍs�֎~�K�����
	 */
	void checkException(
		const MultiRowsPack& walkNoPassage
	);

private:

	/**
	 * @brief	���s�Ғʍs�֎~�K���e�[�u���̃��R�[�h�ԃ`�F�b�N
	 * @param	linkID	[in]	�����NID
	 * @param	timeRegList	[in]	���ꃊ���NID�ɕR�Â��ʍs�֎~�K�����̃��X�g
	 * @param	sameTimeErrCode	[in]	���Ԃ����S��v�̏ꍇ�̃G���[�R�[�h
	 * @param	overlapTimeErrCode	[in]	���Ԃ�����Ă����̏ꍇ�̃G���[�R�[�h
	 */
	void checkTimeRegulationRel(
		long linkID,
		const std::vector<std::pair<_IRowPtr, CTimeReg>>& timeRegList,
		const sindy::CFieldMap& fieldMap,
		const err_code::ECode sameTimeErrCode,
		const err_code::ECode overlapTimeErrCode
	);

	/**
	 * @brief �����NID���s���Ȓl���`�F�b�N
	 * @param [in] linkID         �����NID
	 */
	void checkLinkID(
		long  linkID
	) const;

protected:
	COutPut& m_output; //!< ���O�o�͗p�C���X�^���X�ւ̎Q��
	CComVariant m_modifyFilter; //!< �ҏW�����t�B���^
	sindy::schema::walk_type::type_class::ECode m_dataType;             //!< �f�[�^�^�C�v
};
