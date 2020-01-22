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

// AChiban.h: AChiban �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACHIBAN_H__22170C45_A46A_40CD_B561_307E37C448FF__INCLUDED_)
#define AFX_ACHIBAN_H__22170C45_A46A_40CD_B561_307E37C448FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/** 
    * @brief ���L�t���O�t���̒n�ԑ�\�_�̂������b�V�����E��ɂ�����̂����o�y�уt���O���I�t����N���X 
    */ 
class AChiban  
{
public:
	/** 
  	 * @brief �R���X�g���N�^ 
  	 */ 
	AChiban();

	/** 
  	 * @brief �f�X�g���N�^ 
  	 */ 
	~AChiban();

	/**
	 * @brief �������B
	 *
	 * @param ipChibanClass	[in]	�n�ԑ�\�_�t�B�[�`���N���X
	 * @param cChgMode		[in]	�ύX�t���O
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool init(IFeatureClassPtr ipChibanClass, bool cChgMode);

	/**
	 * @brief ���b�V�����E��̒n�ԑ�\�_�̒��L�t���O���I�t�ɂ���B
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool execAdjust(IFeaturePtr ipMesh, LONG cMeshCode);

private:
	CString m_LayerName;	///< �t�B�[�`���N���X���i���O�p�j
	LONG m_MeshCode;		///< ���s���̃��b�V���R�[�h�i���O�p�j
	LONG m_AnnofIndex;		///< ���L�t���O�̃t�B�[���h�C���f�b�N�X
	IFeatureClassPtr m_ipRep;	///< �n�ԑ�\�_�t�B�[�`���N���X
	bool m_ChgMode;			///< �ύX�t���O

	/**
	 * @brief �w��W�I���g����̒n�ԑ�\�_�Œ��L�t���O�������Ă�����̂����o����B�ύX���[�h�̎��̓I�t�ɂ���B
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool fnCheck(IGeometryPtr ipBoundary);
};

#endif // !defined(AFX_ACHIBAN_H__22170C45_A46A_40CD_B561_307E37C448FF__INCLUDED_)
