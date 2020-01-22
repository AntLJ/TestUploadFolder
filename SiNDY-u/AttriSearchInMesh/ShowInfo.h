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

// ShowInfo.h: ShowInfo �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHOWINFO_H__32B86BDF_61B4_49AF_B479_5B9161DFCD09__INCLUDED_)
#define AFX_SHOWINFO_H__32B86BDF_61B4_49AF_B479_5B9161DFCD09__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <algorithm>
#include "coord_converter.h"
#include "GlobalFunc.h"

/**
 * @enum	���O�o�̓��[�h
 */
enum emLogMode
{
	kNormal = 1,	// �W�����O���[�h
	kCount	= 2		// �I�u�W�F�N�g���J�E���g���[�h
};

/**
 * @class	CShowInfo
 */
class CShowInfo  
{
public:
	CShowInfo() : m_bIsAddMode(false), m_emLogMode(kNormal), 
				  m_bIsNoDupMode(false), m_strWhereClause(_T("OBJECT IS NOT NULL"))
	{}
	~CShowInfo(){}
	
	/**
	 * @brief	�g�p�@
	 */
	static void printUsage();

	/**
	 * @brief	������
	 * @param	argc	[in]	�R�}���h���C�������̐�
	 * @param	argv	[in]	�R�}���h���C������
	 * @return	bool
	 */
	bool init( int argc, char* argv[] );

	/**
	 * @brief	���s
	 * @return	bool
	 */
	bool execute();

private:

	/**
	 * @brief	���b�V�����X�g�ǂݍ���
	 * @param	lpcszFile	[in]	���b�V�����X�g
	 * @return	bool
	 */
	bool loadMeshList( LPCTSTR lpcszFile );

	/**
	 * @brief	�ݒ�t�@�C���ǂݍ���
	 * @param	lpcszFile	[in]	�ݒ�t�@�C��
	 * @return	bool
	 */
	bool loadInitFile( LPCTSTR lpcszFile );

	/**
	 * @brief	���ϐ��擾
	 * @return	bool
	 */
	bool getEnv();

	/**
	 * @brief	�c�a����K�v���擾
	 * @param	lMeshcode	[in]	���b�V���R�[�h
	 * @return	bool
	 */
	bool setFromDB( long lMeshcode );

	/**
	 * @brief	�W�����O�o��
	 * @param	lMeshcode	[in]	���b�V���R�[�h
	 * @param	ipMeshGeo	[in]	���b�V���W�I���g��
	 * @return	bool
	 */
	bool execNormalLog( long lMeshcode, IGeometry* ipMeshGeo );

	/**
	 * @brief	�I�u�W�F�N�g�J�E���g
	 * @param	lMeshcode	[in]	���b�V���R�[�h
	 * @param	ipMeshGeo	[in]	���b�V���W�I���g��
	 * @return	bool
	 */
	bool execObjectCount( long lMeshcode, IGeometry* ipMeshGeo );

	/**	
	 * @brief	���b�V�����E��I�u�W�F�N�g�h�c�ꗗ�쐬
	 * @note	�W�����O���[�h���Ɏg��
	 * @param	lMeshcode		[in]		���b�V���R�[�h
	 * @param	ipBaseMeshGeo	[in]		��ƂȂ郁�b�V���W�I���g��
	 * @param	setID			[in,out]	�I�u�W�F�N�g�h�c�ꗗ
	 * @return	bool
	 */
	bool makeDupIDSet( long lMeshcode, IGeometry* ipBaseMeshGeo, std::set<long>& setID ); 

	/**
	 * @brief	���b�V�����E��I�u�W�F�N�g�`�F�b�N
	 * @note	�I�u�W�F�N�g�J�E���g���Ɏg��
	 * @param	lMeshcode		[in]	���b�V���R�[�h
	 * @param	ipBaseMeshGeo	[in]	��ƂȂ郁�b�V���W�I���g��
	 * @return	���O����ׂ��I�u�W�F�N�g��(�����������b�V����D�悷��)
	 */
	int checkOnMeshObject( LONG lMeshcode, IGeometry* ipBaseMeshGeo );

	/**
	 * @brief	�w�胁�b�V���t�B�[�`���擾
	 * @param	lMeshcode	[in]	���b�V���R�[�h
	 * @param	ipFeature	[out]	���b�V���t�B�[�`��
	 * @return	bool
	 */
	bool getMeshFeature( long lMeshcode, IFeature** ipFeature );

	/**
	 * @brief	�w�胁�b�V���R�[�h�����b�V�����X�g�ɂ��邩�`�F�b�N
	 * @param	lMeshcode	[in]	���b�V���R�[�h
	 * @return	bool
	 */
	bool isExistMesh( long lMeshcode )
	{
		return std::find(m_vecMesh.begin(), m_vecMesh.end(), lMeshcode) != m_vecMesh.end();
	}

	/**
	 * @brief	���O�w�b�_�[�o��
	 * @return	bool
	 */
	bool printHeader();
		
	/**
	 * @brief	�t�B�[�`�����\��
	 * @param	ipFeature	[in]	�t�B�[�`��
	 * @param	lMeshcode	[in]	���b�V���R�[�h
	 * @return	bool
	 */
	bool printData( IFeature* ipFeature, long lMeshcode );

	/**
	 * @brief	�\���|�C���g�擾
	 * @param	ipFeature	[in]	�\���������t�B�[�`��
	 * @param	point		[out]	�\���|�C���g
	 * @return	bool
	 */
	bool getDispPoint( IFeature* ipFeature, WKSPoint& point );

//////////////////////////////////////////
//	�����o�ϐ�
//////////////////////////////////////////
private:

	IWorkspacePtr	m_ipWorkspace;				//!< ���[�N�X�y�[�X

	emLogMode	m_emLogMode;					//!< ���O���[�h
	bool		m_bIsNoDupMode;					//!< �d���Ȃ����[�h

	std::ofstream	m_fout;						//!< ���O�o�̓X�g���[��
	bool			m_bIsAddMode;				//!< �ǋL�Ńt�@�C���I�[�v�����Ă��邩�H

	std::vector<long>		m_vecMesh;			//!< ���b�V�����X�g

	CString			m_strDBProp;				//!< DB�ڑ��v���p�e�B
	CString			m_strTargetFCName;			//!< �^�[�Q�b�g�t�B�[�`���N���X��
	CString			m_strMeshFCName;			//!< ���b�V���t�B�[�`���N���X��
	IFeatureClassPtr	m_ipTargetFC;			//!< �^�[�Q�b�g�t�B�[�`���N���X
	esriGeometryType	m_esriTargetGeoType;	//!< �^�[�Q�b�g�t�B�[�`���N���X�̌`��^�C�v
	IFeatureClassPtr	m_ipMeshFC;				//!< ���b�V���t�B�[�`���N���X

	CString				m_strWhereClause;		//!< Where��

	crd_cnv				m_cCrdCnv;				//!< ���W�ϊ��N���X

	std::vector<CString>		m_vecField;		//!< �t�B�[���h�x�N�^�[
	typedef std::map<CString, long>	FIELDMAP;
	FIELDMAP	m_mapFieldID;					//!< �t�B�[���h�h�c�}�b�v(�L�[���t�B�[���h���A�l���t�B�[���h�h�c)

	std::map<CString, DOMAINMAP>	m_mapDM;	//!< �L�[���t�B�[���h���ŁA�l���h���C���}�b�v
};

/**
 * @brief	Variant����string�^�ɕϊ��֐�
 * @param	vaTarget	[in]	�ϊ�������Variant�^
 * @return	������
 */
const CString VariantToString( CComVariant vaTarget );

#endif // !defined(AFX_SHOWINFO_H__32B86BDF_61B4_49AF_B479_5B9161DFCD09__INCLUDED_)
