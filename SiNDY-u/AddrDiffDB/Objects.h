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

#ifndef __OBJECTS_H__
#define __OBJECTS_H__

/**
 * @file	Objects.h
 */

#include <string>

/**
 * @class	CFeature
 * @brief	�t�B�[�`���Ǘ��N���X
 */
class CFeature
{
public:
	CFeature( IFeature* ipFeature ) : m_ipFeature(ipFeature)
	{
	};

	virtual ~CFeature(){};

	IFeaturePtr getFeature() const { return m_ipFeature; }

	IGeometryPtr getGeo() const
	{
		IGeometryPtr ipGeo;
		m_ipFeature->get_Shape( &ipGeo );
		return ipGeo; 
	};

	LONG getOID() const
	{
		LONG lOID;
		m_ipFeature->get_OID(&lOID);
		return lOID;
	}
	
	CComVariant getValue( LPCTSTR lpszField ) const;
	CComVariant getValue( LONG lFID ) const;

	virtual void setInfo() = 0;	/// �K���p����Ŏ�������

protected:

	IFeaturePtr m_ipFeature;		/// �t�B�[�`��
};

/**
 * @class	CCityAdminFID
 * @brief	�s�s�n�}�s���E�t�B�[���h�h�c�S
 */
class CCityAdminFID
{
public:
	CCityAdminFID(){};
	
	void set( ITablePtr ipTable )
	{
		ipTable->FindField( CComBSTR(city_admin::kCityCode), &m_lCityCode_FID );
		ipTable->FindField( CComBSTR(city_admin::kAddrCode), &m_lAddrCode_FID );
		ipTable->FindField( CComBSTR(city_admin::kGaikuFugo), &m_lGaikuFugo_FID );
		ipTable->FindField( CComBSTR(city_admin::kGaikuFugo), &m_lAddrClass_FID );
	}

	long	m_lCityCode_FID;		/// �s�s�R�[�h�t�B�[���h�C���f�b�N�X
	long	m_lAddrCode_FID;		/// �Z���R�[�h�t�B�[���h�C���f�b�N�X
	long	m_lGaikuFugo_FID;		/// �X�敄���t�B�[���h�C���f�b�N�X
	long	m_lAddrClass_FID;		//!< �Z��������ʃt�B�[���h�C���f�b�N�X
};

/**
 * @class	CCityAdmin
 * @brief	�s�s�n�}�s���E�N���X
 */
class CCityAdmin : public CFeature
{
public:
	CCityAdmin( IFeature* ipFeature, const CCityAdminFID& rFID ) : 
	  CFeature(ipFeature),
	  m_rFID(rFID),
	  m_lGaikuFugo(0)
	{
		setInfo();	/// �ǂ��g����͐�Ɏ擾���Ă���
	};
	
	~CCityAdmin(){};

	/**
	 * @brief	�Z���R�[�h�擾
	 * @note	�͈͊O���w�肵���Ƃ��́A�S���Ԃ�
	 * @param	nLevel	[in]	����
	 * @return	string
	 */
	const CString getAddrCode ( int nLevel ) const
	{
		return ( m_strAddrCode.Left(nLevel) );
	}

private:

	/**
	 * @brief	�����o�ϐ��ɃZ�b�g
	 */
	void setInfo();

public:

	CString		m_strAddrCode;		//!< �Z���R�[�h
	long		m_lGaikuFugo;		//!< �X�敄��
	long		m_lAddrClass;		//!< �Z���������

private:

	const CCityAdminFID&	m_rFID;
};

/**
 * @class	CGouPointFID
 * @brief	���|�C���g�t�B�[���h�h�c�S
 */
class CGouPointFID
{
public:
	CGouPointFID(){};
	
	void set( ITablePtr ipTable )
	{
		ipTable->FindField( CComBSTR(sindy::schema::gou_point::kGouNo), &m_lGouNo_FID );
		ipTable->FindField( CComBSTR(sindy::schema::gou_point::kExpGouNo), &m_lExpGouNo_FID );
	}

	LONG	m_lGouNo_FID;		/// ���|�C���g�t�B�[���h�C���f�b�N�X
	LONG	m_lExpGouNo_FID;	/// �g�����t���O�t�B�[���h�C���f�b�N�X
};

/**
 * @class	CGouPoint
 * @brief	���|�C���g
 */
class CGouPoint : public CFeature
{
public:
	CGouPoint( IFeature* ipFeature, const CGouPointFID& rFID ) : 
	  CFeature(ipFeature),
	  m_rFID(rFID),
	  m_lExGouNoF(0),
	  m_lMeshCode(0)
	  {
		  setInfo();
	  };

	  ~CGouPoint(){};

private:

	/**
	 * @brief	�����o�ϐ��ɃZ�b�g
	 */
	void setInfo();

public:

	CString		m_strGouNo;			/// ���ԍ�
	LONG		m_lExGouNoF;		/// �g�����ԍ��t���O

	LONG		m_lMeshCode;		/// ���|�C���g�����郁�b�V���R�[�h(�O���[�v�����[�h�̂Ƃ��g�p)

private:

	const CGouPointFID&	m_rFID;
};

#endif // !defined(AFX_CITYADMIN_H__C2F259F5_5399_48AA_A610_5A3F4E64E0BC__INCLUDED_)
