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

// MeshIndex.h: MeshIndex �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MESHINDEX_H__232CA619_D99C_4293_A06B_FBC0DA339045__INCLUDED_)
#define AFX_MESHINDEX_H__232CA619_D99C_4293_A06B_FBC0DA339045__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class MeshIndex  
{
public:
	/** 
  	 * @brief �R���X�g���N�^ 
  	 */ 
	MeshIndex();

	/** 
  	 * @brief �R���X�g���N�^ 
	 *
	 * @param ipFeature	[out]	���b�V���|���S���t�B�[�`���N���X
	 *
  	 */ 
	MeshIndex(IFeatureClassPtr ipFeature);

	/** 
  	 * @brief �f�X�g���N�^ 
  	 */ 
	~MeshIndex();

	/**
	 * @brief �������B�i�����Ŏw��j
	 *
	 * @param cMeshField	[in]	���b�V���R�[�h�t�B�[���h��
	 * @param cWhere		[in]	�Ώے��o�pSQL��WHERE������
	 *
	 * @retval true		����
	 * @retval false	���s
	 */
	bool init(LPCTSTR cMeshField, const CString &cWhere);

	/**
	 * @brief �������B�i���b�V���R�[�h�Ŏw��j
	 *
	 * @param cMeshCode		[in]	���b�V���R�[�h
	 * @param cMeshField	[in]	���b�V���R�[�h�t�B�[���h��
	 *
	 * @retval true		����
	 * @retval false	���s
	 */
	bool init(LONG cMeshCode, LPCTSTR cMeshField);

	/**
	 * @brief �i���́j�Ώۃ��b�V���|���S�����擾�B
	 *
	 * @param cpMeshCode	[out]	���b�V���R�[�h
	 *
	 * @retval NULL�ȊO	���b�V���|���S���̃t�B�[�`��
	 * @retval NULL	�I���i�G���[�Ŕ����邩������Ȃ��j
	 */
	IFeaturePtr nextFeature(LONG* cpMeshCode);

private:
	LONG m_MeshIndex;	///< ���b�V���R�[�h�t�B�[���h�C���f�b�N�X
	IFeatureCursorPtr m_ipCursor;	///< �����őI�Ԏ��p�̃t�B�[�`���J�[�\��
	IFeatureClassPtr	m_ipFeature;	///< ���b�V���|���S���t�B�[�`���N���X�i���O������킵���j
};

#endif // !defined(AFX_MESHINDEX_H__232CA619_D99C_4293_A06B_FBC0DA339045__INCLUDED_)
