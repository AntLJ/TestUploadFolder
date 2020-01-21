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
 * @file FieldMap.h
 * @brief CFieldMap�N���X��`�t�@�C��
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#ifndef _FIELDMAP_H_
#define _FIELDMAP_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ErrorCode.h"
#include "sindymacroutil.h"
#include <boost/shared_ptr.hpp>

namespace sindy {

/**
 * @class CFieldMap
 * @brief <b>�t�B�[���h���Ή��\</b>\n
 * ���̃N���X�́A�e�e�[�u���̃t�B�[���h�C���f�b�N�X��
 * �t�B�[���h���̑Ή����i�[���Ă����N���X�ł��B\n
 * ��ɁACRow�n�̃N���X�Ńt�B�[���h�Ή��\���L���b�V�����邽�߂�
 * �g�p����܂��B\n
 */
class SINDYLIB_API CFieldMap
{
public:
	struct _FIELD
	{
		CString name;
		CString alias;
		bool editable;
		bool nullable;
		bool isid;
		esriFieldType type;
		size_t length;
		long precision, scale;
		std::pair<CComVariant,CComVariant> range;
		std::map<CString,long> domain;
		std::map<long,CString> rdomain;
		CComVariant dvalue;
	};
	CFieldMap() : m_lOIDIndex(-1), m_lShapeIndex(-1)
	{
	}
	virtual ~CFieldMap()
	{
		clear();
	}

	CFieldMap( IFields* ipFields )
	{
		CreateFieldMap( ipFields );
	}

	CFieldMap& operator=( const CFieldMap& obj )
	{
		CopyFieldMap( *this, obj );
		return *this;
	}

	/**
	 * @brief CFieldMap���R�s�[����
	 * @param dest	[in]	�y�[�X�g��
	 * @param src	[in]	�R�s�[��
	 */
	friend void CopyFieldMap( CFieldMap& dest, const CFieldMap& src );

	// TODO: �߂�l��bool�ɂ��āAErrorCode.h���g�p���Ȃ��悤�ɂ���B
	/**
	 * @brief <b>�t�B�[���h�Ή��\���쐬����</b>\n
	 * �����̑Ή��\�������Ă��V�K�ɍ�蒼���܂��B
	 * @param ipFields	[in]	IFields*
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode CreateFieldMap( IFields* ipFields );

	/**
	 * @brief <b>�t�B�[���h������t�B�[���h�C���f�b�N�X�ԍ����擾����</b>\n
	 * @param lpcszFieldName	[in]	�t�B�[���h�C���f�b�N�X�ԍ�
	 * @return �Ή�����t�B�[���h�C���f�b�N�X�ԍ����Ȃ��ꍇ��-1
	 * @note �Ȃ���΃f�o�b�O�łł̓g���[�X���ł܂��B�����[�X�łł�
	 * HasField�Ɠ����ł��B
	 */
	long FindField( LPCTSTR lpcszFieldName ) const {
		return _FindField( lpcszFieldName, true );
	}

	/**
	 * @brief <b>�t�B�[���h������t�B�[���h�C���f�b�N�X�ԍ�������Ύ擾����</b>\n
	 * @param lpcszFieldName	[in]	�t�B�[���h�C���f�b�N�X�ԍ�
	 * @return �Ή�����t�B�[���h�C���f�b�N�X�ԍ����Ȃ��ꍇ��-1
	 * @note FindField�Ƃ̈Ⴂ�́A�f�o�b�O�łł��g���[�X���o�Ȃ����Ƃ݂̂ł��B
	 */
	long HasField( LPCTSTR lpcszFieldName ) const {
		return _FindField( lpcszFieldName, false );
	}

	/**
	 *  @brief �t�B�[���h�̕t���������擾����
	 * 
	 * @note Index���t�B�[���h���𒴂���ꍇ�͗�����̂�h�����߂�
	 * ��Ɉ�ԍŌ�̃t�B�[���h����Ԃ��܂��B
	 */
	const _FIELD& GetFieldAttr( long Index ) const;

	/**
	 * @brief <b>�t�B�[���h�C���f�b�N�X����t�B�[���h�����擾����</b>\n
	 * @param Index	[in]	�t�B�[���h�C���f�b�N�X
	 * @return �Ή�����t�B�[���h���i�啶������j
	 */
	LPCTSTR GetName( long Index ) const;

	/**
	 * @brief <b>�t�B�[���h�C���f�b�N�X����t�B�[���h�G�C���A�X�����擾����</b>\n
	 * @param Index	[in]	�t�B�[���h�C���f�b�N�X
	 * @return �Ή�����t�B�[���h�G�C���A�X��
	 */
	LPCTSTR GetAliasName( long Index ) const;

	/**
	 * @brief <b>�t�B�[���h�C���f�b�N�X����f�t�H���g�l���擾����</b>\n
	 * @param Index [in] �t�B�[���h�C���f�b�N�X
	 * @return �Ή�����f�t�H���g�l
	 */
	const VARIANT& GetDefaultValue( long Index ) const;

	/**
	 * @brief <b>�t�B�[���h���ҏW�\���ǂ������`�F�b�N����</b>\n
	 * @param lIndex	[in]	�t�B�[���h�C���f�b�N�X
	 * @retval false	�ҏW�s��
	 * @retval true		�ҏW��
	 */
	bool IsEditable( long lIndex ) const;

	/**
	 * @brief <b>�t�B�[���h��NULL OK���ǂ������`�F�b�N����</b>\n
	 * 
	 * @param lIndex	[in]	�t�B�[���h�C���f�b�N�X
	 * @retval false	NOT NULL
	 * @retval true	    NULL OK
	 */
	bool IsNullable( long lIndex ) const;

	/**
	 * @brief <b>�t�B�[���h��SiNDY�I�ɕҏW�\���ǂ������`�F�b�N����</b>\n
	 * @param lIndex	[in]	�t�B�[���h�C���f�b�N�X
	 * @retval false	�ҏW�s��
	 * @retval true		�ҏW��
	 */
	bool IsSiNDYEditable(long lIndex, bool bForCopy=false) const;

	/**
	 * @brief <b>�t�B�[���h�^�C�v���擾����</b>\n
	 * @param lIndex	[in]	�t�B�[���h�C���f�b�N�X
	 * @return esriFieldType
	 */
	esriFieldType GetFieldType( long lIndex ) const;

	/**
	 * @brief <b>�I�u�W�F�N�gID�̃C���f�b�N�X�ԍ����擾����</b>\n
	 * @return �C���f�b�N�X�ԍ�
	 */
	long GetOIDIndex() const { return m_lOIDIndex; }

	/**
	 * @brief <b>SHAPE�̃C���f�b�N�X�ԍ����擾����</b>\n
	 * @return �C���f�b�N�X�ԍ�
	 */
	long GetShapeIndex() const { return m_lShapeIndex; }

	/**
	 * @brief <b>�t�B�[���h�����擾����</b>\n
	 * @return �t�B�[���h��
	 */
	size_t GetFieldCount() const { return m_vecFieldName.size(); }

	/**
	 * @brief <b>�R�[�h�l�h���C���Ή��\���擾����</b>\n
	 * @warning long�l����ł��B�܂��A�͈̓h���C���ɂ͑Ή����Ă��܂���B
	 * @warning �K���t�B�[���h�������݂��邱�Ƃ��m�F���Ă��������B
	 */
	const std::map<CString,long>& GetDomain( LPCTSTR lpcszFieldName ) const;

	bool IsShapeField( long lIndex ) const { return ( lIndex == m_lShapeIndex ); }
	bool IsOIDField( long lIndex ) const { return ( lIndex == m_lOIDIndex ); }

	typedef std::vector<_FIELD>::iterator iterator;
	typedef std::vector<_FIELD>::const_iterator const_iterator;
	iterator begin() { return m_vecFieldName.begin(); }
	const_iterator begin() const { return m_vecFieldName.begin(); }
	iterator end() { return m_vecFieldName.end(); }
	const_iterator end() const { return m_vecFieldName.end(); }
private:
	/**
	 * @brief <b>�Ή��\���N���A����</b>\n
	 */
	void clear();
	long _FindField( LPCTSTR lpcszFieldName, bool bDebugTrace ) const;
private:
	long						m_lOIDIndex;		//!< OID�t�B�[���h�C���f�b�N�X�ԍ�
	long						m_lShapeIndex;		//!< SHAPE�t�B�[���h�C���f�b�N�X�ԍ�
#pragma warning(push)
#pragma warning(disable:4251)
	std::map<CString,long>		m_mapFieldIndex;	//!< �t�B�[���h���ƃt�B�[���h�C���f�b�N�X��map
	std::vector<_FIELD>			m_vecFieldName;		//!< �t�B�[���h���i�ҏW�s���t�j�̃��X�g�i�C���f�b�N�X���j
#pragma warning(pop)
};

typedef boost::shared_ptr<CFieldMap> CSPFieldMap;

} // sindy

#endif // _FIELDMAP_H_
