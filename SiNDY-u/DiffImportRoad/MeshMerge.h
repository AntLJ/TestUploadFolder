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
 * @brief ���b�V�����E�̃}�[�W�i�s�v�\���_�폜����荞�񂾁j�N���X
 */
class CMeshMerge :
	public CSDEBase
{
public:
	/**
	 * @brief �R���X�g���N�^
	 */
	CMeshMerge(CLayerManage* cLayerManage);

	/**
	 * @brief �f�X�g���N�^
	 */
	virtual~CMeshMerge(void);

	/**
	 * @brief �������֐��B
	 */
	bool init(void);

private:
	typedef std::set<long> KIND_SET;	///< �}�[�W�Ώۑ����Ǘ��p�Z�b�g
	typedef std::list<CString> TGT_LAYER_LIST;	///< �i�����Ώہj���C���Ǘ��p���X�g

	ID_SET m_IDList;		///< �����ς�ID�Ǘ��p�Z�b�g
	KIND_SET m_BGTarget;	///< �}�[�W���O�����Ǘ��p�Z�b�g
	TGT_LAYER_LIST m_TgtList;	///< �����Ώۃ��C���Ǘ��p���X�g

	/**
	 * @brief ���b�V���P�ʂł̋��E��̕s�v�\���_�폜�Ƌ��E�}�[�W�֐��B
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool fnExecMesh(void);

	/**
	 * @brief ���C���P�ʂ̃��b�V�����E��̕s�v�\���_�폜�֐��B
	 *
	 * @param cEnv	[in]	�e�����ێ������\����
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool fnDeleteDisusedPoint(EnvTable& cEnv);

	/**
	 * @brief �t�B�[�`���P�ʂ̃��b�V�����E��̕s�v�\���_�폜�֐��B
	 *
	 * @param ipFeature	[in]	�Ώۃt�B�[�`��
	 * @param cEnv		[in]	�e�����ێ������\����
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool fnCheckAndDelete(IFeaturePtr& ipFeature, EnvTable& cEnv);

	/**
	 * @brief �s�v�\���_���폜������̌`��ƍ폜�����|�C���g�������߂�֐�(�K�v�ɉ����ă|���S���������O�ɕ������čč\������)�B
	 *
	 * @param ipSrcGeom	[in]	�ΏۃW�I���g��
	 * @param cCount	[out]	�폜�����|�C���g�̌�
	 *
	 * @retval NULL�ȊO	���Z��̃W�I���g��
	 * @retval NULL		���s�A�G���[�i�܂��Ȃ��͂��E�E�j
	 */
	IGeometryPtr fnCroppingShape(const IGeometryPtr& ipSrcGeom, int& cCount);

	/**
	 * @brief �s�v�\���_���폜������̌`��ƍ폜�����|�C���g�������߂�֐��i�����O�P�ʁj�B
	 *
	 * @param ipSrcGeom	[in]	�ΏۃW�I���g��
	 * @param cCount	[out]	�폜�����|�C���g�̌�
	 *
	 * @retval NULL�ȊO	���Z��̃W�I���g��
	 * @retval NULL		���s�A�G���[�i�܂��Ȃ��͂��E�E�j
	 */
	IGeometryPtr fnCroppingRing(const IGeometryPtr& ipSrcGeom, int& cCount);

	/**
	 * @brief ���C���P�ʂŃ��b�V�����E�̃}�[�W������֐��B
	 *
	 * @param cIndex	[in]	�e�[�u���̉��Ԗڂ̃t�B�[�`���N���X���������C���f�b�N�X�i0�`2�j
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool fnShapeMerge(EnvTable& cEnv);

	/**
	 * @brief �t�B�[�`�����Ƀ}�[�W�\�Ȃ��̂��}�[�W����֐��B
	 *
	 * @param ipSrcFeature	[in]	�}�[�W���t�B�[�`��
	 * @param cEnv			[in]	�e�����ێ������\����
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool fnSerchTargetsAndMerge(IFeaturePtr& ipSrcFeature, EnvTable& cEnv);

	/**
	 * @brief �`��I�Ƀ}�[�W�ΏۂɂȂ邩�𔻒肷��֐��i���b�V�����E��Ő��������L���Ă��邱�Ɓj�B
	 *
	 * @param ipSrcFeature	[in]	�}�[�W���t�B�[�`���P�i��Ɍ������ꂽ���ł����āA�������ʂ͂Ȃ��j
	 * @param ipTgtFeature	[in]	�}�[�W���t�B�[�`���Q�i�\�[�X���猟�����ꂽ���ł����āA�����ȏ��ʂ͂Ȃ��j
	 * @param cEnv			[in]	�e�����ێ������\����
	 *
	 * @retval  0	�`��I�Ƀ}�[�W�Ώۂł͂Ȃ�
	 * @retval  1	�`��I�Ƀ}�[�W�Ώۂł���
	 * @retval -1	���s�A�G���[
	 */
	int fnIsNeedMerge(IFeaturePtr& ipSrcFeature, IFeaturePtr& ipTgtFeature, EnvTable& cEnv);

	/**
	 * @brief �����I�Ƀ}�[�W�ΏۂɂȂ邩�𔻒肷��֐��i���߂�ꂽ��������v���邱�Ɓj�B
	 *
	 * @param ipSrcFeature	[in]	�}�[�W���t�B�[�`���P�i��Ɍ������ꂽ���ł����āA�������ʂ͂Ȃ��j
	 * @param ipTgtFeature	[in]	�}�[�W���t�B�[�`���Q�i�\�[�X���猟�����ꂽ���ł����āA�����ȏ��ʂ͂Ȃ��j
	 * @param cEnv			[in]	�e�����ێ������\����
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool fnCompAttr(IFeaturePtr& ipSrcFeature, IFeaturePtr& ipTgtFeature, EnvTable& cEnv);

	/**
	 * @brief �Е����}�[�W�����`��ɍX�V���A�Е����폜���āA�}�[�W��̏�Ԃ����֐��B
	 *
	 * @param ipSrcFeature	[in]	�}�[�W���t�B�[�`���P�i��Ɍ������ꂽ���ł����āA�������ʂ͂Ȃ��j
	 * @param ipTgtFeature	[in]	�}�[�W���t�B�[�`���Q�i�\�[�X���猟�����ꂽ���ł����āA�����ȏ��ʂ͂Ȃ��j
	 * @param cEnv			[in]	�e�����ێ������\����
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool fnMerge(IFeaturePtr& ipSrcFeature, IFeaturePtr& ipTgtFeature, EnvTable& cEnv);


	/**
	 * @brief ���e�͈͂������������W�i�l�j�̈�v�m�F�֐��B
	 *
	 * @param cPoint1		[in]	��r���鐔���P�i���W�j
	 * @param cPoint2		[in]	��r���鐔���Q�i���W�j
	 * @param cAcceptable	[in]	����Ƃ݂Ȃ��ۂ̋��e�ł���ő�l
	 *
	 * @retval true		��v
	 * @retval false	�s��v
	 */
	bool fnCompXY(double cPoint1, double cPoint2, double cAcceptable);

	/**
	 * @brief �}�[�W�Ώۑ������𔻒肷��֐�(���k�w�i�Œ�ɂȂ��Ă��܂����̂ŁA�Ώۂ��������璲�����K�v)
	 *
	 * @param ipFeature		[in]	�����𒲂ׂ�t�B�[�`��
	 * @param cEnv			[in]	�e�����ێ������\����
	 *
	 * @retval true		�}�[�W�Ώۑ����ł���
	 * @retval false	�}�[�W�Ώۑ����ł͂Ȃ�
	 */
	bool fnIsTargetKind(const IFeaturePtr& ipFeature, EnvTable& cEnv);

	/**
	 * @brief �w��t�B�[�`���̎w�葮�������O�}�[�W�������𔻒肷��֐��i���k�w�i����j
	 *
	 * @param ipFeature		[in]	�����𒲂ׂ�t�B�[�`��
	 * @param cIndex		[in]	���ׂ鑮���̃t�B�[���h�C���f�b�N�X
	 *
	 * @retval true		�}�[�W�Ώۑ����ł���
	 * @retval false	�}�[�W�Ώۑ����ł͂Ȃ�
	 */
	bool fnIsTargetBaseKind(const IFeaturePtr& ipFeature, long cIndex);

};
