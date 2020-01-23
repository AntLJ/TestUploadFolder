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
 *	@file AppropriateAttribute.cpp
 *	@brief �H���R�[�h�w��t�@�C���N���X ������
 *	@author	F.Adachi
 *	@date	2005/08/10		�V�K�쐬
 *	$ID$
 */

#include "StdAfx.h"
#include "AppropriateAttribute.h"
#include "ZCL/ZSepByTab.h"
#include <sstream>

/// �R���X�g���N�^
AppropriateAttribute::AppropriateAttribute(void)
{
}

/// �f�X�g���N�^
AppropriateAttribute::~AppropriateAttribute()
{
}

/**
 *	@brief	�H���R�[�h�w��t�@�C���̓ǂݍ���
 *	@param	cpFilePath	[in]	�H���R�[�h�w��t�@�C���̃p�X
 **/
void AppropriateAttribute::read( const char* cpFilePath )
{
	std::string aFunc = "AppropriateAttribute::read()";

	ifstream aFin( cpFilePath );
	if (!aFin) {
		throw std::runtime_error( "R1F:" + aFunc + " : �H���R�[�h�w��t�@�C���I�[�v���G���[ : " + cpFilePath );
	}

	std::string aLineBuff;
	int aLineCount = 0;
	while ( std::getline( aFin, aLineBuff ) ) {
		++aLineCount;
		ZSepByTab	aSepByTab;
		aSepByTab.analyze( aLineBuff.c_str() );
		
		if (aSepByTab.getCount() != 2) {
			std::ostringstream aMessage;
			aMessage << aFunc << " : �s���ȃ��R�[�h " << cpFilePath << " LINE " << aLineCount;
			throw std::runtime_error( "R1F:" + aMessage.str() );
			continue;
		}

		m_Table.insert( std::pair<int, int>( atoi(aSepByTab[0]), atoi(aSepByTab[1]) ) );

	}

#ifdef	_DEBUG
	std::cout << cpFilePath << " : " << aLineCount << "�s�ǂݍ��݊���" << std::endl;
#endif

	aFin.close();
}


/**
 *	@brief	�e�[�u���S�̂��X�g���[���o��
 *	@param	crStream	[in]	�o�̓X�g���[��
 **/
void AppropriateAttribute::disp( std::ostream& crStream ) const
{
	for (std::map<long, long>::const_iterator aIter = m_Table.begin();	aIter != m_Table.end();	++aIter ) {
		crStream << aIter->first << "\t" << aIter->second << std::endl;
	}
}


/**
 *	@brief	���H�W����ʂ̎擾
 *	@note	�w�肳�ꂽ�H���R�[�h�ɑΉ����铹�H�W����ʂ��o�b�t�@�Ɋi�[���ĕԂ�
 *	@param	cRoadCode	[in]	�H���R�[�h
 **/
long AppropriateAttribute::getRoadClass( long cRoadCode ) const
{
	std::string aFunc = "AppropriateAttribute::getRoadClass()";

	std::map<long, long>::const_iterator aIter = m_Table.find( cRoadCode );
	if ( aIter == m_Table.end() ) {
		std::ostringstream aMessage;
		aMessage << aFunc << " : �H���R�[�h�w��t�@�C���ɑ��݂��Ȃ��H���R�[�h [" << cRoadCode << "]";
		throw std::runtime_error( "R1F:" + aMessage.str() );
	}

	return aIter->second;
}

