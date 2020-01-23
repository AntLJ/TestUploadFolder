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

class WalkLinkOfBaseMeshCheckFunction
{
public:
	/**
	 * @brief �R���X�g���N�^
	 * @param out              [in] ���O�o�͊Ǘ��N���X�̎Q��
	 * @param modufyFilter     [in] �ҏW�����t�B���^
	 * @param onewayClassMap   [in] ����ʍs�K���R�[�h�̃R�[�h�l�h���C���}�b�v
	 * @param walkNoPassage    [in] �`�F�b�N�Ώۂ̒ʍs�֎~�K��
	 */
	WalkLinkOfBaseMeshCheckFunction(
		COutPut& out
		, CComVariant modifyFilter
		, std::map<long, CString>& onewayClassMap
		, MultiRowsPack& walkNoPassage
		):
		m_output(out)
		, m_modifyFilter(modifyFilter)
		, m_onewayClassMap(onewayClassMap)
		, m_walkNoPassage(walkNoPassage)
	{}

	/**
	 * @brief �x�[�X���b�V���P�ʂōs�����s�҃����N�̑S�`�F�b�N
	 * @param ipMeshGeo       [in] �x�[�X���b�V���`��
	 * @param ipBoundaryMesh  [in] �x�[�X���b�V���̘g��
	 * @param cityMeshMap     [in] �s�s�n�}���b�V���R�[�h�ƌ`��̑Ή��\
	 * <�s�s�n�}���b�V���R�[�h, <�s�s�n�}���b�V���`��, �s�s�n�}���b�V������9���b�V���`��>>
	 * @param walkLink        [in] ���s�҃����N�Q�N���X
	 * @param walkNode        [in] ���s�҃m�[�h�Q�N���X
	 * @param walkNodeLinkIDs [in] ���s�҃m�[�h�ɐڑ�������s�҃����N��ID�Ή��\
	 */
	void checkWalkLinkOfBaseMesh(
		const IGeometryPtr& ipMeshGeo,
		const IGeometryPtr& ipBoundaryMesh,
		const std::map<long, std::pair<IGeometryPtr, IGeometryPtr>>& cityMeshMap,
		const FeaturesPack& walkLink,
		const FeaturesPack& walkNode,
		const NODEIDMAP& walkNodeLinkIDs
		);

	/**
	 * @brief release���[�h�p�̃`�F�b�N
	 * @param ipMeshGeo       [in] �x�[�X���b�V���`��
	 * @param ipBoundaryMesh  [in] �x�[�X���b�V���̘g��
	 * @param cityMeshMap     [in] �s�s�n�}���b�V���R�[�h�ƌ`��̑Ή��\
	 * <�s�s�n�}���b�V���R�[�h, <�s�s�n�}���b�V���`��, �s�s�n�}���b�V������9���b�V���`��>>
	 * @param walkLink        [in] ���s�҃����N�Q�N���X
	 * @param walkNode        [in] ���s�҃m�[�h�Q�N���X
	 * @param walkNodeLinkIDs [in] ���s�҃m�[�h�ɐڑ�������s�҃����N��ID�Ή��\
	 */
	void checkRelease(
		const IGeometryPtr& ipMeshGeo,
		const IGeometryPtr& ipBoundaryMesh,
		const std::map<long, std::pair<IGeometryPtr, IGeometryPtr>>& cityMeshMap,
		const FeaturesPack& walkLink,
		const FeaturesPack& walkNode,
		const NODEIDMAP& walkNodeLinkIDs
		);

protected:
	COutPut& m_output;                                //!< ���O�o�͗p�C���X�^���X�ւ̎Q��
	CComVariant m_modifyFilter;                       //!< �ҏW�����t�B���^
	std::map<long, CString> m_onewayClassMap;         //!< ����ʍs�K���R�[�h�̃R�[�h�l�h���C���}�b�v
	MultiRowsPack& m_walkNoPassage;					  //!< �`�F�b�N�Ώۂ̒ʍs�֎~�K��
};
