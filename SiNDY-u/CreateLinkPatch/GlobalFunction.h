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

/**
 *	@file	GlobalFunction.h
 *	@brief	�O���[�o���֐���`�t�@�C��
 *	@author	F.Adachi
 *	@date	2005/08/05		�V�K�쐬
 *	$ID$
 */

#ifndef ___GLOBALFUNCTION___
#define ___GLOBALFUNCTION___

#include <string>

namespace GlobalFunction
{

	/**
	 *	@brief	�t�B�[���h�l���擾
	 *	@note	��ƃt�B�[���h�����w�肵�ăt�B�[���h�̒l�����o��
	 *	@param	ipRow			[in]	�Ώۗ�C���^�[�t�F�C�X�ւ̃|�C���^
	 *	@param	cFieldName		[in]	�Ώۃt�B�[���h��
	 *	@return	�t�B�[���h�l
	 **/
	inline CComVariant	getValue( const _IRowPtr ipRow, const LPCTSTR cFieldName )
	{
		CComVariant	vaValue;
		vaValue.vt = VT_EMPTY;
		if (ipRow == NULL) { return vaValue; }

		// ��|�C���^����t�B�[���h�|�C���^���擾
		IFieldsPtr	ipFields;
		ipRow->get_Fields( &ipFields );

		// �w��t�B�[���h���̃C���f�b�N�X���擾
		LONG	aFieldIndex;
		if (ipFields->FindField( CComBSTR( cFieldName ), &aFieldIndex ) != S_OK) { return vaValue; }
		// �C���f�b�N�X���w�肵�ăt�B�[���h�l���擾
		if (aFieldIndex >= 0) { 
			ipRow->get_Value( aFieldIndex, &vaValue );
		}

		return vaValue;
	}


	/**
	 *	@brief	�t�B�[���h�l��o�^
	 *	@note	��ƃt�B�[���h�����w�肵�ăt�B�[���h�̒l��ύX����
	 *	@param	ipRow			[in]	�Ώۗ�ւ̃|�C���^
	 *	@param	cFieldName		[in]	�Ώۃt�B�[���h��
	 *	@param	vaValue			[in]	�o�^����t�B�[���h�l
	 **/
	inline void setValue( const _IRowPtr ipRow, const LPCTSTR cFieldName, CComVariant vaValue )
	{
		std::string aFunc = "GlobalFunction::setValue()";

		if( vaValue.vt == VT_ERROR ) { throw std::runtime_error( "R1F:" + aFunc + " : �t�B�[���h�l���s��" ); }
		if (ipRow == NULL) { throw std::runtime_error( "R1F:" + aFunc + " : NULL" ); }

		// ��|�C���^����t�B�[���h�|�C���^���擾
		IFieldsPtr ipFields;
		if (ipRow->get_Fields( &ipFields ) != S_OK) { throw std::runtime_error( aFunc + " : get_Field()���s" ); }

		// �w��t�B�[���h���̃C���f�b�N�X���擾
		LONG	aFieldIndex;
		if (ipFields->FindField( CComBSTR( cFieldName ), &aFieldIndex ) != S_OK) { throw std::runtime_error( "R1F:" + aFunc + " : FindField()���s" ); }
		
		// �C���f�b�N�X���w�肵�ăt�B�[���h�l��o�^
		if (aFieldIndex < 0) { throw std::runtime_error( "R1F:" + aFunc + " : �t�B�[���h��Index���s��" ); }
		if (ipRow->put_Value( aFieldIndex, vaValue ) != S_OK) { throw std::runtime_error( "R1F:" + aFunc + " : put_Value()���s" ); }
	}


	/**
	 * @brief	���ݎ����̎擾
	 * @param	crStr	[out]	�擾�������ݎ������������ޑΏە�����ւ̃��t�@�����X
	 */
	inline void getTime( CString &crStr )
	{
		SYSTEMTIME aSystemTime;
		::GetLocalTime(&aSystemTime);

		CString	aWord("");
		aWord.Format( TEXT( "%04d/%02d/%02d %02d:%02d:%02d"),
							aSystemTime.wYear,	aSystemTime.wMonth, aSystemTime.wDay,
							aSystemTime.wHour,	aSystemTime.wMinute,aSystemTime.wSecond);
		crStr = aWord;
	}

};

#endif
