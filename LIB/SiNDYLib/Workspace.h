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
 * @file Workspace.h
 * @brief CWorkspace�N���X��`�t�@�C��
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#ifndef _WORKSPACE_H_
#define _WORKSPACE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../LIB/WinLib/ComWrapper.h"
#include "ErrorCode.h"
#include "NameString.h"

namespace sindy {
/**
 * @class CWorkspace
 * @brief ���[�N�X�y�[�X�ڑ��Ǘ��N���X
 */
#pragma warning(push)
#pragma warning(disable:4275)
#pragma warning(disable:4251)
class SINDYLIB_API CWorkspace : public CComWrapper<IWorkspace>
{
public:
	explicit CWorkspace();
	virtual ~CWorkspace();

	explicit CWorkspace( IWorkspace* lp )
	{
		*this = lp;
	}
	explicit CWorkspace( LPCTSTR lpcszConnectString )
	{
		Connect( lpcszConnectString );
	}
	CWorkspace& operator=( IWorkspace* lp )
	{
		AtlComPtrAssign( (IUnknown**)&p, lp );

		// �ڑ�������N���A
		m_spNameString.reset( (CWorkspaceNameString*)NULL );

		return *this;
	}

	CWorkspace( LPCTSTR lpcszUserName, 
		LPCTSTR lpcszPassword, 
		LPCTSTR lpcszVersion, 
		LPCTSTR lpcszInstance, 
		LPCTSTR lpcszServerName )
	{
		ConnectToSDE( lpcszUserName, lpcszPassword, lpcszVersion, lpcszInstance, lpcszServerName );
	}
	bool operator ==( const CWorkspace& cWorkspace ) const;

	operator IWorkspace* () const { return p; }

	//@{ @name IWorkspace�C���^�[�t�F�[�X
	IPropertySetPtr GetConnectionProperies() const;
	IEnumDatasetNamePtr GetDatasetNames( esriDatasetType DatasetType ) const;
	IEnumDatasetPtr GetDatasets( esriDatasetType DatasetType ) const;
	errorcode::sindyErrCode ExecuteSQL( LPCTSTR lpcszSqlStatement ) const;
	bool Exists() const;
	bool IsDirectory() const;
	LPCTSTR GetPathName( LPTSTR lpszPathName, int nLen ) const;
	esriWorkspaceType GetType() const;
	IWorkspaceFactoryPtr GetWorkspaceFactory() const;
	//@}
	/**
	 * @brief <b>���������΍��p</b>\n
	 * �����������p�̃R�[�h�ł��BConnect����O�ɌĂяo���܂��B
	 */
	bool PreConnect();

	/**
	 * @brief <b>�f�[�^�x�[�X�ɐڑ�����</b>\n
	 * ��x�ڑ�����ƁA�ڑ�������ŕێ����܂��B
	 * �����̐ڑ���ێ����邱�Ƃ��ł��܂��B
	 * SDE�ƃp�[�\�i���E�W�I�f�[�^�x�[�X�ɂ̂ݑΉ����Ă��܂��B
	 * @see ConnectToSDE
	 * @see ConnectToPGDB
	 * @param lpcszConnectString	[in]	�ڑ�������i<em>username</em>/<em>password</em>/<em>version</em>/<em>instance</em>/<em>servername</em>����PGDB�t�@�C�����j
	 * @return �ڑ��ł�����true
	 */
	bool Connect( LPCTSTR lpcszConnectString );

	/**
	 * @brief <b>SDE�f�[�^�x�[�X�ɐڑ�����</b>\n
	 * ���ɐڑ�����Ă����牽�����܂���B
	 * @param lpcszUserName		[in]	�ڑ����[�U��
	 * @param lpcszPassword		[in]	�ڑ����[�U�p�X���[�h
	 * @param lpcszVersion		[in]	�ڑ��o�[�W����
	 * @param lpcszInstance		[in]	�ڑ��C���X�^���X
	 * @param lpcszServerName	[in]	�ڑ��T�[�o��
	 * @return �ڑ��ł�����true
	 */
	bool ConnectToSDE( LPCTSTR lpcszUserName, 
		LPCTSTR lpcszPassword, 
		LPCTSTR lpcszVersion, 
		LPCTSTR lpcszInstance, 
		LPCTSTR lpcszServerName );
	/**
	 * @brief �p�[�\�i���E�W�I�f�[�^�x�[�X�ɐڑ�����
	 * ���ɐڑ�����Ă����牽�����܂���B
	 * @param lpcszPGDBFileName
	 * @return �ڑ��ł�����true
	 */
	bool ConnectToPGDB( LPCTSTR lpcszPGDBFileName );

	/**
	 * @biref OLEDB�ɐڑ�����
	 * ���ɐڑ�����Ă����牽�����܂���B
	 * @param lpcszConnectString [in] �ڑ�������
	 * @return �ڑ��ł�����true
	 */
	bool ConnectToOLEDB( LPCTSTR lpcszConnectString );

	/**
	 * @brief �e�[�u�����I�[�v������
	 * @param lpcszTableName	[in]	�e�[�u����
	 * @return ITablePtr
	 */
	ITablePtr OpenTable( LPCTSTR lpcszTableName ) const;

	/**
	 * @brief <b>�T�[�o���Ȃǂ��Ǘ����郁���o�̋��L�R�s�[��Ԃ�</b>\n
	 * m_spNameString�̎��̂�shared_ptr��Ԃ��܂��B
	 * @return boost::shared_ptr<CWorkspaceNameString>
	 */
	CSPWorkspaceNameString GetNameString() const;

	//@{ @name IWorkspaceEdit�C���^�[�t�F�[�X�i�ꕔ�j
	/**
	 * @brief <b>�ҏW�J�n����</b>\n
	 * �J�X�^���|�X�g�p�̏ꍇ��g_bCustomPostMode�t���O��
	 * true�ɂ��܂��B
	 * @param bWithUndoRedo	[in]	Undo/Redo��L���ɂ���Ȃ�true
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode StartEditing( bool bWithUndoRedo ) const;

	/**
	 * @brief <b>�ҏW���I������</b>\n
	 * sindyErr_EditMustBeReconcile���Ԃ����ꍇ�̓��R���T�C�����
	 * �ēx�ۑ�����K�v������܂��B
	 * @param bSaveEdits	[in]	�ҏW��ۑ�����Ȃ�true
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode StopEditing( bool bSaveEdits ) const;

	/**
	 * @brief <b>�ҏW�����ɊJ�n����Ă��邩�ǂ���</b>\n
	 * @retval true �ҏW���J�n����Ă���
	 * @retval false �ҏW���J�n����Ă��Ȃ�
	 */
	bool IsBeingEdited() const;

	/**
	 * @brief <b>�I�y���[�V�����̊J�n</b>\n
	 */
	errorcode::sindyErrCode StartEditOperation() const;

	/**
	 * @brief <b>�I�y���[�V�����̊J�n</b>\n
	 */
	errorcode::sindyErrCode AbortEditOperation() const;

	/**
	 * @brief <b>�I�y���[�g�̏I��</b>\n
	 */
	errorcode::sindyErrCode StopEditOperation() const;
	//@}

	//@{ @name ITransaction�C���^�[�t�F�[�X
	/// �g�����U�N�V�������J�n����
	errorcode::sindyErrCode StartTransaction() const;
	/// �g�����U�N�V�������R�~�b�g����
	errorcode::sindyErrCode CommitTransaction() const;
	/// �g�����U�N�V������j������
	errorcode::sindyErrCode AbortTransaction() const;
	//@}

	/**
	 * @brief �p�[�\�i���E�W�I�f�[�^�x�[�X�����ǂ����𔻒f����
	 * �Ō��4�������u.mdb�v���ǂ����Ŕ��f���܂��B
	 * @param lpcszString	[in]	���蕶����
	 * @return �p�[�\�i���E�W�I�f�[�^�x�[�X�̃t�@�C�����Ȃ�true
	 */
	static bool IsPGDBFileName( LPCTSTR lpcszString );

	/**
	 * @brief OLEDB�ڑ������񖼂��ǂ����𔻒f����
	 * �ŏ��̕����񂪁uProvider=�v���ǂ����Ŕ��f���܂��B
	 * @param lpcszString [in] ���蕶����
	 * @return OLEDB�̐ڑ�������Ȃ�true
	 */
	static bool IsOLEDBConnectString( LPCTSTR lpcszString );

	/**
	 * @brief <b>�ڑ����������o�Ɋm�ۂ���</b>\n
	 */
//	void InitProperties();
protected:
	mutable CSPWorkspaceNameString m_spNameString;
};
#pragma warning(pop)
} // sindy

#endif // _WORKSPACE_H_
