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
#include <sindy/schema.h>

/**
 * @brief �G���[�Ȃǂ̕񍐗p�@�\�Q
 * @note	�\�[�X�̌��ʂ���ǂ����邽�߁A�񍐗p�@�\�͂����ɏW�񂷂�B
 */
namespace notifier
{
	using namespace std;
	static std::string fd = "\t";	// �t�B�[���h�f���~�^

	// �G���[���x���񋓌^
	namespace error_level {
		enum ECode {
			fatal,
			error,
			warning,
			notice
		};
	}

	/**
	 * @brief �^����ꂽ�G���[���x���ɑΉ�����G���[���x���������Ԃ�
	 * @param[in]	cErrorLevel	�G���[���x��
	 * @return	�G���[���x��������
	 */
	inline string getErrorLevelStr( error_level::ECode cErrorLevel )
	{
		string aErrorLevelStr;
		switch (cErrorLevel)
		{
			using namespace error_level;
			case fatal:
				aErrorLevelStr = "FATAL";
				break;
			case error:
				aErrorLevelStr = "ERROR";
				break;
			case warning:
				aErrorLevelStr = "WARNING";
				break;
			case notice:
				aErrorLevelStr = "NOTICE";
				break;
			default:
				ATLASSERT(0);
				break;
		}
		return aErrorLevelStr;
	}

	/**
	 * @brief �v���I�ȃG���[�������������ɌĂ΂��֐��Q
	 * @note	std::runtime_error��O�𔭐������ăv���O�����������I��������
	 */
	namespace critical
	{
		inline void asCannotGetWorkspace( std::string cDBStr )
		{
			throw std::runtime_error(
				std::string("���[�N�X�y�[�X�̎擾�Ɏ��s") + fd + cDBStr);
		}

		inline void asCannotGetTable( std::string cTableName )
		{
			throw std::runtime_error(
				std::string("�e�[�u���̎擾�Ɏ��s") + fd + cTableName);
		}

		inline void asCannotOpenMeshListFile(std::string cFilePath)
		{
			throw std::runtime_error(
				std::string("���b�V�����X�g�̃I�[�v���Ɏ��s") + fd + cFilePath);
		}

		inline void asMeshListFileIsInvalid(long cLineNo)
		{
			throw std::runtime_error(
				std::string("���b�V�����X�g���s��") + fd + "Line " + boost::lexical_cast<std::string>(cLineNo));
		}

		inline void asCannotOpenLogFile(std::string cPath)
		{
			throw std::runtime_error(
				std::string("���O�t�@�C���̃I�[�v���Ɏ��s") + fd + cPath);
		}

		inline void asCannotCreateDirectory( std::string cPath )
		{
			throw std::runtime_error(
				std::string("�o�̓f�B���N�g���̍쐬�Ɏ��s") + fd + cPath);
		}

		inline void asCannotSetSpatialReference()
		{
			throw std::runtime_error(
				std::string("��ԎQ�Ƃ̃Z�b�g�Ɏ��s"));
		}
	}

	/**
	 * @brief �ی��t��Assertion
	 * @note	Release�łł��C�t����悤�ɁA�g�p�Ҍ����̃G���[�o�͂�Assertion���s���ɍs���B
	 * @note	�`�F�b�N�����S�ɂȂ����ɁA�u�p�t�H�[�}���X���]���ɂȂ�̂�Debug�ł̂݁v�Ƃ������_�����Ȃ���̂ŁA
	 *			�g�p�̍ۂ͗��ӂ��邱�ƁB
	 * @param[in]		cBool	Assertion���e��\���^�U�l
	 * @param[in,out]	co		�o�̓X�g���[��
	 */
	inline void safeAssert( bool cBool, ostream& co )
	{
		// �������^�Ȃ牽�����Ȃ�
		if (cBool)
			{ return; }

		// �������U�Ȃ�AAssertion�̎��s�ƃG���[�o�͂��s��
		ATLASSERT(0);
		co << "#" << fd << getErrorLevelStr( error_level::fatal )
			<< fd << "�z��O�̃G���[�ł��B���ꂪ�o�͂��ꂽ��A�J���҂ɘA�����Ă��������B" << endl;
	}

	/**
	 * @brief [Bug7560] ���b�V���P�ʂ̏������ɗ�O�����������ꍇ�̃G���[���b�Z�[�W�o��
	 * @param[in,out]	co			�o�̓X�g���[��
	 * @param[in]		cMeshCode	�ΏۂQ�����b�V���R�[�h
	 */
	inline void asAbortedByUnexpectedData( ostream& co, long cMeshCode )
	{
		co << CT2A( sindy::schema::basemesh::kTableName ) << fd << cMeshCode << fd << getErrorLevelStr( error_level::fatal )
			<< fd << "�c�[�����\�����Ă��Ȃ��f�[�^�̑��݂ɂ��A���̃��b�V���̏����͒��f����܂����B" << endl;
	}

	/**
	 * @brief [Bug9292] �w�肳�ꂽ���b�V���ɂ��āA�܂Ƃ߂ď�������ۂɗ�O�����������ꍇ�̃G���[���b�Z�[�W�o��
	 * @param[in,out]	co			�o�̓X�g���[��
	 */
	inline void asAbortedByUnexpectedData( ostream& co )
	{
		co << CT2A( sindy::schema::basemesh::kTableName ) << fd << getErrorLevelStr( error_level::fatal )
			<< fd << "�c�[�����\�����Ă��Ȃ��f�[�^�̑��݂ɂ��A�����͒��f����܂����B" << endl;
	}

	/**
	 * @brief ��Ԃ��P���Ȃ����b�V����Shape�t�@�C���쐬���X�L�b�v��������ʒm����
	 * @param[in,out]	co			�o�̓X�g���[��
	 * @param[in]		cMeshCode	���Y���b�V���R�[�h
	 */
	inline void asMeshWithNoSection( ostream& co, long cMeshCode )
	{
		co << CT2A( sindy::schema::basemesh::kTableName ) << fd << cMeshCode << fd << getErrorLevelStr( error_level::notice )
			<< fd << "��Ԃ��P���쐬����Ȃ������̂ŁAShape�t�@�C���̍쐬���X�L�b�v����܂����B" << endl;
	}

	/**
	 * @brief �Ώۃ����N�̐���0���ł���������ʒm����
	 * @param[in,out]	co			�o�̓X�g���[��
	 * @param[in]		cMeshCode	���Y���b�V���R�[�h
	 */
	inline void asLinkNotFound( ostream& co, long cMeshCode )
	{
		co << CT2A( sindy::schema::basemesh::kTableName ) << fd << cMeshCode << fd << getErrorLevelStr( error_level::notice )
			<< fd << "�Ώۃ����N����0���ł��B" << endl;
	}

} // namespace notifier
