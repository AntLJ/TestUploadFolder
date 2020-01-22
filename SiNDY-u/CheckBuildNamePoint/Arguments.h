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


/**
 * @brief �R�}���h���C�������N���X
 */
class Arguments
{
public:
	/**
	 * @brief �R���X�g���N�^
	 */
	Arguments() : m_bufferRange(50.0), m_chkFixclass(true) {};

	/**
	 * @brief �R�}���h���C�������̉��
	 * @param	argc [in]	�R�}���h���C�������̐�
	 * @param	argv [in]	�R�}���h���C�������̓��e
	 * @retval	true	����
	 * @retval	false	���s
	 */
	bool parse(int argc, TCHAR* argv[]);

	/**
	 * @brief  �R�}���h���C�������̊m�F
	 * @param  optName  [in]  �I�v�V������
	 * @param  optValue [in]  �I�v�V�����l
	 * @param  bFile    [in]  �t�@�C���ł��邩�i�f�t�H���gfalse�j
	 * @retval true    ����OK
	 * @retval false   ����NG
	 */
	bool CheckParam( const char* const optName, const uh::tstring& optValue, bool bFile = false );
public:

	uh::tstring  m_strBlNameDB;    //!< �r������DB�ڑ���
	uh::tstring  m_strBuildingDB;  //!< �s�s����DB�ڑ���
	uh::tstring  m_strAnnoDB;      //!< ���LDB�ڑ���
	uh::tstring  m_strMeshDB;      //!< ���b�V��DB�ڑ���
	uh::tstring  m_strOutput;      //!< �A�E�g�v�b�g
	uh::tstring  m_strMeshList;    //!< ���b�V�����X�g
	uh::tstring  m_strNgBuild;     //!< NG������ʃ��X�g
	uh::tstring  m_strErrWord;     //!< NG�����񃊃X�g(error)
	uh::tstring  m_strWarnWord;    //!< NG�����񃊃X�g(warning)
	uh::tstring  m_strBldcOKWord;  //!< ������ʃR�[�h��OK���[�h���X�g
	uh::tstring  m_strBldcNGWord;  //!< ������ʃR�[�h��NG���[�h���X�g
	bool         m_chkFixclass;    //!< ������ʃR�[�h�ʃ��[�h�`�F�b�N����ʊm��R�[�h�����m�F�i0�j�̃f�[�^�ɑ΂��Ă̂ݍs�����ǂ���
	uh::tstring  m_strReplaceWord; //!< �������r�p�u�����X�g
	uh::tstring  m_strJoinAnno;    //!< ���L��r�p���������񃊃X�g
	uh::tstring  m_strExclude_mark;//!< �������`�F�b�N�����O�L�����X�g
	double       m_bufferRange;    //!< �T���o�b�t�@(m)
};
