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

typedef std::set<long> ID_SET;	///< �I�u�W�F�N�gID�̃Z�b�g

/**
 * @brief �u�w�肵���L�[���[�h���܂ލ��|�C���g�v���܂މƌ`���폜����N���X
 */
class CPgdb
{
public:
	/**
	 * @brief �R���X�g���N�^�B
	 */
	CPgdb(void);

	/**
	 * @brief �f�X�g���N�^�B
	 */
	~CPgdb(void);

	/**
	 * @brief �������E���O�����B
	 *
	 * @param cTargetPath	[in]	�^�[�Q�b�gPGDB�̃x�[�X�p�X
	 * @param cMarkPath		[in]	�}�[�N�pPGDB�̃x�[�X�p�X
	 * @param cDelWord		[in]	�폜�p�L�[���[�h
	 * @param cDate1		[in]	���t�����P�i�������ƌ`�p�j
	 * @param cDate2		[in]	���t�����Q�i�����t���ƌ`�p�j
	 * @param pFile			[in]	���O�t�@�C���p�t�@�C���|�C���^
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool init(LPCTSTR cTargetPath, LPCTSTR cMarkPath, LPCTSTR cDelWord, LPCTSTR cDate1, LPCTSTR cDate2, FILE *pFile);

	/**
	 * @brief �}�[�N�ƌ`�폜�̃��C�����B
	 *
	 * @param cMeshList	[in]	���b�V�����X�g
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool execDeleteKakei(LPCTSTR cMeshList);

private:
	FILE* m_pFile;		///< ���O�t�@�C���p�t�@�C���|�C���^
	CString m_TargetPath;	///< �^�[�Q�b�gPGDB�̃x�[�X�p�X
	CString m_MarkPath;		///< �}�[�N�pPGDB�̃x�[�X�p�X
	CComVariant m_Date[2];	///< ���t�����P�C�Q
	CString m_Word;		///< �폜�p�L�[���[�h
	long m_BuildIndex;	///< �ƌ`�����p�t�B�[���h�C���f�b�N�X
	long m_ModDateIndex[3];	///< �ŏI�X�V���t�p�t�B�[���h�C���f�b�N�X�i���C���p�͎g��Ȃ��j
	IFeatureClassPtr m_ipBuild[3];	///< �����t�B�[�`���N���X�iBuilding,Building_Step,Building_Line�j

	/**
	 * @brief ���b�V���P�ʂőΏۉƌ`���폜�B
	 *
	 * @param cMeshCode	[in]	���b�V���R�[�h
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool fnDeleteKakeiMesh(long cMeshCode);


	/**
	 * @brief �w�胁�b�V���R�[�h��PGDB�I�[�v���B
	 *
	 * @param cBaseDir	[in]	�x�[�X�p�X
	 * @param cMeshCode	[in]	���b�V���R�[�h
	 *
	 * @retval NULL�ȊO		PGDB�̃��[�N�X�y�[�X
	 * @retval NULL			���s�A�G���[
	 */
	IWorkspacePtr fnOpenLocalDataBase(LPCTSTR cBaseDir, long cMeshCode);

	/**
	 * @brief �w��t�B�[�`���N���X�I�[�v���B
	 *
	 * @param ipWorkspace	[in]	���[�N�X�y�[�X
	 * @param cLayerName	[in]	�t�B�[�`���N���X��
	 *
	 * @retval NULL�ȊO		�t�B�[�`���N���X
	 * @retval NULL			���s�A�G���[
	 */
	IFeatureClassPtr fnOpenFeatureClass(const IWorkspacePtr& ipWorkspace, LPCTSTR cLayerName);

	/**
	 * @brief �폜�Ώہi�����𖞂����j��ID�񒊏o�B
	 *
	 * @param ipGou	[in]	���|�C���g�t�B�[�`���N���X
	 * @param cIds	[out]	�폜�Ώۂ�ID��i�����E�i���E���C���̂R���j
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool fnGetDeleteTarget(const IFeatureClassPtr& ipGou, std::vector<ID_SET>& cIds);

	/**
	 * @brief �P�̍��|�C���g����A�폜�ΏۂƂȂ錚���E�i���E���C����ID��̒��o�B
	 *
	 * @param ipGou	[in]	���|�C���g�t�B�[�`���N���X
	 * @param cIds	[out]	�폜�Ώۂ�ID��i�����E�i���E���C���̂R���j
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool fnFindTarget(const IFeaturePtr& ipGouFeature, std::vector<ID_SET>& cIds);

	/**
	 * @brief ���t�����̃`�F�b�N�B
	 *
	 * @param ipFeature	[in]	���ׂ�t�B�[�`��
	 * @param cIndex	[in]	�t�B�[�`���N���X�������C���f�b�N�X�i0:����,1:�i��,2:���C���j
	 * @param cHaveKind	[in]	�����t���ƌ`���i�����t���Ȃ�true�j
	 *
	 * @retval true		���t�����𖞂����Ă���i�폜�j
	 * @retval false	���t�����𖞂����Ă��Ȃ��i�폜�s�j
	 */
	bool fnDateCheck(const IFeaturePtr& ipFeature, int cIndex, bool cHaveKind);

	/**
	 * @brief �ƌ`�Ɋ܂܂��i���E���C���̂����폜�Ώ�ID��𒊏o�B
	 *
	 * @param ipGeom	[in]	�ƌ`�̃W�I���g��
	 * @param cIds		[out]	�폜�Ώۂ�ID��i�����E�i���E���C���̂R���j
	 * @param cIndex	[in]	�t�B�[�`���N���X�������C���f�b�N�X�i0:����,1:�i��,2:���C���j
	 * @param cHaveKind	[in]	�����t���ƌ`���i�����t���Ȃ�true�j
	 *
	 * @retval true		�S�č폜��
	 * @retval false	�폜�s�Ȃ��̂��܂܂��
	 */
	bool fnFindInner(const IGeometryPtr& ipGeom, ID_SET& cIds, int cIndex, bool cHaveKind);

	/**
	 * @brief ID��w��ŃI�u�W�F�N�g�̍폜�B
	 *
	 * @param cIds		[in]	�폜�Ώۂ�ID��
	 * @param cIndex	[in]	�t�B�[�`���N���X�������C���f�b�N�X�i0:����,1:�i��,2:���C���j
	 *
	 * @retval true		����
	 * @retval false	���s�E�G���[
	 */
	bool fnDeleteObject(const ID_SET& cIds, int cIndex);

	/**
	 * @brief ���b�V���EXY�`���̃��O�o�́B
	 *
	 * @param cLayerName	[in]	�t�B�[�`���N���X��
	 * @param ipFeature		[in]	�t�B�[�`���i���W�AID�擾�p�j
	 * @param cErrLv		[in]	�G���[���x���i0:INFO,1:WARNING,2:ERROR�j
	 * @param cMsg			[in]	�����ɏo�͂���i�G���[�j���b�Z�[�W
	 *
	 */
	void fnPrintLog(LPCTSTR cLayerName, const IFeaturePtr& ipFeature, int cErrLv, LPCTSTR cMsg);

	/**
	 * @brief �t�B�[�`�����烍�O�o�͗p�̃��b�V���EXY���v�Z�B
	 *
	 * @param ipFeature		[in]	�t�B�[�`��
	 * @param cMesh			[out]	���b�V���R�[�h
	 * @param cX			[out]	X���W
	 * @param cY			[out]	Y���W
	 *
	 * @retval true		����
	 * @retval false	���s�E�G���[
	 */
	bool fnGetMXY(const IFeaturePtr& ipFeature, int& cMesh, int& cX, int& cY);
};
