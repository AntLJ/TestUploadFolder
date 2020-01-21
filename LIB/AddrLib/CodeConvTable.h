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

#include <WinLib/ADOBase.h>

/**
 * @class	CCodeConvTable
 * @brief	�����t���Z���ƃR�[�h�̑Ή��\
 */
class CCodeConvTable
{
public:
	CCodeConvTable(void){}
	~CCodeConvTable(void){ m_cAdoHelper.disconnect(); }

	/**
	 * @brief	�f�[�^�x�[�X�I�[�v��
	 * @param	lpcszDBProp	[in]	
	 * @return	bool
	 */
	bool OpenDB( LPCTSTR lpcszDBProp );

	/**
	 * @brief	�n�ԃR�[�h�擾
	 * @param	strAddrCode	[in]	11���R�[�h
	 * @param	strPntName	[in]	�n�Ԗ���
	 * @param	bSave		[in]	�擾�����R�[�h���V�K�Ȃ�e�[�u���ɕۑ�����H(�f�t�H���g true)
	 * @return	�V�K�n�ԃR�[�h
	 */
	CString	GetPntCode( const CString& strAddrCode, const CString& strPntName, bool bSave = true );

	/**
	 * @brief	�Z���ԍ��R�[�h�擾
	 * @param	strAddrCode	[in]	11���R�[�h
	 * @param	strGouName	[in]	�Z���ԍ�����
	 * @return	�V�K�Z���ԍ��R�[�h
	 */
	CString	GetGouCode( const CString& strAddrCode, const CString& strGouName );

private:

	/**
	 * @brief	�V�K�R�[�h�o�^
	 * @param	szClass		[in]	p: pnt g: gon
	 * @param	strAddrCode	[in]	11���Z���R�[�h
	 * @param	strName		[in]	����
	 * @param	strNewCode	[in]	�V�K�R�[�h
	 * @return	bool
	 */
	bool InsertCode( TCHAR szClass, const CString& strAddrCode, const CString& strName, const CString& strNewCode );

private:

	CADOBase		m_cAdoHelper;		//!< ADO�w���p�[
};
