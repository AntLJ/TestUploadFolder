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
 * @brief �������\���_�𒲂׈Ղ����邽�߂̍\����
 *        �A������3�_��ێ�
 */
struct CheckPoint
{
	WKSPoint m_Center;
	WKSPoint m_Left;
	WKSPoint m_Right;
	bool operator<(const CheckPoint &cCmp) const{
		if(m_Center.X != cCmp.m_Center.X){
			return m_Center.X < cCmp.m_Center.X;
		}else{
			return m_Center.Y < cCmp.m_Center.Y;
		}
	}
};

typedef std::set<CheckPoint, std::less<CheckPoint>> POINT_SET;	///< �����t���ē_����r���邽�߂̃Z�b�g

/**
 * @brief �������f����ʂ�I������ɑΉ������Ȃ��Ȃ����\���_���폜����N���X
 *        ���H�ƊX�擙�A�אڂ���|���S���ɑ΂��A�Е������ύX���s���Ȃ��������Ƃ���A
 *        ���b�V�����E��̍\���_�ɕs�����𐶂���i�ύX���s��ꂽ���ɂ͐V���ɍ\���_���ł���j���Ƃ�����B
 *        ������������邽�߁A�A������3�_�̐^�񒆂����b�V�����E��̓_�ŁA���̗��[���ʂ̃��b�V���ɂ���ꍇ�A
 *        ���̃|���S���̍\���_�ŁA�������̂����邩�𒲂ׁA�Ȃ���΍폜����B
 */
class CDelSinglePoint :
	public CSDEBase
{
public:
	/**
	 * @brief �R���X�g���N�^�B
	 */
	CDelSinglePoint(void);

	/**
	 * @brief �f�X�g���N�^�B
	 */
	~CDelSinglePoint(void);

	/**
	 * @brief �������֐��B
	 */
	bool init(const CArguments& args, const IWorkspacePtr& ipWorkspace,
		const ISpatialReferencePtr& ipSpRef, const FCArray& ipFeatureClasses, bool isSindyCImport );

private:
	ISpatialReferencePtr m_ipSpRef;


	/**
	 * @brief ���b�V���P�ʂł̕s�v�\���_�폜�i���̂Q�j�֐��B
	 *
	 * @param cCommand		[in]	���f�p�R�}���h�i���g�p�j
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool fnExecMesh(COMMAND cCommand);

	/**
	 * @brief ���C���P�ʂ̃��b�V�����E��̕s�v�\���_�폜�֐��B
	 *
	 * @param cIndex	[in]	�e�[�u���̉��Ԗڂ̃t�B�[�`���N���X���������C���f�b�N�X
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool fnDeleteDisusedPoint(long cIndex);

	/**
	 * @brief �t�B�[�`���P�ʂ̃��b�V�����E��̕s�v�\���_�폜�֐��B
	 *
	 * @param ipFeature	[in]	�Ώۃt�B�[�`��
	 * @param cIndex	[in]	�e�[�u���̉��Ԗڂ̃t�B�[�`���N���X���������C���f�b�N�X
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool fnCheckAndDelete(IFeaturePtr ipFeature, long cIndex);

	/**
	 * @brief �|���S���̍폜���_�Ƃ��̑O��̓_�̏����擾����֐��B
	 *
	 * @param ipGeom		[in]	�ΏۃW�I���g��
	 * @param cCheckPoints	[out]	�폜���|�C���g�Q
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool fnGetCheckPoints(IGeometryPtr ipGeom, POINT_SET& cCheckPoints);

	/**
	 * @brief �폜���|�C���g�̑O�ォ��Ȃ郉�C���������Ă��邩�`�F�b�N���āA�폜�Ώۃ|�C���g�����߂�֐��B
	 *
	 * @param ipFeature		[in]	�ΏۃW�I���g��
	 * @param cCheckPoints	[in]	�폜���|�C���g�Q
	 * @param cDeletePoints	[out]	�폜�Ώۃ|�C���g�Q
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool fnFindPairLine(IFeaturePtr ipFeature, const POINT_SET& cCheckPoints, POINT_SET& cDeletePoints);

	/**
	 * @brief �s�v�\���_���폜������̌`������߂�֐�(�K�v�ɉ����ă|���S���������O�ɕ������čč\������)�B
	 *
	 * @param ipSrcGeom		[in]	�ΏۃW�I���g��
	 * @param cDeletePoints	[in]	�폜�Ώۃ|�C���g�Q
	 * @param cObjID		[in]	�ΏۃI�u�W�F�N�gID
	 * @param cIndex		[in]	�e�[�u���̉��Ԗڂ̃t�B�[�`���N���X���������C���f�b�N�X
	 *
	 * @retval NULL�ȊO	���Z��̃W�I���g��
	 * @retval NULL		���s�A�G���[�i�܂��Ȃ��͂��E�E�j
	 */
	IGeometryPtr fnCroppingShape(IGeometryPtr ipSrcGeom, const POINT_SET& cDeletePoints, long cObjID, long cIndex);

	/**
	 * @brief �i�|���S���𕪉������j�����O�̍폜���_�Ƃ��̑O��̓_�̏����擾����֐��B
	 *
	 * @param ipGeom		[in]	�ΏۃW�I���g��
	 * @param cCheckPoints	[out]	�폜���|�C���g�Q
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool fnCheckRing(IGeometryPtr ipGeom,  POINT_SET& cCheckPoints);

	/**
	 * @brief 2�_��WKSPoint��v�m�F�֐��B
	 *
	 * @param cPoint1		[in]	��r����|�C���g�P
	 * @param cPoint2		[in]	��r����|�C���g�Q
	 *
	 * @retval true		��v
	 * @retval false	�s��v
	 */
	bool fnIsEqual(const WKSPoint& aPoint1, const WKSPoint& aPoint2);

	/**
	 * @brief �s�v�\���_���폜������̌`������߂�֐�(�����O�P��)�B
	 *
	 * @param ipSrcGeom		[in]	�ΏۃW�I���g��
	 * @param cDeletePoints	[in]	�폜�Ώۃ|�C���g�Q
	 * @param cObjID		[in]	�ΏۃI�u�W�F�N�gID
	 * @param cIndex		[in]	�e�[�u���̉��Ԗڂ̃t�B�[�`���N���X���������C���f�b�N�X
	 *
	 * @retval NULL�ȊO	���Z��̃W�I���g��
	 * @retval NULL		���s�A�G���[�i�܂��Ȃ��͂��E�E�j
	 */
	IGeometryPtr fnCroppingRing(IGeometryPtr ipSrcGeom, const POINT_SET& cDeletePoints, long cObjID, long cIndex);

	/**
	 * @brief �|�C���g�i���W�j����s�s�n�}�̃��b�V���R�[�h���v�Z����֐�
	 *
	 * @param cPoint		[in]	���b�V���R�[�h�����߂�|�C���g�i���W�j
	 *
	 * @retval �s�s�n�}�̃��b�V���R�[�h
	 */
	long fnCalcMeshCode(const WKSPoint& cPoint);
};
