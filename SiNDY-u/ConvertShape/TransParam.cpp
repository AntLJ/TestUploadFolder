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

#include "TransParam.h"


CTransParam::CTransParam(void)
{
	m_bNoShape = true;
}


CTransParam::~CTransParam(void)
{
}

// �v������Ă���I�v�V�����̊m�F
bool CTransParam::check_required_option(const po::variables_map& vm, LPCSTR lpszOption, LPCSTR lpszDesc)
{
	if(vm.count(lpszOption) == 0) {
		std::cout << lpszDesc << "�����ݒ�ł��B" << std::endl;
		std::cerr << lpszDesc << "�����ݒ�ł��B" << std::endl;
		return false;
	}
	return true;
}

bool CTransParam::parse(int argc, _TCHAR* argv[])
{
	po::options_description desc("How to use");

	desc.add_options()
		( g_strTaregetDB, tvalue<tstring>(&m_tstrTargetDB), "[�K�{]�ϊ��Ώ�DB" )
		( g_strLayer, tvalue<tstring>(&m_tstrLayer), "[�K�{]���C����" )
		( g_strMeshDB, tvalue<tstring>(&m_tstrMesh), "[�I��]���b�V��DB(�񎟃��b�V��)" )
		( g_strMeshSchema, tvalue<tstring>(&m_tstrMeshSchema), "[�I��]���b�V�����C���̂���X�L�[�}" )
		( g_strMeshlist, tvalue<tstring>(&m_tstrMeshlist), "[�I��]���b�V�����X�g" )
		( g_strOutDir, tvalue<tstring>(&m_tstrOutDir), "[�I��]�V�F�[�v�t�@�C���o�̓f�B���N�g��" )
		( g_strIdentifier, tvalue<tstring>(&m_tstrIdentifier), "[�I��]�o�̓V�F�[�v�t�@�C���̎��ʎq" )
		( g_strOutFileName, tvalue<tstring>(&m_tstrFileName), "[�I��]�o�̓t�@�C����(�g���q�Ȃ�)" )
		( g_strColumnlist, tvalue<tstring>(&m_tstrColumnlist), "[�K�{]�ϊ��t�B�[���h�̑Ή��\" )
		( g_strNoShape, "[�I��]�`����o�͂����Ȃ�" )
		( g_strWhereQuery, tvalue<tstring>(&m_tstrWhereQuery), "[�I��]QueryFilter�pWhere��" )
		( g_strWhereLoop, tvalue<tstring>(&m_tstrWhereLoop), "[�I��]�t�B�[�`���擾���[�v���pWhere��" )
		( g_Runlog, tvalue<tstring>(&m_tstrRunLog), "[�I��]Run���O�t�@�C����" )
		( g_Errlog, tvalue<tstring>(&m_tstrErrLog), "[�I��]Err���O�t�@�C����" );

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	// �K�{�p�����[�^�̃`�F�b�N
	bool bAllow = true;
	bAllow &= check_required_option(vm, g_strTaregetDB, 	"[�K�{]�ϊ��Ώ�DB");
	bAllow &= check_required_option(vm, g_strLayer, 	"[�K�{]���C����");
	bAllow &= check_required_option(vm, g_strColumnlist, 	"[�K�{]�ϊ��t�B�[���h�̑Ή��\");
	// ���b�V���̎w�肪����ꍇ�́A�o�̓f�B���N�g���̎w�肪�K�{
	if( !m_tstrMesh.empty() )
	{
		bAllow &= check_required_option(vm, g_strMeshDB, 	"[�K�{]���b�V��DB(�񎟃��b�V��)");
		bAllow &= check_required_option(vm, g_strMeshlist, 	"[�K�{]���b�V�����X�g");
		if( m_tstrOutDir.empty() && m_tstrFileName.empty() )
		{
			// ���b�V���w�肠��A�V�F�[�v�t�@�C���o�̓f�B���N�g�� or �o�̓t�@�C�����̎w�肪�Ȃ��ꍇ
			bAllow &= check_required_option(vm, "", 	"[�K�{]�V�F�[�v�t�@�C���o�̓f�B���N�g���܂��́A�o�̓t�@�C�����̎w�肪�K�v�ł�");
		}
	}
	else if( !m_tstrFileName.empty() )
	{
		bAllow &= check_required_option(vm, g_strOutFileName, 	"[�K�{]�o�̓t�@�C����(�g���q�Ȃ�)");
	}
	else
		bAllow &= check_required_option(vm, "", 	"[�K�{]���b�V��DB�܂��́A�o�̓t�@�C�����̎w�肪�K�v�ł�");

	m_bNoShape = (vm.count(g_strNoShape) > 0);

	if( !bAllow )
	{
		std::cerr << desc << std::endl;
		return false;
	}

	return true;
}

std::string CTransParam::GetOption()
{
	std::string strOption = "[option]\n";
	if( !m_tstrTargetDB.empty() )
		strOption.append( GetOptionLine( g_strTaregetDB, uh::toStr( m_tstrTargetDB ) ) );

	if( !m_tstrLayer.empty() )
		strOption.append( GetOptionLine( g_strLayer, uh::toStr( m_tstrLayer ) ) );

	if( !m_tstrMesh.empty() )
		strOption.append( GetOptionLine( g_strMeshDB, uh::toStr( m_tstrMesh ) ) );

	if( !m_tstrMeshSchema.empty() )
		strOption.append( GetOptionLine( g_strMeshSchema, uh::toStr( m_tstrMeshSchema ) ) );

	if( !m_tstrMeshlist.empty() )
		strOption.append( GetOptionLine( g_strMeshlist, uh::toStr( m_tstrMeshlist ) ) );

	if( !m_tstrColumnlist.empty() )
		strOption.append( GetOptionLine( g_strColumnlist, uh::toStr( m_tstrColumnlist ) ) );

	if( !m_tstrOutDir.empty() )
		strOption.append( GetOptionLine( g_strOutDir, uh::toStr( m_tstrOutDir ) ) );

	if( !m_tstrFileName.empty() )
		strOption.append( GetOptionLine( g_strOutFileName, uh::toStr( m_tstrFileName ) ) );

	if( !m_tstrWhereQuery.empty() )
		strOption.append( GetOptionLine( g_strWhereQuery, uh::toStr( m_tstrWhereQuery ) ) );

	if( !m_tstrWhereLoop.empty() )
		strOption.append( GetOptionLine( g_strWhereLoop, uh::toStr( m_tstrWhereLoop ) ) );

	if( !m_tstrRunLog.empty() )
		strOption.append( GetOptionLine( g_Runlog, uh::toStr( m_tstrRunLog ) ) );

	if( !m_tstrErrLog.empty() )
		strOption.append( GetOptionLine( g_Errlog, uh::toStr( m_tstrErrLog ) ) );

	return strOption;
}

std::string CTransParam::GetOptionLine(const char* const cParam, std::string strValue)
{
	std::string strOptionLine = "   --";
	strOptionLine.append( cParam );
	strOptionLine.append( " " );
	strOptionLine.append( strValue );
	strOptionLine.append( "\n" );

	return strOptionLine;
}
