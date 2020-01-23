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
 * @file Workspaces.h
 * @brief CWorkspaces �N���X�̃C���^�[�t�F�C�X
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * @version $Id$
 */
#ifndef _WORKSPACES_H_
#define _WORKSPACES_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>
#include "AnnotationClassTable.h"
#include "Workspace.h"

namespace sindy {
//using namespace sindy::annotation_class;

/**
 * @class CWorkspaces
 * @brief <b>���[�N�X�y�[�X�ڑ��Ǘ��N���X</b>
 * ���[�N�X�y�[�X�ւ̐ڑ����Ǘ����܂��B
 * �����ŕ����̐ڑ���ێ����邱�Ƃ��\�ł��B
 *
 * ���̃N���X�͎�Ƀ`�F�b�J�[�Ȃǂ�ArcMap���g�p���Ȃ�
 * �A�v���P�[�V�����Ŏg�p����܂��B
 * @warning ���̃N���X��export���Ă��Ȃ������o�ϐ���錾���Ă��܂����Aprivate�Ȃ��ߊO����
 * �A�N�Z�X����邱�Ƃ͂���܂���B\n
 * �Ȃ��A��L�̂悤�ȏꍇ���̎|���[�j���O���o��̂ł����A�E�U�C�̂Ń��[�j���O�������Ă��܂��B
 */
#pragma warning(push)
#pragma warning(disable : 4251)

class SINDYLIB_API CWorkspaces
{
public:
	CWorkspaces(){};
	virtual ~CWorkspaces(){};

public:
	/**
	 * @brief <b>���������΍��p</b>\n
	 * �����������p�̃R�[�h�ł��BConnect����O�ɌĂяo���܂��B
	 */
	bool PreConnect();

	/**
	 * @brief �f�[�^�x�[�X�ɐڑ�����
	 * ��x�ڑ�����ƁA�ڑ�������ŕێ����܂��B
	 * �����̐ڑ���ێ����邱�Ƃ��ł��܂��B
	 * SDE�ƃp�[�\�i���E�W�I�f�[�^�x�[�X�ɂ̂ݑΉ����Ă��܂��B
	 * @see ConnectToSDE
	 * @see ConnectToPGDB
	 * @param lpcszConnectString	[in]	�ڑ�������i<em>username</em>/<em>password</em>/<em>version</em>/<em>instance</em>/<em>servername</em>����PGDB�t�@�C�����j
	 * @return IWorkspacePtr
	 */
	CWorkspace* Connect( LPCTSTR lpcszConnectString );

	/**
	 * @brief SDE�f�[�^�x�[�X�ɐڑ�����
	 * ���ɐڑ������݂���ꍇ�͊����̐ڑ���Ԃ��܂��B
	 * @param lpcszUserName		[in]	�ڑ����[�U��
	 * @param lpcszPassword		[in]	�ڑ����[�U�p�X���[�h
	 * @param lpcszVersion		[in]	�ڑ��o�[�W����
	 * @param lpcszInstance		[in]	�ڑ��C���X�^���X
	 * @param lpcszServerName	[in]	�ڑ��T�[�o��
	 * @return IWorkspacePtr
	 */
	CWorkspace* ConnectToSDE( LPCTSTR lpcszUserName, LPCTSTR lpcszPassword, LPCTSTR lpcszVersion, LPCTSTR lpcszInstance, LPCTSTR lpcszServerName );

	/**
	 * @brief �p�[�\�i���E�W�I�f�[�^�x�[�X�ɐڑ�����
	 * ���ɐڑ������݂���ꍇ�͊����̐ڑ���Ԃ��܂��B
	 * @param lpcszPGDBFileName
	 * @return IWorkspacePtr
	 */
	CWorkspace* ConnectToPGDB( LPCTSTR lpcszPGDBFileName );

	/**
	 * @brief �����̐ڑ���ǉ�����
	 * @retval ������NULL�̏ꍇ��sindyErr_COMInterfaceIsNull��Ԃ�
	 */
	errorcode::sindyErrCode AddConnection( IWorkspacePtr ipWorkspace );

	/**
	 * @brief <b>���L���[���e�[�u���N���X�̎Q�Ƃ�Ԃ�</b>\n
	 * �܂����[�����[�N�X�y�[�X�ɐڑ����Ă��Ȃ��ꍇ�̓��[�����[�N�X�y�[�X��
	 * �ڑ����Ă���e�[�u����Ԃ��܂��B\n
	 * ���[�����[�N�X�y�[�X�֐ڑ�����ۂɂ�SiNDY-e�̃C���X�g�[���t�H���_��
	 * �ɂ���settings.ini�t�@�C���̒���servername���Q�Ƃ��܂��B
	 * ���̃T�[�o���Ŋ��ɐڑ������݂���ꍇ�͂��̐ڑ��𗘗p���܂��B
	 * �����A�L�q���Ȃ����A���邢��SiNDY-e���C���X�g�[������Ă��Ȃ����
	 * �����̐ڑ�����T���܂��B����ł��Ȃ��ꍇ�ɂ́A�ڑ��𑣂��_�C�A���O
	 * ��\�����܂��B\n
	 * ���I�Ȑڑ����܂ނ��߁Aconst�Ȋ֐��ł͂���܂���B
	 * @return ���L���[���e�[�u��
	 */
//	annotation_class::CAnnotationClassTable& GetAnnotationClassTable();

	/**
	 * @brief <b>���[���p���[�N�X�y�[�X�֐ڑ�����</b>\n
	 * �܂����[�����[�N�X�y�[�X�ɐڑ����Ă��Ȃ��ꍇ�̓��[�����[�N�X�y�[�X��
	 * �ڑ����܂��B���ɐڑ�������ꍇ�́A�����Ԃ��܂��B\n
	 * ���[�����[�N�X�y�[�X�֐ڑ�����ۂɂ�SiNDY-e�̃C���X�g�[���t�H���_��
	 * �ɂ���settings.ini�t�@�C���̒���servername���Q�Ƃ��܂��B
	 * ���̃T�[�o���Ŋ��ɐڑ������݂���ꍇ�͂��̐ڑ��𗘗p���܂��B
	 * �����A�L�q���Ȃ����A���邢��SiNDY-e���C���X�g�[������Ă��Ȃ����
	 * �����̐ڑ�����T���܂��B����ł��Ȃ��ꍇ�ɂ́A�ڑ��𑣂��_�C�A���O
	 * ��\�����܂��B\n
	 * @retval const CWorkspace* ���[�����[�N�X�y�[�X
	 * @retval NULL �ڑ����邱�Ƃ��ł��Ȃ�����
	 */
	const CWorkspace* CWorkspaces::ConnectToRuleWorkspace();
protected:
	/**
	 * @brief �����̐ڑ���T��
	 * @param lpcszUserName		[in]	�ڑ����[�U��
	 * @param lpcszVersion		[in]	�o�[�W����
	 * @param lpcszServerName	[in]	�T�[�o��
	 * @return �ڑ������݂��Ȃ����NULL��Ԃ�
	 */
	const CWorkspace* FindWorkspace( LPCTSTR lpcszUserName,  LPCTSTR lpcszVersion, LPCTSTR lpcszServerName ) const;

	/**
	 * @brief �����̐ڑ���T��
	 * @param ipWorkspace		[in]	IWorkspacePtr
	 * @return �ڑ������݂��Ȃ����NULL��Ԃ�
	 */
	const CWorkspace* FindWorkspace( IWorkspacePtr ipWorkspace ) const;

	/**
	 * @brief �����̐ڑ����T�[�o���ŒT��
	 * @param lpcszServerName	[in]	�T�[�o��
	 * @return �ڑ������݂��Ȃ����NULL��Ԃ�
	 */
	const CWorkspace* FindWorkspaceByServerName( LPCTSTR lpcszServerName ) const;

	/**
	 * @brief �p�[�\�i���E�W�I�f�[�^�x�[�X�����ǂ����𔻒f����
	 * �Ō��4�������u.mdb�v���ǂ����Ŕ��f���܂��B
	 * @param lpcszString	[in]	���蕶����
	 * @return �p�[�\�i���E�W�I�f�[�^�x�[�X�̃t�@�C�����Ȃ�true
	 */
	bool IsPGDBFileName( LPCTSTR lpcszString ) const;
private:
	IDispatchPtr								m_ipDisp;					//!< IApplication�Ƃ�
	std::list<CWorkspace>						m_listWorkspace;			//!< �ڑ��ς݃��[�N�X�y�[�X�̃��X�g
	annotation_class::CAnnotationClassTable		m_cAnnotationClassTable;	//!< ���L���[���e�[�u��
};
#pragma warning(pop)

} // sindy

#endif // _WORKSPACES_H_
