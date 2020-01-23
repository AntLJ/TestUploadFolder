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
#include "IllustSettings.h"
#include "LogMgr.h"
#include "common.h"
#include <sindy/schema/illust.h>

bool IllustSettingsRec::setNamingRule( const std::string& strNamingRule )
{
	std::vector<std::string> values;
	boost::split( values, strNamingRule, boost::is_any_of(",") );
	for( auto str : values )
	{
		m_vecNamingRule.push_back( CString( str.c_str() ) );
	}

	return true;
}

IllustSettings::IllustSettings()
{
}

IllustSettings::~IllustSettings()
{
}

bool IllustSettings::load( const CString& strFileName, const ITablePtr& ipIllustLinkTable )
{
	// �t�@�C�����I�[�v��
	std::ifstream ifs( strFileName );
	if( !ifs.is_open() ) return false;

	while( !ifs.eof() )
	{
		// �s��ǂݍ���
		std::string strLine;
		std::getline( ifs, strLine );

		// ��s��R�����g�s�̓X�L�b�v
		if( strLine.empty() ) continue;
		if( strLine.front() == '#' ) continue;

		// �^�u�ŃJ�����i�g�[�N���j�ɕ���
		std::vector<std::string> vecValues;
		boost::split( vecValues, strLine, boost::is_any_of("\t") );

		if( vecValues.size() != 7 )
		{
			LogMgr::getInstance().writeRunLog( _T("�������ݒ�t�@�C���̃t�B�[���h�����K�؂ł���܂���") );
			return false;
		}

		IllustSettingsRec rec;
		rec.m_nIllustClass	= std::atol( vecValues[0].c_str() );	// �C���X�g��ʃR�[�h
		rec.m_nCustomer		= std::atol( vecValues[1].c_str() );	// �d����R�[�h
		rec.m_strPrefix		= CA2T( vecValues[2].c_str() );			// �C���X�g�t�@�C���̃v���t�B�b�N�X
		rec.m_strExtension	= CA2T( vecValues[3].c_str() );			// �C���X�g�t�@�C���̊g���q
		rec.setNamingRule( vecValues[4] );							// �C���X�g�t�@�C���̖����K��
		rec.m_strIllustDir	= CA2T( vecValues[5].c_str() );			// �C���X�g�t�@�C���̊i�[��f�B���N�g��
		if( !m_cIllustOutputInfo.readExceptionConds( CString( vecValues[6].c_str() ), rec.m_cOutputInfo.mExceptionConds ) )
		{
			LogMgr::getInstance().writeRunLog( _T("�o�͏��O�������K�؂ł���܂���") );
			return false;
		}

		// map�ɑ}��
		IllustSettings::_Pairib ret = insert( IllustSettings::value_type( IllustSettings::key_type( rec.m_nIllustClass, rec.m_nCustomer ), rec ) );

		// �}���Ɏ��s�i�L�[�̏d���j
		if( !ret.second )
		{
			CString strTmp;
			strTmp.Format( _T("�������ݒ�t�@�C���̃L�[���d�����Ă��܂� : %d, %d"), rec.m_nIllustClass, rec.m_nCustomer );
			LogMgr::getInstance().writeRunLog( strTmp );
			return false;
		}
	}

	ifs.close();

	if( !checkCodesRange( ipIllustLinkTable ) ) return false;
	if( !checkDirExistence() ) return false;

	return true;
}

bool IllustSettings::checkCodesRange( const ITablePtr& ipIllustLinkTable )
{
	// �R�[�h�l�h���C���̎擾
	std::set< long, std::less< long > > setValuesIllustClass, setValuesCustomerC;
	if( !common_proc::getCodedValues( ipIllustLinkTable, sindy::schema::illust_link::kIllustClass, setValuesIllustClass ) )
	{
		LogMgr::getInstance().writeRunLog( _T("�R�[�h�l�h���C�����擾���邱�Ƃ��ł��܂��� : ") + CString( sindy::schema::illust_link::kIllustClass ) );
		return false;
	}
	if( !common_proc::getCodedValues( ipIllustLinkTable, sindy::schema::illust_link::kCustomerCode, setValuesCustomerC ) )
	{
		LogMgr::getInstance().writeRunLog( _T("�R�[�h�l�h���C�����擾���邱�Ƃ��ł��܂��� : ") + CString( sindy::schema::illust_link::kCustomerCode ) );
		return false;
	}

	// �e���R�[�h���`�F�b�N
	bool bRet = true;
	for( const auto rec : *this )
	{
		// �C���X�g��ʃR�[�h���R�[�h�l�h���C���Ɋ܂܂�Ȃ�
		if( setValuesIllustClass.find( rec.second.m_nIllustClass ) == setValuesIllustClass.end() )
		{
			CString strTmp;
			strTmp.Format( _T("�R�[�h�l�h���C���Ɋ܂܂�Ȃ��C���X�g��ʃR�[�h�ł� : %d"), rec.second.m_nIllustClass );
			LogMgr::getInstance().writeRunLog( strTmp );
			bRet = false;
		}
		// �d����R�[�h���R�[�h�l�h���C���Ɋ܂܂�Ȃ�
		if( setValuesCustomerC.find( rec.second.m_nCustomer ) == setValuesCustomerC.end() )
		{
			CString strTmp;
			strTmp.Format( _T("�R�[�h�l�h���C���Ɋ܂܂�Ȃ��d����R�[�h�ł� : %d"), rec.second.m_nCustomer );
			LogMgr::getInstance().writeRunLog( strTmp );
			bRet = false;
		}
	}

	return bRet;
}

bool IllustSettings::checkDirExistence()
{
	bool bRet = true;
	for( const auto rec : *this )
	{
		if( !::PathIsDirectory( rec.second.m_strIllustDir ) )
		{
			LogMgr::getInstance().writeRunLog( _T("�w�肳�ꂽ�C���X�g�t�@�C���i�[�f�B���N�g�������݂��܂��� : ") + rec.second.m_strIllustDir );
			bRet = false;
		}
	}

	return bRet;
}
