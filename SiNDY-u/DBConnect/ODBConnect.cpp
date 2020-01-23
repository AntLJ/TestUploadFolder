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

// ODBConnect.cpp: CODBConnect �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ODBConnect.h"

using namespace std;

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

// �f�t�H���g�R���X�g���N�^
CODBConnect::CODBConnect()
{	
	memset( m_szService, '\0', sizeof(m_szService) );
	memset( m_szUser, '\0', sizeof(m_szUser) );
	memset( m_szPass, '\0', sizeof(m_szPass) );
}

// �����L��R���X�g���N�^
CODBConnect::CODBConnect( const char* lpszProp ) 
{
	std::string strProp = lpszProp;

	strcpy( m_szUser, strProp.substr(0, strProp.find('/')).c_str() );
	strProp.replace(0, strProp.find('/')+1, "" );
	strcpy( m_szPass, strProp.substr(0, strProp.find('@')).c_str() );
	strProp.replace(0, strProp.find('@')+1, "" );
	strcpy( m_szService, strProp.c_str() );

}

// �f�R���X�g���N�^
CODBConnect::~CODBConnect()
{
	if( m_oSess.IsOpen() )
		m_oSess.Close();
}

// �ڑ��֐����̂Q
bool CODBConnect::Connect( const char* lpszProp )
{
	std::string strProp = lpszProp;

	strcpy( m_szUser, strProp.substr(0, strProp.find('/')).c_str() );
	strProp.replace(0, strProp.find('/')+1, "" );
	strcpy( m_szPass, strProp.substr(0, strProp.find('@')).c_str() );
	strProp.replace(0, strProp.find('@')+1, "" );
	strcpy( m_szService, strProp.c_str() );

	// �I���N���ɐڑ�
	return ( Connect() );
}

// �ڑ��֐����̑�3
bool CODBConnect::ConnectWithSess( const char* lpszProp )
{
	std::string strProp = lpszProp;

	strcpy( m_szUser, strProp.substr(0, strProp.find('/')).c_str() );
	strProp.replace( 0, strProp.find('/')+1, "" );
	strcpy( m_szPass, strProp.substr(0, strProp.find('@')).c_str() );
	strProp.replace( 0, strProp.find('@')+1, "" );
	strcpy( m_szService, strProp.c_str() );

	// �Z�b�V�����I�[�v��
	m_oSess.Open();

	ShowProperty();

	// �I���N���ɐڑ�
	return ( m_DBSource.Open(m_oSess, m_szService, m_szUser, m_szPass)==S_OK? true : false );
}
