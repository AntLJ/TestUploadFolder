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
#include <TDC/useful_headers/tstring.h>
#include "LogFile.h"

using namespace uh;

class CArguments
{
public:
	/**
	 * @brief �R���X�g���N�^
	 */
	CArguments(void);

	/**
	 * @brief �f�X�g���N�^
	 */
	~CArguments(void);

	/**
	 * @brief �R�}���h���C�������̉��
	 * @param	argc [in]	�R�}���h���C�������̐�
	 * @param	argv [in]	�R�}���h���C�������̓��e
	 * @retval	true	����
	 * @retval	false	���s
	 */
	bool parse( int argc, _TCHAR* argv[] );

public:
	tstring            m_oldPath;       //!< �ҏW�OPGDB�p�X
	tstring            m_newPath;       //!< �ҏW��PGDB�p�X
	tstring            m_meshPath;      //!< ���b�V�����X�g�p�X
	tstring            m_layerListPath; //!< ���C�����X�g�p�X(G���[�h�̂�)
	std::list<CString> m_layerList;     //!< ���C�����X�g
	tstring            m_availDate;     //!< �L�����t(poi�C���|�[�g���̂�)
	tstring            m_suffix;        //!< �g���q�I�v�V�����i�[
	tstring            m_logPath;       //!< ���O�o�͐�
	
	bool               m_testMode;      //!< �e�X�g���[�h�i�X�V���Ȃ��j
	bool               m_forceMode;     //!< �����i�o�[�W������Ή��ł����s�j���[�h
	bool               m_tdcMode;       //!< TDC�D�惂�[�h
	bool               m_exceptSdeMode; //!< SDE�ȊO(PGDB�AFGDB)�ɃC���|�[�g���郂�[�h
	bool               m_geospaceMode;  //!< GEOSPACE�ƌ`�C���|�[�g���[�h
	bool               m_onlyImport;    //!< �}�[�W�Ȃ����[�h
	bool               m_emptyDBMode;   //!< ��DB�C���|�[�g���[�h

	// ���s���I�v�V�����̊i�[�����C���Ȃ̂ŁA��{const�ň������ƂɂȂ邪
	// CLogFile�͌�Ń��b�V���R�[�h�i�[�����肷��̂�mutable�ɂ��Ă���
	mutable CLogFile   m_logFile;       //!< ���O�t�@�C���Ǘ��N���X
};

