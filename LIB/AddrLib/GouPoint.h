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
#include "Building.h"
#include "enum_def.h"

namespace addr
{
using namespace sindy;
using namespace sindy::schema;

typedef std::list<CString> GOUTOKENLIST;

class CGouPoint;
typedef boost::shared_ptr<CGouPoint> CSPGouPoint;

/**
 * @class	CGouPoint
 * @brief	���|�C���g�Ǘ��N���X
 */
class CGouPoint : public CAddrBasePoint
{
public:
	explicit CGouPoint()
	{
		Init();
		m_emTableType = sindyTableType::gou_point;
	}
	virtual ~CGouPoint(){ DeleteCache(); }

	CGouPoint( sindyTableType::ECode emTableType, IFeature* lp )
	{
		SetObject( lp, emTableType );
		SetGouNo();
	}
	CGouPoint( sindyTableType::ECode emTableType, _IRow*lp )
	{
		SetObject( lp, emTableType );
		SetGouNo();
	}
	CGouPoint( _IRow* lp, sindyTableType::ECode emTableType = sindyTableType::gou_point, bool bCreated = false, const CSPFieldMap& spFields = CSPFieldMap(), const CSPTableNameString& spNameString = CSPTableNameString() )
	{
		SetObject( lp, emTableType, bCreated, spFields, spNameString );
		SetGouNo();
	}
	explicit CGouPoint( const CGouPoint& obj )
	{
		*this = obj;
		m_strAddrCode = obj.m_strAddrCode;
		m_strAddrName = obj.m_strAddrName;
		SetGouNo();
		
	}
	CGouPoint& operator=( const CGouPoint& obj )
	{
		obj.CopyTo( *this );
		m_strAddrCode = obj.m_strAddrCode;
		m_strAddrName = obj.m_strAddrName;
		SetGouNo();
		return *this;
	}

public:
	
	/**
	 * @brief	<b>���ԍ��Z�b�g</b>
	 * @note	��{�I�ɃR���X�g���N�^�ŏ���������邪�A
	 *			�f�t�H���g�R���X�g���N�^�Ő錾���͕K���ŏ��ɌĂяo���I
	 */
	void SetGouNo()
	{
		CComVariant vaValue;
		m_strGouNo = (sindyErr_NoErr == GetValueByFieldName(gou_point::kGouNo, vaValue) && VT_BSTR == vaValue.vt)? 
					  CString(vaValue.bstrVal) : _T("");

		Tokenize( m_strGouNo, L"-", m_listTokenName );	// �擾�������ԍ����g�[�N������
	}

	/**
	 * @brief	<b>���ԍ��擾</b>
	 * @param	iStart		[in]	�X�^�[�g�ʒu(-1�Ȃ�S�Ď擾)
	 * @param	iEnd		[in]	�I���ʒu(-1�Ȃ�A�X�^�[�g����S�Ă��擾)
	 * @return	���ԍ�
	 */
	CString GetGouNo( int iStart = -1, int iEnd = -1 )
	{
		if( m_strGouNo.IsEmpty() )
			SetGouNo();
		return (-1 == iStart)? m_strGouNo : GetToken(m_listTokenName, iStart, iEnd);
	}

	/**
	 * @brief	<b>���ԍ��g�[�N�����擾</b>
	 * @return	�g�[�N����
	 */
	long GetTokenCount() const{ return (signed)m_listTokenName.size(); }

	/**
	 * @brief	<b>���^�C�v�擾</b>
	 * @return	long
	 */
	long GetGouType() const{ return GetLongValueByFieldName( gou_point::kGouType, false ); }

	/**
	 * @brief	<b>�D�捆�H</b>
	 * @return	bool
	 */
	bool IsPrior() const{ return GetBoolValueByFieldName( gou_point::kPriority, false ); }

	/**
	 * @brief	<b>�g�����ԍ��H</b>
	 * @return	bool
	 */
	bool IsExGouNo() const{ return GetBoolValueByFieldName( gou_point::kExpGouNo, false ); }

	/**
	 * @brief	<b>�Z����v����</b>
	 * @return	�Z����v����
	 */
	CString GetPrecisionCode() const
	{
		// �ǂ��������`�ŕԂ��̂��͌�ق�
		CComVariant vaValue;
		return (sindyErr_NoErr == GetValueByFieldName(gou_point::kPrecisionCode, vaValue) && VT_BSTR == vaValue.vt)?
				CString(vaValue.bstrVal) : _T("");		
	}

	/**
	 * @brief	<b>�Z���m�F�X�e�[�^�X</b>
	 * @return	�m�F�t���O
	 */
	gou_point::	confirm::ECode GetConfirm() const
	{
		return static_cast<gou_point::confirm::ECode>(GetLongValueByFieldName(gou_point::kConfirm, 0));
	}

	/**
	 * @brief	<b>20���Z���R�[�h�擾</b>
	 * @param	bChar			[in, optional]	�����t���Z���l������H(true�ɂ���ƁA�����t���Z���l���������ʂ��Ԃ�܂�)
	 * @param	bCodeDBSaved	[in, optional]	�����t���Z���R�[�h�ϊ�DB�ɑ��݂��Ȗ��̂̂Ƃ��ۑ�������ŃR�[�h��V�K���s����H(true�ɂ���ƁA�R�[�h�ϊ�DB�ɍX�V����������܂�)
	 * @return	20���Z���R�[�h
	 */
	CString GetAddrCode( bool bChar = false, bool bCodeDBSaved = false );

	/**
	 * @brief	<b>�Z�����̎擾</b>
	 * @param	bChar			[in, optional]	�����t���Z���l������H(true�ɂ���ƁA�����t���Z���l���������ʂ��Ԃ�܂�)
	 * @return	�Z������
	 */
	CString GetAddrName( bool bChar = false );

	/**
	 * @brief	<b>���|�C���g�������錚���擾</b>
	 * @param	ipBldClass	[in, optional]	�s�s�����e�[�u��(NULL���́A���|�C���g�Ɠ��ꃏ�[�N�X�y�[�X���猟��)
	 * @return	�s�s����
	 */
	CBuilding* GetBuilding( IFeatureClassPtr ipBldClass = NULL );

	/**
	 * @brief <b>�L���b�V�����폜����</b>\n
	 * CRow::DeleteCache���I�[�o���[�h
	 */
	void DeleteCache( bool bDeleteContainerCache = false )
	{
		CRow::DeleteCache( bDeleteContainerCache );
		m_strGouNo = _T("");
		m_strAddrCode = _T("");
		m_strAddrName = _T("");
		m_listTokenName.clear();
		m_listTokenYomi.clear();
	}

private:

	/**
	 * @brief	<b>�g�[�N�������֐�</b>
	 * @param	strData		[in]		�f�[�^
	 * @param	lcpszSep	[in]		�Z�p���[�^
	 * @param	rList		[in,out]	�i�[�p�R���e�i
	 */
	void Tokenize( const CString& strData, const wchar_t* lpcszSep, GOUTOKENLIST& rList );

	/**
	 * @brief	<b>�g�[�N���擾�֐�</b>
	 * @param	listToken	[in]	�g�[�N�����X�g
	 * @param	iStart		[in]	�X�^�[�g�ʒu
	 * @param	iEnd		[in]	�I���ʒu(-1)�Ȃ�A�X�^�[�g����S�Ă��擾
	 */
	CString GetToken ( const GOUTOKENLIST& listToken, int iStart, int iEnd = -1 ) const;

	/**
	 * @brief	<b>�����t���Z���H</b>
	 * @return	�X�e�[�^�X
	 */
	addrStatus IsCharAddr();

	/**
	 * @brief	<b>20���R�[�h����(�����l�����Ȃ�)</b>
	 * @return	20���R�[�h
	 */
	CString GetAddrCode_NoChar();

	/**
	 * @brief	<b>20���R�[�h����(�����l��)</b>
	 * @param	bCodeDBSaved	[in, optional]	�����t���Z���R�[�h�ϊ�DB�ɑ��݂��Ȗ��̂̂Ƃ��ۑ�������ŃR�[�h��V�K���s����H(true�ɂ���ƁA�R�[�h�ϊ�DB�ɍX�V����������܂�)
	 * @return	20���R�[�h
	 */
	CString GetAddrCode_Char( bool bCodeDBSaved = false );
	
private:

	CString			m_strGouNo;			//!< ���ԍ�(�L���b�V���p)
	mutable	CString	m_strAddrCode;		//!< 20���Z���R�[�h(�L���b�V���p)
	mutable CString	m_strAddrName;		//!< �Z������(�L���b�V���p)

	GOUTOKENLIST	m_listTokenName;
	GOUTOKENLIST	m_listTokenYomi;
};

}	// namespace addr
