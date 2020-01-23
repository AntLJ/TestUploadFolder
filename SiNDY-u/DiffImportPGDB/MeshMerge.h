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


typedef std::set<long> KIND_SET;  //!< �}�[�W�Ώۑ����Ǘ��p�Z�b�g

/**
 * @brief ���b�V�����E�̃}�[�W�i�s�v�\���_�폜����荞�񂾁j�N���X
 */
class CMeshMerge :
	public CSDEBase
{
public:
	/**
	 * @brief �R���X�g���N�^�B
	 */
	CMeshMerge();

	/**
	 * @brief �f�X�g���N�^�B
	 */
	~CMeshMerge(void);

	/**
	 * @brief �������֐��B
	 */
	bool init(const CArguments& args, const IWorkspacePtr& ipWorkspace,
		const ISpatialReferencePtr& ipSpRef, const FCArray& ipFeatureClasses, bool isSindyCImport );

private:
	// �����t�B�[�`���N���X�̍ő吔���ς������A�ȉ��̃����o�ϐ������킹�ĕύX���邱��
	// TODO: �ǂ����ňꊇ�Ǘ��ł���悤�ɂ���Ȃ�A�����e���₷������
	long m_FloorsIndex[fc_max_size];  //!< �����K���t�B�[���h�C���f�b�N�X
	long m_BGClsIndex[fc_max_size];   //!< �w�i�|���S�������t�B�[���h�C���f�b�N�X
	long m_AreaClsIndex[fc_max_size]; //!< �s�s�G���A�����t�B�[���h�C���f�b�N�X
	long m_AdmClsIndex[fc_max_size];  //!< �s���E�G���A�N���X�t�B�[���h�C���f�b�N�X
	long m_CLClsIndex[fc_max_size];  //!< �w�i���C�������t�B�[���h�C���f�b�N�X
	KIND_SET m_CSTarget;    //!< �}�[�W�Ώۑ����Ǘ��p�Z�b�g(�w�i�|���S��)
	KIND_SET m_CLTarget;    //!< �}�[�W�Ώۑ����Ǘ��p�Z�b�g(�w�i���C��)
	KIND_SET m_APTarget;    //!< �}�[�W�Ώۑ����Ǘ��p�Z�b�g(CITY_AREA)
	KIND_SET m_ADMTarget;   //!< �}�[�W�Ώۑ����Ǘ��p�Z�b�g(�s���E)
	ID_SET m_mergedIDList;  //!< �}�[�W�����ς݃I�u�W�F�N�gID
	ID_SET m_deleteIDList;  //!< �폜�ΏۃI�u�W�F�N�gID

	/**
	 * @brief ���b�V���P�ʂł̋��E��̕s�v�\���_�폜�Ƌ��E�}�[�W�֐��B
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
	 * @brief �s�v�\���_���폜������̌`��ƍ폜�����|�C���g�������߂�֐�(�K�v�ɉ����ă|���S���������O�ɕ������čč\������)�B
	 *
	 * @param ipSrcGeom	[in]	�ΏۃW�I���g��
	 * @param cCount	[out]	�폜�����|�C���g�̌�
	 *
	 * @retval NULL�ȊO	���Z��̃W�I���g��
	 * @retval NULL		���s�A�G���[�i�܂��Ȃ��͂��E�E�j
	 */
	IGeometryPtr fnCroppingShape(IGeometryPtr ipSrcGeom, int& cCount);

	/**
	 * @brief �s�v�\���_���폜������̌`��ƍ폜�����|�C���g�������߂�֐��i�����O�P�ʁj�B
	 *
	 * @param ipSrcGeom	[in]	�ΏۃW�I���g��
	 * @param cCount	[out]	�폜�����|�C���g�̌�
	 *
	 * @retval NULL�ȊO	���Z��̃W�I���g��
	 * @retval NULL		���s�A�G���[�i�܂��Ȃ��͂��E�E�j
	 */
	IGeometryPtr fnCroppingRing(IGeometryPtr ipSrcGeom, int& cCount);

	/**
	 * @brief ���C���P�ʂŃ��b�V�����E�̃}�[�W������֐��B
	 *
	 * @param cIndex	[in]	�e�[�u���̉��Ԗڂ̃t�B�[�`���N���X���������C���f�b�N�X
	 * @param ipCursor	[in]	���b�V�����̃t�B�[�`������Ԍ����������ʂ̃t�B�[�`���J�[�\��
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool fnShapeMerge(long cIndex/*, const IFeatureCursorPtr& ipCursor*/);

	/**
	 * @brief �t�B�[�`�����Ƀ}�[�W�\�Ȃ��̂��}�[�W����֐��B
	 *
	 * @param ipSrcFeature	[in]	�}�[�W���t�B�[�`��
	 * @param cIndex		[in]	�e�[�u���̉��Ԗڂ̃t�B�[�`���N���X���������C���f�b�N�X
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool fnSearchTargetsAndMerge(IFeaturePtr ipSrcFeature, long cIndex);

	/**
	 * @brief �`��I�Ƀ}�[�W�ΏۂɂȂ邩�𔻒肷��֐��i���b�V�����E��Ő��������L���Ă��邱�Ɓj�B
	 *
	 * @param ipSrcFeature	[in]	�}�[�W���t�B�[�`���P�i��Ɍ������ꂽ���ł����āA�������ʂ͂Ȃ��j
	 * @param ipTgtFeature	[in]	�}�[�W���t�B�[�`���Q�i�\�[�X���猟�����ꂽ���ł����āA�����ȏ��ʂ͂Ȃ��j
	 * @param cIndex		[in]	�e�[�u���̉��Ԗڂ̃t�B�[�`���N���X���������C���f�b�N�X
	 *
	 * @retval  0	�`��I�Ƀ}�[�W�Ώۂł͂Ȃ�
	 * @retval  1	�`��I�Ƀ}�[�W�Ώۂł���
	 * @retval -1	���s�A�G���[
	 */
	int fnIsNeedMerge(IFeaturePtr ipSrcFeature, IFeaturePtr ipTgtFeature, long cIndex);

	/**
	 * @brief �����I�Ƀ}�[�W�ΏۂɂȂ邩�𔻒肷��֐��i���߂�ꂽ��������v���邱�Ɓj�B
	 *
	 * @param ipSrcFeature	[in]	�}�[�W���t�B�[�`���P�i��Ɍ������ꂽ���ł����āA�������ʂ͂Ȃ��j
	 * @param ipTgtFeature	[in]	�}�[�W���t�B�[�`���Q�i�\�[�X���猟�����ꂽ���ł����āA�����ȏ��ʂ͂Ȃ��j
	 * @param cIndex		[in]	�e�[�u���̉��Ԗڂ̃t�B�[�`���N���X���������C���f�b�N�X
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool fnCompAttr(IFeaturePtr ipSrcFeature, IFeaturePtr ipTgtFeature, long cIndex);

	/**
	 * @brief �Е����}�[�W�����`��ɍX�V���A�Е����폜���āA�}�[�W��̏�Ԃ����֐��B
	 *
	 * @param ipSrcFeature	[in]	�}�[�W���t�B�[�`���P�i��Ɍ������ꂽ���ł����āA�������ʂ͂Ȃ��j
	 * @param ipTgtFeature	[in]	�}�[�W���t�B�[�`���Q�i�\�[�X���猟�����ꂽ���ł����āA�����ȏ��ʂ͂Ȃ��j
	 * @param cIndex		[in]	�e�[�u���̉��Ԗڂ̃t�B�[�`���N���X���������C���f�b�N�X
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool fnMerge(IFeaturePtr ipSrcFeature, IFeaturePtr ipTgtFeature, long cIndex);


	/**
	 * @brief �����I�Ƀ}�[�W�ΏۂɂȂ邩�𔻒肷��֐��i������������ʈ����������́j�B
	 *
	 * @param ipSrcFeature	[in]	�}�[�W���t�B�[�`���P�i��Ɍ������ꂽ���ł����āA�������ʂ͂Ȃ��j
	 * @param ipTgtFeature	[in]	�}�[�W���t�B�[�`���Q�i�\�[�X���猟�����ꂽ���ł����āA�����ȏ��ʂ͂Ȃ��j
	 * @param cIndex		[in]	�e�[�u���̉��Ԗڂ̃t�B�[�`���N���X���������C���f�b�N�X
	 *
	 * @retval -1�ȊO	�������̌��������i�K���ȊO�̑Ώۑ�������v���Ă���j
	 * @retval -1		�K���ȊO�������ɈႤ���̂�����
	 */
	int fnCompAttrSp(IFeaturePtr ipSrcFeature, IFeaturePtr ipTgtFeature, long cIndex);

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
	 * @brief �}�[�W�Ώۑ������𔻒肷��֐��B
	 *
	 * @param ipFeature [in] �����𒲂ׂ�t�B�[�`��
	 * @param cIndex    [in] �e�[�u���̉��Ԗڂ̃t�B�[�`���N���X���������C���f�b�N�X
	 *
	 * @retval true		�}�[�W�Ώۑ����ł���
	 * @retval false	�}�[�W�Ώۑ����ł͂Ȃ�
	 */
	bool fnIsTargetKind(const IFeaturePtr& ipFeature, const long cIndex);
};
