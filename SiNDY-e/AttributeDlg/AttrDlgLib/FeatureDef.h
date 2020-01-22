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

/*! \file FeatureDef.h
	\brief CFeatureDef �N���X�̃C���^�[�t�F�[�X
	\author INCREMENTP CORP. R&D�Z�p�Z���^�[��S�J���O���[�v �Ð�M�G
	\date 2002-03-05
*/

#if !defined(AFX_FEATUREDEF_H__26B30C3E_C99F_4F1A_B306_F9BAC9CAF460__INCLUDED_)
#define AFX_FEATUREDEF_H__26B30C3E_C99F_4F1A_B306_F9BAC9CAF460__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RelationalTableDef.h"

class CArcHelper;

/*! \class CFeatureDef
	\brief IFeature �̃��b�p�[�N���X

	���̃N���X�� IFeature �ƈ�Έ�̊֌W�ɂȂ�܂��B
	�������ASiNDY-b �Ń����[�V�����������Ă��Ȃ������l�����āA�Ǝ��Ƀ����[�V�����e�[�u�����Ǘ�����
	�����o�������Ă��܂��B

	\b Example:

	CFeatureDef featDef;

	CFeatureDef �I�u�W�F�N�g�𐶐����܂��B

	featDef.SetArcHelper( pHelper );

	CArcHelper ���Z�b�g���܂��BSetFeature() ����O�ɕK���ĂԕK�v������܂��B

	featDef.SetFeature( ipFeature );

	���ƂȂ� IFeature �C���^�[�t�F�[�X�I�u�W�F�N�g���Z�b�g���܂��B
	���̊֐����ĂԂƁA�֘A�e�[�u����t�B�[���h��S�ăZ�b�g���܂��B
*/
class CFeatureDef : public CRowDef
{
public:
	CFeatureDef();
	virtual ~CFeatureDef();

	void SetArcHelper( IApplication* ipApp );
	void SetFeature( _IRowPtr ipFeature );
	_IRowPtr GetFeature();
	list<CRelationalTableDef>* GetRelationalTableDefList();
	CRelationalTableDef* GetRelationalTableDef( LPCTSTR lpcszTableName );
	void Store( BOOL bDoOperation );
	void Clear();
	void Reset();
	void SetWarnDelete(){ m_bWarnDelete = TRUE; };
	BOOL GetWarnDelete(){ return m_bWarnDelete; };
	BOOL Changed();
	CFieldDef* GetFieldDef( LPCTSTR lpszTableName, LPCTSTR lpszRFName );
private:
	/**
	* @brief  �O���֘A�e�[�u�����A�t�B�[���h�����擾����
	* @param  strTableName       [in] �֘A���e�[�u����
	* @param  strFieldName       [in] �֘A���t�B�[���h��
	* @param  strOuterTableName  [in] �֘A��e�[�u����
	* @param  strOuterFieldName  [in] �֘A��t�B�[���h��
	* @retval true  �擾����
	* @retval FALSE �擾���s
	*/
	bool getOuterTableAndFieldName( const CString& strTableName, const CString& strFieldName, CString& strOuterTableName, CString& strOuterFieldName);

public:
	BOOL						m_bError;
	sindyeEditableType			m_sindyeEditableType;
	CComBSTR					m_bstrIsEditableFeature;
private:
	CArcHelper					m_cArcHelper;			//!< CArcHelper �N���X�̃|�C���^

	std::list<CRelationalTableDef>	m_listRelationalTable;	//!< �֘A����O���e�[�u���̃��X�g
	_IRowPtr						m_ipFeature;			//!< ���ƂȂ� IFeature �C���^�[�t�F�[�X�I�u�W�F�N�g
	BOOL						m_bWarnDelete;			//!< �x�����x������������Ȃ�TRUE
};

#endif // !defined(AFX_FEATUREDEF_H__26B30C3E_C99F_4F1A_B306_F9BAC9CAF460__INCLUDED_)
