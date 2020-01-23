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
 * @brief ���f�E�s�ƂȂ����I�u�W�F�N�g��ID�Ǘ��p�\����
 */
struct CHG_ID{
	ID_SET m_OK;	///< ���f�\���������̂�PGDB�̃I�u�W�F�N�gID�̃Z�b�g
	ID_SET m_NG;	///< ���f�s���������̂�PGDB�̃I�u�W�F�N�gID�̃Z�b�g
};

typedef std::map<long, long> ID_MAP;	///< SDE���OID��PGDB��OID�̑Ή��p�}�b�v
typedef std::map<CString, ID_MAP> ID_MAP_MAP;	///< ���C�����ƑΉ�����ID�̑Ή��p�}�b�v�̃}�b�v
typedef std::map<CString, CHG_ID> ID_SET_MAP;	///< ���C������ID�Z�b�g�̃}�b�v
typedef std::map<long, long> LQ_MAP;	///< LQ�̕��тƃ����NID�Ǘ��p�}�b�v�i���я��Ƀ\�[�g�����������j

/**
 * @brief �ҏW�������f����i�����u��C�f�[�^�C���|�[�g�v�̃��C���j�N���X
 *        TODO: ���L���W�b�N���v���I�̂��߁A�s���I�u�W�F�N�g���ł��邱�Ƃ���Ŋm�F���ďC���⌳�ɖ߂��Ă���
 *        TODO: �����N�ύX�E�폜�������N�ǉ����m�[�h�ύX�E�폜���m�[�h�ǉ����E�E�E
 *        TODO: �� ex:�����N���f���A�����N�̕ύX�E�폜���ł��Ă��A�ǉ����ł����A����Ƀm�[�h�����Ȃ����߁A�k�����Ē[�_�m�[�h�Ȃ������N���ł���
 *        TODO:    ex:�����N1�{�폜��1�{�t���ւ����A�폜OK�A�t���ւ�NG�̂��߁A���������N���ł���
 *        TODO: �y�ύX�āz�ύX�E�폜�E�ǉ��𕪂��Ȃ��A���C�����Ƃ̏����ɕ������A�֘A�I�u�W�F�N�g�͈�Z�b�g�ŏ����ł���悤�C������
 *        TODO: �i��Ή��ł��ɂ����E�̂��߁A���̏C���̍ۂɂ́A���IBefore/After���K�v�j
 *        TODO: �y����v�]�zBug9997�̔���̐��퉻�A��Ή��ɔ����A���x���x���Ȃ�A�^�p�㌵�����ƌ����Ă��邽�߁A���x���P�����킹�ĕK�v
 *        TODO: �i�܂��Ƃ��ȏ����ɂ�����x�߂������߁A�x���Ȃ�̂͂��傤���Ȃ��Ƃ͓`���Ă܂��j
 *        TODO: �y�č쐬�̍ۂ̃\�[�X���p�z�F�X���r���[�w�E����u���Ă���̂ŁA���p����ꍇ�́A�K���w�E�ɉ����ďC�����邱��
 *        TODO: �ir2331�Q�Ƃ̂��Ɓj
 */
class CImport :
	public CSDEBase
{
public:
	/**
	 * @brief �R���X�g���N�^
	 */
	CImport(CLayerManage* cLayerManage);

	/**
	 * @brief �f�X�g���N�^
	 */
	virtual ~CImport(void);

	/**
	 * @brief �������֐�
	 *
	 * @param cOldPath		[in]	�ҏW�OPGDB�̃x�[�X�p�X
	 * @param cNewPath		[in]	�ҏW��PGDB�̃x�[�X�p�X
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool init(LPCTSTR cOldPath, LPCTSTR cNewPath);

private:
	CString m_OldPath;			///< �ҏW�OPGDB�̃x�[�X�p�X
	CString m_NewPath;			///< �ҏW��PGDB�̃x�[�X�p�X
	ID_SET m_AppendID;			///< ���b�V�����E�̑����ύX�ɂ��A�V�K�ǉ������ɂȂ������̂��Ǘ�����ID�Z�b�g�i�|���S���p�j
	ID_SET_MAP m_IDSets;		///< �����ς݂̃I�u�W�F�N�g�̌���ID�����C���P�ʂŊǗ�����Z�b�g
	ID_MAP_MAP m_IDMaps;		///< SDE��PGDB��ID�̑Ή��}�b�v�����C�����ƂɊǗ�����}�b�v�i���b�V�����ɃN���A���邱�Ɓj
	TABLE_INFO m_OldTableInfo;	///< �ҏW�OPGDB�p���C�����ƃ��[�U���t���e�[�u�����i���O�o�͗p�j�E�e�[�u���̃}�b�v

	/**
	 * @brief ���b�V���P�ʂł̍����C���|�[�g�i���f�j�֐�
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool fnExecMesh(void);

	/**
	 * @brief PGDB�I�[�v���֐�
	 *
	 * @param cBasePath		[in]	PGDB�x�[�X�p�X
	 *
	 * @retval NULL�ȊO	PGDB�̃��[�N�X�y�[�X
	 * @retval NULL		���s�A�G���[
	 */
	IWorkspacePtr fnOpenLocalDatabase(LPCTSTR cBasePath) const;

	/**
	 * @brief PGDB���̎w�背�C�����̃e�[�u���I�[�v���֐�
	 *
	 * @param ipWorkspace	[in]	PGDB�̃��[�N�X�y�[�X
	 * @param cLayerName	[in]	���C����
	 *
	 * @retval NULL�ȊO	�e�[�u��
	 * @retval NULL		���s�A�G���[
	 */
	ITablePtr fnOpenLocalTable(const IWorkspacePtr& ipWorkspace, LPCTSTR cLayerName) const;

	/**
	 * @brief 	��r�ƕύX�E�폜�֐�
	 *          ���b�V�����E���ӎ�������̂Ə����𕪂��Ă������A����Ȃ͔̂w�i�i�|���S���j�̎������Ȃ̂ŁA���ŕ�����
	 *
	 * @param ipOldCTable	[in]	�ҏW�O�̃t�B�[�`���N���X
	 * @param ipNewTable	[in]	�ҏW��̃t�B�[�`���N���X
	 * @param cEnv			[in]	�e�����ێ������\����
	 * @param cGetIDOnly	[in]	OID�̑Ή��������擾���邩�̃t���O
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool fnCompare(const ITablePtr& ipOldCTable, const ITablePtr& ipNewTable, EnvTable& cEnv, bool cGetIDOnly);

	/**
	 * @brief 	�V�K�ǉ������ꊇ�ǉ��֐�
	 *
	 * @param ipTable	[in]	�ҏW��̃e�[�u��
	 * @param cEnv		[in]	�e�����ێ������\����
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool fnAddNew(const ITablePtr& ipTable, EnvTable& cEnv);

	/**
	 * @brief 	Export���SiNDY��ɕύX��������ꂽ���A�X�V���t2��̔�r�Ŕ��f����֐�
	 *          �֐����Ɩ߂�l���t���ۂ��C�����邪�A�X�V�ł��Ȃ��E�X�V���Ă͂����Ȃ�����false��Ԃ�
	 *
	 * @param cEnv          [in]  �e�����ێ������\����
	 * @param cObjID        [in]  SDE��̃I�u�W�F�N�gID
	 * @param ipOrgRow      [in]  SDE��̃t�B�[�`���iNULL�̎��͓����Ō�������j
	 * @param ipPgdbRow     [in]  �ҏW��iPGDB�j�̃t�B�[�`��
	 * @param cChgType      [in]  �X�V���悤�Ƃ��Ă�����e�i���O�o�͗p�j
	 * @param ipTable[2]    [in]  �X�V�Ώۂ̃e�[�u��(�����N���[0]:INF�A[1]:LQ�B����ȊO��[0]�����g�p
	 * @param cTblType		[in]  INF��LQ��
	 * @param cPgdbIndexMap	[in]  PGDB�̃t�B�[���h�C���f�b�N�X�}�b�v
	 * @param cSdeIndexMap	[in]  SDE�̃t�B�[���h�C���f�b�N�X�}�b�v
	 * @param cTableName	[in]  ���ʗp�̃e�[�u������
	 *
	 * @retval true		�ύX�Ȃ����͕ύX����ł��X�V�͉�
	 * @retval false	�폜����Ă��閔�͍����D�惂�[�h�ŕύX����Ă���
	 */
	bool fnSiNDYChange(EnvTable& cEnv, long cObjID, _IRowPtr& ipOrgRow, const _IRowPtr& ipPgdbRow, LPCTSTR cChgType, const ITablePtr ipTable[2] );
	bool fnSiNDYChange(LqEnvTable& cEnv, long cTblType, long cObjID, _IRowPtr& ipOrgRow, const _IRowPtr& ipPgdbRow, LPCTSTR cChgType, const ITablePtr ipTable[2] );
	bool fnSiNDYChange(INDEX_MAP& cPgdbIndexMap, INDEX_MAP& cSdeIndexMap, const CString& cTableName, long cObjID, _IRowPtr& ipOrgRow, const _IRowPtr& ipPgdbRow, LPCTSTR cChgType, const ITablePtr& ipTable );

	/**
	 * @brief 	���b�V�����܂������I�u�W�F�N�g���𔻒肷��֐�
	 *
	 * @param cEnv			[in]	�e�����ێ������\����
	 * @param cObjID	[in]	SDE��̃I�u�W�F�N�gID
	 * @param ipFeature	[in]	SDE��̃t�B�[�`���iNULL�̎��͓����Ō�������j
	 *
	 * @retval true		�ύX�Ȃ����͕ύX����ł��X�V�͉�
	 * @retval false	�폜����Ă��閔�͍����D�惂�[�h�ŕύX����Ă���
	 */
	bool fnOverMesh(EnvTable& cEnv, long cObjID, IFeaturePtr ipFeature = NULL) const;

	/**
	 * @brief 	���b�V�����܂������I�u�W�F�N�g�̍X�V�֐�
	 *
	 * @param ipNewClass	[in]	�ҏW��̃t�B�[�`���N���X
	 * @param cEnv			[in]	�e�����ێ������\����
	 * @param cObjID		[in]	SDE��̃I�u�W�F�N�gID
	 * @param ipNewFeature	[in]	�ҏW��̃t�B�[�`���i�폜�Ώۂ̎���NULL�j
	 * @param cStatus		[in]	�X�V�X�e�[�^�X
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool fnClipAndApend(const IFeatureClassPtr& ipNewClass, EnvTable& cEnv, long cObjID, const IFeaturePtr& ipNewFeature, long cStatus);

	/**
	 * @brief 	���b�V�����܂����ł��Ȃ����̂̒P���X�V�֐�
	 *
	 * @param ipNewRow	[in]	�ҏW��̃��E
	 * @param cEnv		[in]	�e�����ێ������\����
	 * @param cObjID	[in]	SDE��̃I�u�W�F�N�gID
	 * @param cStatus	[in]	�X�V�X�e�[�^�X
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool fnReflectToSiNDY(const _IRowPtr& ipNewRow, EnvTable& cEnv, long cObjID, long cStatus);

	/**
	 * @brief 	INF,LQ�p�X�V�֐�
	 *
	 * @param ipNewRow	[in]	�ҏW��̃��E
	 * @param cEnv		[in]	�e�����ێ������\����
	 * @param cObjID	[in]	SDE��̃I�u�W�F�N�gID
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool fnInfReflectToSiNDY(const _IRowPtr& ipNewRow, LqEnvTable& cEnv, long cObjID);

	/**
	 * @brief 	���f�Ώۑ����̃R�s�[�֐�
	 *
	 * @param ipTgtRow		[in/out]	�R�s�[�惍�E
	 * @param ipSrcRow		[in]		�R�s�[�����E
	 * @param cSdeIndex		[in]		�e�����ێ������\����
	 * @param cPgdbIndex	[in]		�e�����ێ������\����
	 * @param cField		[out]		�X�V�����t�B�[���h���i���O�o�͗p�j
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool fnReflectAttr(_IRowPtr& ipTgtRow, const _IRowPtr& ipSrcRow, INDEX_MAP& cSDEIndex, INDEX_MAP& cPGDBIndex, CString& cField);

	/**
	 * @brief 	���b�V���|���S���ŃN���b�v�����`������߂�֐�
	 *          fnGetRemakeShape�Ǝ��Ă���悤�Ɍ����邪�A������́A���b�V���|���S���Ő؂������c��̃W�I���g�������߂�
	 *
	 * @param ipFeature		[in]	�Ώۃt�B�[�`��
	 * @param cEnv			[in]	�e�����ێ������\����
	 * @param cObjID		[in]	SDE��̃I�u�W�F�N�gID
	 * @param cForceMode	[in]	true�F�P���ɃN���b�v�����`������߂� false:���b�V�����E���֌W���邩���ȈՓI�ɋ��߂�i�H�j���O�Ɠ��e�̊֌W���킩��Ȃ��Ȃ���
	 *
	 * @retval NULL�ȊO	���Z��̃W�I���g��
	 * @retval NULL		���s�A�G���[
	 */
	IGeometryPtr fnGetClipShape(const IFeaturePtr& ipFeature, EnvTable& cEnv, long cObjID, bool cForceMode = false) const;

	/**
	 * @brief 	���b�V�����E�̃I�u�W�F�N�g�ɑ΂��āA���Y���b�V���O�����Ɠ��Y���b�V�����̐؂�\����s���֐�
	 * �@�@�@�@ �}���`�p�[�g�ɂȂ邱�Ƃ�����A�I���W�i��ID���������̂�1�ɂ܂Ƃ߂悤�Ƃ��邪�A�������ύX�ɂȂ��Ă�����̂�����ΐV�K�ǉ������Ƃ���
	 *
	 * @param ipNewClass	[in]	�ҏW��̃t�B�[�`���N���X
	 * @param ipFeature		[in]	�ҏW��̃t�B�[�`��
	 * @param cIndex		[in]	�e�[�u���̉��Ԗڂ̃t�B�[�`���N���X���������C���f�b�N�X�i0�`2�j
	 * @param cObjID		[in]	SDE��̃I�u�W�F�N�gID
	 *
	 * @retval NULL�ȊO	���Z��̃W�I���g��
	 * @retval NULL		���s�A�G���[
	 */
	IGeometryPtr fnGetRemakeShape(const IFeatureClassPtr& ipNewClass, const IFeaturePtr& ipFeature, EnvTable& cEnv, long cObjID);

	/**
	 * @brief 	���C���P�ʂŕύX�𔽉f����֐�
	 * �@�@�@�@ �ucGetIDOnly�v���utrue�v�̎���ID�̑Ή����擾���邾���Ŕ��f�͂��Ȃ�
	 *
	 * @param ipOldWorkspace	[in]	�ҏW�O��PGDB�̃��[�N�X�y�[�X
	 * @param ipNewWorkspace	[in]	�ҏW���PGDB�̃��[�N�X�y�[�X
	 * @param cEnv				[in]	�e�����ێ������\����
	 * @param cGetIDOnly		[in]	ID�̑Ή������擾���邩�̃t���O
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool fnReflectGeneral(const IWorkspacePtr& ipOldWorkspace, const IWorkspacePtr& ipNewWorkspace, EnvTable& cEnv, bool cGetIDOnly);

	/**
	 * @brief 	INF�ELQ�̃y�A�P�ʂŕύX�𔽉f����֐�
	 *
	 * @param ipOldWorkspace	[in]	�ҏW�O��PGDB�̃��[�N�X�y�[�X
	 * @param ipNewWorkspace	[in]	�ҏW���PGDB�̃��[�N�X�y�[�X
	 * @param cEnv				[in]	�e�����ێ������\����
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool fnReflectLQ(const IWorkspacePtr& ipOldWorkspace, const IWorkspacePtr& ipNewWorkspace, LqEnvTable& cEnv);

	/**
	 * @brief 	�������́i�����ʒu���͓����m�[�hID�E�����NID�����ꎞ��~�|�C���g�j�����ɑ��݂��邩�𒲂ׂ�֐�
	 *
	 * @param ipFeature	[in]	���ׂ錳�ɂȂ�t�B�[�`��
	 * @param cEnv		[in]	�e�����ێ������\����
	 *
	 * @retval true		���݂���
	 * @retval false	���݂��Ȃ�
	 */
	bool fnExistSamePoint(const IFeaturePtr& ipFeature, EnvTable& cEnv);

	/**
	 * @brief 	�����C����ID�������t�B�[���h�ɂ��āA���̎����Ă���I�u�W�F�N�g�̕ω��𒲂ׂ�֐�
	 *�@�@�@�@�@�߂�l���u���f���Ă悢���v�������Ă���̂ŁA���̃t�B�[���h�������Ă��Ȃ��ꍇ��֘A�I�u�W�F�N�g�ɕύX���Ȃ����true���Ԃ�A
	 *�@�@�@�@�@�ύX���������ꍇ��G���[���폜����Ă���ꍇ��false���Ԃ�B
	 *�@�@�@�@�@�ύX���������ꍇ�ł��A���ꂪ�{�v���O�������s���ɕύX���ꂽ���̂ł���΁Atrue���Ԃ�
	 *
	 * @param ipRow			[in]	���ׂ錳�ɂȂ郍�E
	 * @param cPgdbIndexMap	[in]	PGDB�̃t�B�[���h�C���f�b�N�X�}�b�v
	 * @param cTableName	[in]	���ʗp�̃e�[�u������
	 * @param cFieldName	[in]	���̃��C����ID�������t�B�[���h��
	 * @param ipGeom		[in]	���O�o�͗p�̃W�I���g��
	 * @param cRelLayer		[out]	�֘A�I�u�W�F�N�g�̃��C����
	 *
	 * @retval true		�X�V�\
	 * @retval false	�ύX���ꂽ�A�G���[�A�폜���ꂽ���̗��R�ōX�V�s��
	 */
	bool fnRelObjChange(const _IRowPtr& ipRow, INDEX_MAP& cPgdbIndexMap, const CString& cTableName, const CString& cFieldName, const IGeometryPtr& ipGeom, CString& cRelLayer);

	/**
	 * @brief 	�����C����ID�������t�B�[���h�ɂ��āA���̎����Ă���I�u�W�F�N�g�̕ω��𒲂ׂ�֐�
	 *�@�@�@�@�@�֌W����t�B�[���h���ufnRelObjChange�v���ĂԁB�ڍׂ́ufnRelObjChange�v�Q��
	 *
	 * @param ipRow			[in]	���ׂ錳�ɂȂ郍�E
	 * @param cPgdbIndexMap	[in]	PGDB�̃t�B�[���h�C���f�b�N�X�}�b�v
	 * @param cTableName	[in]	���ʗp�̃e�[�u������
	 * @param ipGeom		[in]	���O�o�͗p�̃W�I���g��
	 * @param cRelLayer		[out]	�֘A�I�u�W�F�N�g�̃��C����
	 *
	 * @retval true		�X�V�\
	 * @retval false	�ύX���ꂽ�A�G���[�A�폜���ꂽ���̗��R�ōX�V�s��
	 */
	bool fnRelObjsChange(const _IRowPtr& ipRow, INDEX_MAP& cPgdbIndexMap, const CString& cTableName, const IGeometryPtr& ipGeom, CString& cRelLayer);

	/**
	 * @brief 	�Q�̃t�B�[�`���ɂ��āA�`��̈�v�𒲂ׂ�֐�
	 *
	 * @param ipFeature1		[in]	�`����r����t�B�[�`���P
	 * @param ipFeature2		[in]	�`����r����t�B�[�`���Q
	 *
	 * @retval true		�`���v
	 * @retval false	�`�󂪈Ⴄ�i��v���Ȃ��j
	 */
	bool fnShapeComp(const IFeaturePtr& ipFeature1, const IFeaturePtr& ipFeature2) const;

	/**
	 * @brief 	�����N��̕ύX�ƍ폜�֐�
	 *
	 * @param ipOldCTable	[in]	�ҏW�O�̃e�[�u���iINF,LQ�j
	 * @param ipNewTable	[in]	�ҏW��̃e�[�u���iINF,LQ�j
	 * @param cEnv			[in]	�e�����ێ������\����
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool fnInfModAndDel(const ITablePtr ipOldTable[2], const ITablePtr ipNewTable[2], LqEnvTable& cEnv);

	/**
	 * @brief 	�����N��̐V�K�ǉ��֐�
	 *
	 * @param ipOldCTable	[in]	�ҏW�O�̃e�[�u���iINF,LQ�j
	 * @param ipNewTable	[in]	�ҏW��̃e�[�u���iINF,LQ�j
	 * @param cEnv			[in]	�e�����ێ������\����
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool fnInfAdd(const ITablePtr ipOldTable[2], const ITablePtr ipNewTable[2], LqEnvTable& cEnv);

	/**
	 * @brief 	LQ�̕ύX��Ԃ����Ȃ���ύX�\���𒲂ׂ�֐�
	 *
	 * @param cInfID        [in]  INF�̃I�u�W�F�N�gID�iPGDB�j
	 * @param ipOldTable    [in]  �ҏW�O�̃e�[�u���iLQ�j
	 * @param ipNewTable    [in]  �ҏW��̃e�[�u���iLQ�j
	 * @param ipSdeTable[2] [in]  SDE��̃e�[�u���i[0]:INF, [1]:LQ�j
	 * @param cEnv          [in]  �e�����ێ������\����
	 * @param cAdd          [out] �ǉ����ꂽLQ��ID�Z�b�g	
	 * @param cMod          [out] �ύX���ꂽLQ��ID�Z�b�g
	 * @param aDel          [out] �폜���ꂽLQ��ID�Z�b�g
	 *
	 * @retval true		�ύX�\
	 * @retval false	�ύX�s��
	 */
	bool fnCanRefrectLq(long cInfID, const ITablePtr& ipOldTable, const ITablePtr& ipNewTable, const ITablePtr ipSdeTable[2], LqEnvTable& cEnv, ID_SET& cAdd, ID_SET& cMod, ID_SET& aDel);

	/**
	 * @brief 	�����̕ύX�̗L���𒲂ׂ�֐�
	 *�@�@�@�@�@�P���Ȕ�r�ł͂Ȃ��A���Ȃ��ėǂ��i���Ă͂����Ȃ��j���������ȂǁA�����ɓs���̗ǂ���r������
	 *
	 * @param ipSrcRow		[in]	��r���̃��E
	 * @param cSrcIndex		[in]	��r���p�̃t�B�[���h�C���f�b�N�X�}�b�v
	 * @param ipTgtRow		[in]	��r��̃��E
	 * @param cTgtIndex		[in]	��r��p�̃t�B�[���h�C���f�b�N�X�}�b�v
	 *
	 * @retval true		�ύX����
	 * @retval false	�ύX�Ȃ�
	 */
	bool fnIsAttrChange(const _IRowPtr& ipSrcRow, const INDEX_MAP& cSrcIndex, const _IRowPtr& ipTgtRow, const INDEX_MAP& cTgtIndex);

	/**
	 * @brief 	�Ώ�ID��LQ���폜����֐�
	 *�@�@�@�@�@�폜�\�ł��邱�Ƃ͎��O�Ƀ`�F�b�N����Ă��邱��
	 *
	 * @param cDelIDs		[in]	�폜�Ώۂ�LQ�̃I�u�W�F�N�gID�Z�b�g
	 * @param cLayerName	[in]	LQ�̃��C�����i���O�p�j
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool fnDeleteLQs(const ID_SET& cDelIDs, LPCTSTR cLayerName);

	/**
	 * @brief 	�Ώ�ID��LQ��ǉ�����֐�
	 *�@�@�@�@�@�ǉ��\�ł��邱�Ƃ͎��O�Ƀ`�F�b�N����Ă��邱��
	 *
	 * @param cAddIDs		[in]	�폜�Ώۂ�LQ�̃I�u�W�F�N�gID�Z�b�g
	 * @param cInfID		[in]	LQ�����L����i�H�jINF��ID
	 * @param ipNewTable	[in]	�ҏW��̃e�[�u���iLQ�j
	 * @param cEnv			[in]	�e�����ێ������\����
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool fnAddLQs(const ID_SET& cAddIDs, long cInfID, const ITablePtr& ipNewTable, LqEnvTable& cEnv);

	/**
	 * @brief 	�Ώ�ID��LQ��ύX����֐�
	 *�@�@�@�@�@�ύX�\�ł��邱�Ƃ͎��O�Ƀ`�F�b�N����Ă��邱��
	 *
	 * @param cModIDs		[in]	�ύX�Ώۂ�LQ�̃I�u�W�F�N�gID�Z�b�g
	 * @param ipNewTable	[in]	�ҏW��̃e�[�u���iLQ�j
	 * @param cEnv			[in]	�e�����ێ������\����
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool fnModLQs(const ID_SET& cModIDs, const ITablePtr& ipNewTable, LqEnvTable& cEnv);

	/**
	 * @brief 	���b�V�����E�Ɋւ�閔�̓��b�V���͈͊O���𔻒肷��֐�
	 *
	 * @param ipGeom	[in]	���肷��W�I���g��
	 *
	 * @retval true		���b�V�����E�Ɋւ�邩�͈͊O�i�����ΏۊO�j
	 * @retval false	���S�ɓ����Ă���i�����Ώہj
	 */
	bool fnMeshRelObject(const _IRowPtr& ipGeom) const;

	/**
	 * @brief 	�Q�Ƃ��Ă��鑼�̃��C����ύX�֎~���X�g�ɒǉ�����֐��i�����NID,�m�[�hID���S���j
	 *
	 * @param ipRow			[in]	�����Ώۂ̃��E
	 * @param cPgdbIndexMap	[in]	�Ώۂ̃t�B�[���h�C���f�b�N�X�}�b�v
	 *
	 * @retval true		�i�Œ�j
	 */
	bool fnProhibitRelIDAll(const _IRowPtr& ipRow, INDEX_MAP& cPgdbIndexMap);

	/**
	 * @brief 	�Q�Ƃ��Ă��鑼�̃��C����ύX�֎~���X�g�ɒǉ�����֐��i�t�B�[���h���w��j
	 *
	 * @param ipRow			[in]	�����Ώۂ̃��E
	 * @param cPgdbIndexMap	[in]	�Ώۂ̃t�B�[���h�C���f�b�N�X�}�b�v
	 * @param cFieldName	[in]	���̃��C����ID��ێ����Ă���t�B�[���h��
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool fnProhibitRelID(const _IRowPtr& ipRow, INDEX_MAP& cPgdbIndexMap, const CString& cFieldName);

	/**
	 * @brief 	�����N��Ɗ֘A�t���O�̐��������֐�
	 *�@�@�@�@�@�ύX�ł��Ȃ����������N��ɂ��āA�t���O�̕s�������N�������Ƃ����邽�߁A���̌�n��
	 *
	 * @param cEnv			[in]	�e�����ێ������\����
	 *
	 * @retval true		�i�Œ�j
	 */
	bool fnLinkFlagCare(LqEnvTable& cEnv);

	/**
	 * @brief 	ID��w��Ń����N��Ɗ֘A�t���O�̐��������֐�
	 *
	 * @param ipClass		[in]	���H�����N�̃t�B�[�`���N���X
	 * @param cFlagIndex	[in]	�֘A�t���O�̃t�B�[���h�C���f�b�N�X
	 * @param cIDSet		[in]	�����Ώۂ̃����NID��
	 *
	 * @retval true		�i�Œ�j
	 */
	bool fnLinkFlagCareByID(const IFeatureClassPtr& ipClass, long cFlagIndex, const ID_SET& cIDSet);

	/**
	 * @brief 	�����N��ID�����Ƀ����N��ŎQ�Ƃ���Ă��邩�𔻒肷��֐�
	 *�@�@�@�@�@�߂�l�����̂܂܊֘A�t���O�̒l�Ƃ��邽�߂ɐ��l��0/1��Ԃ�
	 *
	 * @param cObjID	[in]	�����N�̃I�u�W�F�N�gID
	 *
	 * @retval 0	�Q�Ƃ���Ă��Ȃ�
	 * @retval 1	�Q�Ƃ���Ă���
	 */
	int fnIsExistLQ(long cObjID) const;

	/**
	 * @brief 	�����Ƃ݂Ȃ�����̂����ɑ��݂��邩�𒲂ׂ�֐�
	 *
	 * @param ipRow	[in]	���ׂ錳�ɂȂ�t�B�[�`��
	 * @param cEnv	[in]	�e�����ێ������\����
	 *
	 * @retval true		���݂���
	 * @retval false	���݂��Ȃ�
	 */
	bool fnExistSameObj(const _IRowPtr& ipRow, EnvTable& cEnv);

	/**
	 * @brief 	�ŗL�����i���ʑ����ȊO�Łj�̍��ق𒲂ׂ�֐�
	 *�@�@�@�@�@�u����Ƃ݂Ȃ�����̂����݂��邩�v�Ŏg�p����̂ŁA��r�Ώۂ͕K�v�ɉ����Ē����̂���
	 *
	 * @param ipPgdbRow	[in]	PGDB�̃��E
	 * @param ipSdeRow	[in]	SDE�̃��E
	 * @param cEnv		[in]	�e�����ێ������\����
	 *
	 * @retval true		����Ƃ݂Ȃ�
	 * @retval false	�ʕ��Ƃ݂Ȃ�
	 */
	bool fnSameAttr(const _IRowPtr& ipPgdbRow, const _IRowPtr& ipSdeRow, EnvTable& cEnv);

	/**
	 * @brief 	�Q�̃t�B�[�`���ɂ��āA�`��̕�����v�i�Ƃ肠����2�_�ȏ㓯���j�𒲂ׂ�֐�
	 *
	 * @param ipFeature1		[in]	�`����r����t�B�[�`���P
	 * @param ipFeature2		[in]	�`����r����t�B�[�`���Q
	 *
	 * @retval true		�`�󕔕���v�Ƃ݂Ȃ�
	 * @retval false	�ʕ��Ƃ݂Ȃ�
	 */
	bool fnShapePartComp(const IFeaturePtr& ipFeature1, const IFeaturePtr& ipFeature2) const;

	/**
	 * @brief 	�����Ƃ݂Ȃ��郊���N�񂪊��ɑ��݂��邩�𒲂ׂ�֐�
	 *
	 * @param ipLqTable    [in]  LQ�e�[�u���i�ǉ�������̂������Ă���PGDB�́j
	 * @param ipNewRow     [in]  ���ׂ錳�ɂȂ�t�B�[�`��
	 * @param cEnv         [in]  �e�����ێ������\����
	 * @param ipSdeLqTable [in]  SDE���LQ�e�[�u��
	 *
	 * @retval true		���݂���
	 * @retval false	���݂��Ȃ�
	 */
	bool fnExistSameLQ(const ITablePtr& ipLqTable, const _IRowPtr& ipNewRow, LqEnvTable& cEnv, const ITablePtr& ipSdeLqTable );

	/**
	 * @brief 	INF��ID������LQ�����������NID�����ԂɂȂ�悤�擾����֐�
	 *
	 * @param ipLqTable		[in]	LQ�e�[�u��
	 * @param cInfID		[in]	INF�̃I�u�W�F�N�gID
	 * @param cLqIndexMap	[in]	LQ�̃t�B�[���h�C���f�b�N�X�}�b�v
	 * @param cIsPGDB		[in]	PGDB���̃t���O�iID�̕ϊ������邩�ۂ��Ŏg�p�j
	 * @param cLqMap		[out]	LQ�����������N�̏��ԁi�V�[�P���X-�����NID�̃}�b�v�j
	 *
	 * @retval true		���݂���
	 * @retval false	���݂��Ȃ�
	 */
	bool fnGetLqInfo(const ITablePtr& ipLqTable, long cInfID, INDEX_MAP& cLqIndexMap, bool cIsPGDB, LQ_MAP& cLqMap);

	/**
	 * @brief  �����N��ID����A���g�̊֘AInf/Lq���X�V�\���m�F
	 * @note   �����ł̊֘AInf/Lq�Ƃ́A���g��Inf�Ƃ���InfID�����S�Ă�Lq���Ώ�
	 * @param  ipOldTable  [in]  �X�V�OPGDB��Inf/Lq�e�[�u��
	 * @param  ipNewTable  [in]  �X�V��PGDB��Inf/Lq�e�[�u��
	 * @param  ipSdeTable  [in]  SDE��Inf/Lq�e�[�u��
	 * @param  env         [in]  �e�����ێ������\����
	 * @param  linkPgdbOID [in]  �����N��ObjectID(PGDB��ID)
	 * @retval true        �Y�������N���X�VOK
	 * @retval false       �Y�������N���X�VNG
	 */
	bool fnCanRelInfLqUpdate( const ITablePtr ipOldTable[2], const ITablePtr ipNewTable[2], const ITablePtr ipSdeTable[2], LqEnvTable& env, long linkPgdbOID );

};
