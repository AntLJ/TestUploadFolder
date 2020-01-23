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

#include "FieldMap.h"

using namespace std;
using namespace sindy;

enum ERRTYPE {
	INFO     = 0,
	NOTICE   = 1,
	WARNING  = 2,
	ERR      = 3,
	CRITICAL = 4
};

/**
 * @brief   �f�[�^�X�V���
 */
enum UPDATETYPE {
	SAME          = 0,   //!< �ύX�Ȃ�
	NEW           = 1,   //!< �V�K
	DEL           = 2,   //!< �폜
	SHAPE         = 3,   //!< �`��ύX
	ATTR          = 4,   //!< �����ύX
	SHAPE_ATTR    = 5    //!< �`��{�����ύX
};

/**
 * @brief   �X�V�\��f�[�^�̃X�e�[�^�X���
 * @note    �r�b�g�t���O
 */
enum DATASTATUS {
	NONE          = 0, //!< �X�V����
	UPDATE        = 1, //!< �X�V�L��
	CONFLICT      = 2, //!< �R���t���N�g
	REL_LINK      = 4, //!< �����N�Ɉ��������
	REL_NODE      = 8, //!< �m�[�h�Ɉ��������
	REL_AGOPS     = 16, //!< �����N�������ȂǂɈ��������
	REL_INF       = 32, //!< �����N��Ɉ��������
	REL_EDGENODE  = 64, //!< ���b�V�����E�m�[�h�Ɉ��������
	REL_ZERONODE  = 128,  //!< FromTo�m�[�hID��0�̃����N���������
	REL_VEHICLETYPE = 256, //!< �Ԏ�Ɉ�������� 
	OUTMESH       = 512,   //!< �w�胁�b�V���O
};

/**
 * @brief   �擾DB�̃^�C�v
 */
enum DBTYPE {
	BEFORE = 0,   //!< �ҏW�OPGDB
	AFTER  = 1,   //!< �ҏW��PGDB
	SDE    = 2    //!< SDE
};

/**
 * @brief   �e�[�u���̃f�[�^�^�C�v
 * @note    �r�b�g�t���O
 */
enum DATATYPE {
	FEATURECLASS   = 1,    //!< �t�B�[�`���N���X
	TABLE          = 2,    //!< �e�[�u��
	REL_LINKTABLE  = 4,    //!< �����N�R�t���e�[�u��
	INF            = 8,    //!< INF�e�[�u��
	LQ             = 16,   //!< LQ�e�[�u��
	REL_OTHERTABLE = 32    //!< ���̑��R�t���e�[�u��
};


/**
 * @brief   LQ�f�[�^
 */
struct LqData
{
	long lInfID;      //!< INFID
	long lMeshCode;   //!< ���b�V���R�[�h
	long lLinkID;     //!< �����N
	long lLinkDir;    //!< �����N����
	long lSeq;        //!< �V�[�P���X
	long lLast;       //!< ���X�g�t���O

	bool operator==( const LqData& cLqData ) const 
	{
		return (
			this->lInfID == cLqData.lInfID &&
			this->lMeshCode == cLqData.lMeshCode &&
			this->lLinkID== cLqData.lLinkID &&
			this->lLinkDir == cLqData.lLinkDir &&
			this->lSeq == cLqData.lSeq &&
			this->lLast == cLqData.lLast
			);
	}
	bool operator<( const LqData& cLqData ) const 
	{
		return 
			(
			this->lInfID != cLqData.lInfID ? this->lInfID < cLqData.lInfID : 
			this->lSeq != cLqData.lSeq ? this->lSeq < cLqData.lSeq : 
			this->lMeshCode != cLqData.lMeshCode ? this->lMeshCode < cLqData.lMeshCode :
			this->lLinkID != cLqData.lLinkID ? this->lLinkID < cLqData.lLinkID :
			this->lLinkDir != cLqData.lLinkDir ? this->lLinkDir < cLqData.lLinkDir : this->lLast < cLqData.lLast
			);
	}
};

struct TableType
{
	CString strTableName;
	DATATYPE dataType;

};

/**
 * @brief   �e�[�u���f�[�^
 */
struct TableData 
{
	DATATYPE dataType;                     //!< �e�[�u���̃f�[�^�^�C�v
	CString strTableName;                  //!< �e�[�u����
	ITablePtr ipTable;                     //!< �e�[�u��
	CFieldMap cFieldMap;                   //!< �t�B�[���h�}�b�v
	set<long> exceptFieldIndexSet;         //!< �R���t���N�g�̏��O�t�B�[���h�C���f�b�N�X��set(bug 10363)

	map<long,CAdapt<_IRowPtr>> mapRow;     //!< �f�[�^�Q
	map<long,long> mapOrgID2PGDBID;        //!< SDE��OID��PGDB��OID�̃}�b�v
	map<long,long> mapPGDBID2OrgID;        //!< PGDB��OID��SDE��OID�̃}�b�v

	map<long,bool> mapInMeshData;          //!< ���b�V�����f�[�^���ǂ���

	// [�����N�������A�X�΁A����ʍs�A�ʍs�K���A�ȈՌX�΁A���/�ʋ֎Ԏ�ʋK��]�p
	map<long,set<long>> mapLink2AGOPS;     //!< �����N�ɕR�t��[�����N�������A�X�΁A����ʍs�A�ʍs�K���A�ȈՌX�΁A���/�ʋ֎Ԏ�ʋK��]�̃}�b�v
	map<long,long> mapAGOPS2Link;          //!< [�����N�������A�X�΁A����ʍs�A�ʍs�K���A�ȈՌX�΁A���/�ʋ֎Ԏ�ʋK��]�ɕR�t�������N�̃}�b�v
	
	// [�Ԏ�^�C�v�p] (bug 10363)
	map<long,set<long>> mapVehicleReg2Type; //!< ���/�ʋ֎Ԏ�ʋK���ɕR�t���Ԏ�^�C�v�̃}�b�v
	map<long,long> mapType2VehicleReg;      //!< �Ԏ�^�C�v�ɕR�t�����/�ʋ֎Ԏ�ʋK���̃}�b�v

	// �����N��p
	map<long,set<LqData>> mapInfData;      //!< �����N��̃f�[�^�Q
	map<long,set<long>> mapLink2Inf;       //!< �����N�ɕR�t�������N��̃}�b�v
	map<long,set<long>> mapInf2Link;       //!< �����N��ɕR�t�������N�̃}�b�v
};

// ���O�o�͗p
struct UpdateData 
{
	long lOID;
	UPDATETYPE updateType;
	DATASTATUS dataStatus;
};

class CNWData
{
public:
	CNWData(){}
	~CNWData(){}
public:
	/**
	 * @brief   �f�[�^������
	 * @param   mapTableMap    [in]  �e�[�u���}�b�v
	 * @param   ipFWorkspace   [in]  ���[�N�X�y�[�X
	 * @param   lMeshCode      [in]  ���b�V���R�[�h
	 * @param   ipMeshGeom     [in]  ���b�V���`��
	 * @param   exceptFieldMap [in] �C���|�[�g���O�t�B�[���hmap(bug 10363)
	 * @retval  true   ����������
	 * @retval  false  ���������s
	 */
	bool Init( const map<CString,DATATYPE>& mapTableMap, const IFeatureWorkspacePtr& ipFWorkspace, long lMeshCode, const IGeometryPtr& ipMeshGeom, const map<CString,set<CString>>& exceptFieldMap );
	/**
	 * @brief   SDE������
	 * @param   mapTableMap    [in]  �e�[�u���}�b�v
	 * @param   ipFWorkspace   [in]  ���[�N�X�y�[�X
	 * @param   exceptFieldMap [in] �C���|�[�g���O�t�B�[���hmap(bug 10363)
	 * @retval  true   ����������
	 * @retval  false  ���������s
	 */
	bool Init_SDE( const map<CString,DATATYPE>& mapTableMap, const IFeatureWorkspacePtr& ipFWorkspace, const map<CString,set<CString>>& exceptFieldMap );
	
	// ��ԎQ�Ǝ擾�p
	ISpatialReferencePtr GetSpatialReference() const { return m_ipSpRef; }

	// �t�B�[�`�����[�N�X�y�[�X�擾�p
	IFeatureWorkspacePtr GetFWorkspace() const { return m_ipFWorkspace; }
	// ���[�N�X�y�[�X�擾
	IWorkspacePtr GetWorkspace() const { return IWorkspacePtr(m_ipFWorkspace); }
	// ���b�V���`��擾
	IGeometryPtr GetMeshGeometry() const { return m_ipMeshGeom; }
	// ���b�V���R�[�h�擾
	long GetMeshCode() const { return m_lMeshCode; }
	// �e�[�u���擾
	ITablePtr GetTable( const CString& strTableName ) const;
	// �t�B�[���h�}�b�v�擾
	CFieldMap GetFieldMap( const CString& strTableName ) const;
	// �w��OID�̃��R�[�h�擾
	_IRowPtr GetRow( const CString& strTableName, long lOID ) const;
	// PGDB�̎w��OrgOID�̃��R�[�h�擾
	long GetOrgOIDFromPGDBOID( const CString& strTableName, long lPGDBOID ) const;
	// PGDB�̎w��PGDBOID�̃��R�[�h�擾
	long GetPGDBOIDFromOrgOID( const CString& strTableName, long lOrgOID ) const;

private:
	/**
	 * @brief   �e�[�u���̃f�[�^���擾
	 * @param   strTableName   [in]  �e�[�u���̃e�[�u����
	 * @param   dataType       [in]  �e�[�u���̃f�[�^�^�C�v
	 * @return  true  ����
	 *          false ���s
	 */
	bool setTable( const CString& strTableName, const DATATYPE dataType );

	/**
	 * @brief   �t�B�[���h�C���f�b�N�X���擾
	 * @param   strTableName   [in]  �e�[�u����
	 * @return  true  ����
	 *          false ���s
	 */
	bool setFieldMap( const CString& strTableName, const map<CString,set<CString>>& exceptFieldMap );

	/**
	 * @brief   �e�[�u���̃f�[�^���擾
	 * @param   strTableName   [in]  �e�[�u���̃e�[�u����
	 * @param   ipQFilter      [in]  �N�G���t�B���^
	 * @param   bInMesh        [in]  �w�胁�b�V�����̃f�[�^���ǂ���
	 * @return  true  ����
	 *          false ���s
	 */
	bool setTableData( const CString& strTableName, const IQueryFilterPtr& ipQFilter, bool bInMesh );

	/**
	 * @brief   ���b�V���`��ɕR�t���t�B�[�`���N���X�̃f�[�^���擾
	 * @param   strTableName   [in]  �t�B�[�`���N���X�̃e�[�u����
	 * @param   ipGeom         [in]  ���b�V���`��
	 * @return  true  ����
	 *          false ���s
	 */
	bool setFeatureClassData( const CString& strTableName, const IGeometryPtr& ipGeom );
	
	/**
	 * @brief   �����N�ƃm�[�h�̊֘A�����Z�b�g
	 * @return  true  ����
	 *          false ���s
	 */
	bool setLinkNodeRelation();

	/**
	 * @brief   �����N��[�����N�������A�X�΁A����ʍs�A�ʍs�K���A�ȈՌX�΁A���/�ʋ֎Ԏ�ʋK��]�Ƃ̊֌W���Z�b�g
	 * @param   strTableName   [in]  [�����N�������A�X�΁A����ʍs�A�ʍs�K���A�ȈՌX�΁A���/�ʋ֎Ԏ�ʋK��]�̃e�[�u����
	 * @return  true  ����
	 *          false ���s
	 */
	bool setLinkAGOPSRelation( const CString& strTableName );

	/**
	 * @brief   ���/�ʋ֎Ԏ�ʋK���ƎԎ�^�C�v�̊֌W���Z�b�g
	 * @param   strTableName   [in]  �Ԏ�^�C�v�̃e�[�u����
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool setVehicleRegTypeRelation( const CString& strTableName );

	/**
	 * @brief   �����N����Z�b�g
	 * @param   strTableName   [in]  INF��LQ�̃e�[�u����
	 * @return  true  ����
	 *          false ���s
	 */
	bool setInfLq( const CString& strTableName, long lMeshCode );

	/**
	 * @brief   ���b�V�����E�m�[�h���Z�b�g
	 * @return  true  ����
	 *          false ���s
	 */
	bool setMeshNode();

public:
	long m_lMeshCode;                       //!< ���b�V���R�[�h
	IGeometryPtr m_ipMeshGeom;              //!< ���b�V���`��
	IFeatureWorkspacePtr m_ipFWorkspace;    //!< �t�B�[�`�����[�N�X�y�[�X

	ISpatialReferencePtr m_ipSpRef;         //!< ��ԎQ��

	map<CString,TableData> m_mapTableData;  //!< �e�[�u�����̃f�[�^
	map<long,pair<long,long>> m_mapLink2FromToNode; //!< �����N�ɕR�t��FromTo�m�[�h�̃}�b�v
	map<long,set<long>> m_mapNode2Link;     //!< �m�[�h�ɕR�t�������N�Q�̃}�b�v
	set<long> m_setMeshNode;                //!< ���b�V�����E�m�[�h
};
