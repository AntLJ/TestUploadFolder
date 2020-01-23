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

/**
 * @brief �t�B�[���h�C���f�b�N�X�ƃG�C���A�X��ێ�����\����
 *
 */
struct FieldInfo{
	long m_Index;	///< �t�B�[���h�C���f�b�N�X
	CString m_Alias;	///< �G�C���A�X�i�t�B�[���h�̓��{��\���p�j
	/**
	 * @brief �R���X�g���N�^
	 */
	FieldInfo(void)
	{
	}
	/**
	 * @brief �R���X�g���N�^
	 */
	FieldInfo(long cIndex, LPCTSTR cAlias)
	{
		m_Index = cIndex;
		m_Alias = cAlias;
	}
};

typedef std::set<long> MESH_SET;	///< ���b�V�����X�g�ێ��p�Z�b�g
typedef std::map<CString, FieldInfo> INDEX_MAP;	///< �t�B�[���h���ƃt�B�[���h�C���f�b�N�X�Ή��p�}�b�v

/**
 * @brief �t�B�[�`���N���X�P�ʂŁA�o�͂ɕK�v�ȏ���ێ�����\���́i��ʃt�B�[�`���N���X�p�j
 *        ID�̕t���ւ�������i��������Ȃ��j
 *        TODO: PGDB��2���݂���̂ɃC���f�b�N�X��map��1�����Ȃ��̂ŁA�V��PGDB�̃t�B�[���h�̂��肪�Ⴄ�Ɗ�Ȃ����߁A�v�C��
 */
struct EnvTable {
	CString	m_Tag;	///<�@�f�[�^���ʁF��ɐi���\���p�i�Œ�l�ŏ������j
	CString	m_OrgTable;	///< PGDB�̃t�B�[�`���N���X�������C�������ʂ̃^�O�i�Œ�l�ŏ������F���[�U�����܂܂��A�s�ρj
	CString m_SdeTable;		///< SDE�̃t�B�[�`���N���X���i1���s������1�񏉊���:���[�U�����t�����肷�邱�Ƃ�����j
	INDEX_MAP m_SDEIndexMap;	///< SDE�̃t�B�[���h���ƃt�B�[���h�C���f�b�N�X�̊֌W�i1���s������1�񏉊����j
	INDEX_MAP m_PGDBIndexMap;	///< PGDB�̃t�B�[���h���ƃt�B�[���h�C���f�b�N�X�̊֌W�i�O�̂��ߊe���b�V�����Ƃɏ������j
	bool m_IsImport;	///< �����Ώۂ��̃t���O(���g�p�F�s�v�ɂȂ邩��)
	//NAME_SET m_ExcludeFields;	///< �t�B�[���h���O���X�g
	EnvTable(LPCTSTR cTag, LPCTSTR cTableName){
		m_Tag = cTag;
		m_OrgTable = m_SdeTable = cTableName;	//�����l�͈ꏏ
		m_IsImport = true;	//�f�t�H���g�Łu��������v�ł����̂��͌�Ō���
	}
};

/**
 * @brief �����N��̒P�ʁiLQ��INF�j�ŁA�o�͂ɕK�v�ȏ���ێ�����\���́j
 */
struct LqEnvTable {
	CString	m_Tag;	///<�@�f�[�^���ʁF��ɐi���\���p�i�Œ�l�ŏ������j
	CString	m_OrgTables[2];	///< PGDB�̃e�[�u���������C�������ʂ̃^�O�i�Œ�l�ŏ������F���[�U�����܂܂��A�s�ρj
	CString m_SdeTables[2];	///< SDE�̃e�[�u�����i1���s������1�񏉊���:���[�U�����t�����肷�邱�Ƃ�����j
	INDEX_MAP m_SDEIndexMap[2];	///< SDE�̃t�B�[���h���ƃt�B�[���h�C���f�b�N�X�̊֌W�i1���s������1�񏉊����j
	INDEX_MAP m_PGDBIndexMap[2];	///< PGDB�̃t�B�[���h���ƃt�B�[���h�C���f�b�N�X�̊֌W�i�O�̂��ߊe���b�V�����Ƃɏ������j
	LqEnvTable(LPCTSTR cTag, LPCTSTR cInfName, LPCTSTR cLqName){
		m_Tag = cTag;
		m_OrgTables[0] = m_SdeTables[0] = cInfName;	//�����l�͈ꏏ
		m_OrgTables[1] = m_SdeTables[1] = cLqName;	//�����l�͈ꏏ
	}
};

/**
 * @brief ���[�U���t���e�[�u�����ƃe�[�u���i���̓t�B�[�`���N���X�j�̊֘A�ێ��p�\����
 */
struct LayerInfo{
	CString m_LayerName;	///< �i���[�U���t���j�e�[�u����
	ITablePtr m_ipTable;	///< �e�[�u���E�t�B�[�`���N���X
};

typedef std::map<CString, LayerInfo> TABLE_INFO;	///< ���C�����ƃ��[�U���t���e�[�u�����i���O�o�͗p�j�E�e�[�u���̃}�b�v

/**
 * @brief �e�[�u���E�t�B�[�`���N���X�Ə������Ǘ��p�N���X
 */
class CLayerManage
{
public:
	enum CONNECT_DB {	///< ���[�N�X�y�[�X�̎��ʗp
		DB_ROAD = 0,	///< �ꎞ��~�ȊO�̈��
		DB_STOP=1		///< �ꎞ��~�p
	};
	enum TABLE_TYPE {	///< �Ǘ���̃e�[�u���̕��ގ��ʗp
		FC_TABLE = 0,	///< �i�ꎞ��~�ȊO�́j�t�B�[�`���N���X
		EXT_FC_TABLE = 1,	///< ����A�ꎞ��~�̂�
		OTHER_TABLE = 2		///< ���̑��e�[�u���i�����N��֘A�͕ʊǗ��j
	};

	/**
	 * @brief �R���X�g���N�^
	 */
	CLayerManage(void);

	/**
	 * @brief �f�X�g���N�^
	 */
	~CLayerManage(void);

	/**
	 * @brief ������
	 *
	 * @param cConnectStr		[in]	�T�[�o�ڑ��p������
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool init(const CString cConnectStr[2]);

	/**
	 * @brief SDE�ւ̐ڑ�
	 *
	 * @param lpszConnectStr	[in]	�T�[�o�ڑ�������
	 *
	 * @retval NULL�ȊO	�ڑ��������[�N�X�y�[�X
	 * @retval NULL	���s�A�G���[
	 */
	static IWorkspacePtr connectSDE(LPCTSTR lpszConnectStr);

	/**
	 * @brief �����R�s�[�Ώۂ̃t�B�[���h����SDE�̃t�B�[���h�C���f�b�N�X���擾
	 *
	 * @param ipTable		[in]	�����擾����e�[�u��
	 * @param cIndexMap		[out]	�擾�����t�B�[���h�̏��
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	static bool getFieldInfo(const ITablePtr& ipTable, INDEX_MAP& cIndexMap);

	/**
	 * @brief ���ʗp�e�[�u�����̂���Ή�����e�[�u�����擾
	 *
	 * @param cLayerName		[in]	�擾����e�[�u���̖���
	 *
	 * @retval NULL�ȊO	�Ή�����e�[�u��
	 * @retval NULL	�Y���Ȃ��i�o�O�̂͂��j
	 */
	ITablePtr findTable(LPCTSTR cLayerName) const;

	/**
	 * @brief ���ʗp�e�[�u�����̂���Ή�������ۂ́i���[�U���t���Ƃ��́j�e�[�u�����̂��擾
	 *
	 * @param cLayerName		[in]	�擾����e�[�u���̖���
	 *
	 * @retval NULL�ȊO			�Ή�����e�[�u������
	 * @retval "�s���i�o�O�j"	�Y���Ȃ�
	 */
	LPCTSTR findTableName(LPCTSTR cLayerName) const;

	/**
	 * @brief �ҏW�i���f�j�Ώۂ̃��[�N�X�y�[�X���擾
	 *
	 * @retval �ҏW�Ώۂ̃��[�N�X�y�[�X
	 */
	IWorkspacePtr getEditWorkspace(void) const;

	/**
	 * @brief �e�[�u���̎�ޖ��ɍŏ��̏����Ώۂ̏����擾
	 *
	 * @param cType		[in]	�擾����e�[�u���̎��
	 *
	 * @retval NULL�ȊO	�Y������K�v�ȏ������\���̂ւ̃|�C���^
	 * @retval NULL		�Y���Ȃ��i�o�O�̂͂��j
	 */
	EnvTable* getFitstEnv(TABLE_TYPE cType);

	/**
	 * @brief �e�[�u���̎�ޖ��Ɏ��̏����Ώۂ̏����擾
	 *
	 * @param cType		[in]	�擾����e�[�u���̎��
	 *
	 * @retval NULL�ȊO	�Y������K�v�ȏ������\���̂ւ̃|�C���^
	 * @retval NULL		�Y���Ȃ��i�o�O�̂͂��j
	 */
	EnvTable* getNextEnv(TABLE_TYPE cType);

	/**
	 * @brief ���ʗp�e�[�u�����̂���Ή���������擾
	 *
	 * @param cLayerName	[in]	���ʗp�e�[�u������
	 *
	 * @retval NULL�ȊO	�Y������K�v�ȏ������\���̂ւ̃|�C���^
	 * @retval NULL		�Y���Ȃ��i�o�O�̂͂��j
	 */
	EnvTable* findEnvTable(LPCTSTR cLayerName) const;

	/**
	 * @brief �����N��ōŏ��̏����Ώۂ̏����擾
	 *
	 * @retval NULL�ȊO	�Y������K�v�ȏ������\���̂ւ̃|�C���^
	 * @retval NULL		�Y���Ȃ��i�o�O�̂͂��j
	 */
	LqEnvTable* getFitstLQEnv(void);

	/**
	 * @brief �����N��Ŏ��̏����Ώۂ̏����擾
	 *
	 * @retval NULL�ȊO	�Y������K�v�ȏ������\���̂ւ̃|�C���^
	 * @retval NULL		�Y���Ȃ��i�o�O�̂͂��j
	 */
	LqEnvTable* getNextLQEnv(void);

	/**
	 * @brief �㏈���֐��i�S���j������SDE����ؒf����j
	 */
	void unInit(void);

	/**
	 * @brief �v���O���������ʂ̋�ԎQ�Ƃ��擾
	 *
	 * @retval NULL�ȊO	��ԎQ��
	 * @retval NULL		�o�O�̂͂�
	 */
	ISpatialReferencePtr get_SpRef(void) const
	{
		return m_ipSpRef;
	}

private:
	IWorkspacePtr m_ipWorkspace[2];		///< ���f�惏�[�N�X�y�[�X
	ISpatialReferencePtr m_ipSpRef;	///< ���ʂ̋�ԎQ�Ɓi���Z���Ȃ������͖��g�p�j
	bool m_IsInitial;		///< �������ς݂��̃t���O
	long m_FcIndex;		///< �uFC_TABLE�v���̌��ݏ��������w���C���f�b�N�X
	long m_ExtFcIndex;		///< �uEXT_FC_TABLE�v���̌��ݏ��������w���C���f�b�N�X
	long m_OtherIndex;		///< �uOTHER_TABLE�v���̌��ݏ��������w���C���f�b�N�X
	long m_LqIndex;		///< �����N��Ǘ��p�e�[�u�����̌��ݏ��������w���C���f�b�N�X
	CString m_MeshLayer;	///< ���b�V���|���S���̃t�B�[�`���N���X���i�I�[�v���p�j

	static EnvTable m_FcEnvTable[];	///< �t�B�[�`���N���X����t�B�[���h��񓙂�ێ�����e�[�u��
	static EnvTable m_ExtFcEnvTable[];	///< �t�B�[�`���N���X����t�B�[���h��񓙂�ێ�����e�[�u��(����͈ꎞ��~��p)
	static LqEnvTable m_LqEnvTable[];	///< �e�[�u������t�B�[���h��񓙂�ێ�����e�[�u��(�����N��p)
	static EnvTable m_OtherTable[];	///< �e�[�u������t�B�[���h��񓙂�ێ�����e�[�u��(���̑��e�[�u���n)

	TABLE_INFO m_TableInfo;	///< ���C�����ƃ��[�U���t���e�[�u�����i���O�o�͗p�j�E�e�[�u���̃}�b�v�i�O���[�o����static��COM���g���ƌ㏈�����ʓ|�Ȃ̂ł����ŕʊǗ��j

	/**
	 * @brief SDE�̃e�[�u���E�t�B�[�`���N���X�i�����N��֘A�ȊO�̑S���j�̃I�[�v���Ɗe�폀��
	 *
	 * @param cIsEdit	[in]	�ҏW�Ώۂ��̃t���O�i�o�[�W�����Ή��ł���K�v�����邩�̃`�F�b�N�����邩�j
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool fnOpenSDELayers(bool cIsEdit);

	/**
	 * @brief ���ϐ�����ݒ�𔽉f
	 */
	void fnGetEnv(void);

	/**
	 * @brief SDE�̃t�B�[�`���N���X�̃I�[�v���Ɗe�폀���i1���j
	 *
	 * @param ipFeatureWorkspace	[in]		���[�N�X�y�[�X
	 * @param cEnv					[in/out]	�e�����ێ������\����
	 * @param cIsEdit				[in]		�ҏW�Ώۂ��̃t���O�ifalse�Ȃ�o�[�W�����Ή��������Ȃ��F2��ڂ̎����g�p�j
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool fnOpenSDEFeatureClass(const IFeatureWorkspacePtr& ipFeatureWorkspace, EnvTable& cEnv, bool cIsEdit);

	/**
	 * @brief SDE�̃����N��֘A�e�[�u���S���̃I�[�v���Ɗe�폀��
	 *
	 * @param ipFeatureWorkspace	[in]		���[�N�X�y�[�X
	 * @param cEnv					[in/out]	�e�����ێ������\����
	 * @param cIsEdit				[in]		�ҏW�Ώۂ��̃t���O�ifalse�Ȃ�o�[�W�����Ή��������Ȃ��F2��ڂ̎����g�p�j
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool fnOpenSDETables(const IFeatureWorkspacePtr& ipFeatureWorkspace, LqEnvTable& cEnv, bool cIsEdit);

	/**
	 * @brief SDE�̃����N��֘A�e�[�u���̃I�[�v���Ɗe�폀���i1���j
	 *
	 * @param ipFeatureWorkspace	[in]	���[�N�X�y�[�X
	 * @param cTableName			[in]	���ʗp�e�[�u������
	 * @param cIndexMap				[out]	�t�B�[���h�C���f�b�N�X�̏��
	 * @param cIsEdit				[in]	�ҏW�Ώۂ��̃t���O�ifalse�Ȃ�o�[�W�����Ή��������Ȃ��F2��ڂ̎����g�p�j
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool fnOpenSDETable(const IFeatureWorkspacePtr& ipFeatureWorkspace, LPCTSTR cTableName, INDEX_MAP& cIndexMap, bool cIsEdit);

	/**
	 * @brief ���b�V���|���S���I�[�v��
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool fnOpenMeshPoly(void);

	/**
	 * @brief ���ʗp�e�[�u�����̂ƃe�[�u�����֘A�Â��ĊǗ��i�o�^�j
	 *
	 * @param ipTable		[in]	�Ή�����e�[�u��
	 * @param cTableName	[in]	���ʗp�e�[�u������
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool fnRegistTable(const ITablePtr& ipTable, LPCTSTR cTableName);
};
