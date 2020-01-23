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
#include <memory>
#include <fstream>

/// �������ԕێ����v�Z�p
struct ExecTime
{
	time_t m_startTime; //!< �J�n����
	time_t m_endTime;   //!< �I������
};

enum ErrorCode
{
	info = 0,
	warning = 1,
	error = 2,
	critical = 3,
};

class CLogFile
{
public:
	CLogFile(void){};
	CLogFile( const CString& baseDirectory );
	~CLogFile(void);

	CStringA ChangeErrorCode2ErrorLevel( ErrorCode code )
	{
		CStringA level;
		switch( code )
		{
		case info:
			level = "INFO";
			break;
		case warning:
			level = "WARNING";
			break;
		case error:
			level = "ERROR";
			break;
		case critical:
			level = "CRITICAL";
			break;
		default:
			break;
		}
		return level;
	}

	/**
	 * @brief SINDYSTDLOG�`���ŏo��
	 * @param layer [in] ���C����
	 * @param objectId [in] �I�u�W�F�N�gID
	 * @param errorCode [in] �G���[�R�[�h
	 * @param message [in] �G���[���b�Z�[�W
	 * @param meshCode [in] ���b�V���R�[�h
	 */
	void WriteLog( const CString& layer, long objectId, ErrorCode errorCode, const CString& message, long meshCode );

	void WriteLogCore(long meshCode, const CStringA& errLog);

	void WriteLog(
		const CString& layer, long objectId, double lon, double lat, ErrorCode errorCode,
		const CString& message, long meshCode );

	/**
	 * @brief ���O�t�@�C���쐬
	 * @param meshCode [in] ���b�V���R�[�h
	 */
	void InitLogFile( long meshCode );

	/**
	 * @brief ���O�t�@�C������
	 * @param [in] ofs ���O�t�@�C���o�̓X�g���[��
	 */
	void CloseLogFile();

private:
	CString m_logDirctory; //!< ���O�ۑ��f�B���N�g��
	std::map<long,ExecTime> m_execMesh; //!< �����Ώۃ��b�V���R�[�h
	// TODO: �eexecuteALL()���Ń��O���悤�ɂ���΁A�����Ώۃ��b�V���R�[�h��S�������Ȃ��Ă��ǂ��Ȃ�̂ŁA�������Ă݂�
};

