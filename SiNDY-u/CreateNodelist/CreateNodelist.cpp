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

#include "CreateNodelist.h"
#include "NodelistCreator.h"
#include "Defs.h"

// useful_headers
#include <TDC/useful_headers/str_util.h>


// ���s
bool CCreateNodelist::execute()
{
	// ������
	if( ! init() )
		return false;


	// �ݒ�t�@�C���ǂݍ���
	CIllustOutputInfo cOutputInfos;
	if( ! cOutputInfos.read( CT2A( m_args.m_output_ini.c_str() ) ))
	{
		outputErrMsg( _T("�m�[�h���X�g�o�͏��t�@�C���̓ǂݍ��݂Ɏ��s���܂���")); 
		mRunLog   << "��������:\t�ُ�I��" << std::endl;
		return false;
	}

	CIllustRuleInfo cRuleInfos;
	if( ! cRuleInfos.read( m_args.m_name_rule_ini.c_str() ) )
	{
		outputErrMsg( _T("�C���X�g�����[���ݒ�t�@�C���̓ǂݍ��݂Ɏ��s���܂���")); 
		mRunLog   << "��������:\t�ُ�I��" << std::endl;
		return false;
	}


	// �m�[�h���X�g�쐬�I�u�W�F�N�g������
	CNodelistCreator cCreator;
	CString strErrMsg;
	if( m_args.m_road_db.empty() ){
		cCreator.init( m_args.m_input_db.c_str(), m_args.m_output_dir.c_str(), m_args.m_latlon.c_str(), strErrMsg );	
	}else{
		cCreator.init( m_args.m_input_db.c_str(), m_args.m_road_db.c_str(), m_args.m_output_dir.c_str(), m_args.m_latlon.c_str(), strErrMsg );	
	}

	if( ! strErrMsg.IsEmpty() ) {
		outputErrMsg( strErrMsg );
		mRunLog   << "��������:\t�ُ�I��" << std::endl;
		return false;
	}


	// �m�[�h���X�g�o�͏��t�@�C���̃��R�[�h�����̃m�[�h���X�g�쐬
	// (�܂��͊����̃m�[�h���X�g�ɒǋL)
	for( auto iter = cOutputInfos.begin(); iter != cOutputInfos.end(); ++iter )
	{
		// �C���X�g���������[�����擾
		const auto& rule_iter = cRuleInfos.find( *iter );
		if( rule_iter == cRuleInfos.end() )
		{
			outputErrMsg( _T("�C���X�g�����[���̎擾�Ɏ��s���܂���") );
			mRunLog   << "��������:\t�ُ�I��" << std::endl;
			return false;
		}
		
		// �m�[�h���X�g�쐬
		CString strErrMsg;
		if( ! cCreator.create( *iter, rule_iter->mIllustNameRule, strErrMsg ))
		{
			outputErrMsg( strErrMsg );
			mRunLog   << "��������:\t�ُ�I��" << std::endl;
			return false;
		}
	}

	mRunLog << "��������:\t����I��" << std::endl;
	return true;
}


// ������
bool CCreateNodelist::init()
{
	using namespace uh;

	// ���O�̃I�[�v��
	mRunLog.open( m_args.m_runlog );
	if( ! mRunLog.good() ) {
		outputErrMsg( str_util::format( _T("�������O�̃t�@�C���p�X���s���ł� : %s"), m_args.m_runlog) );
		return false;
	}

	mErrLog.open( m_args.m_errlog );
	if( ! mErrLog.good() ) {
		outputErrMsg( str_util::format( _T("�G���[���O�̃t�@�C���p�X���s���ł� : %s"), m_args.m_errlog) );
		return false;
	}

	// �������O�ɐݒ�����o��
	if( ! m_args.m_road_db.empty() )
	{
		mRunLog << "DB�ڑ���(�C���X�g�����N):\t" << uh::toStr( m_args.m_input_db ) << std::endl;
		mRunLog << "DB�ڑ���(���H):\t" << uh::toStr( m_args.m_road_db ) << std::endl;
	}
	else
	{
		mRunLog << "DB�ڑ���:\t" << uh::toStr( m_args.m_input_db ) << std::endl;
	}
	mRunLog << "�C���X�g���������[�����t�@�C��:\t" << uh::toStr( m_args.m_name_rule_ini ) << std::endl;
	mRunLog << "�C���X�g�o�͏��ݒ�t�@�C��:\t" << uh::toStr( m_args.m_output_ini ) << std::endl;
	mRunLog << "�o�̓t�H���_:\t" << uh::toStr( m_args.m_output_dir ) << std::endl;
	mRunLog << "�������O:\t" << uh::toStr( m_args.m_runlog ) << std::endl;
	mRunLog << "�G���[���O:\t" << uh::toStr( m_args.m_errlog ) << std::endl;
	
	if( m_args.m_latlon == latlon::DEG )
		mRunLog << "�ܓx�o�x�o�͐ݒ�:\t�����ܓx�o�x" << std::endl;
	else if( m_args.m_latlon == latlon::DMS )
		mRunLog << "�ܓx�o�x�o�͐ݒ�:\t�x���b�ܓx�o�x" << std::endl;
	else
		mRunLog << "�ܓx�o�x�o�͐ݒ�:\t�o�͂Ȃ�" << std::endl;

	return true;
}


// �G���[���b�Z�[�W�o��
void CCreateNodelist::outputErrMsg( const CString& strErrMsg )
{
	std::cerr << "ERROR\t" << CT2A( strErrMsg ) << std::endl;
	if( mErrLog.is_open() && mErrLog.good() )
		mErrLog   << "ERROR\t" << CT2A( strErrMsg ) << std::endl;
}
