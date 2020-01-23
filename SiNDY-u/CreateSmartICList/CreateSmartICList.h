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
#include "ReadConfig.h"
#include "DBManager.h"
#include <atl2/seal.h>

typedef std::set<long>					lIDSET;
typedef lIDSET::const_iterator			ConstItr;

typedef std::set<CAdapt<CComVariant>>	vaSET;
typedef vaSET::const_iterator			ConstVaItr;

typedef std::set<std::pair<long, long>>	llSET;
typedef	llSET::const_iterator			ConstItr2;

/**
 *	@brief	���HNW�̃t�B�[�`���N���X�Q
 */
struct NwFC
{
	IFeatureClassPtr ipRoadLinkFC;	///< Road_Link �t�B�[�`���N���X
	IFeatureClassPtr ipRoadNodeFC;	///< Road_Node �t�B�[�`���N���X
	IFeatureClassPtr ipBaseMeshFC; ///< BaseMesh �t�B�[�`���N���X
};

/**
 *	@brief	�t�B�[���h�C���f�b�N�X
 */
struct FieldIndex
{
	long lFromID;	///< road_link��From_Node_ID
	long lToID;		///< road_link��To_Node_ID

	long lLinkOID;	///< Link��OID

	long lMeshcode;	///< ���b�V���R�[�h
};

class CSmartICList
{
public:
	CSmartICList(void);
	~CSmartICList(void);

	/*
	 *	@brief	�R�s�[�R���X�g���N�^
	 */
	CSmartICList(CString strConfigPath);

//------------------�����ݒ肱������--------------------------------

	/**
	 *	@brief	�o�̓X�g���[���̃I�[�v���ƃw�b�_�̏o��
	 *	@param	strOutPath	[in]	�o�̓p�X
	 */
	void openfs(CString strOutPath);

	/**
	 *	@brief	�����N���X�g�o�͗p�w�b�_�̏o��
	 */
	void setlinkheader();

	/**
	 *	@brief	�m�[�h���X�g�o�͗p�w�b�_�̏o��
	 */
	void setnodeheader();

	/**
	 *	@brief	�t�B�[�`���N���X�̎擾
	 *	@param	ipNwWorkspace	[in]	NW���[�N�X�y�[�X
	 */
	void getfc(IWorkspacePtr ipNwWorkspace);

	/**
	 *	@brief �t�B�[���h�C���f�b�N�X�̎擾
	 */
	void gerfiledindex();

//------------------�����ݒ肱���܂�--------------------------------

	/**
	 *	@brief	�����̎��s��
	 */
	void Execute();

	/**
	 *	@brief	�X�}�[�g�����N���X�g�̍쐬
	 */
	void CSmartICList::CreateLinkList();

	/**
	 *	@brief	�����m�[�h���X�g�̍쐬
	 */
	void CreateFloatNodeList();

	/**
	 *	@brief	SmartIC�����N�Ԉ����㕂���m�[�h�ɂȂ邩�ǂ����̃`�F�b�N
	 *	@param	vaTouchLinkIDSet	[in]	�m�[�h�ɐڐG���Ă��郊���N��ID�Z�b�g
	 *	@retval	true	�����m�[�h�ɂȂ�
	 *	@retval false	�����m�[�h�ɂȂ�Ȃ�
	 */
	bool IsFloatNode(const vaSET&  vaTouchLinkIDSet);

	/**
	 *	@brief	�`��̑����郁�b�V���R�[�h�̎擾
	 *	@param	ipGeom	[in]	�`��
	 *	@param	eStType
	 *	@retval	���b�V���R�[�h���i�[����set
	 */
	vaSET GetMeshCode(IGeometryPtr ipGeom, esriSpatialRelEnum eStType);

	/**
	 *	@brief	�o�͗p���X�g�̍쐬
	 *	@param	lOID			[in]	ObjectID
	 *	@param	vaMeshCodeSet	[in]	���b�V���R�[�h�i�[�Z�b�g
	 *	@param	llOutSet		[out]	�o�͗p���X�g
	 */
	void CreateList(long lOID, const vaSET& vaMeshCodeSet, llSET& llOutSet);

	/**
	 *	@brief	���X�g���o�͂���
	 *	@param	llOutSet	[in]	�o�͗p���X�g
	 *	@param	ofs			[in]	�o�̓X�g���[��
	 */
	void Output(const llSET& llOutSet, std::ofstream& ofs);

private:
	/**	
	 *	@breif	�p�����[�^�t�@�C��
	 */
	Parameters m_eParams;

	/** 
	 *	@brief	DB����N���X
	 */
	CDBManager m_cDBManager;

	/**
	 *	@brief	�X�}�[�gIC�����N���o�͗p�X�g���[��
	 */
	std::ofstream	m_linkofs;

	/**
	 *	@brief	�X�}�[�gIC�����N�Ԉ�����ɕ����m�[�h�ɂȂ郊�X�g�o�͗p�X�g���[��
	 */
	std::ofstream	m_nodeofs;

	/**
	 *	@brief	�{�N���X�Ŏg�p����t�B�[�`���N���X�Q
	 */
	NwFC	m_NwFC;

	/**
	 *	@brief	�{�N���X�Ŏg�p����t�B�[���h�C���f�b�N�X�Q
	 */
	FieldIndex	m_FieldIndex;

	/**
	 *	@brief	�X�}�[�gIC�����NID
	 */
	lIDSET	m_SmartICLinkSet;

	/**
	 *	@brief	�����m�[�h���ƂȂ�m�[�hID
	 */
	lIDSET	m_SmartICNodeSet;
};
