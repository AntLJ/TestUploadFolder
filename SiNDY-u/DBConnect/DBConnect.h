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

// DBConnect.h: CDBConnect �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DBCONNECT_H__80670A8C_BE1D_4B77_AF7B_E1685746C735__INCLUDED_)
#define AFX_DBCONNECT_H__80670A8C_BE1D_4B77_AF7B_E1685746C735__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
 * @file	DBConnect.h
 * @brief	DB�ڑ��N���X
 */

#include <atlstr.h>

#include <iostream>
#include <string>
#include <map>

using namespace std;

typedef std::map< LONG, std::string > DOMAINMAP;	/// �h���C���}�b�v

/**
 * @class	CDBConnect
 * @brief	SDE�ڑ����N���X
 * @note	�p����N���X�ŁA�ڑ��֐����������Ȃ��Ǝg�p�ł��Ȃ�
 */
class CDBConnect
{
public:

	/**
	 * @breif	�f�t�H���g�R���X�g���N�^
	 */
	CDBConnect() : m_ipWorkspace(NULL){};

	/**
	 * @brief	�f�X�g���N�^
	 */
	virtual ~CDBConnect(){ m_ipWorkspace = NULL; };

	/**
	 * @brief	�ڑ�
	 * @note	�p����Ŏ�������(SDE or PGDB or Shape)
	 * @param	lpszDBProp		[in]	DB�ւ̐ڑ��v���p�e�B
	 * @param	lpszDBPropEx	[in]	DB�ւ̐ڑ��v���p�e�B�g��(�K�v�ȂƂ��g�p)
	 * @return	bool
	 */
	virtual bool Connect( LPCTSTR lpszDBProp, LPCTSTR lpszDBPropEx = NULL ) = 0;

	/**
	 * @brief	���[�N�X�y�[�X�擾
	 * @return	���[�N�X�y�[�X
	 */
	IWorkspacePtr GetWorkspace() const { return m_ipWorkspace; }

	/**
	 * @brief	�t�B�[�`���N���X�擾
	 * @param	lpszName	[in]	�t�B�[�`���N���X��
	 * @return	�t�B�[�`���N���X
	 */
	IFeatureClassPtr GetFeatureClassByName( LPCTSTR lpszName );
	
	/**
	 * @brief	�Q�ƃe�[�u���擾
	 * @param	lpszName	[in]	�e�[�u����
	 * @return	�e�[�u��
	 */
	ITablePtr GetTableByName( LPCTSTR lpszName );

	/**
	 * @brief	�h���C���l�E�h���C�����}�b�v�쐬
	 * @param	lpszTableName	[in]	�e�[�u����
	 * @param	lpszFieldName	[in]	�h���C�������t�B�[���h��
	 * @return	�h���C��������΃h���C���}�b�v��Ԃ��Ȃ��ƃT�C�Y0�̃h���C���}�b�v��Ԃ�
	 */
	DOMAINMAP GetDomainMap( LPCTSTR lpszTableName, LPCTSTR lpszFieldName );

protected:

	IWorkspacePtr		m_ipWorkspace;		/// ���[�N�X�y�[�X

};

/**
 * @class	CSDEConnect
 * @brief	SDE�ڑ��Ǘ��N���X
 */
class CSDEConnect : public CDBConnect
{
public:
	CSDEConnect(){};
	~CSDEConnect(){};

	/**
	 * @brief	�ڑ�
	 * @note	�p����Ŏ�������(SDE)
	 * @param	lpszDBProp		[in]	�v���p�e�B���(Ex. ronly/ronly/SDE.Current05A/5151/ruby)
	 * @param	lpszDBPropEx	[in]	�g�p���Ȃ�
	 * @return	bool
	 */
	bool Connect( LPCTSTR lpszDBProp, LPCTSTR lpszDBPropEx = NULL );

private:
	
	CComVariant m_vaUser, m_vaPass, m_vaVersion, m_vaInstance, m_vaServer;	/// �ڑ����

};

/**
 * @class	CPGDBConnect
 * @brief	PGDB�ڑ��Ǘ��N���X
 */
class CPGDBConnect : public CDBConnect
{
public:
	CPGDBConnect(){};
	~CPGDBConnect(){};

	/**
	 * @brief	�ڑ�
	 * @note	�p����Ŏ�������(PGDB)
	 * @param	lpszDBProp		[in]	�t�@�C����
	 * @param	lpszDBPropEx	[in]	�g�p���Ȃ�
	 * @return	bool
	 */
	bool Connect( LPCTSTR lpszDBProp, LPCTSTR lpszDBPropEx = NULL );

private:

	CString		m_strFile;		/// �t�@�C����
};

/**
 * @brief	CShapeConnect
 * @brief	�V�F�[�v�t�@�C���ڑ��Ǘ��N���X
 */
class CShapeConnect : public CDBConnect
{
public:
	CShapeConnect(){};
	~CShapeConnect(){};

	/**
	 * @brief	�ڑ�
	 * @param	�p����Ŏ�������(Shape)
	 * @param	lpszDBProp		[in]	���[�N�f�B���N�g��
	 * @param	lpszDBPropEx	[in]	�g�p���Ȃ�
	 * @return	bool
	 */
	bool Connect( LPCTSTR lpszDBProp, LPCTSTR lpszDBPropEx = NULL );

private:

	CString		m_strDir;		/// ���[�N�f�B���N�g��
};

/**
 * @brief	PGDB���[�h?
 * @note	���ϐ� PGDB_MODE ���ݒ肳��Ă�����n�j
 */
bool isPGDBMode();

/**
 * @brief	��Ԍ��� (+ ��������)
 * @param	ipGeo			[in]	�Ώۂ̃W�I���g��
 * @param	ipFeatureClass	[in]	�������̃t�B�[�`���N���X
 * @param	ppFeatureCursor	[out]	�������ʂ̃J�[�\��
 * @param	spatialRel		[in]	��������
 * @param	lpszWhereClause	[in]	��������������Ȃ�w�肷��(�f�t�H���g��NULL)
 * @param	lpszSubFields	[in]	�K�v�ȃt�B�[���h(�J���}��؂�)
 * @return	bool
 */
bool SelectByShape( IGeometryPtr ipGeo,
							 IFeatureClassPtr ipFeatureClass,
							 IFeatureCursor** ppFeatureCursor,
				 			 esriSpatialRelEnum spatialRel,
							 LPCTSTR lpszWhereClause = NULL,
							 LPCTSTR lpszSubFields = NULL );

/**
 * @brief	��Ԍ��� (+ ��������)
 * @note	�����̂ݎ擾
 * @param	ipGeo			[in]	�Ώۂ̃W�I���g��
 * @param	ipFeatureClass	[in]	�������̃t�B�[�`���N���X
 * @param	spatialRel		[in]	��������
 * @param	lpszWhereClause	[in]	��������������Ȃ�w�肷��(�f�t�H���g��NULL)
 * @return	�����Ɉ�v��������
 */
LONG SelectByShapeOnlyCount( IGeometryPtr ipGeo,
							 IFeatureClassPtr ipFeatureClass,
				 			 esriSpatialRelEnum spatialRel,
							 LPCTSTR lpszWhereClause = NULL );

/**
 * @brief	��Ԍ��� (+ ��������)
 * @note	�������擾����̂ŁA��������Ȃ��Ȃ�SelectByShape�g��
 * @param	ipGeo			[in]	�Ώۂ̃W�I���g��
 * @param	ipFeatureClass	[in]	�������̃t�B�[�`���N���X
 * @param	ppFeatureCursor	[out]	�������ʂ̃J�[�\��
 * @param	spatialRel		[in]	��������
 * @param	lpszWhereClause	[in]	��������������Ȃ�w�肷��(�f�t�H���g��NULL)
 * @param	lpszSubFields	[in]	�K�v�ȃt�B�[���h(�J���}��؂�)
 * @return	�����Ɉ�v��������
 */
LONG SelectByShapeWithCount( IGeometryPtr ipGeo,
							 IFeatureClassPtr ipFeatureClass,
							 IFeatureCursor** ppFeatureCursor,
				 			 esriSpatialRelEnum spatialRel,
							 LPCTSTR lpszWhereClause = NULL,
							 LPCTSTR lpszSubFields = NULL );

/**
 * @brief	��������
 * @param	ipTable			[in]	�������̃e�[�u��
 * @param	ppCursor		[out]	�������ʂ̃J�[�\��
 * @param	lpszWhereClause	[in]	Where��
 * @param	lpszSubFields	[in]	�K�v�ȃt�B�[���h(�J���}��؂�)
 * @return	bool
 */
bool SelectByAttr( ITablePtr ipTable, _ICursor** ppCursor, LPCTSTR lpszWhereClause, LPCTSTR lpszSubFields = NULL );

/**
 * @brief	��������
 * @note	�����̂ݎ擾
 * @param	ipTable			[in]	�������̃e�[�u��
 * @param	lpszWhereClause	[in]	Where��
 * @return	�����Ɉ�v��������
 */
LONG SelectByAttrOnlyCount( ITablePtr ipTable, LPCTSTR lpszWhereClause );

/**
 * @brief	��������
 * @note	�������擾����̂ŁA��������Ȃ��Ȃ�SelectByShape�g��
 * @param	ipTable			[in]	�������̃e�[�u��
 * @param	ppCursor		[out]	�������ʂ̃J�[�\��
 * @param	lpszWhereClause	[in]	Where��
 * @param	lpszSubFields	[in]	�K�v�ȃt�B�[���h(�J���}��؂�)
 * @return	�����Ɉ�v��������
 */
LONG SelectByAttrWithCount( ITablePtr ipTable, _ICursor** ppCursor, LPCTSTR lpszWhereClause, LPCTSTR lpszSubFields = NULL );



#endif // !defined(AFX_DBCONNECT_H__80670A8C_BE1D_4B77_AF7B_E1685746C735__INCLUDED_)
