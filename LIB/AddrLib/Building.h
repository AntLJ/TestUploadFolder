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

#include <SiNDYLib/Feature.h>

namespace addr
{
using namespace sindy;
using namespace sindy::schema;

class CBuilding;
typedef boost::shared_ptr<CBuilding> CSPBuilding;

/**
 * @class	CBuilding
 * @brief	�s�s�����Ǘ��N���X
 */
class CBuilding : public CFeature
{
public:
	CBuilding()
	{
		Init();
		m_emTableType = sindyTableType::building;
	}
	virtual ~CBuilding(){ DeleteCache(); }
	CBuilding( const CBuilding& obj )
	{
		Init();
		*this = obj;
	}
	CBuilding( _IRow* lp, sindyTableType::ECode emTableType = sindyTableType::building, bool bCreated = false, const CSPFieldMap& spFields = CSPFieldMap(), const CSPTableNameString& spNameString = CSPTableNameString() )
	{
		Init();
		SetObject( lp, emTableType, bCreated, spFields, spNameString );
	}
	CBuilding& operator=( const CBuilding& obj )
	{
		obj.CopyTo( *this );
		m_strRepAddrCode = obj.m_strRepAddrCode;
		return *this;
	}
	CBuilding& operator=( _IRow* lp )
	{
		SetObject( lp, sindyTableType::building );
		return *this;
	}

	/**
	 * @brief	<b>������ʎ擾</b>
	 * @return	�������
	 */
	building::bld_class::ECode GetBldClass() const{ return static_cast<building::bld_class::ECode>(GetLongValueByFieldName(building::kBldClass, 0)); }

	/**
	 * @brief	<b>��\�Z���R�[�h�擾</b>
	 * @note	�����D�揇�ʕύX����\������̂ŁA�ۗ�
	 * @param	ipAddrWorkspace	[in, optional]	�Z���n���[�N�X�y�[�X(NULL�Ȃ�A�����Ɠ��ꃏ�[�N�X�y�[�X�Ɣ��f)
	 * @return	��\�Z���R�[�h
	 */
	CString GetRepAddrCode( IWorkspace* ipAddrWorkspace = NULL ) const;

	/**
	 * @brief	<b>�ʐώ擾</b>
	 * @note	IArea::get_Area ���Ăяo���Ă邾��
	 * @return	�ʐ�
	 */
	double GetArea();

private:

	mutable	CString	m_strRepAddrCode;
};

}	// namespace addr
