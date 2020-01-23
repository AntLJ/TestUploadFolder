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

// �������ʃR�[�h
typedef enum ePointStatC
{
	NOSURVEY = 0,		/// ������
	UNKNOWN_POS,		/// �ʒu�s��
	GAITOU_SISETU,		/// �Y���{�݂���
	HORYU				/// �ۗ�
} POINTSTAT_C;

// �Z���������
typedef enum eAddrClassC
{
	OAZA_NOTSEIBI = 0,		/// �����ڊE������
	JUKYO_SEIBI,			/// �Z���\�������ς�
	TIBAN_SEIBI,			/// �n�Ԑ����ς�
	JUKYO_TIBAN_NOTSEIBI	/// �Z���\���E�n�Ԗ�����
} ADDRCLASS_C;

// ���W�t�^���
typedef enum eAddXYC
{
	NORMAL = 0,			/// �ʏ�
	CLAIM,				/// �N���[��
	SAME_ADDR,			/// ����Z��
	GUIDE_P_GEN,		/// �i�ؗU���|�C���g�i��ʓ��H�j
	GUIDE_P_HWY,		///	�i�ؗU���|�C���g�i�������H�j
	PINPOINT_TWN,		/// TWN�s���|�C���g��������i�����ڍs�j
	PINPOINT_HLP		/// HLP�s���|�C���g��������i�����ڍs�j
} ADDXY_C;

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

	IGeometryPtr getGeo() const {
		IGeometryPtr ipGeo;
		m_ipFeature->get_Shape(&ipGeo);
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

	/**
	 * @brief	�K�v���Z�b�g
	 * @note	���Ɏ擾���Ă���������񓙂��Z�b�g����֐�
	 */
	virtual void setInfo() = 0;


protected:

	IFeaturePtr m_ipFeature;		/// �t�B�[�`��
	//IGeometryPtr m_ipGeo;			/// �W�I���g��
};

/**
 * @class	CTelTP
 * @brief	�g���d�b�ԍ������|�C���g�N���X
 */
class CTelTP : public CFeature
{
public:
	CTelTP( IFeature* ipFeature ) : 
	  CFeature(ipFeature)
	{
		//DistToPOI = 0;
		setInfo();	/// �ǂ��g����͐�Ɏ擾���Ă���
	}

	~CTelTP(){};

	/**
	 * @brief	�Z���R�[�h�擾
	 * @note	�͈͊O���w�肵���Ƃ��́A�S���Ԃ�
	 * @param	nLevel	[in]	����
	 * @return	string
	 */
	const std::string getAddrCode ( int nLevel ) const
	{
		return ( m_strAddrCode.substr(0, nLevel) );
	}

	/**
	 * @brief	�������ʃh���C���擾
	 * @return	string
	 */
	const std::string getPointStatDomain() const;

	/**
	 * @brief	���W�t�^��ʃh���C���擾
	 * @return	string
	 */
	const std::string getAddXYDomain() const;

/*
	void SetDistToPOI(double dist)
	{
		DistToPOI = dist;
	}
*/
private:
	
	/**
	 * @brief	�����o�ϐ��ɃZ�b�g
	 */
	void setInfo();

public:

	std::string	m_strAddrCode;		/// �Z���R�[�h
	POINTSTAT_C	m_ePointStatC;		/// �������ʃR�[�h
	ADDXY_C		m_eAddXYC;			/// ���W�t�^���
	long		m_lSequence;		/// �V�[�P���X

	std::string	m_strAddrName;		/// �Z����
	std::string	m_strKatagaki;		/// ��������
	std::string m_strKeisai;		/// �f�ږ���
	std::string	m_strSource;		/// ���\�[�X
	std::string m_strTelNum;		/// �d�b�ԍ�

//	double dX_TelTP;				/// TelTP��X���W
//	double dY_TelTP;				/// TelTP��Y���W

	//double DistToPOI;				/// TelTP��POI�Ƃ̋���(m)
};



/**
 * @class	CCityAdmin
 * @brief	�s�s�n�}�s���E�N���X
 */
class CCityAdmin : public CFeature
{
public:
	CCityAdmin( IFeature* ipFeature ) : 
	  CFeature(ipFeature),
	  m_lGaikuFugo(0),
	  m_strExGaikuFugo("")

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
	const std::string getAddrCode ( int nLevel ) const
	{
		return ( m_strAddrCode.substr(0, nLevel) );
	}

private:

	/**
	 * @brief	�����o�ϐ��ɃZ�b�g
	 */
	void setInfo();

public:

	std::string	m_strAddrCode;		/// �Z���R�[�h
	LONG		m_lGaikuFugo;		/// �X�敄��
	std::string m_strExGaikuFugo;	/// �g���X�敄��
	ADDRCLASS_C m_eAddrClassC;		/// �Z���\������

};

/**
 * @class	CSite
 * @brief	�w�i�N���X
 * @note	�s�s�n�}�E���k���p
 */
class CSite : public CFeature
{
public:
	CSite( IFeature* ipFeature, char szScale ) : 
	  CFeature(ipFeature),
	  m_szScale(szScale),
	  m_lBGClassC(0),
	  m_strBGClass("")
	{
		setInfo();	/// �ǂ��g����͐�Ɏ擾���Ă���
	}

	~CSite(){};

	/**
	 * @brief	�w�i��ʃh���C��
	 * @return	string
	 */
	const std::string getBGClassDomain();

private:

	/**
	 * @brief	�����o�ϐ��ɃZ�b�g
	 */
	void setInfo();

private:
	char		m_szScale;		/// �w�i�̃X�P�[��(C : �s�s�n�} B : ���k)

public:

	LONG		m_lBGClassC;	/// �w�i���(���k�Ȃ�S4)
	std::string	m_strBGClass;	/// �w�i��ʖ�
};

#endif // !defined(AFX_CITYADMIN_H__C2F259F5_5399_48AA_A610_5A3F4E64E0BC__INCLUDED_)
