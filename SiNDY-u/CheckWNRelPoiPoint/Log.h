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

#include "stdafx.h"
#include "Common.h"
#include "Param.h"

#define RUN_LOG(msg) CLog::WriteRunLog(msg)
#define ERR_LOG(layer, nid, pid, mesh, msg) CLog::WriteErrLog(layer, nid, pid, mesh, LOG_ERROR, msg)

#define NULL_CHAR	""
#define DSP_OK		"OK"
#define DSP_NG		"NG"
#define DSP_HYPHEN	"-"

enum LOG_TYPE
{
	ETYPE_RUN,
	ETYPE_ERR
};

/**
 * @brief ���O�o�͗p�N���X
 */
/**
 * @class CLog
 * @brief ���O�o�͗p�N���X\n
 * @note  ���s���O�E�G���[���O���o�͂���
 */
class CLog
{
public:
	CLog(void);
	~CLog(void);

	/**
	 * @brief	���O������
	 * @param	cParam	[in]	���̓p�����[�^
	 * @return					��������
	 */
	static bool initLog(CParam& cParam);

	/**
	 * @brief	���s���O������
	 * @param	strMsg	[in]	�o�̓��b�Z�[�W
	 */
	static void WriteRunLog(const std::string& strMsg);

	/**
	 * @brief	�G���[���O������
	 * @param	sLayer		[in]	���C��
	 * @param	sNodeId		[in]	NODE ID
	 * @param	sPoiId		[in]	POI ID
	 * @param	sMeshCode	[in]	���b�V���R�[�h
	 * @param	sErrType	[in]	�G���[�^�C�v
	 * @param	strMsg		[in]	�o�̓��b�Z�[�W
	 */
	static void WriteErrLog(const std::string& sLayer,
							const std::string& sNodeId,
							const std::string& sPoiId,
							const std::string& sMeshCode,
							const std::string& sErrType,
							const std::string& strMsg);

	/**
	 * @brief	���s���O������(�����J�n)
	 * @param	sysTime	[in]	�J�n����
	 */
	static void WriteRunLogSTART(const SYSTEMTIME& sysTime);

	/**
	 * @brief	���s���O������(�����I��)
	 * @param	sysTime	[in]	�I������
	 * @param	csTotal	[in]	��������
	 */
	static void WriteRunLogEND(const SYSTEMTIME& sysTime, const CString& csTotal);

	/**
	 * @brief	���s���O������(���s����)
	 * @param	strMsg	[in]	�o�̓��b�Z�[�W
	 */
	static void WriteRunLogNotNewLine(const std::string& strMsg);

private:
	/**
	 * @brief	���s���O������(����)
	 * @param	sysTime		[in]	����
	 * @param	sMessage	[in]	�������e
	 * @param	sTotal		[in]	��������
	 */
	static void WriteRunLogTIME(const SYSTEMTIME& sysTime,
								const CString& csMessage,
								const CString& csTotal);

	/**
	 * @brief	���O�t�@�C���I�[�v��
	 * @param	eType	[in]	���O�^�C�v
	 * @param	strPath	[in]	���O�o�̓f�B���N�g��
	 * @return					��������
	 */
	static bool OpenLog(const LOG_TYPE eType, const std::string& strPath);

public:
	static std::ofstream	m_ofsRunLog;
	static std::ofstream	m_ofsErrLog;	
	static bool				m_bRunLog;		// ���s���O���݃`�F�b�N�p
	static bool				m_bErrLog;		// �G���[���O���݃`�F�b�N�p
};
