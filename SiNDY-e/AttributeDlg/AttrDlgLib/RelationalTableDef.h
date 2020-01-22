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

/*! \file RelationalTableDef.h
	\brief CRelationalTableDef �N���X�̃C���^�[�t�F�[�X
	\author INCREMENTP CORP. R&D�Z�p�Z���^�[��S�J���O���[�v �Ð�M�G
	\date 2002-03-05
*/
#if !defined(AFX_RELATIONALTABLEDEF_H__23E142D3_970F_41E8_900E_36BF81CCBB14__INCLUDED_)
#define AFX_RELATIONALTABLEDEF_H__23E142D3_970F_41E8_900E_36BF81CCBB14__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RowDef.h"

class CArcHelper;

/*! \class CRelationalTableDef
	\brief �t�B�[�`���Ɋ֘A����O���e�[�u���p���b�p�N���X

	\b Example:

	CRelationalTableDef relTableDef;

	relTableDef.SetTable( ipTable, NODE_TABLE_NAME, 12345 );
*/
class CRelationalTableDef : public std::list<CRowDef>
{
public:
	CRelationalTableDef();
	virtual ~CRelationalTableDef();

public:
	void SetArcHelper( IApplication* ipApp ){m_cArcHelper.Init( (IApplicationPtr)ipApp);};
	void Clear();
	void Store();
	void Reset();
	void SetTable( ITablePtr ipTable, LPCTSTR lpcszTableName, LONG lOID, LPCTSTR lpcszOuterKey, BOOL bForce = FALSE, BOOL bNoSearch = FALSE );
	void SetDeleteFlag( LONG lIndex, BOOL bDelete = TRUE );
	BOOL IsNoRows();	//!< �폜���ꂽ���̂̓J�E���g�ΏۊO�iCFeatureDef::Store()�Ŏg�p�j
	BOOL CreateNewRow();

	inline ITablePtr GetTable() const { return m_ipTable; };
	inline CString GetTableName() const { return m_strTableName; };
	inline CString GetOuterKey() const { return m_strOuterKey; };
	inline LONG GetOID() const { return m_lOID; }; 
	inline void setChanged( bool bChanged ){ m_bChanged = bChanged; }
	inline bool getChanged() const { return m_bChanged; }
private:
	BOOL SetOuterKey( LPCTSTR lpcszOuterKey, BOOL bForce, BOOL bNoSearch = FALSE );

private:
	CArcHelper	m_cArcHelper;			//!< CArcHelper �N���X�̃|�C���^
	ITablePtr	m_ipTable;		//!< ���̃N���X�ň����e�[�u��
	CString		m_strTableName;	//!< m_ipTable �̃e�[�u����
	CString		m_strOuterKey;	//!< �O���L�[�p�t�B�[���h��
	LONG		m_lOID;			//!< �O���L�[�l�iOBJECTID�j
	bool        m_bChanged;		//!< �֘A�e�[�u���̍X�V�L��
};

#endif // !defined(AFX_RELATIONALTABLEDEF_H__23E142D3_970F_41E8_900E_36BF81CCBB14__INCLUDED_)
