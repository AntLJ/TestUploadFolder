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
 * @file SiNDYeSettings.h
 * @brief CSiNDYeSettings�N���X��`�t�@�C��
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#ifndef _SINDYESETTINGS_H_
#define _SINDYESETTINGS_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
 * @class CSiNDYeSettings
 * @brief <b>SiNDY-e�̏����ݒ背�W�X�g����settings.ini
 * �̓��e���Ǘ�����N���X</b>\n
 * ���̃N���X�ň����ݒ�͈ȉ��̂Ƃ���F
 * @li SiNDY-e�C���X�g�[���t�H���_��
 * @li ini�t�@�C�����isettings.ini�j
 * @li SiNDY-e�o�[�W������
 * @li ���[���ڑ��p�T�[�o��
 * @li SiNDY-e�x�����x��
 * @warning ���̃N���X��export���Ă��Ȃ������o�ϐ���錾���Ă��܂����Aprivate�Ȃ��ߊO����
 * �A�N�Z�X����邱�Ƃ͂���܂���B
 * �Ȃ��A��L�̂悤�ȏꍇ���̎|���[�j���O���o��̂ł����A�E�U�C�̂Ń��[�j���O�������Ă��܂��B
 */
class CSiNDYeSettings
{
public:
	CSiNDYeSettings();
	virtual ~CSiNDYeSettings();

	//@{ @name �擾�n�֐�
	/**
	 * @brief <b>SiNDY-e�̃C���X�g�[���t�H���_���擾����</b>\n
	 * Load�ŏ���ǂݍ��񂾌�Ɏ��s���Ă��������B\n
	 * �擾����L�[�FHKEY_LOCAL_MACHINE\\Software\\INCREMENT P CORPORATION\\SiNDY-e\InstallRoot\\Path
	 * @return �C���X�g�[���t�H���_�i�Ō��\���t���j
	 */
	LPCTSTR GetInstallRoot() const { return m_strInstallRoot; }

	/**
	 * @brief <b>�C���X�g�[���t�H���_���Ɋi�[�����ini�t�@�C�������擾����</b>\n
	 * Load�ŏ���ǂݍ��񂾌�Ɏ��s���Ă��������B\n
	 * �擾����L�[�FHKEY_LOCAL_MACHINE\\Software\\INCREMENT P CORPORATION\\SiNDY-e\InstallRoot\\InitFileName\n
	 * �ʏ��settings.ini�ł��B
	 * @return ini�t�@�C����
	 */
	LPCTSTR GetInitFileName() const { return m_strInitFileName; }

	/**
	 * @brief <b>�C���X�g�[������Ă���SiNDY-e�̃o�[�W�������擾����</b>\n
	 * Load�ŏ���ǂݍ��񂾌�Ɏ��s���Ă��������B\n
	 * �擾����L�[�FHKEY_LOCAL_MACHINE\\Software\\INCREMENT P CORPORATION\\SiNDY-e\\Version\\Version
	 * @return �o�[�W������
	 */
	LPCTSTR GetVersion() const { return m_strVersion; }

	/**
	 * @brief <b>�x�����x�����擾����</b>\n
	 * Load�ŏ���ǂݍ��񂾌�Ɏ��s���Ă��������B\n
	 * �擾����L�[�FHKEY_LOCAL_MACHINE\\Software\\INCREMENT P CORPORATION\\SiNDY-e\Warning\\Level
	 * @return �x�����x��
	 */
	long GetWarnLevel() const { return m_lWarnLevel; }

	/**
	 * @brief <b>���[���p�̃p�[�\�i���E�W�I�f�[�^�x�[�X�̃p�X���擾����</b>\n
	 * Load�ŏ���ǂݍ��񂾌�Ɏ��s���Ă��������B\n
	 * �擾����L�[�FHKEY_CURRENT_USER\\Software\\Increment P Corporation\SiNDY-e\\Tools\\COMMON\\PGDBFileName
	 * @return PGDB�t�@�C���p�X
	 */
	LPCTSTR GetPGDBFileName() const { return m_strPGDBFileName; }

	/**
	 * @brief <b>���[���f�[�^�x�[�X�Ƀp�[�\�i���E�W�I�f�[�^�x�[�X���g�p���邩�ǂ����̃t���O���擾����</b>\n
	 * Load�ŏ���ǂݍ��񂾌�Ɏ��s���Ă��������B\n
	 * �擾����L�[�FHKEY_CURRENT_USER\\Software\\Increment P Corporation\\SiNDY-e\\Tools\\COMMON\UsePGDB
	 * @return PGDB���g�p����ꍇ��true
	 */
	bool GetUsePGDB() const { return m_bUsePGDB; }

	/**
	 * @brief <b>ini�t�@�C���ɂ�����Ă���T�[�o�����擾����</b>\n
	 * Load�ŏ���ǂݍ��񂾌�Ɏ��s���Ă��������B\n
	 * �擾����L�[�Fsettings.ini\\workdb\\servername\n
	 * �ʏ킱�̃T�[�o���̓��[���ڑ��p�̃T�[�o���ɂȂ�܂��B
	 * @return �T�[�o��
	 */
	LPCTSTR GetServerName() const { return m_strServerName; }

	/**
	 * @brief <b>ini�t�@�C���ɂ�����Ă���I��臒l���擾����</b>\n
	 * Load�ŏ���ǂݍ��񂾌�Ɏ��s���Ă��������B\n
	 * �擾����L�[�Fsettings.ini\\Tools\\Tolerance\n
	 * @return �I��臒l
	 */
	int GetTolerance() const;// { return m_nTolerance; }

	/**
	 * @brief <b>�ĕ`��͈͂̊g�������擾����</b>\n
	 * Load�ŏ���ǂݍ��񂾌�Ɏ��s���Ă��������B\n
	 * �擾����L�[�FHKEY_CURRENT_USER\\Software\\Increment P Corporation\\SiNDY-e\\Tools\\COMMON\\RefreshBuffer
	 * @return �ĕ`��͈͂̊g�����ipx�j
	 */
	int GetRefreshBuffer() const { return m_nRefreshBuffer; }

	/**
	 * @brief <b>�g���[�X���O�t�@�C�������擾����</b>\n
	 * Load�ŏ���ǂݍ��񂾌�Ɏ��s���Ă��������B\n
	 * �擾����L�[�FHKEY_CURRENT_USER\\Software\\Increment P Corporation\\SiNDY-e\\Tools\\COMMON\\TraceLogFileName
	 * @return �g���[�X���O�t�@�C���p�X
	 */
	LPCTSTR GetTraceLogFileName() const { return m_strTraceLogFileName; }

	/**
	 * @brief <b>�G���[���O�t�@�C�������擾����</b>\n
	 * Load�ŏ���ǂݍ��񂾌�Ɏ��s���Ă��������B\n
	 * �擾����L�[�FHKEY_CURRENT_USER\\Software\\Increment P Corporation\\SiNDY-e\\Tools\\COMMON\\ErrorLogFileName
	 * @return �G���[���O�t�@�C���p�X
	 */
	LPCTSTR GetErrorLogFileName() const { return m_strErrorLogFileName; }
	//@}

	//@{ @name �������݌n�֐�
	/**
	 * @brief <b>���[���p�̃p�[�\�i���E�W�I�f�[�^�x�[�X�̃p�X�����W�X�g���ɏ�������</b>\n
	 * �ݒ肷��L�[�FHKEY_CURRENT_USER\\Software\\Increment P Corporation\\SiNDY-e\\Tools\\COMMON\\PGDBFileName
	 * @param lpcszFileName		[in]	PGDB�t�@�C�����i�t���p�X�j
	 * @retval sindyErr_RegOpenFailed	���W�X�g���̃I�[�v���Ɏ��s����
	 * @retval sindyErr_RegWriteFailed	���W�X�g���̏������݂Ɏ��s����
	 * @retval sindyErr_NoErr			�������ݐ���
	 */
	bool SetPGDBFileName( LPCTSTR lpcszFileName );

	/**
	 * @brief <b>���[���f�[�^�x�[�X�Ƀp�[�\�i���E�W�I�f�[�^�x�[�X���g�p���邩�ǂ����̃t���O�����W�X�g���ɏ�������</b>\n
	 * �ݒ肷��L�[�FHKEY_CURRENT_USER\\Software\\Increment P Corporation\\SiNDY-e\\Tools\\COMMON\\UsePGDB
	 * @param bUsePGDB			[in]	PGDB�����[���Ɏg�p���邩�ǂ���
	 * @retval sindyErr_RegOpenFailed	���W�X�g���̃I�[�v���Ɏ��s����
	 * @retval sindyErr_RegWriteFailed	���W�X�g���̏������݂Ɏ��s����
	 * @retval sindyErr_NoErr			�������ݐ���
	 */
	bool SetUsePGDB( bool bUsePGDB );

	/**
	 * @brief <b>ini�t�@�C���ɑI��臒l��ݒ肷��</b>\n
	 * SiNDY-e�̃C���X�g�[���p�X���擾����K�v������̂ŁALoad�ŏ���ǂݍ��񂾌�Ɏ��s���Ă��������B\n
	 * �ݒ肷��L�[�Fsettings.ini\\Tools\\Tolerance\n
	 * @retval sindyErr_RegWriteFailed	�������݂Ɏ��s����
	 * @retval sindyErr_NoErr			�������ݐ���
	 */
	bool SetTolerance( int nTolerance );

	/**
	 * @brief <b>�ĕ`��̈�̊g������ݒ肷��</b>\n
	 * �ݒ肷��L�[�FHKEY_CURRENT_USER\\Software\\Increment P Corporation\\SiNDY-e\\Tools\\COMMON\\TraceLogFileName\\RefreshBuffer
	 * @retval sindyErr_RegWriteFailed	�������݂Ɏ��s����
	 * @retval sindyErr_NoErr			�������ݐ���
	 */
	bool SetRefreshBuffer( int nRefreshBuf );

	/**
	 * @brief <b>�g���[�X���O�t�@�C������ݒ肷��</b>\n
	 * �ݒ肷��L�[�FHKEY_CURRENT_USER\\Software\\Increment P Corporation\\SiNDY-e\\Tools\\COMMON\\TraceLogFileName
	 * @param lpcszFileName		[in]	�g���[�X���O�t�@�C����
	 * @retval sindyErr_RegOpenFailed	���W�X�g���̃I�[�v���Ɏ��s����
	 * @retval sindyErr_RegWriteFailed	���W�X�g���̏������݂Ɏ��s����
	 * @retval sindyErr_NoErr			�������ݐ���
	 */
	bool SetTraceLogFileName( LPCTSTR lpcszFileName );

	/**
	 * @brief <b>�G���[���O�t�@�C������ݒ肷��</b>\n
	 * �ݒ肷��L�[�FHKEY_CURRENT_USER\\Software\\Increment P Corporation\\SiNDY-e\\Tools\\COMMON\\ErrorLogFileName
	 * @param lpcszFileName		[in]	�G���[���O�t�@�C����
	 * @retval sindyErr_RegOpenFailed	���W�X�g���̃I�[�v���Ɏ��s����
	 * @retval sindyErr_RegWriteFailed	���W�X�g���̏������݂Ɏ��s����
	 * @retval sindyErr_NoErr			�������ݐ���
	 */
	bool SetErrorLogFileName( LPCTSTR lpcszFileName );
	//@]
	/**
	 * @brief ���W�X�g����settings.ini�̓��e�������o�ɃL���b�V������
	 * @return sindyErrCode
	 */
	bool Load();
private:
	CString	m_strInstallRoot;		//!< HKEY_LOCAL_MACHINE\\Software\\INCREMENT P CORPORATION\\SiNDY-e\\InstallRoot\\Path�̓��e�̃L���b�V��
	CString	m_strInitFileName;		//!< HKEY_LOCAL_MACHINE\\Software\\INCREMENT P CORPORATION\\SiNDY-e\\InstallRoot\\InitFileName�̓��e�̃L���b�V��
	CString	m_strVersion;			//!< HKEY_LOCAL_MACHINE\\Software\\INCREMENT P CORPORATION\\SiNDY-e\\Version\\Version�̓��e�̃L���b�V��
	CString	m_strServerName;		//!< settings.ini\\workdb\\servername�̓��e�̃L���b�V��
	long	m_lWarnLevel;			//!< HKEY_LOCAL_MACHINE\\Software\\INCREMENT P CORPORATION\\SiNDY-e\\Warning\\Level�̓��e�̃L���b�V��
	CString m_strPGDBFileName;		//!< HKEY_CURRENT_USER\\Software\\Increment P Corporation\\SiNDY-e\\Tools\\COMMON\\PGDBFileName�̓��e�̃L���b�V��
	bool	m_bUsePGDB;				//!< HKEY_CURRENT_USER\\Software\\Increment P Corporation\\SiNDY-e\\Tools\\COMMON\\UsePGDBs�̓��e�̃L���b�V��
	CString m_strTraceLogFileName;	//!< HKEY_CURRENT_USER\\Software\\Increment P Corporation\\SiNDY-e\\Tools\\COMMON\\TraceLogFileName�̓��e�̃L���b�V��
	CString m_strErrorLogFileName;	//!< HKEY_CURRENT_USER\\Software\\Increment P Corporation\\SiNDY-e\\Tools\\COMMON\\ErrorLogFileName�̓��e�̃L���b�V��
	int		m_nTolerance;			//!< settings.ini\\Tools\\Tolerance�̓��e�̃L���b�V��
	int		m_nRefreshBuffer;		//!< HKEY_CURRENT_USER\\Software\\Increment P Corporation\\SiNDY-e\\Tools\\COMMON\\RefreshBuffer�̓��e�̃L���b�V��
};

#endif // _SINDYESETTINGS_H_
