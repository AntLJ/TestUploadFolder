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

// CLAccess.h: CLAccess �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLACCESS_H__510BEF10_BDE1_454C_867C_1F332DFA03D1__INCLUDED_)
#define AFX_CLACCESS_H__510BEF10_BDE1_454C_867C_1F332DFA03D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
 * @class	CListHandle
 * @brief	���X�g�n���h���N���X
 */
class CListHandle
{
public:
	CListHandle(){};
	virtual ~CListHandle(){};

	/**
	 * @brief	�f�[�^���[�h
	 * @param	lpsczFile	[in]	�t�@�C��
	 * @return	bool
	 */
	bool loadData( const _TCHAR* lpcszFile );

	CLList& getRecodList(){ return m_listRecord; }

private:

	CLList	m_listRecord;		//!< ���R�[�h���X�g
};

#endif // !defined(AFX_CLACCESS_H__510BEF10_BDE1_454C_867C_1F332DFA03D1__INCLUDED_)
