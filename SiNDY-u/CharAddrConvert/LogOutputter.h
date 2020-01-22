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

/**
 * @file	LogOutputter.h
 * @brief	�i���n�̏o�͊֐��S��`
 */

/**
 * @enum	ERROR_LEVEL
 * @brief	�G���[���x��
 */
enum ERROR_LEVEL
{
	kInfo,		//!< ���
	kWarning,	//!< �x��
	kError,		//!< �G���[
	kFatal,		//!< �v���I�G���[
};

namespace progress_out
{
	/**
	 * @brief	���b�Z�[�W���`
	 * @param	kErrorLevel	[in]	���b�Z�[�W���x��
	 * @param	lpcszFormat	[in]	�t�H�[�}�b�g
	 * @param	vaList		[in]	����
	 * @return	CString
	 */
	CString FormatMsg( ERROR_LEVEL kErrorLevel, LPCTSTR lpcszFormat, va_list vaList );	

	/**
	 * @brief	�i�����b�Z�[�W�o��
	 * @note	�W���o�́E�W���G���[�ɏo��
	 * @param	lpcszFormat	[in]	���b�Z�[�W
	 * @param	...			[in]	�K�v�ɉ�����
	 */
	void Progress( LPCTSTR lpcszFormat, ... );

	/**
	 * @brief	���ԕ\��
	 * @param	lpcszMsg	[in]	�K�v�ł����
	 */
	void ProgressTime( LPCTSTR lpcszMsg = NULL );

	/**
	 * @brief	�i���n�G���[���b�Z�[�W�o��(�߂�l����)
	 * @param	ret			[in]	�߂�l
	 * @param	kErrorLevel	[in]	�G���[���x��	 
	 * @param	lpcszFormat	[in]	���b�Z�[�W
	 * @param	...			[in]	�K�v�ɉ�����
	 */
	template <class T> inline T ProgressError( T ret, ERROR_LEVEL kErrorLevel, LPCSTR lpcszFormat, ... )
	{
		va_list vaList;
		if( lpcszFormat )
			va_start( vaList, lpcszFormat );

		std::cerr << FormatMsg(kErrorLevel, lpcszFormat, vaList) << std::endl;
		std::cout << FormatMsg(kErrorLevel, lpcszFormat, vaList) << std::endl;


		return ret;
	}
}

namespace object_out
{
	extern CString g_strCurrentCityCode;	//!< �s�撬���R�[�h

	/**
	 * @brief	�w�b�_�[�o��
	 */
	void LogHeader();

	/**
	 * @brief	���b�Z�[�W���`
	 * @param	lpcszFormat	[in]	�t�H�[�}�b�g
	 * @param	vaList		[in]	����
	 * @return	CString
	 */
	inline CString FormatMsg( LPCTSTR lpcszFormat, va_list vaList )
	{
		CString strMsg;
		if( lpcszFormat )
			strMsg.AppendFormatV( lpcszFormat, vaList );
		else
			strMsg.Append( _T("���b�Z�[�W�Ȃ�") );
		return strMsg;
	}

	/**
	 * @brief	�G���[�o��
	 * @param	lpcszLayer	[in]	���C����
	 * @param	lOID		[in]	OBJECTID
	 * @param	lpcszFormat	[in]	���b�Z�[�W
	 * @param	...			[in]	�K�v�ɉ�����
	 */
	void ObjectError( LPCTSTR lpcszLayer, long lOID, LPCTSTR lpcszFormat, ... );

	/**
	 * @brief	�G���[�o��(IRow�w���)
	 * @param	ipRow		[in]	IRow
	 * @param	lpcszFormat	[in]	���b�Z�[�W
	 * @param	...			[in]	�K�v�ɉ�����
	 */
	void ObjectError( _IRowPtr ipRow, LPCTSTR lpcszFormat, ... );
}
