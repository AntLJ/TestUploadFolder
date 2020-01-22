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

#include <fstream>

class CheckLog {
private:
	~CheckLog() {}
	CheckLog ();

	CheckLog(const CheckLog& );
	CheckLog& operator = (const CheckLog&);

public:
	static CheckLog* GetInstance() {
		static CheckLog kInstance;
		return &kInstance;
	}


public:
	void printError(
				long                code,       // �G���[�R�[�hNo
				const std::string&  info,       // �G���[�R�[�h�ɑΉ����郁�b�Z�[�W
				const std::string&  file,       // �ǂݍ���ł���G�C���A�X��
				long                oid,        // OBJECTID
				const std::string&  field,      // �G���[���̃t�B�[���h��
				const std::string&  msg         // �G���[���̓��͒l/��肤��l�A��
	);

	void printWarning(
				long                code,       // �G���[�R�[�hNo
				const std::string&  info,       // �G���[�R�[�h�ɑΉ����郁�b�Z�[�W
				const std::string&  file,       // �ǂݍ���ł���G�C���A�X��
				long                oid,        // OBJECTID
				const std::string&  field,      // �G���[���̃t�B�[���h��
				const std::string&  msg         // �G���[���̓��͒l/��肤��l�A��
	);

	void printInfo(
				long                code,       // �G���[�R�[�hNo
				const std::string&  info,       // �G���[�R�[�h�ɑΉ����郁�b�Z�[�W
				const std::string&  file,       // �ǂݍ���ł���G�C���A�X��
				long                oid,        // OBJECTID
				const std::string&  field,      // �G���[���̃t�B�[���h��
				const std::string&  msg         // �G���[���̓��͒l/��肤��l�A��
	);


protected:
	void print(
				long                level,      // �G���[���x�� (ERR/WAR/INF/DEL)
				long                code,       // �G���[�R�[�hNo
				const std::string&  info,       // �G���[�R�[�h�ɑΉ����郁�b�Z�[�W
				const std::string&  file,       // �ǂݍ���ł���G�C���A�X��
				long                oid,        // OBJECTID
				const std::string&  field,      // �G���[���̃t�B�[���h��
				const std::string&  msg         // �G���[���̓��͒l/��肤��l�A��
	);


private:
	std::ofstream   m_cStream;
};

#define LOGCODE_ERROR_COLUMNS           (3)
#define LOGCODE_ERROR_FORMAT            (1)
#define LOGCODE_ERROR_INVALID           (2)
#define LOGCODE_ERROR_OID_ASC           (4)


#define LOGMSG_ERROR_COLUMNS            "�񐔃`�F�b�N�G���["
#define LOGMSG_ERROR_FORMAT             "�����`�F�b�N�G���["
#define LOGMSG_ERROR_INVALID            "�����l�`�F�b�N�G���["
#define LOGMSG_ERROR_OID_ASC            "OID�����`�F�b�N�G���["

