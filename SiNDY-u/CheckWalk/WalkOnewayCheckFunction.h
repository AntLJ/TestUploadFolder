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

class WalkOnewayCheckFunction
{
public:
	/**
	 * @brief �R���X�g���N�^
	 * @param out           [in] ���O�o�͊Ǘ��N���X�̎Q��
	 * @param modufyFilter  [in] �ҏW�����t�B���^
	 */
	WalkOnewayCheckFunction(
		COutPut& out
		, CComVariant modifyFilter
		, sindy::schema::walk_type::type_class::ECode dataType
	) :
		m_output(out)
		, m_modifyFilter(modifyFilter)
		, m_dataType(dataType)
	{}

	/**
	 * @brief ���s�҈���ʍs�K���e�[�u���̑S�`�F�b�N
	 * @param ipMeshGeo        [in] �`�F�b�N�Ώۃ��b�V���̃W�I���g��
	 * @param walkLink  	   [in] ���s�҃����NMap
	 * @param walkOneway       [in] ���s�҈���ʍs�K�����
	 * @param walkNoPassage    [in] ���s�Ғʍs�֎~�K�����
	 */
	void checkWalkOneway(
		const IGeometryPtr& ipMeshGeo,
		const std::map<long, GeoClass>& walkLinkMap,
		const MultiRowsPack& walkOneway,
		const MultiRowsPack& walkNoPassage
	);

	// TODO: checkWalkNoPassage�Ɠ����\�ȉӏ��𓝍�
	/**
	 * @brief release���[�h�p�̃`�F�b�N
	 * @param ipMeshGeo        [in] �`�F�b�N�Ώۃ��b�V���̃W�I���g��
	 * @param walkLink  	   [in] ���s�҃����NMap
	 * @param walkOneway       [in] ���s�҈���ʍs�K�����
	 */
	void checkRelease(
		const IGeometryPtr& ipMeshGeo,
		const FeaturesPack& walkLink,
		const MultiRowsPack& walkOneway
	);

	/**
	 * @brief ��O���[�h�p�̃`�F�b�N
	 * @param walkOneway       [in] ���s�҈���ʍs�K�����
	 */
	void checkException(
		const MultiRowsPack& walkOneway
	);

private:
	// TODO: �ȉ���WalkNoPassageCheckFunction���������o�[�Ɠ�������
	/**
	 * @brief	���s�Ғʍs�֎~�K���e�[�u���̃��R�[�h�ԃ`�F�b�N
	 * @param	meshcode	[in]	���b�V���R�[�h
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
	 * @brief	�w�肳�ꂽ�����NID�Ɋ֘A���鎞�ԋK�����擾���A�z��Ɋi�[����B
	 * @param	linkID	[in]	�����NID
	 * @param	walkNopassage	[in]	���ԋK���֘A�e�[�u����Map
	 * @param	timRegList	[out]	�����Ǝ��ԋK����񂪃y�A�ɂȂ������X�g
	 */
	void createTimeRegList(
		long linkID,
		const MultiRowsPack& walkNopassage,
		std::vector< std::pair< _IRowPtr, CTimeReg >>& timRegList
	);

	/**
	 * @brief �����NID���s���Ȓl���`�F�b�N
	 * @param [in] linkID         �����NID
	 */
	void checkLinkID(
		long  linkID
	) const;

	/**
	* @brief �����N�����R�[�h���s���Ȓl���`�F�b�N
	* @param [in] linkDir         �����N�����R�[�h
	*/
	void checkLinkDir(
		long  linkDir
	) const;

protected:
	COutPut& m_output; //!< ���O�o�͗p�C���X�^���X�ւ̎Q��
	CComVariant m_modifyFilter; //!< �ҏW�����t�B���^
	sindy::schema::walk_type::type_class::ECode m_dataType;             //!< �f�[�^�^�C�v
};
