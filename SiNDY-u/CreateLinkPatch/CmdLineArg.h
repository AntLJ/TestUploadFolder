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

/**
 *	@file CmdLineArg.h
 *	@brief �R�}���h���C�������N���X ��`��
 *	@note	�R�}���h���C�������Ɉˑ���������Ǘ�����N���X
 *	@author	F.Adachi
 *	@date	2005/10/18		�V�K�쐬
 *	$ID$
 */

#ifndef ___CMDLINEARG___
#define ___CMDLINEARG___

#include "DBInfo.h"
#include <boost/utility.hpp>
#include <string>

/// <b>�R�}���h���C�������N���X</b>
class CmdLineArg : boost::noncopyable
{
public:
	CmdLineArg(void);
	~CmdLineArg();

	void			analyze( int cArgc, char** cArgv );			///< ���

	void			setDBInfo( DBInfo* cpDBInfo ) { m_pDBInfo = cpDBInfo; }
	const DBInfo*	getDBInfo(void) const { return m_pDBInfo; }

	const char*		getApprAttrFilePath(void) const { return m_ApprAttrFilePath.c_str(); }
	const char*		getLinkPatchFilePath(void) const { return m_LinkPatchFilePath.c_str(); }
	const char*		getEtcLinkFilePath(void) const { return m_EtcLinkFilePath.c_str(); }
	bool			useEUC(void) const { return m_EUC; }

private:
	DBInfo*	m_pDBInfo;										///< DB���N���X�ւ̊֘A

	// �I�v�V�����t���O
	std::string	m_ApprAttrFilePath;							///< �H���R�[�h�w��t�@�C���̃p�X
	std::string m_LinkPatchFilePath;						///< �����N�p�b�`�t�@�C���̃p�X
	std::string m_EtcLinkFilePath;							///< �X�}�[�gIC�p���H�����N���X�g�̃p�X
	bool		m_EUC;										///< �����R�[�h��EUC�ɂ��邩�ۂ�

};

#endif
