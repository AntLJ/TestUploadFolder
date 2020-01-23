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

class UndergroundCheckFunction
{
public:
	/**
	 * @brief �R���X�g���N�^
	 * @param out          [in] ���O�o�͊Ǘ��N���X�̎Q��
	 * @param modufyFilter [in] �ҏW�����t�B���^
	 */
	UndergroundCheckFunction(COutPut& out, CComVariant modifyFilter):
		m_output(out)
		, m_modifyFilter(modifyFilter)
	{}

	/**
	 * @brief	�n���X�|���S���̃`�F�b�N
	 * @param	meshcode	[in]	�`�F�b�N�Ώۃ��b�V���R�[�h
	 * @param	ipMeshGeo	[in]	�`�F�b�N�Ώۂ̃W�I���g��
	 * @param	underArea   [in]	�`�F�b�N�Ώےn���X�|���S���Q�N���X
	 * @param	walkLink   	[in]	�`�F�b�N�Q�Ɨp���s�҃m�[�h�Q�N���X
	 */
	void CheckUnderground( 
		const CString& meshcode,
		const IGeometryPtr& ipMeshGeo, 
		const FeaturesPack& underArea,
		const FeaturesPack& walkLink
		);

private:
	/**
	 * @brief �G���[�R�[�h1600�ɊY�����郊���N���G���[�o��
	 * @param underLinkIdAll [in] ���������b�V���ɏd�Ȃ�n���ʘH�����N��ID
	 * @param outsideLinkId  [in] ���������b�V���ɏd�Ȃ�n���ʘH�����N�̓��A
	 * ������̒n���X�|���S���ɂ��܂܂�Ȃ��n���ʘH�����N
	 * @param underAreaUnion [in] ���������b�V���ɏd�Ȃ�n���X�|���S����S��
	 * Union�����`��
	 * @param underAreaId  [in] �������̃��b�V���ɏd�Ȃ�n���X�|���S����ID
	 * @param walkLik      [in] �������̃��b�V��+�א�9���b�V�����̑S�Ă̕��s�҃����N�Q�N���X
	 * @param underArea    [in] �������̃��b�V��+�א�9���b�V�����̑S�Ă̒n���X�|���S���Q�N���X
	 * @param meshcode     [in] �������̃��b�V���̃��b�V���R�[�h
	 */
	void errLinkPartIncludedArea(
		const std::set<long>& underLinkIdAll
		, const std::set<long>& outsideLinkId
		, const IGeometryPtr& underAreaUnion
		, const std::set<long>& underAreaId
		, const FeaturesPack& walkLink
		, const FeaturesPack& underArea
		, const CString& meshcode);

	/**
	 * @brief �`�F�b�N�Ώۃ����N���G���[�R�[�h1600�̏o�͑ΏۂɂȂ邩�m�F
	 * @param linkInfo      [in] �`�F�b�N�Ώۂ̃����N
	 * @param fieldMap      [in] �`�F�b�N�Ώۂ̃t�B�[���h�}�b�v
	 * @param underAreaId   [in] �������̃��b�V���ɏd�Ȃ�n���X�|���S����ID
	 * @param underArea     [in] �������̃��b�V��+�א�9���b�V�����̑S�Ă̒n���X�|���S���̏��
	 * @param crossPoint    [in] �`���b�N�Ώۃ����N�ƒn���X�|���S���̌����ʒu
	 * @retval true  �G���[�o�͑Ώۂł���
	 * @retval false �G���[�o�͑Ώۂł͂Ȃ�
	 */
	bool checkModifyTimeToPartIncludedErr(
		const GeoClass& linkInfo
		, const sindy::CFieldMap& fieldMap
		, const std::set<long>& underAreaId
		, const FeaturesPack& underArea
		, const std::vector<IPointPtr>& crossPoint);

	/**
	 * @brief �G���[�R�[�h1601�ɊY�����郊���N���G���[�o��
	 * @param outLinks     [in] �������̃��b�V�����ŁA������̒n���X�|���S���ɂ��܂܂�Ȃ��n���ʘH�����N
	 * @param walkLik      [in] �������̃��b�V��+�א�9���b�V�����̑S�Ă̕��s�҃����N�̏��
	 * @param meshcode     [in] �������̃��b�V���̃��b�V���R�[�h
	 */
	void errOutLink(
		const std::set<long>& outLinks
		, const FeaturesPack& walkLink
		, const CString& meshcode);

	/**
	 * @brief �G���[�R�[�h1602�ɊY�����郊���N���G���[�o��
	 * @param underLinks   [in] ���������b�V�����̒n���ʘH�����N
	 * @param unionArea    [in] ���������b�V�����̒n���ʘH�|���S���̌`���Union�����`��
	 * @param walkLik      [in] �������̃��b�V��+�א�9���b�V�����̑S�Ă̕��s�҃����N�̏��
	 * @param meshcode     [in] �������̃��b�V���̃��b�V���R�[�h
	 */
	void errNoneExist(
		const std::set<long>& underLinks
		, const IGeometryPtr& unionArea
		, const FeaturesPack& walkLink
		, const CString& meshcode);

protected:
	COutPut& m_output; //!< ���O�o�͗p�C���X�^���X�ւ̎Q��
	CComVariant m_modifyFilter; //!< �ҏW�����t�B���^
};
