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

#include "AddrBasePoint.h"

namespace addr
{
using namespace sindy;
using namespace sindy::errorcode;
using namespace sindy::schema;

/**
 * @class	CBaseRepPoint
 * @brief	���kADDR�|�C���g�Ǘ��N���X
 */
class CBaseRepPoint : public CAddrBasePoint
{
public:
	CBaseRepPoint()
	{
		Init();
		m_emTableType = sindyTableType::base_rep_point;
	}
	virtual ~CBaseRepPoint(){ DeleteCache(); }
	CBaseRepPoint( const CBaseRepPoint& obj )
	{
		Init();
		*this = obj;
	}
	CBaseRepPoint( _IRow* lp, sindyTableType::ECode emTableType = sindyTableType::base_rep_point, bool bCreated = false, const CSPFieldMap& spFields = CSPFieldMap(), const CSPTableNameString& spNameString = CSPTableNameString() )
	{
		Init();
		SetObject( lp, emTableType, bCreated, spFields, spNameString );
	}
	CBaseRepPoint& operator=( const CBaseRepPoint& obj )
	{
		obj.CopyTo( *this );
		m_strAddrName = obj.m_strAddrName;
		return *this;
	}
	CBaseRepPoint& operator=( _IRow* lp )
	{
		SetObject( lp, sindyTableType::base_rep_point );
		return *this;
	}

	/**
	 * @brief	<b>�Z���R�[�h�擾</b>
  	 * @param	bChar			[in, optional]	�����t���Z���l������H(true�ɂ���ƁA�����t���Z���l���������ʂ��Ԃ�܂�)
	 * @param	bCodeDBSaved	[in, optional]	�����t���Z���R�[�h�ϊ�DB�ɑ��݂��Ȗ��̂̂Ƃ��ۑ�������ŃR�[�h��V�K���s����H(true�ɂ���ƁA�R�[�h�ϊ�DB�ɍX�V����������܂�)
	 * @note	���kADDR�|�C���g�ł́A��L�����͑S�Ďg�p���܂���
	 * @return	�Z���R�[�h
	 */
	CString GetAddrCode( bool bChar = false, bool bCodeDBSaved = false )
	{
		CComVariant vaValue;
		return (sindyErr_NoErr == GetValueByFieldName(base_rep_point::kAddrCode, vaValue) && VT_BSTR == vaValue.vt)?
				CString(vaValue.bstrVal) : _T("");
	}

	/**
	 * @brief	<b>�Z�����̎擾</b>
 	 * @param	bChar			[in, optional]	�����t���Z���l������H(true�ɂ���ƁA�����t���Z���l���������ʂ��Ԃ�܂�)
 	 * @note	���kADDR�|�C���g�ł́A��L�����͑S�Ďg�p���܂���
	 * @return	�Z������
	 */
	CString GetAddrName( bool bChar = false );

	/**
	 * @brief	<b>��\�_���̎擾</b>
	 * @return	��\�_����
	 */
	CString GetAdminName() const
	{
		CComVariant vaValue;
		return (sindyErr_NoErr == GetValueByFieldName(base_rep_point::kAdminName, vaValue) && VT_BSTR == vaValue.vt)?
				CString(vaValue.bstrVal) : _T("");
	}

	/**
	 * @brief	<b>�X�֔ԍ��擾</b>
	 * @note	Bug8043
	 * @return	��\�_����
	 */
	CString GetZipCode() const
	{
		CComVariant vaValue;
		return (sindyErr_NoErr == GetValueByFieldName(base_rep_point::kZipCode, vaValue) && VT_BSTR == vaValue.vt)?
				CString(vaValue.bstrVal) : _T("");
	}
};

}	// namespace addr
