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

namespace addr
{
/**
 * @class	CCodeConverter
 * @brief	�����t���Z���R�[�h�w���p�[�N���X
 * @note	�v���Z�X�ň����Ηǂ��̂ŁA�V���O���g��
 */
class CCodeConverter
{
private:
	CCodeConverter(void){}
	~CCodeConverter(void){}
	CCodeConverter( const CCodeConverter& ){}
	CCodeConverter& operator=( const CCodeConverter& ){}

public:

	/**
	 * @brief	�C���X�^���X�擾
	 * @return	CCodeHelper
	 */
	static CCodeConverter& GetCodeConverter()
	{
		static CCodeConverter cCodeConverter;
		return cCodeConverter;
	}

	/**
	 * @brief	������
	 * @param	lpcszCodeConvDB	[in]	�R�[�h�ϊ�DB(AccessDB�p�X)
	 * @return	bool
	 */
	bool Init( const _TCHAR* lpcszCodeConvDB );

	/**
	 * @brief	�n�ԃR�[�h�擾
	 * @param	lpcszAddrCode	[in]			11���R�[�h
	 * @param	lpcszPntName	[in]			�n�Ԗ���
	 * @param	bSave			[in, optional]	�擾�����R�[�h���V�K�Ȃ�e�[�u���ɕۑ�����H(�f�t�H���g false)
	 * @return	�V�K�n�ԃR�[�h(�擾���s���́ANULL)
	 */
	CString	GetPntCode( const _TCHAR* lpcszAddrCode, const _TCHAR* lpcszPntName, bool bSave = false );

	/**
	 * @brief	�Z���ԍ��R�[�h�擾
	 * @param	lpcszAddrCode	[in]			11���R�[�h
	 * @param	lpcszGouName	[in]			�Z���ԍ�����
	 * @param	bSave			[in, optional]	�擾�����R�[�h���V�K�Ȃ�e�[�u���ɕۑ�����H(�f�t�H���g false)
	 * @return	�V�K�Z���ԍ��R�[�h(�擾���s���́ANULL)
	 */
	CString	GetGouCode( const _TCHAR* lpcszAddrCode, const _TCHAR* lpcszGouName, bool bSave = false );

	/**
	 * @brief	�n�Ԗ��̎擾
	 * @param	lpcszAddrCode	[in]	11���R�[�h
	 * @param	lpcszPntCode	[in]	�n�ԃR�[�h
	 * @return	�n�Ԗ���(�擾���s���́ANULL)
	 */
	CString GetPntName( const _TCHAR* lpcszAddrCode, const _TCHAR* lpcszPntCode ) const;

	/**
	 * @brief	�Z���ԍ����̎擾
	 * @param	lpcszAddrCode	[in]	11���R�[�h
	 * @param	lpcszGouCode	[in]	�Z���ԍ��R�[�h
	 * @return	�Z���ԍ�����(�擾���s���́ANULL)
	 */
	CString GetGouName( const _TCHAR* lpcszAddrCode, const _TCHAR* lpcszGouCode ) const;

	/**
	 * @brief	�G���[���b�Z�[�W�擾
	 * @return	�G���[���b�Z�[�W
	 */
	CString GetErrorMessage() const{ return m_strError; }

private:

	/**
	 * @brief	�V�K�R�[�h�o�^
	 * @param	szClass		[in]	p: pnt g: gon
	 * @param	lpcszAddrCode	[in]	11���Z���R�[�h
	 * @param	lpcszName		[in]	����
	 * @param	lpcszNewCode	[in]	�V�K�R�[�h
	 * @return	bool
	 */
	bool InsertCode( TCHAR szClass, const _TCHAR* lpcszAddrCode, const _TCHAR* lpcszName, const _TCHAR* lpcszNewCode );

private:

	CADOBase	m_cAdoHelper;	//!< ADO�w���p�[
	mutable		CString		m_strError;		//!< �Ō�̃G���[���b�Z�[�W

};

}	// namespace addr
