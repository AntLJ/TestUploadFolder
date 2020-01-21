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

// SiNDYConnectPropertySet.h: SiNDYConnectPropertySetSet �N���X�̃C���^�[�t�F�C�X
//
// TODO: RoadCheck��RoadCheckLogViewer�v���W�F�N�g�ɂ��܂܂�Ă��邪�A�܂܂�Ȃ��悤�ɂ��ׂ�

#if !defined(AFX_SINDYCONNECTPROPERTYSET_H__EF602B13_5B56_427D_B726_32D226E7EDB9__INCLUDED_)
#define AFX_SINDYCONNECTPROPERTYSET_H__EF602B13_5B56_427D_B726_32D226E7EDB9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
 * @brief SiNDY�ڑ��ݒ�N���X
 */
class SiNDYConnectPropertySet  
{
public:
// �\�z/����
	SiNDYConnectPropertySet();
	SiNDYConnectPropertySet(LPCTSTR lpszProperties);
	SiNDYConnectPropertySet(const CString& rProperties);

// �ݒ�/�擾
	void clear();
	bool empty() const;

	void setPropertySet(const SiNDYConnectPropertySet& rPropertySet);
	bool setPropertiesString(const CString& rProperties);

	CString getPropertiesString(CString sep = m_szSep) const;
	IPropertySetPtr getPropertySet() const;

	bool operator == (const SiNDYConnectPropertySet& rPropertySet) const;
	bool operator != (const SiNDYConnectPropertySet& rPropertySet) const { return ! (*this == rPropertySet); }

// �v���p�e�B
	CString m_strServer;                 //!< �T�[�o
	CString m_strInstance;               //!< �C���X�^���X�i�T�[�r�X�j
	CString m_strUser;                   //!< ���[�U
	CString m_strPassword;               //!< �p�X���[�h�i�Í����Ȃ��j
	CString m_strVersion;                //!< �o�[�W����
	CString m_strClient;                 //!< �N���C�A���g
	CString m_strAuthenticationMode;     //!< �F�؃��[�h
	CString m_strDBConnectionProperties; //!< �ڑ��v���p�e�B
	CString m_strLocalDB;                //!< PGDB�̃p�X
	bool m_bConnect_AS;                  //!< AS�ڑ���

// �ÓI�v���p�e�B
	static const TCHAR m_szSep[];
	static const CString m_strServerName;                 //!< "SERVER"
	static const CString m_strInstanceName;               //!< "INSTANCE"
	static const CString m_strUserName;                   //!< "USER"
	static const CString m_strPasswordName;               //!< "PASSWORD"
	static const CString m_strVersionName;                //!< "VERSION"
	static const CString m_strLocalDBName;                //!< "DATABASE"
	static const CString m_strClientName;                 //!< "DBCLIENT"
	static const CString m_strAuthenticationModeName;     //!< "AUTHENTICATION_MODE"
	static const CString m_strDBConnectionPropertiesName; //!< "DB_CONNECTION_PROPERTIES"

};

#endif // !defined(AFX_SINDYCONNECTPROPERTY_H__EF602B13_5B56_427D_B726_32D226E7EDB9__INCLUDED_)
