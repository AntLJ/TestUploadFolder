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
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ArcHelperEx/AheGlobalsMisc.h>
#include <boost/optional.hpp>

/**
 * @brief �G���[�Ȃǂ̕񍐗p�@�\�Q
 * @note	�\�[�X�̌��ʂ���ǂ����邽�߁A�񍐗p�@�\�͂����ɏW�񂷂�B
 */
namespace notifier
{
	using namespace std;
	static string fd = "\t";
	static wstring wfd = L"\t";

	/// �G���[���x���񋓌^
	enum class error_level {
		fatal,
		error,
		warning,
		notice,
		none,
	};

	/**
	 * @brief �^����ꂽ�G���[���x���ɑΉ�����G���[���x���������Ԃ�
	 * @param[in]	errorLevel	�G���[���x��
	 * @return	�G���[���x��������
	 */
	inline string getErrorLevelStr( error_level errorLevel )
	{
		string aErrorLevelStr;
		switch (errorLevel)
		{
			case error_level::fatal:
				aErrorLevelStr = "FATAL";
				break;
			case error_level::error:
				aErrorLevelStr = "ERROR";
				break;
			case error_level::warning:
				aErrorLevelStr = "WARNING";
				break;
			case error_level::notice:
				aErrorLevelStr = "NOTICE";
				break;
			default:
				assert(0);
				break;
		}
		return aErrorLevelStr;
	}

	/**
	 * @brief �^����ꂽ������ɑΉ�����G���[���x����Ԃ�
	 * @warning �召�����͋�ʂ��Ȃ�
	 * @param[in]	errorLevelStr	�G���[���x��������
	 * @return	�G���[���x��
	 */
	inline error_level getErrorLevel( const std::string& errorLevelStr )
	{
		if(0==stricmp(errorLevelStr.c_str(), "FATAL"))
			return error_level::fatal;
		if(0==stricmp(errorLevelStr.c_str(), "ERROR"))
			return error_level::error;
		if(0==stricmp(errorLevelStr.c_str(), "WARNING"))
			return error_level::warning;
		if(0==stricmp(errorLevelStr.c_str(), "NOTICE"))
			return error_level::notice;
		return error_level::none;
	}

	/**
	 * @brief �v���I�ȃG���[�������������ɌĂ΂��֐��Q
	 * @exception std::runtime_error��O�𔭐������ăv���O�����������I��������B
	 */
	namespace critical
	{
		/**
		 * @brief �ėp�G���[�����֐�
		 * @note �w�肳�ꂽ�G���[���b�Z�[�W������O�𑗏o����B
		 * @param[in]	message	�G���[���b�Z�[�W
		 */
		inline void versatile( const uh::tstring& message )
		{
			wcerr << L"# ERROR : " << message << endl;
			wcout << L"# �ُ�I��" << endl;

			throw std::runtime_error( uh::toStr(message) );
		}

		/**
		 * @param[in]	tableName	�e�[�u����
		 */
		inline void asCannotOpenTable( const uh::tstring& tableName )
		{
			versatile(
				_T("�e�[�u���̃I�[�v���Ɏ��s") + uh::toTStr(fd) + tableName );
		}

		/**
		 * @param[in]	path	���O�t�@�C���̃p�X
		 */
		inline void asCannotOpenLogFile(const uh::tstring& path)
		{
			versatile(
				_T("���O�t�@�C���̃I�[�v���Ɏ��s") + uh::toTStr(fd) + path );
		}

		/**
		 * @param[in]      db    �Ώ�DB�ڑ��敶����
		 */
		inline void asCannotConnectDB( const uh::tstring& db )
		{
			std::stringstream ss;
			ss << "DB�ւ̐ڑ����s : \"" << uh::toStr( db ) << "\"";
			versatile( uh::toTStr(ss.str()) );
		}
	} // namespace critical


	/**
	 * @brief �ėp�ʒm�����֐�
	 * @note �w�肳�ꂽ�ʒm���b�Z�[�W���o�͂��A�����𑱍s����B
	 * @param[in]  message  �ʒm���b�Z�[�W
	 * @param[in]  wos      �o�̓X�g���[��
	 */
/*	inline void versatile(
		const uh::tstring& message,
		std::wostream& wos = wcerr
	)
	{
		wos << L"# " << message << endl;
	}*/

	/**
	 * @brief �ėp�ʒm�����֐�
	 * @note �w�肳�ꂽ�ʒm���b�Z�[�W���o�͂��A�����𑱍s����B
	 * @param[in]  message  �ʒm���b�Z�[�W
	 * @param[in]  wos      �o�̓X�g���[��
	 */
	inline void versatile(
		const uh::tstring& message,
		std::wostream& wos = wcerr
	)
	{
		wos << L"# " << message.c_str() << endl;
	}

	/**
	 * @brief �I�u�W�F�N�g�̏����o�͂���
	 * @note ������SINDYSTDLOG�B�o�͂���K�v�̂Ȃ���ɂ͉����o�͂��Ȃ��B
	 * @param[in]      tableName  �e�[�u����
	 * @param[in]      oid        �I�u�W�F�N�gID�i�o�͂��Ȃ��ꍇ�͖����l�j
	 * @param[in]      x          �o�x�i�o�͂��Ȃ��ꍇ�͖����l�j
	 * @param[in]      y          �ܓx�i�o�͂��Ȃ��ꍇ�͖����l�j
	 * @param[in]      errorLevel �G���[���x��
	 * @param[in]      errorCode  �G���[�R�[�h
	 * @param[in]      errorMsg   �G���[���b�Z�[�W
	 * @param[in]      meshcode   ���b�V���R�[�h
	 * @param[in,out]  wos        �o�͐�X�g���[��
	 */
	inline void reportObjInfo(
		const uh::tstring& tableName,
		boost::optional< long > oid,
		boost::optional< double > x,
		boost::optional< double > y,
		boost::optional< error_level > errorLevel,
		boost::optional< const uh::tstring& > errorCode,
		const uh::tstring& errorMsg,
		boost::optional< const uh::tstring& > meshcode,
		std::wostream& wos
	)
	{
		// ���W�̗L�������w��
		wos << std::setprecision( 20 );

		// �Ή���
		wos << wfd;
		// ���C����
		wos << uh::toWStr( tableName ) << wfd;
		// �I�u�W�F�N�gID
		if ( oid )
			wos << *oid << wfd;
		else
			wos << wfd;
		// �o�x
		if( x )
			wos << *x << wfd;
		else
			wos	<< wfd;
		// �ܓx
		if( y )
			wos << *y << wfd;
		else
			wos << wfd;
		// �G���[���x��
		if ( errorLevel )
			wos << uh::toWStr( getErrorLevelStr( *errorLevel ) );
		wos << wfd;
		// �G���[�R�[�h
		if ( errorCode )
			wos << *errorCode;
		wos << wfd;
		// �G���[���b�Z�[�W
		wos << uh::toWStr( errorMsg );
		// ���b�V���R�[�h(���1)
		if ( meshcode )
			wos << wfd << uh::toWStr(*meshcode);

		wos <<  endl;
		// �}�j�s�����[�^�����ɖ߂�
		wos << resetiosflags(ios_base::floatfield);
	}

	/**
	 * @brief �I�u�W�F�N�g�̏����o�͂���
	 * @note ������SINDYSTDLOG�B�o�͂���K�v�̂Ȃ���ɂ͉����o�͂��Ȃ��B
	 * @param[in]      tableName  �e�[�u����
	 * @param[in]      oid        �I�u�W�F�N�gID�i�o�͂��Ȃ��ꍇ�͖����l�j
	 * @param[in]      x          X���W�i�o�͂��Ȃ��ꍇ�͖����l�j
	 * @param[in]      y          Y���W�i�o�͂��Ȃ��ꍇ�͖����l�j
	 * @param[in]      errorLevel �G���[���x��
	 * @param[in]      errorMsg   �G���[���b�Z�[�W
	 * @param[in,out]  wos        �o�͐�X�g���[��
	 */
	inline void reportObjInfo(
		const uh::tstring& tableName,
		boost::optional< long > oid,
		boost::optional< double > x,
		boost::optional< double > y,
		boost::optional< error_level > errorLevel,
		const uh::tstring& errorMsg,
		std::wostream& wos)
	{
		reportObjInfo( tableName, oid, x, y, errorLevel, nullptr, errorMsg, nullptr, wos );
	}


	/**
	 * @brief �I�u�W�F�N�g�̏����o�͂���
	 * @note ������SINDYSTDLOG�B�o�͂���K�v�̂Ȃ���ɂ͉����o�͂��Ȃ��B
	 * @param[in]      tableName  �e�[�u����
	 * @param[in]      oid        �I�u�W�F�N�gID�i�o�͂��Ȃ��ꍇ�͖����l�j
	 * @param[in]      errorLevel �G���[���x��
	 * @param[in]      errorMsg   �G���[���b�Z�[�W
	 * @param[in,out]  wos        �o�͐�X�g���[��
	 */
	inline void reportObjInfo(
		const uh::tstring& tableName,
		boost::optional< long > oid,
		boost::optional< error_level > errorLevel,
		const uh::tstring& errorMsg,
		std::wostream& wos
	)
	{
		reportObjInfo( tableName, oid, nullptr, nullptr, errorLevel, errorMsg, wos );
	}

	/**
	 * @brief �I�u�W�F�N�g�̏����o�͂���i��G���[�I�u�W�F�N�g�p�j
	 * @param[in]      tableName  �e�[�u����
	 * @param[in]      oid        �I�u�W�F�N�gID�i�o�͂��Ȃ��ꍇ�͖����l�j
	 * @param[in]      msg        ���b�Z�[�W�i�G���[�ł͂Ȃ����ASINDYSTDLOG�́u�G���[���b�Z�[�W�v��ɏo�͂���j
	 * @param[in,out]  wos        �o�͐�X�g���[��
	 */
	inline void reportObjInfo(
		const uh::tstring& tableName,
		boost::optional< long > oid,
		const uh::tstring& msg,
		std::wostream& wos
	)
	{
		reportObjInfo(
			tableName,
			oid,
			boost::none,
			msg,
			wos
		);
	}

} // namespace notifier
