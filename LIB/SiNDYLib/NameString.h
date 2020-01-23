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

/**
 * @file NameString.h
 * @brief <b>�I�u�W�F�N�g���̃N���X��`�t�@�C��</b>\n
 * @author �n�}�c�a���암�V�X�e���J���O���[�v �Ð�M�G\n
 * $Id$
 */
#ifndef _NAMESTRING_H_
#define _NAMESTRING_H_

#include "exportimport.h"
#include <boost/shared_ptr.hpp>

namespace sindy {
/**
 * @brief <b>���[�N�X�y�[�X����ӂɓ��肷�邽�߂̕�������Ǘ�����N���X</b>\n
 * ���̃N���X�́A���CRow�N���X�ł̖��̃L���b�V���@�\�̈�Ƃ��č쐬����܂����B\n
 * �]���āA������݂̂��Ǘ���IWorkspace����COM�I�u�W�F�N�g�͊Ǘ��ΏۊO�ł��B\n
 * 
 * �擾�E�Ǘ��ł�����͈ȉ��̂Ƃ���F
 * @li �I�[�i�[���iSDE�̏ꍇ�̂݁j
 * @li �C���X�^���X���iSDE�̏ꍇ�̂݁j
 * @li �T�[�o���iSDE�̏ꍇ�̂݁j
 * @li �o�[�W�������iSDE�̏ꍇ�̂݁j
 * @li ���[�N�X�y�[�X�p�X���i�t�@�C���x�[�X�̏ꍇ�j
 *
 * SDE�x�[�X���t�@�C���x�[�X���𔻒f����ꍇ�ɂ́AIsSDE�֐����g�p���Ă��������B
 */
class SINDYLIB_API CWorkspaceNameString
{
public:
	CWorkspaceNameString();
	virtual ~CWorkspaceNameString();

	CWorkspaceNameString( IUnknown* ipUnk )
	{
		SetName( ipUnk );
	}

	bool operator==( const CWorkspaceNameString& obj ) const {
		return IsSameSource( obj );
	}

	bool operator <( const CWorkspaceNameString& obj ) const;

	/**
	 * @brief <b>���[�N�X�y�[�X���當������擾����</b>\n
	 * ipUnk�͎��̃C���^�[�t�F�[�X�̂ǂꂩ���T�|�[�g���Ă���K�v������܂��F
	 * @li IWorkspace
	 * @li IPropertySet
	 * @li IDataset
	 * @li ITable
	 * @li IRow
	 * @param ipUnk		[in]	�I�u�W�F�N�g
	 * @retval true ����
	 * @retval false ���s
	 */
	virtual bool SetName( IUnknown* ipUnk );

	/** 
	 * @brief <b>�ڑ����[�U�����擾����</b>\n
	 * ���ɏ�񂪃Z�b�g����Ă���K�v������܂��B
	 * @return �ڑ����[�U���i�啶���Œ�j
	 */
	LPCTSTR GetUserName() const {
		return m_strUserName;
	}

	/** 
	 * @brief <b>�T�[�o�[�����擾����</b>\n
	 * ���ɏ�񂪃Z�b�g����Ă���K�v������܂��B
	 * @return �T�[�o�[���i�q�����Œ�j
	 */
	LPCTSTR GetServerName() const {
		return m_strServerName;
	}

	/**
	 * @brief <b>�C���X�^���X���擾����</b>\n
	 * ���ɏ�񂪃Z�b�g����Ă���K�v������܂��B
	 * @return �C���X�^���X������
	 */
	LPCTSTR GetInstanceName() const {
		return m_strInstanceName;
	}

	/**
	 * @brief <b>�o�[�W���������擾����</b>\n
	 * ���ɏ�񂪃Z�b�g����Ă���K�v������܂��B
	 * @return �o�[�W������
	 */
	LPCTSTR GetVersionName() const {
		return m_strVersionName;
	}

	/**
	 * @brief <b>�t�@�C���x�[�X�̃��[�N�X�y�[�X�̃p�X���擾����</b>\n
	 * �t�@�C���x�[�X��SDE�x�[�X���𔻒f����ɂ�IsSDE���g�p���Ă��������B\n
	 * ���ɏ�񂪃Z�b�g����Ă���K�v������܂��B
	 * @return �p�X��
	 */
	LPCTSTR GetPathName() const {
		if( ! IsSDE() )
			return GetConnectString();
		else
			return (LPCTSTR)NULL;
	}

	/**
	 * @brief <b>�ڑ���������擾����</b>\n
	 * �����Ō����ڑ�������Ƃ́ASiNDYLib�Œ�`�����Ǝ��`����
	 * ������̂��Ƃł��B
	 * @return �ڑ�������i�t�H�[�}�b�g�Fuser[owner.version]@server:instance����PGDB�t�@�C���p�X�j
	 */
	LPCTSTR GetConnectString() const {
		return m_strConnectString;
	}

	/**
	 * @brief <b>�t�@�C���x�[�X��SDE�x�[�X���𔻒f����</b>\n
	 * ���ɏ�񂪃Z�b�g����Ă���K�v������܂��B
	 * @retval true SDE�x�[�X
	 * @retval false �t�@�C���x�[�X
	 */
	bool IsSDE() const {
		return m_bIsSDE;
	}

	/**
	 * @brief <b>�����\�[�X���ǂ����𔻒f����</b>
	 * �ڑ��v���p�e�B�Ŕ��f���܂��B
	 */
	bool IsSameSource( const CWorkspaceNameString& cNameString ) const;

private:
#pragma warning(push)
#pragma warning(disable:4251)
	CString m_strUserName;
	CString m_strServerName;
	CString m_strInstanceName;
	CString m_strVersionName;
	CString m_strConnectString;
#pragma warning(pop)
	bool m_bIsSDE;
};

typedef boost::shared_ptr<CWorkspaceNameString> CSPWorkspaceNameString;

/**
 * @brief <b>�e�[�u������ӂɓ��肷�邽�߂̕�������Ǘ�����N���X</b>\n
 * ���̃N���X�́A���CRow�N���X�ł̖��̃L���b�V���@�\�̈�Ƃ��č쐬����܂����B\n
 * �]���āA������݂̂��Ǘ���ITable����COM�I�u�W�F�N�g�͊Ǘ��ΏۊO�ł��B\n
 * 
 * �擾�E�Ǘ��ł�����͈ȉ��̂Ƃ���F
 * @li �ڑ����[�U���iSDE�̏ꍇ�̂݁A�啶���Œ�j
 * @li �C���X�^���X���iSDE�̏ꍇ�̂݁j
 * @li �T�[�o���iSDE�̏ꍇ�̂݁j
 * @li �o�[�W�������iSDE�̏ꍇ�̂݁j
 * @li ���[�N�X�y�[�X�p�X���i�t�@�C���x�[�X�̏ꍇ�j
 * @li �e�[�u�����i�啶���Œ�j
 * @li �e�[�u���I�[�i�[���i�啶���Œ�j
 *
 * SDE�x�[�X���t�@�C���x�[�X���𔻒f����ꍇ�ɂ́AIsSDE�֐����g�p���Ă��������B
 */
class SINDYLIB_API CTableNameString : public CWorkspaceNameString
{
public:
	CTableNameString();
	virtual ~CTableNameString();

	CTableNameString( IUnknown* ipUnk )
	{
		SetName( ipUnk );
	}

	bool operator==( const CTableNameString& obj ) const {
		return ( 0 == m_strTableName.CompareNoCase( obj.m_strTableName ) &&
			IsSameSource( obj ) );
	}

	bool operator<( const CTableNameString& obj ) const;

	/**
	 * @brief <b>�e�[�u������e�[�u�����̕�������擾����</b>\n
	 * ipUnk�͎��̃C���^�[�t�F�[�X�̂ǂꂩ���T�|�[�g���Ă���K�v������܂��F
	 * @li Table
	 * @li Row
	 * @param ipUnk		[in]	�I�u�W�F�N�g
	 * @retval true ����
	 * @retval false ���s
	 */
	virtual bool SetName( IUnknown* ipUnk );

	/**
	 * @brief <b>�e�[�u�������擾����</b>\n
	 * ���ɏ�񂪃Z�b�g����Ă���K�v������܂��B
	 * @return �e�[�u�����i�啶���Œ�j
	 */
	LPCTSTR GetTableName() const {
		return m_strTableName;
	}

	/**
	 * @brief <b>�I�[�i�[�����擾����</b>\n
	 * ���ɏ�񂪃Z�b�g����Ă���K�v������܂��B
	 * @return �I�[�i�[���i�啶���Œ�j
	 */
	LPCTSTR GetOwnerName() const {
		return m_strOwnerName;
	}

	/**
	 * @brief <b>�I�[�i�[���������ǂ������`�F�b�N����</b>\n
	 * ���ɏ�񂪃Z�b�g����Ă���K�v������܂��B
	 * @retval true �I�[�i�[��������
	 * @retval false �I�[�i�[���������Ȃ�
	 */
	bool HasOwnerName() const {
		return m_strOwnerName.IsEmpty() ? false : true;
	}
	/**
	 * @brief <b>�I�[�i��.�e�[�u�������擾����</b>\n
	 * ���ɏ�񂪃Z�b�g����Ă���K�v������܂��B\n
	 * @return �I�[�i��.�e�[�u�����i�啶���Œ�j
	 */
	LPCTSTR GetOwnerTableName() const {
		return m_strOwnerTableName;
	}

	/**
	 * @brief <b>�����\�[�X���ǂ����𔻒f����</b>
	 * �I�[�i���Ɛڑ��v���p�e�B�Ŕ��f���܂��B
	 */
	bool IsSameSource( const CTableNameString& cNameString ) const;
private:
#pragma warning(push)
#pragma warning(disable:4251)
	CString m_strTableName;
	CString m_strOwnerName;
	CString m_strOwnerTableName;
#pragma warning(pop)
};

typedef boost::shared_ptr<CTableNameString> CSPTableNameString;

} // sindy

#endif // ifndef _NAMESTRING_H_
