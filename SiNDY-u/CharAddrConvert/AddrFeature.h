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

/**
 * @class	CAddrBaseFeature
 * @brief	�Z���n�t�B�[�`�����b�p���N���X
 */
class CAddrBaseFeature
{
public:

	CAddrBaseFeature( IFeature* ipFeature, const CFieldMap& cFIDMap ) : m_ipFeature(ipFeature), m_cFIDMap(cFIDMap){}
	virtual ~CAddrBaseFeature(){}

	/**
	 * @brief	OID�擾
	 * @return	long
	 */
	long GetOID() const
	{
		long lOID = 0;
		m_ipFeature->get_OID( &lOID );
		return lOID;
	}

	/**
	 * @brief	�W�I���g���擾
	 * @return	IGeometryPtr
	 */
	IGeometryPtr GetGeometry() const
	{
		IGeometryPtr ipGeo;
		m_ipFeature->get_Shape( &ipGeo );
		return ipGeo;
	}

private:

	/**
	 * @brief	�����擾
	 * @note	�p����ŕK������
	 */
	virtual void SetAttr() = 0;

protected:

	IFeaturePtr		m_ipFeature;	//!< IFeature�ێ�
	CFieldMap		m_cFIDMap;		//!< �t�B�[���h�C���f�b�N�X�}�b�v
};

/**
 * @class	CAdminInfo
 * @brief	�s���E���
 */
class CAdminInfo : public CAddrBaseFeature
{
public:

	CAdminInfo( IFeature* ipFeature, const CFieldMap& cFIDMap ) : CAddrBaseFeature(ipFeature, cFIDMap){ SetAttr(); }
	~CAdminInfo(){}

private:

	/**
	 * @brief	�����擾
	 */
	void SetAttr();

public:

	CString		m_strAddrCode;		//!< �Z���R�[�h
	long		m_lGaikuFugo;		//!< �X�敄��
	CString		m_strExtGaiku;		//!< �g���X�敄��
	CString		m_strExtGaikuYomi;	//!< �g���X�敄�����~
	long		m_lAddrClass_C;		//!< �Z���������
	long		m_lAreaClass_C;		//!< �G���A���ʃR�[�h
	CString		m_strOazaAzaCode2;	//!< �ʏ̍s���E�p�厚�E���R�[�h(Bug8177)

};

typedef std::list<CString> TOKENLIST;

/**
 * @class	CGouInfo
 * @brief	���|�C���g���
 */
class CGouInfo : public CAddrBaseFeature
{
public:	

	CGouInfo( IFeature* ipFeature, const CFieldMap& cFIDMap ) : CAddrBaseFeature(ipFeature, cFIDMap), m_bMoji(false){ SetAttr(); }
	~CGouInfo(){};

	/** 
	 * @brief	���ԍ��g�[�N����
	 * @return	�g�[�N����
	 */
	long GetTokenCount() const { return (signed)m_listTokenName.size(); }

	/**
	 * @brief	���ԍ��擾
	 * @param	iStart		[in]	�X�^�[�g�ʒu(-1�Ȃ�S�Ď擾)
	 * @param	iEnd		[in]	�I���ʒu(-1�Ȃ�A�X�^�[�g����S�Ă��擾)
	 * @return	���ԍ�
	 */
	CString GetGouNo( int iStart = -1, int iEnd = -1 ) const
	{
		return (-1 == iStart)? m_strGouNo : GetToken(m_listTokenName, iStart, iEnd);
	}

	/**
	 * @brief	���ԍ����~�擾
	 * @param	iStart		[in]	�X�^�[�g�ʒu(-1�Ȃ�S�Ď擾)
	 * @param	iEnd		[in]	�I���ʒu(-1)�Ȃ�A�X�^�[�g����S�Ă��擾
	 * @return	���ԍ����~
	 */
	CString GetGouNoYomi( int iStart = -1, int iEnd = -1 ) const
	{
		return (-1 == iStart)? m_strGouNoYomi : GetToken(m_listTokenYomi, iStart, iEnd);
	}

private:

	/**
	 * @brief	�����擾
	 */
	void SetAttr();

	/**
	 * @brief	�g�[�N���擾�֐�
	 * @param	listToken	[in]	�g�[�N�����X�g
	 * @param	iStart		[in]	�X�^�[�g�ʒu
	 * @param	iEnd		[in]	�I���ʒu(-1)�Ȃ�A�X�^�[�g����S�Ă��擾
	 */
	CString GetToken ( const TOKENLIST& listToken, int iStart, int iEnd = -1 ) const;

public:

	long			m_lGouType;				//!< ���^�C�v
	bool			m_bExGouF;				//!< �g�����ԍ��t���O
	bool			m_bPriorF;				//!< �D��t���O
	bool			m_bMoji;				//!< �����܂ށH(���ԍ���[#]���邩�ǂ���)

private:

	CString			m_strGouNo;				//!< ���ԍ�([#]�͎�菜��)
	CString			m_strGouNoYomi;			//!< ���ԍ����~([#]���t���Ă���̂̂ݎ擾)

	TOKENLIST	m_listTokenName;			//!< ���ԍ����u-�v�ł��ꂼ���؂�������(#�͏���)
	TOKENLIST	m_listTokenYomi;			//!< ���ԍ����~���u-�v�ł��ꂼ���؂�������(#�͏���)

	/**
	 * @brief	�g�[�N�������֐�
	 * @param	strData		[in]		�f�[�^
	 * @param	lcpszSep	[in]		�Z�p���[�^
	 * @param	rList		[in,out]	�i�[�p�R���e�i
	 */
	void Tokenize( const CString& strData, LPCWSTR lcpszSep, TOKENLIST& rList )
	{
		// ������UNICODE�ɂ��ĕ���
		CStringW strDataW( strData );
		int iCurPos = 0;
		CString strToken( strDataW.Tokenize( lcpszSep, iCurPos ) );
		while( !strToken.IsEmpty() )
		{
			rList.push_back( strToken );
			strToken = strDataW.Tokenize( lcpszSep, iCurPos );
		}
	}
};
