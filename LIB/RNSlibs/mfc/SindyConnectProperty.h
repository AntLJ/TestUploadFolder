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

// SindyConnectProperty.h: CSindyConnectProperty �N���X�̃C���^�[�t�F�C�X
// $Id: SindyConnectProperty.h,v 1.1.1.1 2003/09/12 07:26:04 f_kiku Exp $
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SINDYCONNECTPROPERTY_H__EF602B13_5B56_427D_B726_32D226E7EDB9__INCLUDED_)
#define AFX_SINDYCONNECTPROPERTY_H__EF602B13_5B56_427D_B726_32D226E7EDB9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
 * @brief SiNDY�ڑ��ݒ�N���X
 */
class CSindyConnectProperty  
{
public:
// �ݒ菈��
	void setPropertiesString(CString strProperties);

// �ݒ菈��
	void clear();

// �擾����
	CString getPropertiesString() const;
	IPropertySetPtr getPropertySet() const;

// �v���p�e�B
	CString e_strServer; ///< �T�[�o
	CString e_strInstance; ///< �C���X�^���X�i�T�[�r�X�j
	CString e_strUser; ///< ���[�U
	CString e_strPassword; ///< �p�X���[�h�i�Í����Ȃ��j
	CString e_strVersion; ///< �o�[�W����

// �ÓI�v���p�e�B
	static CString e_strServerName; ///< "SERVER"
	static CString e_strInstanceName; ///< "INSTANCE"
	static CString e_strUserName; ///< "USER"
	static CString e_strPasswordName; ///< "PASSWORD"
	static CString e_strVersionName; ///< "VERSION"
};

/**
 * @brief SiNDY�ڑ��ݒ�N���X�Eini�t�@�C���Ή���
 */
class CSindyIniConnectProperty : public CSindyConnectProperty
{
public:
// �\�z/����
	CSindyIniConnectProperty(LPCTSTR lpSectionName, LPCTSTR lpIniFilePath = 0);

// ����
	void getFromPrivateIni();

// �o��
	void writeToPrivateIni();

// ����
	BOOL isDefault() const;
	BOOL isDefaultServer() const { return e_strServer == m_cDefault.e_strServer; }
	BOOL isDefaultInstance() const { return e_strInstance == m_cDefault.e_strInstance; }
	BOOL isDefaultUser() const { return e_strUser == m_cDefault.e_strUser; }
	BOOL isDefaultPassword() const { return e_strPassword == m_cDefault.e_strPassword; }
	BOOL isDefaultVersion() const { return e_strVersion == m_cDefault.e_strVersion; }

protected:
// ����
	void getFromPrivateIni(LPCTSTR lpSectionName);

// �o��
	void writeToPrivateIni(LPCTSTR lpSectionName);

// �v���p�e�B
	CSindyConnectProperty  m_cDefault;

private:
	CString m_strIniPath; ///< �q���W���[�����݃f�B���N�g���r\\connectproperty.ini
	CString m_strSection; ///< ini�t�@�C���Z�N�V������
};

/**
 * @brief SiNDY�ڑ��ݒ�N���X�E����
 *
 * �R���X�g���N�^��ini�t�@�C�����Z�N�V������"Common"�ɑ�����ݒ��ǂݍ��ށB
 */
class CSindyCommonConnectProperty : public CSindyIniConnectProperty
{
public:
	CSindyCommonConnectProperty(LPCTSTR lpIniFilePath = 0) :
	CSindyIniConnectProperty(_T("Common"), lpIniFilePath) {}
};

/**
 * @brief SiNDY�ڑ��ݒ�N���X�E���H
 *
 * �R���X�g���N�^��ini�t�@�C�����Z�N�V������"Road"�ɑ�����ݒ��ǂݍ��ށB
 */
class CSindyRoadConnectProperty : public CSindyIniConnectProperty
{
public:
	CSindyRoadConnectProperty(LPCTSTR lpIniFilePath = 0) :
	CSindyIniConnectProperty(_T("Road"), lpIniFilePath) {}
};

/**
 * @brief SiNDY�ڑ��ݒ�N���X�E���k
 *
 * �R���X�g���N�^��ini�t�@�C�����Z�N�V������"Base"�ɑ�����ݒ��ǂݍ��ށB
 */
class CSindyBaseConnectProperty : public CSindyIniConnectProperty
{
public:
	CSindyBaseConnectProperty(LPCTSTR lpIniFilePath = 0) :
	CSindyIniConnectProperty(_T("Base"), lpIniFilePath) {}
};

/**
 * @brief SiNDY�ڑ��ݒ�N���X�E�s�s�n�}
 *
 * �R���X�g���N�^��ini�t�@�C�����Z�N�V������"City"�ɑ�����ݒ��ǂݍ��ށB
 */
class CSindyCityConnectProperty : public CSindyIniConnectProperty
{
public:
	CSindyCityConnectProperty(LPCTSTR lpIniFilePath = 0) :
	CSindyIniConnectProperty(_T("City"), lpIniFilePath) {}
};

/**
 * @brief SiNDY�ڑ��ݒ�N���X�E�Z��
 *
 * �R���X�g���N�^��ini�t�@�C�����Z�N�V������"Addr"�ɑ�����ݒ��ǂݍ��ށB
 */
class CSindyAddrConnectProperty : public CSindyIniConnectProperty
{
public:
	CSindyAddrConnectProperty(LPCTSTR lpIniFilePath = 0) :
	CSindyIniConnectProperty(_T("Addr"), lpIniFilePath) {}
};

#endif // !defined(AFX_SINDYCONNECTPROPERTY_H__EF602B13_5B56_427D_B726_32D226E7EDB9__INCLUDED_)
