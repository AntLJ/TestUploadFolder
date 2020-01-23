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

// SindyConnectProperty.cpp
//
// $Id: SindyConnectProperty.cpp,v 1.1.1.1 2003/09/12 07:26:04 f_kiku Exp $
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#define _SINDY_NO_CSTRING
#include "CStringTokenizer.h"
#include "SindyConnectProperty.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CString CSindyConnectProperty::e_strServerName(_T("SERVER"));
CString CSindyConnectProperty::e_strInstanceName(_T("INSTANCE"));
CString CSindyConnectProperty::e_strUserName(_T("USER"));
CString CSindyConnectProperty::e_strPasswordName(_T("PASSWORD"));
CString CSindyConnectProperty::e_strVersionName(_T("VERSION"));

// CSindyConnectProperty �N���X�̃C���v�������e�[�V����
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// �ݒ菈��
//////////////////////////////////////////////////////////////////////

/**
 * @brief "�q���[�U�r/�q�p�X���[�h�r/�q�o�[�W�����r/�q�C���X�^���X�r/�q�T�[�o�r"�Ƃ����`���̕����񂩂�l��ݒ肷��
 *
 * @param strProperties [in] �Ώە�����
 */
void CSindyConnectProperty::setPropertiesString(CString strProperties)
{
	CStringTokenizer tkz(strProperties, _T("/"));
	e_strUser = tkz.GetNextToken();
	e_strPassword = tkz.GetNextToken();
	e_strVersion = tkz.GetNextToken();
	e_strInstance = tkz.GetNextToken();
	e_strServer = tkz.GetNextToken();
}

//////////////////////////////////////////////////////////////////////
// �ݒ菈��
//////////////////////////////////////////////////////////////////////

void CSindyConnectProperty::clear()
{
	e_strServer.Empty();
	e_strInstance.Empty();
	e_strUser.Empty();
	e_strPassword.Empty();
	e_strVersion.Empty();
}

//////////////////////////////////////////////////////////////////////
// �擾����
//////////////////////////////////////////////////////////////////////

/**
 * @brief "�q���[�U�r/�q�p�X���[�h�r/�q�o�[�W�����r/�q�C���X�^���X�r/�q�T�[�o�r"�Ƃ����`���̕�������擾����
 *
 * @return �ڑ��v���p�e�B������
 */
CString CSindyConnectProperty::getPropertiesString() const
{
	CString strProperties;
	strProperties.Format(_T("%s/%s/%s/%s/%s"), e_strUser, e_strPassword, e_strVersion, e_strInstance, e_strServer);
	return strProperties;
}

IPropertySetPtr CSindyConnectProperty::getPropertySet() const
{
	return (
		ScmPropertySet(
			CComBSTR(e_strServer),
			CComBSTR(e_strInstance),
			CComBSTR(_T("")),
			CComBSTR(e_strUser),
			CComBSTR(e_strPassword),
			CComBSTR(e_strVersion)
		)
	);

}

// CSindyIniConnectProperty �N���X�̃C���v�������e�[�V����
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

/**
 * @brief ini�t�@�C���̃Z�N�V���������w�肵�āA�ڑ��ݒ���\�z����
 *
 * @param lpSectionName [in] �Z�N�V�������B
 * @param lpIniFilePath [in] ini�t�@�C���̃p�X�BNULL�ɂ���ƁA���s���W���[���̑��݂���f�B���N�g���́uconnectproperty.ini�v���g�p����B
 */
CSindyIniConnectProperty::CSindyIniConnectProperty(LPCTSTR lpSectionName, LPCTSTR lpIniFilePath)
{
	ATLASSERT(lpSectionName);

	if(lpIniFilePath) {
		m_strIniPath = lpIniFilePath;
	}
	else {
		m_strIniPath = RNSWin32Plant::getModuleDirectoryPath().c_str();
		m_strIniPath += "\\connectproperty.ini";
	}

	m_strSection = lpSectionName;
	if(m_strSection.GetLength()) {
		getFromPrivateIni();
	}
}

//////////////////////////////////////////////////////////////////////
// ����
//////////////////////////////////////////////////////////////////////

/**
 * @brief ini�t�@�C�����K��l��ǂݍ���
 *
 * @param lpSectionName [in] �Z�N�V������
 */
void CSindyIniConnectProperty::getFromPrivateIni(LPCTSTR lpSectionName)
{
	TCHAR aTmpString[256];

	GetPrivateProfileString(lpSectionName, e_strServerName, _T(""), aTmpString, 256, m_strIniPath);
	m_cDefault.e_strServer = e_strServer = aTmpString;

	GetPrivateProfileString(lpSectionName, e_strInstanceName, _T(""), aTmpString, 256, m_strIniPath);
	m_cDefault.e_strInstance =e_strInstance = aTmpString;

	GetPrivateProfileString(lpSectionName, e_strUserName, _T(""), aTmpString, 256, m_strIniPath);
	m_cDefault.e_strUser = e_strUser = aTmpString;

	GetPrivateProfileString(lpSectionName, e_strPasswordName, _T(""), aTmpString, 256, m_strIniPath);
	m_cDefault.e_strPassword = e_strPassword = aTmpString;

	GetPrivateProfileString(lpSectionName, e_strVersionName, _T(""), aTmpString, 256, m_strIniPath);
	m_cDefault.e_strVersion = e_strVersion = aTmpString;
}

void CSindyIniConnectProperty::getFromPrivateIni()
{
	if(m_strSection.GetLength()) {
		getFromPrivateIni((LPCTSTR)m_strSection);
	}
}

//////////////////////////////////////////////////////////////////////
// �o��
//////////////////////////////////////////////////////////////////////

/**
 * @brief ini�t�@�C���ɋK��l�Ƃ��ď�������
 *
 * @param lpSectionName [in] �Z�N�V������
 */
void CSindyIniConnectProperty::writeToPrivateIni(LPCTSTR lpSectionName)
{
	m_cDefault.e_strServer = e_strServer;
	WritePrivateProfileString(lpSectionName, e_strServerName, e_strServer, m_strIniPath);

	m_cDefault.e_strInstance = e_strInstance;
	WritePrivateProfileString(lpSectionName, e_strInstanceName,  e_strInstance, m_strIniPath);

	m_cDefault.e_strUser = e_strUser;
	WritePrivateProfileString(lpSectionName, e_strUserName, e_strUser, m_strIniPath);

	m_cDefault.e_strPassword = e_strPassword;
	WritePrivateProfileString(lpSectionName, e_strPasswordName, e_strPassword, m_strIniPath);

	m_cDefault.e_strVersion = e_strVersion;
	WritePrivateProfileString(lpSectionName, e_strVersionName, e_strVersion, m_strIniPath);
}

void CSindyIniConnectProperty::writeToPrivateIni()
{
	if(m_strSection.GetLength()) {
		writeToPrivateIni((LPCTSTR)m_strSection);
	}
}

//////////////////////////////////////////////////////////////////////
// ����
//////////////////////////////////////////////////////////////////////

/**
 * @brief ���݂̃v���p�e�B�͋K��l���H
 */
BOOL CSindyIniConnectProperty::isDefault() const
{
	return isDefaultServer() && isDefaultInstance() && isDefaultUser() && isDefaultPassword() && isDefaultVersion();
}
