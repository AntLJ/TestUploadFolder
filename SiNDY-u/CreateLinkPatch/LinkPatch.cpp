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
 *	@file LinkPatch.cpp
 *	@brief �����N�p�b�`�N���X ������
 *	@author	F.Adachi
 *	@date	2005/11/9		�V�K�쐬
 *	$ID$
 */

#include "StdAfx.h"
#include "LinkPatch.h"
//#include <sstream>
#include <fstream>
#include <string>

LinkPatchRecord::LinkPatchRecord(void) :
	m_MeshCode(0)
{
}

LinkPatchRecord::~LinkPatchRecord()
{
}

LinkPatch::LinkPatch(void)
{
}

LinkPatch::~LinkPatch()
{
}

/**
 *	@brief	�i�[
 **/
void LinkPatch::assign( LinkPatchRecord cRecord )
{
	m_Data.push_back( cRecord );
}


/**
 *	@brief	�S�폜
 **/
void LinkPatch::clear(void)
{
	m_Data.clear();
}

/**
 *	@brief	��������
 *	@note	�w�肳�ꂽ�X�g���[���Ƀ����N�p�b�`�̓��e���o�͂���
 **/
void LinkPatch::write( std::ostream& crStream )
{
	for( LinkPatchIter aLinkPatchIter = m_Data.begin(); aLinkPatchIter != m_Data.end(); ++aLinkPatchIter ) {
		crStream << "( ";
		crStream << aLinkPatchIter->m_Attribute << ", ";
		crStream << aLinkPatchIter->m_RevisedValue << ", ";
		crStream << aLinkPatchIter->m_MeshCode;
		
		for( LinkPatchRecord::NodeIDIter aNodeIDIter = aLinkPatchIter->m_NodeIDCollection.begin(); 
			 aNodeIDIter != aLinkPatchIter->m_NodeIDCollection.end();
			 ++aNodeIDIter )
		{
			crStream << ", " << *aNodeIDIter;
		}
		
		crStream << " )," << "\n";
	}
}


/**
 *	@brief	��������
 *	@note	�t�@�C�����ƕ����R�[�h�i���s�R�[�h�̂ݍl���j���w�肵�ď�������
 *	@note	�����N�p�b�`�̃��R�[�h�ɂ͓��{��(2Byte�R�[�h)���܂܂�Ȃ����Ƃ��O��
 **/
void LinkPatch::write( const char* cFilePath, bool cUseEUC )
{
	std::string aFunc = "LinkPatch::write()";

	// EUC�̏ꍇ�̓o�C�i�����[�h�ŊJ��
	std::ios::openmode aOpenMode;
	if (cUseEUC) { aOpenMode = std::ios::out | std::ios::binary; }
	else		 { aOpenMode = std::ios::out; }

	std::ofstream aFout( cFilePath, aOpenMode );
	if (!aFout) { throw std::runtime_error( "R1F:" + aFunc + " : �����N�p�b�`�t�@�C���I�[�v���G���[" ); }
	
	write( aFout );

	aFout.close();
}

