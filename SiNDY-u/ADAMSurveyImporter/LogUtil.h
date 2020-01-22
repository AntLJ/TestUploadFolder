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

#include <TDC/useful_headers/boost_program_options.h>

/**
 * �ėp���O�N���X
 */
class LogUtil
{
public:
	LogUtil(void);
	~LogUtil(void);

	/**
	 * �W���o�͂���
	 * 
	 * @param[in] filename	�o�͓��e
	 */
	static void print(const std::string& msg);
	static void print(const uh::tstring& msg);
	static void print(const std::ostream& msg);
	static void print(const boost::program_options::options_description& msg);
	static void print(long id, const std::string& errmsg);
	static void print(long id, const std::string& errmsg, const std::string& value,const std::string& target);

	static void flushLine(const std::string& msg);

	/**
	 * �f�o�b�N���e��W���o�͂���
	 * �v���t�B�b�N�X [DEBUG] ��t���ĕW���o��
	 * 
	 * @param[in] filename	�o�͓��e
	 */
	static void printDebug(const std::string& msg);
	static void printDebug(const uh::tstring& msg);
	static void printDebug(const std::ostream& msg);
	static void printDebug(const boost::program_options::options_description& msg);
	static void printDebug(long id, const std::string& errmsg);
	static void printDebug(long id, const std::string& errmsg, const std::string& value,const std::string& target);

	/**
	 * �G���[���e��W���o�͂���
	 * �v���t�B�b�N�X [WAR] ��t���ĕW���o��
	 * 
	 * @param[in] filename	�o�͓��e
	 */
	static void printWarning(const std::string& msg);
	static void printWarning(const uh::tstring& msg);
	static void printWarning(const std::ostream& msg);
	static void printWarning(const boost::program_options::options_description& msg);
	static void printWarning(long id, const std::string& errmsg);
	static void printWarning(long id, const std::string& errmsg, const std::string& value,const std::string& target);

	/**
	 * �G���[���e��W���o�͂���
	 * �v���t�B�b�N�X [ERR] ��t���ĕW���o��
	 * 
	 * @param[in] filename	�o�͓��e
	 */
	static void printError(const std::string& msg);
	static void printError(const uh::tstring& msg);
	static void printError(const std::ostream& msg);
	static void printError(const boost::program_options::options_description& msg);
	static void printError(long id, const std::string& errmsg);
	static void printError(long id, const std::string& errmsg, const std::string& value,const std::string& target);
	
	/**
	 * ���O���e���t�@�C���o�͂���
	 *
	 * @note �o�͐�́A���s�t�@�C���Ɠ�Dir
	 * �u./ADAM_AuthoringLog_YYYYMMDDhhmmss.txt�v
	 */
	static void write();
	
	/**
	 * ���O���e���t�@�C���o�͂���
	 *
	 * @param[in] dir �o�̓f�B���N�g��
	 */
	static void write(const uh::tstring& dir);

	/**
	 * ���O�o�̓f�B���N�g����ݒ肷��
	 *
	 * @param[in] dir �o�̓f�B���N�g��
	 */
	static void setLogDirPath(const uh::tstring& dir);

	/**
	 * ERR���O�������擾����
	 */
	static long getErrCount();

	/**
	 * WAR���O�������擾����
	 */
	static long getWarCount();

	/**
	 * ��(-----------------)���o�͂���
	 */
	static void printLine();

	/**
	 * ��d��(=====================)���o�͂���
	 */
	static void printDoubleLine();

	/**
	 * �p�����[�^���o�͂���
	 *
	 * @param[in]	param	�p�����[�^�l
	 * @param[in]	value	�ݒ�l
	 */
	static void printParam(const uh::tstring& param, const uh::tstring& value);
	static void printParam(const std::string& param, const std::string& value);

	/**
	 * �t�B�[�`�����Ɛڑ���Ԃ��o�͂���
	 *
	 * @param[in]	name	�t�B�[�`����(�a��)
	 * @param[in]	table	�t�B�[�`����
	 * @param[in]	state	�ڑ����(true:�����Afalse:���s)
	 */
	static void printTableState(const uh::tstring& name, const uh::tstring& table, bool state);
	static void printTableState(const std::string& name, const std::string& table, bool state);
	static void printTableState(long id, const std::string& table, bool state);

	/**
	 * �t�B�[�`�����Ɛڑ���Ԃ��o�͂���
	 *
	 * @param[in]	name	�t�B�[�`����(�a��)
	 * @param[in]	table	�t�B�[�`����
	 * @param[in]	state	�ڑ���Ԗ�
	 */
	static void printTableState(const uh::tstring& name, const uh::tstring& table, const uh::tstring& state);
	static void printTableState(const std::string& name, const std::string& table, const std::string& state);

private:
	/**
	 * ���O���e���t�@�C���o�͂���
	 *
	 * @param filename [in]�t�@�C���p�X
	 */
	static void _writeLog(const uh::tstring& filename);
private:
	static std::stringstream _strLogMsg;
	static uh::tstring _dirPath;
	static long _errCount;
	static long _warCount;
};

