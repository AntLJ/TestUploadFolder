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

class CCityAdmin;
typedef boost::shared_ptr<CCityAdmin> CSPCityAdmin;

/**
 * @class	CCityAdmin
 * @brief	�s�s�n�}�s���E�N���X
 */
class CCityAdmin : public CFeature
{
public:
	CCityAdmin()
	{
		Init();
		m_emTableType = sindyTableType::city_admin;
	}
	virtual ~CCityAdmin(){ DeleteCache(); }
	CCityAdmin( const CCityAdmin& obj )
	{
		Init();
		*this = obj;
	}
	CCityAdmin( _IRow* lp, sindyTableType::ECode emTableType = sindyTableType::city_admin, bool bCreated = false, const CSPFieldMap& spFields = CSPFieldMap(), const CSPTableNameString& spNameString = CSPTableNameString() )
	{
		Init();
		SetObject( lp, emTableType, bCreated, spFields, spNameString );
	}
	CCityAdmin& operator=( const CCityAdmin& obj )
	{
		obj.CopyTo( *this );
		m_strAdminCode = obj.m_strAdminCode;
		m_strAddrName = obj.m_strAddrName;
		return *this;
	}
	CCityAdmin& operator=( _IRow* lp )
	{
		SetObject( lp, sindyTableType::city_admin );
		return *this;
	}	

	/**
	 * @brief	<b>�s�撬���R�[�h�擾</b>
	 * @return	�s�撬���R�[�h
	 */
	CString GetCityCode() const
	{
		CComVariant vaValue;
		return (sindyErr_NoErr == GetValueByFieldName(city_admin::kCityCode, vaValue) && VT_BSTR == vaValue.vt)?
				CString(vaValue.bstrVal) : _T("");
	}

	/**
	 * @brief	<b>�厚�E���R�[�h</b>
	 * @return	�厚�E���R�[�h
	 */
	CString GetOazaAzaCode() const
	{
		CComVariant vaValue;
		return (sindyErr_NoErr == GetValueByFieldName(city_admin::kAddrCode, vaValue) && VT_BSTR == vaValue.vt)?
				CString(vaValue.bstrVal) : _T("");
	}

	/**
	 * @brief	<b>�厚�E���R�[�h2</b>
	 * @note	Bug8041
	 * @return	�厚�E���R�[�h
	 */
	CString GetOazaAzaCode2() const
	{
		CComVariant vaValue;
		return (sindyErr_NoErr == GetValueByFieldName(city_admin::kAddrCode2, vaValue) && VT_BSTR == vaValue.vt)?
				CString(vaValue.bstrVal) : _T("");
	}

	/**
	 * @brief	<b>�X�敄���擾</b>
	 * @return	�X�敄��
	 */
	long GetGaikuFugo() const
	{
		return GetLongValueByFieldName(city_admin::kGaikuFugo, 0);
	}

	/**
	 * @brief	<b>�g���X�敄���擾</b>
	 * @return	�g���X�敄��
	 */
	CString GetExGaikuFugo() const
	{
		CComVariant vaValue;
		return (sindyErr_NoErr == GetValueByFieldName(city_admin::kExtGaikuFugo, vaValue) && VT_BSTR == vaValue.vt)?
				CString(vaValue.bstrVal) : _T("");
	}

	/**
	 * @brief	<b>�g���X�敄�����~�擾</b>
	 * @return	�g���X�敄�����~
	 */
	CString GetExGaikuFugoYomi() const;

	/**
	 * @brief	<b>�Z��������ʎ擾</b>
	 * @return	�Z���������
	 */
	city_admin::addr_class::ECode GetAddrClass() const
	{
		return static_cast<city_admin::addr_class::ECode>(GetLongValueByFieldName(city_admin::kAddrClass, 0));
	}

	/**
	 * @brief	<b>�G���A���ʃR�[�h�擾</b>
	 * @return	�G���A���ʃR�[�h
	 */
	city_admin::area_class::ECode GetAreaClass() const
	{
		return static_cast<city_admin::area_class::ECode>(GetLongValueByFieldName(city_admin::kAreaClass, 0));
	}

	/**
	 * @brief	<b>�Z���ϊ����[�h�擾</b>
	 * @return	�Z���ϊ����[�h
	 */
	city_admin::conv_type::ECode GetConvType() const
	{
		return static_cast<city_admin::conv_type::ECode>(GetLongValueByFieldName(city_admin::kConvType, 0));
	}

	/**
	 * @brief	<b>�Z���R�[�h�擾</b>
	 * @note	�s���E�ŕ\���ł������̏Z���R�[�h��Ԃ�
	 * @param	bChar	[in, optional]	�����t���Z���l������H(true�ɂ���ƁA�����t���Z���l���������ʂ��Ԃ�܂�)
	 * @param	bCodeDBSaved	[in, optional]	�����t���Z���R�[�h�ϊ�DB�ɑ��݂��Ȗ��̂̂Ƃ��ۑ�������ŃR�[�h��V�K���s����H(true�ɂ���ƁA�R�[�h�ϊ�DB�ɍX�V����������܂�)
	 * @return	�Z���R�[�h
	 */
	CString GetAddrCode( bool bChar = false, bool bCodeDBSaved = false ) const;

	/**
	 * @brief	<b>�Z���R�[�h2�擾</b>
	 * @note	�s���E�ŕ\���ł������̏Z���R�[�h(�ʏ̏Z��)��Ԃ�
	 * @param	bChar	[in, optional]	�����t���Z���l������H(true�ɂ���ƁA�����t���Z���l���������ʂ��Ԃ�܂�)
	 * @param	bCodeDBSaved	[in, optional]	�����t���Z���R�[�h�ϊ�DB�ɑ��݂��Ȗ��̂̂Ƃ��ۑ�������ŃR�[�h��V�K���s����H(true�ɂ���ƁA�R�[�h�ϊ�DB�ɍX�V����������܂�)
	 * @return	�Z���R�[�h
	 */
	CString GetAddrCode2( bool bChar = false, bool bCodeDBSaved = false ) const;

	/**
	 * @brief	<b>�Z�����擾</b>
	 * @note	�s���E���\���ł��錅���x���ł̏Z������Ԃ�
	 * @return	�Z����
	 */
	CString GetAddrName() const;

	/**
	 * @brief	<b>�Z�����擾</b>
	 * @note	�s���E���\���ł��錅���x���ł̏Z����(�ʏ̏Z��)��Ԃ�
	 * @return	�Z����
	 */
	CString GetAddrName2() const;

	/**
	 * @brief <b>�L���b�V�����폜����</b>\n
	 * CRow::DeleteCache���I�[�o���[�h
	 */
	void DeleteCache( bool bDeleteContainerCache = false )
	{
		m_strAdminCode = _T("");
		m_strAddrName = _T("");
	}

private:
	/**
	 * @brief <b>11���R�[�h�ɒn�ԃR�[�h��ǉ�����</b>
	 * @note  �s���E�ŕ\���ł������̏Z���R�[�h(�ʏ̏Z��)��Ԃ�
	 * @param strAdminCode [in, out] �����Ώۂ�11���R�[�h
	 * @param bChar	[in, optional] �����t���Z���l������H
	          (true�ɂ���ƁA�����t���Z���l���������ʂ��Ԃ�܂�)
	 * @param bCodeDBSaved	[in, optional]
	          �����t���Z���R�[�h�ϊ�DB�ɑ��݂��Ȃ����̂̂Ƃ��ۑ��������
	          �R�[�h��V�K���s����H(true�ɂ���ƁA�R�[�h�ϊ�DB�ɍX�V����������)
	 */
	void AppendPntCode(
		CString& strAdminCode,
		bool bChar = false,
		bool bCodeDBSaved = false ) const;

protected:

	mutable CString	m_strAdminCode;		//!< �s���E�R�[�h(�L���b�V���p)
	mutable CString	m_strAdminCode2;		//!< �s���E�R�[�h(�ʏ̏Z��)(�L���b�V���p)
	mutable CString	m_strAddrName;		//!< �Z������(�L���b�V���p)
	mutable CString	m_strAddrName2;		//!< �Z������(�ʏ̏Z��)(�L���b�V���p)
};

} // namespace addr
