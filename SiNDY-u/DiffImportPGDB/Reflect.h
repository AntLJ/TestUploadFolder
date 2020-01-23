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
#include "Arguments.h"

// ���b�V�����X�g�R�}���h
enum COMMAND
{
	NONE = -1,   //!< �s��
	DIFF_UPDATE, //!< �����X�V
	ONLY_DELETE, //!< �폜�̂�
	ONLY_ADD,    //!< �ǉ��̂�
	DELETE_ADD,  //!< �폜�{�ǉ�(������������-N�I�v�V�����Ŏw�肵��PGDB�̓��e�𔽉f)
};
typedef std::map<long, COMMAND> MESH_MAP;	///< ���b�V�����̃R�}���h�ێ��p�}�b�v
typedef std::map<long, IGeometryPtr> MESH_POLY_MAP;	///< ���b�V���R�[�h�ƑΉ����郁�b�V���|���S���̕ێ��p�}�b�v

// �����t�B�[�`���N���X�̍ő吔
static const int fc_max_size = 6;
typedef std::array<CAdapt<IFeatureClassPtr>, fc_max_size> FCArray;

/**
 * @brief �ώ�O�E�ҏW��ESiNDY�̃t�B�[���h�C���f�b�N�X�ƃ}�[�W���̔�r�L����ێ����邽�߂̍\����
 */
struct FieldTable {
	LPCTSTR m_fieldName; //!< �t�B�[���h��
	long m_OldPgdbIndex; //!< �ҏW�OPGDB�p�t�B�[���h�C���f�b�N�X�i�ʏ�͕ҏW��Ɠ����͂��j
	long m_NewPgdbIndex; //!< �ҏW��PGDB�p�t�B�[���h�C���f�b�N�X�i�ʏ�͕ҏW�O�Ɠ����͂��j
	long m_SindyIndex;   //!< SiNDY��̃t�B�[���h�C���f�b�N�X
	bool m_Compare;      //!< ���b�V�����E�}�[�W���ɑ������r���邩�̃t���O
};

/**
 * @brief �g���q���Ƀt�B�[�`���N���X����t�B�[���h��񓙂�ێ����邽�߂̍\����
 */
struct EnvTable {
	LPCTSTR	m_Suffix;		///< VDB����̊g���q
	LPCTSTR	*m_OrgTableNames;	///< �Ώۃt�B�[�`���N���X���i�Q�j�Œ�
	CString	*m_TgtTableNames;	///< ���f��t�B�[�`���N���X���i�Q�j���ϐ��ŕύX�\
	FieldTable **m_Fields;	///< �t�B�[���h�e�[�u��
	long m_Type;			///< ���f�̃^�C�v�i���b�V�����E�̓��ʏ������K�v���Ɏg���j
};

enum {
	CHANGE_TYPE_NORMAL = 0,	///< �ʏ�C���|�[�g�^�C�v�i���b�V�����E�̓��ꏈ������j
	CHANGE_TYPE_POINTS = 1,	///< �|�C���g�C���|�[�g�^�C�v�i���b�V�����E���C�ɂ��Ȃ��j
	CHANGE_TYPE_POI = 2,	///< POI�^�C�v�i�C���|�[�g����������Ȃ̂Ń|�C���g�^�C�v�ƕ����Ă���j
};

namespace Reflect
{
	// TODO: �����e���₷���Ȃ�悤�ɂ���
	extern esriGeometryType g_ShapeType[fc_max_size];	///< �t�B�[�`���N���X�̃V�F�[�v�Ή�
	extern CString g_LayerName[fc_max_size];		///< ���f��̃t�B�[�`���N���X���i���O�o�͗p�j
	extern long g_OperatorIndex[fc_max_size];		///< �ŏI�X�V�҂̃t�B�[���h�C���f�b�N�X
	extern long g_UpdateIndex[fc_max_size];			///< �ŏI�X�V���e�̃t�B�[���h�C���f�b�N�X
	extern long g_PurposeIndex[fc_max_size];		///< ��ƖړI�̃t�B�[���h�C���f�b�N�X
	extern long g_ClaimIndex[fc_max_size];			///< �N���[���t���O�̃t�B�[���h�C���f�b�N�X
	extern long g_ProgNameIndex[fc_max_size];		///< �ŏI�X�V�v���O�������̃t�B�[���h�C���f�b�N�X
	extern long g_ModifyDateIndex[fc_max_size];		///< �ŏI�X�V���t�̃t�B�[���h�C���f�b�N�X
	extern long g_ProgModifyDateIndex[fc_max_size];	///< �ŏI�v���O�����X�V���t�̃t�B�[���h�C���f�b�N�X
	extern long g_SourceIndex[fc_max_size];			///< ���\�[�X�̃t�B�[���h�C���f�b�N�X
	extern long g_OrgOperatorIndex[fc_max_size];	///< ORG_OPERATOR(sindy-c)�̃t�B�[���h�C���f�b�N�X
	extern long g_OrgModifyIndex[fc_max_size];		///< ORG_MODIFYDATE(sindy-c)�̃t�B�[���h�C���f�b�N�X
	extern long g_OrgObjIdIndex[fc_max_size];		///< ORG_OBJID(sindy-c)�̃t�B�[���h�C���f�b�N�X
	extern long g_UpdateCIndex[fc_max_size];		///< UPDATE_C(sindy-c)�̃t�B�[���h�C���f�b�N�X
	extern long g_CheckCIndex[fc_max_size];			///< CHECK_C(sindy-c)�̃t�B�[���h�C���f�b�N�X
	extern long g_MeshcodeIndex[fc_max_size];		///< MESHCODE(sindy-c)�̃t�B�[���h�C���f�b�N�X
	extern long g_GouTypeIndex;		///< ���|�C���g�����荆�^�C�v�̃t�B�[���h�C���f�b�N�X
	extern long g_CurrentMesh;	///< ���ݏ��������b�V���R�[�h
	extern bool g_IsContinue;	///< �����̑��s�t���O�i���f����Ŏg�p�j
	extern CString g_Suffix;	///< ��ƒ��́iVDB����́j�g���q
	extern MESH_MAP g_MeshMap;	///< ���b�V�����X�g�����b�V�����̃R�}���h�}�b�v
	extern MESH_POLY_MAP g_MeshPolyMap;	///< ���b�V�����̃��b�V���|���S���}�b�v
	extern long g_NowProccess;		///< �������̍H���Ǘ��p
	extern int g_DataIndex;		///< �����Ώۂ�ێ�����e�[�u���̃C���f�b�N�X
	extern CString g_Operator;	///< �Z�b�g�p�̍ŏI�X�V�Җ�
	
	extern const EnvTable gEnvTable[];	///< �g���q���Ƀt�B�[�`���N���X����t�B�[���h��񓙂�ێ�����e�[�u��
	extern LPCTSTR GOUTYPE_NAME;		///< ���^�C�v�̃t�B�[���h��
	extern LPCTSTR PROG_NAME;			///< �v���O������
}

/**
 * @brief �ҏW�������f�𑍍��I�ɊǗ��E���s����N���X
 */
class CReflect
{
public:
	/**
	 * @brief �R���X�g���N�^�B
	 */
	CReflect(void);

	/**
	 * @brief �f�X�g���N�^�B
	 */
	~CReflect(void);

	/**
	 * @brief �������E���O�����B�����2��ڈȍ~�̕s�v�����͓����Ő���B
	 *
	 * @param cConnectStr [in] �T�[�o�ڑ�������
	 * @param arg         [in] ���s���I�v�V����
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool init(LPCTSTR cConnectStr, const CArguments& args);


	/**
	 * @brief �ҏW�������f��ƃ��C���֐��B���́i�ȑO�͌ʂ������j�@�\���g�ݍ��܂�Ă���
	 *
	 * @param arg [in] ���s���I�v�V����
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool execReflect( const CArguments& args );

	/**
	 * @brief �������E���O�����B�����2��ڈȍ~�̕s�v�����͓����Ő���B
	 *
	 * @param lpszConnectStr	[in]	�T�[�o�ڑ�������
	 *
	 * @retval NULL�ȊO	�ڑ��������[�N�X�y�[�X
	 * @retval NULL	���s�A�G���[
	 */
	static IWorkspacePtr connectSDE(LPCTSTR lpszConnectStr);

private:
	/**
	 * @brief ���ϐ����甽�f��̃t�B�[�`���N���X�����擾�B
	 */
	void fnGetEnv(void);

	/**
	 * @brief ����̂ݕK�v�ȏ������E���O�����B�t�H���_�̗L�����ꕔ�͊O�Ŏ��s�B
	 *
	 * @param arg       [in] ���s���I�v�V����
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool fnFirstOnlyInit(const CArguments& args);

	/**
	 * @brief ���ʂŎg�p����t�B�[���h�C���f�b�N�X�擾�B
	 *
	 * @param cIndex	[in]	�e�[�u���̉��Ԗڂ̃t�B�[�`���N���X���������C���f�b�N�X
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool fnGetCommonFieldIndex(int cIndex);

	/**
	 * @brief �w��t�B�[���h�̃t�B�[���h�C���f�b�N�X�����߂�
	 *
	 * @param cIndex     [in]  �e�[�u���̉��Ԗڂ̃t�B�[�`���N���X���������C���f�b�N�X
	 * @param fieldName  [in]  �t�B�[���h��
	 * @param fieldIndex [out] �t�B�[���h�C���f�b�N�X
	 *
	 * @retval true  �t�B�[���h�C���f�b�N�X�擾�ɐ���
	 * @retval false ���s�A�G���[
	 */
	bool fnFindFieldIndex(int cIndex, const CString& fieldName, long& fieldIndex);

	/**
	 * @brief�R�}���h�t�����b�V�����X�g�̓ǂݍ��݂ƃ��b�V���|���S���擾�B
	 *
	 * @param args	[in]	���s���I�v�V����
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool fnMeshLoad(const CArguments& args);


	/**
	 * @brief �㏈���֐��i�S���j������SDE����ؒf����j
	 */
	void fnUnInit(void);

private:
	CString m_OldPath;  //!< �ҏW�OPGDB�̃x�[�X�p�X
	CString m_NewPath;  //!< �ҏW��PGDB�̃x�[�X�p�X

	IWorkspacePtr m_ipWorkspace;    //!< �C���|�[�g�惏�[�N�X�y�[�X
	FCArray m_ipFeatureClasses;     //!< �C���|�[�g�Ώۃt�B�[�`���N���X�̔z��
	ISpatialReferencePtr m_ipSpRef; //!< ��ԎQ��

	bool m_sindyCImport; //!< �C���|�[�g�悪SiNDY-c�X�L�[�}���ǂ���
};
