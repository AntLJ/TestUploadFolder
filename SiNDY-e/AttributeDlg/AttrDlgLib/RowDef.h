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

/*! \file RowDef.h
	\brief CRowDef �N���X�̃C���^�[�t�F�[�X
	\author INCREMENTP CORP. R&D�Z�p�Z���^�[��S�J���O���[�v �Ð�M�G
	\date 2002-03-05
*/
#if !defined(AFX_ROWDEF_H__C7B7C414_7D23_4C0E_864A_80AE6CE75C9C__INCLUDED_)
#define AFX_ROWDEF_H__C7B7C414_7D23_4C0E_864A_80AE6CE75C9C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FieldDef.h"

/*!	\class CRowDef 
	\brief _IRow �̃��b�p�[�N���X

	���̃N���X�� _IRow �ƈ�Έ�̊֌W�ɂȂ�܂��B
	�������A�g�p���₷���悤�ɂ������̃����o�ϐ���ǉ����Ă���܂��B
	�܂��A���̃N���X������ CFeatureDef �� RelationalTableDef ����`����Ă��܂��B
	
	�ʏ�͂��̃N���X�����̂܂܎g�p���邱�Ƃ͂Ȃ��A��ɔh�������N���X���g�p���邱�ƂɂȂ�܂��B
 */
class CRowDef : public std::list<CFieldDef>
{
public:
	CRowDef();
	virtual ~CRowDef();

	void Clear();
	void Reset();
	BOOL Store( BOOL bDoOperation );
	BOOL Changed();
	LPCTSTR GetTableName();
	void SetDeleteFlag( BOOL bDelete = TRUE );
	void SetCreateFlag();
	void SetRow( _IRowPtr ipRow );
	void SetTable( ITablePtr ipTable );
	LONG GetRowOID();
	inline _IRowPtr GetRow(){ return m_ipRow; };				//!< ���̃N���X�̑匳�ł��� _IRow ��Ԃ�
	inline BOOL IsDeleted(){ return m_bDeleted; };
	CFieldDef* GetFieldDef( LPCTSTR lpszFieldName );
	inline BOOL GetDeleteFlag(){ return m_bDeleteFlag; };	//!< ���̃��R�[�h���폜�Ώۂ��ǂ��������� m_bDeleteFlag ��Ԃ�
	inline BOOL GetCreateFlag(){ return m_bCreateFlag; };	//!< ���̃��R�[�h���V�K�쐬�Ώۂ��ǂ��������� m_bCreateFlag ��Ԃ�
protected:
	void Delete();
	void Create();
protected:
	CString m_strTableName;			//!< m_ipTable �̃e�[�u����
	_IRowPtr m_ipRow;				//!< ���ɂȂ� _IRow �C���^�[�t�F�[�X�|�C���^
	ITablePtr m_ipTable;			//!< m_ipRow �����݂��� ITable �C���^�[�t�F�[�X�|�C���^
	BOOL m_bDeleteFlag;				//!< �O���e�[�u���Ń��R�[�h���폜����ꍇ�A���̃t���O�𗧂Ă�
	BOOL m_bCreateFlag;				//!< �O���e�[�u���Ƀ��R�[�h��ǉ�����ꍇ�A���̃t���O�𗧂Ă�
	BOOL m_bDeleted;				//!< Delete() �̌�͕K�����̃t���O������
};

#endif // !defined(AFX_ROWDEF_H__C7B7C414_7D23_4C0E_864A_80AE6CE75C9C__INCLUDED_)
