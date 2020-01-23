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
#include "DatabaseControl.h"
#include "Properties.h"

class CCheckWalk;
class ScrambleAreaCheckFunction
{
public:
	/**
	 * @brief �R���X�g���N�^
	 * @param [in] out                       ���O�o�͊Ǘ��N���X�̎Q��
	 * @param [in] modufyFilter              �ҏW�����t�B���^
	 * @param [in] dataType                  �`�F�b�N�Ώۂ̃f�[�^�^�C�v
	 */
	ScrambleAreaCheckFunction(
		COutPut& out
		, CComVariant modifyFilter
		, sindy::schema::walk_type::type_class::ECode dataType) :
		m_output(out)
		, m_modifyFilter(modifyFilter)
		, m_dataType(dataType)
	{}

	/**
	 * @brief �X�N�����u���G���A�̑S�Ẵ`�F�b�N�����s
	 * @param [in] meshcode            �������̓s�s�n�}���b�V���R�[�h
	 * @param [in] ipMeshGeo           �s�s�n�}���b�V���`��
	 * @param [in] scrambleArea        �X�N�����u���G���A�Q�N���X
	 * @param [in] walkLink            ���s�҃����N�Q�N���X
	 * @param [in] walkLinkRTree       ���s�҃����NRTree
	 * @param [in] checktarget         �`�F�b�N�Ώ�
	 */
	void check(
		const CString& meshcode,
		const IGeometryPtr& ipMeshGeo,
		const FeaturesPack& scrambleArea,
		const FeaturesPack& walkLink,
		const BGRTree& walkLinkRTree,
		properties::check_target checktarget
	) const;

	/**
	 * @brief ��O���[�h�p�̃`�F�b�N
	 * @param [in] scrambleArea        �X�N�����u���G���A�Q�N���X
	 */
	void checkException(
		const FeaturesPack& scrambleArea
	);

protected:

	/**
	 * @brief �X�N�����u���G���A�`�󂪕s���ł��邩�`�F�b�N
	 * @note �G���[�R�[�h:1800
	 * @param [in] ipTable  �X�N�����u���G���A�e�[�u��
	 */
	void checkScrambleAreaGeometry(const ITablePtr& ipTable) const;

	/**
	 * @brief �X�N�����u���G���A���ɃX�N�����u���t���O���t�^���ꂽ���f������1���������`�F�b�N
	 * @param [in] scrambleAreaObject      �`�F�b�N�Ώۂ� �X�N�����u���G���A
	 * @param [in] walkLink                ���s�҃����N�Q�N���X
	 * @param [in] walkLinkRTree           ���s�҃����NRTree
	 */
	void checkScrambleAreaCrossWalkLink(
		const GeoClass& scrambleAreaObject,
		const FeaturesPack& walkLink,
		const BGRTree& walkLinkRTree
	) const;

	/**
	 * @brief �X�N�����u����ʃR�[�h���s���Ȓl���`�F�b�N
	 * @param [in] scrambleAreaClass       �X�N�����u�����
	 */
	void checkScrambleAreaClass(
		long  scrambleAreaClass
	) const;

protected:
	COutPut& m_output;                                                   //!< ���O�o�͗p�C���X�^���X�ւ̎Q��
	CComVariant m_modifyFilter;                                          //!< �ҏW�����t�B���^
	sindy::schema::walk_type::type_class::ECode m_dataType;              //!< �f�[�^�^�C�v
};
