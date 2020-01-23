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
#include <libpq-fe.h>
#include<TDC/nsdb/PgDatabase.h>
#include <boost/format.hpp>

/**
 *	@class PgDatabaseEx
 *	@brief PgDatabase�̋@�\�g��
 *  �R�s�[�����g�p�����f�[�^�C���|�[�g�@�\�ǉ�
 */
class PgDataBaseEx :
	public ns::PgDatabase
{
public:
	/**
	 *	@brief	�R���X�g���N�^
	 *	@param	conninfo [in] �ڑ���񕶎���
	 */
	PgDataBaseEx(const char *conninfo) : ns::PgDatabase(conninfo) {}
	/**
	 *	@brief	�f�X�g���N�^
	 */
	~PgDataBaseEx(void) {}

	/**
	 *	@brief	�R�s�[�J�n
	 *	@note	�R�s�[�J�n��̓R�s�[���ȊO��SQL�͎��s���Ă͂����Ȃ��B�R�s�[�v���Z�X���I������B
	 *	@param	tablename [in] �e�[�u����
	 *	@param	fields [in] �t�B�[���h��(�J���}��؂�)
	 *	@retval true ����
	 *	@retval	false ���s
	 */
	bool StartCopy(const char *tablename, const char *fields) {
		std::string sql = boost::str(boost::format("COPY %s (%s) FROM STDIN;") % tablename % fields);
		PGconn *conn = getConn();
		PGresult *result = PQexec(conn, sql.c_str());
		if (! result || (PQresultStatus(result) != PGRES_COPY_IN)) {
			setError(PQerrorMessage(conn));
			PQclear(result);
			return false;
		}
		PQclear(result);
		return true;
	}
	/**
	 *	@brief	�R�s�[�I��
	 *	@note	�R�s�[�������ɌĂяo��
	 *	@retval true ����
	 *  @reval  false ����
	 */
	bool EndCopy() {
		PGconn *conn = getConn();
		int result = PQputCopyEnd(conn, NULL);
		switch (result) {
		case 0:
			setError("Writing COPY data blocked.");
			return false;
		case -1:
			setError(PQerrorMessage(conn));
			return false;
		default:
			break;
		}
		PGresult *res = PQgetResult(conn);
		if (res && PQresultStatus(res) != PGRES_COMMAND_OK) {
			setError(boost::str(boost::format("COPY statement failed.\n%s") % PQerrorMessage(conn)).c_str());
			PQclear(res);
			return false;
		}
		PQclear(res);
		return true;
	}
	/**
	 *	@brief	�f�[�^�̃R�s�[
	 *	@note	���O��startCopy���Ă΂�Ă��邱�Ƃ��O��
	 *	@param	data [in] �R�s�[����f�[�^
	 *  @param	size [in] �f�[�^�T�C�Y(data�̒���)
	 *  @retval true ����
	 *  @retval false ���s
	 */
	bool copyData(const char *data, unsigned long size) {
		PGconn *conn = getConn();
		int result = PQputCopyData(conn, data, size);
		switch (result) {
		case 0:
			setError("Writing COPY data blocked.");
			return false;
		case -1:
			setError(PQerrorMessage(conn));
			return false;
		default:
			break;
		}
		return true;
	}

	/**
	 *	@brief	�f�[�^�x�[�X���̎擾
	 *  @return �ڑ��f�[�^�x�[�X��
	 */
	std::string getDBName() {
		std::string retval = PQdb(getConn());
		return retval;
	}
	/**
	 *	@brief	�z�X�g���̎擾
	 *	@return	�z�X�g��
	 */
	std::string getHostName() {
		std::string retval = PQhost(getConn());
		return retval;
	}
	/**
	 *	@brief	�|�[�g�ԍ��̎擾</b>\n
	 *	@return	�|�[�g�ԍ�
	 */
	std::string getPort() {
		std::string retval = PQport(getConn());
		return retval;
	}
	/**
	 *	@brief	���[�U�����擾</b>\n
	 *	@return	���[�U��
	 */
	std::string getUser() {
		std::string retval = PQuser(getConn());
		return retval;
	}
	/**
	 *  @brief	�p�X���[�h���擾����
	 *	@return	�p�X���[�h
	 */
	std::string getPassword() {
		std::string retval = PQpass(getConn());
		return retval;
	}
};

