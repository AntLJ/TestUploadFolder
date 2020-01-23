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

// DiffImportPGDB.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
/*
 �J�������Ɉȉ��̗v����݂��܂����B
�P�D�ɗ͏��Ȃ��c�[���ƊȒP�Ȑݒ�Ŏ��s�ł��邱��
�Q�D��{�I�ɂ̓o�[�W�����Ή��̂��̂ɑ΂��Ă̂ݎ��s�\
�@�@�i���ʂȃI�v�V��������t���邱�ƂŁA��Ή��̏ꍇ�����s�\�Ɂj
�R�D�����ō�ƃo�[�W�������쐬���A�����ɑ΂��čX�V���邱��
�S�D�����̎�@�Ɠ������ʂɂȂ邱��
*/

#include <boost/format.hpp>
#include <sindy/workspace.h>
#include <arctl/coinitializer.h>
#include "Reflect.h"
#include "TimeLog.h"
#include <TDC/useful_headers/flush_scope_guard.h>

namespace {
	CString gConnect = _T("");			///< �T�[�o�ڑ�������
	CString gSuffix = _T("");			///< VDB����̊g���q�i�����Z�b�g�̒P�ʁj
	CString gWorkVersion = _T("");		///< ��Ɨp�q�o�[�W����
}

/** 
 * @brief ���ϐ�����ڑ����̎擾�ƍ�Ɨp�q�o�[�W�����������߂�
 *
 */ 
bool GetConnectInfo(void)
{
	LPCTSTR aEnv = _tgetenv(_T("DB_ANY"));
	if(aEnv != NULL){
		gConnect = aEnv;
	}else{
#ifdef _DEBUG
		//gConnect = _T("TRIAL/TRIAL/TRIAL.ETC_DIFFIMPORT_TEST/5151/Coral2");
		gConnect = _T("D:\\test\\DiffImportPGDB\\�V�t���[2\\Dst_import_empty_f.gdb");
#else
		_ftprintf(stderr, _T("���ϐ��uDB_ANY�v���ݒ肳��Ă��܂���B\n"));
		return false;
#endif // _DEBUG
	}
	aEnv = _tgetenv(_T("WORK_VERSION"));
	if(aEnv != NULL){
		gWorkVersion = aEnv;
	}else{
		SYSTEMTIME stSystemTime;
		::GetLocalTime( &stSystemTime);
		//�����e�ŏ�����悤�ɂ��悤�Ƃ������A����ł͒��f�@�\�����ʂɂȂ�̂Łu_for_post�v�ǉ�
		gWorkVersion.Format(_T("DiffImportPGDB_%04d/%02d/%02d %02d:%02d:%02d_for_post"), 
			stSystemTime.wYear,	stSystemTime.wMonth, stSystemTime.wDay,
			stSystemTime.wHour,	stSystemTime.wMinute,stSystemTime.wSecond);
	}
	return true;
}

/** 
 * @brief ��Ɨp�q�o�[�W�����̍쐬���͎g���܂킵��A���ۂ̃o�[�W��������Ԃ�
 *
 * @retval ��Ɨp�̎q�o�[�W�������i���̂܂ܐڑ�������Ƃ��Ďg�p�\�j
 */ 
CString CreateWorkVersion(void)
{
	// �ڑ�
	IVersionPtr ipCurrentVersion = CReflect::connectSDE( gConnect );
	if(ipCurrentVersion){
		IVersionPtr ipChildVersion;
		((IVersionedWorkspacePtr)ipCurrentVersion)->FindVersion(CComBSTR(gWorkVersion), &ipChildVersion);
		if(ipChildVersion){
			_ftprintf(stderr, _T("#��Ɨp�q�o�[�W�����͊����̂��̂��g���܂�,%s\n"), static_cast<LPCTSTR>(gWorkVersion));
		}else{
			HRESULT hr = ipCurrentVersion->CreateVersion(CComBSTR(gWorkVersion),&ipChildVersion);
			if(ipChildVersion == NULL){
				_ftprintf(stderr, _T("#��Ɨp�q�o�[�W�����쐬���s,%s(hr=%d)\n"), static_cast<LPCTSTR>(gWorkVersion), hr);
				return _T("");
			}
		}
		return sindy::workspace_address((IWorkspacePtr)ipChildVersion).c_str();
	}
	return _T("");
}

/** 
 * @brief ���s���{��
 *
 */ 
void Execute( const CArguments& arg )
{
	CString aWorkVersion;
	if( !arg.m_exceptSdeMode )
	{
		// �q�o�[�W�������쐬(�쐬�������U�ؒf)
		aWorkVersion = CreateWorkVersion();
	}
	else
	{
		CString aExt = gConnect.Right(4);
		if( 0 != aExt.Compare(_T(".mdb")) && 0 != aExt.Compare(_T(".gdb")) )
		{
			_tprintf(_T("�ڑ����PGDB�܂���FGDB���w�肵�Ă�������\n"));
			_ftprintf(stderr, _T("#�ڑ����PGDB�܂���FGDB���w�肵�Ă�������\n"));
			return;
		}
		// SDE�ȊO�ɃC���|�[�g���[�h�Ȃ�ڑ������񂻂̂܂�
		aWorkVersion = gConnect;
	}

	if( !aWorkVersion.IsEmpty() )
	{
		_tprintf(_T("��ƃo�[�W������[%s]�ł�\n"), static_cast<LPCTSTR>(aWorkVersion));
		_ftprintf(stderr, _T("#��ƃo�[�W������[%s]�ł�\n"), static_cast<LPCTSTR>(aWorkVersion));
		bool aContinue = true;
		CReflect aReflect;
		while(aContinue)
		{
			// ���f�ł���悤�Ƀ��[�v
			if( !aReflect.init( aWorkVersion, arg ) )
				break;

			aContinue = aReflect.execReflect( arg );
			if( !aContinue )
				break;

			while( 1 )
			{
				// �啶���́uQ�v���́uS�v�ȊO�͎󂯕t���Ȃ��iCTRL+C�͌����Ă��܂��j
				_tprintf(_T("�uS�v�L�[�ōĊJ���܂��B�uQ�v�L�[�ŏI�����܂��B\n"));
				int aKey = _getch();
				if(aKey == 'Q')
				{
					aContinue = false;
					break;
				}
				if(aKey == 'S')
				{
					break;
				}
			}
		}
	}
}


/**
 * @brief ���C���֐��B
 *
 * @param argc	[in]	�R�}���h���C���̃g�[�N�����i�H�j
 * @param argv	[in]	�R�}���h���C���̃g�[�N����i�H�j
 *
 * @retval 0		����I���i���͂����������Ă͂��Ȃ��j
 * @retval 0�ȊO	�ُ�I��
 */
int _tmain(int argc, _TCHAR* argv[])
{
	// �X�R�[�v�𔲂��鎞�ɋ����I��flush����
	uh::flush_scope_guard fsg;

	::setlocale(LC_ALL, "Japanese");
	const arctl::coinitializer coinit;

	try
	{
		CArguments arg;
		if( !arg.parse( argc, argv ) )
		{
			return 1;
		}

		if( GetConnectInfo() )
		{
			Execute( arg );
			// log�ɏI���������o�͂��ĕ���
			// ���Ȃ݂�open��Execute���ł���Ă���
			arg.m_logFile.CloseLogFile();
		}
	}
	catch( const std::exception& e )
	{
		printf( "ERROR,%s\n", e.what() );
		fprintf( stderr, "#ERROR,%s\n", e.what() );
		return 1;
	}
	return 0;
}

