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

/*! \file RowsControlRel.h
	\brief CRowsControlRel �N���X�̃C���^�[�t�F�[�X
	\author INCREMENTP CORP. R&D�Z�p�Z���^�[��S�J���O���[�v �Ð�M�G
	\date 2002-03-05
*/
#if !defined(AFX_ROWSCONTROLREL_H__343F14A0_BCF0_4BA7_ACFA_B0E09E3CAE6D__INCLUDED_)
#define AFX_ROWSCONTROLREL_H__343F14A0_BCF0_4BA7_ACFA_B0E09E3CAE6D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ControlRel.h"

/*! \class CRowsControlRel
	\brief �R���g���[���ƃt�B�[�`���A�t�B�[���h�A�e�[�u�����֘A�t����N���X

	\b Example:

	SetArcHelper( pHelper );

	ArcObjects �w���p�[�N���X�̃|�C���^��n���ď��������܂��B

	SetControl();

	��ɃR���g���[����o�^���܂��B

	SetFeature();

	��Ńt�B�[�`����o�^���܂��B
*/
class CRowsControlRel  
{
public:
	CRowsControlRel();
	virtual ~CRowsControlRel();

public:
	void SetControl( INT nID, CONTROLTYPE type, INT nGroupFrom, INT nGroupTo, LPCTSTR lpszFieldName = NULL, UINT uiCheckType = NULL, 
		LPCTSTR lpszTableName = NULL, LPCTSTR lpszOuterKey = NULL, INT nFollowCtrlID = -1, UINT uiFollowBy = 0x00, INT nRelType = 0 );
	void SetControl( INT nID, CONTROLTYPE type, INT nGroupFrom, INT nGroupTo, std::list<CString> arrayFieldNames, UINT uiCheckType = NULL, 
		LPCTSTR lpszTableName = NULL, LPCTSTR lpszOuterKey = NULL, INT nFollowCtrlID = -1, UINT uiFollowBy = 0x00, INT nRelType = 0 );

	void SetArcHelper( IApplication* ipApp );
	_IRowPtr SetFeatures( _ISetPtr ipSet );
	void SetFeature( IFeaturePtr ipFeature );
	_IRowPtr GetFeature( LONG lIndex );
	void Clear();
	void Reset();
	void Store( BOOL bDoOperation );
	BOOL Changed();
	CFeatureDef* GetCurrentFeatureDef();
	void SetView( LONG lFeatureIndex, LONG lTableIndex = -1, LONG lRowIndex = -1 );
public:
	CControlRel			    m_pControlRel;		//!< �R���g���[�����X�g�ێ��N���X
	std::list<CFeatureDef>	m_listFeatureDef;	//!< �t�B�[�`�����X�g�ێ��N���X
	
private:
	CArcHelper			    m_cArcHelper;		//!< ArcObjects �w���p�[�N���X�|�C���^
	LONG				    m_lFeatureDefIndex;	//!< ���݃_�C�A���O�ɕ\������Ă���t�B�[�`���̃C���f�b�N�X�ԍ��ێ��p
};

#endif // !defined(AFX_ROWSCONTROLREL_H__343F14A0_BCF0_4BA7_ACFA_B0E09E3CAE6D__INCLUDED_)
