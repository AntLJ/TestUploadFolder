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
#include "Logger.h"
#include "Arguments.h"
#include "MeshType.h"

/**
 * @brief ���s���O�Ǘ��N���X
 */
class CRunLogger :
	public CLogger
{
public:
	explicit CRunLogger(const uh::tstring& path) : CLogger(path){}

	/**
	 * @brief �w�b�_���o�͂���
	 * @note �o�C�i���̃o�[�W������A���s���Ɏw�肳�ꂽ�I�v�V�������o�͂���
	 * @param args[in] �R�}���h���C�������E���b�V���R�[�h�Ǘ��N���X
	 */
	void header( const Arguments& args );

	/**
	 * @brief ���b�V�����X�g�̉�͌��ʂ��o�͂���
	 * @note �������郁�b�V���P�ʂ��o�͂��A����΃��b�V�����X�g���̕s���ȕ�������o��
	 * @param type[in] �������b�V���P��
	 * @param mapInvalidRow[in] ���b�V�����X�g���̂��ׂĂ̕s���ȕ�����
	 */
	void writeParseMeshResult( const meshtype::MeshType& type, const std::map<int, CString>& mapInvalidRow );

	/**
	 * @brief �J�n�E�I�����Ԃ��o�͂���
	 * @note isStart�̒l�ɉ����āA�J�n�������I���������𔻒肷��
	 * @param isStart[in] true�Ȃ�J�n�����Afalse�Ȃ�I������
	 */
	void writeExeTime( bool isStart );

	/**
	 * @brief �G���[���b�Z�[�W���o�͂���
	 * @note �������ɔ��������G���[�̓��e���o�͂���
	 * @param msg[in] �G���[���b�Z�[�W
	 */
	void writeErrMsg( const CString& msg );

	/**
	 * @brief ���O�t�@�C���ƃR���\�[���̗����Ƀ��b�Z�[�W���o�͂���
	 * @param msg[in] ���b�Z�[�W
	 */
	void writeLogAndConsole( const CString& msg );
};

