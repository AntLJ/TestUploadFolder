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

class CLogManager
{
public:
	CLogManager(void);
	~CLogManager(void);

	/**
	 * @brief �����ݒ�N���X 2014/5/27���݃��O�I�[�v�����邾��
	 * @param logPath [in] ���O�t�@�C���̃p�X
	 * @return �����ݒ�̐���
	 */
	bool Init( const CString& logPath );

	/**
	 * @brief �w�b�_�o�� (���o���O)
	 */
	void WriteExtractLogHeader();

	/**
	 * @brief �w�b�_�o�� (�`�F�b�N���O)
	 */
	void WriteCheckLogHeader();

	/**
	 * @brief ���O�ɕ�����o�͂���֐�
	 * @param [in] message ������
	 */
	void WriteInfo( LPCSTR message, ... );
	
	/**
	 * @brief ���O�o�͊֐� (���o���O)
	 * @param [in] layer    ���C����
	 * @param [in] oid      OBJECTID
	 * @param [in] meshcode ���b�V���R�[�h
	 * @param [in] errLevel �G���[���x��
	 * @param [in] message  ���b�Z�[�W
	 * @param [in] linkInfo �����N����
	 */
	void WriteExtractLog( const CStringA& layer, long oid, long meshcode, const CStringA& errLevel, const CStringA& message, const CStringA& linkInfo );

	/**
	 * @brief ���O�o�͊֐� (�`�F�b�N���O)
	 * @param [in] layer    ���C����
	 * @param [in] oid      OBJECTID
	 * @param [in] errCode  �G���[�R�[�h
	 * @param [in] lon      �o�x
	 * @param [in] lat      �ܓx
	 * @param [in] meshcode 2�����b�V���R�[�h
	 * @param [in] linkID   ���������NID
	 * @param [in] nodeID   ���o�m�[�hID
	 */
	void WriteCheckLog( const CStringA& layer, long oid, long errorCode, double lon, double lat, long meshcode, long linkID, long nodeID );

private:
	/**
	 * @brief ���O�t�@�C���I�[�v��
	 * @param [in] logPath ���O�p�X
	 * @return ���O�t�@�C���I�[�v���̐���
	 */
	bool Open( const CString& logPath );

	/**
	 * @brief ���O�t�@�C���N���[�Y
	 */
	void Close();
	
	/**
	 * @brief ���ݎ���(�V�X�e������)�𕶎���ŕԂ�(YYYY/MM/DD hh:mm:ss)
	 */
	CStringA GetTime();

private:
	std::ofstream m_ofs; //!< ���O�t�@�C���X�g���[��
};

