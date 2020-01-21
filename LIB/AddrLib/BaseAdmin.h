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
using namespace sindy::errorcode;
using namespace sindy::schema;

/**
 * @class	CBaseAdmin
 * @brief	���k�s���E�N���X
 */
class CBaseAdmin : public CFeature
{
	ROWCLASS_CONSTRUCTOR( CBaseAdmin, base_admin )

	/**
	 * @brief	<b>�s�撬���R�[�h�擾</b>
	 * @return	�s�撬���R�[�h
	 */
	CString GetCityCode() const
	{
		CComVariant vaValue;
		return (sindyErr_NoErr == GetValueByFieldName(base_admin::kCityCode, vaValue) && VT_BSTR == vaValue.vt)?
				CString(vaValue.bstrVal) : _T("");
	}

	/**
	 * @brief	<b>�s�撬�����擾</b>
	 * @note	�� Initializer.h �� Init_AreaCodeHelper�֐����Ăяo���Ă���g�p���邱�ƁI
	 * @return	�s�撬�����擾
	 */
	CString GetCityName() const;

	/**
	 * @brief <b>�L���b�V�����폜����</b>\n
	 * CRow::DeleteCache���I�[�o���[�h
	 */
	void DeleteCache( bool bDeleteContainerCache = false )
	{
		m_strCityName = _T("");
	}

private:

	mutable	CString m_strCityName;	//!< �s�撬������(�L���b�V���p)
};

} // namespace addr
