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
 * @file AttributeRistrict.h
 * @brief <b>CAttributeRistrict�N���X��`�t�@�C��</b>\n
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#ifndef _ATTRIBUTERISTRICT_H_
#define _ATTRIBUTERISTRICT_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ErrorCode.h"
#include <boost/shared_ptr.hpp>

namespace sindy {

class CRow;

class SINDYLIB_API CAttributeRistrictBase
{
public:
	CAttributeRistrictBase();
	virtual ~CAttributeRistrictBase();

	virtual bool SetRistrict( LPCTSTR lpcszRistrict );
	virtual sindyeEditableType IsEditable( const CRow& cRow, int nLen, LPTSTR lpszMessage ) const = 0;
protected:
	bool	m_bWarning;		//!< �x���t���O
};

typedef boost::shared_ptr<CAttributeRistrictBase> CSPAttributeRistrictBase;

/**
 * @class CRistrict
 * @brief <b>���������l��ێ����鏃�����z�N���X</b>\n
 */
class CAttributeRistrict : public CAttributeRistrictBase
{
public:
	CAttributeRistrict();
	virtual ~CAttributeRistrict();
	/**
	 * @brief <b>����������������p�[�X����</b>\n
	 * ���̊֐��ŏ�������̂̓t�B�[���h���A�x���t���O�݂̂ŁA���̏�����
	 * �I����ɐ؂������������_SetRistrict�֐��ɓn���܂��B
	 * @param lpcszRistrict	[in]	��������������
	 * @retval true �����񂪓K�؂ɏ������ꂽ
	 * @retval false �����񂪓K�؂ɏ�������Ȃ�����
	 */
	virtual bool SetRistrict( LPCTSTR lpcszRistrict );

	/// �^����ꂽ�I�u�W�F�N�g���ҏW�\���ǂ����`�F�b�N
	sindyeEditableType IsEditable( const CRow& cRow, int nLen, LPTSTR lpszMessage ) const;
protected:
	/**
	 * @brief <b>�^����ꂽ�l���ҏW�\�Ȓl���ǂ������`�F�b�N</b>\n
	 * ���̊֐���_IsEditable�֐��̏����ɔے�����y�ьx��������ǉ�����֐��ł��B
	 * ���ۂ̒l��r�ɂ�_IsEditable�֐����g�p���܂��B
	 * @param vaValue	[in]	��r�l
	 * @retval sindyeEditableType
	 */
	sindyeEditableType IsEditable( const VARIANT& vaValue ) const;

	/**
	 * @brief <b>����������������p�[�X���鏃�����z�֐�</b>\n
	 * ���̊֐��͏������z�֐��ł��B���ۂ̏����͌p����N���X�Ŏ���
	 * ����K�v������܂��B
	 * @param lpcszRistrict	[in]	��������������ie.x.�u100-200�v�A�u3,4,5:W�v�j
	 */
	virtual bool _SetRistrict( LPCTSTR lpcszRistrict ) = 0;

	/**
	 * @brief <b>�^����ꂽ�l���ҏW�\�Ȓl���ǂ������`�F�b�N���鏃�����z�֐�</b>\n
	 * ���̊֐��͏������z�֐��ł��B���ۂ̏����͌p����N���X�Ŏ���
	 * ����K�v������܂��B\n
	 * �x���E�ے�����͌Ăяo�����֐��iIsEditable�j�ŏ�������܂��̂ŁA���̊֐��ł�
	 * �������܂���B
	 * @param vaValue	[in]	��r�l
	 * @retval sindyeEditableEditable �ҏW��
	 * @retval sindyeEditableNoEditable �ҏW�s��
	 */
	virtual sindyeEditableType _IsEditable( const VARIANT& vaValue ) const = 0;
public:
	CString	m_strFieldName;	//!< �t�B�[���h��
	CString	m_strRistrict;	//!< ��������������
private:
	bool	m_bNot;			//!< �����ے�t���O
};

/**
 * @brief <b>�ő�ŏ��̑��������l��ێ�����N���X</b>\n
 * �t�H�[�}�b�g�F���l-���l\n
 * ���҂��镶����́u0-100�v�Ƃ��u0-100�v�Ƃ�\n
 * ���l�ł����OK�����A_ttof�ŕϊ��ł��邱�Ƃ�����\n
 * �}�C�i�X�l�̏ꍇ�́A-100����-1�̏ꍇ�u-100--1�v�A-11����30�̏ꍇ�u-11-30�v\n
 */
class CAttributeRistrictMinMax : public CAttributeRistrict, public std::pair<double, double>
{
protected:
	bool _SetRistrict( LPCTSTR lpcszRistrict );
	sindyeEditableType _IsEditable( const VARIANT& vaValue ) const;
};

/**
 * @brief <b>�ʑ��������l��ێ�����N���X</b>\n
 * �t�H�[�}�b�g�F���l[,���l[,���l[...\n
 * ���҂��镶����́u3�v�Ƃ��u0,-1,2�v�Ƃ��uNULL,'����������'�v�Ƃ�\n
 * ���l��_ttof�ŕϊ��ł��邱�Ƃ�����\n
 */
class CAttributeRistrictValues : public CAttributeRistrict, public std::list<CComVariant>
{
protected:
	bool _SetRistrict( LPCTSTR lpcszRistrict );
	sindyeEditableType _IsEditable( const VARIANT& vaValue ) const;
private:
	void AddValue( LPCTSTR lpcszValue );
};

class SINDYLIB_API CAttributeRistricts : public CAttributeRistrictBase
{
public:
	CAttributeRistricts();
	virtual ~CAttributeRistricts();

	virtual sindyeEditableType IsEditable( const CRow& cRow, int nLen, LPTSTR lpszMessage ) const = 0;

	virtual bool AddRistrict( LPCTSTR lpcszRistrict );

	typedef std::list<CSPAttributeRistrictBase>::iterator iterator;
	typedef std::list<CSPAttributeRistrictBase>::const_iterator const_iterator;

	void clear();
protected:
#pragma warning(push)
#pragma warning(disable: 4251)
	std::list<CSPAttributeRistrictBase> m_listRistrict;
#pragma warning(pop)
};

/**
 * @brief <b>AND�����Ŋe����������������N���X</b>\n
 * �t�H�[�}�b�g�F����|����|...[:w]
 */
class SINDYLIB_API CAttributeRistrictsAnd : public CAttributeRistricts
{
public:
protected:
	bool SetRistrict( LPCTSTR lpcszRistrict );
public:
	sindyeEditableType IsEditable( const CRow& cRow, int nLen, LPTSTR lpszMessage ) const;
};

class SINDYLIB_API CAttributeRistrictsOr : public CAttributeRistricts
{
public:
	CAttributeRistrictsOr();
	virtual ~CAttributeRistrictsOr();

	CAttributeRistrictsOr( const CAttributeRistrictsOr& obj );
	CAttributeRistrictsOr& operator=( const CAttributeRistrictsOr& obj );

	bool AddRistrict( LPCTSTR lpcszRistrict );
	sindyeEditableType IsEditable( const CRow& cRow, int nLen, LPTSTR lpszMessage ) const;

/*private:
	typedef std::list< boost::shared_ptr<CAttributeRistrictBase> >::iterator iterator;
	typedef std::list< boost::shared_ptr<CAttributeRistrictBase> >::const_iterator const_iterator;
#pragma warning(push)
#pragma warning(disable: 4251)
	std::list< boost::shared_ptr<CAttributeRistrictBase> > m_listRistrict;
#pragma warning(pop)*/
};

} // sindy

#endif // _ATTRIBUTERISTRICT_H_
