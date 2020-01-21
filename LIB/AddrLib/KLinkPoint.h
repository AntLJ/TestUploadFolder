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
 * @class	CKLinkPoint
 * @brief	�ƌ`�����N�|�C���g�Ǘ��N���X
 */
class CKLinkPoint : public CAddrBasePoint
{
public:
	CKLinkPoint()
	{
		Init();
		m_emTableType = sindyTableType::klink_point;
	}
	virtual ~CKLinkPoint(){ DeleteCache(); }
	CKLinkPoint( const CKLinkPoint& obj )
	{
		Init();
		*this = obj;
	}
	CKLinkPoint( _IRow* lp, sindyTableType::ECode emTableType = sindyTableType::klink_point, bool bCreated = false, const CSPFieldMap& spFields = CSPFieldMap(), const CSPTableNameString& spNameString = CSPTableNameString() )
	{
		Init();
		SetObject( lp, emTableType, bCreated, spFields, spNameString );
	}
	CKLinkPoint& operator=( const CKLinkPoint& obj )
	{
		obj.CopyTo( *this );
		m_strAddrName = obj.m_strAddrName;
		return *this;
	}
	CKLinkPoint& operator=( _IRow* lp )
	{
		SetObject( lp, sindyTableType::klink_point );
		return *this;
	}

	/**
	 * @brief	<b>����22���Z���R�[�h�擾</b>
	 * @return	����22���Z���R�[�h
	 */
	CString GetAddrCodeOrg() const
	{
		CComVariant vaValue;
		return (sindyErr_NoErr == GetValueByFieldName(klink_point::kAddrCodeOrg, vaValue) && VT_BSTR == vaValue.vt)?
				CString(vaValue.bstrVal) : _T("");
	}

	/**
	 * @brief	<b>�ŐV22���Z���R�[�h�擾</b>
   	 * @param	bChar			[in, optional]	�����t���Z���l������H(true�ɂ���ƁA�����t���Z���l���������ʂ��Ԃ�܂�)
	 * @param	bCodeDBSaved	[in, optional]	�����t���Z���R�[�h�ϊ�DB�ɑ��݂��Ȗ��̂̂Ƃ��ۑ�������ŃR�[�h��V�K���s����H(true�ɂ���ƁA�R�[�h�ϊ�DB�ɍX�V����������܂�)
	 * @note	�ƌ`�����N�|�C���g�ł́A��L�����͑S�Ďg�p���܂���
	 * @return	�ŐV22���Z���R�[�h
	 */
	CString GetAddrCode( bool bChar = false, bool bCodeDBSaved = false )
	{
		CComVariant vaValue;
		return (sindyErr_NoErr == GetValueByFieldName(klink_point::kAddrCode, vaValue) && VT_BSTR == vaValue.vt)?
				CString(vaValue.bstrVal) : _T("");
	}

	/**
	 * @brief	<b>�^�E���y�[�W�������̎擾</b>
	 * @return	�^�E���y�[�W��������
	 */
	CString GetKatagakiName() const
	{
		CComVariant vaValue;
		return (sindyErr_NoErr == GetValueByFieldName(klink_point::kKatagakiName, vaValue) && VT_BSTR == vaValue.vt)?
				CString(vaValue.bstrVal) : _T("");
	}

	/**
	 * @brief	<b>�^�E���y�[�W�f�ږ��̎擾</b>
	 * @return	�^�E���y�[�W�f�ږ���
	 */
	CString GetKeisaiName() const
	{
		CComVariant vaValue;
		return (sindyErr_NoErr == GetValueByFieldName(klink_point::kKeisaiName, vaValue) && VT_BSTR == vaValue.vt)?
				CString(vaValue.bstrVal) : _T("");
	}

	/**
	 * @brief	<b>���񋟌��R�[�h�擾</b>
	 * @return	���񋟌��R�[�h
	 */
	klink_point::info_src::ECode GetInfoSrc() const{	return static_cast<klink_point::info_src::ECode>(GetLongValueByFieldName(klink_point::kInfoSrc, 0)); }

	/**
	 * @brief	<b>���͗p���W���x�R�[�h�擾</b>
	 * @return	���͗p���W���x�����R�[�h
	 */
	klink_point::input_seido::ECode GetInputSeido() const{	return static_cast<klink_point::input_seido::ECode>(GetLongValueByFieldName(klink_point::kInputSeido, 0)); }

	/**
	 * @brief	<b>�����[�X�p���W���x�R�[�h�擾</b>
	 * @return	�����[�X�p���W���x�����R�[�h
	 */
	klink_point::release_seido::ECode GetReleaseSeido() const{	return static_cast<klink_point::release_seido::ECode>(GetLongValueByFieldName(klink_point::kReleaseSeido, 0));	}

	/**
	 * @brief	<b>�r�����̎擾</b>
	 * @return	�r������
	 */
	CString GetBuildName() const
	{
		CComVariant vaValue;
		return (sindyErr_NoErr == GetValueByFieldName(klink_point::kBuildName, vaValue) && VT_BSTR == vaValue.vt)?
				CString(vaValue.bstrVal) : _T("");
	}

	/**
	 * @brief	<b>�r�����񋟌��R�[�h�擾</b>
	 * @return	�r�����񋟌��R�[�h
	 */
	klink_point::buildinfo_src::ECode GetBuildInfoSrc() const{	return static_cast<klink_point::buildinfo_src::ECode>(GetLongValueByFieldName(klink_point::kBuildInfoSrc, 0)); }

	/**
	 * @brief	<b>�r���ŏ�K���擾</b>
	 * @return	�r���ŏ�K��
	 */
	long GetMaxFloors() const{	return GetLongValueByFieldName(klink_point::kMaxFloors, 0); }

	/**
	 * @brief	<b>�e�i���g�K���擾</b>
	 * @return	�e�i���g�K��
	 */
	long GetFloor() const{	return GetLongValueByFieldName(klink_point::kFloor, 0); }

	// 11���s��v�t���O�́A�����e����Ă��Ȃ��̂Ŏg�p�s��(����Ǝ��Ŋm�F���Ă���)
			
};

}	// namespace addr
