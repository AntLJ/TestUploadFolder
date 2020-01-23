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

// ODBConnect.h: CODBConnect �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ODBCONNECT_H__DC075AE7_ADE6_42F6_9DA5_857C1821F544__INCLUDED_)
#define AFX_ODBCONNECT_H__DC075AE7_ADE6_42F6_9DA5_857C1821F544__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Oracl.h"

/**
 * @class	CODBConnect
 * @brief	�I���N���ڑ��N���X
 */
class CODBConnect  
{
	
public:

	CODBConnect();

	/**
	 * @brief	�R���X�g���N�^(��Ƀv���p�e�B���Z�b�g)
	 * @param	lpszProp	[in]	�ڑ��v���p�e�B(Ex. k0505/k0505@delltcp)
	 */
	CODBConnect( const char* lpszProp );

	~CODBConnect();

	/**
	 * @brief	�I���N���c�a�ɐڑ��֐����̂P( �v���p�e�B�������ɕێ����Ă���Ƃ� )
	 * @return	bool
	 */
	bool Connect()
	{
		ShowProperty();
		return ( m_DBSource.Open(m_szService, m_szUser, m_szPass)==S_OK? true : false );
	}

	/**
	 * @brief	�I���N���c�a�ɐڑ��֐����̂Q(�v���p�e�B��񒼐ڎw��)
	 * @note	�f�t�H���g�R���X�g���N�^���Ăяo���Ă���Ƃ��͂�������ǂ���
	 * @param	lpszProp	[in]	�ڑ��v���p�e�B(Ex. k0505/k0505@delltcp)
	 * @return	bool
	 */
	bool Connect( const char* lpszProp );

	/**
	 * @brief	�ڑ��֐�����3(�Z�b�V�����t��)
	 * @param	lpszProp	[in]	�ڑ��v���p�e�B
	 * @return	bool
	 */
	bool ConnectWithSess( const char* lpszProp );
	
	/**
	 * @brief	�ڑ����\���֐�
	 */
	void ShowProperty()
	{
		std::cerr << "�y�Z���c�a�ڑ��v���p�e�B�\���z" << std::endl;
		std::cerr << "USER     : " << m_szUser << std::endl;
		std::cerr << "PASSWORD : " << m_szPass << std::endl;
		std::cerr << "SERVICE  : " << m_szService << std::endl;
		std::cerr << std::endl;
	}

	/**
	 * @brief	DB�\�[�X�擾
	 * @return	ODatabase
	 */
	const ODatabase& GetDbSource() const { return m_DBSource; }

	/**
	 * @brief	�Z�b�V�����擾
	 * @return	OSeesion
	 */
	const OSession& GetSession() const { return m_oSess; }

	/**
	 * @brief	SQL���s 
	 * @note	�ǂݎ���p�ł�
	 * @param	lpszSQL		[in]		SQL��
	 * @param	lpDynaset	[in, out]	ODynaset�N���X
	 * @return	bool
	 */
	bool Execute( const char* lpszSQL, ODynaset* lpDynaset )
	{
		lpDynaset->Open( m_DBSource, lpszSQL, ODYNASET_READONLY );

		return ( lpDynaset->IsOpen()? true : false );
	}

//////////////////////////////////////////////////////////////
//	�����o�ϐ�
////////////////////////////////////////////////////////////
private:

	char m_szService[50];	/// �T�[�r�X��(Ex.delltcp)
	char m_szUser[50];		/// ���[�U��
	char m_szPass[50];		/// �p�X���[�h

	ODatabase m_DBSource;	/// DB�\�[�X

	OSession	m_oSess;	/// �Z�b�V����

};

#endif // !defined(AFX_ODBCONNECT_H__DC075AE7_ADE6_42F6_9DA5_857C1821F544__INCLUDED_)
