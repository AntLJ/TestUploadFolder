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
 * @brief	都市建物管理クラス
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
	 * @brief	<b>建物種別取得</b>
	 * @return	建物種別
	 */
	building::bld_class::ECode GetBldClass() const{ return static_cast<building::bld_class::ECode>(GetLongValueByFieldName(building::kBldClass, 0)); }

	/**
	 * @brief	<b>代表住所コード取得</b>
	 * @note	少し優先順位変更する可能性あるので、保留
	 * @param	ipAddrWorkspace	[in, optional]	住所系ワークスペース(NULLなら、建物と同一ワークスペースと判断)
	 * @return	代表住所コード
	 */
	CString GetRepAddrCode( IWorkspace* ipAddrWorkspace = NULL ) const;

	/**
	 * @brief	<b>面積取得</b>
	 * @note	IArea::get_Area を呼び出してるだけ
	 * @return	面積
	 */
	double GetArea();

private:

	mutable	CString	m_strRepAddrCode;
};

}	// namespace addr
