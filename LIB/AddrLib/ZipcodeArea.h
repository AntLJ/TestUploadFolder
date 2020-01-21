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
	class CZipcodeArea;
	typedef boost::shared_ptr<CZipcodeArea> CSPZipcodeArea;

	class CZipcodeArea : public sindy::CFeature
	{
	public:
		CZipcodeArea()
		{
			Init();
			m_emTableType = sindy::sindyTableType::zipcode_area;
		}

		virtual ~CZipcodeArea()
		{
			DeleteCache();
		}

		CZipcodeArea(const CZipcodeArea& obj)
		{
			Init();
			*this = obj;
		}

		CZipcodeArea(
			_IRow* lp,
			sindy::sindyTableType::ECode emTableType = sindy::sindyTableType::zipcode_area,
			bool created = false,
			const sindy::CSPFieldMap& spFields = sindy::CSPFieldMap(),
			const sindy::CSPTableNameString& spNameString = sindy::CSPTableNameString())
		{
			Init();
			SetObject(lp, emTableType, created, spFields, spNameString);
		}

		CZipcodeArea& operator=(const CZipcodeArea& obj)
		{
			obj.CopyTo(*this);
			m_zipCode1 = obj.m_zipCode1;
			m_zipCode2 = obj.m_zipCode2;
			return *this;
		}

		CZipcodeArea& operator=(_IRow* lp)
		{
			SetObject(lp, sindy::sindyTableType::zipcode_area);
			return *this;
		}

		/**
		 * @brief óXï÷î‘çÜéÊìæ
		 * @return óXï÷î‘çÜ
		 */
		CString GetZipcode1() const
		{
			if (!m_zipCode1.IsEmpty())
				return m_zipCode1;

			CComVariant value;
			auto errorCode = GetValueByFieldName(sindy::schema::zipcode_area::kZipCode1, value);
			return (sindy::errorcode::sindyErr_NoErr == errorCode && VT_BSTR == value.vt) ? CString(value.bstrVal) : CString();
		}

		/**
		 * @brief óXï÷î‘çÜéÊìæ
		 * @return óXï÷î‘çÜ
		 */
		CString GetZipcode2() const
		{
			if (!m_zipCode2.IsEmpty())
				return m_zipCode2;

			CComVariant value;
			auto errorCode = GetValueByFieldName(sindy::schema::zipcode_area::kZipCode2, value);
			return (sindy::errorcode::sindyErr_NoErr == errorCode && VT_BSTR == value.vt) ? CString(value.bstrVal) : CString();
		}


	protected:
		mutable CString m_zipCode1; //!< óXï÷î‘çÜ1(ÉLÉÉÉbÉVÉÖ)
		mutable CString m_zipCode2; //!< óXï÷î‘çÜ2(ÉLÉÉÉbÉVÉÖ)

	};
} // namespace addr
