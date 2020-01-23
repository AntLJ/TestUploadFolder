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
#include "sdebase.h"

/**
 * @brief �}���`�p�[�g�|���S���̉���������N���X
 */
class CSeparatePolygon :
	public CSDEBase
{
public:
	/**
	 * @brief �R���X�g���N�^
	 */
	CSeparatePolygon(CLayerManage* cLayerManage);

	/**
	 * @brief �f�X�g���N�^
	 */
	virtual ~CSeparatePolygon(void);

	/**
	 * @brief �������֐�
	 */
	bool init(void);

private:
	typedef std::list<CString> TGT_LAYER_LIST;	///< �i�����Ώہj���C���Ǘ��p���X�g

	TGT_LAYER_LIST m_TgtList;	///< �����Ώۃ��C���Ǘ��p���X�g

	/**
	 * @brief ���b�V���P�ʂł̃}���`�p�[�g�|���S�������֐�
	 */
	bool fnExecMesh(void);

	/**
	 * @brief ���C�����̃}���`�p�[�g�����֐�
	 *
	 * @param cEnv		[in]	�e�����ێ������\����
	 *
	 */
	bool fnSeparate(EnvTable& cEnv);

	/**
	 * @brief �}���`�p�[�g�ł��邩�̃`�F�b�N�ƕ����֐�
	 *
	 * @param ipFeature	[in/out]	�`�F�b�N�E��������t�B�[�`��
	 * @param cEnv		[in]		�e�����ێ������\����
	 *
	 */
	bool fnCheckAndSeparate(IFeaturePtr& ipFeature, EnvTable& cEnv);

	/**
	 * @brief �p�����鑮���̃R�s�[�֐�
	 *
	 * @param ipBuffer	[in/out]	�R�s�[��t�B�[�`��
	 * @param ipFeature	[in/out]	�R�s�[���t�B�[�`��
	 * @param cSDEIndex		[in]		�t�B�[���h�C���f�b�N�X�}�b�v
	 *
	 */
	bool fnCopyAttr(IFeatureBufferPtr& ipBuffer, const IFeaturePtr& ipFeature, INDEX_MAP& cSDEIndex);
};
