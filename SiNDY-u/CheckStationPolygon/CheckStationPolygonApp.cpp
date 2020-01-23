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

#include "stdafx.h"

#include "CheckStationPolygonApp.h"
#include "Logger.h"
#include "StationChecker.h"
#include "StationDetailCheckHandler.h"
#include "MidZoomPlatformCheckHandler.h"
#include "HighZoomStationCheckHandler.h"
#include "MidZoomStationCheckHandler.h"
#include "LowZoomPlatformCheckHandler.h"
#include "RelStationSiteCheckHandler.h"


using namespace sindy;


// ���s
bool CheckStationPolygonApp::execute()
{
	// ���O������
	if( ! initLog() ) {
		finish( false );
		return false;
	}

	// �`�F�b�J�[�o�^
	resistChecker();

	// �`�F�b�J�[������
	if( ! initChecker() ) {
		finish( false );
		return false;
	}

	// �`�F�b�N���s
	if( ! runCheck() ) {
		finish( false );
		return false;
	}

	// �I������
	finish( true );
	return true;
}


// �`�F�b�J�[�o�^
void CheckStationPolygonApp::resistChecker()
{
	m_CheckerContainer.push_back( std::make_shared<StationDetailCheckHandler>() );
	m_CheckerContainer.push_back( std::make_shared<MidZoomPlatformCheckHandler>() );
	m_CheckerContainer.push_back( std::make_shared<HighZoomStationCheckHandler>() );
	m_CheckerContainer.push_back( std::make_shared<MidZoomStationCheckHandler>() );
	m_CheckerContainer.push_back( std::make_shared<LowZoomPlatformCheckHandler>() );
	m_CheckerContainer.push_back( std::make_shared<RelStationSiteCheckHandler>() );
}


// �`�F�b�J�[������
bool CheckStationPolygonApp::initChecker()
{
	// ���[�N�X�y�[�X�I�[�v��
	CWorkspace workspace;
	if( ! workspace.Connect( m_args.getDB().c_str() ) ) {
		RUNLOGGER.Error( _T("DB�̐ڑ��Ɏ��s���܂���") );
		return false;
	}

	// �`�F�b�N���W�b�N��ێ�����I�u�W�F�N�g������
	SPStationChecker checkLogic( std::make_shared<StationChecker>() );
	if( ! checkLogic->init( workspace )) {
		RUNLOGGER.Error( _T("�e�[�u���̃I�[�v���Ɏ��s���܂���") );
		return false;
	}

	// �`�F�b�J�[�̏�����
	for( auto checker : m_CheckerContainer )
	{
		if( ! checker->init( workspace, checkLogic ) ) {
			RUNLOGGER.Error( _T("�`�F�b�N�̑O�����Ɏ��s���܂���") );
			return false;
		}
	}

	return true;
}


// ���O�t�@�C���̏�����
bool CheckStationPolygonApp::initLog()
{
	// �������O�t�@�C���I�[�v��
	if( ! RUNLOGGER.Initialize( m_args.getLogDir() ) )
	{
		std::cerr <<  "�������O�t�@�C���̃I�[�v���Ɏ��s���܂��� : " <<  uh::toStr( m_args.getLogDir() ) << std::endl;
		return false;
	}

	// �c�[�����s���o��
	RUNLOGGER.Log( _T("�ڑ���DB:\t%s\n"), m_args.getDB().c_str() );
	RUNLOGGER.Log( _T("���O�o�̓t�H���_�p�X:\t%s\n"), m_args.getLogDir().c_str() );

	// �G���[���O�t�@�C���I�[�v��
	if( ! ERRLOGGER.Initialize( m_args.getLogDir() ) )
	{
		RUNLOGGER.Error( _T("�G���[���O�t�@�C���̃I�[�v���Ɏ��s���܂��� : ") + m_args.getLogDir() );
		return false;
	}

	RUNLOGGER.Info( _T("���s�J�n "));

	return true;
}


// �`�F�b�N���s
bool CheckStationPolygonApp::runCheck()
{
	for( auto checker : m_CheckerContainer )
	{
		RUNLOGGER.Info( _T("�`�F�b�N�J�n -> ") + checker->getTarget() );

		if( ! checker->check() ) {
			RUNLOGGER.Error( _T("�`�F�b�N�Ɏ��s���܂���") + checker->getTarget() );
			return false;
		}
	}
	return true;
}


// �㏈��
void CheckStationPolygonApp::finish( bool bSuccess )
{
	// �I�����b�Z�[�W�o��
	uh::tstring result = bSuccess ? _T("����I��") : _T("�ُ�I��");
	RUNLOGGER.Info( _T("�������� -> ") + result );

	// �t�@�C���̃N���[�Y
	RUNLOGGER.Finalize();
	ERRLOGGER.Finalize();
}
