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
 * @class	CCSAddrPoint
 * @brief	CS�|�C���g�Ǘ��N���X
 */
class CCSAddrPoint : public CAddrBasePoint
{
public:
	CCSAddrPoint()
	{
		Init();
		m_emTableType = sindyTableType::cs_addr_point;
	}
	virtual ~CCSAddrPoint(){ DeleteCache(); }
	CCSAddrPoint( const CCSAddrPoint& obj )
	{
		Init();
		*this = obj;
	}
	CCSAddrPoint( _IRow* lp, sindyTableType::ECode emTableType = sindyTableType::cs_addr_point, bool bCreated = false, const CSPFieldMap& spFields = CSPFieldMap(), const CSPTableNameString& spNameString = CSPTableNameString() )
	{
		Init();
		SetObject( lp, emTableType, bCreated, spFields, spNameString );
	}
	CCSAddrPoint& operator=( const CCSAddrPoint& obj )
	{
		obj.CopyTo( *this );
		m_strAddrName = obj.m_strAddrName;
		return *this;
	}
	CCSAddrPoint& operator=( _IRow* lp )
	{
		SetObject( lp, sindyTableType::cs_addr_point );
		return *this;
	}

	/**
	 * @brief	<b>�Z���R�[�h�擾</b>
  	 * @param	bChar			[in, optional]	�����t���Z���l������H(true�ɂ���ƁA�����t���Z���l���������ʂ��Ԃ�܂�)
	 * @param	bCodeDBSaved	[in, optional]	�����t���Z���R�[�h�ϊ�DB�ɑ��݂��Ȗ��̂̂Ƃ��ۑ�������ŃR�[�h��V�K���s����H(true�ɂ���ƁA�R�[�h�ϊ�DB�ɍX�V����������܂�)
	 * @note	CS�|�C���g�ł́A��L�����͑S�Ďg�p���܂���
	 * @return	�Z���R�[�h
	 */
	CString GetAddrCode( bool bChar = false, bool bCodeDBSaved = false )
	{
		CComVariant vaValue;
		return (sindyErr_NoErr == GetValueByFieldName(cs_addr_point::kAddrCode, vaValue) && VT_BSTR == vaValue.vt)?
				CString(vaValue.bstrVal) : _T("");
	}

	/**
	 * @brief <b>�L���b�V�����폜����</b>\n
	 * CRow::DeleteCache���I�[�o���[�h
	 */
	void DeleteCache( bool bDeleteContainerCache = false )
	{
		m_strAddrName = _T("");

		// �R���e�i�̃L���b�V�����폜
		if( bDeleteContainerCache )
			CContainer::DeleteCache( bDeleteContainerCache );
	}

private:

	mutable CString	m_strAddrName;	//!< �L���b�V���p

};

}	// namespace addr
