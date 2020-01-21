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
 * @file CacheAttribute.h
 * @brief <b>CCacheAttribute�N���X��`�t�@�C��</b>\n
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * @version $Id$
 */
#ifndef _CACHEATTRIBUTE_H_
#define _CACHEATTRIBUTE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ErrorCode.h"

HRESULT SINDYLIB_API Fix_BSTR_get_Value( _IRow* p, long index, VARIANT* va );

namespace sindy {

class CFieldMap;

/**
 * @class CCacheAttribute
 * @brief <b>�����L���b�V���N���X</b>\n
 * ���̃N���X��IRow���ێ����鑮�����ꕔ�������ăL���b�V�����܂��B\n
 * �L���b�V���쐬�̍ۂɏ������t�B�[���h�^�C�v�͈ȉ��̂Ƃ���ł��B
 * @li �W�I���g���t�B�[���h
 * @li BLOB�t�B�[���h
 * �L���b�V���o�b�t�@��ɂ́A��L���i�[���邽�߂̏ꏊ���p�ӂ���Ă��܂����A
 * ���ۂɂ̓R�s�[����܂���̂Œ��ӂ��ĉ������B
 */
class SINDYLIB_API CCacheAttribute  
{
public:
	explicit CCacheAttribute()
	{
		m_pVars = NULL;
		Init();
	}
	virtual ~CCacheAttribute()
	{
		Init();
	}

	CCacheAttribute( _IRow* lp )
	{
		m_pVars = NULL;
		CreateCache( lp );
	}
	CCacheAttribute& operator=( _IRow* lp )
	{
		CreateCache( lp );
		return *this;
	}
	CCacheAttribute( const CCacheAttribute& lp );
	CCacheAttribute& operator=( const CCacheAttribute& lp );

	/**
	 * @brief <b>�C���f�b�N�X�ԍ��ɑΉ�����f�[�^��Ԃ�</b>\n
	 * �����o�b�t�@�̃|�C���^�����̂܂ܕԂ��܂��̂ŁA������
	 * ���ӂ��ĉ������B
	 * @param index	[in]	�C���f�b�N�X�ԍ�
	 * @return �Ή�����f�[�^���������NULL
	 */
	VARIANT* operator[]( long index )
	{
		return ( m_pVars ) ? &m_pVars[index] : NULL;
	}
	
	/**
	 * @brief <b>�C���f�b�N�X�ԍ��ɑΉ�����f�[�^��Ԃ�</b>\n
	 * �Q�Ɛ�p�ł��B
	 * @param index	[in]	�C���f�b�N�X�ԍ�
	 * @return �Ή�����f�[�^���������NULL
	 */
	const VARIANT* operator[]( long index ) const
	{
		return ( m_pVars ) ? &m_pVars[index] : NULL;
	}
	
	/**
	 * @brief <b>�t�B�[���h�J�E���g��Ԃ�</b>\n
	 * �L���b�V�����ꂽ�t�B�[���h�̐��ł͂���܂���̂�
	 * ���ӂ��ĉ������B
	 */
	long GetCount() const { return m_lCount; }

	/**
	 * @brief <b>�����̃L���b�V�����쐬����</b>\n
	 * �����̂��̂�����ꍇ�͍폜��ɍ쐬����܂��B
	 * �L���b�V���쐬�̍ۂɏ������t�B�[���h�^�C�v�͈ȉ��̂Ƃ���ł��B
	 * @li �W�I���g���t�B�[���h
	 * @li BLOB�t�B�[���h
	 * �L���b�V���o�b�t�@��ɂ́A��L���i�[���邽�߂̏ꏊ���p�ӂ���Ă��܂����A
	 * ���ۂɂ̓R�s�[����܂���̂Œ��ӂ��ĉ������B
	 * @param lp		[in]			_IRow*
	 * @param ipFields	[in,optional]	IFields*�i�f�t�H���g�FNULL�j
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode CreateCache( _IRow* lp, IFields* ipFields = NULL );
	errorcode::sindyErrCode CreateCache( _IRow* lp, const CFieldMap& cFields );

	/**
	 * @brief <b>CCacheAttribute���R�s�[����</b>\n
	 * �����I�ɂ�m_pVars��m_lCount���R�s�[����邾���ł��B\n
	 * �����̂��̂�����ꍇ�͍폜��ɍ쐬����܂��B
	 * @param dest	[out]	CCacheAttribute&
	 * @param src	[in]	CCacheAttribute&
	 * @return sindyErrCode
	 */
	friend errorcode::sindyErrCode CopyCacheAttribute( CCacheAttribute& dest, const CCacheAttribute& src );
private:
	/**
	 * @brief <b>�����o�ϐ�������������</b>\n
	 */
	void Init();
private:
	long		m_lCount;	//!< ������
	VARIANT*	m_pVars;	//!< �����L���b�V���o�b�t�@
};

} // sindy

#endif // _CACHEATTRIBUTE_H_
